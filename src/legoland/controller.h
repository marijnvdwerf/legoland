#pragma once

#include "gamemap.h"
#include "legoland.h"

struct Point;

struct WorkArea {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
};

struct CtrlBuffer {
    /* 0x00 */ int prev_x;
    /* 0x04 */ int prev_y;
    /* 0x08 */ int x;
    /* 0x0c */ int y;
    /* 0x10 */ int delta_x;
    /* 0x14 */ int delta_y;
    /* 0x18 */ unsigned int buttons;
    /* 0x1c */ int mouse_threshold1;
    /* 0x20 */ int mouse_threshold2;
    /* 0x24 */ int mouse_accel;
};

LEGO_EXPORT int SetupControllers(void);
LEGO_EXPORT void ReadGameButtons(void);
void FreeControllers(void);
void UpdateEditSelection(void);

/* 0x00813a84: edit-mode selection rectangle (tiles), built from the mouse drag.
 * Declared here, not at the end of globals.h: appending it there shifts MSVC codegen in MusicThreadProc. */
extern struct MapRect SelectionRect;
