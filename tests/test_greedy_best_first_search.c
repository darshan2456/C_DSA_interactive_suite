#include "graph_traversals.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_greedy_bfs_simple()
{
    weightedGraph* graph = create_weightedGraph(4);
    assert(graph != NULL);

    add_edge_directed(graph, 0, 1, 1);
    add_edge_directed(graph, 0, 2, 1);
    add_edge_directed(graph, 1, 3, 1);
    add_edge_directed(graph, 2, 3, 1);

    int h[] = {4, 1, 2, 0};
    int parent[4];
    int traversal_order[4];
    int traversal_len = 0;

    int found = greedy_best_first_search_solve(graph, 0, 3, h, parent, traversal_order, &traversal_len);

    assert(found == 1);
    assert(traversal_len == 3);
    assert(traversal_order[0] == 0);
    assert(traversal_order[1] == 1);
    assert(traversal_order[2] == 3);
    assert(parent[3] == 1);
    assert(parent[1] == 0);

    free_weightedGraph(graph);
    printf("test_greedy_bfs_simple passed\n");
}

void test_greedy_bfs_unreachable()
{
    weightedGraph* graph = create_weightedGraph(3);
    assert(graph != NULL);

    add_edge_directed(graph, 0, 1, 1);

    int h[] = {2, 1, 0};
    int parent[3];
    int traversal_order[3];
    int traversal_len = 0;

    int found = greedy_best_first_search_solve(graph, 0, 2, h, parent, traversal_order, &traversal_len);

    assert(found == 0);

    free_weightedGraph(graph);
    printf("test_greedy_bfs_unreachable passed\n");
}

void test_greedy_bfs_same_src_dest()
{
    weightedGraph* graph = create_weightedGraph(3);
    assert(graph != NULL);

    add_edge_directed(graph, 0, 1, 1);

    int h[] = {0, 0, 0};
    int parent[3];
    int traversal_order[3];
    int traversal_len = 0;

    int found = greedy_best_first_search_solve(graph, 1, 1, h, parent, traversal_order, &traversal_len);

    assert(found == 1);
    assert(traversal_len == 1);
    assert(traversal_order[0] == 1);

    free_weightedGraph(graph);
    printf("test_greedy_bfs_same_src_dest passed\n");
}

int main()
{
    test_greedy_bfs_simple();
    test_greedy_bfs_unreachable();
    test_greedy_bfs_same_src_dest();

    printf("All Greedy Best-First Search tests passed\n");
    return 0;
}
