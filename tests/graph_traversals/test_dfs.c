#include <assert.h>
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
#include "../src/graph_traversals/dfs.c"
#undef printf

void test_dfs_simple()
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

    dfs(graph, 0);

    // Assert that the DFS traverses all nodes starting with 0 and ending with end
    assert(strstr(g_printf_buf, "0->") != NULL);
    assert(strstr(g_printf_buf, "1->") != NULL);
    assert(strstr(g_printf_buf, "2->") != NULL);
    assert(strstr(g_printf_buf, "3->") != NULL);
    assert(strstr(g_printf_buf, "end") != NULL);

    free_graph(graph);
}

void test_dfs_disconnected()
{
    reset_printf_buf();

    Graph* graph = create_graph(4);
    assert(graph != NULL);

    // Only add 0-1 edge, 2 and 3 are disconnected
    add_edge_undirected(graph, 0, 1);

    dfs(graph, 0);

    // DFS order starting from 0 should only traverse 0 and 1: e.g. 0->1->end
    assert(strstr(g_printf_buf, "0->1->end") != NULL);

    free_graph(graph);
}

int main()
{
    test_dfs_simple();
    test_dfs_disconnected();

    fprintf(stdout, "All DFS tests passed\n");
    return 0;
}
