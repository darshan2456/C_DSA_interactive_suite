#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "stack.h"
#include <stddef.h>

#define EXPR_SUCCESS 0
#define EXPR_ERROR_EMPTY -1
#define EXPR_ERROR_INVALID_CHAR -2
#define EXPR_ERROR_UNMATCHED_PARENTHESES -3
#define EXPR_ERROR_DIVIDE_BY_ZERO -4
#define EXPR_ERROR_STACK_UNDERFLOW -5
#define EXPR_ERROR_MALFORMED -6
#define EXPR_ERROR_INVALID_OPERATOR -7

void infix_to_postfix_demo(void);
void postfix_evaluation_demo(void);
void expression_evaluation_demo(void);
void parantheses_checker_demo(void);
void infix_to_prefix_demo(void);
int precedence(char ch);
int isOperator(char ch);

int check_parantheses(char* s);
int evaluate_postfix_expr(const char* postfix_expr, int* final_result);
int infix_to_postfix_convert(const char* infix_expr, char* postfix_expr, size_t max_size);
int infix_to_prefix_convert(const char* infix_expr, char* prefix_expr, size_t max_size);

#endif