#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

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