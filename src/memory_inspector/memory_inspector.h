#ifndef MEMORY_INSPECTOR_H
#define MEMORY_INSPECTOR_H

#include <stddef.h>

/**
 * Print hexdump of memory region in classic hex editor format.
 * Shows: address, hex bytes, ASCII representation.
 * 
 * @param ptr    Pointer to memory region to inspect
 * @param size   Number of bytes to display
 */
void print_hexdump(const void* ptr, size_t size);

/**
 * Print hexdump with custom bytes per line.
 * 
 * @param ptr         Pointer to memory region
 * @param size        Number of bytes to display
 * @param bytes_per_line  Number of bytes per output line (typically 16 or 32)
 */
void print_hexdump_ex(const void* ptr, size_t size, int bytes_per_line);

/**
 * Analyze and print memory layout of a structure.
 * Shows size, alignment requirements, and padding analysis.
 * 
 * @param ptr       Pointer to structure
 * @param type_name Name of the structure type (for display)
 */
void analyze_struct_layout(const void* ptr, const char* type_name);

/**
 * Print memory as different data types (int, float, pointers).
 * Useful for understanding how data is stored in memory.
 * 
 * @param ptr   Pointer to memory
 * @param size  Size in bytes
 */
void print_memory_as_types(const void* ptr, size_t size);

/**
 * Compare two memory regions and highlight differences.
 * 
 * @param ptr1   First memory region
 * @param ptr2   Second memory region
 * @param size   Number of bytes to compare
 * @return       Number of differing bytes
 */
int compare_memory(const void* ptr1, const void* ptr2, size_t size);

/**
 * Interactive memory inspector demo.
 */
void memory_inspector_demo(void);

#endif // MEMORY_INSPECTOR_H