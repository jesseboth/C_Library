#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void hex_dump(void *start, size_t bytes);
int mem_cpy(void *src, void *dest, size_t bytes);
int str_len(char *str);