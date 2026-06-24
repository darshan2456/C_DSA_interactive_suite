#include "data_structures.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Independently counts the ring by walking at most one full lap. Returns the node count and,
// via *invariant_ok, whether tail->next == head and head->prev == tail currently holds.
// Walking is bounded: it stops when it returns to head, or after length+1 hops as a safety cap
// so a broken ring can never loop forever inside the tests themselves.
static int ring_count(const dcll* list, int* invariant_ok)
{
    if (list->head == NULL)
    {
        *invariant_ok = (list->tail == NULL);
        return 0;
    }

    // Both forward and backward ring invariants must hold.
    *invariant_ok = (list->tail->next == list->head) && (list->head->prev == list->tail);

    int count = 0;
    const dcll_Node* cur = list->head;
    do
    {
        count++;
        cur = cur->next;
    } while (cur != list->head && count <= list->length + 1);

    return count;
}

// Asserts the structural invariant after a mutation: tail->next == head, head->prev == tail,
// and the cached length matches an independent walk of the ring.
static void assert_invariant(const dcll* list)
{
    int invariant_ok = 0;
    int counted = ring_count(list, &invariant_ok);
    assert(invariant_ok);
    assert(counted == list->length);
}

// Copies the list into arr by walking exactly `length` nodes forward. Returns items copied.
static int list_to_array(const dcll* list, int arr[], int max)
{
    int i = 0;
    if (list->head == NULL)
    {
        return 0;
    }
    const dcll_Node* cur = list->head;
    while (i < list->length && i < max)
    {
        arr[i++] = cur->data;
        cur = cur->next;
    }
    return i;
}

// Copies the list into arr by walking backward from tail. Returns items copied.
// Used to verify prev pointers are correctly maintained.
static int list_to_array_reverse(const dcll* list, int arr[], int max)
{
    int i = 0;
    if (list->tail == NULL)
    {
        return 0;
    }
    const dcll_Node* cur = list->tail;
    while (i < list->length && i < max)
    {
        arr[i++] = cur->data;
        cur = cur->prev;
    }
    return i;
}

void test_insert_begin_end()
{
    dcll list;
    dcll_init(&list);

    assert(dcll_insertAtEnd(&list, 10) == 1);
    // Single node points to itself in both directions and is both head and tail.
    assert(list.head == list.tail);
    assert(list.head->next == list.head);
    assert(list.head->prev == list.head);
    assert_invariant(&list);

    assert(dcll_insertAtEnd(&list, 20) == 1);
    assert(dcll_insertAtBeginning(&list, 5) == 1);
    assert_invariant(&list);

    int out[3];
    int n = list_to_array(&list, out, 3);
    assert(n == 3);
    assert(out[0] == 5 && out[1] == 10 && out[2] == 20);
    assert(list.head->data == 5);
    assert(list.tail->data == 20);

    // Verify prev pointers by walking backward.
    int rev[3];
    int m = list_to_array_reverse(&list, rev, 3);
    assert(m == 3);
    assert(rev[0] == 20 && rev[1] == 10 && rev[2] == 5);

    dcll_destroy(&list);
    printf("dcll insert begin/end tests passed\n");
}

void test_insert_at_position()
{
    dcll list;
    dcll_init(&list);

    // Insert into empty list at position 0.
    assert(dcll_insertAtPosition(&list, 10, 0) == 1);
    // Append at the end (position == length).
    assert(dcll_insertAtPosition(&list, 30, 1) == 1);
    // Interior insert.
    assert(dcll_insertAtPosition(&list, 20, 1) == 1);
    assert_invariant(&list);

    int out[3];
    int n = list_to_array(&list, out, 3);
    assert(n == 3);
    assert(out[0] == 10 && out[1] == 20 && out[2] == 30);
    assert(list.tail->data == 30);

    // Invalid positions.
    assert(dcll_insertAtPosition(&list, 99, 10) == -2);
    assert(dcll_insertAtPosition(&list, 99, -1) == -2);
    assert_invariant(&list);

    dcll_destroy(&list);
    printf("dcll insert at position tests passed\n");
}

void test_delete_begin_end()
{
    dcll list;
    dcll_init(&list);
    for (int v = 1; v <= 3; v++)
    {
        assert(dcll_insertAtEnd(&list, v) == 1);
    }

    assert(dcll_deleteAtBeginning(&list) == 1);
    assert(list.head->data == 2);
    assert_invariant(&list);

    assert(dcll_deleteAtEnd(&list) == 1);
    assert(list.tail->data == 2);
    assert(list.head == list.tail); // back down to a single node
    assert_invariant(&list);

    assert(dcll_deleteAtBeginning(&list) == 1);
    assert(list.head == NULL && list.tail == NULL && list.length == 0);

    dcll_destroy(&list);
    printf("dcll delete begin/end tests passed\n");
}

void test_delete_by_value()
{
    dcll list;
    dcll_init(&list);
    for (int v = 1; v <= 4; v++)
    {
        assert(dcll_insertAtEnd(&list, v) == 1);
    }

    // Delete an interior value.
    assert(dcll_deleteByValue(&list, 2) == 1);
    assert_invariant(&list);

    // Delete the tail value: tail must be updated.
    assert(dcll_deleteByValue(&list, 4) == 1);
    assert(list.tail->data == 3);
    assert_invariant(&list);

    // Delete the head value.
    assert(dcll_deleteByValue(&list, 1) == 1);
    assert(list.head->data == 3);
    assert_invariant(&list);

    // Value not present.
    assert(dcll_deleteByValue(&list, 99) == -1);

    int out[2];
    int n = list_to_array(&list, out, 2);
    assert(n == 1 && out[0] == 3);

    dcll_destroy(&list);
    printf("dcll delete by value tests passed\n");
}

void test_delete_at_position()
{
    dcll list;
    dcll_init(&list);
    for (int v = 1; v <= 4; v++)
    {
        assert(dcll_insertAtEnd(&list, v) == 1);
    }

    // Delete head via position 0.
    assert(dcll_deleteAtPosition(&list, 0) == 1);
    // Delete the new last node (position length-1) -> tail update.
    assert(dcll_deleteAtPosition(&list, list.length - 1) == 1);
    assert(list.tail->data == 3);
    assert_invariant(&list);

    int out[2];
    int n = list_to_array(&list, out, 2);
    assert(n == 2 && out[0] == 2 && out[1] == 3);

    // Invalid positions.
    assert(dcll_deleteAtPosition(&list, 10) == -2);
    assert(dcll_deleteAtPosition(&list, -1) == -2);

    dcll_destroy(&list);
    printf("dcll delete at position tests passed\n");
}

void test_search_and_length()
{
    dcll list;
    dcll_init(&list);
    assert(dcll_getLength(&list) == 0);

    int values[] = {5, 10, 15};
    for (int i = 0; i < 3; i++)
    {
        assert(dcll_insertAtEnd(&list, values[i]) == 1);
    }
    assert(dcll_getLength(&list) == 3);

    assert(dcll_search(&list, 5) == 0);
    assert(dcll_search(&list, 15) == 2);
    assert(dcll_search(&list, 100) == -1);

    dcll_destroy(&list);
    printf("dcll search and length tests passed\n");
}

void test_edge_cases()
{
    dcll list;
    dcll_init(&list);

    // Operations on an empty list return the documented error codes.
    assert(dcll_deleteAtBeginning(&list) == -1);
    assert(dcll_deleteAtEnd(&list) == -1);
    assert(dcll_deleteByValue(&list, 10) == -2);
    assert(dcll_deleteAtPosition(&list, 0) == -1); // empty list -> -1 (not an invalid-position -2)
    assert(dcll_search(&list, 10) == -1);
    assert(dcll_getLength(&list) == 0);

    // Single-node list: node points to itself in both directions, head == tail.
    assert(dcll_insertAtBeginning(&list, 42) == 1);
    assert(list.head == list.tail);
    assert(list.head->next == list.head);
    assert(list.head->prev == list.head);
    assert(dcll_search(&list, 42) == 0);
    assert_invariant(&list);

    // Delete down to empty, then reuse the same list object.
    assert(dcll_deleteAtEnd(&list) == 1);
    assert(list.head == NULL && list.length == 0);
    assert(dcll_insertAtEnd(&list, 7) == 1);
    assert(list.head->data == 7);
    assert_invariant(&list);

    dcll_destroy(&list);
    printf("dcll edge case tests passed\n");
}

void test_prev_pointers()
{
    dcll list;
    dcll_init(&list);

    // Specifically test that prev pointers are correct after various operations.
    for (int v = 1; v <= 5; v++)
    {
        assert(dcll_insertAtEnd(&list, v) == 1);
    }

    // Walk backward and verify order is reversed.
    int rev[5];
    int n = list_to_array_reverse(&list, rev, 5);
    assert(n == 5);
    assert(rev[0] == 5 && rev[1] == 4 && rev[2] == 3 && rev[3] == 2 && rev[4] == 1);
    assert_invariant(&list);

    // Delete middle node and re-verify backward walk.
    assert(dcll_deleteByValue(&list, 3) == 1);
    int rev2[4];
    int m = list_to_array_reverse(&list, rev2, 4);
    assert(m == 4);
    assert(rev2[0] == 5 && rev2[1] == 4 && rev2[2] == 2 && rev2[3] == 1);
    assert_invariant(&list);

    dcll_destroy(&list);
    printf("dcll prev pointer tests passed\n");
}

void test_destroy_is_idempotent()
{
    dcll list;
    dcll_init(&list);
    for (int v = 1; v <= 3; v++)
    {
        assert(dcll_insertAtEnd(&list, v) == 1);
    }

    dcll_destroy(&list);
    assert(list.head == NULL && list.tail == NULL && list.length == 0);
    // Destroying an already-empty list is a safe no-op.
    dcll_destroy(&list);
    assert(list.head == NULL);

    printf("dcll destroy tests passed\n");
}

int main()
{
    test_insert_begin_end();
    test_insert_at_position();
    test_delete_begin_end();
    test_delete_by_value();
    test_delete_at_position();
    test_search_and_length();
    test_edge_cases();
    test_prev_pointers();
    test_destroy_is_idempotent();

    printf("All DCLL tests passed\n");
    return 0;
}