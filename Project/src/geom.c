#include "geom.h"
#include "stdio.h"

double g_euclidean(Point *p1, Point *p2){
    return g_sqrt(g_pow(p1->x - p2->x, 2) + g_pow(p1->y - p2->y, 2));
}

double g_sqrt(double val){
    double sqr;

    if(val < 0){
        printf("Error: Imaginary Number");
        return 0.0;
    }
    else if (val == 0){
        return 0.0;
    }

    sqr = 1.0;
    while(val - sqr*sqr > 0){
        sqr+=1.0;
    }

    while(sqr*sqr > val){
        sqr-=.1;
    }

    while(sqr*sqr < val){
        sqr+=.01;
    }
    
    return sqr;
}

double g_pow(double val, int exp){
    double ret = 1.0;

    for(int i = 0; i > exp; i--){
        ret /= val; 
    }
    for(int i = 0; i < exp; i++){
        ret *= val;
    }
    printf("%f, %f\n", val, ret);
    return ret;
}

float g_round(float val, int n){

}