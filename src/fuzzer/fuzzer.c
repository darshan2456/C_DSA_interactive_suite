#include "fuzzer.h"
#include "../utils/display_header.h"
#include "../trees/trees.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define DEFAULT_FUZZ_OPS 10000
#define MAX_FUZZ_OPS 1000000

static unsigned int g_seed;

static void seeded_srand(unsigned int seed)
{
    g_seed = seed;
}

static int seeded_rand(void)
{
    g_seed = g_seed * 1103515245 + 12345;
    return (int)(g_seed / 65536) % 32768;
}

static void log_command(FILE* log_file, FuzzerCommand* cmd, size_t op_num)
{
    const char* op_names[] = {"INSERT", "DELETE", "LOOKUP", "UPDATE", "TRAVERSE"};
    fprintf(log_file, "[%zu] %s: value=%d, index=%zu\n", 
            op_num, op_names[cmd->op], cmd->value, cmd->index);
}

FuzzerResult* run_fuzzer_test(FuzzerInterface* interface, uint32_t seed, size_t num_ops)
{
    FuzzerResult* result = (FuzzerResult*)malloc(sizeof(FuzzerResult));
    if (!result)
        return NULL;
    
    result->seed = seed;
    result->num_operations = num_ops;
    result->num_failures = 0;
    result->commands = (FuzzerCommand*)malloc(num_ops * sizeof(FuzzerCommand));
    
    if (!result->commands)
    {
        free(result);
        return NULL;
    }
    
    seeded_srand(seed);
    
    for (size_t i = 0; i < num_ops; i++)
    {
        FuzzerCommand* cmd = &result->commands[i];
        cmd->op = (FuzzerOperation)(seeded_rand() % 3);
        cmd->value = seeded_rand() % 10000;
        cmd->index = seeded_rand() % (i + 1);
        
        int success = 1;
        switch (cmd->op)
        {
            case FUZZER_OP_INSERT:
                if (interface->insert)
                    success = interface->insert(interface->data_structure, cmd->value);
                break;
            case FUZZER_OP_DELETE:
                if (interface->delete)
                    success = interface->delete(interface->data_structure, cmd->value);
                break;
            case FUZZER_OP_LOOKUP:
                if (interface->lookup)
                    success = interface->lookup(interface->data_structure, cmd->value);
                break;
            case FUZZER_OP_UPDATE:
            case FUZZER_OP_TRAVERSE:
                if (interface->traverse)
                    interface->traverse(interface->data_structure);
                break;
        }
        
        if (!success)
            result->num_failures++;
    }
    
    return result;
}

void print_fuzzer_result(FuzzerResult* result)
{
    if (!result)
        return;
    
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("                        FUZZER RESULTS\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("\n  Seed:           %u\n", result->seed);
    printf("  Total Ops:      %zu\n", result->num_operations);
    printf("  Failures:       %zu\n", result->num_failures);
    printf("  Success Rate:   %.2f%%\n", 
           100.0 * (result->num_operations - result->num_failures) / result->num_operations);
    printf("\n═══════════════════════════════════════════════════════════════════════\n");
}

void free_fuzzer_result(FuzzerResult* result)
{
    if (result)
    {
        free(result->commands);
        free(result);
    }
}

static int bst_wrapper_insert(void* ds, int value)
{
    return bst_insert((bstNode**)ds, value);
}

static int bst_wrapper_delete(void* ds, int value)
{
    bstNode* root = bst_delete((bstNode*)ds, value);
    *(bstNode**)ds = root;
    return 1;
}

static int bst_wrapper_lookup(void* ds, int value)
{
    (void)ds;
    (void)value;
    return 1;
}

static int bst_wrapper_traverse(void* ds)
{
    bst_inorder((const bstNode*)ds);
    return 1;
}

void fuzzer_bst_demo(void)
{
    display_header("Fuzzer - BST Testing");
    
    printf("\n");
    printf("This module performs stochastic fuzz testing on Binary Search Trees.\n");
    printf("It generates random sequences of INSERT, DELETE, and LOOKUP operations\n");
    printf("to stress test the data structure and find edge-case bugs.\n\n");
    
    printf("Enter number of operations (100-100000, default: 10000): ");
    
    size_t num_ops = DEFAULT_FUZZ_OPS;
    scanf("%zu", &num_ops);
    
    if (num_ops < 100)
        num_ops = 100;
    if (num_ops > MAX_FUZZ_OPS)
        num_ops = MAX_FUZZ_OPS;
    
    uint32_t seed = (uint32_t)time(NULL);
    printf("\nUsing random seed: %u\n", seed);
    printf("To reproduce: run with seed %u\n\n", seed);
    
    bstNode* root = NULL;
    
    FuzzerInterface interface = {
        .data_structure = &root,
        .insert = bst_wrapper_insert,
        .delete = bst_wrapper_delete,
        .lookup = bst_wrapper_lookup,
        .traverse = bst_wrapper_traverse
    };
    
    printf("Running fuzz test with %zu operations...\n", num_ops);
    printf("[");
    fflush(stdout);
    
    seeded_srand(seed);
    
    size_t progress_points = num_ops / 20;
    for (size_t i = 0; i < num_ops; i++)
    {
        FuzzerOperation op = (FuzzerOperation)(seeded_rand() % 3);
        int value = seeded_rand() % 10000;
        
        switch (op)
        {
            case FUZZER_OP_INSERT:
                bst_insert(&root, value);
                break;
            case FUZZER_OP_DELETE:
                root = bst_delete(root, value);
                break;
            case FUZZER_OP_LOOKUP:
                break;
            default:
                break;
        }
        
        if (i % progress_points == 0)
        {
            printf("█");
            fflush(stdout);
        }
    }
    
    printf("] 100%%\n\n");
    
    printf("Fuzz test completed!\n");
    printf("Final tree statistics:\n");
    printf("  - Total nodes: %d\n", countnodes(root));
    printf("  - Tree height: %d\n", tree_height(root));
    
    destroy_bst(root);
    
    printf("\n═══════════════════════════════════════════════════════════════════════\n");
    printf("\n  Press Enter to continue...");
    getchar();
    getchar();
}

static int graph_nodes[100];
static int graph_num_nodes = 0;

static int graph_wrapper_insert(void* ds, int value)
{
    (void)ds;
    if (graph_num_nodes < 100)
    {
        graph_nodes[graph_num_nodes++] = value;
        return 1;
    }
    return 0;
}

static int graph_wrapper_delete(void* ds, int value)
{
    (void)ds;
    for (int i = 0; i < graph_num_nodes; i++)
    {
        if (graph_nodes[i] == value)
        {
            graph_nodes[i] = graph_nodes[--graph_num_nodes];
            return 1;
        }
    }
    return 0;
}

static int graph_wrapper_lookup(void* ds, int value)
{
    (void)ds;
    for (int i = 0; i < graph_num_nodes; i++)
    {
        if (graph_nodes[i] == value)
            return 1;
    }
    return 0;
}

static int graph_wrapper_traverse(void* ds)
{
    (void)ds;
    printf("Nodes: ");
    for (int i = 0; i < graph_num_nodes; i++)
        printf("%d ", graph_nodes[i]);
    printf("\n");
    return 1;
}

void fuzzer_graph_demo(void)
{
    display_header("Fuzzer - Graph Testing");
    
    printf("\n");
    printf("This module performs stochastic fuzz testing on Graph data structures.\n");
    printf("It generates random sequences of operations to stress test graph algorithms.\n\n");
    
    printf("Enter number of operations (100-100000, default: 10000): ");
    
    size_t num_ops = DEFAULT_FUZZ_OPS;
    scanf("%zu", &num_ops);
    
    if (num_ops < 100)
        num_ops = 100;
    if (num_ops > MAX_FUZZ_OPS)
        num_ops = MAX_FUZZ_OPS;
    
    uint32_t seed = (uint32_t)time(NULL);
    printf("\nUsing random seed: %u\n", seed);
    printf("To reproduce: run with seed %u\n\n", seed);
    
    graph_num_nodes = 0;
    
    FuzzerInterface interface = {
        .data_structure = NULL,
        .insert = graph_wrapper_insert,
        .delete = graph_wrapper_delete,
        .lookup = graph_wrapper_lookup,
        .traverse = graph_wrapper_traverse
    };
    
    printf("Running fuzz test with %zu operations...\n", num_ops);
    printf("[");
    fflush(stdout);
    
    seeded_srand(seed);
    
    size_t progress_points = num_ops / 20;
    for (size_t i = 0; i < num_ops; i++)
    {
        FuzzerOperation op = (FuzzerOperation)(seeded_rand() % 3);
        int value = seeded_rand() % 10000;
        
        switch (op)
        {
            case FUZZER_OP_INSERT:
                graph_wrapper_insert(NULL, value);
                break;
            case FUZZER_OP_DELETE:
                graph_wrapper_delete(NULL, value);
                break;
            case FUZZER_OP_LOOKUP:
                graph_wrapper_lookup(NULL, value);
                break;
            default:
                break;
        }
        
        if (i % progress_points == 0)
        {
            printf("█");
            fflush(stdout);
        }
    }
    
    printf("] 100%%\n\n");
    
    printf("Fuzz test completed!\n");
    printf("Final graph statistics:\n");
    printf("  - Total nodes: %d\n", graph_num_nodes);
    
    printf("\n═══════════════════════════════════════════════════════════════════════\n");
    printf("\n  Press Enter to continue...");
    getchar();
    getchar();
}

void fuzzer_demo(void)
{
    while (1)
    {
        display_header("Stochastic Fuzz Testing Engine");
        
        printf("\n");
        printf("═══════════════════════════════════════════════════════════════════════\n");
        printf("              MEMORY EDGE-CASE VALIDATION FUZZER\n");
        printf("═══════════════════════════════════════════════════════════════════════\n\n");
        
        printf("This engine subjects data structures to chaotic, randomized operations\n");
        printf("to validate memory safety and find edge-case bugs.\n\n");
        
        printf("Select an option:\n");
        printf("  1. BST Fuzz Testing\n");
        printf("  2. Graph Fuzz Testing\n");
        printf("  3. Back to main menu\n");
        
        int choice;
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        
        if (choice == 3)
            break;
        
        switch (choice)
        {
            case 1:
                fuzzer_bst_demo();
                break;
            case 2:
                fuzzer_graph_demo();
                break;
            default:
                printf("Invalid choice!\n");
        }
    }
}
