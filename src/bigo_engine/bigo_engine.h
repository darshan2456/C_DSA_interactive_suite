#ifndef BIGO_ENGINE_H
#define BIGO_ENGINE_H

#include <stddef.h>

/**
 * Enum representing deduced asymptotic complexity classes.
 */
typedef enum {
    BIGO_CONSTANT,     // O(1) - Ratio ~1
    BIGO_LOGARITHMIC,  // O(log n) - Ratio ~1.something (varies by base)
    BIGO_LINEAR,       // O(n) - Ratio ~2
    BIGO_N_LOG_N,      // O(n log n) - Ratio ~2.x (between 2 and 4)
    BIGO_QUADRATIC,    // O(n²) - Ratio ~4
    BIGO_CUBIC,        // O(n³) - Ratio ~8
    BIGO_EXPONENTIAL,  // O(2^n) - Ratio very large (grows extremely fast)
    BIGO_UNKNOWN       // Unable to determine
} BigOComplexity;

/**
 * Structure to hold benchmark results at a specific input size.
 */
typedef struct {
    int input_size;
    double time_seconds;
    double stddev;
} BigOResult;

/**
 * Structure to hold comparison results between multiple input sizes.
 */
typedef struct {
    int size_n;
    int size_2n;
    double ratio;
    BigOComplexity deduced;
    double confidence;  // 0.0 to 1.0
} BigOComparison;

/**
 * Benchmark a sorting function at a given input size (1-arg signature: void func(int*, int)).
 */
double bigo_benchmark_sort(void (*sort_func)(int*, int), int* arr, int n, int iterations);

/**
 * Benchmark a search function at a given input size.
 */
double bigo_benchmark_search(void (*search_func)(int*, int, int), int* arr, int n, int target, int iterations);

/**
 * Calculate the time complexity ratio between T(2N) and T(N).
 */
double bigo_calculate_ratio(double time_2n, double time_n);

/**
 * Deduce the asymptotic complexity based on the time ratio.
 */
BigOComplexity bigo_deduce_complexity(double ratio, double tolerance);

/**
 * Get string representation of Big-O complexity.
 */
const char* bigo_complexity_to_string(BigOComplexity complexity);

/**
 * Run empirical Big-O verification for a sorting algorithm.
 */
BigOComparison bigo_verify_sorting(void (*sort_func)(int*, int), const char* algo_name, int base_n, int iterations);

/**
 * Run empirical Big-O verification for a search algorithm.
 */
BigOComparison bigo_verify_search(void (*search_func)(int*, int, int), const char* algo_name, int base_n, int iterations);

/**
 * Print a formatted Big-O verification table to stdout.
 */
void bigo_print_verification_table(const BigOComparison* comparison, const BigOResult* results, int result_count);

/**
 * Run the interactive Big-O verification demo menu.
 */
void bigo_engine_demo(void);

/**
 * Verify all sorting algorithms and print a summary table.
 */
void bigo_verify_all_sorting(int base_n, int iterations);

#endif // BIGO_ENGINE_H
