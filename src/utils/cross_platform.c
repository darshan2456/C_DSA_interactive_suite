#include "cross_platform.h"

void sleep_seconds(float seconds)
{
#ifdef _WIN32
    Sleep(seconds * 1000);
#else
    usleep(seconds * 1000000);
#endif
}
