#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Suffix
{
    int index;
    int rank[2];
};

int cmp(const void* a, const void* b)
{
    struct Suffix* s1 = (struct Suffix*)a;
    struct Suffix* s2 = (struct Suffix*)b;
    if (s1->rank[0] == s2->rank[0])
    {
        return (s1->rank[1] < s2->rank[1]) ? -1 : (s1->rank[1] > s2->rank[1] ? 1 : 0);
    }
    return (s1->rank[0] < s2->rank[0]) ? -1 : 1;
}

int* build_suffix_array(const char* txt, int n)
{
    if (txt == NULL || n <= 0)
    {
        return NULL;
    }
    struct Suffix* suffixes = (struct Suffix*)malloc(n * sizeof(struct Suffix));
    if (suffixes == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < n; i++)
    {
        suffixes[i].index = i;
        suffixes[i].rank[0] = txt[i];
        suffixes[i].rank[1] = ((i + 1) < n) ? txt[i + 1] : -1;
    }

    qsort(suffixes, n, sizeof(struct Suffix), cmp);

    int* ind = (int*)malloc(n * sizeof(int));
    if (ind == NULL)
    {
        free(suffixes);
        return NULL;
    }
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
        qsort(suffixes, n, sizeof(struct Suffix), cmp);
    }

    int* suffix_arr = (int*)malloc(n * sizeof(int));
    if (suffix_arr == NULL)
    {
        free(suffixes);
        free(ind);
        return NULL;
    }
    for (int i = 0; i < n; i++)
    {
        suffix_arr[i] = suffixes[i].index;
    }

    free(suffixes);
    free(ind);
    return suffix_arr;
}

int* build_lcp_array(const char* txt, int* suffix_arr, int n)
{
    if (txt == NULL || suffix_arr == NULL || n <= 0)
    {
        return NULL;
    }
    int* lcp = (int*)calloc(n, sizeof(int));
    if (lcp == NULL)
    {
        return NULL;
    }
    int* inv_suff = (int*)malloc(n * sizeof(int));
    if (inv_suff == NULL)
    {
        free(lcp);
        return NULL;
    }

    for (int i = 0; i < n; i++)
    {
        inv_suff[suffix_arr[i]] = i;
    }

    int k = 0;
    for (int i = 0; i < n; i++)
    {
        if (inv_suff[i] == n - 1)
        {
            k = 0;
            continue;
        }
        int j = suffix_arr[inv_suff[i] + 1];
        while (i + k < n && j + k < n && txt[i + k] == txt[j + k])
        {
            k++;
        }
        lcp[inv_suff[i]] = k;
        if (k > 0)
        {
            k--;
        }
    }
    free(inv_suff);
    return lcp;
}

void find_longest_repeated_substring(const char* txt, int n, char* output)
{
    if (txt == NULL || n <= 0 || output == NULL)
    {
        if (output != NULL)
        {
            output[0] = '\0';
        }
        return;
    }
    int* sa = build_suffix_array(txt, n);
    if (sa == NULL)
    {
        output[0] = '\0';
        return;
    }
    int* lcp = build_lcp_array(txt, sa, n);
    if (lcp == NULL)
    {
        free(sa);
        output[0] = '\0';
        return;
    }

    int max_lcp = 0, max_idx = 0;
    for (int i = 0; i < n; i++)
    {
        if (lcp[i] > max_lcp)
        {
            max_lcp = lcp[i];
            max_idx = sa[i];
        }
    }

    if (max_lcp > 0)
    {
        strncpy(output, txt + max_idx, max_lcp);
        output[max_lcp] = '\0';
    }
    else
    {
        output[0] = '\0';
    }

    free(sa);
    free(lcp);
}

void visualize_suffix_array(const char* txt, int n, int* sa, int* lcp)
{
    if (sa == NULL || lcp == NULL)
    {
        return;
    }
    printf("\n--- Suffix Array & LCP Visualization ---\n");
    printf("i\tSA[i]\tLCP[i]\tSuffix\n");
    printf("----------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%s\n", i, sa[i], (i == n - 1) ? 0 : lcp[i], txt + sa[i]);
    }
    printf("----------------------------------------\n");
}

void suffix_array_demo(void)
{
    char txt[256];
    printf("Enter a string to generate its Suffix Array (e.g., banana): ");
    if (fgets(txt, sizeof(txt), stdin) != NULL)
    {
        txt[strcspn(txt, "\n")] = 0; // Remove newline

        if (strlen(txt) == 0)
        {
            printf("Error: Empty string provided.\n");
            return;
        }

        int n = strlen(txt);
        int* sa = build_suffix_array(txt, n);
        if (sa == NULL)
        {
            printf("Error: Memory allocation failed for Suffix Array.\n");
            return;
        }
        int* lcp = build_lcp_array(txt, sa, n);
        if (lcp == NULL)
        {
            printf("Error: Memory allocation failed for LCP Array.\n");
            free(sa);
            return;
        }

        visualize_suffix_array(txt, n, sa, lcp);

        char lrs[256];
        find_longest_repeated_substring(txt, n, lrs);
        printf("\nLongest Repeated Substring: '%s'\n\n", lrs);

        free(sa);
        free(lcp);
    }
}