#include "export.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static void ensure_parent_dir_exists(const char* filepath)
{
    char temp[512];
    strncpy(temp, filepath, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    char* last_slash = strrchr(temp, '/');

    if (last_slash != NULL)
    {
        *last_slash = '\0';
        if (strlen(temp) > 0)
        {
            struct stat st;
            if (stat(temp, &st) == -1)
            {
                mkdir(temp, 0755);
            }
        }
    }
}

void write_data_int(FILE* fp, const void* data)
{
    if (data != NULL)
    {
        fprintf(fp, "%d", *(const int*)data);
    }
}

void write_data_string(FILE* fp, const void* data)
{
    if (data != NULL)
    {
        fprintf(fp, "%s", (const char*)data);
    }
}

int dll_export(const doubly_ll_Node* head, const char* filepath, ExportFormat format,
               void (*write_node_data)(FILE* fp, const void* data))
{
    if (filepath == NULL || strlen(filepath) == 0 || write_node_data == NULL)
    {
        return 0;
    }

    ensure_parent_dir_exists(filepath);
    FILE* fp = fopen(filepath, "w");
    if (fp == NULL)
    {
        return 0;
    }

    const doubly_ll_Node* curr = head;

    if (format == EXPORT_FORMAT_TEXT)
    {
        while (curr != NULL)
        {
            write_node_data(fp, curr->data);
            fprintf(fp, " -> ");
            curr = curr->next;
        }
        fprintf(fp, "NULL\n");
    }
    else if (format == EXPORT_FORMAT_CSV)
    {
        fprintf(fp, "index,value\n");
        int index = 0;
        while (curr != NULL)
        {
            fprintf(fp, "%d,", index);
            write_node_data(fp, curr->data);
            fprintf(fp, "\n");
            index++;
            curr = curr->next;
        }
    }
    else if (format == EXPORT_FORMAT_JSON)
    {
        fprintf(fp, "[\n");
        while (curr != NULL)
        {
            fprintf(fp, "  ");
            write_node_data(fp, curr->data);
            if (curr->next != NULL)
            {
                fprintf(fp, ",\n");
            }
            else
            {
                fprintf(fp, "\n");
            }
            curr = curr->next;
        }
        fprintf(fp, "]\n");
    }

    fclose(fp);
    return 1;
}
