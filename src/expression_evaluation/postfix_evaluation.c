#include "cross_platform.h"
#include "safe_input.h"
#include "stack.h"
#include <ctype.h>
#include <stdio.h>

// if postfix expression attempts to divide by zero or the stack doesnt get emptied at the end of
// main while loop, it indicates malformed postfix expression and program exits with error code '-1'
// and on succesful evaluation returns '0' maximum expression length is 50 characters

static int isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
        return 1;
    return 0;
}

void postfix_evaluation_Demo(void)
{
    char postfix_expr[50];
    while (1)
    {
        stack* operands = createStack();

        printf("\nPostfix Evaluation demo\n");
        int postfix_expr_status = validate_postfix_expr(
            postfix_expr, sizeof(postfix_expr),
            "\nenter valid postfix expression (only single digit operands), enter 'X' to exit:- ");

        if (postfix_expr_status == INPUT_EXIT_SIGNAL)
        {
            destroyStack(operands);
            printf("\nExiting postfix evaluation demo...\n");
            return;
        }

        if (postfix_expr_status != 1)
        {
            destroyStack(operands);
            continue;
        }

        int i = 0;
        int step = 1;
        char action_msg[200];
        int current_result = 0;
        while (postfix_expr[i] != '\0')
        {
            action_msg[0] = '\0';
            char ch = postfix_expr[i];
            if (isdigit(ch))
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
                    destroyStack(operands);
                    return;
                }
                int right_operand = pop(operands);
                if (isEmpty(operands))
                {
                    destroyStack(operands);
                    return;
                }
                int left_operand = pop(operands);
                int result = 0;
                if (ch == '+')
                {
                    result = left_operand + right_operand;
                    snprintf(action_msg, sizeof(action_msg),
                             "Popped operands %d and %d, evaluated %d + %d = %d, pushed result "
                             "back onto stack",
                             left_operand, right_operand, left_operand, right_operand, result);
                }
                else if (ch == '-')
                {
                    result = left_operand - right_operand;
                    snprintf(action_msg, sizeof(action_msg),
                             "Popped operands %d and %d, evaluated %d - %d = %d, pushed result "
                             "back onto stack",
                             left_operand, right_operand, left_operand, right_operand, result);
                }
                else if (ch == '*')
                {
                    result = left_operand * right_operand;
                    snprintf(action_msg, sizeof(action_msg),
                             "Popped operands %d and %d, evaluated %d * %d = %d, pushed result "
                             "back onto stack",
                             left_operand, right_operand, left_operand, right_operand, result);
                }
                else if (ch == '/')
                {
                    if (right_operand == 0)
                    {
                        destroyStack(operands);
                        return;
                    }
                    result = left_operand / right_operand;
                    snprintf(action_msg, sizeof(action_msg),
                             "Popped operands %d and %d, evaluated %d / %d = %d, pushed result "
                             "back onto stack",
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

            i++;
            sleep_seconds(1);
        }

        if (isEmpty(operands))
        {
            printf("\n[Error] Invalid or empty expression provided.\n");
            destroyStack(operands);
            continue;
        }

        int final_result = pop(operands);

        if (!isEmpty(operands))
        {
            printf("\n[Error] Malformed expression: Too many operands.\n");
            destroyStack(operands);
            continue;
        }

        destroyStack(operands);
        printf("\n===================================\n");
        printf("Final Evaluated Result : %d\n", final_result);
        printf("===================================\n\n");
    }

    return;
}