#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

void red_black_tree_demo(void)
{
    printf("\n--- Red-Black Tree (Self-Balancing BST) Demo ---\n");
    rbTree* tree = create_rb_tree();
    if (tree == NULL)
    {
        printf("Error: Failed to create Red-Black Tree due to memory allocation failure.\n");
        return;
    }

    printf("Action: Inserting 10, 20, 30, 15, 25...\n");
    rb_insert(tree, 10);
    rb_insert(tree, 20);
    rb_insert(tree, 30);
    rb_insert(tree, 15);
    rb_insert(tree, 25);

    bool valid = is_rb_tree_valid(tree);
    printf("Invariant Check: Are all 5 RB-Tree properties holding? %s\n",
           valid ? "YES (Valid)" : "NO (Invalid)");
    printf("Root of the tree is: %d (Color: %s)\n", tree->root->data,
           tree->root->color == BLACK ? "BLACK" : "RED");

    destroy_rb_tree(tree);
    printf("------------------------------------------------\n\n");
}