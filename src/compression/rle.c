#include "compression.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rle_encode(const char* input, char* output, int len, int out_max)
{
    if (input == NULL || output == NULL || len < 0 || out_max <= 0)
    {
        return -1;
    }

    if (len == 0)
    {
        output[0] = '\0';
        return 0;
    }

    int out_idx = 0;
    int i = 0;

    while (i < len)
    {
        char ch = input[i];
        int count = 1;

        while (i + 1 < len && input[i + 1] == ch)
        {
            count++;
            i++;
        }
        i++;

        char temp[32];
        int written = snprintf(temp, sizeof(temp), "%c%d", ch, count);
        if (written < 0 || written >= (int)sizeof(temp))
        {
            return -1;
        }

        if (out_idx + written >= out_max)
        {
            return -1;
        }

        memcpy(&output[out_idx], temp, written);
        out_idx += written;
    }

    output[out_idx] = '\0';
    return out_idx;
}

int rle_decode(const char* input, int len, char* output, int out_max)
{
    if (input == NULL || output == NULL || len < 0 || out_max <= 0)
    {
        return -1;
    }

    if (len == 0)
    {
        output[0] = '\0';
        return 0;
    }

    int out_idx = 0;
    int i = 0;

    while (i < len)
    {
        char ch = input[i];
        i++;

        if (i >= len || !isdigit((unsigned char)input[i]))
        {
            return -1;
        }

        int count = 0;
        while (i < len && isdigit((unsigned char)input[i]))
        {
            count = count * 10 + (input[i] - '0');
            i++;
        }

        for (int j = 0; j < count; j++)
        {
            if (out_idx >= out_max - 1)
            {
                return -1;
            }
            output[out_idx++] = ch;
        }
    }

    output[out_idx] = '\0';
    return out_idx;
}
