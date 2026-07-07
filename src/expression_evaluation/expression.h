#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "stack.h"

void infix_to_postfix_demo(void);
void postfix_evaluation_demo(void);
void expression_evaluation_demo(void);
void parantheses_checker_demo(void);
void infix_to_prefix_demo(void);
int precedence(char ch);
int isOperator(char ch);

#endif