#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bricks.h"
#include "driving_school.h"
#include "gamemap.h"
#include "llidb.h"
#include "map_object.h"
#include "objclass.h"
#include "print_sprite.h"
#include "pumps.h"
#include "render3d.h"
#include "ride_queue.h"
#include "roads.h"
#include "tilemap.h"

struct RoadCallbacks {
    unsigned char pad_0[0x18];
    void *field_18;
    void *field_1c;
    void *field_20;
};

struct RoadPlaceArg {
    unsigned int x;
    unsigned int y;
};

struct NeighborResult {
    struct RideQueueEntry *field_0;
    struct RideQueueEntry *field_4;
    struct RideQueueEntry *field_8;
    struct RideQueueEntry *field_c;
    struct RideQueueEntry *field_10;
    struct RideQueueEntry *field_14;
    struct RideQueueEntry *field_18;
    struct RideQueueEntry *field_1c;
};

// FUNCTION: LEGOLAND 0x004132a0
void AddRoadEntry(TileId tile, int param_2, int param_3, unsigned int param_4, unsigned int param_5) {
    struct RideQueueEntry *entry = malloc(sizeof(struct RideQueueEntry));

    entry->id = tile.id;
    entry->x = param_2;
    entry->y = param_3;
    entry->field_14 = (unsigned char)param_4;
    entry->next = RoadEntryList;
    entry->field_1c = 0;
    entry->field_1d = 0;
    RoadEntryList = entry;

    PlaceRoadTiles(param_2, param_3, param_4, param_5);

    Set_UserFlags(param_2 << 8, param_3 << 8, 0);
    Set_UserFlags((param_2 + 1) << 8, param_3 << 8, 0);
    Set_UserFlags((param_2 + 2) << 8, param_3 << 8, 0);
    Set_UserFlags((param_2 + 3) << 8, param_3 << 8, 0);

    Set_UserFlags(param_2 << 8, (param_3 + 1) << 8, 0);
    Set_UserFlags((param_2 + 1) << 8, (param_3 + 1) << 8, 0);
    Set_UserFlags((param_2 + 2) << 8, (param_3 + 1) << 8, 0);
    Set_UserFlags((param_2 + 3) << 8, (param_3 + 1) << 8, 0);

    Set_UserFlags(param_2 << 8, (param_3 + 2) << 8, 0);
    Set_UserFlags((param_2 + 1) << 8, (param_3 + 2) << 8, 0);
    Set_UserFlags((param_2 + 2) << 8, (param_3 + 2) << 8, 0);
    Set_UserFlags((param_2 + 3) << 8, (param_3 + 2) << 8, 0);

    Set_UserFlags(param_2 << 8, (param_3 + 3) << 8, 0);
    Set_UserFlags((param_2 + 1) << 8, (param_3 + 3) << 8, 0);
    Set_UserFlags((param_2 + 2) << 8, (param_3 + 3) << 8, 0);
    Set_UserFlags((param_2 + 3) << 8, (param_3 + 3) << 8, 0);
}

// FUNCTION: LEGOLAND 0x004133e0
void RemoveRoadEntry(int param_1, int param_2) {
    struct RideQueueEntry *entry = (struct RideQueueEntry *)FindRoadEntryAt(param_1, param_2);
    struct PumpNode *pump;
    struct RideQueueEntry *next;
    struct RideQueueEntry *cur;

    if (entry == NULL) {
        return;
    }

    next = entry->next;
    pump = FindPumpAt(entry->x - 1, entry->y + 1);
    if (pump != NULL) {
        RemovePump(pump);
    }

    free(entry);

    if (entry == RoadEntryList) {
        RoadEntryList = next;
        return;
    }

    cur = RoadEntryList;
    while (cur->next != entry) {
        cur = cur->next;
    }
    cur->next = next;
}

// FUNCTION: LEGOLAND 0x00413450
int GetDiagonalRoadNeighbours(int x, int y, struct RideQueueEntry **out) {
    int count = 0;
    struct RideQueueEntry *e;
    int x1 = x + 4;
    int y0 = y - 4;
    e = FindRoadEntryAt(x1, y0);
    if (e != NULL) {
        count = 1;
    }
    if (out != NULL) {
        out[1] = e;
    }
    y += 4;
    e = FindRoadEntryAt(x1, y);
    if (e != NULL) {
        count++;
    }
    if (out != NULL) {
        out[3] = e;
    }
    x1 = x - 4;
    e = FindRoadEntryAt(x1, y);
    if (e != NULL) {
        count++;
    }
    if (out != NULL) {
        out[5] = e;
    }
    e = FindRoadEntryAt(x1, y0);
    if (e != NULL) {
        count++;
    }
    if (out != NULL) {
        out[7] = e;
    }
    return count;
}

/* Neighbour filter for GetConnectedRoadNeighbours: a road start tile (shape 6, the driving school's own exit) only
 * connects to the tile 4 to its east, so it is dropped unless (x, y) is that tile. */
// FUNCTION: LEGOLAND 0x004134f0
struct RideQueueEntry *FilterRoadStartNeighbour(int arg1, int arg2, struct RideQueueEntry *tile) {
    if (tile != NULL) {
        if ((tile->field_14 & 0xf) != 6) {
            return tile;
        }
        if (tile->x + 4 == arg1) {
            if (tile->y == arg2) {
                return tile;
            }
        }
    }
    return 0;
}

/* GetRoadNeighbours (north, east, south, west) with FilterRoadStartNeighbour applied; returns how many remain. */
// FUNCTION: LEGOLAND 0x00413520
int GetConnectedRoadNeighbours(int x, int y, struct NeighborResult *out) {
    struct NeighborResult r;
    struct RideQueueEntry *t3;
    int count = 0;

    GetRoadNeighbours(x, y, &r);
    r.field_0 = (struct RideQueueEntry *)FilterRoadStartNeighbour(x, y, (struct RideQueueEntry *)r.field_0);
    if (r.field_0 != NULL) {
        count = 1;
    }
    r.field_8 = (struct RideQueueEntry *)FilterRoadStartNeighbour(x, y, (struct RideQueueEntry *)r.field_8);
    if (r.field_8 != NULL) {
        count++;
    }
    r.field_10 = (struct RideQueueEntry *)FilterRoadStartNeighbour(x, y, (struct RideQueueEntry *)r.field_10);
    if (r.field_10 != NULL) {
        count++;
    }
    t3 = FilterRoadStartNeighbour(x, y, (struct RideQueueEntry *)r.field_18);
    if (t3 != NULL) {
        count++;
    }
    if (out != NULL) {
        out->field_0 = r.field_0;
        out->field_8 = r.field_8;
        out->field_10 = r.field_10;
        out->field_18 = (struct RideQueueEntry *)t3;
    }
    return count;
}

// FUNCTION: LEGOLAND 0x004135d0
int GetRoadNeighbours(int x, int y, struct NeighborResult *out) {
    struct RideQueueEntry *e;
    int count = 0;

    e = (struct RideQueueEntry *)FindRoadEntryAt(x, y - 4);
    if (e != NULL) {
        count = 1;
    }
    if (out != NULL) {
        out->field_0 = e;
    }

    e = (struct RideQueueEntry *)FindRoadEntryAt(x + 4, y);
    if (e != NULL) {
        count++;
    }
    if (out != NULL) {
        out->field_8 = e;
    }

    e = (struct RideQueueEntry *)FindRoadEntryAt(x, y + 4);
    if (e != NULL) {
        count++;
    }
    if (out != NULL) {
        out->field_10 = e;
    }

    e = (struct RideQueueEntry *)FindRoadEntryAt(x - 4, y);
    if (e != NULL) {
        count++;
    }
    if (out != NULL) {
        out->field_18 = e;
    }

    return count;
}

/* Choose a path tile's shape and rotation from neighboring entries with matching IDs.
 * Shapes that remove an existing zebra crossing refund its cost through the final path. */
// FUNCTION: LEGOLAND 0x00413650
void UpdateQueuePathShape(unsigned short param_1, int param_2, int param_3) {
    struct NeighborResult r;
    struct RideQueueEntry *e;
    int count = 0;
    int bits = 0;
    int mask = 0;
    int flag = 0;
    int m;
    unsigned short id;

    id = param_1;
    e = FindRoadEntryAt(param_2, param_3);
    if (e != NULL) {
        flag = e->field_14 & 0x10;
    }
    GetConnectedRoadNeighbours(param_2, param_3, &r);
    if (r.field_0 != NULL && r.field_0->id == id) {
        mask = count = 1;
    }
    if (r.field_8 != NULL && r.field_8->id == id) {
        count++;
        mask |= 2;
    }
    if (r.field_10 != NULL && r.field_10->id == id) {
        count++;
        mask |= 4;
    }
    if (r.field_18 != NULL && r.field_18->id == id) {
        count++;
        mask |= 8;
    }
    switch (count) {
    case 0:
        return;
    case 2:
        if ((mask & 5) != 5 && (mask & 0xa) != 0xa) {
            switch (mask) {
            case 6:
                PlaceRoadTiles(param_2, param_3, 3, 0);
                break;
            case 12:
                PlaceRoadTiles(param_2, param_3, 3, 1);
                break;
            case 9:
                PlaceRoadTiles(param_2, param_3, 3, 2);
                break;
            case 3:
                PlaceRoadTiles(param_2, param_3, 3, 3);
                break;
            }
            break;
        }
    case 1:
        GetDiagonalRoadNeighbours(param_2, param_3, (struct RideQueueEntry **)&r);
        if (r.field_0 != NULL && r.field_0->id == id) {
            bits = 1;
        }
        if (r.field_4 != NULL && r.field_4->id == id) {
            bits |= 2;
        }
        if (r.field_8 != NULL && r.field_8->id == id) {
            bits |= 4;
        }
        if (r.field_c != NULL && r.field_c->id == id) {
            bits |= 8;
        }
        if (r.field_10 && r.field_10->id == id) {
            bits |= 0x10;
        }
        if (r.field_14 != NULL && r.field_14->id == id) {
            bits |= 0x20;
        }
        if (r.field_18 != NULL) {
            if (r.field_18->id == id) {
                bits |= 0x40;
            }
        }
        if (r.field_1c != NULL && r.field_1c->id == id) {
            bits |= 0x80;
        }
        /* These choices preserve the crossing flag and finish without a refund. Keep a
         * return at each call: MSVC6 then shares the original argument/call tails; joining
         * the branches at one final return changes the generated tail layout. */
        m = bits;
        if ((m & 0x11) != 0) {
            if ((m & 0x83) == 0x83) {
                if ((m & 0x38) == 0x38) {
                    PlaceRoadTiles(param_2, param_3, flag | 7, 0);
                    return;
                } else {
                    PlaceRoadTiles(param_2, param_3, flag | 1, 0);
                    return;
                }
            } else {
                if ((m & 0x38) == 0x38) {
                    PlaceRoadTiles(param_2, param_3, flag | 1, 2);
                    return;
                } else {
                    PlaceRoadTiles(param_2, param_3, flag, 0);
                    return;
                }
            }
        } else if ((m & 0xe0) == 0xe0) {
            if ((m & 0xe) == 0xe) {
                PlaceRoadTiles(param_2, param_3, flag | 7, 1);
                return;
            } else {
                PlaceRoadTiles(param_2, param_3, flag | 1, 3);
                return;
            }
        } else if ((m & 0xe) == 0xe) {
            PlaceRoadTiles(param_2, param_3, flag | 1, 1);
            return;
        } else {
            PlaceRoadTiles(param_2, param_3, flag, 1);
            return;
        }
    case 3:
        switch (mask) {
        case 11:
            PlaceRoadTiles(param_2, param_3, 4, 0);
            break;
        case 7:
            PlaceRoadTiles(param_2, param_3, 4, 1);
            break;
        case 14:
            PlaceRoadTiles(param_2, param_3, 4, 2);
            break;
        case 13:
            PlaceRoadTiles(param_2, param_3, 4, 3);
            break;
        }
        break;
    case 4:
        PlaceRoadTiles(param_2, param_3, 5, 0);
        break;
    }
    if (flag != 0) {
        AddBricks(GetObjCost(ZebraCrossingRide));
    }
}

// FUNCTION: LEGOLAND 0x00413970
unsigned int CountRoadEntriesWithId(unsigned short param_1) {
    struct RideQueueEntry *entry = RoadEntryList;
    unsigned int count = 0;

    while (entry != NULL) {
        if (entry->id == param_1) {
            count++;
        }
        entry = entry->next;
    }
    return count;
}

/* Road tile callback for walkers (TILES FOR DSCHOOL): 2 = no road at the tile, 1 = road without cars, 3 = road with
 * cars on it (field_1c counts cars; IncRoadTileOccupancy counts walkers in field_1d). */
// FUNCTION: LEGOLAND 0x00413990
unsigned char GetRoadTileCarState(unsigned int param_1, unsigned int param_2) {
    struct RideQueueEntry *tile = FindQueueEntryAtTile(param_1 >> 8, param_2 >> 8);
    if (tile == NULL) {
        return 2;
    }
    if (tile->field_1c == 0) {
        return 1;
    }
    return 3;
}

// FUNCTION: LEGOLAND 0x004139c0
void IncRoadTileOccupancy(unsigned int param_1, unsigned int param_2) {
    struct RideQueueEntry *tile = FindQueueEntryAtTile(param_1 >> 8, param_2 >> 8);
    if (tile != NULL) {
        tile->field_1d++;
    }
}

// FUNCTION: LEGOLAND 0x004139e0
void DecRoadTileOccupancy(unsigned int param_1, unsigned int param_2) {
    struct RideQueueEntry *tile = FindQueueEntryAtTile(param_1 >> 8, param_2 >> 8);
    if (tile != NULL && tile->field_1d != 0) {
        tile->field_1d--;
    }
}

// FUNCTION: LEGOLAND 0x00413a10
void LoadDrivingSchoolRoadsResources(struct Element *head) {
    struct Element **handle = &head;

    DrivingSchoolRoadsClass = head->data;
    // STRING: LEGOLAND 0x004b4c94
    if (LLIDB_FindElement("DSCHOOL LIGHTS", handle, 0) == 0) {
        DrivingSchoolLightsData = LLIDB_LoadData(head);
    }
    // STRING: LEGOLAND 0x004b4c80
    if (LLIDB_FindElement("TILES FOR DSCHOOL", handle, 0) == 0) {
        struct RoadCallbacks *callbacks = head->data;
        callbacks->field_18 = GetRoadTileCarState;
        callbacks->field_1c = IncRoadTileOccupancy;
        callbacks->field_20 = DecRoadTileOccupancy;
    }
}

// FUNCTION: LEGOLAND 0x00413a80
void UnloadDrivingSchoolRoadsResources(void) {
    struct RideQueueEntry *entry = RoadEntryList;
    struct Element *handle;
    struct RideQueueEntry *next;

    if (LLIDB_FindElement("DSCHOOL LIGHTS", &handle, 0) == 0) {
        LLIDB_UnLoadData(handle);
    }

    while (entry != NULL) {
        next = entry->next;
        free(entry);
        entry = next;
    }
}

// FUNCTION: LEGOLAND 0x00413ad0
void DrivingSchoolRoadsSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = DrivingSchoolRoadsClass;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&RoadPieceFootprint);
    EditCursor.field_1828 |= 8;
    BuildCursorPtr(&EditCursor, 0x8f8, 0);
    DefaultCursor(&ExtraEditCursor1);
    memcpy(ExtraEditCursor1.field_1414, &RoadPieceFootprint, 20);
    ExtraEditCursor1.field_1828 = 0x34;
}

/* Build and validate the driving-school road cursor, including its connection to an
 * existing entry. Check people, available bricks and neighboring road IDs before linking
 * the auxiliary cursor at the selected entry. */
// FUNCTION: LEGOLAND 0x00413b50
void DrivingSchoolRoadsCalcCursor(Element *obj, int *param_2, unsigned int param_3) {
    int neighbor_mask;
    struct RideQueueEntry *entry;
    struct Ride *ride;

    struct MapRect rect;
    struct NeighborResult neighbors;
    unsigned short type;
    struct RideQueueEntry **neighbor_entries = (struct RideQueueEntry **)&neighbors;
    int result;
    int cost;
    ride = obj->ride;

    /* Initialise before the footprint copy: MSVC6 then preserves the original mask/entry
     * register allocation across the following validation calls. */
    neighbor_mask = 0;
    EditCursor.footprint = ride->footprint;
    EditCursor.field_1830 = neighbor_mask;
    ScreenToMapRef(param_2, &EditCursor.tile_x, param_3);
    entry = SnapCursorToRoadNeighbour(&EditCursor);
    ValidateCursor(&EditCursor, ride);
    if (IsCursorBuildable(&EditCursor) == 0) {
        return;
    }
    rect.x0 = EditCursor.field_1414[0] + EditCursor.tile_x;
    rect.y0 = EditCursor.field_1414[1] + EditCursor.tile_y;
    rect.x1 = EditCursor.field_1414[2] + EditCursor.tile_x;
    rect.y1 = EditCursor.field_1414[3] + EditCursor.tile_y;
    result = CheckForPeople(&rect);
    switch (result) {
    case -1:
        SetCursorBlocked(&EditCursor, 4);
        break;
    case 1:
        SetCursorBlocked(&EditCursor, 3);
        break;
    default:
        cost = GetObjCost(ride);
        if (GetBrickCount() < cost) {
            SetCursorBlocked(&EditCursor, 2);
        } else {
            if (IsCursorBuildable(&EditCursor) != 0) {
                if (!(GetConnectedRoadNeighbours(EditCursor.tile_x, EditCursor.tile_y, &neighbors) == 0)) {
                    if (neighbor_entries[0] != NULL) {
                        type = neighbor_entries[0]->id;
                    } else if (neighbor_entries[2] != NULL) {
                        type = neighbor_entries[2]->id;
                    } else if (neighbor_entries[4] != NULL) {
                        type = neighbor_entries[4]->id;
                    } else if (neighbor_entries[6] != NULL) {
                        type = neighbor_entries[6]->id;
                    } else {
                        /* Preserve the original low-word fallback when no cardinal entry
                         * supplies an ID. Its meaning is still uncertain. */
                        type = (unsigned short)(UINT_PTR)entry;
                    }
                    GetRoadNeighbours(EditCursor.tile_x, EditCursor.tile_y, &neighbors);
                    GetDiagonalRoadNeighbours(EditCursor.tile_x, EditCursor.tile_y, (struct RideQueueEntry **)&neighbors);
                    if (neighbor_entries[0] != NULL && neighbor_entries[0]->id == type) neighbor_mask = 1;
                    if (neighbor_entries[1] != NULL && neighbor_entries[1]->id == type) neighbor_mask |= 2;
                    if (neighbor_entries[2] != NULL && neighbor_entries[2]->id == type) neighbor_mask |= 4;
                    if (neighbor_entries[3] != NULL && neighbor_entries[3]->id == type) neighbor_mask |= 8;
                    if (neighbor_entries[4] != NULL && neighbor_entries[4]->id == type) neighbor_mask |= 0x10;
                    if (neighbor_entries[5] != NULL && neighbor_entries[5]->id == type) neighbor_mask |= 0x20;
                    if (neighbor_entries[6] != NULL && neighbor_entries[6]->id == type) neighbor_mask |= 0x40;
                    if (neighbor_entries[7] != NULL && neighbor_entries[7]->id == type) neighbor_mask |= 0x80;
                    if ((neighbor_mask & 7) == 7 || (neighbor_mask & 0x1c) == 0x1c || (neighbor_mask & 0x70) == 0x70 || (neighbor_mask & 0xc1) == 0xc1) {
                        SetCursorBlocked(&EditCursor, 0xe);
                    }
                } else {
                    SetCursorBlocked(&EditCursor, 0xe);
                }
            }
            if (entry != NULL && IsCursorBuildable(&EditCursor) != 0) {
                memcpy(ExtraEditCursor1.field_1414, RoadPieceFootprint, 20);
                EditCursor.next = &ExtraEditCursor1;
                ExtraEditCursor1.tile_x = entry->x;
                ExtraEditCursor1.tile_y = entry->y;
                ExtraEditCursor1.field_1830 = 0;
                ExtraEditCursor1.field_1828 = 0x2034;
            } else {
                EditCursor.field_1830 = 0;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00413e30
struct RideQueueEntry *SnapCursorToRoadNeighbour(struct Cursor *cur) {
    struct RideQueueEntry *t = FindQueueEntryAtTile(cur->tile_x, cur->tile_y);
    struct RideQueueEntry *a;
    struct RideQueueEntry *b;
    struct RideQueueEntry *c;
    struct RideQueueEntry *d;
    struct RideQueueEntry *r;
    if (t) {
        cur->tile_x = t->x;
        cur->tile_y = t->y;
    }
    a = FindQueueEntryAtTile(cur->tile_x, cur->tile_y - 4);
    if (a && (a->field_14 & 0xf) == 6) a = NULL;
    b = FindQueueEntryAtTile(cur->tile_x + 4, cur->tile_y);
    if (b && (b->field_14 & 0xf) == 6) b = NULL;
    c = FindQueueEntryAtTile(cur->tile_x, cur->tile_y + 4);
    if (c && (c->field_14 & 0xf) == 6) c = NULL;
    d = FindQueueEntryAtTile(cur->tile_x - 4, cur->tile_y);
    if (d && (d->field_14 & 0xf) == 6) d = NULL;
    r = NULL;
    if (a) {
        cur->tile_x = a->x;
        cur->tile_y = a->y + 4;
        r = a;
    } else if (b) {
        cur->tile_x = b->x - 4;
        cur->tile_y = b->y;
        r = b;
    } else if (c) {
        cur->tile_x = c->x;
        cur->tile_y = c->y - 4;
        r = c;
    } else if (d) {
        cur->tile_x = d->x + 4;
        cur->tile_y = d->y;
        r = d;
    }
    return r;
}

// FUNCTION: LEGOLAND 0x00413fa0
void RoadSetQueryCursor(struct Element *dummy, struct RoadPlaceArg *param) {
    struct RideQueueEntry *tile = FindQueueEntryAtTile(param->x, param->y);
    unsigned int *src;
    unsigned int *dst;
    unsigned int count;

    if (tile != NULL) {
        QueryCursor.tile_x = tile->x;
        QueryCursor.tile_y = tile->y;

        src = &RoadPieceFootprint[0];
        dst = &QueryCursor.field_1414[0];
        count = 5;
        while (count != 0) {
            *dst = *src;
            src++;
            dst++;
            count--;
        }

        QueryCursor.field_1828 = 8;
        SetCursorBlocked(&QueryCursor, 1);

        if ((tile->field_14 & 0xf) != 6) {
            ResetCursorBuildable(&QueryCursor);
        }
    }
}

// FUNCTION: LEGOLAND 0x00414020
void DrivingSchoolRoadsAddObject(struct Element *edit, struct RoadPlaceArg *place) {
    struct NeighborResult r;
    struct RideQueueEntry **q = (struct RideQueueEntry **)&r;
    TileId id;
    int x = place->x;
    int y = place->y;

    GetConnectedRoadNeighbours(x, y, &r);
    if (r.field_0) {
        id.id = r.field_0->id;
    } else if (r.field_8) {
        id.id = r.field_8->id;
    } else if (r.field_10) {
        id.id = r.field_10->id;
    } else if (r.field_18) {
        id.id = r.field_18->id;
    }
    AddRoadEntry(id, x, y, 0, 0);
    UpdateQueuePathShape(id.id, x, y);
    ComputeRoadDistances(id);
    GetDiagonalRoadNeighbours(x, y, (struct RideQueueEntry **)&r);
    if (q[0] && q[0]->id == id.id && (q[0]->field_14 & 0xf) != 6) UpdateQueuePathShape(id.id, x, y - 4);
    if (q[1] && q[1]->id == id.id && (q[1]->field_14 & 0xf) != 6) UpdateQueuePathShape(id.id, x + 4, y - 4);
    if (q[2] && q[2]->id == id.id && (q[2]->field_14 & 0xf) != 6) UpdateQueuePathShape(id.id, x + 4, y);
    if (q[3] && q[3]->id == id.id && (q[3]->field_14 & 0xf) != 6) UpdateQueuePathShape(id.id, x + 4, y + 4);
    if (q[4] && q[4]->id == id.id && (q[4]->field_14 & 0xf) != 6) UpdateQueuePathShape(id.id, x, y + 4);
    if (q[5] && q[5]->id == id.id && (q[5]->field_14 & 0xf) != 6) UpdateQueuePathShape(id.id, x - 4, y + 4);
    if (q[6] && q[6]->id == id.id && (q[6]->field_14 & 0xf) != 6) UpdateQueuePathShape(id.id, x - 4, y);
    if (q[7] && q[7]->id == id.id && (q[7]->field_14 & 0xf) != 6) UpdateQueuePathShape(id.id, x - 4, y - 4);
    AddDrivingSchoolScore(id.id, 1);
    IncrementObjectCount((struct Ride *)edit->data);
}

// FUNCTION: LEGOLAND 0x00414220
void RoadRemoveObject(Element *edit, TileId tile, struct Cursor *cursor) {
    struct NeighborResult r;
    struct RideQueueEntry **q = (struct RideQueueEntry **)&r;
    int x = cursor->tile_x;
    int y = cursor->tile_y;
    struct RideQueueEntry *entry;

    BGFullUpdate = 1;
    StandardRemoveObject(edit, tile, cursor);
    entry = (struct RideQueueEntry *)FindRoadEntryAt(x, y);
    tile.id = entry->id;
    if (entry->field_14 & 0x10) {
        IncrementObjectCount((struct Ride *)edit->data);
        DecrementObjectCount((struct Ride *)ZebraCrossingRide);
        entry->field_14 &= 0xef;
        UpdateQueuePathShape(tile.id, x, y);
        AddBricks(GetObjCost((struct Ride *)ZebraCrossingRide));
        return;
    }
    AddDrivingSchoolScore(tile.id, -1);
    RemoveRoadEntry(x, y);
    ComputeRoadDistances(tile);
    GetRoadNeighbours(x, y, &r);
    GetDiagonalRoadNeighbours(x, y, (struct RideQueueEntry **)&r);
    if (q[0] && q[0]->id == tile.id && (q[0]->field_14 & 0xf) != 6) UpdateQueuePathShape(tile.id, x, y - 4);
    if (q[1] && q[1]->id == tile.id && (q[1]->field_14 & 0xf) != 6) UpdateQueuePathShape(tile.id, x + 4, y - 4);
    if (q[2] && q[2]->id == tile.id && (q[2]->field_14 & 0xf) != 6) UpdateQueuePathShape(tile.id, x + 4, y);
    if (q[3] && q[3]->id == tile.id && (q[3]->field_14 & 0xf) != 6) UpdateQueuePathShape(tile.id, x + 4, y + 4);
    if (q[4] && q[4]->id == tile.id && (q[4]->field_14 & 0xf) != 6) UpdateQueuePathShape(tile.id, x, y + 4);
    if (q[5] && q[5]->id == tile.id && (q[5]->field_14 & 0xf) != 6) UpdateQueuePathShape(tile.id, x - 4, y + 4);
    if (q[6] && q[6]->id == tile.id && (q[6]->field_14 & 0xf) != 6) UpdateQueuePathShape(tile.id, x - 4, y);
    if (q[7] && q[7]->id == tile.id && (q[7]->field_14 & 0xf) != 6) UpdateQueuePathShape(tile.id, x - 4, y - 4);
}

// FUNCTION: LEGOLAND 0x00414440
void UpdateTrafficLights(void) {
    struct RideQueueEntry *e = RoadEntryList;
    int ia;
    int ib;
    int ic;
    int id;
    int w;
    int h;
    struct Point pt;
    struct Point ref;
    int b[4];

    TrafficLightTimer--;
    if (TrafficLightTimer <= 0) {
        switch (TrafficLightPhase) {
        case 0:
            TrafficLightA = 0;
            TrafficLightTimer = 0x28;
            break;
        case 1:
            TrafficLightB = 1;
            TrafficLightTimer = 0x8c;
            break;
        case 2:
            TrafficLightB = 0;
            TrafficLightTimer = 0x28;
            break;
        case 3:
            TrafficLightA = 1;
            TrafficLightTimer = 0x8c;
            break;
        }
        TrafficLightPhase = (TrafficLightPhase + 1) & 3;
    }
    if (DrivingSchoolDriveOnLeft != 0) {
        if (TrafficLightA != 0) {
            id = 6;
            ib = 1;
            ic = 2;
            ia = 5;
        } else if (TrafficLightB != 0) {
            id = 7;
            ib = 0;
            ic = 3;
            ia = 4;
        } else {
            id = 7;
            ib = 1;
            ic = 3;
            ia = 5;
        }
    } else if (TrafficLightA != 0) {
        id = 9;
        ib = 10;
        ic = 13;
        ia = 14;
    } else if (TrafficLightB != 0) {
        id = 8;
        ib = 11;
        ic = 12;
        ia = 15;
    } else {
        id = 9;
        ib = 11;
        ic = 13;
        ia = 15;
    }
    for (; e != NULL; e = e->next) {
        if ((e->field_14 & 0xf) != 5) {
            continue;
        }
        GetTileDimensions(&w, &h);
        pt.x = DrivingSchoolLightsData->off_x[ia & 0xff] >> 1;
        pt.y = DrivingSchoolLightsData->off_y[ia & 0xff] >> 1;
        AdjustOffsetForViewMode(&pt);
        ref.x = e->x;
        ref.y = e->y;
        GetTileBounds(&ref, b);
        SortSprite(DrivingSchoolLightsData->sprites[ia & 0xff], b[0] + pt.x, b[1] + pt.y, b[1] - lpConfig->view_y, 0, 0);

        ref.x = e->x + 3;
        ref.y = e->y;
        GetTileBounds(&ref, b);
        pt.x = DrivingSchoolLightsData->off_x[id & 0xff] >> 1;
        pt.y = DrivingSchoolLightsData->off_y[id & 0xff] >> 1;
        AdjustOffsetForViewMode(&pt);
        SortSprite(DrivingSchoolLightsData->sprites[id & 0xff], b[0] + pt.x, b[1] + pt.y, ((b[1] + b[3]) >> 1) - lpConfig->view_y, 0, 0);

        ref.x = e->x + 3;
        ref.y = e->y + 3;
        GetTileBounds(&ref, b);
        pt.x = DrivingSchoolLightsData->off_x[ib & 0xff] >> 1;
        pt.y = DrivingSchoolLightsData->off_y[ib & 0xff] >> 1;
        AdjustOffsetForViewMode(&pt);
        SortSprite(DrivingSchoolLightsData->sprites[ib & 0xff], b[0] + pt.x, b[1] + pt.y, b[3] - lpConfig->view_y, 0, 0);

        ref.x = e->x;
        ref.y = e->y + 3;
        GetTileBounds(&ref, b);
        pt.x = DrivingSchoolLightsData->off_x[ic & 0xff] >> 1;
        pt.y = DrivingSchoolLightsData->off_y[ic & 0xff] >> 1;
        AdjustOffsetForViewMode(&pt);
        SortSprite(DrivingSchoolLightsData->sprites[ic & 0xff], b[0] + pt.x, b[1] + pt.y, ((b[1] + b[3]) >> 1) - lpConfig->view_y, 0, 0);
    }
}

// FUNCTION: LEGOLAND 0x00414830
void ZebraCrossingSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = ZebraCrossingRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&RoadPieceFootprint);
    EditCursor.field_1828 |= 8;
    BuildCursorPtr(&EditCursor, 0, 0);
}

// FUNCTION: LEGOLAND 0x00414880
void ZebraCrossingCalcCursor(struct Element *param_1, int *param_2, unsigned int param_3) {
    void *obj = param_1->data;
    struct RideQueueEntry *tile;
    int cost;

    memcpy(EditCursor.field_1414, (char *)obj + 0x3c, 20);
    EditCursor.field_1830 = 0;
    ScreenToMapRef(param_2, &EditCursor.tile_x, param_3);
    tile = FindQueueEntryAtTile(EditCursor.tile_x, EditCursor.tile_y);
    SetCursorBlocked(&EditCursor, 0xe);
    cost = GetObjCost(obj);
    if (GetBrickCount() < cost) {
        return;
    }
    if (tile == NULL) {
        return;
    }
    EditCursor.tile_x = tile->x;
    EditCursor.tile_y = tile->y;
    if (tile->field_14 & 0x10) {
        return;
    }
    if ((tile->field_14 & 0xf) == 3) return;
    if ((tile->field_14 & 0xf) == 4) return;
    if ((tile->field_14 & 0xf) == 5) return;
    if ((tile->field_14 & 0xf) == 6) return;
    if (FindPumpAt(tile->x - 1, tile->y + 1) != 0) {
        return;
    }
    ResetCursorBuildable(&EditCursor);
}

// FUNCTION: LEGOLAND 0x00414940
void InitZebraCrossing(struct Element *param_1) {
    ZebraCrossingRide = param_1->data;
}

// FUNCTION: LEGOLAND 0x00414950
void ZebraCrossingAddObject(struct Element *edit, struct RoadPlaceArg *place) {
    struct NeighborResult r;
    struct RideQueueEntry *tile = (struct RideQueueEntry *)FindRoadEntryAt(place->x, place->y);

    if (tile != NULL) {
        GetRoadNeighbours(place->x, place->y, &r);
        tile->field_14 |= 0x10;
        UpdateQueuePathShape((short)tile->id, place->x, place->y);
        tile->field_1d = 0;
        tile->field_1c = 0;
        IncrementObjectCount((struct Ride *)edit->data);
    }
}
