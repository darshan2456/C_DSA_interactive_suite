#include "config.h"
#include "dp_visualizer.h"
#include "dynamic_programming.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mcm_demo(void)
{
    int num_matrices;
    while (1)
    {
        printf("\n\nMatrix Chain Multiplication demo");
        int mcm_status = safe_input_int(&num_matrices,
                                        "\nenter the number of matrices "
                                        "(between 1 and 15), enter '-1' to exit:- ",
                                        1, 15);

        if (mcm_status == 0)
        {
            continue;
        }

        if (mcm_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Matrix Chain Multiplication demo....\n");
            return;
        }

        int n = num_matrices + 1;
        int* p = malloc(sizeof(int) * n);
        if (p == NULL)
        {
            printf("Memory allocation failed for dimensions array.\n");
            return;
        }

        printf("\nEnter the dimensions:\n");
        for (int i = 0; i < n; i++)
        {
        retry:
            if (i == 0)
            {
                printf("enter dimension p0 (row count of matrix A1, between 1 and 500):- ");
            }
            else
            {
                printf("enter dimension p%d (column count of matrix A%d, between 1 and 500):- ", i,
                       i);
            }

            int dim_status = safe_input_int(&p[i], NULL, 1, 500);

            if (dim_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Matrix Chain Multiplication demo.....\n");
                free(p);
                return;
            }

            if (dim_status == 0)
            {
                goto retry;
            }
        }

        clock_t start_t = clock();
        int min_multiplications = matrix_chain_order(p, n);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("\nMinimum Scalar Multiplications required: %d", min_multiplications);
        printf("\nTotal CPU time taken:- %f seconds", total_t);

        free(p);
    }
}
