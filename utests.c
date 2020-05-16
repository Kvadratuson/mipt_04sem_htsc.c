#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htsc.h"

#define HTSC_SIZE 10
#define TEST_DATA1_SIZE 10
#define TEST_DATA2_SIZE 26
static const char * const test_data1[] =
{
    "mifihoiwikioguhoponjoavakecewbijasuminehjaruwadorhiftigirhozavab",
    "boavalidigsetuwoimufoowjinurtarjuijdipfegpegijoiczuhasezfupterob",
    "woliokenirkumhusiakmuhbozaocebpuihipijpoljaturozujpuezuvupipajni",
    "bifriunamofecmihlodrucpannuzegejugilujlelfoagiveogerelibihugsamu",
    "namoijziuwloutisewikkamdafijzetivzavuzerihicvasecetgizgiekohobra",
    "rarasapezuwezuparepsarnonezsucjotolapelzokolzusmimalelpurpewukfo",
    "famkivelhoekvamopedejzuwzevgufdahkaucaigivesralicbendoupevoucipi",
    "fazazerovbimzoravkinogmegujegelobujsuzbuotugemuopfivervoivbazumi",
    "zopsilamtaizfusijeluzantabofdiformewkapzanoavalowuvsolejunomihip",
    "ribjigijwudbupapuhorahzomcacozsuipukokzaokuedmaocopeicdoihfuumiu"
};
static const char * const test_data2[] =
{
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "u",
    "v",
    "w",
    "x",
    "y",
    "z"
};

int main()
{
    htsc_exit_codes_t exit_code;
    htsc_t *hash_table;

    { /* Construct */
        hash_table = htsc_construct(HTSC_SIZE, &exit_code);
        if (exit_code != HTSC_SUCCESS) {
            fprintf(stderr, "Constrcut: FAILURE\n");
            exit(exit_code);
        }
        printf("Construct: SUCCESS\n");
    }

    { /* Insert.NULL */
        htsc_insert(NULL, test_data1[0], strlen(test_data1[0]) + 1, &exit_code);
        if (exit_code != HTSC_IS_NULL) {
            fprintf(stderr, "Insert.NULL: FAILURE\n");
            exit(HTSC_FAILURE);
        }
        htsc_insert(hash_table, NULL, strlen(test_data1[0]) + 1, &exit_code);
        if (exit_code != HTSC_IS_NULL) {
            fprintf(stderr, "Insert.NULL: FAILURE\n");
            exit(HTSC_FAILURE);
        }
        printf("Insert.NULL: SUCCESS\n");
    }

    { /* Insert */
        for (size_t i = 0; i < TEST_DATA1_SIZE; ++i) {
            htsc_insert(hash_table, test_data1[i], strlen(test_data1[i]) + 1, &exit_code);
            if (exit_code != HTSC_SUCCESS) {
                fprintf(stderr, "Insert.%lu: FAILURE\n", i);
                exit(exit_code);
            }
        }
        printf("Insert: SUCCESS\n");
    }

    { /* Insert.PRESENT */
        for (size_t i = 0; i < TEST_DATA1_SIZE; ++i) {
            htsc_insert(hash_table, test_data1[i], strlen(test_data1[i]) + 1, &exit_code);
            if (exit_code != HTSC_IS_PRESENT) {
                fprintf(stderr, "Insert.PRESENT.%lu: FAILURE\n", i);
                exit(exit_code);
            }
        }
        printf("Insert.PRESENT: SUCCESS\n");
    }

    { /* Insert.FULL */
        for (size_t i = 0; i < TEST_DATA2_SIZE; ++i) {
            htsc_insert(hash_table, test_data2[i], strlen(test_data2[i]) + 1, &exit_code);
            if (exit_code != HTSC_IS_FULL) {
                fprintf(stderr, "Insert.FULL.%lu: FAILURE\n", i);
                exit(exit_code);
            }
        }
        printf("Insert.FULL: SUCCESS\n");
    }

    { /* Print.NULL */
        htsc_print(NULL, &exit_code);
        if (exit_code != HTSC_IS_NULL) {
            fprintf(stderr, "Print.NULL: FAILURE\n");
            exit(EXIT_FAILURE);
        }
        printf("Print.NULL: SUCCESS\n");
    }

    { /* Print */
        htsc_print(hash_table, &exit_code);
        if (exit_code != HTSC_SUCCESS) {
            fprintf(stderr, "Print: FAILURE\n");
            exit(exit_code);
        }
        printf("Print: SUCCESS\n");
    }

    { /* Deconstruct.NULL */
        htsc_deconstruct(NULL, &exit_code);
        if (exit_code != HTSC_IS_NULL) {
            fprintf(stderr, "Deconstruct.NULL: FAILURE\n");
            exit(HTSC_FAILURE);
        }
        printf("Deconstruct.NULL: SUCCESS\n");
    }

    { /* Deconstruct */
        htsc_deconstruct(hash_table, &exit_code);
        if (exit_code != HTSC_SUCCESS) {
            fprintf(stderr, "Deconstruct: FAILURE\n");
            exit(exit_code);
        }
        printf("Deconstruct: SUCCESS\n");
    }

    exit(HTSC_SUCCESS);
}
