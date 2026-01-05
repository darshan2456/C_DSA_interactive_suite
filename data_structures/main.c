#include<stdio.h>
#include "stack.h"
#include "sll.h"
#include "dll.h"
#include "circular_queue.h"
#include "array.h"
#include "bst.h"
#include "expression.h"
#include "algorithms.h"
#include "safe_input.h"

void data_structures_demo(void);

//only give input as integer value as prompted through the console by programmer. dont attempt to put any other type of value
//neglecting this warning can result in undefined behaviour

int main(){

    while(1){
        int choice;
        int status=safe_input_int(
        &choice,                                                            //variable in which value is to be inserted
        "\nWelcome to DSA library built by Darshan Mukul Parekh"
        "\n(at any point enter '-1' to exit that particular demo)\n\n"
        "click 1 for data structures demo\n"
        "click 2 for expression evaluation (infix to postfix and postfix evaluation) demo\n"
        "click 3 for sorting algorithms demo\n"
        "enter choice : ",
        1,3                                                             //limits
        );
        
        if(status==-111){
            break;
        }

        if(status==0){
            continue;
        }

        switch(choice){
            case 1:
                data_structures_demo();
                break;
            case 2:
                expression_evaluation_demo();
                break;
            case 3:
                sorting_algorithms_demo();
                break;
            }

    }


    return 0;
}