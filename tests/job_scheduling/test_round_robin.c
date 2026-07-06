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
static int g_mock_quantum = 2;

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

// Mock safe_input_int specifically to return our quantum value
#define safe_input_int mock_safe_input_int
int mock_safe_input_int(int* value, const char* prompt, int min, int max)
{
    (void)prompt;
    (void)min;
    (void)max;
    *value = g_mock_quantum;
    return 1;
}

// Include the source file directly so the compiler compiles round_robin_demo
// as part of this translation unit, bypassing duplicate symbol linking.
#include "../../src/job_scheduling/round_robin.c"

void test_round_robin_basic()
{
    // Initialize input processes
    g_input_count = 3;
    g_mock_quantum = 2;

    // Process 1: id=1, arrival=0, burst=4
    g_input_procs[0].id = 1;
    g_input_procs[0].arrival = 0;
    g_input_procs[0].burst = 4;
    g_input_procs[0].priority = 0;

    // Process 2: id=2, arrival=1, burst=5
    g_input_procs[1].id = 2;
    g_input_procs[1].arrival = 1;
    g_input_procs[1].burst = 5;
    g_input_procs[1].priority = 0;

    // Process 3: id=3, arrival=2, burst=2
    g_input_procs[2].id = 3;
    g_input_procs[2].arrival = 2;
    g_input_procs[2].burst = 2;
    g_input_procs[2].priority = 0;

    // Run the scheduler demo (which will execute the mocked functions)
    round_robin_demo();

    // Assert results
    assert(g_output_count == 3);

    // Round Robin execution order:
    // P1: [0-2] remaining=2
    // P2: [2-4] remaining=3
    // P3: [4-6] remaining=0 (P3 completes at 6)
    // P1: [6-8] remaining=0 (P1 completes at 8)
    // P2: [8-10] remaining=1
    // P2: [10-11] remaining=0 (P2 completes at 11)

    // Find and verify each process's computed values
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
            assert(g_output_procs[i].completion == 11);
            assert(g_output_procs[i].turnaround == 10);
            assert(g_output_procs[i].waiting == 5);
        }
        else if (g_output_procs[i].id == 3)
        {
            assert(g_output_procs[i].completion == 6);
            assert(g_output_procs[i].turnaround == 4);
            assert(g_output_procs[i].waiting == 2);
        }
    }
    printf("Round Robin basic test passed\n");
}

int main()
{
    test_round_robin_basic();
    printf("All Round Robin tests passed\n");
    return 0;
}
