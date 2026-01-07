#include<stdio.h>
#include "algorithms.h"
#include "safe_input.h"


void sorting_algorithms_demo(void){
    int status,choice;
    while(1){
        status=safe_input_int(
            &choice,
            "\nenter 1 for bubble sort"
            "\nenter 2 for insertion sort"
            "\nenter 3 for selection sort"
            "\nenter choice : ",
            1,3
        );

        if(status==-111){
            printf("\nExiting sorting_algorithms_demo.....\n");
            return;
        };

        if(status==0)   continue;

        switch(choice){
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