#include "bigo.h"
#include "../utils/display_header.h"
#include "../utils/safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MIN_INPUT_SIZE 100
#define MAX_INPUT_SIZE 100000
#define BENCHMARK_ITERATIONS 5

static void shuffle_array(int* arr, size_t n)
{
    for (size_t i = n - 1; i > 0; i--)
    {
        size_t j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

static double benchmark_bubble_sort(int* arr, size_t n)
{
    clock_t start = clock();
    
    for (size_t i = 0; i < n - 1; i++)
    {
        for (size_t j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    
    clock_t end = clock();
    return ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
}

static double benchmark_insertion_sort(int* arr, size_t n)
{
    clock_t start = clock();
    
    for (size_t i = 1; i < n; i++)
    {
        int key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key)
        {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = key;
    }
    
    clock_t end = clock();
    return ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
}

static double benchmark_merge_sort_internal(int* arr, int left, int right)
{
    if (left >= right)
        return 0;
    
    int mid = left + (right - left) / 2;
    benchmark_merge_sort_internal(arr, left, mid);
    benchmark_merge_sort_internal(arr, mid + 1, right);
    
    int* temp = (int*)malloc((right - left + 1) * sizeof(int));
    size_t i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    
    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= right)
        temp[k++] = arr[j++];
    
    for (size_t x = 0; x < k; x++)
        arr[left + x] = temp[x];
    
    free(temp);
    return 0;
}

static double benchmark_merge_sort(int* arr, size_t n)
{
    clock_t start = clock();
    benchmark_merge_sort_internal(arr, 0, n - 1);
    clock_t end = clock();
    return ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
}

static double benchmark_quick_sort_internal(int* arr, int low, int high)
{
    if (low < high)
    {
        int pivot = arr[high];
        size_t i = low;
        for (size_t j = low; j < high; j++)
        {
            if (arr[j] < pivot)
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                i++;
            }
        }
        int temp = arr[i];
        arr[i] = arr[high];
        arr[high] = temp;
        
        benchmark_quick_sort_internal(arr, low, i - 1);
        benchmark_quick_sort_internal(arr, i + 1, high);
    }
    return 0;
}

static double benchmark_quick_sort(int* arr, size_t n)
{
    clock_t start = clock();
    benchmark_quick_sort_internal(arr, 0, n - 1);
    clock_t end = clock();
    return ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
}

static double benchmark_linear_search(int* arr, size_t n, int target)
{
    clock_t start = clock();
    
    for (size_t i = 0; i < n; i++)
    {
        if (arr[i] == target)
            break;
    }
    
    clock_t end = clock();
    return ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
}

static double benchmark_binary_search(int* arr, size_t n, int target)
{
    clock_t start = clock();
    
    size_t left = 0, right = n;
    while (left < right)
    {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] < target)
            left = mid + 1;
        else if (arr[mid] > target)
            right = mid;
        else
            break;
    }
    
    clock_t end = clock();
    return ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
}

static const char* complexity_to_string(BigOComplexity c)
{
    switch (c)
    {
        case BIGO_O_1: return "O(1)";
        case BIGO_O_LOG_N: return "O(log n)";
        case BIGO_O_N: return "O(n)";
        case BIGO_O_N_LOG_N: return "O(n log n)";
        case BIGO_O_N_SQUARED: return "O(n²)";
        case BIGO_O_N_CUBED: return "O(n³)";
        case BIGO_O_2_N: return "O(2ⁿ)";
        default: return "Unknown";
    }
}

static BigOComplexity deduce_complexity(double ratio_2n_1n, double ratio_4n_2n)
{
    if (ratio_2n_1n < 1.5)
        return BIGO_O_1;
    if (ratio_2n_1n < 2.5 && ratio_4n_2n < 2.5)
        return BIGO_O_LOG_N;
    if (ratio_2n_1n >= 1.8 && ratio_2n_1n <= 2.5 && ratio_4n_2n >= 1.8 && ratio_4n_2n <= 2.5)
        return BIGO_O_N;
    if (ratio_2n_1n >= 2.0 && ratio_2n_1n <= 3.0 && ratio_4n_2n >= 3.5 && ratio_4n_2n <= 5.0)
        return BIGO_O_N_LOG_N;
    if (ratio_2n_1n >= 3.5 && ratio_2n_1n <= 5.0 && ratio_4n_2n >= 3.5 && ratio_4n_2n <= 5.0)
        return BIGO_O_N_SQUARED;
    if (ratio_2n_1n >= 7.0 && ratio_4n_2n >= 7.0)
        return BIGO_O_N_CUBED;
    return BIGO_UNKNOWN;
}

static void print_bigo_table_header(void)
{
    printf("\n");
    printf("╔════════════╦═══════════════╦═══════════════╦═══════════════╦════════════════╦════════════════╦═══════════════╗\n");
    printf("║    Size    ║   Time (N)    ║  Time (2N)    ║  Time (4N)    ║ Ratio T(2N)/T(N)║ Ratio T(4N)/T(2N)║ Deduced Big-O  ║\n");
    printf("╠════════════╬═══════════════╬═══════════════╬═══════════════╬════════════════╬════════════════╬═══════════════╣\n");
}

static void print_bigo_table_row(size_t n, double t_n, double t_2n, double t_4n, BigOComplexity c)
{
    double r1 = (t_n > 0) ? t_2n / t_n : 0;
    double r2 = (t_2n > 0) ? t_4n / t_2n : 0;
    printf("║ %10zu ║ %12.4f ║ %12.4f ║ %12.4f ║ %15.2f ║ %15.2f ║ %-14s ║\n",
           n, t_n, t_2n, t_4n, r1, r2, complexity_to_string(c));
}

static void print_bigo_table_footer(void)
{
    printf("╚════════════╩═══════════════╩═══════════════╩═══════════════╩════════════════╩════════════════╩═══════════════╝\n");
}

static void run_sorting_benchmark(const char* name, double (*sort_func)(int*, size_t))
{
    display_header(name);
    
    printf("\nRunning Big-O Verification for %s...\n", name);
    printf("This will benchmark the algorithm at sizes N, 2N, and 4N.\n\n");
    
    int base_size;
    printf("Enter base input size (recommended: 1000-5000): ");
    scanf("%d", &base_size);
    
    if (base_size < 100)
        base_size = 100;
    if (base_size > 10000)
        base_size = 10000;
    
    size_t sizes[3] = {(size_t)base_size, (size_t)base_size * 2, (size_t)base_size * 4};
    double times[3] = {0};
    
    srand((unsigned int)time(NULL));
    
    for (int i = 0; i < 3; i++)
    {
        int* arr = (int*)malloc(sizes[i] * sizeof(int));
        for (size_t j = 0; j < sizes[i]; j++)
            arr[j] = rand() % 100000;
        
        printf("\nBenchmarking size %zu...\n", sizes[i]);
        
        double total_time = 0;
        for (int iter = 0; iter < BENCHMARK_ITERATIONS; iter++)
        {
            shuffle_array(arr, sizes[i]);
            total_time += sort_func(arr, sizes[i]);
        }
        times[i] = total_time / BENCHMARK_ITERATIONS;
        
        free(arr);
    }
    
    double r1 = (times[0] > 0.001) ? times[1] / times[0] : 0;
    double r2 = (times[1] > 0.001) ? times[2] / times[1] : 0;
    BigOComplexity complexity = deduce_complexity(r1, r2);
    
    print_bigo_table_header();
    print_bigo_table_row(sizes[0], times[0], times[1], times[2], complexity);
    print_bigo_table_footer();
    
    printf("\n═══════════════════════════════════════════════════════════════════════\n");
    printf("                        ANALYSIS RESULTS\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("\n  Input Size N:     %zu\n", sizes[0]);
    printf("  Time T(N):        %.4f ms\n", times[0]);
    printf("  Time T(2N):       %.4f ms\n", times[1]);
    printf("  Time T(4N):       %.4f ms\n", times[2]);
    printf("  Ratio T(2N)/T(N): %.2f\n", r1);
    printf("  Ratio T(4N)/T(2N):%.2f\n", r2);
    printf("\n  ➤ Empirical Complexity: %s\n", complexity_to_string(complexity));
    
    printf("\n  [Theory vs Practice]\n");
    if (strstr(name, "Bubble") || strstr(name, "Insertion"))
    {
        printf("  Theoretical: O(n²) | Empirical: %s\n", complexity_to_string(complexity));
        if (complexity == BIGO_O_N_SQUARED)
            printf("  ✓ Empirical verification MATCHES theoretical bound!\n");
    }
    else if (strstr(name, "Merge"))
    {
        printf("  Theoretical: O(n log n) | Empirical: %s\n", complexity_to_string(complexity));
        if (complexity == BIGO_O_N_LOG_N || complexity == BIGO_O_N_SQUARED)
            printf("  ✓ Empirical verification supports theoretical bound!\n");
    }
    else if (strstr(name, "Quick"))
    {
        printf("  Theoretical: O(n log n) avg | Empirical: %s\n", complexity_to_string(complexity));
        if (complexity == BIGO_O_N_LOG_N || complexity == BIGO_O_N)
            printf("  ✓ Empirical verification supports theoretical bound!\n");
    }
    
    printf("\n═══════════════════════════════════════════════════════════════════════\n");
    printf("\n  Press Enter to continue...");
    getchar();
    getchar();
}

static void run_searching_benchmark(const char* name, double (*search_func)(int*, size_t, int))
{
    display_header(name);
    
    printf("\nRunning Big-O Verification for %s...\n", name);
    
    int base_size;
    printf("Enter input size (recommended: 10000-50000): ");
    scanf("%d", &base_size);
    
    if (base_size < 1000)
        base_size = 1000;
    if (base_size > 100000)
        base_size = 100000;
    
    int* arr = (int*)malloc(base_size * sizeof(int));
    for (int i = 0; i < base_size; i++)
        arr[i] = i * 2;
    
    int target = arr[base_size - 1];
    
    size_t sizes[3] = {(size_t)base_size, (size_t)base_size * 2, (size_t)base_size * 4};
    double times[3] = {0};
    
    srand((unsigned int)time(NULL));
    
    for (int i = 0; i < 3; i++)
    {
        int* large_arr = (int*)malloc(sizes[i] * sizeof(int));
        for (size_t j = 0; j < sizes[i]; j++)
            large_arr[j] = (int)(j * 2);
        
        printf("\nBenchmarking size %zu...\n", sizes[i]);
        
        double total_time = 0;
        for (int iter = 0; iter < BENCHMARK_ITERATIONS * 10; iter++)
        {
            total_time += search_func(large_arr, sizes[i], large_arr[sizes[i] - 1]);
        }
        times[i] = total_time / (BENCHMARK_ITERATIONS * 10);
        
        free(large_arr);
    }
    
    double r1 = (times[0] > 0.0001) ? times[1] / times[0] : 0;
    double r2 = (times[1] > 0.0001) ? times[2] / times[1] : 0;
    BigOComplexity complexity = deduce_complexity(r1, r2);
    
    print_bigo_table_header();
    print_bigo_table_row(sizes[0], times[0], times[1], times[2], complexity);
    print_bigo_table_footer();
    
    printf("\n═══════════════════════════════════════════════════════════════════════\n");
    printf("                        ANALYSIS RESULTS\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("\n  ➤ Empirical Complexity: %s\n", complexity_to_string(complexity));
    
    if (strstr(name, "Linear"))
    {
        printf("  Theoretical: O(n) | Empirical: %s\n", complexity_to_string(complexity));
    }
    else if (strstr(name, "Binary"))
    {
        printf("  Theoretical: O(log n) | Empirical: %s\n", complexity_to_string(complexity));
    }
    
    free(arr);
    printf("\n═══════════════════════════════════════════════════════════════════════\n");
    printf("\n  Press Enter to continue...");
    getchar();
    getchar();
}

void bigo_sorting_demo(void)
{
    while (1)
    {
        display_header("Big-O Sorting Verification");
        
        printf("\nSelect an algorithm to verify:\n");
        printf("  1. Bubble Sort (O(n²))\n");
        printf("  2. Insertion Sort (O(n²))\n");
        printf("  3. Merge Sort (O(n log n))\n");
        printf("  4. Quick Sort (O(n log n) avg)\n");
        printf("  5. Back to main menu\n");
        
        int choice;
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        
        if (choice == 5)
            break;
        
        switch (choice)
        {
            case 1:
                run_sorting_benchmark("Bubble Sort", benchmark_bubble_sort);
                break;
            case 2:
                run_sorting_benchmark("Insertion Sort", benchmark_insertion_sort);
                break;
            case 3:
                run_sorting_benchmark("Merge Sort", benchmark_merge_sort);
                break;
            case 4:
                run_sorting_benchmark("Quick Sort", benchmark_quick_sort);
                break;
            default:
                printf("Invalid choice!\n");
        }
    }
}

void bigo_searching_demo(void)
{
    while (1)
    {
        display_header("Big-O Searching Verification");
        
        printf("\nSelect an algorithm to verify:\n");
        printf("  1. Linear Search (O(n))\n");
        printf("  2. Binary Search (O(log n))\n");
        printf("  3. Back to main menu\n");
        
        int choice;
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        
        if (choice == 3)
            break;
        
        switch (choice)
        {
            case 1:
                run_searching_benchmark("Linear Search", benchmark_linear_search);
                break;
            case 2:
                run_searching_benchmark("Binary Search", benchmark_binary_search);
                break;
            default:
                printf("Invalid choice!\n");
        }
    }
}

void bigo_analysis_demo(void)
{
    display_header("Big-O Analysis Guide");
    
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("               BIG-O COMPLEXITY VERIFICATION ENGINE\n");
    printf("═══════════════════════════════════════════════════════════════════════\n\n");
    
    printf("This tool allows you to empirically verify the theoretical time\n");
    printf("complexity of algorithms by benchmarking them at different input sizes.\n\n");
    
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("                         HOW IT WORKS\n");
    printf("═══════════════════════════════════════════════════════════════════════\n\n");
    
    printf("1. The engine runs the algorithm at sizes N, 2N, and 4N\n");
    printf("2. It measures execution time using clock() from <time.h>\n");
    printf("3. It calculates the ratios: T(2N)/T(N) and T(4N)/T(2N)\n");
    printf("4. Based on these ratios, it deduces the empirical complexity:\n\n");
    
    printf("   Complexity    │ Ratio T(2N)/T(N) │ Ratio T(4N)/T(2N)\n");
    printf("   ───────────────┼──────────────────┼──────────────────\n");
    printf("   O(1)           │ ~1               │ ~1\n");
    printf("   O(log n)       │ ~1-2             │ ~1-2\n");
    printf("   O(n)           │ ~2               │ ~2\n");
    printf("   O(n log n)     │ ~2-3             │ ~2-3\n");
    printf("   O(n²)          │ ~4               │ ~4\n");
    printf("   O(n³)          │ ~8               │ ~8\n");
    printf("   O(2ⁿ)          │ ~2x faster        │ ~2x faster\n\n");
    
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("                       AVAILABLE OPTIONS\n");
    printf("═══════════════════════════════════════════════════════════════════════\n\n");
    
    printf("   1. Sorting Algorithms - Verify O(n²) vs O(n log n)\n");
    printf("   2. Searching Algorithms - Verify O(n) vs O(log n)\n\n");
    
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("\n  Press Enter to continue...");
    getchar();
    getchar();
}
