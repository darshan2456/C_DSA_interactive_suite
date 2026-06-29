#include "../../src/graph_traversals/bipartite_matching.c"
#include "graph_traversals.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_matching_simple()
{
    printf("Running test_matching_simple...\n");
    Graph* g = create_graph(4);
    add_edge_undirected(g, 0, 2);
    add_edge_undirected(g, 0, 3);
    add_edge_undirected(g, 1, 2);

    int match_count = 0;
    int* match_pairs = NULL;
    int res = max_bipartite_matching(g, &match_pairs, &match_count);

    assert(res == 2);
    assert(match_count == 2);
    assert(match_pairs != NULL);

    free(match_pairs);
    free_graph(g);
    printf("test_matching_simple passed.\n");
}

int main()
{
    test_matching_simple();
    printf("All Bipartite Matching Tests Passed!\n");
    return 0;
}
