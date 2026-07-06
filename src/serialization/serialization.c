#include "serialization.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERIAL_FORMAT_VERSION 1
#define SERIAL_FORMAT_NAME "C_DSA_Suite_Serialization"

SerialContext* serial_create_context(void)
{
    SerialContext* ctx = (SerialContext*)malloc(sizeof(SerialContext));
    if (!ctx) return NULL;
    
    ctx->version = SERIAL_FORMAT_VERSION;
    ctx->format_name = SERIAL_FORMAT_NAME;
    ctx->last_error = SERIAL_OK;
    ctx->error_msg[0] = '\0';
    
    return ctx;
}

void serial_destroy_context(SerialContext* ctx)
{
    free(ctx);
}

void serial_set_error(SerialContext* ctx, SerialError err, const char* msg)
{
    if (!ctx) return;
    ctx->last_error = err;
    if (msg)
    {
        strncpy(ctx->error_msg, msg, sizeof(ctx->error_msg) - 1);
        ctx->error_msg[sizeof(ctx->error_msg) - 1] = '\0';
    }
}

const char* serial_error_to_string(SerialError err)
{
    switch (err)
    {
        case SERIAL_OK:                 return "Success";
        case SERIAL_ERR_FILE_OPEN:      return "Failed to open file";
        case SERIAL_ERR_FILE_WRITE:     return "Failed to write to file";
        case SERIAL_ERR_FILE_READ:      return "Failed to read from file";
        case SERIAL_ERR_PARSE:          return "Failed to parse data";
        case SERIAL_ERR_MALLOC:         return "Memory allocation failed";
        case SERIAL_ERR_INVALID_FORMAT: return "Invalid file format";
        case SERIAL_ERR_NULL_PTR:       return "Null pointer provided";
        case SERIAL_ERR_VERSION_MISMATCH: return "Version mismatch";
        default:                        return "Unknown error";
    }
}

void serial_print_error(SerialContext* ctx)
{
    if (!ctx) return;
    if (ctx->last_error != SERIAL_OK)
    {
        printf("  [ERROR] %s", serial_error_to_string(ctx->last_error));
        if (ctx->error_msg[0] != '\0')
        {
            printf(": %s", ctx->error_msg);
        }
        printf("\n");
    }
}

SerialError serial_save_int_array(const char* filename, int* arr, size_t size)
{
    if (!filename || !arr || size == 0)
    {
        return SERIAL_ERR_NULL_PTR;
    }

    FILE* fp = fopen(filename, "w");
    if (!fp)
    {
        return SERIAL_ERR_FILE_OPEN;
    }

    fprintf(fp, "%s\n", SERIAL_FORMAT_NAME);
    fprintf(fp, "VERSION:%d\n", SERIAL_FORMAT_VERSION);
    fprintf(fp, "TYPE:INT_ARRAY\n");
    fprintf(fp, "SIZE:%zu\n", size);
    fprintf(fp, "---\n");

    for (size_t i = 0; i < size; i++)
    {
        fprintf(fp, "%d\n", arr[i]);
    }

    fclose(fp);
    return SERIAL_OK;
}

SerialError serial_load_int_array(const char* filename, int** arr, size_t* size)
{
    if (!filename || !arr || !size)
    {
        return SERIAL_ERR_NULL_PTR;
    }

    FILE* fp = fopen(filename, "r");
    if (!fp)
    {
        return SERIAL_ERR_FILE_OPEN;
    }

    char line[256];
    int version = 0;
    size_t arr_size = 0;

    if (!fgets(line, sizeof(line), fp) || strncmp(line, SERIAL_FORMAT_NAME, strlen(SERIAL_FORMAT_NAME)) != 0)
    {
        fclose(fp);
        return SERIAL_ERR_INVALID_FORMAT;
    }

    if (!fgets(line, sizeof(line), fp) || sscanf(line, "VERSION:%d", &version) != 1)
    {
        fclose(fp);
        return SERIAL_ERR_PARSE;
    }

    if (!fgets(line, sizeof(line), fp) || strncmp(line, "TYPE:INT_ARRAY", 14) != 0)
    {
        fclose(fp);
        return SERIAL_ERR_INVALID_FORMAT;
    }

    if (!fgets(line, sizeof(line), fp) || sscanf(line, "SIZE:%zu", &arr_size) != 1)
    {
        fclose(fp);
        return SERIAL_ERR_PARSE;
    }

    if (!fgets(line, sizeof(line), fp) || strncmp(line, "---", 3) != 0)
    {
        fclose(fp);
        return SERIAL_ERR_INVALID_FORMAT;
    }

    int* result = (int*)malloc(sizeof(int) * arr_size);
    if (!result)
    {
        fclose(fp);
        return SERIAL_ERR_MALLOC;
    }

    for (size_t i = 0; i < arr_size; i++)
    {
        if (fscanf(fp, "%d", &result[i]) != 1)
        {
            free(result);
            fclose(fp);
            return SERIAL_ERR_PARSE;
        }
    }

    fclose(fp);
    *arr = result;
    *size = arr_size;

    return SERIAL_OK;
}

SerialError serial_save_binary_tree(SerialNode* root, const char* filename)
{
    if (!filename)
    {
        return SERIAL_ERR_NULL_PTR;
    }

    FILE* fp = fopen(filename, "w");
    if (!fp)
    {
        return SERIAL_ERR_FILE_OPEN;
    }

    fprintf(fp, "%s\n", SERIAL_FORMAT_NAME);
    fprintf(fp, "VERSION:%d\n", SERIAL_FORMAT_VERSION);
    fprintf(fp, "TYPE:BINARY_TREE\n");
    fprintf(fp, "---\n");

    if (root == NULL)
    {
        fprintf(fp, "NULL\n");
    }
    else
    {
        fprintf(fp, "%d %p %p\n", 
                root->value, 
                (void*)root->left, 
                (void*)root->right);
    }

    fclose(fp);
    return SERIAL_OK;
}

SerialError serial_save_binary_tree_recursive(FILE* fp, SerialNode* node)
{
    if (!fp)
    {
        return SERIAL_ERR_FILE_WRITE;
    }

    if (node == NULL)
    {
        fprintf(fp, "NULL\n");
        return SERIAL_OK;
    }

    fprintf(fp, "NODE:%d\n", node->value);
    
    SerialError err = serial_save_binary_tree_recursive(fp, node->left);
    if (err != SERIAL_OK) return err;
    
    err = serial_save_binary_tree_recursive(fp, node->right);
    if (err != SERIAL_OK) return err;

    return SERIAL_OK;
}

SerialNode* serial_load_binary_tree_recursive(FILE* fp)
{
    char line[256];
    
    if (!fgets(line, sizeof(line), fp))
    {
        return NULL;
    }

    if (strncmp(line, "NULL", 4) == 0)
    {
        return NULL;
    }

    int value;
    if (sscanf(line, "NODE:%d", &value) == 1)
    {
        SerialNode* node = (SerialNode*)malloc(sizeof(SerialNode));
        if (node)
        {
            node->value = value;
            node->left = serial_load_binary_tree_recursive(fp);
            node->right = serial_load_binary_tree_recursive(fp);
        }
        return node;
    }

    return NULL;
}

SerialNode* serial_load_binary_tree(const char* filename)
{
    if (!filename)
    {
        return NULL;
    }

    FILE* fp = fopen(filename, "r");
    if (!fp)
    {
        return NULL;
    }

    char line[256];
    SerialNode* root = NULL;

    if (!fgets(line, sizeof(line), fp) || 
        strncmp(line, SERIAL_FORMAT_NAME, strlen(SERIAL_FORMAT_NAME)) != 0)
    {
        fclose(fp);
        return NULL;
    }

    if (!fgets(line, sizeof(line), fp) || 
        !fgets(line, sizeof(line), fp) || 
        strncmp(line, "TYPE:BINARY_TREE", 16) != 0)
    {
        fclose(fp);
        return NULL;
    }

    if (!fgets(line, sizeof(line), fp) || strncmp(line, "---", 3) != 0)
    {
        fclose(fp);
        return NULL;
    }

    root = serial_load_binary_tree_recursive(fp);
    fclose(fp);
    return root;
}

void serial_destroy_binary_tree(SerialNode* root)
{
    if (!root) return;
    serial_destroy_binary_tree(root->left);
    serial_destroy_binary_tree(root->right);
    free(root);
}

SerialError serial_save_linked_list(void* head, const char* filename, int node_count)
{
    if (!filename)
    {
        return SERIAL_ERR_NULL_PTR;
    }

    FILE* fp = fopen(filename, "w");
    if (!fp)
    {
        return SERIAL_ERR_FILE_OPEN;
    }

    fprintf(fp, "%s\n", SERIAL_FORMAT_NAME);
    fprintf(fp, "VERSION:%d\n", SERIAL_FORMAT_VERSION);
    fprintf(fp, "TYPE:LINKED_LIST\n");
    fprintf(fp, "NODE_COUNT:%d\n", node_count);
    fprintf(fp, "---\n");

    int** current = (int**)head;
    for (int i = 0; i < node_count && current && *current; i++)
    {
        fprintf(fp, "%d\n", **current);
        current = (int**)(*current + 1);
    }

    fclose(fp);
    return SERIAL_OK;
}

void* serial_load_linked_list(const char* filename, int* node_count)
{
    if (!filename || !node_count)
    {
        return NULL;
    }

    FILE* fp = fopen(filename, "r");
    if (!fp)
    {
        return NULL;
    }

    char line[256];
    int count = 0;

    if (!fgets(line, sizeof(line), fp) || 
        strncmp(line, SERIAL_FORMAT_NAME, strlen(SERIAL_FORMAT_NAME)) != 0)
    {
        fclose(fp);
        return NULL;
    }

    if (!fgets(line, sizeof(line), fp) || 
        !fgets(line, sizeof(line), fp) || 
        strncmp(line, "TYPE:LINKED_LIST", 16) != 0)
    {
        fclose(fp);
        return NULL;
    }

    if (!fgets(line, sizeof(line), fp) || sscanf(line, "NODE_COUNT:%d", &count) != 1)
    {
        fclose(fp);
        return NULL;
    }

    if (!fgets(line, sizeof(line), fp) || strncmp(line, "---", 3) != 0)
    {
        fclose(fp);
        return NULL;
    }

    typedef struct ListNode {
        int value;
        struct ListNode* next;
    } ListNode;

    ListNode* head = NULL;
    ListNode* tail = NULL;

    for (int i = 0; i < count; i++)
    {
        int value;
        if (fscanf(fp, "%d", &value) != 1)
        {
            while (head)
            {
                ListNode* temp = head;
                head = head->next;
                free(temp);
            }
            fclose(fp);
            return NULL;
        }

        ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
        if (!new_node)
        {
            while (head)
            {
                ListNode* temp = head;
                head = head->next;
                free(temp);
            }
            fclose(fp);
            return NULL;
        }

        new_node->value = value;
        new_node->next = NULL;

        if (!head)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node;
            tail = new_node;
        }
    }

    fclose(fp);
    *node_count = count;

    return (void*)head;
}

SerialError serial_save_graph_adjlist(int** adj, int vertices, const char* filename)
{
    if (!filename || !adj)
    {
        return SERIAL_ERR_NULL_PTR;
    }

    FILE* fp = fopen(filename, "w");
    if (!fp)
    {
        return SERIAL_ERR_FILE_OPEN;
    }

    fprintf(fp, "%s\n", SERIAL_FORMAT_NAME);
    fprintf(fp, "VERSION:%d\n", SERIAL_FORMAT_VERSION);
    fprintf(fp, "TYPE:GRAPH_ADJLIST\n");
    fprintf(fp, "VERTICES:%d\n", vertices);
    fprintf(fp, "---\n");

    for (int i = 0; i < vertices; i++)
    {
        if (adj[i])
        {
            int j = 0;
            while (adj[i][j] != -1 && j < vertices)
            {
                fprintf(fp, "%d ", adj[i][j]);
                j++;
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    return SERIAL_OK;
}

int** serial_load_graph_adjlist(const char* filename, int* vertices)
{
    if (!filename || !vertices)
    {
        return NULL;
    }

    FILE* fp = fopen(filename, "r");
    if (!fp)
    {
        return NULL;
    }

    char line[1024];
    int vert_count = 0;

    if (!fgets(line, sizeof(line), fp) || 
        strncmp(line, SERIAL_FORMAT_NAME, strlen(SERIAL_FORMAT_NAME)) != 0)
    {
        fclose(fp);
        return NULL;
    }

    if (!fgets(line, sizeof(line), fp) || 
        !fgets(line, sizeof(line), fp) || 
        strncmp(line, "TYPE:GRAPH_ADJLIST", 18) != 0)
    {
        fclose(fp);
        return NULL;
    }

    if (!fgets(line, sizeof(line), fp) || sscanf(line, "VERTICES:%d", &vert_count) != 1)
    {
        fclose(fp);
        return NULL;
    }

    if (!fgets(line, sizeof(line), fp) || strncmp(line, "---", 3) != 0)
    {
        fclose(fp);
        return NULL;
    }

    int** adj = (int**)malloc(sizeof(int*) * vert_count);
    if (!adj)
    {
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < vert_count; i++)
    {
        adj[i] = (int*)malloc(sizeof(int) * (vert_count + 1));
        if (!adj[i])
        {
            for (int j = 0; j < i; j++)
            {
                free(adj[j]);
            }
            free(adj);
            fclose(fp);
            return NULL;
        }

        for (int j = 0; j < vert_count + 1; j++)
        {
            adj[i][j] = -1;
        }
    }

    for (int i = 0; i < vert_count; i++)
    {
        if (!fgets(line, sizeof(line), fp))
        {
            for (int j = 0; j < vert_count; j++)
            {
                free(adj[j]);
            }
            free(adj);
            fclose(fp);
            return NULL;
        }

        char* ptr = line;
        int neighbor;
        int count = 0;
        while (sscanf(ptr, "%d", &neighbor) == 1 && count < vert_count)
        {
            adj[i][count++] = neighbor;
            while (*ptr && *ptr == ' ') ptr++;
            while (*ptr && *ptr != ' ') ptr++;
        }
    }

    fclose(fp);
    *vertices = vert_count;

    return adj;
}

void serial_destroy_graph(int** adj, int vertices)
{
    if (!adj) return;
    for (int i = 0; i < vertices; i++)
    {
        free(adj[i]);
    }
    free(adj);
}

static void demo_int_array_serialization(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||           INT ARRAY SERIALIZATION DEMO                 ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    int arr[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    size_t size = sizeof(arr) / sizeof(arr[0]);

    printf("  Original array: ");
    for (size_t i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    const char* filename = "data/array_demo.dat";
    printf("  Saving to: %s\n", filename);

    SerialError err = serial_save_int_array(filename, arr, size);
    if (err != SERIAL_OK)
    {
        printf("  [ERROR] Failed to save: %s\n", serial_error_to_string(err));
        return;
    }
    printf("  [OK] Array saved successfully!\n");

    int* loaded_arr = NULL;
    size_t loaded_size = 0;

    printf("  Loading from: %s\n", filename);
    err = serial_load_int_array(filename, &loaded_arr, &loaded_size);

    if (err != SERIAL_OK)
    {
        printf("  [ERROR] Failed to load: %s\n", serial_error_to_string(err));
        return;
    }

    printf("  [OK] Array loaded successfully!\n");
    printf("  Loaded array: ");
    for (size_t i = 0; i < loaded_size; i++)
    {
        printf("%d ", loaded_arr[i]);
    }
    printf("\n");
    printf("  Size: %zu elements\n", loaded_size);

    free(loaded_arr);
    printf("\n  [Demo complete]\n");
}

static void demo_binary_tree_serialization(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||          BINARY TREE SERIALIZATION DEMO                ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    SerialNode* root = (SerialNode*)malloc(sizeof(SerialNode));
    root->value = 50;
    root->left = (SerialNode*)malloc(sizeof(SerialNode));
    root->left->value = 30;
    root->left->left = (SerialNode*)malloc(sizeof(SerialNode));
    root->left->left->value = 20;
    root->left->right = (SerialNode*)malloc(sizeof(SerialNode));
    root->left->right->value = 40;
    root->right = (SerialNode*)malloc(sizeof(SerialNode));
    root->right->value = 70;
    root->right->left = (SerialNode*)malloc(sizeof(SerialNode));
    root->right->left->value = 60;
    root->right->right = (SerialNode*)malloc(sizeof(SerialNode));
    root->right->right->value = 80;

    printf("  Original tree (pre-order): 50, 30, 20, 40, 70, 60, 80\n");
    printf("  Structure:\n");
    printf("         50\n");
    printf("        /  \\\n");
    printf("      30    70\n");
    printf("     / \\   / \\\n");
    printf("   20  40 60  80\n");

    const char* filename = "data/tree_demo.dat";
    SerialError err = serial_save_binary_tree(root, filename);

    if (err != SERIAL_OK)
    {
        printf("  [ERROR] Failed to save: %s\n", serial_error_to_string(err));
        serial_destroy_binary_tree(root);
        return;
    }
    printf("  [OK] Tree saved to: %s\n", filename);

    SerialNode* loaded_root = serial_load_binary_tree(filename);

    if (!loaded_root)
    {
        printf("  [ERROR] Failed to load tree\n");
        serial_destroy_binary_tree(root);
        return;
    }
    printf("  [OK] Tree loaded successfully!\n");
    printf("  Loaded tree (pre-order): %d, %d, %d, %d, %d, %d, %d\n",
           loaded_root->value,
           loaded_root->left->value,
           loaded_root->left->left->value,
           loaded_root->left->right->value,
           loaded_root->right->value,
           loaded_root->right->left->value,
           loaded_root->right->right->value);

    serial_destroy_binary_tree(root);
    serial_destroy_binary_tree(loaded_root);
    printf("\n  [Demo complete]\n");
}

static void demo_error_handling(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||              ERROR HANDLING DEMO                      ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    SerialContext* ctx = serial_create_context();

    int* arr = NULL;
    size_t size = 0;

    printf("  Test 1: Load non-existent file\n");
    SerialError err = serial_load_int_array("nonexistent.dat", &arr, &size);
    printf("  Result: %s\n", serial_error_to_string(err));

    printf("\n  Test 2: Save to invalid path\n");
    err = serial_save_int_array("/root/badpath/data.dat", NULL, 10);
    printf("  Result: %s\n", serial_error_to_string(err));

    printf("\n  Test 3: Load corrupted file\n");
    FILE* fp = fopen("data/corrupt.dat", "w");
    if (fp)
    {
        fprintf(fp, "INVALID_HEADER\n");
        fclose(fp);
    }
    err = serial_load_int_array("data/corrupt.dat", &arr, &size);
    printf("  Result: %s\n", serial_error_to_string(err));

    serial_destroy_context(ctx);
    printf("\n  [Demo complete]\n");
}

static void interactive_serialization_demo(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||           INTERACTIVE SERIALIZATION DEMO               ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    int choice;
    printf("  1. Save integer array to file\n");
    printf("  2. Load integer array from file\n");
    printf("  3. Save binary tree to file\n");
    printf("  4. Load binary tree from file\n");
    printf("  5. Back to main menu\n");
    printf("\n");

    if (safe_input_int(&choice, "  Enter choice: ", 1, 5) != 0)
        return;

    switch (choice)
    {
        case 1:
        {
            int count;
            if (safe_input_int(&count, "  Enter number of elements (1-100): ", 1, 100) != 0)
                break;

            int* arr = (int*)malloc(sizeof(int) * count);
            if (!arr)
            {
                printf("  [ERROR] Memory allocation failed\n");
                break;
            }

            printf("  Enter %d integers:\n", count);
            for (int i = 0; i < count; i++)
            {
                char prompt[32];
                sprintf(prompt, "  Element %d: ", i + 1);
                safe_input_int(&arr[i], prompt, -1000000, 1000000);
            }

            char filename[128];
            printf("  Enter filename: ");
            scanf("%127s", filename);

            SerialError err = serial_save_int_array(filename, arr, count);
            if (err == SERIAL_OK)
            {
                printf("  [OK] Saved %d elements to %s\n", count, filename);
            }
            else
            {
                printf("  [ERROR] Failed to save: %s\n", serial_error_to_string(err));
            }

            free(arr);
            break;
        }
        case 2:
        {
            char filename[128];
            printf("  Enter filename: ");
            scanf("%127s", filename);

            int* arr = NULL;
            size_t size = 0;

            SerialError err = serial_load_int_array(filename, &arr, &size);
            if (err == SERIAL_OK)
            {
                printf("  [OK] Loaded %zu elements:\n", size);
                for (size_t i = 0; i < size; i++)
                {
                    printf("    [%zu] = %d\n", i, arr[i]);
                }
                free(arr);
            }
            else
            {
                printf("  [ERROR] Failed to load: %s\n", serial_error_to_string(err));
            }
            break;
        }
        case 3:
        case 4:
            printf("  Binary tree serialization uses pre-order traversal.\n");
            printf("  [Demo not fully interactive - use demo_binary_tree_serialization()]\n");
            break;
        case 5:
            return;
    }
}

static void print_serialization_explanation(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||          WHAT IS SERIALIZATION?                        ||\n");
    printf("  +========================================================+\n");
    printf("\n");
    printf("  Serialization is the process of converting data structures\n");
    printf("  into a format that can be stored in a file or transmitted\n");
    printf("  over a network, then reconstructed later.\n");
    printf("\n");
    printf("  Why Serialize?\n");
    printf("  --------------\n");
    printf("  - Save progress between sessions\n");
    printf("  - Export and import data structures\n");
    printf("  - Share data structures with others\n");
    printf("  - Create checkpoints for complex operations\n");
    printf("\n");
    printf("  This Module Supports:\n");
    printf("  --------------------\n");
    printf("  - Integer arrays\n");
    printf("  - Binary trees (pre-order traversal)\n");
    printf("  - Linked lists\n");
    printf("  - Graphs (adjacency list)\n");
    printf("\n");
    printf("  File Format:\n");
    printf("  ------------\n");
    printf("  All files start with a header:\n");
    printf("    - Format name\n");
    printf("    - Version number\n");
    printf("    - Data type\n");
    printf("    - Metadata (size, count, etc.)\n");
    printf("    - Separator (---)\n");
    printf("    - Actual data\n");
    printf("\n");
    printf("  Error Handling:\n");
    printf("  ---------------\n");
    printf("  - Checks for file existence\n");
    printf("  - Validates file format\n");
    printf("  - Handles memory allocation failures\n");
    printf("  - Reports specific error types\n");
    printf("\n");
}

void serialization_demo(void)
{
    while (1)
    {
        display_header("Serialization & Deserialization Engine");

        printf("\nSave and load data structures to/from files.\n");
        printf("Preserve your work between sessions.\n\n");

        printf("1. Int Array Serialization Demo\n");
        printf("2. Binary Tree Serialization Demo\n");
        printf("3. Error Handling Demo\n");
        printf("4. Interactive Serialization\n");
        printf("5. Educational Guide\n");
        printf("6. Back to main menu\n");
        printf("\n");

        int choice;
        if (safe_input_int(&choice, "Enter choice: ", 1, 6) != 0)
            break;

        switch (choice)
        {
            case 1:
                demo_int_array_serialization();
                break;
            case 2:
                demo_binary_tree_serialization();
                break;
            case 3:
                demo_error_handling();
                break;
            case 4:
                interactive_serialization_demo();
                break;
            case 5:
                print_serialization_explanation();
                break;
            case 6:
                return;
        }

        printf("\nPress Enter to continue...");
        getchar();
    }
}