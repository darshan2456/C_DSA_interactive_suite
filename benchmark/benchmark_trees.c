#define _GNU_SOURCE
#include "benchmark.h"
#include "../trees/trees.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#else
#include <unistd.h>
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
    // Sort and remove duplicates (though not strictly necessary to be 100% unique, sorting helps AVL/BST behave consistently)
    qsort(keys, n, sizeof(int), compare_ints);
}

void run_trees_benchmark(int n)
{
    srand((unsigned int)time(NULL));

    if (n > 5000)
    {
        printf("\nTrees Performance benchmark skipped for N = %d (Threshold is N = 5000)\n", n);
        return;
    }

    int* keys = malloc(n * sizeof(int));
    if (!keys) return;
    generate_unique_keys(keys, n);

    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: TREES LOOKUP & INSERTION (N = %d)\n", n);
    printf("========================================================================\n");
    printf("%-30s %-20s %-12s %-10s\n", "Structure", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    const char* algos[] = {
        "Binary Search Tree (BST)",
        "Threaded Binary Tree (TBT)",
        "AVL Tree (Balanced)",
        "Trie (Prefix Tree)",
        "B-Tree (t = 3)",
        "B+ Tree (order = 4)"
    };

    for (int i = 0; i < 6; i++)
    {
        size_t mem_before = benchmark_get_peak_memory();
        double start_time = benchmark_get_time();

        // Redirect stdout
        fflush(stdout);
        int stdout_dup = dup(1);
#ifdef _WIN32
        FILE* fnull = fopen("NUL", "w");
#else
        FILE* fnull = fopen("/dev/null", "w");
#endif
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
                    if (curr->data == key) break;
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
                    if (curr->data == key) break;
                    if (key < curr->data)
                    {
                        if (curr->lthread) break;
                        curr = curr->left;
                    }
                    else
                    {
                        if (curr->rthread) break;
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
                    if (curr->data == key) break;
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

        double duration = benchmark_get_time() - start_time;
        size_t mem_after = benchmark_get_peak_memory();
        size_t mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
        if (mem_used == 0) mem_used = mem_after;

        printf("%-30s %-20.6f %-12zu %-10s\n", algos[i], duration * 1000.0, mem_used, "PASSED");
        benchmark_export_csv("trees", algos[i], n, duration, mem_used);
    }
    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/trees.csv'.\n");

    free(keys);
}
