#ifndef TREES_H
#define TREES_H

#include <stdbool.h>
#include <stddef.h>

/*
 * TREES MODULE
 *
 * This module is the dedicated directory for tree-based data structures.
 * It includes:
 *   - Binary Search Tree (BST)
 *   - Threaded Binary Tree (TBT)
 *   - AVL Tree
 *   - Trie (Prefix Tree)
 *
 * Upcoming implementations:
 *   - B-Tree
 *   - B+ Tree
 */

/* Top-level demo dispatcher - called from main menu (option 8) */
void trees_demo(void);

// For Binary Search Tree
typedef struct bstNode
{
    int data;
    struct bstNode* left;
    struct bstNode* right;
} bstNode;
void binary_search_tree_Demo(void);
int bst_insert(bstNode** head_ref, int value);
void bst_inorder(const bstNode* head);
void bst_preorder(const bstNode* head);
void bst_postorder(const bstNode* head);
int countnodes(const bstNode* head);
int tree_height(const bstNode* root);
void destroy_bst(bstNode* head);
bstNode* bst_delete(bstNode* root, int value);

// For Threaded Binary Tree
typedef struct TBTnode
{
    int data;
    bool lthread;
    bool rthread;
    struct TBTnode* left;
    struct TBTnode* right;
} TBTnode;
TBTnode* leftmost(TBTnode* node);
TBTnode* create_node_tbt(int data);
void preorder_tbt(TBTnode* node);
void inorder_tbt(TBTnode* node);
int insert_node_tbt(TBTnode** root_ref, int key);
void destroy_tbt(TBTnode* node);
void TBT_demo(void);

// For AVL Tree (Self-Balancing Binary Search Tree)
typedef struct avlNode
{
    int data;
    int height;
    struct avlNode* left;
    struct avlNode* right;
} avlNode;
int avl_insert(avlNode** root_ref, int value);
int avl_delete(avlNode** root_ref, int value);
int avl_height(const avlNode* node);
int avl_balance_factor(const avlNode* node);
void avl_inorder(const avlNode* root);
void avl_preorder(const avlNode* root);
void avl_postorder(const avlNode* root);
void destroy_avl(avlNode* root);
void avl_demo(void);

// For Trie (Prefix Tree)
#define TRIE_ALPHA_SIZE 26

typedef struct TrieNode
{
    struct TrieNode* children[TRIE_ALPHA_SIZE];
    int is_end;
} TrieNode;

TrieNode* trie_create_node(void);
void trie_insert(TrieNode* root, const char* word);
int trie_search(TrieNode* root, const char* word);
int trie_starts_with_prefix(TrieNode* root, const char* prefix);
void trie_delete(TrieNode* root, const char* word);
void trie_free(TrieNode* node);
void trie_demo(void);

// For B-Tree
#define BTREE_MAX_DEGREE 10

typedef struct btreeNode
{
    int keys[2 * BTREE_MAX_DEGREE - 1];
    struct btreeNode* children[2 * BTREE_MAX_DEGREE];
    int num_keys;
    int is_leaf;
} btreeNode;

btreeNode* btree_create_node(int is_leaf);
int btree_insert(btreeNode** root_ref, int key, int t);
int btree_search(btreeNode* root, int key);
void btree_traverse(const btreeNode* root);
btreeNode* btree_delete(btreeNode* root, int key, int t);
void btree_destroy(btreeNode* root);
void btree_demo(void);

// For B+ Tree
typedef struct BPlusNode
{
    bool is_leaf;
    int num_keys;
    int* keys;
    struct BPlusNode** children;
    int* values;
    struct BPlusNode* next;
    struct BPlusNode* prev;
} BPlusNode;

typedef struct BPlusTree
{
    BPlusNode* root;
    int order;
} BPlusTree;

BPlusTree* bplus_tree_create(int order);
void bplus_tree_destroy(BPlusTree* tree);
bool bplus_tree_search(BPlusTree* tree, int key, int* value_out);
bool bplus_tree_insert(BPlusTree* tree, int key, int value);
bool bplus_tree_delete(BPlusTree* tree, int key);
void bplus_tree_range_query(BPlusTree* tree, int lower, int upper);
void bplus_tree_print(BPlusTree* tree);
void bplus_tree_demo(void);

#endif
