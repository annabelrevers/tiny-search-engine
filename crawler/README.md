# CS50 Lab 4
## CS50 Winter 2021

### crawler

A `crawler` takes a url and finds its webpage. It then finds the urls on that webpage and explores them up to a certain depth specified in the arguement `maxDepth`. The information from each webpage explored is stored in files saved in `pageDirectory`.

### Assumptions

I assumed that my hashtable only needed 25 slots, but this can be easily changed in the crawler() function.

### Files

* `Makefile` - compilation procedure
* `crawler.c` - the crawler module
* `testing.sh` - shell script for testing crawler
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make`.

### Testing

To test, run `make test &> testing.out`

