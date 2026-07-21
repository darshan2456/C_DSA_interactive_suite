#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

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
