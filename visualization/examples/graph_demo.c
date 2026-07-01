#include "../include/graph_visualizer.h"

int main() {
    VisualizerConfig config = {
        .animation_delay_ms = 300,
        .use_colors = true,
        .show_step_numbers = true,
        .highlight_path = true
    };
    visualizer_init(&config);
    
    // Create a graph with 6 vertices
    GraphVisualizer *gv = graph_visualizer_create(6);
    
    // Define adjacency matrix
    int matrix[6][6] = {
        {0,1,1,0,0,0},
        {1,0,0,1,1,0},
        {1,0,0,0,0,1},
        {0,1,0,0,0,0},
        {0,1,0,0,0,1},
        {0,0,1,0,1,0}
    };
    
    // Convert to pointer array
    int *matrix_ptr[6];
    for (int i = 0; i < 6; i++) {
        matrix_ptr[i] = matrix[i];
    }
    
    graph_visualizer_set_adj_matrix(gv, (int**)matrix_ptr, 6);
    graph_visualizer_set_speed(gv, 500);
    
    // Run BFS
    graph_visualizer_animate_bfs(gv, 0);
    getchar();
    
    // Run DFS
    graph_visualizer_animate_dfs(gv, 0);
    getchar();
    
    // Run Dijkstra
    graph_visualizer_animate_dijkstra(gv, 0);
    getchar();
    
    graph_visualizer_free(gv);
    visualizer_cleanup();
    
    return 0;
}