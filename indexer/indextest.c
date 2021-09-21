/*
 * indextest.c - CS50 indextest module
 *
 * Annabel Revers, February 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "index.h"

int
main(int argc, char *argv[])
{
    // check for correct number of arguments
    if (argc != 3) {
        printf("usage: indextest oldIndexFilename newIndexFilename\n");
        exit(1);
    }

    // intialize variables for arguments
    char *oldIndexFilename = malloc(strlen(argv[1]) + 1);
    strcpy(oldIndexFilename, argv[1]);
    char *newIndexFilename = malloc(strlen(argv[2]) + 1);
    strcpy(newIndexFilename, argv[2]);

    // load inverse-index data structure
    hashtable_t *ht = indexLoad(oldIndexFilename);

    // save inverse-index data structure to file
    if (ht != NULL) {
        indexSave(ht, newIndexFilename);
        indexDelete(ht);
    } else {
        printf("error creating inverse-index data structure\n");
    }

    // free argument variables
    free(oldIndexFilename);
    free(newIndexFilename);
 }
