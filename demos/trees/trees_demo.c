#include "display_header.h"
#include "safe_input.h"
#include "trees.h"
#include <stdio.h>

void trees_demo(void)
{
    int tree_status, tree_choice;

    while (1)
    {
        display_header("Trees");

        tree_status = safe_input_int(&tree_choice,
                                     "\nenter 1 for Binary Search Tree demo"
                                     "\nenter 2 for AVL Tree demo"
                                     "\nenter 3 for Threaded Binary Tree demo"
                                     "\nenter 4 for Trie demo"
                                     "\nenter 5 for B-Tree demo"
                                     "\nenter 6 for B+ Tree demo"
                                     "\nenter 7 for Segment Tree demo"
                                     "\nenter 8 for Fenwick Tree (BIT) demo"
                                     "\nenter 9 for Splay Tree demo"
                                     "\nenter 10 for Red-Black Tree demo"
                                     "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                     1, 10);

        if (tree_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting trees_demo....\n");
            return;
        }

        if (tree_status == 0)
            continue;

        switch (tree_choice)
        {
            case 1:
                display_header("Binary Search Tree");
                binary_search_tree_demo();
                break;
            case 2:
                display_header("AVL Tree");
                avl_demo();
                break;
            case 3:
                display_header("Threaded Binary Tree");
                TBT_demo();
                break;
            case 4:
                display_header("Trie");
                trie_demo();
                break;
            case 5:
                display_header("B-Tree");
                btree_demo();
                break;
            case 6:
                display_header("B+ Tree");
                bplus_tree_demo();
                break;
            case 7:
                display_header("Segment Tree");
                segment_tree_demo();
                break;

            case 8:
                display_header("Fenwick Tree (BIT)");
                fenwick_tree_demo();
                break;

            case 9:
                display_header("Splay Tree");
                splay_tree_demo();
                break;

            case 10:
                display_header("Red-Black Tree");
                red_black_tree_demo();
                break;
        }
    }
}