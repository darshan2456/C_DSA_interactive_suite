#include "display_header.h"
#include "help.h"
#include "safe_input.h"
#include <stdio.h>

void help_expression_evaluation_menu(void)
{
    while (1)
    {
        display_header("Help - Expression Evaluation");

        printf("Select a sub-topic:\n\n");
        printf("1. Infix Expression Evaluation\n");
        int choice;
        int status = safe_input_int(&choice, "Enter choice (or -1 to go back): ", 1, 1);

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
                display_header("Help - Infix Expression Evaluation");
                printf("INFIX EXPRESSION:\n");
                printf("    Operators are written in-between operands (e.g., A + B).\n");
                printf("    Requires operator precedence and parentheses rules to evaluate.\n\n");
                printf("INFIX TO POSTFIX CONVERSION:\n");
                printf(
                    "    Uses a Stack to store operators and enforce precedence. When parsing:\n");
                printf("    • Operands are output immediately.\n");
                printf("    • Left parentheses '(' are pushed onto the Stack.\n");
                printf("    • Right parentheses ')' pop operators until '(' is encountered.\n");
                printf("    • Operators pop higher or equal precedence operators from Stack, then "
                       "push themselves.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                getchar();
                break;
        }
    }
}
