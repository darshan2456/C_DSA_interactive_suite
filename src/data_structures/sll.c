#include "sll.h"
#include <stdio.h>
#include <stdlib.h>

// methods implemented are - insertAtEnd, deleteAtBeginning, insertAtBeginning, deleteAtEnd,
// printlist, search
// deleteByValue and reverseList

// insert at end returns -1 on allocation failure and 1 on successful insertion
int sll_insertAtEnd(Node** head_ref, void* value)
{
    Node* newnode = malloc(sizeof(Node));
    if (newnode == NULL)
        return -1;
    newnode->data = value;
    newnode->next = NULL;
    if (*head_ref == NULL)
    {
        *head_ref = newnode;
        return 1;
    }

    Node* temp = *head_ref;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newnode;
    return 1;
}

// returns -1 if list and 1 on successful deletion
int sll_deleteAtBeginning(Node** head_ref, void (*free_data)(void*))
{
    if (*head_ref == NULL)
    {
        return -1;
    }
    Node* temp = *head_ref;
    *head_ref = temp->next;
    if (free_data != NULL)
    {
        free_data(temp->data);
    }
    free(temp);
    return 1;
}

// insertAtBeginning function returns -1 on allocation failure and 1 on succesful insertion
int sll_insertAtBeginning(Node** head_ref, void* value)
{
    Node* newnode = malloc(sizeof(Node));
    if (newnode == NULL)
        return -1;
    newnode->data = value;
    newnode->next = *head_ref;
    *head_ref = newnode;
    return 1;
}

// return -1 if list is empty and 1 on successful deletion
int sll_deleteAtEnd(Node** head_ref, void (*free_data)(void*))
{
    if (*head_ref == NULL)
    {
        return -1;
    }
    Node* temp = *head_ref;
    if (temp->next == NULL)
    {
        if (free_data != NULL)
        {
            free_data(temp->data);
        }
        free(temp);
        *head_ref = NULL;
        return 1;
    }
    Node* curr = *head_ref;
    Node* prev = NULL;
    while (curr->next != NULL)
    {
        prev = curr;
        curr = curr->next;
    }
    if (free_data != NULL)
    {
        free_data(curr->data);
    }
    free(curr);
    prev->next = NULL;
    return 1;
}

void sll_printlist(const Node* head, void (*print_element)(const void*))
{
    printf("HEAD->");
    while (head != NULL)
    {
        if (print_element != NULL)
        {
            print_element(head->data);
        }
        else
        {
            printf("%p", head->data);
        }
        printf(" ->");
        head = head->next;
    }
    printf("NULL");
}

int sll_search(const Node* head, const void* key, int (*compare)(const void*, const void*))
{
    int index = 0;
    while (head != NULL)
    {
        int match = 0;
        if (compare != NULL)
        {
            match = (compare(head->data, key) == 0);
        }
        else
        {
            match = (head->data == key);
        }

        if (match)
        {
            return index; // if value found returns index number
        }
        head = head->next;
        index++;
    }
    return -1; // otherwise returns -1
}

// return -2 if list is empty, -1 if element not found and 1 on successful deletion
int sll_deleteByValue(Node** head_ref, const void* value, int (*compare)(const void*, const void*),
                      void (*free_data)(void*))
{
    if (*head_ref == NULL)
    {
        return -2;
    }
    Node* curr = *head_ref;
    Node* prev = NULL;

    int head_match = 0;
    if (compare != NULL)
    {
        head_match = (compare(curr->data, value) == 0);
    }
    else
    {
        head_match = (curr->data == value);
    }

    if (head_match)
    {
        *head_ref = curr->next;
        if (free_data != NULL)
        {
            free_data(curr->data);
        }
        free(curr);
        return 1;
    }
    while (1)
    {
        int match = 0;
        if (compare != NULL)
        {
            match = (compare(curr->data, value) == 0);
        }
        else
        {
            match = (curr->data == value);
        }

        if (match)
        {
            break;
        }

        prev = curr;
        curr = curr->next;
        if (curr == NULL)
        {
            return -1;
        }
    }
    prev->next = curr->next;
    if (free_data != NULL)
    {
        free_data(curr->data);
    }
    free(curr);
    return 1;
}

// returns -2 if list is empty, -1 if list has only 1 element and 1 on successful reversal
int sll_reverseList(Node** head_ref)
{
    Node* prev = NULL;
    Node* curr = *head_ref;
    if (curr == NULL)
    {
        return -2;
    }
    Node* upcoming = curr->next;
    if (upcoming == NULL)
    {
        return -1;
    }
    while (upcoming != NULL)
    {
        curr->next = prev;
        prev = curr;
        curr = upcoming;
        upcoming = upcoming->next;
    }
    curr->next = prev;
    *head_ref = curr;
    return 1;
}

void delete_sll(Node* head, void (*free_data)(void*))
{
    while (head != NULL)
    {
        Node* upcoming = head->next;
        if (free_data != NULL)
        {
            free_data(head->data);
        }
        free(head);
        head = upcoming;
    }
}

// Helper function to get the length of the list
int sll_getLength(const Node* head)
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
int sll_insertAtPosition(Node** head_ref, void* value, int position)
{
    int length = sll_getLength(*head_ref);

    if (position < 0 || position > length)
    {
        return -2;
    }

    Node* newnode = malloc(sizeof(Node));
    if (newnode == NULL)
        return -1;

    newnode->data = value;

    if (position == 0)
    {
        newnode->next = *head_ref;
        *head_ref = newnode;
        return 1;
    }

    Node* temp = *head_ref;
    for (int i = 0; i < position - 1; i++)
    {
        temp = temp->next;
    }

    newnode->next = temp->next;
    temp->next = newnode;
    return 1;
}

// Delete at a specific position (0-indexed)
// Returns 1 on success, -1 on empty list, -2 on invalid position
int sll_deleteAtPosition(Node** head_ref, int position, void (*free_data)(void*))
{
    if (*head_ref == NULL)
    {
        return -1;
    }

    int length = sll_getLength(*head_ref);

    if (position < 0 || position >= length)
    {
        return -2;
    }

    Node* temp = *head_ref;

    if (position == 0)
    {
        *head_ref = temp->next;
        if (free_data != NULL)
        {
            free_data(temp->data);
        }
        free(temp);
        return 1;
    }

    Node* prev = NULL;
    for (int i = 0; i < position; i++)
    {
        prev = temp;
        temp = temp->next;
    }

    prev->next = temp->next;
    if (free_data != NULL)
    {
        free_data(temp->data);
    }
    free(temp);
    return 1;
}