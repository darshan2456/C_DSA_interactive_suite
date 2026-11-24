#include "fuzzer.h"
#include "hash.h"
#include "sll.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VALS 1000

// Helper function to check if array contains a value
static bool contains(int* arr, int size, int val)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == val)
            return true;
    }
    return false;
}

static void run_hash_function_fuzz(FuzzerState* fuzzer, int ops)
{
    for (int i = 0; i < ops; i++)
    {
        int val = fuzzer_rand_int(fuzzer, -50000, 50000);
        int size = fuzzer_rand_int(fuzzer, 1, 1000);
        int h = hash_function(val, size);
        assert(h >= 0 && h < size);
        // Determinism check
        assert(hash_function(val, size) == h);
    }
}

static void run_separate_chaining_fuzz(FuzzerState* fuzzer, int ops)
{
    for (int i = 0; i < ops; i++)
    {
        int table_size = fuzzer_rand_int(fuzzer, 5, 50);
        Node** table = (Node**)calloc(table_size, sizeof(Node*));

        int truth_set[MAX_VALS];
        int truth_count = 0;

        int inserts = fuzzer_rand_int(fuzzer, 10, 200);

        for (int j = 0; j < inserts; j++)
        {
            int val = fuzzer_rand_int(fuzzer, -10000, 10000);
            if (!contains(truth_set, truth_count, val))
            {
                assert(separate_chaining_insert(table, table_size, val) == 1);
                truth_set[truth_count++] = val;
            }
        }

        // Search known
        for (int j = 0; j < truth_count; j++)
        {
            assert(separate_chaining_search(table, table_size, truth_set[j]) == 1);
        }

        // Search unknown
        for (int j = 0; j < 50; j++)
        {
            int r = fuzzer_rand_int(fuzzer, -10000, 10000);
            if (!contains(truth_set, truth_count, r))
            {
                assert(separate_chaining_search(table, table_size, r) == 0);
            }
        }

        // Delete a few
        if (truth_count > 0)
        {
            int dels = fuzzer_rand_int(fuzzer, 1, truth_count);
            for (int j = 0; j < dels; j++)
            {
                int idx = fuzzer_rand_int(fuzzer, 0, truth_count - 1);
                int val = truth_set[idx];

                assert(separate_chaining_delete(table, table_size, val) == 1);
                assert(separate_chaining_search(table, table_size, val) == 0);

                // Swap with last to remove from truth set
                truth_set[idx] = truth_set[truth_count - 1];
                truth_count--;
            }
        }

        for (int k = 0; k < table_size; k++)
        {
            if (table[k] != NULL)
            {
                delete_sll(table[k], NULL);
            }
        }
        free(table);
    }
}

static void run_open_addressing_fuzz(FuzzerState* fuzzer, int ops, int type)
{
    // type: 0 = Linear, 1 = Quadratic, 2 = Double
    for (int i = 0; i < ops; i++)
    {
        // Must use prime numbers for quadratic/double hashing to guarantee insertion
        // We will just pick a decently large prime.
        int primes[] = {11, 19, 31, 47, 97, 199, 401};
        int capacity = primes[fuzzer_rand_int(fuzzer, 0, 6)];

        int* table = (int*)malloc(capacity * sizeof(int));
        for (int k = 0; k < capacity; k++)
            table[k] = HASH_EMPTY_SLOT;

        int truth_set[MAX_VALS];
        int truth_count = 0;

        // Load factor max 50% for quadratic to avoid infinite loops, up to 70% for linear. We use
        // 50% max.
        int inserts = capacity / 2;

        for (int j = 0; j < inserts; j++)
        {
            int val = fuzzer_rand_int(
                fuzzer, 1, 10000); // Positive values > 0 to avoid HASH_EMPTY_SLOT (0) collision
            if (!contains(truth_set, truth_count, val))
            {
                bool success = false;
                if (type == 0)
                    success = linear_probing_insert(table, capacity, val);
                else if (type == 1)
                    success = quadratic_probing_insert(table, capacity, val);
                else
                    success = double_hashing_insert(table, capacity, val);

                assert(success == true);
                truth_set[truth_count++] = val;
            }
        }

        for (int j = 0; j < truth_count; j++)
        {
            int search_res = -1;
            if (type == 0)
                search_res = linear_probing_search(table, capacity, truth_set[j]);
            else if (type == 1)
                search_res = quadratic_probing_search(table, capacity, truth_set[j]);
            else
                search_res = double_hashing_search(table, capacity, truth_set[j]);

            assert(search_res != -1);
        }

        for (int j = 0; j < 20; j++)
        {
            int r = fuzzer_rand_int(fuzzer, 1, 10000);
            if (!contains(truth_set, truth_count, r))
            {
                int search_res = -1;
                if (type == 0)
                    search_res = linear_probing_search(table, capacity, r);
                else if (type == 1)
                    search_res = quadratic_probing_search(table, capacity, r);
                else
                    search_res = double_hashing_search(table, capacity, r);

                assert(search_res == -1);
            }
        }

        free(table);
    }
}

int main(void)
{
    unsigned int seed = (unsigned int)time(NULL);
    FuzzerState fuzzer;

    printf("Starting Hashing Algorithms Fuzzing with seed: %u\n", seed);

    fuzzer_init(&fuzzer, seed, 600); // 600 max iterations if no valgrind variable

    // Check if running under Valgrind CI to reduce iterations
    int run_iterations = 300;
    if (getenv("VALGRIND_MODE") != NULL || getenv("CI_MEMORY_CHECK") != NULL)
    {
        run_iterations = 20;
        printf("Detected Valgrind Mode. Reducing iterations to %d\n", run_iterations);
    }

    printf("--- Running Hash Function Fuzz Tests ---\n");
    run_hash_function_fuzz(&fuzzer, run_iterations);

    printf("--- Running Separate Chaining Fuzz Tests ---\n");
    run_separate_chaining_fuzz(&fuzzer, run_iterations);

    printf("--- Running Linear Probing Fuzz Tests ---\n");
    run_open_addressing_fuzz(&fuzzer, run_iterations, 0);

    printf("--- Running Quadratic Probing Fuzz Tests ---\n");
    run_open_addressing_fuzz(&fuzzer, run_iterations, 1);

    printf("--- Running Double Hashing Fuzz Tests ---\n");
    run_open_addressing_fuzz(&fuzzer, run_iterations, 2);

    fuzzer_free(&fuzzer);
    printf("Hashing Algorithms Fuzzing completed successfully!\n");
    return 0;
}
