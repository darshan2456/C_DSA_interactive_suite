#include "array.h"
#include "hash.h"
#include <limits.h>
#include <safe_input.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool quadratic_probing_insert(int arr[], int length_of_array, int value)
{
    if (arr == NULL || length_of_array <= 0)
    {
        return false;
    }

    int base_hash_location = hash_function(value, length_of_array);
    if (base_hash_location < 0)
    {
        return false;
    }

    for (int i = 0; i < length_of_array; i++)
    {
        int probe_location =
            (int)(((long long)base_hash_location + (long long)i * i) % length_of_array);

        if (arr[probe_location] == HASH_EMPTY_SLOT)
        {
            arr[probe_location] = value;
            return true;
        }
    }
    return false; // table is full or quadratic probing failed
}

int quadratic_probing_search(int arr[], int length_of_array, int search_val)
{
    if (arr == NULL || length_of_array <= 0)
    {
        return -1;
    }

    int base_hash_location = hash_function(search_val, length_of_array);
    if (base_hash_location < 0)
    {
        return -1;
    }

    for (int i = 0; i < length_of_array; i++)
    {
        int probe_location =
            (int)(((long long)base_hash_location + (long long)i * i) % length_of_array);

        if (arr[probe_location] == search_val)
        {
            return probe_location;
        }

        if (arr[probe_location] == HASH_EMPTY_SLOT)
        {
            break; // stop searching if we hit an empty slot
        }
    }
    return -1; // not found
}
