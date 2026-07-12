#include "help.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>

void launch_help_page(void)
{
    static int in_help = 0;
    if (in_help)
    {
        printf("\n[Notice] You are already inside the Help Manual.\n");
        return;
    }
    in_help = 1;

    while (1)
    {
        display_header("C DSA INTERACTIVE SUITE - HELP MANUAL");

        printf("Select a module to view its help page:\n\n");
        printf("1. Data Structures Help\n");
        printf("2. Expression Evaluation Help\n");
        printf("3. Sorting & Searching Help\n");
        printf("4. Graphs & Trees Help\n");
        printf("5. Advanced & Specialized Topics Help\n");
        printf("6. Navigation, Commands & General Info\n");
        int choice;
        int status = safe_input_int(&choice, "Enter choice (or -1 to exit help): ", 1, 6);

        if (status == INPUT_EXIT_SIGNAL)
        {
            break;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                help_data_structures_menu();
                break;
            case 2:
                help_expression_evaluation_menu();
                break;
            case 3:
                help_sorting_searching_menu();
                break;
            case 4:
                help_graphs_trees_menu();
                break;
            case 5:
                help_advanced_topics_menu();
                break;
            case 6:
                display_header("General Navigation & Commands");
                printf("DESCRIPTION\n");
                printf("    The C DSA Interactive Suite is a terminal-based application\n");
                printf("    designed to help you visualize and interact with Data Structures\n");
                printf("    and Algorithms implemented in C.\n\n");
                printf("NAVIGATION & COMMANDS\n");
                printf("    help        - Launches this help page from any input prompt.\n");
                printf("    -1          - Exits the current menu, sub-suite, or application.\n");
                printf("    Numbers     - Choose specific menu items or menu paths.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return to the main help menu...\n");
                printf("=================================================================\n");
                getchar();
                break;
        }
    }

    in_help = 0;
    // Clear screen again to restore the clean look of the app menu
    printf("\033[H\033[2J");
}