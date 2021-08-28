# CC = Your compiler
CC=gcc

# CFLAGS = CompilatorFLAGS
CFLAGS=-c

all: program

program: main.o solve.o
	$(CC) main.o solve.o -o program

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

solve.o: solve.cpp
	$(CC) $(CFLAGS) solve.cpp

clean:
	rm -rf *.o program