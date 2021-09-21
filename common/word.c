/*
 * word.c - CS50 word module
 *
 * see word.h for more information
 *
 * Annabel Revers, February 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "word.h"

/************* functions **************/

/************** NormalizeWord *************/
/* see word.h for more information */
void
NormalizeWord(char *word)
{
    char *ptr;
    char mychar;
    for (ptr = word; *ptr; ptr++) {
        mychar = tolower(*ptr);
        *ptr = mychar;
    }
}

