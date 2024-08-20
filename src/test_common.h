#pragma once

#include <stdio.h>
#include <string.h>

// possible test outcomes
typedef enum {
    SUCCESS,
    FAILURE,
} result_t;

// type of a unit test function
typedef result_t (*test_fun_t)(const char *);

// function that searches for a test of the given name
test_fun_t get_test(const char *name);

// macro for defining a new unit test, to be used in the get_test function
// takes a string test name and the identifier of a unit test function
#define TEST(name, func)                                \
    {                                                   \
        const char *str = strchr(test, ':');            \
        if (str != NULL) {                              \
            if (strncmp(name, test, str - test) == 0) { \
                return func;                            \
            }                                           \
        } else if (strcmp(name, test) == 0) {           \
            return func;                                \
        }                                               \
    }
