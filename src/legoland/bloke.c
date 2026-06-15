#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "bloke.h"
#include "debug_alloc.h"
#include "globals.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "objclass.h"
#include "pathfind.h"
#include "tilemap.h"
#include "timer.h"

struct BestNode {
    unsigned char pad_0[0x20];
    unsigned char flags_20;
};

struct PendingObject {
    unsigned char pad_0[0xe];
    unsigned short field_e;
    unsigned short field_10;
};

struct ActionState {
    unsigned char pad_0[0xe];
    unsigned short field_e;
    unsigned short field_10;
    unsigned char pad_12[0x72 - 0x12];
    unsigned char field_72;
    unsigned char field_73;
    unsigned char pad_74;
    unsigned char field_75;
};

struct OverTile {
    unsigned char pad_0[0x62];
    unsigned short field_62;
    unsigned char pad_64[0x68 - 0x64];
    unsigned int field_68;
    unsigned int field_6c;
};

struct Walker {
    unsigned char pad_0[0x62];
    unsigned short field_62;
    unsigned char pad_64[0x72 - 0x64];
    unsigned char field_72;
    unsigned char field_73;
    unsigned char field_74;
    unsigned char field_75;
};

struct TileWalker {
    unsigned char pad_0[0xe];
    unsigned short field_e;
    unsigned short field_10;
    unsigned char pad_12[0x20 - 0x12];
    unsigned int field_20;
    unsigned char pad_24[0x62 - 0x24];
    unsigned short field_62;
    unsigned char pad_64[0x68 - 0x64];
    int field_68;
    int field_6c;
    unsigned char pad_70[0x72 - 0x70];
    unsigned char field_72;
};

struct TileCallback {
    unsigned char pad_0[0x18];
    unsigned char (*cb_18)(int x, int y);
    void (*cb_1c)(struct Point p);
    void (*cb_20)(int x, int y);
};

struct BlokeDist {
    unsigned char pad_0[0x24];
    int field_24;
    int field_28;
    unsigned char pad_2c[0x98 - 0x2c];
    int field_98;
    int field_9c;
};

struct BNVPath {
    unsigned char pad_0[0x40];
    unsigned int field_40;
};

struct Person {
    unsigned int field_0;
    struct PersonAnim *field_4;
    unsigned char pad_8[0x83 - 0x8];
    unsigned char field_83;
    unsigned char field_84;
};

struct PersonAnim {
    unsigned char pad_0[0x84];
    unsigned int field_84;
};

// FUNCTION: LEGOLAND 0x00482b10
void FUN_00482b10(void) {
    DAT_0066b468 = GetGameTimer();
}

// FUNCTION: LEGOLAND 0x00482b20
void FUN_00482b20(int force) {
    unsigned int now = GetGameTimer();
    if ((int)(now - DAT_0066b468) <= 0xfa0 && force == 0) {
        return;
    }
    DAT_0066b468 = now;
    FUN_00482a90();
    /* DAT_0066b460 is the first uint of an InstancePos-shaped pair (x,y);
       its address is reinterpreted here as the position passed to FUN_00482a40. */
    FUN_00482a40((struct InstancePos *)&DAT_0066b460);
}

// FUNCTION: LEGOLAND 0x00482b60
int FUN_00482b60(struct InstancePos *pos) {
    struct BestNode *node = FUN_00481790(pos);
    if (node != NULL) {
        FUN_00482b20(DAT_0066b46c);
        DAT_0066b46c = 0;
        if (node->flags_20 & 0x2) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00482ba0
LEGO_EXPORT char *GetVisitorName(struct Person *person) {
    char *name;
    if (person->field_4->field_84 == 0) {
        name = PTR_s_Aaron_004bcecc[person->field_83];
    } else {
        name = PTR_s_Abbie_004bd018[person->field_83];
    }
    strcpy(DAT_0066b470, name);
    strcat(DAT_0066b470, " ");
    strcat(DAT_0066b470, PTR_s_Adams_004bd180[person->field_84]);
    return DAT_0066b470;
}

// FUNCTION: LEGOLAND 0x00482c60
void FUN_00482c60(struct Person *person) {
    if (person->field_4->field_84 != 0) {
        person->field_83 = (unsigned char)((unsigned int)rand() % 0x5a);
    } else {
        person->field_83 = (unsigned char)((unsigned int)rand() % 0x53);
    }
    person->field_84 = (unsigned char)((unsigned int)rand() % 0x6b);
}

// FUNCTION: LEGOLAND 0x00482cb0
int FUN_00482cb0(struct Bloke *bloke) {
    switch (bloke->action) {
    case 3:
        return 4;
    case 0xb:
    case 0xc:
        return 1;
    case 0xd:
        return 5;
    }
    if (bloke->field_7a < DAT_0083292c) {
        return 3;
    }
    return ((unsigned int)(bloke->field_7a >= DAT_00832934) - 1 & 8) + 2;
}

// FUNCTION: LEGOLAND 0x00482d30
int FUN_00482d30(struct Bloke *bloke) {
    if (bloke->field_7a < DAT_0083292c) {
        return 3;
    }
    return ((unsigned int)(bloke->field_7a >= DAT_00832934) - 1 & 8) + 2;
}

// FUNCTION: LEGOLAND 0x00482d60
void FUN_00482d60(unsigned int index, int value) {
    DAT_0083293c[index] = value;
}

// FUNCTION: LEGOLAND 0x00482d70
void FUN_00482d70(void) {
    DAT_0083293c[4] = 5;
    DAT_0083293c[5] = 5;
    DAT_0083293c[6] = 5;
    DAT_0083293c[0] = -100;
    DAT_0083293c[1] = -400;
    DAT_0083293c[2] = 33;
    DAT_0083293c[3] = 20;
    DAT_0083293c[7] = -200;
    DAT_0083293c[8] = -1600;
    DAT_0083293c[9] = 400;
    DAT_0083293c[10] = 100;
    DAT_0083293c[11] = 400;
    DAT_0083293c[12] = 100;
}

// FUNCTION: LEGOLAND 0x00482df0
short FUN_00482df0(struct Bloke *bloke, int index, int mul) {
    int value = (DAT_0083293c[index] * mul) / 100 + bloke->field_7a;
    if (value < -30000) {
        value = -30000;
    } else if (value > 30000) {
        value = 30000;
    }
    bloke->field_7a = (short)value;
    return (short)value;
}

// FUNCTION: LEGOLAND 0x00482e50
void FUN_00482e50(void) {
    int i;
    DAT_0066b57c = malloc(lpConfig->field_1a * 0xac);
    if (lpConfig->field_1a != 0) {
        for (i = 0; i < (int)lpConfig->field_1a; i++) {
            memset((char *)DAT_0066b57c + i * 0xac, 0, 0xac);
        }
    }
}

// FUNCTION: LEGOLAND 0x00482ec0
void FUN_00482ec0(void) {
    if (DAT_0066b57c != NULL) {
        free(DAT_0066b57c);
    }
    FirstBloke = NULL;
    DAT_0066b57c = NULL;
}

// FUNCTION: LEGOLAND 0x00482ef0
LEGO_EXPORT struct Bloke *NewBloke(void) {
    struct Bloke *bloke = NULL;
    int i = 0;
    if (lpConfig->field_1a != 0) {
        char *base = (char *)DAT_0066b57c;
        unsigned char *slot = (unsigned char *)base + 0x62;
        while ((*slot & 1) != 0) {
            i++;
            slot += 0xac;
            if ((int)(unsigned int)lpConfig->field_1a <= i) {
                return bloke;
            }
        }
        bloke = (struct Bloke *)(base + i * 0xac);
        if (bloke != NULL) {
            memset(bloke, 0, 0xac);
            bloke->flags = 1;
            bloke->field_64 = 0;
            bloke->next = FirstBloke;
            FirstBloke = bloke;
            Add3DBlokeToList(bloke, 1);
        }
    }
    return bloke;
}

// FUNCTION: LEGOLAND 0x00482f70
LEGO_EXPORT struct Bloke *NewBlokeWOList(void *param_2) {
    struct Bloke *bloke = (struct Bloke *)malloc(0xac);
    if (bloke != NULL) {
        memset(bloke, 0, 0xac);
        bloke->flags = 1;
        bloke->field_64 = 0;
        Add3DBlokeToList(bloke, (unsigned int)param_2);
    }
    return bloke;
}

// FUNCTION: LEGOLAND 0x00482fb0
LEGO_EXPORT int GetBlokeNum(struct Bloke *bloke) {
    if (bloke == NULL) {
        return -1;
    }
    return (int)((char *)bloke - (char *)DAT_0066b57c) / 0xac;
}

// FUNCTION: LEGOLAND 0x00482fe0
LEGO_EXPORT struct Bloke *GetBlokePtr(int index) {
    if (index == -1) {
        return NULL;
    }
    return &((struct Bloke *)DAT_0066b57c)[index];
}

// FUNCTION: LEGOLAND 0x00483010
LEGO_EXPORT void DestroyBloke(struct Bloke *bloke) {
    struct BlokeSampleSource source;
    struct Bloke **prev;
    struct Bloke *current;

    prev = (struct Bloke **)&FirstBloke;
    for (current = FirstBloke; current != NULL && current != bloke; current = current->next) {
        prev = &current->next;
    }
    if (*prev != NULL) {
        *prev = bloke->next;
    } else {
        // STRING: LEGOLAND 0x004bdcb4
        OutputDebugStringA("DestroyBloke: Badly linked list\n");
    }
    source.field_0 = 1;
    source.field_4 = bloke;
    KillAllSamplesFromSource(&source);
    FUN_0043f840((struct Person *)bloke->prev);
    FUN_0043f870((struct PosHeader *)bloke->prev);
    bloke->flags &= 0xfffe;
    bloke->next = NULL;
}

// FUNCTION: LEGOLAND 0x00483090
void FUN_00483090(void) {
    while (FirstBloke != NULL) {
        DestroyBloke(FirstBloke);
    }
    DAT_006661bc = 0;
}

// FUNCTION: LEGOLAND 0x004830c0
LEGO_EXPORT struct Bloke *MakeBloke(void) {
    struct Bloke *bloke = NewBloke();
    if (bloke != NULL) {
        ClearBlokeCounters(GetBlokeNum(bloke));
    }
    return bloke;
}

// FUNCTION: LEGOLAND 0x004830f0
LEGO_EXPORT void InitialiseBlokes(void) { FUN_00482e50(); }

// FUNCTION: LEGOLAND 0x00483130
LEGO_EXPORT void RenderPeople(void) {
    struct Bloke *current;
    Control3DPeople();
    for (current = FirstBloke; current != NULL; current = current->next) {
        if ((current->flags & 0xa0) == 0) {
            SortBlokeIn3D(current);
        }
    }
}

// FUNCTION: LEGOLAND 0x00483160
int FUN_00483160(int x, int y) {
    if (x <= 0) {
        return 0;
    }
    if (x < (lpConfig->width << 8)) {
        if (y <= 0) {
            return 0;
        }
        if (y < (lpConfig->height << 8)) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004831a0
struct Point FUN_004831a0(unsigned int dir, short dist) {
    struct Point result;
    int index = (dir & 0xff) * 2;
    result.x = (int)DAT_004bd32c[index] * (int)dist >> 8;
    result.y = (int)DAT_004bd32c[index + 1] * (int)dist >> 8;
    return result;
}

// FUNCTION: LEGOLAND 0x004831d0
LEGO_EXPORT void SetPathFlag(struct OverTile *obj) {
    int x = obj->field_68;
    if (x >= 0 && x < (int)(lpConfig->width * 0x100)) {
        int y = obj->field_6c;
        if (y >= 0 && y < (int)(lpConfig->height * 0x100)) {
            short mapFlags = Get_MapFlags(x, y);
            unsigned char rf = GetCurrentRFFlags(obj->field_68, obj->field_6c);
            if ((rf & 1) != 0 || ((mapFlags & 0x10) != 0 && (rf & 2) == 0)) {
                *(unsigned char *)&obj->field_62 |= 2;
                return;
            }
        }
    }
    obj->field_62 &= 0xfffd;
}

// FUNCTION: LEGOLAND 0x00483240
LEGO_EXPORT unsigned short DoPendingAction(struct PendingObject *obj) {
    obj->field_e = obj->field_10;
    obj->field_10 = 0;
    return obj->field_e;
}

// FUNCTION: LEGOLAND 0x00483260
void FUN_00483260(struct TileWalker *walker) {
    struct Point tile;
    int x;
    int y;
    struct MapElement *elem;
    struct TileCallback *cb;

    *(unsigned char *)&walker->field_62 |= 8;
    walker->field_10 = walker->field_e;
    walker->field_e = 9;
    walker->field_20 = 0;
    tile = GetTileInDir(walker->field_68, walker->field_6c, walker->field_72);
    x = tile.x >> 8;
    y = tile.y >> 8;
    if (x >= 0 && x < (int)lpConfig->width && y >= 0 && y < (int)lpConfig->height) {
        elem = (struct MapElement *)((char *)GameMap[y] + x * 0x14);
    } else {
        elem = NULL;
    }
    cb = (struct TileCallback *)TileSpriteInfo[elem->field_8].src;
    if (cb->cb_1c != NULL) {
        cb->cb_1c(tile);
    }
}

// FUNCTION: LEGOLAND 0x00483300
int FUN_00483300(struct TileWalker *walker, int x, int y) {
    unsigned char result;
    int tx;
    int ty;
    struct MapElement *elem;
    struct TileCallback *cb;

    if (OverNewTile((struct OverTile *)walker, x, y) == 0) {
        return 0;
    }
    if (FUN_00483160(x, y) == 0) {
        return 0;
    }
    if ((Get_RFFlags(x, y) & 3) != 3) {
        return 0;
    }
    tx = x >> 8;
    ty = y >> 8;
    if (tx < 0 || tx >= (int)lpConfig->width || ty < 0 || ty >= (int)lpConfig->height) {
        elem = NULL;
    } else {
        elem = (struct MapElement *)((char *)GameMap[ty] + tx * 0x14);
    }
    cb = (struct TileCallback *)TileSpriteInfo[elem->field_8].src;
    if (cb->cb_18 != NULL) {
        result = cb->cb_18(x, y);
    } else {
        result = 2;
    }
    if ((result & 3) == 3) {
        FUN_00483260(walker);
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004833d0
LEGO_EXPORT int NewDirForAction(struct ActionState *state, unsigned char dir) {
    unsigned char masked = dir & 0x7;
    if (state->field_72 != masked) {
        state->field_10 = state->field_e;
        state->field_73 = masked;
        state->field_e = 5;
        state->field_75 = 1;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483400
LEGO_EXPORT unsigned int Random_Dir_From_Bits(unsigned int bits) {
    unsigned char mask = (unsigned char)bits;
    unsigned char count;
    unsigned char remaining;
    unsigned char bit;
    int n;
    unsigned int dir;

    if (mask == 0) {
        return 8;
    }
    count = 1;
    remaining = (mask - 1) & mask;
    if (remaining != 0) {
        do {
            count++;
            remaining = remaining & (remaining - 1);
        } while (remaining != 0);
    }
    n = Rand_Max((count & 0xff) - 1);
    bit = 1;
    if ((bits & 1) == 0) {
        do {
            bit = bit << 1;
        } while ((mask & bit) == 0);
    }
    if (n != 0) {
        do {
            mask = mask & ~bit;
            for (; (mask & bit) == 0; bit = bit << 1) {
            }
            n--;
        } while (n != 0);
    }
    dir = Bit_To_Dir(bit);
    DAT_0066b580[dir & 0xff]++;
    return dir;
}

// FUNCTION: LEGOLAND 0x004834a0
LEGO_EXPORT int HitPathEdge(struct OverTile *obj, int x, int y) {
    if ((*(unsigned char *)&obj->field_62 & 2) != 0) {
        if (x < 0 || x >= (int)(lpConfig->width * 0x100) || y < 0 ||
            y >= (int)(lpConfig->height * 0x100)) {
            return 1;
        }
        {
            short mapFlags = Get_MapFlags(x, y);
            unsigned char rf = GetCurrentRFFlags(x, y);
            if ((rf & 1) != 0) {
                return 0;
            }
            if ((mapFlags & 0x10) != 0 && (rf & 2) == 0) {
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483510
LEGO_EXPORT int HitObstacle(struct OverTile *obj, int x, int y) {
    if (x >= 0 && x < (int)(lpConfig->width * 0x100) && y >= 0 && y < (int)(lpConfig->height * 0x100)) {
        if ((GetCurrentRFFlags(obj->field_68, obj->field_6c) & 2) == 0 &&
            (GetCurrentRFFlags(x, y) & 2) != 0) {
            return 1;
        }
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00483580
int FUN_00483580(struct OverTile *obj, int x, int y) {
    if (x >= 0 && x < (int)(lpConfig->width * 0x100) && y >= 0 && y < (int)(lpConfig->height * 0x100)) {
        unsigned char rf1 = GetCurrentRFFlags(obj->field_68, obj->field_6c);
        unsigned short mf1 = Get_MapFlags(obj->field_68, obj->field_6c);
        int blocked1 = ((rf1 & 2) == 0 || (mf1 & 0x8800) != 0) ? 0 : 1;
        unsigned char rf2 = GetCurrentRFFlags(x, y);
        unsigned short mf2 = Get_MapFlags(x, y);
        int blocked2 = ((rf2 & 2) == 0 || (mf2 & 0x8800) != 0) ? 0 : 1;
        if (blocked1 || !blocked2) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00483650
LEGO_EXPORT int OverNewTile(struct OverTile *tile, unsigned int x, unsigned int y) {
    if (((tile->field_68 ^ x) & 0xffffff00) || ((tile->field_6c ^ y) & 0xffffff00)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483680
void FUN_00483680(struct TileWalker *walker, unsigned int x, unsigned int y) {
    struct MapElement *elem;
    struct TileCallback *cb;
    struct Point p;

    if (OverNewTile((struct OverTile *)walker, x, y) == 0) {
        return;
    }
    if ((Get_RFFlags(walker->field_68, walker->field_6c) & 3) == 3) {
        int tx = walker->field_68 >> 8;
        int ty = walker->field_6c >> 8;
        if (tx >= 0 && tx < (int)lpConfig->width && ty >= 0 && ty < (int)lpConfig->height) {
            elem = (struct MapElement *)((char *)GameMap[ty] + tx * 0x14);
        } else {
            elem = NULL;
        }
        cb = (struct TileCallback *)TileSpriteInfo[elem->field_8].src;
        if (cb->cb_1c != NULL) {
            cb->cb_20(walker->field_68, walker->field_6c);
        }
        walker->field_62 &= 0xfff7;
    }
    if ((Get_RFFlags(x, y) & 3) == 3) {
        if ((x >> 8) < lpConfig->width && (y >> 8) < lpConfig->height) {
            elem = (struct MapElement *)((char *)GameMap[y >> 8] + (x >> 8) * 0x14);
        } else {
            elem = NULL;
        }
        cb = (struct TileCallback *)TileSpriteInfo[elem->field_8].src;
        if (cb->cb_1c != NULL) {
            p.x = x;
            p.y = y;
            cb->cb_1c(p);
        }
        *(unsigned char *)&walker->field_62 |= 8;
    }
}

// FUNCTION: LEGOLAND 0x004837a0
int FUN_004837a0(struct Walker *walker, unsigned int x, unsigned int y) {
    if (OverNewTile((struct OverTile *)walker, x, y) != 0) {
        walker->field_62 &= 0xfffb;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004837d0
LEGO_EXPORT int CrossTileCentre(struct TileWalker *walker, unsigned int x, unsigned int y) {
    unsigned int delta;
    switch (walker->field_72) {
    case 0:
    case 1:
    case 4:
    case 5:
        delta = walker->field_6c ^ y;
        break;
    default:
        delta = walker->field_68 ^ x;
    }
    if ((delta & 0x80) != 0 && OverNewTile((struct OverTile *)walker, x, y) == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483830
void FUN_00483830(struct Walker *walker) {
    walker->field_75 = walker->field_75 - 1;
    if (walker->field_75 == 0) {
        walker->field_75 = 2;
        walker->field_74 = (walker->field_74 + 1) & 7;
    }
}

// FUNCTION: LEGOLAND 0x00483850
void FUN_00483850(struct Walker *walker) {
    walker->field_75 = walker->field_75 - 1;
    if (walker->field_75 == 0) {
        walker->field_75 = 3;
        walker->field_72 =
            (((-(((walker->field_72 - walker->field_73) & 4) != 0) & 2) - 1) + walker->field_72) & 7;
    }
}

// FUNCTION: LEGOLAND 0x00483890
void FUN_00483890(struct Walker *walker) {
    walker->field_74 = 2;
}

// FUNCTION: LEGOLAND 0x004838a0
void FUN_004838a0(unsigned int frame) {
    // STRING: LEGOLAND 0x004bdcd8
    DBPrintf("Frame %d.. Bloke %d rethinking\n", DAT_008119a4, frame);
}

// FUNCTION: LEGOLAND 0x004838c0
void FUN_004838c0(struct Walker *walker) {
    walker->field_75--;
    if (walker->field_75 == 0) {
        walker->field_75 = 1;
        DoPendingAction((struct PendingObject *)walker);
    }
}

// FUNCTION: LEGOLAND 0x004838e0
void FUN_004838e0(struct Walker *walker) {
    if (walker->field_72 != walker->field_73) {
        FUN_00483850(walker);
    }
    if (walker->field_72 == walker->field_73) {
        walker->field_75 = 1;
        DoPendingAction((struct PendingObject *)walker);
    }
}

// FUNCTION: LEGOLAND 0x00483920
LEGO_EXPORT int DoRndWalkPathTileAction(struct TileWalker *walker) {
    int coords[2];
    unsigned char rf;
    unsigned char dirs;

    coords[0] = walker->field_68 >> 8;
    coords[1] = walker->field_6c >> 8;
    rf = GetCurrentRFFlags(walker->field_68, walker->field_6c);
    if (walker->field_68 >= 0 && walker->field_68 < (int)(lpConfig->width * 0x100) &&
        walker->field_6c >= 0 && walker->field_6c < (int)(lpConfig->height * 0x100)) {
        short mapFlags = Get_MapFlags(walker->field_68, walker->field_6c);
        unsigned char rf2 = GetCurrentRFFlags(walker->field_68, walker->field_6c);
        if ((rf2 & 1) != 0 || ((mapFlags & 0x10) != 0 && (rf2 & 2) == 0)) {
            if ((rf & 8) != 0) {
                dirs = Get_Path_Directions(coords, 0, 0);
                dirs = ExcludeIsolatedDiags(dirs);
                dirs = dirs & ~Dir_To_Bit(walker->field_72 + 4);
                *(unsigned char *)&walker->field_62 |= 4;
                return NewDirForAction((struct ActionState *)walker, Bit_To_Dir(dirs));
            }
            if ((rf & 0x24) != 0) {
                unsigned char b5;
                unsigned char b4;
                unsigned char b3;
                dirs = Get_Path_Directions(coords, 0, 0);
                dirs = ExcludeIsolatedDiags(dirs);
                b5 = Dir_To_Bit(walker->field_72 + 5);
                b4 = Dir_To_Bit(walker->field_72 + 4);
                b3 = Dir_To_Bit(walker->field_72 + 3);
                dirs = dirs & ~(b5 | b4 | b3);
                *(unsigned char *)&walker->field_62 |= 4;
                return NewDirForAction((struct ActionState *)walker, Random_Dir_From_Bits(dirs));
            }
            if ((rf & 0x10) != 0) {
                unsigned char dir;
                dirs = Get_Path_Directions(coords, 0, 0);
                dirs = ExcludeIsolatedDiags(dirs);
                *(unsigned char *)&walker->field_62 |= 4;
                if (dirs == 0) {
                    dir = rand() & 7;
                } else {
                    dir = Bit_To_Dir(dirs);
                }
                return NewDirForAction((struct ActionState *)walker, dir);
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483b10
LEGO_EXPORT int Handle_RndWalk_TileSpecifics(struct TileWalker *walker, unsigned int x, unsigned int y) {
    if (FUN_004837a0((struct Walker *)walker, x, y) == 0) {
        if (CrossTileCentre(walker, x, y) != 0 && (*(unsigned char *)&walker->field_62 & 4) == 0) {
            return DoRndWalkPathTileAction(walker);
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483b60
void FUN_00483b60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483c20
void FUN_00483c20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483d10
void FUN_00483d10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483d90
void FUN_00483d90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483e20
void FUN_00483e20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483ef0
void FUN_00483ef0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00484090
void FUN_00484090(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004841a0
void FUN_004841a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004841e0
unsigned char FUN_004841e0(struct BlokeDist *bloke) {
    int dx = bloke->field_24 - bloke->field_98;
    int dy = bloke->field_28 - bloke->field_9c;
    return (dx * dx + dy * dy) <= 0x400;
}

// FUNCTION: LEGOLAND 0x00484220
void FUN_00484220(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00484350
void FUN_00484350(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00484470
void FUN_00484470(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00484520
void FUN_00484520(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004845d0
void FUN_004845d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00484630
void FUN_00484630(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004846a0
LEGO_EXPORT struct Point GetTileInDir(int x, int y, unsigned int dir) {
    struct Point result;
    switch (dir & 7) {
    case 0:
        y = y - 0x100;
        break;
    case 1:
        result.x = x;
        result.y = y - 0x100;
        return result;
    case 2:
        result.x = x + 0x100;
        result.y = y - 0x100;
        return result;
    case 3:
        result.x = x + 0x100;
        result.y = y;
        return result;
    case 4:
        result.x = x + 0x100;
        result.y = y + 0x100;
        return result;
    case 5:
        result.x = x;
        result.y = y + 0x100;
        return result;
    case 6:
        y = y + 0x100;
        break;
    case 7:
        result.x = x - 0x100;
        result.y = y;
        return result;
    }
    result.x = x - 0x100;
    result.y = y;
    return result;
}

// FUNCTION: LEGOLAND 0x00484790
void FUN_00484790(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004848e0
void FUN_004848e0(struct Walker *walker) {
    if (!((DAT_004bdd00 >> 8) & 0x2) || DAT_004bdd04 != (unsigned int)walker) {
        walker->field_62 &= 0xfff7;
        DoPendingAction((struct PendingObject *)walker);
    }
}

// FUNCTION: LEGOLAND 0x00484910
LEGO_EXPORT void Bloke_DoNothing(void) {
}

// FUNCTION: LEGOLAND 0x00484920
LEGO_EXPORT void DoLowLevelAI(struct Worker *worker) { STUB(); }

// FUNCTION: LEGOLAND 0x00484950
LEGO_EXPORT void ApplyObjectOrientationToPerson(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00484a70
LEGO_EXPORT void SetBlokePositionFromBNV(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00484c20
LEGO_EXPORT void NewBNVPath(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00484cd0
LEGO_EXPORT void UpdateBlokeFromBNVPath(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00484ff0
LEGO_EXPORT unsigned int BNVPath_GetDFrame(struct BNVPath *path) {
    return path->field_40;
}

// FUNCTION: LEGOLAND 0x00485000
LEGO_EXPORT void BNVPath_GetBINVScreenCoords(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004850b0
LEGO_EXPORT void BNVPath_SetDFrame(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00485260
int CheckForPeople() { STUB(); }
