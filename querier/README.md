# CS50 Lab 6
## CS50 Winter 2021

### querier

The `querier` module supports the Indexer for the CS50 Tiny Search Engine project.

### Notes

There are some memory leaks in querier, particularly in the `performQuery` function that I did not have time to fix.

### Assumptions

* The max number of words in a 'query' is 30, but this maxium can be set by changing 'maxWords.'

### Functionality

I chose to implement the full functionality of querier as according to the specs. Thus, querier supports "and" and "or" operators as well as precedence of "and." It prints the results of the query in decreasing order.

### Files

* `Makefile` - compilation procedure
* `querier.c` - the querier module
* `testing.sh` - shell script for testing querier
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make`.

### Testing

To test, run `make test &> testing.out`

