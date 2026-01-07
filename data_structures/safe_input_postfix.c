#include<stdio.h>
#include<string.h>
#include "safe_input.h"
#include<ctype.h>

//this function returns 1 on valid postfix expression and 0 on EOF and retry until valid expression
//input is taken by scanf() so input parameter cannot contain whitespaces
//so input parameter must not contain whitespaces

int validate_postfix_expr(char *buff,size_t size,const char *prompt){

    while(1){
        
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
                    goto retry;
                    }
                }

                else{
                    printf("\ninvalid character : %c",c);           //character other than alphanumeric and +,-,*,/
                    goto retry;
                }

                i++;
            }
        
        if(depth>1){
            printf("\ninvalid: too many operands");
            continue;
        }
        if(depth==1){
            return 1;
        }
        
        
        retry:      continue;

    }

    return 1;
}