#include <stdio.h>

#include "test_common.h"

int main(int argc, char *argv[]) {
    if ((argc > 1 && argv[1][0] == '-') || argc == 1) {
        fprintf(stderr,
                "Usage:\n"
                "\t%s <testname>\tto execute a specific test by name\n",
                argv[0]);
        return 3;
    }

    result_t (*test_fun)(const char *) = get_test(argv[1]);

    if (test_fun == NULL) {
        fprintf(stderr, "Test name \"%s\" not found!\n", argv[1]);
        return 3;
    }
    result_t result = test_fun(argv[1]);

    if (result == SUCCESS) {
        return 0;
    } else {
        return 2;
    }
}
