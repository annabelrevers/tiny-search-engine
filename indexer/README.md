# CS50 Lab 5
## CS50 Winter 2021

### indexer

The `indexer` module supports the Indexer for the CS50 Tiny Search Engine project.

### Assumptions

* The hashtable for building our inverse-index data structure is 300 slots. Such can be changed with the const int numslots variable provided in the `indexer()` function.
* The filepath in the `indexer()` function is assumed to be 100 chars long or less. Such can be changed with the const int numchars variable provided in the `indexer()` function.

### Files

* `Makefile` - compilation procedure
* `indexer.c` - the indexer module
* `indextest.c` - module to test `indexLoad()` and `indexLoad()` from *index.c* of the common module
* `testing.sh` - shell script for testing indexer
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make`.

### Testing

To test, run `make test &> testing.out`

