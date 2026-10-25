#pragma once

#include "legoland.h"
#include "map_object.h"

/* An overlay on the map (a bridge piece, ...): what the level file gives, then its place and its sprite. */
struct Overlay {
    /* 0x00 */ union {
        struct OverlayParam param; /* in the level file and the saved game */
        struct {
            unsigned int field_0;
            unsigned int field_4;
            unsigned char pad_8[0x10 - 0x8];
            int field_10;
        };
    };
    /* 0x14 */ int x;
    /* 0x18 */ int y;
    /* 0x1c */ struct Overlay *next;
    /* 0x20 */ struct Sprite *sprite;
};

// Per-TU header for mapscreen.c — canonical declarations for the overview map screen.

LEGO_EXPORT void RenderFullMap(void);
LEGO_EXPORT void InitMapScreen(void);
LEGO_EXPORT void KillMapScreen(void);
void RestoreMouseHandlers(void);

LEGO_EXPORT void RenderMouseBounds(void);
struct Point;
LEGO_EXPORT void MapScreenSetScrollPos(struct Point *point);
LEGO_EXPORT void DrawMapScreen(void);
