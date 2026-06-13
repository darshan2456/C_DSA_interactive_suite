#include "history_logger.h"
#include <errno.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#define make_dir(path) _mkdir(path)
#else
#include <sys/stat.h>
#define make_dir(path) mkdir(path, 0777)
#endif

void add_to_history(const char* algo_name, int data_size, double execution_time)
{
    const char* filename = "output/benchmark_history.csv";
    FILE* check_file = NULL;
    FILE* csv_file = NULL;
    int dir_status;
    int is_new = 0;
    time_t raw_time;
    struct tm* time_info;
    char timestamp[30];

    errno = 0;
    dir_status = make_dir("output");
    if (dir_status != 0 && errno != EEXIST)
    {
        fprintf(stderr, "\nError: Unable to create the output directory for benchmark logging.\n");
        return;
    }

    check_file = fopen(filename, "r");
    if (check_file == NULL)
    {
        is_new = 1;
    }
    else
    {
        fseek(check_file, 0, SEEK_END);
        if (ftell(check_file) == 0)
        {
            is_new = 1;
        }
        fclose(check_file);
    }

    csv_file = fopen(filename, "a");
    if (csv_file == NULL)
    {
        fprintf(stderr, "\nError: Unable to open or initialize local metrics log sheet inside "
                        "output directory.\n");
        return;
    }

    if (is_new)
    {
        fprintf(csv_file, "Algorithm/Array Type,Input Size,Execution Time (Seconds),Timestamp\n");
    }

    time(&raw_time);
    time_info = localtime(&raw_time);
    if (time_info == NULL)
    {
        fclose(csv_file);
        fprintf(stderr, "\nError: Unable to resolve local time for benchmark logging.\n");
        return;
    }

    if (strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info) == 0)
    {
        fclose(csv_file);
        fprintf(stderr, "\nError: Unable to format timestamp for benchmark logging.\n");
        return;
    }

    fprintf(csv_file, "%s,%d,%f,%s\n", algo_name, data_size, execution_time, timestamp);
    fclose(csv_file);
}
