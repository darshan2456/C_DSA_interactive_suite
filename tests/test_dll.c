#include "dll.h"
#include <assert.h>
#include <stdio.h>

/* Helper: build list */
static doubly_ll_Node* build_list(int arr[], int n)
{
    doubly_ll_Node* head = NULL;
    for (int i = 0; i < n; i++)
    {
        assert(dll_insertAtEnd(&head, arr[i]) == 1);
    }
    return head;
}

/* Helper: list -> array */
static int list_to_array(doubly_ll_Node* head, int arr[], int max)
{
    int i = 0;
    while (head != NULL && i < max)
    {
        arr[i++] = head->data;
        head = head->next;
    }
    return i;
}

/* Insert tests */
void test_insert()
{
    doubly_ll_Node* head = NULL;

    assert(dll_insertAtBeginning(&head, 10) == 1);
    assert(head->data == 10);

    assert(dll_insertAtBeginning(&head, 5) == 1);
    assert(head->data == 5);

    assert(dll_insertAtEnd(&head, 20) == 1);

    int out[3];
    int n = list_to_array(head, out, 3);

    assert(n == 3);
    assert(out[0] == 5 && out[1] == 10 && out[2] == 20);

    delete_dll(head);

    printf("DLL insert tests passed\n");
}

/* Delete at beginning */
void test_delete_begin()
{
    int arr[] = {1, 2, 3};
    doubly_ll_Node* head = build_list(arr, 3);

    assert(dll_deleteAtBeginning(&head) == 1);

    int out[3];
    int n = list_to_array(head, out, 3);

    assert(n == 2);
    assert(out[0] == 2 && out[1] == 3);

    delete_dll(head);

    printf("DLL delete beginning tests passed\n");
}

/* Delete at end */
void test_delete_end()
{
    int arr[] = {1, 2, 3};
    doubly_ll_Node* head = build_list(arr, 3);

    assert(dll_deleteAtEnd(&head) == 1);

    int out[3];
    int n = list_to_array(head, out, 3);

    assert(n == 2);
    assert(out[0] == 1 && out[1] == 2);

    delete_dll(head);

    printf("DLL delete end tests passed\n");
}

/* Delete by value */
void test_delete_by_value()
{
    int arr[] = {1, 2, 3, 4};
    doubly_ll_Node* head = build_list(arr, 4);

    assert(dll_deleteByValue(&head, 3) == 1);

    int out[4];
    int n = list_to_array(head, out, 4);

    assert(n == 3);
    assert(out[0] == 1 && out[1] == 2 && out[2] == 4);

    delete_dll(head);

    printf("DLL delete by value tests passed\n");
}

/* Search test */
void test_search()
{
    int arr[] = {5, 10, 15};
    doubly_ll_Node* head = build_list(arr, 3);

    assert(dll_search(head, 5) == 0);
    assert(dll_search(head, 15) == 2);
    assert(dll_search(head, 99) == -1);

    delete_dll(head);

    printf("DLL search tests passed\n");
}

/* Reverse test */
void test_reverse()
{
    int arr[] = {1, 2, 3};
    doubly_ll_Node* head = build_list(arr, 3);

    dll_reverselist(&head);

    int out[3];
    int n = list_to_array(head, out, 3);

    assert(n == 3);
    assert(out[0] == 3 && out[1] == 2 && out[2] == 1);

    delete_dll(head);

    printf("DLL reverse tests passed\n");
}

/* Edge cases */
void test_edge_cases()
{
    doubly_ll_Node* head = NULL;

    assert(dll_deleteAtBeginning(&head) == -1);
    assert(dll_deleteAtEnd(&head) == -1);

    assert(dll_insertAtEnd(&head, 42) == 1);

    dll_reverselist(&head); // single node

    delete_dll(head);

    printf("DLL edge case tests passed\n");
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

    printf("All DLL tests passed\n");

    return 0;
}