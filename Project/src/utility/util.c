#include "util.h"

void print_hexbyte(uint8_t byte);
void print_hexnibble(uint8_t nibble);
void print_byte(uint8_t byte);
void print_row(uint8_t *ptr);

#define ROW_LENGTH 0x10

void hex_dump(void *start, size_t bytes){
    int i = bytes;
    while((uint64_t)start % ROW_LENGTH){
        start--;
        i++;
    }

    while(i > 0){
        print_row(start);
        i-=ROW_LENGTH;
        start+=ROW_LENGTH;
    }
}

int mem_cpy(void *src, void *dest, volatile size_t bytes){
    uint8_t *src_8 = src;
    uint8_t *dest_8 = dest;

    if(src == NULL || dest == NULL){
        return 0;
    }

    for(; bytes > 0 && ((uintptr_t)src_8 & 0x3) != 0; bytes--, src_8++, dest_8++){
        *dest_8 = *src_8;
    }

    uint32_t *src_32 = (uint32_t *)src_8;
    uint32_t *dest_32 = (uint32_t *)dest_8;
    for(; bytes > 3; bytes-=4, src_32++, dest_32++){
        *dest_32 = *src_32;
    }

    src_8 = (uint8_t *)src_32;
    dest_8 = (uint8_t *)dest_32;
    for(; bytes > 0; bytes--, src_8++, dest_8++){
        *dest_8 = *src_8;
    }

    return 1;
}

int mem_clear(void *src, size_t bytes){
    uint8_t *src_8 = src;

    if(src == NULL){
        return 0;
    }

    for(; ((uintptr_t)src_8 & 0x3) != 0; src_8++, bytes--){
        *src_8 = (uint8_t)0;
    }

    uint32_t *src_32 = (uint32_t *)src_8;
    for(; bytes > 3; bytes--, src_32++){
        *src_32 = (uint32_t)0;
    }

    src_8 = (uint8_t *)src_32;
    for(; bytes > 0; bytes--, src_8++){
        *src_8 = (uint8_t)0;
    }

    return 1;
}

int str_len(char *str){
    int i;
    for(i = 0; *str; i++, str++);
    return i;
}




void print_hexbyte(uint8_t byte){
    print_hexnibble((byte & 0xF0) >> 4);    // upper
    print_hexnibble(byte & 0x0F);           // lower
}

void print_hexnibble(uint8_t nibble){
    if(nibble < 10){
        printf("%d", nibble);
    }
    else if(nibble < 16){
        printf("%c", (nibble+55));
    }
    else{
        printf("-");
    }
}

void print_byte(uint8_t byte){
    if(byte < 0x20 || byte > 0x7F){
        printf(".");
    }
    else{
        printf("%c", byte);
    }
}

void print_row(uint8_t *ptr){
    uint8_t *chars = ptr;
    printf("%p ", ptr);
    
    for(int i = 0; i < ROW_LENGTH; i++, ptr++){
        printf(" ");
        print_hexbyte(*ptr);
    }

    printf(" | ");
    for(int i = 0; i < ROW_LENGTH; i++, chars++){
        print_byte(*chars);
    }
    printf("\n");
}

int powerOf2(size_t val){
    size_t test = 1;
    for(int i = 1; test < val; i++){
        test = 1 << i;
    }
    if(val == test) return 1;

    return 0;
}

void *align(void *ptr, uint8_t by){
    if(ptr == NULL){
        return NULL;
    }
    if(!powerOf2(by)){
        printf("Invalid alignment\n");
        return NULL;
    }

    for(; (uintptr_t)ptr & (by-1); ptr--);

    return ptr;
}