#include "display_header.h"
#include "help.h"
#include "safe_input.h"
#include <stdio.h>

void help_graphs_trees_menu(void)
{
    while (1)
    {
        display_header("Help - Graphs, Trees & Heaps");

        printf("Select a sub-topic:\n\n");
        printf("1. Trees (BST, AVL, Trie, B-Tree, etc.)\n");
        printf("2. Graph Traversals & Spanning Trees\n");
        printf("3. Advanced Graph Algorithms\n");
        printf("4. Advanced Heaps (Binomial, Fibonacci, Treap, etc.)\n");
        int choice;
        int status =
            safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 4);

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
                printf("RED-BLACK TREE:\n");
                printf("    Self-balancing BST using node colors (red/black) to guarantee O(log "
                       "N) operations.\n\n");
                printf("SPLAY TREE:\n");
                printf("    Self-adjusting BST that moves recently accessed nodes to the root "
                       "(splaying).\n\n");
                printf("SEGMENT TREE & FENWICK TREE (BIT):\n");
                printf("    Trees for fast range queries (sum, min, max) and point updates in "
                       "O(log N).\n\n");
                printf("THREADED BINARY TREE:\n");
                printf("    Binary tree where NULL child pointers are reused as \"threads\" to "
                       "in-order neighbors,\n");
                printf("    allowing traversal without recursion or a stack.\n\n");
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
                printf("    • A* Search: Uses heuristics to speed up shortest-path searches.\n");
                printf("    • Greedy Best-First Search: Expands the node closest to the goal "
                       "according to a heuristic.\n");
                printf("    • Floyd-Warshall: Computes shortest paths between all pairs of "
                       "vertices.\n\n");
                printf("TOPOLOGICAL SORT:\n");
                printf("    Linear ordering of a DAG's vertices such that every edge (u, v) has u "
                       "before v (Kahn's algorithm).\n\n");
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

            case 4:
                display_header("Help - Advanced Heaps");
                printf("D-ARY HEAP:\n");
                printf("    A generalization of a binary heap where each node has D children.\n");
                printf(
                    "    - Formula: Parent of index i is (i-1)/D. Children are D*i+1 to D*i+D.\n");
                printf("    - Trade-offs: Speeds up decrease-key operations; slows down "
                       "delete-min.\n\n");

                printf("MIN-MAX HEAP:\n");
                printf("    A double-ended priority queue implemented using a binary heap "
                       "structure.\n");
                printf("    - Concept: Even levels are Min levels; odd levels are Max levels.\n");
                printf(
                    "    - Capabilities: Allows both find-min and find-max in O(1), and pop-min\n");
                printf("      and pop-max in O(log N).\n\n");

                printf("LEFTIST & SKEW HEAPS:\n");
                printf("    - Leftist Heap: A mergeable binary heap where the null path length "
                       "(npl)\n");
                printf("      of the left child is always >= that of the right child.\n");
                printf("    - Skew Heap: A self-adjusting version of leftist heap that "
                       "unconditionally\n");
                printf("      swaps children during merge (no need to track or compute "
                       "balance/npl).\n");
                printf("    - Key Property: Both support O(log N) merge operations.\n\n");

                printf("BINOMIAL HEAP:\n");
                printf("    A collection of binomial trees where each tree satisfies the heap "
                       "property\n");
                printf("    and there is at most one binomial tree of any given degree k.\n");
                printf("    - Key Property: Supports O(log N) merge, insert, and extract-min "
                       "operations.\n\n");

                printf("FIBONACCI HEAP:\n");
                printf("    A mergeable heap structure optimized for amortized running times.\n");
                printf(
                    "    - Amortized Times: O(1) for Insert, Find-Min, Decrease-Key, and Merge.\n");
                printf("      O(log N) for Extract-Min and Delete.\n");
                printf("    - Use Case: Crucial for Dijkstra and Prim algorithms on sparse "
                       "graphs.\n\n");

                printf("TREAP:\n");
                printf(
                    "    A randomized binary search tree that satisfies both the BST property\n");
                printf("    (on keys) and the Heap property (on randomly assigned priorities).\n");
                printf("    - Key Property: Balanced with high probability, supporting O(log N) "
                       "operations.\n\n");

                printf("RELATIONSHIPS BETWEEN HEAPS:\n");
                printf("    • D-Ary vs. Binary Heap:\n");
                printf("      Binary heap is a 2-Ary heap. D-Ary heap generalizes this to D "
                       "children.\n");
                printf("    • Leftist vs. Skew:\n");
                printf(
                    "      Leftist heap enforces structure using null path length (npl) balance\n");
                printf("      conditions. Skew heap is the self-adjusting version that swaps "
                       "children\n");
                printf("      unconditionally, avoiding tracking node state.\n");
                printf("    • Binomial vs. Fibonacci:\n");
                printf(
                    "      Binomial heap enforces eager, immediate tree merges. Fibonacci heap\n");
                printf("      implements lazy, delayed merges, postponing sorting/restructuring\n");
                printf("      until extract-min is called (giving it O(1) "
                       "insert/merge/decrease-key).\n");
                printf("    • Treap vs. BST:\n");
                printf("      Treap uses randomized priorities to act as a self-balancing BST "
                       "without\n");
                printf("      the complex balancing logic of AVL or Red-Black trees.\n\n");

                printf("WHEN TO USE WHAT & REAL-WORLD USE CASES:\n");
                printf("    • Use D-Ary Heap when memory access is a bottleneck (cache-friendly "
                       "index\n");
                printf("      layout) or when decrease-key operations outnumber delete-min.\n");
                printf("      - Real-world: External memory databases, network routing tables.\n");
                printf("    • Use Min-Max Heap when double-ended priority access is needed.\n");
                printf("      - Real-world: Job schedulers prioritizing highest and lowest "
                       "processes.\n");
                printf("    • Use Leftist / Skew Heaps when merge operations are frequent.\n");
                printf(
                    "      - Real-world: Discrete event simulation systems, lazy evaluations.\n");
                printf("    • Use Fibonacci Heap when implementing priority queues in shortest "
                       "path\n");
                printf("      algorithms (like Dijkstra's or Prim's) on large, sparse graphs.\n");
                printf("    • Use Treap when you need dynamic sorted indexing with randomized "
                       "balance.\n");
                printf("      - Real-world: IP routing tables, randomized database search "
                       "indexes.\n\n");

                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;
        }
    }
}
