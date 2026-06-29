#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_traversals.h"
#include "../../src/graph_traversals/scc.c"

static bool in_same_scc(int** sccs, int* sizes, int count, int u, int v)
{
    for (int i = 0; i < count; i++)
    {
        bool has_u = false;
        bool has_v = false;
        for (int j = 0; j < sizes[i]; j++)
        {
            if (sccs[i][j] == u) has_u = true;
            if (sccs[i][j] == v) has_v = true;
        }
        if (has_u && has_v) return true;
    }
    return false;
}

void test_simple_cycle()
{
    printf("Running test_simple_cycle...\n");
    Graph* g = create_graph(3);
    add_edge_directed_unweighted(g, 0, 1);
    add_edge_directed_unweighted(g, 1, 2);
    add_edge_directed_unweighted(g, 2, 0);

    int tarjan_count = 0;
    int* tarjan_sizes = NULL;
    int** tarjan_sccs = find_scc_tarjan(g, &tarjan_count, &tarjan_sizes);

    assert(tarjan_count == 1);
    assert(tarjan_sizes[0] == 3);
    assert(in_same_scc(tarjan_sccs, tarjan_sizes, tarjan_count, 0, 1));
    assert(in_same_scc(tarjan_sccs, tarjan_sizes, tarjan_count, 1, 2));

    int kosaraju_count = 0;
    int* kosaraju_sizes = NULL;
    int** kosaraju_sccs = find_scc_kosaraju(g, &kosaraju_count, &kosaraju_sizes);

    assert(kosaraju_count == 1);
    assert(kosaraju_sizes[0] == 3);
    assert(in_same_scc(kosaraju_sccs, kosaraju_sizes, kosaraju_count, 0, 1));
    assert(in_same_scc(kosaraju_sccs, kosaraju_sizes, kosaraju_count, 1, 2));

    free_scc_result(tarjan_sccs, tarjan_sizes, tarjan_count);
    free_scc_result(kosaraju_sccs, kosaraju_sizes, kosaraju_count);
    free_graph(g);
    printf("test_simple_cycle passed.\n");
}

void test_disconnected_components()
{
    printf("Running test_disconnected_components...\n");
    Graph* g = create_graph(5);
    add_edge_directed_unweighted(g, 0, 1);
    add_edge_directed_unweighted(g, 1, 2);
    add_edge_directed_unweighted(g, 2, 0);
    add_edge_directed_unweighted(g, 3, 4);

    int tarjan_count = 0;
    int* tarjan_sizes = NULL;
    int** tarjan_sccs = find_scc_tarjan(g, &tarjan_count, &tarjan_sizes);

    assert(tarjan_count == 3);
    assert(in_same_scc(tarjan_sccs, tarjan_sizes, tarjan_count, 0, 1));
    assert(in_same_scc(tarjan_sccs, tarjan_sizes, tarjan_count, 1, 2));
    assert(!in_same_scc(tarjan_sccs, tarjan_sizes, tarjan_count, 3, 4));

    int kosaraju_count = 0;
    int* kosaraju_sizes = NULL;
    int** kosaraju_sccs = find_scc_kosaraju(g, &kosaraju_count, &kosaraju_sizes);

    assert(kosaraju_count == 3);
    assert(in_same_scc(kosaraju_sccs, kosaraju_sizes, kosaraju_count, 0, 1));
    assert(in_same_scc(kosaraju_sccs, kosaraju_sizes, kosaraju_count, 1, 2));
    assert(!in_same_scc(kosaraju_sccs, kosaraju_sizes, kosaraju_count, 3, 4));

    free_scc_result(tarjan_sccs, tarjan_sizes, tarjan_count);
    free_scc_result(kosaraju_sccs, kosaraju_sizes, kosaraju_count);
    free_graph(g);
    printf("test_disconnected_components passed.\n");
}

int main()
{
    test_simple_cycle();
    test_disconnected_components();
    printf("All Strongly Connected Components Tests Passed!\n");
    return 0;
}
