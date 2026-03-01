#include<stdio.h>
#include "sorting_algorithms_n2.h"
#include "safe_input.h"


void sorting_algorithms_n2_demo(void){
    int sorting_algo_status,sorting_algo_choice;
    while(1){
        sorting_algo_status=safe_input_int(
            &sorting_algo_choice,
            "\nenter 1 for bubble sort"
            "\nenter 2 for insertion sort"
            "\nenter 3 for selection sort"
            "\nenter choice : ",
            1,3
        );

        if(sorting_algo_status==-111){
            printf("\nExiting sorting_algorithms_demo.....\n");
            return;
        }

        if(sorting_algo_status==0)   continue;

        switch(sorting_algo_choice){
            case 1:
                bubble_sort_optimized_demo();
                break;
            case 2:
                insertion_sort_demo();
                break;
            case 3:
                selection_sort_demo();
                break;
        }
    }    
}