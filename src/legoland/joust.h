#pragma once

#include "legoland.h"
#include "objclass.h"

#pragma pack(push, 1)
struct JoustSub12 {
    union {
        struct {
            unsigned int a;
            unsigned short b;
        };
        unsigned char v[6];
    };
};

struct JoustPair {
    char c[2];
};

struct JoustNode {
    TileId id;
    unsigned char pad_2[2];
    struct JoustNode *next;
    struct Sample *sample;
    int busy;
    char x;
    char y;
    struct JoustSub12 sub12;
    struct JoustPair pair;
    char phase;
    char frame;
    unsigned int field_1c;
    unsigned int field_20;
};
#pragma pack(pop)

struct Ride;
LEGO_EXPORT void Joust_GetInterfaces(struct Element *head, struct Ride *iface);
