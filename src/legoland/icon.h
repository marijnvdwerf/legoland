#pragma once

#include "legoland.h"

struct Sprite;
struct InfoSource;
struct InterfacePanel;
struct Point;

struct IconNode {
    /* 0x00 */ struct IconNode *next;
    /* 0x04 */ struct Sprite *sprite;
    /* 0x08 */ void *field_8;
    /* 0x0c */ short x;
    /* 0x0e */ short y;
    /* 0x10 */ short width;
    /* 0x12 */ short height;
    /* 0x14 */ unsigned short id;
    /* 0x16 */ unsigned short field_16;
    /* 0x18 */ union {
        unsigned char field_18;
        short field_18s;
        void *field_18p;
        struct Sprite *alt_sprite;
        struct {
            /* 0x18 */ unsigned short box_color;
            /* 0x1a */ unsigned char border;
        } box;
    };
    /* 0x1c */ union {
        void *field_1c;
        unsigned char slot;
        unsigned char field_1cb;
    };
    /* 0x20 */ union {
        struct {
            /* 0x20 */ short field_20;
            /* 0x22 */ short field_22;
        };
        unsigned char field_20b;
        void *field_20p;
    };
    /* 0x24 */ void *field_24;
    /* 0x28 */ void *render_func;
    /* 0x2c */ void *event_handler;
    /* 0x30 */ void *field_30;
    /* 0x34 */ unsigned int flags;
    /* 0x38 */ char *string;
    /* 0x3c */ unsigned int string_id;
};

LEGO_EXPORT void RenderIcons(void);
LEGO_EXPORT int RenderFlashingSpriteIcon(struct IconNode *node);
LEGO_EXPORT void RenderIcons2(short param_1, short param_2, short param_3);
LEGO_EXPORT unsigned char CheckFocussedIcon(void);
LEGO_EXPORT void UpdateFocussedIconPtr(void);
void UpdateEditModeButtonSprites(void);
LEGO_EXPORT void RemoveIconGroup(unsigned short group);
void SetHelpKindObjectiveMessage(void);
void RemoveIconRange(unsigned short val);
int RenderBlinkIcon(struct IconNode *node);
void SetIconSprite(struct IconNode *node, struct Sprite *sprite);
LEGO_EXPORT struct IconNode *InsertIcon(short a1, short a2, int a3, struct Sprite *sprite);
LEGO_EXPORT struct IconNode *LoadSpriteIcon(const char *filename, unsigned int param_2, short param_3, short param_4, int param_5);
void ScrollActiveIconRegionForward(void);
void ScrollActiveIconRegionBack(void);
int SetClippingToScreenRect(int param);
LEGO_EXPORT int RenderFreePlayBar(struct IconNode *node);
LEGO_EXPORT void RenderHelpIcons(void);
void DestroyIconGroup(unsigned int group);
LEGO_EXPORT void MoveIcons(unsigned short mask, unsigned short id, short dx, short dy);
LEGO_EXPORT struct IconNode *FindIcon(unsigned short id);
LEGO_EXPORT void SetNewGroup_Callbacks(void *param_1, void *param_2, void *param_3);
LEGO_EXPORT struct IconNode *AddFullScreenIcon(int icon);
LEGO_EXPORT struct IconNode *SetupInterfacePanelIcons(struct InterfacePanel *param_1, int param_2, int param_3, int param_4, int param_5, int param_6);
LEGO_EXPORT int RenderFreePlayIcons(struct IconNode *node);
LEGO_EXPORT struct IconNode *AddFreePlayIcon(void *param_1, struct InfoSource *src, int a3, int a4, int a5, short a6, void *a7);
LEGO_EXPORT struct IconNode *AddGBarClassIcon(void *param_1, struct InfoSource *src, int a3, int a4, int a5, short a6);
LEGO_EXPORT int RenderBuildObjectIcon(struct IconNode *node);
void ScrollIconRegion(struct InterfacePanel *r, int param_2, int param_3);
unsigned char ScrollIconRegionForward(struct IconNode *icon, unsigned char buttons, int a3, int a4);
unsigned char ScrollIconRegionBack(struct IconNode *icon, unsigned char buttons, int a3, int a4);
int GetIconScrollSnapOffset(unsigned short param_1, short param_2, short param_3, unsigned short param_4, int param_5);
LEGO_EXPORT int RenderBoxIcon(struct IconNode *node);
LEGO_EXPORT int RenderScroll_Icons(struct IconNode *node);
LEGO_EXPORT int RenderGBarSprite(struct IconNode *node);
LEGO_EXPORT int RenderGBarSpriteIcon(struct IconNode *node);
LEGO_EXPORT int RenderEnergyBar(struct IconNode *node);
LEGO_EXPORT int RenderMoneyBar(struct IconNode *node);
int RenderFocusableIcon(struct IconNode *node);
LEGO_EXPORT struct IconNode *AddGBarIcons(struct InterfacePanel *param_1, unsigned int param_2, int param_3, int param_4, int param_5, int param_6);
unsigned char ObjectListIconHandler(struct IconNode *node, unsigned char buttons);
LEGO_EXPORT struct IconNode *GetIconAtPos(struct Point *param_1, unsigned char *param_2);
void RenderIconsExceptId(short param_1);
void LoadGBarSprites(void);
void UnloadGBarSprites(void);
