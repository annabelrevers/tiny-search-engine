/*
 * pagedir.h - header file for the CS50 'pagedir' module
 *
 * The 'pagedir' module is a suite of functions to help the crawler write pages to
 * the pageDirectory and help the indexer read them back in
 *
 * Annabel Revers, February 2021
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>

/**************** functions *******************/

/**************** isCrawlerDir *************/
/* Determines whether the pathname provided as pageDirectory is a Crawler-
 * produced directory.
 *
 * Caller provides:
 *  valid pointer to a directory name
 * We return:
 *  true is pageDirecotry is a crawler-produced directory, else false
 */
bool isCrawlerDir(char *pageDirectory);

/**************** isDir ****************/
/* Determines whether the pathname provided as pageDirectory is
 * indeed a directory, and is writable.
 *
 * Caller provides:
 *  valid pointer to directory name
 * We return: 
 *  true if pageDirectory is a writable directory, otherwise false
 */
bool isDir(char *pageDiretory);

/**************** pagefetcher **************/
/* Retrieves the webpage for a URL.
 *
 * Caller provides: 
 *  valid pointer to webpage
 * We return: 
 *  true if page was successfully fetched, false if error
 */
bool pagefetcher(webpage_t *page);

/**************** pagescanner ****************/
/* Parses a webpage to extract all its embedded URLs, creates webpages for
 * the URLs and adds them to our bag.
 *
 * Caller provides:
 *  valid pointer to bag, hashtable, page, and valid integer
 * We return: 
 *  nothing
 */
void
pagescanner(bag_t *bag, hashtable_t *ht, webpage_t *page, int depth); 

/**************** pagesaver ****************/
/* Writes a webpage to the pageDirectory with a unique document ID, 
 * with its URL on the first line, depth on the second line, and html 
 * in the rest of the file.
 *
 * Caller provides:
 *  valid pointer to page, valid pointer to directory name, and valid integer
 * We return:
 *  nothing */
void pagesaver(webpage_t *page, char *pageDirectory, int ID);

#endif // __PAGEDIR_H
