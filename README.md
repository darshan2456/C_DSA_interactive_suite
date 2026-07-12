

# C_DSA_interactive_suite

A modular, console-based **Data Structures & Algorithms library** written entirely in **C**, built from scratch with pointer-level control, manual memory management (`malloc` / `free`), and defensive input validation.

This project emphasizes **conceptual clarity**, **low-level fundamentals**, and **explicit memory reasoning**. It is designed with an educational intent, allowing learners to observe, experiment with, and understand data structures and algorithms step-by-step through an interactive terminal-based interface.

The codebase is structured as a reusable **DSA library**, with an interactive, console-driven **demo layer** built on top.

---

## Table of Contents
- [Demos](#demos)
- [Build Instructions](#build-instructions)
- [Architectural Breakdown: Docker & The Build System](#architectural-breakdown-docker--the-build-system)
- [Continuous Integration](#continuous-integration)
- [License](#license)

---

## Demos

## Backtracking algos (Knight's tour)

[![asciinema demo](https://asciinema.org/a/G5D8YBvcIbZM2fzF.svg)](https://asciinema.org/a/G5D8YBvcIbZM2fzF)

## Sorting algos (Bubble sort)

[![asciinema demo](https://asciinema.org/a/J8CHAuyJAIyOduYY.svg)](https://asciinema.org/a/J8CHAuyJAIyOduYY)

## Expression Evaluation complete

[![asciinema demo](https://asciinema.org/a/xDm8zlxD6VzzFZFR.svg)](https://asciinema.org/a/xDm8zlxD6VzzFZFR)

## String algos (Robin Karp)

[![asciinema demo](https://asciinema.org/a/d2BU9AVDS94GVovS.svg)](https://asciinema.org/a/d2BU9AVDS94GVovS)


---

## Build Instructions

This project includes a **Makefile** and **CMakeLists.txt** to simplify building across multiple directories.

### Requirements
- GNU Make ≥ 4.4.1
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
Runs all tests and generates test binaries

```bash
make fmt
```
Organizes code style according to the standards defined in `.clang-format`

```bash
make valgrind
```
Runs Valgrind over test binaries to look for memory leaks / use after free errors


### Clean

```bash
make clean
```
Removes executables and generated object/test binaries.

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

### Docker Makefile Helper Commands

Helper targets have been added to the local `Makefile` to simplify building, running, and testing inside Docker:

| Command | Description |
| --- | --- |
| `make docker-test` | Builds the `dev` stage and runs the complete unit test suite inside it. |
| `make docker-run` | Builds the `runtime` stage and launches the interactive application shell. |
| `make docker-build-dev` | Builds the development stage image (`c-dsa-suite:dev`). |
| `make docker-build-runtime` | Builds the slim production stage image (`c-dsa-suite:slim`). |

---

### Multi-Stage Architecture

The project uses a multi-stage `Dockerfile` to separate the build environment from the lightweight runtime image:
- **Stage 1 (`dev`)**: A heavy development environment containing the full C build toolchain, `valgrind`, `gdb`, and the complete source repository.
- **Stage 2 (`runtime`)**: A minimal image packaging only the compiled binary and `libncurses6` runtime library (no source code or compiler).

---

## Continuous Integration

[![CI](https://github.com/darshan2456/C_DSA_interactive_suite/actions/workflows/ci.yml/badge.svg)](https://github.com/darshan2456/C_DSA_interactive_suite/actions/workflows/ci.yml)

This project includes a **GitHub Actions CI pipeline** that automatically verifies code correctness and memory safety.

On every push or pull request:

1. A fresh **Ubuntu VM** is allocated
2. The project is **compiled using GCC**
3. The `make fmt` is run on the runner and checked with your code, if they dont match, CI turns red
4. The complete **unit test suite is executed**
5. All test binaries are run under **Valgrind** to check for:
6. The project is sanitized under asan and ubsan and tests are run to check for undefined behaviour (ie semantic errors)

   - unformatted code
   - memory leaks  
   - invalid reads / writes  
   - use-after-free errors  
   - uninitialized memory usage

If any test fails or Valgrind detects a memory error, the CI job fails automatically.

---

## License
This project is licensed under the MIT License - see the LICENSE file for details.

---

## Author

**Darshan Parekh** and many contributors....

Aspiring systems engineer and cybersecurity engineer
