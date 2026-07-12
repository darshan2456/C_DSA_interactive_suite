#include "io_utility.h"
#include <stdio.h>
#include <string.h>

void trim_newline(char* str)
{
    if (str == NULL)
        return;
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
    {
        str[--len] = '\0';
    }
}

void press_enter_to_continue(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
