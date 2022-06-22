#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "util.h"
#include "macros.h"
#include "array.h"
#include "geom.h"
#include "timer.h"

int *selection_sort(int *arr, int length){
    int least, least_j;
    for(int i = 0; i < length; i++){
        least = INT32_MAX;
        least_j = -1;
        for(int j = i; j < length; j++){
            if(arr[j] < least){
                least = arr[j];
                least_j = j;
            }
        }
        arr[least_j] = arr[i];
        arr[i] = least;
    }
    return arr;
}


int main(int argc, char **argv){


    int dim[3] = {2, 2, 5};
    int ***test = multi_array_create(dim, 3, sizeof(int));
    printf("%p\n", test);

    int count = 0;
    for(int i = 0; i < dim[0]; i++){
        for(int j = 0; j < dim[1]; j++){
            for(int k = 0; k < dim[0]; k++){
                printf("%d\n", count);
                test[i][j][k] = count++;
            }
        }

    }

    for(int i = 0; i < dim[0]; i++){
        printf("[");
        for(int j = 0; j < dim[1]; j++){
            for(int k = 0; k < dim[2]; k++){
                printf("%d, ", test[i][j][k]);
            }
            printf("]");
        }
        printf("\n");
    }


    return 0;
}