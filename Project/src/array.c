#include "array.h"
#include "stdio.h"
#include "stdlib.h"

#include "util.h"
#include "print.h"

struct _array_data{
    unsigned long int dimension;
    unsigned int type_size;
    void *arr;
};

void *array_create(int dimension, int type_size){
    void *data = malloc(sizeof(struct _array_data) + (type_size * dimension));
    
    struct _array_data *meta = (struct _array_data *)data;
    meta->dimension = dimension;
    meta->type_size = type_size;
    meta->arr = (data+sizeof(struct _array_data));
    return meta->arr;
}

void array_destroy(void *arr){
    void *data = (arr - sizeof(struct _array_data));
    struct _array_data *meta = data;
    mem_clear(data, sizeof(struct _array_data) + (meta->dimension * meta->type_size));


    free(data);
}

int ptr_space(int *dimensions, int num_dimensions, int offset){
    int spaces = 1;
    for(int i = (num_dimensions-1)-offset; i >= 0; i--){
        spaces *= dimensions[i];
    }
    return spaces;
}

void *multi_array_create(int *dimensions, int num_dimensions, int type_size){    
    unsigned long int d = 1;
    unsigned long int elems = 1;
    for(int i = num_dimensions-1; i >= 0; i--){
        if(dimensions[i] < 0){
            return NULL;
        }
        else{
            elems *= dimensions[i];
            d = (i < num_dimensions-1) ? d * dimensions[i] + dimensions[i] : 0;
        }
    }

    printf("%lu, %lu\n", d, elems);

    size_t data_size = sizeof(struct _array_data) + (type_size * elems) + (d * sizeof(int *));

    void *data = malloc(data_size);

    struct _array_data *meta = data;
    meta->dimension = d;
    meta->type_size = type_size;
    meta->arr = (data+sizeof(struct _array_data));

    void *ptr = data + data_size;
    void *e_cur = align(ptr - (elems * type_size), sizeof(int **));
    if(!(elems & 1)){
        e_cur -= sizeof(int **);
    }

    void *s_cur;
    int offset = 1;
    int index = num_dimensions-1;
    int **data_ptr;
    for(int i = ptr_space(dimensions, num_dimensions, offset); i > 0; i--){
        // ptr -= (dimensions[index] * type_size);
        for(int i = dimensions[index]; i > 0; i--){
            pprint(ptr);
            *(int *)ptr = 8;
            ptr -= type_size;
        }
        *(int **)e_cur = ptr;
        e_cur-=sizeof(int *);

    }

    ptr = e_cur + ptr_space(dimensions, num_dimensions, offset) * sizeof(int **);
    e_cur = align(e_cur, sizeof(int **));
    while(e_cur != meta->arr){
        *(int **)e_cur = ptr;

        printf("ptr: %p | cur: %p\n", ptr, *(int **)e_cur);
        ptr-=sizeof(int *);
        e_cur -= sizeof(int *);
    }
    *(int **)e_cur = ptr;
    hex_dump(meta->arr, data_size);
    printf("done\n");

    return meta->arr;
}

void set_data(int *index, int len_index, void *arr, int data){
    struct _array_data *meta = arr - sizeof(struct _array_data);

    int **ptr = arr;
    for(int i = 0; i < len_index-1; i++){
        ptr += index[i];
        *ptr = **ptr;
    }    
    ptr+=index[len_index-1];
    **ptr = data;
}

void *get_data(int *index, int len_index, void *arr){
    struct _array_data *meta = arr - sizeof(struct _array_data);

    int *ptr = arr;
    for(int i = 0; i < len_index-1; i++){
        ptr += index[i];
        ptr = *ptr;
    }    
    return ptr+index[len_index-1];
}