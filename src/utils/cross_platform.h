#ifndef CROSS_PLATFORM_H
#define CROSS_PLATFORM_H

#ifdef _WIN32
    #include <windows.h>
    #define clear_screen() system("cls")
#else
    #define _DEFAULT_SOURCE
    #include <unistd.h>
    #define clear_screen() printf("\033[H\033[J")
#endif

void sleep_seconds(float seconds);

#endif