#ifndef SLL_H
#define SLL_H

#include <stdint.h>

typedef struct Node
{
    void* data;
    struct Node* next;
} Node;

// Print the elements of a singly linked list.
void sll_printlist(const Node* head, void (*print_element)(const void*));

// Insert a value at the end of a singly linked list. Returns 1 on success, -1 on allocation
// failure.
int sll_insertAtEnd(Node** head_ref, void* value);

// Delete the first element from a singly linked list. Returns 1 on success, -1 if the list is
// empty.
int sll_deleteAtBeginning(Node** head_ref, void (*free_data)(void*));

// Delete the last element from a singly linked list. Returns 1 on success, -1 if the list is empty.
int sll_deleteAtEnd(Node** head_ref, void (*free_data)(void*));

// Delete the first occurrence of a value from a singly linked list. Returns 1 on success, -2 if the
// list is empty, -1 if not found.
int sll_deleteByValue(Node** head_ref, const void* value, int (*compare)(const void*, const void*),
                      void (*free_data)(void*));

// Insert a value at the beginning of a singly linked list. Returns 1 on success, -1 on allocation
// failure.
int sll_insertAtBeginning(Node** head_ref, void* value);

// Run the singly linked list demonstration module.
void sll_demo(void);

// Search for a key in a singly linked list. Returns The index of the key or -1 if not found.
int sll_search(const Node* head, const void* key, int (*compare)(const void*, const void*));

// Reverse a singly linked list in place. Returns 1 on success, -2 if empty, -1 if only one node
// exists.
int sll_reverseList(Node** head_ref);

// Free all nodes in a singly linked list.
void delete_sll(Node* head, void (*free_data)(void*));

// Get the number of nodes in a singly linked list. Returns The number of nodes in the list.
int sll_getLength(const Node* head);

// Insert a value at a specific position in a singly linked list. Returns 1 on success, -1 on
// allocation failure, -2 on invalid position.
int sll_insertAtPosition(Node** head_ref, void* value, int position);

// Delete a node at a specific position in a singly linked list. Returns 1 on success, -1 if the
// list is empty, -2 on invalid position.
int sll_deleteAtPosition(Node** head_ref, int position, void (*free_data)(void*));

#endif
