#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bricks.h"
#include "gamemap.h"
#include "llidb.h"
#include "map_object.h"
#include "objclass.h"
#include "pumps.h"
#include "ride_queue.h"
#include "tilemap.h"

struct RoadQueueEntry {
    struct RoadQueueEntry *next;
    unsigned char pad_4[0x4];
    unsigned short field_8;
    unsigned char pad_a[0x2];
    int x;
    int y;
    unsigned char field_14;
    unsigned char pad_15[0x7];
    unsigned char field_1c;
    unsigned char field_1d;
    unsigned char pad_1e[0x2];
};

struct RoadTile {
    unsigned char pad_0[0xc];
    unsigned int x;
    unsigned int y;
    unsigned char flags;
    unsigned char pad_15[0x7];
    unsigned char field_1c;
    unsigned char field_1d;
};

struct RoadCallbacks {
    unsigned char pad_0[0x18];
    void *field_18;
    void *field_1c;
    void *field_20;
};

struct LLIDB_Head {
    unsigned char pad_0[0xc];
    struct RoadCallbacks *callbacks;
};

struct RoadEditArg {
    unsigned char pad_0[0xc];
    void *field_c;
};

struct RoadPlaceArg {
    unsigned int field_0;
    unsigned int field_4;
};

struct NeighborResult {
    struct RoadQueueEntry *field_0;
    unsigned char pad_4[4];
    struct RoadQueueEntry *field_8;
    unsigned char pad_c[4];
    struct RoadQueueEntry *field_10;
    unsigned char pad_14[4];
    struct RoadQueueEntry *field_18;
};

// FUNCTION: LEGOLAND 0x004132a0
void FUN_004132a0(unsigned short param_1, int param_2, int param_3, unsigned int param_4, unsigned int param_5) {
    struct RoadQueueEntry *entry = malloc(0x20);

    entry->field_8 = param_1;
    entry->x = param_2;
    entry->y = param_3;
    entry->field_14 = (unsigned char)param_4;
    entry->next = (struct RoadQueueEntry *)DAT_004cbeac;
    entry->field_1c = 0;
    entry->field_1d = 0;
    DAT_004cbeac = (struct RideQueueEntry *)entry;

    FUN_00412680(param_2, param_3, param_4, param_5);

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
void FUN_004133e0(int param_1, int param_2) {
    struct RoadQueueEntry *entry = (struct RoadQueueEntry *)FUN_004125a0(param_1, param_2);
    struct PumpNode *pump;
    struct RoadQueueEntry *next;
    struct RoadQueueEntry *cur;

    if (entry == NULL) {
        return;
    }

    next = entry->next;
    pump = FUN_00411aa0(entry->x - 1, entry->y + 1);
    if (pump != NULL) {
        FUN_00411b20(pump);
    }

    free(entry);

    if (entry == (struct RoadQueueEntry *)DAT_004cbeac) {
        DAT_004cbeac = (struct RideQueueEntry *)next;
        return;
    }

    cur = (struct RoadQueueEntry *)DAT_004cbeac;
    while (cur->next != entry) {
        cur = cur->next;
    }
    cur->next = next;
}

// FUNCTION: LEGOLAND 0x00413450
void FUN_00413450(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004134f0
struct RoadTile *FUN_004134f0(int arg1, int arg2, struct RoadTile *tile) {
    if (tile != NULL) {
        if ((tile->flags & 0xf) != 6) {
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

// FUNCTION: LEGOLAND 0x00413520
void FUN_00413520(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004135d0
int FUN_004135d0(int x, int y, struct NeighborResult *out) {
    struct RoadQueueEntry *e;
    int count = 0;

    e = (struct RoadQueueEntry *)FUN_004125a0(x, y - 4);
    if (e != NULL) {
        count = 1;
    }
    if (out != NULL) {
        out->field_0 = e;
    }

    e = (struct RoadQueueEntry *)FUN_004125a0(x + 4, y);
    if (e != NULL) {
        count++;
    }
    if (out != NULL) {
        out->field_8 = e;
    }

    e = (struct RoadQueueEntry *)FUN_004125a0(x, y + 4);
    if (e != NULL) {
        count++;
    }
    if (out != NULL) {
        out->field_10 = e;
    }

    e = (struct RoadQueueEntry *)FUN_004125a0(x - 4, y);
    if (e != NULL) {
        count++;
    }
    if (out != NULL) {
        out->field_18 = e;
    }

    return count;
}

// FUNCTION: LEGOLAND 0x00413650
void FUN_00413650(short param_1, int param_2, int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x00413970
unsigned int FUN_00413970(unsigned short param_1) {
    struct RoadQueueEntry *entry = (struct RoadQueueEntry *)DAT_004cbeac;
    unsigned int count = 0;

    while (entry != NULL) {
        if (entry->field_8 == param_1) {
            count++;
        }
        entry = entry->next;
    }
    return count;
}

// FUNCTION: LEGOLAND 0x00413990
unsigned char FUN_00413990(unsigned int param_1, unsigned int param_2) {
    struct RoadTile *tile = FUN_004125f0(param_1 >> 8, param_2 >> 8);
    if (tile == NULL) {
        return 2;
    }
    if (tile->field_1c == 0) {
        return 1;
    }
    return 3;
}

// FUNCTION: LEGOLAND 0x004139c0
void FUN_004139c0(unsigned int param_1, unsigned int param_2) {
    struct RoadTile *tile = FUN_004125f0(param_1 >> 8, param_2 >> 8);
    if (tile != NULL) {
        tile->field_1d++;
    }
}

// FUNCTION: LEGOLAND 0x004139e0
void FUN_004139e0(unsigned int param_1, unsigned int param_2) {
    struct RoadTile *tile = FUN_004125f0(param_1 >> 8, param_2 >> 8);
    if (tile != NULL && tile->field_1d != 0) {
        tile->field_1d--;
    }
}

// FUNCTION: LEGOLAND 0x00413a10
void FUN_00413a10(struct LLIDB_Head *head) {
    struct LLIDB_Head **handle = &head;

    DAT_0082c684 = head->callbacks;
    // STRING: LEGOLAND 0x004b4c94
    if (LLIDB_FindElement("DSCHOOL LIGHTS", (unsigned int *)handle, 0) == 0) {
        DAT_0082c680 = LLIDB_LoadData(head);
    }
    // STRING: LEGOLAND 0x004b4c80
    if (LLIDB_FindElement("TILES FOR DSCHOOL", (unsigned int *)handle, 0) == 0) {
        struct RoadCallbacks *callbacks = head->callbacks;
        callbacks->field_18 = FUN_00413990;
        callbacks->field_1c = FUN_004139c0;
        callbacks->field_20 = FUN_004139e0;
    }
}

// FUNCTION: LEGOLAND 0x00413a80
void FUN_00413a80(void) {
    struct RoadQueueEntry *entry = (struct RoadQueueEntry *)DAT_004cbeac;
    unsigned int handle;
    struct RoadQueueEntry *next;

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
void FUN_00413ad0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0082c684;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&DAT_004b4bf0);
    EditCursor.field_1828 |= 8;
    BuildCursorPtr(&EditCursor, 0x8f8, 0);
    DefaultCursor(&DAT_0082f760);
    memcpy(&DAT_00830b74, &DAT_004b4bf0, 20);
    DAT_00830f88 = 0x34;
}

// FUNCTION: LEGOLAND 0x00413b50
void FUN_00413b50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00413e30
void FUN_00413e30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00413fa0
void FUN_00413fa0(unsigned int dummy, struct RoadPlaceArg *param) {
    struct RoadTile *tile = FUN_004125f0(param->field_0, param->field_4);
    unsigned int *src;
    unsigned int *dst;
    unsigned int count;

    if (tile != NULL) {
        QueryCursor.field_1404 = tile->x;
        QueryCursor.field_1408 = tile->y;

        src = &DAT_004b4bf0[0];
        dst = &QueryCursor.field_1414[0];
        count = 5;
        while (count != 0) {
            *dst = *src;
            src++;
            dst++;
            count--;
        }

        QueryCursor.field_1828 = 8;
        FUN_0045f480(&QueryCursor, 1);

        if ((tile->flags & 0xf) != 6) {
            FUN_0045f460(&QueryCursor);
        }
    }
}

// FUNCTION: LEGOLAND 0x00414020
void FUN_00414020(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414220
void FUN_00414220(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414440
void FUN_00414440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414830
void FUN_00414830(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0082c678;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&DAT_004b4bf0);
    EditCursor.field_1828 |= 8;
    BuildCursorPtr(&EditCursor, 0, 0);
}

// FUNCTION: LEGOLAND 0x00414880
void FUN_00414880(struct RoadEditArg *param_1, unsigned int param_2, unsigned int param_3) {
    void *obj = param_1->field_c;
    struct RoadTile *tile;
    int cost;

    memcpy(EditCursor.field_1414, (char *)obj + 0x3c, 20);
    EditCursor.field_1830 = 0;
    ScreenToMapRef(param_2, &EditCursor.field_1404, param_3);
    tile = FUN_004125f0(EditCursor.field_1404, EditCursor.field_1408);
    FUN_0045f480(&EditCursor, 0xe);
    cost = GetObjCost(obj);
    if (GetBrickCount() < cost) {
        return;
    }
    if (tile == NULL) {
        return;
    }
    EditCursor.field_1404 = tile->x;
    EditCursor.field_1408 = tile->y;
    if (tile->flags & 0x10) {
        return;
    }
    if ((tile->flags & 0xf) == 3) return;
    if ((tile->flags & 0xf) == 4) return;
    if ((tile->flags & 0xf) == 5) return;
    if ((tile->flags & 0xf) == 6) return;
    if (FUN_00411aa0(tile->x - 1, tile->y + 1) != 0) {
        return;
    }
    FUN_0045f460(&EditCursor);
}

// FUNCTION: LEGOLAND 0x00414940
void FUN_00414940(struct RoadEditArg *param_1) {
    DAT_0082c678 = param_1->field_c;
}

// FUNCTION: LEGOLAND 0x00414950
void FUN_00414950(void) { STUB(); }
