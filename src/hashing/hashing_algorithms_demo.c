#include "hash.h"
#include "safe_input.h"
#include <stdio.h>

void hashing_algorithms_demo(void)
{
    while (1)
    {
        int hash_algo_choice;
        int hash_algo_status =
            safe_input_int(&hash_algo_choice,
                           "\n\nenter 1 for linear probing, 2 for separate chaining, and 3 for quadratic probing :- ", 1, 3);

        if (hash_algo_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting hashing algorithms demo....");
            return;
        }
        if (hash_algo_status == 0)
        {
            continue;
        }

        switch (hash_algo_choice)
        {
            case 1:
                linear_probing_demo();
                break;

            case 2:
                separate_chaining_demo();
                break;
                
            case 3:
                quadratic_probing_demo();
                break;
        }
    }
}