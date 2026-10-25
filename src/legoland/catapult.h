#pragma once

#include "legoland.h"

/* A catapult on the map: the riders in its four seats and their launches. 0x3c bytes. */
struct CatapultNode {
    /* 0x00 */ union {
        unsigned short tile_id;
        struct {
            unsigned char field_0; /* x */
            unsigned char field_1; /* y */
        };
    };
    /* 0x02 */ unsigned char pad_2[2];
    /* 0x04 */ struct CatapultNode *next;
    /* 0x08 */ unsigned int field_8; /* 1: launching */
    /* 0x0c */ signed char mode;
    /* 0x0d */ signed char field_d; /* launch frame */
    /* 0x0e */ unsigned char pad_e[2];
    /* 0x10 */ struct RideNode *slots[4]; /* in the saved game: 1 + the rider's index in the ride's list, or 0 */
    /* 0x20 */ unsigned char active[4];
    /* 0x24 */ signed char frame[4];
    /* 0x28 */ unsigned int flags[4];
    /* 0x38 */ unsigned char pad_38[4];
};

struct Ride;
void Catapult_GetInterfaces(struct Element *name, struct Ride *ci);
