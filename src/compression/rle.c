#include "compression.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rle_encode(const char* input, char* output, int out_max)
{
    if (input == NULL || output == NULL || out_max <= 0)
    {
        return -1;
    }

    int in_len = strlen(input);
    if (in_len == 0)
    {
        output[0] = '\0';
        return 0;
    }

    int out_idx = 0;
    int i = 0;

    while (i < in_len)
    {
        char ch = input[i];
        int count = 1;

        while (i + 1 < in_len && input[i + 1] == ch)
        {
            count++;
            i++;
        }
        i++;

        // Format run: character + count
        char temp[32];
        int written = snprintf(temp, sizeof(temp), "%c%d", ch, count);
        if (written < 0 || written >= (int)sizeof(temp))
        {
            return -1;
        }

        if (out_idx + written >= out_max)
        {
            return -1; // Buffer overflow
        }

        strcpy(&output[out_idx], temp);
        out_idx += written;
    }

    output[out_idx] = '\0';
    return out_idx;
}

int rle_decode(const char* input, char* output, int out_max)
{
    if (input == NULL || output == NULL || out_max <= 0)
    {
        return -1;
    }

    int in_len = strlen(input);
    if (in_len == 0)
    {
        output[0] = '\0';
        return 0;
    }

    int out_idx = 0;
    int i = 0;

    while (i < in_len)
    {
        char ch = input[i];
        i++;

        // Expecting digits for count
        if (i >= in_len || !isdigit((unsigned char)input[i]))
        {
            return -1; // Invalid RLE format
        }

        int count = 0;
        while (i < in_len && isdigit((unsigned char)input[i]))
        {
            count = count * 10 + (input[i] - '0');
            i++;
        }

        // Repeat the character count times
        for (int j = 0; j < count; j++)
        {
            if (out_idx >= out_max - 1)
            {
                return -1; // Buffer overflow
            }
            output[out_idx++] = ch;
        }
    }

    output[out_idx] = '\0';
    return out_idx;
}
