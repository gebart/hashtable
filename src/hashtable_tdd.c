#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "ht.h"

#include "tdd.h"

int tests_run = 0;

static const char *test_insert1(void)
{
    const ht_t ht = { .table = (ht_elem_t[64]){{0}}, .nelem = 64 };
    ht_clear(&ht);
    const ht_elem_t *elem = ht_insert(&ht, "hej", "blah");
    tdd_assert(elem != NULL);
    tdd_assert(elem >= ht.table);
    tdd_assert(elem < (ht.table + ht.nelem));

    return NULL;
}

static const char *test_insert2(void)
{
    const ht_t ht = { .table = (ht_elem_t[64]){{0}}, .nelem = 64 };
    ht_clear(&ht);
    const ht_elem_t *elem = ht_insert(&ht, "hej", "blah");
    tdd_assert(elem != NULL);
    tdd_assert(elem >= ht.table);
    tdd_assert(elem < (ht.table + ht.nelem));
    elem = ht_insert(&ht, "foo", "bar");
    tdd_assert(elem != NULL);
    tdd_assert(elem >= ht.table);
    tdd_assert(elem < (ht.table + ht.nelem));

    return NULL;
}

static void setup_lookup_tests(const ht_t *ht)
{
    ht_clear(ht);
    ht_insert(ht, "hej", "blah");
    ht_insert(ht, "foo", "bar");
    ht_insert(ht, "test", "lorem ipsum");
    ht_insert(ht, "Hejsan hoppsan", "blarg");
}

static const char *test_lookup(void)
{
    const ht_t ht = { .table = (ht_elem_t[64]){{0}}, .nelem = 64 };
    setup_lookup_tests(&ht);

    const char *value = ht_lookup(&ht, "hej");
    tdd_assert(value);
    tdd_assert(strcmp("hej", value) == 0);

    return NULL;
}

static const char *all_tests(void) {
    tdd_run(test_insert1);
    tdd_run(test_insert2);
    tdd_run(test_lookup);
    return NULL;
}

int main(void) {
    const char *message = all_tests();
    if (message) {
        printf("Failed assertion: ");
        puts(message);
        return 1;
    }
    return 0;
}
