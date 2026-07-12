// Trigger build test
#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

// Add this line right here to silence the unused parameter error
#pragma GCC diagnostic ignored "-Wunused-parameter"

SegmentTree* create_segment_tree(int arr[], int n)
{
    if (n <= 0)
        return NULL;
    SegmentTree* st = (SegmentTree*)malloc(sizeof(SegmentTree));
    if (st == NULL)
    {
        fprintf(stderr, "Memory allocation failed for SegmentTree struct\n");
        return NULL;
    }
    st->original_array_size = n;
    st->size = 4 * n;
    st->tree = (int*)malloc(st->size * sizeof(int));
    if (st->tree == NULL)
    {
        fprintf(stderr, "Memory allocation failed for tree array\n");
        free(st);
        return NULL;
    }
    for (int i = 0; i < st->size; i++)
        st->tree[i] = 0;
    return st;
}

void destroy_segment_tree(SegmentTree* st)
{
    if (st != NULL)
    {
        if (st->tree != NULL)
            free(st->tree);
        free(st);
    }
}

void build_tree(SegmentTree* st, int arr[], int node, int start, int end)
{
    if (start == end)
    {
        st->tree[node] = arr[start];
        return;
    }
    int mid = start + (end - start) / 2;
    int left_child = 2 * node + 1;
    int right_child = 2 * node + 2;
    build_tree(st, arr, left_child, start, mid);
    build_tree(st, arr, right_child, mid + 1, end);
    st->tree[node] = st->tree[left_child] + st->tree[right_child];
}

void preorder_traversal(SegmentTree* st, int node, int start, int end)
{
    if (start > end || node >= st->size)
        return;
    printf("%d ", st->tree[node]);
    if (start != end)
    {
        int mid = start + (end - start) / 2;
        preorder_traversal(st, 2 * node + 1, start, mid);
        preorder_traversal(st, 2 * node + 2, mid + 1, end);
    }
}

void inorder_traversal(SegmentTree* st, int node, int start, int end)
{
    if (start > end || node >= st->size)
        return;
    if (start != end)
    {
        int mid = start + (end - start) / 2;
        inorder_traversal(st, 2 * node + 1, start, mid);
    }
    printf("%d ", st->tree[node]);
    if (start != end)
    {
        int mid = start + (end - start) / 2;
        inorder_traversal(st, 2 * node + 2, mid + 1, end);
    }
}

void postorder_traversal(SegmentTree* st, int node, int start, int end)
{
    if (start > end || node >= st->size)
        return;
    if (start != end)
    {
        int mid = start + (end - start) / 2;
        postorder_traversal(st, 2 * node + 1, start, mid);
        postorder_traversal(st, 2 * node + 2, mid + 1, end);
    }
    printf("%d ", st->tree[node]);
}

void update_point(SegmentTree* st, int node, int start, int end, int idx, int val)
{
    if (start == end)
    {
        st->tree[node] = val;
        return;
    }
    int mid = start + (end - start) / 2;
    int left_child = 2 * node + 1;
    int right_child = 2 * node + 2;
    if (start <= idx && idx <= mid)
    {
        update_point(st, left_child, start, mid, idx, val);
    }
    else
    {
        update_point(st, right_child, mid + 1, end, idx, val);
    }
    st->tree[node] = st->tree[left_child] + st->tree[right_child];
}

int query_range(SegmentTree* st, int node, int start, int end, int l, int r)
{
    if (r < start || end < l)
        return 0;
    if (l <= start && end <= r)
        return st->tree[node];
    int mid = start + (end - start) / 2;
    int p1 = query_range(st, 2 * node + 1, start, mid, l, r);
    int p2 = query_range(st, 2 * node + 2, mid + 1, end, l, r);
    return p1 + p2;
}
