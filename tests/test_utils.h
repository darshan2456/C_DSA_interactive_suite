#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdarg.h>
#include <stdio.h>

static char g_printf_buf[65536];
static int g_printf_len = 0;

static void reset_printf_buf(void)
{
    g_printf_buf[0] = '\0';
    g_printf_len = 0;
}

static int mock_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    g_printf_len += vsnprintf(g_printf_buf + g_printf_len, sizeof(g_printf_buf) - g_printf_len, format, args);
    va_end(args);
    return 0;
}

#endif // TEST_UTILS_H
