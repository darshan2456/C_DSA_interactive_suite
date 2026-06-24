#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Functions under test (forward-declared, same approach as the other tests).
   The string matchers print their results, so each test captures stdout and
   counts the "found at index" lines to compare against the expected count. */
void naive_string_matching(char* text, char* pattern);
void kmp_search(char* text, char* pattern);
void rabin_karp_search(char* text, char* pattern, int q);

/* Run a matcher with stdout redirected to a temp file, then return how many
   "found at index" lines it printed. */
static int count_matches(void (*fn)(char*, char*), char* text, char* pattern)
{
    FILE* tmp = tmpfile();
    assert(tmp != NULL);

    fflush(stdout);
    int saved = dup(fileno(stdout));
    dup2(fileno(tmp), fileno(stdout));

    fn(text, pattern);

    fflush(stdout);
    dup2(saved, fileno(stdout));
    close(saved);

    rewind(tmp);
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), tmp) != NULL)
    {
        if (strstr(line, "found at index ") != NULL)
            count++;
    }
    fclose(tmp);
    return count;
}

static int count_matches_rk(char* text, char* pattern, int q)
{
    FILE* tmp = tmpfile();
    assert(tmp != NULL);

    fflush(stdout);
    int saved = dup(fileno(stdout));
    dup2(fileno(tmp), fileno(stdout));

    rabin_karp_search(text, pattern, q);

    fflush(stdout);
    dup2(saved, fileno(stdout));
    close(saved);

    rewind(tmp);
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), tmp) != NULL)
    {
        if (strstr(line, "found at index ") != NULL)
            count++;
    }
    fclose(tmp);
    return count;
}

void test_basic_matches()
{
    char text[] = "abxabcabcaby";
    char pat[] = "abcaby";

    assert(count_matches(naive_string_matching, text, pat) == 1);
    assert(count_matches(kmp_search, text, pat) == 1);
    assert(count_matches_rk(text, pat, 101) == 1);

    /* overlapping matches */
    char t2[] = "aaaaa";
    char p2[] = "aa";
    assert(count_matches(naive_string_matching, t2, p2) == 4);
    assert(count_matches(kmp_search, t2, p2) == 4);
    assert(count_matches_rk(t2, p2, 101) == 4);

    /* pattern absent */
    char t3[] = "abcdef";
    char p3[] = "xyz";
    assert(count_matches(naive_string_matching, t3, p3) == 0);
    assert(count_matches(kmp_search, t3, p3) == 0);
    assert(count_matches_rk(t3, p3, 101) == 0);

    printf("String matching basic tests passed\n");
}

void test_non_ascii_bytes()
{
    /* Regression: bytes > 127 are negative with signed char, which used to
       corrupt the Rabin-Karp rolling hash and miss real matches. All three
       matchers must agree. */
    char text[] = {'a', 'b', (char)0xC3, 'd', 'a', 'b', (char)0xC3, 'd', 0};
    char pat[] = {(char)0xC3, 'd', 0};

    assert(count_matches(naive_string_matching, text, pat) == 2);
    assert(count_matches(kmp_search, text, pat) == 2);
    assert(count_matches_rk(text, pat, 101) == 2);

    printf("String matching non-ASCII tests passed\n");
}

int main()
{
    test_basic_matches();
    test_non_ascii_bytes();
    printf("All string algorithms tests passed\n");
    return 0;
}
