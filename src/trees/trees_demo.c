#include "display_header.h"
#include "safe_input.h"
#include "trees.h"
#include <stdio.h>

typedef enum
{
    CHOICE_BST = 1,
    CHOICE_AVL,
    CHOICE_TBT,
    CHOICE_TRIE,
    CHOICE_BTREE,
    CHOICE_BPLUS,
    CHOICE_SEGMENT
} TreeChoice;

void trees_demo(void)
{
    int tree_status;
    TreeChoice tree_choice;

    while (1)
    {
        display_header("Trees");

        tree_status = safe_input_int((int*)&tree_choice,
                                     "\nenter 1 for Binary Search Tree demo"
                                     "\nenter 2 for AVL Tree demo"
                                     "\nenter 3 for Threaded Binary Tree demo"
                                     "\nenter 4 for Trie demo"
                                     "\nenter 5 for B-Tree demo"
                                     "\nenter 6 for B+ Tree demo"
                                     "\nenter 7 for Segment Tree demo"
                                     "\nenter choice : ",
                                     1, 7);

        if (tree_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting trees_demo....\n");
            return;
        }

        if (tree_status == 0)
            continue;

        switch (tree_choice)
        {
            case CHOICE_BST:
                display_header("Binary Search Tree");
                binary_search_tree_demo();
                break;

            case CHOICE_AVL:
                display_header("AVL Tree");
                avl_demo();
                break;

            case CHOICE_TBT:
                display_header("Threaded Binary Tree");
                TBT_demo();
                break;

            case CHOICE_TRIE:
                display_header("Trie");
                trie_demo();
                break;

            case CHOICE_BTREE:
                display_header("B-Tree");
                btree_demo();
                break;

            case CHOICE_BPLUS:
                display_header("B+ Tree");
                bplus_tree_demo();
                break;

            case CHOICE_SEGMENT:
                display_header("Segment Tree");
                segment_tree_demo();
                break;
        }
    }
}