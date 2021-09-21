# Makefile for 'crawler' module
#
# Annabel Revers, February 2021

C = ../common
L = ../libcs50

PROGS = crawler
LIBS = 
LLIBS = $C/common.a $L/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L 
CC = gcc
MAKE = make

.PHONY: test valgrind clean

all: $(PROGS)

crawler: crawler.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: $L/webpage.h $C/pagedir.h
crawler.o: $L/memory.h $L/bag.h $L/hashtable.h

# expects a file `testing.out` to exist; it can contain any text
TEST = testing.sh
test: $(TEST) testing.out
	./$(TEST) < testing.out

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

