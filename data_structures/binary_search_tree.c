#include<stdio.h>
#include<stdlib.h>
#include "bst.h"



//insert function returns -1 on malloc failure, 0 when value already exists in the tree and 1 on successful insertion


int bst_insert(bstNode** head_ref,int value){
    if((*head_ref)==NULL){
        bstNode* node=malloc(sizeof(bstNode));
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
       bstNode* node=malloc(sizeof(bstNode));
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
        bstNode* node=malloc(sizeof(bstNode));
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
    return -1;
}

void bst_inorder(const bstNode* head){
    if(head==NULL){
        return;
    }
    bst_inorder(head->left);
    printf("%d,",head->data);
    bst_inorder(head->right);
}

void bst_preorder(const bstNode* head){
    if(head==NULL){
        return;
    }
    printf("%d,",head->data);
    bst_preorder(head->left);
    bst_preorder(head->right);
}

void bst_postorder(const bstNode* head){
    if(head==NULL){
        return;
    }
    bst_postorder(head->left);
    bst_postorder(head->right);
    printf("%d,",head->data);
}

int countnodes(const bstNode* head){
    if(head==NULL) return 0;
    if(head->left==NULL&&head->right==NULL){
        return 1;
    }
    return countnodes(head->left) + countnodes(head->right)+1;
}

void binary_search_tree_Demo(void){
    bstNode* head=NULL;
    int total_bst_nodes;
    printf("enter total number of nodes you want in the bst :- ");
    scanf("%d",&total_bst_nodes);
    while(total_bst_nodes>0){
        int value;
        printf("\nenter value of %d bst node - ",total_bst_nodes);
        scanf("%d",&value);
        bst_insert(&head,value);
        total_bst_nodes--;
    }
    while(1){
        int choice;
        printf("\nenter '1' for inorder, '2' for preorder and '3' for postorder :- ");
        scanf("%d",&choice);
        if(choice==1){
            bst_inorder(head);
        }
        else if(choice==2){
            bst_preorder(head);
        }
        else if(choice==3){
            bst_postorder(head);
        }
        else{
            printf("\nenter only one of the three - 1,2,3");
        }

        printf("\nenter '-1' to exit and any other number to try different traversal pattern :- ");
        scanf("%d",&choice);
        if(choice==-1) break;

    }
    

}
