#include "dynamic_programming.h"
#include "safe_input.h"
#include <stdio.h>

void dynamic_programming_demo(void)
{
    int dp_status, dp_choice;
    while (1)
    {
        dp_status = safe_input_int(&dp_choice,
                                   "\nenter 1 for 0/1 Knapsack demo"
                                   "\nenter 2 for Longest Common Subsequence (LCS) demo"
                                   "\nenter 3 for Fibonacci sequence demo"
                                   "\nenter choice : ",
                                   1, 3);

        if (dp_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Dynamic Programming demo....\n");
            return;
        }

        if (dp_status == 0)
            continue;

        switch (dp_choice)
        {
            case 1:
                knapsack_demo();
                break;
            case 2:
                lcs_demo();
                break;
            case 3:
                fibonacci_demo();
                break;
        }
    }
}
