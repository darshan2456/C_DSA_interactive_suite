#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_traversals.h"

#include "mock_printf.h"

// Redirect printf to our mock
#define printf mock_printf
#include "../src/graph_traversals/topological_sort.c"
#undef printf

void test_topological_sort_dag()
{
    reset_printf_buf();

    // Create a graph with 3 vertices
    Graph* graph = create_graph(3);
    assert(graph != NULL);

    // Add directed unweighted edges: 0 -> 1, 1 -> 2
    add_edge_directed_unweighted(graph, 0, 1);
    add_edge_directed_unweighted(graph, 1, 2);

    topological_sort_kahn(graph);

    // Topological Order should be: 0 1 2
    assert(strstr(g_printf_buf, "0 1 2") != NULL);
    assert(strstr(g_printf_buf, "valid DAG") != NULL);

    free_graph(graph);
}

void test_topological_sort_cycle()
{
    reset_printf_buf();

    Graph* graph = create_graph(3);
    assert(graph != NULL);

    // Create a cycle: 0 -> 1, 1 -> 2, 2 -> 0
    add_edge_directed_unweighted(graph, 0, 1);
    add_edge_directed_unweighted(graph, 1, 2);
    add_edge_directed_unweighted(graph, 2, 0);

    topological_sort_kahn(graph);

    // Should detect cycle
    assert(strstr(g_printf_buf, "Cycle detected!") != NULL);
    assert(strstr(g_printf_buf, "NOT a DAG") != NULL);

    free_graph(graph);
}

int main()
{
    test_topological_sort_dag();
    test_topological_sort_cycle();

    fprintf(stdout, "All Topological Sort tests passed\n");
    return 0;
}
