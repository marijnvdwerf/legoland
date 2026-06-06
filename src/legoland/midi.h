#pragma once

// Per-TU header for midi.c — MIDI playback manager.

void LoadMIDIFile(void);
void FUN_004802c0(void);
void FUN_004802f0(void);
void FUN_00480330(void);
void __stdcall FUN_00480570(unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4, unsigned int p5);
void PlayMIDI(void);
int InitMIDIManager(void);
void KillMIDIManager(void);
