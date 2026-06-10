#include "legoland.h"
#include "math.h"
#include "crt.h"
#include "globals.h"

#include "clipping.h"
#include "timer.h"
#include "icon.h"
#include "render.h"
#include "gfx.h"
#include "llidb.h"
#include "interface.h"
#include "print_sprite.h"

struct Sprite;

struct IconAnim {
    /* 0x00 */ struct LLS *lls;
    /* 0x04 */ unsigned char pad_4[0x14 - 0x4];
    /* 0x14 */ int kind;
};

struct IconSprite {
    /* 0x00 */ unsigned char pad_0[8];
    /* 0x08 */ struct IconAnim *anim;
};

struct PrintCtx {
    /* 0x00 */ unsigned int flags;
    /* 0x04 */ struct IconNode *node;
    /* 0x08 */ unsigned int field_8;
};

struct IconGroupRef {
    unsigned char pad_0[3];
    unsigned char field_3;
    unsigned char field_4;
    unsigned char field_5;
    unsigned char field_6;
};

struct Rect16 {
    unsigned char pad_0[0xc];
    short field_c;
    short field_e;
    short field_10;
    short field_12;
};

struct Rect32 {
    int field_0;
    int field_4;
    int field_8;
    int field_c;
};

struct Config {
    unsigned short field_0;
    unsigned short field_2;
};


struct Bbox {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
};

struct Indicator {
    struct Indicator *next;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
    struct IconNode *field_14;
    unsigned int field_18;
    void (*field_1c)(struct Indicator *);
    unsigned char pad_20[0x22 - 0x20];
    unsigned char field_22;
};

struct CtrlBuffer {
    unsigned char pad_0[8];
    int field_8;
};

struct ScrollRegion {
    /* 0x00 */ short field_0;
    /* 0x02 */ unsigned char pad_2[0x4 - 0x2];
    /* 0x04 */ unsigned char field_4;
    /* 0x05 */ unsigned char pad_5[0xc - 0x5];
    /* 0x0c */ int field_c;
    /* 0x10 */ int field_10;
    /* 0x14 */ int field_14;
    /* 0x18 */ int field_18;
    /* 0x1c */ int field_1c;
    /* 0x20 */ int field_20;
    /* 0x24 */ int field_24;
    /* 0x28 */ int field_28;
};

struct IndicatorFuncs {
    unsigned char pad_0[4];
    unsigned int field_4;
    unsigned char pad_8[0x18 - 0x8];
    unsigned int field_18;
    unsigned int field_1c;
};

struct TimedIndicator {
    struct Indicator *next;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
    struct IconNode *field_14;
    unsigned int field_18;
    unsigned int field_1c;
};

#include "image_sprite.h"

struct SpriteLLS;
LEGO_EXPORT unsigned int GetLLSForSprite(struct SpriteLLS *sprite);
LEGO_EXPORT void PrintCent(int x, int y, int param_3, void *text, int param_5);
int FUN_00458930(double value);
LEGO_EXPORT int GetObjCost(void *info);
LEGO_EXPORT void SetEditObject(void *obj);
void FUN_00471ca0(void *param);
LEGO_EXPORT void *PlayInstanceOfSample(void *def, unsigned int looping, unsigned int a3, unsigned int a4);
LEGO_EXPORT int GetBrickCount(void);
int FUN_0048aef0(void *param_1, void *param_2);
LEGO_EXPORT void MoveIcons(unsigned short mask, unsigned short id, short dx, short dy);
int FUN_0046dd10(unsigned short param_1, short param_2, short param_3, unsigned short param_4, int param_5);
void FUN_0046d850(struct ScrollRegion *r, int param_2, int param_3);
unsigned char FUN_0046d980(struct IconNode *icon, unsigned char buttons, int a3, int a4);
unsigned char FUN_0046da20(struct IconNode *icon, unsigned char buttons, int a3, int a4);


// FUNCTION: LEGOLAND 0x0046d3a0
void FUN_0046d3a0(void) { DAT_006687a4 = 4; }

// FUNCTION: LEGOLAND 0x0046d3c0
void FUN_0046d3c0(struct IconNode *node) {
    if (!node) {
        return;
    }
    if (node->sprite) {
        KillSprite(node->sprite);
        node->sprite = NULL;
    }
    if (node->field_34 & 0x80) {
        if (node->field_30) {
            free(node->field_30);
        }
    }
    free(node);
    if (FocussedIconPtr == node) {
        FocussedIconPtr = NULL;
    }
    if (DAT_004bdd00 == 2 && DAT_004bdd04 == (unsigned int)node) {
        DAT_004bdd04 = 0;
        DAT_004bdd00 = 0x100;
    }
}

// FUNCTION: LEGOLAND 0x0046d440
void FUN_0046d440(struct IconNode *node) {
    ((struct IconNode *)PTR_some_list_head_004ba87c)->next = node;
    PTR_some_list_head_004ba87c = node;
}

// FUNCTION: LEGOLAND 0x0046d460
void FUN_0046d460(struct IconNode **slot) {
    struct IconNode *node = *slot;
    struct IconNode *next;
    if (FocussedIconPtr == node->next) {
        FocussedIconPtr = NULL;
    }
    if (PTR_some_list_head_004ba87c == *slot) {
        PTR_some_list_head_004ba87c = (struct IconNode *)slot;
    }
    node = *slot;
    next = node->next;
    FUN_0046d3c0(node);
    *slot = next;
}

// FUNCTION: LEGOLAND 0x0046d4a0
void FUN_0046d4a0(struct IconNode **slot) {
    struct IconNode *node = *slot;
    struct IconNode *next;
    if (FocussedIconPtr == node->next) {
        FocussedIconPtr = NULL;
    }
    if (PTR_DAT_004ba880 == *slot) {
        PTR_DAT_004ba880 = slot;
    }
    node = *slot;
    next = node->next;
    FUN_0046d3c0(node);
    *slot = next;
}

// FUNCTION: LEGOLAND 0x0046d4e0
void FUN_0046d4e0(struct IconNode *node) {
    struct IconNode *cur;
    struct IconNode *next;
    if (DAT_006687c8 == node) {
        FUN_0046d460(&DAT_006687c8);
        return;
    }
    cur = DAT_006687c8;
    if (cur != NULL) {
        while ((next = cur->next) != node) {
            cur = next;
            if (next == NULL) {
                return;
            }
        }
        if (cur != NULL) {
            FUN_0046d460((struct IconNode **)cur);
        }
    }
}

// FUNCTION: LEGOLAND 0x0046d520
LEGO_EXPORT void RemoveIconGroup(unsigned short group) {
    struct IconNode **slot;
    struct IconNode *node;
    int removed = 0;

    slot = &DAT_006687c8;
    node = DAT_006687c8;
    while (node != NULL) {
        if (node->field_14 == group) {
            node = node->next;
            FUN_0046d460(slot);
            removed++;
        } else {
            slot = &node->next;
            node = node->next;
        }
    }
    slot = &DAT_006687cc;
    node = DAT_006687cc;
    while (node != NULL) {
        if (node->field_14 == group) {
            node = node->next;
            FUN_0046d4a0(slot);
            removed++;
        } else {
            slot = &node->next;
            node = node->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x0046d590
void FUN_0046d590(unsigned short val) {
    struct IconNode **slot;
    struct IconNode *node;
    int removed = 0;

    slot = &DAT_006687c8;
    node = DAT_006687c8;
    while (node != NULL) {
        if (node->field_14 < val || (int)node->field_14 >= val + 7) {
            slot = &node->next;
            node = node->next;
        } else {
            node = node->next;
            FUN_0046d460(slot);
            removed++;
        }
    }
    slot = &DAT_006687cc;
    node = DAT_006687cc;
    while (node != NULL) {
        if (node->field_14 < val || (int)node->field_14 >= val + 7) {
            slot = &node->next;
            node = node->next;
        } else {
            node = node->next;
            FUN_0046d4a0(slot);
            removed++;
        }
    }
}

// FUNCTION: LEGOLAND 0x0046d630
LEGO_EXPORT struct IconNode *FindIcon(unsigned short id) {
    struct IconNode *node = (struct IconNode *)DAT_006687c8;
    while (node) {
        if (node->field_14 == id && (node->field_34 & 0x100) == 0) {
            return node;
        }
        node = node->next;
    }
    node = (struct IconNode *)DAT_006687cc;
    while (node) {
        if (node->field_14 == id && (node->field_34 & 0x100) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0046d680
void FUN_0046d680(struct IconNode *node, struct Sprite *sprite) {
    if (node) {
        struct Sprite *old = node->sprite;
        if (sprite != old) {
            if (old) {
                KillSprite(old);
            }
            if (sprite) {
                ReferenceSprite(sprite);
            }
            node->sprite = sprite;
        }
    }
}

// FUNCTION: LEGOLAND 0x0046d6c0
LEGO_EXPORT struct IconNode *InsertIcon(int a1, int a2, int a3, struct Sprite *sprite) {
    struct IconNode *icon = (struct IconNode *)malloc(sizeof(struct IconNode));
    if (icon != NULL) {
        *icon = DAT_00668858;
        icon->field_c = (short)a1;
        icon->field_e = (short)a2;
        icon->field_14 = (unsigned short)a3;
        ReferenceSprite(sprite);
        icon->sprite = sprite;
        if (sprite != NULL) {
            icon->field_12 = sprite->height;
            icon->field_34 = icon->field_34 | 0x10;
            icon->field_10 = sprite->width;
        }
    }
    FUN_0046d440(icon);
    return icon;
}

// FUNCTION: LEGOLAND 0x0046d740
LEGO_EXPORT void SetNewGroup_Callbacks(void *param_1, void *param_2, void *param_3) {
    DAT_006688a8 = param_1;
    DAT_006688ac = param_2;
    DAT_006688b0 = param_3;
}

// FUNCTION: LEGOLAND 0x0046d760
LEGO_EXPORT struct IconNode *AddFullScreenIcon(void *icon) {
    struct IconNode *result = InsertIcon(0, 0, (int)icon, 0);
    if (result) {
        result->field_10 = ((struct Config *)lpConfig)->field_0;
        result->field_12 = ((struct Config *)lpConfig)->field_2;
        result->field_28 = (void *)FUN_0046df60;
        result->field_34 = result->field_34 | 0x29;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0046d7b0
LEGO_EXPORT struct SpriteIcon *LoadSpriteIcon(const char *filename, unsigned int param_2, int param_3, int param_4, int param_5) {
    struct SpriteIcon *result = NULL;
    struct Sprite *sprite = LoadSprite(filename, param_2);
    if (sprite != NULL) {
        result = (struct SpriteIcon *)InsertIcon(param_3, param_4, param_5, sprite);
    }
    KillSprite(sprite);
    return result;
}

// FUNCTION: LEGOLAND 0x0046d850
void FUN_0046d850(struct ScrollRegion *r, int param_2, int param_3) {
    int dx;
    int dy;
    int ebp;

    if ((r->field_4 & 1) != 0) {
        if (r->field_18 - r->field_10 <= r->field_28 - r->field_20) {
            MoveIcons(0xffff, r->field_0, 0, (short)(r->field_20 - r->field_10));
            return;
        }
    } else {
        if (r->field_14 - r->field_c <= r->field_24 - r->field_1c) {
            MoveIcons(0xffff, r->field_0, (short)(r->field_1c - r->field_c), 0);
            return;
        }
    }

    dy = FUN_0046dd10(0xffff, (short)((r->field_20 - r->field_10) - param_3), (short)(r->field_10 + param_3), r->field_0, param_3);
    dx = param_3;
    ebp = r->field_c + param_3;
    if ((r->field_4 & 1) != 0) {
        if (r->field_10 + dy <= r->field_20) {
            ebp = r->field_18 + dy;
            if (ebp < r->field_28) {
                dy = dy + (r->field_28 - ebp);
            }
        } else {
            dy = dy + (r->field_20 - (r->field_10 + dy));
        }
    } else {
        if (ebp > r->field_1c) {
            dx = dx + (r->field_1c - ebp);
        } else {
            ebp = r->field_14 + param_3;
            if (ebp < r->field_24) {
                dx = dx + (r->field_24 - ebp);
            }
        }
    }
    r->field_c = r->field_c + dx;
    r->field_10 = r->field_10 + dy;
    r->field_14 = r->field_14 + dx;
    r->field_18 = r->field_18 + dy;
    DAT_00668e44[DAT_00668e64] = DAT_00668e44[DAT_00668e64] + dy;
    MoveIcons(0xffff, r->field_0, (short)dx, (short)dy);
}

// FUNCTION: LEGOLAND 0x0046d980
unsigned char FUN_0046d980(struct IconNode *icon, unsigned char buttons, int a3, int a4) {
    struct ScrollRegion *region;
    if ((buttons & 1) != 0) {
        region = (struct ScrollRegion *)icon->field_30;
        DAT_006688b4 = GetTickCount();
        if ((region->field_4 & 1) != 0) {
            FUN_0046d850(region, 0, 6);
            return 2;
        }
        FUN_0046d850(region, 0x20, 0);
        return 2;
    }
    if ((buttons & 4) == 0) {
        return 1;
    }
    if (0xf9 < GetTickCount() - DAT_006688b4) {
        region = (struct ScrollRegion *)icon->field_30;
        DAT_006688b4 = GetTickCount();
        if ((region->field_4 & 1) != 0) {
            FUN_0046d850(region, 0, 6);
            return 2;
        }
        FUN_0046d850(region, 0x20, 0);
    }
    return 2;
}

// FUNCTION: LEGOLAND 0x0046da20
unsigned char FUN_0046da20(struct IconNode *icon, unsigned char buttons, int a3, int a4) {
    struct ScrollRegion *region;
    if ((buttons & 1) != 0) {
        region = (struct ScrollRegion *)icon->field_30;
        DAT_006688b4 = GetTickCount();
        if ((region->field_4 & 1) != 0) {
            FUN_0046d850(region, 0, -6);
            return 2;
        }
        FUN_0046d850(region, -0x20, 0);
        return 2;
    }
    if ((buttons & 4) == 0) {
        return 1;
    }
    if (0xf9 < GetTickCount() - DAT_006688b4) {
        region = (struct ScrollRegion *)icon->field_30;
        DAT_006688b4 = GetTickCount();
        if ((region->field_4 & 1) != 0) {
            FUN_0046d850(region, 0, -6);
            return 2;
        }
        FUN_0046d850(region, -0x20, 0);
    }
    return 2;
}

// FUNCTION: LEGOLAND 0x0046dac0
void FUN_0046dac0(void) {
    unsigned int v;
    struct IconNode *icon;
    if (DAT_008119b4 == 2 && DAT_0080ff88 == 3) {
        int field = ((struct CtrlBuffer *)CONTROLLERBUFFER)->field_8;
        if (field < 0xb2) {
            v = 0xc8;
        } else if (field < 0x143) {
            v = 0x1f4;
        } else {
            unsigned int ecx = (field >= 0x1d4) ? 1 : 0;
            ecx = ecx - 1;
            ecx = ecx & 0x64;
            v = ecx + 0x12c;
        }
    } else {
        v = 0xd2;
    }
    v = v + 3;
    icon = FindIcon((unsigned short)v);
    if (icon) {
        FUN_0046d980(icon, 1, 0, 0);
    }
}

// FUNCTION: LEGOLAND 0x0046db40
void FUN_0046db40(void) {
    unsigned int v;
    struct IconNode *icon;
    if (DAT_008119b4 == 2 && DAT_0080ff88 == 3) {
        int field = ((struct CtrlBuffer *)CONTROLLERBUFFER)->field_8;
        if (field < 0xb2) {
            v = 0xc8;
        } else if (field < 0x143) {
            v = 0x1f4;
        } else {
            unsigned int ecx = (field >= 0x1d4) ? 1 : 0;
            ecx = ecx - 1;
            ecx = ecx & 0x64;
            v = ecx + 0x12c;
        }
    } else {
        v = 0xd2;
    }
    v = v + 4;
    icon = FindIcon((unsigned short)v);
    if (icon) {
        FUN_0046da20(icon, 1, 0, 0);
    }
}

// FUNCTION: LEGOLAND 0x0046dbc0
LEGO_EXPORT void AddGBarIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046dcd0
LEGO_EXPORT void MoveIcons(unsigned short mask, unsigned short id, short dx, short dy) {
    struct IconNode *node;
    for (node = DAT_006687c8; node != NULL; node = node->next) {
        if ((node->field_34 & 1) == 0 && (node->field_14 & mask) == id) {
            node->field_c = node->field_c + dx;
            node->field_e = node->field_e + dy;
        }
    }
}

// FUNCTION: LEGOLAND 0x0046dd10
int FUN_0046dd10(unsigned short param_1, short param_2, short param_3, unsigned short param_4, int param_5) {
    struct IconNode *best = NULL;
    struct IconNode *node = DAT_006687c8;
    int extreme;
    int v;

    if (param_5 < 1) {
        if (param_5 >= 0) {
            return 0;
        }
        extreme = 100000;
        if (node == NULL) {
            return 0;
        }
        do {
            if ((node->field_34 & 1) == 0 && (node->field_14 & param_1) == param_4 &&
                (v = (int)node->field_e, (int)param_2 < (v - param_3) + param_5) && v < extreme) {
                extreme = v;
                best = node;
            }
            node = node->next;
        } while (node != NULL);
    } else {
        extreme = -100000;
        if (node == NULL) {
            return 0;
        }
        do {
            if ((node->field_34 & 1) == 0 && (node->field_14 & param_1) == param_4 &&
                (v = (int)node->field_e, (v - param_3) + param_5 <= (int)param_2 && extreme < v)) {
                extreme = v;
                best = node;
            }
            node = node->next;
        } while (node != NULL);
    }
    if (best != NULL && best->field_e != param_2) {
        return ((int)param_2 - (int)best->field_e) + (int)param_3;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046de50
void FUN_0046de50(struct Rect16 *src, struct Rect32 *dst) {
    dst->field_0 = src->field_c;
    dst->field_4 = src->field_e;
    dst->field_8 = src->field_10 + src->field_c;
    dst->field_c = src->field_12 + src->field_e;
}

// FUNCTION: LEGOLAND 0x0046de90
void FUN_0046de90(struct IconNode *icon, struct Bbox *bbox) {
    FUN_0046de50((struct Rect16 *)icon, (struct Rect32 *)bbox);
    if (icon->field_34 & 0x20) {
        bbox->min_x = bbox->min_x - 3;
        bbox->max_x = bbox->max_x + 3;
        bbox->min_y = bbox->min_y - 3;
        bbox->max_y = bbox->max_y + 3;
    }
    if (icon->field_34 & 0x40) {
        struct Indicator *ind;
        bbox->min_y = bbox->min_y - 0x16;
        ind = (struct Indicator *)icon->field_30;
        if (ind != NULL) {
            if (ind->field_22 & 8) {
                bbox->max_y = bbox->max_y + DAT_00668840;
            }
            if (ind->field_22 & 2) {
                bbox->max_x = bbox->max_x + DAT_0066884c;
            }
        }
    }
    if (icon->field_34 & 0x200) {
        int v = icon->field_22 + icon->field_e;
        int top = v + 0x28;
        if (bbox->min_y > v) {
            bbox->min_y = v;
        }
        if (bbox->max_y < top) {
            bbox->max_y = top;
        }
    }
}

// FUNCTION: LEGOLAND 0x0046df30
int FUN_0046df30(struct Rect16 *src) {
    int rect[4];
    FUN_0046de50(src, (struct Rect32 *)&rect[0]);
    SetClipping(&rect[0]);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046df60
int FUN_0046df60(int param) {
    SetClipping((int *)DAT_007fe020);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046df70
LEGO_EXPORT int RenderBoxIcon(struct IconNode *node) {
    unsigned short border = node->box.border;
    RenderThickBox(node->field_c - border, node->field_e - border,
                   node->field_10 + border * 2, node->field_12 + border * 2,
                   border, node->box.box_color);
    if ((node->field_34 & 0x20) != 0) {
        FUN_0046df30((struct Rect16 *)node);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046dfd0
int FUN_0046dfd0(struct IconNode *node) {
    struct PrintCtx ctx;

    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    if (node->sprite != NULL) {
        PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
    }
    PrintCent(node->field_20 + node->field_c, node->field_22 + node->field_e, 0x40, node->field_18p, 1);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e040
int FUN_0046e040(struct IconNode *node) {
    struct PrintCtx ctx;

    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    if (node == (struct IconNode *)FocussedIconPtr) {
        PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
        return 0;
    }
    PrintSprite(node->alt_sprite, node->field_c, node->field_e, 0, (int *)&ctx);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e0a0
LEGO_EXPORT void RenderBuildObjectIcon(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e300
LEGO_EXPORT int RenderFreePlayIcons(struct IconNode *node) {
    struct PrintCtx ctx;
    short y;
    struct Sprite *sprite;

    ctx.flags = 2;
    ctx.node = node;
    y = node->field_e;
    ctx.field_8 = 0;
    if (y >= 0 && y <= 0x12c) {
        sprite = node->sprite;
        if ((char)node->field_18 != 1) {
            if (sprite != NULL) {
                FUN_0048a840((unsigned int)node->field_1c, 0);
                if (FUN_0048aef0(node->field_1c, node->field_20p) != 0) {
                    PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
                    return 0;
                }
                PrintSprite(node->sprite, node->field_c, node->field_e, 0xff000000, (int *)&ctx);
                return 0;
            }
        } else {
            if (sprite != NULL) {
                PrintSprite(sprite, node->field_c, y, 0, (int *)&ctx);
            }
            PrintSprite(DAT_007cb398, node->field_c + 0x46, node->field_e, 0, (int *)&ctx);
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e400
LEGO_EXPORT int RenderScroll_Icons(struct IconNode *node) {
    struct PrintCtx ctx;
    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    if (node->sprite != NULL) {
        PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
    }
    if (GetBlink() != 0) {
        if ((DAT_006688b8 & 2) != 0 && (char)node->field_18 == 2) {
            PrintSprite(DAT_00668e84, node->field_c, node->field_e, 0, (int *)&ctx);
            DAT_006688b8 = DAT_006688b8 & 0xfffffffd;
            return 0;
        }
        if ((DAT_006688b8 & 1) != 0 && (char)node->field_18 == 1) {
            PrintSprite(DAT_00668e88, node->field_c, node->field_e, 0, (int *)&ctx);
            DAT_006688b8 = DAT_006688b8 & 0xfffffffe;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e4d0
LEGO_EXPORT void RenderEnergyBar(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e670
LEGO_EXPORT void RenderMoneyBar(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e7b0
LEGO_EXPORT int RenderFreePlayBar(struct IconNode *node) {
    struct PrintCtx ctx;
    short x;
    int clip[4];

    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    StoreClipping();
    x = node->field_c;
    clip[0] = x;
    clip[2] = FUN_00458930(node->field_10 * 0.00005f * (int)DAT_007cb3a0) + x;
    clip[1] = node->field_e;
    clip[3] = node->field_12 + clip[1];
    SetClipping(&clip[0]);
    if (node->sprite != NULL) {
        PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
    }
    RestoreClipping();
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e850
LEGO_EXPORT int RenderGBarSpriteIcon(struct IconNode *node) {
    struct PrintCtx ctx;
    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    if (node->sprite != NULL) {
        PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e8a0
LEGO_EXPORT int RenderFlashingSpriteIcon(struct IconNode *node) {
    struct PrintCtx ctx;
    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    if (node->sprite != NULL) {
        if (GetBlink() != 0) {
            PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
            return 0;
        }
        PrintSprite(node->sprite, node->field_c, node->field_e, 0xff000000, (int *)&ctx);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e920
int FUN_0046e920(struct IconNode *node) {
    struct PrintCtx ctx;
    struct LLS *lls;
    int frame;

    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    if (node->sprite != NULL) {
        if (GetBlink() != 0) {
            PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
        } else {
            struct Sprite *alt = node->alt_sprite;
            if (alt != NULL) {
                lls = (struct LLS *)GetLLSForSprite((struct SpriteLLS *)node->sprite);
                if (lls != NULL) {
                    frame = *(short *)lls;
                } else {
                    frame = 0;
                }
                lls = (struct LLS *)GetLLSForSprite((struct SpriteLLS *)alt);
                if (lls != NULL) {
                    LLSSetFrame(lls, frame);
                }
                PrintSprite(alt, node->field_c, node->field_e, 0, (int *)&ctx);
                return 0;
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e9d0
LEGO_EXPORT int RenderGBarSprite(struct IconNode *node) {
    struct PrintCtx ctx;
    ctx.flags = 1;
    ctx.node = NULL;
    ctx.field_8 = 0;
    if (node->sprite != NULL) {
        PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046ea10
int FUN_0046ea10(struct IconNode *node) {
    struct PrintCtx ctx;
    RECT rect;
    RECT out;

    ctx.flags = 2;
    ctx.node = node;
    rect.left = (LONG)node->field_c;
    rect.top = (LONG)node->field_e;
    rect.right = node->field_10 + rect.left;
    rect.bottom = node->field_12 + rect.top;
    ctx.field_8 = 0;
    if (IntersectRect(&out, &rect, (RECT *)&((struct ScrollRegion *)node->field_30)->field_1c) != 0 && node->sprite != NULL) {
        PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046eaa0
int FUN_0046eaa0(struct IconNode *node) {
    struct PrintCtx ctx;

    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    if (GetBlink() != 0) {
        PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046eaf0
LEGO_EXPORT void SetupInterfacePanelIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046ec50
void FUN_0046ec50(char param) {
    char saved_mode = DAT_007fdd80;
    struct IconNode *node = DAT_006687c8;

    if (DAT_007fdd80 == 2 || DAT_007fdd80 == 3) {
        return;
    }
    while (node != NULL) {
        short id = (short)node->field_14;
        if (id == 0xd2 || id == 0xd5 || id == 0xd6 || id == 0xd7) {
            if (DAT_007fdd80 == 0) {
                short old = node->field_c;
                short sum = (short)param + old;
                node->field_c = sum;
                if ((char)node->field_18 == 0xa && sum >= 3) {
                    node->field_c = 0;
                    param = (char)node->field_c - (char)(short)old;
                    DAT_007fdd8c = 3;
                    saved_mode = 3;
                    lpConfig->field_20 = 0x79;
                    ScrollX = ScrollX + 0x7900;
                    lpConfig->field_10 = 0x207;
                    BGFullUpdate = 1;
                    DAT_007fdd88 = 1;
                }
            } else if (DAT_007fdd80 == 1) {
                if (DAT_007fdd88 != 0) {
                    lpConfig->field_20 = 0;
                    ScrollX = ScrollX - 0x7900;
                    lpConfig->field_10 = 0x280;
                    DAT_007fdd88 = 0;
                    BGFullUpdate = 1;
                    return;
                }
                if ((char)node->field_18 == 0xa) {
                    int diff = (int)(short)node->field_c - (int)param;
                    if (diff < -0x7a) {
                        DAT_007fdd80 = 2;
                        saved_mode = 2;
                        DAT_007fdd8c = 0x86;
                        FUN_0046fb40(0xd2);
                        break;
                    }
                    diff = diff + 0x7a;
                    FUN_00461290((lpConfig->field_10 - diff) * 0x100, lpConfig->field_12 << 8, diff * 0x100, 0);
                }
                node->field_c = (short)node->field_c - (short)param;
            }
        }
        node = node->next;
    }

    DAT_007fdd80 = saved_mode;
}

// FUNCTION: LEGOLAND 0x0046ee00
void FUN_0046ee00(void) {
    struct IconNode *node = DAT_006687c8;
    int mode;

    if (DAT_008119b4 == 1) {
        mode = 4;
    } else {
        switch (EditMode) {
        case 1:
            if (DAT_008119b8 != 0) {
                // STRING: LEGOLAND 0x004ba888
                if (strcmp("Path", *(char **)((char *)DAT_008119b8 + 0x78)) == 0) {
                    mode = 1;
                    break;
                }
            }
        default:
            mode = 5;
            break;
        case 0:
            mode = 2;
            break;
        case 2:
            mode = 3;
            break;
        case 4:
            mode = 4;
            break;
        }
    }
    for (; node != NULL; node = node->next) {
        if (node->field_14 == 0x93 && node->field_1c != NULL) {
            struct Sprite *sprite = node->field_20p;
            if (sprite != NULL) {
                FUN_0046d680(node, (node->field_18 == mode) ? node->field_1c : node->field_20p);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0046eee0
LEGO_EXPORT void RenderIcons(void) {
    struct PrintCtx ctx;
    struct IconNode *node = (struct IconNode *)DAT_006687c8;
    int elapsed;

    ctx.node = NULL;
    ctx.flags = 2;
    ctx.field_8 = 0;
    StoreClipping();
    FUN_0046df60(0);
    elapsed = GetTicks() - DAT_006688c8;
    if (elapsed > 0x3de) {
        elapsed = 0x3de;
    }
    FUN_0046ec50(elapsed * 5 / 33);
    DAT_006688c8 = GetTicks();
    while (node) {
        if ((node->field_34 & 0x400) == 0) {
            if (node->field_34 & 0x8) {
                ((void (*)(struct IconNode *))node->field_28)(node);
            } else if (node->sprite) {
                ctx.node = node;
                PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
            }
            if (node == (struct IconNode *)FocussedIconPtr) {
                if (node->sprite) {
                    struct IconAnim *anim = ((struct IconSprite *)node->sprite)->anim;
                    if (anim->kind == 3 || anim->kind == 2) {
                        LLSPlayOnce(anim->lls, (unsigned int)anim);
                    }
                }
                if (node->field_34 & 0x8000) {
                    unsigned int colour = GetNearestColour(0, 0xff, 0);
                    RenderThickBox(node->field_c, node->field_e, node->field_10, node->field_12, 2, colour);
                }
            }
        }
        node = node->next;
    }
    FUN_004760a0();
    RestoreClipping();
}

// FUNCTION: LEGOLAND 0x0046f010
LEGO_EXPORT void RenderIcons2(short param_1, short param_2, short param_3) {
    struct PrintCtx ctx;
    struct IconNode *node = (struct IconNode *)DAT_006687c8;

    ctx.node = NULL;
    ctx.flags = 2;
    ctx.field_8 = 0;
    StoreClipping();
    FUN_0046df60(0);
    while (node) {
        if ((node->field_34 & 0x400) == 0 && node->field_e > 0 && node->field_e < 0x1e0 &&
            ((short)node->field_14 == param_1 || (short)node->field_14 == param_2 || (short)node->field_14 == param_3)) {
            if (node->field_34 & 0x8) {
                ((void (*)(struct IconNode *))node->field_28)(node);
            } else if (node->sprite) {
                ctx.node = node;
                PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
            }
        }
        if (node == (struct IconNode *)FocussedIconPtr && node->sprite) {
            struct IconAnim *anim = ((struct IconSprite *)node->sprite)->anim;
            if (anim->kind == 3 || anim->kind == 2) {
                LLSPlayOnce(anim->lls, (unsigned int)anim);
            }
        }
        node = node->next;
    }
    RestoreClipping();
}

// FUNCTION: LEGOLAND 0x0046f100
void FUN_0046f100(short param_1) {
    struct PrintCtx ctx;
    struct IconNode *node;
    int elapsed;

    ctx.node = NULL;
    ctx.flags = 2;
    ctx.field_8 = 0;
    elapsed = GetTicks() - DAT_006688cc;
    if (elapsed > 0x3de) {
        elapsed = 0x3de;
    }
    FUN_0046ec50(elapsed * 5 / 0x21);
    DAT_006688cc = GetTicks();
    node = DAT_006687c8;
    StoreClipping();
    FUN_0046df60(0);
    for (; node != NULL; node = node->next) {
        if ((node->field_34 & 0x400) == 0 && (short)node->field_14 != param_1) {
            if (node->field_34 & 0x8) {
                ((void (*)(struct IconNode *))node->field_28)(node);
            } else if (node->sprite) {
                ctx.node = node;
                PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
            }
        }
        if (node == (struct IconNode *)FocussedIconPtr && node->sprite) {
            struct IconAnim *anim = ((struct IconSprite *)node->sprite)->anim;
            if (anim->kind == 3 || anim->kind == 2) {
                LLSPlayOnce(anim->lls, (unsigned int)anim);
            }
        }
    }
    FUN_0046df60(0);
    FUN_004760a0();
    RestoreClipping();
}

// FUNCTION: LEGOLAND 0x0046f200
LEGO_EXPORT void RenderHelpIcons(void) {
    struct PrintCtx ctx;
    struct IconNode *node = DAT_006687cc;

    ctx.node = NULL;
    ctx.flags = 2;
    ctx.field_8 = 0;
    StoreClipping();
    FUN_0046df60(0);
    for (; node != NULL; node = node->next) {
        if ((node->field_34 & 0x400) == 0 && node->field_c < 0x1e0 && node->field_c > 0) {
            if ((node->field_34 & 8) != 0) {
                ((void (*)(struct IconNode *))node->field_28)(node);
            } else if (node->sprite != NULL) {
                ctx.node = node;
                PrintSprite(node->sprite, node->field_c, node->field_e, 0, (int *)&ctx);
            }
            if (node == (struct IconNode *)FocussedIconPtr && (node->field_34 & 0x8000) != 0) {
                unsigned int colour = GetNearestColour(0, 0xff, 0);
                RenderThickBox(node->field_c, node->field_e, node->field_10, node->field_12, 2, colour);
            }
        }
    }
    RestoreClipping();
}

// FUNCTION: LEGOLAND 0x0046f2e0
unsigned char FUN_0046f2e0(struct IconNode *node, unsigned int buttons, short dx, short dy) {
    unsigned char result;
    if ((buttons & 1) != 0) {
        return 2;
    }
    result = 2;
    if ((buttons & 4) == 0) {
        result = 1;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0046f300
int FUN_0046f300(struct Point *point, struct Bbox *bbox) {
    return point->x >= bbox->min_x && point->x <= bbox->max_x &&
           point->y >= bbox->min_y && point->y <= bbox->max_y;
}

// FUNCTION: LEGOLAND 0x0046f330
int FUN_0046f330(struct Point *point, struct IconNode *icon) {
    struct Bbox bbox;
    FUN_0046de90(icon, &bbox);
    return FUN_0046f300(point, &bbox);
}

// FUNCTION: LEGOLAND 0x0046f360
LEGO_EXPORT struct IconNode *GetIconAtPos(struct Point *param_1, unsigned char *param_2) {
    short x = (short)param_1->x;
    short y = (short)param_1->y;
    struct IconNode *found = NULL;
    int doGeom = 1;
    int i = 0;
    struct Bbox bbox;

    do {
        struct IconNode *cur = (i != 0) ? DAT_006687cc : DAT_006687c8;
        for (; cur != NULL; cur = cur->next) {
            struct IconNode *candidate = found;
            unsigned int flags = cur->field_34;
            if (doGeom &&
                (((flags & 0x10) == 0 || (flags & 0x400) != 0) ||
                 (x < cur->field_c) ||
                 (y < cur->field_e) ||
                 (cur->field_10 + cur->field_c < x) ||
                 (candidate = cur, cur->field_12 + cur->field_e < y)) &&
                (candidate = found, (flags & 0x40) != 0 && (flags & 0x400) == 0) &&
                FUN_0046f330(param_1, cur) != 0) {
                *param_2 = *param_2 | 4;
                if (found != NULL && (found->field_34 & 0x800) != 0) {
                    if ((unsigned short)(cur->field_14 - 1) == (unsigned short)(found->field_14 - 3) ||
                        (unsigned short)(cur->field_14 - 1) == (unsigned short)(found->field_14 - 4)) {
                        goto next;
                    }
                }
                candidate = NULL;
            }
        next:
            if ((cur->field_34 & 0x20) != 0 && (cur->field_34 & 0x400) == 0) {
                FUN_0046de50((struct Rect16 *)cur, (struct Rect32 *)&bbox);
                if (x < bbox.min_x || bbox.max_x < x || y < bbox.min_y || bbox.max_y < y) {
                    doGeom = 0;
                } else {
                    doGeom = 1;
                }
            }
            found = candidate;
        }
        i++;
    } while (i <= 1);

    return found;
}

// FUNCTION: LEGOLAND 0x0046f4c0
LEGO_EXPORT unsigned char CheckFocussedIcon(void) {
    struct SpriteIcon *icon;

    if (DAT_006687c0 != 0 && (DAT_00813ad4 & 7) != 0) {
        return ((unsigned char (*)(unsigned int, unsigned int, unsigned int, unsigned int))DAT_006687c0)(0, DAT_00813ad4, 0, 0);
    }

    icon = (struct SpriteIcon *)FocussedIconPtr;
    if (icon != NULL) {
        if ((icon->field_34 & 2) != 0 && icon->event_handler != NULL) {
            unsigned int buttons;
            if ((DAT_00813adc & 7) != 0 && DAT_00668954 == 0) {
                return ((unsigned char (*)(struct SpriteIcon *, unsigned int, short, short))icon->event_handler)(
                    icon, DAT_00813adc, (short)DAT_00813a44 - icon->x, (short)DAT_00813a48 - icon->y);
            }
            buttons = DAT_00813ac4;
            DAT_00667c48 = 1;
            return ((unsigned char (*)(struct SpriteIcon *, unsigned int, short, short))icon->event_handler)(
                icon, buttons, (short)DAT_00813a44 - icon->x, (short)DAT_00813a48 - icon->y);
        }
        {
            unsigned int buttons = DAT_00813adc;
            if ((DAT_00813adc & 7) == 0 || DAT_00668954 != 0) {
                buttons = DAT_00813ac4;
                DAT_00667c48 = 1;
            }
            return FUN_0046f2e0((struct IconNode *)icon, buttons, (short)DAT_00813a44 - icon->x, (short)DAT_00813a48 - icon->y);
        }
    }

    if (DAT_006687bc != 0 && (DAT_00813adc & 7) != 0) {
        return ((unsigned char (*)(unsigned int, unsigned int, unsigned int, unsigned int))DAT_006687bc)(0, DAT_00813adc, 0, 0);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046f690
LEGO_EXPORT void AddGBarClassIcon(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046f7a0
LEGO_EXPORT void AddFreePlayIcon(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046f890
void FUN_0046f890(void) {
    if (DAT_006688d0 != 0) {
        return;
    }
    if (DAT_00668828 == 0) {
        // STRING: LEGOLAND 0x004ba8c8
        DAT_00668828 = LoadSprite("GBarFrame.lls", 4);
    }
    if (DAT_0066882c == 0) {
        // STRING: LEGOLAND 0x004ba8b8
        DAT_0066882c = LoadSprite("IF_Side_BUp.lls", 4);
    }
    if (DAT_00668830 == 0) {
        // STRING: LEGOLAND 0x004ba8a4
        DAT_00668830 = LoadSprite("IF_Side_BDown.lls", 4);
    }
    if (DAT_00668834 == 0) {
        // STRING: LEGOLAND 0x004ba894
        DAT_00668834 = LoadSprite("IF_Sidebar1.lls", 4);
    }
    DAT_006688d0 = 1;
}

// FUNCTION: LEGOLAND 0x0046f920
void FUN_0046f920(void) {
    if (DAT_006688d0 != 0) {
        DAT_006688d0 = 0;
        if (DAT_00668828 != 0) {
            KillSprite(DAT_00668828);
            DAT_00668828 = 0;
        }
        if (DAT_0066882c != 0) {
            KillSprite(DAT_0066882c);
            DAT_0066882c = 0;
        }
        if (DAT_00668830 != 0) {
            KillSprite(DAT_00668830);
            DAT_00668830 = 0;
        }
        if (DAT_00668834 != 0) {
            KillSprite(DAT_00668834);
            DAT_00668834 = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x0046f9a0
void FUN_0046f9a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046fb40
void FUN_0046fb40(unsigned int group) {
    /* TODO: fold group id into IconGroupRef; Ghidra models the id base as a struct pointer */
    struct IconGroupRef *g = (struct IconGroupRef *)group;
    struct IconNode *icon1;
    struct IconNode *icon2;
    struct IconNode *icon3;
    struct IconNode *icon4;

    icon1 = FindIcon(&g->field_5);
    if (icon1) {
        icon2 = FindIcon(&g->field_3);
        if (icon2) {
            FUN_0046d4e0(icon2);
        }
        icon3 = FindIcon(&g->field_4);
        if (icon3) {
            FUN_0046d4e0(icon3);
        }
        icon4 = FindIcon(&g->field_6);
        if (icon4) {
            FUN_0046d4e0(icon4);
        }
        free(icon1->field_30);
        FUN_0046d4e0(icon1);
        RemoveIconGroup(group);
    }
}

// FUNCTION: LEGOLAND 0x0046fbc0
unsigned char FUN_0046fbc0(struct IconNode *icon, unsigned char flag) {
    struct Indicator *ind = (struct Indicator *)icon->field_30;
    if (flag & 0x5) {
        if (ind->field_4 & 0x4) {
            ind->field_1c(ind);
        }
        return 2;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046fbf0
LEGO_EXPORT struct TimedIndicator *AllocateTimedIndicator(struct Sprite *sprite, unsigned int a2, unsigned int a3) {
    struct TimedIndicator *ind;
    struct IconNode *icon;

    ind = malloc(40);
    ind->next = DAT_006688d4;
    ind->field_4 = 0;
    ind->field_8 = GetGameTimer();
    ind->field_c = a2;
    ind->field_10 = a3;

    ReferenceSprite(sprite);
    icon = InsertIcon(0, 0, 0xe000, sprite);
    ind->field_14 = icon;
    icon->field_28 = (void *)FUN_0046eaa0;

    icon = ind->field_14;
    icon->field_2c = (void *)FUN_0046fbc0;

    icon = ind->field_14;
    icon->field_34 = icon->field_34 | 0x40a;

    icon = ind->field_14;
    icon->field_30 = ind;

    ind->field_18 = 0;
    ind->field_1c = 0;

    DAT_006688d4 = (struct Indicator *)ind;
    return ind;
}

// FUNCTION: LEGOLAND 0x0046fc80
LEGO_EXPORT struct TimedIndicator *AllocatePermanentIndicator(struct Sprite *sprite, unsigned int param_2) {
    struct TimedIndicator *ind;
    struct IconNode *icon;

    ind = malloc(40);
    ind->next = DAT_006688d4;
    ind->field_4 = 1;
    ind->field_8 = GetGameTimer();
    ind->field_10 = param_2;

    ReferenceSprite(sprite);
    icon = InsertIcon(0, 0, 0xe000, sprite);
    ind->field_14 = icon;
    icon->field_28 = (void *)FUN_0046eaa0;

    icon = ind->field_14;
    icon->field_2c = (void *)FUN_0046fbc0;

    icon = ind->field_14;
    icon->field_34 = icon->field_34 | 0x40a;

    icon = ind->field_14;
    icon->field_30 = ind;

    ind->field_18 = 0;
    ind->field_1c = 0;

    DAT_006688d4 = (struct Indicator *)ind;
    return ind;
}

// FUNCTION: LEGOLAND 0x0046fd00
LEGO_EXPORT void SetCheckFunc(struct IndicatorFuncs *ind, unsigned int func) {
    ind->field_18 = func;
    ind->field_4 |= 0x4;
}

// FUNCTION: LEGOLAND 0x0046fd20
LEGO_EXPORT void SetClickFunc(struct IndicatorFuncs *ind, unsigned int func) {
    ind->field_1c = func;
    ind->field_4 |= 0x4;
}

// FUNCTION: LEGOLAND 0x0046fd40
LEGO_EXPORT void AddIndicator(struct Indicator *ind) {
    struct Indicator *cur = DAT_006688d4;
    struct Indicator *next;
    if (cur == ind) {
        DAT_006688d4 = cur->next;
    } else {
        if (cur == NULL) {
            return;
        }
        while ((next = cur->next) != ind) {
            cur = next;
            if (next == NULL) {
                return;
            }
        }
        cur->next = ind->next;
    }
    if (cur != NULL) {
        ind->next = DAT_006688d8;
        DAT_006688d8 = ind;
        ind->field_4 = ind->field_4 | 8;
        ind->field_14->field_c = 0xf000;
        ind->field_14->field_34 = ind->field_14->field_34 & 0xfffffbff;
    }
}

// FUNCTION: LEGOLAND 0x0046fda0
LEGO_EXPORT void RemoveIndicator(struct Indicator *ind) {
    struct Indicator *cur = DAT_006688d8;
    struct Indicator *next;
    if (cur == ind) {
        DAT_006688d8 = cur->next;
    } else {
        if (cur == NULL) {
            return;
        }
        while ((next = cur->next) != ind) {
            cur = next;
            if (next == NULL) {
                return;
            }
        }
        cur->next = ind->next;
    }
    if (cur != NULL) {
        ind->next = DAT_006688d4;
        DAT_006688d4 = ind;
        ind->field_4 = ind->field_4 & 0xfffffff7;
        ind->field_14->field_34 = ind->field_14->field_34 | 0x400;
    }
}

// FUNCTION: LEGOLAND 0x0046fe00
LEGO_EXPORT void DeleteIndicator(struct Indicator *ind) {
    struct Indicator *cur;

    FUN_0046d4e0(ind->field_14);

    if ((ind->field_4 & 0x8) != 0) {
        if (DAT_006688d8 == ind) {
            DAT_006688d8 = ((struct Indicator *)DAT_006688d8)->next;
            free(ind);
            return;
        }
        cur = DAT_006688d8;
        while (cur != 0) {
            if (cur->next == ind) {
                cur->next = ind->next;
                free(ind);
                return;
            }
            cur = cur->next;
        }
        free(ind);
        return;
    }

    if (DAT_006688d4 == ind) {
        DAT_006688d4 = ((struct Indicator *)DAT_006688d4)->next;
        free(ind);
        return;
    }
    cur = DAT_006688d4;
    while (cur != 0) {
        if (cur->next == ind) {
            cur->next = ind->next;
            free(ind);
            return;
        }
        cur = cur->next;
    }
    free(ind);
}

// FUNCTION: LEGOLAND 0x0046feb0
LEGO_EXPORT void ControlIndicators(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00470000
unsigned char FUN_00470000(struct IconNode *node, unsigned char buttons) {
    int cost;
    unsigned int *sub;
    unsigned int flags;

    if ((buttons & 2) != 0) {
        cost = GetObjCost(node->field_8);
        if (cost <= GetBrickCount()) {
            if ((node->field_34 & 0x1000) != 0) {
                SetEditObject(node->field_8);
            }
            sub = *(unsigned int **)((char *)node->field_8 + 0xc4);
            flags = sub[2];
            if ((flags & 0x20000) != 0) {
                sub[2] = flags & 0xfffdffff;
                FUN_00471ca0(node->field_8);
            }
            PlayInstanceOfSample(DAT_004b929c, 0, 1, 0);
            return 2;
        }
        PlayInstanceOfSample(DAT_004b92d8, 0, 1, 0);
        return 2;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004700a0
LEGO_EXPORT void UpdateFocussedIconPtr(void) {
    if (DAT_004bdd00 == 2) {
        FocussedIconPtr = (void *)DAT_004bdd04;
    } else {
        FocussedIconPtr = NULL;
    }
}
