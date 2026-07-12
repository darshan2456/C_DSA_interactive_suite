#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

splayNode* splay_create_node(int key)
{
    splayNode* node = (splayNode*)malloc(sizeof(splayNode));
    if (node == NULL)
    {
        return NULL;
    }
    node->key = key;
    node->left = node->right = NULL;
    return node;
}

static splayNode* rightRotate(splayNode* x)
{
    splayNode* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

static splayNode* leftRotate(splayNode* x)
{
    splayNode* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// Splay function: brings the key to root if present, else brings the last accessed node
splayNode* splay_tree_splay(splayNode* root, int key)
{
    if (root == NULL || root->key == key)
        return root;

    if (root->key > key)
    {
        if (root->left == NULL)
            return root;

        // Zig-Zig (Left Left)
        if (root->left->key > key)
        {
            root->left->left = splay_tree_splay(root->left->left, key);
            root = rightRotate(root);
        }
        // Zig-Zag (Left Right)
        else if (root->left->key < key)
        {
            root->left->right = splay_tree_splay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }
        return (root->left == NULL) ? root : rightRotate(root);
    }
    else
    {
        if (root->right == NULL)
            return root;

        // Zag-Zig (Right Left)
        if (root->right->key > key)
        {
            root->right->left = splay_tree_splay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        // Zag-Zag (Right Right)
        else if (root->right->key < key)
        {
            root->right->right = splay_tree_splay(root->right->right, key);
            root = leftRotate(root);
        }
        return (root->right == NULL) ? root : leftRotate(root);
    }
}

// Insert, and splay the new node to root
splayNode* splay_tree_insert(splayNode* root, int key)
{
    if (root == NULL)
        return splay_create_node(key);

    root = splay_tree_splay(root, key);
    if (root->key == key)
        return root; // Key already exists

    splayNode* newnode = splay_create_node(key);
    if (root->key > key)
    {
        newnode->right = root;
        newnode->left = root->left;
        root->left = NULL;
    }
    else
    {
        newnode->left = root;
        newnode->right = root->right;
        root->right = NULL;
    }
    return newnode;
}

// Delete, and splay the maximum node in the left subtree to root
splayNode* splay_tree_delete(splayNode* root, int key)
{
    splayNode* temp;
    if (!root)
        return NULL;

    root = splay_tree_splay(root, key);
    if (root->key != key)
        return root; // Key not found

    if (!root->left)
    {
        temp = root;
        root = root->right;
    }
    else
    {
        temp = root;
        root = splay_tree_splay(root->left, key);
        root->right = temp->right;
    }
    free(temp);
    return root;
}

splayNode* splay_tree_search(splayNode* root, int key)
{
    return splay_tree_splay(root, key);
}

void splay_tree_preorder(splayNode* root)
{
    if (root != NULL)
    {
        printf("%d ", root->key);
        splay_tree_preorder(root->left);
        splay_tree_preorder(root->right);
    }
}

void destroy_splay_tree(splayNode* root)
{
    if (root)
    {
        destroy_splay_tree(root->left);
        destroy_splay_tree(root->right);
        free(root);
    }
}
