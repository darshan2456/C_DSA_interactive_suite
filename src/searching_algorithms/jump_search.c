#include "safe_input.h"
#include "searching_algorithms.h"
#include "sorting_algorithms_n2.h"
#include <stdio.h>
#include <time.h>

/*
 * Jump Search
 *
 * Works on sorted arrays.
 *
 * It jumps ahead by fixed-size steps (usually sqrt(n)) and then
 * performs a linear search backward or forward within the identified block.
 *
 * Best Case: O(1)
 * Average Case: O(sqrt(n))
 * Worst Case: O(sqrt(n))
 */

static int integer_sqrt(int n)
{
    if (n < 0)
    {
        return 0;
    }
    if (n == 0 || n == 1)
    {
        return n;
    }
    int start = 1, end = n, ans = 1;
    while (start <= end)
    {
        int mid = start + (end - start) / 2;
        if (mid <= n / mid)
        {
            start = mid + 1;
            ans = mid;
        }
        else
        {
            end = mid - 1;
        }
    }
    return ans;
}

int jump_search(int arr[], int target, int length_of_array)
{
    if (length_of_array <= 0)
    {
        return -1;
    }

    int step = integer_sqrt(length_of_array);
    int prev = 0;

    while (arr[prev + step - 1 < length_of_array ? prev + step - 1 : length_of_array - 1] < target)
    {
        prev += step;
        if (prev >= length_of_array)
        {
            return -1;
        }
    }

    int limit = prev + step < length_of_array ? prev + step : length_of_array;
    for (int i = prev; i < limit; i++)
    {
        if (arr[i] == target)
        {
            return i;
        }
    }

    return -1;
}
