#include "backtracking.h"
#include "fuzzer.h"
#include "step_debugger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void run_sudoku_fuzz(FuzzerState* fuzzer, int runs)
{
    int grid[6][6];
    for (int r = 0; r < runs; r++)
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                // Most squares are empty (0), some are randomized clues (1-6)
                if (fuzzer_rand_int(fuzzer, 0, 4) == 0)
                {
                    grid[i][j] = fuzzer_rand_int(fuzzer, 1, 6);
                }
                else
                {
                    grid[i][j] = 0;
                }
            }
        }
        fuzzer_log_op(fuzzer, "Sudoku run %d", r);
        run_sudoku_test(grid);
    }
}

void run_maze_fuzz(FuzzerState* fuzzer, int runs)
{
    int maze[6][6];
    for (int r = 0; r < runs; r++)
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                // Start and end are always paths
                if ((i == 0 && j == 0) || (i == 5 && j == 5))
                {
                    maze[i][j] = 1;
                }
                else
                {
                    // 1 for path, 0 for wall
                    maze[i][j] = (fuzzer_rand_int(fuzzer, 0, 3) == 0) ? 0 : 1;
                }
            }
        }
        fuzzer_log_op(fuzzer, "Maze run %d", r);
        run_rat_in_maze_test(maze);
    }
}

int main(void)
{
    setenv("DSA_TEST_MODE", "1", 1);
    set_step_mode(0);
    set_paused(0);

    unsigned int seed = (unsigned int)time(NULL);
    FuzzerState fuzzer;
    printf("Starting Backtracking Fuzzing with seed: %u\n", seed);

    fuzzer_init(&fuzzer, seed, 5000);

    // N-Queens
    for (int i = 0; i < 15; i++)
    {
        int n = fuzzer_rand_int(&fuzzer, -2, 10);
        fuzzer_log_op(&fuzzer, "N-Queens test size %d", n);
        run_n_queens_test(n);
    }

    // Knights Tour
    for (int i = 0; i < 15; i++)
    {
        int n = fuzzer_rand_int(&fuzzer, -2, 7);
        fuzzer_log_op(&fuzzer, "Knights Tour test size %d", n);
        run_knights_tour_test(n);
    }

    // Sudoku
    run_sudoku_fuzz(&fuzzer, 20);

    // Maze
    run_maze_fuzz(&fuzzer, 20);

    fuzzer_free(&fuzzer);
    printf("Backtracking Fuzzing completed successfully!\n");
    return 0;
}
