#include "advanced_sorting.h"
#include "safe_input.h"
#include "sorting_visualizer.h"
#include "telemetry.h"
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

    for (int i = 0; i < n; i++)
    {
        if (arr[i] < 0)
        {
            return;
        }
    }

    telemetry_init("radix_sort");

    int max = get_max(arr, n);

    for (int exp = 1; max / exp > 0; exp *= 10)
    {
        counting_sort(arr, n, exp);
    }
    telemetry_close();
}
