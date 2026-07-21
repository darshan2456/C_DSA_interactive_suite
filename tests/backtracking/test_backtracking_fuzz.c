#include "backtracking.h"
#include "fuzzer.h"
#include "step_debugger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
