#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_traversals.h"

#include "mock_printf.h"

// Redirect printf to our mock
#define printf mock_printf
#include "../src/graph_traversals/bfs.c"
#undef printf

void test_bfs_simple()
{
    reset_printf_buf();

    // Create a graph with 4 vertices
    Graph* graph = create_graph(4);
    assert(graph != NULL);

    // Add undirected edges: 0-1, 0-2, 1-3, 2-3
    add_edge_undirected(graph, 0, 1);
    add_edge_undirected(graph, 0, 2);
    add_edge_undirected(graph, 1, 3);
    add_edge_undirected(graph, 2, 3);

    bfs(graph, 0);

    // BFS order should print: 0->1->2->3->end
    assert(strstr(g_printf_buf, "0->1->2->3->end") != NULL);

    free_graph(graph);
}

void test_bfs_disconnected()
{
    reset_printf_buf();

    Graph* graph = create_graph(4);
    assert(graph != NULL);

    // Only add 0-1 edge, 2 and 3 are disconnected
    add_edge_undirected(graph, 0, 1);

    bfs(graph, 0);

    // BFS order starting from 0 should only traverse 0 and 1: 0->1->end
    assert(strstr(g_printf_buf, "0->1->end") != NULL);

    free_graph(graph);
}

int main()
{
    test_bfs_simple();
    test_bfs_disconnected();

    fprintf(stdout, "All BFS tests passed\n");
    return 0;
}
