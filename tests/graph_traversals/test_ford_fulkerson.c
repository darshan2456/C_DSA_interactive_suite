#include "../../src/graph_traversals/ford_fulkerson.c"
#include "graph_traversals.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_ff_simple()
{
    printf("Running test_ff_simple...\n");
    weightedGraph* g = create_weightedGraph(4);
    add_edge_directed(g, 0, 1, 3);
    add_edge_directed(g, 0, 2, 2);
    add_edge_directed(g, 1, 3, 2);
    add_edge_directed(g, 2, 3, 3);
    add_edge_directed(g, 1, 2, 1);

    int flow = ford_fulkerson(g, 0, 3);
    assert(flow == 5);

    free_weightedGraph(g);
    printf("test_ff_simple passed.\n");
}

int main()
{
    test_ff_simple();
    printf("All Ford-Fulkerson Tests Passed!\n");
    return 0;
}
