#include "safe_input.h"
#include "searching_algorithms.h"
#include "sorting_algorithms_n2.h"
#include <stdio.h>
#include <time.h>

/*
 * Interpolation Search
 *
 * Works on sorted arrays.
 *
 * Instead of always checking the middle element like Binary Search,
 * it estimates the likely position of the target based on its value.
 *
 * Best Case: O(1)
 * Average Case: O(log log n)
 * Worst Case: O(n)
 */

int interpolation_search(int arr[], int target, int length_of_array)
{
    int low = 0;
    int high = length_of_array - 1;

    while (low <= high && target >= arr[low] && target <= arr[high])
    {
        if (low == high)
        {
            return (arr[low] == target) ? low : -1;
        }

        if (arr[low] == arr[high])
        {
            if (arr[low] == target)
                return low;
            return -1;
        }

        int pos =
            low + (int)(((double)(high - low) / (arr[high] - arr[low])) * (target - arr[low]));

        if (arr[pos] == target)
            return pos;

        if (arr[pos] < target)
            low = pos + 1;
        else
            high = pos - 1;
    }

    return -1;
}
