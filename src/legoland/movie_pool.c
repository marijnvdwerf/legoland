/* Compiled /Od in the original (frame pointers, 0xCC padding); see CMakeLists.txt. */
#include "movie_pool.h"
#include <stdlib.h>
#include <string.h>
#include "globals.h"

struct MoviePoolElem {
    /* 0x00 */ struct MoviePoolElem *next;
    unsigned char pad_4[0x8 - 0x4];
    /* 0x08 */ unsigned int field_8;
    unsigned char pad_c[0x8c - 0xc];
};

struct MoviePool {
    /* 0x00 */ unsigned int count;
    /* 0x04 */ struct MoviePoolElem *array;
    /* 0x08 */ struct MoviePoolElem *free_elem;
    unsigned char pad_c[0xc - 0xc];
};

// FUNCTION: LEGOLAND 0x00477400
int FUN_00477400(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x00477410
void FUN_00477410(void) {
    if (DAT_00668fb8 != NULL) {
        free(DAT_00668fb8);
        DAT_00668fb8 = NULL;
    }
}

// FUNCTION: LEGOLAND 0x00477440
struct MoviePoolElem *FUN_00477440(void) {
    int var6;
    struct MoviePoolElem *var5;
    struct MoviePool *var4;
    struct MoviePoolElem *var3;
    struct MoviePoolElem *var2;
    int var1;

    if (DAT_00668fb8 == NULL) {
        return NULL;
    }
    if (DAT_00668fb8->array == NULL) {
        var4 = (struct MoviePool *)realloc(DAT_00668fb8, (DAT_00668fb8->count + 0x400) * 0x8c + 0xc);
        if (var4 == NULL) {
            return NULL;
        }
        var6 = (int)var4 - (int)DAT_00668fb8;
        var4->free_elem = (struct MoviePoolElem *)((int)var4->free_elem + var6);
        var5 = var4->free_elem;
        while (var5->next != NULL) {
            var5->next = (struct MoviePoolElem *)((int)var5->next + var6);
            var5 = var5->next;
        }
        DAT_00668fb8 = var4;
        var2 = (struct MoviePoolElem *)((char *)DAT_00668fb8 + DAT_00668fb8->count * 0x8c + 0xc);
        memset(var2, 0, 0x400 * 0x8c);
        DAT_00668fb8->array = var2;
        for (var1 = 0; var1 < 0x3ff; var1++) {
            DAT_00668fb8->array[var1].next = &DAT_00668fb8->array[var1 + 1];
        }
    }
    var3 = DAT_00668fb8->free_elem;
    DAT_00668fb8->free_elem = DAT_00668fb8->array;
    DAT_00668fb8->array = DAT_00668fb8->array->next;
    DAT_00668fb8->free_elem->next = var3;
    return DAT_00668fb8->free_elem;
}

// FUNCTION: LEGOLAND 0x004775b0
void *FUN_004775b0(unsigned int size) {
    return calloc(1, size);
}

// FUNCTION: LEGOLAND 0x004775d0
void FUN_004775d0(void *param) {
    free(param);
}

// FUNCTION: LEGOLAND 0x004775f0
void FUN_004775f0(void) {
}

// FUNCTION: LEGOLAND 0x00477600
int FUN_00477600(unsigned int *total_out, unsigned int *count_out) {
    unsigned int var3;
    unsigned int var2;
    struct MoviePoolElem *var1;

    var3 = 0;
    var2 = 0;
    if (total_out == NULL || count_out == NULL) {
        return 0;
    }
    if (DAT_00668fb8 != NULL) {
        var1 = DAT_00668fb8->free_elem;
        while (var1 != NULL) {
            var3 = var3 + var1->field_8;
            var2 = var2 + 1;
            var1 = var1->next;
        }
    }
    *total_out = var3;
    *count_out = var2;
    return 1;
}
