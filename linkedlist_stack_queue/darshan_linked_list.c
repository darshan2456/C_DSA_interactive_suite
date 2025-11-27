#include<stdio.h>
#include<stdlib.h>
typedef struct Node {
    int data;
    struct Node* next;
} Node;

void insertAtEnd(Node **head_ref,int value){
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

void deleteAtBeginning(Node** head_ref){
    if(*head_ref==NULL){
        printf("cannot perform operation as list is empty");
        return;
    }    
    Node* temp=*head_ref;
    temp=temp->next;
    free(*head_ref);
    *head_ref=temp;
}


void insertAtBeginning(Node** head_ref,int value){
    Node* newnode=malloc(sizeof(Node));
    newnode->data=value;
    newnode->next=*head_ref;
    *head_ref=newnode;
}

void deleteAtEnd(Node** head_ref){
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

void printlist(Node* head){
    printf("\n");
    while(head!=NULL){
        printf("%d ->",head->data);
        head=head->next;
    }
    printf("NULL");
}

int search(Node* head,int key){
    int index=0;
    while(head!=NULL){
        if(head->data==key){
            return index;
        }
        head=head->next;
        index++;
    }return -1;
}

int main(){
    Node* head=NULL;
    insertAtEnd(&head,46);
    insertAtBeginning(&head,87);
    printlist(head);
    return 0;
}