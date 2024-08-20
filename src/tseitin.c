#include "tseitin.h"

#include <stdio.h>

#include "cnf.h"
#include "err.h"
#include "propformula.h"
#include "util.h"
#include "variables.h"

/**
 * Inserts a clause with one literal into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    a literal
 */
void addUnaryClause(VarTable* vt, CNF* cnf, Literal a) {
    Clause* clause = mkTernaryClause(vt, a, 0, 0);
    addClauseToCNF(cnf, clause);
}

/**
 * Inserts a clause with two literals into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    the first literal
 * @param b    the second literal
 */
void addBinaryClause(VarTable* vt, CNF* cnf, Literal a, Literal b) {
    Clause* clause = mkTernaryClause(vt, a, b, 0);
    addClauseToCNF(cnf, clause);
}

/**
 * Inserts a clause with three literals into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    the first literal
 * @param b    the second literal
 * @param c    the third literal
 */
void addTernaryClause(VarTable* vt, CNF* cnf, Literal a, Literal b, Literal c) {
    Clause* clause = mkTernaryClause(vt, a, b, c);
    addClauseToCNF(cnf, clause);
}

/**
 * Adds clauses for a propositional formula to a CNF.
 *
 * For a propositional formula pf, clauses that are added that are equivalent to
 *
 *     x <=> pf
 *
 * where x is usually a fresh variable. This variable is also returned.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param pf   a propositional formula
 * @return     the variable x, as described above
 */
VarIndex addClauses(VarTable* vt, CNF* cnf, const PropFormula* pf) {
    VarIndex var;
    if (pf->kind == VAR) {
        return pf->data.var;
    }

    if (pf->kind == NOT) {
        VarIndex subVar = addClauses(vt, cnf, pf->data.single_op);
        var = mkFreshVariable(vt);

        addBinaryClause(vt, cnf, -var, -subVar);
        addBinaryClause(vt, cnf, subVar, var);
        return var;
    }

    if (pf->kind == AND) {
        VarIndex leftVar = addClauses(vt, cnf, pf->data.operands[0]);
        VarIndex rightVar = addClauses(vt, cnf, pf->data.operands[1]);
        var = mkFreshVariable(vt);
        addBinaryClause(vt, cnf, -var, leftVar);
        addBinaryClause(vt, cnf, -var, rightVar);
        addTernaryClause(vt, cnf, -leftVar, -rightVar, var);
        return var;
    }

    if (pf->kind == OR) {
        VarIndex l = addClauses(vt, cnf, pf->data.operands[0]);
        VarIndex r = addClauses(vt, cnf, pf->data.operands[1]);
        var = mkFreshVariable(vt);

        addTernaryClause(vt, cnf, -var, l, r);
        addBinaryClause(vt, cnf, -l, var);
        addBinaryClause(vt, cnf, -r, var);

        return var;
    }

    if (pf->kind == IMPLIES) {
        VarIndex le = addClauses(vt, cnf, pf->data.operands[0]);
        VarIndex ri = addClauses(vt, cnf, pf->data.operands[1]);
        var = mkFreshVariable(vt);

        addTernaryClause(vt, cnf, -var, -le, ri);
        addBinaryClause(vt, cnf, le, var);
        addBinaryClause(vt, cnf, -ri, var);

        return var;
    }

    if (pf->kind == EQUIV) {
        VarIndex lef = addClauses(vt, cnf, pf->data.operands[0]);
        VarIndex rig = addClauses(vt, cnf, pf->data.operands[1]);
        var = mkFreshVariable(vt);

        addTernaryClause(vt, cnf, -var, -lef, rig);
        addTernaryClause(vt, cnf, -var, -rig, lef);
        addTernaryClause(vt, cnf, var, -lef, -rig);
        addTernaryClause(vt, cnf, var, lef, rig);
        return var;
    }
}

CNF* getCNF(VarTable* vt, const PropFormula* f) {
    CNF* res = mkCNF();

    VarIndex x = addClauses(vt, res, f);

    addUnaryClause(vt, res, x);

    return res;
}
