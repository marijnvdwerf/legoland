#pragma once

#include "legoland.h"

struct Sample;

struct SampleParams {
    /* 0x00 */ int field_0;
    /* 0x04 */ unsigned int pad_4;
    /* 0x08 */ int field_8;
    /* 0x0c */ int field_c;
};

LEGO_EXPORT struct Sample *PlayInstanceOfSample(void *def, unsigned int looping,
    unsigned int oneshot, void *config);
LEGO_EXPORT void UnSourceAndFadeAllSamplesFromSource(void *source, int fade);
LEGO_EXPORT void Load_FXList(const unsigned char *list, int count);
LEGO_EXPORT void Kill_FXList(const unsigned char *list, int count);
void FUN_004969d0(void);
