#include "help.h" // Include our help module header
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
        buffer[strcspn(buffer, "\n")] = '\0';

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
        int parsed_items = sscanf(buffer, "%d%c", &value, &extra_char);

        if (parsed_items < 1)
        {
            printf("That's not a number. Please try again: \n");
            continue;
        }
        if (parsed_items > 1)
        {
            printf("Only a number please (no extra characters). Try again: \n");
            continue;
        }
        if (value == -1)
        {
            *input = -1;
            return -111; // special exit code indicating user entered '-1'
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
