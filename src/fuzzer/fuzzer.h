#ifndef FUZZER_H
#define FUZZER_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t seed;
    int total_operations;
    int insert_operations;
    int delete_operations;
    int search_operations;
    int update_operations;
    int crash_occurred;
    int crash_at_operation;
    int crash_at_seed;
} FuzzerStats;

typedef enum {
    FUZZER_OP_INSERT,
    FUZZER_OP_DELETE,
    FUZZER_OP_SEARCH,
    FUZZER_OP_UPDATE,
    FUZZER_OP_TRAVERSE
} FuzzerOperation;

typedef struct {
    FuzzerOperation op;
    int value;
    int param;
} FuzzOperation;

typedef void* (*FuzzerSetupFn)(void);
typedef void (*FuzzerCleanupFn)(void*);
typedef int (*FuzzerInsertFn)(void*, int);
typedef int (*FuzzerDeleteFn)(void*, int);
typedef int (*FuzzerSearchFn)(void*, int);
typedef int (*FuzzerUpdateFn)(void*, int, int);

typedef struct {
    const char* name;
    FuzzerSetupFn setup;
    FuzzerCleanupFn cleanup;
    FuzzerInsertFn insert;
    FuzzerDeleteFn delete;
    FuzzerSearchFn search;
    FuzzerUpdateFn update;
} FuzzerTarget;

typedef struct {
    int iterations;
    int operations_per_iteration;
    int min_value;
    int max_value;
    int verbose;
} FuzzerConfig;

int fuzzer_validate_target(FuzzerTarget* target);

FuzzerStats fuzzer_run_single(FuzzerTarget* target, FuzzerConfig* config);

FuzzerStats fuzzer_run_stress(FuzzerTarget* target, FuzzerConfig* config);

void fuzzer_print_stats(FuzzerStats* stats);

void fuzzer_print_operation_log(FuzzOperation* ops, int count);

const char* fuzzer_op_to_string(FuzzerOperation op);

void fuzzer_demo(void);

#endif // FUZZER_H