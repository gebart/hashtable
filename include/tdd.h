/*
 * Copyright (C) 2018 Joakim Nohlgård
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

#ifndef TDD_H
#define TDD_H

extern int tests_run;

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/* Based on MinUnit http://www.jera.com/techinfo/jtns/jtn002.html */
#define tdd_assert(test) \
    do { \
        if (!(test)) { \
            return (TOSTRING(__FILE__) ":" TOSTRING(__LINE__) ": " STRINGIFY(test)); \
        } \
    } while (0)
#define tdd_run(test_func) do { const char *message = test_func(); tests_run++; \
                                if (message) return message; } while (0)

#endif /* TDD_H */
