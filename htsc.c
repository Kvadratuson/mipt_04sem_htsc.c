#include "htsc.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

htsc_t *htsc_construct(size_t size, htsc_exit_codes_t *exit_code)
{
    bool exit_code_is_null;
    htsc_t *hash_table;
    exit_code_is_null = (exit_code == NULL);
    hash_table = malloc(sizeof(*hash_table));
    if (hash_table == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_OUT_OF_MEMORY;
        return NULL;
    }
    hash_table->_size = size;
    hash_table->_table = calloc(hash_table->_size + 1, sizeof(*hash_table->_table));
    if (hash_table->_table == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_OUT_OF_MEMORY;
        free(hash_table);
        return NULL;
    }
    hash_table->_index = hash_table->_size;
    if (!exit_code_is_null)
        *exit_code = HTSC_SUCCESS;
    return hash_table;
}

void htsc_deconstruct(htsc_t *hash_table, htsc_exit_codes_t *exit_code)
{
    bool exit_code_is_null;
    exit_code_is_null = (exit_code == NULL);
    if (hash_table == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_NULL;
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
    if (!exit_code_is_null)
        *exit_code = HTSC_SUCCESS;
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

void htsc_set(htsc_t *hash_table, size_t i, const char *data, size_t length, htsc_exit_codes_t *exit_code)
{
    bool exit_code_is_null;
    exit_code_is_null = (exit_code == NULL);
    hash_table->_table[i] = malloc(sizeof(*hash_table->_table[i]));
    if (hash_table->_table[i] == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_OUT_OF_MEMORY;
        return;
    }
    hash_table->_table[i]->_link = 0;
    hash_table->_table[i]->_key._length = length;
    hash_table->_table[i]->_key._data = malloc(hash_table->_table[i]->_key._length
                                                * sizeof(*hash_table->_table[i]->_key._data));
    memcpy(hash_table->_table[i]->_key._data, data, length);
    if (!exit_code_is_null)
        *exit_code = HTSC_SUCCESS;
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

void htsc_insert(htsc_t *hash_table, const char *data, size_t length, htsc_exit_codes_t *exit_code)
{
    bool exit_code_is_null;
    size_t i;
    exit_code_is_null = (exit_code == NULL);
    if ((hash_table == NULL) || (data == NULL)) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_NULL;
        return;
    }
    i = htsc_fnv_1a(data, length) % hash_table->_size + 1;
    if (hash_table->_table[i] == NULL) {
        htsc_set(hash_table, i, data, length, exit_code);
        return;
    } else if (htsc_compare(hash_table, i, data, length)) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_PRESENT;
        return;
    }
    while (hash_table->_table[i]->_link != 0) {
        i = hash_table->_table[i]->_link;
        if (htsc_compare(hash_table, i, data, length)) {
            if (!exit_code_is_null)
                *exit_code = HTSC_IS_PRESENT;
            return;
        }
    }
    while(hash_table->_table[hash_table->_index] != NULL)
        --hash_table->_index;
    if (hash_table->_index == 0) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_FULL;
        return;
    }
    hash_table->_table[i]->_link = hash_table->_index;
    htsc_set(hash_table, hash_table->_index, data, length, exit_code);
    return;
}

void htsc_unset(htsc_t *hash_table, size_t i)
{
    free(hash_table->_table[i]->_key._data);
    free(hash_table->_table[i]);
    hash_table->_table[i] = NULL;
    return;
}

void htsc_delete(htsc_t *hash_table, const char *data, size_t length, htsc_exit_codes_t *exit_code)
{
    bool exit_code_is_null;
    size_t i;
    exit_code_is_null = (exit_code == NULL);
    if ((hash_table == NULL) || (data == NULL)) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_NULL;
        return;
    }
    i = htsc_fnv_1a(data, length) % hash_table->_size + 1;
    if (hash_table->_table[i] == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_NOT_FOUND;
        return;
    } else if (htsc_compare(hash_table, i, data, length)) {
        htsc_unset(hash_table, i);
        if (hash_table->_index < i)
            hash_table->_index = i;
        if (!exit_code_is_null)
            *exit_code = HTSC_SUCCESS;
        return;
    }
    while (hash_table->_table[i]->_link != 0) {
        if (htsc_compare(hash_table, hash_table->_table[i]->_link, data, length)) {
            size_t j;
            j = hash_table->_table[i]->_link;
            hash_table->_table[i]->_link = hash_table->_table[j]->_link;
            htsc_unset(hash_table, j);
            if (hash_table->_index < j)
                hash_table->_index = j;
            if (!exit_code_is_null)
                *exit_code = HTSC_SUCCESS;
            return;
        }
        i = hash_table->_table[i]->_link;
    }
    if (!exit_code_is_null)
        *exit_code = HTSC_NOT_FOUND;
    return;
}

void htsc_print(htsc_t *hash_table, htsc_exit_codes_t *exit_code)
{
    bool exit_code_is_null;
    exit_code_is_null = (exit_code == NULL);
    if (hash_table == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_NULL;
        return;
    }
    printf("Size: %lu Index: %lu\n", hash_table->_size, hash_table->_index);
    for (size_t i = 0; i <= hash_table->_size; ++i) {
        if (hash_table->_table[i] != NULL) {
            printf("%lu: Link: %lu Length: %lu", i, hash_table->_table[i]->_link, hash_table->_table[i]->_key._length);
            printf("\n\t");
            for (size_t j = 0; j < hash_table->_table[i]->_key._length; ++j)
                printf("%c", hash_table->_table[i]->_key._data[j]);
            printf("\n");
        }
    }
    *exit_code = HTSC_SUCCESS;
    return;
}
