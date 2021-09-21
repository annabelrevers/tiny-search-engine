#!/bin/bash
#
# testing.sh executes a sequence of commands that test indexer.c and indextest.c
# 
# Annabel Revers, February 2021

# INDEXER TESTING

# wrong number of arguments 
./indexer pageDir

# wrong number of arguments
./indexer pageDir file1 file1

# pageDirectory is not a crawler produced directory
./indexer testDir file1

# indexFilename is not a writeable file (it is a directory)
./indexer data0 testDir

# test indexer with small Crawler-produced directory
./indexer data0 data0index
gawk -f indexsort.awk data0index > data0index.sorted

# test indexer with a larger Crawler-produced directory
./indexer data1 data1index
gawk -f indexsort.awk data1index > data1index.sorted

# test indexer with large Crawler-produced directory
./indexer data2 data2index
gawk -f indexsort.awk data2index > data2index.sorted

#INDEXTEST TESTING

# wrong number of arguments
./indextest file1

# wrong number of arguments
./indextest file1 file2 file3

# test indextest with data0index
./indextest data0index data0newindex
gawk -f indexsort.awk data0newindex > data0newindex.sorted

# test indextest with data1index
./indextest data1index data1newindex
gawk -f indexsort.awk data1newindex > data1newindex.sorted

# test indextest with data2index
./indextest data2index data2newindex
gawk -f indexsort.awk data2newindex > data2newindex.sorted

