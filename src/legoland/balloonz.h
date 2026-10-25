#pragma once

#include "legoland.h"
#include "llidb.h"

#include "gamemap.h"
#include "obj_instance.h"

typedef struct BalloonNode BalloonNode;

/* Per-balloon-ride state (one 0x20 allocation), list head at BalloonNodeList.
   The ride has six cars on a 24-step wheel; BalloonzCarAtPos maps (pos, lap) to a car. */
struct BalloonNode {
    /* 0x00 */ BalloonNode *next;
    /* 0x04 */ TileId tile;
    /* 0x06 */ unsigned char pad_6[2];
    /* 0x08 */ int queued; /* blokes waiting at the platform */
    /* 0x0c */ char riders; /* blokes in a car */
    /* 0x0d */ char cars[6]; /* 0 empty, 1 boarding, 2 riding, 3 done */
    /* 0x13 */ char lap; /* 0..1 */
    /* 0x14 */ char pos; /* wheel step, 0..23 */
    /* 0x15 */ char frame; /* wheel frame drawn this tick */
    /* 0x16 */ char anim; /* base animation frame, 0..48 */
    /* 0x17 */ char leaving; /* blokes waiting to get off */
    /* 0x18 */ int can_board;
    /* 0x1c */ int can_unload;
};

void AddBalloonNode(TileId *tile);
BalloonNode *FindBalloonNode(TileId *tile);
void RemoveBalloonNode(BalloonNode *node);
void RemoveAllBalloonNodes(void);
int BalloonzCarAtPos(char pos, char lap);

void LoadBalloonzResources(Element *obj);
void BalloonzAddObject(Element *obj, int *coords);
void BalloonzRemoveObject(Element *obj, TileId tile, Cursor *cursor);
void BalloonzUpdate(Element *obj);
RideSpriteInfo *GetBalloonzSpriteInfo(Element *obj, unsigned short id);
void RenderBalloonz(Element *obj, void *param_2, void *param_3, TileId *tile, void *param_5, unsigned int param_6);
void UnloadBalloonzResources(void);
void BalloonzSetEditMode(void);
int SaveBalloonNodes(struct Element *element);
int Balloonz_Load(Element *obj);
