## Querier Implementation Specs

## Overview
Our querier builds an inverse-index data structure for a crawler-produced directory. It uses this structure to see if each word in the query appears in the index. It creates a counters object for this index if so and, depending on whether "and" or "or" was specified after it, finds the intersection or union of this counters object with a running product counters object.


## Psuedo code for major components

### querier

1. start from a command line with `querier pageDirectory indexFilename`
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
5. add the (docID, score) pairs in the resulting counter to an array of `docscore_t` objects. See below for more information on this data structure
6. print this array to the user in the format specified in the "User Interface" section of the Design specs


## Function prototypes
Our querier is implemented through several functions

### indexer.c

```
void querier(char *pageDirectory, char *indeFilename);
int parseQuery(char *query, char *words[]);
bool checkStructure(char *words[], int numWords);
void printQuery(char *words[], int numWords);
docscore_t ** performQuery(char *words[], hashtable_t *ht, int numWords);
counters_t * ctrsIntersect(counters_t *runningproduct, counters_t *wordctr);
void ctrsIntHelper(void *arg, const int key, const int count);
void ctrsUnion(counters_t *ctrsA, counters_t *ctrsB);
void ctrsUnionHelper(void *arg, const int key, const int count);
void printResult(docscore_t **docArray, char *pageDirectory);
static void docCount(void *arg, const int key, const int count);
static void buildDocArray(void *arg, const int key, const int count);

```

`querier` is the central function for performing a query. It recieves the pageDirectory and indexFilename from `main` and proceeds to accept queries from stdin. It calls the appropriate functions to parse the query and check its syntax, and then passes it to `performQuery.` An array of the docID/score pairs matching the queries is retreturned which `querier` then passes to the appropriate function to print it to the user.

`parseQuery` recieves the query line from the user. It makes sure there are no bad characters like numbers or symbols, then converts all chars to lower case and then adds each word to an array of char * strings called words. It also keeps track of how many words are added and returns this number.

`checkStructure` ensures that there are no adjacent literals ("and" and "or") and that all literals come between words and not at the beginning or end of a sequence.

`printQuery` receives the array of words made by `parseQuery` and prints them back to the user in a cleaned up format.

`performQuery` recieves the array of words as well as the hashtable containing the index and the number of words in the query. It loops through these words, looking for "and" and "or" to determine whether to intersect or union the counter of the current word with the running product counter.

`ctrsInt` receives two counters and finds with the help of its helper method `ctrsIntHelper` the intersection between them. It loops through one counter then checks for each docID/count pair if that docID key exists in the second counter. If it does, the docID is added as a key to a new counter and and the count of that key is set to the min between the count for ctrsA and ctrsB. After iterating through all key/item pairs, the new counter is returned. To be able to pass three counters into `counters_iterate` we utilize a ctrpair. See the "Data Structures" section below for more information on this data type.

`ctrsUnion` recieves two countesr and finds with the help of its helper method `ctrsUnionHelper` the union between them. It loop through one counter, adding every docID/count pair it finds to the second counter. After iterating through all pairs, it returns the second counter.

`docCount` counts the number of documents that satisfy a query.

`buildDocArray` puts all of the satisfying docID/score pairs into a docscore object and adds them to a docscore ** array. See the "Data Structures" section below for more information on the docscore data structure.



### The following modules are used and are contained in the 'common' directory.
### index.c

```
void indexLoad(hashtable_t *ht, char *indexFilename);
void indexDelete(hashtable_t *ht);
```

See `index.h` for more information on these functions.

### pagedir.c

```
bool isCrawlerDir(char *pageDirectory);
```

See pagedir.h for more information.

## Data Structures

### Hashtable

```
typedef struct hashtable {
    int num_slots;
    set_t **table;
} hashtable_t
```

### Counter

```
typdef struct counters {
    int key;
    int count;
    struct countersnode *next;
} countesrnode_t;

typdef struct counters {
    struct countersnode *head;
} counters_t;
```

### Docscore

```
typedef struct docscore {
    int docID;
    int score;
} docscore_t;
```

### Ctrpair

```
typedef struct ctrpair {
    counters_t *ctr1;
    counters_t *ctr2;
} ctrpair_t;
```

## Error handling

Command-line arguments are validated in the main of indexer and indextest. The program will be exited if any are not valid. The query is also checked for invalid sytax. If the syntax is invalid, an error message will be printed and the user will be prompted to enter another query. Otherwise, there are checks throughout that will handle errors getting files and adding information to the inverse-index data structures while still allowing the program to continue running.



