#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct avlNode
{
    int data;
    int height;
    struct avlNode* left;
    struct avlNode* right;
} avlNode;

/* Public API */
void avl_tree_demo(void);

/* Insertion: returns 1 on success, 0 if duplicate, -1 on malloc failure */
int avl_insert(avlNode** root_ref, int value);

/* Deletion: returns 1 on success, 0 if value not found, -1 on error */
int avl_delete(avlNode** root_ref, int value);

/* Search: returns 1 if found, 0 if not found */
int avl_search(const avlNode* root, int key);

/* Traversals (read-only) */
void avl_inorder(const avlNode* root);
void avl_preorder(const avlNode* root);
void avl_postorder(const avlNode* root);

/* Utilities */
int avl_height(const avlNode* root);
int avl_countnodes(const avlNode* root);
int avl_balance_factor(const avlNode* root);

/* Cleanup */
void destroy_avl(avlNode* root);

#endif