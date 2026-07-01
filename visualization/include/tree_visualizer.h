#ifndef TREE_VISUALIZER_H
#define TREE_VISUALIZER_H

#include "visualizer_common.h"

typedef struct VisTreeNode {
    int data;
    struct VisTreeNode *left;
    struct VisTreeNode *right;
    int screen_x;
    int screen_y;
    bool highlighted;
    char highlight_color[20];
} VisTreeNode;

typedef struct {
    VisTreeNode *root;
    int node_spacing_x;
    int node_spacing_y;
    bool show_balance_factors;
    int node_count;
    int animation_delay_ms;
} TreeVisualizer;

// Creation/Destruction
TreeVisualizer* tree_visualizer_create(void);
void tree_visualizer_free(TreeVisualizer *tv);
VisTreeNode* tree_visualizer_create_node(int data);

// Rendering
void tree_visualizer_draw(TreeVisualizer *tv);
void tree_visualizer_draw_vertical(TreeVisualizer *tv);

// Node operations
void tree_visualizer_insert(TreeVisualizer *tv, int data);
VisTreeNode* tree_visualizer_search(TreeVisualizer *tv, int data);

// Traversal animations
void tree_visualizer_animate_inorder(TreeVisualizer *tv);
void tree_visualizer_animate_preorder(TreeVisualizer *tv);
void tree_visualizer_animate_postorder(TreeVisualizer *tv);
void tree_visualizer_animate_level_order(TreeVisualizer *tv);

// Highlighting
void tree_visualizer_highlight_node(VisTreeNode *node, const char *color);
void tree_visualizer_clear_highlights(TreeVisualizer *tv);

#endif