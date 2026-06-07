#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "clipping.h"
#include "timer.h"
#include "icon.h"

struct Sprite;

struct IconNode {
    struct IconNode *next;
    struct Sprite *sprite;
    unsigned char pad_8[0x10 - 0x8];
    short field_10;
    short field_12;
    unsigned short field_14;
    unsigned char pad_16[0x28 - 0x16];
    void *field_28;
    void *field_2c;
    void *field_30;
    unsigned int field_34;
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

struct Point {
    int x;
    int y;
};

struct Bbox {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
};

struct Indicator {
    struct Indicator *next;
    unsigned char field_4;
    unsigned char pad_5[0x8 - 0x5];
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
    struct IconNode *field_14;
    unsigned int field_18;
    void (*field_1c)(struct Indicator *);
};

struct CtrlBuffer {
    unsigned char pad_0[8];
    int field_8;
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
        DAT_004bdd04 = NULL;
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
void FUN_0046d4a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046d4e0
void FUN_0046d4e0(struct IconNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x0046d520
LEGO_EXPORT void RemoveIconGroup(unsigned int group) { STUB(); }

// FUNCTION: LEGOLAND 0x0046d590
void FUN_0046d590(unsigned int val) { STUB(); }

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
LEGO_EXPORT struct IconNode *InsertIcon(int a1, int a2, int a3, void *a4) { STUB(); }

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
LEGO_EXPORT struct SpriteIcon *LoadSpriteIcon(const char *filename, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5) { STUB(); }

// FUNCTION: LEGOLAND 0x0046d850
void FUN_0046d850(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046d980
void FUN_0046d980(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046da20
void FUN_0046da20(struct IconNode *icon, int a2, int a3, int a4) { STUB(); }

// FUNCTION: LEGOLAND 0x0046dac0
void FUN_0046dac0(void) { STUB(); }

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
LEGO_EXPORT void MoveIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046dd10
void FUN_0046dd10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046de50
void FUN_0046de50(struct Rect16 *src, struct Rect32 *dst) {
    dst->field_0 = src->field_c;
    dst->field_4 = src->field_e;
    dst->field_8 = src->field_10 + src->field_c;
    dst->field_c = src->field_12 + src->field_e;
}

// FUNCTION: LEGOLAND 0x0046de90
void FUN_0046de90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046df30
int FUN_0046df30(struct Rect16 *src) {
    int rect[4];
    FUN_0046de50(src, (struct Rect32 *)&rect[0]);
    SetClipping(&rect[0]);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046df60
int FUN_0046df60(void) {
    SetClipping((int *)DAT_007fe020);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046df70
LEGO_EXPORT void RenderBoxIcon(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046dfd0
void FUN_0046dfd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e040
void FUN_0046e040(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e0a0
LEGO_EXPORT void RenderBuildObjectIcon(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e300
LEGO_EXPORT void RenderFreePlayIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e400
LEGO_EXPORT void RenderScroll_Icons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e4d0
LEGO_EXPORT void RenderEnergyBar(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e670
LEGO_EXPORT void RenderMoneyBar(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e7b0
LEGO_EXPORT void RenderFreePlayBar(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e850
LEGO_EXPORT void RenderGBarSpriteIcon(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e8a0
LEGO_EXPORT void RenderFlashingSpriteIcon(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e920
void FUN_0046e920(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046e9d0
LEGO_EXPORT void RenderGBarSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046ea10
void FUN_0046ea10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046eaa0
void FUN_0046eaa0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046eaf0
LEGO_EXPORT void SetupInterfacePanelIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046ec50
void FUN_0046ec50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046ee00
void FUN_0046ee00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046eee0
LEGO_EXPORT void RenderIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046f010
LEGO_EXPORT void RenderIcons2(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046f100
void FUN_0046f100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046f200
LEGO_EXPORT void RenderHelpIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046f2e0
void FUN_0046f2e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046f300
int FUN_0046f300(struct Point *point, struct Bbox *bbox) {
    return point->x >= bbox->min_x && point->x <= bbox->max_x &&
           point->y >= bbox->min_y && point->y <= bbox->max_y;
}

// FUNCTION: LEGOLAND 0x0046f330
void FUN_0046f330(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046f360
LEGO_EXPORT void GetIconAtPos(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046f4c0
LEGO_EXPORT void CheckFocussedIcon(void) { STUB(); }

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
LEGO_EXPORT void AllocatePermanentIndicator(void) { STUB(); }

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
LEGO_EXPORT void AddIndicator(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046fda0
LEGO_EXPORT void RemoveIndicator(void) { STUB(); }

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
void FUN_00470000(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004700a0
LEGO_EXPORT void UpdateFocussedIconPtr(void) {
    if (DAT_004bdd00 == 2) {
        FocussedIconPtr = (void *)DAT_004bdd04;
    } else {
        FocussedIconPtr = NULL;
    }
}
