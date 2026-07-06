#define IN_MEMORY_TRACKER_C
#include "memory_tracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AllocatedBlock
{
    void* address;
    size_t size;
    const char* filename;
    int line;
    struct AllocatedBlock* next;
} AllocatedBlock;

static AllocatedBlock* head = NULL;

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
    block->next = head;
    head = block;
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

static void update_block(void* old_addr, void* new_addr, size_t new_size, const char* file,
                         int line)
{
    if (old_addr == NULL)
    {
        add_block(new_addr, new_size, file, line);
        return;
    }
    AllocatedBlock* curr = head;
    while (curr != NULL)
    {
        if (curr->address == old_addr)
        {
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
    if (ptr != NULL)
    {
        add_block(ptr, size, file, line);
    }
    return ptr;
}

void* custom_calloc(size_t num, size_t size, const char* file, int line)
{
    void* ptr = calloc(num, size);
    if (ptr != NULL)
    {
        add_block(ptr, num * size, file, line);
    }
    return ptr;
}

void* custom_realloc(void* ptr, size_t size, const char* file, int line)
{
    void* new_ptr = realloc(ptr, size);
    if (new_ptr != NULL)
    {
        update_block(ptr, new_ptr, size, file, line);
    }
    return new_ptr;
}

void custom_free(void* ptr, const char* file, int line)
{
    (void)file;
    (void)line;
    if (ptr == NULL)
        return;
    remove_block(ptr);
    free(ptr);
}
