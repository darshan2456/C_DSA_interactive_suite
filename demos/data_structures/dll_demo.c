#include "dll.h"
#include "export.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_int(const void* data)
{
    if (data != NULL)
    {
        printf("%d", *(const int*)data);
    }
}

static int compare_ints(const void* a, const void* b)
{
    return *(const int*)a - *(const int*)b;
}

void dll_demo(void)
{
    doubly_ll_Node* head = NULL;
    int dll_element_count;
    int dll_length_status;
start_dll:
    dll_length_status = safe_input_int(&dll_element_count,
                                       "\nenter the number of elements you want to insert, "
                                       "(between 1 and 100), enter '-1' to exit: ",
                                       1, 100);

    if (dll_length_status == INPUT_EXIT_SIGNAL)
    {
        printf("\nExiting dll demo.\n");
        delete_dll(head, free);
        return;
    }
    if (dll_length_status == 0)
    {
        goto start_dll;
    }

    // insertion of nodes in dll
    while (dll_element_count > 0)
    {
        int dll_position_choice;
        int dll_position_status;

    dll_position_selection:
        dll_position_status = safe_input_int(&dll_position_choice,
                                             "\nenter '0' for inserting at beginning"
                                             "\nenter '1' for inserting at end"
                                             "\nenter '2' for inserting at position"
                                             "\nenter '-1' to exit :- ",
                                             0, 2);

        if (dll_position_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting dll demo.\n");
            delete_dll(head, free);
            return;
        }
        if (dll_position_status == 0)
        {
            goto dll_position_selection;
        }

        if (dll_position_choice == 0)
        { // enter element at start
            int dll_end_status;
            int dll_end_value;

        dll_enter_end_value:
            dll_end_status = safe_input_int(
                &dll_end_value,
                "enter the value to be inserted at end, (between 1 and 100), enter '-1' to exit - ",
                1, 100);

            if (dll_end_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dll demo.\n");
                delete_dll(head, free);
                return;
            }
            if (dll_end_status == 0)
            {
                goto dll_enter_end_value;
            }
            int* val = malloc(sizeof(int));
            if (val == NULL)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto dll_enter_end_value;
            }
            *val = dll_end_value;
            int status = dll_insertAtBeginning(&head, val);
            if (status == -1)
            {
                free(val);
                printf("\nmalloc allocation failure. try again\n");
                goto dll_enter_end_value;
            }
            dll_printlist(head, print_int);
        }
        else if (dll_position_choice == 1)
        { // enter element at end
            int dll_start_status;
            int dll_start_value;

        dll_enter_start_value:
            dll_start_status = safe_input_int(&dll_start_value,
                                              "enter the value to be inserted at beginning, "
                                              "(between 1 and 100), enter '-1' to exit: ",
                                              1, 100);

            if (dll_start_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dll demo.\n");
                delete_dll(head, free);
                return;
            }
            if (dll_start_status == 0)
            {
                goto dll_enter_start_value;
            }
            int* val = malloc(sizeof(int));
            if (val == NULL)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto dll_enter_start_value;
            }
            *val = dll_start_value;
            int status = dll_insertAtEnd(&head, val);
            if (status == -1)
            {
                free(val);
                printf("\nmalloc allocation failure. try again\n");
                goto dll_enter_start_value;
            }
            dll_printlist(head, print_int);
        }
        else if (dll_position_choice == 2)
        {
            int dll_pos_status;
            int dll_pos_value;
            int dll_pos_index;
            char dll_pos_prompt[128];

        dll_enter_pos_value:
            dll_pos_status = safe_input_int(&dll_pos_value,
                                            "enter the value to be inserted, (between 1 "
                                            "and 100), enter '-1' to exit :- ",
                                            1, 100);

            if (dll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dll demo.\n");
                delete_dll(head, free);
                return;
            }

            if (dll_pos_status == 0)
            {
                goto dll_enter_pos_value;
            }

        dll_enter_pos_index:
            snprintf(dll_pos_prompt, sizeof(dll_pos_prompt),
                     "enter the position (0 to %d), enter '-1' to exit :- ",
                     dll_getLength(head) - 1);

            dll_pos_status = safe_input_int(&dll_pos_index, dll_pos_prompt, 0, dll_getLength(head));

            if (dll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting dll demo.\n");
                delete_dll(head, free);
                return;
            }

            if (dll_pos_status == 0)
            {
                goto dll_enter_pos_index;
            }

            int* val = malloc(sizeof(int));
            if (val == NULL)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto dll_enter_pos_value;
            }
            *val = dll_pos_value;
            int status = dll_insertAtPosition(&head, val, dll_pos_index);
            if (status == -1)
            {
                free(val);
                printf("\nmalloc allocation failure. try again\n");
                goto dll_enter_pos_value;
            }
            else if (status == -2)
            {
                free(val);
                printf("\ninvalid position. try again\n");
                goto dll_enter_pos_index;
            }
            dll_printlist(head, print_int);
        }
        dll_element_count--;
    }

    // doubly linked list reversal
    int dll_rev_status = dll_reverselist(&head);

    switch (dll_rev_status)
    {
        case -2:
            printf("\nempty list cannot be reversed.");
            break;
        case -1:
            printf("\nsingle node list cannot be reversed");
            break;
        case 1:
            printf("\nreversed list is:- ");
            dll_printlist(head, print_int);
            printf("\nthe restored list is:- ");
            dll_reverselist(&head);
            dll_printlist(head, print_int);
    }

    // searching elements in the dll
    while (1)
    {
        int dll_search_status;
        int dll_search_value;

        dll_search_status = safe_input_int(
            &dll_search_value,
            "\nenter the number you want to search, (between 1 and 100), enter '-1' to exit:- ", 1,
            100);

        if (dll_search_status == INPUT_EXIT_SIGNAL)
        {
            break;
        }

        if (dll_search_status == 0)
        {
            continue;
        }

        int index = dll_search(head, &dll_search_value, compare_ints);
        printf("\nentered number found at index %d", index);
    }

    // deleting values from dll
    while (1)
    {
        int dll_delete_choice;
        int dll_delete_status;

    dll_delete_selection:
        dll_delete_status = safe_input_int(&dll_delete_choice,
                                           "\nenter '0' to delete by value"
                                           "\nenter '1' to delete at position"
                                           "\nenter '-1' to exit :- ",
                                           0, 1);

        if (dll_delete_status == INPUT_EXIT_SIGNAL)
        {
            break;
        }
        if (dll_delete_status == 0)
        {
            goto dll_delete_selection;
        }

        if (dll_delete_choice == 0)
        {
            int dll_delete_value;
            dll_delete_status = safe_input_int(
                &dll_delete_value,
                "\nenter element to be deleted, (between 1 and 100), enter '-1' to exit :- ", 1,
                100);

            if (dll_delete_status == INPUT_EXIT_SIGNAL)
            {
                break;
            }
            if (dll_delete_status == 0)
            {
                continue;
            }

            int status = dll_deleteByValue(&head, &dll_delete_value, compare_ints, free);
            if (status == 1)
            {
                printf("\ndll after deletion - ");
                dll_printlist(head, print_int);
            }
            else if (status == -1)
            {
                printf("\nError: Node with value %d not found.\n", dll_delete_value);
            }
            else if (status == -2)
            {
                printf("\nError: List is empty.\n");
            }
        }
        else if (dll_delete_choice == 1)
        {
            int dll_pos_delete_status;
            int dll_pos_delete_index;
            char dll_pos_delete_prompt[128];

        dll_delete_pos_input:
            snprintf(dll_pos_delete_prompt, sizeof(dll_pos_delete_prompt),
                     "enter the position to delete (0 to %d), enter '-1' to exit :- ",
                     dll_getLength(head) - 1);
            dll_pos_delete_status = safe_input_int(&dll_pos_delete_index, dll_pos_delete_prompt, 0,
                                                   dll_getLength(head) - 1);

            if (dll_pos_delete_status == INPUT_EXIT_SIGNAL)
            {
                break;
            }

            if (dll_pos_delete_status == 0)
            {
                goto dll_delete_pos_input;
            }

            int status = dll_deleteAtPosition(&head, dll_pos_delete_index, free);
            if (status == -1)
            {
                printf("\nList is empty\n");
            }
            else if (status == -2)
            {
                printf("\nInvalid position\n");
            }
            else
            {
                printf("\ndll after deletion - ");
                dll_printlist(head, print_int);
            }
        }
    }

    // Export option
    int export_choice;
    int export_status = safe_input_int(&export_choice,
                                       "\nDo you want to export this Doubly Linked List to a file? "
                                       "(1 for Yes, 2 for No, or '-1' to exit): ",
                                       1, 2);
    if (export_status != INPUT_EXIT_SIGNAL && export_status != 0 && export_choice == 1)
    {
        char path[256];
        int path_status = safe_input_string(path, "Enter filename to export (e.g. list.json): ");
        if (path_status != INPUT_EXIT_SIGNAL && strlen(path) > 0)
        {
            int format_val;
            int format_status = safe_input_int(
                &format_val,
                "Select format (1 for Text, 2 for CSV, 3 for JSON, or '-1' to exit): ", 1, 3);
            if (format_status != INPUT_EXIT_SIGNAL && format_status != 0)
            {
                ExportFormat format = EXPORT_FORMAT_TEXT;
                if (format_val == 2)
                    format = EXPORT_FORMAT_CSV;
                else if (format_val == 3)
                    format = EXPORT_FORMAT_JSON;

                if (dll_export(head, path, format, write_data_int))
                {
                    printf("Exported successfully to %s\n", path);
                }
                else
                {
                    printf("Failed to export to %s\n", path);
                }
            }
        }
    }

    printf("\nExiting dll demo.\n");
    delete_dll(head, free);
}
