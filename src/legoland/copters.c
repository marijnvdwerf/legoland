#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "copters.h"
#include "gamemap.h"
#include "globals.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "objclass.h"
#include "render3d.h"
#include "ride_queue.h"
#include "sound_music.h"
#include "sound_sfx.h"

struct CopterSource {
    unsigned short field_0;
};

struct CopterItem {
    unsigned char pad_0[8];
    struct CopterSub *field_8;
};

struct CopterSub {
    unsigned char pad_0[0x50];
    int field_50;
};

struct CopterModel {
    unsigned char pad_0[0x10];
    unsigned int field_10;
};

struct CopterRide {
    unsigned char pad_0[0x14];
    int field_14;
    int field_18;
    unsigned int field_1c;
    unsigned char pad_20[0x64 - 0x20];
    struct CopterModel *field_64;
};

struct CopterEditObject {
    unsigned char pad_0[0xc];
    struct CopterRide *field_c;
};

// Same object as CopterNode, viewed with the two id bytes split apart.
struct CopterSfxNode {
    /* 0x00 */ unsigned char field_0;
    /* 0x01 */ unsigned char field_1;
    /* 0x02 */ unsigned char field_2;
    /* 0x03 */ unsigned char field_3;
    /* 0x04 */ struct CopterNode *next;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ int field_c;
    /* 0x10 */ unsigned char field_10;
    /* 0x11 */ unsigned char pad_11[3];
    /* 0x14 */ int field_14;
    /* 0x18 */ struct CopterLayer layer[6];
};

// SFX list table (Helicopter_SFX); Load_FXList fills the sample-def slots.
struct CopterFXList {
    /* 0x00 */ unsigned char pad_0[8];
    /* 0x08 */ void *field_8;
    /* 0x0c */ unsigned char pad_c[0x14 - 0xc];
    /* 0x14 */ void *field_14;
};

struct CopterChainNode {
    /* 0x00 */ struct CopterChainNode *next;
};

struct CopterChainRide {
    /* 0x00 */ unsigned char pad_0[0xcc];
    /* 0xcc */ struct CopterChainNode *chain;
};

// One saved layer record (stride 0x20); field_0 holds a 1-based chain index.
struct CopterSaveLayer {
    /* 0x00 */ int field_0;
    /* 0x04 */ unsigned char pad_4[0x20 - 4];
};

// Save-record view of CopterNode used by Copters_Load.
struct CopterLoadNode {
    /* 0x00 */ unsigned char pad_0[4];
    /* 0x04 */ struct CopterLoadNode *next;
    /* 0x08 */ unsigned char pad_8[0x30 - 8];
    /* 0x30 */ struct CopterSaveLayer slots[6];
};

#include "image_sprite.h"

// GLOBAL: LEGOLAND 0x004b4198
static struct PathPair Copters_PathPairs0[6] = {{0, -1}, {1, 0}, {1, 0}, {1, 0}, {-1, -1}, {-1, -1}};
// GLOBAL: LEGOLAND 0x004b41c8
struct PathTable DAT_004b41c8 = {6, Copters_PathPairs0};
// GLOBAL: LEGOLAND 0x004b41d0
static struct PathPair Copters_PathPairs1[7] = {{0, -1}, {-1, 0}, {0, -1}, {0, -4}, {-3, -1}, {1, 0}, {1, 0}};
// GLOBAL: LEGOLAND 0x004b4208
struct PathTable DAT_004b4208 = {7, Copters_PathPairs1};
// GLOBAL: LEGOLAND 0x004b4210
static struct PathPair Copters_PathPairs2[6] = {{0, -1}, {-1, -1}, {2, -4}, {2, -2}, {-2, 0}, {0, -1}};
// GLOBAL: LEGOLAND 0x004b4240
struct PathTable DAT_004b4240 = {6, Copters_PathPairs2};
// GLOBAL: LEGOLAND 0x004b4248
static struct PathPair Copters_PathPairs3[5] = {{0, -1}, {4, -1}, {0, -1}, {-1, -1}, {-1, -2}};
// GLOBAL: LEGOLAND 0x004b4270
struct PathTable DAT_004b4270 = {5, Copters_PathPairs3};
// GLOBAL: LEGOLAND 0x004b4278
static struct PathPair Copters_PathPairs4[4] = {{0, -1}, {-1, -1}, {0, -1}, {0, -1}};
// GLOBAL: LEGOLAND 0x004b4298
struct PathTable DAT_004b4298 = {4, Copters_PathPairs4};

// GLOBAL: LEGOLAND 0x004b4170
static const char *Copters_LLSNames[10] = {
    "mcop_gs.lls",
    "mcop_b2s.lls",
    "mcop_rs.lls",
    "mcop_b1s.lls",
    "mcop_ys.lls",
    "mcop_b1m.lls",
    "mcop_gm.lls",
    "mcop_rm.lls",
    "mcop_ym.lls",
    "mcop_b2m.lls",
};

// FUNCTION: LEGOLAND 0x00403c40
void FUN_00403c40(struct CopterSource *src) {
    struct CopterNode *node = (struct CopterNode *)malloc(sizeof(struct CopterNode));
    if (node != NULL) {
        memset(node, 0, sizeof(struct CopterNode));
        node->field_0 = src->field_0;
        node->next = DAT_004c11b4;
        DAT_004c11b4 = node;
    }
    FUN_00403e90(node);
}

// FUNCTION: LEGOLAND 0x00403c80
void FUN_00403c80(struct CopterNode *node) {
    struct CopterNode *prev;
    struct CopterNode *cur;

    if (DAT_004c11b4 == node) {
        DAT_004c11b4 = node->next;
        free(node);
        return;
    }
    cur = DAT_004c11b4->next;
    prev = DAT_004c11b4;
    while (cur != node) {
        prev = prev->next;
        if (prev == NULL) {
            goto done;
        }
        cur = prev->next;
    }
    if (prev != NULL) {
        prev->next = node->next;
    }
done:;
    free(node);
}

// FUNCTION: LEGOLAND 0x00403ce0
void FUN_00403ce0(void) {
    while (DAT_004c11b4 != NULL) {
        FUN_00403c80(DAT_004c11b4);
    }
}

// FUNCTION: LEGOLAND 0x00403d00
struct CopterNode *FUN_00403d00(struct CopterSource *src) {
    struct CopterNode *node;

    if (DAT_004c11b4 != NULL) {
        node = DAT_004c11b4;
        if (DAT_004c11b4->field_0 == src->field_0) {
            return DAT_004c11b4;
        }
        while (1) {
            node = node->next;
            if (node == NULL) {
                break;
            }
            if (node->field_0 == src->field_0) {
                return node;
            }
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00403d30
int FUN_00403d30(struct CopterItem *item) {
    struct CopterSub *sub = item->field_8;
    int *ptr = &DAT_004c1124[0];
    int index = 0;

    while (1) {
        if (sub->field_50 == *ptr) {
            break;
        }
        ptr++;
        index++;
        if ((int)ptr < (int)&DAT_004c113c) {
            continue;
        }
        index = -1;
        break;
    }

    sub->field_50 = index;
    return index;
}

// FUNCTION: LEGOLAND 0x00403d60
void FUN_00403d60(struct CopterItem *item) {
    struct CopterSub *sub = item->field_8;
    int index = sub->field_50;

    if (index < 0 || index >= 6) {
        sub->field_50 = 0;
    } else {
        sub->field_50 = DAT_004c1124[index];
    }
}

// FUNCTION: LEGOLAND 0x00403d90
void FUN_00403d90(struct CopterEditObject *param_1) {
    struct CopterRide *ride;
    int i;

    ride = param_1->field_c;
    DAT_004c1198 = ride;
    ride->field_1c |= 0x420;
    DAT_004c1138 = ((struct CopterRide *)DAT_004c1198)->field_64;
    ((struct CopterModel *)DAT_004c1138)->field_10 |= 0x2000;
    for (i = 0; i < 10; i++) {
        DAT_004c113c[i] = LoadSprite(Copters_LLSNames[i], 1);
    }
    // STRING: LEGOLAND 0x004b43d0
    DAT_00830f98 = LoadPos("3ddata\\copters.pos");
    DAT_004c1124[0] = FUN_00412100(&DAT_004b41c8);
    DAT_004c1124[2] = FUN_00412100(&DAT_004b4208);
    DAT_004c1124[1] = FUN_00412100(&DAT_004b4240);
    DAT_004c1124[3] = FUN_00412100(&DAT_004b4270);
    DAT_004c1124[4] = FUN_00412100(&DAT_004b4298);
    DAT_004c1194 = &DAT_004c119c;
    DAT_004c1190 = &DAT_004c11a0;
    DAT_004c1164 = &DAT_004c11a4;
    DAT_004c1168 = &DAT_004c11a8;
    DAT_004c1188 = &DAT_004c11ac;
    // STRING: LEGOLAND 0x004b43bc
    DAT_004c1120 = LoadSprite("cop_base Matte.lls", 1);
    Load_FXList(Helicopter_SFX, 4);
}

// FUNCTION: LEGOLAND 0x00403e90
void FUN_00403e90(struct CopterNode *node) {
    unsigned int handle;
    struct LLS *lls;
    char frame;

    node->layer[1].field_8 = 10;
    node->layer[1].field_c = 3;
    node->layer[1].field_10 = 2;
    node->layer[1].field_14 = 7;
    node->layer[1].flags = 0;
    handle = GetSpriteForLayer((struct LayerContainer *)DAT_004c1138, 3);
    if (handle != 0) {
        lls = (struct LLS *)GetLLSForSprite((struct SpriteLLS *)handle);
        if (lls != NULL) {
            frame = (char)lls->frame_count;
            node->layer[1].field_1c = frame;
            node->layer[1].field_4 = frame - 1;
        }
    }
    node->layer[0].field_8 = 2;
    node->layer[0].field_c = 1;
    node->layer[0].field_10 = 0;
    node->layer[0].field_14 = 6;
    node->layer[0].flags = 0;
    handle = GetSpriteForLayer((struct LayerContainer *)DAT_004c1138, 1);
    if (handle != 0) {
        lls = (struct LLS *)GetLLSForSprite((struct SpriteLLS *)handle);
        if (lls != NULL) {
            frame = (char)lls->frame_count;
            node->layer[0].field_1c = frame;
            node->layer[0].field_4 = frame - 1;
        }
    }
    node->layer[2].field_c = 0xb;
    node->layer[2].field_8 = 4;
    node->layer[2].field_10 = 4;
    node->layer[2].field_14 = 8;
    node->layer[2].flags = 0;
    handle = GetSpriteForLayer((struct LayerContainer *)DAT_004c1138, 0xb);
    if (handle != 0) {
        lls = (struct LLS *)GetLLSForSprite((struct SpriteLLS *)handle);
        if (lls != NULL) {
            frame = (char)lls->frame_count;
            node->layer[2].field_1c = frame;
            node->layer[2].field_4 = frame - 1;
        }
    }
    node->layer[3].field_c = 6;
    node->layer[3].field_8 = 5;
    node->layer[3].field_10 = 3;
    node->layer[3].field_14 = 5;
    node->layer[3].flags = 0;
    handle = GetSpriteForLayer((struct LayerContainer *)DAT_004c1138, 6);
    if (handle != 0) {
        lls = (struct LLS *)GetLLSForSprite((struct SpriteLLS *)handle);
        if (lls != NULL) {
            frame = (char)lls->frame_count;
            node->layer[3].field_1c = frame;
            node->layer[3].field_4 = frame - 1;
        }
    }
    node->layer[4].field_8 = 8;
    node->layer[4].field_c = 7;
    node->layer[4].field_10 = 1;
    node->layer[4].field_14 = 9;
    node->layer[4].flags = 0;
    handle = GetSpriteForLayer((struct LayerContainer *)DAT_004c1138, 7);
    if (handle != 0) {
        lls = (struct LLS *)GetLLSForSprite((struct SpriteLLS *)handle);
        if (lls != NULL) {
            frame = (char)lls->frame_count;
            node->layer[4].field_1c = frame;
            node->layer[4].field_4 = frame - 1;
        }
    }
    FUN_004049a0(node, 1);
}

// FUNCTION: LEGOLAND 0x00404040
void FUN_00404040(void) {
    struct Sprite **current;

    if (DAT_004c1120 != NULL) {
        KillSprite(DAT_004c1120);
    }

    current = DAT_004c113c;
    while ((int)current < (int)&DAT_004c1164) {
        if (*current != NULL) {
            KillSprite(*current);
        }
        current++;
    }

    if (DAT_004c1124[0] != 0) {
        FUN_00412290((struct Sprite *)DAT_004c1124[0]);
    }
    if (DAT_004c1124[1] != 0) {
        FUN_00412290((struct Sprite *)DAT_004c1124[1]);
    }
    if (DAT_004c1124[3] != 0) {
        FUN_00412290((struct Sprite *)DAT_004c1124[3]);
    }
    if (DAT_004c1124[4] != 0) {
        FUN_00412290((struct Sprite *)DAT_004c1124[4]);
    }
    if (DAT_004c1124[2] != 0) {
        FUN_00412290((struct Sprite *)DAT_004c1124[2]);
    }

    FUN_00403ce0();
    Kill_FXList(Helicopter_SFX, 4);
    UnloadPos(DAT_00830f98);
}

// FUNCTION: LEGOLAND 0x004040f0
void FUN_004040f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404290
void FUN_00404290(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404450
void FUN_00404450(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = DAT_004c1198;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((unsigned char *)EditMode.unk8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00404490
unsigned int *FUN_00404490(struct CopterEditObject *editobj, unsigned short uid) {
    struct CopterRide *ride = editobj->field_c;

    DAT_004c1170 = (int)ride->field_64;
    DAT_004c1174 = ride->field_14;
    DAT_004c1178 = ride->field_18;
    DAT_004c117c = uid;
    ride->field_64->field_10 |= 0x2000;

    if (FUN_00403d00((struct CopterSource *)&uid) != 0) {
        HideLayer(DAT_004c1138, 1);
        HideLayer(DAT_004c1138, 2);
        HideLayer(DAT_004c1138, 3);
        HideLayer(DAT_004c1138, 0xa);
        HideLayer(DAT_004c1138, 6);
        HideLayer(DAT_004c1138, 5);
        HideLayer(DAT_004c1138, 7);
        HideLayer(DAT_004c1138, 8);
        HideLayer(DAT_004c1138, 0xb);
        HideLayer(DAT_004c1138, 4);
    }

    return (unsigned int *)&DAT_004c1170;
}

// FUNCTION: LEGOLAND 0x00404580
void FUN_00404580(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404600
void FUN_00404600(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404630
void FUN_00404630(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404860
void FUN_00404860(struct CopterNode *node, int index) {
    struct CopterLayer *layer = &node->layer[index];

    if (layer->flags & 1) {
        layer->field_4 = layer->field_4 + 1;
        if (layer->field_4 < layer->field_1c) {
            return;
        }
        layer->field_4 = 0;
        layer->field_1d = layer->field_1d - 1;
        if (layer->field_1d >= 0) {
            return;
        }
        layer->flags &= 0xfffffffe;
    }
}

// FUNCTION: LEGOLAND 0x004048a0
unsigned int FUN_004048a0(unsigned int param) {
    ResumeSinglyPausedSample((struct Sample *)param);
    return 0;
}

// FUNCTION: LEGOLAND 0x004048b0
void FUN_004048b0(struct CopterSfxNode *node) {
    struct CopterFXList *fx = (struct CopterFXList *)Helicopter_SFX;
    struct SampleParams params;
    struct Sample *sample;

    if (node->layer[1].field_18 != 0) node->layer[1].flags |= 1;
    if (node->layer[0].field_18 != 0) node->layer[0].flags |= 1;
    if (node->layer[2].field_18 != 0) node->layer[2].flags |= 1;
    if (node->layer[3].field_18 != 0) node->layer[3].flags |= 1;
    if (node->layer[4].field_18 != 0) node->layer[4].flags |= 1;

    node->field_3 = node->field_2;
    node->field_2 = 0;
    node->field_8 = (node->field_8 & ~0x4000u) | 1;
    node->field_c = 2;
    node->layer[1].field_1d = 3;
    node->layer[0].field_1d = 3;
    node->layer[2].field_1d = 3;
    node->layer[3].field_1d = 3;
    node->layer[4].field_1d = 3;
    node->layer[1].field_4 = 0;
    node->layer[0].field_4 = 0;
    node->layer[2].field_4 = 0;
    node->layer[3].field_4 = 0;
    node->layer[4].field_4 = 0;

    params.field_8 = node->field_0;
    params.field_c = node->field_1;
    params.field_0 = 2;

    PlayInstanceOfSample(fx->field_8, 0, 1, &params);
    sample = PlayInstanceOfSample(fx->field_14, 1, 1, &params);
    PauseSingleSample(sample);
    AddSFX_Callback((struct CallbackEntry *)sample, 0xb54,
        (unsigned int (*)(struct CallbackEntry *))FUN_004048a0);
}

// FUNCTION: LEGOLAND 0x004049a0
void FUN_004049a0(struct CopterNode *node, int param) { STUB(); }

// FUNCTION: LEGOLAND 0x00404a90
void FUN_00404a90(struct CopterNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00404bc0
void FUN_00404bc0(void) {
    struct CopterNode *node = DAT_004c11b4;
    if (node == NULL) {
        return;
    }
    do {
        FUN_00404a90(node);
        node = node->next;
    } while (node != NULL);
}

// FUNCTION: LEGOLAND 0x00404be0
void FUN_00404be0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404f20
void FUN_00404f20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404f60
LEGO_EXPORT int Copters_Save(void) {
    unsigned int *original;
    unsigned int *cursor;
    int i;
    int *saved;
    int *restore;
    int *field;
    int index;
    char *node;
    unsigned int one;
    unsigned int zero;
    int saved_vals[6];

    one = 1;
    zero = 0;
    node = (char *)DAT_004c11b4;
    while (node != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        saved = saved_vals;
        i = 6;
        restore = (int *)(node + 0x30);
        field = restore;
        do {
            original = (unsigned int *)*field;
            index = 0;
            *saved = (int)original;
            for (cursor = *(unsigned int **)((char *)DAT_004c1198 + 0xcc); cursor != NULL; cursor = (unsigned int *)*cursor) {
                if (cursor == original) {
                    if (cursor != NULL) {
                        *field = index + 1;
                        goto next;
                    }
                    break;
                }
                index = index + 1;
            }
            *field = 0;
        next:
            saved++;
            field += 8;
            i--;
        } while (i != 0);
        if (SaveGameWrite(node, 0xd8) == 0) {
            return 0;
        }
        saved = saved_vals;
        i = 6;
        do {
            *restore = *saved;
            saved++;
            restore += 8;
            i--;
        } while (i != 0);
        node = *(char **)(node + 4);
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00405050
LEGO_EXPORT int Copters_Load(void) {
    int count;
    struct CopterLoadNode *prev = NULL;
    struct CopterLoadNode *node;
    struct CopterChainNode *chain;
    int n;
    int i;

    if (!SaveGameRead(&count, 4)) {
        return 0;
    }

    while (count != 0) {
        node = (struct CopterLoadNode *)malloc(0xd8);
        if (!SaveGameRead(node, 0xd8)) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            DAT_004c11b4 = (struct CopterNode *)node;
        }
        prev = node;

        for (i = 6; i != 0; i--) {
            n = node->slots[6 - i].field_0;
            chain = ((struct CopterChainRide *)DAT_004c1198)->chain;
            if (n != 0) {
                while (--n != 0) {
                    chain = chain->next;
                }
                node->slots[6 - i].field_0 = (int)chain;
            } else {
                node->slots[6 - i].field_0 = 0;
            }
        }

        if (!SaveGameRead(&count, 4)) {
            return 0;
        }
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x00405110
void FUN_00405110(struct ClassNode *name, struct CallbackTable *interfaces) {
    // STRING: LEGOLAND 0x004b43e4
    if (_stricmp("COPTERS", name->name) == 0) {
        interfaces->cb_a4 = FUN_00403d90;
        interfaces->cb_8c = FUN_00404450;
        interfaces->cb_98 = FUN_00404600;
        interfaces->cb_9c = FUN_00404580;
        interfaces->cb_a8 = FUN_00404be0;
        interfaces->cb_a0 = FUN_00404490;
        interfaces->cb_b0 = FUN_00404290;
        interfaces->cb_ac = FUN_00404040;
        interfaces->cb_bc = Copters_Save;
        interfaces->cb_b8 = Copters_Load;
    }
}
