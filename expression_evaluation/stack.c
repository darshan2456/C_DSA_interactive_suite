#include "stack.h"
#include "sll.h"
#include<stdlib.h>

struct stack{
    Node* top;
};

int push(stack* s,int value){
    if(sll_insertAtBeginning(&(s->top),value)){
        return 1;
    }
    return 0;
}

int pop(stack* s){
    if(s->top==NULL) return -1;
    int top_of_stack=s->top->data;
    sll_deleteAtBeginning(&(s->top));
    return top_of_stack;
}

stack* createStack(void){
    stack* stack_obj=malloc(sizeof(stack));
    if(stack_obj==NULL) return NULL;
    stack_obj->top=NULL;
    return stack_obj;
}

int isEmpty(stack* s){
    return s->top==NULL;
}

void destroyStack(stack* s){
    while(!isEmpty(s)){
        pop(s);
    }
    free(s);
}

int peek(stack* s){
    if(s->top==NULL) return -1;
    return s->top->data;
}