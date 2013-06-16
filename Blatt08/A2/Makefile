#Compiler
CC = g++

#***Compiler-Optionen***
# -Wall: Enables all compiler warnings
# -pedantic: Issue all the warnings demanded by strict ISO C and ISO C++
# -std=c++11: Enables c++11 support
# -fopenmp: Benutze openmp
# -O2: Zweite Optimierungsstufe
CFLAGS = -Wall -pedantic -std=c++11 -fopenmp

#***Linker-Optionen***
# -L/usr/local/opt/lapack/lib: Verwende mit homebrew installierte lapack-routinen
LDFLAGS = -L/usr/local/opt/lapack/lib -lgfortran -llapack -lblas

all: A2

test: A2.o
	$(CC) $(CFLAGS) test.o $(LDFLAGS) -o test
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@