#pragma once

#include <windows.h>
#include "legoland.h"

LEGO_EXPORT HWND WNDENV_Gethwnd(void);
BOOL MinimizeGameWindow(void);
void RestoreGameWindow(void);
LEGO_EXPORT void *WNDENV_GethInstance(void);
LEGO_EXPORT LRESULT CALLBACK LegoLandWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LEGO_EXPORT int ProcessSystemEvents(void);

struct ResFile;
void ReadBigEndianU32(struct ResFile *file, void *out);
void ReadBigEndianU16(struct ResFile *file, void *out);
void *ReadMidiTrack(struct ResFile *file);
LEGO_EXPORT void WNDENV_Sethwnd(HWND param_1);
