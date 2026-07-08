#include "cache.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cache_simulator_demo(void)
{
    while (1)
    {
        display_header("Cache Replacement Simulator");

        int capacity;
        int cap_status =
            safe_input_int(&capacity, "\nEnter cache capacity (1 to 10), or '-1' to exit: ", 1,
                           CACHE_MAX_CAPACITY);
        if (cap_status == INPUT_EXIT_SIGNAL)
        {
            return;
        }
        if (cap_status == 0)
        {
            continue;
        }

        int algo_choice;
        int algo_status =
            safe_input_int(&algo_choice,
                           "\nSelect Cache Algorithm:\n1. FIFO\n2. LRU\n3. "
                           "MRU\n4. LFU (with aging)\nEnter choice (1 to 4), or '-1' to exit: ",
                           1, 4);
        if (algo_status == INPUT_EXIT_SIGNAL)
        {
            return;
        }
        if (algo_status == 0)
        {
            continue;
        }

        Cache cache;
        cache_init(&cache, capacity);

        char ref_str[256];
        int ref_status = safe_input_string(
            ref_str, "Enter page reference string (e.g., 1,2,3,4,1,2), or 'X' to exit: ");
        if (ref_status == INPUT_EXIT_SIGNAL)
        {
            return;
        }

        printf("\nSimulating %s Cache Replacement:\n",
               algo_choice == 1 ? "FIFO"
                                : (algo_choice == 2 ? "LRU" : (algo_choice == 3 ? "MRU" : "LFU")));
        printf("------------------------------------\n");

        char* token = strtok(ref_str, ", ");
        while (token != NULL)
        {
            int page_id = atoi(token);
            bool is_hit = false;
            if (algo_choice == 1)
            {
                is_hit = cache_access_fifo(&cache, page_id, false);
            }
            else if (algo_choice == 2)
            {
                is_hit = cache_access_lru(&cache, page_id, false);
            }
            else if (algo_choice == 3)
            {
                is_hit = cache_access_mru(&cache, page_id, false);
            }
            else
            {
                is_hit = cache_access_lfu(&cache, page_id, false);
            }
            printf("Access page %d -> %s | ", page_id, is_hit ? "🟢 HIT " : "🔴 MISS");
            cache_print_status(&cache);
            token = strtok(NULL, ", ");
        }

        printf("------------------------------------\n");
        printf("Simulation finished.\n");
        printf("Total Hits: %d, Total Misses (Page Faults): %d\n", cache.hits, cache.misses);
        if (cache.hits + cache.misses > 0)
        {
            printf("Hit Rate: %.2f%%\n", (double)cache.hits * 100 / (cache.hits + cache.misses));
        }

        printf("\nPress [ENTER] to continue...");
        getchar();
    }
}
