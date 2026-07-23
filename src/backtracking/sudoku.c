#include "../utils/config.h"
#include "backtracking.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define N 6

int placements = 0;
int backtracks = 0;
int original_grid_copy[N][N];

void print_sudoku_board(int grid[N][N], int original_grid[N][N])
{
    if (is_instant())
    {
        return;
    }
    clear_screen();
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

bool solve_sudoku_util(int grid[N][N], int row, int col)
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
            print_sudoku_board(grid, original_grid_copy);

            // Checking for next possibility with next column
            if (solve_sudoku_util(grid, row, col + 1))
            {
                return true;
            }

            // Backtrack: this placement led to a dead end, undo it.
            grid[row][col] = 0;
            backtracks++;
            print_sudoku_board(grid, original_grid_copy);
        }
    }
    return false;
}

// --- TEST WRAPPER ---
bool run_sudoku_test(int test_grid[6][6])
{
    for (int r = 0; r < 6; r++)
    {
        for (int c = 0; c < 6; c++)
        {
            original_grid_copy[r][c] = test_grid[r][c];
        }
    }

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