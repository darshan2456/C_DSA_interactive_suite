
# C_DSA_interactive_suite

A modular, console-based **Data Structures & Algorithms library** written entirely in **C**, built from scratch with pointer-level control, manual memory management (`malloc` / `free`), and defensive input validation.

This project emphasizes **conceptual clarity**, **low-level fundamentals**, and **explicit memory reasoning**. It is designed with an educational intent, allowing learners to observe, experiment with, and understand data structures and algorithms step-by-step through an interactive terminal-based interface.

The codebase is structured as a reusable **DSA core**, with an interactive, console-driven **demo layer** built on top.

---

## Demos: 

## Infix to Postfix and Postfix evaluation (step by step)

[![asciinema demo](https://asciinema.org/a/jOr0YxJx25MnjKVK.svg)](https://asciinema.org/a/jOr0YxJx25MnjKVK)

## Hashing algorithms (step-by-step)

[![asciinema demo](https://asciinema.org/a/jmMx2LTJSQJX8Re4.svg)](https://asciinema.org/a/jmMx2LTJSQJX8Re4)

## Binary Search Tree (step-by-step)

[![asciinema demo](https://asciinema.org/a/nmJbUTDtEnjjGk5k.svg)](https://asciinema.org/a/nmJbUTDtEnjjGk5k?speed=1.5)

---

## Continuous Integration

[![CI](https://github.com/darshan2456/C_DSA_interactive_suite/actions/workflows/ci.yml/badge.svg)](https://github.com/darshan2456/C_DSA_interactive_suite/actions/workflows/ci.yml)

This project includes a **GitHub Actions CI pipeline** that automatically verifies code correctness and memory safety.

On every push or pull request:

1. A fresh **Ubuntu VM** is allocated
2. The project is **compiled using GCC**
3. The complete **unit test suite is executed**
4. All test binaries are run under **Valgrind** to check for:

   - memory leaks  
   - invalid reads / writes  
   - use-after-free errors  
   - uninitialized memory usage

If any test fails or Valgrind detects a memory error, the CI job fails automatically.

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

#### Advanced Sorting Algorithms
- Quick sort
- Merge sort

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

* `dsa` (Linux / macOS)
* `dsa.exe` (Windows)

### Clean

```bash
make clean
```

---

## Manual Build (Without Make)

### Linux / macOS

```bash
gcc -Wall -Wextra -std=c11 -g \
-Isrc/data_structures \
-Isrc/expression_evaluation \
-Isrc/sorting_algorithms_n2 \
-Isrc/advanced_sorting_algorithms \
-Isrc/searching_algorithms \
-Isrc/graph_traversals \
-Isrc/hashing \
src/data_structures/*.c \
src/expression_evaluation/*.c \
src/sorting_algorithms_n2/*.c \
src/advanced_sorting_algorithms/*.c \
src/searching_algorithms/*.c \
src/graph_traversals/*.c \
src/hashing/*.c \
-o dsa
```

### Windows

```bash
gcc -Wall -Wextra -std=c11 -g ^
-Isrc/data_structures ^
-Isrc/expression_evaluation ^
-Isrc/sorting_algorithms_n2 ^
-Isrc/advanced_sorting_algorithms ^
-Isrc/searching_algorithms ^
-Isrc/graph_traversals ^
-Isrc/hashing ^
src/data_structures/*.c ^
src/expression_evaluation/*.c ^
src/sorting_algorithms_n2/*.c ^
src/advanced_sorting_algorithms/*.c ^
src/searching_algorithms/*.c ^
src/graph_traversals/*.c ^
src/hashing/*.c ^
-o dsa.exe
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

### Advanced Sorting Algorithms

* Quick sort: **O(n²)**
* Merge sort: **O(nlogn)**

### Graph Traversals (Adjacency List)

* BFS: **O(V+E)**
* DFS: **O(V+E)**

---



## Project Features

### Graph Traversals (BFS & DFS)

* Graphs are represented using an adjacency list
* BFS uses the circular queue from the `data_structures` module
* DFS uses an explicit stack from the `expression_evaluation` module
* `visited[]` invariants are strictly enforced
* Traversals are iterative (non-recursive)

---

### Expression Evaluation

* Stack implementation resides in `expression_evaluation`
* Infix → Postfix conversion using:

  * Operator precedence
  * Parentheses handling
* Postfix evaluation via a stack execution model

This is a classic two-phase algorithm implemented with full control over execution flow and state.

---

### Modularity & Header Discipline

The codebase follows strict modular design rules:

* One `.h` / `.c` pair per logical module
* No function definitions inside headers
* No duplicate symbols across translation units
* Explicit namespacing via function prefixes

Each directory acts as an independent module, making the system easy to extend, debug, or refactor.

---

### Language Features Used Deliberately

* `static` for file-local helper functions
* `const` for API correctness and pointer safety
* Macro `INPUT_EXIT_SIGNAL` (defined in `safe_input.h`) for:

  * Consistent exit signaling
  * Uniform validation behavior

---

### Robust Input Validation

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


## License
This project is licensed under the MIT License - see the LICENSE file for details.

---

## Author

**Darshan Parekh**

Aspiring systems engineer and cybersecurity engineer