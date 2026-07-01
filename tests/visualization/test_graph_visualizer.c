#include "../../visualization/include/graph_visualizer.h"
#include <assert.h>

void test_bfs_visualization() {
    printf("Testing BFS visualization...\n");
    GraphVisualizer *gv = graph_visualizer_create(5);
    
    int matrix[5][5] = {
        {0,1,1,0,0},
        {1,0,0,1,1},
        {1,0,0,0,0},
        {0,1,0,0,1},
        {0,1,0,1,0}
    };
    
    int *matrix_ptr[5];
    for (int i = 0; i < 5; i++) {
        matrix_ptr[i] = matrix[i];
    }
    
    graph_visualizer_set_adj_matrix(gv, (int**)matrix_ptr, 5);
    graph_visualizer_set_speed(gv, 50); // Fast for testing
    
    graph_visualizer_animate_bfs(gv, 0);
    graph_visualizer_free(gv);
    printf("✅ BFS test passed!\n");
}

void test_dfs_visualization() {
    printf("Testing DFS visualization...\n");
    GraphVisualizer *gv = graph_visualizer_create(5);
    
    int matrix[5][5] = {
        {0,1,1,0,0},
        {1,0,0,1,1},
        {1,0,0,0,0},
        {0,1,0,0,1},
        {0,1,0,1,0}
    };
    
    int *matrix_ptr[5];
    for (int i = 0; i < 5; i++) {
        matrix_ptr[i] = matrix[i];
    }
    
    graph_visualizer_set_adj_matrix(gv, (int**)matrix_ptr, 5);
    graph_visualizer_set_speed(gv, 50);
    
    graph_visualizer_animate_dfs(gv, 0);
    graph_visualizer_free(gv);
    printf("✅ DFS test passed!\n");
}

int main() {
    VisualizerConfig config = {
        .animation_delay_ms = 100,
        .use_colors = true,
        .show_step_numbers = true
    };
    visualizer_init(&config);
    
    test_bfs_visualization();
    test_dfs_visualization();
    
    visualizer_cleanup();
    printf("\n🎉 All graph visualization tests passed!\n");
    return 0;
}