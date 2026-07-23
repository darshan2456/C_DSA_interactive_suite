#include "memory_inspector.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void print_hexdump(const void* ptr, size_t size)
{
    if (!ptr || size == 0)
    {
        printf("(NULL pointer or empty memory region)\n");
        return;
    }

    const unsigned char* bytes = (const unsigned char*)ptr;
    printf("\n--- Memory Layout Hexdump (%zu bytes at %p) ---\n", size, ptr);
    printf("  Offset    00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F  |ASCII|\n");
    printf("  --------  -----------------------------------------------  |----------------|\n");

    for (size_t i = 0; i < size; i += 16)
    {
        printf("  %08zX  ", i);

        // Print hex bytes
        for (size_t j = 0; j < 16; j++)
        {
            if (j == 8)
            {
                printf(" ");
            }
            if (i + j < size)
            {
                printf("%02X ", bytes[i + j]);
            }
            else
            {
                printf("   ");
            }
        }

        printf(" |");

        // Print ASCII characters
        for (size_t j = 0; j < 16; j++)
        {
            if (i + j < size)
            {
                unsigned char c = bytes[i + j];
                printf("%c", isprint(c) ? c : '.');
            }
            else
            {
                printf(" ");
            }
        }
        printf("|\n");
    }
    printf("  --------  -----------------------------------------------  |----------------|\n\n");
}

size_t format_hexdump(const void* ptr, size_t size, char* out_buf, size_t buf_size)
{
    if (!out_buf || buf_size == 0)
    {
        return 0;
    }

    if (!ptr || size == 0)
    {
        int written = snprintf(out_buf, buf_size, "(NULL pointer or empty memory region)\n");
        return (written > 0) ? (size_t)written : 0;
    }

    const unsigned char* bytes = (const unsigned char*)ptr;
    size_t offset = 0;

    int written = snprintf(out_buf + offset, buf_size > offset ? buf_size - offset : 0,
                           "Address: %p (%zu bytes)\n", ptr, size);
    if (written > 0)
    {
        offset += (size_t)written;
    }

    for (size_t i = 0; i < size && offset < buf_size; i += 16)
    {
        written =
            snprintf(out_buf + offset, buf_size > offset ? buf_size - offset : 0, "%08zX  ", i);
        if (written > 0)
        {
            offset += (size_t)written;
        }

        for (size_t j = 0; j < 16 && offset < buf_size; j++)
        {
            if (j == 8)
            {
                written =
                    snprintf(out_buf + offset, buf_size > offset ? buf_size - offset : 0, " ");
                if (written > 0)
                    offset += (size_t)written;
            }
            if (i + j < size)
            {
                written = snprintf(out_buf + offset, buf_size > offset ? buf_size - offset : 0,
                                   "%02X ", bytes[i + j]);
            }
            else
            {
                written =
                    snprintf(out_buf + offset, buf_size > offset ? buf_size - offset : 0, "   ");
            }
            if (written > 0)
            {
                offset += (size_t)written;
            }
        }

        written = snprintf(out_buf + offset, buf_size > offset ? buf_size - offset : 0, " |");
        if (written > 0)
        {
            offset += (size_t)written;
        }

        for (size_t j = 0; j < 16 && offset < buf_size; j++)
        {
            if (i + j < size)
            {
                unsigned char c = bytes[i + j];
                written = snprintf(out_buf + offset, buf_size > offset ? buf_size - offset : 0,
                                   "%c", isprint(c) ? c : '.');
            }
            else
            {
                written =
                    snprintf(out_buf + offset, buf_size > offset ? buf_size - offset : 0, " ");
            }
            if (written > 0)
            {
                offset += (size_t)written;
            }
        }

        written = snprintf(out_buf + offset, buf_size > offset ? buf_size - offset : 0, "|\n");
        if (written > 0)
        {
            offset += (size_t)written;
        }
    }

    return offset;
}
