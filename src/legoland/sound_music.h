#pragma once

#include "legoland.h"

struct Sample;

LEGO_EXPORT struct Sample *PlayInstanceOfSample(void *def, unsigned int looping,
                                    unsigned int oneshot, void *config);
LEGO_EXPORT void UnSourceAndFadeAllSamplesFromSource(void *source, int fade);
LEGO_EXPORT void Load_FXList(const unsigned char *list, unsigned int count);
LEGO_EXPORT void Kill_FXList(const unsigned char *list, int count);
