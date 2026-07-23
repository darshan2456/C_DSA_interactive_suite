#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "expression.h"
#include "safe_input.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
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

int infix_to_prefix_convert(const char* infix_expr, char* prefix_expr, size_t max_size)
{
    if (infix_expr == NULL || infix_expr[0] == '\0')
    {
        return EXPR_ERROR_EMPTY;
    }

    char reversed_expr[50];
    char postfix_expr[100];
    stack* operators = createStack();

    int step = 1;
    postfix_expr[0] = '\0';
    prefix_expr[0] = '\0';

    if (strlen(infix_expr) >= sizeof(reversed_expr))
    {
        destroyStack(operators, NULL);
        return EXPR_ERROR_MALFORMED;
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
            if ((size_t)pf_idx + 1 >= sizeof(postfix_expr))
            {
                destroyStack(operators, NULL);
                return EXPR_ERROR_MALFORMED;
            }
            postfix_expr[pf_idx++] = ch;
            postfix_expr[pf_idx] = '\0';

            snprintf(opbuf, sizeof(opbuf), "Added operand '%c' to postfix expression", ch);
            action_msg = opbuf;
        }
        else if (ch == '(')
        {
            push(operators, (void*)(intptr_t)ch);
            action_msg = "Pushed '(' onto stack";
        }
        else if (ch == ')')
        {
            while (!isEmpty(operators) && (char)(intptr_t)peek(operators) != '(')
            {
                if ((size_t)pf_idx + 1 >= sizeof(postfix_expr))
                {
                    destroyStack(operators, NULL);
                    return EXPR_ERROR_MALFORMED;
                }
                postfix_expr[pf_idx++] = (char)(intptr_t)pop(operators);
                postfix_expr[pf_idx] = '\0';
            }

            if (isEmpty(operators))
            {
                destroyStack(operators, NULL);
                return EXPR_ERROR_UNMATCHED_PARENTHESES;
            }
            pop(operators);
            action_msg = "Popped operators until matching '('";
        }
        else if (isOperator(ch))
        {
            while (!isEmpty(operators) && (char)(intptr_t)peek(operators) != '(' &&
                   precedence((char)(intptr_t)peek(operators)) > precedence(ch))
            {
                if ((size_t)pf_idx + 1 >= sizeof(postfix_expr))
                {
                    destroyStack(operators, NULL);
                    return EXPR_ERROR_MALFORMED;
                }
                postfix_expr[pf_idx++] = (char)(intptr_t)pop(operators);
                postfix_expr[pf_idx] = '\0';
            }

            push(operators, (void*)(intptr_t)ch);
            snprintf(opbuf, sizeof(opbuf), "Processed operator '%c'", ch);
            action_msg = opbuf;
        }
        else
        {
            destroyStack(operators, NULL);
            return EXPR_ERROR_INVALID_CHAR;
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

        char op = (char)(intptr_t)pop(operators);
        if (op == '(')
        {
            destroyStack(operators, NULL);
            return EXPR_ERROR_UNMATCHED_PARENTHESES;
        }

        if ((size_t)pf_idx + 1 >= sizeof(postfix_expr))
        {
            destroyStack(operators, NULL);
            return EXPR_ERROR_MALFORMED;
        }
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

    if (strlen(postfix_expr) >= max_size)
    {
        destroyStack(operators, NULL);
        return EXPR_ERROR_MALFORMED;
    }

    strcpy(prefix_expr, postfix_expr);
    reverse_string(prefix_expr);

    destroyStack(operators, NULL);
    return EXPR_SUCCESS;
}
