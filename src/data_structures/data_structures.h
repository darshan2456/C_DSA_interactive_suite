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
void sll_demo(void);
int sll_search(const Node* head, int key);
int sll_reverseList(Node** head_ref);
void delete_sll(Node* head);
int sll_getLength(const Node* head);
int sll_insertAtPosition(Node** head_ref, int value, int position);
int sll_deleteAtPosition(Node** head_ref, int position);

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
void scll_demo(void);

// Universal non-speacial queue structure
typedef struct Queue
{
    int rear;
    int front;
    int N;
    void** arr;
} Queue;

// For circular queue
int init_circ_queue(int N, Queue* queue_ptr);
void destroy_circ_queue(Queue* queue_ptr);
int enqueue(Queue* queue_ptr, void* value);
void* dequeue(Queue* queue_ptr);
void display_circ_queue(Queue* queue_ptr);
void circular_queue_demo(void);

// For simple (linear) queue
int init_simple_queue(int N, Queue* queue_ptr);
void destroy_simple_queue(Queue* queue_ptr);
int enqueue_simple(Queue* queue_ptr, void* value);
void* dequeue_simple(Queue* queue_ptr);
void display_simple_queue(const Queue* queue_ptr);
void simple_queue_demo(void);

// For Double-Ended Queue (Deque)
int init_deque(int N, Queue* dq);
void destroy_deque(Queue* dq);
int deque_insert_front(Queue* dq, void* value);
int deque_insert_rear(Queue* dq, void* value);
void* deque_delete_front(Queue* dq);
void* deque_delete_rear(Queue* dq);
int deque_get_front(const Queue* dq);
int deque_get_rear(const Queue* dq);
bool deque_is_empty(const Queue* dq);
bool deque_is_full(const Queue* dq);
void display_deque(const Queue* dq);
void deque_demo(void);

// For Doubly cirular linked list

typedef struct dcll_Node
{
    int data;
    struct dcll_Node* next;
    struct dcll_Node* prev;
} dcll_Node;

typedef struct dcll
{
    dcll_Node* head;
    dcll_Node* tail;
    int length;
} dcll;

void dcll_init(dcll* list);
int dcll_insertAtBeginning(dcll* list, int value);
int dcll_insertAtEnd(dcll* list, int value);
int dcll_insertAtPosition(dcll* list, int value, int position);
int dcll_deleteAtBeginning(dcll* list);
int dcll_deleteAtEnd(dcll* list);
int dcll_deleteByValue(dcll* list, int value);
int dcll_deleteAtPosition(dcll* list, int position);
int dcll_search(const dcll* list, int key);
int dcll_getLength(const dcll* list);
void dcll_printlist(const dcll* list);
void dcll_destroy(dcll* list);
void dcll_demo(void);
// ==========================================
// Segment Tree (Dynamic Implementation)
// ==========================================

typedef struct
{
    int* tree;
    int size;
    int original_array_size;
} SegmentTree;

SegmentTree* create_segment_tree(int arr[], int n);
void destroy_segment_tree(SegmentTree* st);
void build_tree(SegmentTree* st, int arr[], int node, int start, int end);
void update_point(SegmentTree* st, int node, int start, int end, int idx, int val);
int query_range(SegmentTree* st, int node, int start, int end, int l, int r);

void preorder_traversal(SegmentTree* st, int node, int start, int end);
void inorder_traversal(SegmentTree* st, int node, int start, int end);
void postorder_traversal(SegmentTree* st, int node, int start, int end);
void segment_tree_demo(void);

// For Stack
typedef struct stack
{
    Node* top;
} stack;

stack* createStack(void);
int push(stack* s, int value);
int pop(stack* s);
int isEmpty(const stack* s);
void destroyStack(stack* s);
int peek(const stack* s);
void printStack(const stack* s);
void printStackAsInts(const stack* s);

#endif
