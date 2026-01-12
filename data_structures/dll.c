#include<stdio.h>
#include<stdlib.h>
#include "dll.h"
#include "safe_input.h"

//insert functions returns 1 on succesful insertion and -1 on malloc failure 
//delete functions return 1 on successful deletion and -1 on failure due to empty list
//methods implemented are - insertAtBeginning, insertAtEnd, printlist, search, deleteAtBeginning, deleteAtEnd and deleteByValue


void dll_demo(void){
            doubly_ll_Node* head=NULL;
            int dll_element_count;
            int dll_length_status;
start_dll:  dll_length_status=safe_input_int(&dll_element_count,
            "\nenter the number of elements you want to insert, (between 1 and 100), enter '-1' to exit: ",
            1,100);
            
            if(dll_length_status==INPUT_EXIT_SIGNAL){
                printf("\nExiting dll demo\n");
                return;
            }
            if(dll_length_status==0){
                goto start_dll;
            }
    
    //insertion of nodes in dll
    while(dll_element_count>0){
        int dll_position_choice;
        int dll_position_status;

dll_position_selection:
        dll_position_status=safe_input_int(&dll_position_choice,
        "\nenter '1' for inserting at end and '0' for inserting at beginning, enter '-1' to exit :- ",
        0,1);

        if(dll_position_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting dll demo\n");
            return;
        }
        if(dll_position_status==0){
            goto dll_position_selection;
        }
        
        if(dll_position_choice==0){                 //enter element at end
            int dll_end_status;
            int dll_end_value;

dll_enter_end_value:
            dll_end_status=safe_input_int(&dll_end_value,
            "enter the value to be inserted at end, (between 1 and 100), enter '-1' to exit - ",
            1,100);

            if(dll_end_status==INPUT_EXIT_SIGNAL){
                printf("\nExiting dll demo\n");
                return;
            }
            if(dll_end_status==0){
                goto dll_enter_end_value;
            }
            dll_insertAtEnd(&head,dll_end_value);
            dll_printlist(head);
        }
        else if(dll_position_choice==1){                    //enter element at start
            int dll_start_status;
            int dll_start_value;

dll_enter_start_value:
            dll_start_status=safe_input_int(&dll_start_value,
            "enter the value to be inserted at beginning, (between 1 and 100), enter '-1' to exit: ",
            1,100);

            if(dll_start_status==INPUT_EXIT_SIGNAL){
                printf("\nExiting dll demo\n");
                return;
            }
            if(dll_start_status==0){
                goto dll_enter_start_value;
            }
            dll_insertAtBeginning(&head,dll_start_value);
            dll_printlist(head);
        }
        dll_element_count--;

    }


    //searching elements in the dll
    while(1){
        int dll_search_status;
        int dll_search_value;

        dll_search_status=safe_input_int(&dll_search_value,
        "\nenter the number you want to search, (between 1 and 100), enter '-1' to exit:- ",
        1,100);

        if(dll_search_status==INPUT_EXIT_SIGNAL){
            break;
        }

        if(dll_search_status==0){
            continue;
        }
        
        int index=dll_search(head,dll_search_value);
        printf("\nentered number found at index %d",index);
        
    }
    

    //deleting values from dll
    while(1){
        int dll_delete_status;
        int dll_delete_value;

        dll_delete_status=safe_input_int(&dll_delete_value,
        "\nenter element to be deleted, (between 1 and 100), enter '-1' to exit :- ",
        1,100);

        if(dll_delete_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting dll demo\n");
            return;
        }
        if(dll_delete_status==0){
            continue;
        }
    
        dll_deleteByValue(&head,dll_delete_value);
        printf("\ndll after deletion - ");
        dll_printlist(head);
        
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