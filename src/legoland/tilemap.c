#include "legoland.h"

#include "pathfind.h"
#include "tilemap.h"
#include "llidb.h"

struct MapTile {
    unsigned char pad_0[8];
    unsigned short tile;
    unsigned char pad_a[2];
    unsigned char flags_c;
    unsigned char pad_d[3];
    unsigned char flags_10;
};

struct LegoConfig {
    unsigned char pad_0[0x14];
    unsigned short field_14;
    unsigned short field_16;
};

struct Point {
    int x;
    int y;
};

extern void *TileSpriteArray[];
extern void *PathSprite;
extern struct MapTile **DAT_00801400;
extern struct LegoConfig *lpConfig;
extern unsigned char DAT_004b9550[8];
extern unsigned int DAT_00667c88;
extern unsigned int DAT_00667c8c;
extern unsigned int DAT_00667c90;
extern unsigned int DAT_00667c94;
extern void *DAT_00667c9c;

extern void FUN_0049e4d0(void *block);
extern void KillSprite(unsigned int sprite);

// FUNCTION: LEGOLAND 0x0045a9b0
void AllocTileSpace(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045aa50
void FUN_0045aa50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045aa90
void FreeTileSpace(unsigned short index, unsigned short count) {
    memset(&TileSpriteArray[index], 0xFF, count * 4);
}

// FUNCTION: LEGOLAND 0x0045aad0
void LoadMapTiles(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045ac20
unsigned int FUN_0045ac20(void) {
    unsigned int handle;

    if (DAT_00667c9c != 0) {
        FUN_0049e4d0(DAT_00667c9c);
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
void GetTileBounds(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045ad60
void GetTileCentre(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045ade0
void FUN_0045ade0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045b170
void FUN_0045b170(void) {
}

// FUNCTION: LEGOLAND 0x0045b180
void RenderView(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045bcd0
void PointToIsoPlane(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045be00
void ScreenToMapRef2(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045be90
void ScreenToMapRef(unsigned int x, void *out, unsigned int y) { STUB(); }

// FUNCTION: LEGOLAND 0x0045c010
unsigned char Dir_To_Bit(unsigned char param) {
    return DAT_004b9550[param & 7];
}

// FUNCTION: LEGOLAND 0x0045c020
unsigned char Bit_To_Dir(unsigned char bit) {
    unsigned char result;

    for (result = 0; result < 8; result++) {
        if (DAT_004b9550[result] & bit) {
            return result;
        }
    }
    return 8;
}

// FUNCTION: LEGOLAND 0x0045c050
void Get_Path_Directions(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045c440
void FUN_0045c440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045c830
unsigned char ExcludeIsolatedDiags(unsigned char param) {
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
void AdjustTileRFFlags(void) { STUB(); }

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

    tile = DAT_00801400[param->y];
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
void FUN_0045ceb0(void) { STUB(); }

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
            map_row = (unsigned int *)DAT_00801400[y];
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
            map_row = (unsigned int *)DAT_00801400[y];
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
void AdjustPathTile(struct Point *p, unsigned int a) { STUB(); }

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
void AddPathTileGFX(struct Point *p, unsigned short param1) { STUB(); }

// FUNCTION: LEGOLAND 0x0045d3b0
void AddPathTile(struct Point *p, unsigned short param1) {
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
void RestoreBaseMap(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045daa0
void RemovePathTile(unsigned int param_1, unsigned short param_2) { STUB(); }
