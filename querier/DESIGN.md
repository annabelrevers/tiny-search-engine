## Querier Design Specs

### User interface

The querier is called from the command-line; it must always have two arguments.

```bash
$ querier pageDirectory indexFilename
```

- `pageDirectory` is the pathname of a directory produced by the Crawler, and 
- `indexFilename` is the pathname of a file producted by the Indexer

For example, to perform a query for 'testIndexfile' an index file produced by the Crawler-produced directory 'testDir,' we would use this command-line:

``` bash 
$ indexer testDir testIndexfile
```

After calling 'query' from the command line, the user be prompted by "Query?" to enter words for to be queried. The result of the query will be printed to the user to stdin, in this case the screen. The user will again be prompted by "Query?" to perform a query and they may continue performing queries for as long as desired. To stop getting queries and end the program, the user may type either '^D' or '^C'. 

The user has the following options for entering queries:
* They may simply list words with spaces in between. The resulting query will determine which documents have all of the words listed in them. 
* They may use the words "and" and "or" between the words. If "and" is entered, the query will determine which documents have everything on either side of "and" in them. This is equivalent to the intersection of the sets of documents that contain the word or sequence to the left and right of "and." If "or" is entered, the document will determine which documents have what is on either side of the "or". This is the equivalent between the union of the sets of documents that contain the words or sequences on either side of the "or."
* "and" has precedence over "or" and will thus be performed first.


### Inputs and outputs

*Input:* for querier, the inputs are command-line parameters: the index file, and the directory 
produced by Crawler as specified. The words to query are also inputed using stdin after calling querier. See the User Interface above.

*Output:* Querier will print the result of the query to stdin, in this case the screen. See the User Interface above.

The format of the resulting query is as follows:
    score #  docID #: url


Where...
* score is: number of times the query is satisfied in a particular document.
    * for "or", this is score is how many times either word appears
    * for "and," this score is the minimum of how many times each word appears
* docID is: the ID of the document satisfying the query
 * url is: the url of the webpage the docID represents


### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules.
 2. *querier.c*, which continues getting queries from stdin and 
 3. *index.c* which loads the index file into an inverted-index structure
    inverted-index data struture.
 4. *pagedir.c*, which provides the logic for checking if 'pageDirectory' is a crawler-produced directoy.

And some helper modules that provide data structures:

  1. ***hashtable*** serves as the inverse-index data structure
  2. ***counters*** of '(docID, count)' pairs

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

1. start from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. create a hashtable for storing the index file
4. while the user keeps entering queries on the command-line  
	4.1 parse the query to ensure it is valid
    4.2 check the logic of the query to make sure "ands" and "ors" are in acceptable spots
    4.3 create a counter as a running product to hold all pages satisfying the query
    4.3. for each word found
         4.3.1 check if the "and" (intersection) or "or" (union) operation should be performed
         4.3.2 get the counters set of the docIDs in which it appears and the count for each
         4.3.2 either intersect or union this counters set with the running product

### Dataflow through modules

 1. *main* parses and validate parameters and then calls the `querier()` function
 2. *querier* calls all functions necssary to perform a query

### Major data structures

Three helper modules provide data structures:

 1. ***hashtable*** of words found in the index
 2. ***set*** of counters
 2. ***counters*** of docID/count pairs


### Testing plan

- *Unit testing*.  Test querier with small programs that ensure querier does
 what it's supposed to do. 

- *Integration testing*.  Assemble the querier and test it as a whole. In each case,
 examine the output files carefully to be sure they have the correct words, docIDs, and urls for each query.

A sampling of tests that should be run:

-  Test the program with various forms of incorrect command-line arguments to ensure 
that its command-line parsing, and validation of those parameters, works correctly.

-  Test the querier with only spaces between the words

- Test the querier with only "ors" between the words

- Test the querier with only "ands" between the words

- Test the querier with one "and" and one "or"

- Test the querier with increasingly complex sequences of words with "ands" and "ors" between them
