#include "history_logger.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define access _access
#define F_OK 0
#else
#include <unistd.h>
#endif

void test_history_logger(void)
{
    const char* filename = "output/benchmark_history.csv";
    int header_count = 0;
    int record_count = 0;
    FILE* empty_file = NULL;

    remove(filename);

    add_to_history("Test Algo A", 42, 0.001500);
    add_to_history("Test Algo B", 99, 1.234567);
    assert(access(filename, F_OK) == 0);

    FILE* file = fopen(filename, "r");
    assert(file != NULL);

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, "Algorithm/Array Type") != NULL)
        {
            header_count++;
            continue;
        }

        if (record_count == 0)
        {
            assert(strstr(line, "Test Algo A") != NULL);
            assert(strstr(line, "42") != NULL);
            assert(strstr(line, "0.001500") != NULL);
        }
        else if (record_count == 1)
        {
            assert(strstr(line, "Test Algo B") != NULL);
            assert(strstr(line, "99") != NULL);
            assert(strstr(line, "1.234567") != NULL);
        }

        record_count++;
    }

    fclose(file);

    assert(header_count == 1);
    assert(record_count == 2);
    remove(filename);

    empty_file = fopen(filename, "w");
    assert(empty_file != NULL);
    fclose(empty_file);

    header_count = 0;
    record_count = 0;
    add_to_history("Empty File Algo", 7, 0.123456);

    file = fopen(filename, "r");
    assert(file != NULL);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, "Algorithm/Array Type") != NULL)
        {
            header_count++;
            continue;
        }

        assert(strstr(line, "Empty File Algo") != NULL);
        assert(strstr(line, "7") != NULL);
        assert(strstr(line, "0.123456") != NULL);
        record_count++;
    }

    fclose(file);

    assert(header_count == 1);
    assert(record_count == 1);
    remove(filename);

    printf("History logger tests passed successfully!\n");
}

int main(void)
{
    test_history_logger();
    return 0;
}
