#include "sll.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static int compare_ints(const void* a, const void* b)
{
    return *(const int*)a - *(const int*)b;
}

/* Helper: build list from array */
static Node* build_list(int arr[], int n)
{
    Node* head = NULL;
    for (int i = 0; i < n; i++)
    {
        int* val = malloc(sizeof(int));
        *val = arr[i];
        assert(sll_insertAtEnd(&head, val) == 1);
    }
    return head;
}

/* Helper: convert list to array */
static int list_to_array(Node* head, int arr[], int max)
{
    int i = 0;
    while (head != NULL && i < max)
    {
        arr[i++] = *(int*)(head->data);
        head = head->next;
    }
    return i;
}

/* Test insert operations */
void test_insert()
{
    Node* head = NULL;

    int* val10 = malloc(sizeof(int));
    *val10 = 10;
    assert(sll_insertAtEnd(&head, val10) == 1);
    assert(*(int*)(head->data) == 10);

    int* val5 = malloc(sizeof(int));
    *val5 = 5;
    assert(sll_insertAtBeginning(&head, val5) == 1);
    assert(*(int*)(head->data) == 5);
    assert(*(int*)(head->next->data) == 10);

    delete_sll(head, free);
    printf("sll Insert tests passed\n");
}

/* Test delete at beginning */
void test_delete_begin()
{
    int arr[] = {1, 2, 3};
    Node* head = build_list(arr, 3);

    assert(sll_deleteAtBeginning(&head, free) == 1);
    assert(*(int*)(head->data) == 2);

    int out[3];
    int n = list_to_array(head, out, 3);
    assert(n == 2);
    assert(out[0] == 2 && out[1] == 3);

    delete_sll(head, free);
    printf("sll Delete at beginning tests passed\n");
}

/* Test delete at end */
void test_delete_end()
{
    int arr[] = {1, 2, 3};
    Node* head = build_list(arr, 3);

    assert(sll_deleteAtEnd(&head, free) == 1);

    int out[3];
    int n = list_to_array(head, out, 3);
    assert(n == 2);
    assert(out[0] == 1 && out[1] == 2);

    delete_sll(head, free);
    printf("sll Delete at end tests passed\n");
}

/* Test delete by value */
void test_delete_by_value()
{
    int arr[] = {1, 2, 3, 4};
    Node* head = build_list(arr, 4);

    int key2 = 2;
    assert(sll_deleteByValue(&head, &key2, compare_ints, free) == 1);

    int out[4];
    int n = list_to_array(head, out, 4);
    assert(n == 3);
    assert(out[0] == 1 && out[1] == 3 && out[2] == 4);

    int key99 = 99;
    assert(sll_deleteByValue(&head, &key99, compare_ints, free) == -1);

    delete_sll(head, free);
    printf("sll Delete by value tests passed\n");
}

/* Test search */
void test_search()
{
    int arr[] = {5, 10, 15};
    Node* head = build_list(arr, 3);

    int key5 = 5;
    int key15 = 15;
    int key100 = 100;
    assert(sll_search(head, &key5, compare_ints) == 0);
    assert(sll_search(head, &key15, compare_ints) == 2);
    assert(sll_search(head, &key100, compare_ints) == -1);

    delete_sll(head, free);
    printf("sll Search tests passed\n");
}

/* Test reverse */
void test_reverse()
{
    int arr[] = {1, 2, 4};
    Node* head = build_list(arr, 3);

    assert(sll_reverseList(&head) == 1);

    int out[3];
    int n = list_to_array(head, out, 3);
    assert(n == 3);
    assert(out[0] == 4 && out[1] == 2 && out[2] == 1);

    delete_sll(head, free);
    printf("sll Reverse tests passed\n");
}

/* Test insert at position */
void test_insert_at_position()
{
    Node* head = NULL;

    /* Insert at position 0 in empty list */
    int* val10 = malloc(sizeof(int));
    *val10 = 10;
    assert(sll_insertAtPosition(&head, val10, 0) == 1);
    assert(*(int*)(head->data) == 10);

    /* Insert at position 1 (end) */
    int* val20 = malloc(sizeof(int));
    *val20 = 20;
    assert(sll_insertAtPosition(&head, val20, 1) == 1);
    assert(*(int*)(head->next->data) == 20);

    /* Insert at position 1 (middle) */
    int* val15 = malloc(sizeof(int));
    *val15 = 15;
    assert(sll_insertAtPosition(&head, val15, 1) == 1);

    int out[3];
    int n = list_to_array(head, out, 3);
    assert(n == 3);
    assert(out[0] == 10 && out[1] == 15 && out[2] == 20);

    /* Test invalid position */
    int* val99 = malloc(sizeof(int));
    *val99 = 99;
    assert(sll_insertAtPosition(&head, val99, 10) == -2);
    assert(sll_insertAtPosition(&head, val99, -1) == -2);
    free(val99);

    delete_sll(head, free);
    printf("sll Insert at position tests passed\n");
}

/* Test delete at position */
void test_delete_at_position()
{
    int arr[] = {1, 2, 3, 4};
    Node* head = build_list(arr, 4);

    /* Delete at position 0 */
    assert(sll_deleteAtPosition(&head, 0, free) == 1);

    int out[3];
    int n = list_to_array(head, out, 3);
    assert(n == 3);
    assert(out[0] == 2 && out[1] == 3 && out[2] == 4);

    /* Delete at position 1 (middle) */
    assert(sll_deleteAtPosition(&head, 1, free) == 1);

    n = list_to_array(head, out, 3);
    assert(n == 2);
    assert(out[0] == 2 && out[1] == 4);

    /* Test invalid position */
    assert(sll_deleteAtPosition(&head, 10, free) == -2);
    assert(sll_deleteAtPosition(&head, -1, free) == -2);

    delete_sll(head, free);
    printf("sll Delete at position tests passed\n");
}

/* Edge cases */
void test_edge_cases()
{
    Node* head = NULL;

    assert(sll_reverseList(&head) == -2);
    assert(sll_deleteAtBeginning(&head, free) == -1);
    assert(sll_deleteAtEnd(&head, free) == -1);
    int key10 = 10;
    assert(sll_deleteByValue(&head, &key10, compare_ints, free) == -2);

    int* val42 = malloc(sizeof(int));
    *val42 = 42;
    assert(sll_insertAtEnd(&head, val42) == 1);
    assert(sll_reverseList(&head) == -1);

    delete_sll(head, free);
    printf("sll Edge case tests passed\n");
}

int main()
{
    test_insert();
    test_delete_begin();
    test_delete_end();
    test_delete_by_value();
    test_search();
    test_reverse();
    test_insert_at_position();
    test_delete_at_position();
    test_edge_cases();

    printf("All SLL tests passed\n");
    return 0;
}