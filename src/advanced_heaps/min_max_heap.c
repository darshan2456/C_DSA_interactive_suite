#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

/* Helper to determine if a node is at a min level (even depth) */
static bool is_min_level(int i)
{
    /* Level of node i is floor(log2(i + 1)) */
    int level = 0;
    int val = i + 1;
    while (val > 1)
    {
        val >>= 1;
        level++;
    }
    return (level % 2 == 0);
}

/* Creates an empty Min-Max heap with the specified capacity */
MinMaxHeap* create_min_max_heap(int capacity)
{
    MinMaxHeap* heap = (MinMaxHeap*)malloc(sizeof(MinMaxHeap));
    if (heap == NULL)
    {
        return NULL;
    }
    heap->arr = (MinMaxHeapNode*)malloc(capacity * sizeof(MinMaxHeapNode));
    if (heap->arr == NULL)
    {
        free(heap);
        return NULL;
    }
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

/* Destroys the Min-Max Heap and frees all allocated memory */
void destroy_min_max_heap(MinMaxHeap* heap)
{
    if (heap == NULL)
    {
        return;
    }
    free(heap->arr);
    free(heap);
}

/* Helper to swap elements in the Min-Max heap array */
static void swap_nodes(MinMaxHeap* heap, int i, int j)
{
    MinMaxHeapNode temp = heap->arr[i];
    heap->arr[i] = heap->arr[j];
    heap->arr[j] = temp;
}

/* Push up operations to maintain min-max property after insertion */
static void push_up_min(MinMaxHeap* heap, int i)
{
    if (heap == NULL || i < 0 || i >= heap->size)
    {
        return;
    }
    if (i > 2)
    {
        int grandparent = ((i - 1) / 2 - 1) / 2;
        if (heap->arr[i].key < heap->arr[grandparent].key)
        {
            swap_nodes(heap, i, grandparent);
            push_up_min(heap, grandparent);
        }
    }
}

static void push_up_max(MinMaxHeap* heap, int i)
{
    if (heap == NULL || i < 0 || i >= heap->size)
    {
        return;
    }
    if (i > 2)
    {
        int grandparent = ((i - 1) / 2 - 1) / 2;
        if (heap->arr[i].key > heap->arr[grandparent].key)
        {
            swap_nodes(heap, i, grandparent);
            push_up_max(heap, grandparent);
        }
    }
}

static void push_up(MinMaxHeap* heap, int i)
{
    if (heap == NULL || i < 0 || i >= heap->size)
    {
        return;
    }
    if (i == 0)
    {
        return;
    }

    int parent = (i - 1) / 2;
    if (is_min_level(i))
    {
        if (heap->arr[i].key > heap->arr[parent].key)
        {
            swap_nodes(heap, i, parent);
            push_up_max(heap, parent);
        }
        else
        {
            push_up_min(heap, i);
        }
    }
    else
    {
        if (heap->arr[i].key < heap->arr[parent].key)
        {
            swap_nodes(heap, i, parent);
            push_up_min(heap, parent);
        }
        else
        {
            push_up_max(heap, i);
        }
    }
}

/* Inserts a key-value pair into the Min-Max Heap */
bool min_max_heap_insert(MinMaxHeap* heap, int key, int value)
{
    if (heap == NULL || heap->size >= heap->capacity)
    {
        return false;
    }

    heap->arr[heap->size].key = key;
    heap->arr[heap->size].value = value;
    heap->size++;

    push_up(heap, heap->size - 1);
    return true;
}

/* Returns the minimum key-value pair */
bool min_max_heap_get_min(MinMaxHeap* heap, int* min_key, int* min_val)
{
    if (heap == NULL || heap->size == 0)
    {
        return false;
    }
    if (min_key)
        *min_key = heap->arr[0].key;
    if (min_val)
        *min_val = heap->arr[0].value;
    return true;
}

/* Returns the maximum key-value pair */
bool min_max_heap_get_max(MinMaxHeap* heap, int* max_key, int* max_val)
{
    if (heap == NULL || heap->size == 0)
    {
        return false;
    }
    if (heap->size == 1)
    {
        if (max_key)
            *max_key = heap->arr[0].key;
        if (max_val)
            *max_val = heap->arr[0].value;
        return true;
    }
    if (heap->size == 2)
    {
        if (max_key)
            *max_key = heap->arr[1].key;
        if (max_val)
            *max_val = heap->arr[1].value;
        return true;
    }

    /* Max is the larger of the two children of the root */
    int max_idx = (heap->arr[1].key > heap->arr[2].key) ? 1 : 2;
    if (max_key)
        *max_key = heap->arr[max_idx].key;
    if (max_val)
        *max_val = heap->arr[max_idx].value;
    return true;
}

/* Helper to find smallest children/grandchildren of node i */
static int find_min_descendant(MinMaxHeap* heap, int i)
{
    int left_child = 2 * i + 1;
    if (left_child >= heap->size)
    {
        return -1;
    }

    int candidates[6];
    int count = 0;

    candidates[count++] = left_child;
    if (left_child + 1 < heap->size)
    {
        candidates[count++] = left_child + 1;
    }

    /* Grandchildren */
    int gc_start = 2 * left_child + 1;
    for (int k = 0; k < 4 && gc_start + k < heap->size; k++)
    {
        candidates[count++] = gc_start + k;
    }

    int min_idx = candidates[0];
    for (int k = 1; k < count; k++)
    {
        if (heap->arr[candidates[k]].key < heap->arr[min_idx].key)
        {
            min_idx = candidates[k];
        }
    }
    return min_idx;
}

/* Helper to find largest children/grandchildren of node i */
static int find_max_descendant(MinMaxHeap* heap, int i)
{
    int left_child = 2 * i + 1;
    if (left_child >= heap->size)
    {
        return -1;
    }

    int candidates[6];
    int count = 0;

    candidates[count++] = left_child;
    if (left_child + 1 < heap->size)
    {
        candidates[count++] = left_child + 1;
    }

    /* Grandchildren */
    int gc_start = 2 * left_child + 1;
    for (int k = 0; k < 4 && gc_start + k < heap->size; k++)
    {
        candidates[count++] = gc_start + k;
    }

    int max_idx = candidates[0];
    for (int k = 1; k < count; k++)
    {
        if (heap->arr[candidates[k]].key > heap->arr[max_idx].key)
        {
            max_idx = candidates[k];
        }
    }
    return max_idx;
}

/* Push down helper functions to restore min-max properties on extraction */
static void push_down_min(MinMaxHeap* heap, int i)
{
    if (heap == NULL || i < 0 || i >= heap->size)
    {
        return;
    }
    int m = find_min_descendant(heap, i);
    if (m == -1)
    {
        return;
    }

    /* If m is a grandchild of i */
    if (m >= 4 * i + 3)
    {
        if (heap->arr[m].key < heap->arr[i].key)
        {
            swap_nodes(heap, i, m);
            int parent = (m - 1) / 2;
            if (heap->arr[m].key > heap->arr[parent].key)
            {
                swap_nodes(heap, m, parent);
            }
            push_down_min(heap, m);
        }
    }
    else
    {
        /* m is a child of i */
        if (heap->arr[m].key < heap->arr[i].key)
        {
            swap_nodes(heap, i, m);
        }
    }
}

static void push_down_max(MinMaxHeap* heap, int i)
{
    if (heap == NULL || i < 0 || i >= heap->size)
    {
        return;
    }
    int m = find_max_descendant(heap, i);
    if (m == -1)
    {
        return;
    }

    /* If m is a grandchild of i */
    if (m >= 4 * i + 3)
    {
        if (heap->arr[m].key > heap->arr[i].key)
        {
            swap_nodes(heap, i, m);
            int parent = (m - 1) / 2;
            if (heap->arr[m].key < heap->arr[parent].key)
            {
                swap_nodes(heap, m, parent);
            }
            push_down_max(heap, m);
        }
    }
    else
    {
        /* m is a child of i */
        if (heap->arr[m].key > heap->arr[i].key)
        {
            swap_nodes(heap, i, m);
        }
    }
}

static void push_down(MinMaxHeap* heap, int i)
{
    if (heap == NULL || i < 0 || i >= heap->size)
    {
        return;
    }
    if (is_min_level(i))
    {
        push_down_min(heap, i);
    }
    else
    {
        push_down_max(heap, i);
    }
}

/* Extracts the minimum element from the Min-Max Heap */
bool min_max_heap_extract_min(MinMaxHeap* heap, int* min_key, int* min_val)
{
    if (heap == NULL || heap->size == 0)
    {
        return false;
    }

    if (min_key)
        *min_key = heap->arr[0].key;
    if (min_val)
        *min_val = heap->arr[0].value;

    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;

    if (heap->size > 0)
    {
        push_down(heap, 0);
    }
    return true;
}

/* Extracts the maximum element from the Min-Max Heap */
bool min_max_heap_extract_max(MinMaxHeap* heap, int* max_key, int* max_val)
{
    if (heap == NULL || heap->size == 0)
    {
        return false;
    }

    if (heap->size == 1)
    {
        if (max_key)
            *max_key = heap->arr[0].key;
        if (max_val)
            *max_val = heap->arr[0].value;
        heap->size--;
        return true;
    }

    if (heap->size == 2)
    {
        if (max_key)
            *max_key = heap->arr[1].key;
        if (max_val)
            *max_val = heap->arr[1].value;
        heap->size--;
        return true;
    }

    int max_idx = (heap->arr[1].key > heap->arr[2].key) ? 1 : 2;

    if (max_key)
        *max_key = heap->arr[max_idx].key;
    if (max_val)
        *max_val = heap->arr[max_idx].value;

    heap->arr[max_idx] = heap->arr[heap->size - 1];
    heap->size--;

    if (max_idx < heap->size)
    {
        push_down(heap, max_idx);
    }
    return true;
}
