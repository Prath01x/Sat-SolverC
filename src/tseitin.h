#pragma once

#include "cnf.h"
#include "propformula.h"
#include "variables.h"

/**
 * Converts a propositional formula F to CNF, such that F is satisfiable iff
 * the resulting CNF is satisfiable.
 *
 * @param vt  the underlying variable table
 * @param pf  a propositional formula
 * @return    the resulting CNF
 */
CNF* getCNF(VarTable* vt, const PropFormula* pf);
