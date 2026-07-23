#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "expression.h"
#include "safe_input.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>

int get_validated_input_parantheses(char* buff, size_t size, const char* prompt)
{
    if (prompt)
    {
        printf("%s", prompt);
        fflush(stdout);
    }
    if (!fgets(buff, size, stdin))
    {
        clearerr(stdin);
        printf("\ninput ended unexpectedly\n");
        return 0;
    }
    trim_newline(buff);

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
        printf("\n===================================\n");
        if (result == EXPR_SUCCESS)
            printf("Result  : Valid Expression\n");
        else
            printf("Result  : Invalid Expression\n");
        printf("===================================\n");
    }
}