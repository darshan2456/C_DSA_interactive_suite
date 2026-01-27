#include<stdio.h>
#include<stdlib.h>
#include "bst.h"
#include "safe_input.h"


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
    if((*head_ref)->data>value){
        if((*head_ref)->left==NULL){
            bstNode* node=malloc(sizeof(bstNode));
            if(node==NULL) return -1;
            node->data=value;
            node->left=NULL;
            node->right=NULL;
            (*head_ref)->left=node;
            return 1;
        }
        else{
            return bst_insert(&(*head_ref)->left,value);
        }
    }
    else if((*head_ref)->data<value){
        if((*head_ref)->right==NULL){
            bstNode* node=malloc(sizeof(bstNode));
            if(node==NULL) return -1;
            node->data=value;
            node->left=NULL;
            node->right=NULL;
            (*head_ref)->right=node;
            return 1;
        }
        else{
            return bst_insert(&(*head_ref)->right,value);
        }
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
    return countnodes(head->left) + countnodes(head->right)+1;
}

void destroy_bst(bstNode* head){
    if(head==NULL){
        return;
    }
    destroy_bst(head->left);
    destroy_bst(head->right);
    free(head);
}

void binary_search_tree_Demo(void){

    while(1){
        bstNode* head=NULL;
        int total_bst_nodes;
        int total_bst_nodes_status;

        printf("\n");
        total_bst_nodes_status=safe_input_int(&total_bst_nodes,
        "enter total number of nodes you want in the bst, (between 1 and 100), enter '-1' to exit:- ",
        1,100);

        if(total_bst_nodes_status==INPUT_EXIT_SIGNAL){
            printf("\nExiting binary search tree demo\n");
            return;
        }
        if(total_bst_nodes_status==0){
            continue;
        }


        int i=1;
        while(total_bst_nodes>0){
            int bst_node_value;
            int bst_node_value_status;
            printf("\nenter value of %d bst node - ",i);
            bst_node_value_status=safe_input_int(&bst_node_value,NULL,1,100);
            
            if(bst_node_value_status==INPUT_EXIT_SIGNAL){
                printf("\nExiting binary search tree demo\n");
                return;
            }
            if(bst_node_value_status==0){
                continue;
            }

            int insertion_status=bst_insert(&head,bst_node_value);
            if(insertion_status==0){
                printf("\nentered same value. only unique values please");
                continue;
            }
            if(insertion_status==-1){
                printf("\ncouldnt insert node due to malloc failure");
                return;
            }
            i++;
            total_bst_nodes--;
        }

        while(1){
            int bst_traversal_choice;
            int bst_traversal_status;

            bst_traversal_status=safe_input_int(&bst_traversal_choice,
            "\nenter '1' for inorder, '2' for preorder and '3' for postorder and '-1' to exit:- ",
            1,3);
            
            if(bst_traversal_status==INPUT_EXIT_SIGNAL){
                destroy_bst(head);
                break;
            }
            if(bst_traversal_choice==0){
                continue;
            }
            if(bst_traversal_choice==1){
                bst_inorder(head);
            }
            else if(bst_traversal_choice==2){
                bst_preorder(head);
            }
            else if(bst_traversal_choice==3){
                bst_postorder(head);
            }


        }

    }
    
}