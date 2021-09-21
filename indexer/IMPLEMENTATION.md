## Indexer Implementation Specs

## Overview
Our indexer builds an inverse-index data structure for a crawler-produced directory. This directory as well as a filename for writing the index to can be specified when calling `indexer` from a command-line interface. By parsing through the files in the directory and extracting the url, depth, and html of the webpage, we can build a webpage for each file that allows us to find all the words of that webpage. Those words can then be added to our inverse-index data structure with counts for the number of times a word appears in each webpage and later written into the index file.


## Psuedo code for major components

### indexer

1. start from a command line by calling `indexer pageDirectory indexFilename`
2. in `main()`, parse the command line, validate parameters, and pass them to `indexer()`
3. in `indexer()`, create a hashtable for storing words seen in the pageDirectory files
4. while there are more files in pageDirectory,  
	4.1 pass the filename to `getPage()` to read the file and extract the url, depth, and html
    4.2 create a new webpage to store the above information
    4.3 pass the webpage to `addWords()` to find its words, and for each word found
         4.3.1 normalize the word suing *word.c*
         4.3.2 check if word is already in hashtable
         4.3.3 if it does, check if the file already has a counter
             4.3.3.1 if it does, increment the count for the file where is was found
             4.3.3.2 else create a new counter for the file
         4.3.4 else add word to the hashtable and create a counter for the file
5. save the inverted-index data structure using `indexSave()` from *index.c*

### indextest
1. start from the command line by calling `indextest oldIndexFilename newIndexFilename`
2. in `main()`, call `indexLoad()` from *index.c*
3. in `indexLoad()`, create a hashtable for storing words and their counts
4. while there are more lines in 'oldIndexFilename'
     3.1 get the word
     3.2 while there are more file count pairs
         3.2.1 if word is already in hashtable
             3.2.1.1. check if file has a counter.
             3.2.1.2 if it does, increment the count
             3.2.1.3 else create a counter for the file
         3.2.2 else add the word to the hashtable and create a counter for the file
4. save the inverted-index data structure using `indexSave()` from *index.c*

## Function prototypes
Our indexer is implemented through several modules:

### indexer.c

```
void indexer(char *pageDirectory, char *indexFilename);
webpage_t * getPage(char *filepath);
void addWords(hashtable_t *ht, webpage_t *page, int docID);
```

`indexer` recieves the name of the Crawler-produced directory and the name of the file to write the index into. It then creates a hashtable to begin building the inverse-index data structure and proceeds to loop through files in the Crawler-produced directory. It sends the name of the file to `getPage()` which returns a webpage that is then passed to `addWords()`.

`getPage` recieves the name of a crawler-produced file. It then parses the file to extract a url, depth, and html. It then creates a webpage with those elements which can then be parsed for words.

`addWords` recieves a hashtable and webpage as well as a document ID to find the number of times a word occurs in a webpage. It adds the word to the hashtable using that word as they key and a counter as the item. It adds that count to the hashtable for each docID.

### indextest.c

Calls functions from `index.c` in `main()`.

### The following modules are contained in the 'common' directory.
### index.c

```
void indexSave(hashtable_t *ht, char *indexFilename);
hashtable_t * indexLoad(char *oldIndexFilename);
void indexDelete(hashtable_t *ht);
```

See `index.h` for more information on these functions.

### word.c

```
void NormalizeWord(char *word)
```

See `word.h` for more information.

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

## Error handling

Command-line arguments are validated in the main of indexer and indextest. The program will be exited if any are not valid. Otherwise, there are checks throughout that will handle errors getting files, webpages, and adding information to the inverse-index data structures while still allowing the program to continue running.



