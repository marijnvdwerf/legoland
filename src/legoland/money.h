#pragma once

struct ShopItem {
    unsigned char pad_0[0x28];
    short field_28;
};

struct BuyItemArg {
    unsigned char pad_0[0xc];
    struct ShopItem *field_c;
};

void LoadMoneySFX(void);
void KillMoneySFX(void);
void PlayMoneySFX(int a0, int a1, int a2);
void StopMoneySFX(void);
void BuyItem(struct BuyItemArg *item, int a2, int a3);
