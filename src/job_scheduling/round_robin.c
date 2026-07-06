#include "job_scheduling.h"
#include "safe_input.h"
#include <stdio.h>
#include <time.h>

// Round Robin: processes share the CPU in fixed time slices (the quantum). A
// job that does not finish within its slice goes to the back of the ready
// queue. Jobs become ready at their arrival time; ties keep input order.
// note: the time measured by clock() covers the scheduling computation only (it
// starts after the quantum is read) and is for demonstration only, not a
// measure of the algorithm's efficiency.
void round_robin_demo(void)
{
    Process procs[10];
    int n;

    if (!js_read_processes(procs, &n, 0))
    {
        printf("\nExiting round robin demo....\n");
        return;
    }

    int quantum;

    while (1)
    {
        int quantum_status =
            safe_input_int(&quantum, "\nenter the time quantum (1 to 1000): ", 1, 1000);

        if (quantum_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting round robin demo....\n");
            return;
        }
        if (quantum_status == 0)
        {
            continue;
        }
        break;
    }

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();

    GanttSegment segments[JS_MAX_SEGMENTS];
    int segment_count = 0;

    // circular ready queue: at most n processes are queued at once, so n + 1
    // slots are always enough no matter how many times a job is requeued.
    int queue[11];
    int head = 0;
    int tail = 0;
    int capacity = n + 1;
    int in_queue[10] = {0};
    int completed = 0;
    int current_time = 0;

    // find the earliest arrival so the CPU starts there instead of at 0
    int earliest = procs[0].arrival;
    for (int i = 1; i < n; i++)
    {
        if (procs[i].arrival < earliest)
        {
            earliest = procs[i].arrival;
        }
    }
    current_time = earliest;

    // seed the queue with everything available at the start time, in input order
    for (int i = 0; i < n; i++)
    {
        if (procs[i].arrival <= current_time)
        {
            queue[tail] = i;
            tail = (tail + 1) % capacity;
            in_queue[i] = 1;
        }
    }

    while (completed < n)
    {
        if (head == tail)
        {
            // CPU idle: record idle ticks up to the next arrival, then enqueue it
            int next_arrival = -1;
            int next_index = -1;

            for (int i = 0; i < n; i++)
            {
                if (procs[i].remaining > 0 && !in_queue[i] &&
                    (next_arrival == -1 || procs[i].arrival < next_arrival))
                {
                    next_arrival = procs[i].arrival;
                    next_index = i;
                }
            }

            while (current_time < next_arrival)
            {
                js_add_segment(segments, &segment_count, -1, current_time);
                current_time++;
            }

            queue[tail] = next_index;
            tail = (tail + 1) % capacity;
            in_queue[next_index] = 1;
            continue;
        }

        int idx = queue[head];
        head = (head + 1) % capacity;

        int slice = procs[idx].remaining < quantum ? procs[idx].remaining : quantum;

        for (int t = 0; t < slice; t++)
        {
            js_add_segment(segments, &segment_count, procs[idx].id, current_time);
            current_time++;
        }

        procs[idx].remaining -= slice;

        // any process that arrived during this slice joins the ready queue
        for (int i = 0; i < n; i++)
        {
            if (procs[i].remaining > 0 && !in_queue[i] && procs[i].arrival <= current_time)
            {
                queue[tail] = i;
                tail = (tail + 1) % capacity;
                in_queue[i] = 1;
            }
        }

        if (procs[idx].remaining > 0)
        {
            // requeue the still-unfinished process behind the new arrivals
            queue[tail] = idx;
            tail = (tail + 1) % capacity;
        }
        else
        {
            procs[idx].completion = current_time;
            procs[idx].turnaround = procs[idx].completion - procs[idx].arrival;
            procs[idx].waiting = procs[idx].turnaround - procs[idx].burst;
            completed++;
        }
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    js_print_result(procs, n);
    js_print_gantt(segments, segment_count);
    printf("\ntotal CPU time taken for round robin scheduling:- %f seconds\n", total_t);
}
