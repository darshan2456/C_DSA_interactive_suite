#include "array.h"
#include "hash.h"
#include <limits.h>
#include <safe_input.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

        for (int i = 0; i < length_of_array; i++)
        {
            arr[i] = INT_MIN; // empty slot is represented by INT_MIN
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

            if (linear_probing_insert(arr, length_of_array, value))
            {
                print_array(arr, length_of_array);
            }
            else
            {
                printf("\nHash table full\n");
                break;
            }
            print_array(arr, length_of_array);
        }

        while (1)
        {
            int search_val;
            int search_status = safe_input_int(
                &search_val, "\nenter a value to search in the hash table (enter '-1' to exit):- ",
                0, 1000);

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
            int res = linear_probing_search(arr, length_of_array, search_val);
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
