#pragma once

struct SpiderNode {
    unsigned short field_0;
    unsigned char pad_2[0x2a];
    struct SpiderNode *next;
};

int FUN_00415a90(struct SpiderNode *node);
void FUN_00416330(void);
void SaveSpider(void);
void LoadSpider(void);
