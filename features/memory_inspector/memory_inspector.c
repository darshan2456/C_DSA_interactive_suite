#include "memory_inspector.h"
#include "dll.h"
#include "sll.h"
#include "trees.h"
#include <ctype.h>
#include <stdint.h>
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

void finalize_struct_layout(StructLayout* layout)
{
    if (!layout)
        return;

    size_t payload = 0;
    size_t padding = 0;

    for (size_t i = 0; i < layout->field_count; i++)
    {
        payload += layout->fields[i].size;
        if (i + 1 < layout->field_count)
        {
            size_t next_offset = layout->fields[i + 1].offset;
            size_t end_curr = layout->fields[i].offset + layout->fields[i].size;
            if (next_offset > end_curr)
            {
                layout->fields[i].padding_after = next_offset - end_curr;
            }
            else
            {
                layout->fields[i].padding_after = 0;
            }
        }
        else
        {
            size_t end_curr = layout->fields[i].offset + layout->fields[i].size;
            if (layout->total_size > end_curr)
            {
                layout->fields[i].padding_after = layout->total_size - end_curr;
            }
            else
            {
                layout->fields[i].padding_after = 0;
            }
        }
        padding += layout->fields[i].padding_after;
    }

    layout->payload_size = payload;
    layout->total_padding = padding;
}

void print_struct_layout_report(const StructLayout* layout, const void* instance_ptr)
{
    if (!layout)
    {
        printf("(NULL StructLayout pointer)\n");
        return;
    }

    printf("\n========================================================================\n");
    printf("        STRUCT MEMORY LAYOUT & ALIGNMENT ANALYSIS: %s        \n",
           layout->struct_name ? layout->struct_name : "Anonymous Struct");
    printf("========================================================================\n");
    printf("Total Size: %zu bytes | Payload: %zu bytes | Padding: %zu bytes\n", layout->total_size,
           layout->payload_size, layout->total_padding);
    if (instance_ptr)
    {
        uintptr_t addr = (uintptr_t)instance_ptr;
        printf("Instance Base Address: %p (Alignment: %zu-byte boundary %s)\n", instance_ptr,
               layout->alignment,
               (addr % (layout->alignment ? layout->alignment : 1) == 0) ? "[OK]" : "[MISALIGNED]");
    }
    printf("------------------------------------------------------------------------\n");
    printf("  %-18s | %-8s | %-6s | %-12s | %-12s\n", "Field Name", "Offset", "Size",
           "Padding After", "Byte Range");
    printf("  -------------------+----------+--------+---------------+--------------\n");

    for (size_t i = 0; i < layout->field_count; i++)
    {
        const StructField* f = &layout->fields[i];
        size_t end_byte = f->offset + f->size - 1;
        printf("  %-18s | +%-7zu | %-6zu | %-12zu | [%zu - %zu]\n", f->name, f->offset, f->size,
               f->padding_after, f->offset, end_byte);
    }
    printf("========================================================================\n\n");

    if (instance_ptr && layout->total_size > 0)
    {
        print_hexdump(instance_ptr, layout->total_size);
    }
}

void inspect_sll_node_memory(const void* node_ptr)
{
    StructLayout layout = {.struct_name = "Singly LinkedList Node (Node)",
                           .total_size = sizeof(Node),
                           .alignment = _Alignof(Node),
                           .field_count = 2,
                           .fields = {{"data (void*)", offsetof(Node, data), sizeof(void*), 0},
                                      {"next (Node*)", offsetof(Node, next), sizeof(Node*), 0}}};
    finalize_struct_layout(&layout);
    print_struct_layout_report(&layout, node_ptr);
}

void inspect_dll_node_memory(const void* node_ptr)
{
    StructLayout layout = {
        .struct_name = "Doubly LinkedList Node (doubly_ll_Node)",
        .total_size = sizeof(doubly_ll_Node),
        .alignment = _Alignof(doubly_ll_Node),
        .field_count = 3,
        .fields = {{"data (void*)", offsetof(doubly_ll_Node, data), sizeof(void*), 0},
                   {"prev (Node*)", offsetof(doubly_ll_Node, prev), sizeof(doubly_ll_Node*), 0},
                   {"next (Node*)", offsetof(doubly_ll_Node, next), sizeof(doubly_ll_Node*), 0}}};
    finalize_struct_layout(&layout);
    print_struct_layout_report(&layout, node_ptr);
}

void inspect_bst_node_memory(const void* node_ptr)
{
    StructLayout layout = {
        .struct_name = "Binary Search Tree Node (bstNode)",
        .total_size = sizeof(bstNode),
        .alignment = _Alignof(bstNode),
        .field_count = 3,
        .fields = {{"data (int)", offsetof(bstNode, data), sizeof(int), 0},
                   {"left (bstNode*)", offsetof(bstNode, left), sizeof(bstNode*), 0},
                   {"right (bstNode*)", offsetof(bstNode, right), sizeof(bstNode*), 0}}};
    finalize_struct_layout(&layout);
    print_struct_layout_report(&layout, node_ptr);
}
