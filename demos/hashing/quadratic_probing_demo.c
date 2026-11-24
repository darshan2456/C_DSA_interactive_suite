#include "array.h"
#include "hash.h"
#include <limits.h>
#include <safe_input.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void quadratic_probing_demo(void)
{
    while (1)
    {
        int value;
        int length_of_array;

        int length_arr_status = safe_input_int(
            &length_of_array, "\n\nenter length of the array (between 1 and 1000):- ", 1, 1000);

        if (length_arr_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting quadratic_probing demo");
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

            if (quadratic_probing_insert(arr, length_of_array, value))
            {
                print_array(arr, length_of_array);
            }
            else
            {
                printf("\nHash table full or quadratic probing failed\n");
                break;
            }
        }
        printf("\nCurrent hash table:\n");
        print_array(arr, length_of_array);
        while (1)
        {
            int search_value;

            int search_status = safe_input_int(
                &search_value,
                "\nenter a value to search in the hash table (enter '-1' to exit):- ", 0, 1000);

            if (search_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting quadratic_probing demo.....");
                return;
            }

            if (search_status == 0)
            {
                continue;
            }

            int found_index = quadratic_probing_search(arr, length_of_array, search_value);
            bool found = (found_index != -1);

            if (found)
            {
                printf("\nValue %d found at index %d in the hash table.", search_value,
                       found_index);
            }
            else
            {
                printf("\nValue %d not found in the hash table.", search_value);
            }
        }
    }
}