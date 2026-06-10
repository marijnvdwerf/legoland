#include "legoland.h"
#include "math.h"
#include "crt.h"

#include "pathfind.h"
#include "tilemap.h"
#include "llidb.h"
#include "globals.h"

struct MapTile {
    /* 0x00 */ unsigned char pad_0[8];
    /* 0x08 */ unsigned short tile;
    /* 0x0a */ unsigned short base_id;
    /* 0x0c */ unsigned short flags_c;
    /* 0x0e */ unsigned char pad_e[2];
    /* 0x10 */ unsigned char flags_10;
    /* 0x11 */ unsigned char pad_11[3];
};


struct TileSprite {
    unsigned char pad_0[0x16];
    short size;
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
LEGO_EXPORT void PointToIsoPlane(int *param_1, int *out) {
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    char cVar7;
    int iVar8;
    int iVar9;
    short size;

    size = ((struct TileSprite *)TileSpriteArray[DAT_00667ca4])->size;
    iVar9 = (int)size;
    iVar8 = (short)(size * 2);
    iVar4 = iVar8 + 1 >> 1;
    iVar1 = (*param_1 + iVar4) / iVar8;
    iVar5 = (*param_1 + iVar4) % iVar8;
    iVar2 = param_1[1] / iVar9;
    iVar6 = param_1[1] % iVar9;
    iVar3 = iVar2 + iVar1;
    *out = iVar3;
    iVar2 = iVar2 - iVar1;
    out[1] = iVar2;
    if (iVar5 < 0) {
        iVar5 = iVar5 + -2 + iVar8;
        out[1] = iVar2 + 1;
        *out = iVar3 + -1;
    }
    if (iVar6 < 0) {
        iVar6 = iVar6 + -1 + iVar9;
        out[1] = out[1] + -1;
        *out = *out + -1;
    }
    cVar7 = (iVar4 <= iVar5) + '\x01';
    if (iVar9 + 1 >> 1 < iVar6) {
        cVar7 = (iVar4 <= iVar5) + '\x03';
    }
    switch (cVar7) {
    case '\x01':
        if (iVar5 < iVar4 + iVar6 * -2) {
            *out = *out + -1;
            return;
        }
        break;
    case '\x02':
        if (iVar4 + iVar6 * 2 <= iVar5) {
            out[1] = out[1] + -1;
            return;
        }
        break;
    case '\x03':
        if (iVar5 < iVar4 + (iVar6 - iVar9) * 2) {
            out[1] = out[1] + 1;
            return;
        }
        break;
    case '\x04':
        if (iVar4 + (iVar9 - iVar6) * 2 <= iVar5) {
            *out = *out + 1;
        }
    }
}

// FUNCTION: LEGOLAND 0x0045be00
LEGO_EXPORT unsigned int ScreenToMapRef2(int *param_1, int *out) {
    struct TileSprite *sprite;
    short size;
    int twice;
    int ix;
    int iy;

    sprite = (struct TileSprite *)TileSpriteArray[DAT_00667ca4];
    if (sprite == NULL) {
        return 0xffffffff;
    }
    size = sprite->size;
    twice = (short)(size * 2);
    ix = (((twice + 1 >> 1) - lpConfig->field_20) + (ScrollX >> 8) + *param_1) * (0x100 / twice);
    iy = (((ScrollY >> 8) - lpConfig->field_22) + param_1[1]) * (0x100 / (int)size);
    *out = iy + ix;
    out[1] = iy - ix;
    return 0;
}

// FUNCTION: LEGOLAND 0x0045be90
LEGO_EXPORT unsigned int ScreenToMapRef(int *param_1, int *out, unsigned int param_3) {
    struct TileSprite *sprite;
    short size;
    int w;
    int twice;
    int half;
    int sx;
    int sy;
    int qx;
    int rx;
    int qy;
    int ry;
    char sel;

    sprite = (struct TileSprite *)TileSpriteArray[DAT_00667ca4];
    if (sprite == NULL) {
        return 0xffffffff;
    }
    size = sprite->size;
    w = (int)size;
    twice = (short)(size * 2);
    half = twice + 1 >> 1;
    sx = ((ScrollX >> 8) - lpConfig->field_20) + *param_1 + half;
    sy = ((ScrollY >> 8) - lpConfig->field_22) + param_1[1];
    qx = sx / twice;
    rx = sx % twice;
    qy = sy / w;
    ry = sy % w;
    *out = qx + qy;
    out[1] = qy - qx;
    if (rx < 0) {
        rx = rx + -2 + twice;
        out[1] = (qy - qx) + 1;
        *out = (qx + qy) + -1;
    }
    if (ry < 0) {
        out[1] = out[1] + -1;
        *out = *out + -1;
        ry = ry + -1 + w;
    }
    sel = (char)(half <= rx) + '\x01';
    if (w + 1 >> 1 < ry) {
        sel = (char)(half <= rx) + '\x03';
    }
    switch (sel) {
    case '\x01':
        if (rx < half + ry * -2) {
            *out = *out + -1;
            return 1;
        }
        break;
    case '\x02':
        if (half + ry * 2 <= rx) {
            out[1] = out[1] + -1;
            return 1;
        }
        break;
    case '\x03':
        if (rx < half + (ry - w) * 2) {
            out[1] = out[1] + 1;
            return 1;
        }
        break;
    case '\x04':
        if (half + (w - ry) * 2 <= rx) {
            *out = *out + 1;
        }
    }
    return 1;
}

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
LEGO_EXPORT unsigned char Get_Path_Directions(int *param_1, char *param_2, char *param_3) {
    int x;
    int y;
    int ym1;
    int yp1;
    int xp1;
    char local_15;
    char local_16;
    unsigned char local_17;
    struct MapTile tile;

    y = param_1[1];
    x = *param_1;
    ym1 = y + -1;
    local_15 = '\0';
    local_16 = '\0';
    local_17 = 0;
    if (x < 0 || x >= (int)lpConfig->width || ym1 < 0 || ym1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[ym1] + x * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_16 = '\x01';
        local_17 = 1;
    }
    yp1 = y + 1;
    if (x < 0 || x >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[yp1] + x * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 0x10;
    }
    xp1 = x + 1;
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[y] + xp1 * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 4;
    }
    x = x + -1;
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[y] + x * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 0x40;
    }
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || ym1 < 0 || ym1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[ym1] + xp1 * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_17 = local_17 | 2;
        local_15 = '\x01';
    }
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[yp1] + xp1 * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 8;
    }
    if (x < 0 || x >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[yp1] + x * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 0x20;
    }
    if (x < 0 || x >= (int)lpConfig->width || ym1 < 0 || ym1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[ym1] + x * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 0x80;
    }
    if (param_2 != NULL) {
        *param_2 = local_16;
    }
    if (param_3 != NULL) {
        *param_3 = local_15;
    }
    return local_17;
}

// FUNCTION: LEGOLAND 0x0045c440
unsigned char FUN_0045c440(int *param_1, char *param_2, char *param_3) {
    int x;
    int y;
    int ym1;
    int yp1;
    int xp1;
    char local_15;
    char local_16;
    unsigned char local_17;
    struct MapTile tile;

    y = param_1[1];
    x = *param_1;
    ym1 = y + -1;
    local_15 = '\0';
    local_16 = '\0';
    local_17 = 0;
    if (x < 0 || x >= (int)lpConfig->width || ym1 < 0 || ym1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[ym1] + x * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_16 = '\x01';
        local_17 = 1;
    }
    yp1 = y + 1;
    if (x < 0 || x >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[yp1] + x * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 0x10;
    }
    xp1 = x + 1;
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[y] + xp1 * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 4;
    }
    x = x + -1;
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[y] + x * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 0x40;
    }
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || ym1 < 0 || ym1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[ym1] + xp1 * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_17 = local_17 | 2;
        local_15 = '\x01';
    }
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[yp1] + xp1 * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 8;
    }
    if (x < 0 || x >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[yp1] + x * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 0x20;
    }
    if (x < 0 || x >= (int)lpConfig->width || ym1 < 0 || ym1 >= (int)lpConfig->height) {
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[ym1] + x * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 0x80;
    }
    if (param_2 != NULL) {
        *param_2 = local_16;
    }
    if (param_3 != NULL) {
        *param_3 = local_15;
    }
    return local_17;
}

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
LEGO_EXPORT void AdjustTileRFFlags(int *param_1) {
    struct MapTile *tile;
    unsigned int flags;
    char local_1;

    tile = (struct MapTile *)((char *)GameMap[param_1[1]] + *param_1 * 0x14);
    tile->flags_10 = tile->flags_10 & 0xc3;
    flags = FUN_0045c440(param_1, (char *)&param_1, &local_1);
    if ((char)param_1 == '\0') {
        tile->flags_10 = tile->flags_10 | 0x10;
        return;
    }
    if ((char)param_1 == '\x01') {
        tile->flags_10 = tile->flags_10 | 0x10;
        return;
    }
    if ((char)param_1 == '\x02') {
        if ((flags & 0x11) == 1 || (flags & 0x11) == 0x10) {
            tile->flags_10 = tile->flags_10 | 8;
            return;
        }
    } else {
        if ((char)param_1 == '\x03') {
            tile->flags_10 = tile->flags_10 | 4;
            return;
        }
        if ((char)param_1 == '\x04') {
            tile->flags_10 = tile->flags_10 | 0x20;
        }
    }
}

// FUNCTION: LEGOLAND 0x0045c900
int FUN_0045c900(struct MapRect *param_1) {
    int x;
    int x1;
    int y0;
    int y1;
    int xoff;
    int y;
    struct MapTile tile;

    x = param_1->x0;
    x1 = param_1->x1;
    if (x <= x1) {
        y0 = param_1->y0;
        y1 = param_1->y1;
        xoff = x * 0x14;
        y = y0;
        do {
            for (; y <= y1; y = y + 1) {
                if (xoff < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
                    tile.flags_c = 0;
                    tile.flags_10 = 0;
                } else {
                    tile = *(struct MapTile *)((char *)GameMap[y] + xoff);
                }
                if ((tile.flags_c & 0x10) == 0 || (tile.flags_10 & 2) != 0) {
                    return 0;
                }
            }
            x = x + 1;
            xoff = xoff + 0x14;
            y = y0;
        } while (x <= x1);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0045c9c0
unsigned int FUN_0045c9c0(int *param_1) {
    int y;
    int yend;
    unsigned int result;
    unsigned int acc;
    unsigned int bit;
    int x;
    int xoff;
    int xi;
    struct MapTile tile;

    y = param_1[1];
    result = 0;
    acc = 0;
    bit = 0x1000000;
    yend = y + 5;
    if (y < yend) {
        x = *param_1;
        do {
            if (x < x + 5) {
                xoff = x * 0x14;
                xi = x;
                do {
                    if (xoff < 0 || xi >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
                        tile.flags_c = 0;
                        tile.flags_10 = 0;
                    } else {
                        tile = *(struct MapTile *)((char *)GameMap[y] + xoff);
                    }
                    if ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0) {
                        acc = acc | bit;
                    }
                    xoff = xoff + 0x14;
                    bit = bit >> 1;
                    xi = xi + 1;
                    result = acc;
                } while (xi < x + 5);
            }
            y = y + 1;
        } while (y < yend);
        return result;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045ca90
int FUN_0045ca90(int *param_1, int *param_2) {
    int dx;
    int x;
    unsigned int mask;
    unsigned int *tbl;
    int i;
    int local_8;
    int local_4;

    local_8 = *param_1 + -2;
    local_4 = param_1[1] + -2;
    mask = FUN_0045c9c0(&local_8);
    i = 0;
    tbl = DAT_004b9558;
    do {
        if ((*tbl & mask) == *tbl) {
            dx = DAT_004b957c[i];
            x = *param_1;
            param_2[0] = dx + x;
            i = DAT_004b95a0[i] + param_1[1];
            param_2[1] = i;
            param_2[2] = dx + x + 2;
            param_2[3] = i + 2;
            return 1;
        }
        tbl = tbl + 1;
        i = i + 1;
    } while ((int)tbl < (int)&DAT_004b957c);
    return 0;
}

// FUNCTION: LEGOLAND 0x0045cb20
void FUN_0045cb20(struct MapRect *param_1) {
    unsigned int parity;
    int x;
    int xoff;
    unsigned int sum;
    int y;

    y = param_1->y0;
    if (y <= param_1->y1) {
        do {
            x = param_1->x0;
            if (x <= param_1->x1) {
                sum = x + y;
                xoff = x * 0x14;
                do {
                    parity = sum & 1;
                    xoff = xoff + 0x14;
                    x = x + 1;
                    sum = sum + 1;
                    *(unsigned short *)((char *)GameMap[y] + -0xc + xoff) =
                        (unsigned short)((2 - (parity != 0) & 0xff) + *(unsigned int *)PathSprite);
                } while (x <= param_1->x1);
            }
            y = y + 1;
        } while (y <= param_1->y1);
    }
}

// FUNCTION: LEGOLAND 0x0045cb90
void FUN_0045cb90(struct Point *param) {
    struct MapTile *tile;
    unsigned short value;

    tile = (struct MapTile *)GameMap[param->y];
    value = *(unsigned short *)PathSprite;
    *(unsigned short *)((unsigned char *)tile + param->x * 0x14 + 8) = value;
}

// FUNCTION: LEGOLAND 0x0045cbc0
int FUN_0045cbc0(int *param_1, int param_2) {
    int result;
    struct MapRect r;

    switch (param_2) {
    case 2:
        r.x1 = param_1[2];
        r.y0 = param_1[1] + -1;
        r.x0 = *param_1;
        r.y1 = r.y0;
        result = FUN_0045c900(&r);
        if (result != 0) {
            param_1[1] = param_1[1] + -1;
            return 1;
        }
        break;
    case 0:
        r.x1 = param_1[2];
        r.y0 = param_1[3] + 1;
        r.x0 = *param_1;
        r.y1 = r.y0;
        result = FUN_0045c900(&r);
        if (result != 0) {
            param_1[3] = param_1[3] + 1;
            return 1;
        }
        break;
    case 1:
        r.y0 = param_1[1];
        r.y1 = param_1[3];
        r.x0 = param_1[2] + 1;
        r.x1 = r.x0;
        result = FUN_0045c900(&r);
        if (result != 0) {
            param_1[2] = param_1[2] + 1;
            return 1;
        }
        break;
    case 3:
        r.y0 = param_1[1];
        r.y1 = param_1[3];
        r.x0 = *param_1 + -1;
        r.x1 = r.x0;
        result = FUN_0045c900(&r);
        if (result != 0) {
            *param_1 = *param_1 + -1;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045cd00
void FUN_0045cd00(int *buf) {
    int zero_count = 0;
    int count = 0;

    do {
        if (FUN_0045cbc0(buf, count) != 0) {
            zero_count = 0;
        } else {
            zero_count++;
        }
        count++;
        count = count & 3;
    } while (zero_count < 4);
}

// FUNCTION: LEGOLAND 0x0045cd30
void FUN_0045cd30(int *arg) {
    struct MapRect buf;

    if (FUN_0045ca90(arg, (int *)&buf) != 0) {
        FUN_0045cd00((int *)&buf);
        FUN_0045cb20(&buf);
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
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
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
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
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
int FUN_0045d560(struct MapRect *out, struct MapRect *a, struct MapRect *b) {
    if (a->x0 > b->x0) {
        out->x0 = b->x0;
    } else {
        out->x0 = a->x0;
    }
    if (a->x1 < b->x1) {
        out->x1 = a->x1;
    } else {
        out->x1 = b->x1;
    }
    if (a->y0 > b->y0) {
        out->y0 = b->y0;
    } else {
        out->y0 = a->y0;
    }
    if (a->y1 < b->y1) {
        out->y1 = a->y1;
    } else {
        out->y1 = b->y1;
    }
    if (out->x0 <= out->x1 && out->y0 <= out->y1) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045d5d0
void FUN_0045d5d0(struct MapRect *param) {
    struct MapRect result;
    struct MapRect *cur;
    int i;
    int count;
    int x0, y0, x1;
    int y1;
    int idx;

    i = 0;
    if (0 < DAT_00667d3c) {
        cur = DAT_00801a80;
        do {
            count = DAT_00667d3c;
            if (FUN_0045d560(&result, param, cur) != 0) {
                x0 = cur->x0;
                y0 = cur->y0;
                x1 = cur->x1;
                y1 = cur->y1;
                idx = DAT_00667d3c + -1;
                DAT_00667d3c = idx;
                if (i < idx) {
                    cur->x0 = DAT_00801a80[idx].x0;
                    cur->y0 = DAT_00801a80[idx].y0;
                    cur->x1 = DAT_00801a80[idx].x1;
                    cur->y1 = DAT_00801a80[idx].y1;
                }
                i = i + -1;
                cur = cur + -1;
                if (y0 < result.y0) {
                    DAT_00667d3c = count;
                    DAT_00801a80[idx].x0 = x0;
                    DAT_00801a80[idx].y0 = y0;
                    DAT_00801a80[idx].x1 = x1;
                    DAT_00801a80[idx].y1 = result.y0 + -1;
                    idx = count;
                }
                count = idx;
                if (x0 < result.x0) {
                    count = idx + 1;
                    DAT_00667d3c = count;
                    DAT_00801a80[idx].x0 = x0;
                    DAT_00801a80[idx].y0 = result.y0;
                    DAT_00801a80[idx].x1 = result.x0 + -1;
                    DAT_00801a80[idx].y1 = result.y1;
                }
                y0 = count;
                if (result.x1 < x1) {
                    y0 = count + 1;
                    DAT_00667d3c = y0;
                    DAT_00801a80[count].x0 = result.x1 + 1;
                    DAT_00801a80[count].y0 = result.y0;
                    DAT_00801a80[count].x1 = x1;
                    DAT_00801a80[count].y1 = result.y1;
                }
                count = y0;
                if (result.y1 < y1) {
                    count = y0 + 1;
                    DAT_00667d3c = count;
                    DAT_00801a80[y0].x0 = x0;
                    DAT_00801a80[y0].y0 = result.y1 + 1;
                    DAT_00801a80[y0].x1 = x1;
                    DAT_00801a80[y0].y1 = y1;
                }
            }
            cur = cur + 1;
            i = i + 1;
        } while (i < count);
    }
}

// FUNCTION: LEGOLAND 0x0045d730
int FUN_0045d730(struct MapRect *param) {
    int idx;
    struct MapRect *cur;

    FUN_0045d5d0(param);
    idx = DAT_00667d3c;
    cur = &DAT_00801a80[idx];
    cur->x0 = param->x0;
    cur->y0 = param->y0;
    cur->x1 = param->x1;
    idx = idx + 1;
    DAT_00667d3c = idx;
    cur->y1 = param->y1;
    return idx;
}

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
