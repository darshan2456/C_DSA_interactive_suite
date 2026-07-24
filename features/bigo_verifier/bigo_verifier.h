#ifndef BIGO_VERIFIER_H
#define BIGO_VERIFIER_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_BIGO_SAMPLES 6

typedef enum
{
    BIGO_O1,
    BIGO_OLOGN,
    BIGO_ON,
    BIGO_ONLOGN,
    BIGO_ON2,
    BIGO_ON3,
    BIGO_UNKNOWN
} BigOComplexity;

typedef struct
{
    int n;
    double time_ms;
} BigOSample;

typedef struct
{
    int n_start;
    int n_end;
    double ratio;
    BigOComplexity deduced_class;
} BigORatio;

typedef struct
{
    const char* algorithm_name;
    const char* theoretical_complexity;
    int sample_count;
    BigOSample samples[MAX_BIGO_SAMPLES];
    int ratio_count;
    BigORatio ratios[MAX_BIGO_SAMPLES - 1];
    BigOComplexity overall_empirical_class;
    const char* overall_empirical_str;
    bool matches_theoretical;
} BigOReport;

typedef void (*AlgFunction)(int* data, int n);

/**
 * Get string representation of BigOComplexity enum.
 */
const char* bigo_complexity_to_string(BigOComplexity complexity);

/**
 * Deduce empirical Big-O complexity class from growth ratio T(2N) / T(N) at problem size N.
 *
 * @param ratio Observed execution time ratio T(2N) / T(N).
 * @param n Current base problem size N.
 * @return Deduced BigOComplexity enum.
 */
BigOComplexity deduce_bigo_from_ratio(double ratio, int n);

/**
 * Run empirical Big-O profiling on a given algorithm function across scaling input sizes.
 *
 * @param alg_name Human readable algorithm name.
 * @param theoretical_str Theoretical complexity string (e.g. "O(N^2)").
 * @param func Function pointer to the algorithm under test.
 * @param base_n Starting problem size N.
 * @param samples_to_run Number of scaling samples (e.g. 4 for N, 2N, 4N, 8N).
 * @param report Output report struct to populate.
 * @return True on success, false on error.
 */
bool run_bigo_profile(const char* alg_name, const char* theoretical_str, AlgFunction func,
                      int base_n, int samples_to_run, BigOReport* report);

/**
 * Print a formatted text-based Big-O empirical verification report table.
 *
 * @param report Pointer to the populated BigOReport.
 */
void print_bigo_report(const BigOReport* report);

// Sorting & Searching Algorithm Profilers
bool profile_bubble_sort_bigo(int base_n, BigOReport* report);
bool profile_insertion_sort_bigo(int base_n, BigOReport* report);
bool profile_selection_sort_bigo(int base_n, BigOReport* report);
bool profile_quick_sort_bigo(int base_n, BigOReport* report);
bool profile_merge_sort_bigo(int base_n, BigOReport* report);
bool profile_linear_search_bigo(int base_n, BigOReport* report);
bool profile_binary_search_bigo(int base_n, BigOReport* report);

// Graph, Tree & DP Algorithm Profilers
bool profile_bfs_graph_bigo(int base_n, BigOReport* report);
bool profile_bst_operations_bigo(int base_n, BigOReport* report);
bool profile_dp_fibonacci_bigo(int base_n, BigOReport* report);

/**
 * Interactive Big-O Complexity Verifier CLI/TUI Dashboard demo suite.
 */
void bigo_verifier_demo(void);

#endif /* BIGO_VERIFIER_H */
