#include "../../src/graph_traversals/eulerian_path.c"
#include "graph_traversals.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_eulerian_circuit()
{
    printf("Running test_eulerian_circuit...\n");
    Graph* g = create_graph(3);
    add_edge_directed_unweighted(g, 0, 1);
    add_edge_directed_unweighted(g, 1, 2);
    add_edge_directed_unweighted(g, 2, 0);

    int path_len = 0;
    int* path = NULL;
    int res = find_eulerian_path(g, &path, &path_len);

    assert(res == 1);
    assert(path_len == 4);
    assert(path != NULL);
    assert(path[0] == 0);
    assert(path[1] == 1);
    assert(path[2] == 2);
    assert(path[3] == 0);

    free(path);
    free_graph(g);
    printf("test_eulerian_circuit passed.\n");
}

int main()
{
    test_eulerian_circuit();
    printf("All Eulerian Path Tests Passed!\n");
    return 0;
}
