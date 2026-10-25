#pragma once

#include "llidb.h"

struct Seats {
    unsigned char c[3];
};

struct BrollyNode {
    /* 0x00 */ struct BrollyNode *next;
    /* 0x04 */ unsigned short value;
    /* 0x06 */ struct Seats seats;
    /* 0x09 */ unsigned char field_9;
};

struct SaveBlock {
    /* 0x00 */ struct SaveBlock *next;
    /* 0x04 */ unsigned short value;
    /* 0x06 */ unsigned char field_6;
    /* 0x07 */ unsigned char field_7;
    /* 0x08 */ unsigned char field_8;
    /* 0x09 */ unsigned char field_9;
    /* 0x0a */ unsigned char pad_a[0xc - 0xa];
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ unsigned char field_10;
    /* 0x11 */ unsigned char field_11;
    /* 0x12 */ unsigned char pad_12[0x14 - 0x12];
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned int field_18;
    /* 0x1c */ unsigned int field_1c;
    /* 0x20 */ unsigned int field_20;
    /* 0x24 */ unsigned int field_24;
    /* 0x28 */ unsigned int field_28;
    /* 0x2c */ unsigned int field_2c;
    /* 0x30 */ unsigned int field_30;
    /* 0x34 */ unsigned int field_34;
    /* 0x38 */ unsigned int field_38;
    /* 0x3c */ unsigned int field_3c;
};

struct RideNode;
struct Element;
struct UserFlagsArg;

struct SaveBlock;
struct SaveBlock *FindSaveBlock(unsigned short *param);

void LoadChuckWagonResources(struct Element *obj);
void UnloadChuckWagonResources();
void RenderChuckWagon(struct Element *arg, unsigned int param2, unsigned int param3, unsigned short *value);
void ChuckWagonUpdate(Element *obj);
void LoadBrollyImages(struct Element *obj);
void UnloadBrollyImages();
void BrollySetEditMode();
void BrollyAddObject(struct Element *param_1, unsigned char *param_2);
struct RideSpriteInfo *GetBrollySpriteInfo(struct Element *param_1, unsigned short param_2);
void LoadSharkCafeResources(struct Element *obj);
void UnloadSharkCafeResources();
void SharkCafeUpdate(Element *obj);
void LoadFoodcartDrinkResources(struct Element *obj);
void UnloadFoodcartDrinkResources();
void LoadFoodcartIcecreamResources(struct Element *obj);
void UnloadFoodcartIcecreamResources();
void LoadFoodcartFoodResources(struct Element *obj);
void UnloadFoodcartFoodResources();
void RenderFoodStallCustomers(struct Element *arg, unsigned int param2, unsigned int param3, unsigned short *value);
void LoadCastleBBQResources(struct Element *obj);
void UnloadCastleBBQResources();
void EaterySetEditMode(struct Element *arg);
void RenderCastleBBQ(struct Element *param_1, unsigned int param_2, unsigned int param_3, short *param_4, void *param_5, unsigned int param_6);
void CastleBBQAddObject(struct Element *param_1, unsigned int *param_2);
struct Cursor;
void CastleBBQRemoveObject(struct Element *param_1, unsigned int param_2, struct Cursor *param_3);
void CastleBBQUpdate(Element *obj);
void FoodcartDrinkUpdate(Element *obj);
void FoodcartFoodUpdate(Element *obj);
void Restaurant1AddObject(struct Element *param_1, unsigned char *param_2);
void Restaurant1RemoveObject(struct Element *param_1, TileId tile, struct Cursor *param_3);
void LoadRestaurant1Resources(struct Element *obj);
void Restaurant1Update(Element *obj);
void RenderRestaurant1(struct Element *param_1, unsigned int param_2, unsigned int param_3, short *param_4, void *param_5, unsigned int param_6);
void KillRestMaskSpritesAndMoneySFX();
void LoadRestaurant2Resources(struct Element *obj);
void Restaurant2AddObject(struct Element *param_1, unsigned char *param_2);
void Restaurant2RemoveObject(struct Element *arg1, TileId tile, struct Cursor *arg3, unsigned int arg4, unsigned int arg5);
void Restaurant2Update(Element *obj);
struct RideSpriteInfo *GetRestaurant2SpriteInfo(struct Element *obj, unsigned short a2);
void RenderRestaurant2(struct Element *param_1, unsigned int param_2, unsigned int param_3, short *param_4, void *param_5, unsigned int param_6);
void UnloadRestaurant2Resources();
void FoodcartIcecreamUpdate(Element *obj);
void EateryRemoveObject(struct Element *param_1, TileId tile, struct Cursor *param_3);
void LoadOctopusCafeResources(struct Element *obj);
void OctopusCafeAddObject(struct Element *param_1, struct UserFlagsArg *param_2);
void UnloadOctopusCafeResources();
void OctopusCafeUpdate(Element *obj);
void RenderOctopusCafe(struct Element *param_1, unsigned int param_2, unsigned int param_3, unsigned char *param_4, void *param_5, unsigned int param_6);
int Restaurant1_Save(struct Element *element);
int Restaurant1_Load(struct Element *element);
int Restaurant2_Save(struct Element *element);
int Restaurant2_Load(struct Element *element);