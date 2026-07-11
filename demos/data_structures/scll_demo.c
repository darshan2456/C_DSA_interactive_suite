#include "safe_input.h"
#include "scll.h"
#include <stdio.h>

void scll_demo(void)
{
    scll list;
    scll_init(&list);

    int scll_element_count;
    int scll_length_status;

start_scll:
    scll_length_status = safe_input_int(
        &scll_element_count,
        "enter how many elements you want to insert, (between 1 and 100), enter '-1' to exit :- ",
        1, 100);

    if (scll_length_status == INPUT_EXIT_SIGNAL)
    {
        printf("\nExiting scll demo\n");
        scll_destroy(&list);
        return;
    }

    if (scll_length_status == 0)
    {
        goto start_scll;
    }

    while (scll_element_count > 0)
    {
        int scll_position_choice;
        int scll_position_status;

    scll_position_selection:
        scll_position_status = safe_input_int(&scll_position_choice,
                                              "\nenter '0' for inserting at beginning"
                                              "\nenter '1' for inserting at end"
                                              "\nenter '2' for inserting at specific position"
                                              "\nenter '-1' to exit :- ",
                                              0, 2);

        if (scll_position_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting scll demo\n");
            scll_destroy(&list);
            return;
        }

        if (scll_position_status == 0)
        {
            goto scll_position_selection;
        }

        if (scll_position_choice == 1)
        {
            int scll_end_status;
            int scll_end_value;
        scll_enter_end_value:
            scll_end_status = safe_input_int(&scll_end_value,
                                             "enter the no. you want to insert at end, (between 1 "
                                             "and 100), enter '-1' to exit :- ",
                                             1, 100);

            if (scll_end_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting scll demo\n");
                scll_destroy(&list);
                return;
            }

            if (scll_end_status == 0)
            {
                goto scll_enter_end_value;
            }

            int status = scll_insertAtEnd(&list, scll_end_value);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto scll_enter_end_value;
            }
            printf("\n");
            scll_printlist(&list);
        }
        else if (scll_position_choice == 0)
        {
            int scll_start_status;
            int scll_start_value;

        scll_enter_start_value:
            scll_start_status = safe_input_int(&scll_start_value,
                                               "enter the no. you want to insert at beginning, "
                                               "(between 1 and 100), enter '-1' to exit :- ",
                                               1, 100);

            if (scll_start_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting scll demo\n");
                scll_destroy(&list);
                return;
            }

            if (scll_start_status == 0)
            {
                goto scll_enter_start_value;
            }
            int status = scll_insertAtBeginning(&list, scll_start_value);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto scll_enter_start_value;
            }
            printf("\n");
            scll_printlist(&list);
        }
        else if (scll_position_choice == 2)
        {
            int scll_pos_status;
            int scll_pos_value;
            int scll_pos_index;
            char scll_pos_prompt[128];

        scll_enter_pos_value:
            scll_pos_status = safe_input_int(&scll_pos_value,
                                             "enter the no. you want to insert, (between 1 "
                                             "and 100), enter '-1' to exit :- ",
                                             1, 100);

            if (scll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting scll demo\n");
                scll_destroy(&list);
                return;
            }

            if (scll_pos_status == 0)
            {
                goto scll_enter_pos_value;
            }

        scll_enter_pos_index:
            snprintf(scll_pos_prompt, sizeof(scll_pos_prompt),
                     "enter the position (0 to %d), enter '-1' to exit :- ", scll_getLength(&list));
            scll_pos_status =
                safe_input_int(&scll_pos_index, scll_pos_prompt, 0, scll_getLength(&list));

            if (scll_pos_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting scll demo\n");
                scll_destroy(&list);
                return;
            }

            if (scll_pos_status == 0)
            {
                goto scll_enter_pos_index;
            }

            int status = scll_insertAtPosition(&list, scll_pos_value, scll_pos_index);
            if (status == -1)
            {
                printf("\nmalloc allocation failure. try again\n");
                goto scll_enter_pos_value;
            }
            else if (status == -2)
            {
                printf("\ninvalid position. try again\n");
                goto scll_enter_pos_index;
            }
            printf("\n");
            scll_printlist(&list);
        }

        scll_element_count--;
    }

    // searching elements in scll
    while (1)
    {
        int scll_search_status;
        int scll_search_value;
        scll_search_status = safe_input_int(
            &scll_search_value,
            "\nenter the element to be searched, (between 1 and 100), enter '-1' to exit :- ", 1,
            100);
        if (scll_search_status == INPUT_EXIT_SIGNAL)
        {
            break;
        }
        if (scll_search_status == 0)
        {
            continue;
        }

        int index = scll_search(&list, scll_search_value);
        printf("\nelement found at index :- %d", index);
    }

    // deleting elements in scll
    while (1)
    {
        int scll_delete_choice;
        int scll_delete_status;

    scll_delete_selection:
        scll_delete_status = safe_input_int(&scll_delete_choice,
                                            "\nenter '0' to delete at beginning"
                                            "\nenter '1' to delete at end"
                                            "\nenter '2' to delete by value"
                                            "\nenter '3' to delete at position"
                                            "\nenter '-1' to exit :- ",
                                            0, 3);

        if (scll_delete_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting scll demo\n");
            scll_destroy(&list);
            return;
        }
        if (scll_delete_status == 0)
        {
            goto scll_delete_selection;
        }

        if (scll_delete_choice == 0)
        {
            int status = scll_deleteAtBeginning(&list);
            if (status == -1)
            {
                printf("\nList is empty\n");
            }
            else
            {
                printf("\nscll after deletion - ");
                scll_printlist(&list);
            }
        }
        else if (scll_delete_choice == 1)
        {
            int status = scll_deleteAtEnd(&list);
            if (status == -1)
            {
                printf("\nList is empty\n");
            }
            else
            {
                printf("\nscll after deletion - ");
                scll_printlist(&list);
            }
        }
        else if (scll_delete_choice == 2)
        {
            int scll_delete_value;
            scll_delete_status = safe_input_int(
                &scll_delete_value,
                "\nenter the element to be deleted, (between 1 and 100), enter '-1' to exit :- ", 1,
                100);

            if (scll_delete_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting scll demo\n");
                scll_destroy(&list);
                return;
            }
            if (scll_delete_status == 0)
            {
                continue;
            }

            int status = scll_deleteByValue(&list, scll_delete_value);
            if (status == -2)
            {
                printf("\nList is empty\n");
            }
            else if (status == -1)
            {
                printf("\nvalue not found\n");
            }
            else
            {
                printf("\nscll after deletion - ");
                scll_printlist(&list);
            }
        }
        else if (scll_delete_choice == 3)
        {
            int scll_pos_delete_status;
            int scll_pos_delete_index;
            char scll_pos_delete_prompt[128];

            if (scll_getLength(&list) == 0)
            {
                printf("\nList is empty\n");
                continue;
            }

        scll_delete_pos_input:
            snprintf(scll_pos_delete_prompt, sizeof(scll_pos_delete_prompt),
                     "enter the position to delete (0 to %d), enter '-1' to exit :- ",
                     scll_getLength(&list) - 1);
            scll_pos_delete_status = safe_input_int(&scll_pos_delete_index, scll_pos_delete_prompt,
                                                    0, scll_getLength(&list) - 1);

            if (scll_pos_delete_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting scll demo\n");
                scll_destroy(&list);
                return;
            }

            if (scll_pos_delete_status == 0)
            {
                goto scll_delete_pos_input;
            }

            int status = scll_deleteAtPosition(&list, scll_pos_delete_index);
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
                printf("\nscll after deletion - ");
                scll_printlist(&list);
            }
        }
    }

    scll_destroy(&list);
}
