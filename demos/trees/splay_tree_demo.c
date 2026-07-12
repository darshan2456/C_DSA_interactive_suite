#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

void splay_tree_demo(void)
{
    splayNode* root = NULL;
    printf("\n--- Splay Tree (Amortized O(log n)) Demo ---\n");
    printf("Action: Inserting 10, 20, 30, 40, 50, 25...\n");
    root = splay_tree_insert(root, 10);
    root = splay_tree_insert(root, 20);
    root = splay_tree_insert(root, 30);
    root = splay_tree_insert(root, 40);
    root = splay_tree_insert(root, 50);
    root = splay_tree_insert(root, 25);

    printf("Tree Preorder Traversal (Root is %d): ", root->key);
    splay_tree_preorder(root);

    printf("\n\nAction: Searching for 20 (This will splay '20' to the root!)\n");
    root = splay_tree_search(root, 20);
    printf("Tree Preorder Traversal (New Root is %d): ", root->key);
    splay_tree_preorder(root);

    printf("\n\nAction: Deleting 30\n");
    root = splay_tree_delete(root, 30);
    printf("Tree Preorder Traversal (Root after deletion is %d): ", root->key);
    splay_tree_preorder(root);
    printf("\n--------------------------------------------\n\n");

    destroy_splay_tree(root);
}