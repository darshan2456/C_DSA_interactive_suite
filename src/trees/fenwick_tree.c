#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

FenwickTree* create_fenwick_tree(int size)
{
    if (size <= 0)
        return NULL;
    FenwickTree* ft = (FenwickTree*)malloc(sizeof(FenwickTree));
    if (ft == NULL)
        return NULL;
    ft->size = size;
    // 1-based indexing for BIT
    ft->BIT1 = (int*)calloc(size + 1, sizeof(int));
    ft->BIT2 = (int*)calloc(size + 1, sizeof(int));
    if (ft->BIT1 == NULL || ft->BIT2 == NULL)
    {
        free(ft->BIT1);
        free(ft->BIT2);
        free(ft);
        return NULL;
    }
    return ft;
}

void destroy_fenwick_tree(FenwickTree* ft)
{
    if (ft)
    {
        free(ft->BIT1);
        free(ft->BIT2);
        free(ft);
    }
}

// Point update on a single BIT
void fenwick_point_update(int* BIT, int n, int idx, int delta)
{
    if (BIT == NULL || idx <= 0)
        return;
    for (; idx <= n; idx += idx & -idx)
    {
        BIT[idx] += delta;
    }
}

// Dual-BIT Range Update [l, r] by delta
void fenwick_range_update(FenwickTree* ft, int l, int r, int delta)
{
    if (ft == NULL || ft->BIT1 == NULL || ft->BIT2 == NULL || l <= 0 || r < l || r > ft->size)
        return;
    int n = ft->size;
    fenwick_point_update(ft->BIT1, n, l, delta);
    fenwick_point_update(ft->BIT1, n, r + 1, -delta);

    fenwick_point_update(ft->BIT2, n, l, delta * (l - 1));
    fenwick_point_update(ft->BIT2, n, r + 1, -delta * r);
}

// Point query on a single BIT
int fenwick_point_query(int* BIT, int idx)
{
    if (BIT == NULL)
        return 0;
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx)
    {
        sum += BIT[idx];
    }
    return sum;
}

// Query prefix sum from 1 to x
static int fenwick_prefix_sum(FenwickTree* ft, int x)
{
    if (ft == NULL || ft->BIT1 == NULL || ft->BIT2 == NULL)
        return 0;
    return fenwick_point_query(ft->BIT1, x) * x - fenwick_point_query(ft->BIT2, x);
}

// Range Query [l, r]
int fenwick_range_query(FenwickTree* ft, int l, int r)
{
    if (ft == NULL || ft->BIT1 == NULL || ft->BIT2 == NULL || l <= 0 || r < l || r > ft->size)
        return 0;
    return fenwick_prefix_sum(ft, r) - fenwick_prefix_sum(ft, l - 1);
}

void fenwick_tree_demo(void)
{
    printf("\n--- Fenwick Tree (Dual-BIT) Range Update / Range Query Demo ---\n");
    int n = 5;
    printf("Created Fenwick Tree of size %d initialized to 0.\n", n);
    FenwickTree* ft = create_fenwick_tree(n);
    if (ft == NULL)
    {
        printf("Failed to create Fenwick Tree due to memory allocation failure.\n");
        return;
    }

    printf("Action: Adding 10 to range [1, 3]\n");
    fenwick_range_update(ft, 1, 3, 10);

    printf("Action: Adding 5 to range [2, 4]\n");
    fenwick_range_update(ft, 2, 4, 5);

    printf("\nQueries:\n");
    printf("Sum of range [1, 3]: %d (Expected: 40)\n", fenwick_range_query(ft, 1, 3));
    printf("Sum of range [2, 5]: %d (Expected: 35)\n", fenwick_range_query(ft, 2, 5));
    printf("Sum of range [1, 5]: %d (Expected: 45)\n", fenwick_range_query(ft, 1, 5));

    destroy_fenwick_tree(ft);
    printf("-----------------------------------------------------------\n\n");
}