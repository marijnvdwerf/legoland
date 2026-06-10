#include "legoland.h"
#include "math.h"
#include "crt.h"

#include "pathfind.h"
#include "tilemap.h"
#include "llidb.h"
#include "gamemap.h"
#include "clipping.h"
#include "print_sprite.h"
#include "globals.h"

extern void FUN_004779d0(struct Point *p);

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
void FUN_0045ade0(void) {
    int iVar1;
    int iVar2;
    int iVar3;
    char cVar4;
    int iVar7;
    int iVar8;
    int iVar9;
    int iVar10;
    int iVar11;
    int iVar13;
    int local_50;
    int local_4c;
    int local_44;
    int local_2c;
    unsigned int local_24[4];
    short size;
    struct MapTile tile;

    local_24[0] = lpConfig->field_20;
    local_24[1] = lpConfig->field_22;
    local_24[2] = lpConfig->field_0;
    local_24[3] = lpConfig->field_2;
    SetClipping((int *)local_24);
    size = ((struct TileSprite *)TileSpriteArray[DAT_00667ca4])->size;
    iVar9 = (int)size;
    iVar13 = (short)(size * 2);
    iVar1 = iVar9 + 1 >> 1;
    local_4c = (ScrollY >> 8) - iVar1;
    iVar2 = (ScrollX >> 8) / iVar13;
    iVar7 = iVar13 + 1 >> 1;
    local_50 = (ScrollX >> 8) % iVar13;
    iVar3 = local_4c / iVar9;
    local_4c = local_4c % iVar9;
    local_2c = iVar3 + -3 + iVar2;
    iVar3 = iVar3 - iVar2;
    cVar4 = (iVar7 <= local_50) + '\x01';
    if (iVar1 < local_4c) {
        cVar4 = (iVar7 <= local_50) + '\x03';
    }
    switch (cVar4) {
    case '\x01':
        if (local_50 < iVar7 + local_4c * -2) {
            local_50 = local_50 + iVar7;
            local_2c = local_2c + -1;
            local_4c = local_4c + iVar1;
        }
        break;
    case '\x02':
        if (iVar7 + local_4c * 2 <= local_50) {
            local_50 = local_50 - iVar7;
            iVar3 = iVar3 + -1;
            local_4c = local_4c + iVar1;
        }
        break;
    case '\x03':
        if (iVar7 + (local_4c - iVar9) * 2 <= local_50) {
            break;
        }
        iVar3 = iVar3 + 1;
        iVar2 = iVar7;
        local_50 = local_50 + iVar2;
        local_4c = local_4c - iVar1;
        break;
    case '\x04':
        if (local_50 < iVar7 + (iVar9 - local_4c) * 2) {
            break;
        }
        local_2c = local_2c + 1;
        iVar2 = -iVar7;
        local_50 = local_50 + iVar2;
        local_4c = local_4c - iVar1;
    }
    local_4c = (local_24[1] + iVar9 * -2) - local_4c;
    if (local_4c < (int)(iVar9 * 2 + local_24[3])) {
        do {
            iVar2 = local_2c;
            iVar8 = (local_24[0] + iVar13 * -2) - local_50;
            if (iVar8 < (int)(iVar13 * 2 + local_24[2])) {
                iVar11 = local_2c * 0x14;
                iVar10 = iVar3;
                do {
                    if (iVar11 < 0 || local_2c >= (int)lpConfig->width || iVar10 < 0 ||
                        iVar10 >= (int)lpConfig->height) {
                        tile.flags_10 = 0;
                    } else {
                        tile = *(struct MapTile *)((char *)GameMap[iVar10] + iVar11);
                    }
                    if ((tile.flags_10 & 2) != 0) {
                        PrintSprite((struct Sprite *)TileSpriteArray[(DAT_00805f48 & 0xff) + *(unsigned int *)DAT_00801a6c], iVar8, local_4c, 0xff6868, 0);
                    }
                    local_2c = local_2c + 1;
                    iVar11 = iVar11 + 0x14;
                    iVar8 = iVar8 + iVar13;
                    iVar10 = iVar10 + -1;
                } while (iVar8 < (int)(iVar13 * 2 + local_24[2]));
            }
            iVar2 = iVar2 + 1;
            iVar8 = (local_24[0] + iVar13 * -2) - local_50;
            if (iVar8 < (int)(iVar13 * 2 + local_24[2])) {
                local_44 = iVar2 * 0x14;
                iVar10 = iVar8 + iVar7;
                iVar11 = iVar3;
                local_2c = iVar2;
                do {
                    if (local_44 < 0 || local_2c >= (int)lpConfig->width || iVar11 < 0 ||
                        iVar11 >= (int)lpConfig->height) {
                        tile.flags_10 = 0;
                    } else {
                        tile = *(struct MapTile *)((char *)GameMap[iVar11] + local_44);
                    }
                    if ((tile.flags_10 & 2) != 0) {
                        PrintSprite((struct Sprite *)TileSpriteArray[(DAT_00805f48 & 0xff) + *(unsigned int *)DAT_00801a6c], iVar10, local_4c + iVar1, 0xff6868, 0);
                    }
                    local_2c = local_2c + 1;
                    local_44 = local_44 + 0x14;
                    iVar8 = iVar8 + iVar13;
                    iVar11 = iVar11 + -1;
                    iVar10 = iVar10 + iVar13;
                } while (iVar8 < (int)(iVar13 * 2 + local_24[2]));
            }
            iVar3 = iVar3 + 1;
            local_4c = local_4c + iVar9;
            local_2c = iVar2;
        } while (local_4c < (int)(iVar9 * 2 + local_24[3]));
    }
}

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
void FUN_0045cd70(int *param_1) {
    int iVar1;
    int local_18;
    int local_14;
    struct MapRect local_10;

    local_18 = *param_1 + -2;
    if (local_18 <= *param_1 + 2) {
        do {
            iVar1 = param_1[1];
            local_14 = iVar1 + -2;
            if (local_14 <= iVar1 + 2) {
                do {
                    if ((local_18 != *param_1 || local_14 != iVar1) &&
                        FUN_0045ce30(&local_18) != 0 &&
                        FUN_0045ca90(&local_18, (int *)&local_10) == 0) {
                        FUN_0045cb90((struct Point *)&local_18);
                    }
                    iVar1 = param_1[1];
                    local_14 = local_14 + 1;
                } while (local_14 <= iVar1 + 2);
            }
            local_18 = local_18 + 1;
        } while (local_18 <= *param_1 + 2);
    }
}

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
int FUN_0045ce30(int *param_1) {
    int x;
    int y;
    struct MapTile *tile;

    x = *param_1;
    if (x >= 0 && x < (int)lpConfig->width && (y = param_1[1], y >= 0) &&
        y < (int)lpConfig->height &&
        (tile = (struct MapTile *)((char *)GameMap[y] + x * 0x14), tile != NULL)) {
        if (FUN_0045ce10(tile) != 0) {
            tile = (struct MapTile *)((char *)GameMap[param_1[1]] + *param_1 * 0x14);
            return tile->tile != *(unsigned int *)PathSprite;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045ceb0
unsigned char FUN_0045ceb0(int *coords) {
    int x;
    int y;
    unsigned char local_15;
    struct MapTile tile;

    x = *coords;
    y = coords[1] + -1;
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.tile = 0;
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[y] + x * 0x14);
    }
    local_15 = FUN_0045ce10(&tile) != 0;
    y = coords[1];
    x = *coords + 1;
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.tile = 0;
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[y] + x * 0x14);
    }
    if (FUN_0045ce10(&tile) != 0) {
        local_15 = local_15 | 2;
    }
    x = *coords;
    y = coords[1] + 1;
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.tile = 0;
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[y] + x * 0x14);
    }
    if (FUN_0045ce10(&tile) != 0) {
        local_15 = local_15 | 4;
    }
    y = coords[1];
    x = *coords + -1;
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.tile = 0;
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[y] + x * 0x14);
    }
    if (FUN_0045ce10(&tile) != 0) {
        local_15 = local_15 | 8;
    }
    return local_15;
}

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
LEGO_EXPORT unsigned short *AdjustPathTile(struct Point *p, unsigned int a) {
    unsigned short *flags;
    int x;
    int y;
    struct MapTile tile;

    BGFullUpdate = 1;
    x = p->x;
    if (x < 0 || x >= (int)lpConfig->width || (y = p->y, y < 0) || y >= (int)lpConfig->height) {
        tile.tile = 0;
        tile.flags_c = 0x40;
        tile.flags_10 = 0;
    } else {
        tile = *(struct MapTile *)((char *)GameMap[y] + x * 0x14);
    }
    if ((tile.flags_10 & 1) != 0 || ((tile.flags_c & 0x10) != 0 && (tile.flags_10 & 2) == 0)) {
        AdjustTileRFFlags((int *)p);
    }
    flags = NULL;
    if (FUN_0045ce10(&tile) != 0) {
        flags = (unsigned short *)((char *)GameMap[p->y] + 0xc + p->x * 0x14);
        *flags = *flags & 0xfffc;
    }
    return flags;
}

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
LEGO_EXPORT void AddPathTileGFX(struct Point *p, unsigned short param1) {
    unsigned char *pb;

    pb = (unsigned char *)((char *)GameMap[p->y] + 0xc + p->x * 0x14);
    *pb = *pb | 0x10;
    *(unsigned short *)((char *)GameMap[p->y] + 8 + p->x * 0x14) = param1;
    FUN_0045d260(p);
    if (DAT_00832984 != 0) {
        FUN_0045cd30((int *)p);
    }
}

// FUNCTION: LEGOLAND 0x0045d3b0
LEGO_EXPORT void AddPathTile(struct Point *p, unsigned short param1) {
    AddPathTileGFX(p, param1);
    /* struct Point and struct InstancePos are identical {x,y} layouts; cast
       bridges tilemap's local Point to pathfind's InstancePos (no shared type). */
    AddPathSquare((struct InstancePos *)p);
}

struct PathFootprint {
    /* 0x00 */ unsigned char pad_0[0x20];
    /* 0x20 */ short type;
    /* 0x22 */ unsigned char pad_22[0x3c - 0x22];
    /* 0x3c */ int x_lo;
    /* 0x40 */ int y_lo;
    /* 0x44 */ int x_hi;
    /* 0x48 */ int y_hi;
};

// FUNCTION: LEGOLAND 0x0045d3d0
void FUN_0045d3d0(struct PathFootprint *param_1, int *param_2) {
    struct MapTile *tile;
    int x;
    int y;
    int xoff;
    struct Point local_8;

    if (param_1->type != 0 && param_1->type != 2) {
        y = param_2[1] + param_1->y_lo + -1;
        if (y <= param_2[1] + 1 + param_1->y_hi) {
            do {
                x = *param_2 + param_1->x_lo + -1;
                if (x <= *param_2 + param_1->x_hi + 1) {
                    xoff = x * 0x14;
                    do {
                        *(unsigned short *)((char *)GameMap[y] + 0xc + xoff) =
                            *(unsigned short *)((char *)GameMap[y] + 0xc + xoff) & 0xffe7;
                        *(unsigned char *)((char *)GameMap[y] + 0x10 + xoff) = 0;
                        tile = (struct MapTile *)GameMap[y];
                        *(unsigned short *)((char *)tile + xoff + 8) =
                            *(unsigned short *)((char *)tile + xoff + 0xa);
                        local_8.x = x;
                        local_8.y = y;
                        FUN_0045d260(&local_8);
                        RemovePathSquare((struct InstancePos *)&local_8);
                        x = x + 1;
                        xoff = xoff + 0x14;
                    } while (x <= *param_2 + param_1->x_hi + 1);
                }
                y = y + 1;
            } while (y <= param_2[1] + 1 + param_1->y_hi);
        }
        y = param_2[1] + param_1->y_lo;
        if (y <= param_2[1] + param_1->y_hi) {
            do {
                x = *param_2 + param_1->x_lo;
                if (x <= *param_2 + param_1->x_hi) {
                    xoff = x * 0x14;
                    do {
                        tile = (struct MapTile *)((char *)GameMap[y] + xoff);
                        tile->flags_c = tile->flags_c & 0xffe7;
                        tile->flags_10 = 0;
                        tile->tile = tile->base_id;
                        local_8.x = x;
                        local_8.y = y;
                        FUN_0045d260(&local_8);
                        RemovePathSquare((struct InstancePos *)&local_8);
                        x = x + 1;
                        xoff = xoff + 0x14;
                    } while (x <= *param_2 + param_1->x_hi);
                }
                y = y + 1;
            } while (y <= param_2[1] + param_1->y_hi);
        }
    }
}

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

struct PathItem {
    /* 0x00 */ int x0;
    /* 0x04 */ int y0;
    /* 0x08 */ int x1;
    /* 0x0c */ int y1;
    /* 0x10 */ struct PathItem *next;
};

// FUNCTION: LEGOLAND 0x0045d770
void FUN_0045d770(struct Cursor *param_1) {
    unsigned char *pb;
    struct Cursor *cur;
    struct Cursor *chain;
    struct PathItem *item;
    int i;
    int x;
    int y;
    struct MapRect rect;
    struct Point local_18;

    cur = param_1;
    if (param_1 != NULL) {
        while ((cur->field_1828 & 0x1000) == 0) {
            cur = (struct Cursor *)cur->field_1830;
            if (cur == NULL) {
                return;
            }
        }
        if (cur != NULL) {
            DAT_00667d3c = 0;
            rect.x0 = cur->field_1414[0] + cur->field_1404;
            rect.y0 = cur->field_1414[1] + cur->field_1408;
            rect.x1 = cur->field_1414[2] + cur->field_1404;
            rect.y1 = cur->field_1414[3] + cur->field_1408;
            FUN_0045d730(&rect);
            do {
                chain = param_1;
                if ((param_1->field_1828 & 0x1000) != 0) {
                    break;
                }
                rect.x0 = param_1->field_1404 + param_1->field_1414[0];
                rect.y0 = param_1->field_1414[1] + param_1->field_1408;
                rect.x1 = param_1->field_1414[2] + param_1->field_1404;
                rect.y1 = param_1->field_1414[3] + param_1->field_1408;
                FUN_0045d5d0(&rect);
                for (item = (struct PathItem *)&param_1->field_1414; item != NULL; item = item->next) {
                    rect.x0 = param_1->field_1404 + item->x0;
                    rect.y0 = item->y0 + param_1->field_1408;
                    rect.x1 = item->x1 + param_1->field_1404;
                    rect.y1 = item->y1 + param_1->field_1408;
                    FUN_0045d5d0(&rect);
                }
                param_1 = (struct Cursor *)param_1->field_1830;
            } while (param_1 != NULL);
            if (0 < DAT_00667d3c) {
                i = 0;
                do {
                    y = DAT_00801a80[i].y0;
                    if (y <= DAT_00801a80[i].y1) {
                        do {
                            x = DAT_00801a80[i].x0;
                            if (x <= DAT_00801a80[i].x1) {
                                do {
                                    local_18.x = x;
                                    local_18.y = y;
                                    FUN_004779d0(&local_18);
                                    pb = (unsigned char *)((char *)GameMap[y] + 0x10 + x * 0x14);
                                    *pb = *pb & 0xfc;
                                    AddPathTileGFX(&local_18, *(unsigned short *)PathSprite);
                                    DAT_00668610 = DAT_00668610 | 0x10;
                                    AddPathSquare((struct InstancePos *)&local_18);
                                    x = x + 1;
                                } while (x <= DAT_00801a80[i].x1);
                            }
                            y = y + 1;
                        } while (y <= DAT_00801a80[i].y1);
                    }
                    i = i + 1;
                } while (i < DAT_00667d3c);
            }
            DAT_0066b46c = 1;
            y = chain->field_1414[1] + 1 + chain->field_1408;
            if (y <= chain->field_1408 + -1 + chain->field_1414[3]) {
                do {
                    x = chain->field_1414[0] + 1 + chain->field_1404;
                    if (x <= chain->field_1414[2] + -1 + chain->field_1404) {
                        do {
                            local_18.x = x;
                            local_18.y = y;
                            AddPathTileGFX(&local_18, *(unsigned short *)PathSprite);
                            x = x + 1;
                        } while (x <= chain->field_1414[2] + -1 + chain->field_1404);
                    }
                    y = y + 1;
                } while (y <= chain->field_1408 + -1 + chain->field_1414[3]);
            }
            DAT_0066b46c = 1;
        }
    }
}

// FUNCTION: LEGOLAND 0x0045da60
LEGO_EXPORT unsigned short RestoreBaseMap(int tile_x, int row_y) {
    struct MapTile *tile = (struct MapTile *)((char *)GameMap[row_y] + tile_x * 0x14);
    unsigned short id = tile->base_id;

    tile->tile = id;
    return TileSpriteInfo[id].sprite & 0x20;
}

// FUNCTION: LEGOLAND 0x0045daa0
LEGO_EXPORT void RemovePathTile(int *param_1, unsigned short param_2) {
    unsigned short *flags;
    struct Point local_8;

    RestoreBaseMap(*param_1, param_1[1]);
    flags = (unsigned short *)((char *)GameMap[param_1[1]] + 0xc + *param_1 * 0x14);
    *flags = *flags & 0xffe4;
    *(unsigned char *)((char *)GameMap[param_1[1]] + 0x10 + *param_1 * 0x14) = 0;
    local_8.x = *param_1;
    local_8.y = param_1[1] + -1;
    AdjustPathTile(&local_8, param_2);
    local_8.y = param_1[1];
    local_8.x = *param_1 + 1;
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1;
    local_8.y = param_1[1] + 1;
    AdjustPathTile(&local_8, param_2);
    local_8.y = param_1[1];
    local_8.x = *param_1 + -1;
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1 + -1;
    local_8.y = param_1[1] + 1;
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1 + 1;
    local_8.y = param_1[1] + -1;
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1 + 1;
    local_8.y = param_1[1] + 1;
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1 + -1;
    local_8.y = param_1[1] + -1;
    AdjustPathTile(&local_8, param_2);
    RemovePathSquare((struct InstancePos *)param_1);
    if (DAT_00832984 != 0) {
        FUN_0045cd70(param_1);
    }
}
