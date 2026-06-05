#include "legoland.h"
#include <windows.h>
#include <dinput.h>

struct DInputDeviceVtbl {
    void *pad_0[2];
    void(__stdcall *Release)(void *self);
    void *pad_c[5];
    void(__stdcall *Unacquire)(void *self);
};

struct DInputDevice {
    struct DInputDeviceVtbl *vtable;
};

extern IDirectInputDeviceA *dinput_keyboard;
extern IDirectInputDeviceA *dintput_mouse;
extern struct DInputDevice *dinput;
extern unsigned char DAT_007fdda0[256];
extern unsigned char DAT_00668d78[16];
extern int mouse_granularity;
extern int DAT_00668d80;
extern unsigned char DAT_007fddca;
extern unsigned char DAT_007fddd6;
extern unsigned int DAT_004baff8;
extern unsigned int DAT_00668e34;
extern unsigned char DAT_007fdd80;

extern void FUN_00473a50(void);
extern void FUN_0046db40(void);
extern void FUN_0046dac0(void);
extern void SaveGameWrite(void *data, unsigned int size);
extern void SaveGameRead(void *data, unsigned int size);

// FUNCTION: LEGOLAND 0x00473870
void InitInputSystem(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004738b0
void FUN_004738b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473930
void ScanKeyboard(void) {
    HRESULT hr;

    if (dinput_keyboard == NULL) {
        return;
    }
    while (1) {
        hr = IDirectInputDevice_GetDeviceState(dinput_keyboard, 0x100, DAT_007fdda0);
        if (hr == DI_OK) {
            return;
        }
        if (hr == 0x8007000c || hr == 0x8007001e) {
            IDirectInputDevice_Acquire(dinput_keyboard);
        }
    }
}

// FUNCTION: LEGOLAND 0x00473970
void FUN_00473970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473a50
void FUN_00473a50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473a60
void FUN_00473a60(void) {
    if (dintput_mouse != NULL) {
        ((struct DInputDevice *)dintput_mouse)->vtable->Unacquire(dintput_mouse);
        ((struct DInputDevice *)dintput_mouse)->vtable->Release(dintput_mouse);
    }
}

// FUNCTION: LEGOLAND 0x00473a80
void ScanMouse(void) {
    HRESULT hr;

    if (dintput_mouse != NULL) {
        while (1) {
            hr = IDirectInputDevice_GetDeviceState(dintput_mouse, 0x10, DAT_00668d78);
            if (hr == DI_OK) {
                break;
            }
            if (hr == 0x8007000c || hr == 0x8007001e) {
                IDirectInputDevice_Acquire(dintput_mouse);
            }
        }
    }
    if (DAT_00668d80 <= -mouse_granularity) {
        FUN_0046db40();
    } else if (DAT_00668d80 >= mouse_granularity) {
        FUN_0046dac0();
    }
}

// FUNCTION: LEGOLAND 0x00473ae0
void KillInputSystem(void) {
    FUN_00473a50();
    FUN_00473a60();
    if (dinput != NULL) {
        dinput->vtable->Release(dinput);
    }
}

// FUNCTION: LEGOLAND 0x00473b00
void UpdateControllerFromMouseData(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473c10
void UpdateControllerFromKeyboardData(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00474070
unsigned int FUN_00474070(void) {
    return DAT_007fddca >> 7;
}

// FUNCTION: LEGOLAND 0x00474080
unsigned int FUN_00474080(void) {
    return DAT_007fddd6 >> 7;
}

// FUNCTION: LEGOLAND 0x004740b0
void GetInputChar(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00474130
void FUN_00474130(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00474190
void FUN_00474190(void) {
    SaveGameWrite(&DAT_004baff8, 4);
    SaveGameWrite(&DAT_00668e34, 4);
    SaveGameWrite(&DAT_007fdd80, 16);
}

// FUNCTION: LEGOLAND 0x004741c0
void FUN_004741c0(void) {
    SaveGameRead(&DAT_004baff8, 4);
    SaveGameRead(&DAT_00668e34, 4);
    SaveGameRead(&DAT_007fdd80, 16);
}
