#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_traversals.h"

// Buffer to capture printf outputs from the algorithm
static char g_printf_buf[4096];
static int g_printf_len = 0;

void reset_printf_buf()
{
    g_printf_buf[0] = '\0';
    g_printf_len = 0;
}

int mock_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    g_printf_len += vsnprintf(g_printf_buf + g_printf_len, sizeof(g_printf_buf) - g_printf_len, format, args);
    va_end(args);
    return 0;
}

// Redirect printf to our mock
#define printf mock_printf
#include "../src/graph_traversals/dijkstra.c"
#undef printf

void test_dijkstra_simple_path()
{
    reset_printf_buf();

    // Create a graph with 3 vertices
    weightedGraph* graph = create_weightedGraph(3);
    assert(graph != NULL);

    // 0 -> 1 (2), 1 -> 2 (3), 0 -> 2 (6)
    add_edge_directed(graph, 0, 1, 2);
    add_edge_directed(graph, 1, 2, 3);
    add_edge_directed(graph, 0, 2, 6);

    dijkstra(graph, 0);

    // Assert that shortest path 0 -> 2 has distance 5 (via 1) and not 6
    assert(strstr(g_printf_buf, "0 -> 2") != NULL);
    assert(strstr(g_printf_buf, "5") != NULL);
    assert(strstr(g_printf_buf, "0 -> 1") != NULL);
    assert(strstr(g_printf_buf, "2") != NULL);

    free_weightedGraph(graph);
}

void test_dijkstra_unreachable()
{
    reset_printf_buf();

    weightedGraph* graph = create_weightedGraph(3);
    assert(graph != NULL);

    // 0 -> 1 (2), node 2 is unreachable
    add_edge_directed(graph, 0, 1, 2);

    dijkstra(graph, 0);

    // Assert that node 2 is unreachable (INF)
    assert(strstr(g_printf_buf, "0 -> 2") != NULL);
    assert(strstr(g_printf_buf, "INF") != NULL);

    free_weightedGraph(graph);
}

void test_dijkstra_out_of_bounds()
{
    reset_printf_buf();

    weightedGraph* graph = create_weightedGraph(3);
    assert(graph != NULL);

    // Attempt to add out-of-bounds edge
    add_edge_directed(graph, 5, 1, 10);
    assert(strstr(g_printf_buf, "Invalid edge") != NULL);

    free_weightedGraph(graph);
}

int main()
{
    test_dijkstra_simple_path();
    test_dijkstra_unreachable();
    test_dijkstra_out_of_bounds();

    // Use stdout here since we undefined printf mock
    fprintf(stdout, "All Dijkstra tests passed\n");
    return 0;
}
