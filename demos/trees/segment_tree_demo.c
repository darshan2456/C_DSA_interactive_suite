#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

void segment_tree_demo(void)
{
    while (1)
    {
        int n;
        int n_status = safe_input_int(
            &n, "\n\nenter length of the array, (between 1 and 50), enter '-1' to exit :- ", 1, 50);

        if (n_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Segment Tree demo\n");
            return;
        }
        if (n_status == 0)
        {
            continue;
        }

        int* arr = (int*)malloc(n * sizeof(int));
        if (arr == NULL)
        {
            printf("\nMemory allocation failed for input array\n");
            continue;
        }

        int i = 0;
        bool back_to_size_selection = false;
        while (i < n)
        {
            int val;
            char prompt[128];
            snprintf(prompt, sizeof(prompt),
                     "enter array element %d, (between -10000 and 10000), enter '-1' to exit:- ",
                     i);
            int val_status = safe_input_int(&val, prompt, -10000, 10000);

            if (val_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Segment Tree demo\n");
                free(arr);
                return;
            }
            if (val_status == 0)
            {
                continue;
            }
            arr[i] = val;
            i++;
        }

        SegmentTree* st = create_segment_tree(arr, n);
        if (st == NULL)
        {
            printf("\nFailed to create Segment Tree\n");
            free(arr);
            continue;
        }
        build_tree(st, arr, 0, 0, n - 1);

        printf("\nSegment Tree built successfully!\n");
        printf("Original array: [");
        for (int j = 0; j < n; j++)
        {
            printf("%d", arr[j]);
            if (j < n - 1)
                printf(",");
        }
        printf("]\n");

        while (1)
        {
            int choice;
            int choice_status =
                safe_input_int(&choice,
                               "\nenter '1' to query range sum"
                               "\nenter '2' to update an element"
                               "\nenter '3' to print traversals (preorder, inorder, postorder)"
                               "\nenter '4' to print array and segment tree array representation"
                               "\nenter '-1' to exit segment tree demo:- ",
                               1, 4);

            if (choice_status == INPUT_EXIT_SIGNAL)
            {
                back_to_size_selection = true;
                break;
            }
            if (choice_status == 0)
            {
                continue;
            }

            if (choice == 1)
            {
                int l, r;
                int l_status, r_status;
                char prompt_l[128];
                char prompt_r[128];

                snprintf(prompt_l, sizeof(prompt_l),
                         "enter query range start index (0 to %d), enter '-1' to exit: ", n - 1);
                l_status = safe_input_int(&l, prompt_l, 0, n - 1);
                if (l_status == INPUT_EXIT_SIGNAL)
                    continue;
                if (l_status == 0)
                    continue;

                snprintf(prompt_r, sizeof(prompt_r),
                         "enter query range end index (%d to %d), enter '-1' to exit: ", l, n - 1);
                r_status = safe_input_int(&r, prompt_r, l, n - 1);
                if (r_status == INPUT_EXIT_SIGNAL)
                    continue;
                if (r_status == 0)
                    continue;

                int sum = query_range(st, 0, 0, n - 1, l, r);
                printf("\nSum of elements in range [%d, %d] is: %d\n", l, r, sum);
            }
            else if (choice == 2)
            {
                int idx, val;
                int idx_status, val_status;
                char prompt_idx[128];

                snprintf(prompt_idx, sizeof(prompt_idx),
                         "enter index to update (0 to %d), enter '-1' to exit: ", n - 1);
                idx_status = safe_input_int(&idx, prompt_idx, 0, n - 1);
                if (idx_status == INPUT_EXIT_SIGNAL)
                    continue;
                if (idx_status == 0)
                    continue;

                val_status = safe_input_int(
                    &val,
                    "enter new value (between -10000 and 10000), enter '-1' to exit: ", -10000,
                    10000);
                if (val_status == INPUT_EXIT_SIGNAL)
                    continue;
                if (val_status == 0)
                    continue;

                arr[idx] = val;
                update_point(st, 0, 0, n - 1, idx, val);
                printf("\nElement at index %d updated to %d.\n", idx, val);
            }
            else if (choice == 3)
            {
                printf("\nPreorder traversal: ");
                preorder_traversal(st, 0, 0, n - 1);
                printf("\nInorder traversal: ");
                inorder_traversal(st, 0, 0, n - 1);
                printf("\nPostorder traversal: ");
                postorder_traversal(st, 0, 0, n - 1);
                printf("\n");
            }
            else if (choice == 4)
            {
                printf("\nOriginal array: [");
                for (int j = 0; j < n; j++)
                {
                    printf("%d", arr[j]);
                    if (j < n - 1)
                        printf(",");
                }
                printf("]");
                printf("\nSegment Tree array representation (size %d): [", st->size);
                for (int j = 0; j < st->size; j++)
                {
                    printf("%d", st->tree[j]);
                    if (j < st->size - 1)
                        printf(",");
                }
                printf("]\n");
            }
        }

        destroy_segment_tree(st);
        free(arr);

        if (back_to_size_selection)
        {
            printf("\nReturning to data structures menu...\n");
            return;
        }
    }
}
