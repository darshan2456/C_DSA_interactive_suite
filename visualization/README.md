# Visualization Module

Real-time visualization for graph and tree algorithms using ANSI escape codes.

## Features

- **Graph Algorithms**: BFS, DFS, Dijkstra's algorithm with step-by-step animation
- **Tree Traversals**: Inorder, Preorder, Postorder, Level Order
- **Color Coding**: Different colors for different states
- **Speed Control**: Adjustable animation speed
- **Interactive**: Visual feedback for each operation

## Usage

### Graph Visualization

```c
GraphVisualizer *gv = graph_visualizer_create(6);
graph_visualizer_set_adj_matrix(gv, matrix, 6);
graph_visualizer_animate_bfs(gv, 0);
graph_visualizer_free(gv);

### Tree Visualization

TreeVisualizer *tv = tree_visualizer_create();
tree_visualizer_insert(tv, 50);
tree_visualizer_animate_inorder(tv);
tree_visualizer_free(tv);

## Building

make vis-graph    # Build graph demo
make vis-tree     # Build tree demo
make test-visualization  # Run tests

## Integration

The visualization module integrates with the main TUI through the "Visualization" menu option (7).

## Technical Details
1. Uses ANSI escape sequences for colors and formatting

2. Unicode box-drawing characters for borders

3. No external dependencies

4. Works on all Unix/Linux terminals


---

