#include<stdio.h>
#include<stdlib.h>
#include "circular_queue.h"
#include "safe_input.h"

//init function returns -1 on malloc failure and 1 on correct initialization

//enqueue returns 1 on succesful operation and -1 on failure due to full circular queue

//dequeue returns -1 when circular queue is empty and value when operation is succesful

void circular_queue_Demo(void){

    while(1){
        circular_queue rollnos;
        int queue_capacity_value;
        int queue_capacity_status=safe_input_int(&queue_capacity_value,
        "\n\nenter capacity number (N) of circular queue, (between 1 and 100), enter '-1' to exit:- ",
        1,100);

        if(queue_capacity_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting circular queue demo\n");
            destroy_circ_queue(&rollnos);
            return;
        }
        if(queue_capacity_status==0){
            continue;
        }
        if(!init_circ_queue(queue_capacity_value,&rollnos)){
            printf("\nmalloc allocation failure");
            return;
        }

        //loop of enqueue and dequeue (interleaved enqueue and dequeue)
        
            while (1) {
                int circ_queue_choice;
                int circ_queue_status = safe_input_int(
                    &circ_queue_choice,
                    "\n\nenter 1 for enqueue, 2 for dequeue and '-1' for exit:- ",
                    1, 2
                );
            
                if (circ_queue_status == INPUT_EXIT_SIGNAL) {
                    printf("\nExiting circular queue demo\n");
                    destroy_circ_queue(&rollnos);
                    return;
                }
            
                if (circ_queue_status == 0) {           //invalid input, (chars or number+chars) loopback to start
                    continue;
                }
            
                if (circ_queue_choice == 1) {
                    int enqueue_val;
                    int enqueue_val_status = safe_input_int(
                        &enqueue_val,
                        "\nEnter value to enqueue (between 1 and 100), enter '-1' to exit:- ",
                        1, 100
                    );
                
                    if (enqueue_val_status == INPUT_EXIT_SIGNAL) {
                        printf("\nExiting circular queue demo\n");
                        destroy_circ_queue(&rollnos);
                        return;
                    }
                
                    if (enqueue_val_status == 0) {          //invalid input, (chars or number+chars) loopback to start
                        continue;
                    }
                
                    if (enqueue(&rollnos, enqueue_val) == -1) {
                        printf("\nQueue is full (circular overflow)\n");
                    }

                    display_circ_queue(&rollnos);
                }
            
                else if (circ_queue_choice == 2) {
                    int removed = dequeue(&rollnos);
                
                    if (removed == -1) {
                        printf("\nQueue is empty\n");
                    } else {
                        printf("\nDequeued element: %d\n", removed);
                    }

                    display_circ_queue(&rollnos);
                }
            }
        
    }
}
    
    


int init_circ_queue(int N, circular_queue* queue_ptr){
    if(N<1) return 0;
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