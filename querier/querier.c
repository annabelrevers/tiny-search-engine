/*
 * querier.c - CS50 'querier' module
 *
 * Annabel Revers, February 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "webpage.h"
#include "hashtable.h"
#include "index.h"
#include "bag.h"
#include "pagedir.h"
#include "file.h"
#include "counters.h"

/**************** local types **************/
typedef struct docscore {
    int docID;
    int score;
} docscore_t;

typedef struct ctrpair {
    counters_t *ctr1;
    counters_t *ctr2;
} ctrpair_t;

/*************** functions *************/
void querier(char *pageDirectory, char *indeFilename);
int parseQuery(char *query, char *words[]);
bool checkStructure(char *words[], int numWords);
void printQuery(char *words[], int numWords);
docscore_t ** performQuery(char *words[], hashtable_t *ht, int numWords);
counters_t * ctrsIntersect(counters_t *runningproduct, counters_t *wordctr);
void ctrsIntHelper(void *arg, const int key, const int count);
counters_t * ctrsUnion(counters_t *ctrsA, counters_t *ctrsB);
void ctrsUnionHelper(void *arg, const int key, const int count);
void printResult(docscore_t **docArray, char *pageDirectory);
static void docCount(void *arg, const int key, const int count);
static void buildDocArray(void *arg, const int key, const int count);

/************* main **************/
int
main(int argc, char *argv[])
{
    // check for correct number of arguments
    if (argc != 3) {
        printf("usage: querier pageDirectory indexFilename\n");
        exit(1);
    }

    // intialize variables for arguments
    char *pageDirectory = malloc(strlen(argv[1]) + 1);
    strcpy(pageDirectory, argv[1]);
    char *indexFilename = malloc(strlen(argv[2]) + 1);
    strcpy(indexFilename, argv[2]);

    // validate that pageDirectory is an existing, Crawler-produced directory
    if (!isCrawlerDir(pageDirectory)) {
        printf("error: pageDirectory is not a Crawler-produced directory\n");
        free(pageDirectory);
        free(indexFilename);
        return 1;
    }

    // validate that indexFilename is the pathname of a readable file
    FILE *fp = fopen(indexFilename, "r");
    if (fp == NULL) {
        printf("error: indexFilename is not a readable file\n");
        free(pageDirectory);
        free(indexFilename);
        return 1;
    }
    fclose(fp);

    // pass arguments into querier
    querier(pageDirectory, indexFilename);

    // free argument variables
    free(pageDirectory);
    free(indexFilename);

    return 0;
}

/************** querier *************/
/* performs queries for the Tiny Search Engine */
void
querier(char *pageDirectory, char *indexFilename)
{
    // load index from indexFilename into internal data structure
    hashtable_t *ht = indexLoad(indexFilename);
    if (ht == NULL) {
        return;
    }

    // read queries from stdin, one per line, until EOF
    printf("Query? ");
    char *query;   
    while ((query = freadlinep(stdin)) != NULL) {
        // create array to store words from the query
        int maxWords = 30;
        char *words[maxWords];

        // parse the query
        int numWords = parseQuery(query, words);

        // check if we found invalid char
        if (numWords == -1) {
            free(query);
            break;
        }

        // check if words is empty
        if (numWords == 0) {
            printf("error: no words to query\n");
            free(query);
            break;
        }

        // print query back to the user
        printQuery(words, numWords);

        // validate the structure
        if (!checkStructure(words, numWords)) {
            printf("error: query has invalid syntax\n");
            free(query);
            break;
        }

         // get docIDs satifying the query and their scores
        docscore_t **docArray = performQuery(words, ht, numWords);

        // print result of the query
        if (docArray != NULL) {
            printResult(docArray, pageDirectory);

            // free docArray
            int length = (docArray[0]->score) + 1;
            for (int i = 0; i < length; i++) {
                free(docArray[i]);
            }
            free(docArray);
        } else {
            printf("No documents match.\n");
            printf("--------------------------------------------\n");
        }
        free(query);    // free query
        printf("Query? ");
    }
    indexDelete(ht);
}


/*************** parseQuery *************/
/* parses a query to add words to an array */
int
parseQuery(char *query, char *words[]) {
    char *word = query;
    char *rest = query;
    int wordsIndex = 0;
    int numWords = 0;

    while (1) {
        // increment points until we find beginning of word
        while (isspace(*rest)) {
            rest++;
        }
        // check if we reached end of line
        if (*rest == '\0') {
            break;
        }
        word = rest;
        // slide rest pointer to find end of word
        while (!isspace(*rest)) {
            // check if we reached end of line
            if (*rest == '\0') {
                break;
            }
            // check for invalid char
            if (!isalpha(*rest)) {
                printf("bad character '%c' in query\n", *rest);
                return -1;
            }
            // check if char is upper case
            if (isupper(*rest)) {
                *rest = tolower(*rest);
            }
            rest++;
        }
        // add word to words array
        if (*rest != '\0') {    // haven't reach end of line
            (*rest) = '\0';
            words[wordsIndex] = word;
            wordsIndex++;
            numWords++;
            // set pointers to begin search for next word
            rest++;
            word = rest;
        } else {                // reached end of line
            numWords++;
            words[wordsIndex] = word;
            break;
        }
    }
    return numWords;
}

/************ checkStructure ***********/
/* checks that the placement of "and" and "or" in they query
 * is acceptable according to the requirements spec
 */
bool
checkStructure(char *words[], int numWords)
{
    // check if first word in sequence is a literal
    if (strcmp(words[0], "or") == 0 || strcmp(words[0], "and") == 0) {
        return false;
    }

    // check if last word in sequence is a literal
    if (strcmp(words[numWords-1], "or") == 0 || 
            strcmp(words[numWords-1], "and") == 0) {
        return false;
    }

    // check for repeated literal
    for (int i = 0; i < numWords-1; i++) {
        if (strcmp(words[i], "and") == 0) {
            if (strcmp(words[i+1], "and") == 0) {
                return false;
            }
            if (strcmp(words[i+1], "or") == 0) {
                return false;
            }
        }
        if (strcmp(words[i], "or") == 0) {
            if (strcmp(words[i+1], "and") == 0) {
                return false;
            }
            if (strcmp(words[i+1], "or") == 0) {
                return false;
            }
        }
    }
    return true;
}

/************ printQuery ***********/
/* prints query to the user */
void
printQuery(char *words[], int numWords)
{
    printf("Query: ");
    for (int i = 0; i < numWords; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
}

/***************** performQuery ************/
/* peforms a query returning an array of docscore objects,
 * one for each docID/score pair satisfying query
 */
docscore_t **
performQuery(char *words[], hashtable_t *ht, int numWords)
{
    // counter to hold union of eveything seen so far in sequence
    counters_t *product = hashtable_find(ht, words[0]);
    if (product == NULL) {
        product = counters_new();    
    }

    int i = 1;
    while (i < numWords) {
        if (strcmp(words[i], "and") == 0) {     // skip "and"
            i++;
            continue;
        } else if (strcmp(words[i], "or") == 0) {   // union
            i++;
            counters_t *subProduct = hashtable_find(ht, words[i]);
            if (subProduct == NULL) {
                subProduct = counters_new();
            }
            i++;
            // keep performing "and" operator until we find another "or"
            while (i < numWords && strcmp(words[i], "or") != 0) {
                if (strcmp(words[i], "and") == 0) {     // skip "and"
                    i++;
                    continue;
                }    
                counters_t *wordctr = hashtable_find(ht, words[i]);
                if (wordctr == NULL) {
                    wordctr = counters_new();
                }
                
                counters_t *newSubProduct = ctrsIntersect(subProduct, wordctr);
                subProduct = newSubProduct;
                i++;
            }

            counters_t *result = ctrsUnion(product, subProduct);
            product = result;
                               

        } else {    // intersection
            counters_t *wordctr = hashtable_find(ht, words[i]);
            
            if (wordctr == NULL) {
                wordctr = counters_new();
            }
            counters_t *newProduct = ctrsIntersect(product, wordctr);
            product = newProduct;

            i++;
        }
    }
    
    // loop over counter to get number of documents
    int numDocs = 0;
    counters_iterate(product, &numDocs, docCount);
    if (numDocs == 0) {
        return NULL;
    }
    // create appropriately sized array for number of docs
    docscore_t **docArray = malloc((sizeof(docscore_t) * numDocs) + 1);

    // hide information in first index of docArray
    docscore_t *info = malloc(sizeof(docscore_t));
    info->docID = 1;
    info->score = numDocs;
    docArray[0] = info;

    // build docArray
    counters_iterate(product, docArray, buildDocArray);

    counters_delete(product);   // free product
    
    return docArray;
}


/************* countersIntersect *************/
/* returns the intersection between two counters */
counters_t *
ctrsIntersect(counters_t *product, counters_t *wordctr)
{
    // store product in a ctrpair along with a new counter
    ctrpair_t *ctrpair = malloc(sizeof(ctrpair_t));
    ctrpair->ctr1 = product;
    counters_t *result = counters_new();
    ctrpair->ctr2 = result;

    // iterate over wordctr, find which keys also in product
    counters_iterate(wordctr, ctrpair, ctrsIntHelper);

    return ctrpair->ctr2;

    // need to free ctrpair
}


/************* ctrsIntHelper *************/
/* helper function for ctrsIntersect */
void
ctrsIntHelper(void *arg, const int key, const int count)
{
    ctrpair_t *ctrpair = arg;
    
    // check if key is in product
    int prodCount = counters_get(ctrpair->ctr1, key);
    if (prodCount > 0) {
        // key found, get min score and add to result
        if (prodCount < count) {
            counters_set(ctrpair->ctr2, key, prodCount);            
        } else {            
            counters_set(ctrpair->ctr2, key, count);            
        }
    }           
}


/************ ctrsUnion ************/
/* gets the union of two counters */
counters_t *
ctrsUnion(counters_t *ctrsA, counters_t *ctrsB)
{
    counters_t *result = counters_new();
    
    // iterate through ctrsA and ctrsB, add their keys to result
    counters_iterate(ctrsA, result, ctrsUnionHelper);
    counters_iterate(ctrsB, result, ctrsUnionHelper);

    return result;
}

/************ ctrsUnionHelper *********/
/* helper function for ctrsUnion */
void 
ctrsUnionHelper(void *arg, const int key, const int count)
{
    //printf("in helper\n");
    counters_t *result = arg;

    if (counters_get(result, key) == 0) {    // key not yet in result
        counters_add(result, key);
        counters_set(result, key, count);
    } else {                                // key found, update count
        int currCount = counters_get(result, key);
        counters_set(result, key, currCount + count);
    }
}

/************** printResult **************/
/* prints documents and counts matching a query */
void
printResult(docscore_t **docArray, char *pageDirectory)
{
    // get length of docArray
    int length = (docArray[0])->score;
    // loop over array to print out the list of documents
    printf("Matches %d documents (ranked):\n", length);
    for (int i = length; i > 0; i--) {
        // get doc ID
        int docID = (docArray[i])->docID;
        // create pathname
        char *filepath = malloc(strlen(pageDirectory) + 3);
        sprintf(filepath, "%s/%d", pageDirectory, docID);
        // get the url
        FILE *fp = fopen(filepath, "r");
        char *url = freadlinep(fp);
        // get score
        int score = (docArray[i])->score;
        // print information
        printf("score   %d doc  %d: %s\n", score, docID, url);
        fclose(fp);
        free(url);
    }
    printf("--------------------------------------------\n");
}

// counts number of documents in which word was found
static void docCount(void *arg, const int key, const int count)
{
    int *nitems = arg; 
    if (nitems != NULL) {
        (*nitems)++;
    }
}

// create docandcount for each document in which word was found and add to array
static void buildDocArray(void *arg, const int key, const int count)
{
    // create docandcount for key/count pair
    docscore_t *doc = malloc(sizeof(docscore_t));

    if (doc != NULL) {
        // assign its members
        doc->docID = key;
        doc->score = count;

        // insert-sort approach to drop new docandcount into the array
        docscore_t **docArray = arg;
        int maxIndex = docArray[0]->docID;
        int index = 1;

        // find index where doc belongs
        while ((index < maxIndex) && (docArray[index])->score < count) {
            index++;
        }

        // move items up
        while (index < maxIndex) {
            docArray[maxIndex] = docArray[maxIndex-1];
            maxIndex--;
        }

        // insert doc at index
        docArray[index] = doc; 
        (docArray[0]->docID)++;
    }
}
