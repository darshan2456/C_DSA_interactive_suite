#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include <stdio.h>
#include <time.h>

int binary_search_recursive(int arr[], int target, int low, int high);

// recursively searches arr[low..high] for target, returning its index or -1 if absent.
// low and high carry the shrinking search window, so the demo starts it with 0 and length - 1.
int binary_search_recursive(int arr[], int target, int low, int high)
{
    if (low > high) // base case: empty window, target is not present
    {
        return -1;
    }

    int mid = low + (high - low) / 2;

    if (arr[mid] == target)
    {
        return mid;
    }
    else if (arr[mid] < target)
    {
        // target is greater, search the right half
        return binary_search_recursive(arr, target, mid + 1, high);
    }
    else
    {
        // target is smaller, search the left half
        return binary_search_recursive(arr, target, low, mid - 1);
    }
}
