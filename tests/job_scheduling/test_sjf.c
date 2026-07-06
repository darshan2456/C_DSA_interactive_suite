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

// Include the source file directly so the compiler compiles sjf_demo
// as part of this translation unit, bypassing duplicate symbol linking.
#include "../../src/job_scheduling/sjf.c"

void test_sjf_basic()
{
    // Initialize input processes
    g_input_count = 4;

    // Process 1: id=1, arrival=0, burst=6
    g_input_procs[0].id = 1;
    g_input_procs[0].arrival = 0;
    g_input_procs[0].burst = 6;
    g_input_procs[0].priority = 0;

    // Process 2: id=2, arrival=2, burst=2
    g_input_procs[1].id = 2;
    g_input_procs[1].arrival = 2;
    g_input_procs[1].burst = 2;
    g_input_procs[1].priority = 0;

    // Process 3: id=3, arrival=4, burst=1
    g_input_procs[2].id = 3;
    g_input_procs[2].arrival = 4;
    g_input_procs[2].burst = 1;
    g_input_procs[2].priority = 0;

    // Process 4: id=4, arrival=5, burst=4
    g_input_procs[3].id = 4;
    g_input_procs[3].arrival = 5;
    g_input_procs[3].burst = 4;
    g_input_procs[3].priority = 0;

    // Run the scheduler demo (which will execute the mocked functions)
    sjf_demo();

    // Assert results
    assert(g_output_count == 4);

    // SJF execution order should be P1, P3, P2, P4
    // P1: starts at 0, finishes at 6. Wait = 0, Turnaround = 6
    // P3: starts at 6, finishes at 7. Wait = 2, Turnaround = 3
    // P2: starts at 7, finishes at 9. Wait = 5, Turnaround = 7
    // P4: starts at 9, finishes at 13. Wait = 4, Turnaround = 8

    // Find and verify each process's computed values
    for (int i = 0; i < g_output_count; i++)
    {
        if (g_output_procs[i].id == 1)
        {
            assert(g_output_procs[i].completion == 6);
            assert(g_output_procs[i].turnaround == 6);
            assert(g_output_procs[i].waiting == 0);
        }
        else if (g_output_procs[i].id == 2)
        {
            assert(g_output_procs[i].completion == 9);
            assert(g_output_procs[i].turnaround == 7);
            assert(g_output_procs[i].waiting == 5);
        }
        else if (g_output_procs[i].id == 3)
        {
            assert(g_output_procs[i].completion == 7);
            assert(g_output_procs[i].turnaround == 3);
            assert(g_output_procs[i].waiting == 2);
        }
        else if (g_output_procs[i].id == 4)
        {
            assert(g_output_procs[i].completion == 13);
            assert(g_output_procs[i].turnaround == 8);
            assert(g_output_procs[i].waiting == 4);
        }
    }
    printf("SJF basic test passed\n");
}

int main()
{
    test_sjf_basic();
    printf("All SJF tests passed\n");
    return 0;
}
