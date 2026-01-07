#include<stdio.h>
#include "array.h"

void selection_sort(int arr[],int length_of_array);

void selection_sort_demo(void){
    int length_of_array;
    while(1){
        int choice;
        printf("\nenter the number of elements in the array (not greater than 100) :- ");
        scanf("%d",&length_of_array);
        if(length_of_array<1 || length_of_array>100){
            printf("\nenter length of array between 1 and 100 \n");
            continue;
        }
        int arr[length_of_array];

        for(int i=0;i<length_of_array;i++){
            printf("\nenter element no %d :- ",i);
            scanf("%d",&arr[i]);
        }

        printf("\nsee selection sort in action:\n");
        selection_sort(arr,length_of_array);

        printf("\nenter any number to test another array and enter '-1' to exit :- ");
        scanf("%d",&choice);

        if(choice==-1)  break;
    }

}

void selection_sort(int arr[],int length_of_array){
    
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
        
        printf("after iteration no %d - ",i+1);
        print_array(arr,length_of_array);
        printf("\n");
    }
}