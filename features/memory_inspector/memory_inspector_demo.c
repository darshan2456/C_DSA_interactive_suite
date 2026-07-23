#include "dll.h"
#include "memory_inspector.h"
#include "safe_input.h"
#include "sll.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void memory_inspector_demo(void)
{
    while (1)
    {
        printf("\n========================================================================\n");
        printf("         RAW MEMORY LAYOUT INSPECTOR & HEXDUMP VISUALIZER SUITE          \n");
        printf("========================================================================\n");
        printf("1. Inspect Singly-Linked List Node (Node) Memory Layout\n");
        printf("2. Inspect Doubly-Linked List Node (doubly_ll_Node) Memory Layout\n");
        printf("3. Inspect Binary Search Tree Node (bstNode) Memory Layout\n");
        printf("4. Interactive Custom Memory Buffer Hexdump Visualizer\n");
        printf("-1. Return to Main Menu\n");

        int choice;
        int status = safe_input_int(&choice, "\nEnter choice: ", 1, 4);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Memory Inspector Demo...\n");
            return;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
            {
                int sample_val = 2026;
                Node sample_node = {.data = &sample_val, .next = NULL};
                printf("\n>>> Inspecting Live Singly-Linked List Node at %p...\n",
                       (void*)&sample_node);
                inspect_sll_node_memory(&sample_node);
                break;
            }
            case 2:
            {
                int sample_val = 999;
                doubly_ll_Node sample_dll = {.data = &sample_val, .prev = NULL, .next = NULL};
                printf("\n>>> Inspecting Live Doubly-Linked List Node at %p...\n",
                       (void*)&sample_dll);
                inspect_dll_node_memory(&sample_dll);
                break;
            }
            case 3:
            {
                bstNode sample_bst = {.data = 777, .left = NULL, .right = NULL};
                printf("\n>>> Inspecting Live Binary Search Tree Node at %p...\n",
                       (void*)&sample_bst);
                inspect_bst_node_memory(&sample_bst);
                break;
            }
            case 4:
            {
                char custom_str[128] = "C_DSA_Interactive_Suite_Memory_Inspector_2026";
                printf("\n>>> Hexdumping Custom Memory Buffer at %p (%zu bytes):\n",
                       (void*)custom_str, strlen(custom_str) + 1);
                print_hexdump(custom_str, strlen(custom_str) + 1);
                break;
            }
        }
    }
}
