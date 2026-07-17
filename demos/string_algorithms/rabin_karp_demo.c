#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define d 256

void rabin_karp_demo(void)
{
    while (1)
    {
        char text[100];
        char pattern[100];
        int q = 101; // A prime number

        printf("\nRabin-Karp Algorithm Demo\n");

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
        // 🚨 Point the demo to the visualization function 🚨
        rabin_karp_visualization(text, pattern, q);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("Total CPU time taken: %f seconds\n", total_t);
    }
}