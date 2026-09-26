#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "binv.h"
#include "bloke.h"
#include "debug_alloc.h"
#include "globals.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "objclass.h"
#include "pathfind.h"
#include "screens.h"
#include "sound_music.h"
#include "tilemap.h"
#include "timer.h"
#include "worker.h"

// FUNCTION: LEGOLAND 0x00482b10
void FUN_00482b10(void) {
    DAT_0066b468 = GetGameTimer();
}

// FUNCTION: LEGOLAND 0x00482b20
void FUN_00482b20(int force) {
    int now = GetGameTimer();
    if (now - DAT_0066b468 <= 0xfa0 && force == 0) {
        return;
    }
    DAT_0066b468 = now;
    FUN_00482a90();
    FUN_00482a40(&DAT_0066b460);
}

// FUNCTION: LEGOLAND 0x00482b60
int FUN_00482b60(Point *pos) {
    BestNode *node = FUN_00481790(pos);
    if (node != NULL) {
        FUN_00482b20(DAT_0066b46c);
        DAT_0066b46c = 0;
        if (node->field_20 & 0x2) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00482ba0
LEGO_EXPORT char *GetVisitorName(Bloke *bloke) {
    char *name;
    if (bloke->person->random == 0) {
        name = PTR_s_Aaron_004bcecc[bloke->field_83];
    } else {
        name = PTR_s_Abbie_004bd018[bloke->field_83];
    }
    strcpy(DAT_0066b470, name);
    strcat(DAT_0066b470, " ");
    strcat(DAT_0066b470, PTR_s_Adams_004bd180[bloke->field_84]);
    return DAT_0066b470;
}

// FUNCTION: LEGOLAND 0x00482c60
void FUN_00482c60(Bloke *bloke) {
    unsigned int roll;

    if (bloke->person->random != 0) {
        roll = rand();
        bloke->field_83 = roll % 0x5a;
    } else {
        roll = rand();
        bloke->field_83 = roll % 0x53;
    }
    roll = rand();
    bloke->field_84 = roll % 0x6b;
}

// FUNCTION: LEGOLAND 0x00482cb0
int FUN_00482cb0(Bloke *bloke) {
    switch (bloke->action) {
    case 3:
        return 4;
    case 0xb:
    case 0xc:
        return 1;
    case 0xd:
        return 5;
    }
    if (bloke->field_7a < MapStats.field_12c) {
        return 3;
    }
    return bloke->field_7a < MapStats.field_134 ? 10 : 2;
}

// FUNCTION: LEGOLAND 0x00482d30
int FUN_00482d30(Bloke *bloke) {
    if (bloke->field_7a < MapStats.field_12c) {
        return 3;
    }
    return bloke->field_7a < MapStats.field_134 ? 10 : 2;
}

// FUNCTION: LEGOLAND 0x00482d60
void FUN_00482d60(unsigned int index, int value) {
    MapStats.field_13c[index] = value;
}

// FUNCTION: LEGOLAND 0x00482d70
void FUN_00482d70(void) {
    MapStats.field_13c[4] = 5;
    MapStats.field_13c[5] = 5;
    MapStats.field_13c[6] = 5;
    MapStats.field_13c[0] = -100;
    MapStats.field_13c[1] = -400;
    MapStats.field_13c[2] = 33;
    MapStats.field_13c[3] = 20;
    MapStats.field_13c[7] = -200;
    MapStats.field_13c[8] = -1600;
    MapStats.field_13c[9] = 400;
    MapStats.field_13c[10] = 100;
    MapStats.field_13c[11] = 400;
    MapStats.field_13c[12] = 100;
}

// FUNCTION: LEGOLAND 0x00482df0
int FUN_00482df0(Bloke *bloke, int index, int mul) {
    int value = MapStats.field_13c[index] * mul / 100 + bloke->field_7a;
    if (value < -30000) {
        value = -30000;
    } else if (value > 30000) {
        value = 30000;
    }
    bloke->field_7a = value;
    return bloke->field_7a;
}

// FUNCTION: LEGOLAND 0x00482e50
void FUN_00482e50(void) {
    int i;
    DAT_0066b57c = malloc(lpConfig->field_1a * sizeof(Bloke));
    for (i = 0; i < lpConfig->field_1a; i++) {
        memset(&DAT_0066b57c[i], 0, sizeof(Bloke));
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
LEGO_EXPORT Bloke *NewBloke(void) {
    Bloke *bloke = NULL;
    int i;

    for (i = 0; i < lpConfig->field_1a; i++) {
        if ((DAT_0066b57c[i].flags & 1) == 0) {
            bloke = &DAT_0066b57c[i];
            break;
        }
    }
    if (bloke != NULL) {
        memset(bloke, 0, sizeof(Bloke));
        bloke->flags = 1;
        bloke->field_64 = 0;
        bloke->next = FirstBloke;
        FirstBloke = bloke;
        Add3DBlokeToList(bloke, 1);
    }
    return bloke;
}

// FUNCTION: LEGOLAND 0x00482f70
LEGO_EXPORT Bloke *NewBlokeWOList(int type) {
    Bloke *bloke = malloc(sizeof(Bloke));
    if (bloke != NULL) {
        memset(bloke, 0, sizeof(Bloke));
        bloke->flags = 1;
        bloke->field_64 = 0;
        Add3DBlokeToList(bloke, type);
    }
    return bloke;
}

// FUNCTION: LEGOLAND 0x00482fb0
LEGO_EXPORT int GetBlokeNum(Bloke *bloke) {
    if (bloke == NULL) {
        return -1;
    }
    return bloke - DAT_0066b57c;
}

// FUNCTION: LEGOLAND 0x00482fe0
LEGO_EXPORT Bloke *GetBlokePtr(int index) {
    if (index == -1) {
        return NULL;
    }
    return &DAT_0066b57c[index];
}

// FUNCTION: LEGOLAND 0x00483010
LEGO_EXPORT void DestroyBloke(Bloke *bloke) {
    SampleSource source;
    Bloke **prev;
    Bloke *current;

    prev = &FirstBloke;
    for (current = FirstBloke; current != NULL && current != bloke; current = current->next) {
        prev = &current->next;
    }
    if (*prev != NULL) {
        *prev = bloke->next;
    } else {
        // STRING: LEGOLAND 0x004bdcb4
        OutputDebugStringA("DestroyBloke: Badly linked list\n");
    }
    source.type = 1;
    source.field_4 = bloke;
    KillAllSamplesFromSource(&source);
    FUN_0043f840(bloke->person);
    FUN_0043f870(bloke->person);
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
LEGO_EXPORT Bloke *MakeBloke(int param_1) {
    Bloke *bloke = NewBloke();
    if (bloke != NULL) {
        ClearBlokeCounters(GetBlokeNum(bloke));
    }
    return bloke;
}

// FUNCTION: LEGOLAND 0x004830f0
LEGO_EXPORT void InitialiseBlokes(void) { FUN_00482e50(); }

// FUNCTION: LEGOLAND 0x00483130
LEGO_EXPORT void RenderPeople(void) {
    Bloke *current;
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
Point FUN_004831a0(unsigned char dir, short dist) {
    Point result;
    result.x = DAT_004bd32c[dir][0] * dist >> 8;
    result.y = DAT_004bd32c[dir][1] * dist >> 8;
    return result;
}

// FUNCTION: LEGOLAND 0x004831d0
LEGO_EXPORT void SetPathFlag(Bloke *bloke) {
    int x = bloke->pos.x;
    if (x >= 0 && x < lpConfig->width * 0x100) {
        int y = bloke->pos.y;
        if (y >= 0 && y < lpConfig->height * 0x100) {
            short mapFlags = Get_MapFlags(x, y);
            short rf = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
            if ((rf & 1) != 0 || ((mapFlags & 0x10) != 0 && (rf & 2) == 0)) {
                bloke->flags |= 2;
                return;
            }
        }
    }
    bloke->flags &= 0xfffd;
}

// FUNCTION: LEGOLAND 0x00483240
LEGO_EXPORT unsigned short DoPendingAction(Bloke *bloke) {
    bloke->field_e = bloke->field_10;
    bloke->field_10 = 0;
    return bloke->field_e;
}

// FUNCTION: LEGOLAND 0x00483260
void FUN_00483260(Bloke *bloke) {
    Point tile;
    unsigned int ux;
    unsigned int uy;
    int x;
    int y;
    MapElement *elem;
    FXSpriteList *set;

    bloke->flags |= 8;
    bloke->field_10 = bloke->field_e;
    bloke->field_e = 9;
    bloke->field_20 = 0;
    tile = GetTileInDir(bloke->pos, bloke->field_72);
    ux = tile.x;
    uy = tile.y;
    x = ux >> 8;
    y = uy >> 8;
    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
        elem = &GameMap[y][x];
    } else {
        elem = NULL;
    }
    set = TileSpriteInfo[elem->field_8].src;
    if (set->on_enter != NULL) {
        set->on_enter(tile);
    }
}

// FUNCTION: LEGOLAND 0x00483300
int FUN_00483300(Bloke *bloke, int x, int y) {
    unsigned char result;
    int tx;
    int ty;
    MapElement *elem;
    FXSpriteList *set;

    if (OverNewTile(bloke, x, y) == 0) {
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
    if (tx < 0 || tx >= lpConfig->width || ty < 0 || ty >= lpConfig->height) {
        elem = NULL;
    } else {
        elem = &GameMap[ty][tx];
    }
    set = TileSpriteInfo[elem->field_8].src;
    if (set->get_rf_flags != NULL) {
        result = set->get_rf_flags(x, y);
    } else {
        result = 2;
    }
    if ((result & 3) == 3) {
        FUN_00483260(bloke);
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004833d0
LEGO_EXPORT int NewDirForAction(Bloke *bloke, unsigned char dir) {
    unsigned char masked = dir & 0x7;
    if (bloke->field_72 != masked) {
        bloke->field_10 = bloke->field_e;
        bloke->field_73 = masked;
        bloke->field_e = 5;
        bloke->field_75 = 1;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483400
LEGO_EXPORT unsigned char Random_Dir_From_Bits(unsigned char bits) {
    unsigned char count;
    unsigned char rest;
    unsigned char bit;
    unsigned char dir;
    int n;

    if (bits == 0) {
        return 8;
    }
    count = 1;
    for (rest = (bits - 1) & bits; rest != 0; rest &= rest - 1) {
        count++;
    }
    n = Rand_Max(count - 1);
    for (bit = 1; (bits & bit) == 0; bit <<= 1) {
    }
    for (; n != 0; n--) {
        bits &= ~bit;
        while ((bits & bit) == 0) {
            bit <<= 1;
        }
    }
    dir = Bit_To_Dir(bit);
    DAT_0066b580[dir]++;
    return dir;
}

// FUNCTION: LEGOLAND 0x004834a0
LEGO_EXPORT int HitPathEdge(Bloke *bloke, int x, int y) {
    if ((bloke->flags & 2) != 0) {
        if (x < 0 || x >= lpConfig->width * 0x100 || y < 0 || y >= lpConfig->height * 0x100) {
            return 1;
        }
        {
            short mapFlags = Get_MapFlags(x, y);
            short rf = GetCurrentRFFlags(x, y);
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
LEGO_EXPORT int HitObstacle(Bloke *bloke, int x, int y) {
    if (x >= 0 && x < lpConfig->width * 0x100 && y >= 0 && y < lpConfig->height * 0x100) {
        if ((GetCurrentRFFlags(bloke->pos.x, bloke->pos.y) & 2) == 0 &&
            (GetCurrentRFFlags(x, y) & 2) != 0) {
            return 1;
        }
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00483580
int FUN_00483580(Bloke *bloke, int x, int y) {
    int rf;
    int mapFlags;
    int from;
    int to;

    if (x >= 0 && x < lpConfig->width * 0x100 && y >= 0 && y < lpConfig->height * 0x100) {
        rf = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
        mapFlags = Get_MapFlags(bloke->pos.x, bloke->pos.y);
        from = (rf & 2) != 0 && (mapFlags & 0x8800) == 0;
        rf = GetCurrentRFFlags(x, y);
        mapFlags = Get_MapFlags(x, y);
        to = (rf & 2) != 0 && (mapFlags & 0x8800) == 0;
        if (from || !to) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00483650
LEGO_EXPORT int OverNewTile(Bloke *bloke, unsigned int x, unsigned int y) {
    if (((bloke->pos.x ^ x) & 0xffffff00) || ((bloke->pos.y ^ y) & 0xffffff00)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483680
void FUN_00483680(Bloke *bloke, unsigned int x, unsigned int y) {
    MapElement *elem;
    FXSpriteList *set;
    unsigned int ux;
    unsigned int uy;
    int tx;
    int ty;
    Point p;

    if (OverNewTile(bloke, x, y) == 0) {
        return;
    }
    if ((Get_RFFlags(bloke->pos.x, bloke->pos.y) & 3) == 3) {
        uy = bloke->pos.y;
        ux = bloke->pos.x;
        tx = ux >> 8;
        ty = uy >> 8;
        if (tx >= 0 && tx < lpConfig->width && ty >= 0 && ty < lpConfig->height) {
            elem = &GameMap[ty][tx];
        } else {
            elem = NULL;
        }
        set = TileSpriteInfo[elem->field_8].src;
        if (set->on_enter != NULL) {
            set->on_leave(ux, uy);
        }
        bloke->flags &= 0xfff7;
    }
    if ((Get_RFFlags(x, y) & 3) == 3) {
        tx = x >> 8;
        ty = y >> 8;
        if (tx >= 0 && tx < lpConfig->width && ty >= 0 && ty < lpConfig->height) {
            elem = &GameMap[ty][tx];
        } else {
            elem = NULL;
        }
        set = TileSpriteInfo[elem->field_8].src;
        if (set->on_enter != NULL) {
            p.x = x;
            p.y = y;
            set->on_enter(p);
        }
        bloke->flags |= 8;
    }
}

// FUNCTION: LEGOLAND 0x004837a0
int FUN_004837a0(Bloke *bloke, unsigned int x, unsigned int y) {
    if (OverNewTile(bloke, x, y) != 0) {
        bloke->flags &= 0xfffb;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004837d0
LEGO_EXPORT int CrossTileCentre(Bloke *bloke, unsigned int x, unsigned int y) {
    unsigned int delta;
    switch (bloke->field_72) {
    case 0:
    case 1:
    case 4:
    case 5:
        delta = bloke->pos.y ^ y;
        break;
    default:
        delta = bloke->pos.x ^ x;
    }
    if ((delta & 0x80) != 0 && OverNewTile(bloke, x, y) == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483830
void FUN_00483830(Bloke *bloke) {
    bloke->field_75 = bloke->field_75 - 1;
    if (bloke->field_75 == 0) {
        bloke->field_75 = 2;
        bloke->field_74 = (bloke->field_74 + 1) & 7;
    }
}

// FUNCTION: LEGOLAND 0x00483850
void FUN_00483850(Bloke *bloke) {
    char step;

    if (--bloke->field_75 == 0) {
        bloke->field_75 = 3;
        step = ((bloke->field_72 - bloke->field_73) & 4) ? 1 : -1;
        bloke->field_72 = (step + bloke->field_72) & 7;
    }
}

// FUNCTION: LEGOLAND 0x00483890
void FUN_00483890(Bloke *bloke) {
    bloke->field_74 = 2;
}

// FUNCTION: LEGOLAND 0x004838a0
void FUN_004838a0(Bloke *bloke) {
    // STRING: LEGOLAND 0x004bdcd8
    DBPrintf("Frame %d.. Bloke %d rethinking\n", DAT_008119a4, bloke);
}

// FUNCTION: LEGOLAND 0x004838c0
void FUN_004838c0(Bloke *bloke) {
    bloke->field_75--;
    if (bloke->field_75 == 0) {
        bloke->field_75 = 1;
        DoPendingAction(bloke);
    }
}

// FUNCTION: LEGOLAND 0x004838e0
void FUN_004838e0(Bloke *bloke) {
    if (bloke->field_72 != bloke->field_73) {
        FUN_00483850(bloke);
    }
    if (bloke->field_72 == bloke->field_73) {
        bloke->field_75 = 1;
        DoPendingAction(bloke);
    }
}

// FUNCTION: LEGOLAND 0x00483920
LEGO_EXPORT int DoRndWalkPathTileAction(Bloke *bloke) {
    Point tile;
    short rf;
    short mapFlags;
    short rf2;
    unsigned char paths;
    unsigned char dirs;
    unsigned char dir;

    tile.x = bloke->pos.x >> 8;
    tile.y = bloke->pos.y >> 8;
    rf = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
    if (bloke->pos.x >= 0 && bloke->pos.x < lpConfig->width * 0x100 && bloke->pos.y >= 0 && bloke->pos.y < lpConfig->height * 0x100) {
        mapFlags = Get_MapFlags(bloke->pos.x, bloke->pos.y);
        rf2 = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
        if ((rf2 & 1) != 0 || ((mapFlags & 0x10) != 0 && (rf2 & 2) == 0)) {
            if ((rf & 8) != 0) {
                paths = Get_Path_Directions(&tile, 0, 0);
                dirs = ExcludeIsolatedDiags(paths);
                dirs &= ~Dir_To_Bit(bloke->field_72 + 4);
                dir = Bit_To_Dir(dirs);
                bloke->flags |= 4;
                return NewDirForAction(bloke, dir);
            }
            if ((rf & 0x24) != 0) {
                paths = Get_Path_Directions(&tile, 0, 0);
                dirs = ExcludeIsolatedDiags(paths);
                dirs &= ~(Dir_To_Bit(bloke->field_72 + 5) | Dir_To_Bit(bloke->field_72 + 4) | Dir_To_Bit(bloke->field_72 + 3));
                dir = Random_Dir_From_Bits(dirs);
                bloke->flags |= 4;
                return NewDirForAction(bloke, dir);
            }
            if ((rf & 0x10) != 0) {
                paths = Get_Path_Directions(&tile, 0, 0);
                dirs = ExcludeIsolatedDiags(paths);
                bloke->flags |= 4;
                if (dirs != 0) {
                    dir = Bit_To_Dir(dirs);
                } else {
                    dir = rand() & 7;
                }
                return NewDirForAction(bloke, dir);
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483b10
LEGO_EXPORT int Handle_RndWalk_TileSpecifics(Bloke *bloke, unsigned int x, unsigned int y) {
    if (FUN_004837a0(bloke, x, y) == 0) {
        if (CrossTileCentre(bloke, x, y) != 0 && (bloke->flags & 4) == 0) {
            return DoRndWalkPathTileAction(bloke);
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483b60
int FUN_00483b60(Bloke *bloke, unsigned int x, unsigned int y) {
    Point *pos;
    short mapFlags;
    short rf;

    if (FUN_004837a0(bloke, x, y) == 0 && CrossTileCentre(bloke, x, y) != 0 && (bloke->flags & 4) == 0) {
        pos = &bloke->pos;
        if (pos->x >= 0 && pos->x < lpConfig->width * 0x100 && pos->y >= 0 && pos->y < lpConfig->height * 0x100) {
            mapFlags = Get_MapFlags(pos->x, pos->y);
            rf = GetCurrentRFFlags(pos->x, pos->y);
            if ((rf & 1) != 0 || ((mapFlags & 0x10) != 0 && (rf & 2) == 0)) {
                if (FUN_00481790(pos) != NULL) {
                    bloke->field_e = 0;
                    return 1;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483c20
int FUN_00483c20(Bloke *bloke, int x, int y) {
    int chance;
    int state;
    unsigned char dir;

    chance = (bloke->flags & 2) ? 0 : 20;
    state = bloke->person->field_8;
    if (state != 2 && state != 3) {
        if (HitPathEdge(bloke, x, y) != 0 || HitObstacle(bloke, x, y) != 0 || (rand() & 0x3ff) < chance) {
            dir = rand() & 7;
            if ((bloke->flags & 2) != 0) {
                dir |= 1;
            }
            NewDirForAction(bloke, dir);
            return 1;
        }
    } else {
        if (HitPathEdge(bloke, x, y) != 0 || FUN_00483580(bloke, x, y) != 0) {
            dir = rand() & 7;
            if ((bloke->flags & 2) != 0) {
                dir |= 1;
            }
            NewDirForAction(bloke, dir);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483d10
void FUN_00483d10(Bloke *bloke) {
    Point d = FUN_004831a0(bloke->field_72, bloke->field_7f);

    d.x += bloke->pos.x;
    d.y += bloke->pos.y;
    if (FUN_00483300(bloke, d.x, d.y) == 0) {
        if (Handle_RndWalk_TileSpecifics(bloke, d.x, d.y) == 0) {
            if (FUN_00483c20(bloke, d.x, d.y) == 0) {
                FUN_00483680(bloke, d.x, d.y);
                bloke->pos = d;
                FUN_00483830(bloke);
            }
        }
        if ((bloke->field_5c & 0x7f) == 0) {
            bloke->field_e = 0;
            bloke->field_5c = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00483d90
void FUN_00483d90(Bloke *bloke) {
    Point d = FUN_004831a0(bloke->field_72, bloke->field_7f);

    d.x += bloke->pos.x;
    d.y += bloke->pos.y;
    if (DAT_008119a4 - bloke->field_54 >= 0x32) {
        if (FUN_00483300(bloke, d.x, d.y) == 0) {
            if (Handle_RndWalk_TileSpecifics(bloke, d.x, d.y) == 0) {
                if (FUN_00483c20(bloke, d.x, d.y) == 0) {
                    FUN_00483680(bloke, d.x, d.y);
                    bloke->pos = d;
                    FUN_00483830(bloke);
                }
            }
            if ((bloke->field_5c & 0x7f) == 0) {
                bloke->field_e = 0;
                bloke->field_5c = 0;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00483e20
void FUN_00483e20(Bloke *bloke) {
    Point d;
    if (bloke->pos.x >= 0 && bloke->pos.x < lpConfig->width * 0x100 &&
        bloke->pos.y >= 0 && bloke->pos.y < lpConfig->height * 0x100) {
        short mapFlags = Get_MapFlags(bloke->pos.x, bloke->pos.y);
        short rf = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
        if ((rf & 1) != 0 || ((mapFlags & 0x10) != 0 && (rf & 2) == 0)) {
            bloke->field_e = 0;
            return;
        }
    }
    d = FUN_004831a0(bloke->field_72, bloke->field_7f);
    d.x += bloke->pos.x;
    d.y += bloke->pos.y;
    if (FUN_00483300(bloke, d.x, d.y) == 0) {
        if (FUN_00483b60(bloke, d.x, d.y) == 0) {
            if (FUN_00483c20(bloke, d.x, d.y) == 0) {
                FUN_00483680(bloke, d.x, d.y);
                bloke->pos = d;
                FUN_00483830(bloke);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00483ef0
void FUN_00483ef0(Bloke *bloke) {
    Point d = FUN_004831a0(bloke->field_72, bloke->field_7f);
    short rf;
    short mapFlags;
    short rf2;

    d.x += bloke->pos.x;
    d.y += bloke->pos.y;

    if (FUN_00483300(bloke, d.x, d.y) != 0) {
        return;
    }
    FUN_004837a0(bloke, d.x, d.y);
    if (CrossTileCentre(bloke, d.x, d.y) != 0) {
        bloke->flags |= 4;
        rf = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
        if ((bloke->pos.x < 0 || bloke->pos.x >= lpConfig->width * 0x100 || bloke->pos.y < 0 ||
                bloke->pos.y >= lpConfig->height * 0x100 ||
                (mapFlags = Get_MapFlags(bloke->pos.x, bloke->pos.y), rf2 = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y),
                    (rf2 & 1) == 0 && ((mapFlags & 0x10) == 0 || (rf2 & 2) != 0))) &&
            (Get_MapFlags(bloke->pos.x, bloke->pos.y) & 0x10) == 0) {
            bloke->field_e = 0;
            bloke->field_64 |= 2;
            return;
        }
        if ((rf & 0x24) != 0) {
            bloke->field_e = 0;
            bloke->field_64 |= 4;
            return;
        }
        if ((rf & 8) != 0) {
            int coords[2];
            unsigned char dirs;
            coords[0] = d.x >> 8;
            coords[1] = d.y >> 8;
            dirs = Get_Path_Directions(coords, 0, 0);
            dirs = ExcludeIsolatedDiags(dirs);
            dirs = dirs & ~Dir_To_Bit(bloke->field_72 + 4);
            dirs = Bit_To_Dir(dirs);
            NewDirForAction(bloke, dirs);
        }
    }
    FUN_00483680(bloke, d.x, d.y);
    bloke->pos = d;
    FUN_00483830(bloke);
}

// FUNCTION: LEGOLAND 0x00484090
void FUN_00484090(Bloke *bloke) {
    Point d = FUN_004831a0(bloke->field_72, bloke->field_7f);
    Point next;
    short rf;
    short mapFlags;
    short rf2;

    d.x += bloke->pos.x;
    d.y += bloke->pos.y;
    next = d;
    FUN_004837a0(bloke, next.x, next.y);
    if (CrossTileCentre(bloke, next.x, next.y) != 0) {
        bloke->flags |= 4;
        rf = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
        if (bloke->pos.x < 0 || bloke->pos.x >= lpConfig->width * 0x100 || bloke->pos.y < 0 ||
            bloke->pos.y >= lpConfig->height * 0x100) {
            bloke->field_e = 0;
            bloke->field_64 |= 2;
            return;
        }
        mapFlags = Get_MapFlags(bloke->pos.x, bloke->pos.y);
        rf2 = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
        if ((rf2 & 1) == 0 && ((mapFlags & 0x10) == 0 || (rf2 & 2) != 0)) {
            bloke->field_e = 0;
            bloke->field_64 |= 2;
            return;
        }
        if ((rf & 0x24) != 0) {
            bloke->field_e = 0;
            bloke->field_64 |= 4;
            return;
        }
        if ((rf & 8) != 0) {
            bloke->field_e = 0;
            return;
        }
    }
    FUN_00483680(bloke, next.x, next.y);
    bloke->pos = next;
    FUN_00483830(bloke);
}

// FUNCTION: LEGOLAND 0x004841a0
int FUN_004841a0(Bloke *bloke, int dist) {
    int dx = bloke->dest.x - bloke->pos.x;
    int dy = bloke->dest.y - bloke->pos.y;
    return dist * dist >= dy * dy + dx * dx;
}

// FUNCTION: LEGOLAND 0x004841e0
unsigned char FUN_004841e0(Bloke *bloke) {
    int dx = bloke->dest.x - bloke->nav.x;
    int dy = bloke->dest.y - bloke->nav.y;
    return (dx * dx + dy * dy) <= 0x400;
}

// FUNCTION: LEGOLAND 0x00484220
void FUN_00484220(Bloke *bloke) {
    Point target;
    short mapFlags;
    short rf;

    if (FUN_004841a0(bloke, bloke->field_7f << 1) != 0) {
        bloke->field_e = 0;
        return;
    }
    NavigMoveLine(&bloke->nav, bloke->field_7f, &target);
    if (FUN_004837a0(bloke, target.x, target.y) != 0) {
        if (HitObstacle(bloke, target.x, target.y) != 0) {
            bloke->field_e = 0;
            bloke->field_64 |= 1;
            return;
        }
        if (target.x < 0 || target.x >= lpConfig->width * 0x100 || target.y < 0 ||
            target.y >= lpConfig->height * 0x100 ||
            (mapFlags = Get_MapFlags(target.x, target.y), rf = GetCurrentRFFlags(target.x, target.y),
                (rf & 1) == 0 && ((mapFlags & 0x10) == 0 || (rf & 2) != 0))) {
            bloke->field_e = 0;
            bloke->field_64 |= 2;
            return;
        }
    }
    FUN_00483680(bloke, target.x, target.y);
    bloke->pos = target;
    FUN_00483830(bloke);
}

// FUNCTION: LEGOLAND 0x00484350
void FUN_00484350(Bloke *bloke) {
    Point target;
    short mapFlags;
    short rf;

    if ((bloke->flags & 2) != 0) {
        bloke->field_e = 0;
        return;
    }
    if (FUN_004841a0(bloke, bloke->field_7f << 1) != 0) {
        bloke->field_e = 0;
        return;
    }
    NavigMoveLine(&bloke->nav, bloke->field_7f, &target);
    if (FUN_004837a0(bloke, target.x, target.y) != 0) {
        if (HitObstacle(bloke, target.x, target.y) == 0) {
            if (target.x >= 0 && target.x < lpConfig->width * 0x100 && target.y >= 0 && target.y < lpConfig->height * 0x100) {
                mapFlags = Get_MapFlags(target.x, target.y);
                rf = GetCurrentRFFlags(target.x, target.y);
                if ((rf & 1) != 0 || ((mapFlags & 0x10) != 0 && (rf & 2) == 0)) {
                    bloke->field_e = 0;
                    return;
                }
            }
        } else {
            bloke->field_e = 0;
            return;
        }
    }
    FUN_00483680(bloke, target.x, target.y);
    bloke->pos = target;
    FUN_00483830(bloke);
}

// FUNCTION: LEGOLAND 0x00484470
void FUN_00484470(Bloke *bloke) {
    Point target;
    if (FUN_004841a0(bloke, bloke->field_7f << 1) != 0) {
        bloke->field_e = 0;
        return;
    }
    NavigMoveLine(&bloke->nav, bloke->field_7f, &target);
    if (FUN_004837a0(bloke, target.x, target.y) != 0) {
        if (HitObstacle(bloke, target.x, target.y) != 0) {
            bloke->field_e = 0;
            bloke->field_64 |= 1;
            return;
        }
    }
    FUN_00483680(bloke, target.x, target.y);
    bloke->pos = target;
    FUN_00483830(bloke);
}

// FUNCTION: LEGOLAND 0x00484520
void FUN_00484520(Bloke *bloke) {
    Point target;
    if (FUN_004841a0(bloke, bloke->field_7f << 1) != 0) {
        bloke->field_e = 0;
        return;
    }
    NavigMoveLine(&bloke->nav, bloke->field_7f, &target);
    if (FUN_004837a0(bloke, target.x, target.y) != 0) {
        if (FUN_00483580(bloke, target.x, target.y) != 0) {
            bloke->field_e = 0;
            bloke->field_64 |= 1;
            return;
        }
    }
    FUN_00483680(bloke, target.x, target.y);
    bloke->pos = target;
    FUN_00483830(bloke);
}

// FUNCTION: LEGOLAND 0x004845d0
void FUN_004845d0(Bloke *bloke) {
    Point target;
    if (FUN_004841a0(bloke, bloke->field_7f << 1) != 0) {
        DoPendingAction(bloke);
        return;
    }
    NavigMoveLine(&bloke->nav, bloke->field_7f, &target);
    bloke->pos = target;
    FUN_00483830(bloke);
}

// FUNCTION: LEGOLAND 0x00484630
void FUN_00484630(Bloke *bloke) {
    if (bloke->field_70 == 0) {
        bloke->field_46 = bloke->field_44;
        if (bloke->field_3a-- == 0) {
            DoPendingAction(bloke);
            return;
        }
    }
    bloke->field_70 += bloke->field_46--;
    if (bloke->field_70 <= 0) {
        bloke->field_70 = 0;
    }
    bloke->field_74 = 0;
    bloke->field_73 = (bloke->field_72 + 1) & 7;
    FUN_00483850(bloke);
}

// FUNCTION: LEGOLAND 0x004846a0
LEGO_EXPORT Point GetTileInDir(Point pos, unsigned char dir) {
    switch (dir & 7) {
    case 1:
        pos.y -= 0x100;
        break;
    case 5:
        pos.y += 0x100;
        break;
    case 3:
        pos.x += 0x100;
        break;
    case 7:
        pos.x -= 0x100;
        break;
    case 2:
        pos.y -= 0x100;
        pos.x += 0x100;
        break;
    case 0:
        pos.y -= 0x100;
        pos.x -= 0x100;
        break;
    case 4:
        pos.y += 0x100;
        pos.x += 0x100;
        break;
    case 6:
        pos.y += 0x100;
        pos.x -= 0x100;
        break;
    }
    return pos;
}

// FUNCTION: LEGOLAND 0x00484790
void FUN_00484790(Bloke *bloke) {
    Point tile;
    MapElement *elem;
    FXSpriteList *set;
    int result;
    unsigned int ux;
    unsigned int uy;
    int x;
    int y;

    tile = GetTileInDir(bloke->pos, bloke->field_72);
    if ((Get_RFFlags(tile.x, tile.y) & 3) != 3) {
        bloke->flags &= 0xfff7;
        DoPendingAction(bloke);
    }
    x = tile.x >> 8;
    y = tile.y >> 8;
    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
        elem = &GameMap[y][x];
    } else {
        elem = NULL;
    }
    set = TileSpriteInfo[elem->field_8].src;
    if (set->get_rf_flags != NULL) {
        result = set->get_rf_flags(tile.x, tile.y);
    } else {
        result = 2;
    }
    if ((result & 3) > 0 && (result & 3) <= 2) {
        ux = tile.x;
        uy = tile.y;
        x = ux >> 8;
        y = uy >> 8;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            elem = &GameMap[y][x];
        } else {
            elem = NULL;
        }
        set = TileSpriteInfo[elem->field_8].src;
        if (set->on_enter != NULL) {
            set->on_leave(tile.x, tile.y);
        }
        bloke->flags &= 0xfff7;
        DoPendingAction(bloke);
        PTR_FUN_004bd34c[bloke->field_e](bloke);
        return;
    }
    bloke->field_20++;
    FUN_00483890(bloke);
}

// FUNCTION: LEGOLAND 0x004848e0
void FUN_004848e0(Bloke *bloke) {
    if (!((DAT_004bdd00 >> 8) & 0x2) || DAT_004bdd04 != bloke) {
        bloke->flags &= 0xfff7;
        DoPendingAction(bloke);
    }
}

// FUNCTION: LEGOLAND 0x00484910
LEGO_EXPORT void Bloke_DoNothing(void) {
}

// FUNCTION: LEGOLAND 0x00484920
LEGO_EXPORT void DoLowLevelAI(Bloke *bloke) {
    SetPathFlag(bloke);
    PTR_FUN_004bd34c[bloke->field_e](bloke);
}

// FUNCTION: LEGOLAND 0x00484950
LEGO_EXPORT void ApplyObjectOrientationToPerson(Person *person, float *matrix, void *param_3) {
    float scale = 65536.0f;
    person->fm[0] = matrix[0];
    person->fm[1] = matrix[6];
    person->fm[2] = -matrix[3];
    person->fm[3] = -matrix[2];
    person->fm[4] = -matrix[8];
    person->fm[5] = matrix[5];
    person->fm[6] = -matrix[1];
    person->fm[7] = -matrix[7];
    person->fm[8] = matrix[4];
    person->m[0] = person->fm[0] * scale;
    person->m[3] = person->fm[3] * scale;
    person->m[6] = person->fm[6] * scale;
    person->m[1] = person->fm[1] * scale;
    person->m[4] = person->fm[4] * scale;
    person->m[7] = person->fm[7] * scale;
    person->m[2] = person->fm[2] * scale;
    person->m[5] = person->fm[5] * scale;
    person->m[8] = person->fm[8] * scale;
}

// FUNCTION: LEGOLAND 0x00484a70
LEGO_EXPORT void SetBlokePositionFromBNV(BinVFile *file, Bloke *bloke, char *name, int frame, float near_z, float far_z, float *orient) {
    BinVObject *object;
    Vertex *vertex;
    int sumX = 0;
    float sumZ = 0.0f;
    int sumY = 0;
    BinVFrame *binFrame;
    float len;
    float scale;
    float skew;
    float zscale;
    float depth;
    int z;
    int i;

    binFrame = GetBinVFrame(file, frame);
    object = GetObjectFromName(binFrame, name);
    len = sqrt(object->m18 * object->m18 + object->m14 * object->m14 + object->m10 * object->m10);
    scale = 1.0f / len;
    object->m10 *= scale;
    object->m14 *= scale;
    object->m18 *= scale;
    len = sqrt(object->m24 * object->m24 + object->m20 * object->m20 + object->m1c * object->m1c);
    scale = 1.0f / len;
    object->m1c *= scale;
    object->m20 *= scale;
    object->m24 *= scale;
    len = sqrt(object->m2c * object->m2c + object->m28 * object->m28 + object->m30 * object->m30);
    scale = 1.0f / len;
    object->m28 *= scale;
    object->m2c *= scale;
    object->m30 *= scale;
    for (i = 0; i < 8; i++) {
        vertex = GetVertex(object, i);
        sumX += vertex->x;
        sumY += vertex->y;
        sumZ += vertex->z;
    }
    skew = GetZSkew(file, object, vertex);
    zscale = 49152.0f / (near_z - far_z);
    depth = sumZ * 0.125;
    depth = (depth - far_z) * zscale;
    z = depth + 8192.0f;
    bloke->person->field_34 = z >> 8;
    bloke->screen_x = sumX / 8 / 2;
    bloke->screen_y = sumY / 8 / 2;
    bloke->person->field_38 = skew + skew;
    ApplyObjectOrientationToPerson(bloke->person, &object->m10, orient);
}

// FUNCTION: LEGOLAND 0x00484c20
LEGO_EXPORT BNVPath *NewBNVPath(BinVFile *file, unsigned int param_2, char *name, float param_4, float param_5, int *coords) {
    BNVPath *path = malloc(sizeof(BNVPath));
    BinVFrame *frame;
    BinVObject *object;
    Vertex *vertex;
    float scale = 49152.0f / (param_4 - param_5);
    path->file = file;
    strcpy(path->name, name);
    path->field_1c = scale;
    path->frame_index = 0;
    path->field_20 = param_5;
    path->field_44 = 1;
    path->x = coords[0];
    path->y = coords[1];
    frame = GetBinVFrame(file, 0);
    object = GetObjectFromName(frame, name);
    vertex = GetVertex(object, 0);
    path->field_3c = GetZSkew(file, object, vertex);
    path->field_4 = param_2;
    return path;
}

// FUNCTION: LEGOLAND 0x00484cd0
LEGO_EXPORT int UpdateBlokeFromBNVPath(Bloke *bloke, BNVPath *path) {
    Person *person = bloke->person;
    float sumZ = 0.0f;
    float sumY = 0.0f;
    float sumX = 0.0f;
    int frame = path->frame_index;
    BinVFrame *binFrame;
    BinVObject *object;
    Vertex *vertex;
    float dx;
    float dy;
    float len;
    float scale;
    float speed;
    int z;
    double angle;
    int i;

    if (frame == path->file->frameCount) {
        return 0;
    }
    binFrame = GetBinVFrame(path->file, frame);
    object = GetObjectFromName(binFrame, path->name);
    for (i = 0; i < 8; i++) {
        vertex = GetVertex(object, i);
        sumY += vertex->y;
        sumX += vertex->x;
        sumZ += vertex->z;
    }
    dx = sumX * 0.125 - path->x;
    dy = sumY * 0.125 - path->y;
    if (path->field_44 == 0) {
        path->x = path->dx + path->x;
        path->y = path->dy + path->y;
        speed = bloke->field_7f;
        if (dx * dx + dy * dy < speed * speed) {
            path->field_44 = 1;
            frame = ++path->frame_index;
        }
    }
    if (frame > 0) {
        binFrame = GetBinVFrame(path->file, frame - 1);
    } else {
        binFrame = GetBinVFrame(path->file, 0);
    }
    object = GetObjectFromName(binFrame, path->name);
    len = sqrt(object->m18 * object->m18 + object->m14 * object->m14 + object->m10 * object->m10);
    scale = 1.0f / len;
    object->m10 *= scale;
    object->m14 *= scale;
    object->m18 *= scale;
    object->m1c *= scale;
    object->m20 *= scale;
    object->m24 *= scale;
    object->m28 *= scale;
    object->m2c *= scale;
    object->m30 *= scale;
    ApplyObjectOrientationToPerson(bloke->person, &object->m10, 0);
    if (path->field_44 != 0) {
        path->field_44 = 0;
        sumX = 0.0f;
        sumY = 0.0f;
        sumZ = 0.0f;
        if (frame == path->file->frameCount) {
            return 0;
        }
        binFrame = GetBinVFrame(path->file, frame);
        object = GetObjectFromName(binFrame, path->name);
        for (i = 0; i < 8; i++) {
            vertex = GetVertex(object, i);
            sumY += vertex->y;
            sumX += vertex->x;
            sumZ += vertex->z;
        }
        dx = sumX * 0.125 - path->x;
        dy = sumY * 0.125 - path->y;
        z = (sumZ * 0.125 - path->field_20) * path->field_1c + 8192.0f;
        angle = atan2(dy, dx);
        bloke->person->field_34 = z >> 8;
        path->dx = bloke->field_7f * cos(angle) * 0.25;
        path->dy = bloke->field_7f * sin(angle) * 0.25;
    }
    person->field_38 = path->field_3c + path->field_3c;
    bloke->screen_x = path->x * 0.5f;
    bloke->screen_y = path->y * 0.5f;
    FUN_00483830(bloke);
    return 1;
}

// FUNCTION: LEGOLAND 0x00484ff0
LEGO_EXPORT unsigned int BNVPath_GetDFrame(BNVPath *path) {
    return path->frame_index;
}

// FUNCTION: LEGOLAND 0x00485000
LEGO_EXPORT Point BNVPath_GetBINVScreenCoords(BNVPath *path, int frame) {
    float sumX = 0.0f;
    float sumY = 0.0f;
    float sumZ = 0.0f;
    BinVFrame *binFrame;
    BinVObject *object;
    Vertex *vertex;
    int i;
    Point result;

    binFrame = GetBinVFrame(path->file, frame);
    object = GetObjectFromName(binFrame, path->name);
    for (i = 0; i < 8; i++) {
        vertex = GetVertex(object, i);
        sumX += vertex->x;
        sumY += vertex->y;
        sumZ += vertex->z;
    }
    result.x = sumX * 0.125;
    result.y = sumY * 0.125;
    return result;
}

// FUNCTION: LEGOLAND 0x004850b0
LEGO_EXPORT void BNVPath_SetDFrame(Bloke *bloke, BNVPath *path, int frame) {
    float sumX = 0.0f;
    float sumY = 0.0f;
    float sumZ = 0.0f;
    BinVFrame *binFrame;
    BinVObject *object;
    Vertex *vertex;
    int i;
    int z;
    float dx;
    float dy;
    double angle;

    path->frame_index = frame;
    path->field_44 = 0;
    binFrame = GetBinVFrame(path->file, frame);
    object = GetObjectFromName(binFrame, path->name);
    for (i = 0; i < 8; i++) {
        vertex = GetVertex(object, i);
        sumX += vertex->x;
        sumY += vertex->y;
        sumZ += vertex->z;
    }
    path->x = sumX * 0.125;
    path->y = sumY * 0.125;
    sumX = 0.0f;
    sumY = 0.0f;
    sumZ = 0.0f;
    path->field_44 = 0;
    binFrame = GetBinVFrame(path->file, frame + 1);
    object = GetObjectFromName(binFrame, path->name);
    for (i = 0; i < 8; i++) {
        vertex = GetVertex(object, i);
        sumX += vertex->x;
        sumY += vertex->y;
        sumZ += vertex->z;
    }
    dx = sumX * 0.125 - path->x;
    dy = sumY * 0.125 - path->y;
    z = (sumZ * 0.125 - path->field_20) * path->field_1c + 8192.0f;
    angle = atan2(dy, dx);
    bloke->person->field_34 = z >> 8;
    path->dx = bloke->field_7f * cos(angle) * 0.25;
    path->dy = bloke->field_7f * sin(angle) * 0.25;
}

// FUNCTION: LEGOLAND 0x00485260
int CheckForPeople(MapRect *rect) {
    Bloke *current = FirstBloke;
    int found = 0;
    int x;
    int y;

    for (y = rect->y0; y <= rect->y1; y++) {
        for (x = rect->x0; x <= rect->x1; x++) {
            GameMap[y][x].flags &= 0xefff;
        }
    }
    if (current != NULL) {
        do {
            if ((current->flags & 0x20) == 0) {
                int tx = current->pos.x >> 8;
                int ty = current->pos.y >> 8;
                if (tx >= 0 && tx < lpConfig->width && ty >= 0 && ty < lpConfig->height) {
                    GameMap[ty][tx].flags |= 0x1000;
                    if (tx >= rect->x0 && tx <= rect->x1 && ty >= rect->y0 && ty <= rect->y1) {
                        found = 1;
                    }
                }
            }
            current = current->next;
        } while (current != NULL);
        if (found != 0) {
            return 1;
        }
    }
    FUN_0049cf00(rect);
    for (y = rect->y0; y <= rect->y1; y++) {
        for (x = rect->x0; x <= rect->x1; x++) {
            if ((GameMap[y][x].flags & 0x1000) != 0) {
                return -1;
            }
        }
    }
    return 0;
}
