#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include "stack.h"

//RN this program only has support for four operators - +-/* and parantheses. this program doesnt support '^ or %' operators

int precedence(char ch){        //if precedence returns -1 char is not an operator or a left parantheses
    if(ch=='*' || ch=='/') return 2;
    else if(ch=='+' || ch=='-') return 1;
    else return -1;
}

static int isOperator(char ch){
    if(ch=='+' || ch=='-' || ch=='*' || ch=='/') return 1;
    return 0;
}

void infix_to_postfix_Demo(void){}


// int main(){
//     char infix_expr[50];
//     stack* operators=createStack();
//     printf("enter a mathematical expression without whitespaces :- ");
//     scanf("%49s",infix_expr);
//     int i=0;
//     while(infix_expr[i]!='\0'){
//         char ch=infix_expr[i];
//         if(isalnum(ch)) printf("%c",ch);

//         else if(ch=='(') push(operators,ch);

//         else if(ch==')'){
//             while(!isEmpty(operators) && peek(operators)!='('){
//                 char op=pop(operators);
//                 printf("%c",op);
//             }
//             pop(operators);
//         }

//         else if(isOperator(ch)){
//             if(isEmpty(operators)){
//                 push(operators,ch);
//             }
//             else if(precedence(ch) > precedence(peek(operators))){
//                 push(operators,ch);
//             }

//             else if(precedence(ch) <= precedence(peek(operators))){
//                 int prec_lower=precedence(ch);
//                 while(!isEmpty(operators) && peek(operators)!='(' && precedence(peek(operators))>=prec_lower){
//                     char op=pop(operators);
//                     printf("%c",op);
//                 }
//                 push(operators,ch);
//             }
//         }

//     i++;
//     }

//     while(!isEmpty(operators)){
//         char op=pop(operators);
//         printf("%c",op);
//     }

//     destroyStack(operators);
//     return 0;
// }
