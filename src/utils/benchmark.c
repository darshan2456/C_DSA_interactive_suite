#include "benchmark.h"
#include "sorting_algorithms_n2.h"
#include "advanced_sorting.h"
#include "searching_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif

static int orig_fd = -1;

void benchmark_reset(void)
{
    benchmark_comparisons = 0;
    benchmark_swaps = 0;
}

void disable_stdout(void)
{
    fflush(stdout);
#ifdef _WIN32
    orig_fd = _dup(1);
    int null_fd = _open("NUL", _O_WRONLY);
    if (null_fd >= 0)
    {
        _dup2(null_fd, 1);
        _close(null_fd);
    }
#else
    orig_fd = dup(1);
    int null_fd = open("/dev/null", O_WRONLY);
    if (null_fd >= 0)
    {
        dup2(null_fd, 1);
        close(null_fd);
    }
#endif
}

void restore_stdout(void)
{
    fflush(stdout);
    if (orig_fd >= 0)
    {
#ifdef _WIN32
        _dup2(orig_fd, 1);
        _close(orig_fd);
#else
        dup2(orig_fd, 1);
        close(orig_fd);
#endif
        orig_fd = -1;
    }
}

// Helper to generate a random array of size n
static int* generate_random_array(int n)
{
    int* arr = malloc(sizeof(int) * n);
    if (!arr) return NULL;
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 100000;
    }
    return arr;
}

// Helper to generate a sorted array of size n with distinct elements
static int* generate_sorted_array(int n)
{
    int* arr = malloc(sizeof(int) * n);
    if (!arr) return NULL;
    int current = 1;
    for (int i = 0; i < n; i++)
    {
        arr[i] = current;
        current += 2 + (rand() % 3); // guaranteed distinct ascending order
    }
    return arr;
}

// Copy array helper
static void copy_array(const int src[], int dest[], int n)
{
    for (int i = 0; i < n; i++)
    {
        dest[i] = src[i];
    }
}

typedef struct {
    const char* name;
    int is_comparison_based;
} SortAlgo;

static SortAlgo SORT_ALGOS[] = {
    {"Bubble Sort", 1},
    {"Selection Sort", 1},
    {"Insertion Sort", 1},
    {"Shell Sort", 1},
    {"Quick Sort", 1},
    {"Merge Sort", 1},
    {"Heap Sort", 1},
    {"Radix Sort", 0},
    {"Bucket Sort", 0}
};

static void run_single_sort(const char* name, int is_comparison, int arr[], int n)
{
    benchmark_reset();
    benchmark_active = 1;
    disable_stdout();

    clock_t start = clock();
    if (strcmp(name, "Bubble Sort") == 0)
    {
        bubble_sort_optimized(arr, n);
    }
    else if (strcmp(name, "Selection Sort") == 0)
    {
        selection_sort(arr, n);
    }
    else if (strcmp(name, "Insertion Sort") == 0)
    {
        insertion_sort(arr, n);
    }
    else if (strcmp(name, "Shell Sort") == 0)
    {
        shell_sort(arr, n);
    }
    else if (strcmp(name, "Quick Sort") == 0)
    {
        quicksort(arr, 0, n - 1);
    }
    else if (strcmp(name, "Merge Sort") == 0)
    {
        merge_sort(arr, n);
    }
    else if (strcmp(name, "Heap Sort") == 0)
    {
        heap_sort(arr, n);
    }
    else if (strcmp(name, "Radix Sort") == 0)
    {
        radix_sort(arr, n);
    }
    else if (strcmp(name, "Bucket Sort") == 0)
    {
        bucket_sort(arr, n);
    }
    clock_t end = clock();

    restore_stdout();
    benchmark_active = 0;

    double time_ms = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;

    if (is_comparison)
    {
        printf("%-18s %-10d %-15.2f %-15lld %-15lld\n", name, n, time_ms, benchmark_comparisons, benchmark_swaps);
    }
    else
    {
        printf("%-18s %-10d %-15.2f %-15s %-15s\n", name, n, time_ms, "N/A", "N/A");
    }
}

static void benchmark_sorting(void)
{
    printf("\n=== Sorting Algorithms Performance Benchmark ===\n");
    printf("Please wait while benchmarks are running...\n\n");
    printf("%-18s %-10s %-15s %-15s %-15s\n", "Algorithm", "Size", "Time (ms)", "Comparisons", "Swaps");
    printf("--------------------------------------------------------------------------------\n");

    int sizes[] = {100, 1000, 10000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int num_algos = sizeof(SORT_ALGOS) / sizeof(SORT_ALGOS[0]);

    for (int s = 0; s < num_sizes; s++)
    {
        int size = sizes[s];
        int* orig_arr = generate_random_array(size);
        if (!orig_arr) continue;

        int* temp_arr = malloc(sizeof(int) * size);
        if (!temp_arr)
        {
            free(orig_arr);
            continue;
        }

        for (int a = 0; a < num_algos; a++)
        {
            copy_array(orig_arr, temp_arr, size);
            run_single_sort(SORT_ALGOS[a].name, SORT_ALGOS[a].is_comparison_based, temp_arr, size);
        }
        printf("--------------------------------------------------------------------------------\n");

        free(temp_arr);
        free(orig_arr);
    }
}

typedef struct {
    const char* name;
} SearchAlgo;

static SearchAlgo SEARCH_ALGOS[] = {
    {"Linear Search"},
    {"Binary Search (Iter)"},
    {"Binary Search (Rec)"},
    {"Interpolation Search"},
    {"Jump Search"}
};

static void run_single_search(const char* name, const int arr[], int n, const int targets[], int num_targets)
{
    benchmark_reset();
    benchmark_active = 1;
    disable_stdout();

    // To get stable timing, we repeat the lookups in a loop
    int repeats = 100;
    clock_t start = clock();
    for (int r = 0; r < repeats; r++)
    {
        for (int i = 0; i < num_targets; i++)
        {
            int t = targets[i];
            if (strcmp(name, "Linear Search") == 0)
            {
                (void)linear_search((int*)arr, t, n);
            }
            else if (strcmp(name, "Binary Search (Iter)") == 0)
            {
                (void)binary_search((int*)arr, t, n);
            }
            else if (strcmp(name, "Binary Search (Rec)") == 0)
            {
                (void)binary_search_recursive((int*)arr, t, 0, n - 1);
            }
            else if (strcmp(name, "Interpolation Search") == 0)
            {
                (void)interpolation_search((int*)arr, t, n);
            }
            else if (strcmp(name, "Jump Search") == 0)
            {
                (void)jump_search((int*)arr, t, n);
            }
        }
    }
    clock_t end = clock();

    restore_stdout();
    benchmark_active = 0;

    double time_ms = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;

    printf("%-22s %-10d %-18.2f %-18lld\n", name, n, time_ms, benchmark_comparisons);
}

static void benchmark_searching(void)
{
    printf("\n=== Searching Algorithms Performance Benchmark (10,000 lookups per algorithm) ===\n");
    printf("Please wait while benchmarks are running...\n\n");
    printf("%-22s %-10s %-18s %-18s\n", "Algorithm", "Size", "Time (ms)", "Comparisons");
    printf("--------------------------------------------------------------------------------\n");

    int sizes[] = {100, 1000, 10000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int num_algos = sizeof(SEARCH_ALGOS) / sizeof(SEARCH_ALGOS[0]);

    // Generate random search targets
    int num_targets = 100;
    int targets[100];
    for (int i = 0; i < num_targets; i++)
    {
        targets[i] = rand() % 25000;
    }

    for (int s = 0; s < num_sizes; s++)
    {
        int size = sizes[s];
        int* sorted_arr = generate_sorted_array(size);
        if (!sorted_arr) continue;

        for (int a = 0; a < num_algos; a++)
        {
            run_single_search(SEARCH_ALGOS[a].name, sorted_arr, size, targets, num_targets);
        }
        printf("--------------------------------------------------------------------------------\n");

        free(sorted_arr);
    }
}

void run_benchmark_menu(void)
{
    while (1)
    {
        int choice;
        int status = safe_input_int(
            &choice,
            "\n=== Performance Benchmark Mode ===\n"
            "1. Benchmark Sorting Algorithms (Bubble, Selection, Insertion, Shell, Quick, Merge, Heap, Radix, Bucket)\n"
            "2. Benchmark Searching Algorithms (Linear, Binary Iterative, Binary Recursive, Interpolation, Jump)\n"
            "3. Return to Main Menu\n"
            "Enter choice: ",
            1, 3
        );

        if (status == INPUT_EXIT_SIGNAL || choice == 3)
        {
            break;
        }

        if (status == 0)
        {
            continue;
        }

        if (choice == 1)
        {
            benchmark_sorting();
        }
        else if (choice == 2)
        {
            benchmark_searching();
        }
    }
}
