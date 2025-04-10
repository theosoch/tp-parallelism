int exo3(const int argc, const char* argv[]) {
    return 0;
}

// 

#ifndef EMBEDDED
int main(const int argc, const char* argv[]) { return exo3(argc, argv); }
#else
#include "./exo1.h"
#endif