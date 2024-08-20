#include "err.h"

#include <stdio.h>
#include <stdlib.h>

void err(const char* msg) {
    fprintf(stderr, "Error: %s!\n", msg);
    exit(30);
}
