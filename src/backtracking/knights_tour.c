#define _DEFAULT_SOURCE
#include "../utils/config.h"
#include "backtracking.h"
#include "cross_platform_timer.h"
#include "safe_input.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "clear_screen.h"
#ifdef _WIN32
#include <windows.h>
#endif

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

static void print_board(int N, int board[8][8], int knight_x, int knight_y)
{
    if (!is_instant())
    {
        clear_screen();
    }
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
static int solve_standard(int x, int y, int move_count, int board[8][8], int N)
{
    if (move_count == N * N)
    {
        print_board(N, board, x, y);
        return 1;
    }

    for (int i = 0; i < 8; i++)
    {
        int next_x = x + x_move[i];
        int next_y = y + y_move[i];

        if (next_x >= 0 && next_x < N && next_y >= 0 && next_y < N && board[next_x][next_y] == -1)
        {
            board[next_x][next_y] = move_count;
            print_board(N, board, next_x, next_y);

            if (solve_standard(next_x, next_y, move_count + 1, board, N))
                return 1;

            // Backtrack
            board[next_x][next_y] = -1;
            print_board(N, board, x, y);
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
static int solve_warnsdorff(int x, int y, int move_count, int board[8][8], int N)
{
    if (move_count == N * N)
    {
        print_board(N, board, x, y);
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
        print_board(N, board, next_x, next_y);

        if (solve_warnsdorff(next_x, next_y, move_count + 1, board, N))
            return 1;

        // Backtrack
        board[next_x][next_y] = -1;
        print_board(N, board, x, y);
    }
    return 0;
}

void knights_tour_demo(void)
{
    // Set console output to UTF-8 on Windows for Emojis
#ifdef _WIN32
    UINT old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);
#endif

    int board[8][8];

    while (1)
    {
        int N;
        int status =
            safe_input_int(&N, "\nEnter the board size N (between 4 and 8), or -1 to exit: ", -1, 8);

        if (status == INPUT_EXIT_SIGNAL)
        {
#ifdef _WIN32
            SetConsoleOutputCP(old_cp); // restore code page
#endif
            printf("\nReturning to Backtracking menu...\n");
            return;
        }

        if (status == 0)
            continue;

        // Initialize board
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                board[i][j] = -1;
            }
        }

        // Start from cell (0, 0)
        board[0][0] = 0;

        print_board(N, board, 0, 0);

        clock_t start_t = clock();
        int solved = 0;
        // Use standard backtracking for smaller boards to show backtracking/failures,
        // and Warnsdorff heuristic for larger boards (N >= 6) to solve instantly.
        if (N <= 5)
        {
            solved = solve_standard(0, 0, 1, board, N);
        }
        else
        {
            solved = solve_warnsdorff(0, 0, 1, board, N);
        }
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        if (solved)
        {
            printf("\n SUCCESS: The knight completed the tour successfully!\n");
        }
        else
        {
            printf("\n❌ FAILURE: No tour exists for the knight from the starting position.\n");
        }
        printf("Time taken: %f seconds\n", total_t);
    }
}
// --- TEST WRAPPER ---
bool run_knights_tour_test(int n)
{
    int board[8][8];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            board[i][j] = -1;
    board[0][0] = 0;
    return solve_standard(0, 0, 1, board, n) == 1;
}