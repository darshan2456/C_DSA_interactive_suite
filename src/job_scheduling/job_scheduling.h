#ifndef JOB_SCHEDULING_H
#define JOB_SCHEDULING_H

// A single process / job as seen by the CPU schedulers. arrival and burst are
// supplied by the user; priority is only used by the priority schedulers. The
// remaining fields are filled in while an algorithm runs.
typedef struct Process
{
    int id;
    int arrival;
    int burst;
    int priority;
    int remaining;
    int completion;
    int turnaround;
    int waiting;
} Process;

// One contiguous stretch of CPU time spent on a single process, used to draw
// the Gantt chart. id is the process id, or -1 for an idle stretch.
typedef struct GanttSegment
{
    int id;
    int start;
    int end;
} GanttSegment;

// Upper bound on Gantt segments: at most one tick per unit of total burst
// (10 processes * 1000 max burst), plus idle stretches between them.
#define JS_MAX_SEGMENTS 20021

void job_scheduling_demo(void);
void fcfs_demo(void);
void sjf_demo(void);
void srtf_demo(void);
void priority_scheduling_demo(void);
void preemptive_priority_demo(void);
void round_robin_demo(void);

#ifdef TEST_MOCK_SCHEDULING
#define js_read_processes mock_js_read_processes
#define js_print_result mock_js_print_result
#define js_print_gantt mock_js_print_gantt
#endif

// Prompts for the process count and then each process's arrival and burst time
// (and priority when need_priority is non-zero), filling procs and *n. Returns
// 1 once a valid set has been read, or 0 if the user asked to exit (-1).
int js_read_processes(Process* procs, int* n, int need_priority);

// Prints the per-process completion / turnaround / waiting times followed by
// the average waiting and turnaround times.
void js_print_result(const Process* procs, int n);

// Appends one tick of CPU time on process id (or -1 for idle) at [time,
// time+1) to the segment list, merging it with the previous segment when it
// continues the same process so each contiguous run becomes a single bar.
void js_add_segment(GanttSegment* segments, int* count, int id, int time);

// Prints the recorded segments as an ASCII Gantt chart with a time axis.
void js_print_gantt(const GanttSegment* segments, int count);

#endif
