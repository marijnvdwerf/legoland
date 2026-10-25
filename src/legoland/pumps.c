#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "bricks.h"
#include "gamemap.h"
#include "globals.h"
#include "map_object.h"
#include "obj_instance.h"
#include "objclass.h"
#include "pumps.h"
#include "ride_queue.h"
#include "tilemap.h"

// FUNCTION: LEGOLAND 0x00411a10
void InitDrivingSchoolPumps(struct Element *param_1) {
    DrivingSchoolPumpsRide = param_1->ride;
}

// FUNCTION: LEGOLAND 0x00411a20
void DrivingSchoolPumpsSetEditMode(void) {
    struct Ride *eax_temp;

    eax_temp = DrivingSchoolPumpsRide;
    EditMode.unk0 = 1;
    EditMode.unk8 = eax_temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(DrivingSchoolPumpsFootprint);
    EditCursor.field_1828 = EditCursor.field_1828 | 8;
    BuildCursorPtr(&EditCursor, 0x8f8, 0);
    DefaultCursor(&ExtraEditCursor1);
    memcpy(ExtraEditCursor1.field_1414, DrivingSchoolPumpsFootprint, 0x14);
    ExtraEditCursor1.field_1828 = 0x34;
}

// FUNCTION: LEGOLAND 0x00411aa0
struct PumpNode *FindPumpAt(unsigned int arg1, unsigned int arg2) {
    struct PumpNode *node;

    node = (struct PumpNode *)PumpList;
    while (node) {
        if (node->var_4 == arg1 && node->var_8 == arg2) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00411ad0
void FreePump(struct PumpNode *node) {
    struct PumpNode *next;
    struct PumpNode *current;

    if (PumpList) {
        next = node->next;
        free(node);
        if (PumpList == node) {
            PumpList = next;
        } else {
            current = (struct PumpNode *)PumpList;
            if (current->next != node) {
                do {
                    current = current->next;
                } while (current && current->next != node);
            }
            if (current) {
                current->next = next;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00411b20
void RemovePump(struct PumpNode *node) {
    struct Cursor cursor;
    TileId tile;
    struct Ride *ride;

    ride = DrivingSchoolPumpsRide;
    cursor.footprint = ride->footprint;
    tile.pos.x = (unsigned char)node->var_4;
    tile.pos.y = (unsigned char)node->var_8;
    cursor.tile_x = node->var_4;
    cursor.tile_y = node->var_8;
    StandardRemoveObject(ride->element, tile, &cursor);
    FreePump(node);
}

// FUNCTION: LEGOLAND 0x00411ba0
void RemovePumpsWithId(unsigned short param) {
    struct PumpNode *cur;
    struct PumpNode *next;

    cur = (struct PumpNode *)PumpList;
    if (cur) {
        do {
            next = cur->next;
            if (cur->var_2 == param) {
                RemovePump(cur);
            }
            cur = next;
        } while (cur);
    }
}

// FUNCTION: LEGOLAND 0x00411bd0
void RemoveAllPumps(void) {
    struct PumpNode *current;
    struct PumpNode *next;

    current = (struct PumpNode *)PumpList;
    if (current == NULL) {
        return;
    }
    while (current != NULL) {
        next = current->next;
        RemovePump(current);
        current = next;
    }
}

// FUNCTION: LEGOLAND 0x00411bf0
void DrivingSchoolPumpsAddObject(Element *obj, int *coords) {
    struct RideQueueEntry *info;
    struct PumpNode *node;
    TileId tile;

    tile.pos.x = (unsigned char)coords[0];
    tile.pos.y = (unsigned char)coords[1];
    info = FindQueueEntryAtTile(coords[0] + 1, coords[1]);
    if (info) {
        AddBasicObject(obj, coords);
        node = (struct PumpNode *)malloc(sizeof(struct PumpNode));
        node->id = tile.id;
        node->var_2 = info->id;
        node->var_4 = tile.pos.x;
        node->var_8 = tile.pos.y;
        node->next = (struct PumpNode *)PumpList;
        PumpList = node;
    }
}

// FUNCTION: LEGOLAND 0x00411c70
void DrivingSchoolPumpsRemoveObject(void *param_1, TileId tile, struct Cursor *cursor) {
    struct PumpNode *node;

    BGFullUpdate = 1;
    node = FindPumpAt(tile.pos.x, tile.pos.y);
    if (node) {
        StandardRemoveObject((DrivingSchoolPumpsRide)->element, tile, cursor);
        FreePump(node);
    }
}

// FUNCTION: LEGOLAND 0x00411cd0
void DrivingSchoolPumpsCalcCursor(Element *obj, int *screen, unsigned int param_3) {
    struct Ride *ride;
    struct RideQueueEntry *tile;

    ride = obj->ride;
    EditCursor.footprint = ride->footprint;
    EditCursor.field_1830 = 0;
    ScreenToMapRef(screen, &EditCursor.tile_x, param_3);
    tile = SnapCursorToRoadForPump(&EditCursor);
    ValidateCursor(&EditCursor, ride);
    if (GetBrickCount() < GetObjCost(ride)) {
        SetCursorBlocked(&EditCursor, 2);
        return;
    }
    if (tile != NULL && IsCursorBuildable(&EditCursor)) {
        EditCursor.next = &ExtraEditCursor1;
        memcpy(ExtraEditCursor1.field_1414, RoadPieceFootprint, 0x14);
        ExtraEditCursor1.field_1830 = 0;
        ExtraEditCursor1.tile_x = tile->x;
        ExtraEditCursor1.tile_y = tile->y;
        ExtraEditCursor1.field_1828 = 0x2034;
        return;
    }
    EditCursor.field_1830 = 0;
    SetCursorBlocked(&EditCursor, 0xe);
}

// FUNCTION: LEGOLAND 0x00411dc0
struct RideQueueEntry *SnapCursorToRoadForPump(struct Cursor *cursor) {
    struct RideQueueEntry *tile;
    INT_PTR y; /* the original keeps the result in the same register */

    tile = FindQueueEntryAtTile(cursor->tile_x + 1, cursor->tile_y);
    if (tile != NULL && tile->field_14 != 0) {
        tile = NULL;
    }
    y = 0;
    if (tile != NULL) {
        cursor->tile_x = tile->x - 1;
        y = tile->y - (DrivingSchoolPumpsRide)->footprint.y0;
        cursor->tile_y = y;
        y = (INT_PTR)tile;
    }
    return (struct RideQueueEntry *)y;
}
