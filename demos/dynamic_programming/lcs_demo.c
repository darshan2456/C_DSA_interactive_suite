#include "dp_visualizer.h"
#include "dynamic_programming.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void lcs_demo(void)
{
    while (1)
    {
        char X[100];
        char Y[100];

        printf("\nLCS Demo\n");

        int status_X =
            safe_input_string(X, "Enter first string (no spaces, max 99 chars), or 'X' to exit: ");
        if (status_X == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting LCS demo...\n");
            return;
        }

        int status_Y =
            safe_input_string(Y, "Enter second string (no spaces, max 99 chars), or 'X' to exit: ");
        if (status_Y == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting LCS demo...\n");
            return;
        }

        int m = strlen(X);
        int n = strlen(Y);

        clock_t start_t = clock();
        int lcs_len = lcs(X, Y, m, n);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("Length of LCS is: %d\n", lcs_len);
        printf("Total CPU time taken: %f seconds\n", total_t);
    }
}
