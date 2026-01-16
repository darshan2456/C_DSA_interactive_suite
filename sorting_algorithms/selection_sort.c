#include<stdio.h>
#include "array.h"
#include "safe_input.h"
#include<time.h>

void selection_sort(int arr[],int length_of_array);

void selection_sort_demo(void){
    int length_of_array;
    while(1){
        printf("\n\nSelection sort demo");
        int selection_sort_status=safe_input_int(&length_of_array,
        "\nenter the number of elements in the array (between 1 and 100), enter '-1' to exit:- ",
        1,100);
        
        if(selection_sort_status==0)        continue;       //retry on failure

        if(selection_sort_status==INPUT_EXIT_SIGNAL){           //exit condition, user enters '-1'
            printf("\nExiting selection sort demo....\n");
            return;
        }
        int arr[length_of_array];

        for(int i=0;i<length_of_array;i++){
        
        retry:  printf("\nenter element no %d, (between 1 and 100), enter '-1' to exit:- ",i);
                int element_status=safe_input_int(&arr[i],NULL,1,100);
                if(element_status==INPUT_EXIT_SIGNAL){                          //exit condition, user entered '-1'
                    printf("\nExiting selection sort demo.....\n");
                    return;
                }
            
                if(element_status==0){
                    goto retry;
                }
        }

        
        selection_sort(arr,length_of_array);

    }

}

//note: the time measured by the clock() function is only for the selection sort computation
void selection_sort(int arr[],int length_of_array){
    clock_t start_t,end_t;
    double total_t;

    start_t=clock();
    
    for(int i=0;i<length_of_array-1;i++){
        int min_index=i;

        for(int j=i+1;j<length_of_array;j++){
            if(arr[j]<arr[min_index]){
                min_index=j;
            }
        }

        if(i!=min_index){
            int temp=arr[min_index];
            arr[min_index]=arr[i];
            arr[i]=temp;
        }
        
        // printf("after iteration no %d - ",i+1);
        // print_array(arr,length_of_array);
        // printf("\n");

        
    }

    end_t=clock();
    total_t=(double)(end_t-start_t);

    printf("\n(per iteration array state is not displayed as this function is also used in binary search algorithm)");
    printf("\n\nfinal array sorted by selection sort is:- ");
    print_array(arr,length_of_array);
    printf("\nTotal CPU time taken for selection sort:- %f seconds",total_t);
    printf("\n(most probably execution time would be lesser than clock resolution, resulting in 0.00)");
}