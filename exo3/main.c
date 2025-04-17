#include <omp.h>
#include <stdio.h>

// 

#define NUM_STEPS 1000
#define NUM_OPS 50

// 

int case1() {
    int num = 0;
    
    for (int i=0 ; i<NUM_STEPS ; i++){
        for (int j=0 ; j<NUM_OPS ; j++)
            num++;
        for (int j=0 ; j<NUM_OPS ; j++)
            num--;
    }

    return num;
}

// 

int case2() {
    int num = 0;
    
    #pragma omp parallel for num_threads(4)
    for (int i=0 ; i<NUM_STEPS ; i++){
        for (int j=0 ; j<NUM_OPS ; j++)
            num++;
        for (int j=0 ; j<NUM_OPS ; j++)
            num--;
    }

    return num;
}

// 

int case3() {
    int num = 0;
    
    #pragma omp parallel for reduction(+:num) num_threads(4)
    for (int i=0 ; i<NUM_STEPS ; i++) {
        for (int j=0 ; j<NUM_OPS ; j++)
            num++;
        for (int j=0 ; j<NUM_OPS ; j++)
            num--;
    }

    return num;
}

// 

int main() {
    int (*casefunctab[3])(void) = { case1, case2, case3 };

    printf("case,exectime,result\n");
    for(int i = 0; i < 3; ++i) {
        int (*casefunc)(void) = casefunctab[i];

        const double start = omp_get_wtime();
        const int result = casefunc();
        const double stop = omp_get_wtime();

        const double t = stop-start;
        printf("%d,%f,%d\n", i, t, result);
    }

    return 0;
}