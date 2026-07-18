#include "serialization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static void ensure_parent_dir_exists(const char* filepath)
{
    char temp[512];
    strncpy(temp, filepath, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    char* last_slash = strrchr(temp, '/');

    if (last_slash != NULL)
    {
        *last_slash = '\0';
        if (strlen(temp) > 0)
        {
            struct stat st;
            if (stat(temp, &st) == -1)
            {
                mkdir(temp, 0755);
            }
        }
    }
}

// Helper functions for BST serialization
static void write_bst_node(const bstNode* root, FILE* fp)
{
    if (root == NULL)
    {
        fprintf(fp, "#\n");
        return;
    }
    fprintf(fp, "%d\n", root->data);
    write_bst_node(root->left, fp);
    write_bst_node(root->right, fp);
}

static bstNode* read_bst_node(FILE* fp)
{
    char val[64];
    if (fscanf(fp, "%63s", val) != 1)
    {
        return NULL;
    }
    if (strcmp(val, "#") == 0)
    {
        return NULL;
    }
    bstNode* node = malloc(sizeof(bstNode));
    if (node == NULL)
    {
        return NULL;
    }
    node->data = atoi(val);
    node->left = read_bst_node(fp);
    node->right = read_bst_node(fp);
    return node;
}

bool serialize_bst_to_file(const bstNode* root, const char* filepath)
{
    if (filepath == NULL || strlen(filepath) == 0)
    {
        return false;
    }
    ensure_parent_dir_exists(filepath);
    FILE* fp = fopen(filepath, "w");
    if (fp == NULL)
    {
        return false;
    }
    write_bst_node(root, fp);
    fclose(fp);
    return true;
}

bstNode* deserialize_bst_from_file(const char* filepath)
{
    if (filepath == NULL || strlen(filepath) == 0)
    {
        return NULL;
    }
    FILE* fp = fopen(filepath, "r");
    if (fp == NULL)
    {
        return NULL;
    }
    bstNode* root = read_bst_node(fp);
    fclose(fp);
    return root;
}

// Helper functions for AVL serialization
static void write_avl_node(const avlNode* root, FILE* fp)
{
    if (root == NULL)
    {
        fprintf(fp, "#\n");
        return;
    }
    fprintf(fp, "%d %d\n", root->data, root->height);
    write_avl_node(root->left, fp);
    write_avl_node(root->right, fp);
}

static avlNode* read_avl_node(FILE* fp)
{
    char val[64];
    if (fscanf(fp, "%63s", val) != 1)
    {
        return NULL;
    }
    if (strcmp(val, "#") == 0)
    {
        return NULL;
    }
    int data = atoi(val);
    int height = 1;
    if (fscanf(fp, "%d", &height) != 1)
    {
        return NULL;
    }
    avlNode* node = malloc(sizeof(avlNode));
    if (node == NULL)
    {
        return NULL;
    }
    node->data = data;
    node->height = height;
    node->left = read_avl_node(fp);
    node->right = read_avl_node(fp);
    return node;
}

bool serialize_avl_to_file(const avlNode* root, const char* filepath)
{
    if (filepath == NULL || strlen(filepath) == 0)
    {
        return false;
    }
    ensure_parent_dir_exists(filepath);
    FILE* fp = fopen(filepath, "w");
    if (fp == NULL)
    {
        return false;
    }
    write_avl_node(root, fp);
    fclose(fp);
    return true;
}

avlNode* deserialize_avl_from_file(const char* filepath)
{
    if (filepath == NULL || strlen(filepath) == 0)
    {
        return NULL;
    }
    FILE* fp = fopen(filepath, "r");
    if (fp == NULL)
    {
        return NULL;
    }
    avlNode* root = read_avl_node(fp);
    fclose(fp);
    return root;
}

// Graph Serialization (Unweighted)
bool serialize_graph_to_file(const Graph* graph, const char* filepath)
{
    if (graph == NULL || filepath == NULL || strlen(filepath) == 0)
    {
        return false;
    }
    ensure_parent_dir_exists(filepath);
    FILE* fp = fopen(filepath, "w");
    if (fp == NULL)
    {
        return false;
    }

    // Count unique undirected edges
    int E = 0;
    for (int u = 0; u < graph->V; u++)
    {
        Node* curr = graph->array[u];
        while (curr != NULL)
        {
            if (u < (int)(intptr_t)curr->data)
            {
                E++;
            }
            curr = curr->next;
        }
    }

    fprintf(fp, "%d %d\n", graph->V, E);

    // Write edges
    for (int u = 0; u < graph->V; u++)
    {
        Node* curr = graph->array[u];
        while (curr != NULL)
        {
            if (u < (int)(intptr_t)curr->data)
            {
                fprintf(fp, "%d,%d\n", u, (int)(intptr_t)curr->data);
            }
            curr = curr->next;
        }
    }

    fclose(fp);
    return true;
}

Graph* deserialize_graph_from_file(const char* filepath)
{
    return load_graph_from_csv(filepath);
}

// Graph Serialization (Weighted)
bool serialize_weighted_graph_to_file(const weightedGraph* graph, const char* filepath)
{
    if (graph == NULL || filepath == NULL || strlen(filepath) == 0)
    {
        return false;
    }
    ensure_parent_dir_exists(filepath);
    FILE* fp = fopen(filepath, "w");
    if (fp == NULL)
    {
        return false;
    }

    // Count directed edges
    int E = 0;
    for (int u = 0; u < graph->V; u++)
    {
        Edge* curr = graph->array[u];
        while (curr != NULL)
        {
            E++;
            curr = curr->next;
        }
    }

    fprintf(fp, "%d %d\n", graph->V, E);

    // Write edges
    for (int u = 0; u < graph->V; u++)
    {
        Edge* curr = graph->array[u];
        while (curr != NULL)
        {
            fprintf(fp, "%d,%d,%d\n", u, curr->destination, curr->weight);
            curr = curr->next;
        }
    }

    fclose(fp);
    return true;
}

weightedGraph* deserialize_weighted_graph_from_file(const char* filepath)
{
    return load_weightedGraph_from_csv(filepath);
}
