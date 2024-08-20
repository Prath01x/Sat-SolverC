#include "parser.h"

#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#include "err.h"
#include "lexer.h"
#include "list.h"
#include "util.h"
/**
 * Assigns symbols to strings.
 *
 * Aborts the program with an error message if an invalid input is detected.
 *
 * @param str  a string to translate
 * @return     the resulting symbol
 */
void err1(const char* format, ...) {
    va_list args;
    va_start(args, format);

    fprintf(stderr, "Error: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "!\n");

    va_end(args);

    exit(30);
}
FormulaKind toKind(char* token) {
    if (strcmp(token, "&&") == 0) {
        return AND;
    } else if (strcmp(token, "||") == 0) {
        return OR;
    } else if (strcmp(token, "=>") == 0) {
        return IMPLIES;
    } else if (strcmp(token, "<=>") == 0) {
        return EQUIV;
    } else if (strcmp(token, "!") == 0) {
        return NOT;
    } else {
        // Check if the token is a valid variable
        for (int i = 0; token[i]; i++) {
            if (!(isalnum(token[i]) || token[i] == '_')) {
                if (!(token[i] == '$' && i > 0)) {
                    err1("Invalid token: %s", token);
                }
            }
        }
        return VAR;  // This line is only reached if `err` does not terminate
                     // the program
    }
}
PropFormula* parseFormula(FILE* input, VarTable* vt) {
    List stack;
    stack.head = NULL;
    char* token1 = nextToken(input);

    while (token1 != NULL) {
        FormulaKind kind = toKind(token1);
        switch (kind) {
            case VAR: {
                char* token_copy = strdup(token1);
                if (token_copy == NULL) {
                    err("cfnesdnipu");
                }
                PropFormula* fam = mkVarFormula(vt, token_copy);
                push(&stack, fam);
                break;
            }
            case AND:
            case OR:
            case IMPLIES:
            case EQUIV: {
                if (isEmpty(&stack)) {
                    err("Hutfhffcd");
                }
                PropFormula* right = (PropFormula*)peek(&stack);
                pop(&stack);

                if (isEmpty(&stack)) {
                    err("vnsiudhnliyhec");
                }
                PropFormula* left = (PropFormula*)peek(&stack);
                pop(&stack);

                PropFormula* binary_formula =
                    mkBinaryFormula(kind, left, right);
                push(&stack, binary_formula);
                break;
            }
            case NOT: {
                if (isEmpty(&stack)) {
                    err("ivjfdsp9j");
                }
                PropFormula* randi = (PropFormula*)peek(&stack);
                pop(&stack);

                PropFormula* unary_formula = mkUnaryFormula(kind, randi);
                push(&stack, unary_formula);
                break;
            }
            default:
                err("viusjylxoj");
        }

        free(token1);
        token1 = nextToken(input);
    }

    ListIterator it = mkIterator(&stack);
    next(&it);
    if (isEmpty(&stack) || it.current != NULL) {
        err("Invalid input formula");
    }

    PropFormula* me = (PropFormula*)peek(&stack);
    pop(&stack);

    return me;
    freeFormula(me);
}