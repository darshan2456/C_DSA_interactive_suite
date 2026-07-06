#ifndef MEMORY_TRACKER_H
#define MEMORY_TRACKER_H

#include <stddef.h>

void* custom_malloc(size_t size, const char* file, int line);
void* custom_calloc(size_t num, size_t size, const char* file, int line);
void* custom_realloc(void* ptr, size_t size, const char* file, int line);
void custom_free(void* ptr, const char* file, int line);

size_t get_current_allocated_bytes(void);
size_t get_peak_allocated_bytes(void);
size_t get_total_allocated_blocks(void);
size_t get_total_deallocated_blocks(void);
double get_memory_fragmentation_ratio(void);
double get_block_size_dispersion(void);

void init_memory_tracker(void);
void print_memory_leak_report(void);

// Intercept standard library calls if profiling is active
#ifndef IN_MEMORY_TRACKER_C
#define malloc(size) custom_malloc(size, __FILE__, __LINE__)
#define calloc(num, size) custom_calloc(num, size, __FILE__, __LINE__)
#define realloc(ptr, size) custom_realloc(ptr, size, __FILE__, __LINE__)
#define free(ptr) custom_free(ptr, __FILE__, __LINE__)
#endif

#endif // MEMORY_TRACKER_H
