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
// Swap adjacent pairs in an integer array.
void swap_adjacent_pairs(int arr[], int length_of_array);

// Find the maximum element in an integer array. Returns The maximum value in the array.
int max_array(const int arr[], int length_of_array);

// Find the minimum element in an integer array. Returns The minimum value in the array.
int min_array(const int arr[], int length_of_array);

// Print the contents of an integer array.
void print_array(const int arr[], int length_of_array);

// Reverse the contents of an integer array in place.
void reverse_array(int arr[], int length_of_array);

// Run the array demonstration module.
void array_demo(void);

// For double linked list
typedef struct doubly_ll_Node
{
    int data;
    struct doubly_ll_Node* prev;
    struct doubly_ll_Node* next;
} doubly_ll_Node;

// Insert a value at the beginning of a doubly linked list. Returns 1 on success, -1 on allocation
// failure.
int dll_insertAtBeginning(doubly_ll_Node** head_ref, int value);

// Insert a value at the end of a doubly linked list. Returns 1 on success, -1 on allocation
// failure.
int dll_insertAtEnd(doubly_ll_Node** head_ref, int value);

// Print the elements of a doubly linked list.
void dll_printlist(const doubly_ll_Node* head);

// Search for a key in a doubly linked list. Returns The index of the key or -1 if not found.
int dll_search(const doubly_ll_Node* head, int key);

// Delete the first element from a doubly linked list. Returns 1 on success, -1 if the list is
// empty.
int dll_deleteAtBeginning(doubly_ll_Node** head_ref);

// Delete the last element from a doubly linked list. Returns 1 on success, -1 if the list is empty.
int dll_deleteAtEnd(doubly_ll_Node** head_ref);

// Delete the first occurrence of a key from a doubly linked list. Returns 1 on success, -1 if not
// found.
int dll_deleteByValue(doubly_ll_Node** head_ref, int key);

// Free all nodes in a doubly linked list.
void delete_dll(doubly_ll_Node* head);

// Run the doubly linked list demonstration module.
void dll_demo(void);

// Reverse a doubly linked list in place. Returns 1 on success, -1 on failure.
int dll_reverselist(doubly_ll_Node** head_ref);

// Get the number of nodes in a doubly linked list. Returns The number of nodes in the list.
int dll_getLength(const doubly_ll_Node* head);

// Insert a value at a specific position in a doubly linked list. Returns 1 on success, -1 on
// allocation failure, -2 on invalid position.
int dll_insertAtPosition(doubly_ll_Node** head_ref, int value, int position);

// Delete a node at a specific position in a doubly linked list. Returns 1 on success, -1 if the
// list is empty, -2 on invalid position.
int dll_deleteAtPosition(doubly_ll_Node** head_ref, int position);

// For single linked list
typedef struct Node
{
    int data;
    struct Node* next;
} Node;

// Print the elements of a singly linked list.
void sll_printlist(const Node* head);

// Insert a value at the end of a singly linked list. Returns 1 on success, -1 on allocation
// failure.
int sll_insertAtEnd(Node** head_ref, int value);

// Delete the first element from a singly linked list. Returns 1 on success, -1 if the list is
// empty.
int sll_deleteAtBeginning(Node** head_ref);

// Delete the last element from a singly linked list. Returns 1 on success, -1 if the list is empty.
int sll_deleteAtEnd(Node** head_ref);

// Delete the first occurrence of a value from a singly linked list. Returns 1 on success, -2 if the
// list is empty, -1 if not found.
int sll_deleteByValue(Node** head_ref, int value);

// Insert a value at the beginning of a singly linked list. Returns 1 on success, -1 on allocation
// failure.
int sll_insertAtBeginning(Node** head_ref, int value);

// Run the singly linked list demonstration module.
void sll_demo(void);

// Search for a key in a singly linked list. Returns The index of the key or -1 if not found.
int sll_search(const Node* head, int key);

// Reverse a singly linked list in place. Returns 1 on success, -2 if empty, -1 if only one node
// exists.
int sll_reverseList(Node** head_ref);

// Free all nodes in a singly linked list.
void delete_sll(Node* head);

// Get the number of nodes in a singly linked list. Returns The number of nodes in the list.
int sll_getLength(const Node* head);

// Insert a value at a specific position in a singly linked list. Returns 1 on success, -1 on
// allocation failure, -2 on invalid position.
int sll_insertAtPosition(Node** head_ref, int value, int position);

// Delete a node at a specific position in a singly linked list. Returns 1 on success, -1 if the
// list is empty, -2 on invalid position.
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

// Allocate and initialize a priority queue. Returns Pointer to the created priority queue, or NULL
// on failure.
priority_queue* pq_init(HeapType heapType);

// Insert a value into a priority queue. Returns 1 on success, -1 on failure.
int insert(priority_queue* pq, int val);

// Remove the top value from a priority queue. Returns true if a value was extracted, false if the
// queue is empty.
bool extractTop(priority_queue* pq, int* result);

// Peek at the top value of a priority queue without removing it. Returns true if the queue is not
// empty, false otherwise.
bool peek_pq(priority_queue* pq, int* result);

// Free resources used by a priority queue.
void destroy_pq(priority_queue* pq);

// Display the contents of a priority queue.
void display_heap(priority_queue* pq);

// Run the priority queue demonstration module.
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

// Initialize a singly circular linked list.
void scll_init(scll* list);

// Insert a value at the beginning of a singly circular linked list. Returns 1 on success, -1 on
// failure.
int scll_insertAtBeginning(scll* list, int value);

// Insert a value at the end of a singly circular linked list. Returns 1 on success, -1 on failure.
int scll_insertAtEnd(scll* list, int value);

// Insert a value at a specific position in a singly circular linked list. Returns 1 on success, -1
// on failure, -2 on invalid position.
int scll_insertAtPosition(scll* list, int value, int position);

// Delete the first element from a singly circular linked list. Returns 1 on success, -1 if the list
// is empty.
int scll_deleteAtBeginning(scll* list);

// Delete the last element from a singly circular linked list. Returns 1 on success, -1 if the list
// is empty.
int scll_deleteAtEnd(scll* list);

// Delete the first occurrence of a value from a singly circular linked list. Returns 1 on success,
// -1 on failure.
int scll_deleteByValue(scll* list, int value);

// Delete a node at a specific position in a singly circular linked list. Returns 1 on success, -1
// if the list is empty, -2 on invalid position.
int scll_deleteAtPosition(scll* list, int position);

// Search for a value in a singly circular linked list. Returns The index of the value or -1 if not
// found.
int scll_search(const scll* list, int key);

// Get the number of nodes in a singly circular linked list. Returns The number of nodes in the
// list.
int scll_getLength(const scll* list);

// Print the contents of a singly circular linked list.
void scll_printlist(const scll* list);

// Free resources used by a singly circular linked list.
void scll_destroy(scll* list);

// Run the singly circular linked list demonstration module.
void scll_demo(void);

// Universal non-speacial queue structure
typedef struct Queue
{
    int rear;
    int front;
    int N;
    void** arr;
} Queue;

// Initialize a circular queue. Returns 1 on success, -1 on failure.
int init_circ_queue(int N, Queue* queue_ptr);

// Destroy a circular queue and free its resources.
void destroy_circ_queue(Queue* queue_ptr);

// Enqueue a value into a circular queue. Returns 1 on success, -1 if the queue is full.
int enqueue(Queue* queue_ptr, void* value);

// Dequeue a value from a circular queue. Returns Pointer to the dequeued value, or NULL if empty.
void* dequeue(Queue* queue_ptr);

// Display the contents of a circular queue.
void display_circ_queue(Queue* queue_ptr);

// Run the circular queue demonstration module.
void circular_queue_demo(void);

// Initialize a simple linear queue. Returns 1 on success, -1 on failure.
int init_simple_queue(int N, Queue* queue_ptr);

// Destroy a simple linear queue and free its resources.
void destroy_simple_queue(Queue* queue_ptr);

// Enqueue a value into a simple linear queue. Returns 1 on success, -1 if the queue is full.
int enqueue_simple(Queue* queue_ptr, void* value);

// Dequeue a value from a simple linear queue. Returns Pointer to the dequeued value, or NULL if
// empty.
void* dequeue_simple(Queue* queue_ptr);

// Display the contents of a simple linear queue.
void display_simple_queue(const Queue* queue_ptr);

// Run the simple queue demonstration module.
void simple_queue_demo(void);

// Initialize a double-ended queue. Returns 1 on success, -1 on failure.
int init_deque(int N, Queue* dq);

// Destroy a deque and free its resources.
void destroy_deque(Queue* dq);

// Insert a value at the front of a deque. Returns 1 on success, -1 if the deque is full.
int deque_insert_front(Queue* dq, void* value);

// Insert a value at the rear of a deque. Returns 1 on success, -1 if the deque is full.
int deque_insert_rear(Queue* dq, void* value);

// Delete and return the front value from a deque. Returns Pointer to the removed value, or NULL if
// empty.
void* deque_delete_front(Queue* dq);

// Delete and return the rear value from a deque. Returns Pointer to the removed value, or NULL if
// empty.
void* deque_delete_rear(Queue* dq);

// Get the front value from a deque without removing it. Returns The front value.
int deque_get_front(const Queue* dq);

// Get the rear value from a deque without removing it. Returns The rear value.
int deque_get_rear(const Queue* dq);

// Check whether a deque is empty. Returns true if empty, false otherwise.
bool deque_is_empty(const Queue* dq);

// Check whether a deque is full. Returns true if full, false otherwise.
bool deque_is_full(const Queue* dq);

// Display the contents of a deque.
void display_deque(const Queue* dq);

// Run the deque demonstration module.
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

// Initialize a doubly circular linked list.
void dcll_init(dcll* list);

// Insert a value at the beginning of a doubly circular linked list. Returns 1 on success, -1 on
// failure.
int dcll_insertAtBeginning(dcll* list, int value);

// Insert a value at the end of a doubly circular linked list. Returns 1 on success, -1 on failure.
int dcll_insertAtEnd(dcll* list, int value);

// Insert a value at a specific position in a doubly circular linked list. Returns 1 on success, -1
// on failure, -2 on invalid position.
int dcll_insertAtPosition(dcll* list, int value, int position);

// Delete the first element from a doubly circular linked list. Returns 1 on success, -1 if the list
// is empty.
int dcll_deleteAtBeginning(dcll* list);

// Delete the last element from a doubly circular linked list. Returns 1 on success, -1 if the list
// is empty.
int dcll_deleteAtEnd(dcll* list);

// Delete the first occurrence of a value from a doubly circular linked list. Returns 1 on success,
// -1 if not found.
int dcll_deleteByValue(dcll* list, int value);

// Delete a node at a specific position in a doubly circular linked list. Returns 1 on success, -1
// if the list is empty, -2 on invalid position.
int dcll_deleteAtPosition(dcll* list, int position);

// Search for a value in a doubly circular linked list. Returns The index of the value or -1 if not
// found.
int dcll_search(const dcll* list, int key);

// Get the number of nodes in a doubly circular linked list. Returns The number of nodes in the
// list.
int dcll_getLength(const dcll* list);

// Print the contents of a doubly circular linked list.
void dcll_printlist(const dcll* list);

// Free resources used by a doubly circular linked list.
void dcll_destroy(dcll* list);

// Run the doubly circular linked list demonstration module.
void dcll_demo(void);

// For Stack
typedef struct stack
{
    Node* top;
} stack;

// Create a new stack. Returns Pointer to the created stack, or NULL on failure.
stack* createStack(void);

// Push a value onto the stack. Returns 1 on success, -1 if the stack is invalid.
int push(stack* s, int value);

// Pop a value from the stack. Returns The popped value.
int pop(stack* s);

// Check if the stack is empty. Returns true if empty, false otherwise.
bool isEmpty(const stack* s);

// Destroy a stack and free its resources.
void destroyStack(stack* s);

// Peek at the top value of the stack without popping it. Returns The top value.
int peek(const stack* s);

// Print the stack contents using characters.
void printStack(const stack* s);

// Print the stack contents using integer values.
void printStackAsInts(const stack* s);

#endif
