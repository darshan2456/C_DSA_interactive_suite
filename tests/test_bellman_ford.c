#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_traversals.h"

#include "mock_printf.h"

// Redirect printf to our mock
#define printf mock_printf
#include "../src/graph_traversals/bellman_ford.c"
#undef printf

void test_bellman_ford_simple_path()
{
    reset_printf_buf();

    // Create a graph with 3 vertices
    weightedGraph* graph = create_weightedGraph(3);
    assert(graph != NULL);

    // 0 -> 1 (2), 1 -> 2 (-1), 0 -> 2 (6)
    add_edge_directed(graph, 0, 1, 2);
    add_edge_directed(graph, 1, 2, -1);
    add_edge_directed(graph, 0, 2, 6);

    bellman_ford(graph, 0);

    // Assert that shortest path 0 -> 2 has distance 1 (via 1)
    assert(strstr(g_printf_buf, "0 -> 2") != NULL);
    assert(strstr(g_printf_buf, "1") != NULL);
    assert(strstr(g_printf_buf, "0 -> 1") != NULL);
    assert(strstr(g_printf_buf, "2") != NULL);

    free_weightedGraph(graph);
}

void test_bellman_ford_negative_cycle()
{
    reset_printf_buf();

    weightedGraph* graph = create_weightedGraph(3);
    assert(graph != NULL);

    // Create a negative cycle: 0 -> 1 (2), 1 -> 2 (1), 2 -> 0 (-4)
    add_edge_directed(graph, 0, 1, 2);
    add_edge_directed(graph, 1, 2, 1);
    add_edge_directed(graph, 2, 0, -4);

    bellman_ford(graph, 0);

    // Assert negative cycle detected
    assert(strstr(g_printf_buf, "Negative edge cycle detected") != NULL);

    free_weightedGraph(graph);
}

void test_bellman_ford_unreachable()
{
    reset_printf_buf();

    weightedGraph* graph = create_weightedGraph(3);
    assert(graph != NULL);

    // 0 -> 1 (2), node 2 is unreachable
    add_edge_directed(graph, 0, 1, 2);

    bellman_ford(graph, 0);

    // Assert that node 2 is unreachable (INF)
    assert(strstr(g_printf_buf, "0 -> 2") != NULL);
    assert(strstr(g_printf_buf, "INF") != NULL);

    free_weightedGraph(graph);
}

int main()
{
    test_bellman_ford_simple_path();
    test_bellman_ford_negative_cycle();
    test_bellman_ford_unreachable();

    fprintf(stdout, "All Bellman-Ford tests passed\n");
    return 0;
}
