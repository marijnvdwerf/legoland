#pragma once

#include "obj_instance.h"

struct RoadPlaceArg;
struct NeighborResult;
struct RideQueueEntry;
struct Sprite;

struct RoadLights {
    unsigned char pad_0[0x8];
    struct Sprite **sprites;
    int *off_x;
    int *off_y;
};

unsigned int CountRoadEntriesWithId(unsigned short param_1);
void AddRoadEntry(TileId tile, int param_2, int param_3, unsigned int param_4, unsigned int param_5);
void RemoveRoadEntry(int param_1, int param_2);
void UpdateQueuePathShape(unsigned short param_1, int param_2, int param_3);

struct RideQueueEntry;
int GetDiagonalRoadNeighbours(int x, int y, struct RideQueueEntry **out);
struct RideQueueEntry *FilterRoadStartNeighbour(int arg1, int arg2, struct RideQueueEntry *tile);
int GetConnectedRoadNeighbours(int x, int y, struct NeighborResult *out);
int GetRoadNeighbours(int x, int y, struct NeighborResult *out);

void LoadDrivingSchoolRoadsResources(struct Element *head);
void UnloadDrivingSchoolRoadsResources();
void DrivingSchoolRoadsSetEditMode();
void DrivingSchoolRoadsCalcCursor(Element *obj, int *param_2, unsigned int param_3);
struct Cursor;
struct RideQueueEntry *SnapCursorToRoadNeighbour(struct Cursor *cur);
void RoadSetQueryCursor(struct Element *dummy, struct RoadPlaceArg *param);
void DrivingSchoolRoadsAddObject(struct Element *edit, struct RoadPlaceArg *place);
void RoadRemoveObject(Element *edit, TileId tile, struct Cursor *cursor);
void UpdateTrafficLights(void);
void ZebraCrossingSetEditMode();
void ZebraCrossingCalcCursor(struct Element *param_1, int *param_2, unsigned int param_3);
void InitZebraCrossing(struct Element *param_1);
void ZebraCrossingAddObject(struct Element *edit, struct RoadPlaceArg *place);
