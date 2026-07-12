#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

// Core Initialization
rbTree* create_rb_tree(void)
{
    rbTree* tree = (rbTree*)malloc(sizeof(rbTree));
    if (tree == NULL)
    {
        return NULL;
    }
    tree->TNULL = (rbNode*)malloc(sizeof(rbNode));
    if (tree->TNULL == NULL)
    {
        free(tree);
        return NULL;
    }
    tree->TNULL->color = BLACK;
    tree->TNULL->left = NULL;
    tree->TNULL->right = NULL;
    tree->TNULL->parent = NULL;
    tree->root = tree->TNULL;
    return tree;
}

// Rotations
static void rb_left_rotate(rbTree* tree, rbNode* x)
{
    rbNode* y = x->right;
    x->right = y->left;
    if (y->left != tree->TNULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

static void rb_right_rotate(rbTree* tree, rbNode* x)
{
    rbNode* y = x->left;
    x->left = y->right;
    if (y->right != tree->TNULL)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        tree->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// Insert Fixup
static void rb_insert_fixup(rbTree* tree, rbNode* k)
{
    rbNode* u;
    while (k->parent != NULL && k->parent->color == RED)
    {
        if (k->parent == k->parent->parent->right)
        {
            u = k->parent->parent->left; // uncle
            if (u->color == RED)
            {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            }
            else
            {
                if (k == k->parent->left)
                {
                    k = k->parent;
                    rb_right_rotate(tree, k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rb_left_rotate(tree, k->parent->parent);
            }
        }
        else
        {
            u = k->parent->parent->right; // uncle
            if (u->color == RED)
            {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            }
            else
            {
                if (k == k->parent->right)
                {
                    k = k->parent;
                    rb_left_rotate(tree, k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rb_right_rotate(tree, k->parent->parent);
            }
        }
        if (k == tree->root)
            break;
    }
    tree->root->color = BLACK;
}

void rb_insert(rbTree* tree, int key)
{
    if (tree == NULL)
        return;
    rbNode* node = (rbNode*)malloc(sizeof(rbNode));
    if (node == NULL)
    {
        return;
    }
    node->data = key;
    node->parent = NULL;
    node->left = tree->TNULL;
    node->right = tree->TNULL;
    node->color = RED;

    rbNode* y = NULL;
    rbNode* x = tree->root;

    while (x != tree->TNULL)
    {
        y = x;
        if (node->data < x->data)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;
    if (y == NULL)
        tree->root = node;
    else if (node->data < y->data)
        y->left = node;
    else
        y->right = node;

    if (node->parent == NULL)
    {
        node->color = BLACK;
        return;
    }
    if (node->parent->parent == NULL)
        return;

    rb_insert_fixup(tree, node);
}

// Validator (Proves all properties)
static int check_black_height(rbTree* tree, rbNode* node, bool* is_valid)
{
    if (node == tree->TNULL)
        return 1;

    if (node->color == RED)
    {
        if (node->left->color == RED || node->right->color == RED)
        {
            *is_valid = false; // Double Red Violation
        }
    }

    int left_bh = check_black_height(tree, node->left, is_valid);
    int right_bh = check_black_height(tree, node->right, is_valid);

    if (left_bh != right_bh)
        *is_valid = false; // Black Height Violation

    return left_bh + (node->color == BLACK ? 1 : 0);
}

bool is_rb_tree_valid(rbTree* tree)
{
    if (tree->root->color != BLACK)
        return false;
    bool is_valid = true;
    check_black_height(tree, tree->root, &is_valid);
    return is_valid;
}

// Memory Cleanup
static void free_rb_nodes(rbTree* tree, rbNode* node)
{
    if (node != tree->TNULL)
    {
        free_rb_nodes(tree, node->left);
        free_rb_nodes(tree, node->right);
        free(node);
    }
}

void destroy_rb_tree(rbTree* tree)
{
    if (tree)
    {
        free_rb_nodes(tree, tree->root);
        free(tree->TNULL);
        free(tree);
    }
}
