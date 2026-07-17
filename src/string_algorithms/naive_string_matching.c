#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void naive_string_matching(char* text, char* pattern)
{
    int n = strlen(text);
    int m = strlen(pattern);
    int found = 0;

    for (int i = 0; i <= n - m; i++)
    {
        int j;
        for (j = 0; j < m; j++)
        {
            if (text[i + j] != pattern[j])
                break;
        }
        if (j == m)
        {
            printf("Pattern found at index %d\n", i);
            found++;
        }
    }

    if (found == 0)
    {
        printf("Pattern not found in the text.\n");
    }
    else
    {
        printf("Total occurrences found: %d\n", found);
    }
}
