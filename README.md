
# C_DSA_interactive_suite

A modular, console-based **Data Structures & Algorithms library** written entirely in **C**, built from scratch with pointer-level control, manual memory management (`malloc` / `free`), and defensive input validation.

This project emphasizes **conceptual clarity**, **low-level fundamentals**, and **explicit memory reasoning**. It is designed with an educational intent, allowing learners to observe, experiment with, and understand data structures and algorithms step-by-step through an interactive terminal-based interface.

The codebase is structured as a reusable **DSA core**, with an interactive, console-driven **demo layer** built on top.

---

## Demos: 

## Bubble Sort (step-by-step)

[![asciinema demo](https://asciinema.org/a/750uLyKqEysvvL34.svg)](https://asciinema.org/a/750uLyKqEysvvL34)

## Binary Search Tree (step-by-step)

[![asciicast](https://asciinema.org/a/nmJbUTDtEnjjGk5k.svg)](https://asciinema.org/a/nmJbUTDtEnjjGk5k?speed=1.5)

---

## Continuous Integration

[![CI](https://github.com/darshan2456/C_DSA_interactive_suite/actions/workflows/ci.yml/badge.svg)](https://github.com/darshan2456/C_DSA_interactive_suite/actions/workflows/ci.yml)

Individual tests for circular queues and binary_search_trees

on push:
- Spins up a new VM instance with latest ubuntu version
- Runs make test

## Project Overview

### Data Structures
- Singly Linked List (SLL)
- Doubly Linked List (DLL)
- Circular Queue (array-based)
- Stack (array-based / linked-list-based as required)
- Binary Search Tree (BST)

### Algorithms

#### Expression Processing
- Infix → Postfix conversion
- Postfix expression evaluation

#### Searching
- Linear Search
- Binary Search

#### Sorting (O(n²) family)
- Bubble Sort
- Selection Sort
- Insertion Sort

#### Graph Traversals
- Breadth-First Search (BFS)
- Depth-First Search (DFS)

Graph traversals are implemented using:
- An adjacency matrix representation
- An explicit queue for BFS
- An explicit stack for DFS

Both BFS and DFS are implemented **iteratively** (no recursion).

#### Hashing Algorithms
-Linear Probing
-Separate Chaining

Linear Probing uses modulo arithmetic to wrap-around the hash table/array when last index is full, optimizing resources and using the full array. 
Separate Chaining uses sll API from the 'data_structures' folders

---

## Build Instructions (Recommended)

This project includes a **Makefile** to simplify building across multiple directories.

### Requirements
- GNU Make ≥ 3.81
- GCC (or a compatible C compiler)

### Build
```bash
make
```

This generates a single executable:

* `demo` (Linux / macOS)
* `demo.exe` (Windows)

### Clean

```bash
make clean
```

---

## Manual Build (Without Make)

### Linux / macOS

```bash
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
```

### Windows

```bash
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
```

This mirrors exactly what the Makefile performs.

---

## Time Complexity

### Searching Algorithms

* Linear Search: **O(n)**
* Binary Search: **O(log n)**

### Sorting Algorithms

* Bubble Sort: **O(n²)**
* Selection Sort: **O(n²)**
* Insertion Sort: **O(n²)**

### Graph Traversals (Adjacency Matrix)

* BFS: **O(V²)**
* DFS: **O(V²)**

---

## Design & Implementation Philosophy

This project is structured as a **real C software system**, not a collection of isolated snippets.
All modules are interconnected and accessible through a single executable via an interactive menu-driven interface.

### Built Completely from Scratch

All implementations rely on:

* Raw pointers
* Dynamic memory allocation (`malloc`, `free`)
* Explicit ownership and lifetime reasoning

---

## Pointer-Level Reasoning

### Singly Linked List (SLL)

* Traversal using `next` pointers
* Safe insertion and deletion
* Head and tail edge cases handled
* In-place list reversal using the classic three-pointer technique (`prev`, `curr`, `next`)

### Doubly Linked List (DLL)

* Bidirectional traversal using `prev` / `next`
* Correct invariant maintenance during insertion and deletion
* Head and tail edge cases handled

Strict attention is paid to:

* Pointer validity
* Memory ownership
* Avoiding dangling references

---

## Graph Traversals (BFS & DFS)

* Graphs are represented using an adjacency matrix
* BFS uses the circular queue from the `data_structures` module
* DFS uses an explicit stack from the `expression_evaluation` module
* `visited[]` invariants are strictly enforced
* Traversals are iterative (non-recursive)

---

## Modularity & Header Discipline

The codebase follows strict modular design rules:

* One `.h` / `.c` pair per logical module
* No function definitions inside headers
* No duplicate symbols across translation units
* Explicit namespacing via function prefixes

Each directory acts as an independent module, making the system easy to extend, debug, or refactor.

---

## Language Features Used Deliberately

* `static` for file-local helper functions
* `const` for API correctness and pointer safety
* Macro `INPUT_EXIT_SIGNAL` (defined in `safe_input.h`) for:

  * Consistent exit signaling
  * Uniform validation behavior

---

## Robust Input Validation

All user input across the entire application is handled via:

```
safe_input_int()
```

Validation is implemented through custom-built helper functions, not ad-hoc checks.

Examples include:

* Infix expression validation (`validate_infix_expr`)

  * Allowed tokens
  * Balanced parentheses
* Postfix expression validation (`validate_postfix_expr`)

  * Stack depth invariants
* Numeric range validation for searching, sorting, and graph input

Invalid input:

* Cannot crash the program
* Is rejected, cleaned, and retried safely

---

## Expression Evaluation

* Stack implementation resides in `expression_evaluation`
* Infix → Postfix conversion using:

  * Operator precedence
  * Parentheses handling
* Postfix evaluation via a stack execution model

This is a classic two-phase algorithm implemented with full control over execution flow and state.

---

## Purpose of the Project

* Strengthen low-level C fundamentals
* Understand how abstractions are built, not just used
* Practice real debugging (linker errors, input desynchronization, infinite loops)
* Develop confidence in systems-level programming

---

## License
This project is licensed under the MIT License - see the LICENSE file for details.

---

## Author

**Darshan Parekh**
B.Sc. Computer Science

### Interests

* Systems Programming
* Open-Source Software
* Cybersecurity
* Low-Level Engineering