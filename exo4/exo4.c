#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// 

#define MAX_NUM_OBJ 100

int num_obj = 0;
int capacity;
int weight[MAX_NUM_OBJ];
int utility[MAX_NUM_OBJ];

void readProblem(char *filename){
  char line[256];

  FILE *problem = fopen(filename,"r");
  if (problem == NULL){
    fprintf(stderr,"File %s not found.\n",filename);
    exit(-1);
  }

  while (fgets(line, 256, problem) != NULL){
    switch(line[0]){
    case 'c': // capacity
      if (sscanf(&(line[2]),"%d\n", &capacity) != 1){
	fprintf(stderr,"Error in file format in line:\n");
	fprintf(stderr, "%s", line);
	exit(-1);
      }
      break;

    case 'o': // graph size
      if (num_obj >= MAX_NUM_OBJ){
	fprintf(stderr,"Too many objects (%d): limit is %d\n", num_obj, MAX_NUM_OBJ);
	exit(-1);
      }
      if (sscanf(&(line[2]),"%d %d\n", &(weight[num_obj]), &(utility[num_obj])) != 2){
	fprintf(stderr,"Error in file format in line:\n");
	fprintf(stderr, "%s", line);
	exit(-1);
      }
      else
	num_obj++;
      break;

    default:
      break;
    }
  }
  if (num_obj == 0){
    fprintf(stderr,"Could not find any object in the problem file. Exiting.");
    exit(-1);
  }

}

//

void binpacking(const int objectCount, const int maxCapacity) {
    int M[objectCount];
    int U[maxCapacity];

    int S[maxCapacity][objectCount+1];

    for(int j = 0; j < maxCapacity+1; ++j) {
        S[0][j] = M[0] <= j ? U[0] : 0;
    }

    for(int i = 1; i < objectCount; ++i) {
        for(int j = 0; j < maxCapacity+1; ++j) {
            S[i][j] = M[i] <= j
                ? S[i-1][j-M[i]] + U[0]
                : S[i-1][j]
            ;
        }
    }
}

//  

int exo4(const int argc, const char* argv[]) {
    // read_problem()

    return 0;
}

// 

#ifndef EMBEDDED
int main(const int argc, const char* argv[]) { return exo4(argc, argv); }
#else
#include "./exo4.h"
#endif
