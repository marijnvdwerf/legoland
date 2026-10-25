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

/* Does nothing but return 1. */
// FUNCTION: LEGOLAND 0x00477400
int ReturnOne_00477400(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x00477410
void FreeMoviePool(void) {
    if (ActiveMoviePool != NULL) {
        free(ActiveMoviePool);
        ActiveMoviePool = NULL;
    }
}

// FUNCTION: LEGOLAND 0x00477440
struct MoviePoolElem *AllocMoviePoolElem(void) {
    INT_PTR var6; /* how far realloc moved the pool */
    struct MoviePoolElem *var5;
    struct MoviePool *var4;
    struct MoviePoolElem *var3;
    struct MoviePoolElem *var2;
    int var1;

    if (ActiveMoviePool == NULL) {
        return NULL;
    }
    if (ActiveMoviePool->array == NULL) {
        var4 = (struct MoviePool *)realloc(ActiveMoviePool, (ActiveMoviePool->count + 0x400) * sizeof(struct MoviePoolElem) + sizeof(struct MoviePool));
        if (var4 == NULL) {
            return NULL;
        }
        var6 = (INT_PTR)var4 - (INT_PTR)ActiveMoviePool;
        var4->free_elem = (struct MoviePoolElem *)((INT_PTR)var4->free_elem + var6);
        var5 = var4->free_elem;
        while (var5->next != NULL) {
            var5->next = (struct MoviePoolElem *)((INT_PTR)var5->next + var6);
            var5 = var5->next;
        }
        ActiveMoviePool = var4;
        var2 = (struct MoviePoolElem *)((char *)ActiveMoviePool + ActiveMoviePool->count * sizeof(struct MoviePoolElem) + sizeof(struct MoviePool));
        memset(var2, 0, 0x400 * sizeof(struct MoviePoolElem));
        ActiveMoviePool->array = var2;
        for (var1 = 0; var1 < 0x3ff; var1++) {
            ActiveMoviePool->array[var1].next = &ActiveMoviePool->array[var1 + 1];
        }
    }
    var3 = ActiveMoviePool->free_elem;
    ActiveMoviePool->free_elem = ActiveMoviePool->array;
    ActiveMoviePool->array = ActiveMoviePool->array->next;
    ActiveMoviePool->free_elem->next = var3;
    return ActiveMoviePool->free_elem;
}

// FUNCTION: LEGOLAND 0x004775b0
void *MemAlloc(unsigned int size, int flags, unsigned int tag, int line) {
    return calloc(1, size);
}

// FUNCTION: LEGOLAND 0x004775d0
void MemFree(void *param) {
    free(param);
}

/* Empty in the original (does nothing). */
// FUNCTION: LEGOLAND 0x004775f0
void Empty_004775f0(void) {
}

// FUNCTION: LEGOLAND 0x00477600
int GetMoviePoolStats(unsigned int *total_out, unsigned int *count_out) {
    unsigned int var3;
    unsigned int var2;
    struct MoviePoolElem *var1;

    var3 = 0;
    var2 = 0;
    if (total_out == NULL || count_out == NULL) {
        return 0;
    }
    if (ActiveMoviePool != NULL) {
        var1 = ActiveMoviePool->free_elem;
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
