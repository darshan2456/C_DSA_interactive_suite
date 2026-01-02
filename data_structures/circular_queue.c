#include<stdio.h>
#include<stdlib.h>
#include "circular_queue.h"


//init function returns -1 on malloc failure and 1 on correct initialization

//enqueue returns 1 on succesful operation and -1 on failure due to full circular queue

//dequeue returns -1 when circular queue is empty and value when operation is succesful

void circular_queue_Demo(void){
    circular_queue rollnos;
    int capacity;
    int num1,num2,num3,num4;
    printf("enter capacity number (N) of circular queue :- ");
    scanf("%d",&capacity);
    if(!init_circ_queue(capacity,&rollnos)){
        printf("\nmalloc allocation failure");
        return;
    }

    //taking number of elements user want to enqueue
    else{
        int count;
        while(1){
            printf("\nhow many elements you want to enqueue? :- ");
            scanf("%d",&count);
            if(count>capacity-1){
                printf("cannot enqueue more elements than capacity");
                continue;
            }
            else if(count<=0){
                printf("cannot enqueue less elements than 1");
                continue;
            }
            else{
                break;
            }
        }
        
        //enqueue logic
            while(count>0){
                int value;
                printf("\nenter the element you want to enqueue :- ");
                scanf("%d",&value);
                if(enqueue(&rollnos,value)==-1){
                    printf("\nqueue overflow");
                    break;
                }
                count--;
            }
            printf("\nhere is your circular queue :- ");
            display_circ_queue(&rollnos);
        }

        //dequeue logic
        while(1){
            int choice;
            circular_queue* curr=&rollnos;
            printf("\nif you want to dequeue one element press '1' otherwise exit on '0' :- ");
            scanf("%d",&choice);
            if(choice==1){
                if(curr->front==curr->rear){
                    printf("\ncircular queue is empty now.");
                    return;
                }
                dequeue(&rollnos);
                printf("\nhere is your circular queue after dequeue :- ");
                display_circ_queue(&rollnos);
            }
            else if(choice==0){
                break;
            }
            else{
                continue;
            }
        }

        destroy_circ_queue(&rollnos);

}
    
    


int init_circ_queue(int N, circular_queue* queue_ptr){
    if(N<2) return 0;
    queue_ptr->N=N;
    queue_ptr->rear=0;
    queue_ptr->front=0;
    queue_ptr->arr=malloc(sizeof(int)*N);
    if(queue_ptr->arr==NULL) return 0;
    return 1;
}

void destroy_circ_queue(circular_queue* queue_ptr){
    if(queue_ptr->arr==NULL) return;
    free(queue_ptr->arr);
    queue_ptr->arr=NULL;
}

int enqueue(circular_queue* queue_ptr,int value){
    if(((queue_ptr->rear)+1)%(queue_ptr->N)==queue_ptr->front) return -1;
    queue_ptr->arr[queue_ptr->rear]=value;
    queue_ptr->rear=((queue_ptr->rear)+1)%(queue_ptr->N);
    return 1;
}

int dequeue(circular_queue* queue_ptr){
    if(queue_ptr->rear==queue_ptr->front) return -1;
    int front_value=queue_ptr->front;
    queue_ptr->front=((queue_ptr->front)+1)%(queue_ptr->N);
    return queue_ptr->arr[front_value];
}

void display_circ_queue(circular_queue* queue_ptr){
    int i=queue_ptr->front;
    while(i!=queue_ptr->rear){
        printf("%d<->",queue_ptr->arr[i]);
        i=(i+1)%queue_ptr->N;
    }
}

