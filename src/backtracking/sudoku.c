#include "../utils/config.h"
#include "backtracking.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "safe_input.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define N 6

// A 6x6 Sudoku puzzle with some missing numbers to demonstrate backtracking.
// It is small enough to run quickly even with the 1-second animation delay.
static int initial_grid[N][N] = {{1, 5, 3, 4, 6, 0}, {4, 6, 2, 0, 1, 3}, {2, 4, 5, 6, 3, 1},
                                 {0, 1, 6, 2, 4, 5}, {5, 3, 4, 1, 2, 6}, {6, 0, 1, 3, 5, 4}};

static int original_grid[N][N];

static int placements = 0;
static int backtracks = 0;

static void print_sudoku_board(int grid[N][N], int original_grid[N][N])
{
    if (!is_instant())
    {
        clear_screen();
    }
    printf("\n--- Sudoku Solver Visualization (6x6) ---\n\n");
    for (int row = 0; row < N; row++)
    {
        if (row % 2 == 0 && row != 0)
        {
            printf("---------------------------\n");
        }
        for (int col = 0; col < N; col++)
        {
            if (col % 3 == 0 && col != 0)
            {
                printf(" | ");
            }
            if (grid[row][col] == 0)
            {
                printf(". ");
            }
            else if (original_grid[row][col] != 0)
            {
                printf("[%d] ", grid[row][col]);
            }
            else
            {
                printf(" %d  ", grid[row][col]);
            }
        }
        printf("\n");
    }
    printf("\n");
    dynamic_sleep();
}

static bool is_safe_sudoku(int grid[N][N], int row, int col, int num)
{
    // Check if we find the same num in the similar row
    for (int x = 0; x < N; x++)
    {
        if (grid[row][x] == num)
        {
            return false;
        }
    }

    // Check if we find the same num in the similar column
    for (int x = 0; x < N; x++)
    {
        if (grid[x][col] == num)
        {
            return false;
        }
    }

    // Check if we find the same num in the 2x3 matrix (blocks for 6x6 Sudoku)
    int startRow = row - (row % 2);
    int startCol = col - (col % 3);

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (grid[i + startRow][j + startCol] == num)
            {
                return false;
            }
        }
    }

    return true;
}

static bool solve_sudoku_util(int grid[N][N], int row, int col)
{
    // If we have reached the Nth row, the board is completely filled
    if (row == N)
    {
        return true;
    }

    // If column reaches N, move to the next row and reset column to 0
    if (col == N)
    {
        return solve_sudoku_util(grid, row + 1, 0);
    }

    // If the current position already contains a value > 0, move to next column
    if (grid[row][col] > 0)
    {
        return solve_sudoku_util(grid, row, col + 1);
    }

    for (int num = 1; num <= N; num++)
    {
        // Check if it is safe to place the num (1-6) in the given row,col
        if (is_safe_sudoku(grid, row, col, num))
        {
            grid[row][col] = num;
            placements++;
            print_sudoku_board(grid, original_grid);

            // Checking for next possibility with next column
            if (solve_sudoku_util(grid, row, col + 1))
            {
                return true;
            }

            // Backtrack: this placement led to a dead end, undo it.
            grid[row][col] = 0;
            backtracks++;
            print_sudoku_board(grid, original_grid);
        }
    }
    return false;
}

void sudoku_demo(void)
{
    while (1)
    {
        int choice;
        // safe_input_int prevents crashes from ANY invalid value (letters, symbols, out of range)
        int status = safe_input_int(
            &choice, "\nEnter 1 to solve the predefined 6x6 Sudoku puzzle, or -1 to exit: ", 1, 1);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Sudoku Solver...\n");
            return;
        }
        if (status == 0)
        {
            continue; // Force retry on failure
        }

        int grid[N][N];
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                grid[i][j] = initial_grid[i][j];
                original_grid[i][j] = initial_grid[i][j];
            }
        }

        placements = 0;
        backtracks = 0;
        printf("\nStarting Sudoku Solver...\n");
        dynamic_sleep();
        print_sudoku_board(grid, original_grid);

        if (solve_sudoku_util(grid, 0, 0))
        {
            printf("\nSudoku solved successfully!\n");
            printf("Total placements: %d\n", placements);
            printf("Backtracks: %d\n", backtracks);
        }
        else
        {
            printf("\nNo solution exists for this Sudoku.\n");
        }
    }
}
// --- TEST WRAPPER ---
bool run_sudoku_test(int test_grid[6][6])
{
    // 1. Validate the initial board before attempting to solve!
    for (int r = 0; r < 6; r++)
    {
        for (int c = 0; c < 6; c++)
        {
            if (test_grid[r][c] != 0)
            {
                int temp = test_grid[r][c];
                test_grid[r][c] = 0; // Temporarily clear it
                // Check if this pre-filled number is actually legal
                if (!is_safe_sudoku(test_grid, r, c, temp))
                {
                    test_grid[r][c] = temp; // Put it back
                    return false;           // Reject: The initial board violates Sudoku rules!
                }
                test_grid[r][c] = temp; // Put it back
            }
        }
    }

    // 2. If the initial state is valid, proceed with solving
    placements = 0;
    backtracks = 0;
    return solve_sudoku_util(test_grid, 0, 0);
}