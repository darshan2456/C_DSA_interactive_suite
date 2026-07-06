#define IN_MEMORY_TRACKER_C
#include "memory_tracker.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <execinfo.h>
#endif

#define MAX_BACKTRACE_FRAMES 10

typedef struct AllocatedBlock
{
    void* address;
    size_t size;
    const char* filename;
    int line;
    void* backtrace_buffer[MAX_BACKTRACE_FRAMES];
    int backtrace_size;
    struct AllocatedBlock* next;
} AllocatedBlock;

static AllocatedBlock* head = NULL;
static int profiling_enabled = 0;

static size_t current_allocated_bytes = 0;
static size_t peak_allocated_bytes = 0;
static size_t total_allocated_blocks = 0;
static size_t total_deallocated_blocks = 0;

static void add_block(void* addr, size_t size, const char* file, int line)
{
    if (addr == NULL)
        return;
    AllocatedBlock* block = (AllocatedBlock*)malloc(sizeof(AllocatedBlock));
    if (block == NULL)
        return;
    block->address = addr;
    block->size = size;
    block->filename = file;
    block->line = line;

#ifndef _WIN32
    block->backtrace_size = backtrace(block->backtrace_buffer, MAX_BACKTRACE_FRAMES);
#else
    block->backtrace_size = 0;
#endif

    block->next = head;
    head = block;

    current_allocated_bytes += size;
    total_allocated_blocks++;
    if (current_allocated_bytes > peak_allocated_bytes)
    {
        peak_allocated_bytes = current_allocated_bytes;
    }
}

static void remove_block(void* addr)
{
    if (addr == NULL)
        return;
    AllocatedBlock* curr = head;
    AllocatedBlock* prev = NULL;
    while (curr != NULL)
    {
        if (curr->address == addr)
        {
            current_allocated_bytes -= curr->size;
            total_deallocated_blocks++;

            if (prev == NULL)
            {
                head = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

static void update_block(uintptr_t old_addr_key, void* new_addr, size_t new_size, const char* file,
                         int line)
{
    if (old_addr_key == (uintptr_t)NULL)
    {
        add_block(new_addr, new_size, file, line);
        return;
    }
    AllocatedBlock* curr = head;
    while (curr != NULL)
    {
        if ((uintptr_t)curr->address == old_addr_key)
        {
            current_allocated_bytes = current_allocated_bytes - curr->size + new_size;
            if (current_allocated_bytes > peak_allocated_bytes)
            {
                peak_allocated_bytes = current_allocated_bytes;
            }

            curr->address = new_addr;
            curr->size = new_size;
            curr->filename = file;
            curr->line = line;
            return;
        }
        curr = curr->next;
    }
    add_block(new_addr, new_size, file, line);
}

void* custom_malloc(size_t size, const char* file, int line)
{
    void* ptr = malloc(size);
    if (profiling_enabled && ptr != NULL)
    {
        add_block(ptr, size, file, line);
    }
    return ptr;
}

void* custom_calloc(size_t num, size_t size, const char* file, int line)
{
    void* ptr = calloc(num, size);
    if (profiling_enabled && ptr != NULL)
    {
        add_block(ptr, num * size, file, line);
    }
    return ptr;
}

void* custom_realloc(void* ptr, size_t size, const char* file, int line)
{
    uintptr_t old_addr_key = (uintptr_t)ptr;
    void* new_ptr = realloc(ptr, size);
    if (profiling_enabled && new_ptr != NULL)
    {
        update_block(old_addr_key, new_ptr, size, file, line);
    }
    return new_ptr;
}

void custom_free(void* ptr, const char* file, int line)
{
    (void)file;
    (void)line;
    if (ptr == NULL)
        return;
    if (profiling_enabled)
    {
        remove_block(ptr);
    }
    free(ptr);
}

size_t get_current_allocated_bytes(void)
{
    return current_allocated_bytes;
}

size_t get_peak_allocated_bytes(void)
{
    return peak_allocated_bytes;
}

size_t get_total_allocated_blocks(void)
{
    return total_allocated_blocks;
}

size_t get_total_deallocated_blocks(void)
{
    return total_deallocated_blocks;
}

double get_memory_fragmentation_ratio(void)
{
    if (head == NULL || head->next == NULL)
    {
        return 0.0;
    }

    uintptr_t min_addr = (uintptr_t)head->address;
    uintptr_t max_end_addr = (uintptr_t)head->address + head->size;
    size_t total_size = 0;

    AllocatedBlock* curr = head;
    while (curr != NULL)
    {
        uintptr_t addr = (uintptr_t)curr->address;
        if (addr < min_addr)
        {
            min_addr = addr;
        }
        if (addr + curr->size > max_end_addr)
        {
            max_end_addr = addr + curr->size;
        }
        total_size += curr->size;
        curr = curr->next;
    }

    uintptr_t span = max_end_addr - min_addr;
    if (span == 0)
    {
        return 0.0;
    }

    double ratio = 1.0 - ((double)total_size / (double)span);
    return (ratio < 0.0) ? 0.0 : ratio;
}

double get_block_size_dispersion(void)
{
    if (head == NULL)
    {
        return 0.0;
    }

    size_t total_size = 0;
    int count = 0;
    AllocatedBlock* curr = head;
    while (curr != NULL)
    {
        total_size += curr->size;
        count++;
        curr = curr->next;
    }
    double mean = (double)total_size / count;

    double variance_sum = 0.0;
    curr = head;
    while (curr != NULL)
    {
        double diff = (double)curr->size - mean;
        variance_sum += diff * diff;
        curr = curr->next;
    }

    double variance = variance_sum / count;
    if (variance <= 0.0)
    {
        return 0.0;
    }
    double z = variance;
    for (int i = 0; i < 10; i++)
    {
        z = 0.5 * (z + variance / z);
    }
    return z;
}
void init_memory_tracker(void)
{
    profiling_enabled = 1;
    atexit(print_memory_leak_report);
}

void print_memory_leak_report(void)
{
    if (!profiling_enabled)
        return;

    if (head == NULL)
    {
        printf("\n==================================================\n");
        printf("          MEMORY PROFILER: NO LEAKS DETECTED       \n");
        printf("==================================================\n");
        return;
    }

    printf("\n==================================================\n");
    printf("         MEMORY PROFILER: LEAK REPORT              \n");
    printf("==================================================\n");
    printf("%-20s %-10s %-15s\n", "Address", "Size (bytes)", "Location");
    printf("--------------------------------------------------\n");

    size_t total_leaked = 0;
    int leak_count = 0;
    AllocatedBlock* curr = head;
    while (curr != NULL)
    {
        printf("%-20p %-10zu %s:%d\n", curr->address, curr->size, curr->filename, curr->line);
#ifndef _WIN32
        if (curr->backtrace_size > 0)
        {
            char** symbols = backtrace_symbols(curr->backtrace_buffer, curr->backtrace_size);
            if (symbols != NULL)
            {
                printf("  Call Stack:\n");
                for (int j = 0; j < curr->backtrace_size; j++)
                {
                    printf("    %s\n", symbols[j]);
                }
                free(symbols);
            }
        }
#endif

        total_leaked += curr->size;
        leak_count++;
        curr = curr->next;
    }
    printf("--------------------------------------------------\n");
    printf("Total leaked memory: %zu bytes across %d blocks\n", total_leaked, leak_count);
    printf("==================================================\n");
}
