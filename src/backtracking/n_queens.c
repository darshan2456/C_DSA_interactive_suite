#include "../utils/config.h"
#include "backtracking.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "clear_screen.h"
#include "cross_platform_timer.h"

#define MAX_N 8

static void print_board(int N, char board[MAX_N][MAX_N])
{
    if (is_instant())
    {
        return;
    }
    clear_screen();
    printf("\n--- N-Queens Visualization ---\n\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf(" %c ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    dynamic_sleep();
}

static bool is_safe(int N, char board[MAX_N][MAX_N], int row, int col)
{
    int i, j;

    // Check this row on left side
    for (i = 0; i < col; i++)
        if (board[row][i] == 'Q')
            return false;

    // Check upper diagonal on left side
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j] == 'Q')
            return false;

    // Check lower diagonal on left side
    for (i = row, j = col; j >= 0 && i < N; i++, j--)
        if (board[i][j] == 'Q')
            return false;

    return true;
}

bool solve_n_queens_util(int N, char board[MAX_N][MAX_N], int col)
{
    // Base case: If all queens are placed
    if (col >= N)
        return true;

    // Consider this column and try placing this queen in all rows one by one
    for (int i = 0; i < N; i++)
    {
        if (is_safe(N, board, i, col))
        {
            // Place the queen
            board[i][col] = 'Q';
            print_board(N, board);

            // Recur to place rest of the queens
            if (solve_n_queens_util(N, board, col + 1))
                return true;

            // If placing queen here doesn't lead to a solution, backtrack
            board[i][col] = '.';
            print_board(N, board);
        }
    }

    // If the queen cannot be placed in any row in this column, return false
    return false;
}

// --- TEST WRAPPER ---
bool run_n_queens_test(int n)
{
    if (n <= 0 || n > MAX_N)
    {
        return false;
    }
    char board[MAX_N][MAX_N];
    for (int i = 0; i < MAX_N; i++)
        for (int j = 0; j < MAX_N; j++)
            board[i][j] = '.';
    return solve_n_queens_util(n, board, 0);
}