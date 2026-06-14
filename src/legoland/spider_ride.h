#pragma once

#include "legoland.h"
#include "objclass.h"

struct SpiderNode {
    unsigned short field_0;
    unsigned char pad_2[0x2a];
    struct SpiderNode *next;
};

int FUN_00415a90(struct SpiderNode *node);
void FUN_00416330(void);
LEGO_EXPORT int SaveSpider(void);
LEGO_EXPORT int LoadSpider(void);

void SpiderRide(struct ClassNode *head, struct CallbackTable *iface);
