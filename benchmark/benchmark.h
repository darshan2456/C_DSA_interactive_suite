#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stddef.h>

#ifdef _WIN32
#include <stdio.h>
int _fileno(FILE*);
#endif

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
int benchmark_export_csv(const char* category_name, const char* algo_name, int input_size,
                         double time_seconds, size_t memory_kb);

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
/**
 * Runs benchmarks for MST algorithms with input size V (vertices).
 */
void run_mst_benchmark(int v);
/**
 * Runs benchmarks for Job Scheduling algorithms with input size N.
 */
void run_scheduling_benchmark(int n);

/**
 * Runs benchmarks for String Matching algorithms with input size N.
 */
void run_strings_benchmark(int n);

/**
 * Runs benchmarks for Dynamic Programming algorithms with input size N.
 */
void run_dp_benchmark(int n);

/**
 * Runs benchmarks for Hash Map collision resolution methods with input size N.
 */
void run_hashing_benchmark(int n);

/**
 * Runs benchmarks for Tree lookups and insertions with input size N.
 */
void run_trees_benchmark(int n);

/**
 * Runs benchmarks for Backtracking algorithms with input size N.
 */
void run_backtracking_benchmark(int n);
/**
 * Runs benchmarks for Maximum Flow algorithms with input size V (vertices).
 */
void run_flow_benchmark(int v);

/**
 * Runs benchmarks for Advanced Heaps with input size N.
 */
void run_heaps_benchmark(int n);

#endif // BENCHMARK_H
