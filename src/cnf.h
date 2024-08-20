#pragma once

/**
 * In this file, we provide a data structure for representing a formula in
 * conjunctive normal form. Clauses may contain at most 3 literals.
 *
 * These data structures should only be created using the included functions!
 */

#include "list.h"
#include "variables.h"

/**
 * The type for representing literals, i.e. negated or non-negated variables.
 *
 * Positive literals are identical to the index of the corresponding variable
 * in the variable table.
 * Negative literals are represented by the negated index of the corresponding
 * variable.
 */
typedef int Literal;

#define LITERALS_PER_CLAUSE 3

/**
 * Struct for representing disjunctions of exactly 3 literals.
 *
 * If smaller clauses should be represented, 0 values can be inserted as
 * literals.
 *
 * Contains a TruthValue field that should be updated by a call to the
 * updateTruthValue() function if the value of a contained literal changes.
 */
typedef struct Clause {
    TruthValue val;
    Literal literals[LITERALS_PER_CLAUSE];
} Clause;

/**
 * Struct for representing conjunctions of clauses.
 */
typedef struct CNF {
    List clauses;
} CNF;

/**
 * Creates a new clause with three literals.
 *
 * @param vt  the underlying variable table
 * @param a   the first literal
 * @param b   the second literal
 * @param c   the third literal
 * @return  the new clause
 */
Clause* mkTernaryClause(VarTable* vt, Literal a, Literal b, Literal c);

/**
 * Frees a clause.
 *
 * @param c  the clause to be freed
 */
void freeClause(Clause* c);

/**
 * Creates a new empty CNF.
 *
 * @return  the new CNF
 */
CNF* mkCNF(void);

/**
 * Free a CNF and all its clauses.
 *
 * @param f  the CNF to be freed
 */
void freeCNF(CNF* f);

/**
 * Adds a new clause to a CNF. Duplicates are not removed.
 *
 * @param f  the CNF
 * @param c  the clause to be added
 */
void addClauseToCNF(CNF* f, Clause* c);

/**
 * Evaluates a clause and stores the result in it.
 *
 * @param vt  the underlying variable table
 * @param c   the clause to be evaluated
 */
void updateTruthValue(VarTable* vt, Clause* c);

/**
 * Tests if the clause contains exactly one literal that is UNDEFINED and
 * returns it.
 *
 * @param vt  the underlying variable table
 * @param c  the clause to be tested
 * @return   the unit literal if it exists (!= 0), 0 otherwise
 */
Literal getUnitLiteral(VarTable* vt, Clause* c);

/**
 * Evaluates a CNF.
 *
 * Based on the values stored in the clauses!
 *
 * @param f  the CNF to be evaluated
 * @return   TRUE if all clauses are TRUE, FALSE if there is a FALSE clause,
 *          UNDEFINED otherwise
 */
TruthValue evalCNF(CNF* f);

/**
 * Prints a textual representation of a CNF to stdout.
 *
 * @param vt  the underlying variable table
 * @param f  the CNF to be printed
 */
void prettyPrintCNF(VarTable* vt, CNF* f);
