

# C_DSA_interactive_suite

A modular, console-based **Data Structures & Algorithms library** written entirely in **C**, built from scratch with pointer-level control, manual memory management (`malloc` / `free`), and defensive input validation.

This project emphasizes **conceptual clarity**, **low-level fundamentals**, and **explicit memory reasoning**. It is designed with an educational intent, allowing learners to observe, experiment with, and understand data structures and algorithms step-by-step through an interactive terminal-based interface.

The codebase is structured as a reusable **DSA library**, with an interactive, console-driven **demo layer** built on top.

---

## Table of Contents
- [Demos](#demos)
- [Build Instructions](#build-instructions-recommended)
- [Continuous Integration](#continuous-integration)
- [Project Overview](#project-overview)
- [Time Complexity](#time-complexity)
- [Project Features](#project-features)
- [License](#license)

---

## Demos

## Infix to Postfix and Postfix evaluation (step by step)

[![asciinema demo](https://asciinema.org/a/jOr0YxJx25MnjKVK.svg)](https://asciinema.org/a/jOr0YxJx25MnjKVK)

## Hashing algorithms (step-by-step)

[![asciinema demo](https://asciinema.org/a/jmMx2LTJSQJX8Re4.svg)](https://asciinema.org/a/jmMx2LTJSQJX8Re4)

## Binary Search Tree (step-by-step)

[![asciinema demo](https://asciinema.org/a/nmJbUTDtEnjjGk5k.svg)](https://asciinema.org/a/nmJbUTDtEnjjGk5k?speed=1.5)

---

## Build Instructions

This project includes a **Makefile** to simplify building across multiple directories.

### Requirements
- GNU Make ≥ 3.81
- GCC (or a compatible C compiler)

## TUI Requirements

The Text User Interface (TUI) is built using the **Ncurses** library.

### Ubuntu / Debian
```bash
sudo apt install libncurses5-dev libncursesw5-dev
```
### Fedora
```bash
sudo dnf install ncurses-devel
```
### Arch Linux
```bash
sudo pacman -S ncurses
```
> **Note:** The TUI is supported on Unix/Linux systems. On Windows, the project automatically falls back to the legacy CLI interface.

### Build (Makefile)
```bash
make
```
This generates a single executable:
* `dsa` (Linux / macOS)
* `dsa.exe` (Windows)

### Build (CMake)
Alternatively, you can compile the application and tests using CMake:
```bash
mkdir build && cd build
cmake ..
make
```

To execute all unit tests using CTest:
```bash
ctest --output-on-failure
```



```bash
make run 
```
Builds only when necessary and launches the program.


```bash
make test
```
Runs all tests


### Clean

```bash
make clean
```
Removes executables and generated object files.

---

## Architectural Breakdown: Docker & The Build System

### Why Docker?

Docker acts as a cross-platform wrapper around the build system. Contributors on Windows, macOS, and Linux can use the same isolated Ubuntu environment without manually configuring compiler toolchains, build dependencies, or platform-specific settings.

### Container Build Chain

The current build flow is:

```text
Docker Container
      ↓
   Makefile
      ↓
GCC Compilation
      ↓
dsa Executable
```

The Docker image installs the required build tools and executes the project's Makefile, ensuring consistent builds across different operating systems.

### Relationship Between Docker, Makefile, and CMake

Each component serves a different purpose:

- Docker provides a reproducible Linux build environment.
- The Makefile defines the primary build workflow used by the project today.
- CMakeLists.txt provides an alternative build system that can generate platform-specific build files while supporting testing and future expansion.

These tools are complementary rather than competing solutions.

### Future-Proofing With CMake

The project currently uses a Makefile as its primary build system while also providing CMake support. As the project grows and introduces additional dependencies, CMake can simplify dependency management, testing integration, and cross-platform development while continuing to work inside the Docker environment.

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

### Interface Modes
- Legacy CLI Menu
- Ncurses-based TUI Navigator

### Data Structures
- Singly Linked List (SLL)
- Doubly Linked List (DLL)
- Singly Circular Linked List (SCLL)
- Doubly circularly Linked list(DCLL)
- Simple Queue (Linear Queue, array-based)
- Circular Queue (array-based)
- Double-Ended Queue (Deque) (array-based)
- Stack (array-based / linked-list-based as required)
- Priority Queue(heap based)

### Trees
- Binary Search Tree (BST)-recursive
- Threaded Binary Tree (TBT)
- AVL Tree (self-balancing insertions & deletions)
- Trie
- B+ Tree
- B- Tree


### Algorithms

#### Expression Processing
- Step-by-step visualization for Infix → Postfix conversion
- Step-by-step visualization for Postfix expression evaluation
- visualization for Parentheses Checking

#### Searching
- Linear Search
- Binary Search
- Interpolation Search
- Jump Search

#### Sorting (O(n²) family)
- Bubble Sort
- Selection Sort
- Insertion Sort
- Shell Sort

#### Advanced Sorting Algorithms
- Quick sort
- Merge sort
- Heap sort
- Radix sort (LSD, interactive demo)
- Bucket sort
 
#### Error Correction Algorithms
- Checksum [Sender and Receiver verification]
- Cyclic Redundancy Check (CRC) [Sender and Receiver verification]
- Longitudinal Redundancy Check (LRC) [Sender and Receiver verification]
- Vertical Redundancy Check (VRC) [Sender and Receiver verification]
- Hamming Code [Sender and Receiver verification]
- Parity Bit Generator

#### Backtracking Algorithms
- Rat in a Maze
- Graph Coloring
- Sudoku Solver 6*6
- Knight's tour
- N Queen's algorithm

#### Dynamic Programming
- Fibonacci
- Longest Common Subsequence (LCS)
- 0/1 Knapsack
- Matrix Chain Multiplication (MCM)

#### Job scheduling
* FCFS(First come,First Served)
* SJF( Shortest Job First)
* Priority Scheduling
* SRTF (Shortest Remaining Time First)
* Preemptive Scheduling

#### Graph Algorithms
- GBFS (Greedy Breadth First Search)
- BFS
- DFS
- Dijkstra
- Bellman-Ford
- Floyd-Warshall
- A* (Search algorithm)
- Topological Sort
- Kruskal MST
- Prim MST

Graph traversals are implemented using:
- An adjacency list representation
- An explicit queue for BFS
- An explicit stack for DFS
- Dijkstra is implemented with a special `Edge` struct for weighted nodes 

Both BFS and DFS are implemented **iteratively** (no recursion).

#### Floyd-Warshall

Computes shortest paths between all pairs of vertices.

* Time Complexity: **O(V³)**
* Space Complexity: **O(V²)**


#### Hashing Algorithms
- Linear Probing (with built-in linear search for demo)
- Separate Chaining
- Double Hashing
- Quadratic Probing

Linear Probing uses modulo arithmetic to wrap-around the hash table/array when last index is full, optimizing resources and using the full array. 

Separate Chaining uses sll API from the 'data_structures' folder

Double Hashing uses a second hash function to calculate probe steps, reducing clustering compared to linear probing.

Quadratic Probing resolves collisions by using quadratic increments (i²) to reduce clustering compared to linear probing.

#### String Algorithms

- Naive String Matching
- Knuth-Morris-Pratt (KMP) Algorithm
- Rabin-Karp Algorithm

---

## Time Complexity

### Searching Algorithms
* Linear Search: **O(n)**
* Binary Search: **O(logn)**
* Interpolation Search: **O(log(logn))**
* Jump Search: **O(√n)**

### Sorting Algorithms
* Bubble Sort: **O(n²)**
* Selection Sort: **O(n²)**
* Insertion Sort: **O(n²)**
* Shell sort: **O(nlogn)**

### Advanced Sorting Algorithms
* Quick sort: **O(n²)**
* Merge sort: **O(nlogn)**
* Heap sort: **O(nlogn)**
* Radix sort (LSD): **O(nk)**
* Bucket sort :**O(n+k)**

### Graph Traversals (Adjacency List)
* GBFS: **O(b^m)**
* BFS: **O(V+E)**
* DFS: **O(V+E)**
* Dijkstra's Algorithm: **O((V+E)log V)**
* A* Search: **O((V+E)log V)**
* Greedy Best-First Search: **O((V+E)log V)**
* Bellman-Ford: **O(V·E)**
* Floyd-Warshall: **O(V³)**
* Topological Sort (Kahn's Algorithm): **O(V+E)**

### Job scheduling
* FCFS(First come,First Served): **O(n)**
* SJF( Shortest Job First): **O(nlogn+n)**
* Priority Scheduling: **O(nlogn)**
* SRTF (Shortest Remaining Time First): **O(nlogn)**
* Preemptive Priority Scheduling: **O(nlogn)**

### String Algorithms
* Naive String Matching: **O(n-m+1)**
* Knuth-Morris-Pratt (KMP) Algorithm: **O(n+m)**
* Rabin-Karp Algorithm: **O(n.m)**

### Dynamic Programming
* Fibonacci (DP): **O(n)**
* Longest Common Subsequence (LCS): **O(nm)**
* 0/1 Knapsack: **O(nW)**
* Matrix Chain Multiplication: **O(n³)**

### Backtracking Algorithms
* Rat in a Maze: **O(4^(n²))** 
* Graph Coloring: **O(m^V)**
* Sudoko solver: **O(6^m)**
* Knight's tour:**O(8^(n^2))**
* N Queen's algorithm: **O(N!)**


---

## Project Features

### Queue Implementations

* **Simple Queue (Linear Queue)**  
  - Basic array-based queue with straightforward enqueue/dequeue operations  
  - Limited by “false overflow” when rear reaches the end  

* **Circular Queue**  
  - Optimized array-based queue with wrap-around indexing  
  - Fix applied: initialization of `rollnos` struct to prevent early exit crash  

* **Double-Ended Queue (Deque)**  
  - Implemented using a circular array  
  - Supports insertion and deletion from both ends  
  - Includes overflow/underflow handling and memory cleanup via `destroy_deque()`


### Graph Traversals (BFS & DFS)

* Graphs are represented using an adjacency list
* BFS uses the circular queue from the `data_structures` module
* DFS uses an explicit stack from the `expression_evaluation` module
* `visited[]` invariants are strictly enforced
* Traversals are iterative (non-recursive)
* Dijkstra's shortest path algorithm for weighted graphs
* Priority Queue (min-heap) integrated into Dijkstra for efficient vertex extraction
* Priority Queue support for other graph algorithms and future extensions


### Threaded Binary Tree (TBT)

* Binary tree with threads replacing NULL pointers
* Enables efficient inorder traversal without recursion or stack
* Traversal runs in O(n) time with reduced overhead compared to standard BST traversal
* Useful for educational comparison with BST and AVL Tree


### Priority Queue

* Heap-based implementation
* Efficient insertion and removal operations
* Reusable component for graph algorithms and future extensions


### AVL Tree

* Self-balancing binary search tree  
* Rotations (LL, RR, LR, RL) ensure height balance  
* Guarantees O(log n) search, insertion, and deletion

  
### Terminal User Interface (TUI)

- Built using ncurses
- Provides a structured navigation experience
- Supports browsing algorithms and data structures from a single interface
- Includes split-pane visualizations where applicable
- Can coexist with the traditional menu-driven interface

### Dynamic Programming

- Interactive demonstrations of classic DP problems
- Educational implementations with clear execution flow

### Backtracking Algorithms

- Recursive solution visualization
- Educational exploration of state-space search


### String Algorithms

- Interactive pattern matching demonstrations
- Supports user-defined text and pattern input
- Educational visualization of string searching techniques

#### Naive String Matching

- Compares the pattern at every possible position
- Simple baseline algorithm for learning purposes

#### Knuth-Morris-Pratt (KMP)

- Uses the LPS (Longest Prefix Suffix) array
- Avoids redundant comparisons
- Efficient linear-time pattern matching

#### Rabin-Karp

- Uses rolling hash computation
- Efficient for multiple pattern matching scenarios
- Demonstrates hash-based string searching

---

### Utility Components

- Safe Input System
- Cross-platform screen clearing
- Cross-platform delay utilities
- CSV input support for graph algorithms
- Benchmark logging framework

### Expression Evaluation

* Implementation lives in `expression_evaluation`
* Converts infix expressions to postfix notation using:

  * operator precedence
  * parentheses handling

* Step-by-step visualization of infix-to-postfix conversion, showing the operator stack and current output at each step
* Step-by-step visualization of postfix evaluation, showing operand stack updates and intermediate results
* Includes a parentheses checker with validated input via `get_validated_input_parantheses()` to ensure well-formed expressions before processing

#### Expression Evaluation Enhancements
* Robust validation ensures only well-formed infix/postfix expressions are processed
* Demo rejects invalid tokens and unbalanced parentheses gracefully
* Safe input handling prevents crashes from malformed or unexpected input

---

### Modularity & Header Discipline

The codebase follows strict modular design rules:
* Unified header used in data_structures.
* One `.h` / `.c` pair per logical module
* No function definitions inside headers
* No duplicate symbols across translation units
* Explicit namespacing via function prefixes
* C11-compliant code ensuring portable and standard-safe compilation.



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
