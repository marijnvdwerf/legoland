#pragma once

#include "legoland.h"
#include "llidb.h"

#include "gamemap.h"
#include "obj_instance.h"

typedef struct BalloonNode BalloonNode;

/* Per-balloon-ride state (one 0x20 allocation), list head at DAT_00616060.
   The ride has six cars on a 24-step wheel; FUN_0042aa60 maps (pos, lap) to a car. */
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

void FUN_0042a8f0(TileId *tile);
BalloonNode *FUN_0042a980(TileId *tile);
void FUN_0042a9b0(BalloonNode *node);
void FUN_0042a9f0(void);
int FUN_0042aa60(char pos, char lap);

void FUN_0042a7b0(Element *obj);
void FUN_0042a950(Element *obj, int *coords);
void FUN_0042aa10(Element *obj, TileId tile, Cursor *cursor);
void FUN_0042aa90(Element *obj);
RideSpriteInfo *FUN_0042b2a0(Element *obj, unsigned short id);
void FUN_0042b2e0(Element *obj, void *param_2, void *param_3, TileId *tile, unsigned int param_5, unsigned int param_6);
void FUN_0042b9d0(void);
void FUN_0042ba40(void);
unsigned int FUN_0042ba80(void);
unsigned int FUN_0042baf0(Element *obj);
