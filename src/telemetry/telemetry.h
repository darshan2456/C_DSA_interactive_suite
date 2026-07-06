#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stddef.h>
#include <stdio.h>

typedef enum {
    TELEMETRY_OK = 0,
    TELEMETRY_ERR_FILE_OPEN = -1,
    TELEMETRY_ERR_FILE_WRITE = -2,
    TELEMETRY_ERR_NOT_INITIALIZED = -3,
    TELEMETRY_ERR_INVALID_STATE = -4,
    TELEMETRY_ERR_MALLOC = -5
} TelemetryError;

typedef enum {
    FORMAT_JSON,
    FORMAT_CSV
} ExportFormat;

typedef enum {
    OP_SORT_COMPARE,
    OP_SORT_SWAP,
    OP_SORT_INSERT,
    OP_SORT_MARK,
    OP_SEARCH_VISIT,
    OP_SEARCH_FOUND,
    OP_SEARCH_NOT_FOUND,
    OP_GRAPH_VISIT,
    OP_GRAPH_ENQUEUE,
    OP_GRAPH_DEQUEUE,
    OP_GRAPH_RELAX,
    OP_TREE_INSERT,
    OP_TREE_DELETE,
    OP_TREE_ROTATE,
    OP_CUSTOM
} OperationType;

typedef struct {
    unsigned int step_number;
    OperationType operation;
    int int_value1;
    int int_value2;
    int* array_snapshot;
    size_t array_size;
    char description[256];
} TelemetryEntry;

typedef struct {
    char algorithm_name[64];
    char export_file[256];
    ExportFormat format;
    FILE* file;
    TelemetryError last_error;
    unsigned int entry_count;
    int initialized;
    int enabled;
    int auto_flush;
} TelemetryContext;

void telemetry_init(TelemetryContext* ctx, const char* algorithm_name);
void telemetry_destroy(TelemetryContext* ctx);

void telemetry_set_export_file(TelemetryContext* ctx, const char* filename);
void telemetry_set_format(TelemetryContext* ctx, ExportFormat format);
void telemetry_enable(TelemetryContext* ctx);
void telemetry_disable(TelemetryContext* ctx);
void telemetry_set_auto_flush(TelemetryContext* ctx, int enabled);

TelemetryError telemetry_get_error(TelemetryContext* ctx);
const char* telemetry_error_str(TelemetryError err);

void telemetry_record_step(TelemetryContext* ctx, OperationType op, const char* desc);
void telemetry_record_int(TelemetryContext* ctx, OperationType op, int val1, int val2, const char* desc);
void telemetry_record_array(TelemetryContext* ctx, OperationType op, int* arr, size_t size, const char* desc);

void telemetry_export_csv_header(TelemetryContext* ctx);
void telemetry_export_json_start(TelemetryContext* ctx);
void telemetry_export_json_end(TelemetryContext* ctx);

void telemetry_demo(void);

#endif // TELEMETRY_H