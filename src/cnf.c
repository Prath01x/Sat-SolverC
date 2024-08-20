#include "cnf.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Clause* mkTernaryClause(VarTable* vt, Literal a, Literal b, Literal c) {
    assert(a != 0);

    Clause* res = (Clause*)malloc(sizeof(Clause));
    res->val = UNDEFINED;

    res->literals[0] = a;
    res->literals[1] = b;
    res->literals[2] = c;

    // insert the clause into the list of parentClauses of the variables
    addParentClause(vt, abs(a), res);

    if (b != 0) {
        addParentClause(vt, abs(b), res);
    }

    if (c != 0) {
        addParentClause(vt, abs(c), res);
    }

    return res;
}

void freeClause(Clause* c) { free(c); }

CNF* mkCNF(void) {
    CNF* res = (CNF*)malloc(sizeof(CNF));
    res->clauses = mkList();
    return res;
}

void freeCNF(CNF* f) {
    while (!isEmpty(&f->clauses)) {
        Clause* c = (Clause*)peek(&f->clauses);
        freeClause(c);
        pop(&f->clauses);
    }
    free(f);
}

void addClauseToCNF(CNF* f, Clause* c) { push(&f->clauses, c); }

/**
 * Computes the value of a literal.
 *
 * @param vt  the underlying variable table
 * @param l   a literal
 * @return    the truth value of the literal
 */
TruthValue evalLiteral(VarTable* vt, Literal l) {
    assert(l != 0);

    VarIndex vi = (VarIndex)abs(l);
    TruthValue var_val = getVariableValue(vt, vi);

    if (l > 0) {  // positive literal
        return var_val;
    } else {  // negative literal
        switch (var_val) {
            case TRUE:
                return FALSE;
            case FALSE:
                return TRUE;
            default:
                return UNDEFINED;
        }
    }
}

void updateTruthValue(VarTable* vt, Clause* c) {
    TruthValue res = FALSE;

    for (int i = 0; i < LITERALS_PER_CLAUSE; i++) {
        if (c->literals[i] == 0) {
            continue;
        }

        TruthValue current = evalLiteral(vt, c->literals[i]);
        if (current == UNDEFINED) {
            res = UNDEFINED;
        } else if (current == TRUE) {
            c->val = TRUE;
            return;
        }
    }

    c->val = res;
}

Literal getUnitLiteral(VarTable* vt, Clause* c) {
    Literal res = 0;

    for (int i = 0; i < LITERALS_PER_CLAUSE; i++) {
        Literal current = c->literals[i];

        if (current == 0) {
            continue;
        }

        TruthValue val = evalLiteral(vt, current);

        switch (val) {
            case UNDEFINED:
                if (res == 0 || res == current) {
                    res = current;
                } else {
                    return 0;
                }
                break;
            case TRUE:
                return 0;
            default:
                break;
        }
    }

    return res;
}

TruthValue evalCNF(CNF* f) {
    TruthValue res = TRUE;

    ListIterator it = mkIterator(&f->clauses);
    while (isValid(&it)) {
        TruthValue current = ((Clause*)getCurr(&it))->val;
        if (current == UNDEFINED) {
            res = UNDEFINED;
        } else if (current == FALSE) {
            return FALSE;
        }
        next(&it);
    }

    return res;
}

/**
 * Prints a textual representation of a clause to stdout.
 *
 * @param c  the clause to print
 */
void prettyPrintClause(VarTable* vt, Clause* c) {
    for (int i = 0; i < LITERALS_PER_CLAUSE; i++) {
        Literal current = c->literals[i];

        if (current == 0) {
            continue;
        }

        if (i > 0) {
            printf(" || ");
        }

        if (current < 0) {
            printf("! ");
        }
        printf("%s", getVariableName(vt, abs(current)));
    }
}

void prettyPrintCNF(VarTable* vt, CNF* f) {
    ListIterator it = mkIterator(&f->clauses);
    char first = 1;
    while (isValid(&it)) {
        Clause* current = (Clause*)getCurr(&it);
        if (!first) {
            printf(" && ");
        }
        printf("(");

        prettyPrintClause(vt, current);

        printf(")");

        first = 0;
        next(&it);
    }
    printf("\n");
}