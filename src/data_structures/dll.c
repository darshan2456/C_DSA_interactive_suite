#include "dll.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

// insert functions returns 1 on succesful insertion and -1 on malloc failure
// delete functions return 1 on successful deletion and -1 on failure due to empty list
// methods implemented are - insertAtBeginning, insertAtEnd, printlist, search, deleteAtBeginning,
// deleteAtEnd, deleteByValue, insertAtPosition, deleteAtPosition and reverselist

void dll_demo(void)
{
    doubly_ll_Node* head = NULL;
    int dll_element_count;
    int dll_length_status;
start_dll:
    dll_length_status = safe_input_int(&dll_element_count,
                                       "\nenter the number of elements you want to insert, "
                                       "(between 1 and 100), enter '-1' to exit: ",
                                       1, 100);

    if (dll_length_status == INPUT_EXIT_SIGNAL)
    {
        printf("\nExiting dll demo.\n");
        delete_dll(head);
        return;
    }
    if (dll_length_status == 0)
    {
        goto start_dll;
    }

    // insertion of nodes in dll
    while (dll_element_count > 0)
    {
        int dll_position_choice;
        int dll_position_status;

    dll_position_selection:
        dll_position_status = safe_input_int(&dll_position_choice,
                                             "\nenter '0' for inserting at beginning"
                                             "\nenter '1' for inserting at end"
                                             "\nenter '2' for inserting at position"
                                             "\nenter '-1' to exit :- ",
                                             0, 2);

        if (dll_position_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting dll demo.\n");
            delete_dll(head);
            return;
        }
        if (dll_position_status == 0)
        {
            goto dll_position_selection;
        }

        if (dll_position_choice == 0)
        { // enter element at start
            int dll_end_status;
            int dll_end_value;

        dll_enter_end_value:
            dll_end_status = safe_input_int(
                &dll_end_value,
                "enter the value to be inserted at end, (between 1 and 100), enter '-1' to exit - ",
                1, 100);

            if (dll_end_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dll demo.\n");
                delete_dll(head);
                return;
            }
            if (dll_end_status == 0)
            {
                goto dll_enter_end_value;
            }
            dll_insertAtBeginning(&head, dll_end_value);
            dll_printlist(head);
        }
        else if (dll_position_choice == 1)
        { // enter element at end
            int dll_start_status;
            int dll_start_value;

        dll_enter_start_value:
            dll_start_status = safe_input_int(&dll_start_value,
                                              "enter the value to be inserted at beginning, "
                                              "(between 1 and 100), enter '-1' to exit: ",
                                              1, 100);

            if (dll_start_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dll demo.\n");
                delete_dll(head);
                return;
            }
            if (dll_start_status == 0)
            {
                goto dll_enter_start_value;
            }
            dll_insertAtEnd(&head, dll_start_value);
            dll_printlist(head);
        }
        else if (dll_position_choice == 2)
        {
            int dll_pos_status;
            int dll_pos_value;
            int dll_pos_index;
            char dll_pos_prompt[128];

        dll_enter_pos_value:
            dll_pos_status = safe_input_int(&dll_pos_value,
                                            "enter the value to be inserted, (between 1 "
                                            "and 100), enter '-1' to exit :- ",
                                            1, 100);

            if (dll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dll demo.\n");
                delete_dll(head);
                return;
            }

            if (dll_pos_status == 0)
            {
                goto dll_enter_pos_value;
            }

        dll_enter_pos_index:
            snprintf(dll_pos_prompt, sizeof(dll_pos_prompt),
                     "enter the position (0 to %d), enter '-1' to exit :- ",
                     dll_getLength(head) - 1);

            dll_pos_status = safe_input_int(&dll_pos_index, dll_pos_prompt, 0, dll_getLength(head));

            if (dll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dll demo.\n");
                delete_dll(head);
                return;
            }

            if (dll_pos_status == 0)
            {
                goto dll_enter_pos_index;
            }

            int status = dll_insertAtPosition(&head, dll_pos_value, dll_pos_index);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto dll_enter_pos_value;
            }
            else if (status == -2)
            {
                printf("\ninvalid position. try again\n");
                goto dll_enter_pos_index;
            }
            dll_printlist(head);
        }
        dll_element_count--;
    }

    // doubly linked list reversal
    int dll_rev_status = dll_reverselist(&head);

    switch (dll_rev_status)
    {
        case -2:
            printf("\nempty list cannot be reversed.");
            break;
        case -1:
            printf("\nsingle node list cannot be reversed");
            break;
        case 1:
            printf("\nreversed list is:- ");
            dll_printlist(head);
            printf("\nthe restored list is:- ");
            dll_reverselist(&head);
            dll_printlist(head);
    }

    // searching elements in the dll
    while (1)
    {
        int dll_search_status;
        int dll_search_value;

        dll_search_status = safe_input_int(
            &dll_search_value,
            "\nenter the number you want to search, (between 1 and 100), enter '-1' to exit:- ", 1,
            100);

        if (dll_search_status == INPUT_EXIT_SIGNAL)
        {
            break;
        }

        if (dll_search_status == 0)
        {
            continue;
        }

        int index = dll_search(head, dll_search_value);
        printf("\nentered number found at index %d", index);
    }

    // deleting values from dll
    while (1)
    {
        int dll_delete_choice;
        int dll_delete_status;

    dll_delete_selection:
        dll_delete_status = safe_input_int(&dll_delete_choice,
                                           "\nenter '0' to delete by value"
                                           "\nenter '1' to delete at position"
                                           "\nenter '-1' to exit :- ",
                                           0, 1);

        if (dll_delete_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting dll demo.\n");
            delete_dll(head);
            return;
        }
        if (dll_delete_status == 0)
        {
            goto dll_delete_selection;
        }

        if (dll_delete_choice == 0)
        {
            int dll_delete_value;
            dll_delete_status = safe_input_int(
                &dll_delete_value,
                "\nenter element to be deleted, (between 1 and 100), enter '-1' to exit :- ", 1,
                100);

            if (dll_delete_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dll demo.\n");
                delete_dll(head);
                return;
            }
            if (dll_delete_status == 0)
            {
                continue;
            }

            int status = dll_deleteByValue(&head, dll_delete_value);
            if (status == 1)
            {
                printf("\ndll after deletion - ");
                dll_printlist(head);
            }
            else if (status == -1)
            {
                printf("\nError: Node with value %d not found.\n", dll_delete_value);
            }
            else if (status == -2)
            {
                printf("\nError: List is empty.\n");
            }
        }
        else if (dll_delete_choice == 1)
        {
            int dll_pos_delete_status;
            int dll_pos_delete_index;
            char dll_pos_delete_prompt[128];

        dll_delete_pos_input:
            snprintf(dll_pos_delete_prompt, sizeof(dll_pos_delete_prompt),
                     "enter the position to delete (0 to %d), enter '-1' to exit :- ",
                     dll_getLength(head) - 1);
            dll_pos_delete_status = safe_input_int(&dll_pos_delete_index, dll_pos_delete_prompt, 0,
                                                   dll_getLength(head) - 1);

            if (dll_pos_delete_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dll demo.\n");
                delete_dll(head);
                return;
            }

            if (dll_pos_delete_status == 0)
            {
                goto dll_delete_pos_input;
            }

            int status = dll_deleteAtPosition(&head, dll_pos_delete_index);
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
                printf("\ndll after deletion - ");
                dll_printlist(head);
            }
        }
    }
}

int dll_insertAtBeginning(doubly_ll_Node** head_ref, int value)
{
    doubly_ll_Node* newnode = malloc(sizeof(doubly_ll_Node));
    if (newnode == NULL)
        return -1;
    newnode->data = value;
    if (*head_ref == NULL)
    {
        newnode->next = NULL;
        newnode->prev = NULL;
        *head_ref = newnode;
        return 1;
    }
    newnode->next = *head_ref;
    newnode->prev = NULL;
    (*head_ref)->prev = newnode;
    *head_ref = newnode;
    return 1;
}

int dll_insertAtEnd(doubly_ll_Node** head_ref, int value)
{
    doubly_ll_Node* newnode = malloc(sizeof(doubly_ll_Node));
    if (newnode == NULL)
        return -1;
    newnode->data = value;
    if (*head_ref == NULL)
    {
        newnode->next = NULL;
        newnode->prev = NULL;
        *head_ref = newnode;
        return 1;
    }
    doubly_ll_Node* temp = *head_ref;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    newnode->prev = temp;
    newnode->next = NULL;
    temp->next = newnode;
    return 1;
}

void dll_printlist(const doubly_ll_Node* head)
{
    printf("\nHEAD<-> ");
    while (head != NULL)
    {
        printf("%d <->", head->data);
        head = head->next;
    }
    printf("NULL");
}

int dll_search(const doubly_ll_Node* head, int key)
{
    int index = 0;
    while (head != NULL)
    {
        if (head->data == key)
        {
            return index; // if value found returns index number
        }
        head = head->next;
        index++;
    }
    return -1; // otherwise returns -1
}

int dll_deleteAtBeginning(doubly_ll_Node** head_ref)
{
    if (*head_ref == NULL)
        return -1;
    if ((*head_ref)->next == NULL)
    {
        free(*head_ref);
        *head_ref = NULL;
        return 1;
    }
    doubly_ll_Node* secondnode = (*head_ref)->next;
    free(*head_ref);
    secondnode->prev = NULL;
    *head_ref = secondnode;
    return 1;
}

int dll_deleteAtEnd(doubly_ll_Node** head_ref)
{
    if (*head_ref == NULL)
        return -1;
    if ((*head_ref)->next == NULL)
    {
        free(*head_ref);
        *head_ref = NULL;
        return 1;
    }
    doubly_ll_Node* temp = *head_ref;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    doubly_ll_Node* secondlast = temp->prev;
    free(temp);
    secondlast->next = NULL;
    return 1;
}

int dll_deleteByValue(doubly_ll_Node** head_ref, int key)
{
    if (*head_ref == NULL)
        return -2;

    if ((*head_ref)->next == NULL && (*head_ref)->data == key)
    {
        free(*head_ref);
        *head_ref = NULL;
        return 1;
    }
    doubly_ll_Node* temp = *head_ref;
    while (temp != NULL)
    {
        if (temp->data == key)
        {
            doubly_ll_Node* beforekey = temp->prev;
            doubly_ll_Node* afterkey = temp->next;
            if (beforekey == NULL)
            {
                *head_ref = temp->next;
                (*head_ref)->prev = NULL;
                free(temp);
                return 1;
            }
            beforekey->next = afterkey;
            if (afterkey != NULL)
            {
                afterkey->prev = beforekey;
            }
            free(temp);
            return 1;
        }
        temp = temp->next;
    }

    return -1;
}

void delete_dll(doubly_ll_Node* head)
{
    while (head != NULL)
    {
        doubly_ll_Node* upcoming = head->next;
        free(head);
        head = upcoming;
    }
}

// returns -2 if list is empty. -1 if list is a single node list and 1 on successful reversal.
int dll_reverselist(doubly_ll_Node** head_ref)
{
    doubly_ll_Node* temp = NULL;
    doubly_ll_Node* current = *head_ref;

    if (current == NULL)
    {
        return -2;
    }
    if (current->next == NULL)
    {
        return -1;
    }

    while (current != NULL)
    {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }

    if (temp != NULL)
    {
        *head_ref = temp->prev;
    }

    return 1;
}

// Helper function to get the length of the doubly linked list
int dll_getLength(const doubly_ll_Node* head)
{
    int length = 0;
    while (head != NULL)
    {
        length++;
        head = head->next;
    }
    return length;
}

// Insert at a specific position (0-indexed)
// Returns 1 on success, -1 on malloc failure, -2 on invalid position
int dll_insertAtPosition(doubly_ll_Node** head_ref, int value, int position)
{
    int length = dll_getLength(*head_ref);

    if (position < 0 || position > length)
    {
        return -2;
    }

    doubly_ll_Node* newnode = malloc(sizeof(doubly_ll_Node));
    if (newnode == NULL)
        return -1;

    newnode->data = value;

    if (position == 0)
    {
        if (*head_ref == NULL)
        {
            newnode->next = NULL;
            newnode->prev = NULL;
            *head_ref = newnode;
        }
        else
        {
            newnode->next = *head_ref;
            newnode->prev = NULL;
            (*head_ref)->prev = newnode;
            *head_ref = newnode;
        }
        return 1;
    }

    doubly_ll_Node* temp = *head_ref;
    for (int i = 0; i < position - 1; i++)
    {
        temp = temp->next;
    }

    newnode->next = temp->next;
    newnode->prev = temp;
    if (temp->next != NULL)
    {
        temp->next->prev = newnode;
    }
    temp->next = newnode;
    return 1;
}

// Delete at a specific position (0-indexed)
// Returns 1 on success, -1 on empty list, -2 on invalid position
int dll_deleteAtPosition(doubly_ll_Node** head_ref, int position)
{
    if (*head_ref == NULL)
    {
        return -1;
    }

    int length = dll_getLength(*head_ref);

    if (position < 0 || position >= length)
    {
        return -2;
    }

    doubly_ll_Node* temp = *head_ref;

    if (position == 0)
    {
        if (temp->next != NULL)
        {
            temp->next->prev = NULL;
            *head_ref = temp->next;
        }
        else
        {
            *head_ref = NULL;
        }
        free(temp);
        return 1;
    }

    for (int i = 0; i < position; i++)
    {
        temp = temp->next;
    }

    doubly_ll_Node* prevnode = temp->prev;
    if (temp->next != NULL)
    {
        temp->next->prev = prevnode;
    }
    prevnode->next = temp->next;
    free(temp);
    return 1;
}
