#include "array.h"
#include "hash.h"
#include <limits.h>
#include <safe_input.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static int gcd(int a, int b)
{
    while (b != 0)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// secondary hash: the probe step size for double hashing. 1 + (value % (length - 1))
// always lands in [1, length - 1], so the step is never 0 (which would stall probing)
// and it depends on the key -- the defining property of double hashing. local helper,
// not meant to be used as an API in other modules.
static int second_hash(int value, int length_of_array)
{
    if (length_of_array <= 1)
    {
        return 1; // single-slot table: step value is irrelevant, just stay non-zero
    }
    int step = 1 + (value % (length_of_array - 1));
    if (step <= 0)
    {
        step += (length_of_array - 1);
    }
    while (gcd(step, length_of_array) != 1)
    {
        step++;
        if (step >= length_of_array)
        {
            step = 1;
        }
    }
    return step;
}

bool double_hashing_insert(int arr[], int length_of_array, int value)
{
    if (arr == NULL || length_of_array <= 0)
    {
        return false;
    }

    int h1 = hash_function(value, length_of_array);
    if (h1 < 0)
    {
        return false;
    }

    int h2 = second_hash(value, length_of_array);

    for (int i = 0; i < length_of_array; i++)
    {
        int probe_location = (int)(((long long)h1 + (long long)i * h2) % length_of_array);
        if (probe_location < 0)
        {
            probe_location += length_of_array;
        }

        if (arr[probe_location] == HASH_EMPTY_SLOT)
        {
            arr[probe_location] = value;
            return true;
        }
    }

    return false;
}

int double_hashing_search(int arr[], int length_of_array, int search_val)
{
    if (arr == NULL || length_of_array <= 0)
    {
        return -1;
    }

    int h1 = hash_function(search_val, length_of_array);
    if (h1 < 0)
    {
        return -1;
    }

    int h2 = second_hash(search_val, length_of_array);

    for (int i = 0; i < length_of_array; i++)
    {
        int probe_location = (int)(((long long)h1 + (long long)i * h2) % length_of_array);
        if (probe_location < 0)
        {
            probe_location += length_of_array;
        }

        if (arr[probe_location] == search_val)
        {
            return probe_location;
        }

        if (arr[probe_location] == HASH_EMPTY_SLOT)
        {
            break;
        }
    }

    return -1;
}
