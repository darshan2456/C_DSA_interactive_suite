#include "display_header.h"
#include "job_scheduling.h"
#include "safe_input.h"
#include <stdio.h>

#define JS_MAX_PROCESSES 10

int js_read_processes(Process* procs, int* n, int need_priority)
{
    int count;

    while (1)
    {
        int count_status = safe_input_int(
            &count, "\nenter the number of processes (between 1 and 10), enter '-1' to exit : ", 1,
            JS_MAX_PROCESSES);

        if (count_status == INPUT_EXIT_SIGNAL)
        {
            return 0;
        }

        if (count_status == 0)
        {
            continue;
        }

        break;
    }

    for (int i = 0; i < count; i++)
    {
        procs[i].id = i + 1;
        procs[i].priority = 0;

        printf("\nprocess P%d:\n", procs[i].id);

        while (1)
        {
            int arrival_status =
                safe_input_int(&procs[i].arrival, "  arrival time (0 to 1000): ", 0, 1000);

            if (arrival_status == INPUT_EXIT_SIGNAL)
            {
                return 0;
            }
            if (arrival_status == 0)
            {
                continue;
            }
            break;
        }

        while (1)
        {
            int burst_status =
                safe_input_int(&procs[i].burst, "  burst time (1 to 1000): ", 1, 1000);

            if (burst_status == INPUT_EXIT_SIGNAL)
            {
                return 0;
            }
            if (burst_status == 0)
            {
                continue;
            }
            break;
        }

        if (need_priority)
        {
            while (1)
            {
                int priority_status = safe_input_int(
                    &procs[i].priority,
                    "  priority (lower number = higher priority, 0 to 100): ", 0, 100);

                if (priority_status == INPUT_EXIT_SIGNAL)
                {
                    return 0;
                }
                if (priority_status == 0)
                {
                    continue;
                }
                break;
            }
        }

        procs[i].remaining = procs[i].burst;
    }

    *n = count;
    return 1;
}

void js_print_result(const Process* procs, int n)
{
    int total_waiting = 0;
    int total_turnaround = 0;

    printf("\nP\tarrival\tburst\tcompletion\tturnaround\twaiting\n");

    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t\t%d\t\t%d\n", procs[i].id, procs[i].arrival, procs[i].burst,
               procs[i].completion, procs[i].turnaround, procs[i].waiting);

        total_waiting += procs[i].waiting;
        total_turnaround += procs[i].turnaround;
    }

    printf("\naverage waiting time    : %.2f\n", (double)total_waiting / n);
    printf("average turnaround time : %.2f\n", (double)total_turnaround / n);
}

void js_add_segment(GanttSegment* segments, int* count, int id, int time)
{
    // extend the previous segment when the CPU keeps running the same process,
    // so a run of identical ticks collapses into a single bar
    if (*count > 0 && segments[*count - 1].id == id && segments[*count - 1].end == time)
    {
        segments[*count - 1].end = time + 1;
        return;
    }

    if (*count >= JS_MAX_SEGMENTS)
    {
        return;
    }

    segments[*count].id = id;
    segments[*count].start = time;
    segments[*count].end = time + 1;
    (*count)++;
}

void js_print_gantt(const GanttSegment* segments, int count)
{
    printf("\nGantt chart:\n");

    printf("|");
    for (int i = 0; i < count; i++)
    {
        if (segments[i].id == -1)
        {
            printf(" idle |");
        }
        else
        {
            printf(" P%d |", segments[i].id);
        }
    }
    printf("\n");

    // time axis: each boundary number sits under its bar separator. The gap
    // before a number is the bar width minus the width of the number printed
    // just before it, so multi-digit times stay aligned with the '|' above.
    int prev_width = printf("%d", segments[0].start);

    for (int i = 0; i < count; i++)
    {
        int bar_width = segments[i].id == -1 ? 7 : 5;
        int gap = bar_width - prev_width;

        for (int s = 0; s < gap; s++)
        {
            printf(" ");
        }

        prev_width = printf("%d", segments[i].end);
    }
    printf("\n");
}

void job_scheduling_demo(void)
{
    while (1)
    {
        display_header("Job Scheduling");

        int sched_choice;
        int sched_status =
            safe_input_int(&sched_choice,
                           "\n\nenter 1 for FCFS, 2 for SJF (non-preemptive), 3 for SRTF "
                           "(preemptive SJF), 4 for priority (non-preemptive), 5 for priority "
                           "(preemptive), 6 for round robin : ",
                           1, 6);

        if (sched_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting job scheduling demo....\n");
            return;
        }

        if (sched_status == 0)
        {
            continue;
        }

        switch (sched_choice)
        {
            case 1:
                display_header("FCFS");
                fcfs_demo();
                break;

            case 2:
                display_header("SJF (Non-Preemptive)");
                sjf_demo();
                break;

            case 3:
                display_header("SRTF (Preemptive SJF)");
                srtf_demo();
                break;

            case 4:
                display_header("Priority (Non-Preemptive)");
                priority_scheduling_demo();
                break;

            case 5:
                display_header("Priority (Preemptive)");
                preemptive_priority_demo();
                break;

            case 6:
                display_header("Round Robin");
                round_robin_demo();
                break;
        }
    }
}
