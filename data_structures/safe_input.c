#include<stdio.h>
#include "safe_input.h"


//this function exits only when user input is successfully inserted into given variable or EOF is encountered,
//or when user enters '-1' ie exit signal

int safe_input_int(int *input, const char *prompt, int min_val, int max_val){
    while(1){
        int c;

        if(prompt){
            printf("%s",prompt);
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
            printf("please enter a number between %d and %d. Try again :",min_val,max_val);
            goto clear_buffer;
        }

        *input=value;
        return 1;                           //represents successful insertion of value into the given variable



        clear_buffer:      while((c=getchar())!='\n' && c!=EOF);                     //clears buffer


        if(c==EOF){                                             //if EOF is encountered
            printf("input ended unexpectedly\n");
            return 0;
        }
    }
}