#include "hash.h"
#include <assert.h>
#include <stdio.h>

void test_hash_range()
{
    for (int size = 1; size <= 100; size++)
    {
        for (int value = 0; value <= 1000; value++)
        {
            int h = hash_function(value, size);
            assert(h >= 0);
            assert(h < size);
        }
    }
    printf("Hash range test passed\n");
}

void test_hash_deterministic()
{
    for (int size = 1; size <= 50; size++)
    {
        for (int value = 0; value <= 100; value++)
        {
            int h1 = hash_function(value, size);
            int h2 = hash_function(value, size);
            assert(h1 == h2);
        }
    }
    printf("Hash determinism test passed\n");
}

void test_hash_basic_behavior()
{
    int size = 10;

    int h1 = hash_function(5, size);
    int h2 = hash_function(15, size);

    assert(h1 >= 0 && h1 < size);
    assert(h2 >= 0 && h2 < size);

    printf("Hash basic behavior test passed\n");
}

int main()
{
    test_hash_range();
    test_hash_deterministic();
    test_hash_basic_behavior();

    printf("All hash function tests passed\n");
    return 0;
}