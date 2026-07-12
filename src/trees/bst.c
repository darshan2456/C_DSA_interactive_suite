#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>
int deletionStrategy = 1;
#define SUCCESSOR 1
#define PREDECESSOR 2

// insert function returns -1 on malloc failure, 0 when value already exists in the tree and 1 on
// successful insertion

int bst_insert(bstNode** head_ref, int value)
{
    if ((*head_ref) == NULL)
    {
        bstNode* node = malloc(sizeof(bstNode));
        if (node == NULL)
            return -1;
        node->data = value;
        node->left = NULL;
        node->right = NULL;
        *head_ref = node;
        return 1;
    }
    if ((*head_ref)->data == value)
    {
        return 0;
    }
    if ((*head_ref)->data > value)
    {
        if ((*head_ref)->left == NULL)
        {
            bstNode* node = malloc(sizeof(bstNode));
            if (node == NULL)
                return -1;
            node->data = value;
            node->left = NULL;
            node->right = NULL;
            (*head_ref)->left = node;
            return 1;
        }
        else
        {
            return bst_insert(&(*head_ref)->left, value);
        }
    }
    else if ((*head_ref)->data < value)
    {
        if ((*head_ref)->right == NULL)
        {
            bstNode* node = malloc(sizeof(bstNode));
            if (node == NULL)
                return -1;
            node->data = value;
            node->left = NULL;
            node->right = NULL;
            (*head_ref)->right = node;
            return 1;
        }
        else
        {
            return bst_insert(&(*head_ref)->right, value);
        }
    }
    return -1;
}

void bst_inorder(const bstNode* head)
{
    if (head == NULL)
    {
        return;
    }
    bst_inorder(head->left);
    printf("%d,", head->data);
    bst_inorder(head->right);
}

void bst_preorder(const bstNode* head)
{
    if (head == NULL)
    {
        return;
    }
    printf("%d,", head->data);
    bst_preorder(head->left);
    bst_preorder(head->right);
}

void bst_postorder(const bstNode* head)
{
    if (head == NULL)
    {
        return;
    }
    bst_postorder(head->left);
    bst_postorder(head->right);
    printf("%d,", head->data);
}

void printLevel(bstNode* head, int level)
{
    if (!head)
        return;

    if (level == 1)
        printf("%d ", head->data);
    else
    {
        printLevel(head->left, level - 1);
        printLevel(head->right, level - 1);
    }
}

void bst_level_order(struct bstNode* head)
{
    if (!head)
        return;

    int h = tree_height(head);

    for (int i = 1; i <= h; i++)
    {
        printf("Level %d: ", i);
        printLevel(head, i);
        printf("\n");
    }
}

int countnodes(const bstNode* head)
{
    if (head == NULL)
        return 0;
    return countnodes(head->left) + countnodes(head->right) + 1;
}

int tree_height(const bstNode* root)
{
    if (root == NULL)
        return 0;
    int left_height = tree_height(root->left) + 1;
    int right_height = tree_height(root->right) + 1;

    if (left_height > right_height)
        return left_height;
    else
        return right_height;
}

void destroy_bst(bstNode* head)
{
    if (head == NULL)
    {
        return;
    }
    destroy_bst(head->left);
    destroy_bst(head->right);
    free(head);
}

bstNode* bst_delete(bstNode* root, int value)
{
    if (root == NULL)
        return NULL;

    if (value < root->data)
    {
        root->left = bst_delete(root->left, value);
    }
    else if (value > root->data)
    {
        root->right = bst_delete(root->right, value);
    }
    else
    {
        if (root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }
        else if (root->left == NULL)
        {
            bstNode* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            bstNode* temp = root->left;
            free(root);
            return temp;
        }
        else
        {
            if (deletionStrategy == SUCCESSOR)
            {
                bstNode* successor = root->right;
                while (successor->left != NULL)
                    successor = successor->left;
                root->data = successor->data;
                root->right = bst_delete(root->right, successor->data);
            }
            else if (deletionStrategy == PREDECESSOR)
            {
                bstNode* predecessor = root->left;
                while (predecessor->right != NULL)
                    predecessor = predecessor->right;
                root->data = predecessor->data;
                root->left = bst_delete(root->left, predecessor->data);
            }
        }
    }
    return root;
}
