#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>

void string_algorithms_demo(void)
{
    int status, choice;
    while (1)
    {
        status = safe_input_int(&choice,
                                "\nenter 1 for Naive String Matching demo"
                                "\nenter 2 for Knuth-Morris-Pratt (KMP) demo"
                                "\nenter 3 for Rabin-Karp demo"
                                "\nenter choice : ",
                                1, 3);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting String Algorithms demo....\n");
            return;
        }

        if (status == 0)
            continue;

        switch (choice)
        {
            case 1:
                naive_string_matching_demo();
                break;
            case 2:
                kmp_demo();
                break;
            case 3:
                rabin_karp_demo();
                break;
        }
    }
}
