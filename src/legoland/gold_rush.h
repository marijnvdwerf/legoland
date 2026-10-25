#pragma once

#include "legoland.h"

struct GoldNode {
    unsigned short key;
    unsigned char pad_2[0xa];
    struct GoldNode *next;
    unsigned char pad_10[0x1c];
};

struct Ride;
void GoldRush_GetInterfaces(struct Element *str, struct Ride *module);
