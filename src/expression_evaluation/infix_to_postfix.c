#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "expression.h"
#include "safe_input.h"
#include <ctype.h>
#include <stdio.h>

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

ExpressionResult infix_to_postfix_convert(const char* infix_expr, char* postfix_expr,
                                          size_t max_size)
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
                destroyStack(operators);
                return EXPR_ERROR_MALFORMED;
            }
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
            action_msg = "Popped operators until matching '(' was found";

            while (!isEmpty(operators) && peek(operators) != '(')
            {
                op = pop(operators);
                if ((size_t)pf_idx + 1 >= max_size)
                {
                    destroyStack(operators);
                    return EXPR_ERROR_MALFORMED;
                }
                postfix_expr[pf_idx++] = op;
                postfix_expr[pf_idx] = '\0';
            }

            if (isEmpty(operators))
            {
                destroyStack(operators);
                return EXPR_ERROR_UNMATCHED_PARENTHESES;
            }
            pop(operators); // Remove '('
        }
        else if (isOperator(ch))
        {
            if (isEmpty(operators))
            {
                push(operators, ch);
                snprintf(opbuf, sizeof(opbuf), "Pushed operator '%c' onto stack", ch);
                action_msg = opbuf;
            }
            else if (precedence(ch) > precedence(peek(operators)))
            {
                push(operators, ch);
                snprintf(opbuf, sizeof(opbuf), "Pushed operator '%c' onto stack", ch);
                action_msg = opbuf;
            }
            else if (precedence(ch) <= precedence(peek(operators)))
            {
                int prec_lower = precedence(ch);

                while (!isEmpty(operators) && peek(operators) != '(' &&
                       precedence(peek(operators)) >= prec_lower)
                {
                    op = pop(operators);
                    if ((size_t)pf_idx + 1 >= max_size)
                    {
                        destroyStack(operators);
                        return EXPR_ERROR_MALFORMED;
                    }
                    postfix_expr[pf_idx++] = op;
                    postfix_expr[pf_idx] = '\0';
                }

                push(operators, ch);
                snprintf(opbuf, sizeof(opbuf),
                         "Popped operators with higher/equal precedence, then pushed '%c'", ch);
                action_msg = opbuf;
            }
        }
        else
        {
            destroyStack(operators);
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
        char op = pop(operators);
        if (op == '(')
        {
            destroyStack(operators);
            return EXPR_ERROR_UNMATCHED_PARENTHESES;
        }

        if ((size_t)pf_idx + 1 >= max_size)
        {
            destroyStack(operators);
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

    destroyStack(operators);
    return EXPR_SUCCESS;
}

void infix_to_postfix_demo(void)
{
    char infix_expr[50];
    char postfix_expr[100];

    while (1)
    {
        printf("\nInfix to Postfix Demo\n");

        int infix_expr_status =
            validate_infix_expr(infix_expr, sizeof(infix_expr),
                                "\nEnter a mathematical expression without whitespaces "
                                "(Enter 'X' to exit): ");

        if (infix_expr_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting infix to postfix demo...\n");
            return;
        }

        if (infix_expr_status != 1)
        {
            continue;
        }

        ExpressionResult result =
            infix_to_postfix_convert(infix_expr, postfix_expr, sizeof(postfix_expr));

        if (result == EXPR_SUCCESS)
        {
            printf("\n==================================\n");
            printf("Conversion Complete\n");
            printf("Final Postfix Expression : %s\n", postfix_expr);
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