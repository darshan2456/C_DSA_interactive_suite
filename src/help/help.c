#include "help.h"
#include "../utils/display_header.h"
#include <stdio.h>

void display_help(void)
{
    display_header("HELP - DSA Interactive Suite");

    printf("\n");
    printf("Welcome to the DSA Interactive Suite Help!\n");
    printf("=========================================\n");
    printf("\n");

    printf("GENERAL COMMANDS\n");
    printf("-----------------\n");
    printf("  help     - Display this help page\n");
    printf("  X        - Exit the current module/demo\n");
    printf("  -1       - Exit the current demo and return to menu\n");
    printf("\n");

    printf("NAVIGATION\n");
    printf("-----------\n");
    printf("  Enter the number corresponding to the module you want to explore.\n");
    printf("\n");

    printf("AVAILABLE MODULES\n");
    printf("-----------------\n");
    printf("  1.  Data Structures\n");
    printf("      - Linked Lists, Stacks, Queues, Hash Tables\n");
    printf("\n");
    printf("  2.  Expression Evaluation\n");
    printf("      - Infix to Postfix conversion\n");
    printf("      - Postfix evaluation\n");
    printf("\n");
    printf("  3.  Sorting Algorithms (n² family)\n");
    printf("      - Bubble Sort, Selection Sort, Insertion Sort\n");
    printf("\n");
    printf("  4.  Advanced Sorting Algorithms\n");
    printf("      - Merge Sort, Quick Sort, Heap Sort, Shell Sort\n");
    printf("\n");
    printf("  5.  Searching Algorithms\n");
    printf("      - Linear Search, Binary Search\n");
    printf("\n");
    printf("  6.  Graph Traversals\n");
    printf("      - BFS, DFS, Dijkstra's, A*, Greedy BFS, Bellman Ford\n");
    printf("\n");
    printf("  7.  Hashing Algorithms\n");
    printf("      - Various hashing techniques and collision resolution\n");
    printf("\n");
    printf("  8.  Trees\n");
    printf("      - Binary Trees, BST, AVL, B-Trees, Tries\n");
    printf("\n");
    printf("  9.  Error Correction Algorithms\n");
    printf("      - Hamming Code, CRC, Parity Checks\n");
    printf("\n");
    printf("  10. Job Scheduling\n");
    printf("      - FCFS, SJF, Priority, Round Robin\n");
    printf("\n");
    printf("  11. Backtracking Algorithms\n");
    printf("      - N-Queens, Sudoku Solver, Hamiltonian Path\n");
    printf("\n");
    printf("  12. Dynamic Programming\n");
    printf("      - Fibonacci, LCS, Knapsack, Matrix Chain\n");
    printf("\n");
    printf("  13. String Algorithms\n");
    printf("      - Naive String Matching, KMP, Rabin-Karp\n");
    printf("\n");
    printf("  14. Process Synchronization\n");
    printf("      - Producer-Consumer, Readers-Writers, Dining Philosophers\n");
    printf("\n");
    printf("  15. Settings\n");
    printf("      - Configure animation speed and display preferences\n");
    printf("\n");
    printf("  16. Algorithm Benchmarking & Profiling\n");
    printf("      - Performance analysis and memory profiling\n");
    printf("\n");
    printf("  17. Advanced Graph Algorithms\n");
    printf("      - Strongly Connected Components, Max Flow\n");
    printf("      - Bipartite Matching, Eulerian Path\n");
    printf("\n");
    printf("  18. Advanced Heaps & Priority Queues\n");
    printf("      - Fibonacci Heap, Binomial Heap\n");
    printf("\n");

    printf("INPUT GUIDELINES\n");
    printf("-----------------\n");
    printf("  - Enter only valid integer choices when prompted\n");
    printf("  - For string inputs, avoid spaces unless explicitly allowed\n");
    printf("  - Maximum input length is typically 99 characters\n");
    printf("  - Press X at any prompt to exit the current module\n");
    printf("\n");

    printf("TIPS\n");
    printf("----\n");
    printf("  - Watch the animations to understand algorithm behavior\n");
    printf("  - Use benchmarking to compare algorithm performance\n");
    printf("  - Try different input values to explore edge cases\n");
    printf("  - Check settings to adjust animation speed to your preference\n");
    printf("\n");

    printf("For more information, visit the project repository.\n");
    printf("\n");

    printf("Press Enter to continue...");
    while (getchar() != '\n')
        ;
}
