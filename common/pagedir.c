/* 
 * pagedir - see pagedir.h for usage.
 *
 * Annabel Revers, February 2020
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "webpage.h"
#include "hashtable.h"
#include "bag.h"
#include "pagedir.h"

/*************** local functions **************/
inline static void logr(const char *word, const int depth, const char *url);
        
/*************** isCrawlerDir ***************/
/* see pagedir.h for more info */
bool
isCrawlerDir(char *pageDirectory)
{
    char *filepath = malloc(strlen(pageDirectory)+10);
    sprintf(filepath, "%s/.crawler", pageDirectory);

    FILE *fp = fopen(filepath, "r");
    if (fp != NULL) {
        fclose(fp);
        free(filepath);
        return true;
    } else {
        free(filepath);
        return false;
    }
}
    
/*************** isDir ******************/
/* see pagedir.h for more info */
bool 
isDir(char *pageDirectory)
{
    char *filepath = malloc(strlen(pageDirectory)+10);
    sprintf(filepath, "%s/.crawler", pageDirectory);

    FILE *fp = fopen(filepath, "w");
    if (fp != NULL) {
        fclose(fp);
        free(filepath);
        return true;
    } else {
        free(filepath);
        return false;
    }
}

/**************** pagefetcher ****************/
/* see pagedir.h for more info */
bool
pagefetcher(webpage_t *page)
{
    if (webpage_fetch(page)) {
        int depth = webpage_getDepth(page);
        const char *url = webpage_getURL(page);
        logr("Fetched", depth, url);
        return true;
    } else {
        printf("error: nonexistent page\n");
        return false;
    }       
}

/**************** pagescanner ****************/
/* see pagedir.h for more info */
void
pagescanner(bag_t *bag, hashtable_t *ht, webpage_t *page, int depth) 
{
    // get url
    const char *url = webpage_getURL(page);
    logr("Scanning", depth, url);
   
    int pos = 0;
    char *result;
    while ((result = webpage_getNextURL(page, &pos)) != NULL) {
        int mydepth = depth + 1;
        logr("Found", depth, result);
        if (NormalizeURL(result) && IsInternalURL(result)) {
            int *item = malloc(sizeof(int));
            *item = 3;                          // arbitrary int to add as item to ht
            if (hashtable_insert(ht, result, item)) {
                webpage_t *newpage = webpage_new(result, mydepth, NULL);
                bag_insert(bag, newpage);
                logr("Added", depth, result);
            } else {
                logr("IgnDupl", depth, result);
                free(item);
                free(result);
            }
        } else {
          logr("IgnExtrn", depth, result);
          free(result);
        }    
    }
}

/**************** pagesaver ****************/
/* see pagedir.h for more info */
void
pagesaver(webpage_t *page, char *pageDirectory, int ID)
{
    // make our own filepath
    char *filepath = malloc(strlen(pageDirectory) + 3);
    sprintf(filepath, "%s/%d", pageDirectory, ID);

    // get url, depth, and html
    char *url = webpage_getURL(page);
    int depth = webpage_getDepth(page);
    char *html = webpage_getHTML(page);

    // write information into file
    FILE *fp = fopen(filepath, "w");
    if (fp != NULL) {
        fprintf(fp, "%s\n", url);
        fprintf(fp, "%d\n", depth);
        fprintf(fp, "%s", html);
        fclose(fp);
    } else {
        printf("error creating file\n");
    }

    free(filepath);
}

/**************** logr ****************/
/* prints updates on progress of crawler */
inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}
