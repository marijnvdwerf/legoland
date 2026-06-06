#include <windows.h>
#include "legoland.h"

#include "timer.h"

struct PlayableSample;
struct PlayableSampleVtbl;
struct AVISoundBuffer;
struct AVISoundBufferVtbl;
struct MusicPerformance;
struct MusicPerformanceVtbl;
struct BandObj;
struct BandVtbl;
struct CallbackEntry;
struct SampleSink;
struct SampleSinkVtbl;
struct SampleDef;
struct FXItem;
struct FXList;
struct SampleConfig;

struct MusicPerformanceVtbl {
    unsigned char pad_0[0x10];
    void(__stdcall *PlayMotif)(struct MusicPerformance *self, unsigned int motif, unsigned int flags, int a3, int a4, int a5);
    unsigned char pad_14[0x88 - 0x14];
    void(__stdcall *SetGlobalParam)(struct MusicPerformance *self, const GUID *type, void *data, unsigned int size);
};

struct MusicPerformance {
    struct MusicPerformanceVtbl *vtable;
};

struct AVISoundBufferVtbl {
    unsigned char pad_0[8];
    int(__stdcall *Destroy)(struct AVISoundBuffer *self);
    unsigned char pad_c[0x3c - 0xc];
    int(__stdcall *SetVolume)(struct AVISoundBuffer *self, int volume);
    unsigned char pad_40[0x48 - 0x40];
    void(__stdcall *Stop)(struct AVISoundBuffer *self);
};

struct AVISoundBuffer {
    struct AVISoundBufferVtbl *vtable;
};

struct BandVtbl {
    unsigned char pad_0[0xc];
    int(__stdcall *Create)(struct BandObj *self, void *arg);
};

struct BandObj {
    struct BandVtbl *vtable;
};

struct CallbackEntry {
    struct CallbackEntry *next;
    unsigned char pad_4[0x1c - 0x4];
    unsigned short flags;
    unsigned char pad_1e[0x20 - 0x1e];
    unsigned int timeout;
    unsigned int (*callback)(struct CallbackEntry *self);
    unsigned int active;
};

struct SampleSinkVtbl {
    unsigned char pad_0[0x3c];
    void(__stdcall *Apply)(struct SampleSink *self, unsigned int arg);
};

struct SampleSink {
    struct SampleSinkVtbl *vtable;
};

struct PlayableSample {
    unsigned char pad_0[0xc];
    unsigned int field_c;
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
    unsigned char flags_1c;
    unsigned char pad_1d[0x28 - 0x1d];
    unsigned int active;
    struct SampleSink *sink;
};

struct SampleConfig {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
};

struct FXItem {
    struct SampleDef *sample;
    unsigned char pad_4[0xc - 0x4];
};

struct FXList {
    unsigned char pad_0[8];
    struct FXItem items[1];
};

extern unsigned int DAT_004bf774;
extern unsigned int DAT_0079a694;
extern struct MusicPerformance *DAT_007cacdc;
extern GUID GUID_PerfMasterGrooveLevel;
extern unsigned int DAT_007988a0;
extern unsigned int DAT_007988c0;
extern unsigned int DAT_007988c8;
extern struct PlayableSample *DAT_007988cc;

extern int KillSoundSampleSystem(void);
extern int FUN_00495b00(void);
extern struct PlayableSample *CreatePlayableSample(unsigned int def);
extern int PlaySample(struct PlayableSample *sample, unsigned int looping, unsigned int oneshot);
extern void KillPlayableSample(struct PlayableSample *sample);
extern void PauseSingleSample(struct PlayableSample *sample);
extern void DeleteSampleDef(struct SampleDef *def);

// FUNCTION: LEGOLAND 0x00495b90
void SetMusicGrooveLevel(unsigned int level) {
    if (DAT_004bf774 != 0 && DAT_0079a694 != 0) {
        DAT_007cacdc->vtable->SetGlobalParam(DAT_007cacdc, &GUID_PerfMasterGrooveLevel, &level, 1);
    }
}

// FUNCTION: LEGOLAND 0x00495bc0
void LoadMusicStyle(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00495c90
void LoadMusicBand(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00495d60
void LoadMusicChordMap(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00495e30
void LoadMusicSegment(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00495f00
void FUN_00495f00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496010
void FUN_00496010(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496090
void GetMusicBand(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496110
int CreateMusicBandSegment(struct BandObj *band, void *arg) {
    int result;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DAT_0079a694 == 0) {
        return 0;
    }
    result = band->vtable->Create(band, arg);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00496150
void PlaySegmentFromTemplate(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004961d0
void PlaySegment(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496250
unsigned int SetBand(unsigned int band) {
    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DAT_0079a694 == 0) {
        return 0;
    }
    DAT_007cacdc->vtable->PlayMotif(DAT_007cacdc, band, 0x2080, 0, 0, 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496290
unsigned int PlayMotif(unsigned int motif) {
    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DAT_0079a694 == 0) {
        return 0;
    }
    DAT_007cacdc->vtable->PlayMotif(DAT_007cacdc, motif, 0x2080, 0, 0, 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x004962d0
void BlendMusic(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496360
void KLIBAUDIO_CreateAVISoundBuffer(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004963d0
void KLIBAUDIO_PlayAVISoundBuffer(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004963f0
void KLIBAUDIO_LockAVISoundBuffer(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496490
void KLIBAUDIO_UnLockAVISoundBuffer(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004964c0
int KLIBAUDIO_DestroyAVISoundBuffer(struct AVISoundBuffer *buffer) {
    return buffer->vtable->Destroy(buffer) == 0;
}

// FUNCTION: LEGOLAND 0x004964d0
void KLIBAUDIO_StopAVISoundBuffer(struct AVISoundBuffer *buffer) {
    buffer->vtable->Stop(buffer);
}

// FUNCTION: LEGOLAND 0x004964e0
void KLIBAUDIO_SetAVIVolume(struct AVISoundBuffer *buffer) {
    buffer->vtable->SetVolume(buffer, 0);
}

// FUNCTION: LEGOLAND 0x004964f0
void InitSoundSystem(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496520
int KillSoundSystem(void) {
    if (KillSoundSampleSystem() == 0) {
        return 0;
    }
    return FUN_00495b00() != 0;
}

// FUNCTION: LEGOLAND 0x00496540
void FUN_00496540(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496570
int FUN_00496570(int value) {
    value = value * 4;
    if (value > 10000) {
        return 10000;
    }
    if (value < -10000) {
        value = -10000;
    }
    return value;
}

// FUNCTION: LEGOLAND 0x004965a0
void FUN_004965a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496660
int FUN_00496660(struct PlayableSample *sample) {
    if (DAT_007988c0 == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    sample->sink->vtable->Apply(sample->sink, DAT_007988a0);
    return 1;
}

// FUNCTION: LEGOLAND 0x004966a0
void FUN_004966a0(struct PlayableSample *sample) { STUB(); }

// FUNCTION: LEGOLAND 0x00496760
void FUN_00496760(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004967b0
void FUN_004967b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004967f0
void FUN_004967f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004968d0
void FUN_004968d0(void) {
    unsigned int now;
    struct CallbackEntry *entry;
    unsigned int next;

    now = GetTicks();
    if (DAT_007988c0 == 0) {
        return;
    }
    for (entry = (struct CallbackEntry *)DAT_007988cc; entry != 0; entry = entry->next) {
        if (entry->active != 0 && (entry->flags & 0x10) != 0 && (int)now >= (int)entry->timeout) {
            next = entry->callback(entry);
            if (next != 0) {
                entry->timeout = now + next;
            } else {
                entry->flags &= 0xffef;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00496920
void FUN_00496920(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004969d0
void FUN_004969d0(void) {
    FUN_004968d0();
    FUN_00496760();
    FUN_004967f0();
    FUN_00496920();
}

// FUNCTION: LEGOLAND 0x004969f0
unsigned int UnSourcePlayableSample(struct PlayableSample *sample) {
    if (DAT_007988c0 == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    sample->field_c = 0;
    FUN_004966a0(sample);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496a30
unsigned int SourcePlayableSampleToBloke(struct PlayableSample *sample, unsigned int bloke) {
    if (DAT_007988c0 == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    sample->field_c = 1;
    sample->field_10 = bloke;
    FUN_004966a0(sample);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496a70
void SourcePlayableSampleToMapRef(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496ac0
void SourcePlayableSampleToLevelXY(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496b10
void CountSamplesFromSource(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496b80
void KillAllSamplesFromSource(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496c20
void UnSourceAndFadeSample(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496c80
void UnSourceAndFadeAllSamplesFromSource(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496d10
void FUN_00496d10(struct PlayableSample *sample) {
    sample->flags_1c |= 0x20;
}

// FUNCTION: LEGOLAND 0x00496d20
struct PlayableSample *PlayInstanceOfSample(unsigned int def, unsigned int looping, unsigned int oneshot, struct SampleConfig *config) {
    struct PlayableSample *sample;

    sample = CreatePlayableSample(def);
    if (sample == 0) {
        return 0;
    }
    if (config != 0) {
        memcpy(&sample->field_c, &config->field_0, 16);
        FUN_004966a0(sample);
    } else {
        FUN_00496660(sample);
    }
    if (PlaySample(sample, looping, oneshot) == 0) {
        KillPlayableSample(sample);
        return 0;
    }
    if (DAT_007988c8 != 0) {
        PauseSingleSample(sample);
    }
    return sample;
}

// FUNCTION: LEGOLAND 0x00496db0
void AddSFX_Callback(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496dd0
void Load_FXList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496e30
void Kill_FXList(struct FXList *list, int count) {
    struct FXItem *item;

    if (count <= 0) {
        return;
    }
    item = (struct FXItem *)((unsigned char *)list + 8);
    do {
        if (item->sample != 0) {
            DeleteSampleDef(item->sample);
        }
        item->sample = 0;
        item = (struct FXItem *)((unsigned char *)item + 12);
        count--;
    } while (count != 0);
}

// FUNCTION: LEGOLAND 0x00496e60
void FUN_00496e60(void) { STUB(); }
