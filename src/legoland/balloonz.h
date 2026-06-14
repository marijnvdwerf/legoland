#pragma once

#include "legoland.h"

struct Bloke;

/* Balloonz ride passenger node (one 0x20 allocation), head at DAT_00616060.
 * Several middle fields (field_d, field_11) are 4-/2-byte values at unaligned
 * offsets, so the node body is kept as raw bytes and reached with offset casts. */
struct BalloonNode {
    /* 0x00 */ struct BalloonNode *next;
    /* 0x04 */ unsigned short id;
    /* 0x06 */ unsigned char body[0x20 - 0x6];
};

void FUN_0042a8f0(unsigned short *param_1);
struct BalloonNode *FUN_0042a980(unsigned short *param_1);
void FUN_0042a9b0(struct BalloonNode *param_1);
void FUN_0042a9f0(void);
int FUN_0042aa60(char param_1, char param_2);

void SetBlokePositionFromBNV();
double GetUnitDepth();
char CalcMoveLine();
void NewDirForAction();
LEGO_EXPORT void *LoadBinV(const char *filename);

void FUN_0042a7b0();
void FUN_0042a950();
void FUN_0042aa10();
void FUN_0042aa90();
unsigned int * FUN_0042b2a0(struct BalloonRideObj *arg1, unsigned short arg2);
void FUN_0042b2e0();
void FUN_0042b9d0();
void FUN_0042ba40();
unsigned int FUN_0042ba80(void);
unsigned int FUN_0042baf0(struct BalloonRideObj *param_1);