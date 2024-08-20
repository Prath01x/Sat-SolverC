#include <stdio.h>
#include <string.h>

#include "dpll.h"
#include "parser.h"
#include "propformula.h"
#include "tseitin.h"
#include "variables.h"

void printUsage(char* bin) {
    printf(
        "Usage: %s [options] [file]\n\n"
        "If no file is specified, input from stdin is expected.\n\n"
        "Options:\n"
        "  -v, --verbose       Print additional data.\n"
        "  -p, --printformula  Only parse the propositional formula and print "
        "it.\n"
        "  -c, --printcnf      Only construct the CNF for the formula and "
        "print it.\n"
        "  -h, --help          Display this message.\n",
        bin);
}

int main(int argc, char* argv[]) {
    FILE* input = stdin;

    char has_file = 0;
    char verbose = 0;
    char formula_only = 0;
    char cnf_only = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'v':
                    verbose = 1;
                    break;
                case 'p':
                    formula_only = 1;
                    break;
                case 'c':
                    cnf_only = 1;
                    break;
                case 'h':
                    printUsage(argv[0]);
                    exit(0);
                    break;
                case '-':
                    if (0 == strncmp(argv[i], "--printformula", 15)) {
                        formula_only = 1;
                    } else if (0 == strncmp(argv[i], "--printcnf", 11)) {
                        cnf_only = 1;
                    } else if (0 == strncmp(argv[i], "--verbose", 10)) {
                        verbose = 1;
                    } else if (0 == strncmp(argv[i], "--help", 7)) {
                        printUsage(argv[0]);
                        exit(0);
                    } else {
                        printUsage(argv[0]);
                        exit(2);
                    }
                    break;
                default:
                    printUsage(argv[0]);
                    exit(2);
            }
        } else {
            if (has_file) {
                fputs("Error: too many file arguments!", stderr);
                return 2;
            }
            input = fopen(argv[i], "r");
            if (input == NULL) {
                fputs("Error: invalid file argument!", stderr);
                return 2;
            }
            has_file = 1;
        }
    }

    VarTable* vt = mkVarTable();

    PropFormula* pf = parseFormula(input, vt);

    if (formula_only) {
        fprintf(stderr, "propositional formula:\n  ");
        prettyPrintFormulaEval(vt, pf);
        fprintf(stderr, "\n");

        freeFormula(pf);
        pf = NULL;

        freeVarTable(vt);

        if (input != stdin) {
            fclose(input);
        }

        return 0;
    }

    if (verbose) {
        printf("propositional formula:\n  ");
        prettyPrintFormulaEval(vt, pf);
        printf("\n\n");
    }

    CNF* cnf = getCNF(vt, pf);

    if (cnf_only) {
        fprintf(stderr, "conjunctive normal form:\n  ");
        prettyPrintCNF(vt, cnf);

        freeFormula(pf);
        pf = NULL;

        freeCNF(cnf);
        cnf = NULL;

        freeVarTable(vt);

        if (input != stdin) {
            fclose(input);
        }

        return 0;
    }

    if (verbose) {
        printf("conjunctive normal form:\n  ");
        prettyPrintCNF(vt, cnf);
        printf("\n");
    }

    char sat = 0;

    if (isSatisfiable(vt, cnf)) {
        fprintf(stderr, "sat with assignment:\n");
        printSatisfyingAssignmentEval(vt);
        sat = 1;
    } else {
        fprintf(stderr, "unsat\n");
        sat = 0;
    }

    freeFormula(pf);
    pf = NULL;

    freeCNF(cnf);
    cnf = NULL;

    freeVarTable(vt);

    if (input != stdin) {
        fclose(input);
    }
    if (sat) {
        return 10;
    } else {
        return 20;
    }
}
