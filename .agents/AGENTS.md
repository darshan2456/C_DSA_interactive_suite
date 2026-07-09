# Project Conventions — C_DSA_interactive_suite

## File Organization
- **One algorithm per `.c` file.** Never cram multiple algorithm implementations into a single source file. Each algorithm (e.g., FIFO, LRU, LFU) must have its own dedicated `.c` file, just like `src/job_scheduling/` has `fcfs.c`, `sjf.c`, `round_robin.c`, etc.
- Core helpers (init, print, visualize) stay in the module's main `.c` file (e.g., `cache.c`, `job_scheduling_demo.c`).

## Demo / Menu Code Style
- **Use `switch-case`, not `else if` ladders**, for algorithm selection in all `_demo.c` files. Every existing demo in this project uses `switch-case` — always follow this convention.
- For mapping choice numbers to display names, prefer an array lookup (e.g., `algo_names[choice]`) over nested ternary operators.

## General
- Always run `make fmt` before committing.
- Always verify with `make` (full build) and the relevant `make test_*` target before pushing.
