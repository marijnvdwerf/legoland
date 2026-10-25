#include <windows.h>
#include <dinput.h>
#include "legoland.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "bricks.h"
#include "controller.h"
#include "debug_alloc.h"
#include "globals.h"
#include "icon.h"
#include "input.h"
#include "llidb.h"
#include "map_object.h"
#include "nerps.h"
#include "screens.h"
#include "sound_sfx.h"
#include "wndenv.h"

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

    hinst = (HINSTANCE)GetWindowLongA(WNDENV_Gethwnd(), GWL_HINSTANCE);
    DirectInputCreateA(hinst, 0x300, (LPDIRECTINPUTA *)&dinput, NULL);
    if (CreateKeyboardDevice() == 0) {
        return 0;
    }
    return CreateMouseDevice() != 0;
}

// FUNCTION: LEGOLAND 0x004738b0
int CreateKeyboardDevice(void) {
    DIDEVCAPS caps;

    if (IDirectInput_CreateDevice((LPDIRECTINPUTA)dinput, &GUID_SysKeyboard, (LPDIRECTINPUTDEVICEA *)&dinput_keyboard, NULL) == 0) {
        IDirectInputDevice_SetDataFormat((LPDIRECTINPUTDEVICEA)dinput_keyboard, &c_dfDIKeyboard);
        caps.dwSize = 0x2c;
        IDirectInputDevice_GetCapabilities((LPDIRECTINPUTDEVICEA)dinput_keyboard, &caps);
        if (!((caps.dwFlags == 0) & 1)) {
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
        hr = IDirectInputDevice_GetDeviceState((IDirectInputDeviceA *)dinput_keyboard, 0x100, KeyboardState);
        if (hr == DI_OK) {
            return;
        }
        if (hr == 0x8007000c || hr == 0x8007001e) {
            IDirectInputDevice_Acquire((IDirectInputDeviceA *)dinput_keyboard);
        }
    }
}

// FUNCTION: LEGOLAND 0x00473970
int CreateMouseDevice(void) {
    DIPROPDWORD prop;
    DIDEVCAPS caps;

    if (IDirectInput_CreateDevice((LPDIRECTINPUTA)dinput, &GUID_SysMouse, (LPDIRECTINPUTDEVICEA *)&dintput_mouse, NULL) == 0) {
        IDirectInputDevice_SetCooperativeLevel((LPDIRECTINPUTDEVICEA)dintput_mouse, WNDENV_Gethwnd(), 5);
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
        if (!((caps.dwFlags == 0) & 1)) {
            return IDirectInput_GetDeviceStatus((LPDIRECTINPUTA)dinput, &GUID_SysMouse) == 0;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00473a50
void ReleaseKeyboardDevice(void) {
    if (dinput_keyboard != NULL) {
        IDirectInputDevice_Release((LPDIRECTINPUTDEVICEA)dinput_keyboard);
    }
}

// FUNCTION: LEGOLAND 0x00473a60
void ReleaseMouseDevice(void) {
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
            hr = IDirectInputDevice_GetDeviceState((IDirectInputDeviceA *)dintput_mouse, 0x10, &MouseState);
            if (hr == DI_OK) {
                break;
            }
            if (hr == 0x8007000c || hr == 0x8007001e) {
                IDirectInputDevice_Acquire((IDirectInputDeviceA *)dintput_mouse);
            }
        }
    }
    if (MouseState.lZ <= -mouse_granularity) {
        ScrollActiveIconRegionBack();
    } else if (MouseState.lZ >= mouse_granularity) {
        ScrollActiveIconRegionForward();
    }
}

// FUNCTION: LEGOLAND 0x00473ae0
LEGO_EXPORT void KillInputSystem(void) {
    ReleaseKeyboardDevice();
    ReleaseMouseDevice();
    if (dinput != NULL) {
        ((struct DInputDevice *)dinput)->vtable->Release(dinput);
    }
}

// Updates the mouse controller from the DirectInput state: remembers the previous position, adds
// the movement (doubled once past mouse_threshold1, and again past mouse_threshold2 when
// mouse_accel is 2), clamps the pointer to the screen, stores the deltas and the three buttons
// (bit 0 left, bit 1 right, bit 2 middle).
// FUNCTION: LEGOLAND 0x00473b00
LEGO_EXPORT void UpdateControllerFromMouseData(struct CtrlBuffer *buffer) {
    int dx;
    register int dy;
    unsigned int mode;
    int flags;

    int ptmp175;
    unsigned int cur_x;
    volatile int ptmp176;
    if (buffer == NULL) {
        return;
    }
    buffer->prev_x = buffer->x;
    mode = buffer->mouse_accel;
    ptmp176 = buffer->y;
    buffer->prev_y = ptmp176;
    dx = MouseState.lX;
    dy = MouseState.lY;
    if (0 != mode) {
        if (abs(dx) > ((volatile struct CtrlBuffer *)buffer)->mouse_threshold1 ||
            abs(dy) > ((volatile struct CtrlBuffer *)buffer)->mouse_threshold1) {
            dx += dx;
            dy += dy;
        }
        if (mode == 2) {
            if (abs(dx) > buffer->mouse_threshold2 || abs(dy) > buffer->mouse_threshold2) {
                dx <<= 1;
                dy <<= 1;
            }
        }
    }
    buffer->x += dx;
    buffer->y += dy;
    if (buffer->x >= lpConfig->screen_width) {
        buffer->x = lpConfig->screen_width - 1;
    }
    if (buffer->x < 0) {
        buffer->x = 0;
    }
    if (buffer->y >= lpConfig->screen_height) {
        buffer->y = lpConfig->screen_height - 1;
    }
    if (buffer->y < 0) {
        buffer->y = 0;
    }
    cur_x = buffer->x;
    buffer->delta_x = cur_x - buffer->prev_x;
    buffer->delta_y = buffer->y - buffer->prev_y;
    flags = buffer->buttons & 0xfffffff8;
    buffer->buttons = flags;
    if ((MouseState.rgbButtons[0] & 0x80) != 0) {
        buffer->buttons = flags | 1;
    }
    if ((MouseState.rgbButtons[2] & 0x80) != 0) {
        buffer->buttons = buffer->buttons | 4;
    }
    if ((MouseState.rgbButtons[1] & 0x80) != 0) {
        ptmp175 = buffer->buttons;
        buffer->buttons = 2 | ptmp175;
    }
}

// FUNCTION: LEGOLAND 0x00473c10
LEGO_EXPORT void UpdateControllerFromKeyboardData(struct CtrlBuffer *buffer) {
    char c;
    int arg;

    buffer->buttons = buffer->buttons & 0xfffff807;
    c = ReadCheatKey();
    if (c != 0) {
        memcpy(CheatKeyBuffer, CheatKeyBuffer + 1, 19);
        CheatKeyBuffer[0x13] = c;
        // STRING: LEGOLAND 0x004baf9c
        if (_memicmp(":THEME", &CheatKeyBuffer[0xe], 6) == 0) {
            SetInteractiveMusicTheme(0);
            // STRING: LEGOLAND 0x004baf88
            DBPrintf("CHEATTHEME=THEME\n");
            // STRING: LEGOLAND 0x004baf70
        } else if (_memicmp(":EGYPT", &CheatKeyBuffer[0xe], 6) == 0) {
            SetInteractiveMusicTheme(1);
            // STRING: LEGOLAND 0x004baf58
            DBPrintf("CHEATTHEME=EGYPTIAN\n");
            // STRING: LEGOLAND 0x004baf50
        } else if (_memicmp(":INCA", &CheatKeyBuffer[0xf], 5) == 0) {
            SetInteractiveMusicTheme(2);
            // STRING: LEGOLAND 0x004baf3c
            DBPrintf("CHEATTHEME=INCA\n");
            // STRING: LEGOLAND 0x004baf34
        } else if (_memicmp(":CASTLE", &CheatKeyBuffer[0xd], 7) == 0) {
            SetInteractiveMusicTheme(3);
            // STRING: LEGOLAND 0x004baf20
            DBPrintf("CHEATTHEME=CASTLE\n");
            // STRING: LEGOLAND 0x004baf18
        } else if (_memicmp(":WEST", &CheatKeyBuffer[0xf], 5) == 0) {
            SetInteractiveMusicTheme(4);
            // STRING: LEGOLAND 0x004baf04
            DBPrintf("CHEATTHEME=WEST\n");
            // STRING: LEGOLAND 0x004baefc
        } else if (_memicmp(":STOP", &CheatKeyBuffer[0xf], 5) == 0) {
            StopInteractiveMusic();
            // STRING: LEGOLAND 0x004baee8
            DBPrintf("CHEAT:STOPMUSIC\n");
            // STRING: LEGOLAND 0x004baee0
        } else if (_memicmp("::DIE", &CheatKeyBuffer[0xf], 5) == 0) {
            exit(1);
        }
        if (EditMode.unk4 == 3) {
            // STRING: LEGOLAND 0x004baf78
            if (_memicmp(":ILIKETOTRAVEL", &CheatKeyBuffer[4], 0xe) == 0) {
                if (CheatKeyBuffer[0x12] == '1') {
                    if (CheatKeyBuffer[0x13] == '0') {
                        arg = 0xa;
                        lpConfig->level = 0xf;
                        // STRING: LEGOLAND 0x004baed0
                        DBPrintf("CHEAT:Level %d\n", arg);
                        MapStats.level_end = 2;
                    }
                } else if (CheatKeyBuffer[0x12] == '0') {
                    if (CheatKeyBuffer[0x13] >= '1' && CheatKeyBuffer[0x13] <= '9') {
                        lpConfig->level = CheatKeyBuffer[0x13] - 0x2b;
                        arg = lpConfig->level;
                        DBPrintf("CHEAT:Level %d\n", arg);
                        MapStats.level_end = 2;
                    }
                } else if (CheatKeyBuffer[0x12] == 'T' && CheatKeyBuffer[0x13] >= '1' && CheatKeyBuffer[0x13] <= '5') {
                    lpConfig->level = CheatKeyBuffer[0x13] - 0x30;
                    arg = lpConfig->level;
                    DBPrintf("CHEAT:Level %d\n", arg);
                    MapStats.level_end = 2;
                }
                // STRING: LEGOLAND 0x004baec0
            } else if (_memicmp(":COLDHARDCASH", &CheatKeyBuffer[7], 0xd) == 0) {
                // STRING: LEGOLAND 0x004baeac
                DBPrintf("CHEAT:More Money\n");
                AddBricks(5000);
                // STRING: LEGOLAND 0x004bae9c
            } else if (_memicmp(":HARDASNAILS", &CheatKeyBuffer[8], 0xc) == 0) {
                // STRING: LEGOLAND 0x004bae88
                DBPrintf("CHEAT:No Ride Wear\n");
                MapStats.ride_wear = 0;
                // STRING: LEGOLAND 0x004bae7c
            } else if (_memicmp(":PRAISEME", &CheatKeyBuffer[0xb], 9) == 0) {
                // STRING: LEGOLAND 0x004bae60
                DBPrintf("CHEAT:Instant Appraisal\n");
                AppraisalDeadline = 1;
                // STRING: LEGOLAND 0x004bae50
            } else if (_memicmp(":WELOVELEGOLAND", &CheatKeyBuffer[5], 0xf) == 0) {
                // STRING: LEGOLAND 0x004bae3c
                DBPrintf("CHEAT:Win Level\n", lpConfig->level);
                EndLevel(1);
                // STRING: LEGOLAND 0x004bae30
            } else if (memcmp(":IMPROVISE", &CheatKeyBuffer[10], 10) == 0) {
                // STRING: LEGOLAND 0x004bae1c
                DBPrintf("CHEAT:Stop Script\n", lpConfig->level);
                SetScriptStopped(1);
                // STRING: LEGOLAND 0x004bae14
            } else if (_memicmp(":DIGGER", &CheatKeyBuffer[0xd], 7) == 0) {
                // STRING: LEGOLAND 0x004bae00
                DBPrintf("CHEAT:Set Switch 1\n");
                OpenBridge(0);
                OpenBridge(1);
                OpenBridge(2);
                OpenBridge(3);
                // STRING: LEGOLAND 0x004badf0
            } else if (memcmp(":SHOWCAPACITY", &CheatKeyBuffer[7], 0xd) == 0) {
                MapStats.show_capacity = 1;
                // STRING: LEGOLAND 0x004badd0
                DBPrintf("CHEAT: Capacity Calcs visible\n");
            }
        }
    }
    if ((KeyboardState[0xcb] & 0x80) != 0) {
        buffer->buttons = buffer->buttons | 8;
    }
    if ((KeyboardState[0xcd] & 0x80) != 0) {
        buffer->buttons = buffer->buttons | 0x10;
    }
    if ((KeyboardState[0xc8] & 0x80) != 0) {
        buffer->buttons = buffer->buttons | 0x20;
    }
    if ((KeyboardState[0xd0] & 0x80) != 0) {
        buffer->buttons = buffer->buttons | 0x40;
    }
    if ((KeyboardState[0x39] & 0x80) != 0) {
        buffer->buttons = buffer->buttons | 0x80;
    }
    if ((KeyboardState[0xf] & 0x80) != 0) {
        buffer->buttons = buffer->buttons | 0x100;
    }
    if ((KeyboardState[1] & 0x80) != 0) {
        buffer->buttons = buffer->buttons | 0x200;
    }
    if ((KeyboardState[0x1c] & 0x80) != 0) {
        buffer->buttons = buffer->buttons | 0x400;
    }
}

// FUNCTION: LEGOLAND 0x00474070
unsigned int IsLeftShiftDown(void) {
    return KeyboardState[0x2a] >> 7;
}

// FUNCTION: LEGOLAND 0x00474080
unsigned int IsRightShiftDown(void) {
    return KeyboardState[0x36] >> 7;
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
        prev = (char)PrevKeyState[i];
        state = KeyboardState[CheatKeyMap[i].flags];
        prev = (prev >> 7) & 1;
        PrevKeyState[i] = state;
        if ((state & 0x80) != 0 && prev == 0) {
            alt = GetKeyState(0x14) & 1;
            if (IsLeftShiftDown() != 0 || IsRightShiftDown() != 0) {
                alt = (alt == 0);
            }
            result = CheatKeyMap[i].code;
            if (alt == 0) {
                result = tolower(result);
            }
        }
        i = i + 1;
    } while (i < 0x3b);
    return (char)result;
}

// FUNCTION: LEGOLAND 0x00474130
char ReadCheatKey(void) {
    int i;
    int result;
    unsigned char state;
    unsigned int prev;

    result = 0;
    i = 0;
    do {
        prev = (char)CheatKeyState[i];
        state = KeyboardState[CheatKeyMap[i].flags];
        prev = (prev >> 7) & 1;
        CheatKeyState[i] = state;
        if ((state & 0x80) != 0 && prev == 0) {
            result = CheatKeyMap[i].code;
        }
        i = i + 1;
    } while (i < 0x3b);
    if (result == -10) {
        result = 0x3a;
    }
    return (char)result;
}

// FUNCTION: LEGOLAND 0x00474190
void SaveInputState(void) {
    SaveGameWrite(&SelectedBuildTheme, 4);
    SaveGameWrite(&ResearchTabSelected, 4);
    SaveGameWrite(&BuildSidePanelState, 16);
}

// FUNCTION: LEGOLAND 0x004741c0
void LoadInputState(void) {
    SaveGameRead(&SelectedBuildTheme, 4);
    SaveGameRead(&ResearchTabSelected, 4);
    SaveGameRead(&BuildSidePanelState, 16);
}
