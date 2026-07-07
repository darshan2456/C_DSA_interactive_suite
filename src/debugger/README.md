# Interactive Step Debugger & Event Logger

A built-in module for stepping through algorithms dynamically, allowing line-by-line inspection of sorting and graph traversal processes.

## Features
- **Interactive Control**: Toggle debugger step mode ON/OFF inside the global settings menu.
- **Playback Signals**: Step-through comparisons/swaps in sorting, or node exploration in graph search operations.
- **Event Log History**: Renders a live, scrolling ASCII card detailing the last 5 operations executed.
- **ncurses & Console Compatibility**: Seamlessly hooks into standard TUI sessions or terminal inputs.

## Operations
- Enable Step Mode in Settings (Menu Choice 15).
- Run Bubble Sort or BFS.
- Press `[Space]/Enter` to execute one step, `[r]` to resume normal speed, or `[q]` to disable step mode.
