#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "bloke.h"
#include "debug_alloc.h"
#include "globals.h"
#include "man3d.h"
#include "objclass.h"
#include "pathfind.h"
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
    unsigned char pad_0[0x68];
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
LEGO_EXPORT void GetVisitorName(void) { STUB(); }

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
void FUN_00482cb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482d30
void FUN_00482d30(void) { STUB(); }

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
void FUN_00482df0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482e50
void FUN_00482e50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482ec0
void FUN_00482ec0(void) {
    if (DAT_0066b57c != NULL) {
        free(DAT_0066b57c);
    }
    FirstBloke = NULL;
    DAT_0066b57c = NULL;
}

// FUNCTION: LEGOLAND 0x00482ef0
LEGO_EXPORT struct Bloke *NewBloke(void) { STUB(); }

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
LEGO_EXPORT unsigned int GetBlokeNum(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482fe0
LEGO_EXPORT struct Bloke *GetBlokePtr(int index) {
    if (index == -1) {
        return NULL;
    }
    return &((struct Bloke *)DAT_0066b57c)[index];
}

// FUNCTION: LEGOLAND 0x00483010
LEGO_EXPORT void DestroyBloke(struct Bloke *bloke) { STUB(); }

// FUNCTION: LEGOLAND 0x00483090
void FUN_00483090(void) {
    while (FirstBloke != NULL) {
        DestroyBloke(FirstBloke);
    }
    DAT_006661bc = 0;
}

// FUNCTION: LEGOLAND 0x004830c0
LEGO_EXPORT struct Bloke *MakeBloke(int param_1) {
    struct Bloke *bloke = NewBloke();
    if (bloke != NULL) {
        ClearBlokeCounters(GetBlokeNum(), bloke);
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
void FUN_004831a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004831d0
LEGO_EXPORT void SetPathFlag(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483240
LEGO_EXPORT unsigned short DoPendingAction(struct PendingObject *obj) {
    obj->field_e = obj->field_10;
    obj->field_10 = 0;
    return obj->field_e;
}

// FUNCTION: LEGOLAND 0x00483260
void FUN_00483260(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483300
void FUN_00483300(void) { STUB(); }

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
LEGO_EXPORT void Random_Dir_From_Bits(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004834a0
LEGO_EXPORT void HitPathEdge(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483510
LEGO_EXPORT void HitObstacle(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483580
void FUN_00483580(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483650
LEGO_EXPORT int OverNewTile(struct OverTile *tile, unsigned int x, unsigned int y) {
    if (((tile->field_68 ^ x) & 0xffffff00) || ((tile->field_6c ^ y) & 0xffffff00)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00483680
void FUN_00483680(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004837a0
int FUN_004837a0(struct Walker *walker, unsigned int x, unsigned int y) {
    if (OverNewTile((struct OverTile *)walker, x, y) != 0) {
        walker->field_62 &= 0xfffb;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004837d0
LEGO_EXPORT void CrossTileCentre(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483830
void FUN_00483830(struct Walker *walker) {
    walker->field_75 = walker->field_75 - 1;
    if (walker->field_75 == 0) {
        walker->field_75 = 2;
        walker->field_74 = (walker->field_74 + 1) & 7;
    }
}

// FUNCTION: LEGOLAND 0x00483850
void FUN_00483850(struct Walker *walker) { STUB(); }

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
LEGO_EXPORT void DoRndWalkPathTileAction(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00483b10
LEGO_EXPORT void Handle_RndWalk_TileSpecifics(void) { STUB(); }

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
LEGO_EXPORT void GetTileInDir(void) { STUB(); }

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
LEGO_EXPORT void CheckForPeople(void) { STUB(); }
