#pragma once

struct KLIBAUDIO_Object;

struct KLIBAUDIO_Vtbl {
    void *pad_0[12];
    void(__stdcall *func_30)(void *self, int a1, int a2, int a3);
    void *pad_34[2];
    void(__stdcall *func_3c)(void *self, unsigned int a1);
};

struct KLIBAUDIO_Stop {
    void *pad_0[18];
    void(__stdcall *func_48)(struct KLIBAUDIO_Object *self);
};

struct KLIBAUDIO_Buf {
    void *pad_0[4];
    int(__stdcall *GetCurrentPosition)(void *self, int *play, unsigned int *write);
    void *pad_14[6];
    int(__stdcall *Lock)(void *self, unsigned int off, unsigned int bytes, void **p1, unsigned int *s1, void **p2, unsigned int *s2, unsigned int flags);
    void *pad_30[1];
    void(__stdcall *SetPos)(void *self, unsigned int pos);
    void *pad_38[4];
    void(__stdcall *Stop)(void *self);
    void(__stdcall *Unlock)(void *self, void *p1, unsigned int s1, void *p2, unsigned int s2);
};

struct KLIBAUDIO_Object {
    void *vtable;
};

int SpeechFillSoundBuffer(void);
void RewindSpeechStream(void);
void SpeechRewindToData(void);
int SpeechLoadWavFile(const char *param_1);
void SpeechResetBuffers(void);
int SpeechStop(void);
int SpeechCloseFile(void);
int SpeechPlay(void);
int SpeechStreamUpdate(void);
int SpeechIsPlaying(void);
void SpeechSetVolume(unsigned int param_1);
