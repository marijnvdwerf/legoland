#include "legoland.h"
#include "crt.h"

#include "pathfind.h"
#include "tilemap.h"
#include "llidb.h"
#include "globals.h"

struct MapTile {
    unsigned char pad_0[8];
    unsigned short tile;
    unsigned short base_id;
    unsigned char flags_c;
    unsigned char pad_d[3];
    unsigned char flags_10;
};

struct LegoConfig {
    unsigned char pad_0[0x14];
    unsigned short field_14;
    unsigned short field_16;
    unsigned char pad_18[0x20 - 0x18];
    unsigned short field_20;
    unsigned short field_22;
};

struct TileSprite {
    unsigned char pad_0[0x16];
    short size;
};

struct Point {
    int x;
    int y;
};

#include "image_sprite.h"

struct FXSpriteList {
    unsigned char pad_0[0xc];
    int *sprite_ids;
};

// FUNCTION: LEGOLAND 0x0045a9b0
LEGO_EXPORT unsigned int *AllocTileSpace(void *manager, int count, unsigned int *out) {
    struct FXSpriteList *src = (struct FXSpriteList *)manager;
    int base;
    int run;
    unsigned int *slot;
    unsigned int *cursor;
    unsigned int i;
    int slot_value;

    count = count & 0xffff;
    base = 0;
    while (count + base <= 0x800) {
        slot = (unsigned int *)&TileSpriteArray[base];
        run = 0;
        slot_value = (int)TileSpriteArray[base];
        while (slot_value == -1) {
            run++;
            slot++;
            if (run >= count) {
                cursor = (unsigned int *)&TileSpriteArray[base];
                for (i = (unsigned int)count; i != 0; i--) {
                    *cursor = 0;
                    cursor++;
                }
                for (i = 0; (int)i < count; i++) {
                    TileSpriteInfo[base + i].src = src;
                    if (src != NULL) {
                        TileSpriteInfo[base + i].sprite = (unsigned short)src->sprite_ids[i];
                    } else {
                        TileSpriteInfo[base + i].sprite = 0;
                    }
                }
                *(unsigned short *)out = (unsigned short)base;
                return (unsigned int *)&TileSpriteArray[base];
            }
            slot_value = (int)*slot;
        }
        base = base + 1 + run;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0045aa50
int FUN_0045aa50(struct FXSpriteList **out) {
    int count = 0;
    struct FXSpriteList *prev = NULL;
    void **slot = TileSpriteArray;
    struct TileSpriteEntry *info = TileSpriteInfo;
    struct FXSpriteList *src;

    do {
        if (*slot != (void *)-1 && (src = info->src) != NULL && src != prev) {
            *out = src;
            count++;
            prev = src;
            out++;
        }
        slot++;
        info++;
    } while ((int)slot < (int)&TileSpriteArray[2048]);
    return count;
}

// FUNCTION: LEGOLAND 0x0045aa90
LEGO_EXPORT void FreeTileSpace(unsigned short index, unsigned short count) {
    memset(&TileSpriteArray[index], 0xFF, count * 4);
}

// FUNCTION: LEGOLAND 0x0045aad0
LEGO_EXPORT unsigned int LoadMapTiles(void) {
    unsigned short alloc_index;
    unsigned int handle;
    int offset;
    unsigned int row;
    unsigned int p;

    if (DAT_00667c9c == 0) {
        p = (unsigned int)calloc(0x14041f, 1);
        GameMap = (struct MapElement **)((p + 0x1f) & 0xffffffe0);
        row = (p + 0x41f) & 0xffffffe0;
        DAT_00667c9c = (void *)p;
        offset = 0;
        do {
            *(unsigned int *)((char *)GameMap + offset) = row;
            offset += 4;
            row += 0x1400;
        } while (offset < 0x400);
    }
    FreeTileSpace(0, 0x800);
    AllocTileSpace(0, 1, (unsigned int *)&alloc_index);
    LLIDB_FindElement("MAPPING 1", &handle, 0);
    LLIDB_LoadData((void *)handle);
    LLIDB_FindElement("BASIC TILES 1", &handle, 0);
    DAT_00801a6c = *(void **)(handle + 0xc);
    DAT_00667ca4 = *(unsigned int *)DAT_00801a6c;
    DAT_008003f8 = 0;
    DAT_00801b20 = 1;
    DAT_0080ff60 = 2;
    DAT_00805f48 = 3;
    DAT_0080ff68 = 4;
    LLIDB_FindElement("NORMAL PATH TILES", &handle, 0);
    PathSprite = *(void **)(handle + 0xc);
    // STRING: LEGOLAND 0x004b9bc4
    DAT_00667c8c = LoadSprite("arrow01.lls", 1);
    // STRING: LEGOLAND 0x004b9bb8
    DAT_00667c88 = LoadSprite("arrow02.lls", 1);
    // STRING: LEGOLAND 0x004b9bac
    DAT_00667c94 = LoadSprite("arrow03.lls", 1);
    // STRING: LEGOLAND 0x004b9ba0
    DAT_00667c90 = LoadSprite("arrow04.lls", 1);
    return 1;
}

// FUNCTION: LEGOLAND 0x0045ac20
unsigned int FUN_0045ac20(void) {
    unsigned int handle;

    if (DAT_00667c9c != 0) {
        free(DAT_00667c9c);
    }
    // STRING: LEGOLAND 0x004b9be4
    LLIDB_FindElement("MAPPING 1", &handle, 0);
    LLIDB_UnLoadData(handle);
    if (DAT_00667c8c != 0) {
        KillSprite(DAT_00667c8c);
        DAT_00667c8c = 0;
    }
    if (DAT_00667c88 != 0) {
        KillSprite(DAT_00667c88);
        DAT_00667c88 = 0;
    }
    if (DAT_00667c94 != 0) {
        KillSprite(DAT_00667c94);
        DAT_00667c94 = 0;
    }
    if (DAT_00667c90 != 0) {
        KillSprite(DAT_00667c90);
        DAT_00667c90 = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0045acc0
LEGO_EXPORT void GetTileBounds(struct Point *ref, int *out) {
    struct TileSprite *sprite = (struct TileSprite *)TileSpriteArray[DAT_00667ca4];
    short size = sprite->size;
    int top;

    out[0] = (short)(((short)(size * 2) + 1) >> 1) * ((ref->x - ref->y) - 1) + lpConfig->field_20 - (ScrollX >> 8);
    top = (short)((size + 1) >> 1) * (ref->x + ref->y) + lpConfig->field_22 - (ScrollY >> 8);
    out[1] = top;
    out[2] = (short)(size * 2) - 1 + out[0];
    out[3] = top - 1 + size;
}

// FUNCTION: LEGOLAND 0x0045ad60
LEGO_EXPORT void GetTileCentre(struct Point *ref, int *out) {
    struct TileSprite *sprite = (struct TileSprite *)TileSpriteArray[DAT_00667ca4];
    short size = sprite->size;

    out[0] = (short)(((short)(size * 2) + 1) >> 1) * (ref->x - ref->y) + lpConfig->field_20 - (ScrollX >> 8);
    out[1] = (short)((size + 1) >> 1) * (ref->y + 1 + ref->x) + lpConfig->field_22 - (ScrollY >> 8);
}

// FUNCTION: LEGOLAND 0x0045ade0
void FUN_0045ade0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045b170
void FUN_0045b170(void) {
}

// FUNCTION: LEGOLAND 0x0045b180
LEGO_EXPORT void RenderView(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045bcd0
LEGO_EXPORT void PointToIsoPlane(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045be00
LEGO_EXPORT void ScreenToMapRef2(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045be90
LEGO_EXPORT void ScreenToMapRef(unsigned int x, void *out, unsigned int y) { STUB(); }

// FUNCTION: LEGOLAND 0x0045c010
LEGO_EXPORT unsigned char Dir_To_Bit(unsigned char param) {
    return DAT_004b9550[param & 7];
}

// FUNCTION: LEGOLAND 0x0045c020
LEGO_EXPORT unsigned char Bit_To_Dir(unsigned char bit) {
    unsigned char result;

    for (result = 0; result < 8; result++) {
        if (DAT_004b9550[result] & bit) {
            return result;
        }
    }
    return 8;
}

// FUNCTION: LEGOLAND 0x0045c050
LEGO_EXPORT void Get_Path_Directions(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045c440
void FUN_0045c440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045c830
LEGO_EXPORT unsigned char ExcludeIsolatedDiags(unsigned char param) {
    unsigned char result = param;

    if ((result & 0x5) != 0x5) {
        result &= 0xfd;
    }
    if ((result & 0x41) != 0x41) {
        result &= 0x7f;
    }
    if ((result & 0x14) != 0x14) {
        result &= 0xf7;
    }
    if ((result & 0x50) != 0x50) {
        result &= 0xdf;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0045c870
LEGO_EXPORT void AdjustTileRFFlags(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045c900
void FUN_0045c900(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045c9c0
void FUN_0045c9c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045ca90
unsigned int FUN_0045ca90(unsigned int arg, unsigned int *buf) { STUB(); }

// FUNCTION: LEGOLAND 0x0045cb20
void FUN_0045cb20(unsigned int *buf) { STUB(); }

// FUNCTION: LEGOLAND 0x0045cb90
void FUN_0045cb90(struct Point *param) {
    struct MapTile *tile;
    unsigned short value;

    tile = (struct MapTile *)GameMap[param->y];
    value = *(unsigned short *)PathSprite;
    *(unsigned short *)((unsigned char *)tile + param->x * 0x14 + 8) = value;
}

// FUNCTION: LEGOLAND 0x0045cbc0
unsigned int FUN_0045cbc0(unsigned int buf, unsigned int index) { STUB(); }

// FUNCTION: LEGOLAND 0x0045cd00
void FUN_0045cd00(unsigned int *buf) {
    int zero_count = 0;
    int count = 0;

    do {
        if (FUN_0045cbc0((unsigned int)buf, (unsigned int)count) != 0) {
            zero_count = 0;
        } else {
            zero_count++;
        }
        count++;
        count = count & 3;
    } while (zero_count < 4);
}

// FUNCTION: LEGOLAND 0x0045cd30
void FUN_0045cd30(unsigned int arg) {
    unsigned int buf[4];

    if (FUN_0045ca90(arg, buf) != 0) {
        FUN_0045cd00(buf);
        FUN_0045cb20(buf);
    }
}

// FUNCTION: LEGOLAND 0x0045cd70
void FUN_0045cd70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045ce10
int FUN_0045ce10(struct MapTile *tile) {
    if ((tile->flags_10 & 0x1) == 0) {
        if ((tile->flags_c & 0x10) != 0 && (tile->flags_10 & 0x2) == 0) {
            return 1;
        }
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0045ce30
void FUN_0045ce30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045ceb0
unsigned char FUN_0045ceb0(int *coords) { STUB(); }

// FUNCTION: LEGOLAND 0x0045d080
unsigned char FUN_0045d080(unsigned char flags, int *coords) {
    unsigned char result = 0;
    unsigned int tile_data[5];
    unsigned int *map_row;
    int x, y;

    if ((flags & 0xc) == 0xc) {
        x = coords[0] - 1;
        y = coords[1] + 1;
        if (x >= 0 && x < lpConfig->field_14 && y >= 0 && y < lpConfig->field_16) {
            map_row = (unsigned int *)GameMap[y];
            memcpy(tile_data, map_row + x * 5, sizeof(tile_data));
        } else {
            *(unsigned short *)((unsigned char *)tile_data + 0x8) = 0;
            *(unsigned short *)((unsigned char *)tile_data + 0xc) = 0x40;
            *(unsigned char *)((unsigned char *)tile_data + 0x10) = 0;
        }
        if (!FUN_0045ce10((struct MapTile *)tile_data)) {
            result = 1;
        }
    }
    if ((flags & 0x3) == 0x3) {
        x = coords[0] + 1;
        y = coords[1] - 1;
        if (x >= 0 && x < lpConfig->field_14 && y >= 0 && y < lpConfig->field_16) {
            map_row = (unsigned int *)GameMap[y];
            memcpy(tile_data, map_row + x * 5, sizeof(tile_data));
        } else {
            *(unsigned short *)((unsigned char *)tile_data + 0x8) = 0;
            *(unsigned short *)((unsigned char *)tile_data + 0xc) = 0x40;
            *(unsigned char *)((unsigned char *)tile_data + 0x10) = 0;
        }
        if (!FUN_0045ce10((struct MapTile *)tile_data)) {
            result |= 2;
        }
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0045d1a0
LEGO_EXPORT void AdjustPathTile(struct Point *p, unsigned int a) { STUB(); }

// FUNCTION: LEGOLAND 0x0045d260
void FUN_0045d260(struct Point *param) {
    unsigned int edi = *(unsigned int *)PathSprite;
    struct Point point;

    AdjustPathTile(param, edi);

    point.x = param->x;
    point.y = param->y - 1;
    AdjustPathTile(&point, edi);

    point.x = param->x + 1;
    point.y = param->y;
    AdjustPathTile(&point, edi);

    point.x = param->x;
    point.y = param->y + 1;
    AdjustPathTile(&point, edi);

    point.x = param->x - 1;
    point.y = param->y;
    AdjustPathTile(&point, edi);

    point.x = param->x - 1;
    point.y = param->y + 1;
    AdjustPathTile(&point, edi);

    point.x = param->x + 1;
    point.y = param->y - 1;
    AdjustPathTile(&point, edi);

    point.x = param->x + 1;
    point.y = param->y + 1;
    AdjustPathTile(&point, edi);

    point.x = param->x - 1;
    point.y = param->y - 1;
    AdjustPathTile(&point, edi);
}

// FUNCTION: LEGOLAND 0x0045d350
LEGO_EXPORT void AddPathTileGFX(struct Point *p, unsigned short param1) { STUB(); }

// FUNCTION: LEGOLAND 0x0045d3b0
LEGO_EXPORT void AddPathTile(struct Point *p, unsigned short param1) {
    AddPathTileGFX(p, param1);
    /* struct Point and struct InstancePos are identical {x,y} layouts; cast
       bridges tilemap's local Point to pathfind's InstancePos (no shared type). */
    AddPathSquare((struct InstancePos *)p);
}

// FUNCTION: LEGOLAND 0x0045d3d0
void FUN_0045d3d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045d560
void FUN_0045d560(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045d5d0
void FUN_0045d5d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045d730
void FUN_0045d730(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045d770
void FUN_0045d770(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045da60
LEGO_EXPORT unsigned short RestoreBaseMap(int tile_x, int row_y) {
    struct MapTile *tile = (struct MapTile *)((char *)GameMap[row_y] + tile_x * 0x14);
    unsigned short id = tile->base_id;

    tile->tile = id;
    return TileSpriteInfo[id].sprite & 0x20;
}

// FUNCTION: LEGOLAND 0x0045daa0
LEGO_EXPORT void RemovePathTile(unsigned int param_1, unsigned short param_2) { STUB(); }
