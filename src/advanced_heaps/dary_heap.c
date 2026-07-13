#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

/* Helper function to swap two nodes in the d-ary heap array */
static void swap_nodes(DAryHeap* heap, int i, int j)
{
    DAryHeapNode temp = heap->arr[i];
    heap->arr[i] = heap->arr[j];
    heap->arr[j] = temp;
}

/* Maintain heap property by moving a node up the tree if its key is smaller than its parent */
static void bubble_up(DAryHeap* heap, int i)
{
    while (i > 0)
    {
        int p = (i - 1) / heap->d;
        if (heap->arr[i].key < heap->arr[p].key)
        {
            swap_nodes(heap, i, p);
            i = p;
        }
        else
        {
            break;
        }
    }
}

/* Maintain heap property by moving a node down the tree, replacing it with its smallest child */
static void bubble_down(DAryHeap* heap, int i)
{
    int d = heap->d;
    int size = heap->size;
    while (1)
    {
        int best = i;
        int first_child = d * i + 1;

        for (int k = 0; k < d; k++)
        {
            int child = first_child + k;
            if (child < size)
            {
                if (heap->arr[child].key < heap->arr[best].key)
                {
                    best = child;
                }
            }
            else
            {
                break;
            }
        }

        if (best != i)
        {
            swap_nodes(heap, i, best);
            i = best;
        }
        else
        {
            break;
        }
    }
}

/* Initialize a new d-Ary heap with specified capacity and branching factor d */
DAryHeap* create_dary_heap(int capacity, int d)
{
    if (capacity <= 0 || d < 2)
    {
        return NULL;
    }

    DAryHeap* heap = (DAryHeap*)malloc(sizeof(DAryHeap));
    if (heap == NULL)
    {
        return NULL;
    }

    heap->arr = (DAryHeapNode*)malloc(sizeof(DAryHeapNode) * capacity);
    if (heap->arr == NULL)
    {
        free(heap);
        return NULL;
    }

    heap->capacity = capacity;
    heap->size = 0;
    heap->d = d;
    return heap;
}

/* Clean up and free all memory allocated for the d-Ary heap */
void destroy_dary_heap(DAryHeap* heap)
{
    if (heap != NULL)
    {
        free(heap->arr);
        free(heap);
    }
}

/* Insert a new key-value pair into the d-Ary heap */
bool dary_heap_insert(DAryHeap* heap, int key, int value)
{
    if (heap == NULL || heap->size >= heap->capacity)
    {
        return false;
    }

    int index = heap->size;
    heap->arr[index].key = key;
    heap->arr[index].value = value;
    heap->size++;

    bubble_up(heap, index);
    return true;
}

/* Extract and return the minimum element (root) from the heap */
bool dary_heap_extract_min(DAryHeap* heap, int* min_key, int* min_val)
{
    if (heap == NULL || heap->size == 0)
    {
        return false;
    }

    if (min_key != NULL)
        *min_key = heap->arr[0].key;
    if (min_val != NULL)
        *min_val = heap->arr[0].value;

    heap->size--;
    if (heap->size > 0)
    {
        heap->arr[0] = heap->arr[heap->size];
        bubble_down(heap, 0);
    }

    return true;
}

/* Decrease the key of the node at specified index to a new, smaller key */
bool dary_heap_decrease_key(DAryHeap* heap, int index, int new_key)
{
    if (heap == NULL || index < 0 || index >= heap->size || new_key > heap->arr[index].key)
    {
        return false;
    }

    heap->arr[index].key = new_key;
    bubble_up(heap, index);
    return true;
}

/* Retrieve the minimum element without removing it from the heap */
bool dary_heap_get_min(DAryHeap* heap, int* min_key, int* min_val)
{
    if (heap == NULL || heap->size == 0)
    {
        return false;
    }

    if (min_key != NULL)
        *min_key = heap->arr[0].key;
    if (min_val != NULL)
        *min_val = heap->arr[0].value;

    return true;
}

/* Search for a node with the given key and return its index, or -1 if not found */
int dary_heap_find_index(DAryHeap* heap, int key)
{
    if (heap == NULL)
    {
        return -1;
    }

    for (int i = 0; i < heap->size; i++)
    {
        if (heap->arr[i].key == key)
        {
            return i;
        }
    }
    return -1;
}

/* Search for a node with the given value and return its index, or -1 if not found */
int dary_heap_find_by_value(DAryHeap* heap, int value)
{
    if (heap == NULL)
    {
        return -1;
    }

    for (int i = 0; i < heap->size; i++)
    {
        if (heap->arr[i].value == value)
        {
            return i;
        }
    }
    return -1;
}

/* =========================================================================
 *                         d-Ary Heap Demo & Visualizer
 * ========================================================================= */

static void print_dary_heap_node(DAryHeap* heap, int i, int indent)
{
    if (i >= heap->size)
        return;
    int d = heap->d;
    int first_child = d * i + 1;
    for (int k = d - 1; k >= 0; k--)
    {
        print_dary_heap_node(heap, first_child + k, indent + 6);
    }
    for (int j = 0; j < indent; j++)
        printf(" ");
    printf("[%d : %d]\n", heap->arr[i].key, heap->arr[i].value);
}

void run_dary_demo(void)
{
    int d;
    if (safe_input_int(&d, "\nEnter branching factor d (d >= 2): ", 2, 100) != 1)
        return;

    DAryHeap* heap = create_dary_heap(50, d);
    if (heap == NULL)
    {
        printf("\nError: Failed to create d-Ary Heap (allocation failed)\n");
        return;
    }
    while (1)
    {
        display_header("d-Ary Heap Interactive Demo");
        if (heap->size == 0)
        {
            printf("\nHeap is empty.\n");
        }
        else
        {
            printf("\n%d-Ary Heap Structure:\n", d);
            print_dary_heap_node(heap, 0, 0);
        }
        printf("\n1. Insert node\n"
               "2. Extract min node\n"
               "3. Decrease key\n"
               "4. Exit d-ary heap demo\n");

        int choice;
        if (safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 4) != 1)
            break;
        if (choice == 4)
            break;

        if (choice == 1)
        {
            int key, val;
            if (safe_input_int(&key, "Enter key: ", -100000, 100000) != 1)
                continue;
            if (safe_input_int(&val, "Enter value: ", -100000, 100000) != 1)
                continue;
            if (!dary_heap_insert(heap, key, val))
            {
                printf("\nFailed to insert node!\n");
                printf("\nPress Enter to continue...");
                getchar();
            }
        }
        else if (choice == 2)
        {
            int key, val;
            if (dary_heap_extract_min(heap, &key, &val))
            {
                printf("\nExtracted Min Node -> Key: %d, Value: %d\n", key, val);
            }
            else
            {
                printf("\nHeap is empty!\n");
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
        else if (choice == 3)
        {
            int target_key, new_key;
            if (safe_input_int(&target_key, "Enter key to search for: ", -100000, 100000) != 1)
                continue;
            int idx = dary_heap_find_index(heap, target_key);
            if (idx == -1)
            {
                printf("\nKey not found!\n");
                printf("\nPress Enter to continue...");
                getchar();
                continue;
            }
            if (safe_input_int(&new_key, "Enter new key (must be smaller): ", -100000, 100000) != 1)
                continue;
            if (!dary_heap_decrease_key(heap, idx, new_key))
            {
                printf("\nFailed to decrease key!\n");
                printf("\nPress Enter to continue...");
                getchar();
            }
        }
    }
    destroy_dary_heap(heap);
}
