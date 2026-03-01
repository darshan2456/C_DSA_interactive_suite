#include<stdio.h>
#include "safe_input.h"
#include<time.h>

void linear_search(int arr[], int target, int length_of_array);

void linear_search_demo(void){

    while(1){

            clock_t start_t,end_t;
            double total_t;
            int length_of_array=0;
            int target=0;
            int linear_search_status=0;

            printf("\nLinear search demo\n");
            linear_search_status=safe_input_int(&length_of_array,
            "\nenter length of array, (between 1 and 100), enter '-1' to exit:- ",
            1,100);

            if(linear_search_status==INPUT_EXIT_SIGNAL){
                printf("\nExiting linear search demo....\n");
                return;
            }

            if(linear_search_status==0){
                continue;
            }

            int arr[length_of_array];

            for(int i=0;i<length_of_array;i++){

                int element_status;

retry_element:      printf("\nenter element no %d, (between 1 and 100), enter '-1' to exit:- ",i+1);
                    element_status=safe_input_int(&arr[i],NULL,1,100);

                if(element_status==INPUT_EXIT_SIGNAL){
                    printf("\nExiting linear search demo....\n");
                    return;
                }

                if(element_status==0){
                    printf("\nentered invalid input or characters other than numbers. Try again\n");
                    goto retry_element;
                }
            }

                int target_status;

                while(1){
                    target_status=safe_input_int(&target,
                    "\nEnter target which you want to search, (between 1 and 100), enter '-1' to exit:- ",
                    1,100);

                    if(target_status==INPUT_EXIT_SIGNAL){
                        printf("\nExiting linear search demo....\n");
                        return;
                        }

                    if(target_status==0){
                        continue;
                    }

                    break;
                }

        start_t=clock();
            linear_search(arr,target,length_of_array);
        end_t=clock();
        total_t=(double)(end_t-start_t)/CLOCKS_PER_SEC;
        printf("\ntotal CPU time taken for linear search:- %f seconds",total_t);
        printf("\n(most probably execution time would be lesser than clock resolution, resulting in 0.00)");
    }
}

void linear_search(int arr[], int target, int length_of_array){
    for(int i=0;i<length_of_array;i++){
        if(arr[i]==target){
            printf("\nTarget variable found at index %d.\n",i);
            return;
        }
    }
    printf("\nTarget variable not found in the given array.\n");
    return;
}