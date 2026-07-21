#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

/* Creates a single binomial node with the given key and value */
BinomialNode* create_binomial_node(int key, int value)
{
    BinomialNode* node = (BinomialNode*)malloc(sizeof(BinomialNode));
    if (node == NULL)
    {
        return NULL;
    }
    node->key = key;
    node->value = value;
    node->degree = 0;
    node->parent = NULL;
    node->child = NULL;
    node->sibling = NULL;
    return node;
}

/* Frees all memory allocated for the binomial heap */
void destroy_binomial_heap(BinomialNode* head)
{
    if (head == NULL)
    {
        return;
    }
    destroy_binomial_heap(head->sibling);
    destroy_binomial_heap(head->child);
    free(head);
}

/* Links tree y as a child of tree z (y gets degree z incremented) */
static void binomial_link(BinomialNode* y, BinomialNode* z)
{
    if (y == NULL || z == NULL)
    {
        return;
    }
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

/* Merges two binomial heaps root lists sorted by degree ascending */
static BinomialNode* binomial_heap_merge_lists(BinomialNode* head1, BinomialNode* head2)
{
    if (head1 == NULL)
        return head2;
    if (head2 == NULL)
        return head1;

    BinomialNode* head = NULL;
    BinomialNode** tail_ref = &head;

    while (head1 != NULL && head2 != NULL)
    {
        if (head1->degree <= head2->degree)
        {
            *tail_ref = head1;
            head1 = head1->sibling;
        }
        else
        {
            *tail_ref = head2;
            head2 = head2->sibling;
        }
        tail_ref = &((*tail_ref)->sibling);
    }

    if (head1 != NULL)
    {
        *tail_ref = head1;
    }
    else
    {
        *tail_ref = head2;
    }

    return head;
}

/* Combines two binomial heaps and combines trees of equal degree */
BinomialNode* binomial_heap_merge(BinomialNode* head1, BinomialNode* head2)
{
    BinomialNode* head = binomial_heap_merge_lists(head1, head2);
    if (head == NULL)
    {
        return NULL;
    }

    BinomialNode* prev = NULL;
    BinomialNode* curr = head;
    BinomialNode* next = curr->sibling;

    while (next != NULL)
    {
        /* Case 1: current and next have different degrees, or there is a third
         * tree of the same degree */
        if ((curr->degree != next->degree) ||
            (next->sibling != NULL && next->sibling->degree == curr->degree))
        {
            prev = curr;
            curr = next;
        }
        /* Case 2: current has smaller key, so link next under current */
        else if (curr->key <= next->key)
        {
            curr->sibling = next->sibling;
            binomial_link(next, curr);
        }
        /* Case 3: next has smaller key, so link current under next */
        else
        {
            if (prev == NULL)
            {
                head = next;
            }
            else
            {
                prev->sibling = next;
            }
            binomial_link(curr, next);
            curr = next;
        }
        next = curr->sibling;
    }

    return head;
}

/* Inserts a key-value pair into the binomial heap */
BinomialNode* binomial_heap_insert(BinomialNode* head, int key, int value)
{
    BinomialNode* new_node = create_binomial_node(key, value);
    if (new_node == NULL)
    {
        return head;
    }
    return binomial_heap_merge(head, new_node);
}

/* Reverses a linked list of binomial nodes */
static BinomialNode* reverse_list(BinomialNode* head)
{
    BinomialNode* prev = NULL;
    BinomialNode* curr = head;
    BinomialNode* next = NULL;

    while (curr != NULL)
    {
        next = curr->sibling;
        curr->sibling = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

/* Extracts the minimum element from the binomial heap */
BinomialNode* binomial_heap_extract_min(BinomialNode* head, int* min_key, int* min_val)
{
    if (head == NULL)
    {
        return NULL;
    }

    /* Find the node with the minimum key in the root list */
    BinomialNode* min_node = head;
    BinomialNode* min_prev = NULL;
    BinomialNode* curr = head;
    BinomialNode* prev = NULL;

    while (curr != NULL)
    {
        if (curr->key < min_node->key)
        {
            min_node = curr;
            min_prev = prev;
        }
        prev = curr;
        curr = curr->sibling;
    }

    /* Unlink the minimum node from the root list */
    if (min_prev == NULL)
    {
        head = min_node->sibling;
    }
    else
    {
        min_prev->sibling = min_node->sibling;
    }

    if (min_key)
        *min_key = min_node->key;
    if (min_val)
        *min_val = min_node->value;

    /* Reverse the children of the extracted min node */
    BinomialNode* child_head = min_node->child;
    /* Clear parent pointers of children */
    curr = child_head;
    while (curr != NULL)
    {
        curr->parent = NULL;
        curr = curr->sibling;
    }

    BinomialNode* reversed_children = reverse_list(child_head);

    free(min_node);

    /* Merge the remaining root list with the reversed children */
    return binomial_heap_merge(head, reversed_children);
}

/* Decreases the key of a given target node in the binomial heap */
BinomialNode* binomial_heap_decrease_key(BinomialNode* head, BinomialNode* target, int new_key)
{
    if (target == NULL || new_key > target->key)
    {
        return head; /* Cannot increase key */
    }

    target->key = new_key;
    BinomialNode* curr = target;
    BinomialNode* parent_node = curr->parent;

    /* Bubble up to restore heap order */
    while (parent_node != NULL && curr->key < parent_node->key)
    {
        /* Swap keys and values */
        int temp_key = curr->key;
        int temp_val = curr->value;
        curr->key = parent_node->key;
        curr->value = parent_node->value;
        parent_node->key = temp_key;
        parent_node->value = temp_val;

        curr = parent_node;
        parent_node = curr->parent;
    }

    return head;
}

/* Recursively searches the heap for a node with the given key */
BinomialNode* binomial_heap_find_node(BinomialNode* head, int key)
{
    if (head == NULL)
    {
        return NULL;
    }

    if (head->key == key)
    {
        return head;
    }

    /* Search the child subtree */
    BinomialNode* res = binomial_heap_find_node(head->child, key);
    if (res != NULL)
    {
        return res;
    }

    /* Search the sibling list */
    return binomial_heap_find_node(head->sibling, key);
}
