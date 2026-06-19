#ifndef BACKTRACKING_H
#define BACKTRACKING_H
#include <stdbool.h>

void backtracking_demo(void);

void n_queens_demo(void);
void sudoku_demo(void);
void rat_in_maze_demo(void);
void graph_coloring_demo(void);
void knights_tour_demo(void);


// Test Wrappers
bool run_n_queens_test(int n);
bool run_sudoku_test(int test_grid[6][6]);
bool run_rat_in_maze_test(int test_maze[6][6]);
bool run_knights_tour_test(int n);
bool run_graph_coloring_test(int colors_allowed);

#endif // BACKTRACKING_H
