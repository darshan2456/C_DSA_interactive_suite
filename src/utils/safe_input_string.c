#include "safe_input.h"
#include <stdio.h>
#include <string.h>

int safe_input_string(char* buffer, const char* prompt)
{
    while (1)
    {
        printf("%s", prompt);
        fflush(stdout);

        if (scanf("%99s", buffer) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("Invalid input. Please try again.\n");
            continue;
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ; // Clear the rest of the line

        if (strcmp(buffer, "X") == 0)
        {
            return INPUT_EXIT_SIGNAL;
        }

        return 1;
    }
}
