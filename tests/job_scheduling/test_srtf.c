#define TEST_MOCK_SCHEDULING
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/job_scheduling/job_scheduling.h"

// Globals to feed input and collect output
static Process g_input_procs[10];
static int g_input_count = 0;
static Process g_output_procs[10];
static int g_output_count = 0;

// Mock implementations of the interactive/UI functions
int mock_js_read_processes(Process* procs, int* n, int need_priority)
{
    (void)need_priority;
    for (int i = 0; i < g_input_count; i++)
    {
        procs[i] = g_input_procs[i];
        procs[i].remaining = procs[i].burst;
    }
    *n = g_input_count;
    return 1;
}

void mock_js_print_result(const Process* procs, int n)
{
    g_output_count = n;
    for (int i = 0; i < n; i++)
    {
        g_output_procs[i] = procs[i];
    }
}

void mock_js_print_gantt(const GanttSegment* segments, int count)
{
    (void)segments;
    (void)count;
}

// Stub for add_to_history to avoid linking dependency
void mock_add_to_history(const char* algorithm, int size, double execution_time)
{
    (void)algorithm;
    (void)size;
    (void)execution_time;
}

// Include the source file directly so the compiler compiles srtf_demo
// as part of this translation unit, bypassing duplicate symbol linking.
#include "../src/job_scheduling/srtf.c"

void test_srtf_basic()
{
    // Initialize input processes
    g_input_count = 4;
    
    // Process 1: id=1, arrival=0, burst=8
    g_input_procs[0].id = 1;
    g_input_procs[0].arrival = 0;
    g_input_procs[0].burst = 8;
    g_input_procs[0].priority = 0;
    
    // Process 2: id=2, arrival=1, burst=4
    g_input_procs[1].id = 2;
    g_input_procs[1].arrival = 1;
    g_input_procs[1].burst = 4;
    g_input_procs[1].priority = 0;
    
    // Process 3: id=3, arrival=2, burst=9
    g_input_procs[2].id = 3;
    g_input_procs[2].arrival = 2;
    g_input_procs[2].burst = 9;
    g_input_procs[2].priority = 0;

    // Process 4: id=4, arrival=3, burst=5
    g_input_procs[3].id = 4;
    g_input_procs[3].arrival = 3;
    g_input_procs[3].burst = 5;
    g_input_procs[3].priority = 0;

    // Run the scheduler demo (which will execute the mocked functions)
    srtf_demo();

    // Assert results
    assert(g_output_count == 4);
    
    // SRTF execution order:
    // [0-1]: P1 (P1 remaining becomes 7)
    // [1-5]: P2 runs to completion (P2 finishes at 5)
    // [5-10]: P4 runs to completion (P4 finishes at 10)
    // [10-17]: P1 runs to completion (P1 finishes at 17)
    // [17-26]: P3 runs to completion (P3 finishes at 26)

    // Find and verify each process's computed values
    for (int i = 0; i < g_output_count; i++)
    {
        if (g_output_procs[i].id == 1)
        {
            assert(g_output_procs[i].completion == 17);
            assert(g_output_procs[i].turnaround == 17);
            assert(g_output_procs[i].waiting == 9);
        }
        else if (g_output_procs[i].id == 2)
        {
            assert(g_output_procs[i].completion == 5);
            assert(g_output_procs[i].turnaround == 4);
            assert(g_output_procs[i].waiting == 0);
        }
        else if (g_output_procs[i].id == 3)
        {
            assert(g_output_procs[i].completion == 26);
            assert(g_output_procs[i].turnaround == 24);
            assert(g_output_procs[i].waiting == 15);
        }
        else if (g_output_procs[i].id == 4)
        {
            assert(g_output_procs[i].completion == 10);
            assert(g_output_procs[i].turnaround == 7);
            assert(g_output_procs[i].waiting == 2);
        }
    }
    printf("SRTF basic test passed\n");
}

int main()
{
    test_srtf_basic();
    printf("All SRTF tests passed\n");
    return 0;
}
