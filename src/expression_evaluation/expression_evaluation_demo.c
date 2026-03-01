#include<stdio.h>
#include "expression.h"
#include "safe_input.h"

void expression_evaluation_demo(void){
    int expr_eval_choice;
    int expr_eval_status;
    while(1){
        expr_eval_status=safe_input_int(
            &expr_eval_choice,
            "\nenter 1 for infix to postfix conversion"
            "\nenter 2 for postfix evaluation"
            "\nenter choice : ",
            1,3
        );
        
        if(expr_eval_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting expression_evaluation_demo.....\n");
            return;
        };

        if(expr_eval_status==0)   continue;

        if(expr_eval_choice==1){
            infix_to_postfix_Demo();
            continue;
        }
        else if(expr_eval_choice==2){
            postfix_evaluation_Demo();
            continue;
        }
    }
}