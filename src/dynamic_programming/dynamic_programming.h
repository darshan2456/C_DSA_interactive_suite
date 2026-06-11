#ifndef DYNAMIC_PROGRAMMING_H
#define DYNAMIC_PROGRAMMING_H

void dp_demo(void);

void knapsack_demo(void);
int knapsack(int W, int wt[], int val[], int n);

void lcs_demo(void);
int lcs(char* X, char* Y, int m, int n);

void fibonacci_demo(void);
long long fibonacci_memoization(int n, long long memo[]);
long long fibonacci_tabulation(int n);

#endif // DYNAMIC_PROGRAMMING_H
