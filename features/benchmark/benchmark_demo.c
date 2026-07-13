#include "benchmark.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>

void benchmark_menu_demo(void)
{
    while (1)
    {
        display_header("Algorithm Benchmarking & Profiling");

        char settings_msg[128];
        snprintf(settings_msg, sizeof(settings_msg),
                 "\nCurrent Settings: %d Iterations, %s Format\n", benchmark_iterations,
                 benchmark_output_format == FORMAT_CSV
                     ? "CSV"
                     : (benchmark_output_format == FORMAT_MARKDOWN ? "Markdown" : "JSON"));

        printf("%s", settings_msg);

        int choice;
        int status = safe_input_int(&choice,
                                    "--- Algorithm Benchmarking & Profiling Menu ---\n"
                                    "click 0 to Configure Benchmarking Settings\n"
                                    "click 1 for Sorting Algorithms benchmark\n"
                                    "click 2 for Searching Algorithms benchmark\n"
                                    "click 3 for Graph Shortest Path Algorithms benchmark\n"
                                    "click 4 for MST Algorithms benchmark\n"
                                    "click 5 for Job Scheduling Algorithms benchmark\n"
                                    "click 6 for String Matching Algorithms benchmark\n"
                                    "click 7 for Dynamic Programming vs Naive Recursion benchmark\n"
                                    "click 8 for Hash Map Collision Resolution benchmark\n"
                                    "click 9 for Trees Lookup Performance benchmark\n"
                                    "click 10 for Backtracking Algorithms benchmark\n"
                                    "click 11 for Network Flow Algorithms benchmark\n"
                                    "click 12 for Advanced Heaps benchmark\n"
                                    "click 13 for Cache Replacement Simulator benchmark\n"
                                    "click 14 for Compression & Encoding Algorithms benchmark\n"
                                    "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                    0, 14);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Benchmark menu...\n");
            return;
        }
        if (status == 0)
        {
            continue;
        }

        if (choice == 0)
        {
            int opt;
            int opt_status = safe_input_int(&opt,
                                            "\n--- Configure Benchmarking Settings ---\n"
                                            "1. Change Number of Iterations (currently: %d)\n"
                                            "2. Change Export Reporting Format (currently: %s)\n"
                                            "Enter option (1 or 2): ",
                                            1, 2);
            if (opt_status != 1)
                continue;

            if (opt == 1)
            {
                int iters;
                int iters_status =
                    safe_input_int(&iters, "Enter number of iterations (1 to 50): ", 1, 50);
                if (iters_status == 1)
                {
                    benchmark_iterations = iters;
                    printf("\nBenchmark iterations updated to %d!\n", benchmark_iterations);
                }
            }
            else if (opt == 2)
            {
                int format_val;
                int format_status = safe_input_int(&format_val,
                                                   "\nSelect Output Reporting Format:\n"
                                                   "1. CSV\n"
                                                   "2. Markdown\n"
                                                   "3. JSON\n"
                                                   "Enter choice (1, 2, or 3): ",
                                                   1, 3);
                if (format_status == 1)
                {
                    if (format_val == 1)
                        benchmark_output_format = FORMAT_CSV;
                    else if (format_val == 2)
                        benchmark_output_format = FORMAT_MARKDOWN;
                    else if (format_val == 3)
                        benchmark_output_format = FORMAT_JSON;

                    printf("\nBenchmark output format updated!\n");
                }
            }
            continue;
        }

        int n;
        int n_status = safe_input_int(
            &n, "Enter input size N (between 10 and 100000), or -1 to exit: ", 10, 100000);
        if (n_status == INPUT_EXIT_SIGNAL)
        {
            continue;
        }
        if (n_status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                run_sorting_benchmark(n);
                break;
            case 2:
                run_searching_benchmark(n);
                break;
            case 3:
                run_graphs_benchmark(n);
                break;
            case 4:
                run_mst_benchmark(n);
                break;
            case 5:
                run_scheduling_benchmark(n);
                break;
            case 6:
                run_strings_benchmark(n);
                break;
            case 7:
                run_dp_benchmark(n);
                break;
            case 8:
                run_hashing_benchmark(n);
                break;
            case 9:
                run_trees_benchmark(n);
                break;
            case 10:
                run_backtracking_benchmark(n);
                break;
            case 11:
                run_flow_benchmark(n);
                break;
            case 12:
                run_heaps_benchmark(n);
                break;
            case 13:
                run_cache_benchmark(n);
                break;
            case 14:
                run_compression_benchmark(n);
                break;
        }
    }
}
