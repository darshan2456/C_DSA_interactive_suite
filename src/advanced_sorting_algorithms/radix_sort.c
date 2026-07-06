#include "advanced_sorting.h"
#include "safe_input.h"
#include "sorting_visualizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int get_max(int arr[], int n)
{
    int max = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

static void counting_sort(int arr[], int n, int exp)
{
    int* output = malloc(sizeof(int) * n);
    if (output == NULL)
    {
        return;
    }
    int count[10] = {0};

    for (int i = 0; i < n; i++)
    {
        count[(arr[i] / exp) % 10]++;
    }

    for (int i = 1; i < 10; i++)
    {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--)
    {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++)
    {
        arr[i] = output[i];
        visualize_sort(arr, n, i, -1, -1, "Radix Sort: Copying elements back after digit-sorting");
    }

    free(output);
}

void radix_sort(int arr[], int n)
{
    if (n <= 1)
    {
        return;
    }

    int max = get_max(arr, n);

    for (int exp = 1; max / exp > 0; exp *= 10)
    {
        counting_sort(arr, n, exp);
    }
}

void radix_sort_demo(void)
{
    int length_of_array;
    while (1)
    {
        printf("\n\nRadix sort demo");
        int radix_sort_status = safe_input_int(&length_of_array,
                                               "\nenter the number of elements in the array "
                                               "(between 1 and 100), enter '-1' to exit:- ",
                                               1, 100);

        if (radix_sort_status == 0)
        {
            continue;
        }

        if (radix_sort_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting radix sort demo....\n");
            return;
        }

        int arr[length_of_array];

        for (int i = 0; i < length_of_array; i++)
        {
        retry:
            printf("\nenter the element number %d, (between 1 and 10000), enter '-1' to exit:- ",
                   i);
            int element_status = safe_input_int(&arr[i], NULL, 1, 10000);

            if (element_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting radix sort demo.....\n");
                return;
            }

            if (element_status == 0)
            {
                goto retry;
            }
        }

        clock_t start_t = clock();
        radix_sort(arr, length_of_array);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("\nThe array sorted by radix sort is:- ");
        for (int i = 0; i < length_of_array; i++)
        {
            printf("%d", arr[i]);
            if (i < length_of_array - 1)
                printf(",");
        }
        printf("\nTotal CPU time taken:- %f seconds", total_t);
    }
}