#include "display_header.h"
#include "expression.h"
#include "safe_input.h"
#include <stdio.h>

void expression_evaluation_demo(void)
{
    int expr_eval_choice;
    int expr_eval_status;
    while (1)
    {
        display_header("Expression Evaluation");

        expr_eval_status = safe_input_int(&expr_eval_choice,
                                          "\nenter 1 for infix to postfix conversion"
                                          "\nenter 2 for postfix evaluation"
                                          "\nenter 3 parantheses checker"
                                          "\nenter 4 for infix to prefix conversion"
                                          "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                          1, 4);

        if (expr_eval_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting expression_evaluation_demo.....\n");
            return;
        };

        if (expr_eval_status == 0)
            continue;

        if (expr_eval_choice == 1)
        {
            display_header("Infix to Postfix");
            infix_to_postfix_demo();
            continue;
        }
        else if (expr_eval_choice == 2)
        {
            display_header("Postfix Evaluation");
            postfix_evaluation_demo();
            continue;
        }
        else if (expr_eval_choice == 3)
        {
            display_header("Parentheses Checker");
            parantheses_checker_demo();
            continue;
        }
        else if (expr_eval_choice == 4)
        {
            display_header("Infix to Prefix");
            infix_to_prefix_demo();
            continue;
        }
    }
}