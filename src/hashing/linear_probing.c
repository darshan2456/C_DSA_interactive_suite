#include "array.h"
#include "hash.h"
#include <safe_input.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// below function is a helper function local to this file is not supposed to used as an API in other
// modules
static int next_prime(int n)
{
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
    int next_prime_no = next_prime(length_of_array);
    return ((value + 1) * next_prime_no) % length_of_array;
}

void linear_probing_demo(void)
{
    while (1)
    {
        int value;
        int length_of_array;

        int length_arr_status = safe_input_int(
            &length_of_array, "\n\nenter length of the array (between 1 and 1000):- ", 1, 1000);

        if (length_arr_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting linear_probing demo");
            return;
        }
        if (length_arr_status == 0)
        {
            continue;
        }

        int arr[length_of_array]; // allocate memory for the array

        memset(arr, 0, sizeof(arr)); // setting all values to zero

        while (1)
        {
            int value_status = safe_input_int(
                &value, "\nenter a value between 1 and 1000 (enter '-1' to search elements):- ", 1,
                1000);

            if (value_status == INPUT_EXIT_SIGNAL)
            {
                break;
            }
            if (value_status == 0)
            {
                continue;
            }

            int hash_location = hash_function(
                value, length_of_array); // calling the hash function on user input value

            if (!arr[hash_location])
            {
                arr[hash_location] = value; // inserting value at its hash location
                print_array(arr, length_of_array);
                continue;
            }

            bool array_full = false;

            int start = hash_location; // the modulo arithmetic for wrap-around logic, same as
                                       // circular queue
            do
            {
                hash_location = (hash_location + 1) % length_of_array;
                if (hash_location == start)
                {
                    array_full = true;
                    printf("\nhash table full\n");
                    break;
                }
            } while (arr[hash_location]);

            if (array_full)
                break;
            arr[hash_location] = value; // inserting value at its hash location

            print_array(arr, length_of_array);
        }

        while (1)
        {
            int search_val;
            int search_status = safe_input_int(
                &search_val, "\nenter a value to search in the hash table (enter '-1' to exit):- ",
                1, 1000);

            if (search_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting linear_probing demo.....\n");
                return;
            }
            if (search_status == 0)
            {
                continue;
            }

            // search uses the SAME probing sequence as insertion: start at the hash location and
            // walk forward with wrap-around. instead of placing the value, we record the index
            // where it is found. this reflects the real cost of a hash-table lookup.
            int hash_location = hash_function(search_val, length_of_array);
            int res = -1;
            int start = hash_location;

            do
            {
                if (!arr[hash_location])
                {
                    break; // empty slot ends the probe sequence: value is not present
                }
                if (arr[hash_location] == search_val)
                {
                    res = hash_location; // value found, record its index
                    break;
                }
                hash_location = (hash_location + 1) % length_of_array;
            } while (hash_location != start); // wrapped all the way around: value is not present

            if (res != -1)
            {
                printf("\nValue %d found in the hash table at index %d.", search_val, res);
            }
            else
            {
                printf("\nValue %d not found in the hash table.", search_val);
            }
        }
    }
}
