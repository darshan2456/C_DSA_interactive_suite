#include "sll.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* Helper: build list from array */
static Node* build_list(int arr[], int n)
{
    Node* head = NULL;
    for (int i = 0; i < n; i++)
    {
        assert(sll_insertAtEnd(&head, arr[i]) == 1);
    }
    return head;
}

/* Helper: convert list to array */
static int list_to_array(Node* head, int arr[], int max)
{
    int i = 0;
    while (head != NULL && i < max)
    {
        arr[i++] = head->data;
        head = head->next;
    }
    return i;
}

/* Test insert operations */
void test_insert()
{
    Node* head = NULL;

    assert(sll_insertAtEnd(&head, 10) == 1);
    assert(head->data == 10);

    assert(sll_insertAtBeginning(&head, 5) == 1);
    assert(head->data == 5);
    assert(head->next->data == 10);

    delete_sll(head);
    printf("sll Insert tests passed\n");
}

/* Test delete at beginning */
void test_delete_begin()
{
    int arr[] = {1, 2, 3};
    Node* head = build_list(arr, 3);

    assert(sll_deleteAtBeginning(&head) == 1);
    assert(head->data == 2);

    int out[3];
    int n = list_to_array(head, out, 3);
    assert(n == 2);
    assert(out[0] == 2 && out[1] == 3);

    delete_sll(head);
    printf("sll Delete at beginning tests passed\n");
}

/* Test delete at end */
void test_delete_end()
{
    int arr[] = {1, 2, 3};
    Node* head = build_list(arr, 3);

    assert(sll_deleteAtEnd(&head) == 1);

    int out[3];
    int n = list_to_array(head, out, 3);
    assert(n == 2);
    assert(out[0] == 1 && out[1] == 2);

    delete_sll(head);
    printf("sll Delete at end tests passed\n");
}

/* Test delete by value */
void test_delete_by_value()
{
    int arr[] = {1, 2, 3, 4};
    Node* head = build_list(arr, 4);

    assert(sll_deleteByValue(&head, 2) == 1);

    int out[4];
    int n = list_to_array(head, out, 4);
    assert(n == 3);
    assert(out[0] == 1 && out[1] == 3 && out[2] == 4);

    assert(sll_deleteByValue(&head, 99) == -1);

    delete_sll(head);
    printf("sll Delete by value tests passed\n");
}

/* Test search */
void test_search()
{
    int arr[] = {5, 10, 15};
    Node* head = build_list(arr, 3);

    assert(sll_search(head, 5) == 0);
    assert(sll_search(head, 15) == 2);
    assert(sll_search(head, 100) == -1);

    delete_sll(head);
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

    delete_sll(head);
    printf("sll Reverse tests passed\n");
}

/* Edge cases */
void test_edge_cases()
{
    Node* head = NULL;

    assert(sll_reverseList(&head) == -2);
    assert(sll_deleteAtBeginning(&head) == -1);
    assert(sll_deleteAtEnd(&head) == -1);
    assert(sll_deleteByValue(&head, 10) == -2);

    assert(sll_insertAtEnd(&head, 42) == 1);
    assert(sll_reverseList(&head) == -1);

    delete_sll(head);
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
    test_edge_cases();

    printf("All SLL tests passed\n");
    return 0;
}