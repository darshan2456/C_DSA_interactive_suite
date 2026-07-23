#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "expression.h"
#include "safe_input.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// rn this program only has support for four operators - +-/* and parantheses. this program doesnt
// support '^ or %' operators maximum expression length is 50 characters

int precedence(char ch)
{ // if precedence returns -1 char is not an operator or a left parantheses
    if (ch == '*' || ch == '/')
        return 2;
    else if (ch == '+' || ch == '-')
        return 1;
    else
        return -1;
}

int isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
        return 1;
    return 0;
}

int infix_to_postfix_convert(const char* infix_expr, char* postfix_expr, size_t max_size)
{
    if (infix_expr == NULL || infix_expr[0] == '\0')
    {
        return EXPR_ERROR_EMPTY;
    }

    stack* operators = createStack();
    int i = 0;
    int pf_idx = 0;
    int step = 1;
    postfix_expr[0] = '\0';

    while (infix_expr[i] != '\0')
    {
        if (!is_instant())
        {
            clear_screen();
        }
        char ch = infix_expr[i];
        const char* action_msg = NULL;
        char op;
        char opbuf[100];

        if (isalnum((unsigned char)ch))
        {
            if ((size_t)pf_idx + 1 >= max_size)
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
            action_msg = "Popped operators until matching '(' was found";

            while (!isEmpty(operators) && (char)(intptr_t)peek(operators) != '(')
            {
                op = (char)(intptr_t)pop(operators);
                if ((size_t)pf_idx + 1 >= max_size)
                {
                    destroyStack(operators, NULL);
                    return EXPR_ERROR_MALFORMED;
                }
                postfix_expr[pf_idx++] = op;
                postfix_expr[pf_idx] = '\0';
            }

            if (isEmpty(operators))
            {
                destroyStack(operators, NULL);
                return EXPR_ERROR_UNMATCHED_PARENTHESES;
            }
            pop(operators); // Remove '('
        }
        else if (isOperator(ch))
        {
            if (isEmpty(operators))
            {
                push(operators, (void*)(intptr_t)ch);
                snprintf(opbuf, sizeof(opbuf), "Pushed operator '%c' onto stack", ch);
                action_msg = opbuf;
            }
            else if (precedence(ch) > precedence((char)(intptr_t)peek(operators)))
            {
                push(operators, (void*)(intptr_t)ch);
                snprintf(opbuf, sizeof(opbuf), "Pushed operator '%c' onto stack", ch);
                action_msg = opbuf;
            }
            else if (precedence(ch) <= precedence((char)(intptr_t)peek(operators)))
            {
                int prec_lower = precedence(ch);

                while (!isEmpty(operators) && (char)(intptr_t)peek(operators) != '(' &&
                       precedence((char)(intptr_t)peek(operators)) >= prec_lower)
                {
                    op = (char)(intptr_t)pop(operators);
                    if ((size_t)pf_idx + 1 >= max_size)
                    {
                        destroyStack(operators, NULL);
                        return EXPR_ERROR_MALFORMED;
                    }
                    postfix_expr[pf_idx++] = op;
                    postfix_expr[pf_idx] = '\0';
                }

                push(operators, (void*)(intptr_t)ch);
                snprintf(opbuf, sizeof(opbuf),
                         "Popped operators with higher/equal precedence, then pushed '%c'", ch);
                action_msg = opbuf;
            }
        }
        else
        {
            destroyStack(operators, NULL);
            return EXPR_ERROR_INVALID_CHAR;
        }

        printf("\nStep %d\n", step++);
        printf("Character : %c\n", ch);
        printf("Action    : %s\n", action_msg ? action_msg : "(none)");
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

        if ((size_t)pf_idx + 1 >= max_size)
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
    }

    destroyStack(operators, NULL);
    return EXPR_SUCCESS;
}
