#include "advanced_sorting.h"
#include "array.h"
#include "safe_input.h"
#include "sll.h"
#include "sorting_telemetry.h"
#include "sorting_visualizer.h"
#include "telemetry.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void bucket_sort_internal(int arr[], int n, int is_top_level, int total_len,
                                 SortingTelemetry* telemetry)
{
    if (arr == NULL || n <= 1)
    {
        return;
    }

    // Find min and max values in the array
    int min_val = arr[0];
    int max_val = arr[0];
    for (int i = 1; i < n; i++)
    {
        sorting_telemetry_add_comparison(telemetry, 2);
        if (arr[i] < min_val)
        {
            min_val = arr[i];
        }
        if (arr[i] > max_val)
        {
            max_val = arr[i];
        }
    }

    // Base Case: If all elements are equal, array is sorted
    if (min_val == max_val)
    {
        return;
    }

    // Establish N = n buckets
    int N = n;
    Node** buckets = calloc(N, sizeof(Node*));
    if (buckets == NULL)
    {
        return;
    }

    // Scatter Phase
    for (int i = 0; i < n; i++)
    {
        long long diff = (long long)arr[i] - min_val;
        long long range = (long long)max_val - min_val;
        int bucket_idx = 0;
        if (range > 0)
        {
            bucket_idx = (int)((diff * (N - 1)) / range);
        }

        if (bucket_idx < 0)
        {
            bucket_idx = 0;
        }
        else if (bucket_idx >= N)
        {
            bucket_idx = N - 1;
        }

        int insert_status = sll_insertAtBeginning(&buckets[bucket_idx], (void*)(intptr_t)arr[i]);
        sorting_telemetry_add_copy(telemetry, 1);
        if (insert_status == -1)
        {
            for (int j = 0; j < N; j++)
            {
                delete_sll(buckets[j], NULL);
            }
            free(buckets);
            return;
        }
    }

    // Gather Phase & Recursive Sort
    int arr_idx = 0;
    for (int i = 0; i < N; i++)
    {
        if (buckets[i] == NULL)
        {
            continue;
        }

        int bucket_size = sll_getLength(buckets[i]);

        if (bucket_size == 1)
        {
            arr[arr_idx] = (int)(intptr_t)(buckets[i]->data);
            sorting_telemetry_add_copy(telemetry, 1);
            if (is_top_level)
            {
                visualize_sort(arr, total_len, arr_idx, -1, -1,
                               "Bucket Sort: Gathering elements back to main array");
            }
            arr_idx++;
            delete_sll(buckets[i], NULL);
            buckets[i] = NULL;
        }
        else if (bucket_size > 1)
        {
            int* temp_arr = calloc(bucket_size, sizeof(int));
            if (temp_arr == NULL)
            {
                for (int j = i; j < N; j++)
                {
                    delete_sll(buckets[j], NULL);
                }
                free(buckets);
                return;
            }

            Node* curr = buckets[i];
            int temp_idx = 0;
            while (curr != NULL)
            {
                temp_arr[temp_idx++] = (int)(intptr_t)(curr->data);
                sorting_telemetry_add_copy(telemetry, 1);
                curr = curr->next;
            }
            delete_sll(buckets[i], NULL);
            buckets[i] = NULL;

            sorting_telemetry_enter_recursion(telemetry);
            bucket_sort_internal(temp_arr, bucket_size, 0, bucket_size, telemetry);
            sorting_telemetry_exit_recursion(telemetry);

            for (int temp_idx_gather = 0; temp_idx_gather < bucket_size; temp_idx_gather++)
            {
                arr[arr_idx] = temp_arr[temp_idx_gather];
                sorting_telemetry_add_copy(telemetry, 1);
                if (is_top_level)
                {
                    visualize_sort(
                        arr, total_len, arr_idx, -1, -1,
                        "Bucket Sort: Gathering sorted bucket elements back to main array");
                }
                arr_idx++;
            }

            free(temp_arr);
        }
    }

    free(buckets);
}

void bucket_sort_with_telemetry(int arr[], int n, SortingTelemetry* telemetry)
{
    if (telemetry)
    {
        sorting_telemetry_init(telemetry, "Bucket Sort");
        sorting_telemetry_start(telemetry);
    }
    telemetry_init("bucket_sort");
    bucket_sort_internal(arr, n, 1, n, telemetry);
    telemetry_close();
    if (telemetry)
    {
        sorting_telemetry_stop(telemetry);
    }
}

void bucket_sort(int arr[], int n)
{
    bucket_sort_with_telemetry(arr, n, NULL);
}
