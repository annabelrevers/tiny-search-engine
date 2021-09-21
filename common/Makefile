# Makefile for the 'common' library
#
# Annabel Revers, February 2020

L = ../libcs50

OBJS = pagedir.o index.o word.o 
LIB = common.a

FLAGS =

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L
CC = gcc
MAKE = make

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h $L/webpage.h $L/bag.h $L/hashtable.h
index.o: index.h $L/hashtable.h $L/set.h $L/counters.h $L/file.h
word.o: word.h

clear:
	rm -f core
	rm -f $(LIB) *~ *.o


