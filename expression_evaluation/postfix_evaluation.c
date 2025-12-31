#include<stdio.h>
#include "stack.h"
#include<ctype.h>

//if postfix expression attempts to divide by zero or the stack doesnt get emptied at the end of main while loop, it indicates malformed postfix expression and
//program exits with error code '-1' and on succesful evaluation returns '0'
//maximum expression length is 50 characters

static int isOperator(char ch){
    if(ch=='+' || ch=='-' || ch=='*' || ch=='/') return 1;
    return 0;
}

int postfix_evaluation_Demo(void){
    int choice;
    char postfix_expr[50];
    while(1){
        stack* operands=createStack();
        printf("enter valid postfix expression (only single digit operands):- ");
        scanf("%49s",postfix_expr);
        int i=0;
        while(postfix_expr[i]!='\0'){
            char ch=postfix_expr[i];
            if(isdigit(ch)) push(operands,ch-'0');
            else if(isOperator(ch)){
                if(isEmpty(operands)){
                    destroyStack(operands);
                    return -1;
                }
                int right_operand=pop(operands);
                if(isEmpty(operands)){
                    destroyStack(operands);
                    return -1;
                }
                int left_operand=pop(operands);
                int result=0;
                if(ch=='+') result= left_operand + right_operand;
                else if(ch=='-') result= left_operand - right_operand;
                else if(ch=='*') result= left_operand * right_operand;
                else if(ch=='/'){
                    if(right_operand==0){
                        destroyStack(operands);
                        return -1;
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
            return -1;
        }
        destroyStack(operands);
        printf("Result of postfix expression is :- %d",final_result);
        printf("\nif want to enter another expression enter any number and for exit enter '-1' :- ");
        scanf("%d",&choice);
        if(choice==-1) break;
    }
    
    return 0;
}


    
