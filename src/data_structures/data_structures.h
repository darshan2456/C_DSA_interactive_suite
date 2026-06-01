#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
#define INPUT_EXIT_SIGNAL -111
#include <stddef.h>
#include <stdbool.h>

// For arrays
void swap_adjacent_pairs(int arr[], int length_of_array);
int max_array(const int arr[], int length_of_array);
int min_array(const int arr[], int length_of_array);
void print_array(const int arr[], int length_of_array);
void reverse_array(int arr[], int length_of_array);
void array_demo(void);

//For Binary Search Tree
typedef struct bstNode
{
    int data;
    struct bstNode* left;
    struct bstNode* right;
} bstNode;
void binary_search_tree_Demo(void);
int bst_insert(bstNode** head_ref, int value);
void bst_inorder(const bstNode* head);
void bst_preorder(const bstNode* head);
void bst_postorder(const bstNode* head);
int countnodes(const bstNode* head);
int tree_height(const bstNode* root);
void destroy_bst(bstNode* head);

//For circular queue
typedef struct circular_queue
{
    int rear;
    int front;
    int N;
    int* arr;
} circular_queue;
int init_circ_queue(int N, circular_queue* queue_ptr);
void destroy_circ_queue(circular_queue* queue_ptr);
int enqueue(circular_queue* queue_ptr, int value);
int dequeue(circular_queue* queue_ptr);
void display_circ_queue(circular_queue* queue_ptr);
void circular_queue_Demo(void);

//For double linked list
typedef struct doubly_ll_Node
{
    int data;
    struct doubly_ll_Node* prev;
    struct doubly_ll_Node* next;
} doubly_ll_Node;
int dll_insertAtBeginning(doubly_ll_Node** head_ref, int value);
int dll_insertAtEnd(doubly_ll_Node** head_ref, int value);
void dll_printlist(const doubly_ll_Node* head);
int dll_search(const doubly_ll_Node* head, int key);
int dll_deleteAtBeginning(doubly_ll_Node** head_ref);
int dll_deleteAtEnd(doubly_ll_Node** head_ref);
int dll_deleteByValue(doubly_ll_Node** head_ref, int key);
void delete_dll(doubly_ll_Node* head);
void dll_demo(void);
int dll_reverselist(doubly_ll_Node** head_ref);

//For single linked list
typedef struct Node
{
    int data;
    struct Node* next;
} Node;
void sll_printlist(const Node* head);
int sll_insertAtEnd(Node** head_ref, int value);
int sll_deleteAtBeginning(Node** head_ref);
int sll_deleteAtEnd(Node** head_ref);
int sll_deleteByValue(Node** head_ref, int value);
int sll_insertAtBeginning(Node** head_ref, int value);
void sll_Demo(void);
int sll_search(const Node* head, int key);
int sll_reverseList(Node** head_ref);
void delete_sll(Node* head);

//For Threadded Binary Tree
typedef struct TBTnode
{
    int data;
    bool lthread;
    bool rthread;
    struct TBTnode* left;
    struct TBTnode* right;
} TBTnode;
TBTnode* leftmost(TBTnode* node);
TBTnode* create_node_tbt(int data);
int insert_node_tbt(TBTnode** root_ref, int key);
void destroy_tbt(TBTnode* node);
void TBT_demo(void);

#endif