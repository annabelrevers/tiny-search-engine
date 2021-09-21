#!/bin/bash
#
# testing.sh executes a sequence of commands that tests querier.c
# 
# Annabel Revers, February 2021

# QUERIER TESTING

# wrong number of arguments 
./querier pageDir
usage: querier pageDirectory indexFilename
usage: querier pageDirectory indexFilename
testing.sh: line 11: usage:: command not found

# wrong number of arguments
./querier pageDir file1 file2
usage: querier pageDirectory indexFilename
usage: querier pageDirectory indexFilename
testing.sh: line 15: usage:: command not found

# pageDirectory is not a crawler produced directory
./querier testDir file1
error: pageDirectory is not a Crawler-produced directory
error: pageDirectory is not a Crawler-produced directory
testing.sh: line 19: error:: command not found

# indexFilename is not a writeable file (it is a directory)
./querier data0 filedoesntexist
error: indexFilename is not a readable file
error: indexFilename is not a readable file
testing.sh: line 23: error:: command not found

# test querier with small set of queries
cat testquery1 | ./querier data0 index.0

# test querier with another set of queries
cat testquery2 | ./querier data1 index.1
