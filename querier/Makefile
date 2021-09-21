# Makefile for 'crawler' module
#
# Annabel Revers, February 2021

C = ../common
L = ../libcs50

PROGS = querier
LIBS = 
LLIBS = $C/common.a $L/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L -I$C 
CC = gcc
MAKE = make

.PHONY: test valgrind clean

all: $(PROGS)

querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: $C/pagedir.h $C/index.h $L/file.h $L/counters.h

test: $(PROG) testing.sh
	bash -v testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

