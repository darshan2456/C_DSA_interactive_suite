#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "expression.h"
#include "safe_input.h"
#include <ctype.h>
#include <stdio.h>

#include <string.h>

static void reverse_string(char* str)
{
    int left = 0;
    int right = (int)strlen(str) - 1;

    while (left < right)
    {
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;

        left++;
        right--;
    }
}

static void swap_parentheses(char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '(')
        {
            str[i] = ')';
        }
        else if (str[i] == ')')
        {
            str[i] = '(';
        }
    }
}

void infix_to_prefix_demo(void)
{
    char infix_expr[50];
    char reversed_expr[50];
    char postfix_expr[50];
    char prefix_expr[50];

    while (1)
    {
        stack* operators = createStack();

        int step = 1;

        postfix_expr[0] = '\0';
        prefix_expr[0] = '\0';

        printf("\nInfix to Prefix Demo\n");

        int infix_expr_status =
            validate_infix_expr(infix_expr, sizeof(infix_expr),
                                "\nEnter a mathematical expression without whitespaces "
                                "(Enter 'X' to exit): ");

        if (infix_expr_status == INPUT_EXIT_SIGNAL)
        {
            destroyStack(operators);
            printf("\nExiting infix to prefix demo...\n");
            return;
        }

        if (infix_expr_status != 1)
        {
            destroyStack(operators);
            continue;
        }

        strcpy(reversed_expr, infix_expr);

        reverse_string(reversed_expr);
        swap_parentheses(reversed_expr);

        dynamic_sleep();

        int i = 0;
        int pf_idx = 0;

        while (reversed_expr[i] != '\0')
        {
            if (!is_instant())
            {
                clear_screen();
            }

            char ch = reversed_expr[i];
            const char* action_msg = NULL;
            char opbuf[100];

            if (isalnum((unsigned char)ch))
            {
                postfix_expr[pf_idx++] = ch;
                postfix_expr[pf_idx] = '\0';

                snprintf(opbuf, sizeof(opbuf), "Added operand '%c' to postfix expression", ch);

                action_msg = opbuf;
            }
            else if (ch == '(')
            {
                push(operators, ch);
                action_msg = "Pushed '(' onto stack";
            }
            else if (ch == ')')
            {
                while (!isEmpty(operators) && peek(operators) != '(')
                {
                    postfix_expr[pf_idx++] = pop(operators);
                    postfix_expr[pf_idx] = '\0';
                }

                if (!isEmpty(operators))
                {
                    pop(operators);
                }

                action_msg = "Popped operators until matching '('";
            }
            else if (isOperator(ch))
            {
                while (!isEmpty(operators) && peek(operators) != '(' &&
                       precedence(peek(operators)) > precedence(ch))
                {
                    postfix_expr[pf_idx++] = pop(operators);
                    postfix_expr[pf_idx] = '\0';
                }

                push(operators, ch);

                snprintf(opbuf, sizeof(opbuf), "Processed operator '%c'", ch);

                action_msg = opbuf;
            }

            printf("\nStep %d\n", step++);
            printf("Character : %c\n", ch);
            printf("Action    : %s\n", action_msg);
            printf("Postfix   : %s\n", postfix_expr);
            printf("Stack     : ");

            printStack(operators);

            printf("----------------------------------\n");

            i++;

            dynamic_sleep();
        }

        while (!isEmpty(operators))
        {
            if (!is_instant())
            {
                clear_screen();
            }

            char op = pop(operators);

            postfix_expr[pf_idx++] = op;
            postfix_expr[pf_idx] = '\0';

            printf("\nStep %d\n", step++);
            printf("Character : End\n");
            printf("Action    : Popped remaining operator '%c' from stack\n", op);
            printf("Postfix   : %s\n", postfix_expr);
            printf("Stack     : ");

            printStack(operators);

            printf("----------------------------------\n");

            dynamic_sleep();
        }

        strcpy(prefix_expr, postfix_expr);
        reverse_string(prefix_expr);

        printf("\n==================================\n");
        printf("Conversion Complete\n");
        printf("Intermediate : %s\n", postfix_expr);
        printf("Final Prefix      : %s\n", prefix_expr);
        printf("==================================\n\n");

        dynamic_sleep();

        destroyStack(operators);
    }
}