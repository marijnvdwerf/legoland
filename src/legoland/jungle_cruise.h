#pragma once

struct Cursor;
struct RideObject;

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
    /* 0x02 */ TileId start;
    /* 0x04 */ TileId end;
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
    /* 0x00 */ TileId tile;
    /* 0x02 */ unsigned short owner;
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ struct JungleFish *next;
};

struct JungleObj {
    /* 0x00 */ TileId tile;
    /* 0x02 */ unsigned short owner;
    /* 0x04 */ struct JungleObj *next;
};

struct JunglePath {
    /* 0x00 */ TileId tile;
    /* 0x02 */ unsigned short owner;
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ struct JunglePath *next;
    /* 0x14 */ struct JunglePath *field_14;
    /* 0x18 */ struct JunglePath *field_18;
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
void FUN_00436130(unsigned short owner, int delta);
void FUN_00437570(struct RideNode *node, struct RideObject *obj, TileId *tile, int sfx);
void FUN_00436dc0(int x, int y, int mask, unsigned short *owner);
void FUN_00436f30(void *param_1, TileId tile, struct Cursor *param_3);
unsigned int FUN_00436fb0(int param_1, int param_2, unsigned short *param_3);
struct JunglePath *FUN_004371b0(int x, int y);
int FUN_004371e0(int a, int b, int c, int d);
void FUN_00437260(int param_1, int param_2, int param_3, int param_4, unsigned short *param_5, int *param_6);
void FUN_004373c0(unsigned short param_1);
void FUN_00437440(short param_1);
void FUN_004367b0(int param_1, int param_2, unsigned short *param_3);
void FUN_00433fc0(void *param_1, TileId tile, struct Cursor *param_3);
void FUN_00434670(void *param_1, TileId tile, struct Cursor *param_3);
void FUN_00434b40(void *param_1, TileId tile, struct Cursor *param_3);
int FUN_00435c70(void);
int FUN_00435ec0(void);
void FUN_00435470(struct RideObject *obj, TileId tile, struct Cursor *cursor);

void FUN_00433ca0(struct RideObject *obj);
void FUN_00433cd0();
void FUN_00433ce0();
void FUN_00433d20(unsigned int param_1, int *param_2);
void FUN_00433d90(struct RideObject *obj, unsigned int param_2, unsigned int param_3);
unsigned int FUN_00433fa0(unsigned int param_1, unsigned int param_2);
struct RideSpriteInfo *FUN_00434040(struct RideObject *obj, unsigned short param_2);
void FUN_00434080(struct RideObject *obj);
void FUN_004340b0();
void FUN_004340c0();
void FUN_00434100(struct EditObject *obj, int *coords);
void FUN_00434330(int *param_1, unsigned int param_2, int *param_3);
unsigned int FUN_00434650(unsigned int param_1, unsigned int param_2);
struct RideSpriteInfo *FUN_00434740(struct RideObject *obj, short param_2);
void FUN_00434cb0(struct RideObject *obj);
void FUN_00434e50(struct RideObject *obj);
void FUN_00434f50();
void FUN_00434f90(struct EditObject *obj, int *coords);
void FUN_00435150(struct RideObject *obj, unsigned int param_2, unsigned int param_3);
void FUN_00435230(unsigned int param_1, struct Point *param_2);
void FUN_00435750();
void FUN_00435bd0(int param_1, unsigned int param_2, unsigned int param_3, short *param_4, unsigned int param_5, unsigned int param_6);
int FUN_00436160(struct RideObject *obj, int param_2);
void FUN_00436190(struct RideObject *obj);
void FUN_004361a0();
void FUN_00436200(struct RideObject *obj, unsigned int param_2, unsigned int param_3);
void FUN_00436470(unsigned int param_1, unsigned int *param_2);
void FUN_004365f0(struct RideObject *obj, int *coords);
void FUN_00436a40(struct RideObject *obj, TileId tile, struct Cursor *cursor);
