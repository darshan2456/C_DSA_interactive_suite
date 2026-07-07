#include "array.h"
#include "hash.h"
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

void double_hashing_demo(void)
{
    while (1)
    {
        int value;
        int length_of_array;

        int length_arr_status = safe_input_int(
            &length_of_array, "\n\nenter length of the array (between 1 and 1000):- ", 1, 1000);

        if (length_arr_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting double_hashing demo");
            return;
        }
        if (length_arr_status == 0)
        {
            continue;
        }

        int arr[length_of_array];

        memset(arr, 0, sizeof(arr)); // empty slot is represented by 0

        // ---------- insertion phase ----------
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

            int h1 = hash_function(value, length_of_array); // primary hash
            int h2 = second_hash(value, length_of_array);   // step size, never 0
            bool inserted = false;

            for (int i = 0; i < length_of_array; i++)
            {
                int probe_location = (h1 + i * h2) % length_of_array;

                if (!arr[probe_location])
                {
                    arr[probe_location] = value;
                    print_array(arr, length_of_array);
                    inserted = true;
                    break;
                }
            }

            if (!inserted)
            {
                printf("\nhash table full\n");
                break;
            }
        }

        // ---------- search phase (follows the same double-hash probe chain) ----------
        while (1)
        {
            int search_val;
            int search_status = safe_input_int(
                &search_val, "\nenter a value to search in the hash table (enter '-1' to exit):- ",
                1, 1000);

            if (search_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting double_hashing demo.....\n");
                return;
            }
            if (search_status == 0)
            {
                continue;
            }

            int h1 = hash_function(search_val, length_of_array);
            int h2 = second_hash(search_val, length_of_array);
            int found_index = -1;

            for (int i = 0; i < length_of_array; i++)
            {
                int probe_location = (h1 + i * h2) % length_of_array;

                if (arr[probe_location] == search_val)
                {
                    found_index = probe_location;
                    break;
                }
                if (!arr[probe_location]) // empty slot: value cannot be further down the chain
                {
                    break;
                }
            }

            if (found_index != -1)
            {
                printf("\nValue %d found in the hash table at index %d.", search_val, found_index);
            }
            else
            {
                printf("\nValue %d not found in the hash table.", search_val);
            }
        }
    }
}
