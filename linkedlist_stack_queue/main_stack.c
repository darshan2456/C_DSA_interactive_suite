#include<stdio.h>
#include<stdlib.h>
#include "linkedlist.h"

int main(){
    Node* head=NULL;
    insertAtEnd(&head,46);
    insertAtEnd(&head,87);
    insertAtEnd(&head,65);
    printstack(head);
    deleteAtEnd(&head);
    printstack(head);
    
    return 0;
}