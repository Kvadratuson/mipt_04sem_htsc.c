#ifndef HTSC_C_HTSC_H
#define HTSC_C_HTSC_H

/* HTSC / htsc --- Hash Table with Separate Chaining */
/* FNV / fnv --- 'Fowler/Noll/Vo' hash algorithm */

#include <stddef.h>

typedef enum htsc_exit_codes htsc_exit_codes_t;
enum htsc_exit_codes {
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
    char *_data;
    size_t _length;
};

typedef struct htsc_element htsc_element_t;
struct htsc_element
{
    htsc_key_t _key;
    size_t _link; /* a link to the next element */
};

typedef struct htsc htsc_t;
struct htsc
{
    size_t _size;
    htsc_element_t **_table;
    size_t _index; /* an index of the last free item */
};

htsc_t *htsc_construct(size_t size, htsc_exit_codes_t *exit_code);
void htsc_deconstruct(htsc_t *hash_table, htsc_exit_codes_t *exit_code);
void htsc_insert(htsc_t *hash_table, const char *data, size_t length, htsc_exit_codes_t *exit_code);
void htsc_delete(htsc_t *hash_table, const char *data, size_t length, htsc_exit_codes_t *exit_code);
void htsc_print(htsc_t *hash_table, htsc_exit_codes_t *exit_code);

#endif /* HTSC_C_HTSC_H */
