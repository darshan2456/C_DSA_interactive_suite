#include "bigo_verifier.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

void bigo_verifier_demo(void)
{
    while (1)
    {
        printf("\n========================================================================\n");
        printf("         EMPIRICAL ASYMPTOTIC COMPLEXITY VERIFIER (BIG-O ENGINE)        \n");
        printf("========================================================================\n");
        printf("1. Profile Bubble Sort [Theoretical O(N^2)]\n");
        printf("2. Profile Quick Sort [Theoretical O(N log N)]\n");
        printf("3. Profile Merge Sort [Theoretical O(N log N)]\n");
        printf("4. Profile Linear Search [Theoretical O(N)]\n");
        printf("5. Profile Binary Search [Theoretical O(log N)]\n");
        printf("6. Profile BFS Graph Traversal [Theoretical O(N)]\n");
        printf("7. Profile BST Insertion [Theoretical O(N log N)]\n");
        printf("8. Profile Dynamic Programming Fibonacci [Theoretical O(N)]\n");
        printf("-1. Return to Main Menu\n");

        int choice;
        int status = safe_input_int(&choice, "\nEnter choice: ", 1, 8);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Big-O Verifier Dashboard...\n");
            return;
        }

        if (status == 0)
        {
            continue;
        }

        BigOReport report;
        switch (choice)
        {
            case 1:
                profile_bubble_sort_bigo(200, &report);
                break;
            case 2:
                profile_quick_sort_bigo(2000, &report);
                break;
            case 3:
                profile_merge_sort_bigo(2000, &report);
                break;
            case 4:
                profile_linear_search_bigo(20000, &report);
                break;
            case 5:
                profile_binary_search_bigo(50000, &report);
                break;
            case 6:
                profile_bfs_graph_bigo(2000, &report);
                break;
            case 7:
                profile_bst_operations_bigo(1000, &report);
                break;
            case 8:
                profile_dp_fibonacci_bigo(50000, &report);
                break;
        }

        print_bigo_report(&report);
    }
}
