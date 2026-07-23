#include "memory_inspector.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_hexdump_null_handling(void)
{
    char buf[128];
    size_t written = format_hexdump(NULL, 0, buf, sizeof(buf));
    assert(written > 0);
    assert(strstr(buf, "NULL") != NULL);
    printf("test_hexdump_null_handling passed!\n");
}

void test_hexdump_basic_formatting(void)
{
    char data[] = "Hello C_DSA!";
    char buf[512];
    size_t len = strlen(data);

    size_t written = format_hexdump(data, len, buf, sizeof(buf));
    assert(written > 0);
    assert(strstr(buf, "48 65 6C 6C 6F") != NULL); // "Hello" in hex
    assert(strstr(buf, "Hello C_DSA!") != NULL);

    printf("test_hexdump_basic_formatting passed!\n");
}

int main(void)
{
    test_hexdump_null_handling();
    test_hexdump_basic_formatting();
    return 0;
}
