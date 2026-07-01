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

#endif /* ADVANCED_HEAPS_H */
