#include <stdlib.h>
#include "legoland.h"

#include "controller.h"
#include "globals.h"
#include "map_object.h"
#include "math.h"
#include "tilemap.h"
#include "timer.h"

#include <windows.h>

/* Allocates and resets the controller buffer, initializes mouse/key masks and default tile footprint values, and
 * marks controllers initialized. */
// FUNCTION: LEGOLAND 0x00451e70
LEGO_EXPORT int SetupControllers(void) {
    if (ControllersInitialized != 0) {
        return 1;
    }
    CONTROLLERBUFFER = malloc(sizeof(struct CtrlBuffer));
    CONTROLLERBUFFER->buttons = 0;
    MapRightButtonMask = 2;
    MapLeftButtonMask = 1;
    RightButtonMask = 2;
    GamePad = 0;
    MiddleButtonMask = 4;
    LeftButtonMask = 1;
    WatchedButtonMask = 0x100;
    CancelKeyMask = 0x200;
    ScrollUpKeyMask = 0x20;
    ScrollRightKeyMask = 0x10;
    ScrollDownKeyMask = 0x40;
    ScrollLeftKeyMask = 8;
    OkKeyMask = 0x400;
    MouseTileX = 10;
    MouseTileY = 4;
    FootprintWidth = 1;
    FootprintHeight = 1;
    if (NULL != CONTROLLERBUFFER) {
        ControllersInitialized = 1;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00451f40
void FreeControllers(void) {
    if (ControllersInitialized != 0) {
        free(CONTROLLERBUFFER);
        ControllersInitialized = 0;
    }
}

// FUNCTION: LEGOLAND 0x00451f70
int ScrollMapFromKeys(void) {
    int dx;
    unsigned int dy;
    int result;

    result = 0;
    if ((GamePad & 0x20) == 0) {
        return 0;
    }
    if ((ScrollLeftKeyState & 4) != 0) {
        dx = (unsigned int)lpConfig->field_24 * -2;
        ProcessScrolling(dx, 0);
    } else if ((ScrollRightKeyState & 4) != 0) {
        dx = (unsigned int)lpConfig->field_24 << 1;
        ProcessScrolling(dx, 0);
    }
    if ((ScrollUpKeyState & 4) != 0) {
        dy = -(unsigned int)lpConfig->field_24;
        ProcessScrolling(0, dy);
    } else if ((ScrollDownKeyState & 4) != 0) {
        dy = (unsigned int)lpConfig->field_24;
        ProcessScrolling(0, dy);
    }
    if ((ScrollLeftKeyState & 4) != 0 || (ScrollRightKeyState & 4) != 0 ||
        (ScrollUpKeyState & 4) != 0 || (ScrollDownKeyState & 4) != 0) {
        result = 1;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00452030
void UpdateEditSelection(void) {
    int validate;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    struct Ride *cls;

    validate = 0;
    /* constrain the drag to its dominant axis */
    if ((GamePad & 0x800) != 0) {
        if (abs(DragStartTileX - DragEndTileX) > abs(DragStartTileY - DragEndTileY)) {
            DragEndTileY = DragStartTileY;
        } else {
            DragEndTileX = DragStartTileX;
        }
    }
    if (EditMode.unk0 == 2) {
        min_x = DragStartTileX < DragEndTileX ? DragStartTileX : DragEndTileX;
        min_y = DragStartTileY < DragEndTileY ? DragStartTileY : DragEndTileY;
        max_x = DragStartTileX > DragEndTileX ? DragStartTileX : DragEndTileX;
        max_y = DragEndTileY;
        if (DragStartTileY > DragEndTileY) {
            max_y = DragStartTileY;
        }
        /* both arms end in the same stores; the compiler merges them into one tail */
        if (DragRemoveClass != NULL) {
            cls = DragRemoveClass;
            FootprintWidth = cls->footprint.x1 - cls->footprint.x0 + 1;
            FootprintHeight = cls->footprint.y1 - cls->footprint.y0 + 1;
            SelectionRect.x0 = min_x;
            SelectionRect.y0 = min_y;
            SelectionRect.x1 = max_x;
            SelectionRect.y1 = max_y;
        } else {
            FootprintWidth = 1;
            FootprintHeight = 1;
            SelectionRect.x0 = min_x;
            SelectionRect.y0 = min_y;
            SelectionRect.x1 = max_x;
            SelectionRect.y1 = max_y;
        }
    } else {
        min_x = DragStartTileX < DragEndTileX ? DragStartTileX : DragEndTileX;
        min_y = DragStartTileY < DragEndTileY ? DragStartTileY : DragEndTileY;
        max_x = DragStartTileX > DragEndTileX ? DragStartTileX : DragEndTileX;
        max_y = DragStartTileY > DragEndTileY ? DragStartTileY : DragEndTileY;
        cls = EditMode.unk8;
        FootprintWidth = cls->footprint.x1 - cls->footprint.x0 + 1;
        FootprintHeight = cls->footprint.y1 - cls->footprint.y0 + 1;
        if (memcmp(&DragStartTileX, &DragEndTileX, 8) != 0) {
            /* snap the drag rectangle to whole footprints, anchored at the drag start */
            if (FootprintWidth > 1 && min_x == DragEndTileX && max_x == DragStartTileX) {
                min_x = max_x - (FootprintWidth - min_x + max_x) / FootprintWidth * FootprintWidth;
                max_x = FootprintWidth + DragStartTileX - 1;
            } else {
                max_x = (FootprintWidth - min_x + max_x) / FootprintWidth * FootprintWidth + min_x - 1;
            }
            if (FootprintHeight > 1 && min_y == DragEndTileY && max_y == DragStartTileY) {
                min_y = max_y - (FootprintHeight - min_y + max_y) / FootprintHeight * FootprintHeight;
                max_y = DragStartTileY + FootprintHeight - 1;
            } else {
                max_y = (FootprintHeight - min_y + max_y) / FootprintHeight * FootprintHeight + min_y - 1;
            }
            if (abs(min_x - max_x) <= FootprintWidth && abs(min_y - max_y) <= FootprintHeight) {
                validate = 1;
            }
        } else {
            /* single click: one footprint */
            validate = 1;
            max_x = (FootprintWidth - min_x + max_x) / FootprintWidth * FootprintWidth + min_x - 1;
            max_y = (FootprintHeight - min_y + max_y) / FootprintHeight * FootprintHeight + min_y - 1;
        }
        if (EditMode.unk0 == 1 && EditMode.unk8 != NULL) {
            SelectionRect.x0 = EditMode.unk8->footprint.x0 + min_x;
            SelectionRect.y0 = EditMode.unk8->footprint.y0 + min_y;
            SelectionRect.x1 = EditMode.unk8->footprint.x0 + max_x;
            SelectionRect.y1 = EditMode.unk8->footprint.y0 + max_y;
        }
    }
    EditCursor.field_1414[2] = SelectionRect.x1 - min_x;
    EditCursor.field_1414[3] = SelectionRect.y1 - min_y;
    EditCursor.field_1414[0] = SelectionRect.x0 - min_x;
    EditCursor.field_1414[1] = SelectionRect.y0 - min_y;
    EditCursor.field_1414[4] = 0;
    EditCursor.tile_x = min_x;
    EditCursor.tile_y = min_y;
    EditCursor.field_1830 = 0;
    if (validate) {
        ValidateCursor(&EditCursor, EditMode.unk8);
    } else {
        ResetCursorBuildable(&EditCursor);
    }
}

/* Initializes the selection drag from the queried object footprint or current mouse tile, sets the drag end to the
 * mouse tile, and consumes a pending left click. */
// FUNCTION: LEGOLAND 0x00452390
void BeginTileSelectionDrag(void) {
    struct Ride *cls;

    if (EditMode.unk0 == 2 && QueryClass != NULL) {
        cls = QueryClass;
        DragQueryObjectId = QueryObj.id;
        DragFootprintX0 = cls->footprint.v[0];
        DragFootprintX1 = cls->footprint.v[2];
        DragFootprintY0 = cls->footprint.v[1];
        DragFootprintY1 = cls->footprint.v[3];
        DragFootprintWidth = (DragFootprintX1 - DragFootprintX0) + 1;
        DragFootprintHeight = (DragFootprintY1 - DragFootprintY0) + 1;
        DragStartTileX = QueryObj.pos.x + DragFootprintX0;
        DragStartTileY = QueryObj.pos.y + DragFootprintY0;
        DragEndTileX = MouseTileX;
        DragEndTileY = MouseTileY;
    } else {
        DragStartTileX = MouseTileX;
        DragStartTileY = MouseTileY;
        DragEndTileX = MouseTileX;
        DragEndTileY = MouseTileY;
    }
    if ((MouseClickFlags & 1) != 0) {
        GamePad = GamePad | 0x1000;
        MouseClickFlags = MouseClickFlags & 0xfffffffe;
    }
}

// FUNCTION: LEGOLAND 0x00452460
LEGO_EXPORT void ReadGameButtons(void) {
    volatile unsigned int saved;
    unsigned int held;
    DWORD now;
    int scroll;
    unsigned int released;
    unsigned int repeat;
    unsigned int pressed;
    struct Point mp;

    MouseClickConsumed = 0;
    held = CONTROLLERBUFFER->buttons;
    pressed = (PrevHeldButtons ^ held) & held;
    released = ~pressed & (PrevHeldButtons ^ held);
    if (released != 0 && IgnoreNextButtonRelease != 0) {
        released = 0;
        IgnoreNextButtonRelease = 0;
    }
    if (pressed != 0) {
        ButtonRepeatLastTick = GetTickCount();
        ButtonRepeatDelay = 200;
    }
    now = GetTickCount();
    if (now - ButtonRepeatLastTick > ButtonRepeatDelay) {
        saved = held;
        ButtonRepeatLastTick = GetTickCount();
        ButtonRepeatDelay = 0x32;
        repeat = held;
    } else {
        repeat = 0;
    }
    GamePad = GamePad & 0xffffffe1;
    PrevHeldButtons = held;

    WatchedButtonState = 0;
    if ((pressed & WatchedButtonMask) != 0) WatchedButtonState = 1;
    if ((released & WatchedButtonMask) != 0) WatchedButtonState |= 2;
    if ((held & WatchedButtonMask) != 0) WatchedButtonState |= 4;
    if ((repeat & WatchedButtonMask) != 0) WatchedButtonState |= 8;

    CancelKeyState = 0;
    if ((pressed & CancelKeyMask) != 0) CancelKeyState = 1;
    if ((released & CancelKeyMask) != 0) CancelKeyState |= 2;
    if ((held & CancelKeyMask) != 0) CancelKeyState |= 4;
    if ((repeat & CancelKeyMask) != 0) CancelKeyState |= 8;

    ScrollUpKeyState = 0;
    if ((pressed & ScrollUpKeyMask) != 0) ScrollUpKeyState = 1;
    if ((released & ScrollUpKeyMask) != 0) ScrollUpKeyState |= 2;
    if ((held & ScrollUpKeyMask) != 0) ScrollUpKeyState |= 4;
    if ((repeat & ScrollUpKeyMask) != 0) ScrollUpKeyState |= 8;

    ScrollRightKeyState = 0;
    if ((pressed & ScrollRightKeyMask) != 0) ScrollRightKeyState = 1;
    if ((released & ScrollRightKeyMask) != 0) ScrollRightKeyState |= 2;
    if ((held & ScrollRightKeyMask) != 0) ScrollRightKeyState |= 4;
    if ((repeat & ScrollRightKeyMask) != 0) ScrollRightKeyState |= 8;

    ScrollDownKeyState = 0;
    if ((pressed & ScrollDownKeyMask) != 0) ScrollDownKeyState = 1;
    if ((released & ScrollDownKeyMask) != 0) ScrollDownKeyState |= 2;
    if ((held & ScrollDownKeyMask) != 0) ScrollDownKeyState |= 4;
    if ((repeat & ScrollDownKeyMask) != 0) ScrollDownKeyState |= 8;

    ScrollLeftKeyState = 0;
    if ((pressed & ScrollLeftKeyMask) != 0) ScrollLeftKeyState = 1;
    if ((released & ScrollLeftKeyMask) != 0) ScrollLeftKeyState |= 2;
    if ((held & ScrollLeftKeyMask) != 0) ScrollLeftKeyState |= 4;
    if ((repeat & ScrollLeftKeyMask) != 0) ScrollLeftKeyState |= 8;

    MouseClickFlags = 0;
    if ((pressed & LeftButtonMask) != 0) MouseClickFlags = 1;
    if ((released & LeftButtonMask) != 0) MouseClickFlags |= 2;
    if ((held & LeftButtonMask) != 0) MouseClickFlags |= 4;
    if ((repeat & LeftButtonMask) != 0) MouseClickFlags |= 8;

    RightClickFlags = 0;
    if ((pressed & RightButtonMask) != 0) RightClickFlags = 1;
    if ((released & RightButtonMask) != 0) RightClickFlags |= 2;
    if ((held & RightButtonMask) != 0) RightClickFlags |= 4;
    if ((repeat & RightButtonMask) != 0) RightClickFlags |= 8;

    OkKeyState = 0;
    if ((pressed & OkKeyMask) != 0) OkKeyState = 1;
    if ((released & OkKeyMask) != 0) OkKeyState |= 2;
    if ((held & OkKeyMask) != 0) OkKeyState |= 4;
    if ((repeat & OkKeyMask) != 0) OkKeyState |= 8;

    if (lpConfig->field_1e != 0) {
        MousePos.x = CONTROLLERBUFFER->x;
        MousePos.y = CONTROLLERBUFFER->y;
        if (MapLoaded != 0) {
            scroll = ScrollMapFromKeys();
            if ((GamePad & 0x1000) != 0 || FocussedIconPtr == 0) {
                if (scroll == 0 && (GamePad & 0x20) != 0) {
                    MouseScrollMap();
                }
                if (CONTROLLERBUFFER->delta_x != 0 ||
                    CONTROLLERBUFFER->delta_y != 0 || (GamePad & 0x10) != 0) {
                    GamePad = GamePad | 8;
                    LastPointerActivityTicks = GetTicks();
                }
                ScreenToMapRef(&MousePos.x, &mp.x, 0);
                if (MouseTileX != mp.x || MouseTileY != mp.y) {
                    MouseTileY = mp.y;
                    MouseTileX = mp.x;
                    if ((GamePad & 0x1000) == 0) {
                        GamePad = GamePad | 4;
                    }
                    GamePad = GamePad | 2;
                }
            }
        }

        MapLeftClickFlags = 0;
        if ((pressed & MapLeftButtonMask) != 0) MapLeftClickFlags = 1;
        if ((released & MapLeftButtonMask) != 0) MapLeftClickFlags |= 2;
        if ((held & MapLeftButtonMask) != 0) MapLeftClickFlags |= 4;
        if ((repeat & MapLeftButtonMask) != 0) MapLeftClickFlags |= 8;

        MapRightClickFlags = 0;
        if ((pressed & MapRightButtonMask) != 0) MapRightClickFlags = 1;
        if ((released & MapRightButtonMask) != 0) MapRightClickFlags |= 2;
        if ((held & MapRightButtonMask) != 0) MapRightClickFlags |= 4;
        if ((repeat & MapRightButtonMask) != 0) MapRightClickFlags |= 8;

        if ((GamePad & 4) != 0 && (MouseClickFlags & 4) != 0) {
            MouseClickFlags |= 0x10;
        }
        if ((GamePad & 0x400) != 0 && (Hover.type & 0x100) != 0) {
            if ((GamePad & 0x1000) != 0) {
                DragEndTileX = MouseTileX;
                DragEndTileY = MouseTileY;
                if ((MouseClickFlags & 2) != 0) {
                    MouseClickConsumed = 1;
                    MouseClickFlags = MouseClickFlags | 0x11;
                    GamePad = GamePad & 0xffffefff;
                } else {
                    MouseClickFlags = MouseClickFlags & 0xffffffee;
                }
            } else {
                BeginTileSelectionDrag();
            }
            UpdateEditSelection();
        }
    }
    now = GetTicks();
    if (now - LastPointerActivityTicks >= 2000) {
        GamePad = GamePad | 0x200;
        return;
    }
    GamePad = GamePad & 0xfffffdff;
}
