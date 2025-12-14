#include<stdio.h>
#include<stdlib.h>


typedef struct Node{
int data;
struct Node* left;
struct Node* right;
}Node;


int bst_insert(Node** head_ref,int value){
    if((*head_ref)==NULL){
        Node* node=malloc(sizeof(Node));
        if(node==NULL) return -1;
        node->data=value;
        node->left=NULL;
        node->right=NULL;
        *head_ref=node;
        return 1;
    }
    if((*head_ref)->data==value){
        return 0;
    }
    if((*head_ref)->data>value && (*head_ref)->left==NULL){
       Node* node=malloc(sizeof(Node));
       if(node==NULL) return -1;
       node->data=value;
       node->left=NULL;
        node->right=NULL;
       (*head_ref)->left=node;
       return 1;
    }
    else if((*head_ref)->data>value && (*head_ref)->left!=NULL){
        return bst_insert(&(*head_ref)->left,value);
    }
    else if((*head_ref)->data<value && (*head_ref)->right==NULL){
        Node* node=malloc(sizeof(Node));
        if(node==NULL) return -1;
        node->data=value;
        node->left=NULL;
        node->right=NULL;
        (*head_ref)->right=node;
        return 1;
    }
    else if((*head_ref)->data<value && (*head_ref)->right!=NULL){
        return bst_insert(&(*head_ref)->right,value);
    }
    
}

void bst_inorder(Node* head){
    if(head==NULL){
        return;
    }
    bst_inorder(head->left);
    printf("%d,",head->data);
    bst_inorder(head->right);
}

void bst_preorder(Node* head){
    if(head==NULL){
        return;
    }
    printf("%d,",head->data);
    bst_preorder(head->left);
    bst_preorder(head->right);
}

void bst_postorder(Node* head){
    if(head==NULL){
        return;
    }
    bst_postorder(head->left);
    bst_postorder(head->right);
    printf("%d,",head->data);
}

int countnodes(Node* head){
    if(head==NULL) return 0;
    if(head->left==NULL&&head->right==NULL){
        return 1;
    }
    return countnodes(head->left) + countnodes(head->right)+1;
}
int main(){
    Node* head=NULL;
    bst_insert(&head,78);
    bst_insert(&head,46);
    bst_insert(&head,23);
    bst_insert(&head,12);
    bst_insert(&head,19);
    bst_inorder(head);
    printf("\n");
    bst_preorder(head);
    printf("\n");
    bst_postorder(head);
    int count=0;
    count=countnodes(head);
    printf("\n number of nodes in tree is :- %d",count);
    return 0;
}