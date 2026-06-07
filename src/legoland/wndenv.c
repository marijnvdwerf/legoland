#include "legoland.h"
#include <windows.h>

#include "globals.h"

// FUNCTION: LEGOLAND 0x0047fe40
LEGO_EXPORT void *WNDENV_GethInstance(void) { return g_hInstance; }

// FUNCTION: LEGOLAND 0x0047fe50
LEGO_EXPORT void WNDENV_Sethwnd(void *param_1) { DAT_00669210 = param_1; }

// FUNCTION: LEGOLAND 0x0047fe60
LEGO_EXPORT void *WNDENV_Gethwnd(void) { return DAT_00669210; }

// FUNCTION: LEGOLAND 0x0047fe70
BOOL FUN_0047fe70(void) { return ShowWindow((HWND)WNDENV_Gethwnd(), 6); }

// FUNCTION: LEGOLAND 0x0047fe80
void FUN_0047fe80(void) { ShowWindow((HWND)WNDENV_Gethwnd(), 9); }

// FUNCTION: LEGOLAND 0x0047fe90
LEGO_EXPORT LRESULT CALLBACK LegoLandWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { STUB(); }

// FUNCTION: LEGOLAND 0x00480050
LEGO_EXPORT void ProcessSystemEvents(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00480150
void FUN_00480150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00480170
void FUN_00480170(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004801a0
void FUN_004801a0(void) { STUB(); }
