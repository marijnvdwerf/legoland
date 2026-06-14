#pragma once

struct Cursor;

struct JungleRide {
    /* 0x000 */ unsigned short field_0;
    /* 0x002 */ unsigned char field_2;
    /* 0x003 */ unsigned char field_3;
    /* 0x004 */ unsigned int field_4;
    /* 0x008 */ unsigned int field_8;
    /* 0x00c */ unsigned int field_c;
    /* 0x010 */ unsigned int field_10;
    /* 0x014 */ unsigned int field_14;
    /* 0x018 */ unsigned int field_18;
    /* 0x01c */ int field_1c[0xa0];
    /* 0x29c */ unsigned int field_29c[0x50];
    /* 0x3dc */ unsigned int field_3dc;
    /* 0x3e0 */ unsigned int field_3e0;
    /* 0x3e4 */ unsigned int field_3e4;
    /* 0x3e8 */ unsigned int blokes[3];
    /* 0x3f4 */ struct JungleRide *next;
};

struct JungleScore {
    /* 0x00 */ unsigned short field_0;
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
    /* 0x2c */ unsigned int field_2c;
    /* 0x30 */ unsigned int field_30[3];
    /* 0x3c */ struct JungleScore *next;
    /* 0x40 */ unsigned int field_40;
};

struct JungleFish {
    /* 0x00 */ unsigned short field_0;
    /* 0x02 */ unsigned short field_2;
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ struct JungleFish *next;
};

struct JungleObj {
    /* 0x00 */ unsigned short field_0;
    /* 0x02 */ unsigned short field_2;
    /* 0x04 */ struct JungleObj *next;
};

struct JunglePath {
    /* 0x00 */ unsigned char x;
    /* 0x01 */ unsigned char y;
    /* 0x02 */ unsigned short field_2;
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ struct JunglePath *next;
    /* 0x14 */ struct JunglePath *field_14;
    /* 0x18 */ struct JunglePath *field_18;
};

struct JungleHolder {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct Cursor *cursor;
};

struct JungleTileMap {
    /* 0x00 */ unsigned char pad_0[4];
    /* 0x04 */ short *tiles;
};

void FUN_00432ac0(void);
void FUN_00432d00(int param_1);
int FUN_00432b90(unsigned int param_1, void *param_2, void *param_3, void *param_4);
void FUN_00432cb0(struct JungleRide *param_1);
unsigned int FUN_004332c0(unsigned short *param_1);
void FUN_004332f0(void);
void FUN_004333b0(struct JungleRide *param_1);
struct JungleRide *FUN_004333e0(struct JungleRide *param_1);
void FUN_004334c0(struct JungleRide *param_1, int param_2);
void FUN_00433840(struct JungleRide *param_1, unsigned int param_2, unsigned int param_3);
void FUN_00436130(unsigned short param_1, unsigned int param_2);
void FUN_00436dc0(int param_1, int param_2, int param_3, unsigned short *param_4);
void FUN_00436f30(void *param_1, unsigned short param_2, struct Cursor *param_3);
unsigned int FUN_00436fb0(int param_1, int param_2, unsigned short *param_3);
struct JunglePath *FUN_004371b0(unsigned char param_1, unsigned char param_2);
unsigned int FUN_004371e0(unsigned char param_1, unsigned char param_2, unsigned char param_3, unsigned char param_4);
void FUN_00437260(int param_1, int param_2, int param_3, int param_4, unsigned short *param_5, int *param_6);
void FUN_004373c0(unsigned short param_1);
void FUN_00437440(short param_1);
void FUN_004367b0(int param_1, int param_2, unsigned short *param_3);
void FUN_00433fc0(void *param_1, unsigned short param_2, void *param_3);
void FUN_00434670(void *param_1, unsigned short param_2, struct Cursor *param_3);
void FUN_00434b40(void *param_1, unsigned short param_2, struct Cursor *param_3);
int FUN_00435c70(void);
int FUN_00435ec0(void);
void FUN_00435470(void *param_1, unsigned int param_2, struct Cursor *param_3);

int Find3DPersonFromBloke();
void SetPersonRotation();
void RestoreBaseMap();
void RenderCursor();
void IncrementObjectCount();
int CheckForPeople();
int GetBlokeNum();
int GetBlokePtr();
void BlokeSitAnim();
void BlokeSetFrame();
char CalcMoveLine();
void NewDirForAction();
void BuyItem();
int ArcTan256();
int FUN_00458930(float x);
unsigned char FUN_0045f4b0(struct Cursor *cursor);
void FUN_0045f460(struct Cursor *cursor);
void FUN_0045f480(struct Cursor *cursor, int param);
