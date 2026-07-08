#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>

#define CACHE_MAX_CAPACITY 10

typedef struct
{
    int page_id;
    bool is_valid;
    bool is_dirty;
    int reference_bit;
    int frequency;
    int last_access_time;
} CacheBlock;

typedef struct
{
    CacheBlock blocks[CACHE_MAX_CAPACITY];
    int capacity;
    int size;
    int fifo_index;
    int access_counter;

    // Statistics
    int hits;
    int misses;
} Cache;

void cache_init(Cache* cache, int capacity);
bool cache_access_fifo(Cache* cache, int page_id, bool is_write);
bool cache_access_lru(Cache* cache, int page_id, bool is_write);
bool cache_access_mru(Cache* cache, int page_id, bool is_write);
bool cache_access_lfu(Cache* cache, int page_id, bool is_write);
bool cache_access_opt(Cache* cache, int page_id, const int* ref_str, int ref_len, int current_idx,
                      bool is_write);
bool cache_access_clock(Cache* cache, int page_id, bool is_write);
bool cache_access_enhanced_clock(Cache* cache, int page_id, bool is_write);
void cache_print_status(const Cache* cache);
void cache_simulator_demo(void);

#endif // CACHE_H
