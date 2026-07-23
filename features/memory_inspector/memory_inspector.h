#ifndef MEMORY_INSPECTOR_H
#define MEMORY_INSPECTOR_H

#include <stddef.h>

#define MAX_STRUCT_FIELDS 16

typedef struct StructField
{
    const char* name;
    size_t offset;
    size_t size;
    size_t padding_after;
} StructField;

typedef struct StructLayout
{
    const char* struct_name;
    size_t total_size;
    size_t payload_size;
    size_t total_padding;
    size_t alignment;
    size_t field_count;
    StructField fields[MAX_STRUCT_FIELDS];
} StructLayout;

/**
 * Print a classic hex dump of a memory block to standard output.
 * Formatted with 16 bytes per line, memory address offsets, raw hex bytes, and ASCII
 * representation.
 *
 * @param ptr Pointer to the memory block to inspect.
 * @param size Number of bytes to inspect.
 */
void print_hexdump(const void* ptr, size_t size);

/**
 * Format a hex dump of a memory block into a user-provided string buffer.
 *
 * @param ptr Pointer to the memory block to inspect.
 * @param size Number of bytes to inspect.
 * @param out_buf Buffer to write the formatted string into.
 * @param buf_size Size of the output buffer in bytes.
 * @return Number of characters written or required.
 */
size_t format_hexdump(const void* ptr, size_t size, char* out_buf, size_t buf_size);

/**
 * Print a detailed memory layout report of a structure including field offsets, sizes,
 * byte padding, and alignment efficiency.
 *
 * @param layout StructLayout metadata describing the structure fields.
 * @param instance_ptr Pointer to an instantiated struct instance (optional, for address breakdown).
 */
void print_struct_layout_report(const StructLayout* layout, const void* instance_ptr);

/**
 * Calculate total padding bytes and internal efficiency ratio for a struct layout.
 *
 * @param layout StructLayout to finalize and compute summary stats for.
 */
void finalize_struct_layout(StructLayout* layout);

/**
 * Inspect memory layout, alignment, and raw bytes of a Singly Linked List Node (Node).
 *
 * @param node_ptr Pointer to the SLL node.
 */
void inspect_sll_node_memory(const void* node_ptr);

/**
 * Inspect memory layout, alignment, and raw bytes of a Doubly Linked List Node (doubly_ll_Node).
 *
 * @param node_ptr Pointer to the DLL node.
 */
void inspect_dll_node_memory(const void* node_ptr);

/**
 * Inspect memory layout, alignment, and raw bytes of a Binary Search Tree Node (bstNode).
 *
 * @param node_ptr Pointer to the BST node.
 */
void inspect_bst_node_memory(const void* node_ptr);

/**
 * Interactive memory inspector and raw hexdump visualizer demo suite.
 */
void memory_inspector_demo(void);

#endif /* MEMORY_INSPECTOR_H */
