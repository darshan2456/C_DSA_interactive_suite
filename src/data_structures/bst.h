#ifndef BST_H
#define BST_H

typedef struct bstNode{
int data;
struct bstNode* left;
struct bstNode* right;
}bstNode;

void binary_search_tree_Demo(void);
int bst_insert(bstNode** head_ref,int value);
void bst_inorder(const bstNode* head);
void bst_preorder(const bstNode* head);
void bst_postorder(const bstNode* head);
int countnodes(const bstNode* head);
int tree_height(const bstNode* root);
void destroy_bst(bstNode* head);

#endif