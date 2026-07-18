#include "dll.h"
#include "export.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void verify_file_content(const char* filepath, const char* expected)
{
    FILE* fp = fopen(filepath, "r");
    assert(fp != NULL);

    char buffer[4096];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, fp);
    buffer[bytes_read] = '\0';
    fclose(fp);

    assert(strcmp(buffer, expected) == 0);
    remove(filepath);
}

static void test_dll_export_int(void)
{
    doubly_ll_Node* head = NULL;
    int val1 = 10, val2 = 20, val3 = 30;

    dll_insertAtEnd(&head, &val1);
    dll_insertAtEnd(&head, &val2);
    dll_insertAtEnd(&head, &val3);

    // Text Export Test
    assert(dll_export(head, "test_binaries/dll_int.txt", EXPORT_FORMAT_TEXT, write_data_int));
    verify_file_content("test_binaries/dll_int.txt", "10 -> 20 -> 30 -> NULL\n");

    // CSV Export Test
    assert(dll_export(head, "test_binaries/dll_int.csv", EXPORT_FORMAT_CSV, write_data_int));
    verify_file_content("test_binaries/dll_int.csv", "index,value\n0,10\n1,20\n2,30\n");

    // JSON Export Test
    assert(dll_export(head, "test_binaries/dll_int.json", EXPORT_FORMAT_JSON, write_data_int));
    verify_file_content("test_binaries/dll_int.json", "[\n  10,\n  20,\n  30\n]\n");

    delete_dll(head, NULL);
}

static void test_dll_export_string(void)
{
    doubly_ll_Node* head = NULL;
    char* val1 = "apple";
    char* val2 = "banana";

    dll_insertAtEnd(&head, val1);
    dll_insertAtEnd(&head, val2);

    // Text Export Test
    assert(dll_export(head, "test_binaries/dll_str.txt", EXPORT_FORMAT_TEXT, write_data_string));
    verify_file_content("test_binaries/dll_str.txt", "apple -> banana -> NULL\n");

    // CSV Export Test
    assert(dll_export(head, "test_binaries/dll_str.csv", EXPORT_FORMAT_CSV, write_data_string));
    verify_file_content("test_binaries/dll_str.csv", "index,value\n0,apple\n1,banana\n");

    // JSON Export Test
    assert(dll_export(head, "test_binaries/dll_str.json", EXPORT_FORMAT_JSON, write_data_string));
    verify_file_content("test_binaries/dll_str.json", "[\n  apple,\n  banana\n]\n");

    delete_dll(head, NULL);
}

static void test_stack_export_int(void)
{
    stack* s = createStack();
    assert(s != NULL);

    int val1 = 30, val2 = 20, val3 = 10;
    push(s, &val1);
    push(s, &val2);
    push(s, &val3);

    // Text Export Test
    assert(stack_export(s, "test_binaries/stack.txt", EXPORT_FORMAT_TEXT, write_data_int));
    verify_file_content("test_binaries/stack.txt", "10 -> 20 -> 30 -> NULL\n");

    // CSV Export Test
    assert(stack_export(s, "test_binaries/stack.csv", EXPORT_FORMAT_CSV, write_data_int));
    verify_file_content("test_binaries/stack.csv", "index,value\n0,10\n1,20\n2,30\n");

    // JSON Export Test
    assert(stack_export(s, "test_binaries/stack.json", EXPORT_FORMAT_JSON, write_data_int));
    verify_file_content("test_binaries/stack.json", "[\n  10,\n  20,\n  30\n]\n");

    destroyStack(s, NULL);
}

static void test_queue_export_int(void)
{
    Queue q_simple;
    init_simple_queue(5, &q_simple);
    int* val1 = malloc(sizeof(int));
    int* val2 = malloc(sizeof(int));
    *val1 = 100;
    *val2 = 200;
    enqueue_simple(&q_simple, val1);
    enqueue_simple(&q_simple, val2);

    printf("  Starting simple queue export test...\n");
    assert(queue_export(&q_simple, QUEUE_TYPE_SIMPLE, "test_binaries/queue_simple.txt",
                        EXPORT_FORMAT_TEXT, write_data_int));
    verify_file_content("test_binaries/queue_simple.txt", "100 -> 200 -> NULL\n");
    printf("  Simple queue export passed.\n");

    destroy_simple_queue(&q_simple);

    Queue q_circ;
    init_circ_queue(5, &q_circ);
    int* cval1 = malloc(sizeof(int));
    int* cval2 = malloc(sizeof(int));
    *cval1 = 10;
    *cval2 = 20;
    enqueue(&q_circ, cval1);
    enqueue(&q_circ, cval2);

    printf("  Starting circular queue export test...\n");
    assert(queue_export(&q_circ, QUEUE_TYPE_CIRCULAR, "test_binaries/queue_circ.csv",
                        EXPORT_FORMAT_CSV, write_data_int));
    verify_file_content("test_binaries/queue_circ.csv", "index,value\n0,10\n1,20\n");
    printf("  Circular queue export passed.\n");

    destroy_circ_queue(&q_circ);

    Queue dq;
    init_deque(5, &dq);
    int* dval1 = malloc(sizeof(int));
    int* dval2 = malloc(sizeof(int));
    *dval1 = 5;
    *dval2 = 15;
    deque_insert_rear(&dq, dval1);
    deque_insert_front(&dq, dval2);

    printf("  Starting deque export test...\n");
    assert(queue_export(&dq, QUEUE_TYPE_DEQUE, "test_binaries/deque.json", EXPORT_FORMAT_JSON,
                        write_data_int));
    verify_file_content("test_binaries/deque.json", "[\n  15,\n  5\n]\n");
    printf("  Deque export passed.\n");

    destroy_deque(&dq);
}

int main(void)
{
    printf("Starting Generic File Export unit tests...\n");
    test_dll_export_int();
    printf("test_dll_export_int passed\n");
    test_dll_export_string();
    printf("test_dll_export_string passed\n");
    test_stack_export_int();
    printf("test_stack_export_int passed\n");
    test_queue_export_int();
    printf("test_queue_export_int passed\n");
    printf("All Generic File Export unit tests passed successfully!\n");
    return 0;
}
