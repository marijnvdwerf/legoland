#pragma once

#include "legoland.h"

struct WaterNode {
    /* 0x00 */ struct WaterNode *next;
    /* 0x04 */ unsigned short key;
    /* 0x06 */ unsigned char pad_6[2];
    /* 0x08 */ unsigned char state; /* 0 idle, 1 playing; showers: 2 running, 3 stopping */
    /* 0x09 */ unsigned char frame;
    /* 0x0a */ unsigned char block_index;
    /* 0x0b */ unsigned char pad_b[1];
};

struct Ride;
void WaterWorksGetInterfaces(struct Element *element, struct Ride *ci);
