#include <windows.h>

#include "timer.h"
#include "draw.h"

struct Sprite {
    unsigned char pad_0[0x10];
    unsigned char flags;
};

#include "image_sprite.h"
#include "globals.h"

// FUNCTION: LEGOLAND 0x00499430
LEGO_EXPORT unsigned int GetGameTimer(void) {
    unsigned int now;

    if (DAT_0079a890 != 0) {
        now = DAT_0079a894;
    } else {
        now = GetTickCount();
    }
    return now - DAT_0079a898;
}

// FUNCTION: LEGOLAND 0x00499460
unsigned int FUN_00499460(void) {
    unsigned int now;

    if (DAT_0079a890 != 0) {
        now = DAT_0079a89c;
    } else {
        now = DAT_008119a4;
    }
    return now - DAT_0079a8a0;
}

// FUNCTION: LEGOLAND 0x00499450
unsigned long GetTicks(void) {
    return GetTickCount();
}

// FUNCTION: LEGOLAND 0x00499480
LEGO_EXPORT unsigned int GetBlink(void) {
    return (GetTicks() >> 9) & 1;
}

// FUNCTION: LEGOLAND 0x00499500
int FUN_00499500(struct Sprite *sprite) {
    if (sprite->flags & 0x20) {
        if (RecreateSprite(sprite) == 0) {
            return 0;
        }
    }
    MakeSprite((unsigned int)sprite); /* TODO-fold: timer.c keeps its own struct Sprite view; MakeSprite's canonical param is the uint sprite handle */
    return 1;
}
