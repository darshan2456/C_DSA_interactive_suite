C_DSA_interactive_suite

A modular, console-based Data Structures & Algorithms library written entirely in C, built from scratch with pointer-level control, manual memory management,(malloc/free) and unbreakable/uncrashable input validation.

This project emphasizes conceptual clarity, low-level fundamentals and memory allocation understanding and also it has an educational approach to it. I have made it in a way that it can be used to help future generations understand data structures with visualizations and experimentations.

The codebase is implemented as a reusable DSA core with an interactive, console-driven demo layer on top.
It is designed to be run from a terminal to observe algorithm behavior step-by-step.

Project Overview:-

Data Structures

Singly Linked List (SLL)

Doubly Linked List (DLL)

Circular Queue (array-based)

Stack (array / linked implementation as required)

Binary Search Tree (BST)

Algorithms

Expression Processing

Infix → Postfix conversion

Postfix expression evaluation

Searching

Linear Search

Binary Search

Sorting (O(n²) family)

Bubble Sort

Selection Sort

Insertion Sort

Graph Traversals

Breadth-First Search (BFS)

Depth-First Search (DFS)

Graph traversals are implemented using:

an adjacency matrix representation

an explicit queue (for BFS)

an explicit stack (for DFS)

Both traversals are iterative.

Build Instructions (Recommended)

This project includes a Makefile to simplify building across multiple directories.

Requirements

GNU Make ≥ 3.81

GCC (or a compatible C compiler)

Build
make


This generates a single executable:

demo (Linux / macOS)

demo.exe (Windows)

Clean
make clean

Manual Build (Without Make):-

(If you prefer manual compilation)

(Linux/macOS):

gcc -Wall -Wextra -std=c11 -g \
-Idata_structures \
-Iexpression_evaluation \
-Isorting_algorithms \
-Isearching_algorithms \
-Igraph_traversals_bfs_dfs \
data_structures/*.c \
expression_evaluation/*.c \
sorting_algorithms/*.c \
searching_algorithms/*.c \
graph_traversals_bfs_dfs/*.c \
-o demo

For windows:

gcc -Wall -Wextra -std=c11 -g ^
-Idata_structures ^
-Iexpression_evaluation ^
-Isorting_algorithms ^
-Isearching_algorithms ^
-Igraph_traversals_bfs_dfs ^
data_structures/*.c ^
expression_evaluation/*.c ^
sorting_algorithms/*.c ^
searching_algorithms/*.c ^
graph_traversals_bfs_dfs/*.c ^
-o demo

This mirrors exactly what the Makefile performs.

Time Complexity
Searching Algorithms-
Algorithm	Time Complexity
Linear Search	O(n)
Binary Search	O(log n)
Sorting Algorithms-
Algorithm	Time Complexity
Bubble Sort	O(n²)
Selection Sort	O(n²)
Insertion Sort	O(n²)
Graph Traversals-
Algorithm	Time Complexity
BFS	O(V²) (Adjacency Matrix)
DFS	O(V²) (Adjacency Matrix)

Design & Implementation Philosophy:-

It is structured as a small but REAL C software system.
This is an interconnected program where you can go to any module from anywhere just by following the instructions on the screen. You can test all data structures and algorithms from one execution of the .exe file...

Built Completely from Scratch

No STL

No external libraries

No code generators

No copy-paste templates

All data structures and algorithms are implemented manually using:

raw pointers

dynamic memory allocation (malloc, free)

explicit ownership and lifetime reasoning

Pointer-Level Reasoning (SLL & DLL)

The linked list implementations focus on explicit pointer manipulation, not abstract reasoning.

Singly Linked List (SLL):-

Traversal using next pointers

Safe insertion and deletion

Edge cases handled for head and tail

In-place list reversal using classic three pointers technique (prev, curr and next)

Doubly Linked List (DLL):-

Bidirectional traversal via prev / next

Correct invariant maintenance during insertion and deletion

Edge cases handled for head and tail

Strict attention to:

pointer validity

memory ownership

avoiding dangling references

Graph Traversals (BFS & DFS):-

Graph traversal logic is implemented with the same discipline as core data structures.

Graphs are represented using an adjacency matrix

BFS uses a circular queue implementation from the data_structures module

DFS uses an explicit stack implementation from the expression_evaluation module

visited[] invariants are strictly enforced

Traversals are iterative (no recursion)

Modularity & Header Discipline

The codebase follows strict modular design rules:

One .h / .c pair per logical module

No function definitions inside headers

No duplicate symbols across translation units

Explicit namespacing via function prefixes

Each directory acts as an independent module, making the system easy to extend, debug or refactor.

Language Features Used Deliberately

static for file-local helper functions

const for API correctness and pointer safety

Macro INPUT_EXIT_SIGNAL (defined in safe_input.h) for:

consistent exit signaling

uniform validation behavior

Robust Input Validation (Key Highlight):-

input anywhere and everywhere is taken via a function 'safe_input_int()'

Validation is handled via custom-built helper functions, not ad-hoc checks.

Examples:

Infix expression validation:(validate_infix_expr())

allowed tokens

balanced parentheses

Postfix expression validation:(validate_postfix_expr())

stack depth invariants

Numeric range validation for searching, sorting, and graph input

Invalid input cannot crash the program.
It is rejected, cleaned, and retried safely.

Expression Evaluation (Stack-Driven):- 

Stack implementation lives inside expression_evaluation

Infix → Postfix conversion using:

operator precedence

parentheses handling

Postfix evaluation via a stack execution model

This is a classic two-phase algorithm implemented with full control over flow and state.

Purpose of the Project:- 

Strengthen low-level C fundamentals

Understand how abstractions are built, not just used

Practice real debugging (linker errors, input desync, infinite loops)

Develop confidence in systems-level programming

Author

Darshan Parekh
B.Sc. Computer Science

Interests

Systems programming

Open-source software

Cybersecurity

Low-level engineering