#include "cache.h"
#include <limits.h>
#include <stdio.h>

void cache_init(Cache* cache, int capacity)
{
    if (capacity > CACHE_MAX_CAPACITY)
    {
        capacity = CACHE_MAX_CAPACITY;
    }
    if (capacity < 1)
    {
        capacity = 1;
    }
    cache->capacity = capacity;
    cache->size = 0;
    cache->fifo_index = 0;
    cache->access_counter = 0;
    cache->last_accessed_slot = -1;
    cache->hits = 0;
    cache->misses = 0;
    for (int i = 0; i < CACHE_MAX_CAPACITY; i++)
    {
        cache->blocks[i].page_id = -1;
        cache->blocks[i].is_valid = false;
        cache->blocks[i].is_dirty = false;
        cache->blocks[i].reference_bit = 0;
        cache->blocks[i].frequency = 0;
        cache->blocks[i].last_access_time = 0;
    }
}

bool cache_access_fifo(Cache* cache, int page_id, bool is_write)
{
    // Search for page in the cache (Hit check)
    for (int i = 0; i < cache->capacity; i++)
    {
        if (cache->blocks[i].is_valid && cache->blocks[i].page_id == page_id)
        {
            cache->hits++;
            cache->last_accessed_slot = i;
            if (is_write)
            {
                cache->blocks[i].is_dirty = true;
            }
            return true; // Hit
        }
    }

    // Cache Miss
    cache->misses++;

    // If cache is not full, insert in the first invalid slot
    if (cache->size < cache->capacity)
    {
        for (int i = 0; i < cache->capacity; i++)
        {
            if (!cache->blocks[i].is_valid)
            {
                cache->blocks[i].page_id = page_id;
                cache->blocks[i].is_valid = true;
                cache->blocks[i].is_dirty = is_write;
                cache->last_accessed_slot = i;
                cache->size++;
                return false; // Miss (inserted without eviction)
            }
        }
    }

    // Cache is full: Evict using FIFO policy
    int evict_idx = cache->fifo_index;

    // Perform eviction
    cache->blocks[evict_idx].page_id = page_id;
    cache->blocks[evict_idx].is_valid = true;
    cache->blocks[evict_idx].is_dirty = is_write;
    cache->last_accessed_slot = evict_idx;

    // Update FIFO pointer (circular queue)
    cache->fifo_index = (cache->fifo_index + 1) % cache->capacity;

    return false; // Miss (with eviction)
}

bool cache_access_lru(Cache* cache, int page_id, bool is_write)
{
    cache->access_counter++;

    // Search for page in the cache (Hit check)
    for (int i = 0; i < cache->capacity; i++)
    {
        if (cache->blocks[i].is_valid && cache->blocks[i].page_id == page_id)
        {
            cache->hits++;
            cache->blocks[i].last_access_time = cache->access_counter;
            cache->last_accessed_slot = i;
            if (is_write)
            {
                cache->blocks[i].is_dirty = true;
            }
            return true; // Hit
        }
    }

    // Cache Miss
    cache->misses++;

    // If cache is not full, insert in the first invalid slot
    if (cache->size < cache->capacity)
    {
        for (int i = 0; i < cache->capacity; i++)
        {
            if (!cache->blocks[i].is_valid)
            {
                cache->blocks[i].page_id = page_id;
                cache->blocks[i].is_valid = true;
                cache->blocks[i].is_dirty = is_write;
                cache->blocks[i].last_access_time = cache->access_counter;
                cache->last_accessed_slot = i;
                cache->size++;
                return false; // Miss (inserted without eviction)
            }
        }
    }

    // Cache is full: Find block with MINIMUM last_access_time
    int evict_idx = 0;
    int min_access = cache->blocks[0].last_access_time;
    for (int i = 1; i < cache->capacity; i++)
    {
        if (cache->blocks[i].last_access_time < min_access)
        {
            min_access = cache->blocks[i].last_access_time;
            evict_idx = i;
        }
    }

    // Perform eviction
    cache->blocks[evict_idx].page_id = page_id;
    cache->blocks[evict_idx].is_valid = true;
    cache->blocks[evict_idx].is_dirty = is_write;
    cache->blocks[evict_idx].last_access_time = cache->access_counter;
    cache->last_accessed_slot = evict_idx;

    return false; // Miss (with eviction)
}

bool cache_access_mru(Cache* cache, int page_id, bool is_write)
{
    cache->access_counter++;

    // Search for page in the cache (Hit check)
    for (int i = 0; i < cache->capacity; i++)
    {
        if (cache->blocks[i].is_valid && cache->blocks[i].page_id == page_id)
        {
            cache->hits++;
            cache->blocks[i].last_access_time = cache->access_counter;
            cache->last_accessed_slot = i;
            if (is_write)
            {
                cache->blocks[i].is_dirty = true;
            }
            return true; // Hit
        }
    }

    // Cache Miss
    cache->misses++;

    // If cache is not full, insert in the first invalid slot
    if (cache->size < cache->capacity)
    {
        for (int i = 0; i < cache->capacity; i++)
        {
            if (!cache->blocks[i].is_valid)
            {
                cache->blocks[i].page_id = page_id;
                cache->blocks[i].is_valid = true;
                cache->blocks[i].is_dirty = is_write;
                cache->blocks[i].last_access_time = cache->access_counter;
                cache->last_accessed_slot = i;
                cache->size++;
                return false; // Miss (inserted without eviction)
            }
        }
    }

    // Cache is full: Find block with MAXIMUM last_access_time
    int evict_idx = 0;
    int max_access = cache->blocks[0].last_access_time;
    for (int i = 1; i < cache->capacity; i++)
    {
        if (cache->blocks[i].last_access_time > max_access)
        {
            max_access = cache->blocks[i].last_access_time;
            evict_idx = i;
        }
    }

    // Perform eviction
    cache->blocks[evict_idx].page_id = page_id;
    cache->blocks[evict_idx].is_valid = true;
    cache->blocks[evict_idx].is_dirty = is_write;
    cache->blocks[evict_idx].last_access_time = cache->access_counter;
    cache->last_accessed_slot = evict_idx;

    return false; // Miss (with eviction)
}

bool cache_access_lfu(Cache* cache, int page_id, bool is_write)
{
    cache->access_counter++;

    // Periodic Frequency Aging / Decay (every 8 accesses)
    if (cache->access_counter > 0 && cache->access_counter % 8 == 0)
    {
        for (int i = 0; i < cache->capacity; i++)
        {
            if (cache->blocks[i].is_valid)
            {
                cache->blocks[i].frequency /= 2;
            }
        }
    }

    // Search for page in the cache (Hit check)
    for (int i = 0; i < cache->capacity; i++)
    {
        if (cache->blocks[i].is_valid && cache->blocks[i].page_id == page_id)
        {
            cache->hits++;
            cache->blocks[i].frequency++;
            cache->blocks[i].last_access_time = cache->access_counter;
            cache->last_accessed_slot = i;
            if (is_write)
            {
                cache->blocks[i].is_dirty = true;
            }
            return true; // Hit
        }
    }

    // Cache Miss
    cache->misses++;

    // If cache is not full, insert in the first invalid slot
    if (cache->size < cache->capacity)
    {
        for (int i = 0; i < cache->capacity; i++)
        {
            if (!cache->blocks[i].is_valid)
            {
                cache->blocks[i].page_id = page_id;
                cache->blocks[i].is_valid = true;
                cache->blocks[i].is_dirty = is_write;
                cache->blocks[i].frequency = 1;
                cache->blocks[i].last_access_time = cache->access_counter;
                cache->last_accessed_slot = i;
                cache->size++;
                return false; // Miss (inserted without eviction)
            }
        }
    }

    // Cache is full: Find block with MINIMUM frequency.
    // Break ties using LRU (smallest last_access_time)
    int evict_idx = 0;
    int min_freq = cache->blocks[0].frequency;
    int min_access = cache->blocks[0].last_access_time;

    for (int i = 1; i < cache->capacity; i++)
    {
        if (cache->blocks[i].frequency < min_freq)
        {
            min_freq = cache->blocks[i].frequency;
            min_access = cache->blocks[i].last_access_time;
            evict_idx = i;
        }
        else if (cache->blocks[i].frequency == min_freq)
        {
            // Tie-break with LRU
            if (cache->blocks[i].last_access_time < min_access)
            {
                min_access = cache->blocks[i].last_access_time;
                evict_idx = i;
            }
        }
    }

    // Perform eviction
    cache->blocks[evict_idx].page_id = page_id;
    cache->blocks[evict_idx].is_valid = true;
    cache->blocks[evict_idx].is_dirty = is_write;
    cache->blocks[evict_idx].frequency = 1;
    cache->blocks[evict_idx].last_access_time = cache->access_counter;
    cache->last_accessed_slot = evict_idx;

    return false; // Miss (with eviction)
}

bool cache_access_opt(Cache* cache, int page_id, const int* ref_str, int ref_len, int current_idx,
                      bool is_write)
{
    cache->access_counter++;

    // Search for page in the cache (Hit check)
    for (int i = 0; i < cache->capacity; i++)
    {
        if (cache->blocks[i].is_valid && cache->blocks[i].page_id == page_id)
        {
            cache->hits++;
            cache->blocks[i].last_access_time = cache->access_counter;
            cache->last_accessed_slot = i;
            if (is_write)
            {
                cache->blocks[i].is_dirty = true;
            }
            return true; // Hit
        }
    }

    // Cache Miss
    cache->misses++;

    // If cache is not full, insert in first invalid slot
    if (cache->size < cache->capacity)
    {
        for (int i = 0; i < cache->capacity; i++)
        {
            if (!cache->blocks[i].is_valid)
            {
                cache->blocks[i].page_id = page_id;
                cache->blocks[i].is_valid = true;
                cache->blocks[i].is_dirty = is_write;
                cache->blocks[i].last_access_time = cache->access_counter;
                cache->last_accessed_slot = i;
                cache->size++;
                return false; // Miss (without eviction)
            }
        }
    }

    // Cache is full: Find block whose next access is furthest in future
    int evict_idx = 0;
    int max_next_access = -1;

    for (int i = 0; i < cache->capacity; i++)
    {
        int page = cache->blocks[i].page_id;
        int next_access = INT_MAX; // Infinity if never accessed again

        for (int j = current_idx + 1; j < ref_len; j++)
        {
            if (ref_str[j] == page)
            {
                next_access = j;
                break;
            }
        }

        if (next_access > max_next_access)
        {
            max_next_access = next_access;
            evict_idx = i;
        }
        else if (next_access == max_next_access)
        {
            // Tie-break with LRU
            if (cache->blocks[i].last_access_time < cache->blocks[evict_idx].last_access_time)
            {
                evict_idx = i;
            }
        }
    }

    // Perform eviction
    cache->blocks[evict_idx].page_id = page_id;
    cache->blocks[evict_idx].is_valid = true;
    cache->blocks[evict_idx].is_dirty = is_write;
    cache->blocks[evict_idx].last_access_time = cache->access_counter;
    cache->last_accessed_slot = evict_idx;

    return false; // Miss (with eviction)
}

bool cache_access_clock(Cache* cache, int page_id, bool is_write)
{
    cache->access_counter++;

    // Search for page in the cache (Hit check)
    for (int i = 0; i < cache->capacity; i++)
    {
        if (cache->blocks[i].is_valid && cache->blocks[i].page_id == page_id)
        {
            cache->hits++;
            cache->blocks[i].reference_bit = 1;
            cache->last_accessed_slot = i;
            if (is_write)
            {
                cache->blocks[i].is_dirty = true;
            }
            return true; // Hit
        }
    }

    // Cache Miss
    cache->misses++;

    // If cache is not full, insert in first invalid slot
    if (cache->size < cache->capacity)
    {
        for (int i = 0; i < cache->capacity; i++)
        {
            if (!cache->blocks[i].is_valid)
            {
                cache->blocks[i].page_id = page_id;
                cache->blocks[i].is_valid = true;
                cache->blocks[i].is_dirty = is_write;
                cache->blocks[i].reference_bit = 1;
                cache->last_accessed_slot = i;
                cache->size++;
                return false; // Miss (without eviction)
            }
        }
    }

    // Cache is full: Run Clock (Second Chance) algorithm
    while (1)
    {
        int hand = cache->fifo_index;
        if (cache->blocks[hand].reference_bit == 1)
        {
            cache->blocks[hand].reference_bit = 0;
            cache->fifo_index = (cache->fifo_index + 1) % cache->capacity;
        }
        else
        {
            // Evict page at hand
            cache->blocks[hand].page_id = page_id;
            cache->blocks[hand].is_valid = true;
            cache->blocks[hand].is_dirty = is_write;
            cache->blocks[hand].reference_bit = 1;
            cache->last_accessed_slot = hand;
            cache->fifo_index = (cache->fifo_index + 1) % cache->capacity;
            break;
        }
    }

    return false; // Miss (with eviction)
}

bool cache_access_enhanced_clock(Cache* cache, int page_id, bool is_write)
{
    cache->access_counter++;

    // Search for page in the cache (Hit check)
    for (int i = 0; i < cache->capacity; i++)
    {
        if (cache->blocks[i].is_valid && cache->blocks[i].page_id == page_id)
        {
            cache->hits++;
            cache->blocks[i].reference_bit = 1;
            cache->last_accessed_slot = i;
            if (is_write)
            {
                cache->blocks[i].is_dirty = true;
            }
            return true; // Hit
        }
    }

    // Cache Miss
    cache->misses++;

    // If cache is not full, insert in first invalid slot
    if (cache->size < cache->capacity)
    {
        for (int i = 0; i < cache->capacity; i++)
        {
            if (!cache->blocks[i].is_valid)
            {
                cache->blocks[i].page_id = page_id;
                cache->blocks[i].is_valid = true;
                cache->blocks[i].is_dirty = is_write;
                cache->blocks[i].reference_bit = 1;
                cache->last_accessed_slot = i;
                cache->size++;
                return false; // Miss (without eviction)
            }
        }
    }

    // Cache is full: Run Enhanced Second Chance Clock algorithm
    int start_hand = cache->fifo_index;

    // Step 1: Scan for Class 0 (0, 0)
    for (int step = 0; step < cache->capacity; step++)
    {
        int idx = (start_hand + step) % cache->capacity;
        if (cache->blocks[idx].reference_bit == 0 && !cache->blocks[idx].is_dirty)
        {
            cache->blocks[idx].page_id = page_id;
            cache->blocks[idx].is_valid = true;
            cache->blocks[idx].is_dirty = is_write;
            cache->blocks[idx].reference_bit = 1;
            cache->last_accessed_slot = idx;
            cache->fifo_index = (idx + 1) % cache->capacity;
            return false; // Miss (with eviction)
        }
    }

    // Step 2: Scan for Class 1 (0, 1), modifying reference bit of bypassed pages
    for (int step = 0; step < cache->capacity; step++)
    {
        int idx = (start_hand + step) % cache->capacity;
        if (cache->blocks[idx].reference_bit == 0 && cache->blocks[idx].is_dirty)
        {
            cache->blocks[idx].page_id = page_id;
            cache->blocks[idx].is_valid = true;
            cache->blocks[idx].is_dirty = is_write;
            cache->blocks[idx].reference_bit = 1;
            cache->last_accessed_slot = idx;
            cache->fifo_index = (idx + 1) % cache->capacity;
            return false; // Miss (with eviction)
        }
        else
        {
            cache->blocks[idx].reference_bit = 0;
        }
    }

    // Step 3: If Step 2 failed, repeat Step 1
    for (int step = 0; step < cache->capacity; step++)
    {
        int idx = (start_hand + step) % cache->capacity;
        if (cache->blocks[idx].reference_bit == 0 && !cache->blocks[idx].is_dirty)
        {
            cache->blocks[idx].page_id = page_id;
            cache->blocks[idx].is_valid = true;
            cache->blocks[idx].is_dirty = is_write;
            cache->blocks[idx].reference_bit = 1;
            cache->last_accessed_slot = idx;
            cache->fifo_index = (idx + 1) % cache->capacity;
            return false; // Miss (with eviction)
        }
    }

    // Step 4: If Step 3 failed, repeat Step 2
    for (int step = 0; step < cache->capacity; step++)
    {
        int idx = (start_hand + step) % cache->capacity;
        if (cache->blocks[idx].reference_bit == 0 && cache->blocks[idx].is_dirty)
        {
            cache->blocks[idx].page_id = page_id;
            cache->blocks[idx].is_valid = true;
            cache->blocks[idx].is_dirty = is_write;
            cache->blocks[idx].reference_bit = 1;
            cache->last_accessed_slot = idx;
            cache->fifo_index = (idx + 1) % cache->capacity;
            return false; // Miss (with eviction)
        }
    }

    return false;
}

void cache_print_status(const Cache* cache)
{
    printf("Cache state: [ ");
    for (int i = 0; i < cache->capacity; i++)
    {
        if (cache->blocks[i].is_valid)
        {
            printf("%d ", cache->blocks[i].page_id);
        }
        else
        {
            printf("- ");
        }
    }
    printf("] (Hits: %d, Misses: %d)\n", cache->hits, cache->misses);
}

void cache_visualize(const Cache* cache, int highlighted_slot, bool is_hit)
{
    // Draw top border labels
    for (int i = 0; i < cache->capacity; i++)
    {
        printf("   [Frame %d]  ", i);
    }
    printf("\n");

    // Line 1: Top border of boxes
    for (int i = 0; i < cache->capacity; i++)
    {
        if (i == highlighted_slot)
        {
            printf(is_hit ? "\033[1;32m ┌─────────┐ \033[0m" : "\033[1;31m ┌─────────┐ \033[0m");
        }
        else
        {
            printf(" ┌─────────┐ ");
        }
    }
    printf("\n");

    // Line 2: Page ID
    for (int i = 0; i < cache->capacity; i++)
    {
        char page_str[16];
        if (cache->blocks[i].is_valid)
        {
            sprintf(page_str, "Page: %-3d", cache->blocks[i].page_id);
        }
        else
        {
            sprintf(page_str, "Page: -  ");
        }

        if (i == highlighted_slot)
        {
            printf(is_hit ? "\033[1;32m │ %s │ \033[0m" : "\033[1;31m │ %s │ \033[0m", page_str);
        }
        else
        {
            printf(" │ %s │ ", page_str);
        }
    }
    printf("\n");

    // Line 3: Reference bit
    for (int i = 0; i < cache->capacity; i++)
    {
        char ref_str[16];
        sprintf(ref_str, "Ref:  %-3d", cache->blocks[i].reference_bit);

        if (i == highlighted_slot)
        {
            printf(is_hit ? "\033[1;32m │ %s │ \033[0m" : "\033[1;31m │ %s │ \033[0m", ref_str);
        }
        else
        {
            printf(" │ %s │ ", ref_str);
        }
    }
    printf("\n");

    // Line 4: Frequency
    for (int i = 0; i < cache->capacity; i++)
    {
        char freq_str[16];
        sprintf(freq_str, "Freq: %-3d", cache->blocks[i].frequency);

        if (i == highlighted_slot)
        {
            printf(is_hit ? "\033[1;32m │ %s │ \033[0m" : "\033[1;31m │ %s │ \033[0m", freq_str);
        }
        else
        {
            printf(" │ %s │ ", freq_str);
        }
    }
    printf("\n");

    // Line 5: Dirty bit
    for (int i = 0; i < cache->capacity; i++)
    {
        char dirty_str[16];
        sprintf(dirty_str, "Dirty:%-3s", cache->blocks[i].is_dirty ? "Yes" : "No ");

        if (i == highlighted_slot)
        {
            printf(is_hit ? "\033[1;32m │ %s │ \033[0m" : "\033[1;31m │ %s │ \033[0m", dirty_str);
        }
        else
        {
            printf(" │ %s │ ", dirty_str);
        }
    }
    printf("\n");

    // Line 6: Bottom border of boxes
    for (int i = 0; i < cache->capacity; i++)
    {
        if (i == highlighted_slot)
        {
            printf(is_hit ? "\033[1;32m └─────────┘ \033[0m" : "\033[1;31m └─────────┘ \033[0m");
        }
        else
        {
            printf(" └─────────┘ ");
        }
    }
    printf("\n");

    // Line 7: Clock Hand / Pointer
    for (int i = 0; i < cache->capacity; i++)
    {
        if (i == cache->fifo_index)
        {
            printf("      ▲      ");
        }
        else
        {
            printf("             ");
        }
    }
    printf("\n");

    for (int i = 0; i < cache->capacity; i++)
    {
        if (i == cache->fifo_index)
        {
            printf("  (Clock Hand) ");
        }
        else
        {
            printf("             ");
        }
    }
    printf("\n\n");
}
