#include "backtracking.h"
#include "config.h"
#include "safe_input.h"
#include <stdio.h>
#include <time.h>

void knights_tour_demo(void)
{

    int board[8][8];

    while (1)
    {
        int N;
        int status =
            safe_input_int(&N, "\nEnter the board size N (between 4 and 8), or -1 to exit: ", 4, 8);

        if (status == INPUT_EXIT_SIGNAL)
        {

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

        print_knights_tour_board(N, board, 0, 0);

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
