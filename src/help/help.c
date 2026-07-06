#include "help.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_help_command(const char* input)
{
    if (!input) return 0;
    
    char lower[32];
    int i = 0;
    while (input[i] && i < 31)
    {
        lower[i] = (char)tolower((unsigned char)input[i]);
        i++;
    }
    lower[i] = '\0';
    
    return strcmp(lower, "help") == 0;
}

static void print_banner(void)
{
    printf("\n");
    printf("  ========================================================================\n");
    printf("  ||                                                                    ||\n");
    printf("  ||              C DSA INTERACTIVE SUITE - HELP GUIDE                  ||\n");
    printf("  ||                                                                    ||\n");
    printf("  ========================================================================\n");
}

static void print_usage(void)
{
    printf("\n");
    printf("  USAGE\n");
    printf("  -----\n");
    printf("  Type 'help' at any prompt to see this help page.\n");
    printf("  Type 'help <topic>' for specific help on a topic.\n");
    printf("  Type '-1' to go back/exit from any menu.\n");
    printf("\n");
    printf("  TOPICS\n");
    printf("  ------\n");
    printf("  Type 'help <topic>' where <topic> is one of:\n");
    printf("\n");
    printf("    navigation     - How to navigate the menus\n");
    printf("    data-structures - Arrays, Linked Lists, Stacks, Queues\n");
    printf("    sorting        - Sorting algorithms (Bubble, Quick, Merge, etc.)\n");
    printf("    searching      - Linear and Binary search\n");
    printf("    graphs         - Graph traversals and algorithms\n");
    printf("    trees          - Binary trees and tree operations\n");
    printf("    algorithms     - Dynamic programming, Backtracking\n");
    printf("    debugging      - Algorithm step-debugger\n");
    printf("    profiling      - Memory and performance profiling\n");
    printf("    export         - Export and serialization\n");
    printf("    commands       - List of all commands\n");
    printf("\n");
}

static void print_navigation_help(void)
{
    printf("\n");
    printf("  NAVIGATION HELP\n");
    printf("  ----------------\n");
    printf("\n");
    printf("  The suite uses a numbered menu system.\n");
    printf("\n");
    printf("  1. MAIN MENU\n");
    printf("     - Shows all available modules (1-24+)\n");
    printf("     - Enter a number to select a module\n");
    printf("     - Enter -1 to exit\n");
    printf("\n");
    printf("  2. SUB-MENUS\n");
    printf("     - Each module has its own menu\n");
    printf("     - Select algorithm/demo by number\n");
    printf("     - Enter -1 to return to main menu\n");
    printf("\n");
    printf("  3. KEYBOARD SHORTCUTS\n");
    printf("     - Type 'help' at any prompt for help\n");
    printf("     - Type '-1' to exit/go back\n");
    printf("     - Press Enter after input\n");
    printf("\n");
    printf("  4. INTERACTIVE MODES\n");
    printf("     - Some algorithms pause for input\n");
    printf("     - Step-debugger: Press P/R/S/Q for controls\n");
    printf("     - TUI mode: Use arrow keys and Enter\n");
    printf("\n");
}

static void print_data_structures_help(void)
{
    printf("\n");
    printf("  DATA STRUCTURES\n");
    printf("  -----------------\n");
    printf("\n");
    printf("  Arrays\n");
    printf("    - Basic sequential data storage\n");
    printf("    - O(1) random access, O(n) insert/delete\n");
    printf("    - Menu: 2\n");
    printf("\n");
    printf("  Linked Lists (SLL, DLL, CLL, CSCL)\n");
    printf("    - Singly/Doubly/Circular variants\n");
    printf("    - O(1) insert, O(n) search\n");
    printf("    - Menu: 3\n");
    printf("\n");
    printf("  Stacks\n");
    printf("    - LIFO (Last In First Out)\n");
    printf("    - Used in expression evaluation, recursion\n");
    printf("    - Menu: 4\n");
    printf("\n");
    printf("  Queues (Simple, Circular, Priority)\n");
    printf("    - FIFO (First In First Out)\n");
    printf("    - Used in scheduling, BFS\n");
    printf("    - Menu: 5\n");
    printf("\n");
    printf("  Hash Tables\n");
    printf("    - Key-value storage with hashing\n");
    printf("    - O(1) average case operations\n");
    printf("    - Menu: 11\n");
    printf("\n");
}

static void print_sorting_help(void)
{
    printf("\n");
    printf("  SORTING ALGORITHMS\n");
    printf("  ------------------\n");
    printf("\n");
    printf("  O(n^2) Algorithms:\n");
    printf("    - Bubble Sort: Simple, educational\n");
    printf("    - Selection Sort: Min element selection\n");
    printf("    - Insertion Sort: Efficient for small/nearly sorted\n");
    printf("    - Shell Sort: Improved insertion sort\n");
    printf("\n");
    printf("  O(n log n) Algorithms:\n");
    printf("    - Merge Sort: Stable, divide-and-conquer\n");
    printf("    - Quick Sort: Fast in practice\n");
    printf("    - Heap Sort: Uses binary heap\n");
    printf("\n");
    printf("  Special Algorithms:\n");
    printf("    - Radix Sort: Non-comparison, O(nk)\n");
    printf("    - Count Sort: For integers, O(n+k)\n");
    printf("    - Bucket Sort: Distribute and sort\n");
    printf("\n");
    printf("  Use the step-debugger to see how each works!\n");
}

static void print_searching_help(void)
{
    printf("\n");
    printf("  SEARCHING ALGORITHMS\n");
    printf("  --------------------\n");
    printf("\n");
    printf("  Linear Search\n");
    printf("    - Check each element sequentially\n");
    printf("    - Time: O(n)\n");
    printf("    - Works on unsorted arrays\n");
    printf("\n");
    printf("  Binary Search\n");
    printf("    - Divide array in half each step\n");
    printf("    - Time: O(log n)\n");
    printf("    - Requires sorted array\n");
    printf("\n");
    printf("  Jump Search\n");
    printf("    - Jump by fixed step, then linear search\n");
    printf("    - Time: O(sqrt(n))\n");
    printf("\n");
    printf("  Exponential Search\n");
    printf("    - Find range, then binary search\n");
    printf("    - Time: O(log n) for bounded\n");
    printf("\n");
}

static void print_graphs_help(void)
{
    printf("\n");
    printf("  GRAPHS\n");
    printf("  ------\n");
    printf("\n");
    printf("  Traversals:\n");
    printf("    - BFS (Breadth-First Search): Level by level\n");
    printf("    - DFS (Depth-First Search): Deep first\n");
    printf("\n");
    printf("  Algorithms:\n");
    printf("    - Dijkstra: Shortest path (weighted)\n");
    printf("    - Bellman-Ford: Handles negative weights\n");
    printf("    - Floyd-Warshall: All pairs shortest path\n");
    printf("    - Topological Sort: DAG ordering\n");
    printf("\n");
    printf("  Representations:\n");
    printf("    - Adjacency Matrix: O(1) edge check\n");
    printf("    - Adjacency List: O(V+E) traversal\n");
    printf("\n");
}

static void print_trees_help(void)
{
    printf("\n");
    printf("  TREES\n");
    printf("  -----\n");
    printf("\n");
    printf("  Binary Trees:\n");
    printf("    - Each node has at most 2 children\n");
    printf("    - Traversals: Inorder, Preorder, Postorder\n");
    printf("\n");
    printf("  Binary Search Trees (BST):\n");
    printf("    - Left < Root < Right\n");
    printf("    - O(log n) average operations\n");
    printf("\n");
    printf("  Threaded Binary Trees (TBT):\n");
    printf("    - NULL pointers used as threads\n");
    printf("    - Efficient inorder traversal\n");
    printf("\n");
    printf("  Heaps:\n");
    printf("    - Max-Heap: Parent >= Children\n");
    printf("    - Min-Heap: Parent <= Children\n");
    printf("    - Used in priority queues\n");
    printf("\n");
    printf("  Advanced:\n");
    printf("    - AVL Trees: Self-balancing\n");
    printf("    - Red-Black Trees: Balanced, O(log n)\n");
    printf("\n");
}

static void print_algorithms_help(void)
{
    printf("\n");
    printf("  ALGORITHMS\n");
    printf("  ----------\n");
    printf("\n");
    printf("  Dynamic Programming (DP):\n");
    printf("    - Break problem into subproblems\n");
    printf("    - Memoization / Tabulation\n");
    printf("    - Examples: Fibonacci, Knapsack, LCS\n");
    printf("\n");
    printf("  Backtracking:\n");
    printf("    - Build solutions incrementally\n");
    printf("    - Backtrack on failure\n");
    printf("    - Examples: N-Queens, Sudoku, Rat in Maze\n");
    printf("\n");
    printf("  Greedy Algorithms:\n");
    printf("    - Make locally optimal choice\n");
    printf("    - May not give global optimum\n");
    printf("    - Examples: Job Scheduling, Huffman Coding\n");
    printf("\n");
    printf("  Divide and Conquer:\n");
    printf("    - Divide, Conquer, Combine\n");
    printf("    - Examples: Merge Sort, Quick Sort\n");
    printf("\n");
}

static void print_debugging_help(void)
{
    printf("\n");
    printf("  DEBUGGING & PROFILING\n");
    printf("  ---------------------\n");
    printf("\n");
    printf("  Step Debugger (Menu 23):\n");
    printf("    - Pause and step through algorithms\n");
    printf("    - Controls: P=pause, R=resume, S=step, Q=quit\n");
    printf("    - Press H to see event history\n");
    printf("\n");
    printf("  Fuzz Testing (Menu 21):\n");
    printf("    - Random operations to find bugs\n");
    printf("    - Tracks seed for reproducibility\n");
    printf("    - Run with: make fuzz-test\n");
    printf("\n");
    printf("  Memory Inspector (Menu 20):\n");
    printf("    - View memory as hex dump\n");
    printf("    - Analyze structure padding\n");
    printf("    - Compare memory regions\n");
    printf("\n");
    printf("  Profiling:\n");
    printf("    - Benchmark execution time\n");
    printf("    - Track memory usage\n");
    printf("    - Export to CSV\n");
    printf("\n");
}

static void print_export_help(void)
{
    printf("\n");
    printf("  EXPORT & SERIALIZATION\n");
    printf("  ----------------------\n");
    printf("\n");
    printf("  Telemetry Exporter (Menu 24):\n");
    printf("    - Export algorithm traces to JSON/CSV\n");
    printf("    - Use with web visualizers\n");
    printf("    - Records comparisons, swaps, array states\n");
    printf("\n");
    printf("  Serialization (Menu 22):\n");
    printf("    - Save data structures to files\n");
    printf("    - Load and restore state\n");
    printf("    - Supports: arrays, trees, graphs, lists\n");
    printf("\n");
    printf("  Big-O Engine (Menu 19):\n");
    printf("    - Verify algorithm complexity empirically\n");
    printf("    - Measures T(2N)/T(N) ratio\n");
    printf("    - Compares with expected complexity\n");
    printf("\n");
}

static void print_commands_help(void)
{
    printf("\n");
    printf("  COMMAND REFERENCE\n");
    printf("  -----------------\n");
    printf("\n");
    printf("  GLOBAL COMMANDS:\n");
    printf("    help          - Show this help page\n");
    printf("    help <topic>  - Show specific topic help\n");
    printf("    -1            - Exit/go back\n");
    printf("\n");
    printf("  MENU OPTIONS (Main Menu):\n");
    printf("    1   - Arrays\n");
    printf("    2   - Linked Lists (SLL, DLL, CLL, CSCL)\n");
    printf("    3   - Stacks\n");
    printf("    4   - Queues\n");
    printf("    5   - Trees\n");
    printf("    6   - Sorting Algorithms\n");
    printf("    7   - Searching Algorithms\n");
    printf("    8   - Graphs\n");
    printf("    9   - Hashing\n");
    printf("    10  - Expression Evaluation\n");
    printf("    11  - Error Correction Codes\n");
    printf("    12  - Job Scheduling\n");
    printf("    13  - Dynamic Programming\n");
    printf("    14  - String Algorithms\n");
    printf("    15  - Backtracking\n");
    printf("    16  - Process Synchronization\n");
    printf("    17  - Advanced Graph Algorithms\n");
    printf("    18  - Advanced Heaps\n");
    printf("    19  - Big-O Verification Engine\n");
    printf("    20  - Memory Inspector / Hexdump\n");
    printf("    21  - Fuzz Testing Engine\n");
    printf("    22  - Serialization Engine\n");
    printf("    23  - Algorithm Step-Debugger\n");
    printf("    24  - Telemetry Exporter\n");
    printf("\n");
    printf("  DEBUGGER CONTROLS:\n");
    printf("    P - Pause execution\n");
    printf("    R - Resume execution\n");
    printf("    S - Step forward one operation\n");
    printf("    Q - Quit step mode (run at full speed)\n");
    printf("    H - Show event history\n");
    printf("\n");
}

void show_help(void)
{
    print_banner();
    print_usage();
}

void show_help_for_topic(const char* topic)
{
    if (!topic)
    {
        show_help();
        return;
    }

    if (strcmp(topic, "navigation") == 0 || strcmp(topic, "nav") == 0)
    {
        print_navigation_help();
    }
    else if (strcmp(topic, "data-structures") == 0 || strcmp(topic, "ds") == 0)
    {
        print_data_structures_help();
    }
    else if (strcmp(topic, "sorting") == 0)
    {
        print_sorting_help();
    }
    else if (strcmp(topic, "searching") == 0)
    {
        print_searching_help();
    }
    else if (strcmp(topic, "graphs") == 0)
    {
        print_graphs_help();
    }
    else if (strcmp(topic, "trees") == 0)
    {
        print_trees_help();
    }
    else if (strcmp(topic, "algorithms") == 0 || strcmp(topic, "algo") == 0)
    {
        print_algorithms_help();
    }
    else if (strcmp(topic, "debugging") == 0 || strcmp(topic, "debug") == 0)
    {
        print_debugging_help();
    }
    else if (strcmp(topic, "export") == 0)
    {
        print_export_help();
    }
    else if (strcmp(topic, "commands") == 0 || strcmp(topic, "cmd") == 0)
    {
        print_commands_help();
    }
    else
    {
        printf("\n");
        printf("  Unknown topic: '%s'\n", topic);
        printf("  Type 'help' to see available topics.\n");
        printf("\n");
    }
}

void help_demo(void)
{
    display_header("Help Page");

    printf("\nThis is the Help Demo showing available help topics.\n");
    printf("In the actual application, type 'help' at any prompt.\n\n");

    printf("Available topics:\n");
    printf("  help navigation    - How to navigate menus\n");
    printf("  help data-structures - Arrays, Lists, Stacks, Queues\n");
    printf("  help sorting       - Sorting algorithms\n");
    printf("  help searching     - Search algorithms\n");
    printf("  help graphs       - Graph algorithms\n");
    printf("  help trees        - Tree structures\n");
    printf("  help algorithms   - DP, Backtracking, Greedy\n");
    printf("  help debugging    - Debugger and profiler\n");
    printf("  help export       - Export and serialization\n");
    printf("  help commands     - Command reference\n");
    printf("\n");

    int choice;
    printf("  Try a topic (1-10) or 0 to exit: ");
    scanf("%d", &choice);
    while (getchar() != '\n');

    switch (choice)
    {
        case 1:  print_navigation_help(); break;
        case 2:  print_data_structures_help(); break;
        case 3:  print_sorting_help(); break;
        case 4:  print_searching_help(); break;
        case 5:  print_graphs_help(); break;
        case 6:  print_trees_help(); break;
        case 7:  print_algorithms_help(); break;
        case 8:  print_debugging_help(); break;
        case 9:  print_export_help(); break;
        case 10: print_commands_help(); break;
        default: break;
    }
}