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
void ResetPathUpdateTimer(void) {
    LastPathUpdateTime = GetGameTimer();
}

// FUNCTION: LEGOLAND 0x00482b20
void UpdatePathLinks(int force) {
    int now = GetGameTimer();
    if (now - LastPathUpdateTime <= 0xfa0 && force == 0) {
        return;
    }
    LastPathUpdateTime = now;
    InitEntrance1Point();
    UpdateConnectedNodes(&Entrance1Point);
}

/* True when the path node at the point has bit 1 of field_20 set after refreshing the path links (UpdatePathLinks):
 * used by the linked-objects report, ScriptEventLink and GetLinkedObjectPercent. */
// FUNCTION: LEGOLAND 0x00482b60
int IsPathPointLinked(Point *pos) {
    BestNode *node = FindBestNodeAtPoint(pos);
    if (node != NULL) {
        UpdatePathLinks(PathUpdateNeeded);
        PathUpdateNeeded = 0;
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
        name = PTR_s_Aaron_004bcecc[bloke->first_name_index];
    } else {
        name = PTR_s_Abbie_004bd018[bloke->first_name_index];
    }
    strcpy(VisitorNameBuffer, name);
    strcat(VisitorNameBuffer, " ");
    strcat(VisitorNameBuffer, PTR_s_Adams_004bd180[bloke->last_name_index]);
    return VisitorNameBuffer;
}

// FUNCTION: LEGOLAND 0x00482c60
void RandomiseBlokeName(Bloke *bloke) {
    unsigned int roll;

    if (bloke->person->random != 0) {
        roll = rand();
        bloke->first_name_index = roll % 0x5a;
    } else {
        roll = rand();
        bloke->first_name_index = roll % 0x53;
    }
    roll = rand();
    bloke->last_name_index = roll % 0x6b;
}

/* Mood icon for a visitor's hover bubble (HTBubbleHelpWithMood): fixed icons for long-term actions 3, 11/12 and 13,
 * otherwise from the mood against MapStats.mood_threshold1/3. */
// FUNCTION: LEGOLAND 0x00482cb0
int GetBlokeMoodIconIndex(Bloke *bloke) {
    switch (bloke->action) {
    case 3:
        return 4;
    case 0xb:
    case 0xc:
        return 1;
    case 0xd:
        return 5;
    }
    if (bloke->mood < MapStats.mood_threshold1) {
        return 3;
    }
    return bloke->mood < MapStats.mood_threshold3 ? 10 : 2;
}

// FUNCTION: LEGOLAND 0x00482d30
int GetBlokeMood(Bloke *bloke) {
    if (bloke->mood < MapStats.mood_threshold1) {
        return 3;
    }
    return bloke->mood < MapStats.mood_threshold3 ? 10 : 2;
}

// FUNCTION: LEGOLAND 0x00482d60
void SetMoodDelta(unsigned int index, int value) {
    MapStats.mood_delta[index] = value;
}

// FUNCTION: LEGOLAND 0x00482d70
void SetDefaultMoodDeltas(void) {
    MapStats.mood_delta[4] = 5;
    MapStats.mood_delta[5] = 5;
    MapStats.mood_delta[6] = 5;
    MapStats.mood_delta[0] = -100;
    MapStats.mood_delta[1] = -400;
    MapStats.mood_delta[2] = 33;
    MapStats.mood_delta[3] = 20;
    MapStats.mood_delta[7] = -200;
    MapStats.mood_delta[8] = -1600;
    MapStats.mood_delta[9] = 400;
    MapStats.mood_delta[10] = 100;
    MapStats.mood_delta[11] = 400;
    MapStats.mood_delta[12] = 100;
}

// FUNCTION: LEGOLAND 0x00482df0
int ApplyMoodDelta(Bloke *bloke, int index, int mul) {
    int value = MapStats.mood_delta[index] * mul / 100 + bloke->mood;
    if (value < -30000) {
        value = -30000;
    } else if (value > 30000) {
        value = 30000;
    }
    bloke->mood = value;
    return bloke->mood;
}

// FUNCTION: LEGOLAND 0x00482e50
void AllocateBlokePool(void) {
    int i;
    BlokePool = malloc(lpConfig->max_blokes * sizeof(Bloke));
    for (i = 0; i < lpConfig->max_blokes; i++) {
        memset(&BlokePool[i], 0, sizeof(Bloke));
    }
}

// FUNCTION: LEGOLAND 0x00482ec0
void FreeBlokePool(void) {
    if (BlokePool != NULL) {
        free(BlokePool);
    }
    FirstBloke = NULL;
    BlokePool = NULL;
}

// FUNCTION: LEGOLAND 0x00482ef0
LEGO_EXPORT Bloke *NewBloke(void) {
    Bloke *bloke = NULL;
    int i;

    for (i = 0; i < lpConfig->max_blokes; i++) {
        if ((BlokePool[i].flags & 1) == 0) {
            bloke = &BlokePool[i];
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
    return bloke - BlokePool;
}

// FUNCTION: LEGOLAND 0x00482fe0
LEGO_EXPORT Bloke *GetBlokePtr(int index) {
    if (index == -1) {
        return NULL;
    }
    return &BlokePool[index];
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
    source.bloke = bloke;
    KillAllSamplesFromSource(&source);
    RemovePersonFromList(bloke->person);
    FreePerson(bloke->person);
    bloke->flags &= 0xfffe;
    bloke->next = NULL;
}

// FUNCTION: LEGOLAND 0x00483090
void DestroyAllBlokes(void) {
    while (FirstBloke != NULL) {
        DestroyBloke(FirstBloke);
    }
    ParkVisitorCount = 0;
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
LEGO_EXPORT void InitialiseBlokes(void) { AllocateBlokePool(); }

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
int IsPointOnMap(int x, int y) {
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
Point GetOffsetInDir(unsigned char dir, short dist) {
    Point result;
    result.x = BlokeDirVectors[dir][0] * dist >> 8;
    result.y = BlokeDirVectors[dir][1] * dist >> 8;
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
    bloke->low_level_action = bloke->pending_action;
    bloke->pending_action = 0;
    return bloke->low_level_action;
}

// FUNCTION: LEGOLAND 0x00483260
void StartPassThroughTile(Bloke *bloke) {
    Point tile;
    unsigned int ux;
    unsigned int uy;
    int x;
    int y;
    MapElement *elem;
    FXSpriteList *set;

    bloke->flags |= 8;
    bloke->pending_action = bloke->low_level_action;
    bloke->low_level_action = 9;
    bloke->field_20 = 0;
    tile = GetTileInDir(bloke->pos, bloke->dir);
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
int CheckPassThroughTile(Bloke *bloke, int x, int y) {
    unsigned char result;
    int tx;
    int ty;
    MapElement *elem;
    FXSpriteList *set;

    if (OverNewTile(bloke, x, y) == 0) {
        return 0;
    }
    if (IsPointOnMap(x, y) == 0) {
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
        StartPassThroughTile(bloke);
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004833d0
LEGO_EXPORT int NewDirForAction(Bloke *bloke, unsigned char dir) {
    unsigned char masked = dir & 0x7;
    if (bloke->dir != masked) {
        bloke->pending_action = bloke->low_level_action;
        bloke->target_dir = masked;
        bloke->low_level_action = 5;
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
    DirPickCounts[dir]++;
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
int IsLeavingPath(Bloke *bloke, int x, int y) {
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
void HandleTileEnterLeave(Bloke *bloke, unsigned int x, unsigned int y) {
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
int CheckEnteredNewTile(Bloke *bloke, unsigned int x, unsigned int y) {
    if (OverNewTile(bloke, x, y) != 0) {
        bloke->flags &= 0xfffb;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004837d0
LEGO_EXPORT int CrossTileCentre(Bloke *bloke, unsigned int x, unsigned int y) {
    unsigned int delta;
    switch (bloke->dir) {
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
void AdvanceBlokeFrame(Bloke *bloke) {
    bloke->field_75 = bloke->field_75 - 1;
    if (bloke->field_75 == 0) {
        bloke->field_75 = 2;
        bloke->frame = (bloke->frame + 1) & 7;
    }
}

// FUNCTION: LEGOLAND 0x00483850
void TurnTowardsTargetDir(Bloke *bloke) {
    char step;

    if (--bloke->field_75 == 0) {
        bloke->field_75 = 3;
        step = ((bloke->dir - bloke->target_dir) & 4) ? 1 : -1;
        bloke->dir = (step + bloke->dir) & 7;
    }
}

/* Sets bloke->frame to 2. */
// FUNCTION: LEGOLAND 0x00483890
void SetBlokeFrame2(Bloke *bloke) {
    bloke->frame = 2;
}

// FUNCTION: LEGOLAND 0x004838a0
void LogBlokeRethinking(Bloke *bloke) {
    // STRING: LEGOLAND 0x004bdcd8
    DBPrintf("Frame %d.. Bloke %d rethinking\n", FrameCounter, bloke);
}

// FUNCTION: LEGOLAND 0x004838c0
void BlokeActionWait(Bloke *bloke) {
    bloke->field_75--;
    if (bloke->field_75 == 0) {
        bloke->field_75 = 1;
        DoPendingAction(bloke);
    }
}

// FUNCTION: LEGOLAND 0x004838e0
void BlokeActionTurn(Bloke *bloke) {
    if (bloke->dir != bloke->target_dir) {
        TurnTowardsTargetDir(bloke);
    }
    if (bloke->dir == bloke->target_dir) {
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
                dirs &= ~Dir_To_Bit(bloke->dir + 4);
                dir = Bit_To_Dir(dirs);
                bloke->flags |= 4;
                return NewDirForAction(bloke, dir);
            }
            if ((rf & 0x24) != 0) {
                paths = Get_Path_Directions(&tile, 0, 0);
                dirs = ExcludeIsolatedDiags(paths);
                dirs &= ~(Dir_To_Bit(bloke->dir + 5) | Dir_To_Bit(bloke->dir + 4) | Dir_To_Bit(bloke->dir + 3));
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
    if (CheckEnteredNewTile(bloke, x, y) == 0) {
        if (CrossTileCentre(bloke, x, y) != 0 && (bloke->flags & 4) == 0) {
            return DoRndWalkPathTileAction(bloke);
        }
    }
    return 0;
}

/* While walking until a path, stops low-level movement at a tile centre with suitable path/RF flags and a usable
 * path node. Requires no tile crossing and no bloke flag 4. Returns 1 when stopped. */
// FUNCTION: LEGOLAND 0x00483b60
int TryStopBlokeAtLinkedPath(Bloke *bloke, unsigned int x, unsigned int y) {
    Point *pos;
    short mapFlags;
    short rf;

    if (CheckEnteredNewTile(bloke, x, y) == 0 && CrossTileCentre(bloke, x, y) != 0 && (bloke->flags & 4) == 0) {
        pos = &bloke->pos;
        if (pos->x >= 0 && pos->x < lpConfig->width * 0x100 && pos->y >= 0 && pos->y < lpConfig->height * 0x100) {
            mapFlags = Get_MapFlags(pos->x, pos->y);
            rf = GetCurrentRFFlags(pos->x, pos->y);
            if ((rf & 1) != 0 || ((mapFlags & 0x10) != 0 && (rf & 2) == 0)) {
                if (FindBestNodeAtPoint(pos) != NULL) {
                    bloke->low_level_action = 0;
                    return 1;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483c20
int TurnRandomlyIfBlocked(Bloke *bloke, Point pos) {
    int chance;
    int state;

    chance = (bloke->flags & 2) ? 0 : 20;
    state = bloke->person->character;
    if (state != 2 && state != 3) {
        if (HitPathEdge(bloke, pos.x, pos.y) != 0 || HitObstacle(bloke, pos.x, pos.y) != 0 || (rand() & 0x3ff) < chance) {
            unsigned char dir = rand() & 7;
            if ((bloke->flags & 2) != 0) {
                dir |= 1;
            }
            NewDirForAction(bloke, dir);
            return 1;
        }
    } else {
        if (HitPathEdge(bloke, pos.x, pos.y) != 0 || IsLeavingPath(bloke, pos.x, pos.y) != 0) {
            unsigned char dir = rand() & 7;
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
void BlokeActionRandomWalk(Bloke *bloke) {
    Point d = GetOffsetInDir(bloke->dir, bloke->speed);

    d.x += bloke->pos.x;
    d.y += bloke->pos.y;
    if (CheckPassThroughTile(bloke, d.x, d.y) == 0) {
        if (Handle_RndWalk_TileSpecifics(bloke, d.x, d.y) == 0) {
            if (TurnRandomlyIfBlocked(bloke, d) == 0) {
                HandleTileEnterLeave(bloke, d.x, d.y);
                bloke->pos = d;
                AdvanceBlokeFrame(bloke);
            }
        }
        if ((bloke->field_5c & 0x7f) == 0) {
            bloke->low_level_action = 0;
            bloke->field_5c = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00483d90
void BlokeActionRandomWalkDelayed(Bloke *bloke) {
    Point d = GetOffsetInDir(bloke->dir, bloke->speed);

    d.x += bloke->pos.x;
    d.y += bloke->pos.y;
    if (FrameCounter - bloke->field_54 >= 0x32) {
        if (CheckPassThroughTile(bloke, d.x, d.y) == 0) {
            if (Handle_RndWalk_TileSpecifics(bloke, d.x, d.y) == 0) {
                if (TurnRandomlyIfBlocked(bloke, d) == 0) {
                    HandleTileEnterLeave(bloke, d.x, d.y);
                    bloke->pos = d;
                    AdvanceBlokeFrame(bloke);
                }
            }
            if ((bloke->field_5c & 0x7f) == 0) {
                bloke->low_level_action = 0;
                bloke->field_5c = 0;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00483e20
void BlokeActionWalkUntilPath(Bloke *bloke) {
    Point d;
    if (bloke->pos.x >= 0 && bloke->pos.x < lpConfig->width * 0x100 &&
        bloke->pos.y >= 0 && bloke->pos.y < lpConfig->height * 0x100) {
        short mapFlags = Get_MapFlags(bloke->pos.x, bloke->pos.y);
        short rf = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
        if ((rf & 1) != 0 || ((mapFlags & 0x10) != 0 && (rf & 2) == 0)) {
            bloke->low_level_action = 0;
            return;
        }
    }
    d = GetOffsetInDir(bloke->dir, bloke->speed);
    d.x += bloke->pos.x;
    d.y += bloke->pos.y;
    if (CheckPassThroughTile(bloke, d.x, d.y) == 0) {
        if (TryStopBlokeAtLinkedPath(bloke, d.x, d.y) == 0) {
            if (TurnRandomlyIfBlocked(bloke, d) == 0) {
                HandleTileEnterLeave(bloke, d.x, d.y);
                bloke->pos = d;
                AdvanceBlokeFrame(bloke);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00483ef0
void BlokeActionWalkPath(Bloke *bloke) {
    Point d = GetOffsetInDir(bloke->dir, bloke->speed);
    short rf;
    short mapFlags;
    short rf2;

    d.x += bloke->pos.x;
    d.y += bloke->pos.y;

    if (CheckPassThroughTile(bloke, d.x, d.y) != 0) {
        return;
    }
    CheckEnteredNewTile(bloke, d.x, d.y);
    if (CrossTileCentre(bloke, d.x, d.y) != 0) {
        bloke->flags |= 4;
        rf = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
        if ((bloke->pos.x < 0 || bloke->pos.x >= lpConfig->width * 0x100 || bloke->pos.y < 0 ||
                bloke->pos.y >= lpConfig->height * 0x100 ||
                (mapFlags = Get_MapFlags(bloke->pos.x, bloke->pos.y), rf2 = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y),
                    (rf2 & 1) == 0 && ((mapFlags & 0x10) == 0 || (rf2 & 2) != 0))) &&
            (Get_MapFlags(bloke->pos.x, bloke->pos.y) & 0x10) == 0) {
            bloke->low_level_action = 0;
            bloke->field_64 |= 2;
            return;
        }
        if ((rf & 0x24) != 0) {
            bloke->low_level_action = 0;
            bloke->field_64 |= 4;
            return;
        }
        if ((rf & 8) != 0) {
            int coords[2];
            unsigned char dirs;
            coords[0] = d.x >> 8;
            coords[1] = d.y >> 8;
            dirs = Get_Path_Directions((struct Point *)coords, 0, 0);
            dirs = ExcludeIsolatedDiags(dirs);
            dirs = dirs & ~Dir_To_Bit(bloke->dir + 4);
            dirs = Bit_To_Dir(dirs);
            NewDirForAction(bloke, dirs);
        }
    }
    HandleTileEnterLeave(bloke, d.x, d.y);
    bloke->pos = d;
    AdvanceBlokeFrame(bloke);
}

// FUNCTION: LEGOLAND 0x00484090
void BlokeActionWalkPathStraight(Bloke *bloke) {
    Point d = GetOffsetInDir(bloke->dir, bloke->speed);
    Point next;
    short rf;
    short mapFlags;
    short rf2;

    d.x += bloke->pos.x;
    d.y += bloke->pos.y;
    next = d;
    CheckEnteredNewTile(bloke, next.x, next.y);
    if (CrossTileCentre(bloke, next.x, next.y) != 0) {
        bloke->flags |= 4;
        rf = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
        if (bloke->pos.x < 0 || bloke->pos.x >= lpConfig->width * 0x100 || bloke->pos.y < 0 ||
            bloke->pos.y >= lpConfig->height * 0x100) {
            bloke->low_level_action = 0;
            bloke->field_64 |= 2;
            return;
        }
        mapFlags = Get_MapFlags(bloke->pos.x, bloke->pos.y);
        rf2 = GetCurrentRFFlags(bloke->pos.x, bloke->pos.y);
        if ((rf2 & 1) == 0 && ((mapFlags & 0x10) == 0 || (rf2 & 2) != 0)) {
            bloke->low_level_action = 0;
            bloke->field_64 |= 2;
            return;
        }
        if ((rf & 0x24) != 0) {
            bloke->low_level_action = 0;
            bloke->field_64 |= 4;
            return;
        }
        if ((rf & 8) != 0) {
            bloke->low_level_action = 0;
            return;
        }
    }
    HandleTileEnterLeave(bloke, next.x, next.y);
    bloke->pos = next;
    AdvanceBlokeFrame(bloke);
}

// FUNCTION: LEGOLAND 0x004841a0
int ReachedDestination(Bloke *bloke, int dist) {
    int dx = bloke->dest.x - bloke->pos.x;
    int dy = bloke->dest.y - bloke->pos.y;
    return dist * dist >= dy * dy + dx * dx;
}

// FUNCTION: LEGOLAND 0x004841e0
unsigned char IsNearDestination(Bloke *bloke) {
    int dx = bloke->dest.x - bloke->nav.x;
    int dy = bloke->dest.y - bloke->nav.y;
    return (dx * dx + dy * dy) <= 0x400;
}

// FUNCTION: LEGOLAND 0x00484220
void BlokeActionMoveToDestOnPath(Bloke *bloke) {
    Point target;
    short mapFlags;
    short rf;

    if (ReachedDestination(bloke, bloke->speed << 1) != 0) {
        bloke->low_level_action = 0;
        return;
    }
    NavigMoveLine(&bloke->nav, bloke->speed, &target);
    if (CheckEnteredNewTile(bloke, target.x, target.y) != 0) {
        if (HitObstacle(bloke, target.x, target.y) != 0) {
            bloke->low_level_action = 0;
            bloke->field_64 |= 1;
            return;
        }
        if (target.x < 0 || target.x >= lpConfig->width * 0x100 || target.y < 0 ||
            target.y >= lpConfig->height * 0x100 ||
            (mapFlags = Get_MapFlags(target.x, target.y), rf = GetCurrentRFFlags(target.x, target.y),
                (rf & 1) == 0 && ((mapFlags & 0x10) == 0 || (rf & 2) != 0))) {
            bloke->low_level_action = 0;
            bloke->field_64 |= 2;
            return;
        }
    }
    HandleTileEnterLeave(bloke, target.x, target.y);
    bloke->pos = target;
    AdvanceBlokeFrame(bloke);
}

// FUNCTION: LEGOLAND 0x00484350
void BlokeActionMoveUntilPath(Bloke *bloke) {
    Point target;
    short mapFlags;
    short rf;

    if ((bloke->flags & 2) != 0) {
        bloke->low_level_action = 0;
        return;
    }
    if (ReachedDestination(bloke, bloke->speed << 1) != 0) {
        bloke->low_level_action = 0;
        return;
    }
    NavigMoveLine(&bloke->nav, bloke->speed, &target);
    if (CheckEnteredNewTile(bloke, target.x, target.y) != 0) {
        if (HitObstacle(bloke, target.x, target.y) == 0) {
            if (target.x >= 0 && target.x < lpConfig->width * 0x100 && target.y >= 0 && target.y < lpConfig->height * 0x100) {
                mapFlags = Get_MapFlags(target.x, target.y);
                rf = GetCurrentRFFlags(target.x, target.y);
                if ((rf & 1) != 0 || ((mapFlags & 0x10) != 0 && (rf & 2) == 0)) {
                    bloke->low_level_action = 0;
                    return;
                }
            }
        } else {
            bloke->low_level_action = 0;
            return;
        }
    }
    HandleTileEnterLeave(bloke, target.x, target.y);
    bloke->pos = target;
    AdvanceBlokeFrame(bloke);
}

// FUNCTION: LEGOLAND 0x00484470
void BlokeActionMoveToDestAvoiding(Bloke *bloke) {
    Point target;
    if (ReachedDestination(bloke, bloke->speed << 1) != 0) {
        bloke->low_level_action = 0;
        return;
    }
    NavigMoveLine(&bloke->nav, bloke->speed, &target);
    if (CheckEnteredNewTile(bloke, target.x, target.y) != 0) {
        if (HitObstacle(bloke, target.x, target.y) != 0) {
            bloke->low_level_action = 0;
            bloke->field_64 |= 1;
            return;
        }
    }
    HandleTileEnterLeave(bloke, target.x, target.y);
    bloke->pos = target;
    AdvanceBlokeFrame(bloke);
}

// FUNCTION: LEGOLAND 0x00484520
void BlokeActionMoveToDestStayOnPath(Bloke *bloke) {
    Point target;
    if (ReachedDestination(bloke, bloke->speed << 1) != 0) {
        bloke->low_level_action = 0;
        return;
    }
    NavigMoveLine(&bloke->nav, bloke->speed, &target);
    if (CheckEnteredNewTile(bloke, target.x, target.y) != 0) {
        if (IsLeavingPath(bloke, target.x, target.y) != 0) {
            bloke->low_level_action = 0;
            bloke->field_64 |= 1;
            return;
        }
    }
    HandleTileEnterLeave(bloke, target.x, target.y);
    bloke->pos = target;
    AdvanceBlokeFrame(bloke);
}

// FUNCTION: LEGOLAND 0x004845d0
void BlokeActionMoveToDest(Bloke *bloke) {
    Point target;
    if (ReachedDestination(bloke, bloke->speed << 1) != 0) {
        DoPendingAction(bloke);
        return;
    }
    NavigMoveLine(&bloke->nav, bloke->speed, &target);
    bloke->pos = target;
    AdvanceBlokeFrame(bloke);
}

// FUNCTION: LEGOLAND 0x00484630
void BlokeActionBounce(Bloke *bloke) {
    if (bloke->height == 0) {
        bloke->field_46 = bloke->field_44;
        if (bloke->field_3a-- == 0) {
            DoPendingAction(bloke);
            return;
        }
    }
    bloke->height += bloke->field_46--;
    if (bloke->height <= 0) {
        bloke->height = 0;
    }
    bloke->frame = 0;
    bloke->target_dir = (bloke->dir + 1) & 7;
    TurnTowardsTargetDir(bloke);
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

/* The tile set's RF flags at (x, y): its callback's answer, or 2 when it has none. Inlined into
 * BlokeActionPassThroughTile, where the original keeps the returned byte in a stack slot. */
static __inline unsigned char TileSetRFFlags(FXSpriteList *set, int x, int y) {
    if (set->get_rf_flags != NULL) {
        return set->get_rf_flags(x, y);
    }
    return 2;
}

// FUNCTION: LEGOLAND 0x00484790
void BlokeActionPassThroughTile(Bloke *bloke) {
    Point tile;
    MapElement *elem;
    FXSpriteList *set;
    /* The original stores the tile set's byte answer and tests the whole stack dword. */
    union {
        int value;
        unsigned char flags;
    } result;
    unsigned int ux;
    unsigned int uy;
    int x;
    int y;

    tile = GetTileInDir(bloke->pos, bloke->dir);
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
    result.flags = TileSetRFFlags(set, tile.x, tile.y);
    if ((result.value & 3) > 0 && (result.value & 3) <= 2) {
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
        BlokeActionTable[bloke->low_level_action](bloke);
        return;
    }
    bloke->field_20++;
    SetBlokeFrame2(bloke);
}

// FUNCTION: LEGOLAND 0x004848e0
void BlokeActionWaitWhileHovered(Bloke *bloke) {
    if (!((Hover.type >> 8) & 0x2) || Hover.ptr != bloke) {
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
    BlokeActionTable[bloke->low_level_action](bloke);
}

// FUNCTION: LEGOLAND 0x00484950
LEGO_EXPORT void ApplyObjectOrientationToPerson(Person *person, float *matrix, void *param_3) {
    float scale = 65536.0f;
    int tmp;
    person->fm[0] = matrix[0];
    person->fm[1] = matrix[6];
    person->fm[2] = -matrix[3];
    person->fm[3] = -matrix[2];
    person->fm[4] = -matrix[8];
    person->fm[5] = matrix[5];
    person->fm[6] = -matrix[1];
    person->fm[7] = -matrix[7];
    person->fm[8] = matrix[4];
    tmp = person->m[0];
    __asm {
        fld dword ptr [tmp]
        fmul dword ptr [scale]
        fistp dword ptr [tmp]
    }
    person->m[0] = tmp;
    tmp = person->m[3];
    __asm {
        fld dword ptr [tmp]
        fmul dword ptr [scale]
        fistp dword ptr [tmp]
    }
    person->m[3] = tmp;
    tmp = person->m[6];
    __asm {
        fld dword ptr [tmp]
        fmul dword ptr [scale]
        fistp dword ptr [tmp]
    }
    person->m[6] = tmp;
    tmp = person->m[1];
    __asm {
        fld dword ptr [tmp]
        fmul dword ptr [scale]
        fistp dword ptr [tmp]
    }
    person->m[1] = tmp;
    tmp = person->m[4];
    __asm {
        fld dword ptr [tmp]
        fmul dword ptr [scale]
        fistp dword ptr [tmp]
    }
    person->m[4] = tmp;
    tmp = person->m[7];
    __asm {
        fld dword ptr [tmp]
        fmul dword ptr [scale]
        fistp dword ptr [tmp]
    }
    person->m[7] = tmp;
    tmp = person->m[2];
    __asm {
        fld dword ptr [tmp]
        fmul dword ptr [scale]
        fistp dword ptr [tmp]
    }
    person->m[2] = tmp;
    tmp = person->m[5];
    __asm {
        fld dword ptr [tmp]
        fmul dword ptr [scale]
        fistp dword ptr [tmp]
    }
    person->m[5] = tmp;
    tmp = person->m[8];
    __asm {
        fld dword ptr [tmp]
        fmul dword ptr [scale]
        fistp dword ptr [tmp]
    }
    person->m[8] = tmp;
}

// FUNCTION: LEGOLAND 0x00484a70
LEGO_EXPORT void SetBlokePositionFromBNV(BinVFile *file, Bloke *bloke, char *name, int frame, float near_z, float far_z, float *orient) {
    BinVObject *object;
    Vertex *vertex;
    float m28;
    float sumZ = 0.0f;
    int sumX = 0;
    int sumY = 0;
    BinVFrame *binFrame;
    float len;
    float scale;
    float skew;
    float zscale;
    float depth;
    register int z;
    int i;

    binFrame = GetBinVFrame(file, frame);
    object = GetObjectFromName(binFrame, name);
    len = sqrt(object->m18 * object->m18 + object->m14 * object->m14 + object->m10 * object->m10);
    scale = 1.0f / len;
    object->m10 *= scale;
    object->m14 = object->m14 * scale;
    object->m18 *= scale;
    len = sqrt(object->m24 * object->m24 + object->m20 * object->m20 + object->m1c * object->m1c);
    scale = 1.0f / len;
    object->m1c *= scale;
    object->m20 *= scale;
    object->m24 *= scale;
    m28 = object->m28;
    len = sqrt(object->m2c * object->m2c + m28 * object->m28 + object->m30 * object->m30);
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
    path->z_scale = scale;
    path->frame_index = 0;
    path->z_origin = param_5;
    path->field_44 = 1;
    path->x = coords[0];
    path->y = coords[1];
    frame = GetBinVFrame(file, 0);
    object = GetObjectFromName(frame, name);
    vertex = GetVertex(object, 0);
    path->z_skew = GetZSkew(file, object, vertex);
    path->field_4 = param_2;
    return path;
}

// FUNCTION: LEGOLAND 0x00484cd0
LEGO_EXPORT int UpdateBlokeFromBNVPath(Bloke *bloke, struct BNVPath *path) {
    Person *person = bloke->person;
    float sumX = 0.0f;
    float sumY = 0.0f;
    float sumZ = 0.0f;
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
    float depth;
    int i;

    if (frame == path->file->frameCount) {
        return 0;
    }
    binFrame = GetBinVFrame(path->file, frame);
    object = GetObjectFromName(binFrame, path->name);
    for (i = 0; i < 8; i++) {
        vertex = GetVertex(object, i);
        sumX += vertex->x;
        sumY += vertex->y;
        sumZ += vertex->z;
    }
    dx = sumX * 0.125 - path->x;
    dy = sumY * 0.125 - path->y;
    if (path->field_44 == 0) {
        path->x = path->dx + path->x;
        path->y = path->dy + path->y;
        speed = bloke->speed;
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
            sumX += vertex->x;
            sumY += vertex->y;
            sumZ += vertex->z;
        }
        dx = sumX * 0.125 - path->x;
        dy = sumY * 0.125 - path->y;
        depth = (sumZ * 0.125 - path->z_origin) * path->z_scale;
        z = depth + 8192.0f;
        bloke->person->field_34 = z >> 8;
        angle = atan2(dy, dx);
        path->dx = bloke->speed * cos(angle) * 0.25;
        path->dy = bloke->speed * sin(angle) * 0.25;
    }
    person->field_38 = path->z_skew + path->z_skew;
    bloke->screen_x = path->x * 0.5f;
    bloke->screen_y = path->y * 0.5f;
    AdvanceBlokeFrame(bloke);
    return 1;
}

// FUNCTION: LEGOLAND 0x00484ff0
LEGO_EXPORT int BNVPath_GetDFrame(BNVPath *path) {
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
    BinVFrame *binFrame;
    BinVObject *object;
    Vertex *vertex;
    int i;
    int z;
    double angle;

    path->frame_index = frame;
    {
        float sumX = 0.0f;
        float sumY = 0.0f;
        float sumZ = 0.0f;

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
    }
    {
        float sumX = 0.0f;
        float sumY = 0.0f;
        float sumZ = 0.0f;
        double dx;
        double dy;
        float depth;

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
        depth = (sumZ * 0.125 - path->z_origin) * path->z_scale;
        z = depth + 8192.0f;
        bloke->person->field_34 = z >> 8;
        angle = atan2(dy, dx);
    }
    path->dx = bloke->speed * cos(angle) * 0.25;
    path->dy = bloke->speed * sin(angle) * 0.25;
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
    MarkTilesUnderWorkers(rect);
    for (y = rect->y0; y <= rect->y1; y++) {
        for (x = rect->x0; x <= rect->x1; x++) {
            if ((GameMap[y][x].flags & 0x1000) != 0) {
                return -1;
            }
        }
    }
    return 0;
}
