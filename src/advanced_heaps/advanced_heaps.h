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

/* Constructor & Destructor */
FibonacciNode* create_fibonacci_node(int key, int value);
void destroy_fibonacci_heap(FibonacciNode* min_node);

/* Core Heap Operations */
FibonacciNode* fib_heap_insert(FibonacciNode* min_node, int key, int value);
FibonacciNode* fib_heap_union(FibonacciNode* min1, FibonacciNode* min2);
FibonacciNode* fib_heap_extract_min(FibonacciNode* min_node, int* min_key, int* min_val);
FibonacciNode* fib_heap_decrease_key(FibonacciNode* min_node, FibonacciNode* target, int new_key);
FibonacciNode* fib_heap_delete(FibonacciNode* min_node, FibonacciNode* target);
FibonacciNode* fib_heap_find_node(FibonacciNode* min_node, int key);
#endif /* ADVANCED_HEAPS_H */
