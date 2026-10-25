#include <windows.h>
#include <mmsystem.h>
#include "legoland.h"

#include <stdlib.h>
#include "globals.h"
#include "resource.h"
#include "wndenv.h"

struct MidiFile {
    unsigned int field_0;
    unsigned int time_step;
    int song_time;
    short trackCount;
    unsigned char pad_e[2];
    void **trackArray;
    short playing;
    unsigned char pad_16[2];
};

struct MidiTrack {
    struct MidiFile *parent;
    int length;
    unsigned char *data;
    int pos;
    unsigned int status;
    int field_14;
    short field_18;
    short field_1a;
    unsigned char pad_1c[2];
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

    division = 0;
    midi = (struct MidiFile *)malloc(sizeof(struct MidiFile));
    file = RES_OpenFile(filename);
    RES_ReadFile(file, &header, 4);
    ReadBigEndianU32(file, &chunkSize);
    ReadBigEndianU16(file, &formatType);
    ReadBigEndianU16(file, &midi->trackCount);
    ReadBigEndianU16(file, &division);
    midi->field_0 = division * 5 * 5 * 5 * 5 * 32;
    midi->trackArray = (void **)malloc(midi->trackCount * sizeof(void *));
    for (i = 0; i < midi->trackCount; ++i) {
        midi->trackArray[i] = ReadMidiTrack(file);
        ((struct MidiTrack *)midi->trackArray[i])->parent = midi;
    }
    midi->time_step = 0x100;
    RES_CloseFile(file);
    return midi;
}

// FUNCTION: LEGOLAND 0x004802c0
int ReadMidiVarLen(struct MidiTrack *track) {
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
unsigned int ReadMidiEventStatus(struct MidiTrack *track) {
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

/* Processes MIDI events due at the song time, sends supported channel messages and applies tempo changes. Returns
 * zero when the track is inactive or finished, one while waiting for a later event. */
// FUNCTION: LEGOLAND 0x00480330
unsigned int ProcessMidiTrackEvents(struct MidiTrack *track) {
    int ev;
    unsigned int len;

    if (track->pos >= track->length || track->field_18 != 1) {
        return 0;
    }
    for (;;) {
        if (track->field_1a != 0) {
            track->field_14 += ReadMidiVarLen(track);
            track->field_1a = 0;
        }
        if ((unsigned int)track->parent->song_time >> 8 < (unsigned int)track->field_14) {
            return 1;
        }
        track->field_1a = 1;
        ev = ReadMidiEventStatus(track);
        switch (ev) {
        case 0xff2f:
            track->field_18 = 0;
            track->pos++;
            return 0;
        case 0xf0:
        case 0xf7:
            len = track->data[track->pos++];
            while (len-- != 0) {
                track->pos++;
            }
            break;
        case 0xff51: {
            unsigned int hi;
            track->pos++;
            hi = track->data[track->pos++];
            hi = (hi << 8) | track->data[track->pos];
            track->pos += 2;
            track->parent->time_step = track->parent->field_0 / hi;
            break;
        }
        default:
            switch (ev & 0xf0) {
            case 0x80:
            case 0x90:
            case 0xb0:
            case 0xe0:
                ev |= track->data[track->pos] << 8;
                track->pos++;
                ev |= track->data[track->pos] << 16;
                track->pos++;
                midiOutShortMsg(MidiOutHandle, ev);
                break;
            case 0xa0:
                track->pos += 2;
                break;
            case 0xc0:
                ev |= track->data[track->pos] << 8;
                track->pos++;
                midiOutShortMsg(MidiOutHandle, ev);
                break;
            case 0xd0:
                track->pos++;
                break;
            default:
                if ((ev & 0xff00) == 0xff00) {
                    len = track->data[track->pos++];
                    while (len-- != 0) {
                        track->pos++;
                    }
                }
                break;
            }
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x00480570
void __stdcall MidiTimerCallback(unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4, unsigned int p5) {
    struct MidiFile *midi = (struct MidiFile *)CurrentMidiFile;
    unsigned int busy = 0;
    int i;

    if (midi != NULL && midi->playing == 1) {
        midi->song_time += midi->time_step;
        for (i = 0; i < midi->trackCount; i++) {
            busy |= ProcessMidiTrackEvents((struct MidiTrack *)midi->trackArray[i]);
        }
        if (busy == 0) {
            midi->playing = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x004805d0
LEGO_EXPORT void PlayMIDI(struct MidiFile *midi) {
    int i;

    CurrentMidiFile = midi;
    midi->song_time = 0;
    midi->playing = 1;
    for (i = 0; i < midi->trackCount;) {
        i++;
        ((struct MidiTrack *)midi->trackArray[i - 1])->field_14 = 0;
        ((struct MidiTrack *)midi->trackArray[i - 1])->field_18 = 1;
        ((struct MidiTrack *)midi->trackArray[i - 1])->pos = 0;
        ((struct MidiTrack *)midi->trackArray[i - 1])->field_1a = 1;
    }
}

// FUNCTION: LEGOLAND 0x00480630
LEGO_EXPORT int InitMIDIManager(void) {
    CurrentMidiFile = 0;
    MidiTimerId = timeSetEvent(0x14, 0xa, (LPTIMECALLBACK)MidiTimerCallback, 0, 1);
    midiOutOpen(&MidiOutHandle, (UINT)-1, 0, 0, 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x00480670
LEGO_EXPORT void KillMIDIManager(void) {
    CurrentMidiFile = 0;
    timeKillEvent(MidiTimerId);
    midiOutClose(MidiOutHandle);
}
