#pragma once

#include "legoland.h"

struct Cursor;

struct BoatRide {
    /* 0x000 */ unsigned short id;
    /* 0x002 */ unsigned char pad_2[2];
    /* 0x004 */ int field_4;
    /* 0x008 */ int field_8;
    /* 0x00c */ int field_c;
    /* 0x010 */ int field_10;
    /* 0x014 */ unsigned int field_14;
    /* 0x018 */ unsigned int field_18;
    /* 0x01c */ int field_1c[0xa0];
    /* 0x29c */ unsigned int field_29c[0x50];
    /* 0x3dc */ unsigned int field_3dc;
    /* 0x3e0 */ unsigned int field_3e0;
    /* 0x3e4 */ unsigned int field_3e4;
    /* 0x3e8 */ unsigned int field_3e8;
    /* 0x3ec */ unsigned int field_3ec;
    /* 0x3f0 */ struct BoatRide *next;
};

struct RideNode {
    /* 0x00 */ unsigned short id;
    /* 0x02 */ unsigned char field_2;
    /* 0x03 */ unsigned char field_3;
    /* 0x04 */ unsigned char field_4;
    /* 0x05 */ unsigned char field_5;
    /* 0x06 */ unsigned char pad_6[2];
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ unsigned int field_10;
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned int blokes[5];
    /* 0x2c */ struct RideNode *next;
    /* 0x30 */ unsigned int value;
};

struct PathNode {
    /* 0x00 */ unsigned char x;
    /* 0x01 */ unsigned char y;
    /* 0x02 */ unsigned short field_2;
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ struct PathNode *next;
    /* 0x14 */ struct PathNode *field_14;
    /* 0x18 */ struct PathNode *field_18;
};

struct MermaidNode {
    /* 0x00 */ unsigned short field_0;
    /* 0x02 */ unsigned short field_2;
    /* 0x04 */ struct MermaidNode *next;
};

struct BoatHolder {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct Cursor *cursor;
};

struct BoatTileMap {
    /* 0x00 */ unsigned char pad_0[4];
    /* 0x04 */ short *tiles;
};

struct RideBuilding {
    /* 0x00 */ unsigned char pad_0[0x3c];
    /* 0x3c */ unsigned int footprint[5];
};

struct RideObject {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct RideBuilding *building;
};

int FUN_00418e60(unsigned int param_1, unsigned int param_2);
void FUN_00418fe0(int param_1);
void FUN_00419300(void);
void FUN_004193c0(struct BoatRide *param_1);
struct BoatRide *FUN_00419420(struct BoatRide *param_1);
void FUN_00419520(struct BoatRide *param_1, int param_2);
void FUN_004198a0(struct BoatRide *param_1, unsigned int param_2, unsigned int param_3);
unsigned int FUN_004192d0(struct BoatRide *param_1);
void FUN_00418f90(struct BoatRide *param_1);
void FUN_0041b0d0(unsigned short id, unsigned int value);
struct PathNode *FUN_0041c890(unsigned int a, unsigned int b);
unsigned int FUN_0041c690(int param_1, int param_2, unsigned short *param_3);
void FUN_0041c4c0(int param_1, int param_2, int param_3, unsigned short *param_4);
void FUN_0041c620(void *param_1, unsigned short param_2, struct Cursor *param_3);
void FUN_0041bab0(int param_1, int param_2, unsigned short *param_3);
void FUN_0041caa0(unsigned short param_1);
void FUN_0041cb20(short param_1);
int FUN_0041c8c0(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void FUN_0041c940(int param_1, int param_2, int param_3, int param_4, short *param_5, int *param_6);
void FUN_0041b6f0(void *param_1, unsigned short param_2, struct Cursor *param_3);
void FUN_0041a3d0(void *param_1, unsigned int param_2);
void FUN_0041a530(int param_1, unsigned int param_2, int param_3);

int Find3DPersonFromBloke();
void SetPersonRotation();
void RestoreBaseMap();
int CheckForPeople();
int GetBlokeNum();
int GetBlokePtr();
void BlokeSitAnim();
void BlokeSetFrame();
char CalcMoveLine();
void NewDirForAction();
int ArcTan256();
int FUN_00458930(float x);
void RenderCursor();

void FUN_00419d10();
void FUN_00419ef0();
void FUN_0041a000();
void FUN_0041a040();
void FUN_0041a2f0();
void FUN_0041a720();
void FUN_0041abd0();
int FUN_0041acf0(void);
int FUN_0041aee0(void);
int FUN_0041b100(int dummy, int arg);
void FUN_0041b250();
void FUN_0041b260();
void FUN_0041b2a0();
void FUN_0041b4c0();
unsigned int FUN_0041b6d0(unsigned int param_1, unsigned int param_2);
void FUN_0041b830();
void FUN_0041b880();
void FUN_0041b8e0();
void FUN_0041bd40();
void FUN_0041bfb0();
void FUN_0041c130();
