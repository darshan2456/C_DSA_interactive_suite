#include<stdio.h>
#include "array.h"

void bubble_sort_optimized(int arr[], int length_of_array);

void bubble_sort_optimized_demo(void){
    int length_of_array;
    while(1){
        int choice;
        printf("\nenter no of elements of array you want to sort (dont give greater than 100):- ");
        scanf("%d",&length_of_array);

        if(length_of_array<1 || length_of_array>100){
            printf("\nenter length of array between 1 and 1oo\n");
            continue;
        }
        int arr[length_of_array];

        for(int i=0;i<length_of_array;i++){
            printf("\nenter element no %d :- ",i);
            scanf("%d",&arr[i]);
        }

        printf("\nsee bubble sort in action :- \n");
        bubble_sort_optimized(arr,length_of_array);

        printf("\nenter any number to again try bubble_sort or '-1' to exit :- ");
        scanf("%d",&choice);

        if(choice==-1)  break;
    }
    
}

void bubble_sort_optimized(int arr[], int length_of_array){
    for(int i=0;i<length_of_array-1;i++){
        int swapped=0;

        for(int j=0;j<length_of_array-i-1;j++){
            if(arr[j]>arr[j+1]){                                      //swapping happens here
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
}