#pragma once

#include <windows.h>
#include <dsound.h>
#include "legoland.h"

struct Sample;
struct CallbackEntry;
struct AVISoundBuffer;

struct SampleParams {
    /* 0x00 */ int field_0;
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ int field_8;
    /* 0x0c */ int field_c;
};

LEGO_EXPORT struct Sample *PlayInstanceOfSample(void *def, unsigned int looping,
    unsigned int oneshot, void *config);
LEGO_EXPORT void UnSourceAndFadeAllSamplesFromSource(void *source, int fade);
LEGO_EXPORT void Load_FXList(const unsigned char *list, int count);
LEGO_EXPORT void Kill_FXList(const unsigned char *list, int count);
void FUN_004969d0(void);
void FUN_00496e60(int param_1, int param_2);
void FUN_00496d10(struct Sample *sample);
LEGO_EXPORT void AddSFX_Callback(struct CallbackEntry *entry, unsigned int delay, unsigned int (*callback)(struct CallbackEntry *self));
LEGO_EXPORT void AdjustPSampleFreq(struct Sample *sample, unsigned int param_2);
LEGO_EXPORT int CountSamplesFromSource(struct SampleParams *source);
LEGO_EXPORT LPDIRECTSOUNDBUFFER KLIBAUDIO_CreateAVISoundBuffer(LPWAVEFORMATEX format, unsigned int bytes);
LEGO_EXPORT void KLIBAUDIO_PlayAVISoundBuffer(LPDIRECTSOUNDBUFFER buffer, unsigned int position);
LEGO_EXPORT void KLIBAUDIO_SetAVIVolume();
LEGO_EXPORT void KLIBAUDIO_StopAVISoundBuffer(struct AVISoundBuffer *buffer);
LEGO_EXPORT int KLIBAUDIO_DestroyAVISoundBuffer(struct AVISoundBuffer *buffer);
