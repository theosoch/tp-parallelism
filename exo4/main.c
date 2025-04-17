#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// 

#define MAX_OBJ_COUNT 100

void readProblem(
    const char *filename,
    int* weights,
    int* utilities,
    int* objectCountPtr,
    int* maxCapacityPtr
){
    char line[256];

    FILE *problem = fopen(filename,"r");
    if (problem == NULL){
        fprintf(stderr,"File %s not found.\n",filename);
        exit(-1);
    }

    while (fgets(line, 256, problem) != NULL){
        switch(line[0]){
        case 'c': // capacity
            if (sscanf(&(line[2]),"%d\n", maxCapacityPtr) != 1){
                fprintf(stderr,"Error in file format in line:\n");
                fprintf(stderr, "%s", line);
                exit(-1);
            }
            break;

        case 'o': // graph size
            if (*objectCountPtr >= MAX_OBJ_COUNT){
                fprintf(stderr,"Too many objects (%d): limit is %d\n", *objectCountPtr, MAX_OBJ_COUNT);
                exit(-1);
            }

            if (sscanf(&(line[2]),"%d %d\n", &(weights[*objectCountPtr]), &(utilities[*objectCountPtr])) != 2){
                fprintf(stderr,"Error in file format in line:\n");
                fprintf(stderr, "%s", line);
                exit(-1);
            }
            else
                (*objectCountPtr)++;
            
            break;

        default:
            break;
        }
    }

    if (*objectCountPtr == 0){
        fprintf(stderr,"Could not find any object in the problem file. Exiting.");
        exit(-1);
    }
}

//

void binpacking(
    const int* weights,
    const int* utilities,
    const int objectCount,
    const int maxCapacity,
    int** bag
) {
    bag = malloc(objectCount*sizeof(int*));

    bag[0] = malloc((maxCapacity+1)*sizeof(int));
    for(int j = 0; j < maxCapacity+1; ++j) {
        bag[0][j] = weights[0] <= j ? utilities[0] : 0;
    }

    for(int i = 1; i < objectCount; ++i) {
        bag[i] = malloc((maxCapacity+1)*sizeof(int));

        for(int j = 0; j < maxCapacity+1; ++j) {
            bag[i][j] = weights[i] <= j
                ? bag[i-1][j-weights[i]] + utilities[0]
                : bag[i-1][j]
            ;
        }
    }
}

//  

#define PROBLEM_COUNT 3
#ifdef DEV
    #define PROBLEM_FILEPATHS { "./exo4/pb1.txt", "./exo4/pb2.txt", "./exo4/pb6.txt" }
#else
    #define PROBLEM_FILEPATHS { "./pb1.txt", "./pb2.txt", "./pb6.txt" }
#endif

// 

int main() {
    const char* problemFilepaths[PROBLEM_COUNT] = PROBLEM_FILEPATHS;

    // 
    
    int objectCount = 0;
    int maxCapacity;
    int weights[MAX_OBJ_COUNT];
    int utilities[MAX_OBJ_COUNT];
    
    int** bag = NULL;
    
    // 

    for(int i = 0; i < PROBLEM_COUNT; ++i) {
        const char* problemFilepath = problemFilepaths[i];
        
        readProblem(problemFilepath, weights, utilities, &objectCount, &maxCapacity);
        binpacking(weights, utilities, objectCount, maxCapacity, bag);
    }

    return 0;
}