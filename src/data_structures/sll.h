#ifndef SLL_H
#define SLL_H

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

#endif
