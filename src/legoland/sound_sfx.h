#pragma once

struct SampleDef;
struct SampleBuffer;
struct SampleSink;

/* Canonical playable-sample object. sound_sfx.c manipulates it through the
 * "Sample" field view (next/fade/flags/buffer); sound_music.c through the
 * "PlayableSample" field view (field_c..field_18/flags_1c/sink). Same heap
 * object, same offsets — the two pointer/flags views are unions so each TU
 * still compiles to its original memory-access width with no casts. */
struct Sample {
    struct Sample *next;
    unsigned char pad_4[0x8 - 0x4];
    unsigned int fade;
    unsigned int field_c;
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
    union {
        unsigned short flags;
        unsigned char flags_1c;
    };
    unsigned char pad_1e[0x28 - 0x1e];
    unsigned int active;
    union {
        struct SampleBuffer *buffer;
        struct SampleSink *sink;
    };
};

struct Sample *CreatePlayableSample(unsigned int def);
int PlaySample(struct Sample *sample, unsigned int looping, unsigned int oneshot);
void PauseSingleSample(struct Sample *sample);
void FUN_00492850(void);
int ResumeSinglyPausedSample(struct Sample *sample);
void FUN_00492980(void);
void FUN_00492990(void);
int SetSampleFade(struct Sample *sample, unsigned int fade);
void KillPlayableSample(struct Sample *sample);
void DeleteSampleDef(struct SampleDef *def);
int KillSoundSampleSystem(void);
int FUN_00495b00(void);
