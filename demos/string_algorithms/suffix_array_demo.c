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

void suffix_array_demo(void)
{
    char txt[256];
    printf("Enter a string to generate its Suffix Array (e.g., banana): ");
    if (fgets(txt, sizeof(txt), stdin) != NULL)
    {
        trim_newline(txt);

        int n = (int)strlen(txt);
        if (n <= 0)
        {
            printf("Error: Empty string provided.\n");
            return;
        }

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