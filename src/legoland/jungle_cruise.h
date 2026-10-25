#pragma once

#include "llidb.h"

struct Cursor;

struct Bloke;

struct JungleRide {
    /* 0x000 */ TileId tile;
    /* 0x002 */ unsigned char field_2;
    /* 0x003 */ unsigned char field_3;
    /* 0x004 */ unsigned int cur_x;
    /* 0x008 */ unsigned int cur_y;
    /* 0x00c */ unsigned int next_x;
    /* 0x010 */ unsigned int next_y;
    /* 0x014 */ unsigned int screen_x;
    /* 0x018 */ unsigned int screen_y;
    /* 0x01c */ int step_offsets[0xa0];
    /* 0x29c */ unsigned int step_frames[0x50];
    /* 0x3dc */ unsigned int from_dir;
    /* 0x3e0 */ unsigned int field_3e0;
    /* 0x3e4 */ unsigned int field_3e4;
    /* 0x3e8 */ struct Bloke *blokes[3];
    /* 0x3f4 */ struct JungleRide *next;
};

struct JungleScore {
    /* 0x00 */ TileId tile;
    /* 0x02 */ TileId start;
    /* 0x04 */ TileId end;
    /* 0x06 */ unsigned char pad_6[2];
    /* 0x08 */ unsigned int connected;
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ unsigned int field_10;
    /* 0x14 */ unsigned int bloke_count;
    /* 0x18 */ struct Bloke *blokes[5];
    /* 0x2c */ int timer;
    /* 0x30 */ struct Bloke *seats[3];
    /* 0x3c */ struct JungleScore *next;
    /* 0x40 */ int field_40;
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
    /* 0x02 */ TileId owner;
    /* 0x04 */ unsigned int dir_mask;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ unsigned int visited;
    /* 0x10 */ struct JunglePath *next;
    /* 0x14 */ struct JunglePath *bfs_next;
    /* 0x18 */ struct JunglePath *parent;
};

void InitJungleCruiseSeatOffsets(void);
void DrawJungleCruiseBoats(int param_1);
int AddJungleRide(TileId tile, struct Bloke *bloke0, struct Bloke *bloke1, struct Bloke *bloke2);
void RemoveJungleRide(struct JungleRide *param_1);
unsigned int TagAndCountJungleCruiseBoats(unsigned short *param_1);
void UpdateJungleRides(void);
void LaunchJungleCruiseBoat(struct JungleRide *param_1);
struct JungleRide *DockJungleCruiseBoat(struct JungleRide *param_1);
void SteerJungleCruiseBoat(struct JungleRide *param_1, int param_2);
void JungleCruiseBuildStepPath(struct JungleRide *ride, int from, int to);
void AddJungleScore(unsigned short owner, int delta);
struct RideNode;
void BrowseAndBuyItem(struct RideNode *node, Element *obj, TileId *tile, int sfx);
void AddJunglePathTile(int x, int y, int mask, unsigned short *owner);
void RemoveJunglePath(void *param_1, TileId tile, struct Cursor *param_3);
unsigned int GetJunglePathNeighbourMask(int param_1, int param_2, unsigned short *param_3);
struct JunglePath *FindJunglePathAt(int x, int y);
int AreJunglePathsConnected(int a, int b, int c, int d);
void SearchJunglePathConnected(int x, int y, int tx, int ty, TileId *owner, int *found);
void ComputeJunglePathDistances(unsigned short param_1);
void ComputeJunglePathDistancesStep(short param_1);
void FillJungleWaterCorners(int param_1, int param_2, unsigned short *param_3);
void MonkeyTreeRemoveObject(void *param_1, TileId tile, struct Cursor *param_3);
void MonkeyFishRemoveObject(void *param_1, TileId tile, struct Cursor *param_3);
void RemoveJungleCruiseInstance(void *param_1, TileId tile, struct Cursor *param_3);
int JungleCruise_Save(struct Element *element);
int JungleCruise_Load(struct Element *element);
void JungleCruiseRemoveObject(Element *obj, TileId tile, struct Cursor *cursor);

void LoadBrijMaskSprite(Element *obj);
void KillBrijMaskSprite();
void MonkeyTreeSetEditMode();
struct Element;
void MonkeyTreeAddObject(struct Element *param_1, int *param_2);
void JungleCruiseMonkeyTreeCalcCursor(Element *obj, int *param_2, unsigned int param_3);
struct Point;
unsigned int JungleCruiseMonkeyTreeDCalcCursor(struct Element *param_1, struct Point *param_2);
struct RideSpriteInfo *GetMonkeyTreeSpriteInfo(Element *obj, unsigned short param_2);
void LoadMFish2Sprite(Element *obj);
void KillMFish2Sprite();
void MonkeyFishSetEditMode();
struct Ride;
void MonkeyFishAddObject(struct Element *obj, int *coords);
void JungleCruiseMonkeyFishCalcCursor(Element *obj, int *param_2, unsigned int param_3);
unsigned int JungleCruiseMonkeyFishDCalcCursor(struct Element *param_1, struct Point *param_2);
struct RideSpriteInfo *GetMonkeyFishSpriteInfo(Element *obj, unsigned short param_2);
void LoadJungleCruiseResources(Element *obj);
void UnloadJungleCruiseResources(Element *obj);
void JungleCruiseSetEditMode();
void JungleCruiseAddObject(struct Element *obj, int *coords);
void JungleCruiseCalcCursor(Element *obj, int *param_2, unsigned int param_3);
void JungleCruiseDCalcCursor(struct Element *param_1, struct Point *param_2);
void JungleCruiseUpdate();
void RenderJungleCruise(struct Element *param_1, unsigned int param_2, unsigned int param_3, short *param_4, void *param_5, unsigned int param_6);
int GetBestJungleScore(Element *obj, int param_2);
void InitJungleCruiseWater(Element *obj);
void JungleCruiseWaterSetEditMode();
void JungleCruiseWaterCalcCursor(Element *obj, int *param_2, unsigned int param_3);
void JungleCruiseWaterDCalcCursor(struct Element *param_1, int *coords);
void JungleCruiseWaterAddObject(Element *obj, int *coords);
void JungleCruiseWaterRemoveObject(Element *obj, TileId tile, struct Cursor *cursor);
