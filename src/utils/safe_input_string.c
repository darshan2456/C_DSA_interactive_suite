#include "safe_input.h"
#include <stdio.h>
#include <string.h>

int safe_input_string(char* buffer, size_t size, const char* prompt)
{
    while (1)
    {
        if (prompt)
        {
            printf("%s", prompt);
            fflush(stdout);
        }

        if (!fgets(buffer, size, stdin))
        {
            printf("\ninput ended unexpectedly\n");
            return INPUT_EXIT_SIGNAL;
        }

        // Clean newline or flush excess characters
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
            len--;
        }
        else if (len == size - 1)
        {
            // Input was truncated because it exceeded buffer size
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
        }

        // Check empty
        if (len == 0)
        {
            printf("Invalid input. Empty string. Please try again.\n");
            continue;
        }

        // Check if there is any space character in the string
        int has_space = 0;
        for (size_t i = 0; i < len; i++)
        {
            if (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\r')
            {
                has_space = 1;
                break;
            }
        }
        if (has_space)
        {
            printf("Invalid input. No spaces allowed. Please try again.\n");
            continue;
        }

        if (strcmp(buffer, "X") == 0)
        {
            return INPUT_EXIT_SIGNAL;
        }

        return 1;
    }
}
