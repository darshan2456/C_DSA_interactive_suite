#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

void btree_demo(void)
{
    while (1)
    {
        btreeNode* root = NULL;
        int t;
        int t_status;

        printf("\n");
        t_status = safe_input_int(&t,
                                  "enter minimum degree t for B-Tree (between 2 and 10), "
                                  "enter '-1' to exit: ",
                                  2, 10);

        if (t_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting B-Tree demo\n");
            btree_destroy(root);
            return;
        }
        if (t_status == 0)
            continue;

        int total_keys;
        int total_keys_status;

        total_keys_status = safe_input_int(&total_keys,
                                           "enter total number of keys to insert "
                                           "(between 1 and 100), enter '-1' to exit: ",
                                           1, 100);

        if (total_keys_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting B-Tree demo\n");
            btree_destroy(root);
            return;
        }
        if (total_keys_status == 0)
            continue;

        int i = 1;
        int remaining = total_keys;
        while (remaining > 0)
        {
            int key;
            int key_status;
            printf("\nenter value of key %d - ", i);
            key_status = safe_input_int(&key, NULL, 1, 1000);

            if (key_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting B-Tree demo\n");
                btree_destroy(root);
                return;
            }
            if (key_status == 0)
                continue;

            int result = btree_insert(&root, key, t);
            if (result == 0)
            {
                printf("\nduplicate key. only unique keys please");
                continue;
            }
            if (result == -1)
            {
                printf("\ncould not insert key due to malloc failure. try again\n");
                continue;
            }
            i++;
            remaining--;
        }

        printf("\nB-Tree inorder traversal: ");
        btree_traverse(root);
        printf("\n");

        while (1)
        {
            int op;
            int op_status;

            op_status = safe_input_int(&op,
                                       "\nenter '1' to search, '2' to insert, "
                                       "'3' to delete, '-1' to exit: ",
                                       1, 3);

            if (op_status == INPUT_EXIT_SIGNAL)
            {
                btree_destroy(root);
                break;
            }
            if (op_status == 0)
                continue;

            if (op == 1)
            {
                int search_key;
                int search_status;
                bool cancelled = false;
                while (1)
                {
                    search_status = safe_input_int(&search_key,
                                                   "\nenter key to search (between 1 and 1000), "
                                                   "enter '-1' to exit: ",
                                                   1, 1000);
                    if (search_status == INPUT_EXIT_SIGNAL)
                    {
                        cancelled = true;
                        break;
                    }
                    if (search_status == 0)
                        continue;
                    break;
                }
                if (cancelled)
                    continue;
                if (btree_search(root, search_key))
                    printf("\nkey %d found in B-Tree\n", search_key);
                else
                    printf("\nkey %d not found in B-Tree\n", search_key);
            }
            else if (op == 2)
            {
                int new_key;
                int new_key_status;
                bool cancelled = false;
                while (1)
                {
                    new_key_status = safe_input_int(&new_key,
                                                    "\nenter key to insert (between 1 and 1000), "
                                                    "enter '-1' to exit: ",
                                                    1, 1000);
                    if (new_key_status == INPUT_EXIT_SIGNAL)
                    {
                        cancelled = true;
                        break;
                    }
                    if (new_key_status == 0)
                        continue;
                    break;
                }
                if (cancelled)
                    continue;
                int result = btree_insert(&root, new_key, t);
                if (result == 0)
                    printf("\nduplicate key. only unique keys please\n");
                else if (result == -1)
                    printf("\ncould not insert due to malloc failure\n");
                else
                {
                    printf("\nkey inserted. updated traversal: ");
                    btree_traverse(root);
                    printf("\n");
                }
            }
            else if (op == 3)
            {
                int del_key;
                int del_status;
                bool cancelled = false;
                while (1)
                {
                    del_status = safe_input_int(&del_key,
                                                "\nenter key to delete (between 1 and 1000), "
                                                "enter '-1' to exit: ",
                                                1, 1000);
                    if (del_status == INPUT_EXIT_SIGNAL)
                    {
                        cancelled = true;
                        break;
                    }
                    if (del_status == 0)
                        continue;
                    break;
                }
                if (cancelled)
                    continue;
                root = btree_delete(root, del_key, t);
                printf("\nkey deleted. updated traversal: ");
                btree_traverse(root);
                printf("\n");
            }
        }
    }
}