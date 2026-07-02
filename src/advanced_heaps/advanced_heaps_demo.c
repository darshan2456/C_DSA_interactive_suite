#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

/* =========================================================================
 *                         ASCII Visualizer Printers
 * ========================================================================= */

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

static void print_treap_tree(TreapNode* root, int indent)
{
    if (root == NULL)
        return;
    print_treap_tree(root->right, indent + 8);
    for (int j = 0; j < indent; j++)
        printf(" ");
    printf("[%d:%d (prio: %d)]\n", root->key, root->value, root->priority);
    print_treap_tree(root->left, indent + 8);
}

/* =========================================================================
 *                         Sub-Demo Logic Functions
 * ========================================================================= */

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
        if (safe_input_int(&choice, "\nEnter choice: ", 1, 4) != 1)
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
            int key, val;
            heap = binomial_heap_extract_min(heap, &key, &val);
            if (key == -1 && val == -1 && heap == NULL)
            {
                printf("\nHeap is already empty!\n");
            }
            else
            {
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
        if (safe_input_int(&choice, "\nEnter choice: ", 1, 5) != 1)
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
            int key, val;
            heap = fib_heap_extract_min(heap, &key, &val);
            if (key == -1 && val == -1 && heap == NULL)
            {
                printf("\nHeap is empty!\n");
            }
            else
            {
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
        if (safe_input_int(&choice, "\nEnter choice: ", 1, 3) != 1)
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
            int key, val;
            heap = leftist_heap_extract_min(heap, &key, &val);
            if (key == -1 && val == -1 && heap == NULL)
            {
                printf("\nHeap is empty!\n");
            }
            else
            {
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
        if (safe_input_int(&choice, "\nEnter choice: ", 1, 3) != 1)
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
            int key, val;
            heap = skew_heap_extract_min(heap, &key, &val);
            if (key == -1 && val == -1 && heap == NULL)
            {
                printf("\nHeap is empty!\n");
            }
            else
            {
                printf("\nExtracted Min Node -> Key: %d, Value: %d\n", key, val);
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
    destroy_skew_heap(heap);
}

void run_min_max_demo(void)
{
    MinMaxHeap* heap = create_min_max_heap(50);
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
        if (safe_input_int(&choice, "\nEnter choice: ", 1, 6) != 1)
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

void run_dary_demo(void)
{
    int d;
    if (safe_input_int(&d, "\nEnter branching factor d (d >= 2): ", 2, 100) != 1)
        return;

    DAryHeap* heap = create_dary_heap(50, d);
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
        if (safe_input_int(&choice, "\nEnter choice: ", 1, 4) != 1)
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

void run_treap_demo(void)
{
    TreapNode* root = NULL;
    while (1)
    {
        display_header("Treap (Tree + Heap) Interactive Demo");
        if (root == NULL)
        {
            printf("\nTreap is empty.\n");
        }
        else
        {
            printf("\nTreap Structure [Key:Value (Priority)]:\n");
            print_treap_tree(root, 0);
        }
        printf("\n1. Insert node\n"
               "2. Delete node\n"
               "3. Get min key node\n"
               "4. Exit treap demo\n");

        int choice;
        if (safe_input_int(&choice, "\nEnter choice: ", 1, 4) != 1)
            break;
        if (choice == 4)
            break;

        if (choice == 1)
        {
            int key, val, prio;
            if (safe_input_int(&key, "Enter key (BST order): ", -100000, 100000) != 1)
                continue;
            if (safe_input_int(&val, "Enter value: ", -100000, 100000) != 1)
                continue;
            if (safe_input_int(&prio, "Enter priority (Heap order): ", -100000, 100000) != 1)
                continue;
            root = treap_insert(root, key, val, prio);
        }
        else if (choice == 2)
        {
            int key;
            if (safe_input_int(&key, "Enter key to delete: ", -100000, 100000) != 1)
                continue;
            root = treap_delete(root, key);
            printf("\nNode deleted.\n");
            printf("\nPress Enter to continue...");
            getchar();
        }
        else if (choice == 3)
        {
            TreapNode* min_node = treap_get_min(root);
            if (min_node != NULL)
            {
                printf("\nMin Key Node -> Key: %d, Value: %d, Priority: %d\n", min_node->key,
                       min_node->value, min_node->priority);
            }
            else
            {
                printf("\nTreap is empty!\n");
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
    destroy_treap(root);
}

/* =========================================================================
 *                         Main Demo Coordinator
 * ========================================================================= */

void advanced_heaps_demo(void)
{
    while (1)
    {
        display_header("Advanced Heaps & Priority Queues Suite");

        int choice;
        int status = safe_input_int(&choice,
                                    "\nSelect Heap Type:\n"
                                    "-----------------\n"
                                    "1. Binomial Heap\n"
                                    "2. Fibonacci Heap\n"
                                    "3. Leftist Heap\n"
                                    "4. Skew Heap\n"
                                    "5. Min-Max Heap (Double-Ended Priority Queue)\n"
                                    "6. d-Ary Heap\n"
                                    "7. Treap (Tree + Heap)\n"
                                    "\nEnter choice (-1 to return): ",
                                    1, 7);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nReturning to main menu...\n");
            return;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                run_binomial_demo();
                break;
            case 2:
                run_fibonacci_demo();
                break;
            case 3:
                run_leftist_demo();
                break;
            case 4:
                run_skew_demo();
                break;
            case 5:
                run_min_max_demo();
                break;
            case 6:
                run_dary_demo();
                break;
            case 7:
                run_treap_demo();
                break;
        }
    }
}
