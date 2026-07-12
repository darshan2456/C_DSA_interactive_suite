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
        int choice;
        int status = safe_input_int(&choice, "Enter choice (or -1 to go back): ", 1, 4);

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
                printf("BACKTRACKING:\n");
                printf("    Systematic search method that tries to construct a solution "
                       "incrementally and\n");
                printf("    backs out (backtracks) as soon as it determines the current path "
                       "cannot lead to a valid solution.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 2:
                display_header("Help - Strings & Compression");
                printf("STRING ALGORITHMS:\n");
                printf("    Pattern matching algorithms (e.g., KMP, Rabin-Karp, Z-algorithm).\n\n");
                printf("COMPRESSION & ENCODING:\n");
                printf("    • Huffman Coding: Lossless data compression using variable-length "
                       "prefix codes.\n");
                printf("    • Run-Length Encoding (RLE): Simple form of compression for repeating "
                       "characters.\n");
                printf("    • LZW (Lempel-Ziv-Welch) & BWT (Burrows-Wheeler Transform).\n\n");
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
                printf("      Priority, and Round Robin CPU scheduling.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 4:
                display_header("Help - Cache & Step-Debugger");
                printf("CACHE SIMULATOR:\n");
                printf("    Simulates CPU Cache behavior using replacement algorithms like LRU, "
                       "FIFO, and LFU.\n\n");
                printf("STEP-DEBUGGER:\n");
                printf("    Interactive tool to run algorithms step-by-step to examine variable "
                       "changes and pointer updates.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;
        }
    }
}
