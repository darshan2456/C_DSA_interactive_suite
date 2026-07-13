#include "cross_platform_timer.h"

#ifdef _WIN32
#include <windows.h>
#else
#define _DEFAULT_SOURCE
#include <unistd.h>
#endif

#include <stdlib.h>

void sleep_seconds(float seconds)
{
    if (getenv("DSA_TEST_MODE") != NULL)
    {
        return;
    }
#ifdef _WIN32
    Sleep(seconds * 1000);
#else
    usleep(seconds * 1000000);
#endif
}