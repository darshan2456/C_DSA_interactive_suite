#ifndef EXPORT_H
#define EXPORT_H

#include "dll.h"
#include <stdio.h>

typedef enum
{
    EXPORT_FORMAT_TEXT,
    EXPORT_FORMAT_CSV,
    EXPORT_FORMAT_JSON
} ExportFormat;

// Pre-defined formatter helpers
void write_data_int(FILE* fp, const void* data);
void write_data_string(FILE* fp, const void* data);

// Generic export function for Doubly Linked List
int dll_export(const doubly_ll_Node* head, const char* filepath, ExportFormat format,
               void (*write_node_data)(FILE* fp, const void* data));

#endif // EXPORT_H
