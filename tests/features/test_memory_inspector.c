#include "dll.h"
#include "memory_inspector.h"
#include "sll.h"
#include "trees.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct TestNode
{
    char tag;
    int value;
    void* next;
} TestNode;

void test_hexdump_null_handling(void)
{
    char buf[128];
    memset(buf, 0, sizeof(buf));
    size_t written = format_hexdump(NULL, 0, buf, sizeof(buf));
    assert(written > 0);
    assert(strstr(buf, "NULL") != NULL);
    printf("test_hexdump_null_handling passed!\n");
}

void test_hexdump_basic_formatting(void)
{
    char data[] = "Hello C_DSA!";
    char buf[512];
    memset(buf, 0, sizeof(buf));
    size_t len = strlen(data);

    size_t written = format_hexdump(data, len, buf, sizeof(buf));
    assert(written > 0);
    assert(strstr(buf, "48 65 6C 6C 6F") != NULL); // "Hello" in hex
    assert(strstr(buf, "Hello C_DSA!") != NULL);

    printf("test_hexdump_basic_formatting passed!\n");
}

void test_struct_layout_analyzer(void)
{
    TestNode node;
    memset(&node, 0, sizeof(node));
    node.tag = 'A';
    node.value = 42;
    node.next = NULL;

    StructLayout layout;
    memset(&layout, 0, sizeof(layout));
    layout.struct_name = "TestNode";
    layout.total_size = sizeof(TestNode);
    layout.alignment = _Alignof(TestNode);
    layout.field_count = 3;
    layout.fields[0] = (StructField){"tag", offsetof(TestNode, tag), sizeof(node.tag), 0};
    layout.fields[1] = (StructField){"value", offsetof(TestNode, value), sizeof(node.value), 0};
    layout.fields[2] = (StructField){"next", offsetof(TestNode, next), sizeof(node.next), 0};

    finalize_struct_layout(&layout);

    assert(layout.total_size == sizeof(TestNode));
    assert(layout.payload_size == sizeof(char) + sizeof(int) + sizeof(void*));
    assert(layout.total_padding == layout.total_size - layout.payload_size);

    print_struct_layout_report(&layout, &node);

    printf("test_struct_layout_analyzer passed!\n");
}

void test_dsa_node_inspectors(void)
{
    int val = 100;
    Node sll_node = {.data = &val, .next = NULL};
    doubly_ll_Node dll_node = {.data = &val, .prev = NULL, .next = NULL};
    bstNode bst_node = {.data = 42, .left = NULL, .right = NULL};

    inspect_sll_node_memory(&sll_node);
    inspect_dll_node_memory(&dll_node);
    inspect_bst_node_memory(&bst_node);

    printf("test_dsa_node_inspectors passed!\n");
}

int main(void)
{
    test_hexdump_null_handling();
    test_hexdump_basic_formatting();
    test_struct_layout_analyzer();
    test_dsa_node_inspectors();
    return 0;
}
