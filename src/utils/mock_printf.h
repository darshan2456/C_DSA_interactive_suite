#ifndef MOCK_PRINTF_H
#define MOCK_PRINTF_H

#include <stdarg.h>

extern char g_printf_buf[65536];
extern int g_printf_len;

void reset_printf_buf(void);
int mock_printf(const char* format, ...);

#endif // MOCK_PRINTF_H
