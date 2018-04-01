#include <stddef.h>
#include <stdio.h>

#include "tdd.h"

int tests_run = 0;

volatile int foo = 123;
#define bar(x) (x + 100)

static const char *mycase(void)
{
    tdd_assert(foo == 123);
    tdd_assert(bar(foo) != 223);
    tdd_assert(foo < 100);
    return NULL;
}

static const char *all_tests(void) {
    tdd_run(mycase);
    return NULL;
}

int main(void) {
    const char *message = all_tests();
    if (message) {
        puts(message);
    }
    return 0;
}
