#include "../include/tree_visualizer.h"

TreeVisualizer* tree_visualizer_create(void) {
    TreeVisualizer *tv = calloc(1, sizeof(TreeVisualizer));
    if (!tv) return NULL;
    
    tv->node_spacing_x = 4;
    tv->node_spacing_y = 3;
    tv->show_balance_factors = true;
    tv->node_count = 0;
    tv->animation_delay_ms = 300;
    
    return tv;
}

void tree_visualizer_free(TreeVisualizer *tv) {
    if (!tv) return;
    // Recursive free would be needed, simplified for demo
    free(tv);
}

VisTreeNode* tree_visualizer_create_node(int data) {
    VisTreeNode *node = calloc(1, sizeof(VisTreeNode));
    if (!node) return NULL;
    node->data = data;
    node->highlighted = false;
    strcpy(node->highlight_color, COLOR_WHITE);
    return node;
}

void tree_visualizer_insert(TreeVisualizer *tv, int data) {
    if (!tv) return;
    
    VisTreeNode *new_node = tree_visualizer_create_node(data);
    if (!new_node) return;
    
    if (!tv->root) {
        tv->root = new_node;
        tv->node_count++;
        return;
    }
    
    VisTreeNode *current = tv->root;
    VisTreeNode *parent = NULL;
    
    while (current) {
        parent = current;
        if (data < current->data) {
            current = current->left;
        } else if (data > current->data) {
            current = current->right;
        } else {
            free(new_node);
            return; // Duplicate
        }
    }
    
    if (data < parent->data) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
    tv->node_count++;
}

VisTreeNode* tree_visualizer_search(TreeVisualizer *tv, int data) {
    if (!tv || !tv->root) return NULL;
    
    VisTreeNode *current = tv->root;
    while (current) {
        tree_visualizer_highlight_node(current, BRIGHT_YELLOW);
        tree_visualizer_draw(tv);
        visualizer_delay(tv->animation_delay_ms);
        
        if (data == current->data) {
            tree_visualizer_highlight_node(current, BRIGHT_GREEN);
            tree_visualizer_draw(tv);
            return current;
        } else if (data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    printf(BRIGHT_RED "✗ Element %d not found!\n" ANSI_RESET, data);
    return NULL;
}

void tree_visualizer_draw(TreeVisualizer *tv) {
    if (!tv) return;
    
    visualizer_clear();
    visualizer_draw_header("Tree Visualization");
    
    if (!tv->root) {
        printf("  Tree is empty\n");
        return;
    }
    
    tree_visualizer_draw_vertical(tv);
    printf("\n  Total nodes: %d\n", tv->node_count);
}

void tree_visualizer_draw_vertical(TreeVisualizer *tv) {
    if (!tv || !tv->root) return;
    
    // Simple recursive display with indentation
    printf("\n");
    tree_visualizer_draw_node(tv->root, 0);
}

void tree_visualizer_draw_node(VisTreeNode *node, int depth) {
    if (!node) return;
    
    // Right child (with indentation)
    tree_visualizer_draw_node(node->right, depth + 1);
    
    // Current node
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    
    const char *color = node->highlighted ? node->highlight_color : COLOR_WHITE;
    printf("%s[%d]%s\n", color, node->data, ANSI_RESET);
    
    // Left child
    tree_visualizer_draw_node(node->left, depth + 1);
}

void tree_visualizer_animate_inorder(TreeVisualizer *tv) {
    if (!tv || !tv->root) return;
    
    printf("\n" BRIGHT_CYAN "=== Inorder Traversal ===\n" ANSI_RESET);
    tree_visualizer_inorder_rec(tv->root, tv);
    printf("\n" BRIGHT_GREEN "✅ Inorder traversal complete!\n" ANSI_RESET);
}

void tree_visualizer_inorder_rec(VisTreeNode *node, TreeVisualizer *tv) {
    if (!node) return;
    
    tree_visualizer_inorder_rec(node->left, tv);
    
    tree_visualizer_highlight_node(node, BRIGHT_RED);
    tree_visualizer_draw(tv);
    printf(BRIGHT_YELLOW "Visiting: %d\n" ANSI_RESET, node->data);
    visualizer_delay(tv->animation_delay_ms);
    tree_visualizer_clear_highlights(tv);
    
    tree_visualizer_inorder_rec(node->right, tv);
}

void tree_visualizer_animate_preorder(TreeVisualizer *tv) {
    if (!tv || !tv->root) return;
    
    printf("\n" BRIGHT_CYAN "=== Preorder Traversal ===\n" ANSI_RESET);
    tree_visualizer_preorder_rec(tv->root, tv);
    printf("\n" BRIGHT_GREEN "✅ Preorder traversal complete!\n" ANSI_RESET);
}

void tree_visualizer_preorder_rec(VisTreeNode *node, TreeVisualizer *tv) {
    if (!node) return;
    
    tree_visualizer_highlight_node(node, BRIGHT_BLUE);
    tree_visualizer_draw(tv);
    printf(BRIGHT_YELLOW "Visiting: %d\n" ANSI_RESET, node->data);
    visualizer_delay(tv->animation_delay_ms);
    tree_visualizer_clear_highlights(tv);
    
    tree_visualizer_preorder_rec(node->left, tv);
    tree_visualizer_preorder_rec(node->right, tv);
}

void tree_visualizer_animate_postorder(TreeVisualizer *tv) {
    if (!tv || !tv->root) return;
    
    printf("\n" BRIGHT_CYAN "=== Postorder Traversal ===\n" ANSI_RESET);
    tree_visualizer_postorder_rec(tv->root, tv);
    printf("\n" BRIGHT_GREEN "✅ Postorder traversal complete!\n" ANSI_RESET);
}

void tree_visualizer_postorder_rec(VisTreeNode *node, TreeVisualizer *tv) {
    if (!node) return;
    
    tree_visualizer_postorder_rec(node->left, tv);
    tree_visualizer_postorder_rec(node->right, tv);
    
    tree_visualizer_highlight_node(node, BRIGHT_MAGENTA);
    tree_visualizer_draw(tv);
    printf(BRIGHT_YELLOW "Visiting: %d\n" ANSI_RESET, node->data);
    visualizer_delay(tv->animation_delay_ms);
    tree_visualizer_clear_highlights(tv);
}

void tree_visualizer_animate_level_order(TreeVisualizer *tv) {
    if (!tv || !tv->root) return;
    
    printf("\n" BRIGHT_CYAN "=== Level Order Traversal ===\n" ANSI_RESET);
    
    VisTreeNode *queue[100];
    int front = 0, rear = 0;
    queue[rear++] = tv->root;
    
    while (front < rear) {
        VisTreeNode *current = queue[front++];
        
        tree_visualizer_highlight_node(current, BRIGHT_CYAN);
        tree_visualizer_draw(tv);
        printf(BRIGHT_YELLOW "Visiting: %d\n" ANSI_RESET, current->data);
        visualizer_delay(tv->animation_delay_ms);
        tree_visualizer_clear_highlights(tv);
        
        if (current->left) queue[rear++] = current->left;
        if (current->right) queue[rear++] = current->right;
    }
    
    printf("\n" BRIGHT_GREEN "✅ Level order traversal complete!\n" ANSI_RESET);
}

void tree_visualizer_highlight_node(VisTreeNode *node, const char *color) {
    if (!node) return;
    node->highlighted = true;
    strcpy(node->highlight_color, color);
}

void tree_visualizer_clear_highlights(TreeVisualizer *tv) {
    if (!tv) return;
    tree_visualizer_clear_highlights_rec(tv->root);
}

void tree_visualizer_clear_highlights_rec(VisTreeNode *node) {
    if (!node) return;
    node->highlighted = false;
    strcpy(node->highlight_color, COLOR_WHITE);
    tree_visualizer_clear_highlights_rec(node->left);
    tree_visualizer_clear_highlights_rec(node->right);
}