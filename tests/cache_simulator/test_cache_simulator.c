#include "cache.h"
#include <assert.h>
#include <stdio.h>

void test_fifo_basic()
{
    Cache cache;
    cache_init(&cache, 3);

    // Initial state
    assert(cache.capacity == 3);
    assert(cache.size == 0);
    assert(cache.hits == 0);
    assert(cache.misses == 0);

    // Access 1: Miss
    assert(!cache_access_fifo(&cache, 1, false));
    assert(cache.size == 1);
    assert(cache.misses == 1);

    // Access 2: Miss
    assert(!cache_access_fifo(&cache, 2, false));
    assert(cache.size == 2);
    assert(cache.misses == 2);

    // Access 3: Miss
    assert(!cache_access_fifo(&cache, 3, false));
    assert(cache.size == 3);
    assert(cache.misses == 3);

    // Access 1: Hit
    assert(cache_access_fifo(&cache, 1, false));
    assert(cache.size == 3);
    assert(cache.hits == 1);

    // Access 4: Miss (FIFO evicts 1)
    assert(!cache_access_fifo(&cache, 4, false));
    assert(cache.misses == 4);

    // 1 should no longer be in the cache
    bool found_1 = false;
    for (int i = 0; i < 3; i++)
    {
        if (cache.blocks[i].page_id == 1)
            found_1 = true;
    }
    assert(!found_1);

    // Access 1: Miss (FIFO evicts 2)
    assert(!cache_access_fifo(&cache, 1, false));
    assert(cache.misses == 5);

    // 2 should no longer be in cache
    bool found_2 = false;
    for (int i = 0; i < 3; i++)
    {
        if (cache.blocks[i].page_id == 2)
            found_2 = true;
    }
    assert(!found_2);

    printf("FIFO basic cache tests passed\n");
}

void test_lru_basic()
{
    Cache cache;
    cache_init(&cache, 3);

    // Access 1, 2, 3: Misses
    assert(!cache_access_lru(&cache, 1, false));
    assert(!cache_access_lru(&cache, 2, false));
    assert(!cache_access_lru(&cache, 3, false));

    // Access 1: Hit (updates recency)
    assert(cache_access_lru(&cache, 1, false));

    // Access 2: Hit (updates recency)
    assert(cache_access_lru(&cache, 2, false));

    // Access 4: Miss. Recency list: 3 (least recent), 1, 2. So evict 3.
    assert(!cache_access_lru(&cache, 4, false));

    // 3 should no longer be in the cache
    bool found_3 = false;
    for (int i = 0; i < 3; i++)
    {
        if (cache.blocks[i].page_id == 3)
            found_3 = true;
    }
    assert(!found_3);

    printf("LRU basic cache tests passed\n");
}

void test_mru_basic()
{
    Cache cache;
    cache_init(&cache, 3);

    // Access 1, 2, 3: Misses
    assert(!cache_access_mru(&cache, 1, false));
    assert(!cache_access_mru(&cache, 2, false));
    assert(!cache_access_mru(&cache, 3, false));

    // Access 2: Hit (updates recency - 2 becomes most recent)
    assert(cache_access_mru(&cache, 2, false));

    // Access 4: Miss. Evict most recent (2).
    assert(!cache_access_mru(&cache, 4, false));

    // 2 should no longer be in the cache
    bool found_2 = false;
    for (int i = 0; i < 3; i++)
    {
        if (cache.blocks[i].page_id == 2)
            found_2 = true;
    }
    assert(!found_2);

    printf("MRU basic cache tests passed\n");
}

void test_lfu_basic()
{
    Cache cache;
    cache_init(&cache, 3);

    // Access 1, 2, 3: Misses
    assert(!cache_access_lfu(&cache, 1, false));
    assert(!cache_access_lfu(&cache, 2, false));
    assert(!cache_access_lfu(&cache, 3, false));

    // Access 1: Hit (frequency becomes 2)
    assert(cache_access_lfu(&cache, 1, false));

    // Access 2: Hit (frequency becomes 2)
    assert(cache_access_lfu(&cache, 2, false));

    // Access 4: Miss. Frequencies: 1 (freq 2), 2 (freq 2), 3 (freq 1). Evict min freq (3).
    assert(!cache_access_lfu(&cache, 4, false));

    // 3 should no longer be in the cache
    bool found_3 = false;
    for (int i = 0; i < 3; i++)
    {
        if (cache.blocks[i].page_id == 3)
            found_3 = true;
    }
    assert(!found_3);

    // Let's test tie-break (LRU)
    // Access 4: freq becomes 2 (since 4 was inserted with freq 1, then we access it)
    assert(cache_access_lfu(&cache, 4, false));
    // Access 1: freq becomes 3
    assert(cache_access_lfu(&cache, 1, false));
    // Current cache state page/frequency: 4/2, 1/3, 2/2.
    // Recency list: 2 (least recent), 4, 1.
    // Access 5: Miss. Frequencies: 4 (freq 2), 1 (freq 3), 2 (freq 2). Min freq are 4 and 2.
    // LRU tie break: 2 was accessed least recently, so evict 2.
    assert(!cache_access_lfu(&cache, 5, false));

    bool found_2 = false;
    for (int i = 0; i < 3; i++)
    {
        if (cache.blocks[i].page_id == 2)
            found_2 = true;
    }
    assert(!found_2);

    printf("LFU basic cache tests passed\n");
}

int main()
{
    test_fifo_basic();
    test_lru_basic();
    test_mru_basic();
    test_lfu_basic();
    printf("All cache simulator tests passed\n");
    return 0;
}
