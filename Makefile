# CC = Your compiler
CC=gcc

# CFLAGS = CompilatorFLAGS
CFLAGS=-c

all: program

program: main.o processing.o
	$(CC) main.o processing.o -o program

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

processing.o: processing.cpp
	$(CC) $(CFLAGS) processing.cpp

clean:
	rm -rf *.o program