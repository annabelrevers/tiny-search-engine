/*
 * index.c - CS50 'index' module
 *
 * see index.h for more information
 *
 * Annabel Revers, February 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "set.h"
#include "counters.h"
#include "index.h"
#include "file.h"

/************** local functions ************/
static void mySetIterate(void *arg, const char *key, void *item);
static void myCtrsIterate(void *arg, const int key, const int count);
static void deleteCounter(void *item);
 
/************** indexSave ************/
/* see index.h for more information */
void
indexSave(hashtable_t *ht, char *indexFilename)
{    
    // open indexFilename for writing
    FILE *fp = fopen(indexFilename, "w");
    // write hashtable into indexFilename
    hashtable_iterate(ht, fp, mySetIterate);
    // close file
    fclose(fp);
}

/************** indexLoad *************/
/* see index.h for more information */
hashtable_t *
indexLoad(char *oldIndexFilename)
{
    // open oldIndexFilename for reading
    FILE *fp = fopen(oldIndexFilename, "r");
    if (fp == NULL) {
        printf("error opening file\n");
        return NULL;            // error opening file for reading
    }

    // get number of words
    int lines = lines_in_file(fp);

    // create hashtable
    hashtable_t *ht = hashtable_new(lines);
    if (ht == NULL) {
        printf("error creating hashtable\n");
        return NULL;
    }

    while (lines > 0) {
        // get word
        char *word = freadwordp(fp);
        // get docID/count pairs
        int docID, count;
        while (fscanf(fp, "%d %d", &docID, &count) == 2) {
            if (hashtable_find(ht, word) == NULL) {     // word not yet in hashtable
                counters_t *counter = counters_new();
                counters_add(counter, docID);
                counters_set(counter, docID, count);
                hashtable_insert(ht, word, counter);
            } else {
                // get counter from hashtable and add count for docID
                counters_t *counter = hashtable_find(ht, word);
                counters_add(counter, docID);
                counters_set(counter, docID, count);
            }
        }
        free(word);
        lines--;
    }
    fclose(fp);
    return ht;
}

/*************** indexDelete ****************/
/* see index.h for more info */
void 
indexDelete(hashtable_t *ht)
{
    hashtable_delete(ht, deleteCounter);
}

// iterates through set and calls myCtrsIterate on each counter item
static void mySetIterate(void *arg, const char *key, void *item)
{
    // print word followed by space to current file line
    fprintf(arg, "%s ", key);

    // call counters_iterate on each counter item
    if (item != NULL) {
        counters_iterate(item, arg, myCtrsIterate);
        fputc('\n', arg);
    }
}

// iteratures through counter and prints each key and count separated by space
static void myCtrsIterate(void *arg, const int key, const int count)
{
    fprintf(arg, "%d ", key);
    fprintf(arg, "%d ", count);
}

// deletes counters in our inverse-index data structure
static void deleteCounter(void *item)
{
    if (item != NULL) {
        counters_delete(item);
    }
}
