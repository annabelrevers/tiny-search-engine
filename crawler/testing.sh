#!/bin/bash
#
# testing.sh executes a sequence of commands that test words.c
# 
# Annabel Revers, February 2021


# wrong number of arguments 
./crawler url test

# wrong number of arguments
./crawler url test 4 hi

# pageDirectory does not exist or is not writeable
./crawler http://cs50tse.cs.dartmouth.edu/tse/ notdir 0

# maxDepth is not an int
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ mydir a

# maxDepth is negative
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html mydir -10

# seedURL points to a nonexistent server
./crawler nonexistentserver mydir 1

# seedURL points to a noninternal server
./crawler https://en.wikipedia.org/wiki/Algorithm mydir 1

# seedURL points to a valid server but non-existent page
./crawler http://cs50tse.cs.dartmouth.edu/tse/notapage mydir 1

# crawl with a cycle, seedURL is the homepage for the CS50 TSE playground
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html mydir 1

# another crawl with a cycle in the CS50 TSE playground
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/B.html mydir 1

# seedURL is the Wikipedia playground: depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse//wikipedia/index.html mydir 1

