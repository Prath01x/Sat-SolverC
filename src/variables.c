#include "variables.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cnf.h"
#include "list.h"

#define VAR_MAX_LEN 32

#define INIT_SIZE 8

/**
 * Struct for representing a named variable.
 *
 * Contains a list of parent clauses that contain a literal with this variable.
 */
typedef struct Variable {
    char* name;
    TruthValue val;
    List parentClauses;
} Variable;

struct VarTable {
    Variable* content;
    unsigned size;
    unsigned capacity;
};

Variable* getVariableForIndex(VarTable* vt, VarIndex i) {
    assert(0 < i);
    assert(i <= vt->size);
    // valid variable indices start at 1, array indices at 0
    return vt->content + (i - 1);
}

TruthValue getVariableValue(VarTable* vt, VarIndex vi) {
    Variable* var = getVariableForIndex(vt, vi);
    return var->val;
}

char* getVariableName(VarTable* vt, VarIndex vi) {
    Variable* var = getVariableForIndex(vt, vi);
    return var->name;
}

void addParentClause(VarTable* vt, VarIndex vi, Clause* c) {
    Variable* var = getVariableForIndex(vt, vi);
    push(&var->parentClauses, c);
}

void updateVariableValue(VarTable* vt, VarIndex vi, TruthValue val) {
    Variable* var = getVariableForIndex(vt, vi);

    var->val = val;

    ListIterator it = mkIterator(&var->parentClauses);

    while (isValid(&it)) {
        Clause* c = (Clause*)getCurr(&it);
        updateTruthValue(vt, c);
        next(&it);
    }
}

VarIndex getNextUndefinedVariable(VarTable* vt) {
    for (unsigned i = 1; i <= vt->size; i++) {
        Variable* current = getVariableForIndex(vt, i);

        if (current->val == UNDEFINED) {
            return i;
        }
    }

    return 0;
}

VarTable* mkVarTable(void) {
    VarTable* res = (VarTable*)malloc(sizeof(VarTable));

    res->content = (Variable*)malloc(INIT_SIZE * sizeof(Variable));
    res->size = 0;
    res->capacity = INIT_SIZE;

    return res;
}

VarIndex mkVariable(VarTable* vt, char* name) {
    // first check if a variable with this name already exists
    for (unsigned i = 1; i <= vt->size; i++) {
        Variable* v = getVariableForIndex(vt, i);

        int cmp = strcmp(v->name, name);

        if (cmp == 0) {
            free(name);
            return i;
        }
    }

    // if not, insert a new one into the table

    if (vt->size == vt->capacity) {  // increase capacity if necessary
        vt->capacity *= 2;
        vt->content =
            (Variable*)realloc(vt->content, vt->capacity * sizeof(Variable));
    }

    vt->size += 1;
    VarIndex idx = vt->size;

    Variable* var = getVariableForIndex(vt, idx);

    var->name = name;
    var->val = UNDEFINED;
    var->parentClauses = mkList();

    return idx;
}

/**
 * Frees the data contained in a variable (but not the variable itself!).
 */
void clearVariable(Variable* var) {
    clearList(&var->parentClauses);
    free(var->name);
}

void freeVarTable(VarTable* varTable) {
    for (unsigned i = 1; i <= varTable->size; i++) {
        clearVariable(getVariableForIndex(varTable, i));
    }
    free(varTable->content);
    free(varTable);
}

VarIndex mkFreshVariable(VarTable* vt) {
    static unsigned index = 0;

    char buf[VAR_MAX_LEN];

    sprintf(buf, "$%d", index);
    buf[VAR_MAX_LEN - 1] = '\0';

    char* name = (char*)malloc(sizeof(char) * (strlen(buf) + 1));

    strcpy(name, buf);

    index++;

    return mkVariable(vt, name);
}

void printVarTable(VarTable* vt) {
    char first = 1;
    printf("variable table:\n  ");
    for (unsigned i = 1; i <= vt->size; i++) {
        Variable* current = getVariableForIndex(vt, i);
        if (!first) {
            printf("\n  ");
        }

        const char* val = NULL;

        switch (current->val) {
            case TRUE:
                val = "TRUE";
                break;

            case FALSE:
                val = "FALSE";
                break;

            case UNDEFINED:
                val = "UNDEFINED";
                break;
        }

        printf("%s -> %s", current->name, val);

        first = 0;
    }
    printf("\n");
}

/**
 * Comparison function for variables. The order of variables is determined by
 * the lexicographical order of their names (strcmp).
 */
int var_cmp(const void* a, const void* b) {
    Variable* var_a = (Variable*)a;
    Variable* var_b = (Variable*)b;

    return strcmp(var_a->name, var_b->name);
}

void printSatisfyingAssignmentEval(VarTable* vt) {
    // copy the variables into a new buffer and sort them there
    unsigned size = vt->size * sizeof(Variable);
    Variable* buf = (Variable*)malloc(size);
    memcpy(buf, vt->content, size);
    qsort(buf, vt->size, sizeof(Variable), var_cmp);

    char first = 1;
    fprintf(stderr, "  ");
    for (unsigned i = 0; i < vt->size; i++) {
        Variable* current = buf + i;

        // do not create assignments for tseitin variables
        if (current->name[0] == '$') {
            continue;
        }

        if (!first) {
            fprintf(stderr, "\n  ");
        }

        const char* val = NULL;

        switch (current->val) {
            case UNDEFINED:  // undefined variables can be assigned arbitrarily
            case TRUE:
                val = "TRUE";
                break;

            case FALSE:
                val = "FALSE";
                break;
        }

        fprintf(stderr, "%s -> %s", current->name, val);

        first = 0;
    }
    fprintf(stderr, "\n");
    free(buf);
}
