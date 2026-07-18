#include "serialization.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_bst_serialization(void)
{
    /* Build a simple BST:
     *      5
     *     / \
     *    3   7
     */
    bstNode* root = NULL;
    bst_insert(&root, 5);
    bst_insert(&root, 3);
    bst_insert(&root, 7);

    assert(serialize_bst_to_file(root, "test_binaries/bst_test.dat"));

    bstNode* reconstructed = deserialize_bst_from_file("test_binaries/bst_test.dat");
    assert(reconstructed != NULL);
    assert(reconstructed->data == 5);
    assert(reconstructed->left != NULL && reconstructed->left->data == 3);
    assert(reconstructed->right != NULL && reconstructed->right->data == 7);

    destroy_bst(root);
    destroy_bst(reconstructed);
    remove("test_binaries/bst_test.dat");
}

static void test_avl_serialization(void)
{
    /* Build a simple AVL tree:
     *      10
     *     /  \
     *    5    15
     */
    avlNode* root = NULL;
    avl_insert(&root, 10);
    avl_insert(&root, 5);
    avl_insert(&root, 15);

    assert(serialize_avl_to_file(root, "test_binaries/avl_test.dat"));

    avlNode* reconstructed = deserialize_avl_from_file("test_binaries/avl_test.dat");
    assert(reconstructed != NULL);
    assert(reconstructed->data == 10);
    assert(reconstructed->height == 2);
    assert(reconstructed->left != NULL && reconstructed->left->data == 5);
    assert(reconstructed->right != NULL && reconstructed->right->data == 15);

    destroy_avl(root);
    destroy_avl(reconstructed);
    remove("test_binaries/avl_test.dat");
}

static void test_graph_serialization(void)
{
    Graph* graph = create_graph(4);
    add_edge_undirected(graph, 0, 1);
    add_edge_undirected(graph, 1, 2);
    add_edge_undirected(graph, 2, 3);

    assert(serialize_graph_to_file(graph, "test_binaries/graph_test.dat"));

    Graph* reconstructed = deserialize_graph_from_file("test_binaries/graph_test.dat");
    assert(reconstructed != NULL);
    assert(reconstructed->V == 4);

    // Verify adjacency list connections exist
    Node* curr = reconstructed->array[1];
    bool has_zero = false;
    bool has_two = false;
    while (curr != NULL)
    {
        int val = (int)(intptr_t)curr->data;
        if (val == 0)
            has_zero = true;
        if (val == 2)
            has_two = true;
        curr = curr->next;
    }
    assert(has_zero);
    assert(has_two);

    free_graph(graph);
    free_graph(reconstructed);
    remove("test_binaries/graph_test.dat");
}

static void test_weighted_graph_serialization(void)
{
    weightedGraph* graph = create_weightedGraph(3);
    add_edge_directed(graph, 0, 1, 10);
    add_edge_directed(graph, 1, 2, 20);

    assert(serialize_weighted_graph_to_file(graph, "test_binaries/wgraph_test.dat"));

    weightedGraph* reconstructed =
        deserialize_weighted_graph_from_file("test_binaries/wgraph_test.dat");
    assert(reconstructed != NULL);
    assert(reconstructed->V == 3);

    // Verify edge 0->1 weight is 10
    Edge* curr = reconstructed->array[0];
    assert(curr != NULL);
    assert(curr->destination == 1);
    assert(curr->weight == 10);

    // Verify edge 1->2 weight is 20
    curr = reconstructed->array[1];
    assert(curr != NULL);
    assert(curr->destination == 2);
    assert(curr->weight == 20);

    free_weightedGraph(graph);
    free_weightedGraph(reconstructed);
    remove("test_binaries/wgraph_test.dat");
}

int main(void)
{
    printf("Starting State Serialization unit tests...\n");
    test_bst_serialization();
    test_avl_serialization();
    test_graph_serialization();
    test_weighted_graph_serialization();
    printf("All State Serialization unit tests passed successfully!\n");
    return 0;
}
