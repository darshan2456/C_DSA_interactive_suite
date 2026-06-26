#define _GNU_SOURCE

#include "benchmark.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#include <psapi.h>
#include <windows.h>
#define make_dir(path) _mkdir(path)
#else
#include <sys/resource.h>
#include <sys/stat.h>
#define make_dir(path) mkdir(path, 0777)
#endif

double benchmark_get_time(void)
{
#ifdef _WIN32
    LARGE_INTEGER count, freq;
    if (QueryPerformanceCounter(&count) && QueryPerformanceFrequency(&freq))
    {
        return (double)count.QuadPart / (double)freq.QuadPart;
    }
    return 0.0;
#else
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
    {
        return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
    }
    return 0.0;
#endif
}

size_t benchmark_get_peak_memory(void)
{
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        return (size_t)(pmc.PeakWorkingSetSize / 1024);
    }
    return 0;
#else
    struct rusage r_usage;
    if (getrusage(RUSAGE_SELF, &r_usage) == 0)
    {
#ifdef __APPLE__
        // macOS maxrss is in bytes
        return (size_t)(r_usage.ru_maxrss / 1024);
#else
        // Linux maxrss is in kilobytes
        return (size_t)r_usage.ru_maxrss;
#endif
    }
    return 0;
#endif
}

int benchmark_export_csv(const char* category_name, const char* algo_name, int input_size, double time_seconds, size_t memory_kb)
{
    char filepath[256];
    FILE* check_file = NULL;
    FILE* csv_file = NULL;
    int is_new = 0;
    time_t raw_time;
    struct tm* time_info;
    char timestamp[30];

    // Create benchmarks directory
    errno = 0;
    int dir_status = make_dir("benchmarks");
    if (dir_status != 0 && errno != EEXIST)
    {
        fprintf(stderr, "\nError: Unable to create benchmarks directory.\n");
        return -1;
    }

    // Format filepath safely
    snprintf(filepath, sizeof(filepath), "benchmarks/%s.csv", category_name);

    // Check if file is new or empty
    check_file = fopen(filepath, "r");
    if (check_file == NULL)
    {
        is_new = 1;
    }
    else
    {
        fseek(check_file, 0, SEEK_END);
        if (ftell(check_file) == 0)
        {
            is_new = 1;
        }
        fclose(check_file);
    }

    // Open file in append mode
    csv_file = fopen(filepath, "a");
    if (csv_file == NULL)
    {
        fprintf(stderr, "\nError: Unable to write to benchmark log %s\n", filepath);
        return -1;
    }

    // Write header if new file
    if (is_new)
    {
        fprintf(csv_file, "Algorithm,Input Size,Execution Time (Seconds),Peak Memory (KB),Timestamp\n");
    }

    // Format timestamp
    raw_time = time(NULL);
    time_info = localtime(&raw_time);
    if (time_info != NULL)
    {
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);
    }
    else
    {
        strncpy(timestamp, "N/A", sizeof(timestamp));
    }

    // Write data row
    fprintf(csv_file, "%s,%d,%.6f,%zu,%s\n", algo_name, input_size, time_seconds, memory_kb, timestamp);
    fclose(csv_file);

    return 0;
}
