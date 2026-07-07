#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

// Exact headers from your project structure
#include "../src/trees/trees.h"
#include "../src/graph_traversals/graph_traversals.h"
#include "../src/backtracking/backtracking.h"

#define FUZZ_ITERATIONS 10000
#define MAX_VAL 5000

// ---------------------------------------------------------
// 1. TREES FUZZER (Using Binary Search Tree)
// ---------------------------------------------------------
void fuzz_trees(unsigned int seed) {
    printf("[*] Fuzzing Trees (BST) with %d iterations...\n", FUZZ_ITERATIONS);
    
    bstNode* root = NULL;

    for (int i = 0; i < FUZZ_ITERATIONS; i++) {
        int op = rand() % 2; // 0: Insert, 1: Delete
        int val = rand() % MAX_VAL;

        if (op == 0) {
            bst_insert(&root, val);
        } else {
            root = bst_delete(root, val);
        }
    }
    
    // Explicitly free the tree to pass Valgrind's memory leak check
    destroy_bst(root); 
    printf("[+] Tree fuzzing completed successfully.\n");
}

// ---------------------------------------------------------
// 2. GRAPHS FUZZER (Using Weighted Graphs & Dijkstra)
// ---------------------------------------------------------
void fuzz_graphs(unsigned int seed) {
    printf("[*] Fuzzing Graph Traversals with random edges...\n");
    
    int num_vertices = (rand() % 50) + 10; // 10 to 60 vertices
    weightedGraph* g = create_weightedGraph(num_vertices);

    // Randomly add edges with weights
    int num_edges = num_vertices * 3;
    for (int i = 0; i < num_edges; i++) {
        int src = rand() % num_vertices;
        int dest = rand() % num_vertices;
        int wt = (rand() % 100) + 1; // Weights between 1 and 100
        
        if (src != dest) {
            add_edge_directed(g, src, dest, wt);
        }
    }

    // Run Dijkstra's Algorithm from a random starting node
    int start_node = rand() % num_vertices;
    dijkstra(g, start_node);

    // Free the entire graph memory
    free_weightedGraph(g);
    printf("[+] Graph fuzzing completed successfully.\n");
}

// ---------------------------------------------------------
// 3. BACKTRACKING FUZZER (N-Queens & Knight's Tour)
// ---------------------------------------------------------
void fuzz_backtracking(unsigned int seed) {
    printf("[*] Fuzzing Backtracking Algorithms...\n");
    
    for(int i = 0; i < 20; i++) {
        // N-Queens with random board sizes between 4 and 10
        int n_queens_size = (rand() % 7) + 4; 
        run_n_queens_test(n_queens_size);

        // Knight's Tour with smaller sizes (4 to 6) to prevent CI timeout
        int knight_size = (rand() % 3) + 4;
        run_knights_tour_test(knight_size);
    }
    printf("[+] Backtracking fuzzing completed successfully.\n");
}

// ---------------------------------------------------------
// MAIN ENGINE
// ---------------------------------------------------------
int main() {
    // Generate a master seed
    unsigned int master_seed = (unsigned int)time(NULL);
    srand(master_seed);

    printf("==================================================\n");
    printf("🚀 STOCHASTIC FUZZ TESTING ENGINE STARTED\n");
    printf("📌 MASTER SEED: %u\n", master_seed);
    printf("💡 (If a crash occurs, replay using this exact seed)\n");
    printf("==================================================\n\n");

    fuzz_trees(master_seed);
    fuzz_graphs(master_seed);
    fuzz_backtracking(master_seed);

    printf("\n==================================================\n");
    printf("✅ ALL FUZZ TESTS PASSED. NO SEGFAULTS DETECTED.\n");
    printf("==================================================\n");

    return 0;
}