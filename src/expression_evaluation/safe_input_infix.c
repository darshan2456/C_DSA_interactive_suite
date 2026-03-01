#include<stdio.h>
#include "safe_input.h"
#include<ctype.h>
#include "string.h"

//this function returns 1 on valid infix expression and 0 on EOF/failure, -111 on exit signal ie 'X'
//input can not contain whitespaces

int validate_infix_expr(char *buff,size_t size,const char *prompt){

    if(prompt){
        printf("%s",prompt);
        fflush(stdout);
    }
    if(!fgets(buff,size,stdin)){                            //return code 0 represents EOF
        printf("\ninput ended unexpectedly");
        return 0;
    }
    buff[strcspn(buff,"\n")]='\0';
    if(buff[0]=='X' && buff[1]=='\0'){              //when user enters 'X'
        return INPUT_EXIT_SIGNAL;
    }
    size_t i=0;
    int balance=0;

    while(buff[i]!='\0'){                 //main while loop which does the infix expression validation
        char c=buff[i];
        if(isalnum((unsigned char)c) || c=='(' || c==')' || c=='+' || c=='-' || c=='*' || c=='/'){
            if(c=='('){
                balance++;
                i++;
                continue;
            }
            if(c==')'){
                balance--;
                if(balance<0){
                    printf("\ninvalid: ')' before '('\n");
                    return 0;
                }
                i++;
                continue;
                }
            }
            
            else{
                printf("\ninvalid character: %c\n",c);
                printf("\nonly alphanumeric character, parantheses, +,-,*,/ operators are supported.\neg - A+B-C*D/E+(F-G) is a valid"
                    " infix expression where alphabets can be any alphanumeric character - A,b,1,2 etc");
                
                return 0;                       
            }
            i++;
    }                               //end of main while loop which performs validation
    if(balance!=0){
        printf("\ninvalid: '(' without ')'\n");
        return 0;
    }
    return 1;                           //return code 1 represents valid string input

}