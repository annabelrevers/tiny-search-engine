/*
 * index.h - header file for CS50 index module
 *
 * index is a suite of functions that save and load index files
 *
 * Annabel Revers, February 2021
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>

/*********** functions ***********/

/*********** indexSave **********/
/* Writes the inverse-index data structure into a file.
 *
 * Caller provides:
 *  valid hashtable pointer, valid name for file to write into
 * We return: 
 *  nothing
 */
void indexSave(hashtable_t *ht, char *indexFilename);

/************ indexLoad ***********/
/* Loads an inverse-index data structure for an index file.
 *
 * Caller provides:
 *  valid pointer to an index file
 * We return:
 *  hashtable as our inverse-index data structure
 */

hashtable_t * indexLoad(char *oldIndexFilename);

/************ indexDelete **************/
/* Deletes the hashtable inverse-index data structure 
 *
 * Caller provides:
 *  valid pointer to hashtable
 * We return: 
 *  nothing
 */

void indexDelete(hashtable_t *ht);

#endif // __INDEX_H
