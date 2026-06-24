#include <assert.h>
#include <stdio.h>

int matrix_chain_order(int p[], int n);

void test_mcm_trivial()
{
    int p[] = {10};
    int cost = matrix_chain_order(p, 1);
    assert(cost == 0);
    printf("test_mcm_trivial passed\n");
}

void test_mcm_single_multiplication()
{
    int p[] = {10, 20, 30};
    int cost = matrix_chain_order(p, 3);
    assert(cost == 6000); // 10 * 20 * 30
    printf("test_mcm_single_multiplication passed\n");
}

void test_mcm_four_matrices()
{
    int p[] = {40, 20, 30, 10, 30};
    int cost = matrix_chain_order(p, 5);
    assert(cost == 26000);
    printf("test_mcm_four_matrices passed\n");
}

void test_mcm_another_four_matrices()
{
    int p[] = {10, 20, 30, 40, 30};
    int cost = matrix_chain_order(p, 5);
    assert(cost == 30000);
    printf("test_mcm_another_four_matrices passed\n");
}

int main()
{
    test_mcm_trivial();
    test_mcm_single_multiplication();
    test_mcm_four_matrices();
    test_mcm_another_four_matrices();
    printf("All Matrix Chain Multiplication tests passed successfully!\n");
    return 0;
}
