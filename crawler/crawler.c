/*
 * crawler.c - CS50 crawler module
 *
 * see crawler.h for more information.
 *
 * Annabel Revers, February 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/memory.h"
#include "../common/pagedir.h"

/**************** local functions ****************/
void crawler(char *seedURL, char *pageDirectory, int maxDepth);
static void mydelete(void *item);

/**************** main ****************/
int
main(int argc, char *argv[])
{ 
    // initalize variables for arguments
    char *seedURL = malloc(strlen(argv[1]) + 1);
    char *pageDirectory = malloc(strlen(argv[2]) + 1);
    int maxDepth;
    strcpy(seedURL, argv[1]);
    strcpy(pageDirectory, argv[2]);
    
    // check for correct number of arguments
    if (argc != 4)
    {
        printf("usage: crawler seedURL pageDirectory maxDepth\n");
        return 1;       
    }

    // check if seedURL points to a valid server
    if (!IsInternalURL(seedURL)) {
        printf("error: seedURL is not a valid server\n");
        free(seedURL);
        free(pageDirectory);
        return 1;
    }

    // check if pageDirectory exists
    if (!isDir(pageDirectory)) {
        printf("error: pageDirectory is not valid\n");
        free(seedURL);
        free(pageDirectory);
        return 1;
    }

    // check if maxDepth is int
    if (sscanf(argv[3], "%d", &maxDepth) != 1) {
        printf("error: maxDepth is not an int\n");
        return 1;
    } 
    
    // convert third argument into integer
    maxDepth = atoi(argv[3]);
    
    // check if maxDepth is negative
    if (maxDepth < 0) {
        printf("error: maxDepth cannot be negative\n");
        return 1;
    }

    // pass arguments to crawler
    crawler(seedURL, pageDirectory, maxDepth);
 
    free(pageDirectory);

    return 0;
}


/**************** crawler() ****************/
void
crawler(char *seedURL, char *pageDirectory, int maxDepth)
{
    // create bag to hold pages to explore
    bag_t *bag = bag_new();     

    // create hashtable to hold urls
    hashtable_t *ht = hashtable_new(50);        

    // creat a webpage for seedURL
    webpage_t *seedpage = webpage_new(seedURL, 0, NULL);

    // add seedpage to the bag and hashtable
    bag_insert(bag, seedpage);                     
    int *item = malloc(sizeof(int));
    *item = 3;                          // arbitrary number chosen as item for ht
    hashtable_insert(ht, seedURL, item);         

    int ID = 0;
    bool notEmpty = true;
    while (notEmpty) {
        webpage_t *page = bag_extract(bag);     // extract webpage from the bag
        if (page == NULL) {
            notEmpty = false;
            break;                              // no more pages to add
        }
        if (!pagefetcher(page)) {
            webpage_delete(page);               // invalid page
            continue;
        } 
        pagesaver(page, pageDirectory, ID++);   // save page info to file       
        int depth = webpage_getDepth(page);
        if (depth < maxDepth) {
            pagescanner(bag, ht, page, depth);  // get urls from page
        }
        webpage_delete(page);                   // done with page
    }    
      
    
    // delete the bag and hashtable
    bag_delete(bag, NULL);    
    hashtable_delete(ht, mydelete);
}

/*************** mydelete **************/
/* deletes the items in the hashtable */
void
mydelete(void *item)
{   
    if (item != NULL) {
        free(item);
    }
}


