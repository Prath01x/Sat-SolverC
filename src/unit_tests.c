#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cnf.h"
#include "propformula.h"
#include "test_common.h"
#include "tseitin.h"
#include "variables.h"

unsigned cnf_size(CNF* cnf) {
    unsigned res = 0;
    for (ListIterator it = mkIterator(&cnf->clauses); isValid(&it); next(&it)) {
        res++;
    }

    return res;
}

char literal_eq(VarTable* vt, Literal l, char* x) {
    if (x == NULL) {
        return l == 0;
    }

    if (l == 0) {
        return 0;
    }

    assert(x[0] == '-' || x[0] == '+');

    if ((x[0] == '-' && l > 0) || (x[0] == '+' && l < 0)) {
        return 0;
    }

    char* t = getVariableName(vt, abs(l));

    return 0 == strcmp(t, (x + 1));
}

char clause_eq(VarTable* vt, Clause* cl, char* a, char* b, char* c) {
    return literal_eq(vt, cl->literals[0], a) &&
           literal_eq(vt, cl->literals[1], b) &&
           literal_eq(vt, cl->literals[2], c);
}

char cnf_contains(VarTable* vt, CNF* cnf, char* a, char* b, char* c) {
    for (ListIterator it = mkIterator(&cnf->clauses); isValid(&it); next(&it)) {
        Clause* cl = (Clause*)getCurr(&it);
        if (clause_eq(vt, cl, a, b, c) || clause_eq(vt, cl, a, c, b) ||
            clause_eq(vt, cl, b, a, c) || clause_eq(vt, cl, b, c, a) ||
            clause_eq(vt, cl, c, a, b) || clause_eq(vt, cl, c, b, a)) {
            return 1;
        }
    }

    return 0;
}

result_t check_variable(const char* test) {
    (void)test;
    VarTable* vt = mkVarTable();

    char* name = malloc(4 * sizeof(char));
    memcpy(name, "foo", 4 * sizeof(char));

    PropFormula* pf = mkVarFormula(vt, name);

    CNF* cnf = getCNF(vt, pf);

#define CHECK(v)              \
    {                         \
        if (!(v)) {           \
            freeFormula(pf);  \
            freeCNF(cnf);     \
            freeVarTable(vt); \
            return FAILURE;   \
        }                     \
    }

    CHECK(cnf_size(cnf) == 1);
    CHECK(cnf_contains(vt, cnf, "+foo", NULL, NULL));

    freeFormula(pf);
    freeCNF(cnf);
    freeVarTable(vt);

#undef CHECK

    return SUCCESS;
}

result_t check_tseitin01(const char* test) {
    (void)test;
    VarTable* vt = mkVarTable();

    char* name1 = malloc(2 * sizeof(char));
    memcpy(name1, "c", 2 * sizeof(char));

    char* name2 = malloc(2 * sizeof(char));
    memcpy(name2, "d", 2 * sizeof(char));

    /* "c d ! ||" */
    PropFormula* pf1 = mkVarFormula(vt, name1);
    PropFormula* pf2 = mkVarFormula(vt, name2);
    PropFormula* pf3 = mkUnaryFormula(NOT, pf2);
    PropFormula* pf4 = mkBinaryFormula(OR, pf1, pf3);

    CNF* cnf = getCNF(vt, pf4);

#define CHECK(v)              \
    {                         \
        if (!(v)) {           \
            freeFormula(pf4); \
            freeCNF(cnf);     \
            freeVarTable(vt); \
            return FAILURE;   \
        }                     \
    }

    CHECK(cnf_size(cnf) == 6);
    CHECK(cnf_contains(vt, cnf, "+$1", NULL, NULL));
    CHECK(cnf_contains(vt, cnf, "-$1", "+c", "+$0"));
    CHECK(cnf_contains(vt, cnf, "+$1", "-$0", NULL));
    CHECK(cnf_contains(vt, cnf, "+$1", "-c", NULL));
    CHECK(cnf_contains(vt, cnf, "+$0", "+d", NULL));
    CHECK(cnf_contains(vt, cnf, "-$0", "-d", NULL));

    freeFormula(pf4);
    freeCNF(cnf);
    freeVarTable(vt);

#undef CHECK

    return SUCCESS;
}

result_t check_array_equal(unsigned size, int* A, int* B) {
    for (unsigned i = 0; i < size; i++) {
        if (A[i] != B[i]) {
            return FAILURE;
        }
    }

    return SUCCESS;
}

result_t check_empty(const char* test) {
    (void)test;
    List l = mkList();

    if (!isEmpty(&l)) {
        return FAILURE;
    }

    return SUCCESS;
}

result_t check_empty_clear(const char* test) {
    (void)test;
    List l = mkList();

    clearList(&l);

    if (!isEmpty(&l)) {
        return FAILURE;
    }

    return SUCCESS;
}

result_t check_single_element_push(const char* test) {
    (void)test;
    List l = mkList();

    int A[18] = {1, 17, 3, 15, 5, 13, 7, 11, 9, 10, 8, 12, 6, 14, 4, 16, 2, 18};
    int B[18] = {1, 17, 3, 15, 5, 13, 7, 11, 9, 10, 8, 12, 6, 14, 4, 16, 2, 18};

    push(&l, A + 0);

    if (isEmpty(&l)) {
        return FAILURE;
    }

    clearList(&l);

    return check_array_equal(18, A, B);
}

result_t check_single_element_peek(const char* test) {
    (void)test;
    List l = mkList();

    int A[18] = {1, 17, 3, 15, 5, 13, 7, 11, 9, 10, 8, 12, 6, 14, 4, 16, 2, 18};
    int B[18] = {1, 17, 3, 15, 5, 13, 7, 11, 9, 10, 8, 12, 6, 14, 4, 16, 2, 18};

    push(&l, A + 0);

    if (isEmpty(&l)) {
        return FAILURE;
    }

    int* t = peek(&l);

    if (isEmpty(&l)) {
        return FAILURE;
    }

    if (t != A + 0) {
        return FAILURE;
    }

    if (*t != 1) {
        return FAILURE;
    }

    clearList(&l);

    return check_array_equal(18, A, B);
}

result_t check_single_element_pop(const char* test) {
    (void)test;
    List l = mkList();

    int A[18] = {1, 17, 3, 15, 5, 13, 7, 11, 9, 10, 8, 12, 6, 14, 4, 16, 2, 18};
    int B[18] = {1, 17, 3, 15, 5, 13, 7, 11, 9, 10, 8, 12, 6, 14, 4, 16, 2, 18};

    push(&l, A + 0);

    if (isEmpty(&l)) {
        return FAILURE;
    }

    pop(&l);

    if (!isEmpty(&l)) {
        return FAILURE;
    }

    clearList(&l);

    return check_array_equal(18, A, B);
}

test_fun_t get_test(const char* test) {
    TEST("public.cnf.variable", check_variable);
    TEST("public.cnf.tseitin01", check_tseitin01);

    TEST("public.stack.empty", check_empty);
    TEST("public.stack.emptyclear", check_empty_clear);
    TEST("public.stack.singlepop", check_single_element_pop);
    TEST("public.stack.singlepush", check_single_element_push);
    TEST("public.stack.singlepeek", check_single_element_peek);

    return NULL;
}
