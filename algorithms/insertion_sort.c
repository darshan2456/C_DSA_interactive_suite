#include<stdio.h>
#include "array.h"

void insertion_sort(int arr[], int length_of_array);

void insertion_sort_demo(void){
    int length_of_array;
    while(1){
        int choice;
        printf("\nenter the number of elements in the array (not greater than 100) :- ");
        scanf("%d",&length_of_array);

        if(length_of_array<1 || length_of_array>100){
            printf("\nenter length of array between 1 and 100\n");
            continue;
        }
        int arr[length_of_array];

        for(int i=0;i<length_of_array;i++){
            printf("\nenter the element number %d :- ",i);
            scanf("%d",&arr[i]);
        }

        printf("\nsee insertion sort in action :-\n");
        insertion_sort(arr,length_of_array);

        printf("\nenter any number to test another array or enter '-1' to exit :- ");
        scanf("%d",&choice);

        if(choice==-1)   break;

    }
    
}

void insertion_sort(int arr[], int length_of_array){

    for(int i=1;i<length_of_array;i++){
        int key=arr[i];
        int j=i-1;
        while(j>=0 && arr[j]>key){
            arr[j+1]=arr[j];
            j--;
        }
        arr[j+1]=key;

        print_array(arr,length_of_array);
        printf("\n");
    }
}