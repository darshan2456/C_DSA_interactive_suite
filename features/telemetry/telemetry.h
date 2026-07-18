#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdbool.h>

void set_telemetry_enabled(bool enabled);
bool is_telemetry_enabled(void);

void set_telemetry_filepath(const char* filepath);
const char* get_telemetry_filepath(void);

void telemetry_init(const char* algorithm_name);
void telemetry_log_step(const int* arr, int size, const char* event_msg);
void telemetry_export_state(const char* algorithm_name, int step, const int* arr, int size,
                            const char* event_msg);
void telemetry_close(void);

#endif // TELEMETRY_H
