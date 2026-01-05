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
        printf("\nenter 1 for standard linear data structures"
                "\nenter 2 for circular variants of linear data structures"
                "\nenter 3 for non-linear data structures"
                "\nenter choice : ");

        status=safe_input_int(&choice,NULL,1,3);

        if(status==-111){
            printf("Exiting data structures demo....\n");
            return;
        }

        if(status==0){
            printf("input ended unexpectedly. Exiting....");
            return;
        }   



        switch(choice){

            case 1:                                                 //sll,dll and arrays case
                while(1){
                    status=safe_input_int(&choice,"\nenter 1 for singly linked list demo"
                    "\nenter 2 for doubly linked list demo"
                    "\nenter 3 for arrays demo"
                    "\nenter choice : ",1,3);
                    
                    if(status==-111)    goto outer_exit;
                    
                    if(status==0)   continue;

                    if(choice==1){
                        sll_Demo();
                        break;
                    }
                    if(choice==2){
                        dll_demo();
                        break;
                    }
                    if(choice==3){
                        array_demo();
                        break;
                    }
                }
                break;

            case 2:                                              //circular variants case
                status=safe_input_int(
                &choice,
                "enter 1 for circular queue demo : ",
                1,1
                );
                if(choice==-111)    goto outer_exit;
                if(status==1){
                    circular_queue_Demo();
                }
                break;
                    
            case 3:                                                   //non linear data structures case

                status=safe_input_int(
                &choice,
                "enter 1 for binary search tree demo : ",
                1,1
                );
                if(status==-111)    goto outer_exit;
                if(status==1){
                    binary_search_tree_Demo();
                }
                break;
                    
        }       

    outer_exit: printf("\nreturning to main menu.");

    }
}      
