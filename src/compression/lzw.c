#include "compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LZW_MAX_CODES 4096

typedef struct
{
    char str[512];
} LzwDictEntry;

static int find_dict_entry(const LzwDictEntry* dict, int size, const char* str)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(dict[i].str, str) == 0)
        {
            return i;
        }
    }
    return -1;
}

static void reset_dict(LzwDictEntry* dict, int* size)
{
    *size = 256;
    for (int i = 0; i < 256; i++)
    {
        dict[i].str[0] = (char)i;
        dict[i].str[1] = '\0';
    }
}

int lzw_encode(const char* input, int* output, int out_max)
{
    if (input == NULL || output == NULL || out_max <= 0)
    {
        return -1;
    }

    int in_len = strlen(input);
    if (in_len == 0)
    {
        return 0;
    }

    LzwDictEntry* dict = malloc(sizeof(LzwDictEntry) * LZW_MAX_CODES);
    if (dict == NULL)
    {
        return -1;
    }

    int dict_size = 256;
    reset_dict(dict, &dict_size);

    int out_idx = 0;
    char p[512] = "";
    int p_len = 0;

    for (int i = 0; i < in_len; i++)
    {
        char c = input[i];

        char pc[512];
        if (p_len + 1 >= (int)sizeof(pc))
        {
            free(dict);
            return -1;
        }

        memcpy(pc, p, p_len);
        pc[p_len] = c;
        pc[p_len + 1] = '\0';

        int pc_idx = find_dict_entry(dict, dict_size, pc);
        if (pc_idx != -1)
        {
            strcpy(p, pc);
            p_len++;
        }
        else
        {
            int p_idx = find_dict_entry(dict, dict_size, p);
            if (p_idx == -1)
            {
                free(dict);
                return -1;
            }

            if (out_idx >= out_max)
            {
                free(dict);
                return -1;
            }
            output[out_idx++] = p_idx;

            if (dict_size < LZW_MAX_CODES)
            {
                strcpy(dict[dict_size].str, pc);
                dict_size++;
            }

            if (dict_size >= LZW_MAX_CODES)
            {
                reset_dict(dict, &dict_size);
            }

            p[0] = c;
            p[1] = '\0';
            p_len = 1;
        }
    }

    if (p_len > 0)
    {
        int p_idx = find_dict_entry(dict, dict_size, p);
        if (p_idx == -1 || out_idx >= out_max)
        {
            free(dict);
            return -1;
        }
        output[out_idx++] = p_idx;
    }

    free(dict);
    return out_idx;
}

int lzw_decode(const int* input, int in_len, char* output, int out_max)
{
    if (input == NULL || output == NULL || out_max <= 0)
    {
        return -1;
    }
    if (in_len == 0)
    {
        output[0] = '\0';
        return 0;
    }

    LzwDictEntry* dict = malloc(sizeof(LzwDictEntry) * LZW_MAX_CODES);
    if (dict == NULL)
    {
        return -1;
    }

    int dict_size = 256;
    reset_dict(dict, &dict_size);

    int out_idx = 0;

    int old_code = input[0];
    if (old_code < 0 || old_code >= dict_size)
    {
        free(dict);
        return -1;
    }

    const char* old_str = dict[old_code].str;
    int old_len = strlen(old_str);
    if (out_idx + old_len >= out_max)
    {
        free(dict);
        return -1;
    }
    strcpy(output + out_idx, old_str);
    out_idx += old_len;

    for (int i = 1; i < in_len; i++)
    {
        int new_code = input[i];
        char string[514];
        int string_len = 0;

        if (new_code >= 0 && new_code < dict_size)
        {
            strcpy(string, dict[new_code].str);
            string_len = strlen(string);
        }
        else if (new_code == dict_size)
        {
            strcpy(string, dict[old_code].str);
            int len = strlen(string);
            if (len + 1 < (int)sizeof(string))
            {
                string[len] = string[0];
                string[len + 1] = '\0';
                string_len = len + 1;
            }
            else
            {
                free(dict);
                return -1;
            }
        }
        else
        {
            free(dict);
            return -1;
        }

        if (out_idx + string_len >= out_max)
        {
            free(dict);
            return -1;
        }
        strcpy(output + out_idx, string);
        out_idx += string_len;

        if (dict_size < LZW_MAX_CODES)
        {
            char new_entry[514];
            strcpy(new_entry, dict[old_code].str);
            int len = strlen(new_entry);
            if (len + 1 < 512)
            {
                new_entry[len] = string[0];
                new_entry[len + 1] = '\0';
                strcpy(dict[dict_size].str, new_entry);
                dict_size++;
            }
        }

        if (dict_size >= LZW_MAX_CODES)
        {
            reset_dict(dict, &dict_size);
        }

        old_code = new_code;
    }

    output[out_idx] = '\0';
    free(dict);
    return out_idx;
}
