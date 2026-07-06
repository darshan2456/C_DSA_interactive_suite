#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_tracker.h"
#include "memory_visualizer.h"

void test_basic_allocation(void)
{
    init_memory_tracker();
    size_t initial_bytes = get_current_allocated_bytes();
    size_t initial_blocks = get_total_allocated_blocks();

    // Allocate
    int* ptr = (int*)malloc(100 * sizeof(int));
    assert(ptr != NULL);
    assert(get_current_allocated_bytes() == initial_bytes + 100 * sizeof(int));
    assert(get_total_allocated_blocks() == initial_blocks + 1);

    // Free
    free(ptr);
    assert(get_current_allocated_bytes() == initial_bytes);
    assert(get_total_deallocated_blocks() == initial_blocks + 1);

    printf("test_basic_allocation passed successfully.\n");
}

void test_fragmentation_calculation(void)
{
    // Allocate multiple blocks to test fragmentation ratio
    void* p1 = malloc(64);
    void* p2 = malloc(128);
    void* p3 = malloc(64);

    assert(p1 != NULL && p2 != NULL && p3 != NULL);

    double frag = get_memory_fragmentation_ratio();
    assert(frag >= 0.0 && frag <= 1.0);

    free(p1);
    free(p2);
    free(p3);

    printf("test_fragmentation_calculation passed successfully.\n");
}

void test_visualizer(void)
{
    print_live_memory_card();
    size_t samples[] = {100, 500, 200, 800, 400};
    print_ascii_memory_graph(samples, 5);
    printf("test_visualizer passed successfully.\n");
}

int main(void)
{
    printf("Starting Memory Profiler unit tests...\n");
    test_basic_allocation();
    test_fragmentation_calculation();
    test_visualizer();
    printf("All Memory Profiler unit tests passed successfully!\n");
    return 0;
}
