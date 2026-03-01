#include<stdio.h>
#include<string.h>
#include "sll.h"
#include "safe_input.h"
#include "hash.h"

void separate_chaining_demo(void){
    while (1){
        int value;
        int hash_table_size;
        int hash_table_status=safe_input_int(&hash_table_size,
        "\nenter the size of the hash table (between 1 and 1000), enter '-1' to exit:- ",1,1000);

        if(hash_table_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting separate chaining demo....");
            return;
        }
        if(hash_table_status==0){
            continue;
        }

        Node* table[hash_table_size];       //initialize an array consisting of pointers of type Node*

        memset(table,0,sizeof(table));      //initialize every pointer to NULL

        while (1){
            int value_status=safe_input_int(&value,
            "\nenter the value between 1 and 1000 (enter '-1' to exit):- ",1,1000);

            if(value_status==INPUT_EXIT_SIGNAL){
                printf("\nExiting separate chaining demo....");
                for(int i=0;i<hash_table_size;i++){
                    delete_sll(table[i]);
                }
                return;
            }
            if(value_status==0){
                continue;
            }

            int hash_location=hash_function(value,hash_table_size);

            sll_insertAtEnd(&table[hash_location],value);       //insert value by passing the address of the pointer to Node

            for(int i=0;i<hash_table_size;i++){
                printf("bucket %d -> ",i);
                sll_printlist(table[i]);
                printf("\n");
            }
        }
    }
}