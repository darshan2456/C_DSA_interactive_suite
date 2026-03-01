#include<stdio.h>
#include<stdlib.h>
#include "array.h"
#include "safe_input.h"

void array_demo(void){

    while(1){
        int arr_length_status;
        int length_of_array;

start_array:                                                
        arr_length_status=safe_input_int(&length_of_array,                                      //taking length of the array
        "\n\nenter length of the array, (between 1 and 100), enter '-1' to exit :- ",
        1,100);

        if(arr_length_status==INPUT_EXIT_SIGNAL){
                printf("\nExiting array demo\n");
                return;
            }
        if(arr_length_status==0){
            goto start_array;
        }
        
        int arr[length_of_array];

        for(int i=0;i<length_of_array;i++){                               //main loop which accepts arr elements value
            int arr_element_status;
enter_arr_element:
            printf("enter array element %d, (between 1 and 100), enter '-1' to exit:- ",i);
            arr_element_status=safe_input_int(&arr[i],NULL,1,100);

            if(arr_element_status==INPUT_EXIT_SIGNAL){
                printf("\nExiting array demo");
                return;
            }
            if(arr_element_status==0){
                goto enter_arr_element;
            }
            printf("\n");
        }


        printf("array you inserted is :- ");                    //print before swap, as user gave, same to same
        print_array(arr,length_of_array);

        swap_adjacent_pairs(arr,length_of_array);              //swapping happens here
        printf("\nafter swap, the array is :- ");                           
        print_array(arr, length_of_array);                      

        swap_adjacent_pairs(arr,length_of_array);               //revert to normal
        printf("\noriginal array:- ");
        print_array(arr,length_of_array);

        
        int max_element=max_array(arr, length_of_array);                     //max element of array
        printf("\n\nmaximum element is :- %d",max_element);

        int min_element=min_array(arr, length_of_array);                    //min element of array
        printf("\nminimum element is :- %d",min_element);


        reverse_array(arr,length_of_array);                 //array is reversed in memory
        printf("\n\nreverse of the array is :- ");
        print_array(arr,length_of_array);

        reverse_array(arr,length_of_array);                 //revert to normal
        printf("\noriginal array :- ");
        print_array(arr,length_of_array);
    }
    
}


void swap_adjacent_pairs(int arr[],int length_of_array){

    for(int i=0;i<length_of_array-1;i+=2){
        int temp=arr[i];
        arr[i]=arr[i+1];
        arr[i+1]=temp;
    }
}


void reverse_array(int arr[], int length_of_array){
    for(int i=0;i<(length_of_array/2);i++){
        int temp=arr[i];
        arr[i]=arr[length_of_array-i-1];
        arr[length_of_array-i-1]=temp;
    }
}

void print_array(const int arr[], int length_of_array){
    printf("[");
    for(int i=0;i<length_of_array;i++){
        printf("%d",arr[i]);
        if(i<length_of_array-1)  printf(",");
    }
    printf("]");
}

int max_array(const int arr[], int length_of_array){
    int max_element=arr[0];
    for(int i=1;i<length_of_array;i++){
        if(arr[i]>max_element){
            max_element=arr[i];
        }
    }
    return max_element;
}

int min_array(const int arr[], int length_of_array){
    int min_element=arr[0];
    for(int i=1;i<length_of_array;i++){
        if(arr[i]<min_element){
            min_element=arr[i];
        }
    }
    return min_element;
}