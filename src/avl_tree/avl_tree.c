#include "avl_tree.h"
#include <stdio.h>
#include <stdlib.h>

static int max_int(int a, int b)
{
    return (a > b) ? a : b;
}

// treats NULL as height 0, so leaves' children don't need special-casing
static int node_height(const avlNode* node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

static void update_height(avlNode* node)
{
    if (node == NULL)
        return;
    node->height = 1 + max_int(node_height(node->left), node_height(node->right));
}

// returns NULL on malloc failure
static avlNode* create_avl_node(int value)
{
    avlNode* node = malloc(sizeof(avlNode));
    if (node == NULL)
        return NULL;
    node->data = value;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// rotations only rearrange existing pointers, no malloc, so they cannot leak.
// both return the new root of the rotated subtree.
static avlNode* rotate_right(avlNode* y)
{
    avlNode* x = y->left;
    avlNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    // update y first since it is now the lower node
    update_height(y);
    update_height(x);

    return x;
}

static avlNode* rotate_left(avlNode* x)
{
    avlNode* y = x->right;
    avlNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    update_height(x);
    update_height(y);

    return y;
}

static avlNode* rebalance(avlNode* node)
{
    update_height(node);
    int bf = node_height(node->left) - node_height(node->right);

    // left heavy, left child leans left (or balanced) -> LL
    if (bf > 1 && node_height(node->left->left) >= node_height(node->left->right))
        return rotate_right(node);

    // left heavy, left child leans right -> LR
    if (bf > 1 && node_height(node->left->left) < node_height(node->left->right))
    {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // right heavy, right child leans right (or balanced) -> RR
    if (bf < -1 && node_height(node->right->right) >= node_height(node->right->left))
        return rotate_left(node);

    // right heavy, right child leans left -> RL
    if (bf < -1 && node_height(node->right->right) < node_height(node->right->left))
    {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

static avlNode* min_value_node(avlNode* node)
{
    avlNode* current = node;
    while (current != NULL && current->left != NULL)
        current = current->left;
    return current;
}

static avlNode* avl_insert_recursive(avlNode* node, int value, int* status)
{
    if (node == NULL)
    {
        avlNode* new_node = create_avl_node(value);
        if (new_node == NULL)
        {
            *status = -1;
            return NULL;
        }
        *status = 1;
        return new_node;
    }

    if (value == node->data)
    {
        *status = 0;
        return node;
    }

    if (value < node->data)
    {
        avlNode* new_left = avl_insert_recursive(node->left, value, status);
        if (*status == -1)
            return node;
        node->left = new_left;
    }
    else
    {
        avlNode* new_right = avl_insert_recursive(node->right, value, status);
        if (*status == -1)
            return node;
        node->right = new_right;
    }

    if (*status == 0)
        return node;

    return rebalance(node);
}

static avlNode* avl_delete_recursive(avlNode* node, int key, int* status)
{
    if (node == NULL)
    {
        *status = 0;
        return NULL;
    }

    if (key < node->data)
    {
        node->left = avl_delete_recursive(node->left, key, status);
    }
    else if (key > node->data)
    {
        node->right = avl_delete_recursive(node->right, key, status);
    }
    else
    {
        *status = 1;

        // one child or none
        if (node->left == NULL || node->right == NULL)
        {
            avlNode* child = (node->left != NULL) ? node->left : node->right;
            free(node);
            return child;
        }

        // two children: copy inorder successor's value, then delete it
        avlNode* successor = min_value_node(node->right);
        node->data = successor->data;
        int dummy_status;
        node->right = avl_delete_recursive(node->right, successor->data, &dummy_status);
    }

    if (node == NULL)
        return NULL;

    return rebalance(node);
}

int avl_insert(avlNode** root_ref, int value)
{
    if (root_ref == NULL)
        return -1;
    int status = 0;
    *root_ref = avl_insert_recursive(*root_ref, value, &status);
    return status;
}

int avl_delete(avlNode** root_ref, int value)
{
    if (root_ref == NULL)
        return -1;
    int status = 0;
    *root_ref = avl_delete_recursive(*root_ref, value, &status);
    return status;
}

int avl_search(const avlNode* root, int key)
{
    while (root != NULL)
    {
        if (key == root->data)
            return 1;
        if (key < root->data)
            root = root->left;
        else
            root = root->right;
    }
    return 0;
}

void avl_inorder(const avlNode* root)
{
    if (root == NULL)
        return;
    avl_inorder(root->left);
    printf("%d,", root->data);
    avl_inorder(root->right);
}

void avl_preorder(const avlNode* root)
{
    if (root == NULL)
        return;
    printf("%d,", root->data);
    avl_preorder(root->left);
    avl_preorder(root->right);
}

void avl_postorder(const avlNode* root)
{
    if (root == NULL)
        return;
    avl_postorder(root->left);
    avl_postorder(root->right);
    printf("%d,", root->data);
}

int avl_height(const avlNode* root)
{
    return node_height(root);
}

int avl_countnodes(const avlNode* root)
{
    if (root == NULL)
        return 0;
    return 1 + avl_countnodes(root->left) + avl_countnodes(root->right);
}

int avl_balance_factor(const avlNode* root)
{
    if (root == NULL)
        return 0;
    return node_height(root->left) - node_height(root->right);
}

void destroy_avl(avlNode* root)
{
    if (root == NULL)
        return;
    destroy_avl(root->left);
    destroy_avl(root->right);
    free(root);
}