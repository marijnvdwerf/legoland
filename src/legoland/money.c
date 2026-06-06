#include "legoland.h"

struct BuyItemArg {
    unsigned char pad_0[0xc];
    struct ShopItem *field_c;
};

struct ShopItem {
    unsigned char pad_0[0x28];
    short field_28;
};

extern unsigned int DAT_00667120;
extern unsigned char MONEY_SFX[12];

extern void Kill_FXList(unsigned char *list, unsigned int count);
extern void AddBricks(unsigned int amount);

// FUNCTION: LEGOLAND 0x00453900
void LoadMoneySFX(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453930
void KillMoneySFX(void)
{
    if (--DAT_00667120 == 0) {
        Kill_FXList(MONEY_SFX, 2);
    }
}

// FUNCTION: LEGOLAND 0x00453950
void PlayMoneySFX(int a0, int a1, int a2) { STUB(); }

// FUNCTION: LEGOLAND 0x004539a0
void StopMoneySFX(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004539e0
void BuyItem(struct BuyItemArg *item, int a2, int a3)
{
    int val = item->field_c->field_28;

    if (val != 0) {
        if (a3 >= 0) {
            PlayMoneySFX(a2, a3, 0);
        }
        AddBricks(val);
    }
}
