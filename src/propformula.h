#pragma once

#include "variables.h"

/**
 * This file provides data structures to represent general propositional
 * formulas.
 */

typedef enum FormulaKind {
    VAR,

    AND,
    OR,
    IMPLIES,
    EQUIV,

    NOT,
} FormulaKind;

/**
 * A recursive struct to represent propositional formulas. The union entry
 * contains the appropriate information depending on the kind of the formula.
 */
typedef struct PropFormula {
    FormulaKind kind;

    union {
        VarIndex var;  // for formulas only consisting of a variable
        struct PropFormula* single_op;     // for unary operators
        struct PropFormula*(operands[2]);  // for binary operators
    } data;
} PropFormula;

/**
 * Creates a new variable formula and adds the variable to the variable table
 * if necessary. The given string for the name will be freed automatically
 * when the variable table is freed.
 *
 * @param vt    a variable table
 * @param name  a name for the variable
 * @return      the resulting formula
 */
PropFormula* mkVarFormula(VarTable* vt, char* name);

/**
 * Creates a new binary formula.
 *
 * @param kind      a binary operator
 * @param left_op   the formula for the left operand
 * @param right_op  the formula for the right operand
 * @return          the resulting formula
 */
PropFormula* mkBinaryFormula(FormulaKind kind, PropFormula* left_op,
                             PropFormula* right_op);

/**
 * Creates a new unary formula.
 *
 * @param kind     a unary operator
 * @param operand  the formula for the operand
 * @return         the resulting formula
 */
PropFormula* mkUnaryFormula(FormulaKind kind, PropFormula* operand);

/**
 * Frees a formula recursively.
 *
 * Contained variables are not freed.
 *
 * @param pf  a formula to free
 */
void freeFormula(PropFormula* pf);

/**
 * Prints a textual representation of a propositional formula to stdout.
 *
 * @param vt  a variable table
 * @param pf  a formula
 */
void prettyPrintFormula(VarTable* vt, PropFormula* pf);

/**
 * Prints a textual representation of a propositional formula to stderr.
 *
 * Do not use this function for debugging purposes!
 *
 * @param vt  a variable table
 * @param pf  a formula
 */
void prettyPrintFormulaEval(VarTable* vt, PropFormula* pf);
