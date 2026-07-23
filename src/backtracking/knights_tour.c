#define _DEFAULT_SOURCE
#include "../utils/config.h"
#include "backtracking.h"
#include "cross_platform_timer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "clear_screen.h"

// ANSI styles
#define ANSI_COLOR_RESET "\033[0m"

static int delay_time = 150; // default delay in ms

static const int x_move[8] = {2, 1, -1, -2, -2, -1, 1, 2};
static const int y_move[8] = {1, 2, 2, 1, -1, -2, -2, -1};

typedef struct
{
    int index;
    int degree;
} MoveCandidate;

// Compare degrees for Warnsdorff sorting (ascending order)
static int compare_candidates(const void* a, const void* b)
{
    return ((MoveCandidate*)a)->degree - ((MoveCandidate*)b)->degree;
}

void print_knights_tour_board(int N, int board[8][8], int knight_x, int knight_y)
{
    if (is_instant())
    {
        return;
    }
    clear_screen();
    printf("\n=== KNIGHT'S TOUR BACKTRACKING VISUALIZER ===\n\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // Checkerboard backgrounds (alternating dark and medium gray blocks)
            const char* bg = ((i + j) % 2 == 0) ? "\033[48;5;236m" : "\033[48;5;240m";

            if (i == knight_x && j == knight_y)
            {
                // Print Knight ♞ in bold yellow
                printf("%s\033[38;5;226;1m ♞ " ANSI_COLOR_RESET, bg);
            }
            else if (board[i][j] != -1)
            {
                // Print step count in cyan
                printf("%s\033[38;5;51m %02d" ANSI_COLOR_RESET, bg, board[i][j]);
            }
            else
            {
                // Print empty checkered space
                printf("%s   " ANSI_COLOR_RESET, bg);
            }
        }
        printf("\n");
    }
    printf("\nLegend: ♞ Knight | Numbers show sequence of moves\n");
    printf("Delay: %dms\n", delay_time);
    fflush(stdout);
    dynamic_sleep();
}

// Standard Backtracking solver (no heuristic) - Good for small boards
int solve_standard(int x, int y, int move_count, int board[8][8], int N)
{
    if (move_count == N * N)
    {
        print_knights_tour_board(N, board, x, y);
        return 1;
    }

    for (int i = 0; i < 8; i++)
    {
        int next_x = x + x_move[i];
        int next_y = y + y_move[i];

        if (next_x >= 0 && next_x < N && next_y >= 0 && next_y < N && board[next_x][next_y] == -1)
        {
            board[next_x][next_y] = move_count;
            print_knights_tour_board(N, board, next_x, next_y);

            if (solve_standard(next_x, next_y, move_count + 1, board, N))
                return 1;

            // Backtrack
            board[next_x][next_y] = -1;
            print_knights_tour_board(N, board, x, y);
        }
    }
    return 0;
}

// Helper for Warnsdorff: Get number of onward unvisited moves from cell
static int get_degree(int x, int y, int board[8][8], int N)
{
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        int nx = x + x_move[i];
        int ny = y + y_move[i];
        if (nx >= 0 && nx < N && ny >= 0 && ny < N && board[nx][ny] == -1)
            count++;
    }
    return count;
}

// Warnsdorff Heuristic guided Backtracking solver - Fast for 8x8 boards
int solve_warnsdorff(int x, int y, int move_count, int board[8][8], int N)
{
    if (move_count == N * N)
    {
        print_knights_tour_board(N, board, x, y);
        return 1;
    }

    MoveCandidate candidates[8];
    int cand_count = 0;

    for (int i = 0; i < 8; i++)
    {
        int next_x = x + x_move[i];
        int next_y = y + y_move[i];
        if (next_x >= 0 && next_x < N && next_y >= 0 && next_y < N && board[next_x][next_y] == -1)
        {
            candidates[cand_count].index = i;
            candidates[cand_count].degree = get_degree(next_x, next_y, board, N);
            cand_count++;
        }
    }

    // Sort moves in ascending order of degree (minimum degree tried first)
    qsort(candidates, cand_count, sizeof(MoveCandidate), compare_candidates);

    for (int i = 0; i < cand_count; i++)
    {
        int move_idx = candidates[i].index;
        int next_x = x + x_move[move_idx];
        int next_y = y + y_move[move_idx];

        board[next_x][next_y] = move_count;
        print_knights_tour_board(N, board, next_x, next_y);

        if (solve_warnsdorff(next_x, next_y, move_count + 1, board, N))
            return 1;

        // Backtrack
        board[next_x][next_y] = -1;
        print_knights_tour_board(N, board, x, y);
    }
    return 0;
}

// --- TEST WRAPPER ---
bool run_knights_tour_test(int n)
{
    int board[8][8];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            board[i][j] = -1;
    board[0][0] = 0;
    if (n <= 5)
    {
        return solve_standard(0, 0, 1, board, n) == 1;
    }
    else
    {
        return solve_warnsdorff(0, 0, 1, board, n) == 1;
    }
}