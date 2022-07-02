#include "timer.h"

clock_t reset_time(int t){
    static clock_t time;
    if(t){
        time = clock();
        return time;
    }
    else{
        return time;
    }
}

double get_elapsed(){
    return (double)(clock() - reset_time(0)) / CLOCKS_PER_SEC;
}