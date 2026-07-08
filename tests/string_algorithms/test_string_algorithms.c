#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#include <io.h>
#define dup _dup
#define dup2 _dup2
int _fileno(FILE*);
#ifndef fileno
#define fileno _fileno
#endif
#endif

/* Functions under test (forward-declared, same approach as the other tests).
   The string matchers print their results, so each test captures stdout and
   counts the "found at index" lines to compare against the expected count. */
void naive_string_matching(char* text, char* pattern);
void kmp_search(char* text, char* pattern);
void rabin_karp_search(char* text, char* pattern, int q);
void find_longest_repeated_substring(const char* txt, int n, char* output);
int* build_suffix_array(const char* txt, int n);
int* build_lcp_array(const char* txt, int* suffix_arr, int n);

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

void test_suffix_array()
{
    char txt[] = "banana";
    int n = strlen(txt);
    char lrs[50];
    find_longest_repeated_substring(txt, n, lrs);
    assert(strcmp(lrs, "an") == 0 || strcmp(lrs, "ana") == 0);

    assert(build_suffix_array(NULL, 5) == NULL);
    assert(build_suffix_array("abc", 0) == NULL);
    assert(build_suffix_array("abc", -1) == NULL);

    int sa[] = {0, 1, 2};
    assert(build_lcp_array(NULL, sa, 3) == NULL);
    assert(build_lcp_array("abc", NULL, 3) == NULL);
    assert(build_lcp_array("abc", sa, 0) == NULL);

    find_longest_repeated_substring(NULL, 5, lrs);
    find_longest_repeated_substring("abc", 3, NULL);
    find_longest_repeated_substring("abc", 0, lrs);

    printf("Suffix Array validation tests passed\n");
}

int main()
{
    test_basic_matches();
    test_non_ascii_bytes();
    test_suffix_array();
    printf("All string algorithms tests passed\n");
    return 0;
}
