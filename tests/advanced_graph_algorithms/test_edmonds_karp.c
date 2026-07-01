#include "../../src/advanced_graph_algorithms/edmonds_karp.c"
#include "advanced_graph_algorithms.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_ek_simple()
{
    printf("Running test_ek_simple...\n");
    weightedGraph* g = create_weightedGraph(4);
    add_edge_directed(g, 0, 1, 3);
    add_edge_directed(g, 0, 2, 2);
    add_edge_directed(g, 1, 3, 2);
    add_edge_directed(g, 2, 3, 3);
    add_edge_directed(g, 1, 2, 1);

    int flow = edmonds_karp(g, 0, 3);
    assert(flow == 5);

    free_weightedGraph(g);
    printf("test_ek_simple passed.\n");
}

int main()
{
    test_ek_simple();
    printf("All Edmonds-Karp Tests Passed!\n");
    return 0;
}
