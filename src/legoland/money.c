#include "money.h"
#include "globals.h"
#include "legoland.h"

#include "bricks.h"
#include "obj_instance.h"
#include "sound_music.h"

struct ShopItem {
    unsigned char pad_0[0x28];
    short field_28;
};

struct BuyItemArg {
    unsigned char pad_0[0xc];
    struct ShopItem *field_c;
};

// FUNCTION: LEGOLAND 0x00453900
LEGO_EXPORT void LoadMoneySFX(void) {
    if (DAT_00667120++ == 0) {
        Load_FXList(MONEY_SFX, 2);
    }
}

// FUNCTION: LEGOLAND 0x00453930
LEGO_EXPORT void KillMoneySFX(void) {
    if (--DAT_00667120 == 0) {
        Kill_FXList(MONEY_SFX, 2);
    }
}

// FUNCTION: LEGOLAND 0x00453950
LEGO_EXPORT void PlayMoneySFX(TileId *tile, int sfx, int a2) {
    struct SampleParams config;
    void *def;

    config.field_0 = 2;
    config.field_8 = tile->pos.x;
    config.field_c = tile->pos.y;
    def = PTR_004b87b0[sfx * 3];
    PlayInstanceOfSample(def, a2, 1, &config);
}

// FUNCTION: LEGOLAND 0x004539a0
LEGO_EXPORT void StopMoneySFX(unsigned char *param_1) {
    struct SampleParams params;
    params.field_0 = 2;
    params.field_8 = param_1[0];
    params.field_c = param_1[1];
    UnSourceAndFadeAllSamplesFromSource(&params, -400);
}

// FUNCTION: LEGOLAND 0x004539e0
LEGO_EXPORT void BuyItem(struct BuyItemArg *item, TileId *tile, int sfx) {
    int val = item->field_c->field_28;

    if (val != 0) {
        if (sfx >= 0) {
            PlayMoneySFX(tile, sfx, 0);
        }
        AddBricks(val);
    }
}
