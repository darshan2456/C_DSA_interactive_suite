#include "clear_screen.h"
#include <stdlib.h>

void clear_screen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
