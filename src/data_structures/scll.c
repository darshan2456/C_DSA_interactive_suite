#include "scll.h"
#include <stdio.h>
#include <stdlib.h>

// Singly circular linked list. See data_structures.h for the head/tail/length invariant.
//
// A note on traversal: because the list is circular there is no NULL terminator to stop on.
// Every traversal here therefore walks at most `length` nodes, using the idiom
//     cur = head; do { ...; cur = cur->next; } while (cur != head);
// which visits each node exactly once and is guaranteed to terminate (it stops the moment it
// arrives back at head). This is what prevents the infinite loops a circular list invites.

// Allocates a node holding `value`. Returns NULL on malloc failure. The caller is responsible
// for linking it into the ring (next is left as NULL on purpose).
static scll_Node* scll_create_node(int value)
{
    scll_Node* node = malloc(sizeof(scll_Node));
    if (node == NULL)
    {
        return NULL;
    }
    node->data = value;
    node->next = NULL;
    return node;
}

void scll_init(scll* list)
{
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

int scll_insertAtBeginning(scll* list, int value)
{
    scll_Node* node = scll_create_node(value);
    if (node == NULL)
    {
        return -1;
    }

    if (list->head == NULL)
    {
        // First node: it must point to itself so that tail->next == head holds.
        node->next = node;
        list->head = node;
        list->tail = node;
    }
    else
    {
        // New node becomes the head; tail keeps closing the ring onto the new head.
        node->next = list->head;
        list->head = node;
        list->tail->next = list->head;
    }

    list->length++;
    return 1;
}

int scll_insertAtEnd(scll* list, int value)
{
    scll_Node* node = scll_create_node(value);
    if (node == NULL)
    {
        return -1;
    }

    if (list->head == NULL)
    {
        node->next = node;
        list->head = node;
        list->tail = node;
    }
    else
    {
        // Link after the current tail and close the ring back onto head. O(1) thanks to tail.
        node->next = list->head;
        list->tail->next = node;
        list->tail = node;
    }

    list->length++;
    return 1;
}

int scll_insertAtPosition(scll* list, int value, int position)
{
    // Valid insert positions are 0..length (length == append at the end).
    if (position < 0 || position > list->length)
    {
        return -2;
    }

    if (position == 0)
    {
        return scll_insertAtBeginning(list, value);
    }
    if (position == list->length)
    {
        return scll_insertAtEnd(list, value);
    }

    // Interior insert: walk to the node just before the target position.
    scll_Node* node = scll_create_node(value);
    if (node == NULL)
    {
        return -1;
    }

    scll_Node* prev = list->head;
    for (int i = 0; i < position - 1; i++)
    {
        prev = prev->next;
    }

    node->next = prev->next;
    prev->next = node;
    // head and tail are untouched here (position is strictly interior), invariant preserved.

    list->length++;
    return 1;
}

int scll_deleteAtBeginning(scll* list)
{
    if (list->head == NULL)
    {
        return -1;
    }

    scll_Node* old_head = list->head;

    if (list->head == list->tail)
    {
        // Single node deletes down to the empty list.
        free(old_head);
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        list->head = old_head->next;
        list->tail->next = list->head; // keep the ring closed onto the new head
        free(old_head);
    }

    list->length--;
    return 1;
}

int scll_deleteAtEnd(scll* list)
{
    if (list->head == NULL)
    {
        return -1;
    }

    if (list->head == list->tail)
    {
        free(list->tail);
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        // Singly linked: we must walk from head to find the node before the tail.
        scll_Node* prev = list->head;
        while (prev->next != list->tail)
        {
            prev = prev->next;
        }
        free(list->tail);
        list->tail = prev;
        list->tail->next = list->head; // re-close the ring
    }

    list->length--;
    return 1;
}

int scll_deleteByValue(scll* list, int value)
{
    if (list->head == NULL)
    {
        return -2;
    }

    // Deleting the head value is delegated so the head/tail bookkeeping stays in one place.
    if (list->head->data == value)
    {
        return scll_deleteAtBeginning(list);
    }

    // Search for the predecessor of the node holding `value`, bounded by one full lap.
    scll_Node* prev = list->head;
    while (prev->next != list->head)
    {
        if (prev->next->data == value)
        {
            scll_Node* target = prev->next;
            prev->next = target->next;
            if (target == list->tail)
            {
                list->tail = prev; // removed the tail: prev becomes the new tail
            }
            free(target);
            list->length--;
            return 1;
        }
        prev = prev->next;
    }

    return -1; // value not present
}

int scll_deleteAtPosition(scll* list, int position)
{
    if (list->head == NULL)
    {
        return -1;
    }
    // Valid delete positions are 0..length-1.
    if (position < 0 || position >= list->length)
    {
        return -2;
    }

    if (position == 0)
    {
        return scll_deleteAtBeginning(list);
    }

    // Walk to the predecessor of the target node.
    scll_Node* prev = list->head;
    for (int i = 0; i < position - 1; i++)
    {
        prev = prev->next;
    }

    scll_Node* target = prev->next;
    prev->next = target->next;
    if (target == list->tail)
    {
        list->tail = prev; // deleting the last node updates tail
    }
    free(target);

    list->length--;
    return 1;
}

int scll_search(const scll* list, int key)
{
    if (list->head == NULL)
    {
        return -1;
    }

    int index = 0;
    const scll_Node* cur = list->head;
    do
    {
        if (cur->data == key)
        {
            return index;
        }
        cur = cur->next;
        index++;
    } while (cur != list->head);

    return -1;
}

int scll_getLength(const scll* list)
{
    return list->length;
}

void scll_printlist(const scll* list)
{
    printf("HEAD->");
    if (list->head == NULL)
    {
        printf("(empty)");
        return;
    }

    const scll_Node* cur = list->head;
    do
    {
        printf("%d->", cur->data);
        cur = cur->next;
    } while (cur != list->head);

    printf("(back to HEAD)");
}

void scll_destroy(scll* list)
{
    if (list->head == NULL)
    {
        return;
    }

    // Break the ring first so the walk has a NULL terminator and we never revisit a freed node.
    list->tail->next = NULL;
    scll_Node* cur = list->head;
    while (cur != NULL)
    {
        scll_Node* upcoming = cur->next;
        free(cur);
        cur = upcoming;
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}
