#include "help.h" // Include our help module header
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

// this function return 1 on successful insertion, 0 on failure (invalid input or EOF or number out
// of range) and -111 on exit signal ie when user gives input as '-1'
int safe_input_int(int* input, const char* prompt, int min_val, int max_val)
{
    int c;
    char buffer[100]; // Buffer to read raw user input as a string first

    while (1)
    {
        if (prompt)
        {
            printf("%s", prompt);
            fflush(stdout);
        }

        // Read input safely and detect EOF
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            if (feof(stdin))
            {
                clearerr(stdin);
            }
            printf("input ended unexpectedly\n");
            return 0;
        }

        // Remove trailing newline, if present
        trim_newline(buffer);

        // 1. Intercept "help" command
        if (strcmp(buffer, "help") == 0)
        {
            launch_help_page(); // Summon the manual
            continue;           // Reprint the exact same prompt seamlessly!
        }

        int value;
        char extra_char;

        // 2. Parse the integer from our string buffer
        // Checks if it's a number, and ensures no trailing junk characters exist
        if (sscanf(buffer, "%d%c", &value, &extra_char) == 1)
        {
            // Successfully read an integer and nothing else (except maybe exit check)
        }
        else
        {
            // If they just typed -1, but it wasn't parsed as 1 item (shouldn't happen, but let's
            // check)
            if (strcmp(buffer, "-1") == 0)
            {
                return -111; // Return exit signal
            }
            printf("Invalid input. Only enter numerical values.\n");
            printf("press 'ENTER' to try again :- ");
            // Clear rest of the line
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            continue;
        }

        if (value == -1)
        {
            return -111; // return -111 to caller, signaling exit
        }

        if (value < min_val || value > max_val)
        {
            printf("only enter values between %d and %d.\n", min_val, max_val);
            printf("press 'ENTER' to try again :- ");
            // Pause so they can read the error
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            continue;
        }

        *input = value;
        return 1; // Successful insertion
    }
}

#include "io_utility.c"
