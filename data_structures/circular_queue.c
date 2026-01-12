#include<stdio.h>
#include<stdlib.h>
#include "circular_queue.h"
#include "safe_input.h"

//init function returns -1 on malloc failure and 1 on correct initialization

//enqueue returns 1 on succesful operation and -1 on failure due to full circular queue

//dequeue returns -1 when circular queue is empty and value when operation is succesful

void circular_queue_Demo(void){

    while(1){

circular_queue_start:
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
            goto circular_queue_start;
        }
        if(!init_circ_queue(queue_capacity_value,&rollnos)){
            printf("\nmalloc allocation failure");
            return;
        }

        //taking number of elements user want to enqueue
        else{
            int queue_count_value;

            while(1){
                int queue_count_status;
                queue_count_status=safe_input_int(&queue_count_value,
                "\nhow many elements you want to enqueue? (between 1 and circular queue capacity -1), enter '-1' to exit:- ",
                1,100);
                
                if(queue_count_status==INPUT_EXIT_SIGNAL){
                    printf("\nExiting circular queue demo\n");
                    destroy_circ_queue(&rollnos);
                    return;
                }
                if(queue_count_value==0){
                    continue;
                }

                if(queue_count_value>queue_capacity_value-1){
                    printf("cannot enqueue more elements than capacity");
                    continue;
                }
                else if(queue_count_value<=0){
                    printf("cannot enqueue less elements than 1");
                    continue;
                }
                else{
                    break;
                }
            }

            //enqueue logic
                while(queue_count_value>0){
                    int circular_queue_element;
                    int circ_queue_elem_status;
                    circ_queue_elem_status=safe_input_int(&circular_queue_element,
                    "\nenter the element you want to enqueue, (between 1 and 100), enter '-1' to exit:- ",
                    1,100);

                    if(circ_queue_elem_status==INPUT_EXIT_SIGNAL){
                        printf("\nExiting circular queue demo\n");
                        destroy_circ_queue(&rollnos);
                        return;
                    }
                    if(circ_queue_elem_status==0){
                        continue;
                    }
                    if(enqueue(&rollnos,circular_queue_element)==-1){
                        printf("\nqueue overflow");
                        break;
                    }
                    queue_count_value--;
                }
                printf("\nhere is your circular queue :- ");
                display_circ_queue(&rollnos);
            }

            //dequeue logic
            while(1){
                int circ_queue_deque_status;
                int circ_queue_deque_signal;
                circular_queue* curr=&rollnos;

                circ_queue_deque_status=safe_input_int(&circ_queue_deque_signal,
                "\nif you want to dequeue one element press '1' otherwise exit on '-1' :- ",
                1,1);

                if(circ_queue_deque_status==INPUT_EXIT_SIGNAL){
                    printf("\nExiting circular queue demo\n");
                    destroy_circ_queue(&rollnos);
                    return;
                }
                if(circ_queue_deque_status==0){
                    continue;
                }
        
                if(circ_queue_deque_signal==1){
                    if(curr->front==curr->rear){
                        printf("\ncannot deque empty queue. exiting.\n");
                        break;
                    }
                    dequeue(&rollnos);
                    printf("\nhere is your circular queue after dequeue :- ");
                    display_circ_queue(&rollnos);
                }
                
                else{
                    continue;
                }
            }

            destroy_circ_queue(&rollnos);
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

