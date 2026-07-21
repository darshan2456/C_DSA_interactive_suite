#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

static void print_leftist_tree(LeftistNode* root, int indent)
{
    if (root == NULL)
        return;
    print_leftist_tree(root->right, indent + 6);
    for (int j = 0; j < indent; j++)
        printf(" ");
    printf("[%d : %d] (npl: %d)\n", root->key, root->value, root->npl);
    print_leftist_tree(root->left, indent + 6);
}

static void print_skew_tree(SkewNode* root, int indent)
{
    if (root == NULL)
        return;
    print_skew_tree(root->right, indent + 6);
    for (int j = 0; j < indent; j++)
        printf(" ");
    printf("[%d : %d]\n", root->key, root->value);
    print_skew_tree(root->left, indent + 6);
}

void run_leftist_demo(void)
{
    LeftistNode* heap = NULL;
    while (1)
    {
        display_header("Leftist Heap Interactive Demo");
        if (heap == NULL)
        {
            printf("\nLeftist Heap is empty.\n");
        }
        else
        {
            printf("\nLeftist Heap Structure:\n");
            print_leftist_tree(heap, 0);
        }
        printf("\n1. Insert node\n"
               "2. Extract min node\n"
               "3. Exit leftist heap demo\n");

        int choice;
        if (safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 3) != 1)
            break;
        if (choice == 3)
            break;

        if (choice == 1)
        {
            int key, val;
            if (safe_input_int(&key, "Enter key: ", -100000, 100000) != 1)
                continue;
            if (safe_input_int(&val, "Enter value: ", -100000, 100000) != 1)
                continue;
            heap = leftist_heap_insert(heap, key, val);
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
                heap = leftist_heap_extract_min(heap, &key, &val);
                printf("\nExtracted Min Node -> Key: %d, Value: %d\n", key, val);
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
    destroy_leftist_heap(heap);
}

void run_skew_demo(void)
{
    SkewNode* heap = NULL;
    while (1)
    {
        display_header("Skew Heap Interactive Demo");
        if (heap == NULL)
        {
            printf("\nSkew Heap is empty.\n");
        }
        else
        {
            printf("\nSkew Heap Structure:\n");
            print_skew_tree(heap, 0);
        }
        printf("\n1. Insert node\n"
               "2. Extract min node\n"
               "3. Exit skew heap demo\n");

        int choice;
        if (safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 3) != 1)
            break;
        if (choice == 3)
            break;

        if (choice == 1)
        {
            int key, val;
            if (safe_input_int(&key, "Enter key: ", -100000, 100000) != 1)
                continue;
            if (safe_input_int(&val, "Enter value: ", -100000, 100000) != 1)
                continue;
            heap = skew_heap_insert(heap, key, val);
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
                heap = skew_heap_extract_min(heap, &key, &val);
                printf("\nExtracted Min Node -> Key: %d, Value: %d\n", key, val);
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
    destroy_skew_heap(heap);
}
