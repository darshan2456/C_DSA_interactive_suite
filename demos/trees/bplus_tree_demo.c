#include "mwst_utils.h"
#include "safe_input.h"
#include "trees.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bplus_tree_demo(void)
{
    int order;
    while (1)
    {
        int status = safe_input_int(
            &order, "\nEnter B+ Tree order (minimum 3, maximum 10), enter '-1' to exit: ", 3, 10);
        if (status == INPUT_EXIT_SIGNAL)
            return;
        if (status == 0)
            continue;
        break;
    }

    BPlusTree* tree = bplus_tree_create(order);
    if (!tree)
    {
        printf("Failed to create B+ Tree\n");
        return;
    }

    while (1)
    {
        int choice;
        int status = safe_input_int(&choice,
                                    "\n--- B+ Tree Demo Menu ---\n"
                                    "1. Insert key-value pair\n"
                                    "2. Delete key\n"
                                    "3. Search key\n"
                                    "4. Range Query\n"
                                    "5. Print Tree Structure\n"
                                    "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                    1, 5);
        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("Exiting B+ Tree demo...\n");
            bplus_tree_destroy(tree);
            return;
        }
        if (status == 0)
            continue;

        switch (choice)
        {
            case 1:
            {
                int key, val;
                while (1)
                {
                    int s =
                        safe_input_int(&key, "Enter key to insert (positive integer): ", 1, 10000);
                    if (s == INPUT_EXIT_SIGNAL)
                        break;
                    if (s == 0)
                        continue;
                    bool cancelled = false;
                    while (1)
                    {
                        int s2 = safe_input_int(
                            &val, "Enter corresponding value (positive integer): ", 1, 10000);
                        if (s2 == INPUT_EXIT_SIGNAL)
                        {
                            cancelled = true;
                            break;
                        }
                        if (s2 == 0)
                            continue;
                        break;
                    }
                    if (cancelled)
                        break;
                    if (bplus_tree_insert(tree, key, val))
                        printf("Successfully inserted [%d: %d]\n", key, val);
                    else
                        printf("Insertion failed! Key %d may already exist.\n", key);
                    break;
                }
                break;
            }
            case 2:
            {
                int key;
                while (1)
                {
                    int s = safe_input_int(&key, "Enter key to delete: ", 1, 10000);
                    if (s == INPUT_EXIT_SIGNAL)
                        break;
                    if (s == 0)
                        continue;
                    if (bplus_tree_delete(tree, key))
                        printf("Successfully deleted key %d\n", key);
                    else
                        printf("Key %d not found in tree.\n", key);
                    break;
                }
                break;
            }
            case 3:
            {
                int key, val;
                while (1)
                {
                    int s = safe_input_int(&key, "Enter key to search: ", 1, 10000);
                    if (s == INPUT_EXIT_SIGNAL)
                        break;
                    if (s == 0)
                        continue;
                    if (bplus_tree_search(tree, key, &val))
                        printf("Found key %d with value: %d\n", key, val);
                    else
                        printf("Key %d not found.\n", key);
                    break;
                }
                break;
            }
            case 4:
            {
                int lower, upper;
                while (1)
                {
                    int s = safe_input_int(&lower, "Enter lower bound key: ", 1, 10000);
                    if (s == INPUT_EXIT_SIGNAL)
                        break;
                    if (s == 0)
                        continue;
                    bool cancelled = false;
                    while (1)
                    {
                        int s2 = safe_input_int(&upper, "Enter upper bound key: ", lower, 10000);
                        if (s2 == INPUT_EXIT_SIGNAL)
                        {
                            cancelled = true;
                            break;
                        }
                        if (s2 == 0)
                            continue;
                        break;
                    }
                    if (cancelled)
                        break;
                    printf("Range query results for [%d, %d]:\n", lower, upper);
                    bplus_tree_range_query(tree, lower, upper);
                    break;
                }
                break;
            }
            case 5:
            {
                bplus_tree_print(tree);
                break;
            }
        }
    }
}
