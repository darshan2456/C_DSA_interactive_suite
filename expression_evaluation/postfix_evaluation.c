#include<stdio.h>
#include "stack.h"
#include<ctype.h>
#include "safe_input.h"

//if postfix expression attempts to divide by zero or the stack doesnt get emptied at the end of main while loop, it indicates malformed postfix expression and
//program exits with error code '-1' and on succesful evaluation returns '0'
//maximum expression length is 50 characters

static int isOperator(char ch){
    if(ch=='+' || ch=='-' || ch=='*' || ch=='/') return 1;
    return 0;
}

void postfix_evaluation_Demo(void){
    int choice;
    char postfix_expr[50];
    while(1){
        stack* operands=createStack();

        int postfix_expr_status=validate_postfix_expr(postfix_expr,sizeof(postfix_expr),
        "\nenter valid postfix expression (only single digit operands), enter 'X' to exit:- ");
        
        if(postfix_expr_status==INPUT_EXIT_SIGNAL){
            destroyStack(operands);
            printf("\nExiting postfix evaluation demo...\n");
            return;
        }

        if(postfix_expr_status!=1){
            destroyStack(operands);
            continue;
        }
        int i=0;
        while(postfix_expr[i]!='\0'){
            char ch=postfix_expr[i];
            if(isdigit(ch)) push(operands,ch-'0');
            else if(isOperator(ch)){
                if(isEmpty(operands)){
                    destroyStack(operands);
                    return;
                }
                int right_operand=pop(operands);
                if(isEmpty(operands)){
                    destroyStack(operands);
                    return;
                }
                int left_operand=pop(operands);
                int result=0;
                if(ch=='+') result= left_operand + right_operand;
                else if(ch=='-') result= left_operand - right_operand;
                else if(ch=='*') result= left_operand * right_operand;
                else if(ch=='/'){
                    if(right_operand==0){
                        destroyStack(operands);
                        return;
                    }
                    result= left_operand / right_operand;
                }
                push(operands,result);
            }
            i++;
        }

        int final_result=pop(operands);

        if(!isEmpty(operands)){
            destroyStack(operands);
            return;
        }
        destroyStack(operands);
        printf("Result of postfix expression is :- %d",final_result);
        
    }
    
    return;
}