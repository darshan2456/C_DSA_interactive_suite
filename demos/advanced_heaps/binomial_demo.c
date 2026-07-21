#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

static void print_binomial_tree(BinomialNode* node, int indent)
{
    if (node == NULL)
        return;
    for (int j = 0; j < indent; j++)
        printf(" ");
    printf("|- [%d : %d] (deg: %d)\n", node->key, node->value, node->degree);

    BinomialNode* child = node->child;
    while (child != NULL)
    {
        print_binomial_tree(child, indent + 4);
        child = child->sibling;
    }
}

static void display_binomial_heap(BinomialNode* head)
{
    if (head == NULL)
    {
        printf("\nBinomial Heap is empty.\n");
        return;
    }
    printf("\nBinomial Heap Structure:\n");
    BinomialNode* curr = head;
    int tree_num = 1;
    while (curr != NULL)
    {
        printf("Tree %d (Root Degree: %d):\n", tree_num++, curr->degree);
        print_binomial_tree(curr, 2);
        curr = curr->sibling;
    }
}

void run_binomial_demo(void)
{
    BinomialNode* heap = NULL;
    while (1)
    {
        display_header("Binomial Heap Interactive Demo");
        display_binomial_heap(heap);
        printf("\n1. Insert node\n"
               "2. Extract min node\n"
               "3. Decrease key\n"
               "4. Exit binomial heap demo\n");

        int choice;
        if (safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 4) != 1)
            break;
        if (choice == 4)
            break;

        if (choice == 1)
        {
            int key, val;
            if (safe_input_int(&key, "Enter key (integer): ", -100000, 100000) != 1)
                continue;
            if (safe_input_int(&val, "Enter value (integer): ", -100000, 100000) != 1)
                continue;
            heap = binomial_heap_insert(heap, key, val);
        }
        else if (choice == 2)
        {
            if (heap == NULL)
            {
                printf("\nHeap is already empty!\n");
            }
            else
            {
                int key, val;
                heap = binomial_heap_extract_min(heap, &key, &val);
                printf("\nExtracted Min Node -> Key: %d, Value: %d\n", key, val);
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
        else if (choice == 3)
        {
            int target_key, new_key;
            if (safe_input_int(&target_key, "Enter current key to search for: ", -100000, 100000) !=
                1)
                continue;
            BinomialNode* target = binomial_heap_find_node(heap, target_key);
            if (target == NULL)
            {
                printf("\nKey not found in the heap!\n");
                printf("\nPress Enter to continue...");
                getchar();
                continue;
            }
            if (safe_input_int(&new_key, "Enter new key (must be smaller): ", -100000, 100000) != 1)
                continue;
            if (new_key > target->key)
            {
                printf("\nError: New key is larger than current key!\n");
                printf("\nPress Enter to continue...");
                getchar();
                continue;
            }
            heap = binomial_heap_decrease_key(heap, target, new_key);
        }
    }
    destroy_binomial_heap(heap);
}
