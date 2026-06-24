#include "mock_printf.h"
#include <stdio.h>

char g_printf_buf[65536];
int g_printf_len = 0;

void reset_printf_buf(void)
{
    g_printf_buf[0] = '\0';
    g_printf_len = 0;
}

int mock_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    g_printf_len += vsnprintf(g_printf_buf + g_printf_len, sizeof(g_printf_buf) - g_printf_len, format, args);
    va_end(args);
    return 0;
}
