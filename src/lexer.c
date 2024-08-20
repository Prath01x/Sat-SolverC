#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"

#define ID_MAX_LEN 32

/**
 * Checks if a character is a whitespace.
 *
 * @param c  a character
 * @return   1 if it is a whitespace, 0 otherwise
 */
char isWhiteSpace(int c) {
    return (c == ' ') || (c == '\t') || (c == '\v') || (c == '\n');
}

/**
 * Skips all leading whitespaces and returns the first non-whitespace character.
 * This can be an end-of-file character.
 *
 * @param input  an open file or stdin
 * @return       a non-whitespace character
 */
int stripWhiteSpaces(FILE* input) {
    while (1) {
        int c = getc(input);
        if (!isWhiteSpace(c)) {
            return c;
        }
    }
}

char* nextToken(FILE* input) {
    static char reachedEOF = 0;

    if (reachedEOF) {
        return NULL;
    }

    char buf[ID_MAX_LEN];

    int t = stripWhiteSpaces(input);

    if (t == EOF) {
        reachedEOF = 1;
        return NULL;
    }

    char c = (char)t;

    int i = 0;

    while (i < ID_MAX_LEN - 1) {
        if (isWhiteSpace(c)) {
            break;
        }

        buf[i] = c;
        i++;

        int t = getc(input);

        if (t == EOF) {
            reachedEOF = 1;
            break;
        }
        c = (char)t;

        if (i == ID_MAX_LEN - 1) {
            err("Identifier exceeds maximal length");
        }
    }
    buf[i] = '\0';

    char* res = (char*)malloc((i + 1) * sizeof(char));

    return strcpy(res, buf);
}
