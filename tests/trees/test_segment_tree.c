#include "trees.h"
#include <assert.h>
#include <stdio.h>

void test_create_and_destroy_valid(void)
{
    int arr[] = {1, 2, 3};
    SegmentTree* st = create_segment_tree(arr, 3);
    assert(st != NULL);
    assert(st->original_array_size == 3);
    assert(st->size == 12);
    assert(st->tree != NULL);
    for (int i = 0; i < 12; i++)
    {
        assert(st->tree[i] == 0);
    }
    destroy_segment_tree(st);
    printf("test_create_and_destroy_valid passed.\n");
}

void test_create_invalid_size(void)
{
    SegmentTree* st1 = create_segment_tree(NULL, 0);
    assert(st1 == NULL);

    SegmentTree* st2 = create_segment_tree(NULL, -5);
    assert(st2 == NULL);

    destroy_segment_tree(NULL); // Should safely do nothing
    printf("test_create_invalid_size passed.\n");
}

void test_build_tree_positive(void)
{
    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = 6;
    SegmentTree* st = create_segment_tree(arr, n);
    assert(st != NULL);
    build_tree(st, arr, 0, 0, n - 1);

    // Assert mathematically correct values for a segment tree
    // Root should have sum of all elements: 1+3+5+7+9+11 = 36
    assert(st->tree[0] == 36);
    // Left child (range 0-2): 1+3+5 = 9
    assert(st->tree[1] == 9);
    // Right child (range 3-5): 7+9+11 = 27
    assert(st->tree[2] == 27);

    destroy_segment_tree(st);
    printf("test_build_tree_positive passed.\n");
}

void test_build_tree_mixed(void)
{
    int arr[] = {-1, 3, -5, 7, 0, 11};
    int n = 6;
    SegmentTree* st = create_segment_tree(arr, n);
    assert(st != NULL);
    build_tree(st, arr, 0, 0, n - 1);

    // Root sum: -1 + 3 - 5 + 7 + 0 + 11 = 15
    assert(st->tree[0] == 15);
    // Left child (range 0-2): -1 + 3 - 5 = -3
    assert(st->tree[1] == -3);
    // Right child (range 3-5): 7 + 0 + 11 = 18
    assert(st->tree[2] == 18);

    destroy_segment_tree(st);
    printf("test_build_tree_mixed passed.\n");
}

void test_query_range(void)
{
    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = 6;
    SegmentTree* st = create_segment_tree(arr, n);
    assert(st != NULL);
    build_tree(st, arr, 0, 0, n - 1);

    // Test full range query
    assert(query_range(st, 0, 0, n - 1, 0, 5) == 36);

    // Test sub-range queries
    assert(query_range(st, 0, 0, n - 1, 1, 3) == 15); // 3 + 5 + 7
    assert(query_range(st, 0, 0, n - 1, 2, 4) == 21); // 5 + 7 + 9
    assert(query_range(st, 0, 0, n - 1, 2, 2) == 5);  // Single element

    // Test out-of-bounds ranges
    assert(query_range(st, 0, 0, n - 1, -5, -1) == 0);  // Entirely left
    assert(query_range(st, 0, 0, n - 1, 6, 10) == 0);   // Entirely right
    assert(query_range(st, 0, 0, n - 1, -5, 10) == 36); // Enclosing range
    assert(query_range(st, 0, 0, n - 1, -2, 2) == 9);   // Overlapping left: elements 0 to 2
    assert(query_range(st, 0, 0, n - 1, 4, 12) == 20);  // Overlapping right: elements 4 to 5

    // Test invalid range (left > right)
    assert(query_range(st, 0, 0, n - 1, 4, 2) == 0);

    destroy_segment_tree(st);
    printf("test_query_range passed.\n");
}

void test_update_point(void)
{
    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = 6;
    SegmentTree* st = create_segment_tree(arr, n);
    assert(st != NULL);
    build_tree(st, arr, 0, 0, n - 1);

    // Update index 2 from 5 to 10
    update_point(st, 0, 0, n - 1, 2, 10);
    assert(query_range(st, 0, 0, n - 1, 2, 2) == 10);
    assert(query_range(st, 0, 0, n - 1, 1, 3) == 20); // 3 + 10 + 7
    assert(query_range(st, 0, 0, n - 1, 0, 5) == 41); // 36 - 5 + 10

    // Update index 5 from 11 to -3
    update_point(st, 0, 0, n - 1, 5, -3);
    assert(query_range(st, 0, 0, n - 1, 5, 5) == -3);
    assert(query_range(st, 0, 0, n - 1, 4, 5) == 6);  // 9 + (-3)
    assert(query_range(st, 0, 0, n - 1, 0, 5) == 27); // 41 - 11 - 3

    destroy_segment_tree(st);
    printf("test_update_point passed.\n");
}

void test_traversals(void)
{
    int arr[] = {1, 3, 5};
    int n = 3;
    SegmentTree* st = create_segment_tree(arr, n);
    assert(st != NULL);
    build_tree(st, arr, 0, 0, n - 1);

    // We call traversal functions to ensure they complete without crashing or invalid accesses
    printf("Preorder traversal output: ");
    preorder_traversal(st, 0, 0, n - 1);
    printf("\n");

    printf("Inorder traversal output: ");
    inorder_traversal(st, 0, 0, n - 1);
    printf("\n");

    printf("Postorder traversal output: ");
    postorder_traversal(st, 0, 0, n - 1);
    printf("\n");

    destroy_segment_tree(st);
    printf("test_traversals passed.\n");
}

int main(void)
{
    test_create_and_destroy_valid();
    test_create_invalid_size();
    test_build_tree_positive();
    test_build_tree_mixed();
    test_query_range();
    test_update_point();
    test_traversals();

    printf("All segment tree tests passed successfully.\n");
    return 0;
}
