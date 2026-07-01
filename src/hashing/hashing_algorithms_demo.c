#include "display_header.h"
#include "hash.h"
#include "safe_input.h"
#include <stdio.h>

void hashing_algorithms_demo(void)
{
    while (1)
    {
        display_header("Hashing Algorithms");

        int hash_algo_choice;
        int hash_algo_status = safe_input_int(&hash_algo_choice,
                                              "\n\nenter 1 for linear probing, 2 for separate "
                                              "chaining, 3 for quadratic probing, and 4 for double "
                                              "hashing :- ",
                                              1, 4);

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
                display_header("Linear Probing");
                linear_probing_demo();
                break;

            case 2:
                display_header("Separate Chaining");
                separate_chaining_demo();
                break;

            case 3:
                display_header("Quadratic Probing");
                quadratic_probing_demo();
                break;

            case 4:
                display_header("Double Hashing");
                double_hashing_demo();
                break;
        }
    }
}
