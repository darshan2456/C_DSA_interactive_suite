#include<stdio.h>
#include<stdlib.h>
#include "dll.h"

//insert functions returns 1 on succesful insertion and -1 on malloc failure 
//delete functions return 1 on successful deletion and -1 on failure due to empty list
//methods implemented are - insertAtBeginning, insertAtEnd, printlist, search, deleteAtBeginning, deleteAtEnd and deleteByValue


void dll_Demo(void){
    doubly_ll_Node* head=NULL;
    int total_nodes;
    printf("how many value you want to insert? \ngive number of nodes : ");
    scanf("%d",&total_nodes);
    
    //insertion of nodes in dll
    while(total_nodes>0){
        int indicator,value;
        printf("\ntype '0' if you want to insert from end and '1' if you want to insert from beginning - ");
        scanf("%d",&indicator);
        if(indicator==0){
            printf("enter the value to be inserted at end - ");
            scanf("%d",&value);
            dll_insertAtEnd(&head,value);
            dll_printlist(head);
        }
        else if(indicator==1){
            printf("enter the value to be inserted at beginning - ");
            scanf("%d",&value);
            dll_insertAtBeginning(&head,value);
            dll_printlist(head);
        }
        else{
            printf("select only one option from 0 or 1. this chance is lost as punishment lol");
        }
        total_nodes--;
    }


    //searching elements in the dll
    while(1){
        int search_value,choice;
        printf("\nenter the number you want to search :- ");
        scanf("%d",&search_value);
        int index=dll_search(head,search_value);
        printf("entered number found at index %d",index);
        printf("\nenter '-1' if you want to exit and any number to again search : ");
        scanf("%d",&choice);
        if(choice==-1) break;
    }
    

    //deleting values from dll
    while(1){
        int del_val,choice;
        printf("enter element to be deleted :- ");
        scanf("%d",&del_val);
        dll_deleteByValue(&head,del_val);
        printf("\ndll after deletion - ");
        dll_printlist(head);
        printf("\nenter '-1' if you want to exit and any number to again delete :- ");
        scanf("%d",&choice);
        if(choice==-1){
            break;
        }
    }
    
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

void dll_printlist(const doubly_ll_Node* head){
    printf("\nHEAD<-> ");
    while(head!=NULL){
        printf("%d <->",head->data);
        head=head->next;
    }
    printf("NULL");
}

int dll_search(const doubly_ll_Node* head,int key){
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
    printf("\nNode not found!!!");
    return -1;
}

