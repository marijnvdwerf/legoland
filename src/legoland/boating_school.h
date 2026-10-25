#pragma once

#include "legoland.h"
#include "llidb.h"
#include "obj_instance.h"

struct Cursor;

struct BoatRide {
    /* 0x000 */ unsigned short id;
    /* 0x002 */ unsigned char pad_2[2];
    /* 0x004 */ int tile_x;
    /* 0x008 */ int tile_y;
    /* 0x00c */ int next_x;
    /* 0x010 */ int next_y;
    /* 0x014 */ unsigned int screen_x;
    /* 0x018 */ unsigned int screen_y;
    /* 0x01c */ int step_xy[0xa0];
    /* 0x29c */ unsigned int step_sprite[0x50];
    /* 0x3dc */ unsigned int field_3dc;
    /* 0x3e0 */ unsigned int field_3e0;
    /* 0x3e4 */ unsigned int field_3e4;
    /* 0x3e8 */ unsigned int field_3e8;
    /* 0x3ec */ struct Bloke *bloke;
    /* 0x3f0 */ struct BoatRide *next;
};

struct BoatRideNode {
    /* 0x00 */ unsigned short id;
    /* 0x02 */ TileId start;
    /* 0x04 */ TileId end;
    /* 0x06 */ unsigned char pad_6[2];
    /* 0x08 */ unsigned int connected;
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ unsigned int field_10;
    /* 0x14 */ unsigned int bloke_count;
    /* 0x18 */ struct Bloke *blokes[5]; /* indices (GetBlokeNum) in a save */
    /* 0x2c */ struct BoatRideNode *next;
    /* 0x30 */ unsigned int value;
};

struct PathNode {
    /* 0x00 */ TileId tile;
    /* 0x02 */ TileId owner;
    /* 0x04 */ unsigned int dir_mask;
    /* 0x08 */ unsigned int dist_to_end;
    /* 0x0c */ unsigned int visited;
    /* 0x10 */ struct PathNode *next;
    /* 0x14 */ struct PathNode *wave_next;
    /* 0x18 */ struct PathNode *parent;
};

struct MermaidNode {
    /* 0x00 */ TileId tile;
    /* 0x02 */ unsigned short owner;
    /* 0x04 */ struct MermaidNode *next;
};

/* LLIDB "BOATING SCHOOL BOATS" data: boat sprites with per-frame screen offsets. */
int AddBoatRide(TileId tile, struct Bloke *bloke);
void RenderBoatingSchoolBoats(int param_1);
void UpdateBoatRides(void);
void LaunchBoatingSchoolBoat(struct BoatRide *param_1);
struct BoatRide *DockBoatingSchoolBoat(struct BoatRide *param_1);
void SteerBoatingSchoolBoat(struct BoatRide *param_1, int param_2);
void BoatingSchoolBuildStepPath(struct BoatRide *ride, int from, int to);
unsigned int TagAndCountBoatingSchoolBoats(void *param_1);
void RemoveBoatRide(struct BoatRide *param_1);
void AddBoatRideNodeValue(unsigned short id, unsigned int value);
struct PathNode *FindBoatPathAt(unsigned int a, unsigned int b);
unsigned int GetBoatPathNeighbourMask(int param_1, int param_2, unsigned short *param_3);
void AddBoatPathTile(int param_1, int param_2, int param_3, unsigned short *param_4);
void RemoveBoatPath(void *param_1, TileId tile, struct Cursor *param_3);
void FillBoatingWaterCorners(int param_1, int param_2, unsigned short *param_3);
void ComputeBoatPathDistances(unsigned short param_1);
void ComputeBoatPathDistancesStep(short param_1);
int IsBoatPathConnected(int a, int b, int c, int d);
void SearchBoatPathConnected(int x, int y, int tx, int ty, TileId *owner, int *found);
void BoatingSchoolMermaidRemoveObject(void *param_1, TileId tile, struct Cursor *param_3);
struct Point;
void BoatingSchoolDCalcCursor(void *param_1, struct Point *param_2);
void BoatingSchoolRemoveObject(Element *obj, TileId tile, struct Cursor *cursor);

void LoadBoatingSchoolResources(Element *obj);
void UnloadBoatingSchoolResources();
void SetBoatingSchoolEditCursor();
struct Ride;
void BoatingSchoolAddObject(struct Element *obj, int *coords);
void BoatingSchoolCalcCursor(struct Element *param_1, int *param_2, unsigned int param_3);
void BoatingSchoolUpdate();
void RenderBoatingSchool(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int clip);
int BoatingSchool_Save(struct Element *element);
int BoatingSchool_Load(struct Element *element);
int GetBestBoatRideScore(struct Element *dummy, int arg);
void InitBoatingSchoolMermaid(Element *param_1);
void BoatingSchoolMermaidSetEditMode();
void BoatingSchoolMermaidAddObject(struct Element *obj, int *coords);
void BoatingSchoolMermaidCalcCursor(Element *obj, int *param_2, unsigned int param_3);
struct Element;
unsigned int BoatingSchoolMermaidDCalcCursor(struct Element *param_1, struct Point *param_2);
void BoatingSchoolWaterLoad(Element *arg);
void BoatingSchoolSetEditMode();
void BoatingSchoolWaterAddObject(Element *obj, int *coords);
void BoatingSchoolWaterCalcCursor(Element *obj, int *param_2, unsigned int param_3);
void BoatingSchoolWaterDCalcCursor(struct Element *param_1, int *coords);
void BoatingSchoolWaterRemoveObject(Element *obj, TileId tile, struct Cursor *cursor);
