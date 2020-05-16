#include <stdio.h>
#include <stdlib.h>

#include "htsc.h"

#define HTSC_SIZE 10

int main()
{
    htsc_errors_t err;
    htsc_t *hash_table;

    { /* Construct */
        hash_table = htsc_construct(HTSC_SIZE, &err);
        if (err != HTSC_SUCCESS) {
            fprintf(stderr, "Constrcut: FAILURE\n");
            exit(err);
        }
        printf("Construct: SUCCESS\n");
    }

    { /* Deconstruct.NULL */
        htsc_deconstruct(NULL, &err);
        if (err != HTSC_IS_NULL) {
            fprintf(stderr, "Deconstruct.NULL: FAILURE\n");
            exit(HTSC_FAILURE);
        }
        printf("Deconstruct.NULL: SUCCESS\n");
    }

    { /* Deconstruct */
        htsc_deconstruct(hash_table, &err);
        if (err != HTSC_SUCCESS) {
            fprintf(stderr, "Deconstruct: FAILURE\n");
            exit(err);
        }
        printf("Deconstruct: SUCCESS\n");
    }

    exit(HTSC_SUCCESS);
}
