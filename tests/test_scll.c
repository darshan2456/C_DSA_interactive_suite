#include "data_structures.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Independently counts the ring by walking at most one full lap. Returns the node count and,
// via *invariant_ok, whether tail->next == head currently holds. Walking is bounded: it stops
// when it returns to head, or after length+1 hops as a safety cap so a broken ring can never
// loop forever inside the tests themselves.
static int ring_count(const scll* list, int* invariant_ok)
{
    if (list->head == NULL)
    {
        *invariant_ok = (list->tail == NULL);
        return 0;
    }

    *invariant_ok = (list->tail->next == list->head);

    int count = 0;
    const scll_Node* cur = list->head;
    do
    {
        count++;
        cur = cur->next;
    } while (cur != list->head && count <= list->length + 1);

    return count;
}

// Asserts the structural invariant after a mutation: tail->next == head, and the cached length
// matches an independent walk of the ring.
static void assert_invariant(const scll* list)
{
    int invariant_ok = 0;
    int counted = ring_count(list, &invariant_ok);
    assert(invariant_ok);
    assert(counted == list->length);
}

// Copies the list into arr by walking exactly `length` nodes. The loop is bounded by the cached
// length, which proves termination on a circular structure. Returns the number of items copied.
static int list_to_array(const scll* list, int arr[], int max)
{
    int i = 0;
    if (list->head == NULL)
    {
        return 0;
    }
    const scll_Node* cur = list->head;
    while (i < list->length && i < max)
    {
        arr[i++] = cur->data;
        cur = cur->next;
    }
    return i;
}

void test_insert_begin_end()
{
    scll list;
    scll_init(&list);

    assert(scll_insertAtEnd(&list, 10) == 1);
    // Single node points to itself and is both head and tail.
    assert(list.head == list.tail);
    assert(list.head->next == list.head);
    assert_invariant(&list);

    assert(scll_insertAtEnd(&list, 20) == 1);
    assert(scll_insertAtBeginning(&list, 5) == 1);
    assert_invariant(&list);

    int out[3];
    int n = list_to_array(&list, out, 3);
    assert(n == 3);
    assert(out[0] == 5 && out[1] == 10 && out[2] == 20);
    assert(list.head->data == 5);
    assert(list.tail->data == 20);

    scll_destroy(&list);
    printf("scll insert begin/end tests passed\n");
}

void test_insert_at_position()
{
    scll list;
    scll_init(&list);

    // Insert into empty list at position 0.
    assert(scll_insertAtPosition(&list, 10, 0) == 1);
    // Append at the end (position == length).
    assert(scll_insertAtPosition(&list, 30, 1) == 1);
    // Interior insert.
    assert(scll_insertAtPosition(&list, 20, 1) == 1);
    assert_invariant(&list);

    int out[3];
    int n = list_to_array(&list, out, 3);
    assert(n == 3);
    assert(out[0] == 10 && out[1] == 20 && out[2] == 30);
    assert(list.tail->data == 30);

    // Invalid positions.
    assert(scll_insertAtPosition(&list, 99, 10) == -2);
    assert(scll_insertAtPosition(&list, 99, -1) == -2);
    assert_invariant(&list);

    scll_destroy(&list);
    printf("scll insert at position tests passed\n");
}

void test_delete_begin_end()
{
    scll list;
    scll_init(&list);
    for (int v = 1; v <= 3; v++)
    {
        assert(scll_insertAtEnd(&list, v) == 1);
    }

    assert(scll_deleteAtBeginning(&list) == 1);
    assert(list.head->data == 2);
    assert_invariant(&list);

    assert(scll_deleteAtEnd(&list) == 1);
    assert(list.tail->data == 2);
    assert(list.head == list.tail); // back down to a single node
    assert_invariant(&list);

    assert(scll_deleteAtBeginning(&list) == 1);
    assert(list.head == NULL && list.tail == NULL && list.length == 0);

    scll_destroy(&list);
    printf("scll delete begin/end tests passed\n");
}

void test_delete_by_value()
{
    scll list;
    scll_init(&list);
    for (int v = 1; v <= 4; v++)
    {
        assert(scll_insertAtEnd(&list, v) == 1);
    }

    // Delete an interior value.
    assert(scll_deleteByValue(&list, 2) == 1);
    assert_invariant(&list);

    // Delete the tail value: tail must be updated.
    assert(scll_deleteByValue(&list, 4) == 1);
    assert(list.tail->data == 3);
    assert_invariant(&list);

    // Delete the head value.
    assert(scll_deleteByValue(&list, 1) == 1);
    assert(list.head->data == 3);
    assert_invariant(&list);

    // Value not present.
    assert(scll_deleteByValue(&list, 99) == -1);

    int out[2];
    int n = list_to_array(&list, out, 2);
    assert(n == 1 && out[0] == 3);

    scll_destroy(&list);
    printf("scll delete by value tests passed\n");
}

void test_delete_at_position()
{
    scll list;
    scll_init(&list);
    for (int v = 1; v <= 4; v++)
    {
        assert(scll_insertAtEnd(&list, v) == 1);
    }

    // Delete head via position 0.
    assert(scll_deleteAtPosition(&list, 0) == 1);
    // Delete the new last node (position length-1) -> tail update.
    assert(scll_deleteAtPosition(&list, list.length - 1) == 1);
    assert(list.tail->data == 3);
    assert_invariant(&list);

    int out[2];
    int n = list_to_array(&list, out, 2);
    assert(n == 2 && out[0] == 2 && out[1] == 3);

    // Invalid positions.
    assert(scll_deleteAtPosition(&list, 10) == -2);
    assert(scll_deleteAtPosition(&list, -1) == -2);

    scll_destroy(&list);
    printf("scll delete at position tests passed\n");
}

void test_search_and_length()
{
    scll list;
    scll_init(&list);
    assert(scll_getLength(&list) == 0);

    int values[] = {5, 10, 15};
    for (int i = 0; i < 3; i++)
    {
        assert(scll_insertAtEnd(&list, values[i]) == 1);
    }
    assert(scll_getLength(&list) == 3);

    assert(scll_search(&list, 5) == 0);
    assert(scll_search(&list, 15) == 2);
    assert(scll_search(&list, 100) == -1);

    scll_destroy(&list);
    printf("scll search and length tests passed\n");
}

void test_edge_cases()
{
    scll list;
    scll_init(&list);

    // Operations on an empty list return the documented error codes.
    assert(scll_deleteAtBeginning(&list) == -1);
    assert(scll_deleteAtEnd(&list) == -1);
    assert(scll_deleteByValue(&list, 10) == -2);
    assert(scll_deleteAtPosition(&list, 0) == -1); // empty list -> -1 (not an invalid-position -2)
    assert(scll_search(&list, 10) == -1);
    assert(scll_getLength(&list) == 0);

    // Single-node list: node points to itself, head == tail.
    assert(scll_insertAtBeginning(&list, 42) == 1);
    assert(list.head == list.tail);
    assert(list.head->next == list.head);
    assert(scll_search(&list, 42) == 0);
    assert_invariant(&list);

    // Delete down to empty, then reuse the same list object.
    assert(scll_deleteAtEnd(&list) == 1);
    assert(list.head == NULL && list.length == 0);
    assert(scll_insertAtEnd(&list, 7) == 1);
    assert(list.head->data == 7);
    assert_invariant(&list);

    scll_destroy(&list);
    printf("scll edge case tests passed\n");
}

void test_destroy_is_idempotent()
{
    scll list;
    scll_init(&list);
    for (int v = 1; v <= 3; v++)
    {
        assert(scll_insertAtEnd(&list, v) == 1);
    }

    scll_destroy(&list);
    assert(list.head == NULL && list.tail == NULL && list.length == 0);
    // Destroying an already-empty list is a safe no-op.
    scll_destroy(&list);
    assert(list.head == NULL);

    printf("scll destroy tests passed\n");
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
    test_destroy_is_idempotent();

    printf("All SCLL tests passed\n");
    return 0;
}
