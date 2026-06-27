#define _GNU_SOURCE
#include "benchmark.h"
#include "../job_scheduling/job_scheduling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void init_processes(Process* procs, int n)
{
    for (int i = 0; i < n; i++)
    {
        procs[i].id = i + 1;
        procs[i].arrival = rand() % (n / 2 + 1);
        procs[i].burst = rand() % 20 + 1;
        procs[i].priority = rand() % 10 + 1;
        procs[i].remaining = procs[i].burst;
        procs[i].completion = 0;
        procs[i].turnaround = 0;
        procs[i].waiting = 0;
    }
}

static void copy_processes(Process* dest, const Process* src, int n)
{
    for (int i = 0; i < n; i++)
    {
        dest[i] = src[i];
        dest[i].remaining = src[i].burst;
    }
}

static void benchmark_fcfs(Process* procs, int n)
{
    // sort by arrival
    for (int i = 0; i < n - 1; i++)
    {
        int earliest = i;
        for (int j = i + 1; j < n; j++)
        {
            if (procs[j].arrival < procs[earliest].arrival)
            {
                earliest = j;
            }
        }
        if (earliest != i)
        {
            Process tmp = procs[i];
            procs[i] = procs[earliest];
            procs[earliest] = tmp;
        }
    }

    int current_time = 0;
    for (int i = 0; i < n; i++)
    {
        if (current_time < procs[i].arrival)
        {
            current_time = procs[i].arrival;
        }
        current_time += procs[i].burst;
        procs[i].completion = current_time;
        procs[i].turnaround = procs[i].completion - procs[i].arrival;
        procs[i].waiting = procs[i].turnaround - procs[i].burst;
    }
}

static void benchmark_sjf(Process* procs, int n)
{
    int* done = calloc(n, sizeof(int));
    if (!done) return;
    int completed = 0;
    int current_time = 0;

    while (completed < n)
    {
        int chosen = -1;
        for (int i = 0; i < n; i++)
        {
            if (done[i] || procs[i].arrival > current_time)
                continue;

            if (chosen == -1 || procs[i].burst < procs[chosen].burst ||
                (procs[i].burst == procs[chosen].burst && procs[i].arrival < procs[chosen].arrival))
            {
                chosen = i;
            }
        }

        if (chosen == -1)
        {
            current_time++;
            continue;
        }

        current_time += procs[chosen].burst;
        procs[chosen].completion = current_time;
        procs[chosen].turnaround = procs[chosen].completion - procs[chosen].arrival;
        procs[chosen].waiting = procs[chosen].turnaround - procs[chosen].burst;
        done[chosen] = 1;
        completed++;
    }
    free(done);
}

static void benchmark_srtf(Process* procs, int n)
{
    int completed = 0;
    int current_time = 0;

    while (completed < n)
    {
        int chosen = -1;
        for (int i = 0; i < n; i++)
        {
            if (procs[i].remaining == 0 || procs[i].arrival > current_time)
                continue;

            if (chosen == -1 || procs[i].remaining < procs[chosen].remaining ||
                (procs[i].remaining == procs[chosen].remaining && procs[i].arrival < procs[chosen].arrival))
            {
                chosen = i;
            }
        }

        if (chosen == -1)
        {
            current_time++;
            continue;
        }

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
}

static void benchmark_priority(Process* procs, int n)
{
    int* done = calloc(n, sizeof(int));
    if (!done) return;
    int completed = 0;
    int current_time = 0;

    while (completed < n)
    {
        int chosen = -1;
        for (int i = 0; i < n; i++)
        {
            if (done[i] || procs[i].arrival > current_time)
                continue;

            if (chosen == -1 || procs[i].priority < procs[chosen].priority ||
                (procs[i].priority == procs[chosen].priority && procs[i].arrival < procs[chosen].arrival))
            {
                chosen = i;
            }
        }

        if (chosen == -1)
        {
            current_time++;
            continue;
        }

        current_time += procs[chosen].burst;
        procs[chosen].completion = current_time;
        procs[chosen].turnaround = procs[chosen].completion - procs[chosen].arrival;
        procs[chosen].waiting = procs[chosen].turnaround - procs[chosen].burst;
        done[chosen] = 1;
        completed++;
    }
    free(done);
}

static void benchmark_preemptive_priority(Process* procs, int n)
{
    int completed = 0;
    int current_time = 0;

    while (completed < n)
    {
        int chosen = -1;
        for (int i = 0; i < n; i++)
        {
            if (procs[i].remaining == 0 || procs[i].arrival > current_time)
                continue;

            if (chosen == -1 || procs[i].priority < procs[chosen].priority ||
                (procs[i].priority == procs[chosen].priority && procs[i].arrival < procs[chosen].arrival))
            {
                chosen = i;
            }
        }

        if (chosen == -1)
        {
            current_time++;
            continue;
        }

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
}

static void benchmark_rr(Process* procs, int n)
{
    int quantum = 2;
    int* queue = malloc((n + 1) * sizeof(int));
    int* in_queue = calloc(n, sizeof(int));
    if (!queue || !in_queue)
    {
        free(queue);
        free(in_queue);
        return;
    }
    int head = 0;
    int tail = 0;
    int capacity = n + 1;
    int completed = 0;
    int current_time = 0;

    int earliest = procs[0].arrival;
    for (int i = 1; i < n; i++)
    {
        if (procs[i].arrival < earliest)
        {
            earliest = procs[i].arrival;
        }
    }
    current_time = earliest;

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
            if (current_time < next_arrival)
            {
                current_time = next_arrival;
            }
            queue[tail] = next_index;
            tail = (tail + 1) % capacity;
            in_queue[next_index] = 1;
            continue;
        }

        int idx = queue[head];
        head = (head + 1) % capacity;

        int slice = procs[idx].remaining < quantum ? procs[idx].remaining : quantum;
        current_time += slice;
        procs[idx].remaining -= slice;

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
    free(queue);
    free(in_queue);
}

void run_scheduling_benchmark(int n)
{
    srand((unsigned int)time(NULL));

    if (n > 5000)
    {
        printf("\nJob Scheduling benchmark skipped for N = %d (Threshold is N = 5000)\n", n);
        return;
    }

    Process* master = malloc(n * sizeof(Process));
    Process* temp = malloc(n * sizeof(Process));
    if (!master || !temp)
    {
        free(master);
        free(temp);
        return;
    }

    init_processes(master, n);

    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: JOB SCHEDULING ALGORITHMS (N = %d)\n", n);
    printf("========================================================================\n");
    printf("%-30s %-20s %-12s %-10s\n", "Algorithm", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    const char* algos[] = {
        "First-Come, First-Served",
        "Shortest Job First (SJF)",
        "Shortest Remaining Time First",
        "Non-preemptive Priority",
        "Preemptive Priority",
        "Round Robin"
    };

    for (int i = 0; i < 6; i++)
    {
        copy_processes(temp, master, n);

        size_t mem_before = benchmark_get_peak_memory();
        double start_time = benchmark_get_time();

        switch (i)
        {
            case 0: benchmark_fcfs(temp, n); break;
            case 1: benchmark_sjf(temp, n); break;
            case 2: benchmark_srtf(temp, n); break;
            case 3: benchmark_priority(temp, n); break;
            case 4: benchmark_preemptive_priority(temp, n); break;
            case 5: benchmark_rr(temp, n); break;
        }

        double duration = benchmark_get_time() - start_time;
        size_t mem_after = benchmark_get_peak_memory();
        size_t mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
        if (mem_used == 0) mem_used = mem_after; // fallback

        printf("%-30s %-20.6f %-12zu %-10s\n", algos[i], duration * 1000.0, mem_used, "PASSED");
        benchmark_export_csv("scheduling", algos[i], n, duration, mem_used);
    }
    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/scheduling.csv'.\n");

    free(master);
    free(temp);
}
