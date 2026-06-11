#include "legoland.h"
#include <windows.h>
#include <dinput.h>

#include "icon.h"
#include "llidb.h"
#include "globals.h"
#include "controller.h"
#include "input.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "debug_alloc.h"
#include "bricks.h"
#include "screens.h"
#include "nerps.h"
#include "map_object.h"
#include "wndenv.h"
#include "sound_sfx.h"

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
LEGO_EXPORT void UpdateControllerFromKeyboardData(struct CtrlBuffer *buffer) {
    char c;
    int arg;

    buffer->field_18 = buffer->field_18 & 0xfffff807;
    c = FUN_00474130();
    if (c == 0) {
        goto flags;
    }
    memcpy(DAT_00668d94, DAT_00668d94 + 1, 19);
    DAT_00668d94[0x13] = c;
    if (_memicmp(":THEME", &DAT_00668d94[0xe], 6) == 0) {
        FUN_00492ce0(0);
        // STRING: LEGOLAND 0x004baf88
        DBPrintf("CHEATTHEME=THEME\n");
    } else if (_memicmp(":EGYPT", &DAT_00668d94[0xe], 6) == 0) {
        FUN_00492ce0(1);
        // STRING: LEGOLAND 0x004baf58
        DBPrintf("CHEATTHEME=EGYPTIAN\n");
    } else if (_memicmp(":INCA", &DAT_00668d94[0xf], 5) == 0) {
        FUN_00492ce0(2);
        // STRING: LEGOLAND 0x004baf3c
        DBPrintf("CHEATTHEME=INCA\n");
    } else if (_memicmp(":CASTLE", &DAT_00668d94[0xd], 7) == 0) {
        FUN_00492ce0(3);
        // STRING: LEGOLAND 0x004baf20
        DBPrintf("CHEATTHEME=CASTLE\n");
    } else if (_memicmp(":WEST", &DAT_00668d94[0xf], 5) == 0) {
        FUN_00492ce0(4);
        // STRING: LEGOLAND 0x004baf04
        DBPrintf("CHEATTHEME=WEST\n");
    } else if (_memicmp(":STOP", &DAT_00668d94[0xf], 5) == 0) {
        FUN_00492d80();
        // STRING: LEGOLAND 0x004baee8
        DBPrintf("CHEAT:STOPMUSIC\n");
    } else if (_memicmp("::DIE", &DAT_00668d94[0xf], 5) == 0) {
        _exit(1);
    }
    if (DAT_008119b4 == 3) {
        if (_memicmp(":ILIKETOTRAVEL", &DAT_00668d94[4], 0xe) == 0) {
            if (DAT_00668d94[0x12] == '1') {
                if (DAT_00668d94[0x13] != '0') {
                    goto flags;
                }
                arg = 0xa;
                lpConfig->field_28 = 0xf;
            } else if (DAT_00668d94[0x12] == '0') {
                if (DAT_00668d94[0x13] < '1' || DAT_00668d94[0x13] > '9') {
                    goto flags;
                }
                lpConfig->field_28 = DAT_00668d94[0x13] - 0x2b;
                arg = lpConfig->field_28;
            } else {
                if (DAT_00668d94[0x12] != 'T' || DAT_00668d94[0x13] < '1' || DAT_00668d94[0x13] > '5') {
                    goto flags;
                }
                lpConfig->field_28 = DAT_00668d94[0x13] - 0x30;
                arg = lpConfig->field_28;
            }
            // STRING: LEGOLAND 0x004baed0
            DBPrintf("CHEAT:Level %d\n", arg);
            DAT_00832ba0 = 2;
        } else if (_memicmp(":COLDHARDCASH", &DAT_00668d94[7], 0xd) == 0) {
            // STRING: LEGOLAND 0x004baeac
            DBPrintf("CHEAT:More Money\n");
            AddBricks(5000);
        } else if (_memicmp(":HARDASNAILS", &DAT_00668d94[8], 0xc) == 0) {
            // STRING: LEGOLAND 0x004bae88
            DBPrintf("CHEAT:No Ride Wear\n");
            DAT_00832980 = 0;
        } else if (_memicmp(":PRAISEME", &DAT_00668d94[0xb], 9) == 0) {
            // STRING: LEGOLAND 0x004bae60
            DBPrintf("CHEAT:Instant Appraisal\n");
            DAT_00666098 = 1;
        } else if (_memicmp(":WELOVELEGOLAND", &DAT_00668d94[5], 0xf) == 0) {
            // STRING: LEGOLAND 0x004bae3c
            DBPrintf("CHEAT:Win Level\n", lpConfig->field_28);
            FUN_00459820(1);
        } else if (_memicmp(":IMPROVISE", &DAT_00668d94[9], 9) == 0) {
            // STRING: LEGOLAND 0x004bae1c
            DBPrintf("CHEAT:Stop Script\n", lpConfig->field_28);
            FUN_0046b240(1);
        } else if (_memicmp(":DIGGER", &DAT_00668d94[0xd], 7) == 0) {
            // STRING: LEGOLAND 0x004bae00
            DBPrintf("CHEAT:Set Switch 1\n");
            FUN_00460560(0);
            FUN_00460560(1);
            FUN_00460560(2);
            FUN_00460560(3);
        } else if (_memicmp(":SHOWCAPACITY", &DAT_00668d94[7], 0xd) == 0) {
            DAT_00832994 = 1;
            // STRING: LEGOLAND 0x004badd0
            DBPrintf("CHEAT: Capacity Calcs visible\n");
        }
    }
flags:
    if ((DAT_007fdda0[0xcb] & 0x80) != 0) {
        buffer->field_18 = buffer->field_18 | 8;
    }
    if ((DAT_007fdda0[0xcd] & 0x80) != 0) {
        buffer->field_18 = buffer->field_18 | 0x10;
    }
    if ((DAT_007fdda0[0xc8] & 0x80) != 0) {
        buffer->field_18 = buffer->field_18 | 0x20;
    }
    if ((DAT_007fdda0[0xd0] & 0x80) != 0) {
        buffer->field_18 = buffer->field_18 | 0x40;
    }
    if ((DAT_007fdda0[0x39] & 0x80) != 0) {
        buffer->field_18 = buffer->field_18 | 0x80;
    }
    if ((DAT_007fdda0[0xf] & 0x80) != 0) {
        buffer->field_18 = buffer->field_18 | 0x100;
    }
    if ((DAT_007fdda0[1] & 0x80) != 0) {
        buffer->field_18 = buffer->field_18 | 0x200;
    }
    if ((DAT_007fdda0[0x1c] & 0x80) != 0) {
        buffer->field_18 = buffer->field_18 | 0x400;
    }
}

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
    unsigned int prev;
    unsigned char state;
    int alt;

    result = 0;
    i = 0;
    do {
        prev = DAT_00668da8[i];
        state = DAT_007fdda0[DAT_004bad58[i].flags];
        prev = (prev >> 7) & 1;
        DAT_00668da8[i] = state;
        if ((state & 0x80) != 0 && prev == 0) {
            alt = GetKeyState(0x14) & 1;
            if (FUN_00474070() != 0 || FUN_00474080() != 0) {
                alt = (alt == 0);
            }
            result = DAT_004bad58[i].code;
            if (alt == 0) {
                result = tolower(result);
            }
        }
        i = i + 1;
    } while (i < 0x3b);
    return (char)result;
}

// FUNCTION: LEGOLAND 0x00474130
char FUN_00474130(void) {
    int i;
    int result;
    unsigned char state;
    unsigned int prev;

    result = 0;
    i = 0;
    do {
        prev = DAT_00668de4[i];
        state = DAT_007fdda0[DAT_004bad58[i].flags];
        prev = (prev >> 7) & 1;
        DAT_00668de4[i] = state;
        if ((state & 0x80) != 0 && prev == 0) {
            result = DAT_004bad58[i].code;
        }
        i = i + 1;
    } while (i < 0x3b);
    if (result == -10) {
        result = 0x3a;
    }
    return (char)result;
}

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
