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
        printf("1. Postfix Expression Evaluation\n");
        printf("2. Infix Expression Evaluation\n");
        int choice;
        int status =
            safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 1);

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
                display_header("Help - Postfix Expression Evaluation");
                printf("POSTFIX EXPRESSION (Reverse Polish Notation):\n");
                printf("    Operators are written after operands (e.g., A B +).\n");
                printf("    No parentheses are required as operations are evaluated in order.\n\n");
                printf("POSTFIX EVALUATION:\n");
                printf("    Uses a Stack to hold operands. When parsing:\n");
                printf("    • If operand is encountered, push it onto Stack.\n");
                printf("    • If operator is encountered, pop the required operands (e.g., 2 for "
                       "binary operators),\n");
                printf("      evaluate the result, and push the result back onto Stack.\n");
                printf("    • The final result will be the single value left on Stack.\n\n");
                getchar();
                break;
            case 2:
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
