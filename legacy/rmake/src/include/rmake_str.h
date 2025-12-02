#ifndef SAFE_STR_HH
#define SAFE_STR_HH

#include <stdio.h>
#include <stddef.h>

#undef bool
#define bool unsigned char

#undef true
#define true 1

#undef false
#define false 0

void str_trim(char* str);
void str_replace(char* str, unsigned int size, char old, char new);
void str_split(char* str, char at, unsigned int col, char dst[2][col], bool incSplChar);
void str_split_at_occ(char* str, char at, int occ, unsigned int col, char dst[2][col], bool incSplChar);

unsigned short str_contain(char* str, char c);
unsigned short str_contain_str(char* str, char* sequence);

unsigned int str_size(char* str);
bool str_cmp(char* str, char* rts);
void str_mset(char* str, unsigned int size, unsigned char value);
void str_lower(char* str);
void str_concat(char* dst,  unsigned int dst_max_size, char* src);
void str_remove_char(char* str, char rm);

#endif
