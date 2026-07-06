#include "bigo_engine.h"
#include "utils/config.h"
#include "utils/display_header.h"
#include "utils/safe_input.h"
#include "benchmark.h"
#include "sorting_algorithms_n2/sorting_algorithms_n2.h"
#include "advanced_sorting_algorithms/advanced_sorting.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BASE_N 1000
#define DEFAULT_ITERATIONS 5
#define MIN_BASE_N 100

typedef void (*sort_func_1arg)(int*, int);

static void quick_sort_wrapper(int* arr, int n)
{
    quicksort(arr, 0, n - 1);
}

double bigo_benchmark_sort(sort_func_1arg sort_func, int* arr, int n, int iterations)
{
    double times[BENCHMARK_DEFAULT_ITERATIONS];
    int* clone = malloc(n * sizeof(int));
    if (clone == NULL)
    {
        return -1.0;
    }

    for (int iter = 0; iter < iterations; iter++)
    {
        memcpy(clone, arr, n * sizeof(int));
        double start = benchmark_get_time();
        sort_func(clone, n);
        double end = benchmark_get_time();
        times[iter] = end - start;
    }

    double mean = benchmark_mean(times, iterations);
    free(clone);
    return mean;
}

double bigo_benchmark_search(void (*search_func)(int*, int, int), int* arr, int n, int target, int iterations)
{
    double times[BENCHMARK_DEFAULT_ITERATIONS];
    int* clone = malloc(n * sizeof(int));
    if (clone == NULL)
    {
        return -1.0;
    }

    for (int iter = 0; iter < iterations; iter++)
    {
        memcpy(clone, arr, n * sizeof(int));
        double start = benchmark_get_time();
        search_func(clone, n, target);
        double end = benchmark_get_time();
        times[iter] = end - start;
    }

    double mean = benchmark_mean(times, iterations);
    free(clone);
    return mean;
}

double bigo_calculate_ratio(double time_2n, double time_n)
{
    if (time_n <= 0.0 || time_2n <= 0.0)
    {
        return 0.0;
    }
    return time_2n / time_n;
}

BigOComplexity bigo_deduce_complexity(double ratio, double tolerance)
{
    (void)tolerance;
    if (ratio <= 0.0)
    {
        return BIGO_UNKNOWN;
    }

    if (ratio < 1.5)
    {
        return BIGO_CONSTANT;
    }

    if (ratio < 2.5)
    {
        return BIGO_LINEAR;
    }

    if (ratio < 3.5)
    {
        return BIGO_N_LOG_N;
    }

    if (ratio < 6.0)
    {
        return BIGO_QUADRATIC;
    }

    if (ratio < 10.0)
    {
        return BIGO_CUBIC;
    }

    return BIGO_EXPONENTIAL;
}

const char* bigo_complexity_to_string(BigOComplexity complexity)
{
    switch (complexity)
    {
        case BIGO_CONSTANT:
            return "O(1)";
        case BIGO_LOGARITHMIC:
            return "O(log n)";
        case BIGO_LINEAR:
            return "O(n)";
        case BIGO_N_LOG_N:
            return "O(n log n)";
        case BIGO_QUADRATIC:
            return "O(n^2)";
        case BIGO_CUBIC:
            return "O(n^3)";
        case BIGO_EXPONENTIAL:
            return "O(2^n)";
        case BIGO_UNKNOWN:
        default:
            return "Unknown";
    }
}

static double calculate_confidence(double ratio, BigOComplexity deduced)
{
    double expected_ratio;
    switch (deduced)
    {
        case BIGO_CONSTANT:
            expected_ratio = 1.0;
            break;
        case BIGO_LINEAR:
            expected_ratio = 2.0;
            break;
        case BIGO_N_LOG_N:
            expected_ratio = 2.14;
            break;
        case BIGO_QUADRATIC:
            expected_ratio = 4.0;
            break;
        case BIGO_CUBIC:
            expected_ratio = 8.0;
            break;
        default:
            expected_ratio = ratio;
            break;
    }

    double diff = fabs(ratio - expected_ratio);
    double normalized_diff = diff / expected_ratio;

    if (normalized_diff < 0.15)
        return 0.95;
    if (normalized_diff < 0.30)
        return 0.80;
    if (normalized_diff < 0.50)
        return 0.60;
    return 0.40;
}

static void generate_random_array(int* arr, int n, int seed)
{
    srand(seed);
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 100000;
    }
}

BigOComparison bigo_verify_sorting(sort_func_1arg sort_func, const char* algo_name, int base_n, int iterations)
{
    (void)algo_name;
    BigOComparison result = {0};

    if (base_n < MIN_BASE_N)
    {
        base_n = MIN_BASE_N;
    }

    int sizes[3] = {base_n, base_n * 2, base_n * 4};
    double times[3] = {0.0, 0.0, 0.0};
    int seed = BENCHMARK_SEED;

    for (int i = 0; i < 3; i++)
    {
        int* arr = malloc(sizes[i] * sizeof(int));
        if (arr == NULL)
        {
            continue;
        }

        generate_random_array(arr, sizes[i], seed);
        times[i] = bigo_benchmark_sort(sort_func, arr, sizes[i], iterations);
        free(arr);
    }

    result.size_n = base_n;
    result.size_2n = base_n * 2;

    if (times[0] > 0.0 && times[1] > 0.0)
    {
        result.ratio = bigo_calculate_ratio(times[1], times[0]);
        result.deduced = bigo_deduce_complexity(result.ratio, 0.3);
        result.confidence = calculate_confidence(result.ratio, result.deduced);
    }
    else
    {
        result.ratio = 0.0;
        result.deduced = BIGO_UNKNOWN;
        result.confidence = 0.0;
    }

    return result;
}

BigOComparison bigo_verify_search(void (*search_func)(int*, int, int), const char* algo_name, int base_n, int iterations)
{
    (void)algo_name;
    BigOComparison result = {0};

    if (base_n < MIN_BASE_N)
    {
        base_n = MIN_BASE_N;
    }

    int sizes[3] = {base_n, base_n * 2, base_n * 4};
    double times[3] = {0.0, 0.0, 0.0};
    int seed = BENCHMARK_SEED;

    for (int i = 0; i < 3; i++)
    {
        int* arr = malloc(sizes[i] * sizeof(int));
        if (arr == NULL)
        {
            continue;
        }

        generate_random_array(arr, sizes[i], seed);
        times[i] = bigo_benchmark_search(search_func, arr, sizes[i], arr[sizes[i] / 2], iterations);
        free(arr);
    }

    result.size_n = base_n;
    result.size_2n = base_n * 2;

    if (times[0] > 0.0 && times[1] > 0.0)
    {
        result.ratio = bigo_calculate_ratio(times[1], times[0]);
        result.deduced = bigo_deduce_complexity(result.ratio, 0.3);
        result.confidence = calculate_confidence(result.ratio, result.deduced);
    }
    else
    {
        result.ratio = 0.0;
        result.deduced = BIGO_UNKNOWN;
        result.confidence = 0.0;
    }

    return result;
}

void bigo_print_verification_table(const BigOComparison* comparison, const BigOResult* results, int result_count)
{
    (void)results;
    (void)result_count;

    printf("\n");
    printf("========================================\n");
    printf("     Big-O Verification Results\n");
    printf("========================================\n");
    printf("  Input Size N:      %d\n", comparison->size_n);
    printf("  Input Size 2N:     %d\n", comparison->size_2n);
    printf("\n");

    if (comparison->ratio > 0)
    {
        printf("  T(2N)/T(N) Ratio:  %.2f\n", comparison->ratio);
    }
    else
    {
        printf("  T(2N)/T(N) Ratio:  N/A\n");
    }

    printf("\n");
    printf("  Empirical Complexity: %s\n", bigo_complexity_to_string(comparison->deduced));
    printf("  Confidence Level:     %.0f%%\n", comparison->confidence * 100);
    printf("\n");

    printf("  Complexity Thresholds:\n");
    printf("    Ratio ~2.0  => O(n)\n");
    printf("    Ratio ~2.x  => O(n log n)\n");
    printf("    Ratio ~4.0  => O(n^2)\n");
    printf("    Ratio ~8.0  => O(n^3)\n");
    printf("\n");
    printf("NOTE: For accurate results, run multiple times.\n");
    printf("      Different hardware affects timing.\n");
    printf("\n");
}

void bigo_verify_all_sorting(int base_n, int iterations)
{
    printf("\n");
    printf("================================================================================\n");
    printf("           EMPIRICAL BIG-O VERIFICATION: ALL SORTING ALGORITHMS\n");
    printf("================================================================================\n");
    printf("%-20s %-15s %-15s %-10s %-10s\n", "Algorithm", "Theoretical", "Empirical", "Ratio", "Status");
    printf("--------------------------------------------------------------------------------\n");

    typedef struct {
        const char* name;
        sort_func_1arg func;
        const char* theoretical;
        BigOComplexity expected;
    } SortAlgo;

    SortAlgo algos[] = {
        {"Bubble Sort", bubble_sort_optimized, "O(n^2)", BIGO_QUADRATIC},
        {"Selection Sort", selection_sort, "O(n^2)", BIGO_QUADRATIC},
        {"Insertion Sort", insertion_sort, "O(n^2)", BIGO_QUADRATIC},
        {"Shell Sort", shell_sort, "O(n log n)", BIGO_N_LOG_N},
        {"Merge Sort", merge_sort, "O(n log n)", BIGO_N_LOG_N},
        {"Quick Sort", quick_sort_wrapper, "O(n log n)", BIGO_N_LOG_N},
        {"Heap Sort", heap_sort, "O(n log n)", BIGO_N_LOG_N},
    };

    int algo_count = sizeof(algos) / sizeof(algos[0]);

    for (int i = 0; i < algo_count; i++)
    {
        if (base_n > 5000 && algos[i].expected == BIGO_QUADRATIC)
        {
            printf("%-20s %-15s %-15s %-10s %-10s\n", algos[i].name, algos[i].theoretical, "Skipped", "N/A", "SKIP*");
            continue;
        }

        BigOComparison result = bigo_verify_sorting(algos[i].func, algos[i].name, base_n, iterations);

        const char* empirical = bigo_complexity_to_string(result.deduced);
        char ratio_str[16];
        if (result.ratio > 0)
        {
            snprintf(ratio_str, sizeof(ratio_str), "%.2f", result.ratio);
        }
        else
        {
            snprintf(ratio_str, sizeof(ratio_str), "N/A");
        }

        const char* status = "VERIFIED";
        if (result.deduced != algos[i].expected && result.deduced != BIGO_UNKNOWN)
        {
            status = "MISMATCH";
        }
        else if (result.deduced == BIGO_UNKNOWN)
        {
            status = "ERROR";
        }

        printf("%-20s %-15s %-15s %-10s %-10s\n", algos[i].name, algos[i].theoretical, empirical, ratio_str, status);
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("* O(n^2) algorithms skipped for large N to prevent long wait times.\n");
    printf("\n");
    printf("Legend:\n");
    printf("  VERIFIED  - Empirical complexity matches theoretical\n");
    printf("  MISMATCH  - Empirical complexity differs from theoretical\n");
    printf("  ERROR     - Could not determine complexity\n");
    printf("\n");
    printf("Note: Small input sizes may show imprecise ratios due to timing resolution.\n");
    printf("      For accurate results, use larger base N values.\n");
    printf("\n");
}

static void bigo_engine_single_algo_demo(void)
{
    printf("\n");
    printf("Select an algorithm to verify:\n");
    printf("1. Bubble Sort (O(n^2))\n");
    printf("2. Selection Sort (O(n^2))\n");
    printf("3. Insertion Sort (O(n^2))\n");
    printf("4. Shell Sort (O(n log n))\n");
    printf("5. Merge Sort (O(n log n))\n");
    printf("6. Quick Sort (O(n log n))\n");
    printf("7. Heap Sort (O(n log n))\n");
    printf("\n");

    int choice;
    int status = safe_input_int(&choice, "Enter choice: ", 1, 7);
    if (status != 0)
    {
        return;
    }

    printf("\n");
    int base_n;
    status = safe_input_int(&base_n, "Enter base input size N (min 100): ", 100, 50000);
    if (status != 0)
    {
        return;
    }

    typedef struct {
        const char* name;
        sort_func_1arg func;
    } SortAlgo;

    SortAlgo algos[] = {
        {"Bubble Sort", bubble_sort_optimized},
        {"Selection Sort", selection_sort},
        {"Insertion Sort", insertion_sort},
        {"Shell Sort", shell_sort},
        {"Merge Sort", merge_sort},
        {"Quick Sort", quick_sort_wrapper},
        {"Heap Sort", heap_sort},
    };

    int iterations = DEFAULT_ITERATIONS;
    printf("\nRunning Big-O verification for %s with N=%d...\n", algos[choice - 1].name, base_n);
    printf("(This may take a few seconds)\n\n");

    BigOResult results[3];
    int sizes[3] = {base_n, base_n * 2, base_n * 4};

    for (int i = 0; i < 3; i++)
    {
        int* arr = malloc(sizes[i] * sizeof(int));
        if (arr == NULL)
        {
            continue;
        }

        srand(BENCHMARK_SEED);
        for (int j = 0; j < sizes[i]; j++)
        {
            arr[j] = rand() % 100000;
        }

        double time = bigo_benchmark_sort(algos[choice - 1].func, arr, sizes[i], iterations);
        results[i].input_size = sizes[i];
        results[i].time_seconds = time;
        results[i].stddev = 0.0;

        free(arr);
    }

    BigOComparison comparison;
    comparison.size_n = base_n;
    comparison.size_2n = base_n * 2;
    if (results[0].time_seconds > 0 && results[1].time_seconds > 0)
    {
        comparison.ratio = results[1].time_seconds / results[0].time_seconds;
        comparison.deduced = bigo_deduce_complexity(comparison.ratio, 0.3);
        comparison.confidence = calculate_confidence(comparison.ratio, comparison.deduced);
    }
    else
    {
        comparison.ratio = 0;
        comparison.deduced = BIGO_UNKNOWN;
        comparison.confidence = 0;
    }

    bigo_print_verification_table(&comparison, results, 3);

    printf("\nRaw Timing Data:\n");
    printf("%-15s %-15s\n", "Input Size", "Time (seconds)");
    printf("%-15d %-15.6f\n", results[0].input_size, results[0].time_seconds);
    printf("%-15d %-15.6f\n", results[1].input_size, results[1].time_seconds);
    printf("%-15d %-15.6f\n", results[2].input_size, results[2].time_seconds);
    printf("\n");
}

static void bigo_engine_verify_all_demo(void)
{
    printf("\n");
    int base_n;
    int status = safe_input_int(&base_n, "Enter base input size N (min 100, recommended 1000): ", 100, 10000);
    if (status != 0)
    {
        return;
    }

    bigo_verify_all_sorting(base_n, DEFAULT_ITERATIONS);
}

static void bigo_engine_educational_demo(void)
{
    printf("\n");
    printf("================================================================================\n");
    printf("                    BIG-O COMPLEXITY EDUCATIONAL GUIDE\n");
    printf("================================================================================\n");
    printf("\n");
    printf("What is Big-O Notation?\n");
    printf("------------------------\n");
    printf("Big-O notation describes the upper bound of an algorithm's time complexity\n");
    printf("as the input size grows towards infinity.\n");
    printf("\n");
    printf("How Does This Engine Work?\n");
    printf("---------------------------\n");
    printf("1. It runs an algorithm at sizes N, 2N, and 4N\n");
    printf("2. Measures the time taken for each size\n");
    printf("3. Calculates the ratio T(2N) / T(N)\n");
    printf("4. Deduced complexity based on the ratio:\n");
    printf("\n");
    printf("  Ratio ~1.0  => O(1)       Constant time\n");
    printf("  Ratio ~2.0  => O(n)       Linear time\n");
    printf("  Ratio ~2.x  => O(n log n) Linearithmic time\n");
    printf("  Ratio ~4.0  => O(n^2)     Quadratic time\n");
    printf("  Ratio ~8.0  => O(n^3)     Cubic time\n");
    printf("  Ratio >>4   => O(2^n)     Exponential time\n");
    printf("\n");
    printf("Example:\n");
    printf("---------\n");
    printf("If doubling the input doubles the time, the algorithm is O(n).\n");
    printf("If doubling the input quadruples the time, the algorithm is O(n^2).\n");
    printf("\n");
    printf("Why is this Important?\n");
    printf("-----------------------\n");
    printf("- Helps choose the right algorithm for your needs\n");
    printf("- Predicts performance as data grows\n");
    printf("- Essential for writing efficient code\n");
    printf("\n");
    printf("================================================================================\n");
}

void bigo_engine_demo(void)
{
    while (1)
    {
        display_header("Big-O Verification Engine");

        printf("\nEmpirically verify the theoretical time complexity of algorithms.\n");
        printf("This engine measures execution time at different input sizes and\n");
        printf("calculates the growth rate to deduce Big-O complexity.\n\n");

        printf("1. Verify single algorithm\n");
        printf("2. Verify all sorting algorithms\n");
        printf("3. Educational guide\n");
        printf("4. Back to main menu\n");
        printf("\n");

        int choice;
        int status = safe_input_int(&choice, "Enter choice: ", 1, 4);
        if (status != 0)
        {
            break;
        }

        switch (choice)
        {
            case 1:
                bigo_engine_single_algo_demo();
                break;
            case 2:
                bigo_engine_verify_all_demo();
                break;
            case 3:
                bigo_engine_educational_demo();
                break;
            case 4:
                return;
        }

        printf("\nPress Enter to continue...");
        getchar();
    }
}
