#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node{
    int data;
    struct Node* next;
} Node;

void printstack(Node* head);
void printqueue(Node* head);
void insertAtEnd(Node **head_ref,int value);
void deleteAtBeginning(Node** head_ref);
void deleteAtEnd(Node** head_ref);
#endif