#include <stdlib.h>
#include "legoland.h"

#include "controller.h"
#include "globals.h"
#include "map_object.h"
#include "math.h"
#include "tilemap.h"
#include "timer.h"

#include <windows.h>

// FUNCTION: LEGOLAND 0x00451e70
LEGO_EXPORT int SetupControllers(void) {
    if (DAT_00667104 != 0) {
        return 1;
    }
    CONTROLLERBUFFER = malloc(sizeof(struct CtrlBuffer));
    ((struct CtrlBuffer *)CONTROLLERBUFFER)->field_18 = 0;
    DAT_00813a5c = 2;
    DAT_00813ac8 = 2;
    GamePad = 0;
    DAT_00813a4c = 1;
    DAT_00813a54 = 4;
    DAT_00813ac0 = 1;
    DAT_00813ab8 = 0x100;
    DAT_00813ad0 = 0x200;
    DAT_00813a98 = 0x20;
    DAT_00813aa0 = 0x10;
    DAT_00813aa8 = 0x40;
    DAT_00813ab0 = 8;
    DAT_00813ad8 = 0x400;
    DAT_00813a64 = 10;
    DAT_00813a68 = 4;
    DAT_00813a6c = 1;
    DAT_00813a70 = 1;
    if (CONTROLLERBUFFER != NULL) {
        DAT_00667104 = 1;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00451f40
void FUN_00451f40(void) {
    if (DAT_00667104 != 0) {
        free(CONTROLLERBUFFER);
        DAT_00667104 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00451f70
int FUN_00451f70(void) {
    int dx;
    unsigned int dy;
    int result;

    result = 0;
    if ((GamePad & 0x20) == 0) {
        return 0;
    }
    if ((DAT_00813ab4 & 4) != 0) {
        dx = (unsigned int)lpConfig->field_24 * -2;
        ProcessScrolling(dx, 0);
    } else if ((DAT_00813aa4 & 4) != 0) {
        dx = (unsigned int)lpConfig->field_24 << 1;
        ProcessScrolling(dx, 0);
    }
    if ((DAT_00813a9c & 4) != 0) {
        dy = -(unsigned int)lpConfig->field_24;
        ProcessScrolling(0, dy);
    } else if ((DAT_00813aac & 4) != 0) {
        dy = (unsigned int)lpConfig->field_24;
        ProcessScrolling(0, dy);
    }
    if ((DAT_00813ab4 & 4) != 0 || (DAT_00813aa4 & 4) != 0 ||
        (DAT_00813a9c & 4) != 0 || (DAT_00813aac & 4) != 0) {
        result = 1;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00452030
void FUN_00452030(void) {
    int bValidate;
    struct ObjClass *cls;
    int v74;
    int v78;
    int v7c;
    int v80;
    int local_14;
    int local_10;
    int local_c;
    int local_8;
    int iVar5;

    bValidate = 0;
    if ((GamePad & 0x800) != 0) {
        if (abs((int)(DAT_00813a78 - DAT_00813a80)) < abs((int)(DAT_00813a74 - DAT_00813a7c))) {
            DAT_00813a80 = DAT_00813a78;
        } else {
            DAT_00813a7c = DAT_00813a74;
        }
    }
    v74 = DAT_00813a74;
    v7c = DAT_00813a7c;
    v78 = DAT_00813a78;
    v80 = DAT_00813a80;
    if (EditMode.unk0 == 2) {
        local_14 = v7c;
        if (v74 < v7c) {
            local_14 = v74;
        }
        local_10 = v80;
        if (v78 < v80) {
            local_10 = v78;
        }
        DAT_00813a8c = v74;
        if (v74 <= v7c) {
            DAT_00813a8c = v7c;
        }
        DAT_00813a90 = v80;
        if (v80 < v78) {
            DAT_00813a90 = v78;
        }
        DAT_00813a84 = local_14;
        DAT_00813a88 = local_10;
        if (DAT_0080ff6c == NULL) {
            DAT_00813a6c = 1;
            DAT_00813a70 = 1;
        } else {
            cls = (struct ObjClass *)DAT_0080ff6c;
            DAT_00813a6c = (cls->field_44 - cls->field_3c) + 1;
            DAT_00813a70 = (cls->field_48 - cls->field_40) + 1;
        }
    } else {
        local_14 = v74;
        if (v7c <= v74) {
            local_14 = v7c;
        }
        local_10 = v78;
        if (v80 <= v78) {
            local_10 = v80;
        }
        iVar5 = v7c;
        if (v7c < v74) {
            iVar5 = v74;
        }
        local_8 = v78;
        if (v78 <= v80) {
            local_8 = v80;
        }
        cls = (struct ObjClass *)EditMode.unk8;
        local_c = cls->field_44 - cls->field_3c;
        DAT_00813a6c = local_c + 1;
        DAT_00813a70 = (cls->field_48 - cls->field_40) + 1;
        if (v74 == v7c && v78 == v80) {
            bValidate = 1;
            local_c = ((DAT_00813a6c - local_14) + iVar5) / DAT_00813a6c * DAT_00813a6c + -1 + local_14;
            iVar5 = ((DAT_00813a70 - local_10) + local_8) / DAT_00813a70 * DAT_00813a70 + -1 + local_10;
        } else {
            if ((int)DAT_00813a6c < 2 || local_14 != v7c || iVar5 != v74) {
                local_c = ((DAT_00813a6c - local_14) + iVar5) / DAT_00813a6c * DAT_00813a6c + -1 + local_14;
            } else {
                local_14 = iVar5 - ((DAT_00813a6c - local_14) + iVar5) / DAT_00813a6c * DAT_00813a6c;
                local_c = local_c + v74;
            }
            if ((int)DAT_00813a70 < 2 || local_10 != v80 || local_8 != v78) {
                iVar5 = ((DAT_00813a70 - local_10) + local_8) / DAT_00813a70 * DAT_00813a70 + -1 + local_10;
            } else {
                iVar5 = v78 + -1 + DAT_00813a70;
                local_10 = local_8 - ((DAT_00813a70 - local_10) + local_8) / DAT_00813a70 * DAT_00813a70;
            }
            if (abs(local_14 - local_c) <= (int)DAT_00813a6c && abs(local_10 - iVar5) <= (int)DAT_00813a70) {
                bValidate = 1;
            }
        }
        if (EditMode.unk0 == 1 && EditMode.unk8 != NULL) {
            cls = (struct ObjClass *)EditMode.unk8;
            DAT_00813a84 = cls->field_3c + local_14;
            DAT_00813a88 = cls->field_40 + local_10;
            DAT_00813a8c = cls->field_3c + local_c;
            DAT_00813a90 = cls->field_40 + iVar5;
        }
    }
    EditCursor.field_1414[2] = DAT_00813a8c - local_14;
    EditCursor.field_1414[3] = DAT_00813a90 - local_10;
    EditCursor.field_1414[0] = DAT_00813a84 - local_14;
    EditCursor.field_1414[1] = DAT_00813a88 - local_10;
    EditCursor.field_1414[4] = 0;
    EditCursor.field_1830 = 0;
    EditCursor.field_1404 = local_14;
    EditCursor.field_1408 = local_10;
    if (bValidate == 0) {
        FUN_0045f460(&EditCursor);
        return;
    }
    ValidateCursor(&EditCursor, (unsigned int)EditMode.unk8);
}

// FUNCTION: LEGOLAND 0x00452390
void FUN_00452390(void) {
    struct ObjClass *cls;

    if (EditMode.unk0 == 2 && QueryClass != NULL) {
        cls = (struct ObjClass *)QueryClass;
        DAT_00813a34 = QueryObj;
        DAT_00813af0 = cls->field_3c;
        DAT_00813af8 = cls->field_44;
        DAT_00813af4 = cls->field_40;
        DAT_00813afc = cls->field_48;
        DAT_00813a38 = (DAT_00813af8 - DAT_00813af0) + 1;
        DAT_00813a3c = (DAT_00813afc - DAT_00813af4) + 1;
        DAT_00813a74 = (unsigned char)QueryObj + DAT_00813af0;
        DAT_00813a78 = (unsigned char)(QueryObj >> 8) + DAT_00813af4;
        DAT_00813a7c = DAT_00813a64;
        DAT_00813a80 = DAT_00813a68;
    } else {
        DAT_00813a74 = DAT_00813a64;
        DAT_00813a78 = DAT_00813a68;
        DAT_00813a7c = DAT_00813a64;
        DAT_00813a80 = DAT_00813a68;
    }
    if ((DAT_00813ac4 & 1) != 0) {
        GamePad = GamePad | 0x1000;
        DAT_00813ac4 = DAT_00813ac4 & 0xfffffffe;
    }
}

// FUNCTION: LEGOLAND 0x00452460
LEGO_EXPORT void ReadGameButtons(void) {
    unsigned int held;
    DWORD now;
    int scroll;
    unsigned int released;
    unsigned int repeat;
    unsigned int pressed;
    int mx;
    int my;

    DAT_00667c48 = 0;
    held = ((struct CtrlBuffer *)CONTROLLERBUFFER)->field_18;
    pressed = (DAT_00813a94 ^ held) & held;
    released = ~pressed & (DAT_00813a94 ^ held);
    if (released != 0 && DAT_00667108 != 0) {
        released = 0;
        DAT_00667108 = 0;
    }
    if (pressed != 0) {
        DAT_0066710c = GetTickCount();
        DAT_006670fc = 200;
    }
    now = GetTickCount();
    if (now - DAT_0066710c > DAT_006670fc) {
        DAT_0066710c = GetTickCount();
        DAT_006670fc = 0x32;
        repeat = held;
    } else {
        repeat = 0;
    }
    GamePad = GamePad & 0xffffffe1;

    DAT_00813abc = 0;
    if ((pressed & DAT_00813ab8) != 0) DAT_00813abc = 1;
    if ((released & DAT_00813ab8) != 0) DAT_00813abc |= 2;
    if ((held & DAT_00813ab8) != 0) DAT_00813abc |= 4;
    if ((repeat & DAT_00813ab8) != 0) DAT_00813abc |= 8;

    DAT_00813ad4 = 0;
    if ((pressed & DAT_00813ad0) != 0) DAT_00813ad4 = 1;
    if ((released & DAT_00813ad0) != 0) DAT_00813ad4 |= 2;
    if ((held & DAT_00813ad0) != 0) DAT_00813ad4 |= 4;
    if ((repeat & DAT_00813ad0) != 0) DAT_00813ad4 |= 8;

    DAT_00813a9c = 0;
    if ((pressed & DAT_00813a98) != 0) DAT_00813a9c = 1;
    if ((released & DAT_00813a98) != 0) DAT_00813a9c |= 2;
    if ((held & DAT_00813a98) != 0) DAT_00813a9c |= 4;
    if ((repeat & DAT_00813a98) != 0) DAT_00813a9c |= 8;

    DAT_00813aa4 = 0;
    if ((pressed & DAT_00813aa0) != 0) DAT_00813aa4 = 1;
    if ((released & DAT_00813aa0) != 0) DAT_00813aa4 |= 2;
    if ((held & DAT_00813aa0) != 0) DAT_00813aa4 |= 4;
    if ((repeat & DAT_00813aa0) != 0) DAT_00813aa4 |= 8;

    DAT_00813aac = 0;
    if ((pressed & DAT_00813aa8) != 0) DAT_00813aac = 1;
    if ((released & DAT_00813aa8) != 0) DAT_00813aac |= 2;
    if ((held & DAT_00813aa8) != 0) DAT_00813aac |= 4;
    if ((repeat & DAT_00813aa8) != 0) DAT_00813aac |= 8;

    DAT_00813ab4 = 0;
    if ((pressed & DAT_00813ab0) != 0) DAT_00813ab4 = 1;
    if ((released & DAT_00813ab0) != 0) DAT_00813ab4 |= 2;
    if ((held & DAT_00813ab0) != 0) DAT_00813ab4 |= 4;
    if ((repeat & DAT_00813ab0) != 0) DAT_00813ab4 |= 8;

    DAT_00813ac4 = 0;
    if ((pressed & DAT_00813ac0) != 0) DAT_00813ac4 = 1;
    if ((released & DAT_00813ac0) != 0) DAT_00813ac4 |= 2;
    if ((held & DAT_00813ac0) != 0) DAT_00813ac4 |= 4;
    if ((repeat & DAT_00813ac0) != 0) DAT_00813ac4 |= 8;

    DAT_00813acc = 0;
    if ((pressed & DAT_00813ac8) != 0) DAT_00813acc = 1;
    if ((released & DAT_00813ac8) != 0) DAT_00813acc |= 2;
    if ((held & DAT_00813ac8) != 0) DAT_00813acc |= 4;
    if ((repeat & DAT_00813ac8) != 0) DAT_00813acc |= 8;

    DAT_00813adc = 0;
    if ((pressed & DAT_00813ad8) != 0) DAT_00813adc = 1;
    if ((released & DAT_00813ad8) != 0) DAT_00813adc |= 2;
    if ((held & DAT_00813ad8) != 0) DAT_00813adc |= 4;
    if ((repeat & DAT_00813ad8) != 0) DAT_00813adc |= 8;

    DAT_00813a94 = held;
    if (lpConfig->field_1e != 0) {
        DAT_00813a44.x = ((struct CtrlBuffer *)CONTROLLERBUFFER)->field_8;
        DAT_00813a44.y = ((struct CtrlBuffer *)CONTROLLERBUFFER)->field_c;
        if (DAT_00667c7c != 0) {
            scroll = FUN_00451f70();
            if ((GamePad & 0x1000) != 0 || FocussedIconPtr == 0) {
                if (scroll == 0 && (GamePad & 0x20) != 0) {
                    MouseScrollMap();
                }
                if (((struct CtrlBuffer *)CONTROLLERBUFFER)->field_10 != 0 ||
                    ((struct CtrlBuffer *)CONTROLLERBUFFER)->field_14 != 0 || (GamePad & 0x10) != 0) {
                    GamePad = GamePad | 8;
                    DAT_00813ae0 = GetTicks();
                }
                ScreenToMapRef((unsigned int)&DAT_00813a44, &mx, 0);
                if (DAT_00813a64 != mx || DAT_00813a68 != my) {
                    DAT_00813a68 = my;
                    DAT_00813a64 = mx;
                    if ((GamePad & 0x1000) == 0) {
                        GamePad = GamePad | 4;
                    }
                    GamePad = GamePad | 2;
                }
            }
        }

        DAT_00813a50 = 0;
        if ((pressed & DAT_00813a4c) != 0) DAT_00813a50 = 1;
        if ((released & DAT_00813a4c) != 0) DAT_00813a50 |= 2;
        if ((held & DAT_00813a4c) != 0) DAT_00813a50 |= 4;
        if ((repeat & DAT_00813a4c) != 0) DAT_00813a50 |= 8;

        DAT_00813a60 = 0;
        if ((pressed & DAT_00813a5c) != 0) DAT_00813a60 = 1;
        if ((released & DAT_00813a5c) != 0) DAT_00813a60 |= 2;
        if ((held & DAT_00813a5c) != 0) DAT_00813a60 |= 4;
        if ((repeat & DAT_00813a5c) != 0) DAT_00813a60 |= 8;

        if ((GamePad & 4) != 0 && (DAT_00813ac4 & 4) != 0) {
            DAT_00813ac4 |= 0x10;
        }
        if ((GamePad & 0x400) != 0 && (DAT_004bdd00 & 0x100) != 0) {
            if ((GamePad & 0x1000) != 0) {
                DAT_00813a7c = DAT_00813a64;
                DAT_00813a80 = DAT_00813a68;
                if ((DAT_00813ac4 & 2) != 0) {
                    DAT_00667c48 = 1;
                    DAT_00813ac4 = DAT_00813ac4 | 0x11;
                    GamePad = GamePad & 0xffffefff;
                } else {
                    DAT_00813ac4 = DAT_00813ac4 & 0xffffffee;
                }
            } else {
                FUN_00452390();
            }
            FUN_00452030();
        }
    }
    now = GetTicks();
    if (now - DAT_00813ae0 >= 2000) {
        GamePad = GamePad | 0x200;
        return;
    }
    GamePad = GamePad & 0xfffffdff;
}
