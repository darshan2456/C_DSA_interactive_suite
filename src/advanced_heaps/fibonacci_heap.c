#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* Allocates and initializes a new Fibonacci Heap node */
FibonacciNode* create_fibonacci_node(int key, int value)
{
    FibonacciNode* node = (FibonacciNode*)malloc(sizeof(FibonacciNode));
    if (node == NULL)
    {
        return NULL;
    }
    node->key = key;
    node->value = value;
    node->degree = 0;
    node->mark = false;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;  /* Point to itself to form a circular list */
    node->right = node; /* Point to itself to form a circular list */
    return node;
}

/* Helper to insert node x into a circular doubly linked list containing node min */
static void list_insert(FibonacciNode* min, FibonacciNode* x)
{
    x->left = min;
    x->right = min->right;
    min->right->left = x;
    min->right = x;
}

/* Helper to remove node x from a circular doubly linked list */
static void list_remove(FibonacciNode* x)
{
    x->left->right = x->right;
    x->right->left = x->left;
}

/* Inserts a new key-value pair into the Fibonacci Heap */
FibonacciNode* fib_heap_insert(FibonacciNode* min_node, int key, int value)
{
    FibonacciNode* node = create_fibonacci_node(key, value);
    if (node == NULL)
    {
        return min_node;
    }

    if (min_node == NULL)
    {
        return node;
    }

    list_insert(min_node, node);
    if (node->key < min_node->key)
    {
        min_node = node;
    }
    return min_node;
}

/* Combines two Fibonacci Heaps by merging their root lists */
FibonacciNode* fib_heap_union(FibonacciNode* min1, FibonacciNode* min2)
{
    if (min1 == NULL)
        return min2;
    if (min2 == NULL)
        return min1;

    /* Merge circular list of min2 into min1 */
    FibonacciNode* min1_next = min1->right;
    FibonacciNode* min2_prev = min2->left;

    min1->right = min2;
    min2->left = min1;
    min1_next->left = min2_prev;
    min2_prev->right = min1_next;

    /* Return the node with the smaller key */
    if (min1->key < min2->key)
    {
        return min1;
    }
    else
    {
        return min2;
    }
}

/* Links node y as a child of node x */
static void fib_heap_link(FibonacciNode* y, FibonacciNode* x)
{
    /* Remove y from the root list */
    list_remove(y);

    /* Make y a child of x */
    y->parent = x;
    if (x->child == NULL)
    {
        x->child = y;
        y->left = y;
        y->right = y;
    }
    else
    {
        list_insert(x->child, y);
    }
    x->degree++;
    y->mark = false;
}

/* Consolidates the root list by combining trees of equal degree */
static FibonacciNode* fib_heap_consolidate(FibonacciNode* min_node)
{
    if (min_node == NULL)
    {
        return NULL;
    }

    /* Max degree for any node in Fibonacci heap of size n is bounded by O(log n) */
    /* An array of size 64 is more than enough for up to 2^60 elements */
    FibonacciNode* arr[64];
    for (int i = 0; i < 64; i++)
    {
        arr[i] = NULL;
    }

    /* Count number of roots in the list to avoid infinite loops during updates */
    int num_roots = 0;
    FibonacciNode* x = min_node;
    if (x != NULL)
    {
        do
        {
            num_roots++;
            x = x->right;
        } while (x != min_node);
    }

    /* Allocate roots array to iterate safely */
    FibonacciNode** roots = (FibonacciNode**)malloc(num_roots * sizeof(FibonacciNode*));
    if (roots == NULL)
    {
        return min_node;
    }
    x = min_node;
    for (int i = 0; i < num_roots; i++)
    {
        roots[i] = x;
        x = x->right;
    }

    for (int i = 0; i < num_roots; i++)
    {
        x = roots[i];
        int d = x->degree;
        while (arr[d] != NULL)
        {
            FibonacciNode* y = arr[d];
            if (x->key > y->key)
            {
                /* Swap x and y */
                FibonacciNode* temp = x;
                x = y;
                y = temp;
            }
            fib_heap_link(y, x);
            arr[d] = NULL;
            d++;
        }
        arr[d] = x;
    }

    free(roots);

    /* Reconstruct the root list from the array of consolidated trees */
    FibonacciNode* new_min = NULL;
    for (int i = 0; i < 64; i++)
    {
        if (arr[i] != NULL)
        {
            arr[i]->left = arr[i];
            arr[i]->right = arr[i];
            if (new_min == NULL)
            {
                new_min = arr[i];
            }
            else
            {
                list_insert(new_min, arr[i]);
                if (arr[i]->key < new_min->key)
                {
                    new_min = arr[i];
                }
            }
        }
    }

    return new_min;
}

/* Extracts the minimum element and consolidates the heap */
FibonacciNode* fib_heap_extract_min(FibonacciNode* min_node, int* min_key, int* min_val)
{
    FibonacciNode* z = min_node;
    if (z == NULL)
    {
        return NULL;
    }

    if (min_key)
        *min_key = z->key;
    if (min_val)
        *min_val = z->value;

    /* Add all children of z to the root list of the heap */
    if (z->child != NULL)
    {
        FibonacciNode* child = z->child;
        FibonacciNode* curr = child;
        /* First, count children */
        int count = 0;
        do
        {
            count++;
            curr = curr->right;
        } while (curr != child);

        /* Collect children pointers to update parent references safely */
        FibonacciNode** children_arr = (FibonacciNode**)malloc(count * sizeof(FibonacciNode*));
        if (children_arr == NULL)
        {
            return min_node;
        }
        curr = child;
        for (int i = 0; i < count; i++)
        {
            children_arr[i] = curr;
            curr = curr->right;
        }

        for (int i = 0; i < count; i++)
        {
            FibonacciNode* c = children_arr[i];
            list_insert(z, c);
            c->parent = NULL;
        }
        free(children_arr);
    }

    /* Remove z from the root list */
    if (z->right == z)
    {
        /* z was the only node in the root list */
        free(z);
        return NULL;
    }

    FibonacciNode* next_min = z->right;
    list_remove(z);
    free(z);

    /* Consolidate the root list */
    return fib_heap_consolidate(next_min);
}

/* Recursively frees a tree in the Fibonacci Heap */
static void free_fib_tree(FibonacciNode* node)
{
    if (node == NULL)
    {
        return;
    }

    if (node->child != NULL)
    {
        FibonacciNode* curr = node->child;
        curr->left->right = NULL; /* Break circular list */
        while (curr != NULL)
        {
            FibonacciNode* next = curr->right;
            free_fib_tree(curr);
            curr = next;
        }
    }
    free(node);
}

/* Destroys the entire Fibonacci Heap */
void destroy_fibonacci_heap(FibonacciNode* min_node)
{
    if (min_node == NULL)
    {
        return;
    }

    min_node->left->right = NULL; /* Break circular list */
    FibonacciNode* curr = min_node;
    while (curr != NULL)
    {
        FibonacciNode* next = curr->right;
        free_fib_tree(curr);
        curr = next;
    }
}

/* Cuts node x from parent y and adds x to the root list */
static FibonacciNode* fib_heap_cut(FibonacciNode* min_node, FibonacciNode* x, FibonacciNode* y)
{
    /* Remove x from y's child list */
    if (x->right == x)
    {
        y->child = NULL;
    }
    else
    {
        if (y->child == x)
        {
            y->child = x->right;
        }
        list_remove(x);
    }
    y->degree--;

    /* Add x to the root list */
    list_insert(min_node, x);
    x->parent = NULL;
    x->mark = false;

    return min_node;
}

/* Performs cascading cut on node y */
static FibonacciNode* fib_heap_cascading_cut(FibonacciNode* min_node, FibonacciNode* y)
{
    FibonacciNode* z = y->parent;
    if (z != NULL)
    {
        if (y->mark == false)
        {
            y->mark = true;
        }
        else
        {
            /* Cut y from z and repeat cascading cut on z */
            min_node = fib_heap_cut(min_node, y, z);
            min_node = fib_heap_cascading_cut(min_node, z);
        }
    }
    return min_node;
}

/* Decreases the key of target node in the Fibonacci Heap */
FibonacciNode* fib_heap_decrease_key(FibonacciNode* min_node, FibonacciNode* target, int new_key)
{
    if (target == NULL || min_node == NULL || new_key > target->key)
    {
        return min_node;
    }

    target->key = new_key;
    FibonacciNode* y = target->parent;

    if (y != NULL && target->key < y->key)
    {
        min_node = fib_heap_cut(min_node, target, y);
        min_node = fib_heap_cascading_cut(min_node, y);
    }

    if (target->key < min_node->key)
    {
        min_node = target;
    }

    return min_node;
}

/* Deletes a node from the Fibonacci Heap */
FibonacciNode* fib_heap_delete(FibonacciNode* min_node, FibonacciNode* target)
{
    if (min_node == NULL || target == NULL)
    {
        return min_node;
    }

    /* Decrease key to negative infinity */
    min_node = fib_heap_decrease_key(min_node, target, -2147483647 - 1);

    /* Extract the minimum element (which is now target) */
    return fib_heap_extract_min(min_node, NULL, NULL);
}

/* Recursively searches for a node with the given key starting from a root/sibling list */
FibonacciNode* fib_heap_find_node(FibonacciNode* min_node, int key)
{
    if (min_node == NULL)
    {
        return NULL;
    }

    FibonacciNode* curr = min_node;
    FibonacciNode* start = min_node;

    do
    {
        if (curr->key == key)
        {
            return curr;
        }

        /* Search children of current node */
        FibonacciNode* res = fib_heap_find_node(curr->child, key);
        if (res != NULL)
        {
            return res;
        }

        curr = curr->right;
    } while (curr != start);

    return NULL;
}

/* =========================================================================
 *                         Fibonacci Heap Demo & Visualizer
 * ========================================================================= */

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
