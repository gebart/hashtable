/*
 * hashtable.c
 *
 * Copyright 2018 Joakim Nohlgård <jgn@mazarin>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include "ht.h"

#define ENABLE_DEBUG 1
#if ENABLE_DEBUG
#define DEBUG(...) printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

/* Sentinel element which is used for signaling deleted items */
/* Does not matter what contents it has, only the address */
static const char *deleted_key = (const char[]){'\0'};

/**
 * @brief   Hash function
 *
 * @param[in]       data    Data array to hash
 * @param[in]       nbytes  length of @p data, in bytes
 *
 * @return Hash of @p data
 */
static uint32_t _hash(const void *data, size_t nbytes);

/**
 * @brief   Generate hash of key
 *
 * The hash of the key will be limited to fit inside the table of @p ht.
 *
 * @param[in]       ht      hash table object to operate on
 * @param[in]       key     key to hash
 *
 * @return  Hash table offset of @p key
 */
static size_t _ht_hash(const ht_t *ht, const char *key);

/**
 * @brief   Find the location of an element in a hash table
 *
 * This will return the element where @p key would be placed if it is
 * inserted, or a pointer to the matching element if it already exists.
 *
 * @param[in]       ht      hash table object to operate on
 * @param[in]       key     key to look for
 *
 * @return  Pointer to the element in @c ht->table where @p key should be located
 * @return  NULL if the table is full
 */
static ht_elem_t *_ht_locate(const ht_t *ht, const char *key);

void ht_init(ht_t *ht, ht_elem_t *buf, size_t nelem)
{
    if (!ht) {
        DEBUG(__func__);
        DEBUG(": ht == NULL\n");
        return;
    }
    ht->table = buf;
    ht->nelem = nelem;
    ht_clear(ht);
}

void ht_clear(const ht_t *ht)
{
    if (!ht) {
        DEBUG(__func__);
        DEBUG(": ht == NULL\n");
        return;
    }
    memset(ht->table, 0, sizeof(ht->table[0]) * ht->nelem);
}

static uint32_t _hash(const void *data, size_t nbytes)
{
    uint32_t h = 0;
    const uint8_t *input = data;

    /* sdbm hash algorithm, see  */
    while (nbytes--) {
        uint8_t c = *(input++);
        h = c + (h << 6) + (h << 16) - h;
    }
    return h;
}

static size_t _ht_hash(const ht_t *ht, const char *key)
{
    if (!ht) {
        DEBUG(__func__);
        DEBUG(": ht == NULL\n");
        return 0;
    }
    uint32_t h = _hash(key, strlen(key));
    if (ht->nelem < (1 << 16)) {
        /* Mix in the upper bytes as well */
        h = (h ^ (h >> 16));
    }
    h = h % ht->nelem;
    DEBUG("k: '%s', h: %" PRIu32 "\n", key, h);
    return h;
}

static ht_elem_t *_ht_locate(const ht_t *ht, const char *key)
{
    size_t h = _ht_hash(ht, key);
    ht_elem_t *elp_free = NULL;
    for (size_t offset = 0; offset < ht->nelem; ++offset) {
        /* Open addressing with linear offset */
        ht_elem_t *elp = &ht->table[(h + offset) % ht->nelem];
        if (!elp->key) {
            /* Not found in table, but found the spot where it should exist */
            return elp;
        }
        if (elp->key == deleted_key) {
            /* skip past deleted keys */
            if (!elp_free) {
                /* Remember first hole */
                elp_free = elp;
            }
            continue;
        }
        if (0 == strcmp(elp->key, key)) {
            /* Found matching key */
            return elp;
        }
        DEBUG("collision: key '%s' wants to go where '%s' is\n", key, elp->key);
    }
    /* Either table was full, or we found a deleted key where this key
     * can be placed */
    return elp_free;
}

const ht_elem_t *ht_insert(const ht_t *ht, const char *key, void *value)
{
    if (!ht) {
        DEBUG(__func__);
        DEBUG(": ht == NULL\n");
        return NULL;
    }
    ht_elem_t *elp = _ht_locate(ht, key);
    if (!elp) {
        /* Not found, and table was full */
        return NULL;
    }
    elp->key = key;
    elp->value = value;
    return elp;
}

void *ht_lookup(const ht_t *ht, const char *key)
{
    if (!ht) {
        DEBUG(__func__);
        DEBUG(": ht == NULL\n");
        return NULL;
    }
    ht_elem_t *elp = _ht_locate(ht, key);
    if (!elp) {
        /* Not found, and table was full */
        return NULL;
    }
    if (!elp->key) {
        /* Not found */
        return NULL;
    }
    /* Found */
    return elp->value;
}

void *ht_pop(const ht_t *ht, const char *key)
{
    if (!ht) {
        DEBUG(__func__);
        DEBUG(": ht == NULL\n");
        return NULL;
    }
    ht_elem_t *elp = _ht_locate(ht, key);
    if (!elp) {
        /* Not found, and table was full */
        return NULL;
    }
    if (!elp->key) {
        /* Not found */
        return NULL;
    }
    /* Found, mark key as deleted */
    elp->key = deleted_key;
    /* The element pointed to by elp may be modified during rehashing */
    void *value = elp->value;
    return value;
}

void ht_dump(const ht_t *ht)
{
    if (!ht) {
        DEBUG(__func__);
        DEBUG(": ht == NULL\n");
        return;
    }
    printf("ht %p:\n", (void *)ht);
    for (size_t k = 0; k < ht->nelem; ++k) {
        printf("%08zx: ", k);
        if (ht->table[k].key) {
            printf("'%s' -> %p\n", ht->table[k].key, ht->table[k].value);
        }
        else {
            printf(" *\n");
        }
    }
}
