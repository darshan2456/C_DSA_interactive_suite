#include "history_logger.h"
#include "job_scheduling.h"
#include <stdio.h>
#include <time.h>

// Preemptive priority scheduling: at every time unit run the arrived,
// unfinished job with the highest priority (lowest priority number) for one
// tick, so a higher-priority arrival preempts the running job. Ties are broken
// by earlier arrival, then input order, so an equal-priority arrival does not
// preempt the running job.
// note: the time measured by clock() covers the scheduling computation only and
// is for demonstration only, not a measure of the algorithm's efficiency.
void preemptive_priority_demo(void)
{
    Process procs[10];
    int n;

    if (!js_read_processes(procs, &n, 1))
    {
        printf("\nExiting preemptive priority demo....\n");
        return;
    }

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();

    GanttSegment segments[JS_MAX_SEGMENTS];
    int segment_count = 0;
    int completed = 0;
    int current_time = 0;

    while (completed < n)
    {
        int chosen = -1;

        for (int i = 0; i < n; i++)
        {
            if (procs[i].remaining == 0 || procs[i].arrival > current_time)
            {
                continue;
            }

            if (chosen == -1 || procs[i].priority < procs[chosen].priority ||
                (procs[i].priority == procs[chosen].priority &&
                 procs[i].arrival < procs[chosen].arrival))
            {
                chosen = i;
            }
        }

        if (chosen == -1)
        {
            // nothing has arrived yet, idle one tick
            js_add_segment(segments, &segment_count, -1, current_time);
            current_time++;
            continue;
        }

        js_add_segment(segments, &segment_count, procs[chosen].id, current_time);
        procs[chosen].remaining--;
        current_time++;

        if (procs[chosen].remaining == 0)
        {
            procs[chosen].completion = current_time;
            procs[chosen].turnaround = procs[chosen].completion - procs[chosen].arrival;
            procs[chosen].waiting = procs[chosen].turnaround - procs[chosen].burst;
            completed++;
        }
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    js_print_result(procs, n);
    js_print_gantt(segments, segment_count);
    printf("\ntotal CPU time taken for preemptive priority scheduling:- %f seconds\n", total_t);
    add_to_history("Preemptive Priority Scheduling", n, total_t);
}
