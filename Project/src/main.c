#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "utility/util.h"
#include "utility/macros.h"
#include "utility/geom.h"
#include "utility/timer.h"

#include "array.h"
#include "linked_lib.h"

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

    test();

    return 0;
}