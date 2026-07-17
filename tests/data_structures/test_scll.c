#include "scll.h"
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
        arr[i++] = *(int*)(cur->data);
        cur = cur->next;
    }
    return i;
}

static int compare_ints(const void* a, const void* b)
{
    return *(const int*)a - *(const int*)b;
}

void test_insert_begin_end()
{
    scll list;
    scll_init(&list);

    int* val10 = malloc(sizeof(int));
    *val10 = 10;
    assert(scll_insertAtEnd(&list, val10) == 1);
    // Single node points to itself and is both head and tail.
    assert(list.head == list.tail);
    assert(list.head->next == list.head);
    assert_invariant(&list);

    int* val20 = malloc(sizeof(int));
    *val20 = 20;
    assert(scll_insertAtEnd(&list, val20) == 1);
    
    int* val5 = malloc(sizeof(int));
    *val5 = 5;
    assert(scll_insertAtBeginning(&list, val5) == 1);
    assert_invariant(&list);

    int out[3];
    int n = list_to_array(&list, out, 3);
    assert(n == 3);
    assert(out[0] == 5 && out[1] == 10 && out[2] == 20);
    assert(*(int*)(list.head->data) == 5);
    assert(*(int*)(list.tail->data) == 20);

    scll_destroy(&list, free);
    printf("scll insert begin/end tests passed\n");
}

void test_insert_at_position()
{
    scll list;
    scll_init(&list);

    // Insert into empty list at position 0.
    int* val10 = malloc(sizeof(int));
    *val10 = 10;
    assert(scll_insertAtPosition(&list, val10, 0) == 1);
    // Append at the end (position == length).
    int* val30 = malloc(sizeof(int));
    *val30 = 30;
    assert(scll_insertAtPosition(&list, val30, 1) == 1);
    // Interior insert.
    int* val20 = malloc(sizeof(int));
    *val20 = 20;
    assert(scll_insertAtPosition(&list, val20, 1) == 1);
    assert_invariant(&list);

    int out[3];
    int n = list_to_array(&list, out, 3);
    assert(n == 3);
    assert(out[0] == 10 && out[1] == 20 && out[2] == 30);
    assert(*(int*)(list.tail->data) == 30);

    // Invalid positions.
    int* val99 = malloc(sizeof(int));
    *val99 = 99;
    assert(scll_insertAtPosition(&list, val99, 10) == -2);
    assert(scll_insertAtPosition(&list, val99, -1) == -2);
    free(val99);
    assert_invariant(&list);

    scll_destroy(&list, free);
    printf("scll insert at position tests passed\n");
}

void test_delete_begin_end()
{
    scll list;
    scll_init(&list);
    for (int v = 1; v <= 3; v++)
    {
        int* val = malloc(sizeof(int));
        *val = v;
        assert(scll_insertAtEnd(&list, val) == 1);
    }

    assert(scll_deleteAtBeginning(&list, free) == 1);
    assert(*(int*)(list.head->data) == 2);
    assert_invariant(&list);

    assert(scll_deleteAtEnd(&list, free) == 1);
    assert(*(int*)(list.tail->data) == 2);
    assert(list.head == list.tail); // back down to a single node
    assert_invariant(&list);

    assert(scll_deleteAtBeginning(&list, free) == 1);
    assert(list.head == NULL && list.tail == NULL && list.length == 0);

    scll_destroy(&list, free);
    printf("scll delete begin/end tests passed\n");
}

void test_delete_by_value()
{
    scll list;
    scll_init(&list);
    for (int v = 1; v <= 4; v++)
    {
        int* val = malloc(sizeof(int));
        *val = v;
        assert(scll_insertAtEnd(&list, val) == 1);
    }

    // Delete an interior value.
    int key2 = 2;
    assert(scll_deleteByValue(&list, &key2, compare_ints, free) == 1);
    assert_invariant(&list);

    // Delete the tail value: tail must be updated.
    int key4 = 4;
    assert(scll_deleteByValue(&list, &key4, compare_ints, free) == 1);
    assert(*(int*)(list.tail->data) == 3);
    assert_invariant(&list);

    // Delete the head value.
    int key1 = 1;
    assert(scll_deleteByValue(&list, &key1, compare_ints, free) == 1);
    assert(*(int*)(list.head->data) == 3);
    assert_invariant(&list);

    // Value not present.
    int key99 = 99;
    assert(scll_deleteByValue(&list, &key99, compare_ints, free) == -1);

    int out[2];
    int n = list_to_array(&list, out, 2);
    assert(n == 1 && out[0] == 3);

    scll_destroy(&list, free);
    printf("scll delete by value tests passed\n");
}

void test_delete_at_position()
{
    scll list;
    scll_init(&list);
    for (int v = 1; v <= 4; v++)
    {
        int* val = malloc(sizeof(int));
        *val = v;
        assert(scll_insertAtEnd(&list, val) == 1);
    }

    // Delete head via position 0.
    assert(scll_deleteAtPosition(&list, 0, free) == 1);
    // Delete the new last node (position length-1) -> tail update.
    assert(scll_deleteAtPosition(&list, list.length - 1, free) == 1);
    assert(*(int*)(list.tail->data) == 3);
    assert_invariant(&list);

    int out[2];
    int n = list_to_array(&list, out, 2);
    assert(n == 2 && out[0] == 2 && out[1] == 3);

    // Invalid positions.
    assert(scll_deleteAtPosition(&list, 10, free) == -2);
    assert(scll_deleteAtPosition(&list, -1, free) == -2);

    scll_destroy(&list, free);
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
        int* val = malloc(sizeof(int));
        *val = values[i];
        assert(scll_insertAtEnd(&list, val) == 1);
    }
    assert(scll_getLength(&list) == 3);

    int key5 = 5;
    int key15 = 15;
    int key100 = 100;
    assert(scll_search(&list, &key5, compare_ints) == 0);
    assert(scll_search(&list, &key15, compare_ints) == 2);
    assert(scll_search(&list, &key100, compare_ints) == -1);

    scll_destroy(&list, free);
    printf("scll search and length tests passed\n");
}

void test_edge_cases()
{
    scll list;
    scll_init(&list);

    // Operations on an empty list return the documented error codes.
    assert(scll_deleteAtBeginning(&list, free) == -1);
    assert(scll_deleteAtEnd(&list, free) == -1);
    int key10 = 10;
    assert(scll_deleteByValue(&list, &key10, compare_ints, free) == -2);
    assert(scll_deleteAtPosition(&list, 0, free) == -1); // empty list -> -1 (not an invalid-position -2)
    assert(scll_search(&list, &key10, compare_ints) == -1);
    assert(scll_getLength(&list) == 0);

    // Single-node list: node points to itself, head == tail.
    int* val42 = malloc(sizeof(int));
    *val42 = 42;
    assert(scll_insertAtBeginning(&list, val42) == 1);
    assert(list.head == list.tail);
    assert(list.head->next == list.head);
    int key42 = 42;
    assert(scll_search(&list, &key42, compare_ints) == 0);
    assert_invariant(&list);

    // Delete down to empty, then reuse the same list object.
    assert(scll_deleteAtEnd(&list, free) == 1);
    assert(list.head == NULL && list.length == 0);
    int* val7 = malloc(sizeof(int));
    *val7 = 7;
    assert(scll_insertAtEnd(&list, val7) == 1);
    assert(*(int*)(list.head->data) == 7);
    assert_invariant(&list);

    scll_destroy(&list, free);
    printf("scll edge case tests passed\n");
}

void test_destroy_is_idempotent()
{
    scll list;
    scll_init(&list);
    for (int v = 1; v <= 3; v++)
    {
        int* val = malloc(sizeof(int));
        *val = v;
        assert(scll_insertAtEnd(&list, val) == 1);
    }

    scll_destroy(&list, free);
    assert(list.head == NULL && list.tail == NULL && list.length == 0);
    // Destroying an already-empty list is a safe no-op.
    scll_destroy(&list, free);
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
