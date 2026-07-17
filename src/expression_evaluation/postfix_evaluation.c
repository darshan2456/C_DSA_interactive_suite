#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "expression.h"
#include "safe_input.h"
#include <ctype.h>
#include <stdio.h>

// if postfix expression attempts to divide by zero or the stack doesnt get emptied at the end of
// main while loop, it indicates malformed postfix expression and program exits with error code '-1'
// and on succesful evaluation returns '0' maximum expression length is 50 characters

int evaluate_postfix_expr(const char* postfix_expr, int* final_result)
{
    if (postfix_expr == NULL || postfix_expr[0] == '\0' || final_result == NULL)
    {
        return EXPR_ERROR_EMPTY;
    }

    stack* operands = createStack();
    int i = 0;
    int step = 1;
    char action_msg[200];
    int current_result = 0;

    while (postfix_expr[i] != '\0')
    {
        if (!is_instant())
        {
            clear_screen();
        }
        action_msg[0] = '\0';
        char ch = postfix_expr[i];
        if (isdigit((unsigned char)ch))
        {
            push(operands, ch - '0');
            snprintf(action_msg, sizeof(action_msg), "Pushed operand '%c' onto stack", ch);
            current_result = ch - '0';

            printf("Step    : %d\n", step);
            printf("Char    : %c\n", ch);
            printf("Action  : %s\n", action_msg);
            printf("Stack   : ");
            printStackAsInts(operands);
            printf("Result  : %d\n", current_result);
            printf("-----------------------------------\n\n");
            step++;
        }
        else if (isOperator(ch))
        {
            if (isEmpty(operands))
            {
                printf("\n[Error] Invalid expression: Stack underflow (missing operands for "
                       "operator '%c')\n",
                       ch);
                destroyStack(operands);
                return EXPR_ERROR_STACK_UNDERFLOW;
            }
            int right_operand = pop(operands);
            if (isEmpty(operands))
            {
                printf("\n[Error] Invalid expression: Stack underflow (missing operands for "
                       "operator '%c')\n",
                       ch);
                destroyStack(operands);
                return EXPR_ERROR_STACK_UNDERFLOW;
            }
            int left_operand = pop(operands);
            int result = 0;
            if (ch == '+')
            {
                result = left_operand + right_operand;
                snprintf(action_msg, sizeof(action_msg),
                         "Popped operands %d and %d, evaluated %d + %d = %d, pushed result back "
                         "onto stack",
                         left_operand, right_operand, left_operand, right_operand, result);
            }
            else if (ch == '-')
            {
                result = left_operand - right_operand;
                snprintf(action_msg, sizeof(action_msg),
                         "Popped operands %d and %d, evaluated %d - %d = %d, pushed result back "
                         "onto stack",
                         left_operand, right_operand, left_operand, right_operand, result);
            }
            else if (ch == '*')
            {
                result = left_operand * right_operand;
                snprintf(action_msg, sizeof(action_msg),
                         "Popped operands %d and %d, evaluated %d * %d = %d, pushed result back "
                         "onto stack",
                         left_operand, right_operand, left_operand, right_operand, result);
            }
            else if (ch == '/')
            {
                if (right_operand == 0)
                {
                    printf("\n[Error] Division by zero encountered.\n");
                    destroyStack(operands);
                    return EXPR_ERROR_DIVIDE_BY_ZERO;
                }
                result = left_operand / right_operand;
                snprintf(action_msg, sizeof(action_msg),
                         "Popped operands %d and %d, evaluated %d / %d = %d, pushed result back "
                         "onto stack",
                         left_operand, right_operand, left_operand, right_operand, result);
            }
            push(operands, result);
            current_result = result;

            printf("Step    : %d\n", step);
            printf("Char    : %c\n", ch);
            printf("Action  : %s\n", action_msg);
            printf("Stack   : ");
            printStackAsInts(operands);
            printf("Result  : %d\n", current_result);
            printf("-----------------------------------\n\n");
            step++;
        }
        else
        {
            destroyStack(operands);
            return EXPR_ERROR_INVALID_CHAR;
        }

        i++;
        dynamic_sleep();
    }

    if (isEmpty(operands))
    {
        destroyStack(operands);
        return EXPR_ERROR_EMPTY;
    }

    *final_result = pop(operands);

    if (!isEmpty(operands))
    {
        destroyStack(operands);
        return EXPR_ERROR_MALFORMED;
    }

    destroyStack(operands);
    return EXPR_SUCCESS;
}

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