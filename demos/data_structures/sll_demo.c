#include "safe_input.h"
#include "sll.h"
#include <stdio.h>

void sll_demo(void)
{
    Node* head = NULL;
    int sll_element_count;
    int sll_length_status;

start_sll:
    sll_length_status = safe_input_int(
        &sll_element_count,
        "enter how many elements you want to insert, (between 1 and 100), enter '-1' to exit :- ",
        1, 100);

    if (sll_length_status == INPUT_EXIT_SIGNAL)
    {
        printf("\nExiting sll demo.\n");
        delete_sll(head);
        return;
    }

    if (sll_length_status == 0)
    {
        goto start_sll;
    }

    while (sll_element_count > 0)
    {
        int sll_position_choice;
        int sll_position_status;

    sll_position_selection:
        sll_position_status = safe_input_int(&sll_position_choice,
                                             "\nenter '0' for inserting at beginning"
                                             "\nenter '1' for inserting at end"
                                             "\nenter '2' for inserting at specific position"
                                             "\nenter '-1' to exit :- ",
                                             0, 2);

        if (sll_position_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting sll demo.\n");
            delete_sll(head);
            return;
        }

        if (sll_position_status == 0)
        {
            goto sll_position_selection;
        }

        if (sll_position_choice == 1)
        {
            int sll_end_status;
            int sll_end_value;
        sll_enter_end_value:
            sll_end_status = safe_input_int(&sll_end_value,
                                            "enter the no. you want to insert at end, (between 1 "
                                            "and 100), enter '-1' to exit :- ",
                                            1, 100);

            if (sll_end_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting sll demo.\n");
                delete_sll(head);
                return;
            }

            if (sll_end_status == 0)
            {
                goto sll_enter_end_value;
            }

            int status = sll_insertAtEnd(&head, sll_end_value);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto sll_enter_end_value;
            }
            printf("\n");
            sll_printlist(head);
        }
        else if (sll_position_choice == 0)
        {
            int sll_start_status;
            int sll_start_value;

        sll_enter_start_value:
            sll_start_status = safe_input_int(&sll_start_value,
                                              "enter the no. you want to insert at beginning, "
                                              "(between 1 and 100), enter '-1' to exit :- ",
                                              1, 100);

            if (sll_start_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting sll demo.\n");
                delete_sll(head);
                return;
            }

            if (sll_start_status == 0)
            {
                goto sll_enter_start_value;
            }
            int status = sll_insertAtBeginning(&head, sll_start_value);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto sll_enter_start_value;
            }
            printf("\n");
            sll_printlist(head);
        }
        else if (sll_position_choice == 2)
        {
            int sll_pos_status;
            int sll_pos_value;
            int sll_pos_index;
            char sll_pos_prompt[128];

        sll_enter_pos_value:
            sll_pos_status = safe_input_int(&sll_pos_value,
                                            "enter the no. you want to insert, (between 1 "
                                            "and 100), enter '-1' to exit :- ",
                                            1, 100);

            if (sll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting sll demo.\n");
                delete_sll(head);
                return;
            }

            if (sll_pos_status == 0)
            {
                goto sll_enter_pos_value;
            }

        sll_enter_pos_index:
            snprintf(sll_pos_prompt, sizeof(sll_pos_prompt),
                     "enter the position (0 to %d), enter '-1' to exit :- ", sll_getLength(head));
            sll_pos_status = safe_input_int(&sll_pos_index, sll_pos_prompt, 0, sll_getLength(head));

            if (sll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting sll demo.\n");
                delete_sll(head);
                return;
            }

            if (sll_pos_status == 0)
            {
                goto sll_enter_pos_index;
            }

            int status = sll_insertAtPosition(&head, sll_pos_value, sll_pos_index);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto sll_enter_pos_value;
            }
            else if (status == -2)
            {
                printf("\ninvalid position. try again\n");
                goto sll_enter_pos_index;
            }
            printf("\n");
            sll_printlist(head);
        }

        sll_element_count--;
    }

    int rev_sll_status = sll_reverseList(&head);

    if (rev_sll_status == 1)
    {
        printf("\nReverse of the given list is :- ");
        sll_printlist(head);

        sll_reverseList(&head);

        printf("\nRestored original list :- ");
        sll_printlist(head);
    }
    else if (rev_sll_status == -1)
    {
        printf("\nSingle node list cannot be reversed.");
    }
    else if (rev_sll_status == -2)
    {
        printf("\nEmpty list cannot be reversed.");
    }

    // searching elements in sll
    while (1)
    {
        int sll_search_status;
        int sll_search_value;
        sll_search_status = safe_input_int(
            &sll_search_value,
            "\nenter the element to be searched, (between 1 and 100), enter '-1' to exit :- ", 1,
            100);
        if (sll_search_status == INPUT_EXIT_SIGNAL)
        {
            break;
        }
        if (sll_search_status == 0)
        {
            continue;
        }

        int index = sll_search(head, sll_search_value);
        printf("\nelement found at index :- %d", index);
    }

    // deleting elements in sll
    while (1)
    {
        int sll_delete_choice;
        int sll_delete_status;

    sll_delete_selection:
        sll_delete_status = safe_input_int(&sll_delete_choice,
                                           "\nenter '0' to delete by value"
                                           "\nenter '1' to delete at position"
                                           "\nenter '-1' to exit :- ",
                                           0, 1);

        if (sll_delete_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting sll demo.\n");
            delete_sll(head);
            return;
        }
        if (sll_delete_status == 0)
        {
            goto sll_delete_selection;
        }

        if (sll_delete_choice == 0)
        {
            int sll_delete_value;
            sll_delete_status = safe_input_int(
                &sll_delete_value,
                "\nenter the element to be deleted, (between 1 and 100), enter '-1' to exit :- ", 1,
                100);

            if (sll_delete_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting sll demo.\n");
                delete_sll(head);
                return;
            }
            if (sll_delete_status == 0)
            {
                continue;
            }

            int status = sll_deleteByValue(&head, sll_delete_value);
            if (status == -2)
            {
                printf("\nList is empty.Nothing to delete.");
            }
            else if (status == -1)
            {
                printf("\nelement not found");
            }
            else
            {
                printf("\nsll after deletion - ");
                sll_printlist(head);
            }
        }
        else if (sll_delete_choice == 1)
        {
            int sll_pos_delete_status;
            int sll_pos_delete_index;
            char sll_pos_delete_prompt[128];

        sll_delete_pos_input:
            snprintf(sll_pos_delete_prompt, sizeof(sll_pos_delete_prompt),
                     "enter the position to delete (0 to %d), enter '-1' to exit :- ",
                     sll_getLength(head) - 1);
            sll_pos_delete_status = safe_input_int(&sll_pos_delete_index, sll_pos_delete_prompt, 0,
                                                   sll_getLength(head) - 1);

            if (sll_pos_delete_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting sll demo.\n");
                delete_sll(head);
                return;
            }

            if (sll_pos_delete_status == 0)
            {
                goto sll_delete_pos_input;
            }

            int status = sll_deleteAtPosition(&head, sll_pos_delete_index);
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
                printf("\nsll after deletion - ");
                sll_printlist(head);
            }
        }
    }
}
