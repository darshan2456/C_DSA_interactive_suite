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

int main(void)
{
    printf("Starting Generic File Export unit tests...\n");
    test_dll_export_int();
    test_dll_export_string();
    printf("All Generic File Export unit tests passed successfully!\n");
    return 0;
}
