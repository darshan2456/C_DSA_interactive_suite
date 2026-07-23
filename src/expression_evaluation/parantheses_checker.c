#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "expression.h"
#include "safe_input.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>

int check_parantheses(char* s)
{
    int i = 0;
    int step = 1;
    stack* parantheses = createStack();
    while (s[i] != '\0')
    {
        if (!is_instant())
        {
            clear_screen();
        }
        printf("\nStep    : %d\n", step++);
        printf("Char    : %c\n", s[i]);
        if (s[i] == '(' || s[i] == '[' || s[i] == '{')
        {
            push(parantheses, (void*)(intptr_t)s[i]);
            printf("Action  : Pushed '%c' onto stack\n", s[i]);
            if (isEmpty(parantheses))
                printf("Stack   : Empty\n");
            else
            {
                printf("Stack   : ");
                printStack(parantheses);
            }
        }
        else
        {
            if (isEmpty(parantheses))
            {
                printf("Action  : No matching opening bracket found for '%c'\n", s[i]);
                printf("Stack   : Empty\n");
                destroyStack(parantheses, NULL);
                return EXPR_ERROR_UNMATCHED_PARENTHESES;
            }
            int top = (int)(intptr_t)pop(parantheses);

            if (s[i] == ')')
            {
                if (top != '(')
                {
                    printf("Action  : Mismatch - expected '(' but found '%c'\n", top);

                    printf("Stack   : ");
                    if (isEmpty(parantheses))
                        printf("Empty\n");
                    else
                        printStack(parantheses);

                    destroyStack(parantheses, NULL);
                    return EXPR_ERROR_UNMATCHED_PARENTHESES;
                }
                printf("Action  : Matched '(' with ')'\n");
            }
            else if (s[i] == ']')
            {
                if (top != '[')
                {
                    printf("Action  : Mismatch - expected '[' but found '%c'\n", top);

                    printf("Stack   : ");
                    if (isEmpty(parantheses))
                        printf("Empty\n");
                    else
                        printStack(parantheses);

                    destroyStack(parantheses, NULL);
                    return EXPR_ERROR_UNMATCHED_PARENTHESES;
                }
                printf("Action  : Matched '[' with ']'\n");
            }
            else if (s[i] == '}')
            {
                if (top != '{')
                {
                    printf("Action  : Mismatch - expected '{' but found '%c'\n", top);

                    printf("Stack   : ");
                    if (isEmpty(parantheses))
                        printf("Empty\n");
                    else
                        printStack(parantheses);

                    destroyStack(parantheses, NULL);
                    return EXPR_ERROR_UNMATCHED_PARENTHESES;
                }
                printf("Action  : Matched '{' with '}'\n");
            }
            if (isEmpty(parantheses))
            {
                printf("Stack   : Empty\n");
            }
            else
            {
                printf("Stack   : ");
                printStack(parantheses);
            }
        }
        printf("-----------------------------------\n");
        i++;
        dynamic_sleep();
    }
    int result = isEmpty(parantheses);
    if (!result)
    {
        printf("\nAction  : Unmatched opening bracket(s) remain in stack\n");

        printf("Stack   : ");
        printStack(parantheses);
    }
    destroyStack(parantheses, NULL);
    return result ? EXPR_SUCCESS : EXPR_ERROR_UNMATCHED_PARENTHESES;
}
