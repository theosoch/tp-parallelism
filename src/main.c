#include <stdio.h>
#include <stdlib.h>

// 

#include "./exo1.h"
#include "./exo2.h"
#include "./exo3.h"

// 

int str2int(const char* str) {
    const char firstchar = str[0];
    if(firstchar == '\0') return 0;

    const int sign = firstchar == '-' ? -1 : 1;

    int p = 1;
    int result = 0;

    char c;
    for(int i = sign == 1 ? 0 : 1; (c = str[i]) != '\0'; ++i) {
        result += p*atoi(&c);
        p *= 10;
    }

    return result*sign;
}

// 

int main(const int argc, const char* argv[]) {
    if(argc != 2) {
        printf("Invalid syntax: main <exercise number>\n");
        return 1;
    }

    const int exonum = str2int(argv[1]);

    int (*exofunc)(const int argc, const char* argv[]);
    switch (exonum) {
        case 1: exofunc = exo1; break;
        case 2: exofunc = exo2; break;
        case 3: exofunc = exo3; break;
        default: exofunc = NULL; break;
    }

    if(exofunc == NULL) {
        printf("Exercise n°%d not found.\n", exonum);
        return 1;
    }
    
    return exofunc(argc, argv);
}