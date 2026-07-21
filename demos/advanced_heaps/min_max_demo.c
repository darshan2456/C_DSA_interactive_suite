#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

static void print_min_max_heap_node(MinMaxHeap* heap, int i, int indent)
{
    if (i >= heap->size)
        return;
    print_min_max_heap_node(heap, 2 * i + 2, indent + 6);
    for (int j = 0; j < indent; j++)
        printf(" ");
    printf("[%d : %d]\n", heap->arr[i].key, heap->arr[i].value);
    print_min_max_heap_node(heap, 2 * i + 1, indent + 6);
}

void run_min_max_demo(void)
{
    MinMaxHeap* heap = create_min_max_heap(50);
    if (heap == NULL)
    {
        printf("\nError: Failed to create Min-Max Heap (allocation failed)\n");
        return;
    }
    while (1)
    {
        display_header("Min-Max Heap (Double-Ended Queue) Demo");
        if (heap->size == 0)
        {
            printf("\nHeap is empty.\n");
        }
        else
        {
            printf("\nMin-Max Heap Structure:\n");
            print_min_max_heap_node(heap, 0, 0);
        }
        printf("\n1. Insert node\n"
               "2. Get min node\n"
               "3. Get max node\n"
               "4. Extract min node\n"
               "5. Extract max node\n"
               "6. Exit min-max heap demo\n");

        int choice;
        if (safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 6) != 1)
            break;
        if (choice == 6)
            break;

        if (choice == 1)
        {
            int key, val;
            if (safe_input_int(&key, "Enter key: ", -100000, 100000) != 1)
                continue;
            if (safe_input_int(&val, "Enter value: ", -100000, 100000) != 1)
                continue;
            if (!min_max_heap_insert(heap, key, val))
            {
                printf("\nFailed to insert node (heap full)!\n");
                printf("\nPress Enter to continue...");
                getchar();
            }
        }
        else if (choice == 2)
        {
            int key, val;
            if (min_max_heap_get_min(heap, &key, &val))
            {
                printf("\nMin Node -> Key: %d, Value: %d\n", key, val);
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
            int key, val;
            if (min_max_heap_get_max(heap, &key, &val))
            {
                printf("\nMax Node -> Key: %d, Value: %d\n", key, val);
            }
            else
            {
                printf("\nHeap is empty!\n");
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
        else if (choice == 4)
        {
            int key, val;
            if (min_max_heap_extract_min(heap, &key, &val))
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
        else if (choice == 5)
        {
            int key, val;
            if (min_max_heap_extract_max(heap, &key, &val))
            {
                printf("\nExtracted Max Node -> Key: %d, Value: %d\n", key, val);
            }
            else
            {
                printf("\nHeap is empty!\n");
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
    destroy_min_max_heap(heap);
}
