#include "graph_traversals.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void astar_demo(void)
{
    int edges;
    int graph_capacity;
    int starting_node;
    int destination_node;
    int input_method;
    weightedGraph* graph = NULL;
    int* h = NULL;

    while (1)
    {
        int method_status = safe_input_int(&input_method,
                                           "\nenter 1 to build the graph manually, 2 to load it "
                                           "from a CSV file, enter '-1' to exit : ",
                                           1, 2);

        if (method_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting A* demo.....\n");
            return;
        }

        if (method_status == 0)
        {
            continue;
        }

        break;
    }

    if (input_method == 2)
    {
        while (1)
        {
            char path[256];
            printf("\nenter the path to the CSV file, enter '-1' to exit : ");
            fflush(stdout);

            if (!fgets(path, sizeof(path), stdin))
            {
                printf("\ninput ended unexpectedly\n");
                clearerr(stdin);
                return;
            }

            size_t len = strlen(path);
            while (len > 0 && (path[len - 1] == '\n' || path[len - 1] == '\r'))
                path[--len] = '\0';

            if (strcmp(path, "-1") == 0)
            {
                printf("\nExiting A* demo.....\n");
                return;
            }

            if (len == 0)
            {
                continue;
            }

            // Loads the graph and its per-vertex heuristic together, so the
            // manual heuristic-entry block below is skipped on this path.
            graph = load_weightedGraph_with_heuristic_from_csv(path, &h);

            if (!graph)
            {
                // error already reported by the loader, let the user retry
                continue;
            }

            break;
        }

        graph_capacity = graph->V;
    }
    else
    {
        while (1)
        {
            int graph_capacity_status =
                safe_input_int(&graph_capacity,
                               "\nenter the number of vertices in the graph, "
                               "(between 1 and 10), enter '-1' to exit : ",
                               1, 10);

            if (graph_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo.....\n");
                return;
            }

            if (graph_capacity_status == 0)
            {
                continue;
            }

            graph = create_weightedGraph(graph_capacity);

            if (!graph)
            {
                printf("\nmemory allocation failed\n");
                return;
            }

            break;
        }

        while (1)
        {
            int edges_capacity_status = safe_input_int(
                &edges, "\nenter number of edges (between 1 and 100), enter '-1' to exit :", 0,
                100);

            if (edges_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo\n");
                free_weightedGraph(graph);
                return;
            }

            if (edges_capacity_status == 0)
            {
                continue;
            }

            break;
        }

        printf(
            "\nEnter source, destination, weight pairs (Source, Destination must be b/w 0 and %d "
            "(both inclusive)):\n",
            graph_capacity - 1);

        for (int i = 0; i < edges; i++)
        {
            int src_status;
            int dest_status;
            int wt_status;
            int src;
            int dest;
            int wt;

        retry:
            src_status = safe_input_int(&src, "src: ", 0, graph_capacity - 1);

            if (src_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo\n");
                free_weightedGraph(graph);
                return;
            }
            if (src_status == 0)
            {
                goto retry;
            }

            dest_status = safe_input_int(&dest, "dest: ", 0, graph_capacity - 1);

            if (dest_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo\n");
                free_weightedGraph(graph);
                return;
            }
            if (dest_status == 0)
            {
                goto retry;
            }

            wt_status = safe_input_int(&wt, "weight: ", 0, INT_MAX);

            if (wt_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo\n");
                free_weightedGraph(graph);
                return;
            }
            if (wt_status == 0)
            {
                goto retry;
            }

            add_edge_directed(graph, src, dest, wt);
        }
    }

    while (1)
    {
        if (!h)
        {
            h = malloc(graph_capacity * sizeof(int));
            if (!h)
            {
                printf("\nmemory allocation failed for heuristics\n");
                free_weightedGraph(graph);
                return;
            }

            printf("\nEnter heuristic values for each vertex:\n");
            for (int i = 0; i < graph_capacity; i++)
            {
                int h_val;
                int h_status;
                char prompt[50];
                snprintf(prompt, sizeof(prompt), "h(%d): ", i);
            retry_h:
                h_status = safe_input_int(&h_val, prompt, 0, INT_MAX);
                if (h_status == INPUT_EXIT_SIGNAL)
                {
                    printf("\nExiting A* demo\n");
                    free(h);
                    free_weightedGraph(graph);
                    return;
                }
                if (h_status == 0)
                {
                    goto retry_h;
                }
                h[i] = h_val;
            }
        }

        while (1)
        {
            int start_status =
                safe_input_int(&starting_node, "\nenter starting node: ", 0, graph_capacity - 1);

            if (start_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo.....\n");
                free(h);
                free_weightedGraph(graph);
                return;
            }

            if (start_status == 0)
            {
                continue;
            }

            break;
        }

        while (1)
        {
            int dest_status = safe_input_int(&destination_node, "\nenter destination node: ", 0,
                                             graph_capacity - 1);

            if (dest_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo.....\n");
                free(h);
                free_weightedGraph(graph);
                return;
            }

            if (dest_status == 0)
            {
                continue;
            }

            break;
        }

        printf("\n");
        astar(graph, starting_node, destination_node, h);

        int choice;
    retry_choice:
        printf("\nOptions:\n1. Re-run A* with NEW heuristics\n2. Re-run A* with SAME heuristics "
               "(new start/destination)\n0. Exit A* demo\n");
        int choice_status =
            safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 0, 2);
        if (choice_status == INPUT_EXIT_SIGNAL || choice == 0)
        {
            printf("\nExiting A* demo.....\n");
            free(h);
            free_weightedGraph(graph);
            return;
        }
        if (choice_status == 0)
        {
            goto retry_choice;
        }

        if (choice == 1)
        {
            free(h);
            h = NULL;
        }
    }
}
