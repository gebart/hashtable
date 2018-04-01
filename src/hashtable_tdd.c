#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "ht.h"

#include "tdd.h"

int tests_run = 0;

#define HT_TEST_SIZE 32

static void setup_ht_small(const ht_t *ht)
{
    ht_clear(ht);
    ht_insert(ht, "hej", "blah");
    ht_insert(ht, "foo", "bar");
    ht_insert(ht, "test", "lorem ipsum");
    ht_insert(ht, "Hejsan hoppsan", "blarg");
}

static void setup_ht_full(const ht_t *ht)
{
    ht_clear(ht);
    ht_insert(ht, "k001", "value001");
    ht_insert(ht, "k002", "value002");
    ht_insert(ht, "k003", "value003");
    ht_insert(ht, "k004", "value004");
    ht_insert(ht, "k005", "value005");
    ht_insert(ht, "k006", "value006");
    ht_insert(ht, "k007", "value007");
    ht_insert(ht, "k008", "value008");
    ht_insert(ht, "k009", "value009");
    ht_insert(ht, "k010", "value010");
    ht_insert(ht, "k011", "value011");
    ht_insert(ht, "k012", "value012");
    ht_insert(ht, "k013", "value013");
    ht_insert(ht, "k014", "value014");
    ht_insert(ht, "k015", "value015");
    ht_insert(ht, "k016", "value016");
    ht_insert(ht, "k017", "value017");
    ht_insert(ht, "k018", "value018");
    ht_insert(ht, "k019", "value019");
    ht_insert(ht, "k020", "value020");
    ht_insert(ht, "k021", "value021");
    ht_insert(ht, "k022", "value022");
    ht_insert(ht, "k023", "value023");
    ht_insert(ht, "k024", "value024");
    ht_insert(ht, "k025", "value025");
    ht_insert(ht, "k026", "value026");
    ht_insert(ht, "k027", "value027");
    ht_insert(ht, "k028", "value028");
    ht_insert(ht, "k029", "value029");
    ht_insert(ht, "k030", "value030");
    ht_insert(ht, "k031", "value031");
    ht_insert(ht, "k032", "value032");
}

static const char *test_clear(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    ht_clear(&ht);
    static const ht_elem_t empty = { .key = NULL, .value = NULL };
    for (size_t k = 0; k < HT_TEST_SIZE; ++k) {
        tdd_assert(0 == memcmp(&ht.table[k], &empty, sizeof(empty)));
    }
    setup_ht_small(&ht);
    ht_clear(&ht);
    for (size_t k = 0; k < HT_TEST_SIZE; ++k) {
        tdd_assert(0 == memcmp(&ht.table[k], &empty, sizeof(empty)));
    }

    return NULL;
}

static const char *test_insert1(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    ht_clear(&ht);
    const ht_elem_t *elp = ht_insert(&ht, "hej", "blah");
    tdd_assert(elp != NULL);
    tdd_assert(elp >= ht.table);
    tdd_assert(elp < (ht.table + ht.nelem));

    return NULL;
}

static const char *test_insert2(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    ht_clear(&ht);
    const ht_elem_t *elp = ht_insert(&ht, "hej", "blah");
    tdd_assert(elp != NULL);
    tdd_assert(elp >= ht.table);
    tdd_assert(elp < (ht.table + ht.nelem));
    elp = ht_insert(&ht, "foo", "bar");
    tdd_assert(elp != NULL);
    tdd_assert(elp >= ht.table);
    tdd_assert(elp < (ht.table + ht.nelem));

    return NULL;
}

static const char *test_insert_full(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    setup_ht_full(&ht);
    const ht_elem_t *elp = ht_insert(&ht, "hej", "blah");
    tdd_assert(elp == NULL);

    return NULL;
}

static const char *test_insert_after_pop(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    setup_ht_full(&ht);
    void *value = ht_pop(&ht, "k014");
    tdd_assert(value);
    const ht_elem_t *elp = ht_insert(&ht, "hej", "jadu");
    tdd_assert(elp != NULL);
    value = ht_lookup(&ht, "hej");
    tdd_assert(value);
    tdd_assert(0 == strcmp(value, "jadu"));

    return NULL;
}

static const char *test_lookup_found(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    setup_ht_small(&ht);

    const char *value = ht_lookup(&ht, "hej");
    tdd_assert(value);
    tdd_assert(0 == strcmp("blah", value));

    value = ht_lookup(&ht, "test");
    tdd_assert(value);
    tdd_assert(0 == strcmp("lorem ipsum", value));

    return NULL;
}

static const char *test_lookup_not_found(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    setup_ht_small(&ht);

    const char *value = ht_lookup(&ht, "blag");
    tdd_assert(!value);
    value = ht_lookup(&ht, "");
    tdd_assert(!value);
    value = ht_lookup(&ht, "not found");
    tdd_assert(!value);
    value = ht_lookup(&ht, "bla");
    tdd_assert(!value);
    value = ht_lookup(&ht, "tess");
    tdd_assert(!value);

    return NULL;
}

static const char *test_pop1(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    setup_ht_small(&ht);

    const char *value = ht_pop(&ht, "foo");
    tdd_assert(value);
    tdd_assert(0 == strcmp("bar", value));

    return NULL;
}

static const char *test_pop2(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    setup_ht_small(&ht);

    const char *value = ht_pop(&ht, "foo");
    tdd_assert(value);
    tdd_assert(0 == strcmp("bar", value));

    value = ht_pop(&ht, "hej");
    tdd_assert(value);
    tdd_assert(0 == strcmp("blah", value));

    return NULL;
}

static const char *test_pop_twice(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    setup_ht_small(&ht);

    const char *value = ht_pop(&ht, "foo");
    tdd_assert(value);
    tdd_assert(0 == strcmp("bar", value));

    value = ht_pop(&ht, "foo");
    tdd_assert(!value);

    return NULL;
}

static const char *test_pop_empty(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    ht_clear(&ht);

    const char *value = ht_pop(&ht, "foo");
    tdd_assert(!value);
    value = ht_pop(&ht, "");
    tdd_assert(!value);

    return NULL;
}

static const char *test_pop_past_deleted(void)
{
    const ht_t ht = { .table = (ht_elem_t[HT_TEST_SIZE]){{0}}, .nelem = HT_TEST_SIZE };
    ht_clear(&ht);
    setup_ht_full(&ht);

    const char *value = ht_pop(&ht, "k002");
    tdd_assert(value);

    value = ht_pop(&ht, "k013");
    tdd_assert(value);

    value = ht_pop(&ht, "k014");
    tdd_assert(value);

    value = ht_pop(&ht, "k020");
    tdd_assert(value);

    value = ht_pop(&ht, "k032");
    tdd_assert(value);

    return NULL;
}

static const char *all_tests(void) {
    tdd_run(test_clear);
    tdd_run(test_insert1);
    tdd_run(test_insert2);
    tdd_run(test_insert_full);
    tdd_run(test_lookup_found);
    tdd_run(test_lookup_not_found);
    tdd_run(test_pop1);
    tdd_run(test_pop2);
    tdd_run(test_pop_twice);
    tdd_run(test_pop_empty);
    tdd_run(test_pop_past_deleted);
    tdd_run(test_insert_after_pop);
    return NULL;
}

int main(void) {
    const char *message = all_tests();
    printf("Tests run: %u\n", tests_run);
    if (message) {
        printf("Failed assertion: ");
        puts(message);
        return 1;
    }
    return 0;
}
