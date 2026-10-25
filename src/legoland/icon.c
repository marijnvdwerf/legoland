#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"
#include "math.h"

#include "clipping.h"
#include "controller.h"
#include "gfx.h"
#include "icon.h"
#include "interface.h"
#include "llidb.h"
#include "map_object.h"
#include "print_sprite.h"
#include "render.h"
#include "string.h"
#include "timer.h"

struct Sprite;

struct Rect32 {
    int left;
    int top;
    int right;
    int bottom;
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
    void (*field_18)(struct Indicator *); /* the check function */
    void (*field_1c)(struct Indicator *); /* the click function */
    unsigned char pad_20[0x22 - 0x20];
    unsigned char field_22;
    unsigned char pad_23[0x28 - 0x23];
};

struct ListElement {
    /* 0x00 */ unsigned char pad_0[8];
    /* 0x08 */ unsigned char field_8;
    /* 0x09 */ unsigned char pad_9[0xc - 0x9];
    /* 0x0c */ int *field_c;
};

struct InfoSource {
    /* 0x00 */ unsigned char pad_0[4];
    /* 0x04 */ void *field_4;
    /* 0x08 */ void *field_8;
    /* 0x0c */ unsigned char pad_c[0x10 - 0xc];
    /* 0x10 */ char *field_10; /* the icon string */
    /* 0x14 */ struct Sprite *field_14;
    /* 0x18 */ unsigned char pad_18[0x68 - 0x18];
    /* 0x68 */ struct Sprite *field_68;
    /* 0x6c */ unsigned char pad_6c[0x78 - 0x6c];
    /* 0x78 */ void *field_78;
    /* 0x7c */ void *field_7c;
    /* 0x80 */ unsigned char pad_80[0xc4 - 0x80];
    /* 0xc4 */ unsigned int *field_c4;
};

#include "bricks.h"
#include "freeplay.h"
#include "image_sprite.h"
#include "objclass.h"
#include "popupinfo.h"
#include "render3d.h"
#include "screens.h"
#include "sound_music.h"
#include "text.h"

/* Sets CurrentHelpKind to 4 when ShowNextObjectiveMessage starts an objective message's speech
 * (UpdateSpeechPlayback only replays kinds 0-2). */
// FUNCTION: LEGOLAND 0x0046d3a0
void SetHelpKindObjectiveMessage(void) { CurrentHelpKind = 4; }

// FUNCTION: LEGOLAND 0x0046d3c0
void FreeIcon(struct IconNode *node) {
    if (!node) {
        return;
    }
    if (node->sprite) {
        KillSprite(node->sprite);
        node->sprite = NULL;
    }
    if (node->flags & 0x80) {
        if (node->field_30) {
            free(node->field_30);
        }
    }
    free(node);
    if (FocussedIconPtr == node) {
        FocussedIconPtr = NULL;
    }
    if (Hover.type == 2 && Hover.ptr == (struct Bloke *)node) {
        Hover.ptr = 0;
        Hover.type = 0x100;
    }
}

// FUNCTION: LEGOLAND 0x0046d440
void AppendIcon(struct IconNode *node) {
    ((struct IconNode *)IconListTail)->next = node;
    IconListTail = node;
}

// FUNCTION: LEGOLAND 0x0046d460
void UnlinkIconAtSlot(struct IconNode **slot) {
    struct IconNode *node = *slot;
    struct IconNode *next;
    if (FocussedIconPtr == node->next) {
        FocussedIconPtr = NULL;
    }
    if (IconListTail == *slot) {
        IconListTail = (struct IconNode *)slot;
    }
    node = *slot;
    next = node->next;
    FreeIcon(node);
    *slot = next;
}

// FUNCTION: LEGOLAND 0x0046d4a0
void FreeIconAtSlot(struct IconNode **slot) {
    struct IconNode *node = *slot;
    struct IconNode *next;
    if (FocussedIconPtr == node->next) {
        FocussedIconPtr = NULL;
    }
    if (HelpIconListTail == *slot) {
        HelpIconListTail = slot;
    }
    node = *slot;
    next = node->next;
    FreeIcon(node);
    *slot = next;
}

// FUNCTION: LEGOLAND 0x0046d4e0
void RemoveIconFromList(struct IconNode *node) {
    struct IconNode *cur;
    struct IconNode *next;
    if (IconListHead == node) {
        UnlinkIconAtSlot(&IconListHead);
    } else {
        cur = IconListHead;
        while (cur != NULL && (next = cur->next) != node) {
            cur = next;
        }
        if (cur != NULL) {
            UnlinkIconAtSlot((struct IconNode **)cur);
        }
    }
}

// FUNCTION: LEGOLAND 0x0046d520
LEGO_EXPORT void RemoveIconGroup(unsigned short group) {
    struct IconNode **slot;
    struct IconNode *node;
    int removed = 0;

    slot = &IconListHead;
    node = IconListHead;
    while (node != NULL) {
        if (node->id == group) {
            node = node->next;
            UnlinkIconAtSlot(slot);
            removed++;
        } else {
            slot = &node->next;
            node = node->next;
        }
    }
    slot = &HelpIconListHead;
    node = HelpIconListHead;
    while (node != NULL) {
        if (node->id == group) {
            node = node->next;
            FreeIconAtSlot(slot);
            removed++;
        } else {
            slot = &node->next;
            node = node->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x0046d590
void RemoveIconRange(unsigned short val) {
    struct IconNode **slot;
    struct IconNode *node;
    int removed = 0;

    slot = &IconListHead;
    node = IconListHead;
    while (node != NULL) {
        if (node->id < val || (int)node->id >= val + 7) {
            slot = &node->next;
            node = node->next;
        } else {
            node = node->next;
            UnlinkIconAtSlot(slot);
            removed++;
        }
    }
    slot = &HelpIconListHead;
    node = HelpIconListHead;
    while (node != NULL) {
        if (node->id < val || (int)node->id >= val + 7) {
            slot = &node->next;
            node = node->next;
        } else {
            node = node->next;
            FreeIconAtSlot(slot);
            removed++;
        }
    }
}

// FUNCTION: LEGOLAND 0x0046d630
LEGO_EXPORT struct IconNode *FindIcon(unsigned short id) {
    struct IconNode *node = (struct IconNode *)IconListHead;
    while (node) {
        if (node->id == id && (node->flags & 0x100) == 0) {
            return node;
        }
        node = node->next;
    }
    node = (struct IconNode *)HelpIconListHead;
    while (node) {
        if (node->id == id && (node->flags & 0x100) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0046d680
void SetIconSprite(struct IconNode *node, struct Sprite *sprite) {
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
LEGO_EXPORT struct IconNode *InsertIcon(short a1, short a2, int a3, struct Sprite *sprite) {
    struct IconNode *icon = (struct IconNode *)malloc(sizeof(struct IconNode));
    if (icon != NULL) {
        *icon = DefaultIconNode;
        icon->x = a1;
        icon->y = a2;
        icon->id = (unsigned short)a3;
        ReferenceSprite(sprite);
        icon->sprite = sprite;
        if (sprite != NULL) {
            short h = *(volatile short *)&sprite->height;
            short w = *(volatile short *)&sprite->width;
            icon->height = h;
            icon->width = w;
            icon->flags = icon->flags | 0x10;
        }
    }
    AppendIcon(icon);
    return icon;
}

// FUNCTION: LEGOLAND 0x0046d740
LEGO_EXPORT void SetNewGroup_Callbacks(void *param_1, void *param_2, void *param_3) {
    NewIconGroupCallbacks = param_1;
    NewGroupRenderFunc = param_2;
    NewGroupEventHandler = param_3;
}

// FUNCTION: LEGOLAND 0x0046d760
LEGO_EXPORT struct IconNode *AddFullScreenIcon(int icon) {
    struct IconNode *result = InsertIcon(0, 0, icon, 0);
    if (result) {
        result->width = ((struct Config *)lpConfig)->field_0;
        result->height = ((struct Config *)lpConfig)->field_2;
        result->render_func = (void *)SetClippingToScreenRect;
        result->flags = result->flags | 0x29;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0046d7b0
LEGO_EXPORT struct IconNode *LoadSpriteIcon(const char *filename, unsigned int param_2, short param_3, short param_4, int param_5) {
    struct IconNode *result = NULL;
    struct Sprite *sprite = LoadSprite(filename, param_2);
    if (sprite != NULL) {
        result = InsertIcon(param_3, param_4, param_5, sprite);
    }
    KillSprite(sprite);
    return result;
}

// FUNCTION: LEGOLAND 0x0046d850
void ScrollIconRegion(struct InterfacePanel *r, int param_2, int param_3) {
    int dy;
    int dx;
    int l;
    int t;
    int nl;
    int nt;
    int edge;

    int field_0;
    int content_bottom;
    int clip_left;
    if ((1 & r->field_4) != 0) {
        if (r->content_bottom - r->content_top <= r->clip_bottom - r->clip_top) {
            MoveIcons(0xffff, r->group, 0, (short)(r->clip_top - r->content_top));
            return;
        }
    } else {
        if (r->field_14 - r->content_left <= r->clip_right - r->clip_left) {
            MoveIcons(0xffff, r->group, (short)(r->clip_left - r->content_left), 0);
            return;
        }
    }

    dy = GetIconScrollSnapOffset(0xffff, (short)((r->clip_top - r->content_top) - param_3), (short)(r->content_top + param_3), r->group, param_3);
    l = r->content_left;
    dx = param_2;
    t = r->content_top;
    nl = l + dx;
    nt = t + dy;
    if (!((1 & r->field_4) != 0)) {
        if (!(nl > r->clip_left)) {
            edge = dx + r->field_14;
            if (edge < r->clip_right) {
                dx = dx + (r->clip_right - edge);
            }
        } else {
            clip_left = r->clip_left;
            dx = dx + (clip_left - nl);
        }
    } else {
        if (!(nt > r->clip_top)) {
            edge = r->content_bottom + dy;
            if (edge < r->clip_bottom) {
                dy = dy + (r->clip_bottom - edge);
            }
        } else {
            dy = dy + (r->clip_top - nt);
        }
    }
    r->content_left = dx + l;
    r->content_top = t + dy;
    r->field_14 = r->field_14 + dx;
    content_bottom = r->content_bottom;
    r->content_bottom = content_bottom + dy;
    BuildPanelScrollOffsets[BuildPanelScrollIndex] = BuildPanelScrollOffsets[BuildPanelScrollIndex] + dy;
    field_0 = r->group;
    MoveIcons(0xffff, field_0, (short)dx, (short)dy);
}

// FUNCTION: LEGOLAND 0x0046d980
unsigned char ScrollIconRegionForward(struct IconNode *icon, unsigned char buttons, int a3, int a4) {
    struct InterfacePanel *region;
    if ((buttons & 1) != 0) {
        do {
            region = (struct InterfacePanel *)icon->field_30;
            LastScrollIconTick = GetTickCount();
            if ((region->field_4 & 1) != 0) {
                ScrollIconRegion(region, 0, 6);
            } else {
                ScrollIconRegion(region, 0x20, 0);
            }
        } while (0);
        return 2;
    }
    if ((buttons & 4) != 0) {
        do {
            if (GetTickCount() - LastScrollIconTick >= 0xfa) {
                region = (struct InterfacePanel *)icon->field_30;
                LastScrollIconTick = GetTickCount();
                if ((region->field_4 & 1) != 0) {
                    ScrollIconRegion(region, 0, 6);
                } else {
                    ScrollIconRegion(region, 0x20, 0);
                }
            }
        } while (0);
        return 2;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046da20
unsigned char ScrollIconRegionBack(struct IconNode *icon, unsigned char buttons, int a3, int a4) {
    struct InterfacePanel *region;
    if ((buttons & 1) != 0) {
        region = (struct InterfacePanel *)icon->field_30;
        LastScrollIconTick = GetTickCount();
        if ((region->field_4 & 1) != 0) {
            ScrollIconRegion(region, 0, -6);
            return 2;
        }
        ScrollIconRegion(region, -0x20, 0);
        return 2;
    }
    if ((buttons & 4) != 0) {
        if (GetTickCount() - LastScrollIconTick >= 0xfa) {
            region = (struct InterfacePanel *)icon->field_30;
            LastScrollIconTick = GetTickCount();
            if ((region->field_4 & 1) != 0) {
                ScrollIconRegion(region, 0, -6);
            } else {
                ScrollIconRegion(region, -0x20, 0);
            }
        }
        return 2;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046dac0
void ScrollActiveIconRegionForward(void) {
    unsigned int v;
    struct IconNode *icon;
    if (EditMode.unk4 == 2 && FrontEndScreenMode.unk8 == 3) {
        int field = CONTROLLERBUFFER->x;
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
        ScrollIconRegionForward(icon, 1, 0, 0);
    }
}

// FUNCTION: LEGOLAND 0x0046db40
void ScrollActiveIconRegionBack(void) {
    unsigned int v;
    struct IconNode *icon;
    if (EditMode.unk4 == 2 && FrontEndScreenMode.unk8 == 3) {
        int field = CONTROLLERBUFFER->x;
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
        ScrollIconRegionBack(icon, 1, 0, 0);
    }
}

// FUNCTION: LEGOLAND 0x0046dbc0
LEGO_EXPORT struct IconNode *AddGBarIcons(struct InterfacePanel *param_1, unsigned int param_2, int param_3, int param_4, int param_5, int param_6) {
    struct IconNode *icon;

    AddFullScreenIcon(param_6 + 6);
    icon = InsertIcon(param_2, param_3, param_6 + 3, IfSideBUpSprite);
    icon->field_30 = param_1;
    icon->string_id = 0x95;
    icon->string = GetString(0x95);
    icon->height = 0x1e;
    icon->flags = icon->flags | 0x2013;
    icon->event_handler = (void *)ScrollIconRegionForward;
    icon = InsertIcon(param_2, param_5 - 0x1e + param_3, param_6 + 4, IfSideBDownSprite);
    icon->field_30 = param_1;
    icon->string_id = 0x94;
    icon->string = GetString(0x94);
    icon->height = 0x1e;
    icon->flags = icon->flags | 0x2013;
    icon->event_handler = (void *)ScrollIconRegionBack;
    icon = InsertIcon(param_2, param_3 + 0x1e, param_6 + 5, 0);
    icon->field_30 = param_1;
    icon->width = 0x79;
    icon->height = (short)param_5 - 0x3c;
    icon->box.box_color = GetNearestColour(0, 10, 0x14);
    icon->flags = icon->flags | 0x29;
    icon->render_func = (void *)RenderBoxIcon;
    return icon;
}

// FUNCTION: LEGOLAND 0x0046dcd0
LEGO_EXPORT void MoveIcons(unsigned short mask, unsigned short id, short dx, short dy) {
    struct IconNode *node;
    for (node = IconListHead; node != NULL; node = node->next) {
        if ((node->flags & 1) == 0 && (node->id & mask) == id) {
            node->x = node->x + dx;
            node->y = node->y + dy;
        }
    }
}

/* Finds the next eligible icon edge in the requested vertical scroll direction and returns the offset needed to
 * align it with the reference edge. Filters icon IDs with the supplied mask and skips flag-1 icons; returns zero
 * when no snap is needed. */
// FUNCTION: LEGOLAND 0x0046dd10
int GetIconScrollSnapOffset(unsigned short param_1, short param_2, short param_3, unsigned short param_4, int param_5) {
    struct IconNode *best = NULL;
    struct IconNode *node = IconListHead;
    int extreme;
    int v;

    if (param_5 > 0) {
        extreme = -100000;
        if (node == NULL) {
            return 0;
        }
        do {
            if ((node->flags & 1) == 0 && (node->id & param_1) == param_4 &&
                (v = (int)node->y, (v - param_3) + param_5 <= (int)param_2 && v > extreme)) {
                best = node;
                extreme = v;
            }
            node = node->next;
        } while (node != NULL);
    } else {
        if (param_5 >= 0) {
            return 0;
        }
        extreme = 100000;
        if (node == NULL) {
            return 0;
        }
        do {
            if ((node->flags & 1) == 0 && (node->id & param_1) == param_4 &&
                (v = (int)node->y, (v - param_3) + param_5 > (int)param_2) && v < extreme) {
                best = node;
                extreme = v;
            }
            node = node->next;
        } while (node != NULL);
    }
    if (best != NULL && best->y != param_2) {
        return ((int)param_2 - (int)best->y) + (int)param_3;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046de50
void GetIconRect(struct IconNode *src, struct Rect32 *dst) {
    dst->left = src->x;
    dst->top = src->y;
    dst->right = src->width + src->x;
    dst->bottom = src->height + src->y;
}

// FUNCTION: LEGOLAND 0x0046de90
void GetIconBBox(struct IconNode *icon, struct Bbox *bbox) {
    GetIconRect((struct IconNode *)icon, (struct Rect32 *)bbox);
    if (icon->flags & 0x20) {
        bbox->min_x = bbox->min_x - 3;
        bbox->max_x = bbox->max_x + 3;
        bbox->min_y = bbox->min_y - 3;
        bbox->max_y = bbox->max_y + 3;
    }
    if (icon->flags & 0x40) {
        struct Indicator *ind;
        bbox->min_y = bbox->min_y - 0x16;
        ind = (struct Indicator *)icon->field_30;
        if (ind != NULL) {
            if (ind->field_22 & 8) {
                bbox->max_y = bbox->max_y + IconBBoxExtraHeight;
            }
            if (ind->field_22 & 2) {
                bbox->max_x = bbox->max_x + IconBBoxExtraWidth;
            }
        }
    }
    if (icon->flags & 0x200) {
        int v = icon->field_22 + icon->y;
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
int SetClippingToIcon(struct IconNode *src) {
    RECT rect;
    GetIconRect(src, (struct Rect32 *)&rect);
    SetClipping(&rect);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046df60
int SetClippingToScreenRect(int param) {
    SetClipping(&ScreenRect);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046df70
LEGO_EXPORT int RenderBoxIcon(struct IconNode *node) {
    unsigned short border = node->box.border;
    RenderThickBox(node->x - border, node->y - border,
        node->width + border * 2, node->height + border * 2,
        border, node->box.box_color);
    if ((node->flags & 0x20) != 0) {
        SetClippingToIcon((struct IconNode *)node);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046dfd0
int RenderIconWithText(struct IconNode *node) {
    struct PrintCtx ctx;

    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    if (node->sprite != NULL) {
        PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
    }
    PrintCent(node->field_20 + node->x, node->field_22 + node->y, 0x40, node->field_18p, 1);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e040
int RenderFocusableIcon(struct IconNode *node) {
    struct PrintCtx ctx;

    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    if (node == (struct IconNode *)FocussedIconPtr) {
        PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
        return 0;
    }
    PrintSprite(node->alt_sprite, node->x, node->y, 0, (int *)&ctx);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e0a0
LEGO_EXPORT int RenderBuildObjectIcon(struct IconNode *node) {
    struct PrintCtx ctx = {2, node};
    int cost;
    unsigned int *objsub;

    if (EditMode.unk0 == 1 && EditMode.unk8 != 0) {
        objsub = (unsigned int *)((struct Ride *)node->field_8)->element;
        if ((unsigned int *)EditMode.unk8->element == objsub) {
            objsub[2] = objsub[2] & 0xfffdffff;
        }
    }
    if (node->y > 0 && node->y < 0x174) {
        char buf[12];
        do {
            cost = GetObjCost(node->field_8);
            if (node->sprite != NULL) {
                if (cost <= GetBrickCount()) {
                    PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
                } else {
                    PrintSprite(node->sprite, node->x, node->y, UnaffordableIconColour, (int *)&ctx);
                }
            }
            if (node->field_8 != NULL) {
                // STRING: LEGOLAND 0x004b8a80
                sprintf(buf, "%d", cost);
                PrintTextBox(buf, node->x + 0x3c, node->y + 0x14, 0x41, 0x14, 1, 1, 0, 0xffffff);
            }
            switch (node->field_20b) {
            case 1:
                PrintSprite(LinkMiddleSprite, node->x, node->y, 0, 0);
                break;
            case 2:
                PrintSprite(LinkBottomSprite, node->x, node->y, 0, 0);
                break;
            }
            if (EditMode.unk0 == 1 && EditMode.unk8 != 0) {
                // STRING: LEGOLAND 0x004ba888
                if (strcmp("Path", EditMode.unk8->name) != 0 &&
                    EditMode.unk8->element == ((struct Ride *)node->field_8)->element) {
                    struct Sprite *s = AttractHighlightOnSprite;
                    if (GetBlink() != 0) {
                        s = AttractHighlightOffSprite;
                    }
                    PrintSprite(s, node->x, node->y, 0, 0);
                }
            }
            if (((struct Ride *)node->field_8)->element->flags & 0x20000) {
                PrintSprite(GetBlink() != 0 ? AttractNewOffSprite : AttractNewOnSprite, node->x, node->y, 0, 0);
            }
        } while (0);
    }
    if (((struct Ride *)node->field_8)->element->flags & 0x20000) {
        if (node->y < 0x3e) {
            NewObjectScrollHintFlags = NewObjectScrollHintFlags | 1;
            return 0;
        }
        if (0x156 < node->height + node->y) {
            NewObjectScrollHintFlags = NewObjectScrollHintFlags | 2;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e300
LEGO_EXPORT int RenderFreePlayIcons(struct IconNode *node) {
    struct PrintCtx ctx;
    short y;
    struct Sprite *sprite;

    ctx.flags = 2;
    ctx.node = node;
    y = node->y;
    ctx.field_8 = 0;
    if (y >= 0 && y <= 0x12c) {
        if ((char)node->field_18 != 1) {
            sprite = node->sprite;
            if (sprite != NULL) {
                GetFreePlayObjectCost((const char *)node->field_1c, 0);
                if (CanAffordFreePlayObject(node->field_1c, node->field_20p) != 0) {
                    PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
                    return 0;
                }
                PrintSprite(node->sprite, node->x, node->y, 0xff000000, (int *)&ctx);
                return 0;
            }
        } else {
            sprite = node->sprite;
            if (sprite != NULL) {
                PrintSprite(sprite, node->x, y, 0, (int *)&ctx);
            }
            PrintSprite(FreePlayTickSprite, node->x + 0x46, node->y, 0, (int *)&ctx);
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
        PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
    }
    if (GetBlink() != 0) {
        if ((NewObjectScrollHintFlags & 2) != 0 && (char)node->field_18 == 2) {
            PrintSprite(SideScrollDownLitSprite, node->x, node->y, 0, (int *)&ctx);
            NewObjectScrollHintFlags = NewObjectScrollHintFlags & 0xfffffffd;
            return 0;
        }
        if ((NewObjectScrollHintFlags & 1) != 0 && (char)node->field_18 == 1) {
            PrintSprite(SideScrollUpLitSprite, node->x, node->y, 0, (int *)&ctx);
            NewObjectScrollHintFlags = NewObjectScrollHintFlags & 0xfffffffe;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e4d0
LEGO_EXPORT int RenderEnergyBar(struct IconNode *node) {
    struct PrintCtx ctx = {1};
    int fill1;
    int fill2;
    int scale;
    RECT clip;

    if (MapStats.power_supply == 0 && MapStats.power_demand == 0) {
        fill1 = 0;
        fill2 = 0;
    } else {
        scale = MapStats.power_demand * 2;
        if (MapStats.power_supply > scale) {
            scale = MapStats.power_supply;
        }
        if (scale == 0) {
            scale = 1;
        }
        fill1 = (node->width * MapStats.power_supply) / scale;
        fill2 = (node->width * MapStats.power_demand) / scale;
    }
    if (node->sprite != NULL && MapStats.power_enabled != 0) {
        StoreClipping();
        if (fill1 > EnergyBarSupplyWidth) {
            EnergyBarSupplyWidth += 6;
            if (EnergyBarSupplyWidth > fill1) {
                EnergyBarSupplyWidth = fill1;
            }
        } else {
            EnergyBarSupplyWidth -= 6;
            if (EnergyBarSupplyWidth < fill1) {
                EnergyBarSupplyWidth = fill1;
            }
        }
        if (fill2 > PowerDemandBarPos) {
            PowerDemandBarPos += 6;
            if (PowerDemandBarPos > fill2) {
                PowerDemandBarPos = fill2;
            }
        } else {
            PowerDemandBarPos -= 6;
            if (PowerDemandBarPos < fill2) {
                PowerDemandBarPos = fill2;
            }
        }
        clip.left = node->x;
        clip.top = node->y;
        clip.right = EnergyBarSupplyWidth + clip.left;
        clip.bottom = node->height + clip.top;
        SetClipping(&clip);
        PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
        RestoreClipping();
        PrintSprite(BarPointerSprite, (node->x - BarPointerSprite->width / 2) + PowerDemandBarPos, node->height / 2 + node->y, 0, (int *)&ctx);
        return 0;
    }
    if (MapStats.power_enabled == 0) {
        PrintSprite(NoEnergySprite, node->x - 0x15, node->y - 6, 0, (int *)&ctx);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e670
LEGO_EXPORT int RenderMoneyBar(struct IconNode *node) {
    struct PrintCtx ctx = {1};
    int fill;
    int width;
    int bricks;
    RECT clip;
    char buf[100];

    if (AreBricksLimited() == 0) {
        return 0;
    }
    width = node->width;
    bricks = GetBrickCount();
    fill = (bricks * width) / (int)MapStats.brick_meter_max;
    if (fill < 0) {
        fill = 0;
    }
    if (fill > width) {
        fill = width;
    }
    if (fill > MoneyBarShownWidth) {
        MoneyBarShownWidth += 6;
        if (MoneyBarShownWidth > fill) {
            MoneyBarShownWidth = fill;
        }
    } else {
        MoneyBarShownWidth -= 6;
        if (MoneyBarShownWidth < fill) {
            MoneyBarShownWidth = fill;
        }
    }
    StoreClipping();
    clip.left = node->x;
    clip.right = MoneyBarShownWidth + clip.left;
    clip.top = node->y;
    clip.bottom = node->height + clip.top;
    SetClipping(&clip);
    if (node->sprite != NULL) {
        PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
    }
    RestoreClipping();
    bricks = GetBrickCount();
    if (bricks < 0) {
        bricks = 0;
    }
    // STRING: LEGOLAND 0x004ba890
    sprintf(buf, "%5d", bricks);
    PrintTextBox(buf, node->x + 10 + node->width, node->y - 3, 200, node->height, 0, 0, 0xff0000, 0xffffff);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e7b0
LEGO_EXPORT int RenderFreePlayBar(struct IconNode *node) {
    struct PrintCtx ctx;
    RECT clip;

    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    StoreClipping();
    clip.left = node->x;
    clip.right = (int)(node->width * 0.00005f * (int)FreePlayCostTotal);
    clip.right += clip.left;
    clip.top = node->y;
    clip.bottom = node->height + clip.top;
    SetClipping(&clip);
    if (node->sprite != NULL) {
        PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
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
        PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
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
            PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
            return 0;
        }
        PrintSprite(node->sprite, node->x, node->y, 0xff000000, (int *)&ctx);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e920
int RenderBlinkIcon(struct IconNode *node) {
    struct PrintCtx ctx;
    struct LLS *lls;
    int frame;

    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    if (node->sprite != NULL) {
        if (GetBlink() != 0) {
            PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
        } else {
            struct Sprite *alt = node->alt_sprite;
            if (alt != NULL) {
                lls = GetLLSForSprite((struct Sprite *)node->sprite);
                if (lls != NULL) {
                    frame = *(short *)lls;
                } else {
                    frame = 0;
                }
                lls = GetLLSForSprite((struct Sprite *)alt);
                if (lls != NULL) {
                    LLSSetFrame(lls, frame);
                }
                PrintSprite(alt, node->x, node->y, 0, (int *)&ctx);
                return 0;
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046e9d0
LEGO_EXPORT int RenderGBarSprite(struct IconNode *node) {
    struct PrintCtx ctx = {1};
    if (node->sprite != NULL) {
        PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046ea10
int RenderPanelIcon(struct IconNode *node) {
    struct PrintCtx ctx;
    RECT rect;
    RECT out;

    ctx.flags = 2;
    ctx.node = node;
    rect.left = (LONG)node->x;
    rect.top = (LONG)node->y;
    rect.right = node->width + rect.left;
    rect.bottom = node->height + rect.top;
    ctx.field_8 = 0;
    if (IntersectRect(&out, &rect, (RECT *)&((struct InterfacePanel *)node->field_30)->clip_left) != 0 && node->sprite != NULL) {
        PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046eaa0
int RenderBlinkingIcon(struct IconNode *node) {
    struct PrintCtx ctx;

    ctx.flags = 2;
    ctx.node = node;
    ctx.field_8 = 0;
    if (GetBlink() != 0) {
        PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046eaf0
LEGO_EXPORT struct IconNode *SetupInterfacePanelIcons(struct InterfacePanel *param_1, int param_2, int param_3, int param_4, int param_5, int param_6) {
    struct IconNode *icon;

    NewObjectScrollHintFlags = 0;
    AddFullScreenIcon(param_6 + 6);
    icon = InsertIcon(param_2 - 3, 0x20, param_6, IfSidebar1Sprite);
    icon->render_func = (void *)RenderGBarSprite;
    icon->field_18 = 10;
    icon->flags = icon->flags | 9;
    icon = InsertIcon(param_2 - 1, param_3, param_6 + 3, IfSideBUpSprite);
    icon->field_30 = param_1;
    icon->string_id = 0x95;
    icon->string = GetString(0x95);
    icon->width = 0x79;
    icon->height = 0x1e;
    icon->flags = icon->flags | 0x201b;
    icon->render_func = (void *)RenderScroll_Icons;
    icon->event_handler = (void *)ScrollIconRegionForward;
    icon->field_18 = 1;
    icon = InsertIcon(param_2 - 1, param_5 - 0x20 + param_3, param_6 + 4, IfSideBDownSprite);
    icon->field_30 = param_1;
    icon->string_id = 0x94;
    icon->string = GetString(0x94);
    icon->width = 0x79;
    icon->height = 0x1e;
    icon->flags = icon->flags | 0x201b;
    icon->render_func = (void *)RenderScroll_Icons;
    icon->event_handler = (void *)ScrollIconRegionBack;
    icon->field_18 = 2;
    icon = InsertIcon(param_2, param_3 + 0x1e, param_6 + 5, 0);
    icon->field_30 = param_1;
    icon->width = 0x79;
    icon->height = (short)param_5 - 0x3e;
    icon->box.box_color = GetNearestColour(0, 10, 0x14);
    icon->flags = icon->flags | 0x29;
    icon->render_func = (void *)RenderBoxIcon;
    return icon;
}

/* Moves build-side-panel icons by the supplied pixel step while opening or closing. Reserves/releases the 121-pixel
 * map viewport strip and adjusts scroll; destroys the panel group once fully closed. */
// FUNCTION: LEGOLAND 0x0046ec50
void AnimateBuildSidePanel(char param) {
    char saved_mode = BuildSidePanelState;
    struct IconNode *node = IconListHead;

    if (BuildSidePanelState == 2 || BuildSidePanelState == 3) {
        return;
    }
    while (node != NULL) {
        short id = (short)node->id;
        if (id == 0xd2 || id == 0xd5 || id == 0xd6 || id == 0xd7) {
            if (BuildSidePanelState == 0) {
                int old = node->x;
                node->x += param;
                if ((char)node->field_18 == 0xa && node->x >= 3) {
                    node->x = 0;
                    param = (char)node->x - (char)old;
                    BuildSidePanelOriginX = 3;
                    saved_mode = 3;
                    lpConfig->view_x = 0x79;
                    ScrollX = ScrollX + 0x7900;
                    lpConfig->view_width = 0x207;
                    BGFullUpdate = 1;
                    BuildSidePanelViewportReserved = 1;
                }
            } else if (BuildSidePanelState == 1) {
                if (BuildSidePanelViewportReserved != 0) {
                    lpConfig->view_x = 0;
                    ScrollX = ScrollX - 0x7900;
                    lpConfig->view_width = 0x280;
                    BuildSidePanelViewportReserved = 0;
                    BGFullUpdate = 1;
                    return;
                }
                if ((char)node->field_18 == 0xa) {
                    int diff = (int)(short)node->x - (int)param;
                    if (diff < -0x7a) {
                        BuildSidePanelState = 2;
                        saved_mode = 2;
                        BuildSidePanelOriginX = 0x86;
                        DestroyIconGroup(0xd2);
                        break;
                    }
                    diff = diff + 0x7a;
                    ClampScrollPosition((lpConfig->view_width - diff) * 0x100, lpConfig->view_height << 8, diff * 0x100, 0);
                }
                node->x = (short)node->x - (short)param;
            }
        }
        node = node->next;
    }

    BuildSidePanelState = saved_mode;
}

// FUNCTION: LEGOLAND 0x0046ee00
void UpdateEditModeButtonSprites(void) {
    struct IconNode *node = IconListHead;
    int mode;

    if (EditMode.unk4 == 1) {
        mode = 4;
    } else {
        switch (EditMode.unk0) {
        case 1:
            if (EditMode.unk8 != 0) {
                if (strcmp("Path", EditMode.unk8->name) == 0) {
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
        if (node->id == 0x93 && node->field_1c != NULL) {
            void *sprite = node->field_20p;
            if (sprite != NULL) {
                if (node->field_18 == mode) {
                    SetIconSprite(node, node->field_1c);
                } else {
                    SetIconSprite(node, sprite);
                }
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0046eee0
LEGO_EXPORT void RenderIcons(void) {
    struct PrintCtx ctx = {2};
    struct IconNode *node = (struct IconNode *)IconListHead;
    int elapsed;

    StoreClipping();
    SetClippingToScreenRect(0);
    elapsed = GetTicks() - RenderIconsLastTicks;
    if (elapsed > 0x3de) {
        elapsed = 0x3de;
    }
    AnimateBuildSidePanel(elapsed * 5 / 33);
    RenderIconsLastTicks = GetTicks();
    while (node) {
        if ((node->flags & 0x400) == 0) {
            if (node->flags & 0x8) {
                ((void (*)(struct IconNode *))node->render_func)(node);
            } else if (node->sprite) {
                ctx.node = node;
                PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
            }
            if (node == (struct IconNode *)FocussedIconPtr) {
                if (node->sprite) {
                    struct SpriteAnim *anim = ((struct Sprite *)node->sprite)->anim;
                    if (anim->type == 3 || anim->type == 2) {
                        LLSPlayOnce(anim->lls, anim);
                    }
                }
                if (node->flags & 0x8000) {
                    unsigned int colour = GetNearestColour(0, 0xff, 0);
                    RenderThickBox(node->x, node->y, node->width, node->height, 2, colour);
                }
            }
        }
        node = node->next;
    }
    DrawFlashingButtons();
    RestoreClipping();
}

// FUNCTION: LEGOLAND 0x0046f010
LEGO_EXPORT void RenderIcons2(short param_1, short param_2, short param_3) {
    struct PrintCtx ctx = {2};
    struct IconNode *node = (struct IconNode *)IconListHead;

    StoreClipping();
    SetClippingToScreenRect(0);
    while (node) {
        if ((node->flags & 0x400) == 0 && node->y > 0 && node->y < 0x1e0 &&
            ((short)node->id == param_1 || (short)node->id == param_2 || (short)node->id == param_3)) {
            if (node->flags & 0x8) {
                ((void (*)(struct IconNode *))node->render_func)(node);
            } else if (node->sprite) {
                ctx.node = node;
                PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
            }
        }
        if (node == (struct IconNode *)FocussedIconPtr && node->sprite) {
            struct SpriteAnim *anim = ((struct Sprite *)node->sprite)->anim;
            if (anim->type == 3 || anim->type == 2) {
                LLSPlayOnce(anim->lls, anim);
            }
        }
        node = node->next;
    }
    RestoreClipping();
}

/* Updates the sliding-panel animation, renders visible icons except the supplied ID, advances the focused sprite
 * animation, and draws flashing buttons with screen clipping. */
// FUNCTION: LEGOLAND 0x0046f100
void RenderIconsExceptId(short param_1) {
    struct PrintCtx ctx = {2};
    struct IconNode *node;
    int elapsed;

    elapsed = GetTicks() - RenderIconsExceptIdLastTicks;
    if (elapsed > 0x3de) {
        elapsed = 0x3de;
    }
    AnimateBuildSidePanel(elapsed * 5 / 0x21);
    RenderIconsExceptIdLastTicks = GetTicks();
    node = IconListHead;
    StoreClipping();
    SetClippingToScreenRect(0);
    for (; node != NULL; node = node->next) {
        if ((node->flags & 0x400) == 0 && (short)node->id != param_1) {
            if (node->flags & 0x8) {
                ((void (*)(struct IconNode *))node->render_func)(node);
            } else if (node->sprite) {
                ctx.node = node;
                PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
            }
        }
        if (node == (struct IconNode *)FocussedIconPtr && node->sprite) {
            struct SpriteAnim *anim = ((struct Sprite *)node->sprite)->anim;
            if (anim->type == 3 || anim->type == 2) {
                LLSPlayOnce(anim->lls, anim);
            }
        }
    }
    SetClippingToScreenRect(0);
    DrawFlashingButtons();
    RestoreClipping();
}

// FUNCTION: LEGOLAND 0x0046f200
LEGO_EXPORT void RenderHelpIcons(void) {
    struct PrintCtx ctx = {2};
    struct IconNode *node = HelpIconListHead;
    StoreClipping();
    SetClippingToScreenRect(0);
    for (; node != NULL; node = node->next) {
        if ((node->flags & 0x400) == 0 && node->x < 0x1e0 && node->x > 0) {
            if ((node->flags & 8) != 0) {
                ((void (*)(struct IconNode *))node->render_func)(node);
            } else if (node->sprite != NULL) {
                ctx.node = node;
                PrintSprite(node->sprite, node->x, node->y, 0, (int *)&ctx);
            }
            if (node == (struct IconNode *)FocussedIconPtr && (node->flags & 0x8000) != 0) {
                unsigned int colour = GetNearestColour(0, 0xff, 0);
                RenderThickBox(node->x, node->y, node->width, node->height, 2, colour);
            }
        }
    }
    RestoreClipping();
}

/* Returns two for button bits one or four and one for other events; the node and pointer offsets are unused. */
// DefaultIconEventHandler: the icons' default event handler: 2 if the event has button 1 or 4 (bits 0x1 / 0x4), else 1.
// FUNCTION: LEGOLAND 0x0046f2e0
unsigned char DefaultIconEventHandler(struct IconNode *node, unsigned char buttons, short dx, short dy) {
    /* buttons: 1 left, 2 right, 4 middle (UpdateControllerFromMouseData) */
    if (buttons & 1) {
        return 2;
    }
    if (buttons & 4) {
        return 2;
    }
    if (buttons & 2) {
        return 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046f300
int PointInBBox(struct Point *point, struct Bbox *bbox) {
    return point->x >= bbox->min_x && point->x <= bbox->max_x &&
        point->y >= bbox->min_y && point->y <= bbox->max_y;
}

// FUNCTION: LEGOLAND 0x0046f330
int IsPointInIcon(struct Point *point, struct IconNode *icon) {
    struct Bbox bbox;
    GetIconBBox(icon, &bbox);
    return PointInBBox(point, &bbox);
}

// FUNCTION: LEGOLAND 0x0046f360
LEGO_EXPORT struct IconNode *GetIconAtPos(struct Point *param_1, unsigned char *param_2) {
    short x;
    short y;
    struct IconNode *found;
    struct IconNode *cur;
    int doGeom;
    int i;
    unsigned int flags;
    struct Bbox bbox;

    found = NULL;
    doGeom = 1;
    x = (short)param_1->x;
    y = (short)param_1->y;
    for (i = 0; i < 2; i++) {
        for (cur = i != 0 ? HelpIconListHead : IconListHead; cur != NULL; cur = cur->next) {
            if (doGeom) {
                flags = cur->flags;
                if ((flags & 0x10) != 0 && (flags & 0x400) == 0 && x >= cur->x && y >= cur->y &&
                    x <= cur->width + cur->x && y <= cur->height + cur->y) {
                    found = cur;
                } else if ((flags & 0x40) != 0 && (flags & 0x400) == 0 && IsPointInIcon(param_1, cur) != 0) {
                    *param_2 |= 4;
                    if (found == NULL || (found->flags & 0x800) == 0 ||
                        (cur->id - 1 != found->id - 3 && cur->id - 1 != found->id - 4)) {
                        found = NULL;
                    }
                }
            }
            if ((cur->flags & 0x20) != 0 && (cur->flags & 0x400) == 0) {
                GetIconRect((struct IconNode *)cur, (struct Rect32 *)&bbox);
                if (x < bbox.min_x || x > bbox.max_x || y < bbox.min_y || y > bbox.max_y) {
                    doGeom = 0;
                } else {
                    doGeom = 1;
                }
            }
        }
    }
    return found;
}

// FUNCTION: LEGOLAND 0x0046f4c0
LEGO_EXPORT unsigned char CheckFocussedIcon(void) {
    struct IconNode *icon;

    if (CancelKeyHandler != 0 && (CancelKeyState & 7) != 0) {
        return ((unsigned char (*)(unsigned int, unsigned int, unsigned int, unsigned int))CancelKeyHandler)(0, CancelKeyState, 0, 0);
    }

    icon = (struct IconNode *)FocussedIconPtr;
    if (icon != NULL) {
        if ((icon->flags & 2) != 0 && icon->event_handler != NULL) {
            unsigned int buttons;
            if ((OkKeyState & 7) != 0 && WorkerOnMouseActive == 0) {
                return ((unsigned char (*)(struct IconNode *, unsigned int, short, short))icon->event_handler)(
                    icon, OkKeyState, (short)MousePos.x - icon->x, (short)MousePos.y - icon->y);
            }
            buttons = MouseClickFlags;
            MouseClickConsumed = 1;
            return ((unsigned char (*)(struct IconNode *, unsigned int, short, short))icon->event_handler)(
                icon, buttons, (short)MousePos.x - icon->x, (short)MousePos.y - icon->y);
        }
        {
            unsigned int buttons = OkKeyState;
            if ((OkKeyState & 7) == 0 || WorkerOnMouseActive != 0) {
                buttons = MouseClickFlags;
                MouseClickConsumed = 1;
            }
            return DefaultIconEventHandler(icon, buttons, (short)MousePos.x - icon->x, (short)MousePos.y - icon->y);
        }
    }

    if (OkKeyHandler != 0 && (OkKeyState & 7) != 0) {
        return ((unsigned char (*)(unsigned int, unsigned int, unsigned int, unsigned int))OkKeyHandler)(0, OkKeyState, 0, 0);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046f690
LEGO_EXPORT struct IconNode *AddGBarClassIcon(void *param_1, struct InfoSource *src, int a3, int a4, int a5, short a6) {
    struct IconNode *icon = InsertIcon(a3, a4, a5, GBarFrameSprite);
    if (icon != NULL) {
        struct Sprite *sprite = src->field_68;
        void *value;
        unsigned int flags;
        if (sprite != NULL) {
            unsigned short width = sprite->width;
            unsigned short height = sprite->height;
            SetIconSprite(icon, sprite);
            icon->height = height;
            icon->width = width;
        } else {
            SetIconSprite(icon, NULL);
        }
        icon->field_18p = src->field_78;
        if ((src->field_c4[2] & 0x10000) != 0) {
            value = src->field_78;
        } else {
            value = src->field_7c;
        }
        icon->string = value;
        icon->field_1c = value;
        icon->string_id = 0xffffffff;
        if ((src->field_c4[2] & 0x20000) != 0) {
            *(unsigned char *)&icon->field_1c = 0x14;
        }
        flags = icon->flags & 0xfffffdff;
        icon->field_30 = param_1;
        icon->render_func = (void *)RenderGBarSpriteIcon;
        icon->flags = flags | 0x3008;
        icon->field_8 = src;
        icon->field_16 = a6;
        if (NewIconGroupCallbacks != 0) {
            icon->field_24 = NewIconGroupCallbacks;
            icon->flags = icon->flags | 4;
        }
        if (NewGroupRenderFunc != 0) {
            icon->render_func = NewGroupRenderFunc;
            icon->flags = icon->flags | 8;
        }
        if (NewGroupEventHandler != 0) {
            icon->event_handler = NewGroupEventHandler;
            icon->flags = icon->flags | 2;
        }
    }
    return icon;
}

// FUNCTION: LEGOLAND 0x0046f7a0
LEGO_EXPORT struct IconNode *AddFreePlayIcon(void *param_1, struct InfoSource *src, int a3, int a4, int a5, short a6, void *a7) {
    struct IconNode *icon = InsertIcon(a3, a4, a5, GBarFrameSprite);
    if (icon != NULL) {
        struct Sprite *sprite = src->field_14;
        unsigned int flags;
        if (sprite != NULL) {
            SetIconSprite(icon, sprite);
        } else {
            SetIconSprite(icon, NULL);
        }
        flags = icon->flags;
        icon->render_func = (void *)RenderGBarSpriteIcon;
        icon->string = src->field_10;
        icon->string_id = 0xffffffff;
        icon->flags = flags | 0x3008;
        icon->field_16 = a6;
        icon->field_8 = src->field_8;
        icon->field_18 = 0;
        icon->field_1c = src->field_4;
        icon->field_20p = a7;
        if (NewIconGroupCallbacks != 0) {
            icon->field_24 = NewIconGroupCallbacks;
            icon->flags = icon->flags | 4;
        }
        if (NewGroupRenderFunc != 0) {
            icon->render_func = NewGroupRenderFunc;
            icon->flags = icon->flags | 8;
        }
        if (NewGroupEventHandler != 0) {
            icon->event_handler = NewGroupEventHandler;
            icon->flags = icon->flags | 2;
        }
    }
    return icon;
}

// FUNCTION: LEGOLAND 0x0046f890
void LoadGBarSprites(void) {
    if (GBarSpritesLoaded != 0) {
        return;
    }
    if (GBarFrameSprite == 0) {
        // STRING: LEGOLAND 0x004ba8c8
        GBarFrameSprite = LoadSprite("GBarFrame.lls", 4);
    }
    if (IfSideBUpSprite == 0) {
        // STRING: LEGOLAND 0x004ba8b8
        IfSideBUpSprite = LoadSprite("IF_Side_BUp.lls", 4);
    }
    if (IfSideBDownSprite == 0) {
        // STRING: LEGOLAND 0x004ba8a4
        IfSideBDownSprite = LoadSprite("IF_Side_BDown.lls", 4);
    }
    if (IfSidebar1Sprite == 0) {
        // STRING: LEGOLAND 0x004ba894
        IfSidebar1Sprite = LoadSprite("IF_Sidebar1.lls", 4);
    }
    GBarSpritesLoaded = 1;
}

// FUNCTION: LEGOLAND 0x0046f920
void UnloadGBarSprites(void) {
    if (GBarSpritesLoaded != 0) {
        GBarSpritesLoaded = 0;
        if (GBarFrameSprite != 0) {
            KillSprite(GBarFrameSprite);
            GBarFrameSprite = 0;
        }
        if (IfSideBUpSprite != 0) {
            KillSprite(IfSideBUpSprite);
            IfSideBUpSprite = 0;
        }
        if (IfSideBDownSprite != 0) {
            KillSprite(IfSideBDownSprite);
            IfSideBDownSprite = 0;
        }
        if (IfSidebar1Sprite != 0) {
            KillSprite(IfSidebar1Sprite);
            IfSidebar1Sprite = 0;
        }
    }
}

/* Creates an icon panel for all database object classes whose parent element equals kind. Lays out matching entries
 * vertically or horizontally from flags, adjusts panel bounds, destroys an empty group, and returns the number of
 * children found. */
// FUNCTION: LEGOLAND 0x0046f9a0
int CreateChildObjectIconPanel(int id, struct Element *kind, int param_3, int param_4, unsigned int flags, int param_6) {
    int found;
    int i;
    int total;
    int x;
    int y;
    struct InterfacePanel *group;
    struct IconNode *icon;
    struct Element *elem;

    found = 0;
    group = (struct InterfacePanel *)malloc(sizeof(struct InterfacePanel));
    if (group == NULL) {
        return 0;
    }
    icon = AddGBarIcons(group, param_3, param_4, flags, param_6, id);
    group->icon = icon;
    x = group->content_left = group->clip_left = icon->x;
    y = group->content_top = group->clip_top = icon->y;
    group->clip_right = icon->x + icon->width;
    group->clip_bottom = icon->y + icon->height;
    group->field_4 = flags;
    group->group = (short)id;
    SetNewGroup_Callbacks(0, 0, (void *)ObjectListIconHandler);
    total = LLIDB_GetCount();
    for (i = 0; i < total; i++) {
        LLIDB_GetElement(i, &elem);
        if ((elem->flags & 0x10) && elem->ride->parent_element == kind) {
            found++;
            AddGBarClassIcon(group, elem->data, x, y, id, (short)i);
            /* bit 0: vertical bar */
            if (flags & 1) {
                y += 0x38;
            } else {
                x += 0x79;
            }
        }
    }
    AddFullScreenIcon(id + 6);
    group->field_14 = x;
    group->content_bottom = y;
    icon = group->icon;
    if (flags & 1) {
        if (group->content_bottom < icon->y + icon->height) {
            icon->height = (short)group->content_bottom - (short)group->content_top;
            icon = FindIcon((unsigned short)(id + 4));
            if (icon != NULL) {
                icon->y = (short)group->content_bottom;
            }
        }
    } else if (group->field_14 < icon->x + icon->width) {
        icon->width = (short)group->field_14 - (short)group->content_left;
        icon = FindIcon((unsigned short)(id + 4));
        if (icon != NULL) {
            icon->x = (short)group->field_14;
        }
    }
    if (found == 0) {
        DestroyIconGroup(id);
    }
    return found;
}

// FUNCTION: LEGOLAND 0x0046fb40
void DestroyIconGroup(unsigned int group) {
    struct IconNode *icon1;
    struct IconNode *icon2;
    struct IconNode *icon3;
    struct IconNode *icon4;

    icon1 = FindIcon(group + 5);
    if (icon1) {
        icon2 = FindIcon(group + 3);
        if (icon2) {
            RemoveIconFromList(icon2);
        }
        icon3 = FindIcon(group + 4);
        if (icon3) {
            RemoveIconFromList(icon3);
        }
        icon4 = FindIcon(group + 6);
        if (icon4) {
            RemoveIconFromList(icon4);
        }
        free(icon1->field_30);
        RemoveIconFromList(icon1);
        RemoveIconGroup(group);
    }
}

// FUNCTION: LEGOLAND 0x0046fbc0
unsigned char IndicatorIconHandler(struct IconNode *icon, unsigned char flag) {
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
LEGO_EXPORT struct Indicator *AllocateTimedIndicator(struct Sprite *sprite, unsigned int a2, unsigned int a3) {
    struct Indicator *ind;
    struct IconNode *icon;

    ind = malloc(sizeof(struct Indicator));
    ind->next = IndicatorList;
    ind->field_4 = 0;
    ind->field_8 = GetGameTimer();
    ind->field_c = a2;
    ind->field_10 = a3;

    ReferenceSprite(sprite);
    icon = InsertIcon(0, 0, 0xe000, sprite);
    ind->field_14 = icon;
    icon->render_func = (void *)RenderBlinkingIcon;

    icon = ind->field_14;
    icon->event_handler = (void *)IndicatorIconHandler;

    icon = ind->field_14;
    icon->flags = icon->flags | 0x40a;

    icon = ind->field_14;
    icon->field_30 = ind;

    ind->field_18 = 0;
    ind->field_1c = 0;

    IndicatorList = (struct Indicator *)ind;
    return ind;
}

// FUNCTION: LEGOLAND 0x0046fc80
LEGO_EXPORT struct Indicator *AllocatePermanentIndicator(struct Sprite *sprite, unsigned int param_2) {
    struct Indicator *ind;
    struct IconNode *icon;

    ind = malloc(sizeof(struct Indicator));
    ind->next = IndicatorList;
    ind->field_4 = 1;
    ind->field_8 = GetGameTimer();
    ind->field_10 = param_2;

    ReferenceSprite(sprite);
    icon = InsertIcon(0, 0, 0xe000, sprite);
    ind->field_14 = icon;
    icon->render_func = (void *)RenderBlinkingIcon;

    icon = ind->field_14;
    icon->event_handler = (void *)IndicatorIconHandler;

    icon = ind->field_14;
    icon->flags = icon->flags | 0x40a;

    icon = ind->field_14;
    icon->field_30 = ind;

    ind->field_18 = 0;
    ind->field_1c = 0;

    IndicatorList = (struct Indicator *)ind;
    return ind;
}

// FUNCTION: LEGOLAND 0x0046fd00
LEGO_EXPORT void SetCheckFunc(struct Indicator *ind, void (*func)(struct Indicator *)) {
    ind->field_18 = func;
    ind->field_4 |= 0x4;
}

// FUNCTION: LEGOLAND 0x0046fd20
LEGO_EXPORT void SetClickFunc(struct Indicator *ind, void (*func)(struct Indicator *)) {
    ind->field_1c = func;
    ind->field_4 |= 0x4;
}

// FUNCTION: LEGOLAND 0x0046fd40
LEGO_EXPORT void AddIndicator(struct Indicator *ind) {
    struct Indicator *cur = IndicatorList;
    if (cur == ind) {
        IndicatorList = cur->next;
    } else {
        while (cur != NULL) {
            if (cur->next == ind) {
                cur->next = ind->next;
                break;
            }
            cur = cur->next;
        }
    }
    if (cur != NULL) {
        ind->next = ActiveIndicators;
        ActiveIndicators = ind;
        ind->field_4 = ind->field_4 | 8;
        ind->field_14->x = 0xf000;
        ind->field_14->flags = ind->field_14->flags & 0xfffffbff;
    }
}

// FUNCTION: LEGOLAND 0x0046fda0
LEGO_EXPORT void RemoveIndicator(struct Indicator *ind) {
    struct Indicator *cur = ActiveIndicators;
    if (cur == ind) {
        ActiveIndicators = cur->next;
    } else {
        while (cur != NULL) {
            if (cur->next == ind) {
                cur->next = ind->next;
                break;
            }
            cur = cur->next;
        }
    }
    if (cur != NULL) {
        ind->next = IndicatorList;
        IndicatorList = ind;
        ind->field_4 = ind->field_4 & 0xfffffff7;
        ind->field_14->flags = ind->field_14->flags | 0x400;
    }
}

// FUNCTION: LEGOLAND 0x0046fe00
LEGO_EXPORT void DeleteIndicator(struct Indicator *ind) {
    struct Indicator *cur;

    RemoveIconFromList(ind->field_14);

    if ((ind->field_4 & 0x8) != 0) {
        if (ActiveIndicators == ind) {
            ActiveIndicators = ((struct Indicator *)ActiveIndicators)->next;
            free(ind);
            return;
        }
        cur = ActiveIndicators;
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

    if (IndicatorList == ind) {
        IndicatorList = ((struct Indicator *)IndicatorList)->next;
        free(ind);
        return;
    }
    cur = IndicatorList;
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
LEGO_EXPORT void ControlIndicators(void) {
    struct Indicator *node = ActiveIndicators;
    int now = GetGameTimer();
    int x = ((struct Config *)lpConfig)->field_0 - 0x50;
    struct Indicator *cur;
    struct Indicator *head;
    struct Indicator *prev;

    ActiveIndicators = NULL;
    while (node != NULL) {
        node->field_10 &= 0xffff;
        if (now - (int)node->field_8 < 0x1388) {
            node->field_10 |= 0x10000;
        } else {
            node->field_14->flags &= ~8;
        }
        if ((node->field_4 & 1) != 0) {
            node->field_10 |= 0x20000;
        } else if (now - (int)node->field_8 >= (int)node->field_c) {
            cur = node;
            node = node->next;
            cur->next = ActiveIndicators;
            ActiveIndicators = cur;
            RemoveIndicator(cur);
            if ((cur->field_4 & 2) != 0) {
                DeleteIndicator(cur);
            }
            continue;
        }
        head = ActiveIndicators;
        cur = node;
        node = node->next;
        if (head == NULL) {
            ActiveIndicators = cur;
            cur->next = NULL;
            continue;
        }
        for (prev = NULL; head != NULL; prev = head, head = head->next) {
            if ((int)head->field_10 < (int)cur->field_10) {
                if (prev != NULL) {
                    prev->next = cur;
                } else {
                    ActiveIndicators = cur;
                }
                cur->next = head;
                break;
            }
        }
        if (head == NULL) {
            prev->next = cur;
            cur->next = NULL;
        }
    }

    for (node = ActiveIndicators; node != NULL; node = node->next) {
        node->field_14->x = (short)x;
        x = x - 0x34;
        node->field_14->y = 8;
        if (x <= (int)(((struct Config *)lpConfig)->field_0 >> 2)) {
            break;
        }
    }
    for (; node != NULL; node = node->next) {
        node->field_14->x = (short)0xf000;
    }
}

// FUNCTION: LEGOLAND 0x00470000
unsigned char ObjectListIconHandler(struct IconNode *node, unsigned char buttons) {
    int cost;
    struct Element *sub;
    unsigned int flags;

    if ((buttons & 2) != 0) {
        do {
            cost = GetObjCost(node->field_8);
            if (cost <= GetBrickCount()) {
                if ((node->flags & 0x1000) != 0) {
                    SetEditObject(node->field_8);
                }
                sub = ((struct Ride *)node->field_8)->element;
                flags = sub->flags;
                if ((flags & 0x20000) != 0) {
                    sub->flags = flags & 0xfffdffff;
                    RemoveNewObject(node->field_8);
                }
                PlayInstanceOfSample(GameFX[FX_CLICK1].sample, 0, 1, 0);
            } else {
                PlayInstanceOfSample(GameFX[FX_RASP].sample, 0, 1, 0);
            }
        } while (0);
        return 2;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004700a0
LEGO_EXPORT void UpdateFocussedIconPtr(void) {
    if (Hover.type == 2) {
        FocussedIconPtr = (void *)Hover.ptr;
    } else {
        FocussedIconPtr = NULL;
    }
}
