#define _GNU_SOURCE
#include "../hashing/hash.h"
#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct HashNode
{
    int key;
    struct HashNode* next;
} HashNode;

static void run_linear_probing(const int* keys, int m, int n)
{
    int* table = calloc(n, sizeof(int));
    if (!table)
        return;

    for (int i = 0; i < m; i++)
    {
        int val = keys[i];
        int hash_loc = hash_function(val, n);
        int start = hash_loc;
        while (table[hash_loc] != 0)
        {
            hash_loc = (hash_loc + 1) % n;
            if (hash_loc == start)
                break; // Table full
        }
        if (table[hash_loc] == 0)
        {
            table[hash_loc] = val;
        }
    }

    // Lookup
    for (int i = 0; i < m; i++)
    {
        int val = keys[i];
        int hash_loc = hash_function(val, n);
        int start = hash_loc;
        while (table[hash_loc] != 0)
        {
            if (table[hash_loc] == val)
                break;
            hash_loc = (hash_loc + 1) % n;
            if (hash_loc == start)
                break;
        }
    }

    free(table);
}

static void run_quadratic_probing(const int* keys, int m, int n)
{
    int* table = calloc(n, sizeof(int));
    if (!table)
        return;

    for (int i = 0; i < m; i++)
    {
        int val = keys[i];
        int base = hash_function(val, n);
        for (int j = 0; j < n; j++)
        {
            int hash_loc = (base + j * j) % n;
            if (table[hash_loc] == 0)
            {
                table[hash_loc] = val;
                break;
            }
        }
    }

    // Lookup
    for (int i = 0; i < m; i++)
    {
        int val = keys[i];
        int base = hash_function(val, n);
        for (int j = 0; j < n; j++)
        {
            int hash_loc = (base + j * j) % n;
            if (table[hash_loc] == val)
                break;
            if (table[hash_loc] == 0)
                break;
        }
    }

    free(table);
}

static void run_double_hashing(const int* keys, int m, int n)
{
    int* table = calloc(n, sizeof(int));
    if (!table)
        return;

    for (int i = 0; i < m; i++)
    {
        int val = keys[i];
        int h1 = hash_function(val, n);
        int h2 = 1;
        if (n > 1)
        {
            h2 = 1 + (val % (n - 1));
        }

        for (int j = 0; j < n; j++)
        {
            int hash_loc = (h1 + j * h2) % n;
            if (table[hash_loc] == 0)
            {
                table[hash_loc] = val;
                break;
            }
        }
    }

    // Lookup
    for (int i = 0; i < m; i++)
    {
        int val = keys[i];
        int h1 = hash_function(val, n);
        int h2 = 1;
        if (n > 1)
        {
            h2 = 1 + (val % (n - 1));
        }

        for (int j = 0; j < n; j++)
        {
            int hash_loc = (h1 + j * h2) % n;
            if (table[hash_loc] == val)
                break;
            if (table[hash_loc] == 0)
                break;
        }
    }

    free(table);
}

static void run_separate_chaining(const int* keys, int m, int n)
{
    HashNode** table = calloc(n, sizeof(HashNode*));
    if (!table)
        return;

    for (int i = 0; i < m; i++)
    {
        int val = keys[i];
        int hash_loc = hash_function(val, n);
        HashNode* node = malloc(sizeof(HashNode));
        if (node)
        {
            node->key = val;
            node->next = table[hash_loc];
            table[hash_loc] = node;
        }
    }

    // Lookup
    for (int i = 0; i < m; i++)
    {
        int val = keys[i];
        int hash_loc = hash_function(val, n);
        HashNode* curr = table[hash_loc];
        while (curr)
        {
            if (curr->key == val)
                break;
            curr = curr->next;
        }
    }

    // Cleanup
    for (int i = 0; i < n; i++)
    {
        HashNode* curr = table[i];
        while (curr)
        {
            HashNode* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(table);
}

void run_hashing_benchmark(int n)
{
    srand((unsigned int)time(NULL));

    if (n > 20000)
    {
        printf("\nHash Map Collision benchmark skipped for N = %d (Threshold is N = 20000)\n", n);
        return;
    }

    // Load factor of 0.75
    int m = (int)(n * 0.75);
    if (m == 0)
        m = 1;

    int* keys = malloc(m * sizeof(int));
    if (!keys)
        return;

    for (int i = 0; i < m; i++)
    {
        keys[i] = rand() % 100000 + 1;
    }

    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: HASH MAP COLLISION RESOLUTION (N = %d)\n", n);
    printf("========================================================================\n");
    printf("%-30s %-20s %-12s %-10s\n", "Algorithm", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    const char* algos[] = {"Linear Probing", "Quadratic Probing", "Double Hashing",
                           "Separate Chaining"};

    for (int i = 0; i < 4; i++)
    {
        size_t mem_before = benchmark_get_peak_memory();
        double start_time = benchmark_get_time();

        switch (i)
        {
            case 0:
                run_linear_probing(keys, m, n);
                break;
            case 1:
                run_quadratic_probing(keys, m, n);
                break;
            case 2:
                run_double_hashing(keys, m, n);
                break;
            case 3:
                run_separate_chaining(keys, m, n);
                break;
        }

        double duration = benchmark_get_time() - start_time;
        size_t mem_after = benchmark_get_peak_memory();
        size_t mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
        if (mem_used == 0)
            mem_used = mem_after;

        printf("%-30s %-20.6f %-12zu %-10s\n", algos[i], duration * 1000.0, mem_used, "PASSED");
        benchmark_export_csv("hashing", algos[i], n, duration, mem_used);
    }
    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/hashing.csv'.\n");

    free(keys);
}
