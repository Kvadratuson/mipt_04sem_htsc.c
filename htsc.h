#ifndef HTSC_C_HTSC_H
#define HTSC_C_HTSC_H

/* HTSC / htsc --- Hash Table with Separate Chaining */

#include <stddef.h>

typedef enum htsc_errors htsc_errors_t;
enum htsc_errors {
    HTSC_SUCCESS = 0,
    HTSC_FAILURE,
    HTSC_IS_NULL,
    HTSC_OUT_OF_MEMORY,
};

typedef struct htsc_element htsc_element_t;
struct htsc_element
{};

typedef struct htsc htsc_t;
struct htsc
{
    size_t _size;
    htsc_element_t **_table;
    size_t _index; /* an index of the last free item */
};

htsc_t *htsc_construct(size_t size, htsc_errors_t *err);
void htsc_deconstruct(htsc_t *hash_table, htsc_errors_t *err);

#endif /* HTSC_C_HTSC_H */
