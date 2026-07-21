#include "advanced_heaps.h"
#include "fuzzer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void run_binomial_fuzz(FuzzerState* fuzzer, int ops)
{
    BinomialNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "Binomial insert %d:%d", key, val);
            root = binomial_heap_insert(root, key, val);
        }
        else if (op == 1 && root != NULL)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "Binomial extract min");
            root = binomial_heap_extract_min(root, &mk, &mv);
        }
        else
        {
            fuzzer_log_op(fuzzer, "Binomial find %d", key);
            binomial_heap_find_node(root, key);
        }
    }
    destroy_binomial_heap(root);
}

void run_fibonacci_fuzz(FuzzerState* fuzzer, int ops)
{
    FibonacciNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "Fibonacci insert %d:%d", key, val);
            root = fib_heap_insert(root, key, val);
        }
        else if (op == 1 && root != NULL)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "Fibonacci extract min");
            root = fib_heap_extract_min(root, &mk, &mv);
        }
        else if (op == 2 && root != NULL)
        {
            fuzzer_log_op(fuzzer, "Fibonacci delete %d", key);
            FibonacciNode* target = fib_heap_find_node(root, key);
            if (target != NULL)
            {
                root = fib_heap_delete(root, target);
            }
        }
    }
    destroy_fibonacci_heap(root);
}

void run_minmax_fuzz(FuzzerState* fuzzer, int ops)
{
    MinMaxHeap* heap = create_min_max_heap(ops + 10);
    assert(heap != NULL);
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "MinMax insert %d:%d", key, val);
            min_max_heap_insert(heap, key, val);
        }
        else if (op == 1)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "MinMax extract min");
            min_max_heap_extract_min(heap, &mk, &mv);
        }
        else
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "MinMax extract max");
            min_max_heap_extract_max(heap, &mk, &mv);
        }
    }
    destroy_min_max_heap(heap);
}

void run_leftist_fuzz(FuzzerState* fuzzer, int ops)
{
    LeftistNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 1);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "Leftist insert %d:%d", key, val);
            root = leftist_heap_insert(root, key, val);
        }
        else if (op == 1 && root != NULL)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "Leftist extract min");
            root = leftist_heap_extract_min(root, &mk, &mv);
        }
    }
    destroy_leftist_heap(root);
}

void run_skew_fuzz(FuzzerState* fuzzer, int ops)
{
    SkewNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 1);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "Skew insert %d:%d", key, val);
            root = skew_heap_insert(root, key, val);
        }
        else if (op == 1 && root != NULL)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "Skew extract min");
            root = skew_heap_extract_min(root, &mk, &mv);
        }
    }
    destroy_skew_heap(root);
}

void run_dary_fuzz(FuzzerState* fuzzer, int ops)
{
    DAryHeap* heap = create_dary_heap(ops + 10, 3);
    assert(heap != NULL);
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "d-Ary insert %d:%d", key, val);
            dary_heap_insert(heap, key, val);
        }
        else if (op == 1)
        {
            int mk, mv;
            fuzzer_log_op(fuzzer, "d-Ary extract min");
            dary_heap_extract_min(heap, &mk, &mv);
        }
        else
        {
            fuzzer_log_op(fuzzer, "d-Ary find index %d", key);
            dary_heap_find_index(heap, key);
        }
    }
    destroy_dary_heap(heap);
}

void run_treap_fuzz(FuzzerState* fuzzer, int ops)
{
    TreapNode* root = NULL;
    for (int i = 0; i < ops; i++)
    {
        int key = fuzzer_rand_int(fuzzer, 1, 1000);
        int val = fuzzer_rand_int(fuzzer, 1, 1000);
        int pri = fuzzer_rand_int(fuzzer, 1, 1000);
        int op = fuzzer_rand_int(fuzzer, 0, 2);
        if (op == 0)
        {
            fuzzer_log_op(fuzzer, "Treap insert %d:%d:%d", key, val, pri);
            root = treap_insert(root, key, val, pri);
        }
        else if (op == 1)
        {
            fuzzer_log_op(fuzzer, "Treap delete %d", key);
            root = treap_delete(root, key);
        }
        else
        {
            fuzzer_log_op(fuzzer, "Treap find %d", key);
            treap_find_node(root, key);
        }
    }
    destroy_treap(root);
}

int main(void)
{
    unsigned int seed = (unsigned int)time(NULL);
    FuzzerState fuzzer;
    printf("Starting Heap Fuzzing with seed: %u\n", seed);

    fuzzer_init(&fuzzer, seed, 5000);
    run_binomial_fuzz(&fuzzer, 500);
    run_fibonacci_fuzz(&fuzzer, 500);
    run_minmax_fuzz(&fuzzer, 500);
    run_leftist_fuzz(&fuzzer, 500);
    run_skew_fuzz(&fuzzer, 500);
    run_dary_fuzz(&fuzzer, 500);
    run_treap_fuzz(&fuzzer, 500);
    fuzzer_free(&fuzzer);

    printf("Heap Fuzzing completed successfully!\n");
    return 0;
}
