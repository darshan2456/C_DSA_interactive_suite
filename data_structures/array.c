#include<stdio.h>
#include "array.h"

void array_demo(void){
    int length_of_array;
    int choice;

    while(1){
        printf("enter the length of array :- ");
        scanf("%d",&length_of_array);
        int arr[length_of_array];

        for(int i=0;i<length_of_array;i++){
            printf("enter array element %d :- ",i);
            scanf("%d",&arr[i]);
            printf("\n");
        }


        printf("before swap, the array is :- ");                    //print before swap, as user gave, same to same
        print_array(arr,length_of_array);


        swap_adjacent_pairs(arr,length_of_array);              //main swapping happens here


        printf("\nafter swap, the array is :- ");                           //print swapped array
        print_array(arr, length_of_array);                      


        int max_element=max_array(arr, length_of_array);                     //max element of array
        printf("\n\nmaximum element is :- %d",max_element);


        int min_element=min_array(arr, length_of_array);                    //min element of array
        printf("\nminimum element is :- %d",min_element);


        printf("\nenter any number to test another array and '-1' to exit\n");
        scanf("%d",&choice);

        if(choice==-1) break;

    }
    
}


void swap_adjacent_pairs(int arr[],int length_of_array){

    for(int i=0;i<length_of_array-1;i+=2){
        int temp=arr[i];
        arr[i]=arr[i+1];
        arr[i+1]=temp;
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

