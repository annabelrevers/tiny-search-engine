/*
 * word.h - header file for CS50 word module
 *
 * The word module implements 'NormalizeWord'
 *
 * Annabel Revers, February 2020
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdlib.h>

/*************** functions *************/

/*************** NormalizeWord *************/
/* Converts all letters in a char * array to lower case
 *
 * Caller provides:
 *  valid pointer to word
 * We return:
 *  nothing
 */
void NormalizeWord(char *word);

#endif // __WEBPAGE_H
