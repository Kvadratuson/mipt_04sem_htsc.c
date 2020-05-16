#include "htsc.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

htsc_t *htsc_construct(size_t size, htsc_errors_t *err)
{
    bool err_is_null;
    htsc_t *hash_table;
    err_is_null = (err == NULL);
    hash_table = malloc(sizeof(*hash_table));
    if (hash_table == NULL) {
        if (!err_is_null)
            *err = HTSC_OUT_OF_MEMORY;
        return NULL;
    }
    hash_table->_size = size;
    hash_table->_table = calloc(hash_table->_size + 1, sizeof(*hash_table->_table));
    if (hash_table->_table == NULL) {
        if (!err_is_null)
            *err = HTSC_OUT_OF_MEMORY;
        free(hash_table);
        return NULL;
    }
    hash_table->_index = hash_table->_size;
    if (!err_is_null)
        *err = HTSC_SUCCESS;
    return hash_table;
}

void htsc_deconstruct(htsc_t *hash_table, htsc_errors_t *err)
{
    bool err_is_null;
    err_is_null = (err == NULL);
    if (hash_table == NULL) {
        if (!err_is_null)
            *err = HTSC_IS_NULL;
        return;
    }
    for (size_t i = 0; i <= hash_table->_size; ++i) {
        if (hash_table->_table[i] != NULL) {
            free(hash_table->_table[i]->_key._data);
            free(hash_table->_table[i]);
        }
    }
    free(hash_table->_table);
    free(hash_table);
    if (!err_is_null)
        *err = HTSC_SUCCESS;
    return;
}

static const size_t htsc_fnv_prime = 16777619u;
static const size_t htsc_fnv_offset_basis = 2166136261u;

size_t htsc_fnv_1a(const char *data, size_t length)
{
    size_t hash;
    hash = htsc_fnv_offset_basis;
    for (size_t i = 0; i < length; ++i) {
        hash ^= data[i];
        hash *= htsc_fnv_prime;
    }
    return hash;
}

void htsc_set(htsc_t *hash_table, size_t i, const char *data, size_t length, htsc_errors_t *err)
{
    bool err_is_null;
    err_is_null = (err == NULL);
    hash_table->_table[i] = malloc(sizeof(*hash_table->_table[i]));
    if (hash_table->_table[i] == NULL) {
        if (!err_is_null)
            *err = HTSC_OUT_OF_MEMORY;
        return;
    }
    hash_table->_table[i]->_link = 0;
    hash_table->_table[i]->_key._length = length;
    hash_table->_table[i]->_key._data = malloc(hash_table->_table[i]->_key._length
                                                * sizeof(*hash_table->_table[i]->_key._data));
    memcpy(hash_table->_table[i]->_key._data, data, length);
    if (!err_is_null)
        *err = HTSC_SUCCESS;
    return;
}

bool htsc_compare(htsc_t *hash_table, size_t i, const char *data, size_t length)
{
    size_t j;
    if (length == hash_table->_table[i]->_key._length) {
        for (j = 0; j < length; ++j) {
            if (data[j] != hash_table->_table[i]->_key._data[j])
                break;
        }
        if (j == length)
            return true;
    }
    return false;
}

void htsc_insert(htsc_t *hash_table, const char *data, size_t length, htsc_errors_t *err)
{
    bool err_is_null;
    size_t i;
    err_is_null = (err == NULL);
    if ((hash_table == NULL) || (data == NULL)) {
        if (!err_is_null)
            *err = HTSC_IS_NULL;
        return;
    }
    i = htsc_fnv_1a(data, length) % hash_table->_size + 1;
    if (hash_table->_table[i] == NULL) {
        htsc_set(hash_table, i, data, length, err);
        return;
    } else if (htsc_compare(hash_table, i, data, length)) {
        if (!err_is_null)
            *err = HTSC_IS_PRESENT;
        return;
    }
    while (hash_table->_table[i]->_link != 0) {
        i = hash_table->_table[i]->_link;
        if (htsc_compare(hash_table, i, data, length)) {
            if (!err_is_null)
                *err = HTSC_IS_PRESENT;
            return;
        }
    }
    while(hash_table->_table[hash_table->_index] != NULL)
        --hash_table->_index;
    if (hash_table->_index == 0) {
        if (!err_is_null)
            *err = HTSC_IS_FULL;
        return;
    }
    hash_table->_table[i]->_link = hash_table->_index;
    htsc_set(hash_table, hash_table->_index, data, length, err);
    return;
}
