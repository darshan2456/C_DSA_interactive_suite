#include "backtracking.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>

void backtracking_demo(void)
{
    int bt_status, bt_choice;
    while (1)
    {
        display_header("Backtracking Algorithms");

        bt_status = safe_input_int(&bt_choice,
                                   "\n--- Backtracking Algorithms ---"
                                   "\nenter 1 for N-Queens problem"
                                   "\nenter 2 for Sudoku solver"
                                   "\nenter 3 for Rat in a Maze"
                                   "\nenter 4 for Graph Coloring"
                                   "\nenter 5 for Knight's Tour"
                                   "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                   1, 5);

        if (bt_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Backtracking demo....\n");
            return;
        }

        if (bt_status == 0)
            continue;

        switch (bt_choice)
        {
            case 1:
                display_header("N-Queens Problem");
                n_queens_demo();
                break;
            case 2:
                display_header("Sudoku Solver");
                sudoku_demo();
                break;
            case 3:
                display_header("Rat in a Maze");
                rat_in_maze_demo();
                break;
            case 4:
                display_header("Graph Coloring");
                graph_coloring_demo();
                break;
            case 5:
                display_header("Knight's Tour");
                knights_tour_demo();
                break;
        }
    }
}
