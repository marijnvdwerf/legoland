#pragma once

#include "legoland.h"

struct Element;
struct Ride;

struct Cursor;

/* Earth-slide class node (one 0x24 allocation), head at EarthNodeHead. */
struct EarthNode {
    /* 0x00 */ unsigned short id;
    /* 0x02 */ unsigned char pad_2[0x4 - 0x2];
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ unsigned char field_8;
    /* 0x09 */ unsigned char field_9;
    /* 0x0a */ unsigned char field_a;
    /* 0x0b */ unsigned char field_b;
    /* 0x0c */ struct EarthNode *next;
    /* 0x10 */ unsigned int field_10;
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned char queue_count;
    /* 0x19 */ unsigned char pad_19[0x1c - 0x19];
    /* 0x1c */ struct EarthQueue *queue_head;
    /* 0x20 */ struct EarthQueue *queue_tail;
};

/* Per-node bloke queue element (8-byte allocation). */
struct EarthQueue {
    /* 0x00 */ struct EarthQueue *next;
    /* 0x04 */ struct RideNode *elem;
};

/* Bloke-list element hung off the ride object (Cursor) at offset 0xcc. */

/* Callback argument; field_c points at the ride/cursor object (EarthSlideRide). */

void AddEarthNode(unsigned short *a1);
void RemoveEarthNode(struct EarthNode *node);
struct EarthNode *FindEarthNode(volatile unsigned short *param_1);
void EnqueueEarthRider(struct EarthNode *node, struct RideNode *elem);
void EarthNodeAppendQueueEntry(struct EarthNode *p, struct EarthQueue *value);
void GetEarthSlideQueuePosition(struct Ride *param_1, unsigned char *param_2, int *param_3);
int IsEarthQueueHead(struct EarthNode *param_1, void *param_2);
void AdvanceEarthSlideQueue(struct EarthNode *param_1);
void DequeueEarthRider(struct EarthNode *list);
unsigned int CountEarthBlokeElemsUntil(struct RideNode *param_1, struct RideNode *param_2);
struct RideNode *GetNthNextEarthBlokeElem(struct RideNode *param_1, unsigned int param_2);
void UpdateEarthNodes(void);

void RenderEarthSlide(struct Element *param_1, unsigned int param_2, unsigned int param_3, unsigned char *param_4, void *param_5, unsigned int param_6);
void LoadEarthSlideResources(struct Element *param_1);
void UnloadEarthSlideResources(struct Element *arg1);
void EarthSlideSetEditMode();
void EarthSlideRemoveObject(struct Element *param_1, TileId tile, struct Cursor *param_3);
void EarthSlideAddObject(struct Element *param_1, unsigned char *param_2);
int EarthSlideRide_Save(struct Element *element);
int EarthSlideRide_Load(struct Element *element);
void EarthSlideRideUpdate(struct Element *obj);
