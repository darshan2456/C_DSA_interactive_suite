#include "safe_input.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>

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

int get_validated_input_parantheses(char* buff, size_t size, const char* prompt)
{
    if (prompt)
    {
        printf("%s", prompt);
        fflush(stdout);
    }
    if (!fgets(buff, size, stdin))
    {
        printf("\ninput ended unexpectedly");
        return 0;
    }
    buff[strcspn(buff, "\n")] = '\0';
    if (buff[0] == 'X' && buff[1] == '\0')
    {
        return INPUT_EXIT_SIGNAL;
    }
    size_t i = 0;
    while (buff[i] != '\0')
    {
        char c = buff[i];
        if (c != '(' && c != ')' && c != '{' && c != '}' && c != '[' && c != ']')
        {
            printf("\ninvalid character: %c\n", c);
            printf("\nonly '(', ')', '{', '}', '[', ']' are allowed.\n");
            return 0;
        }
        i++;
    }
    return 1;
}

void parantheses_checker_demo(void)
{
    char parantheses_expression[50];
    while (1)
    {
        int status = get_validated_input_parantheses(
            parantheses_expression, sizeof(parantheses_expression),
            "\nenter an expression with parantheses, enter 'X' to exit: ");

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting parantheses checker demo.....\n");
            return;
        }

        if (status != 1)
            continue;

        int result = check_parantheses(parantheses_expression);

        if (result)
            printf("valid expression balanced parantheses\n");
        else
            printf("invalid expression\n");
    }
}