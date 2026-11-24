#include "array.h"
#include "hash.h"
#include <limits.h>
#include <safe_input.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

        for (int i = 0; i < length_of_array; i++)
        {
            arr[i] = HASH_EMPTY_SLOT; // empty slot is represented by HASH_EMPTY_SLOT
        }

        // ---------- insertion phase ----------
        while (1)
        {
            int value_status = safe_input_int(
                &value, "\nenter a value between 0 and 1000 (enter '-1' to search elements):- ", 0,
                1000);

            if (value_status == INPUT_EXIT_SIGNAL)
            {
                break;
            }
            if (value_status == 0)
            {
                continue;
            }

            if (double_hashing_insert(arr, length_of_array, value))
            {
                print_array(arr, length_of_array);
            }
            else
            {
                printf("\nHash table full\n");
                break;
            }
        }

        // ---------- search phase (follows the same double-hash probe chain) ----------
        while (1)
        {
            int search_val;
            int search_status = safe_input_int(
                &search_val, "\nenter a value to search in the hash table (enter '-1' to exit):- ",
                0, 1000);

            if (search_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting double_hashing demo.....\n");
                return;
            }
            if (search_status == 0)
            {
                continue;
            }

            int found_index = double_hashing_search(arr, length_of_array, search_val);

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
