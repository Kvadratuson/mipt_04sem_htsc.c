#include "htsc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

htsc_t *htsc_construct(size_t size, htsc_exit_code_t *exit_code)
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
    hash_table->m_size = size;
    hash_table->pp_table = calloc(hash_table->m_size + 1, sizeof(*hash_table->pp_table));
    if (hash_table->pp_table == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_OUT_OF_MEMORY;
        free(hash_table);
        return NULL;
    }
    hash_table->i_index = hash_table->m_size;
    for (size_t i = 0; i <= hash_table->m_size; ++i) {
        hash_table->pp_table[i] = calloc(1, sizeof(*hash_table->pp_table[i]));
        if (hash_table->pp_table[i] == NULL) {
            if (!exit_code_is_null)
                *exit_code = HTSC_OUT_OF_MEMORY;
            for (size_t j = i - 1; j >= 0; --j)
                free(hash_table->pp_table[j]);
            free(hash_table);
            return NULL;
        }
    }
    if (!exit_code_is_null)
        *exit_code = HTSC_SUCCESS;
    return hash_table;
}

void htsc_deconstruct(htsc_t *hash_table, htsc_exit_code_t *exit_code)
{
    bool exit_code_is_null;
    exit_code_is_null = (exit_code == NULL);
    if (hash_table == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_NULL;
        return;
    }
    for (size_t i = 0; i <= hash_table->m_size; ++i) {
        free(hash_table->pp_table[i]->m_key.p_data);
        free(hash_table->pp_table[i]);
    }
    free(hash_table->pp_table);
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

void htsc_set(htsc_t *hash_table, size_t i, const char *data, size_t length, htsc_exit_code_t *exit_code)
{
    bool exit_code_is_null;
    exit_code_is_null = (exit_code == NULL);
    hash_table->pp_table[i]->m_key.m_length = length;
    hash_table->pp_table[i]->m_key.p_data = malloc(hash_table->pp_table[i]->m_key.m_length
                                                   * sizeof(*hash_table->pp_table[i]->m_key.p_data));
    if (hash_table->pp_table[i]->m_key.p_data == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_OUT_OF_MEMORY;
        return;
    }
    memcpy(hash_table->pp_table[i]->m_key.p_data, data, length);
    if (!exit_code_is_null)
        *exit_code = HTSC_SUCCESS;
    return;
}

bool htsc_compare(htsc_t *hash_table, size_t i, const char *data, size_t length)
{
    size_t j;
    if (length == hash_table->pp_table[i]->m_key.m_length) {
        for (j = 0; j < length; ++j) {
            if (data[j] != hash_table->pp_table[i]->m_key.p_data[j])
                break;
        }
        if (j == length)
            return true;
    }
    return false;
}

void htsc_insert(htsc_t *hash_table, const char *data, size_t length, htsc_exit_code_t *exit_code)
{
    bool exit_code_is_null;
    size_t i;
    exit_code_is_null = (exit_code == NULL);
    if ((hash_table == NULL) || (data == NULL)) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_NULL;
        return;
    }
    i = htsc_fnv_1a(data, length) % hash_table->m_size + 1;
    if (hash_table->pp_table[i]->m_key.p_data == NULL) {
        htsc_set(hash_table, i, data, length, exit_code);
        return;
    } else if (htsc_compare(hash_table, i, data, length)) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_PRESENT;
        return;
    }
    while (hash_table->pp_table[i]->i_link != 0) {
        i = hash_table->pp_table[i]->i_link;
        if (htsc_compare(hash_table, i, data, length)) {
            if (!exit_code_is_null)
                *exit_code = HTSC_IS_PRESENT;
            return;
        }
    }
    while(hash_table->pp_table[hash_table->i_index]->m_key.p_data != NULL)
        --hash_table->i_index;
    if (hash_table->i_index == 0) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_FULL;
        return;
    }
    hash_table->pp_table[i]->i_link = hash_table->i_index;
    htsc_set(hash_table, hash_table->i_index, data, length, exit_code);
    return;
}

void htsc_unset(htsc_t *hash_table, size_t i)
{
    free(hash_table->pp_table[i]->m_key.p_data);
    hash_table->pp_table[i]->m_key.p_data = NULL;
    return;
}

void htsc_erase(htsc_t *hash_table, const char *data, size_t length, htsc_exit_code_t *exit_code)
{
    bool exit_code_is_null;
    size_t i;
    exit_code_is_null = (exit_code == NULL);
    if ((hash_table == NULL) || (data == NULL)) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_NULL;
        return;
    }
    i = htsc_fnv_1a(data, length) % hash_table->m_size + 1;
    if (hash_table->pp_table[i]->m_key.p_data == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_NOT_FOUND;
        return;
    }
    while (i != 0) {
        if (htsc_compare(hash_table, i, data, length)) {
            htsc_unset(hash_table, i);
            if (hash_table->i_index < i)
                hash_table->i_index = i;
            if (!exit_code_is_null)
                *exit_code = HTSC_SUCCESS;
            return;
        }
        i = hash_table->pp_table[i]->i_link;
    }
    if (!exit_code_is_null)
        *exit_code = HTSC_NOT_FOUND;
    return;
}

bool htsc_find(htsc_t *hash_table, const char *data, size_t length, htsc_exit_code_t *exit_code)
{
    bool exit_code_is_null;
    size_t i;
    exit_code_is_null = (exit_code == NULL);
    if ((hash_table == NULL) || (data == NULL)) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_NULL;
        return false;
    }
    i = htsc_fnv_1a(data, length) % hash_table->m_size + 1;
    if (hash_table->pp_table[i]->m_key.p_data == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_SUCCESS;
        return false;
    }
    while (i != 0) {
        if (htsc_compare(hash_table, i, data, length)) {
            if (!exit_code_is_null)
                *exit_code = HTSC_SUCCESS;
            return true;
        }
        i = hash_table->pp_table[i]->i_link;
    }
    if (!exit_code_is_null)
        *exit_code = HTSC_SUCCESS;
    return false;
}

void htsc_print(htsc_t *hash_table, htsc_exit_code_t *exit_code)
{
    bool exit_code_is_null;
    exit_code_is_null = (exit_code == NULL);
    if (hash_table == NULL) {
        if (!exit_code_is_null)
            *exit_code = HTSC_IS_NULL;
        return;
    }
    printf("Size: %lu Index: %lu\n", hash_table->m_size, hash_table->i_index);
    for (size_t i = 0; i <= hash_table->m_size; ++i) {
        printf("%lu: Link: %lu Length: %lu", i, hash_table->pp_table[i]->i_link, hash_table->pp_table[i]->m_key.m_length);
        printf("\n\t");
        if (hash_table->pp_table[i]->m_key.p_data != NULL)
            for (size_t j = 0; j < hash_table->pp_table[i]->m_key.m_length; ++j)
                printf("%c", hash_table->pp_table[i]->m_key.p_data[j]);
        printf("\n");
    }
    if (!exit_code_is_null)
        *exit_code = HTSC_SUCCESS;
    return;
}
