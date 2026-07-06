#include "clear_screen.h"
#include "config.h"
#include <stdlib.h>

void clear_screen(void)
{
    if (!is_terminal_interactive())
    {
        return;
    }
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
