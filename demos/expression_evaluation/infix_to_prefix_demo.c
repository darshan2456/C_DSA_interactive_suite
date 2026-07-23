#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "expression.h"
#include "safe_input.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void infix_to_prefix_demo(void)
{
    char infix_expr[50];
    char prefix_expr[100];

    while (1)
    {
        printf("\nInfix to Prefix Demo\n");

        int infix_expr_status =
            validate_infix_expr(infix_expr, sizeof(infix_expr),
                                "\nEnter a mathematical expression without whitespaces "
                                "(Enter 'X' to exit): ");

        if (infix_expr_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting infix to prefix demo...\n");
            return;
        }

        if (infix_expr_status != 1)
        {
            continue;
        }

        int result = infix_to_prefix_convert(infix_expr, prefix_expr, sizeof(prefix_expr));

        if (result == EXPR_SUCCESS)
        {
            printf("\n==================================\n");
            printf("Conversion Complete\n");
            printf("Final Prefix      : %s\n", prefix_expr);
            printf("==================================\n\n");
        }
        else if (result == EXPR_ERROR_UNMATCHED_PARENTHESES)
        {
            printf("\n[Error] Conversion failed: Unmatched parentheses.\n");
        }
        else
        {
            printf("\n[Error] Conversion failed: Code %d.\n", result);
        }

        dynamic_sleep();
    }
}