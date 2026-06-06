#include "graph_traversals.h"
#include <assert.h>
#include <stdio.h>

/*
 * Tests for Prim's Minimum Spanning Tree
 *
 * These tests call prim_mst() through the public API, matching the
 * integration-test style used by the rest of the project.
 *
 * Since prim_mst() prints its output rather than returning a value,
 * each test verifies that the function completes without crashing and
 * that memory is released cleanly (confirmed by running under Valgrind
 * via `make valgrind`).
 */

/* -------------------------------------------------------------------------
 * Helpers
 * -----------------------------------------------------------------------*/

static void add_undirected(weightedGraph* g, int u, int v, int w)
{
    add_edge_directed(g, u, v, w);
    add_edge_directed(g, v, u, w);
}

/* -------------------------------------------------------------------------
 * Test cases
 * -----------------------------------------------------------------------*/

/*
 * NULL graph pointer — prim_mst() must return immediately without crashing.
 */
static void test_null_graph(void)
{
    printf("\n[test_null_graph] expects silent return, no crash\n");
    prim_mst(NULL, 0);
    printf("PASS  test_null_graph\n");
}

/*
 * Invalid start vertex — prim_mst() must print an error and return.
 */
static void test_invalid_start(void)
{
    weightedGraph* g = create_weightedGraph(3);
    assert(g != NULL);

    add_undirected(g, 0, 1, 4);
    add_undirected(g, 1, 2, 2);

    printf("\n[test_invalid_start] start=99 is out of range, expects error\n");
    prim_mst(g, 99);

    free_weightedGraph(g);
    printf("PASS  test_invalid_start\n");
}

/*
 * Single vertex — MST is trivially empty, total weight = 0.
 */
static void test_single_vertex(void)
{
    weightedGraph* g = create_weightedGraph(1);
    assert(g != NULL);

    printf("\n[test_single_vertex] no edges, total must = 0\n");
    prim_mst(g, 0);

    free_weightedGraph(g);
    printf("PASS  test_single_vertex\n");
}

/*
 * Two vertices connected by one edge.
 */
static void test_two_vertices(void)
{
    weightedGraph* g = create_weightedGraph(2);
    assert(g != NULL);

    add_undirected(g, 0, 1, 7);

    printf("\n[test_two_vertices] one edge of weight 7, total must = 7\n");
    prim_mst(g, 0);

    free_weightedGraph(g);
    printf("PASS  test_two_vertices\n");
}

/*
 * Zero-weight edge — many Prim implementations incorrectly skip edges with
 * weight 0 because they confuse it with "no edge found". The algorithm must
 * treat weight 0 as a valid, cheapest-possible edge.
 *
 *   0 --0-- 1 --5-- 2
 *   |               |
 *   +------10-------+
 *
 * Expected MST: 0-1 (0), 1-2 (5)   total = 5
 */
static void test_zero_weight_edge(void)
{
    weightedGraph* g = create_weightedGraph(3);
    assert(g != NULL);

    add_undirected(g, 0, 1, 0);
    add_undirected(g, 1, 2, 5);
    add_undirected(g, 0, 2, 10);

    printf("\n[test_zero_weight_edge] expected total = 5\n");
    prim_mst(g, 0);

    free_weightedGraph(g);
    printf("PASS  test_zero_weight_edge\n");
}

/*
 * Reviewer's example — 4 vertices, 5 edges.
 *
 *   0 --2-- 1
 *   |      /|
 *   6     8 3
 *   |    /  |
 *   3 --5-- 2
 *
 * Expected MST: 0-1 (2), 1-2 (3), 2-3 (5)   total = 10
 */
static void test_connected(void)
{
    weightedGraph* g = create_weightedGraph(4);
    assert(g != NULL);

    add_undirected(g, 0, 1, 2);
    add_undirected(g, 0, 3, 6);
    add_undirected(g, 1, 2, 3);
    add_undirected(g, 1, 3, 8);
    add_undirected(g, 2, 3, 5);

    printf("\n[test_connected] expected MST: 0-1(2), 1-2(3), 2-3(5)  total=10\n");
    prim_mst(g, 0);

    free_weightedGraph(g);
    printf("PASS  test_connected\n");
}

/*
 * Same graph, different start vertex.
 * Prim's must reach all vertices regardless of where it begins.
 */
static void test_different_start(void)
{
    weightedGraph* g = create_weightedGraph(4);
    assert(g != NULL);

    add_undirected(g, 0, 1, 2);
    add_undirected(g, 0, 3, 6);
    add_undirected(g, 1, 2, 3);
    add_undirected(g, 1, 3, 8);
    add_undirected(g, 2, 3, 5);

    printf("\n[test_different_start] starting from vertex 2, total must still = 10\n");
    prim_mst(g, 2);

    free_weightedGraph(g);
    printf("PASS  test_different_start\n");
}

/*
 * Disconnected graph.
 *
 * Expects a disconnection error on stderr and an immediate return —
 * no MST table is printed because prim_mst() treats a disconnected
 * graph as a fatal input error rather than printing a partial result.
 *
 *   component A: 0 --1-- 1
 *   component B: 2 --3-- 3
 */
static void test_disconnected(void)
{
    weightedGraph* g = create_weightedGraph(4);
    assert(g != NULL);

    add_undirected(g, 0, 1, 1);
    add_undirected(g, 2, 3, 3);

    printf("\n[test_disconnected] expects disconnection error on stderr, no MST table\n");
    prim_mst(g, 0);

    free_weightedGraph(g);
    printf("PASS  test_disconnected\n");
}

/* -------------------------------------------------------------------------
 * Runner
 * -----------------------------------------------------------------------*/

int main(void)
{
    printf("=== Prim's MST tests ===\n");

    test_null_graph();
    test_invalid_start();
    test_single_vertex();
    test_two_vertices();
    test_zero_weight_edge();
    test_connected();
    test_different_start();
    test_disconnected();

    printf("\n=== All tests passed ===\n");
    return 0;
}