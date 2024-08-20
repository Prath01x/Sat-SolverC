#pragma once

#include <stdio.h>

#include "propformula.h"
#include "variables.h"

/**
 * Reads a propositional formula in reverse polish notation from the input.
 *
 * The program is aborted if an error is detected in the input.
 *
 * @param input  a readable open file or stdin
 * @param vt     a variable table
 * @return       the resulting propositional formula
 */
PropFormula* parseFormula(FILE* input, VarTable* vt);
