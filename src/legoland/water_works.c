#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "debug_alloc.h"
#include "draw.h"
#include "gamemap.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"
#include "sound_music.h"
#include "tilemap.h"

struct WaterNode {
    /* 0x00 */ struct WaterNode *next;
    /* 0x04 */ unsigned short key;
    /* 0x06 */ unsigned char pad_6[2];
    /* 0x08 */ unsigned char field_8;
    /* 0x09 */ unsigned char field_9;
    /* 0x0a */ unsigned char field_a;
    /* 0x0b */ unsigned char pad_b[1];
};

struct FxSource {
    unsigned int field_0;
    unsigned char pad_4[4];
    unsigned int field_8;
    unsigned int field_c;
};

struct WaterContext {
    /* 0x00 */ unsigned char pad_0[0x14];
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned int field_18;
    /* 0x1c */ unsigned int field_1c;
    /* 0x20 */ unsigned char pad_20[0x3c - 0x20];
    /* 0x3c */ unsigned int field_3c;
    /* 0x40 */ unsigned int field_40;
    /* 0x44 */ unsigned int field_44;
    /* 0x48 */ unsigned int field_48;
    /* 0x4c */ unsigned char pad_4c[0x64 - 0x4c];
    /* 0x64 */ struct WaterSub *field_64;
};

struct WaterSub {
    unsigned char pad_0[0x10];
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x64 - 0x1c];
    unsigned int field_64;
};

struct WaterArg {
    unsigned char pad_0[0xc];
    struct WaterContext *field_c;
};

struct CursorData {
    unsigned char pad_0[0x3c];
    unsigned int field_3c;
};

struct CursorNode {
    unsigned char pad_0[0xc];
    struct CursorData *field_c;
};

struct WaterListItem {
    struct WaterListItem *next;
    unsigned char pad_4[4];
    unsigned char pad_8[4];
};

struct WaterPoint {
    struct WaterPoint *next;
    unsigned char pad_4[0x68 - 0x4];
    int x;
    int y;
};

struct WaterRect {
    int x0;
    int y0;
    int x1;
    int y1;
};

struct RenderNode {
    struct RenderNode *next;
    unsigned char pad_4[4];
    unsigned int fn;
    unsigned short id;
    unsigned char pad_e[2];
};

struct WaterRender {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ void *field_c;
    /* 0x10 */ unsigned char pad_10[0xcc - 0x10];
    /* 0xcc */ struct RenderNode *nodes;
};

struct AdjustOffset {
    int x;
    int y;
};

struct WaterListNode {
    struct WaterListNode *next;
    unsigned char pad_4[4];
    unsigned char field_8;
    unsigned char field_9;
};

#include "image_sprite.h"
#include "power.h"

// FUNCTION: LEGOLAND 0x00417a90
unsigned int FUN_00417a90(void) {
    if (DAT_004cc028++ == 0) {
        Load_FXList(WATERWORKS_SFX, 3);
    }
}

// FUNCTION: LEGOLAND 0x00417ac0
void FUN_00417ac0(void) {
    DAT_004cc028--;
    if (DAT_004cc028 == 0) {
        Kill_FXList(WATERWORKS_SFX, 3);
    }
}

// FUNCTION: LEGOLAND 0x00417ae0
void thunk_FUN_00417ac0(void) { FUN_00417ac0(); }

// FUNCTION: LEGOLAND 0x00417af0
void FUN_00417af0(unsigned char *src, int index, unsigned int looping) {
    struct SampleParams params;

    params.field_0 = 2;
    params.field_8 = src[0];
    params.field_c = src[1];
    PlayInstanceOfSample(*(void **)(WATERWORKS_SFX + 8 + index * 0xc), looping, 1, &params);
}

// FUNCTION: LEGOLAND 0x00417b40
void FUN_00417b40(unsigned char *src) {
    struct FxSource source;

    source.field_0 = 2;
    source.field_8 = src[0];
    source.field_c = src[1];
    UnSourceAndFadeAllSamplesFromSource(&source, -400);
}

// FUNCTION: LEGOLAND 0x00417b80
void FUN_00417b80(struct WaterNode **head, struct WaterNode *node) {
    if (node != NULL) {
        node->next = *head;
        *head = node;
    }
}

// FUNCTION: LEGOLAND 0x00417ba0
struct WaterNode *FUN_00417ba0(struct WaterNode *list, short *key) {
    if (list != NULL) {
        if ((short)list->key == *key) {
            return list;
        }
        while ((list = list->next) != NULL) {
            if ((short)list->key == *key) {
                return list;
            }
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00417bd0
void FUN_00417bd0(struct WaterNode **head, struct WaterNode *node) {
    struct WaterNode *cur;
    struct WaterNode *next;

    cur = *head;
    if (cur == node) {
        *head = node->next;
        return;
    }
    next = cur->next;
    while (1) {
        if (next == node) {
            if (cur != NULL) {
                cur->next = node->next;
            }
            return;
        }
        if (next == NULL) {
            return;
        }
        cur = next;
        next = cur->next;
    }
}

// FUNCTION: LEGOLAND 0x00417c00
unsigned int FUN_00417c00(struct WaterArg *arg) {
    DAT_004cbfe4 = (unsigned int)arg;
    DAT_004cc01c = (unsigned int)arg->field_c;
    return FUN_00417a90();
}

// FUNCTION: LEGOLAND 0x00417c20
void FUN_00417c20(struct EditObject *editObj, int *coords) {
    struct SampleParams params;

    AddBasicObject(editObj, coords);
    params.field_8 = coords[0];
    params.field_0 = 2;
    params.field_c = coords[1];
    PlayInstanceOfSample(*(void **)(WATERWORKS_SFX + 0x14), 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x00417c70
void FUN_00417c70(unsigned int a, unsigned int b, unsigned int c) {
    RemoveSoundObject(a, b, c);
}

// FUNCTION: LEGOLAND 0x00417c90
unsigned int FUN_00417c90(void) {
    // DAT_004cbfe4 is an int-typed global that stores an object pointer (set to a
    // WaterArg*); pass it as the match vtable. Cast justified: the global is genuinely
    // used as both an int and various struct pointers, so it stays unsigned int.
    return GetFirstObjectMatching((struct RenderObjectVtable *)DAT_004cbfe4) != 0;
}

// FUNCTION: LEGOLAND 0x00417cb0
void FUN_00417cb0(unsigned short key) {
    struct WaterNode *node;

    node = (struct WaterNode *)malloc(sizeof(struct WaterNode));
    if (node != NULL) {
        ((unsigned int *)node)[0] = 0;
        ((unsigned int *)node)[1] = 0;
        ((unsigned int *)node)[2] = 0;
        node->key = key;
        FUN_00417b80(&DAT_004cc02c, node);
    }
}

// FUNCTION: LEGOLAND 0x00417cf0
void FUN_00417cf0(struct WaterNode *node) {
    FUN_00417bd0(&DAT_004cc02c, node);
    free(node);
}

// FUNCTION: LEGOLAND 0x00417d10
struct WaterNode *FUN_00417d10(unsigned short *key) {
    return FUN_00417ba0(DAT_004cc02c, key);
}

// FUNCTION: LEGOLAND 0x00417d30
void FUN_00417d30(struct WaterArg *arg) {
    FUN_00417a90();
    DAT_004cc008 = arg->field_c;
    DAT_004cc008->field_1c |= 0x420;
    DAT_004cc008->field_3c = 0;
    DAT_004cc008->field_44 = 0;
    DAT_004cc008->field_40 = 0;
    DAT_004cc008->field_48 = 0;
    DAT_004cc008->field_14 = 0;
    DAT_004cc008->field_18 = 0;
    // STRING: LEGOLAND 0x004b501c
    if (LLIDB_FindElement("WATER WORKS IMAGE LIST", &DAT_004cbfe8, 0) == 0) {
        DAT_004cc018 = LLIDB_LoadData((void *)DAT_004cbfe8);
    }
    // STRING: LEGOLAND 0x004b500c
    DAT_004cbfd8 = LoadSprite("topwater.lls", 1);
}

// FUNCTION: LEGOLAND 0x00417dd0
void FUN_00417dd0(struct WaterArg *arg, unsigned int a, unsigned int b) {
    struct WaterContext *context;

    context = arg->field_c;
    *(struct Footprint *)EditCursor.field_1414 = *(struct Footprint *)&context->field_3c;
    EditCursor.field_1830 = 0;
    ScreenToMapRef(a, &EditCursor.field_1404, b);
    ValidateCursor(&EditCursor, (unsigned int)context);
    if (FUN_00417c90() != 0) {
        return;
    }
    FUN_0045f480(&EditCursor, 0xc);
    FUN_0045f4d0(&EditCursor);
}

// FUNCTION: LEGOLAND 0x00417e40
void FUN_00417e40(void) {
    if (DAT_004cbfd8 != 0) {
        KillSprite(DAT_004cbfd8);
    }
    LLIDB_UnLoadData(DAT_004cbfe8);
    FUN_00417ac0();
}

// FUNCTION: LEGOLAND 0x00417e70
int FUN_00417e70(struct WaterPoint *list, struct WaterRect *rect) {
    struct WaterPoint *current;

    current = list;
    while (current != NULL) {
        if ((current->x >> 8) >= rect->x0 && (current->x >> 8) <= rect->x1) {
            if ((current->y >> 8) >= rect->y0 && (current->y >> 8) <= rect->y1) {
                return 1;
            }
        }
        current = current->next;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00417ec0
unsigned int FUN_00417ec0(unsigned char *point) {
    struct WaterRect rect;

    rect.x0 = point[0];
    rect.x1 = point[0];
    rect.y0 = point[1];
    rect.y1 = point[1];
    if (FUN_00417e70(FirstBloke, &rect) != 0) {
        return 1;
    }
    if (FUN_00417e70(GardenerList, &rect) != 0) {
        return 1;
    }
    return FUN_00417e70(MechanicList, &rect) != 0;
}

// FUNCTION: LEGOLAND 0x00417f40
void FUN_00417f40(void) {
    struct WaterNode *node;

    node = (struct WaterNode *)DAT_004cc02c;
    while (node != NULL) {
        if (FUN_00417ec0((unsigned char *)&node->key) != 0) {
            if (node->field_8 == 0) {
                node->field_8 = 1;
                node->field_9 = 0;
                FUN_00417af0((unsigned char *)&node->key, 0, 0);
            }
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00417f90
void FUN_00417f90(struct WaterArg *arg) {
    struct WaterNode *node;
    struct WaterRender *render;
    struct RenderNode *rnode;
    struct RenderNode *next;
    unsigned char *bloke;
    int lls;
    int limit;
    int rx;
    int ry;

    unsigned char bx;
    unsigned char by;
    char dir;

    FUN_00417f40();
    for (node = (struct WaterNode *)DAT_004cc02c; node != NULL; node = node->next) {
        if (node->field_8 == 1) {
            limit = 0;
            node->field_9++;
            lls = *(int *)(*(int *)((char *)DAT_004cc018 + 8) + node->field_a * 4);
            if (lls != 0) {
                lls = GetLLSForSprite((struct SpriteLLS *)lls);
                if (lls != 0) {
                    limit = *(short *)(lls + 0x10);
                } else {
                    // STRING: LEGOLAND 0x004b504c
                    DBPrintf("Bad Sprite in Waterworks block\n");
                }
            } else {
                // STRING: LEGOLAND 0x004b5034
                DBPrintf("Bad Block Index (%d)\n", node->field_a);
            }
            if ((int)node->field_9 >= limit) {
                node->field_8 = 0;
                node->field_9 = 0;
            }
        } else {
            lls = GetLLSForSprite(*(struct SpriteLLS **)(*(int *)((char *)DAT_004cc018 + 8) + node->field_a * 4));
            if (lls == 0) {
                DBPrintf("Bad Sprite in Waterworks block\n");
            } else {
                node->field_9 = 0;
            }
        }
    }
    render = (struct WaterRender *)arg->field_c;
    rnode = render->nodes;
    while (rnode != NULL) {
        rx = *(int *)((char *)render + 0xc);
        bloke = (unsigned char *)rnode->fn;
        ry = *(int *)((char *)render + 0x10);
        bx = *(unsigned char *)&rnode->id;
        next = rnode->next;
        by = *((unsigned char *)&rnode->id + 1);
        ry = by + ry;
        rx = bx + rx;
        if (*(short *)(bloke + 0xe) == 0) {
            switch (bloke[0x60]) {
            case 0:
                bloke[0x62] |= 8;
                ry = ry * 0x100 + 0x80;
                *(unsigned int *)(bloke + 0x24) = rx * 0x100 + -0x80;
                *(int *)(bloke + 0x28) = ry;
                dir = CalcMoveLine(*(int *)(bloke + 0x68), *(int *)(bloke + 0x6c), *(int *)(bloke + 0x24), ry, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                bloke[0x73] = dir + 0x10;
                NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke[0x60]++;
                break;
            case 1:
                RemoveBlokeFromRide((struct Ride *)render, (struct RideNode *)rnode);
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
                break;
            }
        }
        rnode = next;
    }
}

// FUNCTION: LEGOLAND 0x00418110
unsigned int *FUN_00418110(unsigned int *arg1, unsigned short arg2) {
    struct WaterNode *node;
    int idx;

    node = FUN_00417d10(&arg2);
    if (node != NULL) {
        idx = node->field_a * 4;
        DAT_004cbffc = arg2;
        DAT_004cbff0 = *(int *)(*(int *)((char *)DAT_004cc018 + 8) + idx);
        DAT_004cbff4 = *(int *)(*(int *)((char *)DAT_004cc018 + 0xc) + idx) >> 1;
        DAT_004cc000 = 0;
        DAT_004cbff8 = *(int *)(*(int *)((char *)DAT_004cc018 + 0x10) + idx) >> 1;
        *(unsigned int *)(DAT_004cbff0 + 0x10) |= 0x2000;
        SetOverrideFrame(node->field_9);
    }
    return &DAT_004cbff0;
}

// FUNCTION: LEGOLAND 0x004181a0
void FUN_004181a0(struct WaterArg *arg, int a, int b, unsigned short *key) {
    struct RenderNode *node;

    node = ((struct WaterRender *)arg->field_c)->nodes;
    while (node != NULL) {
        if (*key == node->id) {
            IP_RenderBlokeIn3DNow((struct Bloke *)node->fn);
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x004181e0
int FUN_004181e0(struct EditObject *editObj, unsigned char *coords) {
    unsigned short key;
    struct WaterNode *node;
    int rnd;
    int result;

    *(unsigned char *)&key = coords[0];
    *((unsigned char *)&key + 1) = coords[4];
    AddBasicObject(editObj, (int *)coords);
    FUN_00417cb0(key);
    node = FUN_00417d10(&key);
    result = 0;
    if (node != NULL) {
        rnd = rand();
        node->field_a = (char)(rnd % *(int *)((char *)DAT_004cc018 + 4));
        result = rnd / *(int *)((char *)DAT_004cc018 + 4);
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00418230
void FUN_00418230(unsigned int a, unsigned int b, unsigned int c) {
    struct WaterNode *node;

    StandardRemoveObject(a, b, c);
    node = FUN_00417d10((unsigned short *)&b);
    if (node != NULL) {
        FUN_00417cf0(node);
    }
}

// FUNCTION: LEGOLAND 0x00418260
void FUN_00418260(unsigned short key) {
    struct WaterNode *node;

    node = (struct WaterNode *)malloc(sizeof(struct WaterNode));
    if (node != NULL) {
        ((unsigned int *)node)[0] = 0;
        ((unsigned int *)node)[1] = 0;
        ((unsigned int *)node)[2] = 0;
        node->key = key;
        FUN_00417b80(&DAT_004cc030, node);
    }
}

// FUNCTION: LEGOLAND 0x004182a0
void FUN_004182a0(struct WaterNode *node) {
    FUN_00417bd0(&DAT_004cc030, node);
    free(node);
}

// FUNCTION: LEGOLAND 0x004182c0
struct WaterNode *FUN_004182c0(unsigned short *key) {
    return FUN_00417ba0(DAT_004cc030, key);
}

// FUNCTION: LEGOLAND 0x004182e0
void FUN_004182e0(struct WaterArg *arg) {
    FUN_00417a90();
    DAT_004cc024 = arg->field_c;
    DAT_004cc024->field_1c |= 0x420;
    DAT_004cbfe0 = DAT_004cc024->field_64;
    DAT_004cbfe0->field_10 |= 0x2000;
    // STRING: LEGOLAND 0x004b506c
    DAT_004cc014 = LoadSprite("shower.lls", 1);
}

// FUNCTION: LEGOLAND 0x00418330
void FUN_00418330(void) {
    if (DAT_004cc014 != 0) {
        KillSprite(DAT_004cc014);
    }
    FUN_00417ac0();
}

// FUNCTION: LEGOLAND 0x00418350
void FUN_00418350(void) {
    struct WaterNode *node;

    node = (struct WaterNode *)DAT_004cc030;
    while (node != NULL) {
        if (FUN_00417ec0((unsigned char *)&node->key) != 0) {
            if (node->field_8 == 0) {
                node->field_8 = 1;
                node->field_9 = 0;
                FUN_00417af0((unsigned char *)&node->key, 0, 0);
            }
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x004183a0
void FUN_004183a0(void) {
    struct WaterNode *node;
    int lls;
    int limit;

    FUN_00418350();
    for (node = DAT_004cc030; node != NULL; node = node->next) {
        if (node->field_8 == 1 && (unsigned char)(node->field_9 += 1) >= 0xb) {
            node->field_8 = 2;
            node->field_9 = 0;
        }
        if (node->field_8 == 2) {
            limit = 0;
            node->field_9++;
            lls = GetLLSForSprite((struct SpriteLLS *)DAT_004cc014);
            if (lls != 0) {
                limit = *(short *)(lls + 0x10);
            }
            if ((int)node->field_9 >= limit) {
                if (FUN_00417ec0((unsigned char *)&node->key) != 0) {
                    node->field_8 = 2;
                    node->field_9 = 0xb;
                } else {
                    node->field_8 = 3;
                    node->field_9 = 0x24;
                }
            }
        }
        if (node->field_8 == 3 && (unsigned char)(node->field_9 += 1) >= 0x2c) {
            node->field_8 = 0;
            node->field_9 = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00418450
void FUN_00418450(struct WaterRender *render, unsigned int p2, unsigned int p3, unsigned char *coords, unsigned int p5, unsigned int p6) {
    struct WaterNode *node;
    void *ctx;
    struct AdjustOffset offset;
    union {
        __int64 q;
        int i[2];
    } sc;

    ctx = render->field_c;
    node = FUN_004182c0((unsigned short *)coords);
    if (node != NULL && node->field_8 == 2) {
        offset.x = -8;
        offset.y = 0x14;
        AdjustOffsetForViewMode((struct AdjustStruct *)&offset);
        sc.q = GetScreenCoordsForObject(coords, ctx);
        LLSSetFrame((struct LLS *)GetLLSForSprite((struct SpriteLLS *)DAT_004cc014), node->field_9);
        PrintSprite(DAT_004cc014, sc.i[0] + offset.x, sc.i[1] + offset.y, p6, 0);
    }
}

// FUNCTION: LEGOLAND 0x004184e0
void FUN_004184e0(struct EditObject *editObj, unsigned char *coords) {
    unsigned short key;

    *(unsigned char *)&key = coords[0];
    *((unsigned char *)&key + 1) = coords[4];
    AddBasicObject(editObj, (int *)coords);
    FUN_00418260(key);
}

// FUNCTION: LEGOLAND 0x00418510
void FUN_00418510(unsigned int a, unsigned int b, unsigned int c) {
    struct WaterNode *node;

    StandardRemoveObject(a, b, c);
    node = FUN_004182c0((unsigned short *)&b);
    if (node != NULL) {
        FUN_004182a0(node);
    }
}

// FUNCTION: LEGOLAND 0x00418540
unsigned int *FUN_00418540(unsigned int *arg1, unsigned short arg2) {
    struct WaterSub *sub;
    struct WaterListNode *node;

    sub = (struct WaterSub *)arg1[3];
    DAT_004cbffc = arg2;
    DAT_004cbff0 = sub->field_64;
    DAT_004cbff4 = sub->field_14;
    DAT_004cbff8 = sub->field_18;
    DAT_004cc000 = 0;
    node = (struct WaterListNode *)FUN_004182c0(&arg2);
    if (node != NULL) {
        if (node->field_8 == 2) {
            SetOverrideFrame(0xb);
        } else {
            SetOverrideFrame(node->field_9);
        }
    }
    return &DAT_004cbff0;
}

// FUNCTION: LEGOLAND 0x004185c0
void FUN_004185c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00418630
void FUN_00418630(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00418670
void FUN_00418670(struct WaterNode *node) {
    FUN_00417bd0(&DAT_004cc034, node);
    free(node);
}

// FUNCTION: LEGOLAND 0x00418690
struct WaterNode *FUN_00418690(unsigned short *key) {
    return FUN_00417ba0(DAT_004cc034, key);
}

// FUNCTION: LEGOLAND 0x004186b0
unsigned int FUN_004186b0(struct WaterArg *arg) {
    FUN_00417a90();
    DAT_004cbfdc = arg->field_c;
    DAT_004cbfdc->field_1c |= 0x20;
    DAT_004cbfec = (unsigned int)DAT_004cbfdc->field_64;
    // STRING: LEGOLAND 0x004b5078
    DAT_004cc020 = LoadSprite("ww_elsquirt.lls", 1);
    return (unsigned int)DAT_004cc020;
}

// FUNCTION: LEGOLAND 0x004186f0
void FUN_004186f0(void) {
    if (DAT_004cc020 != 0) {
        KillSprite(DAT_004cc020);
    }
    FUN_00417ac0();
}

// FUNCTION: LEGOLAND 0x00418710
unsigned int FUN_00418710(void *arg1, unsigned int arg2) { STUB(); }

// FUNCTION: LEGOLAND 0x004187b0
void FUN_004187b0(void) {
    struct WaterNode *node;

    node = (struct WaterNode *)DAT_004cc034;
    while (node != NULL) {
        if (FUN_00418710((unsigned char *)&node->key, (unsigned int)DAT_004cbfdc + 0x3c) != 0) {
            if (node->field_8 == 0) {
                node->field_8 = 1;
                node->field_9 = 0;
            }
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x004187f0
void FUN_004187f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004188d0
void FUN_004188d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00418910
void FUN_00418910(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00418950
void FUN_00418950(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004189c0
void FUN_004189c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00418a10
void FUN_00418a10(unsigned int a, unsigned int b, unsigned int c) {
    RemoveSoundObject(a, b, c);
}

// FUNCTION: LEGOLAND 0x00418a30
void FUN_00418a30(struct CursorNode *arg, unsigned int a, unsigned int b) {
    struct CursorData *data;

    data = arg->field_c;
    memcpy(EditCursor.field_1414, &data->field_3c, 5 * sizeof(unsigned int));
    EditCursor.field_1830 = 0;
    ScreenToMapRef(a, &EditCursor.field_1404, b);
    ValidateCursor(&EditCursor, (unsigned int)data);
    if (FUN_00417c90() != 0) {
        return;
    }
    FUN_0045f480(&EditCursor, 0xc);
    FUN_0045f4d0(&EditCursor);
}

// FUNCTION: LEGOLAND 0x00418aa0
LEGO_EXPORT int Save_WaterBlock(void) {
    struct WaterListItem *current;
    unsigned int one;
    unsigned int zero;

    one = 1;
    zero = 0;
    current = (struct WaterListItem *)DAT_004cc02c;
    while (current != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(current, 12) == 0) {
            return 0;
        }
        current = current->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00418b10
LEGO_EXPORT int Load_WaterBlock(void) {
    struct WaterNode *node;
    struct WaterNode *prev;
    unsigned int count;

    prev = NULL;
    if (SaveGameRead(&count, 4) == 0) {
        return 0;
    }
    while (count != 0) {
        node = (struct WaterNode *)malloc(12);
        if (SaveGameRead(node, 12) == 0) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            DAT_004cc02c = node;
        }
        prev = node;
        if (SaveGameRead(&count, 4) == 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00418b90
LEGO_EXPORT int Save_ElephantF(void) {
    struct WaterListItem *current;
    unsigned int one;
    unsigned int zero;

    one = 1;
    zero = 0;
    current = (struct WaterListItem *)DAT_004cc034;
    while (current != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(current, 12) == 0) {
            return 0;
        }
        current = current->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00418c00
LEGO_EXPORT int Load_ElephantF(void) {
    struct WaterNode *node;
    struct WaterNode *prev;
    unsigned int count;

    prev = NULL;
    if (SaveGameRead(&count, 4) == 0) {
        return 0;
    }
    while (count != 0) {
        node = (struct WaterNode *)malloc(12);
        if (SaveGameRead(node, 12) == 0) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            DAT_004cc034 = node;
        }
        prev = node;
        if (SaveGameRead(&count, 4) == 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00418c80
void FUN_00418c80(void) { STUB(); }
