#ifndef BENCHMARK_H
#define BENCHMARK_H

extern long long benchmark_comparisons;
extern long long benchmark_swaps;
extern int benchmark_active;

void benchmark_reset(void);
void disable_stdout(void);
void restore_stdout(void);
void run_benchmark_menu(void);

#endif // BENCHMARK_H
