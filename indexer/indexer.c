/*
 * indexer.c - CS50 indexer module
 *
 * Annabel Revers, February 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include "webpage.h"
#include "hashtable.h"
#include "bag.h"
#include "pagedir.h"
#include "file.h"
#include "counters.h"
#include "index.h"
#include "word.c"

/************** local functions *************/
void indexer(char *pageDirectory, char *indexFilename);
webpage_t * getPage(char *filepath);
void addWords(hashtable_t *ht, webpage_t *page, int docID);

/************** main *************/
int
main(int argc, char *argv[])
{
    // check for correct number of arguments
    if (argc != 3) {
        printf("usage: indexer pageDirectory indexFilename\n");
        exit(1);
    }

    // initialize variables for arguments
    char *pageDirectory = malloc(strlen(argv[1]) + 1);
    strcpy(pageDirectory, argv[1]);
    char *indexFilename = malloc(strlen(argv[2]) + 1);
    strcpy(indexFilename, argv[2]);

    // check if pageDirectory is an existing, crawler-produced directory
    if (!isCrawlerDir(pageDirectory)) {
        printf("error: pageDirectory is not a Crawler-produced directory\n");
        free(pageDirectory);
        free(indexFilename);    
        exit(1);
    }

    // check if indexFilename is a writeable file
    FILE *indexfp = fopen(indexFilename, "w");      
    if (indexfp == NULL) {      
        printf("error: indexFilename is not a writeable file\n");
        free(pageDirectory);
        free(indexFilename);
        exit(1);
    }
    fclose(indexfp);

    // pass arguments to indexer
    indexer(pageDirectory, indexFilename);

    // free argument variables
    free(pageDirectory);
    free(indexFilename);

    return 0;
 }

/*************** indexer ****************/
/* build inverted-index data structure */
void indexer(char *pageDirectory, char *indexFilename)
{
    // create hashtable for storing words
    const int numslots = 300;               // variable for setting number of slots in ht
    hashtable_t *ht = hashtable_new(numslots);
    if (ht == NULL) {
        printf("error creating hashtable\n");
        return;
    }

    // read documents from pageDirectory
    DIR *dir = opendir(pageDirectory);
    struct dirent *dp;
    const int numchars = 100;               // variable for setting length of 'filepath'

    while ((dp = readdir(dir)) != NULL) {
        // make filepath for opening current file
        char *filepath = malloc(sizeof(char) * numchars);
        sprintf(filepath, "%s/%s", pageDirectory, dp->d_name);
        if (filepath == NULL) {
            printf("error creating filepath\n");
            continue;
        }
        // create webpage for current file
        webpage_t *mypage = getPage(filepath);
        // add words in webpage to hashtable
        if (mypage != NULL) {
            int docID = atoi(dp->d_name);
            addWords(ht, mypage, docID);
            webpage_delete(mypage);
        } 
        free(filepath);
    }
    closedir(dir);

    // build the index file
    indexSave(ht, indexFilename);

    // delete the hashtable
    indexDelete(ht);
}

/************** getPage() **************/
/* get webpage for Crawler produced file */
webpage_t * getPage(char *filepath)
{
    // open file for reading
    FILE *fp = fopen(filepath, "r");   

    // check if file failed to open for reading
    if (fp == NULL) {
        return NULL;
    }    

    // get url and check if NULL
    char *url = freadlinep(fp);
    if (url == NULL) {
        fclose(fp);
        return NULL;
    }

    // get depth and check if NULL
    char *depthStr = freadlinep(fp);
    if (depthStr == NULL) {
        free(url);
        fclose(fp);
        return NULL;
    }

    // get html and check if NULL
    char *html = freadfilep(fp);
    if (html == NULL) {
        free(url);
        free(depthStr);
        fclose(fp);
        return NULL;
    }

    // convert depthStr to int
    int depth = atoi(depthStr);
    free(depthStr);

    // create new webpage
    webpage_t *page = webpage_new(url, depth, html);

    // close file
    fclose(fp);

    return page;
}

void
addWords(hashtable_t *ht, webpage_t *page, int docID)
{
    int pos = 0;
    char *result;

    while ((result = webpage_getNextWord(page, &pos)) != NULL) {
        // ignore word if fewer that three characters
        if (strlen(result) < 3) {
            free(result);
            continue;   
        } 
        // normalize word
        NormalizeWord(result);
        // check if word is in hashtable
        if (hashtable_find(ht, result) == NULL) {      // word not yet in ht
            counters_t *counter = counters_new();      // create counter for word
            counters_add(counter, docID);              // add count for docID
            hashtable_insert(ht, result, counter);     // insert into hashtable
        } else {
            // get counter from hashtable
            counters_t *counter = hashtable_find(ht, result);
            // increment count for docID or create counter for docID
            counters_add(counter, docID);  
        }         
        free(result);   
    }
}

