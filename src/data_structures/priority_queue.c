#include "priority_queue.h"
#include "array.h"
#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

char* return_heap_type(int heapType)
{
    return heapType == 0 ? "Min" : "Max";
}

priority_queue* pq_init(HeapType heapType)
{
    priority_queue* pq = malloc(sizeof(priority_queue));

    if (pq == NULL)
        return NULL;
    pq->size = 0;
    pq->heapType = heapType;

    return pq;
}

int insert(priority_queue* pq, int val)
{
    if (pq == NULL || pq->size == HEAP_CAPACITY)
        return 0;

    int i = pq->size;
    pq->heap[i] = val;
    pq->size++;

    printf("Inserted :%d\n", val);
    printf("Before Heapify:\n");
    display_heap(pq);

    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (pq->heapType == MIN_HEAP)
        {
            if (pq->heap[i] >= pq->heap[parent])
                break;
        }
        else
        {
            if (pq->heap[i] <= pq->heap[parent])
                break;
        }
        printf("Heapify Up : Swap(%d<->%d)\n", pq->heap[i], pq->heap[parent]);
        swap(&pq->heap[i], &pq->heap[parent]);
        printf("\nAfter Heapify:\n");
        display_heap(pq);
        i = parent;
    }

    return 1;
}

bool extractTop(priority_queue* pq, int* result)
{
    if (pq == NULL || result == NULL || pq->size == 0)
        return false;

    int topIndex = 0;
    int topElement = pq->heap[topIndex];
    int lastElementIndex = pq->size - 1;

    pq->heap[topIndex] = pq->heap[lastElementIndex];
    pq->size--;

    int i = 0;

    while (1)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int target = i;

        if (pq->heapType == MIN_HEAP)
        {
            if (left < pq->size && pq->heap[left] < pq->heap[target])
                target = left;

            if (right < pq->size && pq->heap[right] < pq->heap[target])
                target = right;
        }
        else
        {
            if (left < pq->size && pq->heap[left] > pq->heap[target])
                target = left;

            if (right < pq->size && pq->heap[right] > pq->heap[target])
                target = right;
        }

        if (target == i)
            break;

        swap(&pq->heap[i], &pq->heap[target]);
        i = target;
    }

    *result = topElement;

    return true;
}

bool peek_pq(priority_queue* pq, int* result)
{
    if (pq == NULL || result == NULL || pq->size == 0)
        return false;
    *result = pq->heap[0];
    return true;
}

void destroy_pq(priority_queue* pq)
{
    if (pq == NULL)
        return;

    free(pq);
}

void print_heap_tree(priority_queue* pq)
{
    if (pq == NULL || pq->size == 0)
    {
        printf("\nHeap Empty\n");
        return;
    }

    printf("\nHeap Tree:\n");

    int level = 0;
    int index = 0;

    while (index < pq->size)
    {
        int nodes = 1 << level;

        // indentation
        for (int s = 0; s < (3 - level) * 4; s++)
            printf(" ");

        for (int j = 0; j < nodes && index < pq->size; j++)
        {
            printf("%d", pq->heap[index++]);

            // spacing between siblings
            for (int s = 0; s < (4 - level) * 5; s++)
                printf(" ");
        }

        printf("\n\n");

        level++;
    }
}
void display_heap(priority_queue* pq)
{
    printf("\nArray Representation: ");
    print_array(pq->heap, pq->size);
    print_heap_tree(pq);
    printf("\n");
}
