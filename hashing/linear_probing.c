#include<stdio.h>
#include<safe_input.h>
#include "array.h"
#include "hash.h"

//below two functions are helper functions local to this file and not supposed to used as an API in other modules

static int next_prime(int n){
    int size=sizeof(PRIMES)/sizeof(PRIMES[0]);
    for(int i=n;i<size;i++){
        if(PRIMES[i]){
            return i;
        }
    }
    return -1;
}
 
int hash_function(int value,int length_of_array){
    int next_prime_no=next_prime(length_of_array);
    return ((value+1)*next_prime_no)%length_of_array;
}
 
void linear_probing_demo(void){
    while(1){
        int value;
        int length_of_array;

        int length_arr_status=safe_input_int(&length_of_array,
        "\n\nenter length of the array (between 1 and 1000):- ",1,1000);
 
        if(length_arr_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting linear_probing demo");
            return;
        }
        if(length_arr_status==0){
            continue;
        }

        int arr[length_of_array];           //allocate memory for the array

        for(int i=0;i<length_of_array;i++){         //initialize all elements to 0
            arr[i]=0;
        }
take_value:
        int value_status=safe_input_int(&value,
        "\nenter a value between 1 and 1000 (enter '-1' to exit):- ",1,1000);
        
        if(value_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting linear_probing demo.....");
            return;
        }
        if(value_status==0){
            goto take_value;
        }

        int hash_location=hash_function(value,length_of_array);             //calling the hash function on user input value
 
        if(!arr[hash_location]){
            arr[hash_location]=value;           //inserting value at its hash location
            print_array(arr,length_of_array);
            goto take_value;
        }
        
        int start=hash_location;            //the modulo arithmetic for wrap-around logic, same as circular queue
        do{
            hash_location=(hash_location+1)%length_of_array;
            if(hash_location==start){
                printf("\nhash table full....\n");
                goto take_value;
            }
        }while (arr[hash_location]);
        arr[hash_location]=value;           //inserting value at its hash location
 
        print_array(arr,length_of_array);
        goto take_value;
    }
}