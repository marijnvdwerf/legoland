#pragma once

struct SafariNode {
    unsigned short field_0;
    unsigned char pad_2[14];
    struct SafariNode *next;
    unsigned char pad_14[0x14];
};

void FUN_00414b10(struct SafariNode *node);
