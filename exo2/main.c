#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

// 

#define USED_THREAD_COUNT 8
#define SIZE (int) (30000)

// 

int sequential_mat_sum(const int* M, const int Msize, int* usedThreadCountPtr) {
    int result = 0;
    const int Mtablength = Msize*Msize;
    
    // Here only to retrieve information data for results comparison
    *usedThreadCountPtr = 1;

    // 

    for(int i = 0; i < Mtablength; ++i)
        result += M[i];

    // 

    return result;
}

// 

int parallelized1_mat_sum(const int* M, const int Msize, int* usedThreadCountPtr) {
    int result = 0;
    const int Mtablength = Msize*Msize;

    // 

    int partialsums[USED_THREAD_COUNT];

    #pragma omp parallel num_threads(USED_THREAD_COUNT)
    {
        #pragma omp single
        // Here only to retrieve information data for results comparison
        *usedThreadCountPtr = omp_get_num_threads();

        // 

        const int threadNum = omp_get_thread_num();

        partialsums[threadNum] = 0;

        #pragma omp for
        for(int i = 0; i < Mtablength; ++i)
            partialsums[threadNum] += M[i];

        #pragma omp single
        for(int i = 0; i < USED_THREAD_COUNT; ++i)
            result += partialsums[i];
    }

    // 

    return result;
}

// 

int parallelized2_1_mat_sum(const int* M, const int Msize, int* usedThreadCountPtr) {
    int result = 0;
    const int Mtablength = Msize*Msize;

    // 

    #pragma omp parallel num_threads(USED_THREAD_COUNT)
    {
        #pragma omp single
        // Here only to retrieve information data for results comparison
        *usedThreadCountPtr = omp_get_num_threads();

        // 

        int partialsum = 0;

        #pragma omp for
        for(int i = 0; i < Mtablength; ++i)
            partialsum += M[i];

        // Mutex incrementation
        #pragma omp critical
        result += partialsum;
    }

    // 

    return result;
}

// 

int parallelized2_2_mat_sum(const int* M, const int Msize, int* usedThreadCountPtr) {
    int result = 0;
    const int Mtablength = Msize*Msize;

    // 

    #pragma omp parallel num_threads(USED_THREAD_COUNT)
    {
        #pragma omp single
        // Here only to retrieve information data for results comparison
        *usedThreadCountPtr = omp_get_num_threads();

        // 

        int partialsum = 0;

        #pragma omp for
        for(int i = 0; i < Mtablength; ++i)
            partialsum += M[i];

        // Mutex incrementation
        #pragma omp atomic
        result += partialsum;
    }

    // 

    return result;
}

// 

int parallelized3_mat_sum(const int* M, const int Msize, int* usedThreadCountPtr) {
    int result = 0;
    const int Mtablength = Msize*Msize;

    // 

    #pragma omp parallel num_threads(USED_THREAD_COUNT)
    {
        #pragma omp single
        // Here only to retrieve information data for results comparison
        *usedThreadCountPtr = omp_get_num_threads();

        // 

        #pragma omp for reduction(+:result)
        for(int i = 0; i < Mtablength; ++i)
            result += M[i];
    }

    // 

    return result;
}

// 

void benchmarkTest(const char* casename, int (*sumcalc)(const int* M, const int Msize, int* threadcount), const int* M, const int Msize) {
    int threadcount = 1;
    
    const double start = omp_get_wtime();
    const int result = sumcalc(M, Msize, &threadcount);
    const double stop = omp_get_wtime();

    const double t = stop-start;

    printf("%s,%d,%f,%d,%d\n", casename, threadcount, t, Msize, result);
}

// 

int main() {
    const int Mtablength = SIZE*SIZE;
    int* M = (int*) malloc(Mtablength*sizeof(int));

    // 

    #pragma omp parallel for
    for(int i = 0; i < Mtablength; i++) M[i] = 1;

    // 

    printf("case,threadcount,exectime,matsize,result\n");

    benchmarkTest("sequential", sequential_mat_sum, M, SIZE);
    benchmarkTest("partial sums", parallelized1_mat_sum, M, SIZE);
    benchmarkTest("global sum incremented by each thread (critical)", parallelized2_1_mat_sum, M, SIZE);
    benchmarkTest("global sum incremented by each thread (atomic)", parallelized2_2_mat_sum, M, SIZE);
    benchmarkTest("reduction", parallelized3_mat_sum, M, SIZE);

    // 

    free(M);

    return EXIT_SUCCESS;
}