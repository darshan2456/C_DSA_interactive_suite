#include "backtracking.h"
#include "cross_platform_timer.h"
#include "safe_input.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "clear_screen.h"

#define N 6

// 1 represents an open path, 0 represents a wall
static int initial_maze[N][N] = {{1, 1, 1, 1, 0, 1}, {1, 0, 0, 1, 0, 1}, {1, 1, 1, 1, 1, 1},
                                 {0, 1, 0, 0, 0, 1}, {1, 1, 0, 1, 1, 1}, {1, 0, 0, 1, 0, 1}};

static void print_maze_state(int maze[N][N], int solution[N][N])
{
    clear_screen();
    printf("\n--- Rat in a Maze Visualization (6x6) ---\n\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (solution[i][j] == 1)
            {
                // Rat's current path
                printf(" R ");
            }
            else if (maze[i][j] == 0)
            {
                // Wall
                printf(" | ");
            }
            else
            {
                // Open path
                printf(" . ");
            }
        }
        printf("\n");
    }
    printf("\n");
    sleep_seconds(1);
}

// Function to check if x, y is valid index for N*N maze
static bool is_safe_rat(int maze[N][N], int x, int y)
{
    // if (x, y outside maze) return false
    if (x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 1)
    {
        return true;
    }
    return false;
}

static bool solve_maze_util(int maze[N][N], int x, int y, int solution[N][N])
{
    // If (x, y is goal) return true
    if (x == N - 1 && y == N - 1 && maze[x][y] == 1)
    {
        solution[x][y] = 1;
        print_maze_state(maze, solution);
        return true;
    }

    // Check if maze[x][y] is valid
    if (is_safe_rat(maze, x, y))
    {
        // Check if the current block is already part of solution path
        if (solution[x][y] == 1)
        {
            return false;
        }

        // Mark x, y as part of solution path
        solution[x][y] = 1;
        print_maze_state(maze, solution);

        // Move Forward in X direction
        if (solve_maze_util(maze, x + 1, y, solution))
        {
            return true;
        }

        // If moving in X direction doesn't give solution then Move down in Y direction
        if (solve_maze_util(maze, x, y + 1, solution))
        {
            return true;
        }

        // If moving down doesn't give solution then Move backwards in X direction
        if (solve_maze_util(maze, x - 1, y, solution))
        {
            return true;
        }

        // If moving backwards in X direction doesn't give solution then Move upwards in Y direction
        if (solve_maze_util(maze, x, y - 1, solution))
        {
            return true;
        }

        // If none of the above movements work then BACKTRACK: unmark x, y as part of solution path
        solution[x][y] = 0;
        print_maze_state(maze, solution);
        return false;
    }

    return false;
}

void rat_in_maze_demo(void)
{
    while (1)
    {
        int choice;
        int status = safe_input_int(
            &choice, "\nEnter 1 to solve the predefined 6x6 Rat in a Maze, or -1 to exit: ", 1, 1);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Rat in a Maze...\n");
            return;
        }
        if (status == 0)
        {
            continue;
        }

        int maze[N][N];
        int solution[N][N];

        // Initialize maze and empty solution matrix
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                maze[i][j] = initial_maze[i][j];
                solution[i][j] = 0;
            }
        }

        printf("\nStarting Rat in a Maze Solver...\n");
        sleep_seconds(1);
        print_maze_state(maze, solution);

        if (solve_maze_util(maze, 0, 0, solution))
        {
            printf("\nSolution found successfully!\n");
        }
        else
        {
            printf("\nNo solution exists for this maze.\n");
        }
    }
}
