#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "stack.h"
#include <stddef.h>

typedef enum
{
    EXPR_SUCCESS = 0,
    EXPR_ERROR_EMPTY = -1,
    EXPR_ERROR_INVALID_CHAR = -2,
    EXPR_ERROR_UNMATCHED_PARENTHESES = -3,
    EXPR_ERROR_DIVIDE_BY_ZERO = -4,
    EXPR_ERROR_STACK_UNDERFLOW = -5,
    EXPR_ERROR_MALFORMED = -6,
    EXPR_ERROR_INVALID_OPERATOR = -7
} ExpressionResult;

void infix_to_postfix_demo(void);
void postfix_evaluation_demo(void);
void expression_evaluation_demo(void);
void parantheses_checker_demo(void);
void infix_to_prefix_demo(void);
int precedence(char ch);
int isOperator(char ch);

ExpressionResult check_parantheses(char* s);
ExpressionResult evaluate_postfix_expr(const char* postfix_expr, int* final_result);
ExpressionResult infix_to_postfix_convert(const char* infix_expr, char* postfix_expr,
                                          size_t max_size);
ExpressionResult infix_to_prefix_convert(const char* infix_expr, char* prefix_expr,
                                         size_t max_size);

#endif