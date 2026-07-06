#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <stddef.h>

typedef enum {
    SERIAL_OK = 0,
    SERIAL_ERR_FILE_OPEN = -1,
    SERIAL_ERR_FILE_WRITE = -2,
    SERIAL_ERR_FILE_READ = -3,
    SERIAL_ERR_PARSE = -4,
    SERIAL_ERR_MALLOC = -5,
    SERIAL_ERR_INVALID_FORMAT = -6,
    SERIAL_ERR_NULL_PTR = -7,
    SERIAL_ERR_VERSION_MISMATCH = -8
} SerialError;

typedef struct {
    int version;
    const char* format_name;
    SerialError last_error;
    char error_msg[256];
} SerialContext;

typedef struct SerialNode {
    int value;
    struct SerialNode* left;
    struct SerialNode* right;
} SerialNode;

SerialContext* serial_create_context(void);
void serial_destroy_context(SerialContext* ctx);
void serial_set_error(SerialContext* ctx, SerialError err, const char* msg);

SerialError serial_save_int_array(const char* filename, int* arr, size_t size);
SerialError serial_load_int_array(const char* filename, int** arr, size_t* size);

SerialError serial_save_binary_tree(SerialNode* root, const char* filename);
SerialNode* serial_load_binary_tree(const char* filename);
void serial_destroy_binary_tree(SerialNode* root);

SerialError serial_save_linked_list(void* head, const char* filename, int node_count);
void* serial_load_linked_list(const char* filename, int* node_count);

SerialError serial_save_graph_adjlist(int** adj, int vertices, const char* filename);
int** serial_load_graph_adjlist(const char* filename, int* vertices);
void serial_destroy_graph(int** adj, int vertices);

const char* serial_error_to_string(SerialError err);
void serial_print_error(SerialContext* ctx);

void serialization_demo(void);

#endif // SERIALIZATION_H