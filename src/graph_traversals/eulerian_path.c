#include "graph_traversals.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int find_eulerian_path(Graph* graph, int** path, int* path_len)
{
    if (graph == NULL || path == NULL || path_len == NULL)
        return 0;

    int V = graph->V;
    int* in_degree = calloc(V, sizeof(int));
    int* out_degree = calloc(V, sizeof(int));
    if (in_degree == NULL || out_degree == NULL)
    {
        free(in_degree);
        free(out_degree);
        return 0;
    }

    int total_edges = 0;
    for (int u = 0; u < V; u++)
    {
        Node* temp = graph->array[u];
        while (temp != NULL)
        {
            out_degree[u]++;
            in_degree[temp->data]++;
            total_edges++;
            temp = temp->next;
        }
    }

    int start_node = -1;
    int end_node = -1;
    int odd_out = 0;
    int odd_in = 0;
    int equal_deg = 0;
    int active_nodes = 0;
    (void)end_node;
    (void)equal_deg;
    (void)active_nodes;

    for (int i = 0; i < V; i++)
    {
        if (in_degree[i] > 0 || out_degree[i] > 0)
        {
            active_nodes++;
        }

        if (out_degree[i] - in_degree[i] == 1)
        {
            odd_out++;
            start_node = i;
        }
        else if (in_degree[i] - out_degree[i] == 1)
        {
            odd_in++;
            end_node = i;
        }
        else if (in_degree[i] == out_degree[i])
        {
            equal_deg++;
        }
        else
        {
            // Invalid degrees for Eulerian Path
            free(in_degree);
            free(out_degree);
            return 0;
        }
    }

    // Check if Eulerian Path / Circuit conditions are satisfied
    bool is_circuit = (odd_out == 0 && odd_in == 0);
    bool is_path = (odd_out == 1 && odd_in == 1);

    if (!is_circuit && !is_path)
    {
        free(in_degree);
        free(out_degree);
        return 0;
    }

    if (is_circuit)
    {
        // Start at any node with outgoing edges
        for (int i = 0; i < V; i++)
        {
            if (out_degree[i] > 0)
            {
                start_node = i;
                break;
            }
        }
    }

    if (start_node == -1)
    {
        // Empty graph or no edges
        free(in_degree);
        free(out_degree);
        *path_len = 0;
        *path = NULL;
        return 1;
    }

    // Copy adjacency list pointer structure to keep track of remaining outgoing edges
    Node** adj_copy = malloc(sizeof(Node*) * V);
    if (adj_copy == NULL)
    {
        free(in_degree);
        free(out_degree);
        return 0;
    }
    for (int i = 0; i < V; i++)
    {
        adj_copy[i] = graph->array[i];
    }

    // Hierholzer's Algorithm using arrays as stacks
    // Maximum stack size needed is total_edges + 2
    int max_stack = total_edges + 2;
    int* curr_stack = malloc(sizeof(int) * max_stack);
    int* circuit_stack = malloc(sizeof(int) * max_stack);
    if (curr_stack == NULL || circuit_stack == NULL)
    {
        free(in_degree);
        free(out_degree);
        free(adj_copy);
        free(curr_stack);
        free(circuit_stack);
        return 0;
    }

    int curr_top = 0;
    int circuit_top = 0;

    curr_stack[curr_top++] = start_node;

    while (curr_top > 0)
    {
        int u = curr_stack[curr_top - 1];
        if (adj_copy[u] != NULL)
        {
            int v = adj_copy[u]->data;
            adj_copy[u] = adj_copy[u]->next;
            curr_stack[curr_top++] = v;
        }
        else
        {
            curr_top--;
            circuit_stack[circuit_top++] = u;
        }
    }

    // If the circuit stack doesn't contain all edges + 1, graph was disconnected
    if (circuit_top != total_edges + 1)
    {
        free(in_degree);
        free(out_degree);
        free(adj_copy);
        free(curr_stack);
        free(circuit_stack);
        return 0;
    }

    // Reverse circuit_stack to get the path in correct order
    int* res_path = malloc(sizeof(int) * circuit_top);
    if (res_path == NULL)
    {
        free(in_degree);
        free(out_degree);
        free(adj_copy);
        free(curr_stack);
        free(circuit_stack);
        return 0;
    }

    for (int i = 0; i < circuit_top; i++)
    {
        res_path[i] = circuit_stack[circuit_top - 1 - i];
    }

    free(in_degree);
    free(out_degree);
    free(adj_copy);
    free(curr_stack);
    free(circuit_stack);

    *path_len = circuit_top;
    *path = res_path;
    return 1;
}
