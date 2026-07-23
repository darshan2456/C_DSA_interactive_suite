#ifndef MEMORY_INSPECTOR_H
#define MEMORY_INSPECTOR_H

#include <stddef.h>

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

#endif /* MEMORY_INSPECTOR_H */
