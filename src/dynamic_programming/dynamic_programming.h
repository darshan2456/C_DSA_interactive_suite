#ifndef DYNAMIC_PROGRAMMING_H
#define DYNAMIC_PROGRAMMING_H

void dynamic_programming_demo(void);

void knapsack_demo(void);
int knapsack(int W, int wt[], int val[], int n);

void lcs_demo(void);
int lcs(char* X, char* Y, int m, int n);

void fibonacci_demo(void);
long long fibonacci_recursive(int n, long long memo[]);
long long fibonacci_iterative(int n);

int matrix_chain_order(int p[], int n);
void mcm_demo(void);

#endif // DYNAMIC_PROGRAMMING_H
