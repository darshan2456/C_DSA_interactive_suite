#ifndef ADVANCED_HEAPS_H
#define ADVANCED_HEAPS_H

#include <stdbool.h>

/*
 * =========================================================================
 *                              Binomial Heap
 * =========================================================================
 */

typedef struct BinomialNode
{
    int key;
    int value;
    int degree;
    struct BinomialNode* parent;
    struct BinomialNode* child;
    struct BinomialNode* sibling;
} BinomialNode;

/* Constructor & Destructor */
BinomialNode* create_binomial_node(int key, int value);
void destroy_binomial_heap(BinomialNode* head);

/* Core Heap Operations */
BinomialNode* binomial_heap_insert(BinomialNode* head, int key, int value);
BinomialNode* binomial_heap_merge(BinomialNode* head1, BinomialNode* head2);
BinomialNode* binomial_heap_extract_min(BinomialNode* head, int* min_key, int* min_val);
BinomialNode* binomial_heap_decrease_key(BinomialNode* head, BinomialNode* target, int new_key);
/* Search / Utility Helpers */
BinomialNode* binomial_heap_find_node(BinomialNode* head, int key);

/*
 * =========================================================================
 *                              Fibonacci Heap
 * =========================================================================
 */

typedef struct FibonacciNode
{
    int key;
    int value;
    int degree;
    bool mark;
    struct FibonacciNode* parent;
    struct FibonacciNode* child;
    struct FibonacciNode* left;
    struct FibonacciNode* right;
} FibonacciNode;

FibonacciNode* create_fibonacci_node(int key, int value);
void destroy_fibonacci_heap(FibonacciNode* min_node);
FibonacciNode* fib_heap_insert(FibonacciNode* min_node, int key, int value);
FibonacciNode* fib_heap_union(FibonacciNode* min1, FibonacciNode* min2);
FibonacciNode* fib_heap_extract_min(FibonacciNode* min_node, int* min_key, int* min_val);
FibonacciNode* fib_heap_decrease_key(FibonacciNode* min_node, FibonacciNode* target, int new_key);
FibonacciNode* fib_heap_delete(FibonacciNode* min_node, FibonacciNode* target);
FibonacciNode* fib_heap_find_node(FibonacciNode* min_node, int key);

/*
 * =========================================================================
 *                           Leftist & Skew Heaps
 * =========================================================================
 */

typedef struct LeftistNode
{
    int key;
    int value;
    int npl;
    struct LeftistNode* left;
    struct LeftistNode* right;
} LeftistNode;

typedef struct SkewNode
{
    int key;
    int value;
    struct SkewNode* left;
    struct SkewNode* right;
} SkewNode;

LeftistNode* create_leftist_node(int key, int value);
void destroy_leftist_heap(LeftistNode* root);
LeftistNode* leftist_heap_merge(LeftistNode* h1, LeftistNode* h2);
LeftistNode* leftist_heap_insert(LeftistNode* root, int key, int value);
LeftistNode* leftist_heap_extract_min(LeftistNode* root, int* min_key, int* min_val);

SkewNode* create_skew_node(int key, int value);
void destroy_skew_heap(SkewNode* root);
SkewNode* skew_heap_merge(SkewNode* h1, SkewNode* h2);
SkewNode* skew_heap_insert(SkewNode* root, int key, int value);
SkewNode* skew_heap_extract_min(SkewNode* root, int* min_key, int* min_val);

/*
 * =========================================================================
 *                  Min-Max Heap (Double-Ended Queue)
 * =========================================================================
 */

typedef struct
{
    int key;
    int value;
} MinMaxHeapNode;

typedef struct
{
    MinMaxHeapNode* arr;
    int capacity;
    int size;
} MinMaxHeap;

MinMaxHeap* create_min_max_heap(int capacity);
void destroy_min_max_heap(MinMaxHeap* heap);
bool min_max_heap_insert(MinMaxHeap* heap, int key, int value);
bool min_max_heap_get_min(MinMaxHeap* heap, int* min_key, int* min_val);
bool min_max_heap_get_max(MinMaxHeap* heap, int* max_key, int* max_val);
bool min_max_heap_extract_min(MinMaxHeap* heap, int* min_key, int* min_val);
bool min_max_heap_extract_max(MinMaxHeap* heap, int* max_key, int* max_val);
#endif /* ADVANCED_HEAPS_H */
