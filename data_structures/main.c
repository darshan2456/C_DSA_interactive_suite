#include<stdio.h>
#include "stack.h"
#include "sll.h"
#include "dll.h"
#include "circular_queue.h"
#include "array.h"
#include "bst.h"
#include "expression.h"
#include "sorting_algorithms_n2.h"
#include "searching_algorithms.h"
#include "graph_traversals.h"
#include "hash.h"
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
        "click 3 for sorting algorithms (the n^2 family) demo\n"
        "click 4 for searching algorithms demo\n"
        "click 5 for graph traversals (bfs/dfs) demo\n"
        "click 6 for hashing algorithms demo\n"
        "enter choice : ",
        1,6                                                             //limits
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
                sorting_algorithms_n2_demo();
                break;
            case 4:
                searching_algorithms_demo();
                break;
            case 5:
                graph_traversals_demo();
                break;
            case 6:
                hashing_algorithms_demo();
                break;
        }
    }


    return 0;
}