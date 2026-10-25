#include "money.h"
#include "globals.h"
#include "legoland.h"

#include "bricks.h"
#include "obj_instance.h"
#include "sound_music.h"

// FUNCTION: LEGOLAND 0x00453900
LEGO_EXPORT void LoadMoneySFX(void) {
    if (MoneySFXLoadCount++ == 0) {
        Load_FXList(MONEY_SFX, 2);
    }
}

// FUNCTION: LEGOLAND 0x00453930
LEGO_EXPORT void KillMoneySFX(void) {
    if (--MoneySFXLoadCount == 0) {
        Kill_FXList(MONEY_SFX, 2);
    }
}

// FUNCTION: LEGOLAND 0x00453950
LEGO_EXPORT void PlayMoneySFX(TileId *tile, int sfx, int a2) {
    struct SampleSource config;
    void *def;

    config.type = 2;
    config.x = tile->pos.x;
    config.y = tile->pos.y;
    def = MONEY_SFX[sfx].sample;
    PlayInstanceOfSample(def, a2, 1, &config);
}

// FUNCTION: LEGOLAND 0x004539a0
LEGO_EXPORT void StopMoneySFX(unsigned char *param_1) {
    struct SampleSource params;
    params.type = 2;
    params.x = param_1[0];
    params.y = param_1[1];
    UnSourceAndFadeAllSamplesFromSource(&params, -400);
}

// FUNCTION: LEGOLAND 0x004539e0
LEGO_EXPORT void BuyItem(struct Element *item, TileId *tile, int sfx) {
    int val = item->ride->field_28;

    if (val != 0) {
        if (sfx >= 0) {
            PlayMoneySFX(tile, sfx, 0);
        }
        AddBricks(val);
    }
}
