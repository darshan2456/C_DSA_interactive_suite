#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

void advanced_heaps_demo(void)
{
    while (1)
    {
        display_header("Advanced Heaps & Priority Queues Suite");

        int choice;
        int status = safe_input_int(&choice,
                                    "\nSelect Heap Type:\n"
                                    "-----------------\n"
                                    "1. Binomial Heap\n"
                                    "2. Fibonacci Heap\n"
                                    "3. Leftist Heap\n"
                                    "4. Skew Heap\n"
                                    "5. Min-Max Heap (Double-Ended Priority Queue)\n"
                                    "6. d-Ary Heap\n"
                                    "7. Treap (Tree + Heap)\n"
                                    "\nEnter choice (-1 to return): ",
                                    1, 7);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nReturning to main menu...\n");
            return;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                run_binomial_demo();
                break;
            case 2:
                run_fibonacci_demo();
                break;
            case 3:
                run_leftist_demo();
                break;
            case 4:
                run_skew_demo();
                break;
            case 5:
                run_min_max_demo();
                break;
            case 6:
                run_dary_demo();
                break;
            case 7:
                run_treap_demo();
                break;
        }
    }
}
