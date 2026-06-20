#pragma once

struct CallbackTable;
struct ClassNode;

struct BarrelNode {
    struct BarrelNode *next;
    unsigned short field_4;
    unsigned char field_6;
    unsigned char field_7;
    unsigned char field_8;
    unsigned char pad_9[3];
    unsigned int field_c;
    unsigned char field_10;
    unsigned char pad_11[3];
    unsigned int field_14;
    unsigned char field_18;
    unsigned char pad_19[0x34 - 0x19];
};

void FUN_0043c2f0(struct BarrelNode *node);
void FUN_0043c950(void);

void FUN_0043c760(struct ClassNode *str, struct CallbackTable *ride);
