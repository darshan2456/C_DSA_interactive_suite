#include "advanced_graph_algorithms.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

void advanced_graph_algorithms_demo(void)
{
    while (1)
    {
        display_header("Advanced Graph Algorithms");

        int choice;
        int status = safe_input_int(&choice,
                                    "\nAdvanced Graph Algorithms Demo\n"
                                    "------------------------------\n"
                                    "\n"
                                    "1. Strongly Connected Components (SCC)\n"
                                    "2. Maximum Flow\n"
                                    "3. Bipartite Matching\n"
                                    "4. Eulerian Path\n"
                                    "\nEnter choice (-1 to return): ",
                                    1, 4);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nReturning to main menu...\n");
            return;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                display_header("Strongly Connected Components (SCC)");
                scc_demo();
                break;
            case 2:
                display_header("Maximum Flow");
                max_flow_demo();
                break;
            case 3:
                display_header("Bipartite Matching");
                bipartite_matching_demo();
                break;
            case 4:
                display_header("Eulerian Path");
                eulerian_path_demo();
                break;
        }
    }
}

void bipartite_matching_demo(void)
{
    int vertices, edges;
    Graph* graph = NULL;

    printf("\nMaximum Bipartite Matching Demo\n");
    int input_status = safe_input_int(&vertices, "Enter number of vertices (2-20): ", 2, 20);
    if (input_status == INPUT_EXIT_SIGNAL)
        return;

    graph = create_graph(vertices);
    int edge_status = safe_input_int(&edges, "Enter number of edges (1-100): ", 1, 100);
    if (edge_status == INPUT_EXIT_SIGNAL)
    {
        free_graph(graph);
        return;
    }

    printf("Enter undirected edges (u v) with vertices from 0 to %d:\n", vertices - 1);
    for (int i = 0; i < edges; i++)
    {
        int u, v;
        if (safe_input_int(&u, "u: ", 0, vertices - 1) == INPUT_EXIT_SIGNAL)
        {
            free_graph(graph);
            return;
        }
        if (safe_input_int(&v, "v: ", 0, vertices - 1) == INPUT_EXIT_SIGNAL)
        {
            free_graph(graph);
            return;
        }
        add_edge_undirected(graph, u, v);
    }

    int match_count_dinic = 0;
    int* match_pairs_dinic = NULL;
    int max_dinic = max_bipartite_matching(graph, &match_pairs_dinic, &match_count_dinic);

    int match_count_hk = 0;
    int* match_pairs_hk = NULL;
    int max_hk = hopcroft_karp(graph, &match_pairs_hk, &match_count_hk);

    printf("\n=== Bipartite Matching Results ===\n");
    printf("Max Matching (Dinic's Flow Network): %d\n", max_dinic);
    if (match_count_dinic > 0)
    {
        printf("Matching Pairs (Dinic's): ");
        for (int i = 0; i < match_count_dinic; i++)
        {
            printf("(%d - %d) ", match_pairs_dinic[2 * i], match_pairs_dinic[2 * i + 1]);
        }
        printf("\n");
        free(match_pairs_dinic);
    }

    printf("Max Matching (Hopcroft-Karp): %d\n", max_hk);
    if (match_count_hk > 0)
    {
        printf("Matching Pairs (Hopcroft-Karp): ");
        for (int i = 0; i < match_count_hk; i++)
        {
            printf("(%d - %d) ", match_pairs_hk[2 * i], match_pairs_hk[2 * i + 1]);
        }
        printf("\n");
        free(match_pairs_hk);
    }

    free_graph(graph);
    printf("\nPress Enter to continue...");
    getchar();
}

void eulerian_path_demo(void)
{
    int vertices, edges;
    Graph* graph = NULL;

    printf("\nEulerian Path & Circuit Demo (Directed Graph)\n");
    int input_status = safe_input_int(&vertices, "Enter number of vertices (2-20): ", 2, 20);
    if (input_status == INPUT_EXIT_SIGNAL)
        return;

    graph = create_graph(vertices);
    int edge_status = safe_input_int(&edges, "Enter number of edges (1-100): ", 1, 100);
    if (edge_status == INPUT_EXIT_SIGNAL)
    {
        free_graph(graph);
        return;
    }

    printf("Enter directed edges (src dest) with vertices from 0 to %d:\n", vertices - 1);
    for (int i = 0; i < edges; i++)
    {
        int u, v;
        if (safe_input_int(&u, "src: ", 0, vertices - 1) == INPUT_EXIT_SIGNAL)
        {
            free_graph(graph);
            return;
        }
        if (safe_input_int(&v, "dest: ", 0, vertices - 1) == INPUT_EXIT_SIGNAL)
        {
            free_graph(graph);
            return;
        }
        add_edge_directed_unweighted(graph, u, v);
    }

    int path_len = 0;
    int* path = NULL;
    int res = find_eulerian_path(graph, &path, &path_len);

    if (res == 1 && path_len > 0)
    {
        printf("\nEulerian Path/Circuit Found:\n");
        for (int i = 0; i < path_len; i++)
        {
            printf("%d", path[i]);
            if (i < path_len - 1)
                printf(" -> ");
        }
        printf("\n");
        free(path);
    }
    else
    {
        printf("\nNo Eulerian Path or Circuit exists for the given graph.\n");
    }

    free_graph(graph);
    printf("\nPress Enter to continue...");
    getchar();
}
