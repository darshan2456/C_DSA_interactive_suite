#ifndef FUZZER_H
#define FUZZER_H

#include <stddef.h>
#include <stdint.h>

typedef enum
{
    FUZZER_OP_INSERT,
    FUZZER_OP_DELETE,
    FUZZER_OP_LOOKUP,
    FUZZER_OP_UPDATE,
    FUZZER_OP_TRAVERSE
} FuzzerOperation;

typedef struct
{
    FuzzerOperation op;
    int value;
    size_t index;
} FuzzerCommand;

typedef struct
{
    uint32_t seed;
    size_t num_operations;
    size_t num_failures;
    FuzzerCommand* commands;
} FuzzerResult;

typedef struct
{
    void* data_structure;
    int (*insert)(void* ds, int value);
    int (*delete)(void* ds, int value);
    int (*lookup)(void* ds, int value);
    void (*traverse)(void* ds);
} FuzzerInterface;

void fuzzer_demo(void);
FuzzerResult* run_fuzzer_test(FuzzerInterface* interface, uint32_t seed, size_t num_ops);
void print_fuzzer_result(FuzzerResult* result);
void free_fuzzer_result(FuzzerResult* result);

void fuzzer_bst_demo(void);
void fuzzer_graph_demo(void);

#endif
