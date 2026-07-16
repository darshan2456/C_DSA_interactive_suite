#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include <stdio.h>
#include <time.h>

int binary_search(int arr[], int target, int length_of_array);

int binary_search(int arr[], int target, int length_of_array)
{
    int low = 0;
    int high = length_of_array - 1;
    int mid = 0;

    if (length_of_array == 1)
    {
        if (arr[0] == target)
        {
            return 0;
        }
        return -1;
    }

    while (low <= high)
    { // main loop which performs the binary search algorithm
        mid = low + (high - low) / 2;
        // printf("DEBUG: low=%d, mid=%d, high=%d, arr[mid]=%d\n", low, mid, high, arr[mid]);
        if (arr[mid] == target)
        {
            return mid;
        }
        else if (arr[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return -1;
}