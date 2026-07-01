# Contributing to C_DSA_interactive_suite

Thank you for taking the time to contribute! This project is open to learners, systems enthusiasts, and low-level programmers who want to build something meaningful in C. You dont need deep technical knowledge, as this project is made with raw C, you will only need algorithmic thinking and logical thinking. Thats it.

---

## Table of Contents

- [Getting Started](#getting-started)
- [How to Contribute](#how-to-contribute)
- [Commit and Branch Naming Convention](#commit-and-branch-naming-convention)
- [Coding Standards](#coding-standards)
- [Pull Request Process](#pull-request-process)
- [Testing](#testing)
- [Reporting Issues](#reporting-issues)

---


## Getting Started

### Note for SSOC contributors

This project is Linux native. Which means it is made, maintained on Linux and works the best on Linux, however, you can definitely develop and run/use this application on windows through WSL(Windows Subsystem for Linux). WSL will work in VScode too so you can rest assured, just be familiar with WSL before contributing to this project. You can also use a VM, but WSL is sufficient as per the project requirements.

Also note that you must run `make fmt` before submitting your PR otherwise CI will definitely fail

I have created a special discord server to resolve your issues and doubts regarding the project and contribution. There we can discuss everything about the project as well as your technical/conceptual doubts. 
Here is the link - https://discord.gg/RxepKBWBX

### AI usage policy

I am not against usage of AI, you can use AI, but you do the thinking part, let AI do the typing part, thats it. You dont let AI think for you, contruct a PR for you. If you cannot answer questions about your code and its utility, your PR wont be merged. As simple as that

### Prerequisites

- GCC (or compatible C compiler)
- GNU Make ≥ 4.4.1
- Valgrind (for checking memory leaks)
- Git

### Build and Run

```bash
make run
```

### Testing

```bash
make test
```

---

## How to Contribute

You can contribute in the following ways:

- Fix an open issue or open an issue
- Implement a new data structure or algorithm
- Improve existing implementations (refactoring)
- Add or improve test cases
- Improve documentation

Please check the [Issues](https://github.com/darshan2456/C_DSA_interactive_suite/issues) tab before starting work to avoid duplicates. If you plan to work on something, comment on the issue to get it assigned.

---

## Commit and Branch Naming Convention

Use clear, concise commit messages in the following form:

```
feat: Add AVL tree insertion with rotation logic
fix: Fix memory leak in DLL delete_node
refactor: Refactor hash table to use safe_input_int API
docs: Fix typo in README.md
```

Avoid vague messages like `fix stuff` or `update code`.

```
feature/<short-description>      # for new features or modules
fix/<short-description>          # for bug fixes
docs/<short-description>         # for documentation changes
test/<short-description>         # for adding/improving tests
```
**Branch naming conventions**

same as commit conventions, whatever you are doing, followed by slash followed by brief one line description 

Example: `feature/avl-tree`, `fix/bst-delete-edge-case`

---

## Coding Standards

This project is written in **C11**. All contributions must follow the existing style:

- No function definitions inside header files
- Prefix all public functions with the module name (e.g., `bst_insert`, `sll_delete`)
- Use `static` for all file-local helper functions which are not supposed to used elsewhere but where they are created
- Use `const` where pointer safety is appropriate ie functions which do not modify data but only read data, like sll_printlist(), sll_search() functions
- All user input must go through input validation functions in `src/utils` — no raw `scanf`,`fgets`
- Format your code using `.clang-format` before submitting — run `make fmt`. Note that formatting checks are enforced at the CI level. Not running `make fmt` will cause the CI to fail
- When adding a new module, respect the existing code conventions and ask straight away if you dont understand anything
- Compile cleanly under `-Wall -Wextra -Werror -std=c11` so even warning are treated as errors. It helps to avoid accumulation of dead code.

---


## Pull Request Process

1. Fork the repository and create your branch from `main`
2. Make your changes following the **coding standards** above
3. Ensure the CI pass and Valgrind reports with no errors
6. Link the relevant issue using `Closes #<issue-number>`

PRs without tests(in case of new ds/algo), failing CI, or unformatted code will not be merged.

---


## Testing

<!-- Describe your test structure, how to run tests, and what is expected -->

All contributions which introduces a new module (ds/algo) must include tests:

```bash
# Run all tests
make test

# Run with Valgrind
make valgrind
```

The CI pipeline runs all tests and Valgrind checks automatically on every push and pull request. A PR will not be merged if CI fails.

---

## Reporting Issues

When opening an issue, please include:

- A clear title and description
- Steps to reproduce (if it's a bug)
- Expected vs actual behavior
- Your OS and GCC version

Use the appropriate label: `bug`, `enhancement`, `documentation`, `good first issue`, `refactor` etc.
