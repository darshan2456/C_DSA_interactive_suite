

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
- Singly Circular Linked List (SCLL)
- Simple Queue (Linear Queue, array-based)
- Circular Queue (array-based)
- Double-Ended Queue (Deque) (array-based)
- Stack (array-based / linked-list-based as required)
- Binary Search Tree (BST)-recursive
- Threaded Binary Tree (TBT)
- Priority Queue(heap based)
- AVL Tree (self-balancing insertions & deletions)

### Algorithms

#### Expression Processing
- Step-by-step visualization for Infix → Postfix conversion
- Step-by-step visualization for Postfix expression evaluation

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
- Heap sort
- Radix sort (LSD, interactive demo)

#### Graph Traversals
- Breadth-First Search (BFS)
- Depth-First Search (DFS)
- Dijkstra's algorithm

Graph traversals are implemented using:
- An adjacency list representation
- An explicit queue for BFS
- An explicit stack for DFS
- Dijkstra is implemented with a special `Edge` struct for weighted nodes 

Both BFS and DFS are implemented **iteratively** (no recursion).


#### Hashing Algorithms
- Linear Probing (with built-in linear search for demo)
- Separate Chaining
- Double Hashing
- Quadratic Probing

Linear Probing uses modulo arithmetic to wrap-around the hash table/array when last index is full, optimizing resources and using the full array. 

Separate Chaining uses sll API from the 'data_structures' folder

Double Hashing uses a second hash function to calculate probe steps, reducing clustering compared to linear probing.

Quadratic Probing resolves collisions by using quadratic increments (i²) to reduce clustering compared to linear probing.

---

## Time Complexity

### Searching Algorithms

* Linear Search: **O(n)**
* Binary Search: **O(logn)**

### Sorting Algorithms

* Bubble Sort: **O(n²)**
* Selection Sort: **O(n²)**
* Insertion Sort: **O(n²)**

### Advanced Sorting Algorithms

* Quick sort: **O(n²)**
* Merge sort: **O(nlogn)**
* Heap sort: **O(nlogn)**
* Radix sort (LSD): **O(nk)**

### Graph Traversals (Adjacency List)

* BFS: **O(V+E)**
* DFS: **O(V+E)**
* Dijkstra's Algorithm: **O((V+E)log V)**
* A* Search: **O((V+E)log V)**
* Greedy Best-First Search: **O((V+E)log V)**
* Bellman-Ford: **O(V·E)**
* Topological Sort (Kahn's Algorithm): **O(V+E)**

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
  
---

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
