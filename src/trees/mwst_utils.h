#ifndef MWST_UTILS_H
#define MWST_UTILS_H

#include "trees.h"

/* -------------------------------------------------------
 * Shared utilities for multi-way search trees
 * Used by both B-Tree (btree.c) and B+ Tree (bplus_tree.c)
 * ------------------------------------------------------- */

/* Key array shifts */
void mwst_shift_keys_right(int* keys, int num_keys, int from);
void mwst_shift_keys_left(int* keys, int num_keys, int from);

/* Child pointer array shifts (for internal nodes) */
void mwst_shift_children_right(void** children, int num_keys, int from);
void mwst_shift_children_left(void** children, int num_keys, int from);

/* B+ Tree node allocation / destruction */
BPlusNode* bplus_node_create(int order, bool is_leaf);
void bplus_node_free(BPlusNode* node);
void bplus_node_destroy_recursive(BPlusNode* node);

/* B+ Tree invariant validators (debug builds) */
bool bplus_validate_key_ordering(BPlusNode* node);
bool bplus_validate_child_count(BPlusNode* node);
bool bplus_validate_leaf_links(BPlusNode* node);

#endif
