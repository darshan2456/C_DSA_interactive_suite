# Contributing to C_DSA_interactive_suite

Thank you for taking the time to contribute! This project is open to learners, systems enthusiasts, and low-level programmers who want to build something meaningful in C.

---

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [How to Contribute](#how-to-contribute)
- [Branch Naming Convention](#branch-naming-convention)
- [Commit Message Guidelines](#commit-message-guidelines)
- [Coding Standards](#coding-standards)
- [Adding a New Module](#adding-a-new-module)
- [Testing](#testing)
- [Pull Request Process](#pull-request-process)
- [Reporting Issues](#reporting-issues)

---

## Code of Conduct

<!-- Add your code of conduct or link to a CODE_OF_CONDUCT.md -->

---

## Getting Started

### Prerequisites

- GCC (or compatible C compiler)
- GNU Make ≥ 3.81
- Valgrind (for memory testing)
- Git

### Fork & Clone

```bash
git clone https://github.com/<your-username>/C_DSA_interactive_suite.git
cd C_DSA_interactive_suite
```

### Build

```bash
make
./dsa
```

### Clean

```bash
make clean
```

---

## How to Contribute

You can contribute in the following ways:

- Fix an open issue
- Implement a new data structure or algorithm
- Improve existing implementations
- Add or improve test cases
- Improve documentation

Please check the [Issues](https://github.com/darshan2456/C_DSA_interactive_suite/issues) tab before starting work to avoid duplicates. If you plan to work on something, comment on the issue to get it assigned.

---

## Branch Naming Convention

```
feature/<short-description>      # for new features or modules
fix/<short-description>          # for bug fixes
docs/<short-description>         # for documentation changes
test/<short-description>         # for adding/improving tests
```

Example: `feature/avl-tree`, `fix/bst-delete-edge-case`

---

## Commit Message Guidelines

Use clear, concise commit messages in the imperative form:

```
Add AVL tree insertion with rotation logic
Fix memory leak in DLL delete_node
Refactor hash table to use safe_input API
```

Avoid vague messages like `fix stuff` or `update code`.

---

## Coding Standards

This project is written in **C11**. All contributions must follow the existing style:

- Format your code using `.clang-format` before submitting — run `clang-format -i <file>`
- One `.h` / `.c` pair per logical module — no exceptions
- No function definitions inside header files
- Prefix all public functions with the module name (e.g., `bst_insert`, `sll_delete`)
- Use `static` for all file-local helper functions
- Use `const` where pointer safety is appropriate
- All user input must go through `safe_input_int()` — no raw `scanf`
- Compile cleanly under `-Wall -Wextra -std=c11` with zero warnings

---

## Adding a New Module

<!-- Describe your preferred directory structure, how to hook into the main menu, and any naming conventions specific to this project -->

Example structure for a new module:

```
src/
└── your_module/
    ├── your_module.h
    ├── your_module.c
    └── your_module_demo.c
```

Steps:
1. Create the module directory under `src/`
2. Implement the core logic in `your_module.c`
3. Expose the public API via `your_module.h`
4. Write the interactive demo in `your_module_demo.c`
5. Hook it into the main menu
6. Update the `Makefile` to include the new module
7. Add corresponding tests under `tests/`

---

## Testing

<!-- Describe your test structure, how to run tests, and what is expected -->

All contributions must include tests:

```bash
# Run all tests
make test

# Run with Valgrind
valgrind --leak-check=full ./test_binary
```

The CI pipeline runs all tests and Valgrind checks automatically on every push and pull request. A PR will not be merged if CI fails.

---

## Pull Request Process

1. Fork the repository and create your branch from `main`
2. Make your changes following the coding standards above
3. Ensure all tests pass and Valgrind reports no errors
4. Format your code with `clang-format`
5. Open a pull request with a clear title and description
6. Link the relevant issue using `Closes #<issue-number>`
7. Wait for review — address any feedback promptly

PRs without tests, failing CI, or unformatted code will not be merged.

---

## Reporting Issues

When opening an issue, please include:

- A clear title and description
- Steps to reproduce (if it's a bug)
- Expected vs actual behavior
- Your OS and GCC version

Use the appropriate label: `bug`, `enhancement`, `documentation`, `good first issue`, etc.

---

## Questions?

<!-- Add contact info, discussion board, or Discord link if any -->