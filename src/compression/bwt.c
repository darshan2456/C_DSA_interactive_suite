#include "compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    const char* str;
    int len;
    int idx;
} BwtRotation;

static int compare_rotations(const void* a, const void* b)
{
    const BwtRotation* ra = (const BwtRotation*)a;
    const BwtRotation* rb = (const BwtRotation*)b;

    for (int i = 0; i < ra->len; i++)
    {
        char ca = ra->str[(ra->idx + i) % ra->len];
        char cb = rb->str[(rb->idx + i) % rb->len];
        if (ca != cb)
        {
            return (unsigned char)ca - (unsigned char)cb;
        }
    }
    return 0;
}

int bwt_forward(const char* input, char* output, int* primary_index)
{
    if (input == NULL || output == NULL || primary_index == NULL)
    {
        return -1;
    }

    int len = strlen(input);
    if (len == 0)
    {
        output[0] = '\0';
        *primary_index = 0;
        return 0;
    }

    BwtRotation* rotations = malloc(sizeof(BwtRotation) * len);
    if (rotations == NULL)
    {
        return -1;
    }

    for (int i = 0; i < len; i++)
    {
        rotations[i].str = input;
        rotations[i].len = len;
        rotations[i].idx = i;
    }

    qsort(rotations, len, sizeof(BwtRotation), compare_rotations);

    *primary_index = -1;
    for (int i = 0; i < len; i++)
    {
        int last_char_idx = (rotations[i].idx + len - 1) % len;
        output[i] = input[last_char_idx];

        if (rotations[i].idx == 0)
        {
            *primary_index = i;
        }
    }
    output[len] = '\0';

    free(rotations);
    return len;
}

int bwt_inverse(const char* input, int primary_index, char* output)
{
    if (input == NULL || output == NULL)
    {
        return -1;
    }

    int len = strlen(input);
    if (len == 0)
    {
        output[0] = '\0';
        return 0;
    }

    int* count = calloc(256, sizeof(int));
    int* lf = malloc(sizeof(int) * len);
    int* l_count = malloc(sizeof(int) * len);
    int* start = calloc(256, sizeof(int));

    if (count == NULL || lf == NULL || l_count == NULL || start == NULL)
    {
        free(count);
        free(lf);
        free(l_count);
        free(start);
        return -1;
    }

    for (int i = 0; i < len; i++)
    {
        unsigned char ch = (unsigned char)input[i];
        l_count[i] = count[ch];
        count[ch]++;
    }

    int accum = 0;
    for (int i = 0; i < 256; i++)
    {
        start[i] = accum;
        accum += count[i];
    }

    for (int i = 0; i < len; i++)
    {
        unsigned char ch = (unsigned char)input[i];
        lf[i] = start[ch] + l_count[i];
    }

    int curr = primary_index;
    for (int i = len - 1; i >= 0; i--)
    {
        if (curr < 0 || curr >= len)
        {
            free(count);
            free(lf);
            free(l_count);
            free(start);
            return -1;
        }
        output[i] = input[curr];
        curr = lf[curr];
    }
    output[len] = '\0';

    free(count);
    free(lf);
    free(l_count);
    free(start);
    return len;
}

int mtf_encode(const char* input, char* output, int len)
{
    if (input == NULL || output == NULL || len < 0)
    {
        return -1;
    }

    unsigned char list[256];
    for (int i = 0; i < 256; i++)
    {
        list[i] = (unsigned char)i;
    }

    for (int i = 0; i < len; i++)
    {
        unsigned char ch = (unsigned char)input[i];
        int idx = 0;
        while (idx < 256 && list[idx] != ch)
        {
            idx++;
        }

        output[i] = (char)idx;

        for (int j = idx; j > 0; j--)
        {
            list[j] = list[j - 1];
        }
        list[0] = ch;
    }
    output[len] = '\0';
    return len;
}

int mtf_decode(const char* input, char* output, int len)
{
    if (input == NULL || output == NULL || len < 0)
    {
        return -1;
    }

    unsigned char list[256];
    for (int i = 0; i < 256; i++)
    {
        list[i] = (unsigned char)i;
    }

    for (int i = 0; i < len; i++)
    {
        int idx = (unsigned char)input[i];
        unsigned char ch = list[idx];
        output[i] = (char)ch;

        for (int j = idx; j > 0; j--)
        {
            list[j] = list[j - 1];
        }
        list[0] = ch;
    }
    output[len] = '\0';
    return len;
}
