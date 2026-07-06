#include "data_structures.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

extern void show_help(void);

// Check if input is a help command
static int is_help_input(const char* buffer)
{
    if (!buffer) return 0;
    char lower[32];
    int i = 0;
    while (buffer[i] && i < 31 && !isspace(buffer[i]))
    {
        lower[i] = (char)tolower((unsigned char)buffer[i]);
        i++;
    }
    lower[i] = '\0';
    return strcmp(lower, "help") == 0;
}

// Check if input is exit signal
static int is_exit_input(const char* buffer)
{
    if (!buffer) return 0;
    return strcmp(buffer, "-1") == 0 || strcmp(buffer, "X") == 0;
}

// this function return 1 on successful insertion, 0 on failure (invalid input or EOF or number out
// of range)
//  and -111 on exit signal ie when user gives input as '-1'

int safe_input_int(int* input, const char* prompt, int min_val, int max_val)
{

    int c;
    char buffer[64];

    if (prompt)
    {
        printf("%s", prompt);
        fflush(stdout);
    }

    if (scanf("%63s", buffer) != 1)
    {
        goto clear_buffer;
    }

    // Check for help command
    if (is_help_input(buffer))
    {
        while ((c = getchar()) != '\n' && c != EOF);
        show_help();
        return 0; // Retry the input
    }

    // Check for exit signal
    if (is_exit_input(buffer))
    {
        *input = -1;
        while ((c = getchar()) != '\n' && c != EOF);
        return -111;
    }

    int value;

    if (sscanf(buffer, "%d", &value) != 1)
    {
        printf("That's not a number. Please try again: \n");
        goto clear_buffer;
    }

    if (value == -1)
    {
        *input = -1;
        return -111;
    }
    if (value < min_val || value > max_val)
    {
        printf("only enter values between %d and %d.\n", min_val, max_val);
        printf("press 'ENTER' to try again :- ");
        goto clear_buffer;
    }

    *input = value;
    while ((c = getchar()) != '\n' && c != EOF);
    return 1;

clear_buffer:
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    if (c == EOF)
    {
        clearerr(stdin);
        fflush(stdin);
        printf("input ended unexpectedly\n");
        return 0;
    }
    return 0;
}