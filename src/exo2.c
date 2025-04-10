#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

// 

#define USED_THREAD_COUNT 8
#define SIZE 20000

// 

int sequential_mat_sum(const int* M, const int Msize, int* usedThreadCount) {
    int result = 0;
    
    *usedThreadCount = 1;

    // 

    for(int i = 0; i < Msize; ++i) {
        for(int j = 0; j < Msize; ++j) {
            result += M[i*Msize + j];
        }
    }

    // 

    return result;
}

// 

int parallelized1_mat_sum(const int* M, const int Msize, int* usedThreadCount) {
    int result = 0;

    // 

    int partialsums[USED_THREAD_COUNT];

    #pragma omp parallel num_threads(USED_THREAD_COUNT)
    {
        *usedThreadCount = omp_get_num_threads();

        const int threadNum = omp_get_thread_num();

        partialsums[threadNum] = 0;

        #pragma omp for
        for(int i = 0; i < Msize; ++i) {
            for(int j = 0; j < Msize; ++j) {
                partialsums[threadNum] += M[i*Msize + j];
            }
        }
    }

    for(int i = 0; i < USED_THREAD_COUNT; ++i) {
        result += partialsums[i];
    }

    // 

    return result;
}

// 

int parallelized2_mat_sum(const int* M, const int Msize, int* usedThreadCount) {
    int result = 0;

    // 

    #pragma omp parallel num_threads(USED_THREAD_COUNT)
    {
        *usedThreadCount = omp_get_num_threads();

        int partialsum = 0;

        #pragma omp for
        for(int i = 0; i < Msize; ++i) {
            for(int j = 0; j < Msize; ++j) {
                partialsum += M[i*Msize + j];
            }
        }

        #pragma omp critical
        {
            result += partialsum;
        }
    }

    // 

    return result;
}

// 

int parallelized3_mat_sum(const int* M, const int Msize, int* usedThreadCount) {
    int result = 0;

    // 

    #pragma omp parallel num_threads(USED_THREAD_COUNT)
    {
        *usedThreadCount = omp_get_num_threads();

        #pragma omp for reduction(+:result)
        for(int i = 0; i < Msize; ++i) {
            for(int j = 0; j < Msize; ++j) {
                result += M[i*Msize + j];
            }
        }
    }

    // 

    return result;
}

// 

void benchmarkTest(const char* casename, int (*sumcalc)(const int* M, const int Msize, int* threadcount), const int* M, const int Msize) {
    double start = omp_get_wtime();

    // 

    int threadcount = 1;
    const int result = sumcalc(M, Msize, &threadcount);

    // 

    double stop = omp_get_wtime();
    double t = stop-start;

    printf("%s,%d,%f,%d\n", casename, threadcount, t, result);
}

// 

int exo2(const int argc, const char* argv[]) {
    int* M = (int*) malloc(SIZE*SIZE*sizeof(int));

    // Initialisations
    #pragma omp parallel
    {
        #pragma omp for
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j < SIZE; j++){
                M[i*SIZE + j] = 1;
            }
        }
    }

    // Calcul en faisant varier le nombre de threads
    printf("case,threadcount,exectime,result\n");

    benchmarkTest("sequential", sequential_mat_sum, M, SIZE);
    benchmarkTest("partial sums", parallelized1_mat_sum, M, SIZE);
    benchmarkTest("partial sums global increment", parallelized2_mat_sum, M, SIZE);
    benchmarkTest("reduction", parallelized3_mat_sum, M, SIZE);

    // 

    // Liberations
    free(M);

    return EXIT_SUCCESS;
}

// 

#ifndef EMBEDDED
int main(const int argc, const char* argv[]) { return exo2(argc, argv); }
#else
#include "./exo1.h"
#endif