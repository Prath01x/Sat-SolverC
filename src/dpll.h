#pragma once

#include "cnf.h"
#include "variables.h"

/**
 * Tests whether a formula in CNF is satisfiable.
 *
 * @param vt       the underlying variable table
 * @param cnf      a formula to test
 * @return         1 if the formula is satisfiable, 0 otherwise
 */
char isSatisfiable(VarTable *vt, CNF *cnf);
