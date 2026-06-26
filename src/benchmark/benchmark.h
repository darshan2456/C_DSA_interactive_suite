#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stddef.h>

/**
 * Returns a high-resolution monotonic timestamp in seconds.
 * Suitable for measuring code execution duration.
 */
double benchmark_get_time(void);

/**
 * Returns the peak resident set size (RSS) of the current process in kilobytes.
 * Handles platform differences (POSIX / Windows / macOS).
 */
size_t benchmark_get_peak_memory(void);

/**
 * Appends a benchmark result row to a CSV file named "benchmarks/<category_name>.csv".
 * Creates the file and directory if they do not exist.
 *
 * @param category_name Name of the CSV file (e.g. "sorting", "graphs").
 * @param algo_name Name of the benchmarked algorithm (e.g. "Bubble Sort").
 * @param input_size Size of the input dataset (N).
 * @param time_seconds Time taken by the algorithm in seconds.
 * @param memory_kb Peak RAM consumed by the process in KB.
 * @return 0 on success, -1 on failure.
 */
int benchmark_export_csv(const char* category_name, const char* algo_name, int input_size, double time_seconds, size_t memory_kb);

/**
 * Interactive benchmark menu interface.
 * Allows selecting the category of algorithms to benchmark and the input size N.
 */
void benchmark_menu_demo(void);

/**
 * Runs benchmarks for sorting algorithms with input size N.
 */
void run_sorting_benchmark(int n);
/**
 * Runs benchmarks for searching algorithms with input size N.
 */
void run_searching_benchmark(int n);
/**
 * Runs benchmarks for Graph Shortest Path algorithms with input size V (vertices).
 */
void run_graphs_benchmark(int v);
#endif // BENCHMARK_H
