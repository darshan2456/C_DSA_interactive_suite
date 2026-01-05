#include<stdio.h>
#include "expression.h"
#include "safe_input.h"

void expression_evaluation_demo(void){
    int choice;
    int status;
    while(1){
        status=safe_input_int(
            &choice,
            "\nenter 1 for infix to postfix conversion"
            "\nenter 2 for postfix evaluation"
            "\nenter choice : ",
            1,3
        );
        
        if(status==-111)    break;

        if(status==0)   continue;

        if(choice==1){
            infix_to_postfix_Demo();
            continue;
        }
        else if(choice==2){
            postfix_evaluation_Demo();
            continue;
        }
    }



}