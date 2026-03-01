#include<stdio.h>
#include "searching_algorithms.h"
#include "safe_input.h"

void searching_algorithms_demo(void){
    int searching_algo_status,searching_algo_choice;
    while(1){
        searching_algo_status=safe_input_int(&searching_algo_choice,
        "\nenter 1 for linear search demo"
        "\nenter 2 for binary search"
        "\nenter choice : ",
        1,2);

        if(searching_algo_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting searching_algorithms_demo....\n");
            return;
        }

        if(searching_algo_status==0)            continue;

        switch(searching_algo_choice){
            case 1:
                linear_search_demo();
                break;
            case 2:
                binary_search_demo();
                break;
        }
    }
}