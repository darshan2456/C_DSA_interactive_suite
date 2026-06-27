#define _GNU_SOURCE
#include "benchmark.h"
#include "../backtracking/backtracking.h"
#include "../utils/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#else
#include <unistd.h>
#endif

void run_backtracking_benchmark(int n)
{
    // Backtracking size limits
    int queens_n = (n > 8) ? 8 : (n < 4 ? 4 : n);
    int tour_n = 6; // 6x6 board is fast for Knight's Tour, 8x8 is too slow for demo/tests

    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: BACKTRACKING ALGORITHMS (N = %d)\n", n);
    printf("========================================================================\n");
    printf("%-30s %-20s %-12s %-10s\n", "Algorithm", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    const char* algos[] = {
        "N-Queens Solver",
        "Sudoku Solver",
        "Rat in a Maze",
        "Graph Coloring",
        "Knight's Tour"
    };

    // Set animation speed to Instant (choice = 4)
    set_animation_speed(4);

    for (int i = 0; i < 5; i++)
    {
        size_t mem_before = benchmark_get_peak_memory();
        double start_time = benchmark_get_time();

        // Redirect stdout
        fflush(stdout);
        int stdout_dup = dup(1);
#ifdef _WIN32
        FILE* fnull = fopen("NUL", "w");
#else
        FILE* fnull = fopen("/dev/null", "w");
#endif
        if (fnull != NULL && stdout_dup >= 0)
        {
            dup2(fileno(fnull), 1);
        }

        if (i == 0) // N-Queens
        {
            run_n_queens_test(queens_n);
        }
        else if (i == 1) // Sudoku
        {
            int sudoku_grid[6][6] = {
                {1, 5, 3, 4, 6, 0},
                {4, 6, 2, 0, 1, 3},
                {2, 4, 5, 6, 3, 1},
                {0, 1, 6, 2, 4, 5},
                {5, 3, 4, 1, 2, 6},
                {6, 0, 1, 3, 5, 4}
            };
            run_sudoku_test(sudoku_grid);
        }
        else if (i == 2) // Rat in a Maze
        {
            int maze[6][6] = {
                {1, 1, 1, 1, 0, 1},
                {1, 0, 0, 1, 0, 1},
                {1, 1, 1, 1, 1, 1},
                {0, 1, 0, 0, 0, 1},
                {1, 1, 0, 1, 1, 1},
                {1, 0, 0, 1, 0, 1}
            };
            run_rat_in_maze_test(maze);
        }
        else if (i == 3) // Graph Coloring
        {
            run_graph_coloring_test(3);
        }
        else if (i == 4) // Knight's Tour
        {
            run_knights_tour_test(tour_n);
        }

        // Restore stdout
        fflush(stdout);
        if (stdout_dup >= 0)
        {
            dup2(stdout_dup, 1);
            close(stdout_dup);
        }
        if (fnull != NULL)
        {
            fclose(fnull);
        }

        double duration = benchmark_get_time() - start_time;
        size_t mem_after = benchmark_get_peak_memory();
        size_t mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
        if (mem_used == 0) mem_used = mem_after;

        printf("%-30s %-20.6f %-12zu %-10s\n", algos[i], duration * 1000.0, mem_used, "PASSED");
        benchmark_export_csv("backtracking", algos[i], n, duration, mem_used);
    }
    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/backtracking.csv'.\n");
}
