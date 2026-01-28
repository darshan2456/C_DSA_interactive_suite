#include<stdio.h>
#include<stdlib.h>
#include "sll.h"
#include "safe_input.h"

//methods implemented are - insertAtEnd, deleteAtBeginning, insertAtBeginning, deleteAtEnd, printlist, search
// deleteByValue and reverseList



void sll_Demo(void){
            Node* head=NULL;
            int sll_element_count;
            int sll_length_status;

start_sll:  sll_length_status=safe_input_int(&sll_element_count,
            "enter how many elements you want to insert, (between 1 and 100), enter '-1' to exit :- ",
            1,100);
            
            if(sll_length_status==INPUT_EXIT_SIGNAL){
                printf("\nExiting sll demo\n");
                delete_sll(head);
                return;
            }

            if(sll_length_status==0){
                goto start_sll;
            }

            while(sll_element_count>0){
                int sll_position_choice;
                int sll_position_status;

sll_position_selection:
                sll_position_status=safe_input_int(&sll_position_choice,
                "\nenter '1' for inserting at end and '0' for inserting at beginning, enter '-1' to exit :- ",
                0,1);

                if(sll_position_status==INPUT_EXIT_SIGNAL){
                    printf("\nExiting sll demo\n");
                    delete_sll(head);
                    return;
                }

                if(sll_position_status==0){
                    goto sll_position_selection;
                }

                if(sll_position_choice==1){
                    int sll_end_status;
                    int sll_end_value;
sll_enter_end_value:sll_end_status=safe_input_int(&sll_end_value,
                    "enter the no. you want to insert at end, (between 1 and 100), enter '-1' to exit :- ",
                    1,100);

                    if(sll_end_status==INPUT_EXIT_SIGNAL){
                        printf("\nExiting sll demo\n");
                        delete_sll(head);
                        return;
                    }

                    if(sll_end_status==0){
                        goto sll_enter_end_value;
                    }

                    int status=sll_insertAtEnd(&head,sll_end_value);
                    if(status==0){
                        printf("\nmalloc allocation failure. try again\n");
                        goto sll_enter_end_value;
                    }
                    sll_printlist(head);
                }
                else if(sll_position_choice==0){
                    int sll_start_status;
                    int sll_start_value;

sll_enter_start_value:  
                    sll_start_status=safe_input_int(&sll_start_value,
                    "enter the no. you want to insert at beginning, (between 1 and 100), enter '-1' to exit :- ",
                    1,100);

                    if(sll_start_status==INPUT_EXIT_SIGNAL){
                        printf("\nExiting sll demo\n");
                        delete_sll(head);
                        return;
                    }

                    if(sll_start_status==0){
                        goto sll_enter_start_value;
                    }
                    int status=sll_insertAtBeginning(&head,sll_start_value);
                    if(status==0){
                        printf("\nmalloc allocation failure. try again\n");
                        goto sll_enter_start_value;
                    }
                    sll_printlist(head);
                }

                sll_element_count--;
            }

    printf("\n\nReverse of the given list is :-");
    sll_reverseList(&head);
    sll_printlist(head);
    sll_reverseList(&head);
    printf("\n\ncurrent list is :- ");
    sll_printlist(head);


    //searching elements in sll
    while(1){
        int sll_search_status;
        int sll_search_value;
        sll_search_status=safe_input_int(&sll_search_value,
        "\nenter the element to be searched, (between 1 and 100), enter '-1' to exit :- ",
        1,100);
        if(sll_search_status==INPUT_EXIT_SIGNAL){
            break;
        }
        if(sll_search_status==0){
            continue;
        }

        int index=sll_search(head,sll_search_value);
        printf("\nelement found at index :- %d",index);
                
    }
    

    //deleting elements in sll
    while(1){
        int sll_delete_status;
        int sll_delete_value;
        sll_delete_status=safe_input_int(&sll_delete_value,
        "\nenter the element to be deleted, (between 1 and 100), enter '-1' to exit :- ",
        1,100);
        
        if(sll_delete_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting sll demo\n");
            delete_sll(head);
            return;
        }
        if(sll_delete_status==0){
            continue;
        }

        sll_deleteByValue(&head,sll_delete_value);
        printf("\nsll after deletion - ");
        sll_printlist(head);
    
    }
}


//insert at end returns 0 on allocation failure and 1 on successful insertion

int sll_insertAtEnd(Node **head_ref,int value){
    Node* newnode=malloc(sizeof(Node));
    if(newnode==NULL)   return 0;
    newnode -> data=value;
    newnode -> next=NULL;
    if(*head_ref==NULL){
        *head_ref=newnode;
        return 1;
    }

    Node* temp=*head_ref;
    while (temp -> next != NULL){
        temp=temp->next;
    }
    temp->next=newnode;
    return 1;
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

void delete_sll(Node* head){
    while (head!=NULL){
        Node* upcoming=head->next;
        free(head);
        head=upcoming;
    }
}