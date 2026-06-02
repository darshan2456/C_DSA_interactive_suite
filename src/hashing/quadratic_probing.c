#include "data_structures.h"
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
                printf("\nExiting quadratic_probing demo.....");
                return;
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
                printf("\nhash table full or quadratic probe failed, old table destroyed, new table created\n");
                break;
            }
        }
    }
}