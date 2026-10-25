#pragma once

#include "legoland.h"
#include "objclass.h"

struct RideNode;
struct Sprite;

struct CopterLayer {
    /* 0x00 */ unsigned int flags;
    /* 0x04 */ char frame;
    /* 0x05 */ unsigned char pad_5[3];
    /* 0x08 */ int idle_layer;
    /* 0x0c */ int active_layer;
    /* 0x10 */ int field_10;
    /* 0x14 */ int field_14;
    /* 0x18 */ struct RideNode *rider;
    /* 0x1c */ char frame_count;
    /* 0x1d */ char field_1d;
    /* 0x1e */ unsigned char pad_1e[2];
};

struct CopterNode {
    union {
        /* 0x00 */ unsigned short field_0;
        struct {
            unsigned char pad_0;
            /* 0x01 */ unsigned char field_1;
        };
    };
    /* 0x02 */ unsigned char field_2;
    /* 0x03 */ unsigned char field_3;
    /* 0x04 */ struct CopterNode *next;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ int field_c;
    /* 0x10 */ unsigned char field_10;
    /* 0x11 */ unsigned char pad_11[3];
    /* 0x14 */ int field_14;
    /* 0x18 */ struct CopterLayer layer[6];
};

struct CopterSource;
unsigned int CoptersFindChainIndex(struct RideNode *node, struct CopterSource *id);
void CoptersInitNode(struct CopterNode *node);
void CoptersRenderLayer(struct CopterNode *node, int index, unsigned int param_3);
void ResetCoptersRide(struct CopterNode *node, int param);
void CoptersPlaceRider(struct CopterNode *node, int index);

struct Ride;
void CoptersRide(struct Element *name, struct Ride *interfaces);
