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
        int algo_status = safe_input_int(&algo_choice,
                                         "\nSelect Cache Algorithm:\n1. FIFO\n2. LRU\n3. "
                                         "MRU\n4. LFU (with aging)\n5. OPT (Belady's "
                                         "Optimal)\n6. Clock (Second Chance)\n7. Enhanced "
                                         "Clock\nEnter choice (1 to 7), or '-1' to exit: ",
                                         1, 7);
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

        // Parse reference string into an array first
        int ref_arr[256];
        int ref_len = 0;
        char ref_str_copy[256];
        strncpy(ref_str_copy, ref_str, sizeof(ref_str_copy) - 1);
        ref_str_copy[sizeof(ref_str_copy) - 1] = '\0';

        char* token = strtok(ref_str_copy, ", ");
        while (token != NULL && ref_len < 256)
        {
            ref_arr[ref_len++] = atoi(token);
            token = strtok(NULL, ", ");
        }

        printf(
            "\nSimulating %s Cache Replacement:\n",
            algo_choice == 1
                ? "FIFO"
                : (algo_choice == 2
                       ? "LRU"
                       : (algo_choice == 3
                              ? "MRU"
                              : (algo_choice == 4
                                     ? "LFU"
                                     : (algo_choice == 5
                                            ? "OPT"
                                            : (algo_choice == 6 ? "Clock" : "Enhanced Clock"))))));
        printf("------------------------------------\n");

        for (int i = 0; i < ref_len; i++)
        {
            int page_id = ref_arr[i];
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
            else if (algo_choice == 4)
            {
                is_hit = cache_access_lfu(&cache, page_id, false);
            }
            else if (algo_choice == 5)
            {
                is_hit = cache_access_opt(&cache, page_id, ref_arr, ref_len, i, false);
            }
            else if (algo_choice == 6)
            {
                is_hit = cache_access_clock(&cache, page_id, false);
            }
            else
            {
                is_hit = cache_access_enhanced_clock(&cache, page_id, false);
            }
            printf("Access page %d -> %s | ", page_id, is_hit ? "🟢 HIT " : "🔴 MISS");
            cache_print_status(&cache);
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
