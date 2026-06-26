#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "dynamic_programming.h"
#include "mock_printf.h"

// Define overrides to suppress history logging prints
#define printf mock_printf
#include "../../src/dynamic_programming/fibonacci.c"
#undef printf

void test_fibonacci_base_cases()
{
    long long memo_0[1] = {-1};
    assert(fibonacci_recursive(0, memo_0) == 0);
    assert(fibonacci_iterative(0) == 0);

    long long memo_1[2] = {-1, -1};
    assert(fibonacci_recursive(1, memo_1) == 1);
    assert(fibonacci_iterative(1) == 1);
}

void test_fibonacci_recursive_memoized()
{
    int n = 10;
    long long* memo = malloc((n + 1) * sizeof(long long));
    for (int i = 0; i <= n; i++) memo[i] = -1;
    
    assert(fibonacci_recursive(n, memo) == 55);
    free(memo);

    n = 45;
    memo = malloc((n + 1) * sizeof(long long));
    for (int i = 0; i <= n; i++) memo[i] = -1;
    assert(fibonacci_recursive(n, memo) == 1134903170LL);
    free(memo);
}

void test_fibonacci_tabulation()
{
    assert(fibonacci_iterative(10) == 55);
    assert(fibonacci_iterative(45) == 1134903170LL);
    assert(fibonacci_iterative(90) == 2880067194370816120LL);
}

int main()
{
    test_fibonacci_base_cases();
    test_fibonacci_recursive_memoized();
    test_fibonacci_tabulation();

    fprintf(stdout, "All Fibonacci tests passed successfully!\n");
    return 0;
}
