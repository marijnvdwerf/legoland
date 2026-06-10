#include "legoland.h"
#include <windows.h>
#include <dinput.h>

#include "icon.h"
#include "llidb.h"
#include "globals.h"
#include "controller.h"
#include "input.h"
#include "crt.h"

LEGO_EXPORT void *WNDENV_Gethwnd(void);
int FUN_004738b0(void);
int FUN_00473970(void);

struct DInputDeviceVtbl {
    void *pad_0[2];
    void(__stdcall *Release)(void *self);
    void *pad_c[5];
    void(__stdcall *Unacquire)(void *self);
};

struct DInputDevice {
    struct DInputDeviceVtbl *vtable;
};

// FUNCTION: LEGOLAND 0x00473870
LEGO_EXPORT int InitInputSystem(void) {
    HINSTANCE hinst;

    hinst = (HINSTANCE)GetWindowLongA((HWND)WNDENV_Gethwnd(), GWL_HINSTANCE);
    DirectInputCreateA(hinst, 0x300, (LPDIRECTINPUTA *)&dinput, NULL);
    if (FUN_004738b0() == 0) {
        return 0;
    }
    return FUN_00473970() != 0;
}

// FUNCTION: LEGOLAND 0x004738b0
int FUN_004738b0(void) {
    DIDEVCAPS caps;

    if (IDirectInput_CreateDevice((LPDIRECTINPUTA)dinput, &GUID_SysKeyboard, (LPDIRECTINPUTDEVICEA *)&dinput_keyboard, NULL) == 0) {
        IDirectInputDevice_SetDataFormat((LPDIRECTINPUTDEVICEA)dinput_keyboard, &c_dfDIKeyboard);
        caps.dwSize = 0x2c;
        IDirectInputDevice_GetCapabilities((LPDIRECTINPUTDEVICEA)dinput_keyboard, &caps);
        if (caps.dwFlags != 0) {
            return IDirectInput_GetDeviceStatus((LPDIRECTINPUTA)dinput, &GUID_SysKeyboard) == 0;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00473930
LEGO_EXPORT void ScanKeyboard(void) {
    HRESULT hr;

    if (dinput_keyboard == NULL) {
        return;
    }
    while (1) {
        hr = IDirectInputDevice_GetDeviceState((IDirectInputDeviceA *)dinput_keyboard, 0x100, DAT_007fdda0);
        if (hr == DI_OK) {
            return;
        }
        if (hr == 0x8007000c || hr == 0x8007001e) {
            IDirectInputDevice_Acquire((IDirectInputDeviceA *)dinput_keyboard);
        }
    }
}

// FUNCTION: LEGOLAND 0x00473970
int FUN_00473970(void) {
    DIPROPDWORD prop;
    DIDEVCAPS caps;

    if (IDirectInput_CreateDevice((LPDIRECTINPUTA)dinput, &GUID_SysMouse, (LPDIRECTINPUTDEVICEA *)&dintput_mouse, NULL) == 0) {
        IDirectInputDevice_SetCooperativeLevel((LPDIRECTINPUTDEVICEA)dintput_mouse, (HWND)WNDENV_Gethwnd(), 5);
        IDirectInputDevice_SetDataFormat((LPDIRECTINPUTDEVICEA)dintput_mouse, &c_dfDIMouse);
        prop.diph.dwSize = 0x14;
        prop.diph.dwHeaderSize = 0x10;
        prop.diph.dwObj = 8;
        prop.diph.dwHow = 1;
        if (IDirectInputDevice_GetProperty((LPDIRECTINPUTDEVICEA)dintput_mouse, (REFGUID)3, &prop.diph) == 0) {
            mouse_granularity = prop.dwData;
        }
        caps.dwSize = 0x2c;
        IDirectInputDevice_GetCapabilities((LPDIRECTINPUTDEVICEA)dintput_mouse, &caps);
        if (caps.dwFlags != 0) {
            return IDirectInput_GetDeviceStatus((LPDIRECTINPUTA)dinput, &GUID_SysMouse) == 0;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00473a50
void FUN_00473a50(void) {
    if (dinput_keyboard != NULL) {
        IDirectInputDevice_Release((LPDIRECTINPUTDEVICEA)dinput_keyboard);
    }
}

// FUNCTION: LEGOLAND 0x00473a60
void FUN_00473a60(void) {
    if (dintput_mouse != NULL) {
        ((struct DInputDevice *)dintput_mouse)->vtable->Unacquire(dintput_mouse);
        ((struct DInputDevice *)dintput_mouse)->vtable->Release(dintput_mouse);
    }
}

// FUNCTION: LEGOLAND 0x00473a80
LEGO_EXPORT void ScanMouse(void) {
    HRESULT hr;

    if (dintput_mouse != NULL) {
        while (1) {
            hr = IDirectInputDevice_GetDeviceState((IDirectInputDeviceA *)dintput_mouse, 0x10, &DAT_00668d78);
            if (hr == DI_OK) {
                break;
            }
            if (hr == 0x8007000c || hr == 0x8007001e) {
                IDirectInputDevice_Acquire((IDirectInputDeviceA *)dintput_mouse);
            }
        }
    }
    if (DAT_00668d78.lZ <= -mouse_granularity) {
        FUN_0046db40();
    } else if (DAT_00668d78.lZ >= mouse_granularity) {
        FUN_0046dac0();
    }
}

// FUNCTION: LEGOLAND 0x00473ae0
LEGO_EXPORT void KillInputSystem(void) {
    FUN_00473a50();
    FUN_00473a60();
    if (dinput != NULL) {
        ((struct DInputDevice *)dinput)->vtable->Release(dinput);
    }
}

// FUNCTION: LEGOLAND 0x00473b00
LEGO_EXPORT void UpdateControllerFromMouseData(struct CtrlBuffer *buffer) {
    int x;
    int y;
    int dx;
    int dy;
    int lX;
    int lY;
    unsigned int flags;

    if (buffer == NULL) {
        return;
    }
    buffer->field_0 = buffer->field_8;
    buffer->field_4 = buffer->field_c;
    lX = DAT_00668d78.lX;
    lY = DAT_00668d78.lY;
    dx = lX;
    dy = lY;
    if (buffer->field_24 != 0) {
        if (abs(lX) > buffer->field_1c || abs(lY) > buffer->field_1c) {
            dx = lX * 2;
            dy = lY * 2;
        }
        if (buffer->field_24 == 2 && (abs(dx) > buffer->field_20 || abs(dy) > buffer->field_20)) {
            dx = dx << 1;
            dy = dy << 1;
        }
    }
    x = buffer->field_8;
    y = buffer->field_c;
    buffer->field_8 = x + dx;
    buffer->field_c = y + dy;
    if ((int)(unsigned int)(unsigned short)lpConfig->field_0 <= x + dx) {
        buffer->field_8 = lpConfig->field_0 - 1;
    }
    if (buffer->field_8 < 0) {
        buffer->field_8 = 0;
    }
    if ((int)(unsigned int)(unsigned short)lpConfig->field_2 <= y + dy) {
        buffer->field_c = lpConfig->field_2 - 1;
    }
    if (buffer->field_c < 0) {
        buffer->field_c = 0;
    }
    buffer->field_10 = buffer->field_8 - buffer->field_0;
    flags = buffer->field_18 & 0xfffffff8;
    buffer->field_14 = buffer->field_c - buffer->field_4;
    buffer->field_18 = flags;
    if ((DAT_00668d78.rgbButtons[0] & 0x80) != 0) {
        buffer->field_18 = flags | 1;
    }
    if ((DAT_00668d78.rgbButtons[2] & 0x80) != 0) {
        buffer->field_18 = buffer->field_18 | 4;
    }
    if ((DAT_00668d78.rgbButtons[1] & 0x80) != 0) {
        buffer->field_18 = buffer->field_18 | 2;
    }
}

// FUNCTION: LEGOLAND 0x00473c10
LEGO_EXPORT void UpdateControllerFromKeyboardData(struct CtrlBuffer *buffer) { STUB(); }

// FUNCTION: LEGOLAND 0x00474070
unsigned int FUN_00474070(void) {
    return DAT_007fdda0[0x2a] >> 7;
}

// FUNCTION: LEGOLAND 0x00474080
unsigned int FUN_00474080(void) {
    return DAT_007fdda0[0x36] >> 7;
}

// FUNCTION: LEGOLAND 0x004740b0
LEGO_EXPORT char GetInputChar(void) {
    int i;
    int result;
    signed char *p;
    unsigned int prev;
    unsigned char state;
    int alt;

    result = 0;
    i = 0;
    p = &DAT_004bad58[0].code;
    do {
        prev = DAT_00668da8[i];
        state = DAT_007fdda0[(unsigned char)p[-1]];
        prev = (prev >> 7) & 1;
        DAT_00668da8[i] = state;
        if ((state & 0x80) != 0 && prev == 0) {
            alt = GetKeyState(0x14) & 1;
            if (FUN_00474070() != 0 || FUN_00474080() != 0) {
                alt = (alt == 0);
            }
            result = *p;
            if (alt == 0) {
                result = tolower(result);
            }
        }
        i = i + 1;
        p = p + 2;
    } while ((int)p < 0x4badcf);
    return (char)result;
}

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
