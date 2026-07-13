#include "display_header.h"
#include "help.h"
#include "safe_input.h"
#include <stdio.h>

void help_graphs_trees_menu(void)
{
    while (1)
    {
        display_header("Help - Graphs & Trees");

        printf("Select a sub-topic:\n\n");
        printf("1. Trees (BST, AVL, Trie, B-Tree, etc.)\n");
        printf("2. Graph Traversals & Spanning Trees\n");
        printf("3. Advanced Graph Algorithms\n");
        int choice;
        int status =
            safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 3);

        if (status == INPUT_EXIT_SIGNAL)
        {
            break;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                display_header("Help - Trees");
                printf("BINARY SEARCH TREE (BST):\n");
                printf("    Node-based binary tree where left child < parent and right child > "
                       "parent.\n\n");
                printf("AVL TREE:\n");
                printf("    Self-balancing binary search tree where heights of children differ by "
                       "at most 1.\n\n");
                printf("TRIE:\n");
                printf("    Search tree used to store associative structures, typically keys are "
                       "strings.\n\n");
                printf("B-TREE & B+ TREE:\n");
                printf("    Self-balancing search trees optimized for systems that read/write "
                       "large blocks of data.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 2:
                display_header("Help - Graph Traversals & Spanning Trees");
                printf("BFS & DFS:\n");
                printf("    • Breadth-First Search: Explores vertices layer-by-layer using a "
                       "Queue.\n");
                printf("    • Depth-First Search: Explores as deep as possible before backtracking "
                       "using a Stack.\n\n");
                printf("SHORTEST PATHS:\n");
                printf("    • Dijkstra: Finds single-source shortest path for non-negative "
                       "weights.\n");
                printf("    • Bellman-Ford: Finds single-source shortest path; handles negative "
                       "weights.\n");
                printf("    • A* Search: Uses heuristics to speed up shortest-path searches.\n\n");
                printf("SPANNING TREES:\n");
                printf("    • Prim's & Kruskal's algorithms to find Minimum Spanning Trees "
                       "(MST).\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 3:
                display_header("Help - Advanced Graph Algorithms");
                printf("STRONGLY CONNECTED COMPONENTS (SCC):\n");
                printf("    Tarjan's or Kosaraju's algorithm to find directed cycles.\n\n");
                printf("MAXIMUM FLOW:\n");
                printf("    Ford-Fulkerson or Edmonds-Karp to compute maximum flow through a flow "
                       "network.\n\n");
                printf("BIPARTITE MATCHING:\n");
                printf("    Hopcroft-Karp or augmenting path methods to find maximum cardinality "
                       "matching.\n\n");
                printf("EULERIAN PATH:\n");
                printf("    Fleury's algorithm or Hierholzer's algorithm to visit every edge "
                       "exactly once.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;
        }
    }
}
