#include<stdio.h>
#include<stdlib.h>
#include "dll.h"

//insert functions returns 1 on succesful insertion and -1 on malloc failure 
//delete functions return 1 on successful deletion and -1 on failure due to empty list
//methods implemented are - insertAtBeginning, insertAtEnd, printlist, search, deleteAtBeginning, deleteAtEnd and deleteByValue

void dll_Demo(void){
    doubly_ll_Node* head=NULL;
    int value,value1,value2;
    printf("enter number to be inserted at beginning - ");
    scanf("%d",&value);
    dll_insertAtBeginning(&head,value);
    printf("enter number to be inserted at beginning - ");
    scanf("%d",&value1);
    dll_insertAtBeginning(&head,value1);
    printf("\nenter number to be inserted at beginning - ");
    scanf("%d",&value2);
    dll_insertAtBeginning(&head,value2);
    printf("the current list is :-\n");
    dll_printlist(head);
    int search_value;
    printf("\nenter the number whose index you want to search :- ");
    scanf("%d",&search_value);
    int index=dll_search(head,search_value);
    printf("\nentered number found at index %d",index,"\n");
    dll_printlist(head);
}

int dll_insertAtBeginning(doubly_ll_Node** head_ref,int value){
    doubly_ll_Node* newnode=malloc(sizeof(doubly_ll_Node));
    if(newnode==NULL) return -1;
    newnode->data=value;
    if(*head_ref==NULL){
        newnode->next=NULL;
        newnode->prev=NULL;
        *head_ref=newnode;
        return 1;
    }
    newnode->next=*head_ref;
    newnode->prev=NULL;
    (*head_ref)->prev=newnode;
    *head_ref=newnode;
    return 1;
}

int dll_insertAtEnd(doubly_ll_Node** head_ref,int value){
    doubly_ll_Node* newnode=malloc(sizeof(doubly_ll_Node));
    if(newnode==NULL) return -1;
    newnode->data=value;
    if(*head_ref==NULL){
        newnode->next=NULL;
        newnode->prev=NULL;
        *head_ref=newnode;
        return 1;
    }
    doubly_ll_Node* temp=*head_ref;
    while(temp->next!=NULL){
        temp=temp->next;
    }
    newnode->prev=temp;
    newnode->next=NULL;
    temp->next=newnode;
    return 1;
}

void dll_printlist(doubly_ll_Node* head){
    printf("\n");
    while(head!=NULL){
        printf("%d <->",head->data);
        head=head->next;
    }
    printf("NULL");
}

int dll_search(doubly_ll_Node* head,int key){
    int index=0;
    while(head!=NULL){
        if(head->data==key){
            return index;           //if value found returns index number 
        }
        head=head->next;
        index++;
    }return -1;                     //otherwise returns -1
}

int dll_deleteAtBeginning(doubly_ll_Node** head_ref){
    if(*head_ref==NULL) return -1;
    if((*head_ref)->next==NULL){
        free(*head_ref);
        *head_ref=NULL;
        return 1;
    }
    doubly_ll_Node* secondnode=(*head_ref)->next;
    free(*head_ref);
    secondnode->prev=NULL;
    *head_ref=secondnode;
    return 1;
}

int dll_deleteAtEnd(doubly_ll_Node** head_ref){
    if(*head_ref==NULL) return -1;
    if((*head_ref)->next==NULL){
        free(*head_ref);
        *head_ref=NULL;
        return 1;
    }
    doubly_ll_Node* temp=*head_ref;
    while(temp->next!=NULL){
        temp=temp->next;
    }
    doubly_ll_Node* secondlast=temp->prev;
    free(temp);
    secondlast->next=NULL;
    return 1;
}

int dll_deleteByValue(doubly_ll_Node** head_ref, int key){
    if(*head_ref==NULL) return -1;
    if((*head_ref)->next==NULL && (*head_ref)->data==key){
        free(*head_ref);
        *head_ref=NULL;
        return 1;
    }
    doubly_ll_Node* temp=*head_ref;
    while(temp!=NULL){
        if(temp->data==key){
            doubly_ll_Node* beforekey=temp->prev;
            doubly_ll_Node* afterkey=temp->next;
            if(beforekey==NULL){
                *head_ref=temp->next;
                (*head_ref)->prev=NULL;
                free(temp);
                return 1;
            }
            beforekey->next=afterkey;
            if(afterkey!=NULL){
                afterkey->prev=beforekey;
            }
            free(temp);
            return 1;
        }
        temp=temp->next;
    }
    return -1;
}

