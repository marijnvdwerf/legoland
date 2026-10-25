#include <windows.h>

#include "draw.h"
#include "timer.h"

#include "globals.h"
#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00499430
LEGO_EXPORT unsigned int GetGameTimer(void) {
    unsigned int now;

    if (GameTimerPaused != 0) {
        now = GameTimerPausedTicks;
    } else {
        now = GetTickCount();
    }
    return now - GameTimerBaseTicks;
}

// FUNCTION: LEGOLAND 0x00499460
unsigned int GetGameTimerFrames(void) {
    unsigned int now;

    if (GameTimerPaused != 0) {
        now = GameTimerPausedFrame;
    } else {
        now = FrameCounter;
    }
    return now - GameTimerBaseFrame;
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
int EnsureSpriteSurface(struct Sprite *sprite) {
    if (sprite->flags & 0x20) {
        if (RecreateSprite(sprite) == 0) {
            return 0;
        }
    }
    MakeSprite(sprite);
    return 1;
}
