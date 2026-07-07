#include "dcll.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

// Doubly circular linked list. See data_structures.h for the head/tail/length invariant.
//
// A note on traversal: because the list is circular there is no NULL terminator to stop on.
// Every traversal here therefore walks at most `length` nodes, using the idiom
//     cur = head; do { ...; cur = cur->next; } while (cur != head);
// which visits each node exactly once and is guaranteed to terminate (it stops the moment it
// arrives back at head). This is what prevents the infinite loops a circular list invites.
//
// Unlike scll, each node also carries a prev pointer so deletions at the end are O(1) —
// no predecessor walk is needed.

// Allocates a node holding `value`. Returns NULL on malloc failure. The caller is responsible
// for linking it into the ring (next and prev are left as NULL on purpose).

static dcll_Node* dcll_create_node(int value)
{
    dcll_Node* node = malloc(sizeof(dcll_Node));
    if (node == NULL)
    {
        return NULL;
    }
    node->data = value;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void dcll_init(dcll* list)
{
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

int dcll_insertAtBeginning(dcll* list, int value)
{
    dcll_Node* node = dcll_create_node(value);
    if (node == NULL)
    {
        return -1;
    }

    if (list->head == NULL)
    {
        // First node: it must point to itself so that tail->next == head and head->prev == tail.
        node->next = node;
        node->prev = node;
        list->head = node;
        list->tail = node;
    }
    else
    {
        // New node becomes the head; tail keeps closing the ring onto the new head.
        node->next = list->head;
        node->prev = list->tail;
        list->head->prev = node;
        list->tail->next = node;
        list->head = node;
    }

    list->length++;
    return 1;
}

int dcll_insertAtEnd(dcll* list, int value)
{
    dcll_Node* node = dcll_create_node(value);
    if (node == NULL)
    {
        return -1;
    }

    if (list->head == NULL)
    {
        // First node: point to itself in both directions.
        node->next = node;
        node->prev = node;
        list->head = node;
        list->tail = node;
    }
    else
    {
        // Link after the current tail and close the ring back onto head. O(1) thanks to tail.
        node->next = list->head;
        node->prev = list->tail;
        list->tail->next = node;
        list->head->prev = node;
        list->tail = node;
    }

    list->length++;
    return 1;
}

int dcll_insertAtPosition(dcll* list, int value, int position)
{
    // Valid insert positions are 0..length (length == append at the end).
    if (position < 0 || position > list->length)
    {
        return -2;
    }

    if (position == 0)
    {
        return dcll_insertAtBeginning(list, value);
    }
    if (position == list->length)
    {
        return dcll_insertAtEnd(list, value);
    }

    // Interior insert: walk to the node just before the target position.
    dcll_Node* node = dcll_create_node(value);
    if (node == NULL)
    {
        return -1;
    }

    dcll_Node* prev = list->head;
    for (int i = 0; i < position - 1; i++)
    {
        prev = prev->next;
    }

    node->next = prev->next;
    node->prev = prev;
    prev->next->prev = node;
    prev->next = node;
    // head and tail are untouched here (position is strictly interior), invariant preserved.

    list->length++;
    return 1;
}

int dcll_deleteAtBeginning(dcll* list)
{
    if (list->head == NULL)
    {
        return -1;
    }

    dcll_Node* old_head = list->head;

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
        list->head->prev = list->tail; // new head's prev closes onto tail
        list->tail->next = list->head; // tail closes ring onto new head
        free(old_head);
    }

    list->length--;
    return 1;
}

int dcll_deleteAtEnd(dcll* list)
{
    if (list->head == NULL)
    {
        return -1;
    }

    dcll_Node* old_tail = list->tail;

    if (list->head == list->tail)
    {
        // Single node deletes down to the empty list.
        free(old_tail);
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        // O(1) because prev pointer gives us the predecessor directly.
        list->tail = old_tail->prev;
        list->tail->next = list->head; // re-close the ring forward
        list->head->prev = list->tail; // re-close the ring backward
        free(old_tail);
    }

    list->length--;
    return 1;
}

int dcll_deleteByValue(dcll* list, int value)
{
    if (list->head == NULL)
    {
        return -2;
    }

    // Deleting the head value is delegated so the head/tail bookkeeping stays in one place.
    if (list->head->data == value)
    {
        return dcll_deleteAtBeginning(list);
    }

    // Search for the node holding `value`, bounded by one full lap.
    dcll_Node* cur = list->head->next;
    while (cur != list->head)
    {
        if (cur->data == value)
        {
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            if (cur == list->tail)
            {
                list->tail = cur->prev; // removed the tail: prev becomes the new tail
            }
            free(cur);
            list->length--;
            return 1;
        }
        cur = cur->next;
    }

    return -1; // value not present
}

int dcll_deleteAtPosition(dcll* list, int position)
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
        return dcll_deleteAtBeginning(list);
    }

    if (position == list->length - 1)
    {
        return dcll_deleteAtEnd(list);
    }

    // Walk to the target node.
    dcll_Node* target = list->head;
    for (int i = 0; i < position; i++)
    {
        target = target->next;
    }

    target->prev->next = target->next;
    target->next->prev = target->prev;
    free(target);

    list->length--;
    return 1;
}

int dcll_search(const dcll* list, int key)
{
    if (list->head == NULL)
    {
        return -1;
    }

    int index = 0;
    const dcll_Node* cur = list->head;
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

int dcll_getLength(const dcll* list)
{
    return list->length;
}

void dcll_printlist(const dcll* list)
{
    printf("HEAD->");
    if (list->head == NULL)
    {
        printf("(empty)");
        return;
    }

    const dcll_Node* cur = list->head;
    do
    {
        printf("%d->", cur->data);
        cur = cur->next;
    } while (cur != list->head);

    printf("(back to HEAD)");
}

void dcll_destroy(dcll* list)
{
    if (list->head == NULL)
    {
        return;
    }

    // Break the ring first so the walk has a NULL terminator and we never revisit a freed node.
    list->tail->next = NULL;
    dcll_Node* cur = list->head;
    while (cur != NULL)
    {
        dcll_Node* upcoming = cur->next;
        free(cur);
        cur = upcoming;
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

void dcll_demo(void)
{
    dcll list;
    dcll_init(&list);

    int dcll_element_count;
    int dcll_length_status;

start_dcll:
    dcll_length_status = safe_input_int(
        &dcll_element_count,
        "enter how many elements you want to insert, (between 1 and 100), enter '-1' to exit :- ",
        1, 100);

    if (dcll_length_status == INPUT_EXIT_SIGNAL)
    {
        printf("\nExiting dcll demo\n");
        dcll_destroy(&list);
        return;
    }

    if (dcll_length_status == 0)
    {
        goto start_dcll;
    }

    while (dcll_element_count > 0)
    {
        int dcll_position_choice;
        int dcll_position_status;

    dcll_position_selection:
        dcll_position_status = safe_input_int(&dcll_position_choice,
                                              "\nenter '0' for inserting at beginning"
                                              "\nenter '1' for inserting at end"
                                              "\nenter '2' for inserting at specific position"
                                              "\nenter '-1' to exit :- ",
                                              0, 2);

        if (dcll_position_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting dcll demo\n");
            dcll_destroy(&list);
            return;
        }

        if (dcll_position_status == 0)
        {
            goto dcll_position_selection;
        }

        if (dcll_position_choice == 1)
        {
            int dcll_end_status;
            int dcll_end_value;
        dcll_enter_end_value:
            dcll_end_status = safe_input_int(&dcll_end_value,
                                             "enter the no. you want to insert at end, (between 1 "
                                             "and 100), enter '-1' to exit :- ",
                                             1, 100);

            if (dcll_end_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }

            if (dcll_end_status == 0)
            {
                goto dcll_enter_end_value;
            }

            int status = dcll_insertAtEnd(&list, dcll_end_value);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto dcll_enter_end_value;
            }
            printf("\n");
            dcll_printlist(&list);
        }
        else if (dcll_position_choice == 0)
        {
            int dcll_start_status;
            int dcll_start_value;

        dcll_enter_start_value:
            dcll_start_status = safe_input_int(&dcll_start_value,
                                               "enter the no. you want to insert at beginning, "
                                               "(between 1 and 100), enter '-1' to exit :- ",
                                               1, 100);

            if (dcll_start_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }

            if (dcll_start_status == 0)
            {
                goto dcll_enter_start_value;
            }
            int status = dcll_insertAtBeginning(&list, dcll_start_value);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto dcll_enter_start_value;
            }
            printf("\n");
            dcll_printlist(&list);
        }
        else if (dcll_position_choice == 2)
        {
            int dcll_pos_status;
            int dcll_pos_value;
            int dcll_pos_index;
            char dcll_pos_prompt[128];

        dcll_enter_pos_value:
            dcll_pos_status = safe_input_int(&dcll_pos_value,
                                             "enter the no. you want to insert, (between 1 "
                                             "and 100), enter '-1' to exit :- ",
                                             1, 100);

            if (dcll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }

            if (dcll_pos_status == 0)
            {
                goto dcll_enter_pos_value;
            }

        dcll_enter_pos_index:
            snprintf(dcll_pos_prompt, sizeof(dcll_pos_prompt),
                     "enter the position (0 to %d), enter '-1' to exit :- ", dcll_getLength(&list));
            dcll_pos_status =
                safe_input_int(&dcll_pos_index, dcll_pos_prompt, 0, dcll_getLength(&list));

            if (dcll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }

            if (dcll_pos_status == 0)
            {
                goto dcll_enter_pos_index;
            }

            int status = dcll_insertAtPosition(&list, dcll_pos_value, dcll_pos_index);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto dcll_enter_pos_value;
            }
            else if (status == -2)
            {
                printf("\ninvalid position. try again\n");
                goto dcll_enter_pos_index;
            }
            printf("\n");
            dcll_printlist(&list);
        }

        dcll_element_count--;
    }

    // searching elements in dcll
    while (1)
    {
        int dcll_search_status;
        int dcll_search_value;
        dcll_search_status = safe_input_int(
            &dcll_search_value,
            "\nenter the element to be searched, (between 1 and 100), enter '-1' to exit :- ", 1,
            100);
        if (dcll_search_status == INPUT_EXIT_SIGNAL)
        {
            break;
        }
        if (dcll_search_status == 0)
        {
            continue;
        }

        int index = dcll_search(&list, dcll_search_value);
        printf("\nelement found at index :- %d", index);
    }

    // deleting elements in dcll
    while (1)
    {
        int dcll_delete_choice;
        int dcll_delete_status;

    dcll_delete_selection:
        dcll_delete_status = safe_input_int(&dcll_delete_choice,
                                            "\nenter '0' to delete at beginning"
                                            "\nenter '1' to delete at end"
                                            "\nenter '2' to delete by value"
                                            "\nenter '3' to delete at position"
                                            "\nenter '-1' to exit :- ",
                                            0, 3);

        if (dcll_delete_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting dcll demo\n");
            dcll_destroy(&list);
            return;
        }
        if (dcll_delete_status == 0)
        {
            goto dcll_delete_selection;
        }

        if (dcll_delete_choice == 0)
        {
            int status = dcll_deleteAtBeginning(&list);
            if (status == -1)
            {
                printf("\nList is empty\n");
            }
            else
            {
                printf("\ndcll after deletion - ");
                dcll_printlist(&list);
            }
        }
        else if (dcll_delete_choice == 1)
        {
            int status = dcll_deleteAtEnd(&list);
            if (status == -1)
            {
                printf("\nList is empty\n");
            }
            else
            {
                printf("\ndcll after deletion - ");
                dcll_printlist(&list);
            }
        }
        else if (dcll_delete_choice == 2)
        {
            int dcll_delete_value;
            dcll_delete_status = safe_input_int(
                &dcll_delete_value,
                "\nenter the element to be deleted, (between 1 and 100), enter '-1' to exit :- ", 1,
                100);

            if (dcll_delete_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }
            if (dcll_delete_status == 0)
            {
                continue;
            }

            int status = dcll_deleteByValue(&list, dcll_delete_value);
            if (status == -2)
            {
                printf("\nList is empty\n");
            }
            else if (status == -1)
            {
                printf("\nvalue not found\n");
            }
            else
            {
                printf("\ndcll after deletion - ");
                dcll_printlist(&list);
            }
        }
        else if (dcll_delete_choice == 3)
        {
            int dcll_pos_delete_status;
            int dcll_pos_delete_index;
            char dcll_pos_delete_prompt[128];

            if (dcll_getLength(&list) == 0)
            {
                printf("\nList is empty\n");
                continue;
            }

        dcll_delete_pos_input:
            snprintf(dcll_pos_delete_prompt, sizeof(dcll_pos_delete_prompt),
                     "enter the position to delete (0 to %d), enter '-1' to exit :- ",
                     dcll_getLength(&list) - 1);
            dcll_pos_delete_status = safe_input_int(&dcll_pos_delete_index, dcll_pos_delete_prompt,
                                                    0, dcll_getLength(&list) - 1);

            if (dcll_pos_delete_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dcll demo\n");
                dcll_destroy(&list);
                return;
            }

            if (dcll_pos_delete_status == 0)
            {
                goto dcll_delete_pos_input;
            }

            int status = dcll_deleteAtPosition(&list, dcll_pos_delete_index);
            if (status == -1)
            {
                printf("\nList is empty\n");
            }
            else if (status == -2)
            {
                printf("\nInvalid position\n");
            }
            else
            {
                printf("\ndcll after deletion - ");
                dcll_printlist(&list);
            }
        }
    }

    dcll_destroy(&list);
}