#include <windows.h>
#include "legoland.h"
#include "crt.h"
#include "globals.h"
#include "sound_sfx.h"

struct DirectSoundObj;
struct DirectMusicObj;

struct SampleCounter {
    unsigned char pad_0[4];
    int count;
};

struct SampleDefVtbl {
    void *func_0;
    void *func_4;
    void(__stdcall *func_8)(struct SampleBuffer *self);
};

struct DirectSoundVtbl {
    void *QueryInterface;
    void *AddRef;
    void(__stdcall *Release)(struct DirectSoundObj *self);
};

struct DirectSoundObj {
    struct DirectSoundVtbl *vtable;
};

struct DirectMusicLoaderVtbl {
    void *QueryInterface;
    void *AddRef;
    void(__stdcall *Release)(struct DirectMusicObj *self);
    unsigned char pad_c[0x24 - 0xc];
    void(__stdcall *ClearCache)(struct DirectMusicObj *self, const GUID *type);
};

struct DirectMusicPerformanceVtbl {
    void *QueryInterface;
    void *AddRef;
    void(__stdcall *Release)(struct DirectMusicObj *self);
    unsigned char pad_c[0x14 - 0xc];
    void(__stdcall *Stop)(struct DirectMusicObj *self, int a1, int a2, int a3, int a4);
    unsigned char pad_18[0x98 - 0x18];
    void(__stdcall *CloseDown)(struct DirectMusicObj *self);
};

struct DirectMusicComposerVtbl {
    void *QueryInterface;
    void *AddRef;
    void(__stdcall *Release)(struct DirectMusicObj *self);
};

struct DirectMusicObj {
    void *vtable;
};


// FUNCTION: LEGOLAND 0x004921c0
void FUN_004921c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00492380
LEGO_EXPORT void CreateSampleFromWAV(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00492690
LEGO_EXPORT struct Sample *CreatePlayableSample(unsigned int def) { STUB(); }

// FUNCTION: LEGOLAND 0x00492710
LEGO_EXPORT int PlaySample(struct Sample *sample, unsigned int looping, unsigned int oneshot) {
    if (DAT_007988c0 == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }

    if (sample->buffer->vtable->method_0x34(sample->buffer, 0) != 0) {
        return 0;
    }

    if (looping != 0) {
        if (sample->buffer->vtable->method_0x30(sample->buffer, 0, 0, 1) != 0) {
            return 0;
        }
        sample->flags |= 4;
    } else {
        if (sample->buffer->vtable->method_0x30(sample->buffer, 0, 0, 0) != 0) {
            return 0;
        }
        sample->flags &= 0xfffb;
    }

    if (oneshot != 0) {
        sample->flags |= 8;
    } else {
        sample->flags &= 0xfff7;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004927b0
int FUN_004927b0(struct Sample *sample) { STUB(); }

// FUNCTION: LEGOLAND 0x00492800
LEGO_EXPORT void PauseSingleSample(struct Sample *sample) { STUB(); }

// FUNCTION: LEGOLAND 0x00492830
void FUN_00492830(void) {
    struct Sample *sample;

    sample = DAT_007988cc;
    if (sample != 0) {
        do {
            PauseSingleSample(sample);
            sample = sample->next;
        } while (sample != 0);
    }
}

// FUNCTION: LEGOLAND 0x00492850
void FUN_00492850(void) {
    struct Sample *sample;

    sample = DAT_007988cc;
    if (sample != 0) {
        do {
            ResumeSinglyPausedSample(sample);
            sample = sample->next;
        } while (sample != 0);
    }
}

// FUNCTION: LEGOLAND 0x00492870
LEGO_EXPORT void Mute_SFX(void) {
    struct Sample *sample;

    sample = DAT_007988cc;
    if (sample != 0) {
        do {
            FUN_004927b0(sample);
            sample = sample->next;
        } while (sample != 0);
    }
    DAT_007988c4 = 1;
}

// FUNCTION: LEGOLAND 0x004928a0
int FUN_004928a0(struct Sample *sample) { STUB(); }

// FUNCTION: LEGOLAND 0x00492910
LEGO_EXPORT int ResumeSinglyPausedSample(struct Sample *sample) {
    if (DAT_007988c0 != 0 && sample != 0 && (sample->flags & 1) != 0) {
        sample->flags &= 0xfffe;
        if (DAT_007988c4 == 0) {
            FUN_004928a0(sample);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00492950
LEGO_EXPORT void UnMute_FX(void) {
    struct Sample *sample;

    sample = DAT_007988cc;
    while (sample != 0) {
        FUN_004928a0(sample);
        sample = sample->next;
    }
    DAT_007988c4 = 0;
}

// FUNCTION: LEGOLAND 0x00492980
void FUN_00492980(void) {
    DAT_007988c8 = 1;
}

// FUNCTION: LEGOLAND 0x00492990
void FUN_00492990(void) {
    DAT_007988c8 = 0;
}

// FUNCTION: LEGOLAND 0x004929a0
LEGO_EXPORT int SetSampleVolume(struct Sample *sample, int volume) {
    if (DAT_007988c0 == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }

    volume -= 100;
    volume <<= 5;
    return sample->buffer->vtable->method_0x3c(sample->buffer, volume) == 0;
}

// FUNCTION: LEGOLAND 0x004929e0
LEGO_EXPORT int SetSamplePan(struct Sample *sample, int pan) {
    if (DAT_007988c0 == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }

    pan = pan * 5 * 5 * 4;
    return sample->buffer->vtable->method_0x40(sample->buffer, pan) == 0;
}

// FUNCTION: LEGOLAND 0x00492a20
LEGO_EXPORT int SetSampleFrequency(struct Sample *sample, int frequency) {
    if (DAT_007988c0 == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    return sample->buffer->vtable->method_0x44(sample->buffer, frequency) == 0;
}

// FUNCTION: LEGOLAND 0x00492a60
struct Sample *FUN_00492a60(struct Sample *sample) {
    struct SampleBuffer *buffer;

    if (DAT_007988c0 == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }

    buffer = sample->buffer;
    return buffer->vtable->method_0x20(buffer, &sample) == 0 ? sample : 0;
}

// FUNCTION: LEGOLAND 0x00492aa0
LEGO_EXPORT void AdjustPSampleFreq(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00492af0
LEGO_EXPORT int SetSampleFade(struct Sample *sample, unsigned int fade) {
    if (DAT_007988c0 == 0) {
        return 0;
    }
    if (sample == 0) {
        return 0;
    }
    if (sample->active == 0) {
        return 0;
    }
    sample->fade = fade;
    return 1;
}

// FUNCTION: LEGOLAND 0x00492b20
void FUN_00492b20(struct Sample *sample) {
    if (sample != 0) {
        ((struct SampleDefVtbl *)sample->buffer->vtable)->func_8(sample->buffer);
        ((struct SampleCounter *)sample->active)->count--;
        free(sample);
    }
}

// FUNCTION: LEGOLAND 0x00492b50
LEGO_EXPORT void KillPlayableSample(struct Sample *sample) {
    struct Sample *node;

    if (DAT_007988cc == sample) {
        DAT_007988cc = sample->next;
    } else if (DAT_007988cc != 0) {
        node = DAT_007988cc;
        while (node != 0) {
            if (node->next == sample) {
                node->next = sample->next;
                break;
            }
            node = node->next;
        }
    }
    FUN_00492b20(sample);
}

// FUNCTION: LEGOLAND 0x00492b90
LEGO_EXPORT void DeletePlayableSamples(unsigned int param_1) {
    struct Sample *current;
    struct Sample *previous;
    struct Sample *next;

    current = DAT_007988cc;
    previous = 0;
    if (current == 0) {
        return;
    }
    do {
        next = current->next;
        if (param_1 == 0 || current->active == param_1) {
            if (previous != 0) {
                previous->next = next;
            } else {
                DAT_007988cc = next;
            }
            FUN_00492b20(current);
            current = next;
        } else {
            previous = current;
            current = next;
        }
    } while (current != 0);
}

// FUNCTION: LEGOLAND 0x00492be0
LEGO_EXPORT void DeleteSampleDef(struct SampleDef *def) {
    if (def == 0) {
        return;
    }
    DeletePlayableSamples((unsigned int)def);
    free(def->block_30);
    free(def->block_34);
    ((struct SampleDefVtbl *)def->buffer->vtable)->func_8(def->buffer);
    free(def);
}

// FUNCTION: LEGOLAND 0x00492c20
LEGO_EXPORT int KillSoundSampleSystem(void) {
    if (DAT_007988c0 == 0) {
        return 0;
    }
    DeletePlayableSamples(0);
    ((struct DirectSoundObj *)DAT_007cad40)->vtable->Release(DAT_007cad40);
    DAT_007cad40 = 0;
    DAT_007988c0 = 0;
    return 1;
}

// FUNCTION: LEGOLAND 0x00492c60
void FUN_00492c60(void) {
    if (DAT_004bf774 != 0) {
        SuspendThread(DAT_0079a698);
    }
}

// FUNCTION: LEGOLAND 0x00492c80
void FUN_00492c80(void) {
    if (DAT_004bf774 != 0) {
        ResumeThread(DAT_0079a698);
    }
}

// FUNCTION: LEGOLAND 0x00492ca0
void FUN_00492ca0(int param_1) {
    if (DAT_004bf778 == 1 || DAT_004bf778 == 2) {
        DAT_0079a6a4 = 3;
        DAT_0079a6a8 = param_1 % 5;
        SetEvent(DAT_0079a6a0);
    }
}

// FUNCTION: LEGOLAND 0x00492ce0
void FUN_00492ce0(int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x00492d80
BOOL FUN_00492d80(void) {
    DAT_0079a6a4 = 1;
    return SetEvent(DAT_0079a6a0);
}

// FUNCTION: LEGOLAND 0x00492da0
void FUN_00492da0(void) {
    FUN_00492ce0(DAT_0079a6ac);
}

// FUNCTION: LEGOLAND 0x00492db0
void FUN_00492db0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00495a10
int FUN_00495a10(void) {
    if (DAT_004bf774 != 0) {
        DAT_0079a698 = CreateThread(0, 0x4000, (LPTHREAD_START_ROUTINE)FUN_00492db0, 0, 0, (LPDWORD)&DAT_007cad48);
        return 1;
    }
    DAT_007988bc = 1;
    return 0;
}

// FUNCTION: LEGOLAND 0x00495a50
void FUN_00495a50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00495a90
LEGO_EXPORT void UpdateSoundVols(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00495b00
int FUN_00495b00(void) {
    if (DAT_004bf774 != 0 && DMusicInitialised != 0) {
        TerminateThread(DAT_0079a698, 0);

        ((struct DirectMusicLoaderVtbl *)((struct DirectMusicObj *)DAT_007cacd8)->vtable)
            ->ClearCache(DAT_007cacd8, &DAT_004ab8b0);
        ((struct DirectMusicLoaderVtbl *)((struct DirectMusicObj *)DAT_007cacd8)->vtable)->Release(DAT_007cacd8);

        ((struct DirectMusicPerformanceVtbl *)((struct DirectMusicObj *)DAT_007cacdc)->vtable)
            ->Stop(DAT_007cacdc, 0, 0, 0, 0);
        ((struct DirectMusicPerformanceVtbl *)((struct DirectMusicObj *)DAT_007cacdc)->vtable)->CloseDown(DAT_007cacdc);
        ((struct DirectMusicPerformanceVtbl *)((struct DirectMusicObj *)DAT_007cacdc)->vtable)->Release(DAT_007cacdc);

        ((struct DirectMusicComposerVtbl *)((struct DirectMusicObj *)DAT_007cad44)->vtable)->Release(DAT_007cad44);

        DMusicInitialised = 0;
    }
    return 1;
}
