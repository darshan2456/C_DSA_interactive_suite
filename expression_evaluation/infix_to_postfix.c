    #include<stdio.h>
    #include<ctype.h>
    #include<string.h>
    #include "stack.h"
    #include "safe_input.h"

    //rn this program only has support for four operators - +-/* and parantheses. this program doesnt support '^ or %' operators
    //maximum expression length is 50 characters

    int precedence(char ch){        //if precedence returns -1 char is not an operator or a left parantheses
        if(ch=='*' || ch=='/') return 2;
        else if(ch=='+' || ch=='-') return 1;
        else return -1;
    }

    static int isOperator(char ch){
        if(ch=='+' || ch=='-' || ch=='*' || ch=='/') return 1;
        return 0;
    }

    void infix_to_postfix_Demo(void){
        int infix_to_postfix_choice;
        char infix_expr[50];
        while(1){
            stack* operators=createStack();

            int infix_expr_status=validate_infix_expr(infix_expr,sizeof(infix_expr),
            "\nenter a mathematical expression without whitespaces, enter 'X' to exit:- ");

            if(infix_expr_status==INPUT_EXIT_SIGNAL){
                destroyStack(operators);
                printf("\nExiting infix to postfix demo ...\n");
                return;
            }
            
            if(infix_expr_status!=1){
                destroyStack(operators);
                continue;
            }

            int i=0;
            while(infix_expr[i]!='\0'){
            char ch=infix_expr[i];
                if(isalnum((unsigned char)ch)) printf("%c",ch);

                else if(ch=='(') push(operators,ch);

                else if(ch==')'){
                    while(!isEmpty(operators) && peek(operators)!='('){
                        char op=pop(operators);
                        printf("%c",op);
                    }
                    pop(operators);
                }

                else if(isOperator(ch)){
                    if(isEmpty(operators)){
                        push(operators,ch);
                    }
                    else if(precedence(ch) > precedence(peek(operators))){
                        push(operators,ch);
                    }

                    else if(precedence(ch) <= precedence(peek(operators))){
                        int prec_lower=precedence(ch);
                        while(!isEmpty(operators) && peek(operators)!='(' && precedence(peek(operators))>=prec_lower){
                            char op=pop(operators);
                            printf("%c",op);
                        }
                        push(operators,ch);
                    }
                }

            i++;
            }

            while(!isEmpty(operators)){         //flushing all remaining operators in the stack
                char op=pop(operators);
                printf("%c",op);
            }

            destroyStack(operators);


           /* printf("\nenter any number to test another expression and '-1' to exit :- ");
            scanf("%d",&choice);
            if(choice==-1) break;   */
        }
        
    }




