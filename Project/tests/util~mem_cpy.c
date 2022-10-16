#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#include "../src/utility/util.h"

int main(int argc, char *argv[]) {
    char *src_str = "Hello World\n";
    void *ptr = malloc(str_len(src_str));
    char *dest_str = ptr;

    if(!mem_cpy(src_str, dest_str, str_len(src_str))){
        return 1;
    };
    for(; *src_str && *dest_str; src_str++, dest_str++){
        if(*src_str != *dest_str){
            return 1;
        }
    }

    free(ptr);

    int src_int[] = {0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666};
    size_t src_length = sizeof(src_int) / sizeof(int);
    ptr = malloc(sizeof(src_int)+1);
    void *dest_int = ptr+1;



    if(!(mem_cpy(src_int, dest_int, sizeof(src_int)))){
        return 1;
    };

    uint8_t *src, *dest;

    src = (uint8_t *)src_int;
    dest = (uint8_t *)dest_int;
    for(int i = 0; i < src_length*sizeof(int); i++, src++, dest++){
        if(*src != *dest){
            return 1;
        }
    }

    free(ptr);
    return 0;
}