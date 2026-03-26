#include "safe_input.h"
#include "stack.h"
#include <stdio.h>

int check_parantheses(char* s)
{
    int i = 0;
    stack* parantheses = createStack();
    while (s[i] != '\0')
    {
        if (s[i] == '(' || s[i] == '[' || s[i] == '{')
        {
            push(parantheses, s[i]);
        }
        else if (s[i] == ')' && pop(parantheses) != '(')
        {
            destroyStack(parantheses);
            return 0;
        }
        else if (s[i] == ']' && pop(parantheses) != '[')
        {
            destroyStack(parantheses);
            return 0;
        }
        else if (s[i] == '}' && pop(parantheses) != '{')
        {
            destroyStack(parantheses);
            return 0;
        }
        i++;
    }

    int result = isEmpty(parantheses);
    destroyStack(parantheses);
    return result;
}

void parantheses_checker_demo(void)
{
    char parantheses_expression[50] = {0};

    printf("\nenter an expression with parantheses: ");

    scanf("%s", parantheses_expression);

    int result = check_parantheses(parantheses_expression);

    if (result)
        printf("valid expression balanced parantheses\n");
    else
        printf("invalid expression\n");
}