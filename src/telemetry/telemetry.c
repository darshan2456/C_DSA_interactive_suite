#include "telemetry.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* op_type_to_string(OperationType op)
{
    switch (op)
    {
        case OP_SORT_COMPARE:    return "COMPARE";
        case OP_SORT_SWAP:       return "SWAP";
        case OP_SORT_INSERT:     return "INSERT";
        case OP_SORT_MARK:       return "MARK_SORTED";
        case OP_SEARCH_VISIT:    return "VISIT";
        case OP_SEARCH_FOUND:    return "FOUND";
        case OP_SEARCH_NOT_FOUND: return "NOT_FOUND";
        case OP_GRAPH_VISIT:     return "GRAPH_VISIT";
        case OP_GRAPH_ENQUEUE:   return "ENQUEUE";
        case OP_GRAPH_DEQUEUE:   return "DEQUEUE";
        case OP_GRAPH_RELAX:     return "RELAX";
        case OP_TREE_INSERT:     return "TREE_INSERT";
        case OP_TREE_DELETE:     return "TREE_DELETE";
        case OP_TREE_ROTATE:     return "ROTATE";
        case OP_CUSTOM:          return "CUSTOM";
        default:                 return "UNKNOWN";
    }
}

void telemetry_init(TelemetryContext* ctx, const char* algorithm_name)
{
    if (!ctx) return;
    
    memset(ctx, 0, sizeof(TelemetryContext));
    if (algorithm_name)
    {
        strncpy(ctx->algorithm_name, algorithm_name, sizeof(ctx->algorithm_name) - 1);
    }
    else
    {
        strcpy(ctx->algorithm_name, "Unknown");
    }
    ctx->format = FORMAT_JSON;
    ctx->enabled = 1;
    ctx->initialized = 1;
    ctx->last_error = TELEMETRY_OK;
}

void telemetry_destroy(TelemetryContext* ctx)
{
    if (!ctx) return;
    
    if (ctx->file)
    {
        if (ctx->format == FORMAT_JSON)
        {
            fprintf(ctx->file, "\n  ]\n}\n");
        }
        fclose(ctx->file);
        ctx->file = NULL;
    }
    ctx->initialized = 0;
}

void telemetry_set_export_file(TelemetryContext* ctx, const char* filename)
{
    if (!ctx || !filename) return;
    
    if (ctx->file)
    {
        fclose(ctx->file);
        ctx->file = NULL;
    }
    
    strncpy(ctx->export_file, filename, sizeof(ctx->export_file) - 1);
    
    if (ctx->initialized)
    {
        ctx->file = fopen(filename, "w");
        if (!ctx->file)
        {
            ctx->last_error = TELEMETRY_ERR_FILE_OPEN;
            return;
        }
        
        if (ctx->format == FORMAT_JSON)
        {
            telemetry_export_json_start(ctx);
        }
        else
        {
            telemetry_export_csv_header(ctx);
        }
    }
}

void telemetry_set_format(TelemetryContext* ctx, ExportFormat format)
{
    if (!ctx) return;
    ctx->format = format;
}

void telemetry_enable(TelemetryContext* ctx)
{
    if (!ctx) return;
    ctx->enabled = 1;
}

void telemetry_disable(TelemetryContext* ctx)
{
    if (!ctx) return;
    ctx->enabled = 0;
}

void telemetry_set_auto_flush(TelemetryContext* ctx, int enabled)
{
    if (!ctx) return;
    ctx->auto_flush = enabled;
}

TelemetryError telemetry_get_error(TelemetryContext* ctx)
{
    if (!ctx) return TELEMETRY_ERR_INVALID_STATE;
    return ctx->last_error;
}

const char* telemetry_error_str(TelemetryError err)
{
    switch (err)
    {
        case TELEMETRY_OK:                   return "Success";
        case TELEMETRY_ERR_FILE_OPEN:        return "Failed to open file";
        case TELEMETRY_ERR_FILE_WRITE:       return "Failed to write to file";
        case TELEMETRY_ERR_NOT_INITIALIZED:  return "Telemetry not initialized";
        case TELEMETRY_ERR_INVALID_STATE:    return "Invalid state";
        case TELEMETRY_ERR_MALLOC:           return "Memory allocation failed";
        default:                             return "Unknown error";
    }
}

void telemetry_export_csv_header(TelemetryContext* ctx)
{
    if (!ctx || !ctx->file) return;
    fprintf(ctx->file, "step,operation,value1,value2,array,description\n");
}

void telemetry_export_json_start(TelemetryContext* ctx)
{
    if (!ctx || !ctx->file) return;
    
    fprintf(ctx->file, "{\n");
    fprintf(ctx->file, "  \"algorithm\": \"%s\",\n", ctx->algorithm_name);
    fprintf(ctx->file, "  \"format_version\": 1,\n");
    fprintf(ctx->file, "  \"entries\": [\n");
}

void telemetry_export_json_end(TelemetryContext* ctx)
{
    if (!ctx || !ctx->file) return;
    fprintf(ctx->file, "\n  ]\n}\n");
}

void telemetry_record_step(TelemetryContext* ctx, OperationType op, const char* desc)
{
    if (!ctx || !ctx->initialized || !ctx->enabled) return;
    
    ctx->entry_count++;
    
    if (!ctx->file && ctx->export_file[0] == '\0')
    {
        sprintf(ctx->export_file, "data/%s_trace.json", ctx->algorithm_name);
        ctx->file = fopen(ctx->export_file, "w");
        if (ctx->file)
        {
            telemetry_export_json_start(ctx);
        }
    }
    
    if (!ctx->file) return;
    
    if (ctx->format == FORMAT_JSON)
    {
        fprintf(ctx->file, "    {\"step\": %u, \"op\": \"%s\", \"desc\": \"%s\"}",
                ctx->entry_count, op_type_to_string(op), desc);
        if (ctx->auto_flush)
        {
            fprintf(ctx->file, "\n");
        }
        else
        {
            fprintf(ctx->file, ",\n");
        }
    }
    else
    {
        fprintf(ctx->file, "%u,%s,0,0,,\n", ctx->entry_count, op_type_to_string(op));
    }
}

void telemetry_record_int(TelemetryContext* ctx, OperationType op, int val1, int val2, const char* desc)
{
    if (!ctx || !ctx->initialized || !ctx->enabled) return;
    
    ctx->entry_count++;
    
    if (!ctx->file && ctx->export_file[0] == '\0')
    {
        sprintf(ctx->export_file, "data/%s_trace.json", ctx->algorithm_name);
        ctx->file = fopen(ctx->export_file, "w");
        if (ctx->file)
        {
            telemetry_export_json_start(ctx);
        }
    }
    
    if (!ctx->file) return;
    
    if (ctx->format == FORMAT_JSON)
    {
        fprintf(ctx->file, "    {\"step\": %u, \"op\": \"%s\", \"v1\": %d, \"v2\": %d, \"desc\": \"%s\"}",
                ctx->entry_count, op_type_to_string(op), val1, val2, desc);
        if (ctx->auto_flush)
        {
            fprintf(ctx->file, "\n");
        }
        else
        {
            fprintf(ctx->file, ",\n");
        }
    }
    else
    {
        fprintf(ctx->file, "%u,%s,%d,%d,,\n", ctx->entry_count, op_type_to_string(op), val1, val2);
    }
}

void telemetry_record_array(TelemetryContext* ctx, OperationType op, int* arr, size_t size, const char* desc)
{
    if (!ctx || !ctx->initialized || !ctx->enabled) return;
    
    ctx->entry_count++;
    
    if (!ctx->file && ctx->export_file[0] == '\0')
    {
        sprintf(ctx->export_file, "data/%s_trace.json", ctx->algorithm_name);
        ctx->file = fopen(ctx->export_file, "w");
        if (ctx->file)
        {
            telemetry_export_json_start(ctx);
        }
    }
    
    if (!ctx->file) return;
    
    if (ctx->format == FORMAT_JSON)
    {
        fprintf(ctx->file, "    {\"step\": %u, \"op\": \"%s\", \"array\": [", 
                ctx->entry_count, op_type_to_string(op));
        
        for (size_t i = 0; i < size; i++)
        {
            fprintf(ctx->file, "%d", arr[i]);
            if (i < size - 1) fprintf(ctx->file, ", ");
        }
        
        fprintf(ctx->file, "], \"desc\": \"%s\"}", desc);
        
        if (ctx->auto_flush)
        {
            fprintf(ctx->file, "\n");
        }
        else
        {
            fprintf(ctx->file, ",\n");
        }
    }
    else
    {
        fprintf(ctx->file, "%u,%s,0,0,\"", ctx->entry_count, op_type_to_string(op));
        for (size_t i = 0; i < size; i++)
        {
            fprintf(ctx->file, "%d", arr[i]);
            if (i < size - 1) fprintf(ctx->file, ";");
        }
        fprintf(ctx->file, "\",\n");
    }
}

static void demo_bubble_sort_export(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||         BUBBLE SORT TELEMETRY EXPORT DEMO               ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    TelemetryContext ctx;
    telemetry_init(&ctx, "BubbleSort");
    telemetry_set_auto_flush(&ctx, 1);
    telemetry_set_export_file(&ctx, "data/bubblesort_trace.json");

    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("  Sorting array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n\n");

    telemetry_record_array(&ctx, OP_CUSTOM, arr, n, "Initial array");

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            telemetry_record_int(&ctx, OP_SORT_COMPARE, j, j + 1, 
                              "Comparing adjacent elements");

            if (arr[j] > arr[j + 1])
            {
                telemetry_record_int(&ctx, OP_SORT_SWAP, j, j + 1, "Swapping elements");
                
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
        telemetry_record_int(&ctx, OP_SORT_MARK, n - i - 1, arr[n - i - 1], 
                           "Element sorted");
    }

    telemetry_record_array(&ctx, OP_CUSTOM, arr, n, "Final sorted array");

    printf("  [Exported %u steps to data/bubblesort_trace.json]\n", ctx.entry_count);
    printf("  Sorted array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    telemetry_destroy(&ctx);
    printf("\n  [Demo complete]\n");
}

static void demo_linear_search_export(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||         LINEAR SEARCH TELEMETRY EXPORT DEMO             ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    TelemetryContext ctx;
    telemetry_init(&ctx, "LinearSearch");
    telemetry_set_auto_flush(&ctx, 1);
    telemetry_set_export_file(&ctx, "data/linearsearch_trace.json");

    int arr[] = {10, 23, 45, 70, 85, 92};
    int n = sizeof(arr) / sizeof(arr[0]);
    int target = 70;

    printf("  Array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n  Target: %d\n\n", target);

    telemetry_record_array(&ctx, OP_CUSTOM, arr, n, "Initial array");

    int found = -1;
    for (int i = 0; i < n; i++)
    {
        telemetry_record_int(&ctx, OP_SEARCH_VISIT, i, arr[i], "Visiting element");

        if (arr[i] == target)
        {
            found = i;
            telemetry_record_int(&ctx, OP_SEARCH_FOUND, i, arr[i], "Target found!");
            break;
        }
    }

    if (found == -1)
    {
        telemetry_record_int(&ctx, OP_SEARCH_NOT_FOUND, -1, target, "Target not found");
    }

    printf("  [Exported %u steps to data/linearsearch_trace.json]\n", ctx.entry_count);
    printf("  Result: %s (index %d)\n", found == -1 ? "NOT FOUND" : "FOUND", found);

    telemetry_destroy(&ctx);
    printf("\n  [Demo complete]\n");
}

static void demo_csv_export(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||              CSV EXPORT DEMO                           ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    TelemetryContext ctx;
    telemetry_init(&ctx, "BubbleSortCSV");
    telemetry_set_format(&ctx, FORMAT_CSV);
    telemetry_set_auto_flush(&ctx, 1);
    telemetry_set_export_file(&ctx, "data/bubblesort_trace.csv");

    int arr[] = {5, 2, 8, 1, 9};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("  Sorting array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n\n");

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            telemetry_record_int(&ctx, OP_SORT_COMPARE, j, j + 1, "Comparing");
            if (arr[j] > arr[j + 1])
            {
                telemetry_record_int(&ctx, OP_SORT_SWAP, j, j + 1, "Swapping");
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    printf("  [Exported %u steps to data/bubblesort_trace.csv]\n", ctx.entry_count);

    telemetry_destroy(&ctx);
    printf("\n  [Demo complete]\n");
}

static void demo_integration_with_debugger(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||       INTEGRATION WITH DEBUGGER DEMO                   ||\n");
    printf("  +========================================================+\n");
    printf("\n");

    printf("  This demo shows how to use telemetry with step-by-step\n");
    printf("  debugging for comprehensive algorithm analysis.\n\n");

    TelemetryContext ctx;
    telemetry_init(&ctx, "BubbleSortDebug");
    telemetry_set_auto_flush(&ctx, 1);
    telemetry_set_export_file(&ctx, "data/debug_trace.json");

    int arr[] = {30, 10, 50, 20, 40};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("  Initial: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n\n");

    telemetry_record_array(&ctx, OP_CUSTOM, arr, n, "Starting sort");

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            telemetry_record_int(&ctx, OP_SORT_COMPARE, j, j + 1, "Comparing");

            if (arr[j] > arr[j + 1])
            {
                telemetry_record_int(&ctx, OP_SORT_SWAP, j, j + 1, "Swapping");
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                telemetry_record_array(&ctx, OP_CUSTOM, arr, n, "After swap");
            }

            printf("    Step %u: arr[%d]=%d vs arr[%d]=%d\n", 
                   ctx.entry_count, j, arr[j], j+1, arr[j+1]);
        }
    }

    printf("\n  [Exported %u steps]\n", ctx.entry_count);
    printf("  Final: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    telemetry_destroy(&ctx);
}

static void print_telemetry_explanation(void)
{
    printf("\n");
    printf("  +========================================================+\n");
    printf("  ||          WHAT IS ALGORITHMIC TELEMETRY?                 ||\n");
    printf("  +========================================================+\n");
    printf("\n");
    printf("  Telemetry exports step-by-step algorithm execution data\n");
    printf("  to structured files (JSON/CSV) for external visualization.\n");
    printf("\n");
    printf("  Why Export Telemetry?\n");
    printf("  --------------------\n");
    printf("  - Visualize algorithms on web platforms\n");
    printf("  - Analyze algorithm behavior programmatically\n");
    printf("  - Share algorithm traces with others\n");
    printf("  - Create custom visualizations\n");
    printf("  - Educational presentations\n");
    printf("\n");
    printf("  Supported Formats:\n");
    printf("  ------------------\n");
    printf("  JSON: Machine-readable, nested structure\n");
    printf("  CSV:  Spreadsheet-friendly, flat structure\n");
    printf("\n");
    printf("  JSON Output Example:\n");
    printf("  --------------------\n");
    printf("  {\n");
    printf("    \"algorithm\": \"BubbleSort\",\n");
    printf("    \"format_version\": 1,\n");
    printf("    \"entries\": [\n");
    printf("      {\"step\": 1, \"op\": \"SWAP\", \"v1\": 0, \"v2\": 2, ...},\n");
    printf("      {\"step\": 2, \"op\": \"COMPARE\", ...}\n");
    printf("    ]\n");
    printf("  }\n");
    printf("\n");
    printf("  CSV Output Example:\n");
    printf("  -------------------\n");
    printf("  step,operation,value1,value2,array,description\n");
    printf("  1,SWAP,0,2,,\n");
    printf("  2,COMPARE,1,3,,\n");
    printf("\n");
    printf("  Integration:\n");
    printf("  ------------\n");
    printf("  Add telemetry calls in your algorithm:\n");
    printf("    telemetry_record_int(&ctx, OP_SORT_COMPARE, i, j, \"msg\");\n");
    printf("    telemetry_record_array(&ctx, OP_SORT_SWAP, arr, n, \"msg\");\n");
    printf("\n");
}

void telemetry_demo(void)
{
    while (1)
    {
        display_header("Algorithmic Step-Telemetry Exporter");

        printf("\nExport algorithm execution data to JSON/CSV.\n");
        printf("Visualize on web platforms or analyze programmatically.\n\n");

        printf("1. Bubble Sort Telemetry (JSON)\n");
        printf("2. Linear Search Telemetry (JSON)\n");
        printf("3. CSV Export Demo\n");
        printf("4. Integration with Debugger Demo\n");
        printf("5. Educational Guide\n");
        printf("6. Back to main menu\n");
        printf("\n");

        int choice;
        if (safe_input_int(&choice, "Enter choice: ", 1, 6) != 0)
            break;

        switch (choice)
        {
            case 1:
                demo_bubble_sort_export();
                break;
            case 2:
                demo_linear_search_export();
                break;
            case 3:
                demo_csv_export();
                break;
            case 4:
                demo_integration_with_debugger();
                break;
            case 5:
                print_telemetry_explanation();
                break;
            case 6:
                return;
        }

        printf("\nPress Enter to continue...");
        getchar();
    }
}