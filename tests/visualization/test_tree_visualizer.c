#include "../../visualization/include/tree_visualizer.h"
#include <assert.h>

void test_tree_insert() {
    printf("Testing tree insert...\n");
    TreeVisualizer *tv = tree_visualizer_create();
    tv->animation_delay_ms = 50;
    
    tree_visualizer_insert(tv, 50);
    tree_visualizer_insert(tv, 30);
    tree_visualizer_insert(tv, 70);
    tree_visualizer_insert(tv, 20);
    tree_visualizer_insert(tv, 40);
    
    assert(tv->root != NULL);
    assert(tv->root->data == 50);
    assert(tv->node_count == 5);
    
    tree_visualizer_free(tv);
    printf("✅ Tree insert test passed!\n");
}

void test_tree_search() {
    printf("Testing tree search...\n");
    TreeVisualizer *tv = tree_visualizer_create();
    tv->animation_delay_ms = 50;
    
    int values[] = {50, 30, 70, 20, 40};
    for (int i = 0; i < 5; i++) {
        tree_visualizer_insert(tv, values[i]);
    }
    
    VisTreeNode *found = tree_visualizer_search(tv, 40);
    assert(found != NULL);
    assert(found->data == 40);
    
    found = tree_visualizer_search(tv, 100);
    assert(found == NULL);
    
    tree_visualizer_free(tv);
    printf("✅ Tree search test passed!\n");
}

int main() {
    VisualizerConfig config = {
        .animation_delay_ms = 100,
        .use_colors = true,
        .show_step_numbers = true
    };
    visualizer_init(&config);
    
    test_tree_insert();
    test_tree_search();
    
    visualizer_cleanup();
    printf("\n🎉 All tree visualization tests passed!\n");
    return 0;
}