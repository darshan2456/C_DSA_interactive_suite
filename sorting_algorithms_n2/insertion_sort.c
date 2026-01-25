#include<stdio.h>
#include "array.h"
#include "safe_input.h"
#include<time.h>

void insertion_sort(int arr[], int length_of_array);

void insertion_sort_demo(void){
    int length_of_array;
    while(1){
        printf("\n\nInsertion sort demo");
        int insertion_sort_status=safe_input_int(&length_of_array,
        "\nenter the number of elements in the array (between 1 and 100), enter '-1' to exit:- ",
        1,100);

        if(insertion_sort_status==0)        continue;                   //retry on failure

        if(insertion_sort_status==INPUT_EXIT_SIGNAL){           //exit condition, user enters '-1'
            printf("\nExiting insertion sort demo....\n");
            return;
        }    

        int arr[length_of_array];

        for(int i=0;i<length_of_array;i++){

        retry:  printf("\nenter the element number %d, (between 1 and 100), enter '-1' to exit:- ",i);
                int element_status=safe_input_int(&arr[i],NULL,1,100);
            
                if(element_status==INPUT_EXIT_SIGNAL){
                    printf("\nExiting insertion sort demo.....\n");
                    return;
                }
            
                if(element_status==0){
                    goto retry;
                }
        }

        printf("\nsee insertion sort in action :-\n");
        insertion_sort(arr,length_of_array);

    }
    
}

//note: the time measured by clock() function includes the time for insertion sort computation and 
//printing the array state after each iteration. the CPU time must not be treated as a measure of efficiency of 
//the algorithm and is for demonstration only.
void insertion_sort(int arr[], int length_of_array){

    clock_t start_t,end_t;
    double total_t;

    start_t=clock();                //time recorded at the start of algorithm

    for(int i=1;i<length_of_array;i++){
        int key=arr[i];
        int j=i-1;
        while(j>=0 && arr[j]>key){
            arr[j+1]=arr[j];
            j--;
        }
        arr[j+1]=key;

        printf("after iteration no %d - ",i);
        print_array(arr,length_of_array);
        printf("\n");
    }

    end_t=clock();
    total_t=(double)(end_t-start_t)/CLOCKS_PER_SEC;

    printf("\nfinal array sorted by insertion sort - ");
    print_array(arr,length_of_array);
    printf("\nTotal CPU time taken:- %f seconds",total_t);
}