#include "array.h"
#include "hash.h"
#include <limits.h>
#include <safe_input.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// below function is a helper function local to this file is not supposed to used as an API in other
// modules
static int next_prime(int n)
{
    if (n <= 1)
    {
        return 2;
    }
    int size = sizeof(PRIMES) / sizeof(PRIMES[0]);
    for (int i = n + 1; i < size; i++)
    {
        if (PRIMES[i])
        {
            return i;
        }
    }
    // No prime greater than n fits in the table (n is near its upper bound).
    // Fall back to the largest prime <= n so the multiplier stays a positive
    // prime and hash_function never yields a negative (out-of-bounds) index.
    for (int i = (n < size ? n : size - 1); i >= 2; i--)
    {
        if (PRIMES[i])
        {
            return i;
        }
    }
    return 1;
}

int hash_function(int value, int length_of_array)
{
    if (length_of_array <= 0)
    {
        return -1;
    }
    int next_prime_no = next_prime(length_of_array);
    int hash = ((value + 1) * next_prime_no) % length_of_array;
    if (hash < 0)
    {
        hash += length_of_array;
    }
    return hash;
}

bool linear_probing_insert(int arr[], int length_of_array, int value)
{
    int hash_location = hash_function(value, length_of_array);
    if (hash_location == -1)
    {
        return false;
    }

    if (arr[hash_location] == 0)
    {
        arr[hash_location] = value; // inserting value at its hash location
        return true;
    }

    bool array_full = false;

    int start =
        hash_location; // the modulo arithmetic for wrap-around logic, same as circular queue
    do
    {
        hash_location = (hash_location + 1) % length_of_array;
        if (hash_location == start)
        {
            array_full = true;
            break;
        }
    } while (arr[hash_location] != 0);

    if (array_full)
        return false;

    arr[hash_location] = value; // inserting value at its hash location
    return true;
}

int linear_probing_search(int arr[], int length_of_array, int search_val)
{
    int hash_location = hash_function(search_val, length_of_array);
    if (hash_location == -1)
    {
        return -1;
    }

    int start =
        hash_location; // the modulo arithmetic for wrap-around logic, same as circular queue
    do
    {
        if (arr[hash_location] == search_val)
        {
            return hash_location;
        }
        hash_location = (hash_location + 1) % length_of_array;
    } while (hash_location != start && arr[hash_location] != 0);

    return -1; // value not found
}
