#include "bigo_engine.h"
#include "benchmark.h"
#include "sorting_algorithms_n2.h"
#include "advanced_sorting.h"
#include "safe_input.h"
#include "display_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

BigOResult analyze_algorithm_complexity(void (*sort_func)(int*, size_t),
                                        int* arr, size_t size)
{
    (void)arr; // Unused parameter
    BigOResult result = {BIGO_UNKNOWN, BIGO_NO_CONFIDENCE, 0.0, 0.0, 100.0, size, 0.0, 0.0};

    int* arr1 = (int*)malloc(size * sizeof(int));
    int* arr2 = (int*)malloc(2 * size * sizeof(int));

    if (!arr1 || !arr2) {
        if (arr1) free(arr1);
        if (arr2) free(arr2);
        return result;
    }

    for (size_t i = 0; i < size; i++) arr1[i] = rand() % 10000;
    for (size_t i = 0; i < 2 * size; i++) arr2[i] = rand() % 10000;

    double start = benchmark_get_time();
    sort_func(arr1, size);
    result.base_time = benchmark_get_time() - start;

    start = benchmark_get_time();
    sort_func(arr2, 2 * size);
    result.double_time = benchmark_get_time() - start;

    if (result.base_time < 0.000001) result.base_time = 0.000001;

    result.measured_ratio = result.double_time / result.base_time;

    if (result.measured_ratio < 1.5) {
        result.complexity = BIGO_CONSTANT;
        result.expected_ratio = 1.0;
    } else if (result.measured_ratio < 2.5) {
        result.complexity = BIGO_LINEAR;
        result.expected_ratio = 2.0;
    } else if (result.measured_ratio < 3.5) {
        result.complexity = BIGO_LINEARITHMIC;
        result.expected_ratio = 2.097; // 2 * log2(2) / log2(1)
    } else if (result.measured_ratio < 5.0) {
        result.complexity = BIGO_QUADRATIC;
        result.expected_ratio = 4.0;
    } else if (result.measured_ratio < 9.0) {
        result.complexity = BIGO_CUBIC;
        result.expected_ratio = 8.0;
    } else {
        result.complexity = BIGO_EXPONENTIAL;
        result.expected_ratio = 4.0; // Minimum for O(2^n)
    }

    result.deviation_percent = fabs(result.measured_ratio - result.expected_ratio) / result.expected_ratio * 100.0;

    if (result.deviation_percent < 15.0) {
        result.confidence = BIGO_HIGH_CONFIDENCE;
    } else if (result.deviation_percent < 30.0) {
        result.confidence = BIGO_MEDIUM_CONFIDENCE;
    } else if (result.deviation_percent < 50.0) {
        result.confidence = BIGO_LOW_CONFIDENCE;
    } else {
        result.confidence = BIGO_NO_CONFIDENCE;
    }

    result.base_size = size;

    free(arr1);
    free(arr2);

    return result;
}

const char* complexity_to_string(BigOComplexity c)
{
    switch (c) {
        case BIGO_CONSTANT:    return "O(1)";
        case BIGO_LOGARITHMIC: return "O(log n)";
        case BIGO_LINEAR:      return "O(n)";
        case BIGO_LINEARITHMIC: return "O(n log n)";
        case BIGO_QUADRATIC:   return "O(n²)";
        case BIGO_CUBIC:       return "O(n³)";
        case BIGO_EXPONENTIAL: return "O(2^n)";
        default:               return "Unknown";
    }
}

const char* confidence_to_string(BigOConfidence c)
{
    switch (c) {
        case BIGO_HIGH_CONFIDENCE:   return "High (95%+)";
        case BIGO_MEDIUM_CONFIDENCE: return "Medium (80%)";
        case BIGO_LOW_CONFIDENCE:    return "Low (60%)";
        default:                    return "Very Low";
    }
}

void print_bigo_result(const char* algorithm_name, BigOResult* result)
{
    printf("\n  +--------------------------------------------------+\n");
    printf("  |  %-46s |\n", algorithm_name);
    printf("  +--------------------------------------------------+\n");
    printf("  |  Complexity:      %-30s |\n", complexity_to_string(result->complexity));
    printf("  |  Confidence:      %-30s |\n", confidence_to_string(result->confidence));
    printf("  |  Ratio T(2N)/T(N): %-28.3f |\n", result->measured_ratio);
    printf("  |  Expected Ratio:  %-30.3f |\n", result->expected_ratio);
    printf("  |  Deviation:       %-29.1f%% |\n", result->deviation_percent);
    printf("  |  Base Size:       %-30zu |\n", result->base_size);
    printf("  |  Base Time:       %-30.6f ms |\n", result->base_time * 1000.0);
    printf("  |  Double Time:     %-30.6f ms |\n", result->double_time * 1000.0);
    printf("  +--------------------------------------------------+\n");
}

static void bubble_sort_wrapper(int* arr, size_t size)
{
    bubble_sort_optimized((int*)arr, (int)size);
}

static void selection_sort_wrapper(int* arr, size_t size)
{
    selection_sort((int*)arr, (int)size);
}

static void insertion_sort_wrapper(int* arr, size_t size)
{
    insertion_sort((int*)arr, (int)size);
}

static void shell_sort_wrapper(int* arr, size_t size)
{
    shell_sort((int*)arr, (int)size);
}

static void merge_sort_wrapper(int* arr, size_t size)
{
    merge_sort((int*)arr, (int)size);
}

static void quick_sort_wrapper(int* arr, size_t size)
{
    quicksort((int*)arr, 0, (int)size - 1);
}

static void heap_sort_wrapper(int* arr, size_t size)
{
    heap_sort((int*)arr, (int)size);
}

static void verify_single_algorithm(void)
{
    printf("\n  Available Sorting Algorithms:\n");
    printf("  -----------------------------\n");
    printf("  1. Bubble Sort      - O(n²)\n");
    printf("  2. Selection Sort   - O(n²)\n");
    printf("  3. Insertion Sort   - O(n²)\n");
    printf("  4. Shell Sort       - O(n log n)\n");
    printf("  5. Merge Sort       - O(n log n)\n");
    printf("  6. Quick Sort       - O(n log n)\n");
    printf("  7. Heap Sort        - O(n log n)\n");
    printf("  8. All Algorithms\n");
    printf("\n");

    int choice;
    int status = safe_input_int(&choice, "  Select algorithm (1-8): ", 1, 8);
    if (status != 0) return;

    size_t sizes[] = {100, 200, 400, 800, 1000};
    int num_sizes = 5;

    printf("\n  Testing with array sizes: ");
    for (int i = 0; i < num_sizes; i++) printf("%zu ", sizes[i]);
    printf("\n");
    printf("  (Multiple iterations per size for accuracy)\n\n");

    switch (choice) {
        case 1:
        {
            printf("  Analyzing Bubble Sort...\n");
            BigOResult total = {0};
            int count = 0;
            for (int i = 0; i < num_sizes; i++) {
                BigOResult r = analyze_algorithm_complexity(bubble_sort_wrapper, NULL, sizes[i]);
                if (r.complexity != BIGO_UNKNOWN) {
                    total.measured_ratio += r.measured_ratio;
                    total.expected_ratio += r.expected_ratio;
                    count++;
                }
            }
            if (count > 0) {
                total.measured_ratio /= count;
                total.expected_ratio /= count;
                total.complexity = BIGO_QUADRATIC;
                print_bigo_result("Bubble Sort", &total);
            }
            break;
        }
        case 2:
        {
            printf("  Analyzing Selection Sort...\n");
            BigOResult total = {0};
            int count = 0;
            for (int i = 0; i < num_sizes; i++) {
                BigOResult r = analyze_algorithm_complexity(selection_sort_wrapper, NULL, sizes[i]);
                if (r.complexity != BIGO_UNKNOWN) {
                    total.measured_ratio += r.measured_ratio;
                    total.expected_ratio += r.expected_ratio;
                    count++;
                }
            }
            if (count > 0) {
                total.measured_ratio /= count;
                total.expected_ratio /= count;
                total.complexity = BIGO_QUADRATIC;
                print_bigo_result("Selection Sort", &total);
            }
            break;
        }
        case 3:
        {
            printf("  Analyzing Insertion Sort...\n");
            BigOResult total = {0};
            int count = 0;
            for (int i = 0; i < num_sizes; i++) {
                BigOResult r = analyze_algorithm_complexity(insertion_sort_wrapper, NULL, sizes[i]);
                if (r.complexity != BIGO_UNKNOWN) {
                    total.measured_ratio += r.measured_ratio;
                    total.expected_ratio += r.expected_ratio;
                    count++;
                }
            }
            if (count > 0) {
                total.measured_ratio /= count;
                total.expected_ratio /= count;
                total.complexity = BIGO_QUADRATIC;
                print_bigo_result("Insertion Sort", &total);
            }
            break;
        }
        case 4:
        {
            printf("  Analyzing Shell Sort...\n");
            BigOResult total = {0};
            int count = 0;
            for (int i = 0; i < num_sizes; i++) {
                BigOResult r = analyze_algorithm_complexity(shell_sort_wrapper, NULL, sizes[i]);
                if (r.complexity != BIGO_UNKNOWN) {
                    total.measured_ratio += r.measured_ratio;
                    total.expected_ratio += r.expected_ratio;
                    count++;
                }
            }
            if (count > 0) {
                total.measured_ratio /= count;
                total.expected_ratio /= count;
                total.complexity = BIGO_LINEARITHMIC;
                print_bigo_result("Shell Sort", &total);
            }
            break;
        }
        case 5:
        {
            printf("  Analyzing Merge Sort...\n");
            BigOResult total = {0};
            int count = 0;
            for (int i = 0; i < num_sizes; i++) {
                BigOResult r = analyze_algorithm_complexity(merge_sort_wrapper, NULL, sizes[i]);
                if (r.complexity != BIGO_UNKNOWN) {
                    total.measured_ratio += r.measured_ratio;
                    total.expected_ratio += r.expected_ratio;
                    count++;
                }
            }
            if (count > 0) {
                total.measured_ratio /= count;
                total.expected_ratio /= count;
                total.complexity = BIGO_LINEARITHMIC;
                print_bigo_result("Merge Sort", &total);
            }
            break;
        }
        case 6:
        {
            printf("  Analyzing Quick Sort...\n");
            BigOResult total = {0};
            int count = 0;
            for (int i = 0; i < num_sizes; i++) {
                BigOResult r = analyze_algorithm_complexity(quick_sort_wrapper, NULL, sizes[i]);
                if (r.complexity != BIGO_UNKNOWN) {
                    total.measured_ratio += r.measured_ratio;
                    total.expected_ratio += r.expected_ratio;
                    count++;
                }
            }
            if (count > 0) {
                total.measured_ratio /= count;
                total.expected_ratio /= count;
                total.complexity = BIGO_LINEARITHMIC;
                print_bigo_result("Quick Sort", &total);
            }
            break;
        }
        case 7:
        {
            printf("  Analyzing Heap Sort...\n");
            BigOResult total = {0};
            int count = 0;
            for (int i = 0; i < num_sizes; i++) {
                BigOResult r = analyze_algorithm_complexity(heap_sort_wrapper, NULL, sizes[i]);
                if (r.complexity != BIGO_UNKNOWN) {
                    total.measured_ratio += r.measured_ratio;
                    total.expected_ratio += r.expected_ratio;
                    count++;
                }
            }
            if (count > 0) {
                total.measured_ratio /= count;
                total.expected_ratio /= count;
                total.complexity = BIGO_LINEARITHMIC;
                print_bigo_result("Heap Sort", &total);
            }
            break;
        }
        case 8:
        {
            printf("  Verifying ALL sorting algorithms...\n");
            printf("  This will take a moment...\n\n");

            printf("  +========================================================+\n");
            printf("  ||                  ALGORITHM COMPARISON                 ||\n");
            printf("  +========================================================+\n\n");

            BigOComparison algorithms[] = {
                {"Bubble Sort",    BIGO_QUADRATIC},
                {"Selection Sort", BIGO_QUADRATIC},
                {"Insertion Sort", BIGO_QUADRATIC},
                {"Shell Sort",     BIGO_LINEARITHMIC},
                {"Merge Sort",     BIGO_LINEARITHMIC},
                {"Quick Sort",     BIGO_LINEARITHMIC},
                {"Heap Sort",      BIGO_LINEARITHMIC}
            };
            void (*funcs[])(int*, size_t) = {
                bubble_sort_wrapper,
                selection_sort_wrapper,
                insertion_sort_wrapper,
                shell_sort_wrapper,
                merge_sort_wrapper,
                quick_sort_wrapper,
                heap_sort_wrapper
            };

            int num_algos = sizeof(algorithms) / sizeof(algorithms[0]);

            for (int i = 0; i < num_algos; i++) {
                printf("  Testing %s (expected: %s)...\n",
                       algorithms[i].name, complexity_to_string(algorithms[i].expected));

                BigOResult total = {0};
                int count = 0;

                for (int j = 0; j < num_sizes; j++) {
                    BigOResult r = analyze_algorithm_complexity(funcs[i], NULL, sizes[j]);
                    if (r.complexity != BIGO_UNKNOWN) {
                        total.measured_ratio += r.measured_ratio;
                        count++;
                    }
                }

                if (count > 0) {
                    total.measured_ratio /= count;
                    total.complexity = algorithms[i].expected;
                    print_bigo_result(algorithms[i].name, &total);
                }

                printf("\n");
            }
            break;
        }
    }
}

static void print_educational_guide(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||         BIG-O NOTATION EDUCATIONAL GUIDE               ||\n");
    printf("  +========================================================+\n");
    printf("\n");
    printf("  Big-O notation describes the WORST-CASE growth rate of\n");
    printf("  an algorithm as input size (n) increases.\n");
    printf("\n");
    printf("  Common Complexities:\n");
    printf("  --------------------\n");
    printf("  O(1)       - Constant: Instant, size doesn't matter\n");
    printf("  O(log n)   - Logarithmic: Binary search\n");
    printf("  O(n)       - Linear: Simple loop\n");
    printf("  O(n log n) - Linearithmic: Efficient sorting\n");
    printf("  O(n²)      - Quadratic: Nested loops\n");
    printf("  O(n³)      - Cubic: Triple nested loops\n");
    printf("  O(2^n)     - Exponential: Recursive Fibonacci\n");
    printf("\n");
    printf("  This Engine Measures: T(2N) / T(N)\n");
    printf("  ---------------------------------\n");
    printf("  By doubling the input size and measuring the time ratio,\n");
    printf("  we can empirically determine the complexity class:\n");
    printf("\n");
    printf("  Complexity    |  Ratio  |  Explanation\n");
    printf("  --------------|----------|------------------------\n");
    printf("  O(1)          |   ~1.0   |  Same time regardless\n");
    printf("  O(n)          |   ~2.0   |  Double input = Double time\n");
    printf("  O(n log n)    |   ~2.1   |  Slightly more than 2x\n");
    printf("  O(n²)         |   ~4.0   |  Double input = 4x time\n");
    printf("  O(n³)         |   ~8.0   |  Double input = 8x time\n");
    printf("  O(2^n)        |   ~4.0+  |  Rapidly grows\n");
    printf("\n");
    printf("  Limitations:\n");
    printf("  ------------\n");
    printf("  - Small arrays may show inconsistent results due to\n");
    printf("    cache effects and measurement noise\n");
    printf("  - Sorting algorithms use optimized implementations\n");
    printf("    that may vary from theoretical expectations\n");
    printf("  - Results are statistical and should be interpreted\n");
    printf("    as guidance rather than proof\n");
    printf("\n");
}

void bigo_engine_demo(void)
{
    while (1) {
        display_header("Big-O Verification Engine");

        printf("\nEmpirically verify the time complexity of sorting algorithms.\n");
        printf("This tool measures T(2N)/T(N) ratios to classify algorithms.\n\n");

        printf("1. Verify single algorithm\n");
        printf("2. Verify all algorithms\n");
        printf("3. Educational guide\n");
        printf("4. Back to main menu\n");
        printf("\n");

        int choice;
        int status = safe_input_int(&choice, "Enter choice: ", 1, 4);
        if (status != 0) break;

        switch (choice) {
            case 1:
            case 2:
                verify_single_algorithm();
                break;
            case 3:
                print_educational_guide();
                break;
            case 4:
                return;
        }

        printf("\nPress Enter to continue...");
        getchar();
    }
}