#include<stdio.h>
#include "stack.h"
#include "sll.h"
#include "dll.h"
#include "bst.h"
#include "expression.h"
#include "circular_queue.h"

//only give input as integer value as prompted through the console by programmer. dont attempt to put any other type of value
//neglecting this warning can result in undefined behaviour

int main(){

    while(1){
        int choice;
    start:  printf(
        "\nwelcome to DSA library built by Darshan Mukul Parekh\n"
        "click 1 for linked-list\n"
        "click 2 for circular queue\n"
        "click 3 for doubly linked-list\n"
        "click 4 for binary search tree\n"
        "click 5 for infix to postfix conversion\n"
        "click 6 for postfix evaluation\n\n"
        "enter choice : "
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

        printf("\n\nEnter any number to test another Data Structure / Algorithm and '-1' to exit :- ");
        scanf("%d",&choice);
        
        
        if(choice==-1) break;
    }


    return 0;
}