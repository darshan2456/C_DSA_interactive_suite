#include<stdio.h>
#include "stack.h"
#include "sll.h"
#include "dll.h"
#include "bst.h"
#include "expression.h"
#include "circular_queue.h"

int main(){
    int choice;
start:  printf(
        "welcome to DSA library built by Darshan Mukul Parekh\n"
        "click 1 for linked-list\n"
        "click 2 for circular queue\n"
        "click 3 for doubly linked-list\n"
        "click 4 for binary search tree\n"
        "click 5 for infix to postfix conversion\n"
        "click 6 for postfix evaluation\n"
        );
    scanf("%d",&choice);
    if(choice<1 || choice >6){
        printf("invalid choice try again\n");
        goto start;
    }

    switch(choice){
        case 1:
            sll_Demo();
            break;
        case 2:
            circular_queue_Demo();
            break;
        case 3:
            dll_Demo();
            break;
        case 4:
            binary_search_tree_Demo();
            break;
        case 5:
            infix_to_postfix_Demo();
            break;
        case 6:
            postfix_evaluation_Demo();
            break;
    }

}