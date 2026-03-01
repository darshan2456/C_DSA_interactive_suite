#include<stdio.h>
#include "array.h"
#include<time.h>
#include "safe_input.h"


void bubble_sort_optimized(int arr[], int length_of_array);

void bubble_sort_optimized_demo(void){
    int length_of_array;
    while(1){
        printf("\n\nBubble sort (optimized) demo");
        int bubble_sort_status=safe_input_int(&length_of_array,
        "\nenter no of elements of array you want to sort (between 1 and 100), enter '-1' to exit:- ",
        1,100);

        if(bubble_sort_status==0)      continue;            //retry on failure

        if(bubble_sort_status==INPUT_EXIT_SIGNAL){           //exit condition, user enters '-1'
            printf("\nExiting bubble sort demo....\n");
            return;
        }
        
        int arr[length_of_array];

        for(int i=0;i<length_of_array;i++){
        retry:  printf("\nenter element no %d, (between 1 and 100), enter '-1' to exit:- ",i);
                int element_status=safe_input_int(&arr[i],NULL,1,100);

                if(element_status==INPUT_EXIT_SIGNAL){
                    printf("\nExiting bubble sort demo\n");
                    return;
                }

                if(element_status==0){
                    goto retry;
                }
        }

        printf("\nsee bubble sort in action :- \n");
        bubble_sort_optimized(arr,length_of_array);

    }
    
}


//note: the time measured by clock() function includes the time for bubble sort computation and 
//printing the array state after each iteration. the CPU time must not be treated as a measure of efficiency of 
//the algorithm and is for demonstration only.
void  bubble_sort_optimized(int arr[], int length_of_array){

    clock_t start_t,end_t;
    double total_t;

    start_t=clock();        //time recorded at the start of the algorithm

    for(int i=0;i<length_of_array-1;i++){
        int swapped=0;

        for(int j=0;j<length_of_array-i-1;j++){                         //Swapping happens here
            if(arr[j]>arr[j+1]){                                      
                int temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
                swapped=1;
            }
        }

        if(swapped==0) break;
        printf("after iteration no %d - ",i+1);
        print_array(arr,length_of_array);
        printf("\n");
    }

    end_t=clock();          //time recorded at the end of the algorithm

    total_t=(double)(end_t-start_t)/CLOCKS_PER_SEC;

    printf("\nfinal array sorted by bubble sort is - ");
    print_array(arr,length_of_array);
    printf("\nTotal CPU time taken:- %f seconds",total_t);
}