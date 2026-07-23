#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "expression.h"
#include "safe_input.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

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
            push(operands, (void*)(intptr_t)(ch - '0'));
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
                destroyStack(operands, NULL);
                return EXPR_ERROR_STACK_UNDERFLOW;
            }
            int right_operand = (int)(intptr_t)pop(operands);
            if (isEmpty(operands))
            {
                printf("\n[Error] Invalid expression: Stack underflow (missing operands for "
                       "operator '%c')\n",
                       ch);
                destroyStack(operands, NULL);
                return EXPR_ERROR_STACK_UNDERFLOW;
            }
            int left_operand = (int)(intptr_t)pop(operands);
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
                    destroyStack(operands, NULL);
                    return EXPR_ERROR_DIVIDE_BY_ZERO;
                }
                result = left_operand / right_operand;
                snprintf(action_msg, sizeof(action_msg),
                         "Popped operands %d and %d, evaluated %d / %d = %d, pushed result back "
                         "onto stack",
                         left_operand, right_operand, left_operand, right_operand, result);
            }
            push(operands, (void*)(intptr_t)result);
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
            destroyStack(operands, NULL);
            return EXPR_ERROR_INVALID_CHAR;
        }

        i++;
        dynamic_sleep();
    }

    if (isEmpty(operands))
    {
        destroyStack(operands, NULL);
        return EXPR_ERROR_EMPTY;
    }

    *final_result = (int)(intptr_t)pop(operands);

    if (!isEmpty(operands))
    {
        destroyStack(operands, NULL);
        return EXPR_ERROR_MALFORMED;
    }

    destroyStack(operands, NULL);
    return EXPR_SUCCESS;
}
