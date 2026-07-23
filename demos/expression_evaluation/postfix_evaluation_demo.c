#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "expression.h"
#include "safe_input.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void postfix_evaluation_demo(void)
{
    char postfix_expr[50];
    while (1)
    {
        printf("\nPostfix Evaluation demo\n");
        int postfix_expr_status = validate_postfix_expr(
            postfix_expr, sizeof(postfix_expr),
            "\nenter valid postfix expression (only single digit operands), enter 'X' to exit:- ");

        if (postfix_expr_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting postfix evaluation demo...\n");
            return;
        }

        if (postfix_expr_status != 1)
        {
            continue;
        }

        int final_result = 0;
        int result = evaluate_postfix_expr(postfix_expr, &final_result);

        if (result == EXPR_SUCCESS)
        {
            printf("\n===================================\n");
            printf("Final Evaluated Result : %d\n", final_result);
            printf("===================================\n\n");
        }
        else if (result == EXPR_ERROR_DIVIDE_BY_ZERO)
        {
            printf("\n[Error] Evaluation failed: Division by zero.\n");
        }
        else if (result == EXPR_ERROR_STACK_UNDERFLOW)
        {
            printf("\n[Error] Evaluation failed: Stack underflow (missing operands).\n");
        }
        else if (result == EXPR_ERROR_MALFORMED)
        {
            printf("\n[Error] Evaluation failed: Malformed expression (too many operands).\n");
        }
        else
        {
            printf("\n[Error] Evaluation failed: Code %d.\n", result);
        }
    }
}