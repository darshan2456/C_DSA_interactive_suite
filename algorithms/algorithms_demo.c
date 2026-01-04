#include "algorithms.h"
#include<stdio.h>

void sorting_algorithm_demo(void){

    while(1){
        int repeat,choice;
        printf(
            "\nenter 1 for bubble sort"
            "\nenter 2 for insertion sort"
            "\nenter 3 for selection sort"
            "\nenter choice : "
        );
        scanf("%d",&choice);

        if(choice<1 || choice>3){
            printf("\nenter only between '1','2' and '3'");
            continue;
        }

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

        printf("\nif you want to test another sorting algorithm enter any number, for exit enter '-1' :- ");
        scanf("%d",&repeat);

        if(repeat==-1)  break;
    }
    
}