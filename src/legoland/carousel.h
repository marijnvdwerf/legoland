#pragma once

#include "legoland.h"

struct Bloke;

/* Carousel ride node (one 0x2c allocation), head at DAT_006160c4. */
struct CarouselNode {
    /* 0x00 */ struct CarouselNode *next;
    /* 0x04 */ unsigned short id;
    /* 0x06 */ unsigned char field_6;
    /* 0x07 */ unsigned char field_7;
    /* 0x08 */ unsigned char field_8;
    /* 0x09 */ unsigned char pad_9[0xc - 0x9];
    /* 0x0c */ unsigned int flags;
    /* 0x10 */ unsigned char field_10;
    /* 0x11 */ unsigned char pad_11[0x14 - 0x11];
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned char field_18;
    /* 0x19 */ unsigned char pad_19[0x1c - 0x19];
    /* 0x1c */ unsigned int field_1c;
    /* 0x20 */ unsigned char slots[0x2c - 0x20];
};

struct CarouselListElem {
    /* 0x00 */ struct CarouselListElem *next;
    /* 0x04 */ unsigned char pad_4[0x8 - 0x4];
    /* 0x08 */ struct Bloke *bloke;
    /* 0x0c */ unsigned short id;
};

struct CarouselRide {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ unsigned int x;
    /* 0x10 */ unsigned int y;
    /* 0x14 */ unsigned char pad_14[0x1c - 0x14];
    /* 0x1c */ unsigned int flags;
    /* 0x20 */ unsigned char pad_20[0x2e - 0x20];
    /* 0x2e */ unsigned short capacity;
    /* 0x30 */ unsigned char pad_30[0x64 - 0x30];
    /* 0x64 */ void *layer;
    /* 0x68 */ unsigned char pad_68[0xcc - 0x68];
    /* 0xcc */ struct CarouselListElem *list;
};

struct CarouselRideObj {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct CarouselRide *ride;
};

void FUN_0042bbc0(unsigned short *param_1);
void FUN_0042bc00(struct CarouselNode *node);
void FUN_0042bc40(void);
struct CarouselNode *FUN_0042bc60(unsigned short *param_1);
void FUN_0042bc90(struct CarouselNode *node);
void FUN_0042c210(struct CarouselNode *node);
void FUN_0042c800(void);
int FUN_0042cd20(struct CarouselListElem *elem, struct CarouselNode *node, signed char divisor);

void SetBlokePositionFromBNV();
void *NewBNVPath();
double GetUnitDepth();
int UpdateBlokeFromBNVPath();
char CalcMoveLine();
void NewDirForAction();
void BlokeSetFrame();
void UnAdjustBlokePosition();
LEGO_EXPORT void *LoadBinV(const char *filename);

void FUN_0042bcf0();
void FUN_0042c280();
void FUN_0042c3f0();
void FUN_0042c460();
void FUN_0042c4a0();
void FUN_0042c520();
unsigned int * FUN_0042c550(struct CarouselRideObj *param1, unsigned short param2);
int FUN_0042c590(void);
int FUN_0042c600(struct CarouselRideObj *param_1);
void FUN_0042c820();
