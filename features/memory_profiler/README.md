# Built-in Memory Profiler

A lightweight, real-time memory profiling tool designed to track, visualize, and debug memory usage of DSA operations.

## Features
- **Allocation Tracking**: Intercepts `malloc`, `calloc`, `realloc`, and `free` with file, line, and size metadata.
- **Leak Detection**: Scans allocation registry on program termination and outputs a summary of un-freed blocks.
- **Call Site Backtraces**: Captures POSIX call stacks for each active memory block to isolate leaks.
- **Heap Diagnostics**: Calculates live peak usage, allocation counts, and heap fragmentation estimates.
- **Visual Terminal Cards & ASCII Charts**: Renders formatted performance cards and ASCII trend graphs.
- **Algorithm Comparisons**: Side-by-side memory footprint comparison helper.

## API Usage
Include `<memory_tracker.h>` in translation units to trace allocations.
Run the application with the `--profile` command line flag to enable profiling.
