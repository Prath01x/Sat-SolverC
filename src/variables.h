#pragma once

#include <stdlib.h>

/**
 * This file provides means to manage variables without duplicates.
 */

typedef enum TruthValue { UNDEFINED, TRUE, FALSE } TruthValue;

/**
 * Type to represent indices into the variable table.
 *
 * Valid indices are strictly greater than 0, 0 can be used as an indicator for
 * an invalid value.
 */
typedef unsigned VarIndex;

/**
 * Struct to manage variables without duplicates.
 */
typedef struct VarTable VarTable;

/**
 * Retrieves the assigned truth value of a variable.
 *
 * @param vt  the underlying variable table
 * @param vi  the index of the variable to read
 * @return    the truth value
 */
TruthValue getVariableValue(VarTable* vt, VarIndex vi);

/**
 * Retrieves the name of a variable.
 *
 * @param vt  the underlying variable table
 * @param vi  the index of the variable to read
 * @return    the name
 */
char* getVariableName(VarTable* vt, VarIndex vi);

// Forward declaration of Clause to avoid circular dependencies
typedef struct Clause Clause;

/**
 * Adds a clause as a parent to a variable.
 *
 * @param vt  the underlying variable table
 * @param vi  the index of the variable to read
 * @param c   the clause to add
 */
void addParentClause(VarTable* vt, VarIndex vi, Clause* c);

/**
 * Updates the value of a variable.
 *
 * This function also updates the truth values of all parent clauses.
 *
 * @param vt   the underlying variable table
 * @param vi   a variable
 * @param val  the new truth value
 */
void updateVariableValue(VarTable* vt, VarIndex vi, TruthValue val);

/**
 * Retrieves the next undefined variable.
 *
 * @param vt  the underlying variable table
 * @return    the index of an undefined variable, 0 if there is none
 */
VarIndex getNextUndefinedVariable(VarTable* vt);

/**
 * Creates a new variable table.
 *
 * @return  the new variable table
 */
VarTable* mkVarTable(void);

/**
 * Frees a variable table.
 *
 * @param vt  a variable table
 */
void freeVarTable(VarTable* vt);

/**
 * Creates a new variable and adds it to a variable table if it is not already
 * contained.
 *
 * Names may only consist of letters and digits.
 *
 * The name should be a NULL-terminated string in a memory region obtained from
 * malloc. It will be freed when the variable table is freed.
 *
 * @param vt    the variable table
 * @param name  a name for the variable
 * @return      the index of the variable
 */
VarIndex mkVariable(VarTable* vt, char* name);

/**
 * Creates a new variable with a fresh name.
 *
 * Names should conform to "$(0|[1-9][0-9]*)" (e.g. $0, $1, ...).
 *
 * @param vt  the variable table
 * @return    the index of the variable
 */
VarIndex mkFreshVariable(VarTable* vt);

/**
 * Prints the content of a variable table to stdout.
 *
 * @param vt  a variable table
 */
void printVarTable(VarTable* vt);

/**
 * Prints a satisfying assignment for all non-Tseitin variables to stderr.
 * The assignment is either TRUE or FALSE.
 * This is a satisfying assignment if the variable table represents the state
 * after a SAT execution of the DPLL algorithm.
 *
 * Do not use this function for debugging purposes!
 *
 * @param vt  a variable table after a SAT execution
 */
void printSatisfyingAssignmentEval(VarTable* vt);
