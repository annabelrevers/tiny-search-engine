# Makefile for 'crawler' module
#
# Annabel Revers, February 2021

C = ../common
L = ../libcs50

PROGS = indexer indextest
LIBS = 
LLIBS = $C/common.a $L/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L -I$C 
CC = gcc
MAKE = make

.PHONY: test valgrind clean

all: $(PROGS)

indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o:	$C/pagedir.h $L/webpage.h $L/hashtable.h $L/file.h $L/counters.h $C/index.h
indextest.o: $L/hashtable.h $C/index.h

test: $(PROG) testing.sh
	bash -v testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

