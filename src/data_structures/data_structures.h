#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
/*
 * DATA STRUCTURES INDEX
 *
 * This header aggregates all data structures used in the project.
 * The data structures appear in the following order:
 *
 * 1. Arrays
 * 2. Binary Search Tree (BST)
 * 3. Circular Queue
 * 4. Doubly Linked List
 * 5. Singly Linked List
 * 6. Threaded Binary Tree (TBT)
 * 7. Priority Queue
 *
 * Refer to the corresponding section below for structure
 * definitions and function declarations.
 */
#define HEAP_CAPACITY 100
#include <stdbool.h>
#include <stddef.h>

// For arrays
void swap_adjacent_pairs(int arr[], int length_of_array);
int max_array(const int arr[], int length_of_array);
int min_array(const int arr[], int length_of_array);
void print_array(const int arr[], int length_of_array);
void reverse_array(int arr[], int length_of_array);
void array_demo(void);

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

// For circular queue
typedef struct circular_queue
{
    int rear;
    int front;
    int N;
    int* arr;
} circular_queue;
int init_circ_queue(int N, circular_queue* queue_ptr);
void destroy_circ_queue(circular_queue* queue_ptr);
int enqueue(circular_queue* queue_ptr, int value);
int dequeue(circular_queue* queue_ptr);
void display_circ_queue(circular_queue* queue_ptr);
void circular_queue_Demo(void);

// For double linked list
typedef struct doubly_ll_Node
{
    int data;
    struct doubly_ll_Node* prev;
    struct doubly_ll_Node* next;
} doubly_ll_Node;
int dll_insertAtBeginning(doubly_ll_Node** head_ref, int value);
int dll_insertAtEnd(doubly_ll_Node** head_ref, int value);
void dll_printlist(const doubly_ll_Node* head);
int dll_search(const doubly_ll_Node* head, int key);
int dll_deleteAtBeginning(doubly_ll_Node** head_ref);
int dll_deleteAtEnd(doubly_ll_Node** head_ref);
int dll_deleteByValue(doubly_ll_Node** head_ref, int key);
void delete_dll(doubly_ll_Node* head);
void dll_demo(void);
int dll_reverselist(doubly_ll_Node** head_ref);
int dll_getLength(const doubly_ll_Node* head);
int dll_insertAtPosition(doubly_ll_Node** head_ref, int value, int position);
int dll_deleteAtPosition(doubly_ll_Node** head_ref, int position);

// For single linked list
typedef struct Node
{
    int data;
    struct Node* next;
} Node;
void sll_printlist(const Node* head);
int sll_insertAtEnd(Node** head_ref, int value);
int sll_deleteAtBeginning(Node** head_ref);
int sll_deleteAtEnd(Node** head_ref);
int sll_deleteByValue(Node** head_ref, int value);
int sll_insertAtBeginning(Node** head_ref, int value);
void sll_Demo(void);
int sll_search(const Node* head, int key);
int sll_reverseList(Node** head_ref);
void delete_sll(Node* head);
int sll_getLength(const Node* head);
int sll_insertAtPosition(Node** head_ref, int value, int position);
int sll_deleteAtPosition(Node** head_ref, int position);

// For Threadded Binary Tree
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

// For Priority Queue
typedef enum
{
    MIN_HEAP = 0,
    MAX_HEAP = 1
} HeapType;

typedef struct priority_queue
{
    int size;
    HeapType heapType;
    int heap[HEAP_CAPACITY];
} priority_queue;

priority_queue* pq_init(HeapType heapType);
int insert(priority_queue* pq, int val);
bool extractTop(priority_queue* pq, int* result);
bool peek_pq(priority_queue* pq, int* result);
void destroy_pq(priority_queue* pq);
void display_heap(priority_queue* pq);
void priority_queue_demo(void);

// For singly circular linked list
// Invariant: when non-empty, tail->next == head (the ring closes back onto head);
// when empty, head == tail == NULL and length == 0. head and tail are both tracked and
// length is cached so getLength is O(1) and positional inserts need no counting pass.
typedef struct scll_Node
{
    int data;
    struct scll_Node* next;
} scll_Node;
typedef struct scll
{
    scll_Node* head;
    scll_Node* tail;
    int length;
} scll;
void scll_init(scll* list);
int scll_insertAtBeginning(scll* list, int value);
int scll_insertAtEnd(scll* list, int value);
int scll_insertAtPosition(scll* list, int value, int position);
int scll_deleteAtBeginning(scll* list);
int scll_deleteAtEnd(scll* list);
int scll_deleteByValue(scll* list, int value);
int scll_deleteAtPosition(scll* list, int position);
int scll_search(const scll* list, int key);
int scll_getLength(const scll* list);
void scll_printlist(const scll* list);
void scll_destroy(scll* list);
void scll_Demo(void);

// For simple (linear) queue
// A fixed-capacity array queue WITHOUT wrap-around. front and rear are array indices (both -1
// when empty) and rear only ever advances. Once rear reaches N-1 the queue reports full even
// if dequeues freed slots at the front - that freed space is never reused. This "false
// overflow" is the limitation the circular queue (above) solves by wrapping front/rear modulo
// N; the two live side-by-side for comparison.
typedef struct simple_queue
{
    int front;
    int rear;
    int N;
    int* arr;
} simple_queue;
int init_simple_queue(int N, simple_queue* queue_ptr);
void destroy_simple_queue(simple_queue* queue_ptr);
int enqueue_simple(simple_queue* queue_ptr, int value);
int dequeue_simple(simple_queue* queue_ptr);
void display_simple_queue(const simple_queue* queue_ptr);
void simple_queue_Demo(void);

// For Double-Ended Queue (Deque)
typedef struct deque
{
    int front;
    int rear;
    int N;
    int* arr;
} deque;
int init_deque(int N, deque* dq);
void destroy_deque(deque* dq);
int deque_insert_front(deque* dq, int value);
int deque_insert_rear(deque* dq, int value);
int deque_delete_front(deque* dq, int* val);
int deque_delete_rear(deque* dq, int* val);
int deque_get_front(const deque* dq);
int deque_get_rear(const deque* dq);
bool deque_is_empty(const deque* dq);
bool deque_is_full(const deque* dq);
void display_deque(const deque* dq);
void deque_demo(void);

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
    struct TrieNode *children[TRIE_ALPHA_SIZE];
    int is_end;
} TrieNode;

TrieNode *trie_create_node(void);
void      trie_insert(TrieNode *root, const char *word);
int       trie_search(TrieNode *root, const char *word);
int       trie_starts_with_prefix(TrieNode *root, const char *prefix);
void      trie_delete(TrieNode *root, const char *word);
void      trie_free(TrieNode *node);
void      trie_demo(void);

#endif
