#ifndef DLL_H
#define DLL_H

typedef struct doubly_ll_Node{
    int data;
    struct doubly_ll_Node* prev;
    struct doubly_ll_Node* next;
}doubly_ll_Node;

int dll_insertAtBeginning(doubly_ll_Node** head_ref,int value);
int dll_insertAtEnd(doubly_ll_Node** head_ref,int value);
void dll_printlist(const doubly_ll_Node* head);
int dll_search(const doubly_ll_Node* head,int key);
int dll_deleteAtBeginning(doubly_ll_Node** head_ref);
int dll_deleteAtEnd(doubly_ll_Node** head_ref);
int dll_deleteByValue(doubly_ll_Node** head_ref, int key);
void dll_Demo(void);

#endif