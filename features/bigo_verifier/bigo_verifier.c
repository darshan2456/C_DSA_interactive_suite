#include "bigo_verifier.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char* bigo_complexity_to_string(BigOComplexity complexity)
{
    switch (complexity)
    {
        case BIGO_O1:
            return "O(1)";
        case BIGO_OLOGN:
            return "O(log N)";
        case BIGO_ON:
            return "O(N)";
        case BIGO_ONLOGN:
            return "O(N log N)";
        case BIGO_ON2:
            return "O(N^2)";
        case BIGO_ON3:
            return "O(N^3)";
        default:
            return "O(Unknown)";
    }
}

BigOComplexity deduce_bigo_from_ratio(double ratio, int n)
{
    if (ratio <= 0.0)
    {
        return BIGO_UNKNOWN;
    }

    double log_factor = (n > 1) ? (1.0 + (log(2.0) / log((double)n))) : 1.2;
    double expected_onlogn_ratio = 2.0 * log_factor;

    if (ratio < 1.15)
    {
        return BIGO_O1;
    }
    else if (ratio < 1.6)
    {
        return BIGO_OLOGN;
    }
    else if (ratio < (expected_onlogn_ratio - 0.15))
    {
        return BIGO_ON;
    }
    else if (ratio < 3.1)
    {
        return BIGO_ONLOGN;
    }
    else if (ratio < 6.0)
    {
        return BIGO_ON2;
    }
    else
    {
        return BIGO_ON3;
    }
}

static double measure_execution_time_ms(AlgFunction func, int n)
{
    if (func == NULL || n <= 0)
    {
        return 0.0;
    }

    int* data = (int*)malloc((size_t)n * sizeof(int));
    if (!data)
    {
        return 0.0;
    }

    srand(42);
    for (int i = 0; i < n; i++)
    {
        data[i] = rand() % (n * 10 + 1);
    }

    clock_t start = clock();
    func(data, n);
    clock_t end = clock();

    free(data);

    double duration_ms = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;
    return (duration_ms < 0.001) ? 0.001 : duration_ms;
}

bool run_bigo_profile(const char* alg_name, const char* theoretical_str, AlgFunction func,
                      int base_n, int samples_to_run, BigOReport* report)
{
    if (!alg_name || !func || !report || base_n <= 0)
    {
        return false;
    }

    memset(report, 0, sizeof(BigOReport));
    report->algorithm_name = alg_name;
    report->theoretical_complexity = theoretical_str ? theoretical_str : "Unknown";

    if (samples_to_run < 2)
    {
        samples_to_run = 2;
    }
    if (samples_to_run > MAX_BIGO_SAMPLES)
    {
        samples_to_run = MAX_BIGO_SAMPLES;
    }

    report->sample_count = samples_to_run;

    int current_n = base_n;
    for (int i = 0; i < samples_to_run; i++)
    {
        report->samples[i].n = current_n;

        double t1 = measure_execution_time_ms(func, current_n);
        double t2 = measure_execution_time_ms(func, current_n);
        double t3 = measure_execution_time_ms(func, current_n);

        report->samples[i].time_ms = (t1 + t2 + t3) / 3.0;
        current_n *= 2;
    }

    report->ratio_count = samples_to_run - 1;
    double sum_ratio = 0.0;

    for (int i = 0; i < report->ratio_count; i++)
    {
        double prev_t = report->samples[i].time_ms;
        double next_t = report->samples[i + 1].time_ms;
        double ratio = (prev_t > 0.0) ? (next_t / prev_t) : 1.0;

        report->ratios[i].n_start = report->samples[i].n;
        report->ratios[i].n_end = report->samples[i + 1].n;
        report->ratios[i].ratio = ratio;
        report->ratios[i].deduced_class = deduce_bigo_from_ratio(ratio, report->samples[i].n);

        sum_ratio += ratio;
    }

    double avg_ratio = sum_ratio / (double)report->ratio_count;
    report->overall_empirical_class = deduce_bigo_from_ratio(avg_ratio, report->samples[0].n);
    report->overall_empirical_str = bigo_complexity_to_string(report->overall_empirical_class);

    report->matches_theoretical =
        (strstr(report->overall_empirical_str, report->theoretical_complexity) != NULL ||
         strstr(report->theoretical_complexity, report->overall_empirical_str) != NULL);

    return true;
}

void print_bigo_report(const BigOReport* report)
{
    if (!report)
    {
        printf("(NULL BigOReport pointer)\n");
        return;
    }

    printf("\n========================================================================\n");
    printf("        EMPIRICAL ASYMPTOTIC COMPLEXITY VERIFIER (BIG-O ENGINE)        \n");
    printf("========================================================================\n");
    printf("Algorithm: %-25s | Theoretical: %s\n", report->algorithm_name,
           report->theoretical_complexity);
    printf("------------------------------------------------------------------------\n");
    printf("  %-10s | %-16s | %-14s | %-12s\n", "Size (N)", "Time (ms)", "Ratio T(2N)/T(N)",
           "Deduced Bound");
    printf("  -----------+------------------+----------------+--------------\n");

    for (int i = 0; i < report->sample_count; i++)
    {
        if (i < report->ratio_count)
        {
            printf("  N=%-8d | %-16.3f | %-14.2fx | %-12s\n", report->samples[i].n,
                   report->samples[i].time_ms, report->ratios[i].ratio,
                   bigo_complexity_to_string(report->ratios[i].deduced_class));
        }
        else
        {
            printf("  N=%-8d | %-16.3f | %-14s | %-12s\n", report->samples[i].n,
                   report->samples[i].time_ms, "-", "-");
        }
    }

    printf("========================================================================\n");
    printf("Empirical Bound Deduced: %s\n", report->overall_empirical_str);
    printf("Theoretical Match: %s\n", report->matches_theoretical ? "[MATCHED]" : "[DIVERGED]");
    printf("========================================================================\n\n");
}

// Implementations for Sorting Algorithms
static void alg_bubble_sort(int* data, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (data[j] > data[j + 1])
            {
                int tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }
        }
    }
}

static void alg_insertion_sort(int* data, int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key)
        {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

static void alg_selection_sort(int* data, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
        {
            if (data[j] < data[min_idx])
            {
                min_idx = j;
            }
        }
        int tmp = data[i];
        data[i] = data[min_idx];
        data[min_idx] = tmp;
    }
}

static int qsort_cmp(const void* a, const void* b)
{
    return (*(const int*)a - *(const int*)b);
}

static void alg_quick_sort(int* data, int n)
{
    qsort(data, (size_t)n, sizeof(int), qsort_cmp);
}

static void merge_sort_helper(int* data, int l, int r)
{
    if (l >= r)
        return;
    int m = l + (r - l) / 2;
    merge_sort_helper(data, l, m);
    merge_sort_helper(data, m + 1, r);

    int n1 = m - l + 1;
    int n2 = r - m;
    int* L = (int*)malloc((size_t)n1 * sizeof(int));
    int* R = (int*)malloc((size_t)n2 * sizeof(int));
    if (!L || !R)
    {
        free(L);
        free(R);
        return;
    }
    for (int i = 0; i < n1; i++)
        L[i] = data[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = data[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            data[k++] = L[i++];
        else
            data[k++] = R[j++];
    }
    while (i < n1)
        data[k++] = L[i++];
    while (j < n2)
        data[k++] = R[j++];

    free(L);
    free(R);
}

static void alg_merge_sort(int* data, int n)
{
    merge_sort_helper(data, 0, n - 1);
}

static void alg_linear_search(int* data, int n)
{
    int target = -9999;
    for (int i = 0; i < n; i++)
    {
        if (data[i] == target)
        {
            break;
        }
    }
}

static void alg_binary_search(int* data, int n)
{
    int target = data[n / 2];
    int low = 0, high = n - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (data[mid] == target)
            break;
        if (data[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }
}

// Graph, Tree, and DP algorithm implementations
static void alg_bfs_graph(int* data, int n)
{
    (void)data;
    bool* visited = (bool*)calloc((size_t)n, sizeof(bool));
    int* queue = (int*)malloc((size_t)n * sizeof(int));
    if (!visited || !queue)
    {
        free(visited);
        free(queue);
        return;
    }
    int head = 0, tail = 0;
    visited[0] = true;
    queue[tail++] = 0;

    while (head < tail)
    {
        int u = queue[head++];
        int next_v = (u + 1) % n;
        if (!visited[next_v])
        {
            visited[next_v] = true;
            queue[tail++] = next_v;
        }
    }
    free(visited);
    free(queue);
}

typedef struct BSTNode
{
    int key;
    struct BSTNode *left, *right;
} BSTNode;

static BSTNode* insert_bst(BSTNode* node, int key)
{
    if (!node)
    {
        BSTNode* new_node = (BSTNode*)malloc(sizeof(BSTNode));
        if (new_node)
        {
            new_node->key = key;
            new_node->left = new_node->right = NULL;
        }
        return new_node;
    }
    if (key < node->key)
        node->left = insert_bst(node->left, key);
    else
        node->right = insert_bst(node->right, key);
    return node;
}

static void free_bst(BSTNode* node)
{
    if (!node)
        return;
    free_bst(node->left);
    free_bst(node->right);
    free(node);
}

static void alg_bst_ops(int* data, int n)
{
    BSTNode* root = NULL;
    for (int i = 0; i < n; i++)
    {
        root = insert_bst(root, data[i]);
    }
    free_bst(root);
}

static void alg_dp_fibonacci(int* data, int n)
{
    (void)data;
    if (n <= 1)
        return;
    long long* dp = (long long*)malloc((size_t)(n + 1) * sizeof(long long));
    if (!dp)
        return;
    dp[0] = 0;
    dp[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        if (dp[i - 1] > LLONG_MAX - dp[i - 2])
        {
            dp[i] = LLONG_MAX;
            break;
        }
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    free(dp);
}

bool profile_bubble_sort_bigo(int base_n, BigOReport* report)
{
    return run_bigo_profile("Bubble Sort", "O(N^2)", alg_bubble_sort, base_n, 4, report);
}

bool profile_insertion_sort_bigo(int base_n, BigOReport* report)
{
    return run_bigo_profile("Insertion Sort", "O(N^2)", alg_insertion_sort, base_n, 4, report);
}

bool profile_selection_sort_bigo(int base_n, BigOReport* report)
{
    return run_bigo_profile("Selection Sort", "O(N^2)", alg_selection_sort, base_n, 4, report);
}

bool profile_quick_sort_bigo(int base_n, BigOReport* report)
{
    return run_bigo_profile("Quick Sort", "O(N log N)", alg_quick_sort, base_n, 4, report);
}

bool profile_merge_sort_bigo(int base_n, BigOReport* report)
{
    return run_bigo_profile("Merge Sort", "O(N log N)", alg_merge_sort, base_n, 4, report);
}

bool profile_linear_search_bigo(int base_n, BigOReport* report)
{
    return run_bigo_profile("Linear Search", "O(N)", alg_linear_search, base_n, 4, report);
}

bool profile_binary_search_bigo(int base_n, BigOReport* report)
{
    return run_bigo_profile("Binary Search", "O(log N)", alg_binary_search, base_n, 4, report);
}

bool profile_bfs_graph_bigo(int base_n, BigOReport* report)
{
    return run_bigo_profile("BFS Graph Traversal", "O(N)", alg_bfs_graph, base_n, 4, report);
}

bool profile_bst_operations_bigo(int base_n, BigOReport* report)
{
    return run_bigo_profile("BST Node Insertion", "O(N log N)", alg_bst_ops, base_n, 4, report);
}

bool profile_dp_fibonacci_bigo(int base_n, BigOReport* report)
{
    return run_bigo_profile("DP Fibonacci", "O(N)", alg_dp_fibonacci, base_n, 4, report);
}
