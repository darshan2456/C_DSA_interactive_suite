#define _GNU_SOURCE

#include "benchmark.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int benchmark_iterations = 5;
BenchmarkFormat benchmark_output_format = FORMAT_CSV;

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
    if (QueryPerformanceCounter(&count) && QueryPerformanceFrequency(&freq) && freq.QuadPart > 0)
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

int benchmark_export_csv(const char* category_name, const char* algo_name, int input_size,
                         double time_seconds, size_t memory_kb)
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
        fprintf(csv_file,
                "Algorithm,Input Size,Execution Time (Seconds),Peak Memory (KB),Timestamp\n");
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
    fprintf(csv_file, "%s,%d,%.6f,%zu,%s\n", algo_name, input_size, time_seconds, memory_kb,
            timestamp);
    fclose(csv_file);

    return 0;
}

double benchmark_mean(const double* values, int count)
{
    if (count <= 0)
        return 0.0;
    double sum = 0.0;
    for (int i = 0; i < count; i++)
    {
        sum += values[i];
    }
    return sum / count;
}

static double simple_sqrt(double x)
{
    if (x <= 0.0)
        return 0.0;
    double z = x;
    for (int i = 0; i < 10; i++)
    {
        z = 0.5 * (z + x / z);
    }
    return z;
}

double benchmark_stddev(const double* values, int count, double mean)
{
    if (count <= 1)
        return 0.0;
    double variance_sum = 0.0;
    for (int i = 0; i < count; i++)
    {
        double diff = values[i] - mean;
        variance_sum += diff * diff;
    }
    return simple_sqrt(variance_sum / (count - 1));
}

int benchmark_export_markdown(const char* category, const char* name, int n, double time_sec,
                              size_t mem)
{
    char filepath[256];
    errno = 0;
    int dir_status = make_dir("benchmarks");
    if (dir_status != 0 && errno != EEXIST)
    {
        return -1;
    }

    snprintf(filepath, sizeof(filepath), "benchmarks/%s.md", category);
    FILE* check_file = fopen(filepath, "r");
    int is_new = (check_file == NULL);
    if (check_file)
    {
        fseek(check_file, 0, SEEK_END);
        if (ftell(check_file) == 0)
        {
            is_new = 1;
        }
        fclose(check_file);
    }

    FILE* file = fopen(filepath, "a");
    if (!file)
        return -1;
    if (is_new)
    {
        fprintf(file, "# Benchmark Report: %s\n\n", category);
        fprintf(file, "| Algorithm | Input Size | Time (Seconds) | Peak Memory (KB) |\n");
        fprintf(file, "| --- | --- | --- | --- |\n");
    }
    fprintf(file, "| %s | %d | %.6f | %zu |\n", name, n, time_sec, mem);
    fclose(file);
    return 0;
}

int benchmark_export_json(const char* category, const char* name, int n, double time_sec,
                          size_t mem)
{
    char filepath[256];
    errno = 0;
    int dir_status = make_dir("benchmarks");
    if (dir_status != 0 && errno != EEXIST)
    {
        return -1;
    }

    snprintf(filepath, sizeof(filepath), "benchmarks/%s.json", category);
    FILE* check_file = fopen(filepath, "r");
    int is_new = (check_file == NULL);
    if (check_file)
    {
        fseek(check_file, 0, SEEK_END);
        if (ftell(check_file) == 0)
        {
            is_new = 1;
        }
        fclose(check_file);
    }

    FILE* file = fopen(filepath, "a");
    if (!file)
        return -1;
    if (is_new)
    {
        fprintf(file, "[\n");
    }
    else
    {
        // To append cleanly to a JSON array structure, we can seek back past the closing bracket
        // "]\n" But for simplicity of append, we can also just write objects separated by commas or
        // write lines. Let's write standard JSON by tracking if we need a comma prefix.
    }

    // Let's write JSON lines format or simple records. Let's write standard objects.
    // If it's not new, we can write a comma.
    if (!is_new)
    {
        fprintf(file, ",\n");
    }
    fprintf(file,
            "  {\n    \"algorithm\": \"%s\",\n    \"input_size\": %d,\n    \"time_seconds\": "
            "%.6f,\n    \"peak_memory_kb\": %zu\n  }",
            name, n, time_sec, mem);
    fclose(file);
    return 0;
}

void benchmark_report_result(const char* category, const char* name, int n, const double times[],
                             size_t peak_mem)
{
    double mean = benchmark_mean(times, BENCHMARK_DEFAULT_ITERATIONS);
    double stddev = benchmark_stddev(times, BENCHMARK_DEFAULT_ITERATIONS, mean);

    char time_str[64];
    if (mean < 0.001)
    {
        snprintf(time_str, sizeof(time_str), "%.3f ± %.3f ms", mean * 1000.0, stddev * 1000.0);
    }
    else if (mean < 1.0)
    {
        snprintf(time_str, sizeof(time_str), "%.2f ± %.2f ms", mean * 1000.0, stddev * 1000.0);
    }
    else
    {
        snprintf(time_str, sizeof(time_str), "%.3f ± %.3f s", mean, stddev);
    }

    char mem_str[32];
    snprintf(mem_str, sizeof(mem_str), "%zu KB", peak_mem);

    printf("%-30s %-25s %-15s %-10s\n", name, time_str, mem_str, "PASSED");

    if (benchmark_output_format == FORMAT_CSV)
    {
        benchmark_export_csv(category, name, n, mean, peak_mem);
    }
    else if (benchmark_output_format == FORMAT_MARKDOWN)
    {
        benchmark_export_markdown(category, name, n, mean, peak_mem);
    }
    else if (benchmark_output_format == FORMAT_JSON)
    {
        benchmark_export_json(category, name, n, mean, peak_mem);
    }
}
