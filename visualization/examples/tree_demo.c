#include "../include/tree_visualizer.h"

int main() {
    VisualizerConfig config = {
        .animation_delay_ms = 300,
        .use_colors = true,
        .show_step_numbers = true
    };
    visualizer_init(&config);
    
    TreeVisualizer *tv = tree_visualizer_create();
    tv->animation_delay_ms = 400;
    
    // Insert nodes
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85};
    for (int i = 0; i < 15; i++) {
        tree_visualizer_insert(tv, values[i]);
    }
    
    tree_visualizer_draw(tv);
    getchar();
    
    // Show traversals
    tree_visualizer_animate_inorder(tv);
    getchar();
    
    tree_visualizer_animate_preorder(tv);
    getchar();
    
    tree_visualizer_animate_postorder(tv);
    getchar();
    
    tree_visualizer_animate_level_order(tv);
    getchar();
    
    // Search for a node
    printf("\nSearching for 40...\n");
    tree_visualizer_search(tv, 40);
    getchar();
    
    tree_visualizer_free(tv);
    visualizer_cleanup();
    
    return 0;
}