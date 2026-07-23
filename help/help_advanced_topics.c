#include "display_header.h"
#include "help.h"
#include "safe_input.h"
#include <stdio.h>

void help_advanced_topics_menu(void)
{
    while (1)
    {
        display_header("Help - Advanced Topics");

        printf("Select a sub-topic:\n\n");
        printf("1. Dynamic Programming & Backtracking\n");
        printf("2. String Algorithms & Compression\n");
        printf("3. Process Synchronization & Job Scheduling\n");
        printf("4. Cache Simulator & Step-Debugger\n");
        printf("5. Algorithm Benchmarking & Profiling\n");
        int choice;
        int status =
            safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 5);

        if (status == INPUT_EXIT_SIGNAL)
        {
            break;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                display_header("Help - DP & Backtracking");
                printf("DYNAMIC PROGRAMMING (DP):\n");
                printf("    Solves complex problems by breaking them down into simpler "
                       "subproblems,\n");
                printf("    solving each subproblem once, and storing their solutions (Memoization "
                       "/ Tabulation).\n\n");
                printf("IMPLEMENTED DP DEMOS:\n");
                printf(
                    "    • Fibonacci Sequence: Computes F(N) using memoization/tabulation in O(N) "
                    "time.\n");
                printf("    • 0/1 Knapsack: Maximizes value without exceeding weight capacity.\n");
                printf("    • Longest Common Subsequence (LCS): Finds longest shared character "
                       "subsequence.\n");
                printf("    • Matrix Chain Multiplication (MCM): Finds optimal matrix "
                       "multiplication order.\n\n");
                printf("DP VISUALIZER:\n");
                printf("    An interactive helper tool integrated into DP demos that traces "
                       "recursion\n");
                printf("    trees, caches subproblem states (memoization/tabulation), and "
                       "visualizes state\n");
                printf("    transitions step-by-step.\n\n");
                printf("BACKTRACKING:\n");
                printf("    Systematic search method that tries to construct a solution "
                       "incrementally and\n");
                printf("    backs out (backtracks) as soon as it determines the current path "
                       "cannot lead to a valid solution.\n\n");
                printf("IMPLEMENTED BACKTRACKING DEMOS:\n");
                printf("    • N-Queens Problem: Places N non-attacking queens on an N x N "
                       "chessboard.\n");
                printf("    • Sudoku Solver: Solves a standard 9x9 grid constraint-satisfaction "
                       "puzzle.\n");
                printf("    • Rat in a Maze: Finds a valid route through obstacles in a grid.\n");
                printf("    • Graph Coloring: Colors graph vertices such that no adjacent vertices "
                       "share colors.\n");
                printf("    • Knight's Tour: Visits every square of a chessboard exactly once with "
                       "a knight.\n\n");
                printf("HOW TO RUN IN THIS SUITE:\n");
                printf("    • Backtracking: Main Menu ➔ Option 11.\n");
                printf("    • Dynamic Programming: Main Menu ➔ Option 12.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 2:
                display_header("Help - Strings & Compression");
                printf("STRING ALGORITHMS:\n");
                printf("    • Naive Matching: Checks the pattern at every position of the text. "
                       "O(N*M).\n");
                printf("    • KMP (Knuth-Morris-Pratt): Uses a prefix table to skip re-checking "
                       "characters. O(N+M).\n");
                printf("    • Rabin-Karp: Uses rolling hashes to compare pattern and text "
                       "windows.\n");
                printf("    • Suffix Array: Sorted array of all suffixes, enabling fast substring "
                       "queries.\n");
                printf("    • Kasai's LCP Array: Computes the longest common prefixes between "
                       "consecutive\n");
                printf("      suffixes in the Suffix Array in O(N) time. Essential for substring "
                       "queries.\n\n");
                printf("COMPRESSION & ENCODING:\n");
                printf("    • Huffman Coding: Lossless data compression using variable-length "
                       "prefix codes.\n");
                printf("    • Run-Length Encoding (RLE): Simple form of compression for repeating "
                       "characters.\n");
                printf("    • LZW (Lempel-Ziv-Welch): Dictionary-based compression algorithm.\n");
                printf("    • BWT (Burrows-Wheeler Transform): Reversible string permutation that "
                       "groups\n");
                printf("      similar characters together to improve compression ratios.\n");
                printf("    • Move-to-Front (MTF) Transform: Shifts processed symbols to the front "
                       "of the alphabet,\n");
                printf("      producing sequences of small numbers for consecutive/repeating "
                       "characters.\n\n");
                printf("HOW TO RUN IN THIS SUITE:\n");
                printf("    • String Algorithms: Main Menu ➔ Option 13.\n");
                printf("    • String Compression: Main Menu ➔ Option 18.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 3:
                display_header("Help - Process Sync & Job Scheduling");
                printf("PROCESS SYNCHRONIZATION:\n");
                printf("    • Peterson's Algorithm: Software-based solution to critical section "
                       "problem.\n");
                printf("    • Producer-Consumer Problem using semaphores.\n");
                printf("    • Dining Philosophers: Classic synchronization problem.\n\n");
                printf("JOB SCHEDULING:\n");
                printf("    • FCFS (First Come First Served), SJF (Shortest Job First),\n");
                printf("      Priority, and Round Robin CPU scheduling.\n");
                printf("    • SRTF (Shortest Remaining Time First): Preemptive version of SJF.\n");
                printf("      Preempts the running process if a new process has a shorter "
                       "remaining burst time.\n");
                printf(
                    "      - Real-World Use: Minimizes average waiting time in CPU schedulers.\n");
                printf("    • Preemptive Priority Scheduling: Preempts the running process if a "
                       "newly\n");
                printf("      arrived process has a higher priority.\n");
                printf("      - Real-World Use: Used in Real-Time Operating Systems (RTOS) for "
                       "critical tasks.\n\n");
                printf("HOW TO RUN IN THIS SUITE:\n");
                printf("    1. Exit to the main menu and select option 10 (Job Scheduling).\n");
                printf("    2. Choose 3 for SRTF, or 5 for Preemptive Priority Scheduling.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 4:
                display_header("Help - Cache & Step-Debugger");
                printf("CACHE SIMULATOR:\n");
                printf("    Simulates CPU Cache behavior using various replacement algorithms:\n");
                printf("    • FIFO (First In First Out): Replaces the oldest page in the cache.\n");
                printf("    • LRU (Least Recently Used): Replaces the page that has not been used "
                       "for the longest time.\n");
                printf("    • MRU (Most Recently Used): Replaces the page that was accessed most "
                       "recently.\n");
                printf("      - Real-World Use: Looping access patterns in databases where oldest "
                       "data is reused first.\n");
                printf("    • LFU (Least Frequently Used): Replaces the page with the lowest "
                       "access count (with aging).\n");
                printf("    • OPT (Belady's Optimal): Replaces the page that will not be used for "
                       "the longest future duration.\n");
                printf("      - Real-World Use: Theoretical benchmark to measure efficiency of "
                       "online caching.\n");
                printf("    • Clock (Second Chance): Approximates LRU using a reference bit. It "
                       "circular-scans pages, giving\n");
                printf("      them a \"second chance\" if their bit is 1 (setting it to 0), "
                       "replacing the first page with bit 0.\n");
                printf("      - Real-World Use: Operating system page replacement (low overhead, "
                       "no locks).\n");
                printf("    • Enhanced Clock: Extends Clock by considering both reference and "
                       "modified (dirty) bits, preferring\n");
                printf("      to replace clean pages first to avoid unnecessary write-backs.\n");
                printf("      - Real-World Use: Modern virtual memory management to minimize disk "
                       "write operations.\n\n");
                printf("HOW TO RUN IN THIS SUITE:\n");
                printf("    1. Exit to the main menu and select option 17 (Cache Simulator).\n");
                printf("    2. Specify the cache capacity, then choose from options 1-7 to select "
                       "the replacement policy.\n\n");
                printf("STEP-DEBUGGER:\n");
                printf("    Interactive tool to run algorithms step-by-step to examine variable "
                       "changes and pointer updates.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 5:
                display_header("Help - Benchmarking & Profiling");
                printf("ALGORITHM BENCHMARKING & PROFILING:\n");
                printf("    Measures and compares execution time, memory usage, or operational\n");
                printf("    efficiency across different algorithms under uniform conditions.\n\n");
                printf("CONFIGURATION SETTINGS:\n");
                printf("    • Iterations: Define how many times each algorithm is executed to "
                       "calculate\n");
                printf("                  an accurate average time (mitigates CPU spike "
                       "anomalies).\n");
                printf("    • Export Reporting Format: Save the benchmark results to external "
                       "report\n");
                printf(
                    "                               files in CSV, Markdown, or JSON formats.\n\n");
                printf("BENCHMARK SUITES:\n");
                printf("    1. Sorting Algorithms           8. Hash Map Collision Resolution\n");
                printf("    2. Searching Algorithms         9. Trees Lookup Performance\n");
                printf("    3. Graph Shortest Paths         10. Backtracking Algorithms\n");
                printf("    4. Minimum Spanning Trees (MST) 11. Network Flow Algorithms\n");
                printf("    5. Job Scheduling               12. Advanced Heaps\n");
                printf("    6. String Matching              13. Cache Replacement Simulator\n");
                printf("    7. DP vs Naive Recursion        14. Compression & Encoding\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;
        }
    }
}
