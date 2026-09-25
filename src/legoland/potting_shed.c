#include <stdlib.h>
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "gamemap.h"
#include "globals.h"
#include "map_object.h"
#include "obj_instance.h"
#include "potting_shed.h"

struct PSCarInner {
    unsigned char pad_0[0x10];
    unsigned int field_10;
};

struct PSCarSub {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x48];
    struct PSCarInner *field_64;
};

struct PSCarLayer {
    unsigned char pad_0[0xc];
    struct PSCarSub *field_c;
};

struct EditTarget {
    unsigned char pad_0[0x3c];
    unsigned char field_3c;
};

#include "image_sprite.h"
#include "mechanics_hut.h"
#include "print_sprite.h"
#include "render3d.h"
#include "worker.h"

// FUNCTION: LEGOLAND 0x0043ce60
void FUN_0043ce60(struct RideObject *obj) {
    DAT_0081caf0 = obj->ride;
    DAT_0081caf0->flags |= 0x420;
    DAT_0062fe48 = DAT_0081caf0->layer;
    DAT_0081caf0->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b7994
    DAT_0062fe4c = LoadSprite("gshedmatte.lls", 1);
}

// FUNCTION: LEGOLAND 0x0043ceb0
unsigned int FUN_0043ceb0(unsigned int param_1, unsigned int param_2) {
    return AddBasicObject(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x0043ced0
void FUN_0043ced0(struct RideObject *obj, unsigned int tile, struct Cursor *cursor) {
    StandardRemoveObject((struct EditObject *)obj, *(union TileId *)&tile, cursor);
    FUN_0043d7c0(obj->ride, tile, 1);
}

// FUNCTION: LEGOLAND 0x0043cf00
void FUN_0043cf00(struct RideObject *obj) {
    struct Ride *shed = obj->ride;
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *bloke;
    char dir;

    for (node = shed->riders; node != NULL; node = next) {
        bloke = node->rider;
        next = node->next;
        if (bloke->field_e != 0) {
            continue;
        }
        switch (bloke->param_action) {
        case 0:
            bloke->flags |= 8;
            bloke->dest.x += 0x900;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->field_73 = dir + 0x10;
            bloke->field_e = 7;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 1:
            bloke->flags &= 0xffd7;
            RemoveBlokeFromList((struct BlokeList *)shed, (struct Bloke *)node);
            NewLongTermAction(bloke, 0x10);
            free(node);
            break;
        case 100:
            bloke->flags |= 8;
            bloke->dest.x -= 0x900;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->field_73 = dir + 0x10;
            bloke->field_e = 7;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 101:
            RemoveBlokeFromList((struct BlokeList *)shed, (struct Bloke *)node);
            bloke->field_36 = 100;
            RefundGardener();
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x0043d0b0
void FUN_0043d0b0(struct RideObject *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile) {
    struct Ride *shed = obj->ride;
    struct RideNode *node = shed->riders;
    struct Bloke *blokes[30] = {0};
    char count = 0;
    char i;
    struct Point pos;

    while (node != NULL) {
        if (*tile == node->tile.id) {
            blokes[count++] = node->rider;
        }
        node = node->next;
    }
    if (count == 0) {
        return;
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 0) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 1) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 100) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 101) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    pos = GetScreenCoordsForObject((unsigned char *)tile, shed);
    PrintSprite(DAT_0062fe4c, pos.x, pos.y, 0, 0);
}

// FUNCTION: LEGOLAND 0x0043d1c0
void FUN_0043d1c0(void) {
    KillSprite(DAT_0062fe4c);
}

// FUNCTION: LEGOLAND 0x0043d1d0
void FUN_0043d1d0(void) {
    void *p = DAT_0081caf0;
    EditMode.unk0 = 1;
    EditMode.unk8 = p;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct EditTarget *)EditMode.unk8)->field_3c);
}

// FUNCTION: LEGOLAND 0x0043d210
unsigned int *FUN_0043d210(struct PSCarLayer *param1, unsigned short param2) {
    struct PSCarSub *s2 = param1->field_c;
    struct PSCarInner *s3;

    DAT_0062fe10 = (unsigned int)s2->field_64;
    DAT_0062fe14 = s2->field_14;
    DAT_0062fe18 = s2->field_18;
    DAT_0062fe1c = param2;

    s3 = s2->field_64;
    s3->field_10 |= 0x2000;

    return &DAT_0062fe10;
}
