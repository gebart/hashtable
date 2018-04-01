/*
 * Copyright (C) 2018 Joakim Nohlgård
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>

typedef struct {
    const char *key;
    void *value;
} ht_elem_t;

typedef struct {
    ht_elem_t *table;   /**< pointer to buffer */
    size_t nelem;       /**< number of buckets */
} ht_t;

/**
 * @brief   Initialize a hash table
 *
 * @p buf will be used as a buffer for the actual table data, be careful
 * when using stack allocated buffers.
 *
 * @param[in,out]   ht      hash table object to operate on
 * @param[in]       buf     storage buffer for table
 * @param[in]       nelem   number of elements available in the buffer
 */
void ht_init(ht_t *ht, ht_elem_t *buf, size_t nelem);

/**
 * @brief   Clear the table contents of a hash table
 *
 * @param[in]      ht      hash table object to operate on
 */
void ht_clear(const ht_t *ht);

/**
 * @brief   Insert into hash table
 *
 * @param[in]       ht      hash table object to operate on
 * @param[in]       key     key to hash
 * @param[in]       value   value to store at key
 *
 * @return  pointer to table entry where the item was inserted
 * @return  NULL if the table was full
 */
const ht_elem_t *ht_insert(const ht_t *ht, const char *key, void *value);

/**
 * @brief   Lookup key in hash table, without removing
 *
 * @param[in]       ht      hash table object to operate on
 * @param[in]       key     key to look up
 *
 * @return  pointer to value if @p key was found
 * @return  @c NULL otherwise
 */
void *ht_lookup(const ht_t *ht, const char *key);

/**
 * @brief   Lookup key in a hash table, remove and return value
 *
 * @param[in]       ht      hash table object to operate on
 * @param[in]       key     key to look for
 *
 * @return  pointer to value if @p key was found
 * @return  @c NULL otherwise
 */
void *ht_pop(const ht_t *ht, const char *key);

/**
 * @brief   Dump all contents of hash table to stdout
 *
 * @param[in]       ht      hash table object to operate on
 */
void ht_dump(const ht_t *ht);

#endif /* HASHTABLE_H */
