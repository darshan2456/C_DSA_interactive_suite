#ifndef SCLL_H
#define SCLL_H

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

#endif
