#include "safe_input.h"
#include "searching_algorithms.h"
#include <stdio.h>

void searching_algorithms_demo(void)
{
    int searching_algo_status, searching_algo_choice;
    while (1)
    {
        searching_algo_status = safe_input_int(&searching_algo_choice,
                                               "\nenter 1 for linear search demo"
                                               "\nenter 2 for binary search"
                                               "\nenter 3 for recursive binary search"
                                               "\nenter 4 for interpolation search"
                                               "\nenter 5 for jump search"
                                               "\nenter choice : ",
                                               1, 5);

        if (searching_algo_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting searching_algorithms_demo....\n");
            return;
        }

        if (searching_algo_status == 0)
            continue;

        switch (searching_algo_choice)
        {
            case 1:
                linear_search_demo();
                break;
            case 2:
                binary_search_demo();
                break;
            case 3:
                binary_search_recursive_demo();
                break;
            case 4:
                interpolation_search_demo();
                break;
            case 5:
                jump_search_demo();
                break;
        }
    }
}
