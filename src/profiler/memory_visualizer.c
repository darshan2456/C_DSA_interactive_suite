#include "memory_visualizer.h"
#include "memory_tracker.h"
#include <stdio.h>

void print_live_memory_card(void)
{
    size_t current = get_current_allocated_bytes();
    size_t peak = get_peak_allocated_bytes();
    size_t allocated = get_total_allocated_blocks();
    size_t freed = get_total_deallocated_blocks();
    double frag = get_memory_fragmentation_ratio();

    printf("\n┌──────────────────────────────────────────────────┐\n");
    printf("│             LIVE MEMORY PROFILE CARD             │\n");
    printf("├──────────────────────────────────────────────────┤\n");
    printf("│ Current Allocated : %-28zu │\n", current);
    printf("│ Peak Allocated    : %-28zu │\n", peak);
    printf("│ Active Blocks     : %-28zu │\n", allocated - freed);
    printf("│ Cumulative Alloc  : %-28zu │\n", allocated);
    printf("│ Cumulative Freed  : %-28zu │\n", freed);
    printf("│ Est. Fragmentation: %-27.2f%% │\n", frag * 100.0);
    printf("└──────────────────────────────────────────────────┘\n");
}

void print_ascii_memory_graph(size_t* values, int count)
{
    if (count <= 0)
        return;
    size_t max_val = 0;
    for (int i = 0; i < count; i++)
    {
        if (values[i] > max_val)
        {
            max_val = values[i];
        }
    }

    printf("\n--- Memory Consumption Trend (ASCII Graph) ---\n");
    for (int i = 0; i < count; i++)
    {
        printf("Step %02d: ", i + 1);
        int bar_len = 0;
        if (max_val > 0)
        {
            bar_len = (int)(((double)values[i] / max_val) * 40.0);
        }
        for (int j = 0; j < bar_len; j++)
        {
            printf("█");
        }
        for (int j = bar_len; j < 40; j++)
        {
            printf(" ");
        }
        printf(" %zu bytes\n", values[i]);
    }
    printf("----------------------------------------------\n");
}

void compare_algorithms_memory(const char* algo1_name, void (*algo1_func)(void*),
                               const char* algo2_name, void (*algo2_func)(void*), void* input_arg)
{
    size_t before1 = get_peak_allocated_bytes();
    algo1_func(input_arg);
    size_t after1 = get_peak_allocated_bytes();
    size_t peak1 = (after1 > before1) ? (after1 - before1) : 0;

    size_t before2 = get_peak_allocated_bytes();
    algo2_func(input_arg);
    size_t after2 = get_peak_allocated_bytes();
    size_t peak2 = (after2 > before2) ? (after2 - before2) : 0;

    printf("\n==================================================\n");
    printf("         MEMORY PROFILER: COMPARISON ENGINE        \n");
    printf("==================================================\n");
    printf("%-25s : %zu bytes (Peak)\n", algo1_name, peak1);
    printf("%-25s : %zu bytes (Peak)\n", algo2_name, peak2);
    printf("--------------------------------------------------\n");
    if (peak1 < peak2)
    {
        double diff = (peak2 > 0) ? ((double)(peak2 - peak1) / peak2 * 100.0) : 0.0;
        printf("%s is %.1f%% more memory efficient than %s.\n", algo1_name, diff, algo2_name);
    }
    else if (peak2 < peak1)
    {
        double diff = (peak1 > 0) ? ((double)(peak1 - peak2) / peak1 * 100.0) : 0.0;
        printf("%s is %.1f%% more memory efficient than %s.\n", algo2_name, diff, algo1_name);
    }
    else
    {
        printf("Both algorithms consumed the same amount of peak memory.\n");
    }
    printf("==================================================\n");
}
