#include "globals.h"
#include "legoland.h"

#include "entrance.h"
#include "money.h"
#include "sound_music.h"

struct EntranceParam {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0042d970
void FUN_0042d970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042d9c0
void FUN_0042d9c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042de50
void FUN_0042de50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042def0
void FUN_0042def0(struct EntranceParam *param) {
    Kill_FXList(ENTRANCE_SFX, 1);
    KillMoneySFX();
    DAT_006160f4 = param->field_c;
    if (DAT_0061610c != 0) {
        KillSprite(DAT_0061610c);
    }
    if (DAT_006160fc != 0) {
        KillSprite(DAT_006160fc);
    }
    if (DAT_00616100 != 0) {
        KillSprite(DAT_00616100);
    }
    if (DAT_00616104 != 0) {
        KillSprite(DAT_00616104);
    }
    if (DAT_00616108 != 0) {
        KillSprite(DAT_00616108);
    }
}

// FUNCTION: LEGOLAND 0x0042df70
void FUN_0042df70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042dfa0
void FUN_0042dfa0(void) { STUB(); }
