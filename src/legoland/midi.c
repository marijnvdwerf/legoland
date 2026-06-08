#include "legoland.h"
#include <windows.h>
#include <mmsystem.h>

#include "globals.h"
#include "crt.h"
#include "resource.h"
#include "wndenv.h"

struct MidiFile {
    unsigned int field_0;
    unsigned int field_4;
    unsigned char pad_8[4];
    short trackCount;
    unsigned char pad_e[2];
    void **trackArray;
    unsigned char pad_14[4];
};

struct MidiTrack {
    struct MidiFile *parent;
    unsigned char pad_4[4];
    unsigned char *data;
    int pos;
    unsigned int status;
    unsigned char pad_14[10];
};

// FUNCTION: LEGOLAND 0x00480200
LEGO_EXPORT struct MidiFile *LoadMIDIFile(const char *filename) {
    struct MidiFile *midi;
    struct ResFile *file;
    unsigned int header;
    unsigned int division;
    unsigned int chunkSize;
    unsigned short formatType;
    int i;

    header = 0;
    midi = (struct MidiFile *)malloc(0x18);
    file = RES_OpenFile(filename);
    RES_ReadFile(file, &header, 4);
    FUN_00480150(file, &chunkSize);
    FUN_00480170(file, &formatType);
    FUN_00480170(file, &midi->trackCount);
    FUN_00480170(file, &division);
    midi->field_0 = division * 5 * 5 * 5 * 5 * 32;
    midi->trackArray = (void **)malloc(midi->trackCount * 4);
    for (i = 0; i < midi->trackCount; ++i) {
        midi->trackArray[i] = FUN_004801a0(file);
        ((struct MidiTrack *)midi->trackArray[i])->parent = midi;
    }
    midi->field_4 = 0x100;
    RES_CloseFile(file);
    return midi;
}

// FUNCTION: LEGOLAND 0x004802c0
int FUN_004802c0(struct MidiTrack *track) {
    unsigned int value;
    unsigned int b;

    value = 0;
    b = 0;
    do {
        value <<= 7;
        b = track->data[track->pos];
        ++track->pos;
        value |= b & 0x7f;
    } while (b & 0x80);
    return value;
}

// FUNCTION: LEGOLAND 0x004802f0
unsigned int FUN_004802f0(struct MidiTrack *track) {
    unsigned int b;

    b = track->data[track->pos];
    ++track->pos;
    if ((b & 0x80) == 0) {
        --track->pos;
        return track->status;
    }
    if (b == 0xff) {
        unsigned int meta = track->data[track->pos] | 0xff00;
        ++track->pos;
        return meta;
    }
    track->status = b;
    return b;
}

// FUNCTION: LEGOLAND 0x00480330
void FUN_00480330(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00480570
void __stdcall FUN_00480570(unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4, unsigned int p5) { STUB(); }

// FUNCTION: LEGOLAND 0x004805d0
LEGO_EXPORT void PlayMIDI(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00480630
LEGO_EXPORT int InitMIDIManager(void) {
    DAT_007fd634 = 0;
    DAT_007fd630 = timeSetEvent(0x14, 0xa, (LPTIMECALLBACK)FUN_00480570, 0, 1);
    midiOutOpen((LPHMIDIOUT)&DAT_007fd638, (UINT)-1, 0, 0, 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x00480670
LEGO_EXPORT void KillMIDIManager(void) {
    DAT_007fd634 = 0;
    timeKillEvent(DAT_007fd630);
    midiOutClose((HMIDIOUT)DAT_007fd638);
}
