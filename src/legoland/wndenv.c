#include "legoland.h"
#include <windows.h>

#include "globals.h"
#include "crt.h"
#include "resource.h"
#include "wndenv.h"
#include "input.h"
#include "gfx.h"


struct MidiTrack {
    unsigned char pad_0[4];
    unsigned int size;
    void *data;
    unsigned char pad_c[12];
    short field_18;
};

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
LEGO_EXPORT int ProcessSystemEvents(void) {
    MSG msg;
    int peeked;

    WNDENV_Gethwnd();
    peeked = 0;
    do {
        while (PeekMessageA(&msg, (HWND)WNDENV_Gethwnd(), 0, 0, 0) != 0) {
            if (GetMessageA(&msg, (HWND)WNDENV_Gethwnd(), 0, 0) == 0) {
                return 1;
            }
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
            SetCursor(NULL);
        }
        if ((lpConfig->field_1c & 1) != 0) {
            peeked = 1;
            WaitMessage();
        }
    } while ((lpConfig->field_1c & 1) != 0);
    if (peeked) {
        ResendPalette();
    }
    ScanKeyboard();
    ScanMouse();
    UpdateControllerFromMouseData(CONTROLLERBUFFER);
    UpdateControllerFromKeyboardData(CONTROLLERBUFFER);
    return 1;
}

// FUNCTION: LEGOLAND 0x00480150
void FUN_00480150(struct ResFile *file, void *dst) {
    RES_ReadFile(file, dst, 4);
    __asm {
        mov eax, dst
        mov edx, [eax]
        bswap edx
        mov [eax], edx
    }
}

// FUNCTION: LEGOLAND 0x00480170
void FUN_00480170(struct ResFile *file, void *dst) {
    RES_ReadFile(file, dst, 2);
    __asm {
        mov eax, dst
        movzx edx, word ptr [eax]
        xchg dh, dl
        mov word ptr [eax], dx
    }
}

// FUNCTION: LEGOLAND 0x004801a0
void *FUN_004801a0(struct ResFile *file) {
    struct MidiTrack *track;
    unsigned int chunkSize;
    unsigned int header;

    track = (struct MidiTrack *)malloc(0x1c);
    RES_ReadFile(file, &header, 4);
    FUN_00480150(file, &chunkSize);
    track->size = chunkSize;
    track->data = malloc(chunkSize);
    RES_ReadFile(file, track->data, chunkSize);
    track->field_18 = 0;
    return track;
}
