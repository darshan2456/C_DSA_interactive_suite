#include<stdio.h>
#include "sll.h"
#include "dll.h"
#include "array.h"
#include "circular_queue.h"
#include "bst.h"
#include "safe_input.h"

void data_structures_demo(void){
    int choice;
    int status;
    while(1){
        int data_structures_choice;
        int data_structures_status=safe_input_int(&data_structures_choice,"\nenter 1 for standard linear data structures"
                "\nenter 2 for circular variants of linear data structures"
                "\nenter 3 for non-linear data structures"
                "\nenter choice : ",1,3);

        if(data_structures_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting data structures demo....\n");
            return;
        }

        if(data_structures_status==0){
            printf("input ended unexpectedly. Exiting....");
            return;
        }   



        switch(data_structures_choice){

            case 1:                                                 //sll,dll and arrays case
                while(1){
                    int linear_ds_choice;
                    int linear_ds_status=safe_input_int(&linear_ds_choice,"\nenter 1 for singly linked list demo"
                    "\nenter 2 for doubly linked list demo"
                    "\nenter 3 for arrays demo"
                    "\nenter choice : ",1,3);
                    
                    if(linear_ds_status==INPUT_EXIT_SIGNAL)    break;
                    
                    if(linear_ds_status==0)     continue;

                    if(linear_ds_choice==1){
                        sll_Demo();
                        break;
                    }
                    if(linear_ds_choice==2){
                        dll_demo();
                        break;
                    }
                    if(linear_ds_choice==3){
                        array_demo();
                        break;
                    }
                }
                break;

            case 2:                                                     //circular variants case
                
                while(1){
                    int circular_variant_choice=0;                                           
                    int circular_variant_status=safe_input_int(
                    &circular_variant_choice,
                    "enter 1 for circular queue demo : ",
                    1,1
                    );
                    if(circular_variant_status==0)  continue;
                    if(circular_variant_status==INPUT_EXIT_SIGNAL)    break;
                    if(circular_variant_choice==1){
                        circular_queue_Demo();
                    }
                    break;
                    }
                
                break;
                    
            case 3:                                                 //non linear data structures case
                while(1){                                      
                    int non_linear_ds_choice;
                    int non_linear_ds_status=safe_input_int(
                    &non_linear_ds_choice,
                    "enter 1 for binary search tree demo : ",
                    1,1
                    );
                    if(non_linear_ds_status==INPUT_EXIT_SIGNAL)    break;
                    if(non_linear_ds_status==0)     continue;
                    if(non_linear_ds_choice==1){
                        binary_search_tree_Demo();
                    }
                    break;
                    }
                break;
                    
        }       

    
    printf("\nreturning to main menu....\n");

    }
}      
