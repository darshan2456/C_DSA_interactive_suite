#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void print_fibonacci_tree(FibonacciNode* node, int indent)
{
    if (node == NULL)
        return;
    FibonacciNode* curr = node;
    do
    {
        for (int j = 0; j < indent; j++)
            printf(" ");
        printf("|- [%d : %d] (deg: %d, mark: %s)\n", curr->key, curr->value, curr->degree,
               curr->mark ? "true" : "false");
        if (curr->child != NULL)
        {
            print_fibonacci_tree(curr->child, indent + 4);
        }
        curr = curr->right;
    } while (curr != node);
}

static void display_fibonacci_heap(FibonacciNode* min_node)
{
    if (min_node == NULL)
    {
        printf("\nFibonacci Heap is empty.\n");
        return;
    }
    printf("\nFibonacci Heap Structure (min_node marked with *):\n");
    FibonacciNode* curr = min_node;
    do
    {
        printf("%s |- [%d : %d] (deg: %d)\n", (curr == min_node) ? " *" : "  ", curr->key,
               curr->value, curr->degree);
        if (curr->child != NULL)
        {
            print_fibonacci_tree(curr->child, 6);
        }
        curr = curr->right;
    } while (curr != min_node);
}

void run_fibonacci_demo(void)
{
    FibonacciNode* heap = NULL;
    while (1)
    {
        display_header("Fibonacci Heap Interactive Demo");
        display_fibonacci_heap(heap);
        printf("\n1. Insert node\n"
               "2. Extract min node\n"
               "3. Decrease key\n"
               "4. Delete node\n"
               "5. Exit fibonacci heap demo\n");

        int choice;
        if (safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 5) != 1)
            break;
        if (choice == 5)
            break;

        if (choice == 1)
        {
            int key, val;
            if (safe_input_int(&key, "Enter key: ", -100000, 100000) != 1)
                continue;
            if (safe_input_int(&val, "Enter value: ", -100000, 100000) != 1)
                continue;
            heap = fib_heap_insert(heap, key, val);
        }
        else if (choice == 2)
        {
            if (heap == NULL)
            {
                printf("\nHeap is empty!\n");
            }
            else
            {
                int key, val;
                heap = fib_heap_extract_min(heap, &key, &val);
                printf("\nExtracted Min Node -> Key: %d, Value: %d\n", key, val);
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
        else if (choice == 3)
        {
            int target_key, new_key;
            if (safe_input_int(&target_key, "Enter key to search for: ", -100000, 100000) != 1)
                continue;
            FibonacciNode* target = fib_heap_find_node(heap, target_key);
            if (target == NULL)
            {
                printf("\nKey not found!\n");
                printf("\nPress Enter to continue...");
                getchar();
                continue;
            }
            if (safe_input_int(&new_key, "Enter new key (must be smaller): ", -100000, 100000) != 1)
                continue;
            if (new_key > target->key)
            {
                printf("\nError: New key is larger!\n");
                printf("\nPress Enter to continue...");
                getchar();
                continue;
            }
            heap = fib_heap_decrease_key(heap, target, new_key);
        }
        else if (choice == 4)
        {
            int target_key;
            if (safe_input_int(&target_key, "Enter key to delete: ", -100000, 100000) != 1)
                continue;
            FibonacciNode* target = fib_heap_find_node(heap, target_key);
            if (target == NULL)
            {
                printf("\nKey not found!\n");
                printf("\nPress Enter to continue...");
                getchar();
                continue;
            }
            heap = fib_heap_delete(heap, target);
            printf("\nNode deleted.\n");
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
    destroy_fibonacci_heap(heap);
}
