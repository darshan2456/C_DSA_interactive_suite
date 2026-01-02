#include<stdio.h>
#include<stdlib.h>
#include "sll.h"

//methods implemented are - insertAtEnd, deleteAtBeginning, insertAtBeginning, deleteAtEnd, printlist, search
// deleteByValue and reverseList



void sll_Demo(void){
    Node* head=NULL;
    int element_count;
    printf("enter how many elements you want to insert :- ");
    scanf("%d",&element_count);
    while(element_count>0){
        int choice,value;
        printf("\nenter '1' for inserting at end and '0' for inserting at beginning :- ");
        scanf("%d",&choice);
        if(choice==1){
            printf("enter the element you want to insert at end :- ");
            scanf("%d",&value);
            sll_insertAtEnd(&head,value);
            sll_printlist(head);
        }
        else if(choice==0){
            printf("enter the element you want to insert at beginning :- ");
            scanf("%d",&value);
            sll_insertAtBeginning(&head,value);
            sll_printlist(head);
        }
        else{
            printf("only select one between 0 and 1. now this chance is gone lol.");
        }
        element_count--;
    }

    //searching elements in sll
    while(1){
        int value,choice;
        printf("\nenter the element to be searched :- ");
        scanf("%d",&value);
        int index=sll_search(head,value);
        printf("\nelement found at index :- %d",index);
        printf("\nenter '-1' to exit or any number to search again :- ");
        scanf("%d",&choice);
        if(choice==-1){
            break;
        }
    }
    

    //deleting elements in sll
    while(1){
        int value,choice;
        printf("enter the element to be deleted :- ");
        scanf("%d",&value);
        sll_deleteByValue(&head,value);
        printf("\nsll after deletion - ");
        sll_printlist(head);
        printf("\nenter '-1' to exit or any number to delete again :- ");
        scanf("%d",&choice);
        if(choice==-1){
            break;
        }
    }
}


void sll_insertAtEnd(Node **head_ref,int value){
    Node* newnode=malloc(sizeof(Node));
    newnode -> data=value;
    newnode -> next=NULL;
    if(*head_ref==NULL){
        *head_ref=newnode;
        return;
    }

    Node* temp=*head_ref;
    while (temp -> next != NULL){
        temp=temp->next;
    }
    temp->next=newnode;
    
}

void sll_deleteAtBeginning(Node** head_ref){
    if(*head_ref==NULL){
        printf("\ncannot perform operation as list is empty");
        return;
    }    
    Node* temp=*head_ref;
    temp=temp->next;
    free(*head_ref);
    *head_ref=temp;
}

//insertAtBeginning function returns 0 on allocation failure and 1 on succesful insertion

int sll_insertAtBeginning(Node** head_ref,int value){
    Node* newnode=malloc(sizeof(Node));
    if(newnode==NULL) return 0;
    newnode->data=value;
    newnode->next=*head_ref;
    *head_ref=newnode;
    return 1;
}

void sll_deleteAtEnd(Node** head_ref){
    if(*head_ref==NULL){
        printf("cannot perform operation as list is empty");
        return;
    }    
    Node* temp=*head_ref;
    if(temp->next==NULL){
        free(temp);
        *head_ref=NULL;
        return;
    }
    Node* curr=*head_ref;
    Node* prev=NULL;
    while (curr->next!=NULL){
        prev=curr;
        curr=curr->next;
    }
    free(curr);
    prev->next=NULL;
}

void sll_printlist(const Node* head){
    printf("\nHEAD->");
    while(head!=NULL){
        printf("%d ->",head->data);
        head=head->next;
    }
    printf("NULL");
}

int sll_search(const Node* head,int key){
    int index=0;
    while(head!=NULL){
        if(head->data==key){
            return index;               //if value found returns index number
        }
        head=head->next;
        index++;
    }return -1;                         //otherwise returns -1
}

void sll_deleteByValue(Node** head_ref,int value){
    if(*head_ref==NULL){
        printf("cannot perform operation as list is empty");
        return;
    }
    Node* curr=*head_ref;
    Node* prev=NULL;
    if(curr->data==value){
        *head_ref=curr->next;
        free(curr);
        return;
    }
    while(curr->data!=value){
        prev=curr;
        curr=curr->next;
        if(curr==NULL){
            printf("\nNode not found!!!");
            return;
        }
    }
    prev->next=curr->next;
    free(curr);
}

void sll_reverseList(Node** head_ref){
    Node* prev=NULL;
    Node* curr=*head_ref;
    if(curr==NULL){
        printf("\n cant perform operation as list is empty.");
        return;
    }
    Node* upcoming=(*head_ref)->next;
    if(upcoming==NULL){
        printf("\nsingle node list cannot be reversed.");
        return;
    }
    while(upcoming!=NULL){
        curr->next=prev;
        prev=curr;
        curr=upcoming;
        upcoming=upcoming->next;
    }
    curr->next=prev;
    *head_ref=curr;
}

