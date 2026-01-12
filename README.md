🧠 DSA_using_C

A modular, console-based Data Structures & Algorithms library written entirely in C, built from scratch with pointer-level control, manual memory management, and robust input validation.

This project emphasizes foundational correctness, systems-level thinking, and engineering discipline over shortcuts, abstractions, or library crutches.

⚙️ Implemented as a reusable DSA core with an interactive, console-driven demo layer on top. Designed to be run from a terminal to observe algorithm behavior step-by-step

📁 Project Overview

📦 Data Structures

Singly Linked List (SLL)

Doubly Linked List (DLL)

Circular Queue (array-based)

Binary Search Tree (BST)

⚙️ Algorithms

Infix → Postfix conversion

Postfix expression evaluation

Linear Search, Binary Search

Bubble Sort, Selection Sort, Insertion Sort

🛠️ Build Instructions (Recommended)

This project includes a Makefile to simplify building across multiple directories.

✅ Requirements

GNU Make ≥ 3.81

GCC (or compatible C compiler)

▶️ Build
make

This generates a single executable:     demo (or demo.exe on Windows)

🧹 Clean
make clean

🔧 Manual Build (Without Make)

If you prefer manual compilation:

gcc -Wall -Wextra -std=c11 -g ^
-Idata_structures ^
-Iexpression_evaluation ^
-Isorting_algorithms ^
-Isearching_algorithms ^
data_structures/*.c ^
expression_evaluation/*.c ^
sorting_algorithms/*.c ^
searching_algorithms/*.c ^
-o demo

This mirrors exactly what the Makefile does.

⏱️ Time Complexity

🔍 Searching Algorithms

Algorithm	    | Time Complexity
                |
Linear Search	| O(n)
Binary Search	| O(log n)

🔃 Sorting Algorithms

Algorithm	       | Time Complexity
                   |
Bubble Sort	       | O(n²)
Selection Sort	   | O(n²)
Insertion Sort	   | O(n²)

🧠 Design & Implementation Philosophy

This project is not a collection of snippets — it is structured as a small but real C software system.

🔨 Built Completely from Scratch

No STL

No external libraries

No code generators

No copy-paste templates

All data structures and algorithms are implemented manually using:

raw pointers

dynamic memory allocation (malloc, free)

explicit ownership and lifetime reasoning

🔁 Pointer-Level Reasoning (SLL & DLL)

The linked list implementations focus on explicit pointer manipulation, not abstract reasoning.

🔹 Singly Linked List (SLL)

Traversal using next pointers

Safe insertion and deletion, Edge cases handled for head and tail operations

In-place list reversal implemented and integrated into the console application

Uses classic prev, curr, next pointer reassignment

No extra memory or auxiliary structures

🔹 Doubly Linked List (DLL)

Bidirectional traversal via prev / next

Correct invariant maintenance during insertion and deletion

Edge cases handled for head and tail operations

All operations are written with strict attention to:

pointer validity

memory ownership

avoiding dangling references

🧩 Modularity & Header Discipline

The codebase follows strict modular design rules:

One .h / .c pair per logical module

No function definitions inside headers

No duplicate symbols across translation units

Explicit namespacing via function prefixes

🔐 Language Features Used Deliberately

'static' for file-local helper functions

'const' for API correctness and pointer safety

Macro 'INPUT_EXIT_SIGNAL' (defined in safe_input.h) for:   exit signal

consistent validation behavior

Each directory acts as an independent module, making the system easy to extend or refactor.

🛡️ Robust Input Validation (Key Highlight)

The following modules implement dedicated, unbreakable input validation:

/expression_evaluation

/sorting_algorithms

/searching_algorithms

Validation is handled via custom-built helper functions, not ad-hoc checks.

Examples:

Infix expression validation (lexical, not semantic):- 

allowed tokens
parentheses balance

Postfix expression validation:- 

stack depth invariants
Numeric range validation for searching and sorting

❌ Invalid input cannot crash the program
✅ It is rejected, cleaned, and retried safely

🧮 Expression Evaluation (Stack-Driven)

Stack implementation lives inside /expression_evaluation

Infix → Postfix conversion using:

operator precedence

parentheses handling

Postfix evaluation via a stack execution model

This is a classic two-phase algorithm, implemented with full control over flow and state.

🎯 Purpose of the Project

Strengthen low-level C fundamentals

Understand how abstractions are built, not just used

Practice real debugging (linker errors, input desync, infinite loops)

Develop confidence in systems-level programming


👤 Author

Darshan Parekh
🎓 B.Sc. Computer Science

Interests

Systems programming

Open-source software

Cybersecurity

Low-level engineering