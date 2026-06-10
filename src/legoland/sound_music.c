#include <windows.h>
#include <dsound.h>
#include "legoland.h"
#include "crt.h"

#include "sound_sfx.h"
#include "sound_music.h"
#include "timer.h"
#include "globals.h"
#include "math.h"

struct AVISoundBuffer;
struct AVISoundBufferVtbl;
struct MusicPerformance;
struct MusicPerformanceVtbl;
struct BandObj;
struct BandVtbl;
struct CallbackEntry;
struct SampleDef;
struct FXItem;
struct FXList;
struct SampleConfig;
struct MusicLoader;
struct MusicLoaderVtbl;
struct ObjectDesc;

struct MusicLoaderVtbl {
    unsigned char pad_0[0xc];
    int(__stdcall *GetObject)(struct MusicLoader *self, struct ObjectDesc *desc, const GUID *iid, void **out);
};

struct MusicLoader {
    struct MusicLoaderVtbl *vtable;
};

struct MusicContainer;
struct MusicContainerVtbl {
    unsigned char pad_0[0xc];
    int(__stdcall *GetObject)(struct MusicContainer *self, const unsigned short *name, void **out);
    unsigned char pad_10[0x28 - 0x10];
    int(__stdcall *EnumObject)(struct MusicContainer *self, const unsigned short *name, void **out);
};

struct MusicContainer {
    struct MusicContainerVtbl *vtable;
};

struct MusicSegment;
struct MusicSegmentVtbl {
    unsigned char pad_0[8];
    int(__stdcall *Release)(struct MusicSegment *self);
    unsigned char pad_c[0x18 - 0xc];
    int(__stdcall *SetRepeats)(struct MusicSegment *self, unsigned int repeats);
};

struct MusicSegment {
    struct MusicSegmentVtbl *vtable;
};

struct MusicComposer;
struct MusicComposerVtbl {
    unsigned char pad_0[0xc];
    int(__stdcall *ComposeSegmentFromTemplate)(struct MusicComposer *self, void *style, unsigned int templateMode, unsigned int activity, void *chordMap, struct MusicSegment **out);
    int(__stdcall *ComposeSegmentFromShape)(struct MusicComposer *self, unsigned int numMeasures, unsigned int shape, unsigned int activity, int intro, int end, void *style, void *chordMap, struct MusicSegment **out);
    unsigned char pad_14[0x18 - 0x14];
    int(__stdcall *ComposeTransition)(struct MusicComposer *self, struct MusicPerformance *perf, unsigned int numMeasures, unsigned int activity, unsigned int command, void *chordMap, struct MusicSegment **out, int param8, int param9);
};

struct MusicComposer {
    struct MusicComposerVtbl *vtable;
};

struct AVIBufferDesc {
    /* 0x00 */ unsigned int dwSize;
    /* 0x04 */ unsigned int dwFlags;
    /* 0x08 */ unsigned int dwBufferBytes;
    /* 0x0c */ unsigned int dwReserved;
    /* 0x10 */ LPWAVEFORMATEX lpwfxFormat;
    /* 0x14 */ GUID guid3DAlgorithm;
};

struct ObjectDesc {
    /* 0x000 */ unsigned int dwSize;
    /* 0x004 */ unsigned int dwValidData;
    /* 0x008 */ GUID guidObject;
    /* 0x018 */ GUID guidClass;
    /* 0x028 */ unsigned char pad_28[0x38 - 0x28];
    /* 0x038 */ unsigned short wszName[0x40];
    /* 0x0b8 */ unsigned char pad_b8[0x138 - 0xb8];
    /* 0x138 */ unsigned short wszFileName[0x10c];
};

extern void *WNDENV_Gethwnd(void);
extern int FUN_00492130(void *hwnd);
extern int FUN_00495a10(void *hwnd);
extern LEGO_EXPORT void GetTileCentre(struct Point *ref, int *out);

struct MusicPerformanceVtbl {
    unsigned char pad_0[8];
    int(__stdcall *Release)(struct MusicPerformance *self);
    unsigned char pad_c[0x10 - 0xc];
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

// FUNCTION: LEGOLAND 0x00495b90
LEGO_EXPORT void SetMusicGrooveLevel(unsigned int level) {
    if (DAT_004bf774 != 0 && DMusicInitialised != 0) {
        ((struct MusicPerformance *)DAT_007cacdc)->vtable->SetGlobalParam((struct MusicPerformance *)DAT_007cacdc, &GUID_PerfMasterGrooveLevel, &level, 1);
    }
}

// FUNCTION: LEGOLAND 0x00495bc0
LEGO_EXPORT int LoadMusicStyle(const char *filename, void **out) {
    struct ObjectDesc desc;
    unsigned short wide[512];
    int result;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, filename, -1, wide, 0x200);
    desc.guidClass = DAT_004ab980;
    desc.dwSize = 0x350;
    wcscpy(desc.wszFileName, wide);
    desc.dwValidData = 0x12;
    result = ((struct MusicLoader *)DAT_007cacd8)->vtable->GetObject((struct MusicLoader *)DAT_007cacd8, &desc, &DAT_004ab610, out);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00495c90
LEGO_EXPORT int LoadMusicBand(const char *filename, void **out) {
    struct ObjectDesc desc;
    unsigned short wide[512];
    int result;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, filename, -1, wide, 0x200);
    desc.guidClass = DAT_004ab8e0;
    desc.dwSize = 0x350;
    wcscpy(desc.wszFileName, wide);
    desc.dwValidData = 0x12;
    result = ((struct MusicLoader *)DAT_007cacd8)->vtable->GetObject((struct MusicLoader *)DAT_007cacd8, &desc, &DAT_004ab5e0, out);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00495d60
LEGO_EXPORT int LoadMusicChordMap(const char *filename, void **out) {
    struct ObjectDesc desc;
    unsigned short wide[512];
    int result;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, filename, -1, wide, 0x200);
    desc.guidClass = DAT_004ab930;
    desc.dwSize = 0x350;
    wcscpy(desc.wszFileName, wide);
    desc.dwValidData = 0x12;
    result = ((struct MusicLoader *)DAT_007cacd8)->vtable->GetObject((struct MusicLoader *)DAT_007cacd8, &desc, &DAT_004ab600, out);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00495e30
LEGO_EXPORT int LoadMusicSegment(const char *filename, void **out) {
    struct ObjectDesc desc;
    unsigned short wide[512];
    int result;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, filename, -1, wide, 0x200);
    desc.guidClass = DAT_004ab9f0;
    desc.dwSize = 0x350;
    wcscpy(desc.wszFileName, wide);
    desc.dwValidData = 0x12;
    result = ((struct MusicLoader *)DAT_007cacd8)->vtable->GetObject((struct MusicLoader *)DAT_007cacd8, &desc, &DAT_004ab670, out);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00495f00
int FUN_00495f00(const char *file, const char *name, void **out) {
    struct ObjectDesc desc;
    unsigned short wideFile[512];
    unsigned short wideName[512];
    int result;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, file, -1, wideFile, 0x200);
    MultiByteToWideChar(0, 0, name, -1, wideName, 0x200);
    desc.guidClass = DAT_004ab9f0;
    desc.dwSize = 0x350;
    wcscpy(desc.wszFileName, wideFile);
    wcscpy(desc.wszName, wideName);
    desc.dwValidData = 0x16;
    result = ((struct MusicLoader *)DAT_007cacd8)->vtable->GetObject((struct MusicLoader *)DAT_007cacd8, &desc, &DAT_004ab670, out);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00496010
int FUN_00496010(const char *filename, struct MusicContainer *container, void **out) {
    unsigned short wide[512];
    int result;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, filename, -1, wide, 0x200);
    result = container->vtable->EnumObject(container, wide, out);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00496090
LEGO_EXPORT int GetMusicBand(const char *filename, struct MusicContainer *container, void **out) {
    unsigned short wide[512];
    int result;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, filename, -1, wide, 0x200);
    result = container->vtable->GetObject(container, wide, out);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00496110
LEGO_EXPORT int CreateMusicBandSegment(struct BandObj *band, void *arg) {
    int result;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    result = band->vtable->Create(band, arg);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00496150
LEGO_EXPORT unsigned int PlaySegmentFromTemplate(void *style, unsigned int templateMode, void *chordMap) {
    struct MusicSegment *segment;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    ((struct MusicComposer *)DAT_007cad44)->vtable->ComposeSegmentFromTemplate((struct MusicComposer *)DAT_007cad44, style, templateMode, 0, chordMap, &segment);
    segment->vtable->SetRepeats(segment, 999);
    ((struct MusicPerformance *)DAT_007cacdc)->vtable->PlayMotif((struct MusicPerformance *)DAT_007cacdc, (unsigned int)segment, 0x2000, 0, 0, 0);
    segment->vtable->Release(segment);
    return 1;
}

// FUNCTION: LEGOLAND 0x004961d0
LEGO_EXPORT unsigned int PlaySegment(unsigned int numMeasures, void *chordMap) {
    struct MusicSegment *segment;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    ((struct MusicComposer *)DAT_007cad44)->vtable->ComposeSegmentFromShape((struct MusicComposer *)DAT_007cad44, numMeasures, 0xa, 2, 3, 0, 0, chordMap, &segment);
    segment->vtable->SetRepeats(segment, 999);
    ((struct MusicPerformance *)DAT_007cacdc)->vtable->PlayMotif((struct MusicPerformance *)DAT_007cacdc, (unsigned int)segment, 0x2000, 0, 0, 0);
    segment->vtable->Release(segment);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496250
LEGO_EXPORT unsigned int SetBand(unsigned int band) {
    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    ((struct MusicPerformance *)DAT_007cacdc)->vtable->PlayMotif((struct MusicPerformance *)DAT_007cacdc, band, 0x2080, 0, 0, 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496290
LEGO_EXPORT unsigned int PlayMotif(unsigned int motif) {
    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    ((struct MusicPerformance *)DAT_007cacdc)->vtable->PlayMotif((struct MusicPerformance *)DAT_007cacdc, motif, 0x2080, 0, 0, 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x004962d0
LEGO_EXPORT unsigned int BlendMusic(unsigned int numMeasures, unsigned int unused, void *chordMap) {
    struct MusicSegment *segment;
    struct MusicSegment *transition;
    unsigned int shape;

    if (DAT_004bf774 == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    shape = 0xa;
    ((struct MusicComposer *)DAT_007cad44)->vtable->ComposeSegmentFromShape((struct MusicComposer *)DAT_007cad44, numMeasures, shape, 2, 3, 0, 0, chordMap, &segment);
    segment->vtable->SetRepeats(segment, 999);
    ((struct MusicComposer *)DAT_007cad44)->vtable->ComposeTransition((struct MusicComposer *)DAT_007cad44, (struct MusicPerformance *)DAT_007cacdc, shape, 0, 0x2022, chordMap, &transition, 0, 0);
    segment->vtable->Release(segment);
    transition->vtable->Release(transition);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496360
LEGO_EXPORT LPDIRECTSOUNDBUFFER KLIBAUDIO_CreateAVISoundBuffer(LPWAVEFORMATEX format, unsigned int bytes) {
    LPDIRECTSOUNDBUFFER buffer;
    struct AVIBufferDesc desc;
    int result;

    if (DAT_007988c0 != 0) {
        buffer = 0;
        desc.dwReserved = 0;
        desc.dwBufferBytes = bytes;
        desc.dwSize = 0x24;
        desc.dwFlags = 0xe0;
        desc.lpwfxFormat = format;
        result = ((LPDIRECTSOUND)DAT_007cad40)->lpVtbl->CreateSoundBuffer((LPDIRECTSOUND)DAT_007cad40, (LPCDSBUFFERDESC)&desc, &buffer, 0);
        if (result != 0) {
            return 0;
        }
        return buffer;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004963d0
LEGO_EXPORT void KLIBAUDIO_PlayAVISoundBuffer(LPDIRECTSOUNDBUFFER buffer, unsigned int position) {
    if (buffer->lpVtbl->SetCurrentPosition(buffer, position) == 0) {
        buffer->lpVtbl->Play(buffer, 0, 0, 1);
    }
}

// FUNCTION: LEGOLAND 0x004963f0
LEGO_EXPORT void *KLIBAUDIO_LockAVISoundBuffer(LPDIRECTSOUNDBUFFER buffer, unsigned int offset, unsigned int size) {
    unsigned int status;
    unsigned int playPos;
    unsigned int writePos;
    int result;

    if (buffer == 0) {
        return 0;
    }
    if (buffer->lpVtbl->GetStatus(buffer, (LPDWORD)&status) == 0 && (status & 1) != 0) {
        while (buffer->lpVtbl->GetCurrentPosition(buffer, (LPDWORD)&playPos, (LPDWORD)&writePos) == 0 &&
               playPos >= offset && playPos < offset + size) {
        }
    }
    result = buffer->lpVtbl->Lock(buffer, offset, size, &DAT_007988a4, (LPDWORD)&DAT_00798898, &DAT_007988a8, (LPDWORD)&DAT_0079889c, 0);
    return result != 0 ? 0 : DAT_007988a4;
}

// FUNCTION: LEGOLAND 0x00496490
LEGO_EXPORT void KLIBAUDIO_UnLockAVISoundBuffer(LPDIRECTSOUNDBUFFER buffer) {
    if (buffer != 0) {
        buffer->lpVtbl->Unlock(buffer, DAT_007988a4, DAT_00798898, DAT_007988a8, DAT_0079889c);
    }
}

// FUNCTION: LEGOLAND 0x004964c0
LEGO_EXPORT int KLIBAUDIO_DestroyAVISoundBuffer(struct AVISoundBuffer *buffer) {
    return buffer->vtable->Destroy(buffer) == 0;
}

// FUNCTION: LEGOLAND 0x004964d0
LEGO_EXPORT void KLIBAUDIO_StopAVISoundBuffer(struct AVISoundBuffer *buffer) {
    buffer->vtable->Stop(buffer);
}

// FUNCTION: LEGOLAND 0x004964e0
LEGO_EXPORT void KLIBAUDIO_SetAVIVolume(struct AVISoundBuffer *buffer) {
    buffer->vtable->SetVolume(buffer, 0);
}

// FUNCTION: LEGOLAND 0x004964f0
LEGO_EXPORT int InitSoundSystem(void) {
    DAT_007988b0 = WNDENV_Gethwnd();
    if (FUN_00492130(DAT_007988b0) == 0) {
        return 0;
    }
    return FUN_00495a10(DAT_007988b0) != 0;
}

// FUNCTION: LEGOLAND 0x00496520
LEGO_EXPORT int KillSoundSystem(void) {
    if (KillSoundSampleSystem() == 0) {
        return 0;
    }
    return FUN_00495b00() != 0;
}

// FUNCTION: LEGOLAND 0x00496540
int FUN_00496540(int param_1) {
    int result;

    result = DAT_007988a0 - param_1 / 0x3c;
    if (result < -3000 || result > 0) {
        result = -10000;
    }
    return result;
}

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
int FUN_004965a0(struct Sample *sample, int x, int y) {
    unsigned int dim;
    int vol;
    int pan;
    int result;

    x = x - (lpConfig->field_10 >> 1);
    y = y - (lpConfig->field_12 >> 1);
    vol = FUN_00496570(x);
    dim = lpConfig->field_10;
    if (x < -(int)dim >> 1) {
        x = x + (dim >> 1);
    } else if (x > (int)(dim >> 1)) {
        x = x - (dim >> 1);
    } else {
        x = 0;
    }
    dim = lpConfig->field_12;
    if (y < -(int)dim >> 1) {
        y = y + (dim >> 1);
    } else if (y > (int)(dim >> 1)) {
        y = y - (dim >> 1);
    } else {
        y = 0;
    }
    pan = FUN_00496540(y * y + x * x);
    result = sample->buffer->vtable->method_0x3c(sample->buffer, pan);
    if (result != 0) {
        return 0;
    }
    return sample->buffer->vtable->method_0x40(sample->buffer, vol) == 0;
}

// FUNCTION: LEGOLAND 0x00496660
int FUN_00496660(struct Sample *sample) {
    if (DAT_007988c0 == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    sample->buffer->vtable->method_0x3c(sample->buffer, DAT_007988a0); /* Apply: same vtable slot, return ignored */
    return 1;
}

// FUNCTION: LEGOLAND 0x004966a0
int FUN_004966a0(struct Sample *sample) {
    int coord[2];
    int *obj;

    if (DAT_007988c0 == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    switch (sample->field_c) {
    case 0:
        return FUN_00496660(sample);
    case 1:
        obj = *(int **)(*(int **)&sample->field_10 + 1);
        coord[0] = obj[7];
        coord[1] = obj[8];
        break;
    case 2:
        GetTileCentre((struct Point *)&sample->field_14, coord);
        break;
    case 3:
        coord[0] = sample->field_14 - (ScrollX >> 8);
        coord[1] = sample->field_18 - (ScrollY >> 8);
    }
    return FUN_004965a0(sample, coord[0], coord[1]);
}

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
LEGO_EXPORT unsigned int UnSourcePlayableSample(struct Sample *sample) {
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
LEGO_EXPORT unsigned int SourcePlayableSampleToBloke(struct Sample *sample, unsigned int bloke) {
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
LEGO_EXPORT void SourcePlayableSampleToMapRef(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496ac0
LEGO_EXPORT void SourcePlayableSampleToLevelXY(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496b10
LEGO_EXPORT void CountSamplesFromSource(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496b80
LEGO_EXPORT void KillAllSamplesFromSource(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496c20
LEGO_EXPORT void UnSourceAndFadeSample(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496c80
LEGO_EXPORT void UnSourceAndFadeAllSamplesFromSource(void *source, int fade) { STUB(); }

// FUNCTION: LEGOLAND 0x00496d10
void FUN_00496d10(struct Sample *sample) {
    sample->flags |= 0x20;
}

// FUNCTION: LEGOLAND 0x00496d20
LEGO_EXPORT struct Sample *PlayInstanceOfSample(void *def, unsigned int looping, unsigned int oneshot, void *config) {
    struct Sample *sample;

    sample = CreatePlayableSample((unsigned int)def);
    if (sample == 0) {
        return 0;
    }
    if (config != 0) {
        memcpy(&sample->field_c, &((struct SampleConfig *)config)->field_0, 16);
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
LEGO_EXPORT void AddSFX_Callback(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496dd0
LEGO_EXPORT void Load_FXList(const unsigned char *list, unsigned int count) { STUB(); }

// FUNCTION: LEGOLAND 0x00496e30
LEGO_EXPORT void Kill_FXList(const unsigned char *list, int count) {
    struct FXItem *item;

    if (count <= 0) {
        return;
    }
    item = (struct FXItem *)((unsigned char *)list + 8); /* SFX arrays are raw byte buffers (varied sizes per TU); not a struct view to fold */
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
