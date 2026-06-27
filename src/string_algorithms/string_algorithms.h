#ifndef STRING_ALGORITHMS_H
#define STRING_ALGORITHMS_H

void string_algorithms_demo(void);

void naive_string_matching_demo(void);
void naive_string_matching(char* text, char* pattern);

void kmp_demo(void);
void kmp_search(char* text, char* pattern);
void kmp_visualization(char* text, char* pattern);

void rabin_karp_demo(void);
void rabin_karp_search(char* text, char* pattern, int q);
void rabin_karp_visualization(char* text, char* pattern, int q);

void boyer_moore_search(char* text, char* pattern);
void boyer_moore_visualization(char* text, char* pattern);
void boyer_moore_demo(void);

#endif // STRING_ALGORITHMS_H
