#include<stdio.h>
#include "stack.h"
#include "sll.h"
#include "dll.h"
#include "circular_queue.h"
#include "array.h"
#include "bst.h"
#include "expression.h"
#include "algorithms.h"

//only give input as integer value as prompted through the console by programmer. dont attempt to put any other type of value
//neglecting this warning can result in undefined behaviour

int main(){

    while(1){
        int choice;
    start:  printf(
        "\nwelcome to DSA library built by Darshan Mukul Parekh\n"
        "click 1 for array\n"
        "click 2 for linked-list\n"
        "click 3 for circular queue\n"
        "click 4 for doubly linked-list\n"
        "click 5 for binary search tree\n"
        "click 6 for infix to postfix conversion\n"
        "click 7 for postfix evaluation\n"
        "click 8 for sorting algorithms demo\n"
        "enter choice : "
        );
        scanf("%d",&choice);
        if(choice<1 || choice >10){
            printf("invalid choice try again\n");
            goto start;
        }

        switch(choice){
            case 1:
                array_demo();
                break;
            case 2:
                sll_Demo();
                break;
            case 3:
                circular_queue_Demo();
                break;
            case 4:
                dll_Demo();
                break;
            case 5:
                binary_search_tree_Demo();
                break;
            case 6:
                infix_to_postfix_Demo();
                break;
            case 7:
                postfix_evaluation_Demo();
                break;
            case 8:
                sorting_algorithm_demo();
                break;
        }

        printf("\n\nEnter any number to test another Data Structure / Algorithm and '-1' to exit :- ");
        scanf("%d",&choice);
        
        
        if(choice==-1) break;
    }


    return 0;
}