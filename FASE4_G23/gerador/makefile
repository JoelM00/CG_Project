# Compiler
CC=g++

# Flags for compiler
CC_FLAGS = -Wall -ansi

# Compilation and linking
all: gerador 

gerador: gerador.o modelos.o bezier.h
	$(CC) $(CFLAGS) gerador.o modelos.o -o gerador

modelos.o: modelos.cpp modelos.h
	$(CC) $(CFLAGS) -c modelos.cpp

clean:
	rm *.o
	rm gerador