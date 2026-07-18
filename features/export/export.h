#ifndef EXPORT_H
#define EXPORT_H

#include "dll.h"
#include "queue.h"
#include "stack.h"
#include <stdio.h>

typedef enum
{
    EXPORT_FORMAT_TEXT,
    EXPORT_FORMAT_CSV,
    EXPORT_FORMAT_JSON
} ExportFormat;

typedef enum
{
    QUEUE_TYPE_SIMPLE,
    QUEUE_TYPE_CIRCULAR,
    QUEUE_TYPE_DEQUE
} QueueType;

// Pre-defined formatter helpers
void write_data_int(FILE* fp, const void* data);
void write_data_string(FILE* fp, const void* data);

// Generic export function for Doubly Linked List
int dll_export(const doubly_ll_Node* head, const char* filepath, ExportFormat format,
               void (*write_node_data)(FILE* fp, const void* data));

// Generic export function for Stack
int stack_export(const stack* s, const char* filepath, ExportFormat format,
                 void (*write_node_data)(FILE* fp, const void* data));

// Generic export function for Queue (simple, circular, and deque)
int queue_export(const Queue* q, QueueType type, const char* filepath, ExportFormat format,
                 void (*write_node_data)(FILE* fp, const void* data));

#endif // EXPORT_H
