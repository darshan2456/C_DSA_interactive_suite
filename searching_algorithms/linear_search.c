#include<stdio.h>
#include "safe_input.h"

void linear_search(int arr[], int target, int length_of_array);

void linear_search_demo(void){

    while(1){

            int length_of_array=0;
            int target=0;
            int linear_search_status=0;

retry_len:  printf("\nLinear search demo\n");
            linear_search_status=safe_input_int(&length_of_array,
            "\nenter length of array, (between 1 and 100), enter '-1' to exit:- ",
            1,100);

            if(linear_search_status==INPUT_EXIT_SIGNAL){
                printf("\nExiting linear search demo....\n");
                return;
            }

            if(linear_search_status==0){
                goto retry_len;
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
retry_target:       target_status=safe_input_int(&target,
                    "\nEnter target which you want to search, (between 1 and 100), enter '-1' to exit:- ",
                    1,100);

                if(target_status==INPUT_EXIT_SIGNAL){
                    printf("\nExiting linear search demo....\n");
                    return;
                    }

                if(target_status==0){
                    printf("\nentered invalid input or characters other than numbers. Try again\n");
                    goto retry_target;
                }

            linear_search(arr,target,length_of_array);
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