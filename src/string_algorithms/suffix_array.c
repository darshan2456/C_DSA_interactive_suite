#include "../utils/config.h"
#include "clear_screen.h"
#include "history_logger.h"
#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Suffix {
    int index;
    int rank[2];
} Suffix;

static int suffix_cmp(const void* a, const void* b)
{
    Suffix* sa = (Suffix*)a;
    Suffix* sb = (Suffix*)b;
    if (sa->rank[0] == sb->rank[0])
        return (sa->rank[1] < sb->rank[1]) ? -1 : (sa->rank[1] > sb->rank[1] ? 1 : 0);
    return (sa->rank[0] < sb->rank[0]) ? -1 : 1;
}

int* build_suffix_array(char* txt, int n)
{
    Suffix* suffixes = (Suffix*)malloc(n * sizeof(Suffix));
    int* ind = (int*)malloc(n * sizeof(int));
    if (!suffixes || !ind)
    {
        free(suffixes);
        free(ind);
        return NULL;
    }

    for (int i = 0; i < n; i++)
    {
        suffixes[i].index = i;
        suffixes[i].rank[0] = (unsigned char)txt[i];
        suffixes[i].rank[1] = ((i + 1) < n) ? (unsigned char)txt[i + 1] : -1;
    }

    qsort(suffixes, n, sizeof(Suffix), suffix_cmp);

    for (int k = 4; k < 2 * n; k = k * 2)
    {
        int rank = 0;
        int prev_rank = suffixes[0].rank[0];
        suffixes[0].rank[0] = rank;
        ind[suffixes[0].index] = 0;

        for (int i = 1; i < n; i++)
        {
            if (suffixes[i].rank[0] == prev_rank && suffixes[i].rank[1] == suffixes[i - 1].rank[1])
            {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = rank;
            }
            else
            {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = ++rank;
            }
            ind[suffixes[i].index] = i;
        }

        for (int i = 0; i < n; i++)
        {
            int nextindex = suffixes[i].index + k / 2;
            suffixes[i].rank[1] = (nextindex < n) ? suffixes[ind[nextindex]].rank[0] : -1;
        }

        qsort(suffixes, n, sizeof(Suffix), suffix_cmp);
    }

    int* suffix_arr = (int*)malloc(n * sizeof(int));
    if (suffix_arr)
    {
        for (int i = 0; i < n; i++)
            suffix_arr[i] = suffixes[i].index;
    }

    free(suffixes);
    free(ind);
    return suffix_arr;
}

void suffix_array_search(char* text, char* pattern)
{
    int n = strlen(text);
    int m = strlen(pattern);
    if (n == 0 || m == 0)
    {
        printf("Pattern not found in the text.\n");
        return;
    }

    int* sa = build_suffix_array(text, n);
    if (!sa)
        return;

    int l = 0, r = n - 1;
    int found = 0;

    while (l <= r)
    {
        int mid = l + (r - l) / 2;
        int res = strncmp(pattern, text + sa[mid], m);

        if (res == 0)
        {
            int left = mid;
            while (left >= 0 && strncmp(pattern, text + sa[left], m) == 0)
            {
                printf("Pattern found at index %d\n", sa[left]);
                found++;
                left--;
            }
            int right = mid + 1;
            while (right < n && strncmp(pattern, text + sa[right], m) == 0)
            {
                printf("Pattern found at index %d\n", sa[right]);
                found++;
                right++;
            }
            break;
        }
        else if (res < 0)
        {
            r = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }

    free(sa);

    if (found == 0)
    {
        printf("Pattern not found in the text.\n");
    }
    else
    {
        printf("Total occurrences found: %d\n", found);
    }
}

void suffix_array_visualization(char* text, char* pattern)
{
    int n = strlen(text);
    int m = strlen(pattern);
    if (n == 0 || m == 0)
    {
        printf("Pattern not found in the text.\n");
        return;
    }

    int* sa = build_suffix_array(text, n);
    if (!sa)
        return;

    if (!is_instant())
    {
        clear_screen();
    }

    printf("\nSorted Suffix Array:\n");
    printf("%-5s %-8s %-s\n", "Pos", "SA_Val", "Suffix");
    printf("---------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("%-5d %-8d %-s\n", i, sa[i], text + sa[i]);
    }
    dynamic_sleep();

    int l = 0, r = n - 1;
    int found = 0;
    int step = 1;

    while (l <= r)
    {
        int mid = l + (r - l) / 2;
        int res = strncmp(pattern, text + sa[mid], m);

        if (!is_instant())
        {
            clear_screen();
        }

        printf("\nBinary Search Step %d\n", step++);
        printf("Search Bounds : [L=%d, R=%d], Mid=%d\n", l, r, mid);
        printf("Comparing pattern '%s' with suffix '%s'\n", pattern, text + sa[mid]);

        if (res == 0)
        {
            printf("Result        : Match found at mid=%d!\n", mid);
            int left = mid;
            while (left >= 0 && strncmp(pattern, text + sa[left], m) == 0)
            {
                printf("-> Match at text index %d\n", sa[left]);
                found++;
                left--;
            }
            int right = mid + 1;
            while (right < n && strncmp(pattern, text + sa[right], m) == 0)
            {
                printf("-> Match at text index %d\n", sa[right]);
                found++;
                right++;
            }
            break;
        }
        else if (res < 0)
        {
            printf("Result        : Pattern '%s' < suffix '%s'. Shifting search range to left\n", pattern, text + sa[mid]);
            r = mid - 1;
        }
        else
        {
            printf("Result        : Pattern '%s' > suffix '%s'. Shifting search range to right\n", pattern, text + sa[mid]);
            l = mid + 1;
        }
        dynamic_sleep();
    }

    free(sa);

    printf("\n-----------------------------------\n");
    if (found == 0)
    {
        printf("Pattern not found in the text.\n");
    }
    else
    {
        printf("Total occurrences found: %d\n", found);
    }
}

void suffix_array_demo(void)
{
    while (1)
    {
        char text[100];
        char pattern[100];

        printf("\nSuffix Array Search Demo\n");

        int status_T =
            safe_input_string(text, "Enter text (no spaces, max 99 chars), or 'X' to exit: ");
        if (status_T == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting demo...\n");
            return;
        }

        int status_P =
            safe_input_string(pattern, "Enter pattern (no spaces, max 99 chars), or 'X' to exit: ");
        if (status_P == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting demo...\n");
            return;
        }

        clock_t start_t = clock();
        suffix_array_visualization(text, pattern);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("Total CPU time taken: %f seconds\n", total_t);

        add_to_history("Suffix Array Search", strlen(text), total_t);
    }
}
