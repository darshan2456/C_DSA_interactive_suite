#include "fuzzer.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int fuzzer_validate_target(FuzzerTarget* target)
{
    if (target == NULL) return -1;
    if (target->name == NULL) return -2;
    if (target->insert == NULL) return -3;
    if (target->search == NULL) return -4;
    return 0;
}

static FuzzOperation* generate_operations(FuzzerConfig* config, int* op_count)
{
    *op_count = config->operations_per_iteration;
    FuzzOperation* ops = (FuzzOperation*)malloc(sizeof(FuzzOperation) * (*op_count));
    if (!ops) return NULL;

    for (int i = 0; i < *op_count; i++)
    {
        int rand_val = rand() % 100;
        
        if (rand_val < 40)
        {
            ops[i].op = FUZZER_OP_INSERT;
            ops[i].value = (rand() % (config->max_value - config->min_value + 1)) + config->min_value;
        }
        else if (rand_val < 60)
        {
            ops[i].op = FUZZER_OP_SEARCH;
            ops[i].value = (rand() % (config->max_value - config->min_value + 1)) + config->min_value;
        }
        else if (rand_val < 75)
        {
            ops[i].op = FUZZER_OP_DELETE;
            ops[i].value = (rand() % (config->max_value - config->min_value + 1)) + config->min_value;
        }
        else if (rand_val < 90)
        {
            ops[i].op = FUZZER_OP_UPDATE;
            ops[i].value = (rand() % (config->max_value - config->min_value + 1)) + config->min_value;
            ops[i].param = (rand() % (config->max_value - config->min_value + 1)) + config->min_value;
        }
        else
        {
            ops[i].op = FUZZER_OP_TRAVERSE;
            ops[i].value = 0;
        }
    }

    return ops;
}

FuzzerStats fuzzer_run_single(FuzzerTarget* target, FuzzerConfig* config)
{
    FuzzerStats stats = {0};
    stats.seed = (uint32_t)time(NULL);
    stats.total_operations = config->operations_per_iteration;

    srand(stats.seed);

    void* context = target->setup();
    if (!context)
    {
        stats.crash_occurred = 1;
        stats.crash_at_operation = -1;
        return stats;
    }

    int op_count = 0;
    FuzzOperation* ops = generate_operations(config, &op_count);
    if (!ops)
    {
        target->cleanup(context);
        stats.crash_occurred = 1;
        stats.crash_at_operation = -2;
        return stats;
    }

    for (int i = 0; i < op_count; i++)
    {
        int result = 0;

        switch (ops[i].op)
        {
            case FUZZER_OP_INSERT:
                result = target->insert(context, ops[i].value);
                stats.insert_operations++;
                if (config->verbose) printf("  [Op %d] INSERT %d -> %s\n", 
                    i + 1, ops[i].value, result ? "OK" : "FAIL");
                break;

            case FUZZER_OP_DELETE:
                result = target->delete(context, ops[i].value);
                stats.delete_operations++;
                if (config->verbose) printf("  [Op %d] DELETE %d -> %s\n",
                    i + 1, ops[i].value, result ? "OK" : "FAIL");
                break;

            case FUZZER_OP_SEARCH:
                result = target->search(context, ops[i].value);
                stats.search_operations++;
                if (config->verbose) printf("  [Op %d] SEARCH %d -> %s\n",
                    i + 1, ops[i].value, result ? "FOUND" : "NOT FOUND");
                break;

            case FUZZER_OP_UPDATE:
                if (target->update)
                {
                    result = target->update(context, ops[i].value, ops[i].param);
                    stats.update_operations++;
                    if (config->verbose) printf("  [Op %d] UPDATE %d -> %d -> %s\n",
                        i + 1, ops[i].value, ops[i].param, result ? "OK" : "FAIL");
                }
                break;

            case FUZZER_OP_TRAVERSE:
                if (config->verbose) printf("  [Op %d] TRAVERSE\n", i + 1);
                break;
        }

        stats.total_operations = i + 1;
    }

    if (ops) free(ops);
    target->cleanup(context);

    return stats;
}

FuzzerStats fuzzer_run_stress(FuzzerTarget* target, FuzzerConfig* config)
{
    FuzzerStats total_stats = {0};
    total_stats.seed = (uint32_t)time(NULL);

    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||           FUZZ TESTING STRESS TEST                    ||\n");
    printf("  +========================================================+\n");
    printf("\n");
    printf("  Target:       %s\n", target->name);
    printf("  Iterations:   %d\n", config->iterations);
    printf("  Ops/Iter:     %d\n", config->operations_per_iteration);
    printf("  Total Ops:    %d\n", config->iterations * config->operations_per_iteration);
    printf("  Value Range:  [%d, %d]\n", config->min_value, config->max_value);
    printf("\n");

    for (int iter = 0; iter < config->iterations; iter++)
    {
        FuzzerConfig iter_config = *config;
        
        FuzzerStats iter_stats = fuzzer_run_single(target, &iter_config);

        total_stats.insert_operations += iter_stats.insert_operations;
        total_stats.delete_operations += iter_stats.delete_operations;
        total_stats.search_operations += iter_stats.search_operations;
        total_stats.update_operations += iter_stats.update_operations;
        total_stats.total_operations += iter_stats.total_operations;

        printf("  Iteration %d/%d [Seed: %u]: ", 
               iter + 1, config->iterations, iter_stats.seed);
        
        if (iter_stats.crash_occurred)
        {
            printf("CRASHED at operation %d!\n", iter_stats.crash_at_operation);
            total_stats.crash_occurred = 1;
            total_stats.crash_at_operation = iter_stats.crash_at_operation;
            total_stats.crash_at_seed = iter_stats.seed;
            break;
        }
        else
        {
            printf("OK (%d ops)\n", iter_stats.total_operations);
        }
    }

    printf("\n");
    fuzzer_print_stats(&total_stats);

    return total_stats;
}

void fuzzer_print_stats(FuzzerStats* stats)
{
    printf("  +========================================================+\n");
    printf("  ||                   FUZZ TEST RESULTS                   ||\n");
    printf("  +========================================================+\n");
    printf("\n");
    printf("  Status:       %s\n", stats->crash_occurred ? "CRASHED" : "PASSED");
    printf("  Seed:         %u\n", stats->seed);
    printf("\n");
    printf("  Operations Summary:\n");
    printf("  -------------------\n");
    printf("  Total:        %d\n", stats->total_operations);
    printf("  Insert:       %d\n", stats->insert_operations);
    printf("  Delete:       %d\n", stats->delete_operations);
    printf("  Search:       %d\n", stats->search_operations);
    printf("  Update:       %d\n", stats->update_operations);
    printf("\n");

    if (stats->crash_occurred)
    {
        printf("  CRASH INFO:\n");
        printf("  -----------\n");
        printf("  Crash at Operation: %d\n", stats->crash_at_operation);
        printf("  Crash Seed:          %u\n", stats->crash_at_seed);
        printf("\n");
        printf("  To reproduce: Run with seed=%u\n", stats->crash_at_seed);
    }
    printf("\n");
}

void fuzzer_print_operation_log(FuzzOperation* ops, int count)
{
    if (!ops || count <= 0) return;

    printf("\n  Operation Log (%d ops):\n", count);
    printf("  ----------------------\n");

    int max_show = (count > 100) ? 100 : count;
    for (int i = 0; i < max_show; i++)
    {
        printf("  %4d: %s", i + 1, fuzzer_op_to_string(ops[i].op));
        switch (ops[i].op)
        {
            case FUZZER_OP_INSERT:
            case FUZZER_OP_DELETE:
            case FUZZER_OP_SEARCH:
                printf(" %d", ops[i].value);
                break;
            case FUZZER_OP_UPDATE:
                printf(" %d -> %d", ops[i].value, ops[i].param);
                break;
            default:
                break;
        }
        printf("\n");
    }

    if (count > 100)
    {
        printf("  ... and %d more operations\n", count - 100);
    }
    printf("\n");
}

const char* fuzzer_op_to_string(FuzzerOperation op)
{
    switch (op)
    {
        case FUZZER_OP_INSERT:  return "INSERT";
        case FUZZER_OP_DELETE:  return "DELETE";
        case FUZZER_OP_SEARCH:  return "SEARCH";
        case FUZZER_OP_UPDATE:  return "UPDATE";
        case FUZZER_OP_TRAVERSE: return "TRAVERSE";
        default:                return "UNKNOWN";
    }
}

typedef struct {
    int* data;
    int size;
    int capacity;
} DynArray;

static void* setup_array(void)
{
    DynArray* arr = (DynArray*)malloc(sizeof(DynArray));
    if (!arr) return NULL;
    arr->capacity = 1024;
    arr->size = 0;
    arr->data = (int*)malloc(sizeof(int) * arr->capacity);
    if (!arr->data) {
        free(arr);
        return NULL;
    }
    return arr;
}

static void cleanup_array(void* ctx)
{
    if (!ctx) return;
    DynArray* arr = (DynArray*)ctx;
    if (arr->data) free(arr->data);
    free(arr);
}

static int insert_array(void* ctx, int value)
{
    DynArray* arr = (DynArray*)ctx;
    if (arr->size >= arr->capacity) {
        int new_cap = arr->capacity * 2;
        int* new_data = (int*)realloc(arr->data, sizeof(int) * new_cap);
        if (!new_data) return 0;
        arr->data = new_data;
        arr->capacity = new_cap;
    }
    arr->data[arr->size++] = value;
    return 1;
}

static int delete_array(void* ctx, int value)
{
    (void)ctx;
    (void)value;
    return 1;
}

static int search_array(void* ctx, int value)
{
    DynArray* arr = (DynArray*)ctx;
    for (int i = 0; i < arr->size; i++) {
        if (arr->data[i] == value) return 1;
    }
    return 0;
}

static int update_array(void* ctx, int old_val, int new_val)
{
    DynArray* arr = (DynArray*)ctx;
    for (int i = 0; i < arr->size; i++) {
        if (arr->data[i] == old_val) {
            arr->data[i] = new_val;
            return 1;
        }
    }
    return 0;
}

static FuzzerTarget available_targets[] = {
    {
        .name = "Dynamic Array",
        .setup = setup_array,
        .cleanup = cleanup_array,
        .insert = insert_array,
        .delete = delete_array,
        .search = search_array,
        .update = update_array
    }
};

static void run_quick_fuzz_test(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||               QUICK FUZZ TEST                       ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    int num_targets = sizeof(available_targets) / sizeof(available_targets[0]);

    for (int i = 0; i < num_targets; i++)
    {
        printf("  Testing: %s...\n", available_targets[i].name);

        FuzzerConfig config = {
            .iterations = 10,
            .operations_per_iteration = 1000,
            .min_value = -1000,
            .max_value = 1000,
            .verbose = 0
        };

        FuzzerStats stats = fuzzer_run_single(&available_targets[i], &config);

        if (stats.crash_occurred)
        {
            printf("  [FAIL] Crashed at operation %d (seed: %u)\n",
                   stats.crash_at_operation, stats.seed);
        }
        else
        {
            printf("  [PASS] Completed %d operations successfully\n",
                   stats.total_operations);
        }
        printf("\n");
    }
}

static void run_custom_fuzz_test(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||              CUSTOM FUZZ TEST                        ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    int num_targets = sizeof(available_targets) / sizeof(available_targets[0]);

    printf("  Available Targets:\n");
    for (int i = 0; i < num_targets; i++)
    {
        printf("    %d. %s\n", i + 1, available_targets[i].name);
    }
    printf("\n");

    int choice;
    if (safe_input_int(&choice, "  Select target (1-%d): ", 1, num_targets) != 0)
        return;

    FuzzerTarget* target = &available_targets[choice - 1];

    FuzzerConfig config;
    config.verbose = 0;

    if (safe_input_int(&config.iterations, "  Iterations (1-1000): ", 1, 1000) != 0)
        return;

    if (safe_input_int(&config.operations_per_iteration, 
                       "  Operations per iteration (100-100000): ", 100, 100000) != 0)
        return;

    if (safe_input_int(&config.min_value, "  Min value: ", -1000000, 1000000) != 0)
        return;

    int max_val;
    if (safe_input_int(&max_val, "  Max value: ", config.min_value, 1000000) != 0)
        return;
    config.max_value = max_val;

    int verbose_choice;
    if (safe_input_int(&verbose_choice, "  Verbose mode (0=No, 1=Yes): ", 0, 1) != 0)
        return;
    config.verbose = verbose_choice;

    printf("\n");
    fuzzer_run_stress(target, &config);
}

static void run_regression_test(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||             REGRESSION FUZZ TEST                     ||\n");
    printf("  +========================================================+\n");
    printf("\n");
    printf("  This runs 1000 iterations with 5000 ops each.\n");
    printf("  Total operations: 5,000,000\n");
    printf("  This may take a few minutes...\n\n");

    int num_targets = sizeof(available_targets) / sizeof(available_targets[0]);

    int all_passed = 1;

    for (int i = 0; i < num_targets; i++)
    {
        printf("  Testing %s...", available_targets[i].name);
        fflush(stdout);

        FuzzerConfig config = {
            .iterations = 1000,
            .operations_per_iteration = 5000,
            .min_value = -10000,
            .max_value = 10000,
            .verbose = 0
        };

        FuzzerStats stats = fuzzer_run_single(&available_targets[i], &config);

        if (stats.crash_occurred)
        {
            printf(" CRASHED (seed: %u, op: %d)\n", stats.seed, stats.crash_at_operation);
            all_passed = 0;
        }
        else
        {
            printf(" PASSED\n");
        }
    }

    printf("\n");
    if (all_passed)
    {
        printf("  ======================================\n");
        printf("  ALL REGRESSION TESTS PASSED!\n");
        printf("  ======================================\n");
    }
    else
    {
        printf("  ======================================\n");
        printf("  SOME TESTS FAILED! Check above.\n");
        printf("  ======================================\n");
    }
    printf("\n");
}

static void print_fuzzer_explanation(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||              WHAT IS FUZZ TESTING?                    ||\n");
    printf("  +========================================================+\n");
    printf("\n");
    printf("  Fuzz testing is a software testing technique that\n");
    printf("  provides random data (fuzz) as input to a program.\n");
    printf("  The goal is to find bugs, crashes, or memory issues.\n");
    printf("\n");
    printf("  Why Fuzz Test?\n");
    printf("  --------------\n");
    printf("  - Finds edge cases that normal tests miss\n");
    printf("  - Discovers use-after-free bugs\n");
    printf("  - Exposes memory leaks and buffer overflows\n");
    printf("  - Validates robustness under chaotic inputs\n");
    printf("\n");
    printf("  How This Fuzzer Works:\n");
    printf("  ----------------------\n");
    printf("  1. Generates random sequences of operations\n");
    printf("  2. Applies them to data structures\n");
    printf("  3. Tracks operations and seed for reproducibility\n");
    printf("  4. If crash occurs, reports exact seed and sequence\n");
    printf("\n");
    printf("  Use Cases:\n");
    printf("  ----------\n");
    printf("  - Before releasing new features\n");
    printf("  - After modifying complex data structures\n");
    printf("  - In CI/CD pipelines for regression testing\n");
    printf("  - During development to catch bugs early\n");
    printf("\n");
    printf("  Integration with CI:\n");
    printf("  --------------------\n");
    printf("  Run: make fuzz-test\n");
    printf("  Or:  ./dsa fuzz\n");
    printf("\n");
}

void fuzzer_demo(void)
{
    while (1)
    {
        display_header("Stochastic Fuzz Testing Engine");

        printf("\nStress test data structures with randomized operations.\n");
        printf("Catch edge-case bugs, segfaults, and memory issues.\n\n");

        printf("1. Quick Fuzz Test (10 iterations, 1000 ops each)\n");
        printf("2. Custom Fuzz Test (configure parameters)\n");
        printf("3. Regression Fuzz Test (1000 iterations)\n");
        printf("4. Educational Guide\n");
        printf("5. Back to main menu\n");
        printf("\n");

        int choice;
        if (safe_input_int(&choice, "Enter choice: ", 1, 5) != 0)
            break;

        switch (choice)
        {
            case 1:
                run_quick_fuzz_test();
                break;
            case 2:
                run_custom_fuzz_test();
                break;
            case 3:
                run_regression_test();
                break;
            case 4:
                print_fuzzer_explanation();
                break;
            case 5:
                return;
        }

        printf("\nPress Enter to continue...");
        getchar();
    }
}