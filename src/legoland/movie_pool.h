#pragma once

#include "legoland.h"

struct MoviePoolElem;

int ReturnOne_00477400(void);
void FreeMoviePool(void);
struct MoviePoolElem *AllocMoviePoolElem(void);
void *MemAlloc(unsigned int size, int flags, unsigned int tag, int line);
void MemFree(void *param);
void Empty_004775f0(void);
int GetMoviePoolStats(unsigned int *total_out, unsigned int *count_out);
