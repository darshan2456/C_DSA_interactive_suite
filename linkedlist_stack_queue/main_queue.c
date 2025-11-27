#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

int main(){
    Node* head=NULL;
    insertAtEnd(&head,46);
    insertAtEnd(&head,87);
    insertAtEnd(&head,65);
    printqueue(head);
    deleteAtBeginning(&head);
    printqueue(head);
    
    return 0;
}