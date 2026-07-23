#ifndef BIT_MANIPULATION_H
#define BIT_MANIPULATION_H

int set_bit(int n, int k);
int clear_bit(int n, int k);
int toggle_bit(int n, int k);
int check_bit(int n, int k);

int count_set_bits(int n);
int is_power_of_two(int n);
void xor_swap(int* a, int* b);

int get_rightmost_set_bit(int n);
int turn_off_rightmost_set_bit(int n);
unsigned int reverse_bits(unsigned int n);

#endif // BIT_MANIPULATION_H
