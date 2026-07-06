#define _GNU_SOURCE
#include "../trees/trees.h"
#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#define DEV_NULL "NUL"
#else
#include <unistd.h>
#define DEV_NULL "/dev/null"
#endif

static int compare_ints(const void* a, const void* b)
{
    return (*(const int*)a - *(const int*)b);
}

static void generate_unique_keys(int* keys, int n)
{
    for (int i = 0; i < n; i++)
    {
        keys[i] = rand() % (n * 10) + 1;
    }
    // Sort and remove duplicates (though not strictly necessary to be 100% unique, sorting helps
    // AVL/BST behave consistently)
    qsort(keys, n, sizeof(int), compare_ints);
}

void run_trees_benchmark(int n)
{
    // Seed random generator with fixed seed
    srand(BENCHMARK_SEED);

    if (n > 5000)
    {
        printf("\nTrees Performance benchmark skipped for N = %d (Threshold is N = 5000)\n", n);
        return;
    }

    int* keys = malloc(n * sizeof(int));
    if (!keys)
        return;
    generate_unique_keys(keys, n);

    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: TREES LOOKUP & INSERTION (N = %d)\n", n);
    printf("========================================================================\n");
    printf("%-30s %-25s %-15s %-10s\n", "Structure", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    const char* algos[] = {"Binary Search Tree (BST)",
                           "Threaded Binary Tree (TBT)",
                           "AVL Tree (Balanced)",
                           "Trie (Prefix Tree)",
                           "B-Tree (t = 3)",
                           "B+ Tree (order = 4)",
                           "Segment Tree",
                           "Fenwick Tree (BIT)"};

    for (int i = 0; i < 8; i++)
    {
        double times[BENCHMARK_DEFAULT_ITERATIONS];
        size_t peak_mem = 0;

        RUN_BENCHMARK(times, peak_mem, {
            // Redirect stdout
            fflush(stdout);
            int stdout_dup = dup(1);
            FILE* fnull = fopen(DEV_NULL, "w");
            if (fnull != NULL && stdout_dup >= 0)
            {
                dup2(fileno(fnull), 1);
            }

            if (i == 0) // BST
            {
                bstNode* root = NULL;
                for (int k = 0; k < n; k++)
                {
                    bst_insert(&root, keys[k]);
                }
                for (int k = 0; k < n; k++)
                {
                    bstNode* curr = root;
                    int key = keys[k];
                    while (curr)
                    {
                        if (curr->data == key)
                            break;
                        curr = (key < curr->data) ? curr->left : curr->right;
                    }
                }
                destroy_bst(root);
            }
            else if (i == 1) // TBT
            {
                TBTnode* root = NULL;
                for (int k = 0; k < n; k++)
                {
                    insert_node_tbt(&root, keys[k]);
                }
                for (int k = 0; k < n; k++)
                {
                    TBTnode* curr = root;
                    int key = keys[k];
                    while (curr)
                    {
                        if (curr->data == key)
                            break;
                        if (key < curr->data)
                        {
                            if (curr->lthread)
                                break;
                            curr = curr->left;
                        }
                        else
                        {
                            if (curr->rthread)
                                break;
                            curr = curr->right;
                        }
                    }
                }
                destroy_tbt(root);
            }
            else if (i == 2) // AVL
            {
                avlNode* root = NULL;
                for (int k = 0; k < n; k++)
                {
                    avl_insert(&root, keys[k]);
                }
                for (int k = 0; k < n; k++)
                {
                    avlNode* curr = root;
                    int key = keys[k];
                    while (curr)
                    {
                        if (curr->data == key)
                            break;
                        curr = (key < curr->data) ? curr->left : curr->right;
                    }
                }
                destroy_avl(root);
            }
            else if (i == 3) // Trie
            {
                TrieNode* root = trie_create_node();
                char buf[32];
                for (int k = 0; k < n; k++)
                {
                    sprintf(buf, "%d", keys[k]);
                    trie_insert(root, buf);
                }
                for (int k = 0; k < n; k++)
                {
                    sprintf(buf, "%d", keys[k]);
                    trie_search(root, buf);
                }
                trie_free(root);
            }
            else if (i == 4) // B-Tree
            {
                btreeNode* root = NULL;
                int t = 3;
                for (int k = 0; k < n; k++)
                {
                    btree_insert(&root, keys[k], t);
                }
                for (int k = 0; k < n; k++)
                {
                    btree_search(root, keys[k]);
                }
                btree_destroy(root);
            }
            else if (i == 5) // B+ Tree
            {
                BPlusTree* tree = bplus_tree_create(4);
                if (tree)
                {
                    for (int k = 0; k < n; k++)
                    {
                        bplus_tree_insert(tree, keys[k], keys[k]);
                    }
                    int val_out;
                    for (int k = 0; k < n; k++)
                    {
                        bplus_tree_search(tree, keys[k], &val_out);
                    }
                    bplus_tree_destroy(tree);
                }
            }
            else if (i == 6) // Segment Tree Simulation
            {
                SegmentTree* st = create_segment_tree(keys, n);
                if (st != NULL)
                {
                    for (int k = 0; k < n; k++)
                    {
                        update_point(st, 1, 0, n - 1, k, keys[k] / 2);
                    }
                    destroy_segment_tree(st);
                }
            }
            else if (i == 7) // Fenwick Tree Simulation
            {
                FenwickTree* ft = create_fenwick_tree(n);
                if (ft != NULL)
                {
                    for (int k = 0; k < n; k++)
                    {
                        fenwick_range_update(ft, 1, k + 1, keys[k]);
                    }
                    destroy_fenwick_tree(ft);
                }
            }

            // Restore stdout
            fflush(stdout);
            if (stdout_dup >= 0)
            {
                dup2(stdout_dup, 1);
                close(stdout_dup);
            }
            if (fnull != NULL)
            {
                fclose(fnull);
            }
        });

        benchmark_report_result("trees", algos[i], n, times, peak_mem);
    }
    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/trees.csv'.\n");

    free(keys);
}
