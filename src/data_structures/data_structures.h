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
void sll_Demo(void);
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
void scll_Demo(void);

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
void circular_queue_Demo(void);

// For simple (linear) queue
int init_simple_queue(int N, Queue* queue_ptr);
void destroy_simple_queue(Queue* queue_ptr);
int enqueue_simple(Queue* queue_ptr, void* value);
void* dequeue_simple(Queue* queue_ptr);
void display_simple_queue(const Queue* queue_ptr);
void simple_queue_Demo(void);

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

#endif
