#pragma once

#include "legoland.h"

#include <windows.h>

struct SampleBuffer;

struct SampleDef {
    /* 0x00 */ unsigned char pad_0[0x10];
    /* 0x10 */ void *field_10;
    /* 0x14 */ unsigned char pad_14[0x2c - 0x14];
    /* 0x2c */ struct SampleBuffer *buffer;
    /* 0x30 */ void *block_30;
    /* 0x34 */ void *block_34;
};

/* Sound-buffer object behind a playable sample. sound_sfx.c drives playback
 * through method_0x20..0x48; sound_music.c reuses the same vtable slot at 0x3c
 * (Apply) to push the sample's source config. One object, one vtable. */
struct SampleBufferVtbl {
    unsigned char pad_0[0x18];
    int(__stdcall *method_0x18)(struct SampleBuffer *self, int *out);
    unsigned char pad_1c[0x20 - 0x1c];
    int(__stdcall *method_0x20)(struct SampleBuffer *self, struct Sample **out);
    int(__stdcall *method_0x24)(struct SampleBuffer *self, unsigned int *out);
    unsigned char pad_28[0x30 - 0x28];
    int(__stdcall *method_0x30)(struct SampleBuffer *self, int arg1, int arg2, int arg3);
    int(__stdcall *method_0x34)(struct SampleBuffer *self, int arg1);
    unsigned char pad_38[0x3c - 0x38];
    int(__stdcall *method_0x3c)(struct SampleBuffer *self, int arg1);
    int(__stdcall *method_0x40)(struct SampleBuffer *self, int arg1);
    int(__stdcall *method_0x44)(struct SampleBuffer *self, int arg1);
    int(__stdcall *method_0x48)(struct SampleBuffer *self);
};

struct SampleBuffer {
    struct SampleBufferVtbl *vtable;
};

/* Canonical playable-sample object. sound_sfx.c manipulates it through the
 * "Sample" field view (next/fade/flags/buffer); sound_music.c through the
 * "PlayableSample" field view (field_c..field_18/flags/buffer). Same heap
 * object, same offsets. */
struct Sample {
    struct Sample *next;
    unsigned char pad_4[0x8 - 0x4];
    unsigned int fade;
    unsigned int field_c;
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
    unsigned short flags;
    unsigned char pad_1e[0x28 - 0x1e];
    unsigned int active;
    struct SampleBuffer *buffer;
};

LEGO_EXPORT struct SampleDef *CreateSampleFromWAV(const char *path);
LEGO_EXPORT struct Sample *CreatePlayableSample(unsigned int def);
LEGO_EXPORT int PlaySample(struct Sample *sample, unsigned int looping, unsigned int oneshot);
LEGO_EXPORT void PauseSingleSample(struct Sample *sample);
void FUN_00492830(void);
void FUN_00492850(void);
LEGO_EXPORT void DeletePlayableSamples(unsigned int param_1);
LEGO_EXPORT void UpdateSoundVols(void);
LEGO_EXPORT int ResumeSinglyPausedSample(struct Sample *sample);
void FUN_00492980(void);
void FUN_00492990(void);
LEGO_EXPORT int SetSampleFade(struct Sample *sample, unsigned int fade);
LEGO_EXPORT void KillPlayableSample(struct Sample *sample);
LEGO_EXPORT void DeleteSampleDef(struct SampleDef *def);
LEGO_EXPORT int KillSoundSampleSystem(void);
void FUN_00492b20(struct Sample *sample);
void FUN_00492ce0(int param_1);
BOOL FUN_00492d80(void);
void FUN_00492da0(void);
int FUN_00495a10(void *hwnd);
int FUN_00495b00(void);
