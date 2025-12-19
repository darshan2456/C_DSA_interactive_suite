🧠📦 DSA_using_C

A Data Structures and Algorithms repository implemented in pure C, focused on pointer-level reasoning, manual memory management, and correctness invariants.
Built from scratch to develop systems-grade C fundamentals suitable for open-source contributions, GSoC, and FAANG-level engineering.

✅ What Is Implemented (Concrete Work)

All implementations are written from scratch in C, using dynamic memory, explicit ownership, and pointer manipulation.

🔹 Core C Foundations

Variables, control flow, functions

Arrays and strings

Modular code using header (.h) and source (.c) files

Multi-file program structure

🔹 Pointer & Memory Mastery

Pointer semantics and dereferencing

Pointer arithmetic

Pointer-to-pointer usage (e.g. modifying head pointers)

malloc / free

Ownership and lifetime reasoning

Safe handling of NULL

🧱 Data Structures
🔗 Singly Linked List

Full CRUD operations

Insertion at head, tail, and arbitrary positions

Deletion with correct head manipulation

Traversal and display

Pointer-based reversal

Edge case handling (empty list, single node)

🔁 Doubly Linked List

Bidirectional traversal

Insertion and deletion

Correct maintenance of prev / next pointers

Robust handling of boundary cases

📚 Stack (Linked List Based)

Implemented as an abstraction over a linked list.

Operations:

push

pop

peek

isEmpty

destroyStack

Focus areas:

LIFO invariants

Empty-stack safety

Memory cleanup discipline

🔄 Circular Queue (Array Based)

Fixed-size array implementation

Circular behavior using modulo arithmetic

One-empty-slot invariant to distinguish full vs empty

Front and rear pointer management

Wrap-around enqueue/dequeue logic

🌳 Binary Search Tree (BST)

Node insertion

Recursive traversals:

In-order

Pre-order

Post-order

Node counting

Recursive invariant reasoning

🔢 Expression Parsing
➕ Infix to Postfix Conversion

Stack-based parsing

Operator precedence handling

Left associativity

Parentheses handling

Character classification

➗ Postfix Evaluation

Stack-based evaluation
🚧 Planned and currently in progress

⚙️ Build System & Tooling

Compiled using GCC with explicit multi-file compilation and linking.

gcc main.c stack.c linkedlist.c -I../include -o program


Practiced concepts:

Include paths (-I)

Relative paths

Compilation vs linking stages

Linker error debugging

🧪 Testing & Validation

Console-based testing

Manual verification of:

Empty structures

Single-element cases

Overflow / underflow scenarios

Emphasis on reasoning before execution

📌 Abstract & Intent (Why This Repo Exists)

This repository is not about memorizing APIs or using STL-like abstractions.
Every data structure here is built manually, with a strong focus on:

🧠 Reasoning about pointers and memory

🧮 Maintaining invariants

🧼 Writing clean, modular C

🔍 Debugging at the source level

The goal is to move from “I can write C” to “I can safely work inside a large C codebase.”

This repo serves as:

A foundation for open-source C contributions

Preparation for GSoC

Groundwork for systems-oriented FAANG roles

🗂 Repository Structure
.
├── *.c        // Implementations
├── *.h        // Interfaces
├── main.c     // Driver / testing


Principles followed:

Clear separation of interface and implementation

No circular dependencies

Readable, reviewable code

🚀 Future Enhancements

Complete postfix expression evaluation

Add defensive programming and error handling

Introduce debugging workflows (gdb, valgrind)

Refactor modules for production-level clarity

Use this codebase as a launchpad for OSS contributions

👤 Author

Darshan Parekh
B.Sc. Computer Science
Focused on systems programming, open-source contributions and cybersecurity 🚀
