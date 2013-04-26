# Generic GNUMakefile
 
    # Just a snippet to stop executing under other make(1) commands
    # that won't understand these lines
    ifneq (,)
    This makefile requires GNU Make.
    endif
 
    PROGRAM = A1
    C_FILES := $(wildcard *.c)
    CPP_FILES := $(wildcard *.cpp)
    OBJS := $(patsubst %.c, %.o, $(C_FILES))
    CC = gcc
    
    #***Compiler-Optionen***
    # -Wall: Enables all compiler warnings
    # -pedantic: Issue all the warnings demanded by strict ISO C and ISO C++
    # -std=c++11: Enables c++11 support
    CFLAGS = -Wall -pedantic -std=c++11
    
    #***Linker-Optionen***
    LDFLAGS =
 
    all: $(PROGRAM)
 
    $(PROGRAM): .depend $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(PROGRAM)
 
    depend: .depend
 
    .depend: cmd = gcc -MM -MF depend $(var); cat depend >> .depend;
    .depend:
		@echo "Generating dependencies..."
		@$(foreach var, $(C_FILES), $(cmd))
		@rm -f depend
 
    -include .depend
 
    # These are the pattern matching rules. In addition to the automatic
    # variables used here, the variable $* that matches whatever % stands for
    # can be useful in special cases.
    %.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@
 
    %: %.c
		$(CC) $(CFLAGS) -o $@ $<
 
    clean:
		rm -f .depend *.o
 
    .PHONY: clean depend