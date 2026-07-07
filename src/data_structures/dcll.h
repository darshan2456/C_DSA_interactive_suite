#ifndef DCLL_H
#define DCLL_H

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

#endif
