## Indexer Design Specs

### User interface

The indexer's only interface with the user is on the command-line; it must always
have two arguments.

```bash
$ indexer pageDirectory indexFilename
```

- `pageDirectory` is the pathname of a directory produced by the Crawler, and 
- `indexFilename` is the pathname of a file into which the index should be written;
 the indexer creates the file (if needed) and overwrites the file (if it already 
 exists)

For example, to build an index for the files in directory 'testDir' and write that 
index to the file 'testFile', we would use this command-line:

``` bash 
$ indexer testDir testFile
```
The indextest's only interface with the user is on the command-line; it must always
have two arguments.

```bash
$ indextest oldIndexFilename newIndexFilename
```

- `oldIndexFilename` is the name of a file produced by the indexer, and
- `newIndexFilename` is the name of a file into which the index should be written
 
For example, to build an indextest for the file 'oldIndexTest' and write that index
to the file 'newIndexTest', we would use this command-line:

``` bash
$ indextest oldIndexFilename newIndexFilename
```


### Inputs and outputs

*Input:* for indexer, the inputs are command-line parameters and the directory 
produced by Crawler isspecified by the first command-line parameter. For index test, the inputs are filenames specified by the first and second command-line parameters. 
See the User Interface above.

*Output:* Indexer will save the index it builds to the file specified in the second 
command-line arguement. See the User Interface above. Indextest will write the index into the file specified in the second command-line argument.

Within this file, indexer writes:

 * one line per word
 * each line provides the word and one of more (docID, count) pairs, in the format 
'word docID count [docID count]...'
 * where 'word' is a string of lower case letters
 * where 'docID' is a positive non-zero integer
 * where 'count' is a positive non-zero integer
 * where the word and integers are separated by spaces.

indextest writes its file in the same format.


### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules.
 2. *indexer*, which loops over pages and builds the inverted-index data structure.
 3. *indexertest* which loads the index from indexFilename back into an 
    inverted-index data struture.
 4. *index.c*, whichcontains the logic for saving and loading index files.
 5. *word.c*, which implements `NormalizeWord`.
 6. *pagedir.c* which contains logic for saving pages to a crawler output directory, 
    for loading pages from a crawler output directory.

And some helper modules that provide data structures:

  1. ***hashtable*** of words seen
  2. ***counters*** of '(docID, count)' pairs

### Pseudo code for logic/algorithmic flow

The indexer will run as follows:

1. start from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. create a hashtable for storing words seen in the pageDirectory files
4. while there are more files in pageDirectory,  
	4.1 parse the file to extract the url, depth, and html
    4.2 create a new webpage to store the above information
    4.3. for each word found
         4.3.1 normalize the word suing *word.c*
         4.3.2 check if word is already in hashtable
         4.3.3 if it does, check if the file already has a counter
             4.3.3.1 if it does, increment the count for the file where is was
                     found
             4.3.3.2 else create a new counter for the file
         4.3.4 else add word to the hashtable and create a counter for the file
5. save the inverted-index data structure using *index.c*


The indextest will run as follows:

1. start from the command line as shown in the User Interface
2. create a hashtable for storing words and their counts in different files
3. while there are more lines in 'oldIndexFilename'
     3.1 get the word
     3.2 while there are more file count pairs
         3.2.1 if word is already in hashtable
             3.2.1.1. check if file has a counter.
             3.2.1.2 if it does, increment the count
             3.2.1.3 else create a counter for the file
         3.2.2 else add the word to the hashtable and create a counter for the 
               file
4. save the inverted-index data structure using *index.c*


### Dataflow through modules

 1. *main* parses and validate parameters and then parses files in pageDirectory 
    which it passes to indexer
 2. *indexer* iterates through the words in the file and builds the inverse-index 
    data structure
 3. *index* loads the inverse-index data structure to a file
 4. *indextest* calls functions from *index* to load the file back into an inverse-in    dex data structure then back to a file


### Major data structures

Three helper modules provide data structures:

 1. ***hashtable*** of words
 2. ***set*** of counters
 2. ***counters*** of filename/count pairs

A data structure that will be used:

*webpage* which contains all the data read for a given webpage, plus the URL
and the depth at which it was fetched

### Testing plan

- *Unit testing*.  A small test program to test each module to make sure it does
 what it's supposed to do. 

- *Integration testing*.  Assemble the indexer and test it as a whole. In each case,
 examine the output files carefully to be sure they have the words from the correct
 pages.

A sampling of tests that should be run:

-  Test the program with various forms of incorrect command-line arguments to ensure 
that its command-line parsing, and validation of those parameters, works correctly.

-  Test the indexer with a pageDirectory containing one file

-  Test the indexer with a pageDirectory containing a few files from small webpages

-  Test the indexer with a larger pageDirectory to ensure it runs smoothly

-  For each test above, use indextest to ensure that its output files match those 
produced by indexer
