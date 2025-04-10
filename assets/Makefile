COMPILER=gcc
COMPILER_FLAGS=-std=c23 -Wall -Wextra -g -pedantic -Wformat=0
LIBRARIES_FLAGS=-lm -fopenmp

all:
	$(COMPILER) $(COMPILER_FLAGS) -c ./main.c -o ./main.o $(LIBRARIES_FLAGS)
	$(COMPILER) $(COMPILER_FLAGS) ./main.o -o ./main $(LIBRARIES_FLAGS)

clean:
	rm -rf *.o main