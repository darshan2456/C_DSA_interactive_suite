#include<stdio.h>
#include "safe_input.h"

//this function return 1 on successful insertion, 0 on failure (invalid input or EOF) 
// and -111 on exit signal ie when user gives input as '-1'

int safe_input_int(int *input, const char *prompt, int min_val, int max_val){
    while(1){
        int c;
        if(prompt){
            printf("%s",prompt);
            fflush(stdout);
        }
        int value;                                  //will be used to store integer part of input
        char extra_char;                            //will be used to store/detect characters in the input buffer after number
        if(scanf("%d",&value)!=1){
            printf("That's not a number. Please try again: \n");
            goto clear_buffer;
        }

        if(scanf("%c",&extra_char)==1 && extra_char!='\n'){
            printf("Only a number please (no extra characters). Try again: \n");
            goto clear_buffer;
        }
        if(value==-1){
            *input=-1;
            return -111;                   //special exit code indicating user entered '-1' ie a signal to exit.
        }
        if(value<min_val || value>max_val){
            printf("only enter values between %d and %d.\n",min_val,max_val);
            printf("enter anything to try again :- ");
            goto clear_buffer;
        }
        *input=value;
        return 1;                           //represents successful insertion of value into the given variable

        clear_buffer:      while((c=getchar())!='\n' && c!=EOF);                     //clears buffer for next attempt
        if(c==EOF){                                             //if EOF is encountered
            printf("input ended unexpectedly\n");
            return 0;
        }

        return 0;     //failure to take input, due to invalid input (characters, special characters or out of range)
    }
    
}