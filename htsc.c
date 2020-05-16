#include "htsc.h"

#include <stdbool.h>
#include <stdlib.h>

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
        if (hash_table->_table[i] != NULL)
            free(hash_table->_table[i]);
    }
    free(hash_table->_table);
    free(hash_table);
    if (!err_is_null)
        *err = HTSC_SUCCESS;
    return;
}
