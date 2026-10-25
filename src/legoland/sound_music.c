#include <windows.h>
#include <dsound.h>
#include <stdio.h>
#include <string.h>
#include "legoland.h"

#include "debug_alloc.h"
#include "globals.h"
#include "math.h"
#include "profile_io.h"
#include "sound_music.h"
#include "sound_sfx.h"
#include "stream.h"
#include "tilemap.h"
#include "timer.h"
#include "wndenv.h"

struct AVISoundBuffer;
struct AVISoundBufferVtbl;
struct MusicPerformance;
struct MusicPerformanceVtbl;
struct BandObj;
struct BandVtbl;
struct FXList;
struct MusicLoader;
struct MusicLoaderVtbl;
struct ObjectDesc;

struct MusicLoaderVtbl {
    void *pad_0[3];
    int(__stdcall *GetObject)(struct MusicLoader *self, struct ObjectDesc *desc, const GUID *iid, void **out);
};

struct MusicLoader {
    struct MusicLoaderVtbl *vtable;
};

struct MusicStyle;
struct MusicStyleVtbl {
    void *pad_0[3];
    int(__stdcall *GetObject)(struct MusicStyle *self, const unsigned short *name, void **out);
    void *pad_10[6];
    int(__stdcall *EnumObject)(struct MusicStyle *self, const unsigned short *name, void **out);
};

struct MusicStyle {
    struct MusicStyleVtbl *vtable;
};

struct MusicSegment;
struct MusicSegmentVtbl {
    void *pad_0[2];
    int(__stdcall *Release)(struct MusicSegment *self);
    void *pad_c[3];
    int(__stdcall *SetRepeats)(struct MusicSegment *self, unsigned int repeats);
};

struct MusicSegment {
    struct MusicSegmentVtbl *vtable;
};

struct MusicComposer;
struct MusicComposerVtbl {
    void *pad_0[3];
    int(__stdcall *ComposeSegmentFromTemplate)(struct MusicComposer *self, void *style, unsigned int templateMode, unsigned int activity, void *chordMap, struct MusicSegment **out);
    int(__stdcall *ComposeSegmentFromShape)(struct MusicComposer *self, unsigned int numMeasures, unsigned int shape, unsigned int activity, int intro, int end, void *style, void *chordMap, struct MusicSegment **out);
    void *pad_14[1];
    int(__stdcall *AutoTransition)(struct MusicComposer *self, struct MusicPerformance *perf, struct MusicSegment *to_segment, unsigned int command, unsigned int flags, void *chordMap, struct MusicSegment **out, void *seg_state_to, void *seg_state_transition);
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

struct MusicPerformanceVtbl {
    void *pad_0[2];
    int(__stdcall *Release)(struct MusicPerformance *self);
    void *pad_c[1];
    void(__stdcall *PlayMotif)(struct MusicPerformance *self, void *segment, unsigned int flags, int a3, int a4, int a5); /* PlaySegment */
    void *pad_14[29];
    void(__stdcall *SetGlobalParam)(struct MusicPerformance *self, const GUID *type, void *data, unsigned int size);
};

struct MusicPerformance {
    struct MusicPerformanceVtbl *vtable;
};

struct AVISoundBufferVtbl {
    void *pad_0[2];
    int(__stdcall *Destroy)(struct AVISoundBuffer *self);
    void *pad_c[12];
    int(__stdcall *SetVolume)(struct AVISoundBuffer *self, int volume);
    void *pad_40[2];
    void(__stdcall *Stop)(struct AVISoundBuffer *self);
};

struct AVISoundBuffer {
    struct AVISoundBufferVtbl *vtable;
};

struct BandVtbl {
    void *pad_0[3];
    int(__stdcall *Create)(struct BandObj *self, void *arg);
};

struct BandObj {
    struct BandVtbl *vtable;
};

// FUNCTION: LEGOLAND 0x00495b90
LEGO_EXPORT void SetMusicGrooveLevel(unsigned int level) {
    if (MusicEnabled != 0 && DMusicInitialised != 0) {
        ((struct MusicPerformance *)DMusicPerformance)->vtable->SetGlobalParam((struct MusicPerformance *)DMusicPerformance, &GUID_PerfMasterGrooveLevel, &level, 1);
    }
}

// FUNCTION: LEGOLAND 0x00495bc0
LEGO_EXPORT int LoadMusicStyle(const char *filename, void **out) {
    struct ObjectDesc desc;
    unsigned short wide[512];
    int result;

    if (MusicEnabled == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, filename, -1, wide, 0x200);
    desc.guidClass = CLSID_DirectMusicStyle;
    desc.dwSize = 0x350;
    wcscpy(desc.wszFileName, wide);
    desc.dwValidData = 0x12;
    result = ((struct MusicLoader *)DMusicLoader)->vtable->GetObject((struct MusicLoader *)DMusicLoader, &desc, &IID_IDirectMusicStyle, out);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00495c90
LEGO_EXPORT int LoadMusicBand(const char *filename, void **out) {
    struct ObjectDesc desc;
    unsigned short wide[512];
    int result;

    if (MusicEnabled == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, filename, -1, wide, 0x200);
    desc.guidClass = DirectMusicBandClassGuid;
    desc.dwSize = 0x350;
    wcscpy(desc.wszFileName, wide);
    desc.dwValidData = 0x12;
    result = ((struct MusicLoader *)DMusicLoader)->vtable->GetObject((struct MusicLoader *)DMusicLoader, &desc, &IID_IDirectMusicBand, out);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00495d60
LEGO_EXPORT int LoadMusicChordMap(const char *filename, void **out) {
    struct ObjectDesc desc;
    unsigned short wide[512];
    int result;

    if (MusicEnabled == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, filename, -1, wide, 0x200);
    desc.guidClass = DirectMusicChordMapClassGuid;
    desc.dwSize = 0x350;
    wcscpy(desc.wszFileName, wide);
    desc.dwValidData = 0x12;
    result = ((struct MusicLoader *)DMusicLoader)->vtable->GetObject((struct MusicLoader *)DMusicLoader, &desc, &IID_IDirectMusicChordMap, out);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00495e30
LEGO_EXPORT int LoadMusicSegment(const char *filename, void **out) {
    struct ObjectDesc desc;
    unsigned short wide[512];
    int result;

    if (MusicEnabled == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, filename, -1, wide, 0x200);
    desc.guidClass = CLSID_DirectMusicSegment;
    desc.dwSize = 0x350;
    wcscpy(desc.wszFileName, wide);
    desc.dwValidData = 0x12;
    result = ((struct MusicLoader *)DMusicLoader)->vtable->GetObject((struct MusicLoader *)DMusicLoader, &desc, &IID_IDirectMusicSegment, out);
    return 0 <= result;
}

// FUNCTION: LEGOLAND 0x00495f00
int LoadDirectMusicSegment(const char *file, const char *name, void **out) {
    struct ObjectDesc desc;
    unsigned short wideFile[512];
    unsigned short wideName[512];
    int result;

    if (MusicEnabled == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    *out = 0;
    MultiByteToWideChar(0, 0, file, -1, wideFile, 0x200);
    MultiByteToWideChar(0, 0, name, -1, wideName, 0x200);
    desc.guidClass = CLSID_DirectMusicSegment;
    desc.dwSize = 0x350;
    wcscpy(desc.wszFileName, wideFile);
    wcscpy(desc.wszName, wideName);
    desc.dwValidData = 0x16;
    result = ((struct MusicLoader *)DMusicLoader)->vtable->GetObject((struct MusicLoader *)DMusicLoader, &desc, &IID_IDirectMusicSegment, out);
    return 0 <= result;
}

/* Gets the chord map called filename from a DirectMusic style: vtable slot 10 of IDirectMusicStyle is GetChordMap
 * (slot 3, used by GetMusicBand, is GetBand). Exported like its neighbours; the game itself never calls it. */
// FUNCTION: LEGOLAND 0x00496010
int GetMusicChordMap(const char *filename, struct MusicStyle *container, void **out) {
    unsigned short wide[512];
    int result;

    if (MusicEnabled == 0) {
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
LEGO_EXPORT int GetMusicBand(const char *filename, struct MusicStyle *container, void **out) {
    unsigned short wide[512];
    int result;

    if (MusicEnabled == 0) {
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

    if (MusicEnabled == 0) {
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

    if (MusicEnabled == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    ((struct MusicComposer *)DMusicComposer)->vtable->ComposeSegmentFromTemplate((struct MusicComposer *)DMusicComposer, style, templateMode, 0, chordMap, &segment);
    segment->vtable->SetRepeats(segment, 999);
    ((struct MusicPerformance *)DMusicPerformance)->vtable->PlayMotif((struct MusicPerformance *)DMusicPerformance, segment, 0x2000, 0, 0, 0);
    segment->vtable->Release(segment);
    return 1;
}

// FUNCTION: LEGOLAND 0x004961d0
LEGO_EXPORT unsigned int PlaySegment(unsigned int numMeasures, void *chordMap) {
    struct MusicSegment *segment;

    if (MusicEnabled == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    ((struct MusicComposer *)DMusicComposer)->vtable->ComposeSegmentFromShape((struct MusicComposer *)DMusicComposer, numMeasures, 0xa, 2, 3, 0, 0, chordMap, &segment);
    segment->vtable->SetRepeats(segment, 999);
    ((struct MusicPerformance *)DMusicPerformance)->vtable->PlayMotif((struct MusicPerformance *)DMusicPerformance, segment, 0x2000, 0, 0, 0);
    segment->vtable->Release(segment);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496250
LEGO_EXPORT unsigned int SetBand(void *band) {
    if (MusicEnabled == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    ((struct MusicPerformance *)DMusicPerformance)->vtable->PlayMotif((struct MusicPerformance *)DMusicPerformance, band, 0x2080, 0, 0, 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496290
LEGO_EXPORT unsigned int PlayMotif(void *motif) {
    if (MusicEnabled == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    ((struct MusicPerformance *)DMusicPerformance)->vtable->PlayMotif((struct MusicPerformance *)DMusicPerformance, motif, 0x2080, 0, 0, 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x004962d0
LEGO_EXPORT unsigned int BlendMusic(unsigned int numMeasures, unsigned int unused, void *chordMap) {
    struct MusicSegment *segment;
    struct MusicSegment *transition;
    unsigned int shape;

    if (MusicEnabled == 0) {
        return 0;
    }
    if (DMusicInitialised == 0) {
        return 0;
    }
    shape = 0xa;
    ((struct MusicComposer *)DMusicComposer)->vtable->ComposeSegmentFromShape((struct MusicComposer *)DMusicComposer, numMeasures, shape, 2, 3, 0, 0, chordMap, &segment);
    segment->vtable->SetRepeats(segment, 999);
    ((struct MusicComposer *)DMusicComposer)->vtable->AutoTransition((struct MusicComposer *)DMusicComposer, (struct MusicPerformance *)DMusicPerformance, segment, 0, 0x2022, chordMap, &transition, 0, 0);
    segment->vtable->Release(segment);
    transition->vtable->Release(transition);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496360
LEGO_EXPORT LPDIRECTSOUNDBUFFER KLIBAUDIO_CreateAVISoundBuffer(LPWAVEFORMATEX format, unsigned int bytes) {
    LPDIRECTSOUNDBUFFER buffer;
    struct AVIBufferDesc desc;
    int result;

    if (SoundAvailable != 0) {
        buffer = 0;
        desc.dwReserved = 0;
        desc.dwBufferBytes = bytes;
        desc.dwSize = 0x24;
        desc.dwFlags = 0xe0;
        desc.lpwfxFormat = format;
        result = ((LPDIRECTSOUND)DSound)->lpVtbl->CreateSoundBuffer((LPDIRECTSOUND)DSound, (LPCDSBUFFERDESC)&desc, &buffer, 0);
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
    result = buffer->lpVtbl->Lock(buffer, offset, size, &AviLockPtr1, (LPDWORD)&AviLockBytes1, &AviLockPtr2, (LPDWORD)&AviLockBytes2, 0);
    return result != 0 ? 0 : AviLockPtr1;
}

// FUNCTION: LEGOLAND 0x00496490
LEGO_EXPORT void KLIBAUDIO_UnLockAVISoundBuffer(LPDIRECTSOUNDBUFFER buffer) {
    if (buffer != 0) {
        buffer->lpVtbl->Unlock(buffer, AviLockPtr1, AviLockBytes1, AviLockPtr2, AviLockBytes2);
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
LEGO_EXPORT void KLIBAUDIO_SetAVIVolume(struct AVISoundBuffer *buffer, int volume) {
    buffer->vtable->SetVolume(buffer, 0);
}

// FUNCTION: LEGOLAND 0x004964f0
LEGO_EXPORT int InitSoundSystem(void) {
    SoundHwnd = WNDENV_Gethwnd();
    if (InitDirectSound(SoundHwnd) == 0) {
        return 0;
    }
    return StartMusicThread(SoundHwnd) != 0;
}

// FUNCTION: LEGOLAND 0x00496520
LEGO_EXPORT int KillSoundSystem(void) {
    if (KillSoundSampleSystem() == 0) {
        return 0;
    }
    return ShutDownDirectMusic() != 0;
}

// FUNCTION: LEGOLAND 0x00496540
int CalcDistanceVolume(int param_1) {
    int result;

    result = SfxDSVolume - param_1 / 0x3c;
    if (result < -3000 || result > 0) {
        result = -10000;
    }
    return result;
}

/* Converts a horizontal screen offset to DirectSound pan by multiplying by four and clamping to -10000..10000. */
// FUNCTION: LEGOLAND 0x00496570
int ScreenOffsetToSamplePan(int value) {
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
int SetSampleVolumeAndPan(struct Sample *sample, int x, int y) {
    unsigned int dim;
    int vol;
    int pan;
    int result;

    x = x - (lpConfig->view_width >> 1);
    y = y - (lpConfig->view_height >> 1);
    vol = ScreenOffsetToSamplePan(x);
    dim = lpConfig->view_width;
    if (x < -(int)dim >> 1) {
        x = x + (dim >> 1);
    } else if (x > (int)(dim >> 1)) {
        x = x - (dim >> 1);
    } else {
        x = 0;
    }
    dim = lpConfig->view_height;
    if (y < -(int)dim >> 1) {
        y = y + (dim >> 1);
    } else if (y > (int)(dim >> 1)) {
        y = y - (dim >> 1);
    } else {
        y = 0;
    }
    pan = CalcDistanceVolume(y * y + x * x);
    result = sample->buffer->vtable->method_0x3c(sample->buffer, pan);
    if (result != 0) {
        return 0;
    }
    return sample->buffer->vtable->method_0x40(sample->buffer, vol) == 0;
}

// FUNCTION: LEGOLAND 0x00496660
int ApplySampleVolume(struct Sample *sample) {
    if (SoundAvailable == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    sample->buffer->vtable->method_0x3c(sample->buffer, SfxDSVolume); /* Apply: same vtable slot, return ignored */
    return 1;
}

/* Updates an active sample using its source type: non-positional volume, a rider position, a tile center, or a
 * screen position adjusted by the scroll offset. */
// FUNCTION: LEGOLAND 0x004966a0
int UpdateSampleVolume(struct Sample *sample) {
    int coord[2];
    int *obj;

    unsigned int obj8;
    if (SoundAvailable == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    switch (sample->source_type) {
    case 0:
        return ApplySampleVolume(sample);
    case 1:
        obj = *(int **)(*(int **)&sample->bloke + 1);
        obj8 = obj[8];
        coord[0] = obj[7];
        coord[1] = obj8;
        break;
    case 2:
        GetTileCentre((struct Point *)&sample->source_x, coord);
        break;
    case 3:
        coord[1] = sample->source_y - (ScrollY >> 8);
        coord[0] = sample->source_x - (ScrollX >> 8);
    }
    return SetSampleVolumeAndPan(sample, coord[0], coord[1]);
}

// FUNCTION: LEGOLAND 0x00496760
void UpdatePlayingSamples(void) {
    struct Sample *sample;
    unsigned int status;

    if (SoundAvailable == 0) {
        return;
    }
    for (sample = (struct Sample *)SampleListHead; sample != 0; sample = sample->next) {
        if (sample->active != 0 && sample->source_type != 0 &&
            sample->buffer->vtable->method_0x24(sample->buffer, &status) == 0 && (status & 1) != 0) {
            UpdateSampleVolume(sample);
        }
    }
}

// FUNCTION: LEGOLAND 0x004967b0
void RefreshSampleVolumes(void) {
    struct Sample *sample;
    unsigned int status;

    if (SoundAvailable == 0) {
        return;
    }
    for (sample = (struct Sample *)SampleListHead; sample != 0; sample = sample->next) {
        if (sample->active != 0 &&
            sample->buffer->vtable->method_0x24(sample->buffer, &status) == 0) {
            UpdateSampleVolume(sample);
        }
    }
}

// FUNCTION: LEGOLAND 0x004967f0
void UpdateSampleFades(void) {
    struct Sample *sample;
    int vol;

    if (SoundAvailable == 0) {
        return;
    }
    for (sample = (struct Sample *)SampleListHead; sample != 0; sample = sample->next) {
        if (sample->active != 0 && (sample->flags & 2) == 0 && sample->fade != 0 &&
            sample->buffer->vtable->method_0x18(sample->buffer, &vol) == 0) {
            vol = vol + sample->fade;
            // STRING: LEGOLAND 0x004bfddc
            DBPrintf("Fading Sample (%s) (%x) to Vol %d\n", ((struct Sample *)sample->active)->name, sample, vol);
            if (vol >= 0) {
                vol = 0;
                sample->fade = 0;
            } else if (vol <= -3000) {
                vol = -10000;
                sample->fade = 0;
                if ((sample->flags & 8) != 0) {
                    sample->buffer->vtable->method_0x48(sample->buffer);
                    // STRING: LEGOLAND 0x004bfdb4
                    DBPrintf("\tStopping Sample for the kill %s (%x)\n", ((struct Sample *)sample->active)->name, sample);
                }
            }
            sample->buffer->vtable->method_0x3c(sample->buffer, vol);
        }
    }
}

// FUNCTION: LEGOLAND 0x004968d0
void UpdateSfxCallbacks(void) {
    unsigned int now;
    struct Sample *entry;
    unsigned int next;

    now = GetTicks();
    if (SoundAvailable == 0) {
        return;
    }
    for (entry = (struct Sample *)SampleListHead; entry != 0; entry = entry->next) {
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
void AutoKillFinishedSamples(void) {
    struct Sample *sample;
    struct Sample *prev;
    struct Sample *next;
    unsigned int status;

    prev = 0;
    sample = (struct Sample *)SampleListHead;
    while (sample != 0) {
        next = sample->next;
        if ((sample->flags & 8) != 0 && (sample->flags & 2) == 0 &&
            sample->buffer->vtable->method_0x24(sample->buffer, &status) == 0 && ((status == 0) & 1)) {
            // STRING: LEGOLAND 0x004bfe14
            DBPrintf("Autokilling Sample %s (%x)\n", ((struct Sample *)sample->active)->name, sample);
            if (sample->active == JOUST_SFX[0].sample) {
                // STRING: LEGOLAND 0x004bfe00
                DBPrintf("Killing Joust FX\n");
            }
            if (prev != 0) {
                prev->next = sample->next;
                FreeSample(sample);
            } else {
                SampleListHead = sample->next;
                FreeSample(sample);
            }
        } else {
            prev = sample;
        }
        sample = next;
    }
}

// FUNCTION: LEGOLAND 0x004969d0
void UpdateSound(void) {
    UpdateSfxCallbacks();
    UpdatePlayingSamples();
    UpdateSampleFades();
    AutoKillFinishedSamples();
}

// FUNCTION: LEGOLAND 0x004969f0
LEGO_EXPORT unsigned int UnSourcePlayableSample(struct Sample *sample) {
    if (SoundAvailable == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    sample->source_type = 0;
    UpdateSampleVolume(sample);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496a30
LEGO_EXPORT unsigned int SourcePlayableSampleToBloke(struct Sample *sample, void *bloke) {
    if (SoundAvailable == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    sample->source_type = 1;
    sample->bloke = bloke;
    UpdateSampleVolume(sample);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496a70
LEGO_EXPORT unsigned int SourcePlayableSampleToMapRef(struct Sample *sample, unsigned int x, unsigned int y) {
    if (SoundAvailable == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    sample->source_x = x;
    sample->source_type = 2;
    sample->source_y = y;
    UpdateSampleVolume(sample);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496ac0
LEGO_EXPORT unsigned int SourcePlayableSampleToLevelXY(struct Sample *sample, unsigned int x, unsigned int y) {
    if (SoundAvailable == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    sample->source_x = x;
    sample->source_type = 3;
    sample->source_y = y;
    UpdateSampleVolume(sample);
    return 1;
}

// FUNCTION: LEGOLAND 0x00496b10
LEGO_EXPORT int CountSamplesFromSource(struct SampleSource *source) {
    int count;
    struct Sample *sample;

    count = 0;
    if (SampleListHead != 0) {
        sample = (struct Sample *)SampleListHead;
        do {
            if (sample->source_type == source->type) {
                switch (source->type) {
                case 0:
                    count = count + 1;
                    break;
                case 1:
                    if (sample->bloke == source->bloke) {
                        count = count + 1;
                    }
                    break;
                case 2:
                case 3:
                    if (sample->source_x == source->x && sample->source_y == source->y) {
                        count = count + 1;
                    }
                }
            }
            sample = sample->next;
        } while (sample != 0);
    }
    return count;
}

// FUNCTION: LEGOLAND 0x00496b80
LEGO_EXPORT void KillAllSamplesFromSource(struct SampleSource *source) {
    struct Sample *prev;
    struct Sample *sample;
    struct Sample *next;
    int matched;

    prev = 0;
    for (sample = (struct Sample *)SampleListHead; sample != 0; sample = next) {
        next = sample->next;
        if (sample->source_type == source->type) {
            switch (source->type) {
            case 0:
                matched = 1;
                break;
            case 1:
                matched = sample->bloke == source->bloke;
                break;
            case 2:
            case 3:
                matched = sample->source_x == source->x && sample->source_y == source->y;
                break;
            }
            if (matched) {
                if (prev != 0) {
                    prev->next = next;
                } else {
                    SampleListHead = next;
                }
                FreeSample(sample);
                continue;
            }
        }
        prev = sample;
    }
}

// FUNCTION: LEGOLAND 0x00496c20
LEGO_EXPORT void UnSourceAndFadeSample(struct Sample *sample, unsigned int fade) {
    int vol;

    sample->flags |= 8;
    sample->fade = fade;
    sample->source_type = 0;
    sample->buffer->vtable->method_0x18(sample->buffer, &vol);
    // STRING: LEGOLAND 0x004bfe58
    DBPrintf("Fading out Sample %s (%x) [Current Vol = %d]\n", ((struct Sample *)sample->active)->name, sample, vol);
    if ((sample->flags & 2) != 0) {
        sample->flags &= 0xfffd;
        // STRING: LEGOLAND 0x004bfe30
        DBPrintf("Sample should Kill immediately (%x)\n", sample);
    }
}

// FUNCTION: LEGOLAND 0x00496c80
LEGO_EXPORT void UnSourceAndFadeAllSamplesFromSource(void *source, int fade) {
    struct SampleSource *src;
    struct Sample *sample;
    unsigned int matched;

    src = (struct SampleSource *)source;
    for (sample = (struct Sample *)SampleListHead; sample != 0; sample = sample->next) {
        if (sample->source_type == src->type) {
            switch (src->type) {
            case 0:
                matched = 1;
                break;
            case 1:
                matched = sample->bloke == src->bloke;
                break;
            case 2:
            case 3:
                matched = sample->source_x == src->x && sample->source_y == src->y;
                break;
            }
            if (matched) {
                UnSourceAndFadeSample(sample, fade);
            }
        }
    }
}

/* Sets sample flag 0x20. */
// FUNCTION: LEGOLAND 0x00496d10
void SetSampleFlag20(struct Sample *sample) {
    sample->flags |= 0x20;
}

// FUNCTION: LEGOLAND 0x00496d20
LEGO_EXPORT struct Sample *PlayInstanceOfSample(void *def, unsigned int looping, unsigned int oneshot, void *config) {
    struct Sample *sample;

    sample = CreatePlayableSample(def);
    if (sample == 0) {
        return 0;
    }
    if (config != 0) {
        memcpy(&sample->source_type, &((struct SampleSource *)config)->type, 16);
        UpdateSampleVolume(sample);
    } else {
        ApplySampleVolume(sample);
    }
    if (PlaySample(sample, looping, oneshot) == 0) {
        KillPlayableSample(sample);
        return 0;
    }
    if (PauseNewSamples != 0) {
        PauseSingleSample(sample);
    }
    return sample;
}

// FUNCTION: LEGOLAND 0x00496db0
LEGO_EXPORT void AddSFX_Callback(struct Sample *entry, unsigned int delay, unsigned int (*callback)(struct Sample *self)) {
    unsigned int now;

    now = GetTicks();
    entry->timeout = now + delay;
    entry->flags |= 0x10;
    entry->callback = callback;
}

// FUNCTION: LEGOLAND 0x00496dd0
LEGO_EXPORT void Load_FXList(struct FXItem *list, int count) {
    char path[100];
    struct FXItem *item;
    int i;

    for (i = 0; i < count; i++) {
        item = &list[i];
        // STRING: LEGOLAND 0x004bfea0
        sprintf(path, ".\\sfx\\%s", item->name);
        item->sample = CreateSampleFromWAV(path);
        if (item->sample != 0) {
            item->sample->name = item->name;
        } else {
            // STRING: LEGOLAND 0x004bfe88
            DBPrintf("Failed to load SFX %s\n", item->name);
        }
    }
}

// FUNCTION: LEGOLAND 0x00496e30
LEGO_EXPORT void Kill_FXList(struct FXItem *list, int count) {
    struct FXItem *item;

    if (count <= 0) {
        return;
    }
    item = list;
    do {
        if (item->sample != 0) {
            DeleteSampleDef(item->sample);
        }
        item->sample = 0;
        item = item + 1;
        count--;
    } while (count != 0);
}

/* Fades speech and effects volumes to zero in timed steps, pauses samples and closes speech, then restores the
 * configured volume settings. */
// FUNCTION: LEGOLAND 0x00496e60
void FadeOutSpeechAndEffects(int param_1, int param_2) {
    int savedC4;
    int savedCC;
    unsigned int start;
    unsigned int now;

    savedC4 = CurrentProfile.speech_volume;
    savedCC = CurrentProfile.fx_volume;
    while (CurrentProfile.fx_volume > 0 || CurrentProfile.speech_volume > 0) {
        start = GetTicks();
        CurrentProfile.fx_volume = CurrentProfile.fx_volume - param_1;
        if (CurrentProfile.fx_volume <= 0) {
            CurrentProfile.fx_volume = 0;
        }
        CurrentProfile.speech_volume = CurrentProfile.speech_volume - param_1;
        if (CurrentProfile.speech_volume <= 0) {
            CurrentProfile.speech_volume = 0;
        }
        UpdateSoundVols();
        now = GetTicks();
        while ((int)(start + param_2) > (int)now) {
            now = GetTicks();
        }
    }
    PauseAllSamples();
    SpeechCloseFile();
    SpeechDelayFrames = 4;
    CurrentProfile.speech_volume = savedC4;
    CurrentProfile.fx_volume = savedCC;
    UpdateSoundVols();
}
