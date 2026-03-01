#include<stdio.h>
#include<string.h>
#include "safe_input.h"
#include<ctype.h>

//this function returns 1 on valid postfix expression and 0 on EOF/failure, -111 on exit signal ie 'X'
//input cannot contain whitespaces

int validate_postfix_expr(char *buff,size_t size,const char *prompt){

        int depth=0;

        if(prompt){
            printf("%s",prompt);
            fflush(stdout);
        }

        if(!fgets(buff,size,stdin)){                                //return code 0 represents EOF
            printf("\ninput ended unexpectedly");
            return 0;
        }

        buff[strcspn(buff,"\n")]='\0';

        if(buff[0]=='X' && buff[1]=='\0'){                      //when user enters 'X'
            return INPUT_EXIT_SIGNAL;
        }   

        size_t i=0;

        while(buff[i]!='\0'){               //main loop which validates the string character by character
            unsigned char c=buff[i];
            if(isalnum(c))      depth++;
            else if(c=='+' || c=='-' || c=='*' || c=='/'){
                depth--;
                if(depth<1){
                    printf("\ninvalid: too many operators");
                    return 0;
                    }
                }

                else{
                    printf("\ninvalid character : %c",c);           //character other than alphanumeric and +,-,*,/
                    return 0;
                }

                i++;
            }
        
        if(depth>1){
            printf("\ninvalid: too many operands");
            return 0;
        }
        if(depth==1){
            return 1;
        }
    return 1;
}