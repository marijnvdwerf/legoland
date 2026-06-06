#pragma once

struct Sample;

struct Sample *PlayInstanceOfSample(void *def, unsigned int looping,
                                    unsigned int oneshot, void *config);
void UnSourceAndFadeAllSamplesFromSource(void *source, int fade);
void Load_FXList(const unsigned char *list, unsigned int count);
void Kill_FXList(const unsigned char *list, int count);
