#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "graph_traversals.h"
#include "trees.h"
#include <stdbool.h>

bool serialize_bst_to_file(const bstNode* root, const char* filepath);
bstNode* deserialize_bst_from_file(const char* filepath);

bool serialize_avl_to_file(const avlNode* root, const char* filepath);
avlNode* deserialize_avl_from_file(const char* filepath);

bool serialize_graph_to_file(const Graph* graph, const char* filepath);
Graph* deserialize_graph_from_file(const char* filepath);

bool serialize_weighted_graph_to_file(const weightedGraph* graph, const char* filepath);
weightedGraph* deserialize_weighted_graph_from_file(const char* filepath);

#endif // SERIALIZATION_H
