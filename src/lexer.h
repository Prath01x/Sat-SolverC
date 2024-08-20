#pragma once

#include <stdio.h>

/**
 * Reads the next string separated by whitespace from the input.
 *
 * It must not be longer than 31 characters. The returned memory region must be
 * freed.
 *
 * @param input  an open file or stdin
 * @return       a string in a memory region obtained from malloc or NULL if the
 *              end of the input was reached
 */
char* nextToken(FILE* input);
