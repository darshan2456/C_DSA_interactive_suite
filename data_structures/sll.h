#ifndef SLL_H
#define SLL_H

typedef struct Node{
    int data;
    struct Node* next;
} Node;

void sll_printlist(const Node* head);
void sll_insertAtEnd(Node **head_ref,int value);
void sll_deleteAtBeginning(Node** head_ref);
void sll_deleteAtEnd(Node** head_ref);
void sll_deleteByValue(Node** head_ref,int value);
int sll_insertAtBeginning(Node** head_ref,int value);
void sll_Demo(void);
int sll_search(const Node* head,int key);
void sll_reverseList(Node** head_ref);

#endif