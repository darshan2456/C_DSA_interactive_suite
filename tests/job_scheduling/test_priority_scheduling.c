#define TEST_MOCK_SCHEDULING
#include "../../src/job_scheduling/job_scheduling.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Include the source file directly so the compiler compiles priority_scheduling_demo
// as part of this translation unit, bypassing duplicate symbol linking.
#include "../../src/job_scheduling/priority_scheduling.c"

void test_priority_scheduling_basic()
{
    // Initialize input processes
    g_input_count = 3;

    // Process 1: id=1, arrival=0, burst=4, priority=3
    g_input_procs[0].id = 1;
    g_input_procs[0].arrival = 0;
    g_input_procs[0].burst = 4;
    g_input_procs[0].priority = 3;

    // Process 2: id=2, arrival=1, burst=5, priority=1
    g_input_procs[1].id = 2;
    g_input_procs[1].arrival = 1;
    g_input_procs[1].burst = 5;
    g_input_procs[1].priority = 1;

    // Process 3: id=3, arrival=2, burst=2, priority=2
    g_input_procs[2].id = 3;
    g_input_procs[2].arrival = 2;
    g_input_procs[2].burst = 2;
    g_input_procs[2].priority = 2;

    // Run the scheduler demo (which will execute the mocked functions)
    priority_scheduling_demo();

    // Assert results
    assert(g_output_count == 3);

    // Priority non-preemptive execution order:
    // P1 runs [0-4] (finishes at 4)
    // P2 runs [4-9] (finishes at 9)
    // P3 runs [9-11] (finishes at 11)

    // Find and verify each process's computed values
    for (int i = 0; i < g_output_count; i++)
    {
        if (g_output_procs[i].id == 1)
        {
            assert(g_output_procs[i].completion == 4);
            assert(g_output_procs[i].turnaround == 4);
            assert(g_output_procs[i].waiting == 0);
        }
        else if (g_output_procs[i].id == 2)
        {
            assert(g_output_procs[i].completion == 9);
            assert(g_output_procs[i].turnaround == 8);
            assert(g_output_procs[i].waiting == 3);
        }
        else if (g_output_procs[i].id == 3)
        {
            assert(g_output_procs[i].completion == 11);
            assert(g_output_procs[i].turnaround == 9);
            assert(g_output_procs[i].waiting == 7);
        }
    }
    printf("Priority non-preemptive scheduling basic test passed\n");
}

int main()
{
    test_priority_scheduling_basic();
    printf("All Priority scheduling tests passed\n");
    return 0;
}