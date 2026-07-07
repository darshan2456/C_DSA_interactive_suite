#include "array.h"
#include "hash.h"
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

        memset(arr, 0, sizeof(arr));

        while (1)
        {
            int value_status = safe_input_int(
                &value, "\nenter a value between 1 and 1000 (enter '-1' to exit):- ", 1, 1000);

            if (value_status == INPUT_EXIT_SIGNAL)
            {
                break;
            }
            if (value_status == 0)
            {
                continue;
            }

            int base_hash_location = hash_function(value, length_of_array);
            bool inserted = false;

            for (int i = 0; i < length_of_array; i++)
            {
                int probe_location = (base_hash_location + i * i) % length_of_array;

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
                printf("\nhash table full or quadratic probe failed, old table destroyed, new "
                       "table created\n");
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
                "\nenter a value to search in the hash table (enter '-1' to exit):- ", 1, 1000);

            if (search_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting quadratic_probing demo.....");
                return;
            }

            if (search_status == 0)
            {
                continue;
            }

            int base_hash_location = hash_function(search_value, length_of_array);
            bool found = false;

            for (int i = 0; i < length_of_array; i++)
            {
                int probe_location = (base_hash_location + i * i) % length_of_array;

                if (arr[probe_location] == search_value)
                {
                    printf("\nValue %d found in the hash table at index %d.", search_value,
                           probe_location);
                    found = true;
                    break;
                }

                if (arr[probe_location] == 0)
                {
                    break;
                }
            }

            if (!found)
            {
                printf("\nValue %d not found in the hash table.", search_value);
            }
        }
    }
}