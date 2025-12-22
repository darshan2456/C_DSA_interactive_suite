#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

typedef struct circular_queue{
    int rear;
    int front;
    int N;
    int* arr;
}circular_queue;

int init_circ_queue(int N, circular_queue* queue_ptr);
void destroy_circ_queue(circular_queue* queue_ptr);
int enqueue(circular_queue* queue_ptr,int value);
int dequeue(circular_queue* queue_ptr);
void display_circ_queue(circular_queue* queue_ptr);
void circular_queue_Demo(void);

#endif