#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../src/backtracking/backtracking.h"


// --- 1. N-Queens Tests ---
void test_n_queens() {
    printf("[TEST] Running N-Queens...\n");
    assert(run_n_queens_test(4) == true && "N=4 should have a solution");
    assert(run_n_queens_test(8) == true && "N=8 should have a solution");
    assert(run_n_queens_test(3) == false && "N=3 has no valid solutions");
    printf("  --> PASSED\n");
}

// --- 2. Sudoku 6x6 Tests ---
void test_sudoku_6x6() {
    printf("[TEST] Running Sudoku 6x6...\n");
    int solvable_board[6][6] = {
        {1, 5, 3, 4, 6, 0},
        {4, 6, 2, 0, 1, 3},
        {2, 4, 5, 6, 3, 1},
        {0, 1, 6, 2, 4, 5},
        {5, 3, 4, 1, 2, 6},
        {6, 0, 1, 3, 5, 4}
    };
    assert(run_sudoku_test(solvable_board) == true && "Valid board should be solvable");

    int unsolvable_board[6][6] = {
        {1, 5, 3, 4, 6, 1}, 
        {4, 6, 2, 0, 1, 3},
        {2, 4, 5, 6, 3, 1},
        {0, 1, 6, 2, 4, 5},
        {5, 3, 4, 1, 2, 6},
        {6, 0, 1, 3, 5, 4}
    };
    assert(run_sudoku_test(unsolvable_board) == false && "Invalid board should fail");
    printf("  --> PASSED\n");
}

// --- 3. Rat in a Maze 6x6 Tests ---
void test_rat_in_maze() {
    printf("[TEST] Running Rat in a Maze 6x6...\n");
    int solvable_maze[6][6] = {
        {1, 1, 1, 1, 0, 1}, 
        {1, 0, 0, 1, 0, 1}, 
        {1, 1, 1, 1, 1, 1},
        {0, 1, 0, 0, 0, 1}, 
        {1, 1, 0, 1, 1, 1}, 
        {1, 0, 0, 1, 0, 1}
    };
    assert(run_rat_in_maze_test(solvable_maze) == true && "Valid maze should have a path");

    int blocked_maze[6][6] = {
        {1, 0, 1, 1, 0, 1},
        {0, 0, 0, 1, 0, 1}, // Blocked at spawn
        {1, 1, 1, 1, 1, 1},
        {0, 1, 0, 0, 0, 1},
        {1, 1, 0, 1, 1, 1},
        {1, 0, 0, 1, 0, 1}
    };
    assert(run_rat_in_maze_test(blocked_maze) == false && "Blocked maze should return false");
    printf("  --> PASSED\n");
}

// --- 4. Graph Coloring Tests ---
void test_graph_coloring() {
    printf("[TEST] Running Graph Coloring (K4)...\n");
    assert(run_graph_coloring_test(3) == false && "K4 needs exactly 4 colors");
    assert(run_graph_coloring_test(4) == true && "K4 is solvable with 4 colors");
    printf("  --> PASSED\n");
}

// --- 5. Knight's Tour Tests ---
void test_knights_tour() {
    printf("[TEST] Running Knight's Tour...\n");
    assert(run_knights_tour_test(5) == true && "5x5 board should be solvable");
    printf("  --> PASSED\n");
}

int main() {
    printf("\n===================================\n");
    printf("   Running Backtracking Test Suite \n");
    printf("===================================\n\n");

    test_n_queens();
    test_sudoku_6x6();
    test_rat_in_maze();
    test_graph_coloring();
    test_knights_tour();
    
    printf("\n===================================\n");
    printf(" ✅ ALL BACKTRACKING TESTS PASSED!\n");
    printf("===================================\n\n");

    return 0;
}