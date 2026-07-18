#include "telemetry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static bool telemetry_enabled = false;
static char telemetry_filepath[512] = "benchmarks/algo_trace.json";
static bool is_first_entry = true;

static void ensure_parent_dir_exists(const char* filepath)
{
    char temp[512];
    strncpy(temp, filepath, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    char* last_slash = strrchr(temp, '/');

    if (last_slash != NULL)
    {
        *last_slash = '\0';
        if (strlen(temp) > 0)
        {
            struct stat st;
            if (stat(temp, &st) == -1)
            {
                mkdir(temp, 0755);
            }
        }
    }
}

void set_telemetry_enabled(bool enabled)
{
    telemetry_enabled = enabled;
}

bool is_telemetry_enabled(void)
{
    return telemetry_enabled;
}

void set_telemetry_filepath(const char* filepath)
{
    if (filepath != NULL && strlen(filepath) > 0)
    {
        strncpy(telemetry_filepath, filepath, sizeof(telemetry_filepath) - 1);
        telemetry_filepath[sizeof(telemetry_filepath) - 1] = '\0';
    }
}

const char* get_telemetry_filepath(void)
{
    return telemetry_filepath;
}

static char current_algorithm_name[128] = "unknown";
static int current_step_count = 0;

void telemetry_init(const char* algorithm_name)
{
    if (!telemetry_enabled)
    {
        return;
    }

    if (algorithm_name != NULL)
    {
        strncpy(current_algorithm_name, algorithm_name, sizeof(current_algorithm_name) - 1);
        current_algorithm_name[sizeof(current_algorithm_name) - 1] = '\0';
    }
    else
    {
        strcpy(current_algorithm_name, "unknown");
    }

    ensure_parent_dir_exists(telemetry_filepath);

    FILE* fp = fopen(telemetry_filepath, "w");
    if (fp == NULL)
    {
        return;
    }

    fprintf(fp, "[\n");
    fclose(fp);
    is_first_entry = true;
    current_step_count = 0;
}

void telemetry_log_step(const int* arr, int size, const char* event_msg)
{
    telemetry_export_state(current_algorithm_name, current_step_count++, arr, size, event_msg);
}

void telemetry_export_state(const char* algorithm_name, int step, const int* arr, int size,
                            const char* event_msg)
{
    if (!telemetry_enabled)
    {
        return;
    }

    FILE* fp = fopen(telemetry_filepath, "a");
    if (fp == NULL)
    {
        return;
    }

    if (!is_first_entry)
    {
        fprintf(fp, ",\n");
    }
    is_first_entry = false;

    fprintf(fp, "  {\n");
    fprintf(fp, "    \"algorithm\": \"%s\",\n", algorithm_name ? algorithm_name : "unknown");
    fprintf(fp, "    \"step\": %d,\n", step);

    if (arr != NULL && size >= 0)
    {
        fprintf(fp, "    \"array\": [");
        for (int i = 0; i < size; i++)
        {
            fprintf(fp, "%d", arr[i]);
            if (i < size - 1)
            {
                fprintf(fp, ", ");
            }
        }
        fprintf(fp, "],\n");
    }
    else
    {
        fprintf(fp, "    \"array\": null,\n");
    }

    fprintf(fp, "    \"message\": \"%s\"\n", event_msg ? event_msg : "");
    fprintf(fp, "  }");

    fclose(fp);
}

void telemetry_close(void)
{
    if (!telemetry_enabled)
    {
        return;
    }

    FILE* fp = fopen(telemetry_filepath, "a");
    if (fp == NULL)
    {
        return;
    }

    fprintf(fp, "\n]\n");
    fclose(fp);
    is_first_entry = true;
    current_step_count = 0;
}
