#include "advanced_sorting.h"
#include "safe_input.h"
#include "sorting_telemetry.h"
#include "sorting_visualizer.h"
#include "telemetry.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int get_max(int arr[], int n, SortingTelemetry* telemetry)
{
    int max = arr[0];
    for (int i = 1; i < n; i++)
    {
        sorting_telemetry_add_comparison(telemetry, 1);
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

static void counting_sort(int arr[], int n, int exp, SortingTelemetry* telemetry)
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
        sorting_telemetry_add_copy(telemetry, 1);
        count[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++)
    {
        arr[i] = output[i];
        sorting_telemetry_add_copy(telemetry, 1);
        visualize_sort(arr, n, i, -1, -1, "Radix Sort: Copying elements back after digit-sorting");
    }

    free(output);
}

void radix_sort_with_telemetry(int arr[], int n, SortingTelemetry* telemetry)
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

    if (telemetry)
    {
        sorting_telemetry_init(telemetry, "Radix Sort");
        sorting_telemetry_start(telemetry);
    }
    telemetry_init("radix_sort");

    int max = get_max(arr, n, telemetry);

    for (int exp = 1; max / exp > 0; exp *= 10)
    {
        sorting_telemetry_add_pass(telemetry, 1);
        counting_sort(arr, n, exp, telemetry);
    }

    telemetry_close();
    if (telemetry)
    {
        sorting_telemetry_stop(telemetry);
    }
}

void radix_sort(int arr[], int n)
{
    radix_sort_with_telemetry(arr, n, NULL);
}
