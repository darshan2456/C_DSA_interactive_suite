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
        printf("\ncannot perform operation as list is empty");
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

void deleteByValue(Node** head_ref,int value){
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
            printf("\nNODE NOT FOUND!!!");
            return;
        }
    }
    prev->next=curr->next;
    free(curr);
}

void reverseList(Node** head_ref){
    Node* prev=NULL;
    Node* curr=*head_ref;
    Node* upcoming=(*head_ref)->next;
    if(curr==NULL){
        printf("\n cant perform operation as list is empty.");
        return;
    }
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

int main(){
    Node* head=NULL;
    insertAtBeginning(&head,78);
    printlist(head);
    insertAtBeginning(&head,84);
    printlist(head);
    insertAtBeginning(&head,62);
    printlist(head);
    insertAtBeginning(&head,29);
    printlist(head);
    reverseList(&head);
    printlist(head);
    return 0;
}