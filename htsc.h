#ifndef HTSC_C_HTSC_H
#define HTSC_C_HTSC_H

/* HTSC / htsc --- Hash Table with Separate Chaining */
/* FNV / fnv --- 'Fowler/Noll/Vo' hash algorithm */

#include <stdbool.h>
#include <stddef.h>

typedef enum htsc_exit_code htsc_exit_code_t;
enum htsc_exit_code
{
    HTSC_NOT_FOUND = -2,
    HTSC_IS_PRESENT,
    HTSC_SUCCESS = 0,
    HTSC_FAILURE,
    HTSC_IS_NULL,
    HTSC_OUT_OF_MEMORY,
    HTSC_IS_FULL,
};

typedef struct htsc_key htsc_key_t;
struct htsc_key
{
    char *p_data;
    size_t m_length;
};

typedef struct htsc_element htsc_element_t;
struct htsc_element
{
    htsc_key_t m_key;
    size_t i_link; /* a link to the next element */
};

typedef struct htsc htsc_t;
struct htsc
{
    size_t m_size;
    htsc_element_t **pp_table;
    size_t i_index; /* an index of the last free item */
};

htsc_t *htsc_construct(size_t size, htsc_exit_code_t *exit_code);
void htsc_deconstruct(htsc_t *hash_table, htsc_exit_code_t *exit_code);
void htsc_insert(htsc_t *hash_table, const char *data, size_t length, htsc_exit_code_t *exit_code);
void htsc_erase(htsc_t *hash_table, const char *data, size_t length, htsc_exit_code_t *exit_code);
bool htsc_find(htsc_t *hash_table, const char *data, size_t length, htsc_exit_code_t *exit_code);
void htsc_print(htsc_t *hash_table, htsc_exit_code_t *exit_code);

#endif /* HTSC_C_HTSC_H */
