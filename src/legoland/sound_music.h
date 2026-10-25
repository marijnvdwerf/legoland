#pragma once

#include <windows.h>
#include <dsound.h>
#include "legoland.h"

struct Sample;
struct FXItem;
struct AVISoundBuffer;

struct SampleSource {
    /* 0x00 */ unsigned int type;
    /* 0x04 */ void *bloke;
    /* 0x08 */ unsigned int x;
    /* 0x0c */ unsigned int y;
};
typedef struct SampleSource SampleSource;

/* SampleSource for a map position (type 2). The bloke slot is an unnamed bit-field: initialisers skip it, so it is
 * left unwritten as in the original. */
struct TileSampleSource {
    /* 0x00 */ unsigned int type;
    /* 0x04 */ unsigned int : 32;
    /* 0x08 */ unsigned int x;
    /* 0x0c */ unsigned int y;
};

LEGO_EXPORT void KillAllSamplesFromSource(struct SampleSource *source);

LEGO_EXPORT struct Sample *PlayInstanceOfSample(void *def, unsigned int looping,
    unsigned int oneshot, void *config);
LEGO_EXPORT void UnSourceAndFadeAllSamplesFromSource(void *source, int fade);
LEGO_EXPORT void Load_FXList(struct FXItem *list, int count);
LEGO_EXPORT void Kill_FXList(struct FXItem *list, int count);
void UpdateSound(void);
void FadeOutSpeechAndEffects(int param_1, int param_2);
void SetSampleFlag20(struct Sample *sample);
LEGO_EXPORT void AddSFX_Callback(struct Sample *entry, unsigned int delay, unsigned int (*callback)(struct Sample *self));
LEGO_EXPORT int CountSamplesFromSource(struct SampleSource *source);
LEGO_EXPORT LPDIRECTSOUNDBUFFER KLIBAUDIO_CreateAVISoundBuffer(LPWAVEFORMATEX format, unsigned int bytes);
LEGO_EXPORT void KLIBAUDIO_PlayAVISoundBuffer(LPDIRECTSOUNDBUFFER buffer, unsigned int position);
LEGO_EXPORT void KLIBAUDIO_SetAVIVolume(struct AVISoundBuffer *buffer, int volume);
LEGO_EXPORT void KLIBAUDIO_StopAVISoundBuffer(struct AVISoundBuffer *buffer);
LEGO_EXPORT int KLIBAUDIO_DestroyAVISoundBuffer(struct AVISoundBuffer *buffer);
LEGO_EXPORT void *KLIBAUDIO_LockAVISoundBuffer(LPDIRECTSOUNDBUFFER buffer, unsigned int offset, unsigned int size);
LEGO_EXPORT void KLIBAUDIO_UnLockAVISoundBuffer(LPDIRECTSOUNDBUFFER buffer);
void RefreshSampleVolumes(void);
LEGO_EXPORT int InitSoundSystem(void);
LEGO_EXPORT int KillSoundSystem(void);
LEGO_EXPORT void SetMusicGrooveLevel(unsigned int level);
