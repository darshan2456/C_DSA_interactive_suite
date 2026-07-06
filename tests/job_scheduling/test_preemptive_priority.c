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

// Include the source file directly so the compiler compiles preemptive_priority_demo
// as part of this translation unit, bypassing duplicate symbol linking.
#include "../../src/job_scheduling/preemptive_priority.c"

void test_preemptive_priority_basic()
{
    // Initialize input processes
    g_input_count = 3;

    // Process 1: id=1, arrival=0, burst=4, priority=3
    g_input_procs[0].id = 1;
    g_input_procs[0].arrival = 0;
    g_input_procs[0].burst = 4;
    g_input_procs[0].priority = 3;

    // Process 2: id=2, arrival=1, burst=2, priority=1
    g_input_procs[1].id = 2;
    g_input_procs[1].arrival = 1;
    g_input_procs[1].burst = 2;
    g_input_procs[1].priority = 1;

    // Process 3: id=3, arrival=2, burst=2, priority=2
    g_input_procs[2].id = 3;
    g_input_procs[2].arrival = 2;
    g_input_procs[2].burst = 2;
    g_input_procs[2].priority = 2;

    // Run the scheduler demo (which will execute the mocked functions)
    preemptive_priority_demo();

    // Assert results
    assert(g_output_count == 3);

    // Trace:
    // P1 runs [0-1) (remaining=3)
    // P2 preempts and runs [1-3) (completed)
    // P3 runs [3-5) (completed)
    // P1 resumes and runs [5-8) (completed)

    for (int i = 0; i < g_output_count; i++)
    {
        if (g_output_procs[i].id == 1)
        {
            assert(g_output_procs[i].completion == 8);
            assert(g_output_procs[i].turnaround == 8);
            assert(g_output_procs[i].waiting == 4);
        }
        else if (g_output_procs[i].id == 2)
        {
            assert(g_output_procs[i].completion == 3);
            assert(g_output_procs[i].turnaround == 2);
            assert(g_output_procs[i].waiting == 0);
        }
        else if (g_output_procs[i].id == 3)
        {
            assert(g_output_procs[i].completion == 5);
            assert(g_output_procs[i].turnaround == 3);
            assert(g_output_procs[i].waiting == 1);
        }
    }
    printf("Preemptive Priority scheduling basic test passed\n");
}

void test_preemptive_priority_ties_and_idle()
{
    // Initialize input processes
    g_input_count = 2;

    // Process 1: id=1, arrival=2, burst=2, priority=1
    g_input_procs[0].id = 1;
    g_input_procs[0].arrival = 2;
    g_input_procs[0].burst = 2;
    g_input_procs[0].priority = 1;

    // Process 2: id=2, arrival=3, burst=2, priority=1
    g_input_procs[1].id = 2;
    g_input_procs[1].arrival = 3;
    g_input_procs[1].burst = 2;
    g_input_procs[1].priority = 1;

    preemptive_priority_demo();

    assert(g_output_count == 2);

    // Trace:
    // Time [0-2): Idle
    // Time 2: P1 arrives and runs [2-3) (remaining=1)
    // Time 3: P2 arrives (priority=1, same as P1). P1 has earlier arrival, so P1 continues. P1 runs
    // [3-4) (completed) Time 4: P2 runs [4-6) (completed)

    for (int i = 0; i < g_output_count; i++)
    {
        if (g_output_procs[i].id == 1)
        {
            assert(g_output_procs[i].completion == 4);
            assert(g_output_procs[i].turnaround == 2);
            assert(g_output_procs[i].waiting == 0);
        }
        else if (g_output_procs[i].id == 2)
        {
            assert(g_output_procs[i].completion == 6);
            assert(g_output_procs[i].turnaround == 3);
            assert(g_output_procs[i].waiting == 1);
        }
    }
    printf("Preemptive Priority scheduling ties and idle test passed\n");
}

int main()
{
    test_preemptive_priority_basic();
    test_preemptive_priority_ties_and_idle();
    printf("All Preemptive Priority scheduling tests passed\n");
    return 0;
}
