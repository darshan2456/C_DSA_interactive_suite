#include "display_header.h"
#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>

void string_algorithms_demo(void)
{
    int status, choice;
    while (1)
    {
        display_header("String Algorithms");

        status = safe_input_int(&choice,
                                "\nenter 1 for Naive String Matching demo"
                                "\nenter 2 for Knuth-Morris-Pratt (KMP) demo"
                                "\nenter 3 for Rabin-Karp demo"
                                "\nenter 4 for Suffix Array & Kasai's LCP demo"
                                "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                1, 4);

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
                display_header("Naive String Matching");
                naive_string_matching_demo();
                break;
            case 2:
                display_header("Knuth-Morris-Pratt (KMP)");
                kmp_demo();
                break;
            case 3:
                display_header("Rabin-Karp");
                rabin_karp_demo();
                break;
            case 4:
                display_header("Suffix Array & Kasai's LCP");
                suffix_array_demo();
                break;
        }
    }
}