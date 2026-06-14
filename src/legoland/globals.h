#pragma once

#include <windows.h>

#include "gamemap.h"
#include "legoland.h"

/* DirectInput stays private to the input subsystem (input.c) + its definitions
 * (globals.c) — both include <dinput.h> directly. globals.h only needs an opaque
 * handle to the one DInput-typed global below, so it does NOT drag <dinput.h>
 * across every TU that includes globals.h. */
struct _DIMOUSESTATE;

// ---------------------------------------------------------------------------
// Forward declarations for typed/instance globals declared below. Full struct
// definitions live in the owning translation units; these globals only need an
// incomplete type for their pointer/extern declarations. (struct Cursor's full
// layout comes from gamemap.h, used by the EditCursor / QueryCursor instances.)
// ---------------------------------------------------------------------------
struct Sprite;
struct Image;
struct CursorSource;
struct Building;
struct JailCell;
struct FXSpriteList;
struct ResVolume;
struct RideQueueEntry;
struct ObjectiveEvent;
struct MapElement;
struct WaterContext;
struct WaterSub;
struct EateryFX;
struct BlokeNode;
struct SaveBlock;
struct JungleRide;
struct JungleCursor;
struct JungleFish;
struct JungleScore;
struct JungleObj;
struct JunglePath;
struct JungleTileMap;
struct CatapultNode;
struct Position;
struct RinData;
struct CopterNode;
struct GoldNode;
struct CarouselNode;
struct JoustNode;
struct InterfaceIconNode;
struct IconNode;
struct InterfaceProfileObj;
struct InterfaceListNode;
struct InterfaceQueryNode;
struct InterfaceEventNode;
struct InterfaceResearchNode;
struct InfoIcon;
struct InfoObjData;
struct Element;
struct SortNode;

// Full definitions needed by typed globals defined in globals.c.
struct MapElement {
    /* 0x00 */ unsigned int field_0;
    /* 0x04 */ unsigned short field_4;
    /* 0x06 */ unsigned char pad_6[0xc - 0x6];
    /* 0x0c */ unsigned short flags;
    /* 0x0e */ unsigned char pad_e[0x10 - 0xe];
    /* 0x10 */ unsigned char field_10;
    /* 0x11 */ unsigned char pad_11[0x14 - 0x11];
};

struct LegoConfig {
    /* 0x00 */ unsigned short field_0;
    /* 0x02 */ unsigned short field_2;
    /* 0x04 */ unsigned char pad_4[0x10 - 0x04];
    /* 0x10 */ unsigned short field_10;
    /* 0x12 */ unsigned short field_12;
    /* 0x14 */ unsigned short width;
    /* 0x16 */ unsigned short height;
    /* 0x18 */ unsigned char pad_18[2];
    /* 0x1a */ unsigned short field_1a;
    /* 0x1c */ unsigned char field_1c;
    /* 0x1d */ unsigned char pad_1d[1];
    /* 0x1e */ unsigned short field_1e;
    /* 0x20 */ unsigned short field_20;
    /* 0x22 */ unsigned short field_22;
    /* 0x24 */ unsigned short field_24;
    /* 0x26 */ unsigned char pad_26[2];
    /* 0x28 */ unsigned int field_28;
    /* 0x2c */ unsigned char pad_2c[4];
    /* 0x30 */ unsigned int field_30;
    /* 0x34 */ unsigned int field_34;
    /* 0x38 */ unsigned int field_38;
    /* 0x3c */ unsigned char pad_3c[4];
    /* 0x40 */ unsigned int field_40;
};

struct ObjTableEntry {
    unsigned short key;
    unsigned short value;
};

struct FreePlaySpriteSlot {
    struct Sprite *field_0;
    struct Sprite *field_4;
    unsigned char pad_8[0x14];
};

struct ClipRect {
    int left;
    int top;
    int right;
    int bottom;
};

struct InfoTimedEntry {
    /* 0x00 */ unsigned int sample;
    /* 0x04 */ int interval;
    /* 0x08 */ int last_time;
};

struct MapRect {
    /* 0x00 */ int x0;
    /* 0x04 */ int y0;
    /* 0x08 */ int x1;
    /* 0x0c */ int y1;
};

struct TextCell {
    /* 0x00 */ int width;
    /* 0x04 */ int height;
    /* 0x08 */ unsigned int format;
    /* 0x0c */ char *name;
    /* 0x10 */ unsigned int bg_color;
    /* 0x14 */ unsigned int text_color;
    /* 0x18 */ int font;
    /* 0x1c */ struct Sprite *sprite;
};

struct TileSpriteEntry {
    struct FXSpriteList *src;
    unsigned short sprite;
    unsigned short pad_6;
};

struct DeferredSprite {
    struct Sprite *sprite;
    unsigned int x;
    unsigned int y;
    unsigned int flags;
};

struct ListLink {
    unsigned char pad_0[0x14];
    struct ListLink *var_14;
    struct ListLink *var_18;
};

struct EditFootPrint {
    unsigned char data[16];
    void *field_10;
};

struct Settings {
    unsigned char field_0;
    unsigned char gap_1[0x1d];
    unsigned char field_1e;
    unsigned char gap_1f;
    unsigned int field_20;
    unsigned char field_24;
    unsigned char gap_25[3];
    unsigned int field_28;
    unsigned int field_2c;
    unsigned int field_30;
    unsigned int field_34;
    unsigned int field_38;
    unsigned int field_3c;
    unsigned short field_40;
    unsigned char field_42;
    unsigned char field_43[200];
    unsigned char field_10b;
    unsigned char gap_10c[4];
};

struct SoftPrintState {
    /* 0x00 */ unsigned int field_0;
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ int height;
    /* 0x0c */ int width;
    /* 0x10 */ int pitch;
    /* 0x14 */ unsigned char pad_14[0x24 - 0x14];
    /* 0x24 */ void *pixels;
    /* 0x28 */ unsigned char pad_28[0x6c - 0x28];
};

struct ScreenState {
    unsigned char gap_0[0x20];
    unsigned int field_20;
    unsigned int field_24;
    unsigned int field_28;
    unsigned int field_2c;
    unsigned char field_30[4];
    unsigned char field_34[0xf];
    unsigned char field_43;
    unsigned char field_44;
    unsigned char field_45;
    unsigned char gap_46[0xca];
};

// ---------------------------------------------------------------------------
// All game globals, sorted by ascending address. Definitions (with any
// initializers) live in globals.c. This phase only collects the declarations;
// per-TU inline externs are migrated to use this header in a later phase.
// ---------------------------------------------------------------------------
// 0x004ab38c
extern float DAT_004ab38c;
// 0x004ab390
extern float FLOAT_004ab390;
// 0x004ab3a8
extern double DAT_004ab3a8;
// 0x004ab3d0
extern float DAT_004ab3d0;
// 0x004ab3dc
extern float DAT_004ab3dc;
// 0x004ab3e0
extern float DAT_004ab3e0;
// 0x004ab3e4
extern float DAT_004ab3e4;
// 0x004ab3e8
extern float DAT_004ab3e8;
// 0x004ab3ec
extern float DAT_004ab3ec;
// 0x004ab3f0
extern float DAT_004ab3f0;
// 0x004ab3f4
extern float DAT_004ab3f4;
// 0x004ab3f8
extern float DAT_004ab3f8;
// 0x004ab3fc
extern float DAT_004ab3fc;
// 0x004ab4a0
extern float DAT_004ab4a0;
// 0x004ab430
extern float DAT_004ab430;
// 0x004ab454
extern float FLOAT_004ab454;
// 0x004ab458
extern float FLOAT_004ab458;
// 0x004ab45c
extern float FLOAT_004ab45c;
// 0x004ab480
extern float FLOAT_004ab480;
// 0x004ab4c8
extern double DAT_004ab4c8;
// 0x004ab560 c_dfDIKeyboard, 0x004ab578 c_dfDIMouse — declared in <dinput.h>, defined in globals.c
// 0x004ab5e0
extern GUID DAT_004ab5e0;
// 0x004ab600
extern GUID DAT_004ab600;
// 0x004ab610
extern GUID DAT_004ab610;
// 0x004ab670
extern GUID DAT_004ab670;
// 0x004ab6d0
extern GUID GUID_PerfMasterGrooveLevel;
// 0x004ab8b0
extern GUID DAT_004ab8b0;
// 0x004ab8e0
extern GUID DAT_004ab8e0;
// 0x004ab930
extern GUID DAT_004ab930;
// 0x004ab980
extern GUID DAT_004ab980;
// 0x004ab9f0
extern GUID DAT_004ab9f0;
// 0x004ac090 GUID_SysKeyboard, 0x004ac0a0 GUID_SysMouse — declared in <dinput.h>, defined in globals.c
// 0x004acf80
extern GUID DAT_004acf80;
// 0x004b4034
extern int DAT_004b4034[16];
// 0x004b40c8
extern unsigned char Catapult_SFX[0x70];
// 0x004b4140
extern unsigned char Helicopter_SFX[0x70];
// 0x004b43f8
extern unsigned char DRIVING_SCHOOL_SFX[0xa8];
// 0x004b4688
extern unsigned char JOUST_SFX[12];
// 0x004b4728
extern unsigned int DAT_004b4728;
// 0x004b472c
extern unsigned int DAT_004b472c;
// 0x004b4730
extern unsigned int DAT_004b4730;
// 0x004b4734
extern unsigned int DAT_004b4734;
// 0x004b47f8
extern struct Sprite *DAT_004b47f8;
// 0x004b4804
extern struct Sprite *DAT_004b4804;
// 0x004b4838
extern struct Sprite *DAT_004b4838;
// 0x004b4850
extern struct Sprite *DAT_004b4850;
// 0x004b4bd0
extern unsigned char DAT_004b4bd0[0x14];
// 0x004b4bf0
extern unsigned int DAT_004b4bf0[5];
// 0x004b4cb8
extern unsigned char SAFARI_SFX[8];
// 0x004b4d88
extern unsigned char SpiderRide_SFX[8];
// 0x004b4d90
extern void *DAT_004b4d90;
// 0x004b4fa8
extern unsigned char WATERWORKS_SFX[28];
// 0x004b53c0
extern unsigned int DAT_004b53c0[5];
// 0x004b55f4
extern unsigned int DAT_004b55f4;
// 0x004b5608
extern unsigned int DAT_004b5608;
// 0x004b560c
extern unsigned int DAT_004b560c;
// 0x004b5b20
extern void *DAT_004b5b20;
// 0x004b5b28
extern int DAT_004b5b28;
// 0x004b5b4c
extern int DAT_004b5b4c;
// 0x004b5b50
extern int DAT_004b5b50;
// 0x004b5c9c
extern unsigned int DAT_004b5c9c;
// 0x004b5ca0
extern unsigned int DAT_004b5ca0;
// 0x004b5ca4
extern unsigned int DAT_004b5ca4;
// 0x004b5ca8
extern unsigned int DAT_004b5ca8;
// 0x004b5cbc
extern float DAT_004b5cbc;
// 0x004b5cc0
extern unsigned int DAT_004b5cc0;
// 0x004b5cc4
extern unsigned int DAT_004b5cc4;
// 0x004b5cc8
extern unsigned int DAT_004b5cc8;
// 0x004b5dc8
extern const unsigned char DAT_004b5dc8[1];
// 0x004b5f60
extern unsigned char DAT_004b5f60[1];
// 0x004b62f0
extern unsigned int DAT_004b62f0;
// 0x004b6300
extern unsigned int DAT_004b6300;
// 0x004b64d8
extern unsigned char CAROUSSEL_SFX[12];
// 0x004b6668
extern unsigned char ENTRANCE_SFX[0x1c];
// 0x004b66e8
extern unsigned char RESTAURANT_SFX[0x1c];
// 0x004b6968
extern unsigned char OCTOPUS_SFX[0x1c];
// 0x004b7260
extern int DAT_004b7260[6];
// 0x004b7278
extern int DAT_004b7278[4];
// 0x004b7288
extern void *DAT_004b7288;
// 0x004b7148
extern int DAT_004b7148[0x10];
// 0x004b7188
extern float DAT_004b7188[0x10];
// 0x004b71c8
extern float DAT_004b71c8[0x10];
// 0x004b72b0
extern int DAT_004b72b0[2];
// 0x004b7230
extern struct Footprint DAT_004b7230;
// 0x004b7248
extern struct Footprint DAT_004b7248;
// 0x004b72e4
extern unsigned char DAT_004b72e4[0x190];
// 0x004b7478
extern struct Footprint DAT_004b7478;
// 0x004b7618
extern const unsigned char SPACE_TOWER_SFX[15];
// 0x004b7620
extern void *DAT_004b7620;
// 0x004b79d0
extern unsigned char DAT_004b79d0[0x18];
// 0x004b7abc
extern unsigned short DAT_004b7abc;
// 0x004b7ac0
extern unsigned char DAT_004b7ac0[0x18];
// 0x004b7d74
extern unsigned int DAT_004b7d74;
// 0x004b7d78
extern unsigned int DAT_004b7d78;
// 0x004b7d7e
extern unsigned short DAT_004b7d7e;
// 0x004b7d84
extern unsigned int DAT_004b7d84;
// 0x004b7e9c
extern unsigned int DAT_004b7e9c;
// 0x004b81c0
extern char *DAT_004b81c0;
// 0x004b81c4
extern char *DAT_004b81c4;
// 0x004b81c8
extern char *DAT_004b81c8;
// 0x004b81cc
extern char *DAT_004b81cc;
// 0x004b81d0
extern char *DAT_004b81d0;
// 0x004b81d4
extern char *DAT_004b81d4;
// 0x004b81d8
extern char *DAT_004b81d8;
// 0x004b8334
extern int DAT_004b8334;
// 0x004b8340
extern int DAT_004b8340;
// 0x004b8344
extern int DAT_004b8344;
// 0x004b8368
extern void (*PTR_Bloke_DoNothing_004b8368[16])(void *);
// 0x004b8710
extern unsigned char DAT_004b8710[0x40];
// 0x004b8750
extern unsigned char DAT_004b8750[0x18];
// 0x004b8768
extern unsigned char DINO_SFX[0x18];
// 0x004b87a8
extern unsigned char MONEY_SFX[12];
// 0x004b8bbc
extern unsigned char DAT_004b8bbc[1];
// 0x004b90f8
extern unsigned int DAT_004b90f8;
// 0x004b90fc
extern unsigned int DAT_004b90fc;
// 0x004b9220
extern LEGO_EXPORT unsigned int BGFullUpdate;
// 0x004b9228
extern const unsigned char DAT_004b9228[1];
// 0x004b929c
extern void *DAT_004b929c;
// 0x004b92c0
extern void *PTR_004b92c0;
extern void *PTR_004b9314;
extern void *PTR_004b9338;
// 0x004b92d8
extern void *DAT_004b92d8;
// 0x004b92e4
extern void *DAT_004b92e4;
// 0x004b9308
extern void *DAT_004b9308;
// 0x004b95f0
extern struct DeferredSprite *PTR_DAT_004b95f0;
// 0x004b9550
extern unsigned char DAT_004b9550[8];
// 0x004b9558
extern unsigned int DAT_004b9558[9];
// 0x004b957c
extern int DAT_004b957c[9];
// 0x004b95a0
extern int DAT_004b95a0[9];
// 0x004b9ca4
extern int (*DAT_004b9ca4)(void);
// 0x004b9ca8
extern unsigned int DAT_004b9ca8;
// 0x004b9e5c
extern unsigned int DAT_004b9e5c[1];
// 0x004b9f78
extern unsigned int DAT_004b9f78[4];
// 0x004b9f88
extern unsigned int DAT_004b9f88;
// 0x004b9f8c
extern unsigned int DAT_004b9f8c;
// 0x004ba87c
extern void *PTR_some_list_head_004ba87c;
// 0x004ba880
extern void *PTR_DAT_004ba880;
// 0x004ba884
extern unsigned int DAT_004ba884;
// 0x004ba8e0
extern struct InfoTimedEntry DAT_004ba8e0[17];
// 0x004ba9ac
extern unsigned int DAT_004ba9ac[1];
// 0x004bad54
extern int mouse_granularity;
struct KeyMapping {
    /* 0x00 */ unsigned char flags;
    /* 0x01 */ signed char code;
};
// 0x004bad58
extern struct KeyMapping DAT_004bad58[0x3b];
// 0x004bafa8
extern unsigned int DAT_004bafa8[20];
// 0x004baff8
extern unsigned int DAT_004baff8;
// 0x004bb094
extern int DAT_004bb04c[18];
extern unsigned int DAT_004bb094;
// 0x004bb098
extern unsigned int DAT_004bb098;
// 0x004bb09c
extern unsigned int DAT_004bb09c;
// 0x004bb0a0
extern unsigned int DAT_004bb0a0;
// 0x004bb18c
extern unsigned int DAT_004bb18c[4];
// 0x004bb5ac
extern unsigned int DAT_004bb5ac;
// 0x004bb5b0
extern unsigned int DAT_004bb5b0;
// 0x004bb688
extern unsigned int DAT_004bb688;
// 0x004bb6bc
extern unsigned int DAT_004bb6bc;
// 0x004bb6f8
extern unsigned int DAT_004bb6f8;
// 0x004bcba4
extern const char *DAT_004bcba4[3];
// 0x004bcbf4
extern LEGO_EXPORT struct LegoConfig *lpConfig;
// 0x004bdd00
extern unsigned int DAT_004bdd00;
// 0x004bdd04
extern unsigned int DAT_004bdd04;
// 0x004bdd08
extern unsigned int DAT_004bdd08;
// 0x004bdea0
extern LEGO_EXPORT struct ClipRect SPRITE_ClipRect;
// 0x004beb94
extern struct FreePlaySpriteSlot DAT_004beb94[10];
// 0x004beba0
extern unsigned char DAT_004beba0;
// 0x004becac
extern unsigned int DAT_004becac;
// 0x004becb0
extern int DAT_004becb0[1];
// 0x004bef9c
extern unsigned int DAT_004bef9c;
// 0x004bf670
extern unsigned int DAT_004bf670;
// 0x004bf774
extern unsigned int DAT_004bf774;
// 0x004bf778
extern unsigned int DAT_004bf778;
// 0x004c10d4
extern void *DAT_004c10d4;
// 0x004c10dc
extern void *DAT_004c10dc;
// 0x004c10e4
extern struct Sprite *DAT_004c10e4;
// 0x004c10f0
extern void *DAT_004c10f0;
// 0x004c10f4
extern void *DAT_004c10f4;
// 0x004c1100
extern void *DAT_004c1100;
// 0x004c1104
extern unsigned int DAT_004c1104;
// 0x004c1108
extern unsigned int DAT_004c1108;
// 0x004c110c
extern unsigned short DAT_004c110c;
// 0x004c1118
extern struct CatapultNode *DAT_004c1118;
// 0x004c1120
extern struct Sprite *DAT_004c1120;
// 0x004c1124
extern int DAT_004c1124[6];
// 0x004c113c
extern struct Sprite *DAT_004c113c[10];
// 0x004c1164
extern struct Sprite *DAT_004c1164;
// 0x004c1198
extern void *DAT_004c1198;
// 0x004c11b4
extern struct CopterNode *DAT_004c11b4;
// 0x004c11bc
extern void *DAT_004c11bc;
// 0x004c11c4
extern unsigned int DAT_004c11c4;
// 0x004c11c8
extern unsigned int DAT_004c11c8;
// 0x004c11cc
extern struct Sprite *DAT_004c11cc;
// 0x004c11dc
extern unsigned int DAT_004c11dc;
// 0x004c11e4
extern struct Sprite *DAT_004c11e4;
// 0x004c11f0
extern void *DAT_004c11f0;
// 0x004c11f4
extern struct Sprite *DAT_004c11f4;
// 0x004c11f8
extern struct Sprite *DAT_004c11f8;
// 0x004c11fc
extern struct Sprite *DAT_004c11fc;
// 0x004c1200
extern struct Sprite *DAT_004c1200;
// 0x004c1204
extern struct GoldNode *DAT_004c1204;
// 0x004c1218
extern void *DAT_004c1218;
// 0x004c121c
extern unsigned int DAT_004c121c;
// 0x004c1228
extern unsigned int DAT_004c1228;
// 0x004c122c
extern unsigned int DAT_004c122c;
// 0x004c1230
extern unsigned int DAT_004c1230;
// 0x004c1234
extern unsigned short DAT_004c1234;
// 0x004c123c
extern void *DAT_004c123c[1];
// 0x004c1240
extern struct Sprite *DAT_004c1240;
// 0x004c1244
extern struct Sprite *DAT_004c1244;
// 0x004c1248
extern struct Sprite *DAT_004c1248;
// 0x004c124c
extern struct Sprite *DAT_004c124c;
// 0x004c1250
extern struct JoustNode *DAT_004c1250;
// 0x004c1258
extern struct Sprite *DAT_004c1258;
// 0x004c2a88
extern unsigned int DAT_004c2a88;
// 0x004c2a94
extern struct Sprite *DAT_004c2a94;
// 0x004c2a98
extern struct Sprite *DAT_004c2a98;
// 0x004c2aa0
extern struct CursorSource *DAT_004c2aa0;
// 0x004c2abc
extern struct Sprite *DAT_004c2abc;
// 0x004c2ae4
extern struct Sprite *DAT_004c2ae4;
// 0x004c2ae8
extern void *DAT_004c2ae8;
// 0x004c2aec
extern struct Sprite *DAT_004c2aec;
// 0x004c2af0
extern struct Sprite *DAT_004c2af0;
// 0x004c2af4
extern unsigned int DAT_004c2af4;
// 0x004c2af8
extern void *DAT_004c2af8;
// 0x004c2afc
extern unsigned int DAT_004c2afc;
// 0x004c2b0c
extern struct CursorSource *DAT_004c2b0c;
// 0x004c2b10
extern unsigned int DAT_004c2b10;
// 0x004c2b50
extern void *DAT_004c2b50;
// 0x004c2b60
extern struct CursorSource *DAT_004c2b60;
// 0x004c2b64
extern struct Sprite *DAT_004c2b64;
// 0x004c2b6c
extern struct Sprite *DAT_004c2b6c;
// 0x004c2b70
extern struct Sprite *DAT_004c2b70;
// 0x004c2b98
extern unsigned int DAT_004c2b98;
// 0x004c2b9c
extern unsigned int DAT_004c2b9c;
// 0x004c2ba0
extern void *DAT_004c2ba0;
// 0x004c2bf0
extern struct CursorSource *DAT_004c2bf0;
// 0x004c445c
extern struct CursorSource *DAT_004c445c;
// 0x004c4460
extern void *DAT_004c4460;
// 0x004c5c90
extern unsigned int DAT_004c5c90;
// 0x004c74c8
extern unsigned int DAT_004c74c8;
// 0x004c74d4
extern struct CursorSource *DAT_004c74d4;
// 0x004c8d2c
extern struct Sprite *DAT_004c8d2c;
// 0x004c8d4c
extern void *DAT_004c8d4c;
// 0x004c8d50
extern unsigned int DAT_004c8d50;
// 0x004c8d68
extern struct Sprite *DAT_004c8d68;
// 0x004c8d6c
extern struct CursorSource *DAT_004c8d6c;
// 0x004c8d70
extern struct Sprite *DAT_004c8d70;
// 0x004cbdd8
extern unsigned int DAT_004cbdd8;
// 0x004cbe08
extern struct Sprite *DAT_004cbe08;
// 0x004cbe0c
extern struct Sprite *DAT_004cbe0c;
// 0x004cbe10
extern void *DAT_004cbe10;
// 0x004cbe14
extern struct Sprite *DAT_004cbe14;
// 0x004cbe18
extern struct CursorSource *DAT_004cbe18;
// 0x004cbe1c
extern struct Sprite *DAT_004cbe1c;
// 0x004cbe20
extern unsigned int DAT_004cbe20;
// 0x004cbe24
extern unsigned int DAT_004cbe24;
// 0x004cbe28
extern unsigned int DAT_004cbe28;
// 0x004cbe2c
extern unsigned int DAT_004cbe2c;
// 0x004cbe48
extern void *DAT_004cbe48;
// 0x004cbe4c
extern struct Sprite *DAT_004cbe4c;
// 0x004cbe74
extern struct Sprite *DAT_004cbe74;
// 0x004cbe78
extern struct Sprite *DAT_004cbe78;
// 0x004cbe7c
extern struct Sprite *DAT_004cbe7c;
// 0x004cbe80
extern struct Sprite *DAT_004cbe80;
// 0x004cbe84
extern void *DAT_004cbe84;
// 0x004cbe88
extern struct Sprite *DAT_004cbe88;
// 0x004cbe8c
extern struct Sprite *DAT_004cbe8c;
// 0x004cbe90
extern struct Sprite *DAT_004cbe90;
// 0x004cbe94
extern struct Sprite *DAT_004cbe94;
// 0x004cbe98
extern struct Sprite *DAT_004cbe98;
// 0x004cbe9c
extern unsigned int DAT_004cbe9c;
// 0x004cbea4
extern void *DAT_004cbea4;
// 0x004cbeac
extern struct RideQueueEntry *DAT_004cbeac;
// 0x004cbeb0
extern int DAT_004cbeb0;
// 0x004cbeb4
extern int DAT_004cbeb4;
// 0x004cbec4
extern unsigned int DAT_004cbec4;
// 0x004cbed0
extern unsigned int DAT_004cbed0;
// 0x004cbed4
extern unsigned int DAT_004cbed4;
// 0x004cbed8
extern unsigned int DAT_004cbed8;
// 0x004cbedc
extern unsigned short DAT_004cbedc;
// 0x004cbef8
extern void *DAT_004cbef8;
// 0x004cbefc
extern void *DAT_004cbefc;
// 0x004cbf00
extern void *DAT_004cbf00;
// 0x004cbf04
extern void *DAT_004cbf04[1];
// 0x004cbf08
extern struct Sprite *DAT_004cbf08;
// 0x004cbf0c
extern void *DAT_004cbf0c;
// 0x004cbf10
extern void *DAT_004cbf10;
// 0x004cbf14
extern struct Sprite *DAT_004cbf14;
// 0x004cbf18
extern void *DAT_004cbf18;
// 0x004cbf1c
extern struct Sprite *DAT_004cbf1c;
// 0x004cbf20
extern unsigned int DAT_004cbf20;
// 0x004cbf24
extern void *DAT_004cbf24;
// 0x004cbf30
extern void *DAT_004cbf30[2];
// 0x004cbf38
extern void *DAT_004cbf38[2];
// 0x004cbf40
extern unsigned int DAT_004cbf40;
// 0x004cbf44
extern unsigned int DAT_004cbf44;
// 0x004cbf48
extern unsigned int DAT_004cbf48;
// 0x004cbf4c
extern unsigned short DAT_004cbf4c;
// 0x004cbf58
extern void *DAT_004cbf58;
// 0x004cbf5c
extern void *DAT_004cbf5c;
// 0x004cbf68
extern struct Sprite *DAT_004cbf68;
// 0x004cbf6c
extern struct Sprite *DAT_004cbf6c;
// 0x004cbf78
extern struct Sprite *DAT_004cbf78;
// 0x004cbf80
extern unsigned int DAT_004cbf80;
// 0x004cbf98
extern unsigned int DAT_004cbf98;
// 0x004cbf9c
extern unsigned int DAT_004cbf9c;
// 0x004cbfa0
extern unsigned int DAT_004cbfa0;
// 0x004cbfa4
extern unsigned short DAT_004cbfa4;
// 0x004cbfc4
extern void *DAT_004cbfc4;
// 0x004cbfd0
extern struct Sprite *DAT_004cbfd0;
// 0x004cbfd4
extern void *DAT_004cbfd4;
// 0x004cbfd8
extern struct Sprite *DAT_004cbfd8;
// 0x004cbfdc
extern struct WaterContext *DAT_004cbfdc;
// 0x004cbfe0
extern struct WaterSub *DAT_004cbfe0;
// 0x004cbfe4
extern unsigned int DAT_004cbfe4;
// 0x004cbfe8
extern unsigned int DAT_004cbfe8;
// 0x004cbfec
extern unsigned int DAT_004cbfec;
// 0x004cbff0
extern unsigned int DAT_004cbff0;
// 0x004cbff4
extern unsigned int DAT_004cbff4;
// 0x004cbff8
extern unsigned int DAT_004cbff8;
// 0x004cbffc
extern unsigned short DAT_004cbffc;
// 0x004cc000
extern unsigned int DAT_004cc000;
// 0x004cc014
extern struct Sprite *DAT_004cc014;
// 0x004cc01c
extern unsigned int DAT_004cc01c;
// 0x004cc020
extern struct Sprite *DAT_004cc020;
// 0x004cc024
extern struct WaterContext *DAT_004cc024;
// 0x004cc028
extern unsigned int DAT_004cc028;
// 0x004cc02c
extern void *DAT_004cc02c;
// 0x004cc030
extern void *DAT_004cc030;
// 0x004cc034
extern void *DAT_004cc034;
// 0x004cc03c
extern void *DAT_004cc03c;
// 0x004cc048
extern unsigned int DAT_004cc048;
// 0x004cc060
extern unsigned int DAT_004cc060;
// 0x004cc070
extern void *DAT_004cc070;
// 0x004cc074
extern void *DAT_004cc074;
// 0x004cc078
extern unsigned int DAT_004cc078;
// 0x004cc088
extern void *DAT_004cc088;
// 0x004d823c
extern void *DAT_004d823c;
// 0x004d8250
extern const unsigned char DAT_004d8250[1];
// 0x004d8268
extern unsigned int DAT_004d8268;
// 0x004d8270
extern unsigned int DAT_004d8270;
// 0x004d88f4
extern unsigned char DAT_004d88f4[0x80];
// 0x004d8974
extern unsigned char DAT_004d8974[0x40];
// 0x004d89c4
extern void *DAT_004d89c4;
// 0x004d89c8
extern unsigned int DAT_004d89c8[1];
// 0x004d8a40
extern unsigned int DAT_004d8a40[1];
// 0x004d8abc
extern unsigned int DAT_004d8abc[1];
// 0x004d8b34
extern unsigned int DAT_004d8b34[1];
// 0x004d8bac
extern unsigned int DAT_004d8bac;
// 0x004d8bb0
extern char DAT_004d8bb0[0x100];
// 0x004dcbd0
extern void *DAT_004dcbd0[10];
// 0x004dcbf8
extern unsigned int DAT_004dcbf8;
// 0x004dcc00
extern unsigned char DAT_004dcc00[1];
// 0x004dd5d8
extern unsigned int DAT_004dd5d8;
// 0x004dd5e0
extern void *DAT_004dd5e0[24];
// 0x004dd758
extern unsigned int DAT_004dd758;
// 0x004dd760
extern char DAT_004dd760[1];
// 0x004dd860
extern unsigned int DAT_004dd860;
// 0x004dd868
extern unsigned int DAT_004dd868;
// 0x004dd86c
extern unsigned int DAT_004dd86c;
// 0x0060f924
extern unsigned int DAT_0060f924[1];
// 0x0060f938
extern unsigned int DAT_0060f938;
// 0x006102f8
extern unsigned int DAT_006102f8;
// 0x006103a8
extern unsigned int DAT_006103a8;
// 0x00610a04
extern unsigned int DAT_00610a04;
// 0x00610a08
extern unsigned int DAT_00610a08;
// 0x00611648
extern unsigned int DAT_00611648;
// 0x00611650
extern float DAT_00611650;
// 0x00611688
extern unsigned int DAT_00611688;
// 0x0061168c
extern unsigned int DAT_0061168c;
// 0x00611958
extern unsigned int DAT_00611958;
// 0x00612178
extern char DAT_00612178[1];
// 0x00616180
extern struct Cursor DAT_00616180[8];
// 0x00622320
extern struct Cursor DAT_00622320[4];
// 0x00615f6c
extern unsigned int DAT_00615f6c;
// 0x00615f98
extern unsigned int DAT_00615f98;
// 0x00616000
extern unsigned int DAT_00616000;
// 0x00616004
extern unsigned int DAT_00616004;
// 0x00616018
extern void *DAT_00616018;
// 0x00616028
extern unsigned int DAT_00616028;
// 0x0061602c
extern unsigned int DAT_0061602c;
// 0x00616030
extern unsigned int DAT_00616030;
// 0x00616034
extern unsigned short DAT_00616034;
// 0x00616048
extern struct Sprite *DAT_00616048;
// 0x0061604c
extern struct Sprite *DAT_0061604c;
// 0x00616050
extern struct Sprite *DAT_00616050;
// 0x00616054
extern struct Sprite *DAT_00616054;
// 0x00616058
extern struct Sprite *DAT_00616058;
// 0x0061605c
extern struct Sprite *DAT_0061605c;
// 0x00616060
extern void *DAT_00616060;
// 0x0061606c
extern struct Sprite *DAT_0061606c;
// 0x00616070
extern struct Sprite *DAT_00616070;
// 0x00616090
extern void *DAT_00616090;
// 0x00616094
extern void *DAT_00616094;
// 0x00616098
extern void *DAT_00616098;
// 0x006160a0
extern unsigned int DAT_006160a0;
// 0x006160a4
extern unsigned int DAT_006160a4;
// 0x006160a8
extern unsigned int DAT_006160a8;
// 0x006160ac
extern unsigned short DAT_006160ac;
// 0x006160bc
extern unsigned int DAT_006160bc;
// 0x006160c0
extern struct Sprite *DAT_006160c0;
// 0x006160c4
extern struct CarouselNode *DAT_006160c4;
// 0x006160d0
extern unsigned int DAT_006160d0;
// 0x006160d4
extern struct RinData *DAT_006160d4;
// 0x006160d8
extern struct Sprite *DAT_006160d8;
// 0x006160e0
extern struct Sprite *DAT_006160e0;
// 0x006160e4
extern struct Position *DAT_006160e4;
// 0x006160e8
extern void *DAT_006160e8;
// 0x006160f4
extern unsigned int DAT_006160f4;
// 0x006160fc
extern struct Sprite *DAT_006160fc;
// 0x00616100
extern struct Sprite *DAT_00616100;
// 0x00616104
extern struct Sprite *DAT_00616104;
// 0x00616108
extern struct Sprite *DAT_00616108;
// 0x0061610c
extern struct Sprite *DAT_0061610c;
// 0x00616120
extern unsigned int DAT_00616120;
// 0x00616124
extern unsigned int DAT_00616124;
// 0x00616128
extern unsigned int DAT_00616128;
// 0x0061612c
extern unsigned short DAT_0061612c;
// 0x0061613c
extern unsigned int DAT_0061613c;
// 0x00616144
extern struct BlokeNode *DAT_00616144;
// 0x00616148
extern struct SaveBlock *DAT_00616148;
// 0x0061614c
extern unsigned int DAT_0061614c;
// 0x00616150
extern unsigned int DAT_00616150;
// 0x00616158
extern unsigned int DAT_00616158;
// 0x0061615c
extern unsigned int DAT_0061615c;
// 0x00616164
extern struct JungleRide *DAT_00616164;
// 0x00629c2c
extern struct JungleObj *DAT_00629c2c;
// 0x00629c30
extern struct JungleFish *DAT_00629c30;
// 0x00629c34
extern struct JungleFish *DAT_00629c34;
// 0x00629c3c
extern struct JungleScore *DAT_00629c3c;
// 0x00629c40
extern int DAT_00629c40[5];
// 0x00629c50
extern void *DAT_00629c50;
// 0x00629c54
extern int DAT_00629c54;
// 0x00629c58
extern struct Cursor DAT_00629c58[4];
// 0x0062fd2c
extern struct JunglePath *DAT_0062fd2c;
// 0x0062fd30
extern struct JunglePath *DAT_0062fd30;
// 0x0062fd34
extern struct JunglePath *DAT_0062fd34;
// 0x0062fd3c
extern struct JailCell *DAT_0062fd3c;
// 0x0062fd48
extern unsigned int DAT_0062fd48;
// 0x0062fd4c
extern unsigned int DAT_0062fd4c;
// 0x0062fd50
extern unsigned int DAT_0062fd50;
// 0x0062fd54
extern unsigned short DAT_0062fd54;
// 0x0062fd60
extern void *DAT_0062fd60;
// 0x0062fd68
extern struct Sprite *DAT_0062fd68;
// 0x0062fd6c
extern struct Sprite *DAT_0062fd6c;
// 0x0062fd74
extern void *DAT_0062fd74;
// 0x0062fd7c
extern struct Sprite *DAT_0062fd7c;
// 0x0062fd80
extern struct Sprite *DAT_0062fd80;
// 0x0062fda8
extern void *DAT_0062fda8;
// 0x0062fdb0
extern unsigned int DAT_0062fdb0;
// 0x0062fdb4
extern unsigned int DAT_0062fdb4;
// 0x0062fdb8
extern unsigned int DAT_0062fdb8;
// 0x0062fdbc
extern unsigned short DAT_0062fdbc;
// 0x0062fdc8
extern void *DAT_0062fdc8;
// 0x0062fdcc
extern struct Sprite *DAT_0062fdcc;
// 0x0062fdd0
extern struct Sprite *DAT_0062fdd0;
// 0x0062fde4
extern unsigned int DAT_0062fde4;
// 0x0062fde8
extern void *DAT_0062fde8;
// 0x0062fdf0
extern void *DAT_0062fdf0[3];
// 0x0062fdfc
extern void *DAT_0062fdfc;
// 0x0062fe00
extern void *DAT_0062fe00[1];
// 0x0062fe04
extern struct Sprite *DAT_0062fe04;
// 0x0062fe08
extern void *DAT_0062fe08;
// 0x0062fe10
extern unsigned int DAT_0062fe10;
// 0x0062fe14
extern unsigned int DAT_0062fe14;
// 0x0062fe18
extern unsigned int DAT_0062fe18;
// 0x0062fe1c
extern unsigned short DAT_0062fe1c;
// 0x0062fe48
extern unsigned int DAT_0062fe48;
// 0x0062fe4c
extern struct Sprite *DAT_0062fe4c;
// 0x0062fe50
extern unsigned int DAT_0062fe50;
// 0x0062fe54
extern struct Sprite *DAT_0062fe54;
// 0x0062fe58
extern unsigned int DAT_0062fe58;
// 0x0062fe60
extern unsigned int DAT_0062fe60;
// 0x0062fe64
extern unsigned int DAT_0062fe64;
// 0x0062fe68
extern unsigned int DAT_0062fe68;
// 0x0062fe6c
extern unsigned short DAT_0062fe6c;
// 0x0062fe78
extern void *DAT_0062fe78;
// 0x0062fe84
extern void *DAT_0062fe84[3];
// 0x0062fe90
extern void *DAT_0062fe90;
// 0x0062fe94
extern void *DAT_0062fe94;
// 0x0062fe9c
extern void *DAT_0062fe9c;
// 0x0062fea8
extern void *DAT_0062fea8;
// 0x0062feac
extern void *DAT_0062feac;
// 0x0062feb0
extern void *DAT_0062feb0[2];
// 0x0062feb8
extern void *DAT_0062feb8[1];
// 0x0062febc
extern void *DAT_0062febc[6];
// 0x0062fed4
extern void *DAT_0062fed4[6];
// 0x0062feec
extern unsigned int DAT_0062feec[1];
// 0x0062fef0
extern unsigned int DAT_0062fef0;
// 0x0062fef4
extern void *DAT_0062fef4;
// 0x0062fef8
extern void *DAT_0062fef8;
// 0x00630100
extern void *DAT_00630100;
// 0x00630108
extern unsigned int DAT_00630108[1];
// 0x00638218
extern unsigned int DAT_00638218[1];
// 0x00638358
extern unsigned short DAT_00638358;
// 0x0064cd8c
extern void *DAT_0064cd8c;
// 0x00655a38
extern void *DAT_00655a38;
// 0x00655a3c
extern void *DAT_00655a3c;
// 0x00655a4c
extern unsigned int DAT_00655a4c;
// 0x00655a50
extern unsigned int DAT_00655a50;
// 0x00665e8c
extern unsigned int DAT_00665e8c;
// 0x00665f60
extern unsigned int DAT_00665f60;
// 0x00665fe8
extern unsigned int DAT_00665fe8;
// 0x00665fec
extern unsigned int DAT_00665fec;
// 0x00665ff8
extern unsigned int DAT_00665ff8;
// 0x00665ffc
extern unsigned int DAT_00665ffc;
// 0x00666000
extern unsigned int DAT_00666000;
// 0x00666004
extern unsigned int DAT_00666004;
// 0x00666008
extern unsigned int DAT_00666008;
// 0x0066600c
extern unsigned int DAT_0066600c;
// 0x00666010
extern unsigned int DAT_00666010;
// 0x00666014
extern unsigned int DAT_00666014;
// 0x00666018
extern unsigned int DAT_00666018;
// 0x0066601c
extern unsigned int DAT_0066601c;
// 0x00666020
extern unsigned int DAT_00666020;
// 0x00666024
extern unsigned int DAT_00666024;
// 0x00666030
extern unsigned int DAT_00666030;
// 0x00666034
extern unsigned int DAT_00666034;
// 0x00666038
extern unsigned int DAT_00666038;
// 0x0066603c
extern unsigned int DAT_0066603c;
// 0x00666040
extern unsigned int DAT_00666040;
// 0x00666044
extern unsigned int DAT_00666044;
// 0x00666048
extern unsigned int DAT_00666048;
// 0x0066604c
extern unsigned int DAT_0066604c;
// 0x00666050
extern unsigned int DAT_00666050;
// 0x00666054
extern unsigned int DAT_00666054;
// 0x00666058
extern unsigned int DAT_00666058;
// 0x0066605c
extern unsigned int DAT_0066605c;
// 0x00666060
extern unsigned int DAT_00666060;
// 0x00666064
extern unsigned int DAT_00666064;
// 0x00666068
extern unsigned int DAT_00666068;
// 0x0066606c
extern unsigned int DAT_0066606c;
// 0x00666070
extern unsigned int DAT_00666070;
// 0x00666074
extern unsigned int DAT_00666074;
// 0x00666078
extern unsigned int DAT_00666078;
// 0x0066607c
extern unsigned int DAT_0066607c;
// 0x00666080
extern unsigned int DAT_00666080;
// 0x00666084
extern unsigned int DAT_00666084;
// 0x00666088
extern unsigned int DAT_00666088;
// 0x0066608c
extern unsigned int DAT_0066608c;
// 0x00666090
extern unsigned int DAT_00666090;
// 0x00666094
extern unsigned int DAT_00666094;
// 0x00666098
extern unsigned int DAT_00666098;
// 0x006660a8
extern unsigned int DAT_006660a8;
// 0x006660ac
extern unsigned int DAT_006660ac;
// 0x006660b0
extern char DAT_006660b0[256];
// 0x006661bc
extern int DAT_006661bc;
// 0x006661c4
extern unsigned int DAT_006661c4;
// 0x006661c8
extern int DAT_006661c8;
// 0x006664f8
extern unsigned int DAT_006664f8[768];
// 0x006670f8
extern unsigned int DAT_006670f8;
// 0x006670fc
extern unsigned int DAT_006670fc;
// 0x00667104
extern unsigned int DAT_00667104;
// 0x00667108
extern unsigned int DAT_00667108;
// 0x0066710c
extern unsigned int DAT_0066710c;
// 0x00667114
extern unsigned int DAT_00667114;
// 0x00667118
extern unsigned int DAT_00667118;
// 0x0066711c
extern unsigned int DAT_0066711c;
// 0x00667120
extern unsigned int DAT_00667120;
// 0x00667128
extern char DAT_00667128[512];
// 0x006675b4
extern unsigned int DAT_006675b4;
// 0x006675b8
extern int DAT_006675b8;
// 0x006675c0
extern struct TextCell DAT_006675c0[50];
// 0x00667c10
extern unsigned int DAT_00667c10;
// 0x00667c28
extern unsigned int DAT_00667c28;
// 0x00667c2c
extern struct Sprite *DAT_00667c2c;
// 0x00667c30
extern unsigned int DAT_00667c30;
// 0x00667c34
extern struct Sprite *DAT_00667c34;
// 0x00667c54
extern LEGO_EXPORT unsigned short QueryObj;
// 0x00667c58
extern LEGO_EXPORT void *QueryClass;
// 0x00667c60
extern unsigned int DAT_00667c60;
// 0x00667c64
extern const char *DAT_00667c40;
// 0x00667c48
extern unsigned int DAT_00667c48;
extern unsigned int DAT_00667c64;
extern unsigned int DAT_00667c78;
// 0x00667c7c
extern unsigned int DAT_00667c7c;
// 0x00667c80
extern unsigned int DAT_00667c80;
// 0x00667c88
extern struct Sprite *DAT_00667c88;
// 0x00667c8c
extern struct Sprite *DAT_00667c8c;
// 0x00667c90
extern struct Sprite *DAT_00667c90;
// 0x00667c94
extern struct Sprite *DAT_00667c94;
// 0x00667c9c
extern void *DAT_00667c9c;
// 0x00667ca4
extern unsigned int DAT_00667ca4;
// 0x00667ca8
extern LEGO_EXPORT void *OverlayList;
// 0x00667cac
extern LEGO_EXPORT unsigned int OverlayILF;
// 0x00667cb4
extern LEGO_EXPORT int ScrollX;
// 0x00667cb8
extern LEGO_EXPORT int ScrollY;
// 0x00667cbc
extern LEGO_EXPORT int ScrollSpeedX;
// 0x00667cc0
extern LEGO_EXPORT int ScrollSpeedY;
// 0x00667cd8
extern unsigned int DAT_00667cd8;
// 0x00667ce0
extern int DAT_00667ce0;
// 0x00667ce4
extern int DAT_00667ce4;
// 0x00667ce8
extern int DAT_00667ce8;
// 0x00667cec
extern int DAT_00667cec;
// 0x00667cf0
extern int DAT_00667cf0;
// 0x00667cf4
extern int DAT_00667cf4;
// 0x00667cf8
extern int DAT_00667cf8;
// 0x00667cfc
extern int DAT_00667cfc;
// 0x00667d08
extern int DAT_00667d08;
// 0x00667d10
extern unsigned int DAT_00667d10;
// 0x00667d3c
extern int DAT_00667d3c;
// 0x00667d40
extern unsigned int DAT_00667d40;
// 0x00667d44
extern int DAT_00667d44;
// 0x00667d48
extern unsigned int DAT_00667d48;
// 0x00667d54
extern unsigned int DAT_00667d54;
// 0x00667d58
extern int DAT_00667d58;
// 0x00667d60
extern int DAT_00667d60;
// 0x00667d68
extern unsigned int DAT_00667d68;
// 0x00667d6c
extern int DAT_00667d6c;
// 0x00667d70
extern LEGO_EXPORT unsigned int DDRAWENV[246];
// 0x00668070
extern void *DAT_00668070;
// 0x00668078
extern void *DAT_00668078;
// 0x0066807c
extern void *renderEngine;
// 0x00668080
extern void *DAT_00668080;
// 0x00668084
extern unsigned int DAT_00668084;
// 0x00668088
extern unsigned int DAT_00668088;
// 0x0066808c
extern void *PTR_0066808c;
// 0x00668090
extern void *PTR_00668090;
// 0x00668094
extern void *PTR_00668094;
// 0x00668098
extern void *PTR_00668098;
// 0x0066809c
extern struct SoftPrintState DAT_0066809c;
// 0x00668108
extern int DAT_00668108;
// 0x0066810c
extern int DAT_0066810c;
// 0x00668110
extern int DAT_00668110;
// 0x00668114
extern int DAT_00668114;
// 0x00668118
extern int renderEngineTargetIdx;
// 0x0066811c
extern void *renderEngineTargets[1];
// 0x00668144
extern int DAT_00668144;
// 0x00668148
extern struct Sprite *DAT_00668148;
// 0x0066814c
extern unsigned int DAT_0066814c;
// 0x00668164
extern int DAT_00668164[32];
// 0x006681e4
extern int DAT_006681e4;
// 0x006681e8
extern unsigned int DAT_006681e8;
// 0x006681f0
extern unsigned int DAT_006681f0;
// 0x006681f4
extern unsigned int DAT_006681f4;
// 0x006681f8
extern int DAT_006681f8;
// 0x006681fc
extern LEGO_EXPORT unsigned int LastFrameMS;
// 0x00668200
extern unsigned int DAT_00668200;
// 0x00668204
extern unsigned int DAT_00668204;
// 0x00668208
extern struct Sprite *DAT_00668208;
// 0x00668610
extern unsigned int DAT_00668610;
// 0x00668614
extern unsigned int DAT_00668614;
// 0x00668618
extern unsigned int DAT_00668618;
// 0x0066861c
extern unsigned char DAT_0066861c;
// 0x0066869b
extern unsigned char DAT_0066869b;
// 0x0066869c
extern char DAT_0066869c[128];
// 0x0066871c
extern unsigned int DAT_0066871c;
// 0x00668720
extern unsigned int DAT_00668720;
// 0x00668724
extern struct ObjectiveEvent *DAT_00668724;
// 0x00668728
extern struct ObjectiveEvent *DAT_00668728;
// 0x0066872c
extern unsigned int DAT_0066872c[0x15];
// 0x00668780
extern unsigned int DAT_00668780;
// 0x00668784
extern void *DAT_00668784;
// 0x00668788
extern unsigned int DAT_00668788;
// 0x0066878c
extern int DAT_0066878c;
// 0x00668790
extern unsigned int DAT_00668790;
// 0x00668794
extern unsigned int DAT_00668794;
// 0x00668798
extern void *DAT_00668798;
// 0x0066879c
extern unsigned int DAT_0066879c;
// 0x006687a0
extern unsigned int DAT_006687a0;
// 0x006687a4
extern unsigned int DAT_006687a4;
// 0x006687a8
extern unsigned int DAT_006687a8;
// 0x006687ac
extern unsigned int DAT_006687ac;
// 0x006687b0
extern unsigned int DAT_006687b0;
// 0x006687bc
extern unsigned int DAT_006687bc;
// 0x006687c0
extern unsigned int DAT_006687c0;
// 0x006687c8
extern struct IconNode *DAT_006687c8;
// 0x006687cc
extern struct IconNode *DAT_006687cc;
// 0x006687d0
extern LEGO_EXPORT void *FocussedIconPtr;
// 0x00668828
extern struct Sprite *DAT_00668828;
// 0x0066882c
extern struct Sprite *DAT_0066882c;
// 0x00668830
extern struct Sprite *DAT_00668830;
// 0x00668834
extern struct Sprite *DAT_00668834;
// 0x00668840
extern short DAT_00668840;
// 0x0066884c
extern short DAT_0066884c;
// 0x00668858
extern struct IconNode DAT_00668858;
// 0x006688a8
extern void *DAT_006688a8;
// 0x006688ac
extern void *DAT_006688ac;
// 0x006688b0
extern void *DAT_006688b0;
// 0x006688b4
extern unsigned int DAT_006688b4;
// 0x006688b8
extern unsigned int DAT_006688b8;
// 0x006688bc
extern int DAT_006688bc;
// 0x006688c0
extern int DAT_006688c0;
// 0x006688c4
extern int DAT_006688c4;
// 0x006688c8
extern unsigned int DAT_006688c8;
// 0x006688cc
extern unsigned int DAT_006688cc;
// 0x006688d0
extern unsigned int DAT_006688d0;
// 0x006688d4
extern void *DAT_006688d4;
// 0x006688d8
extern void *DAT_006688d8;
// 0x006688e0
extern struct Sprite *DAT_006688e0;
// 0x006688e4
extern struct Sprite *DAT_006688e4;
// 0x006688e8
extern struct Sprite *DAT_006688e8;
// 0x006688ec
extern struct Sprite *DAT_006688ec;
// 0x006688f0
extern struct Sprite *DAT_006688f0;
// 0x006688f4
extern struct Sprite *DAT_006688f4;
// 0x006688f8
extern struct Sprite *DAT_006688f8;
// 0x006688fc
extern struct Sprite *DAT_006688fc;
// 0x00668900
extern struct Sprite *DAT_00668900;
// 0x00668904
extern struct Sprite *DAT_00668904;
// 0x00668908
extern struct Sprite *DAT_00668908;
// 0x0066890c
extern struct Sprite *DAT_0066890c;
// 0x00668910
extern struct Sprite *DAT_00668910;
// 0x00668914
extern struct Sprite *DAT_00668914;
// 0x00668918
extern struct Sprite *DAT_00668918;
// 0x0066891c
extern struct Sprite *DAT_0066891c;
// 0x00668920
extern struct Sprite *DAT_00668920;
// 0x00668924
extern struct Sprite *DAT_00668924;
// 0x00668928
extern struct Sprite *DAT_00668928;
// 0x0066892c
extern struct Sprite *DAT_0066892c;
// 0x00668930
extern struct Sprite *DAT_00668930;
// 0x00668934
extern struct Sprite *DAT_00668934;
// 0x00668938
extern struct Sprite *DAT_00668938;
// 0x0066893c
extern struct Sprite *DAT_0066893c;
// 0x00668940
extern struct Sprite *DAT_00668940;
// 0x00668944
extern struct Sprite *DAT_00668944;
// 0x00668948
extern struct Sprite *DAT_00668948;
// 0x0066894c
extern struct Sprite *DAT_0066894c;
// 0x00668950
extern struct Sprite *DAT_00668950;
// 0x00668954
extern unsigned int DAT_00668954;
// 0x00668958
extern struct Sprite *DAT_00668958;
// 0x00668960
extern unsigned int DAT_00668960;
// 0x00668964
extern int DAT_00668964;
// 0x00668968
extern char DAT_00668968[0x400];
// 0x00668d68
extern unsigned int DAT_00668d68;
// 0x00668d78  (DIMOUSESTATE; full type in <dinput.h>, dereferenced only by input.c)
extern struct _DIMOUSESTATE DAT_00668d78;
// 0x00668d88
extern void *dinput;
// 0x00668d8c
extern void *dinput_keyboard;
// 0x00668d90
extern void *dintput_mouse;
// 0x00668d94
extern char DAT_00668d94[0x14];
// 0x00668da8
extern unsigned char DAT_00668da8[0x3b];
// 0x00668de4
extern unsigned char DAT_00668de4[0x3b];
// 0x00668e20
extern unsigned int DAT_00668e20[4];
// 0x00668e34
extern unsigned int DAT_00668e34;
// 0x00668e38
extern unsigned int DAT_00668e38;
// 0x00668e40
extern struct InterfaceListNode *DAT_00668e40;
// 0x00668e44
extern int DAT_00668e44[8];
// 0x00668e64
extern unsigned char DAT_00668e64;
// 0x00668e68
extern struct Sprite *DAT_00668e68;
// 0x00668e6c
extern struct Sprite *DAT_00668e6c;
// 0x00668e70
extern struct Sprite *DAT_00668e70;
// 0x00668e74
extern struct Sprite *DAT_00668e74;
// 0x00668e78
extern struct Sprite *DAT_00668e78;
// 0x00668e7c
extern struct Sprite *DAT_00668e7c;
// 0x00668e80
extern struct Sprite *DAT_00668e80;
// 0x00668e84
extern struct Sprite *DAT_00668e84;
// 0x00668e88
extern struct Sprite *DAT_00668e88;
// 0x00668e8c
extern struct Sprite *DAT_00668e8c;
// 0x00668e90
extern struct Sprite *DAT_00668e90;
// 0x00668e94
extern struct Sprite *DAT_00668e94;
// 0x00668e98
extern struct Sprite *DAT_00668e98;
// 0x00668e9c
extern void *DAT_00668e9c;
// 0x00668ea0
extern struct Sprite *DAT_00668ea0;
// 0x00668ea4
extern unsigned int DAT_00668ea4;
// 0x00668eb0
extern unsigned int DAT_00668eb0;
// 0x00668eb4
extern unsigned int DAT_00668eb4;
// 0x00668eb8
extern unsigned int DAT_00668eb8;
// 0x00668ebc
extern unsigned int DAT_00668ebc;
extern unsigned int DAT_00668ec0;
// 0x00668ed8
extern struct InterfaceResearchNode *DAT_00668ed8;
extern unsigned int DAT_00668f98;
// 0x00668fc0
extern void *DAT_00668fc0;
// 0x00668fc4
extern struct InterfaceQueryNode *DAT_00668fc4;
// 0x00669050
extern unsigned char DAT_00669050;
// 0x00669054
extern unsigned int DAT_00669054;
// 0x00669058
extern char DAT_00669058[0x40];
// 0x00669098
extern unsigned int DAT_00669098;
// 0x006691a0
extern unsigned int DAT_006691a0;
// 0x006691a4
extern unsigned int DAT_006691a4;
// 0x006691a8
extern struct Element **DAT_006691a8;
// 0x006691b0
extern int DAT_006691b0;
// 0x006691bc
extern int DAT_006691bc[16];
// 0x006691fc
extern int DAT_006691fc;
// 0x00669200
extern unsigned int *DAT_00669200;
// 0x00669204
extern unsigned int DAT_00669204;
// 0x00669208
extern void *g_hInstance;
// 0x0066920c
extern int DAT_0066920c;
// 0x00669210
extern void *DAT_00669210;
// 0x00669240
extern LEGO_EXPORT void *ObjectClassList;
// 0x00669248
extern void *DAT_00669248;
// 0x0066924c
extern unsigned int DAT_0066924c;
// 0x00669258
extern unsigned int DAT_00669258[1152];
// 0x0066b44c
extern void *DAT_0066b44c;
// 0x0066b450
extern void *DAT_0066b450;
// 0x0066b458
extern void *DAT_0066b458;
// 0x0066b460
extern unsigned int DAT_0066b460;
// 0x0066b468
extern unsigned int DAT_0066b468;
// 0x0066b46c
extern unsigned int DAT_0066b46c;
// 0x0066b574
extern LEGO_EXPORT void *FirstBloke;
// 0x0066b57c
extern void *DAT_0066b57c;
// 0x0066b5a4
extern struct SortNode *DAT_0066b5a4;
// 0x0066b5a8
extern unsigned int DAT_0066b5a8;
// 0x0066b5ac
extern unsigned int DAT_0066b5ac;
// 0x0066b5b0
extern struct SoftPrintState DAT_0066b5b0;
// 0x0066b61c
extern unsigned int DAT_0066b61c;
// 0x0066b620
extern int DAT_0066b620;
// 0x0066b624
extern int DAT_0066b624;
// 0x0066b628
extern int DAT_0066b628;
// 0x0066b62c
extern int DAT_0066b62c;
// 0x0066b630
extern void *DAT_0066b630;
// 0x0066be40
extern unsigned int DAT_0066be40;
// 0x0066be44
extern unsigned int DAT_0066be44;
// 0x0066be48
extern unsigned int DAT_0066be48;
// 0x0066be4c
extern unsigned int DAT_0066be4c;
// 0x0066be50
extern struct Image *DAT_0066be50;
// 0x0066be54
extern unsigned short DAT_0066be54[0x4b002];
// 0x00701e58
extern unsigned int DAT_00701e58;
// 0x00701e5c
extern unsigned char *DAT_00701e5c;
// 0x00701e60
extern unsigned int DAT_00701e60;
// 0x00701e64
extern struct Sprite *DAT_00701e64;
// 0x00701e68
extern unsigned short DAT_00701e68[0x4b000];
// 0x00797e68
extern unsigned int DAT_00797e68;
// 0x00797e6c
extern void *DAT_00797e6c;
// 0x00798190
extern void *DAT_00798190[256];
// 0x00798590
extern void *DAT_00798590;
// 0x00798624
extern void *DAT_00798624;
// 0x00798628
extern void *DAT_00798628;
// 0x0079862c
extern void *DAT_0079862c;
// 0x00798630
extern unsigned int DAT_00798630;
// 0x00798634
extern unsigned int DAT_00798634;
// 0x00798638
extern unsigned int DAT_00798638;
// 0x0079863c
extern unsigned int DAT_0079863c;
// 0x00798648
extern unsigned int DAT_00798648;
// 0x00798660
extern unsigned int DAT_00798660;
// 0x00798664
extern unsigned int DAT_00798664;
// 0x00798668
extern unsigned int DAT_00798668;
// 0x00798674
extern struct Sprite *DAT_00798674;
// 0x00798678
extern struct Sprite *DAT_00798678;
// 0x0079867c
extern struct Sprite *DAT_0079867c;
// 0x00798680
extern struct Sprite *DAT_00798680;
// 0x00798684
extern struct Sprite *DAT_00798684;
// 0x00798688
extern struct Sprite *DAT_00798688;
// 0x0079868c
extern struct Sprite *DAT_0079868c;
// 0x00798690
extern struct Sprite *DAT_00798690;
// 0x00798694
extern struct Sprite *DAT_00798694;
// 0x00798698
extern struct Sprite *DAT_00798698;
// 0x0079869c
extern struct Sprite *DAT_0079869c;
// 0x007986a0
extern struct Sprite *DAT_007986a0;
// 0x007986a4
extern struct Sprite *DAT_007986a4;
// 0x007986a8
extern struct Sprite *DAT_007986a8;
// 0x007986ac
extern struct Sprite *DAT_007986ac;
// 0x007986b0
extern struct Sprite *DAT_007986b0;
// 0x007986b4
extern struct Sprite *DAT_007986b4;
// 0x007986b8
extern struct Sprite *DAT_007986b8;
// 0x007986bc
extern struct Sprite *DAT_007986bc;
// 0x007986c0
extern struct Sprite *DAT_007986c0;
// 0x007986c4
extern struct Sprite *DAT_007986c4;
// 0x007986c8
extern struct Sprite *DAT_007986c8;
// 0x007986cc
extern struct Sprite *DAT_007986cc;
// 0x007986d0
extern struct Sprite *DAT_007986d0;
// 0x007986d4
extern struct Sprite *DAT_007986d4;
// 0x007986d8
extern unsigned int DAT_007986d8;
// 0x007986dc
extern unsigned int DAT_007986dc;
// 0x007986e0
extern unsigned int DAT_007986e0;
// 0x007986e4
extern unsigned int DAT_007986e4;
// 0x007986e8
extern unsigned int DAT_007986e8;
// 0x007986f4
extern unsigned int DAT_007986f4;
// 0x007986f8
extern unsigned int DAT_007986f8;
// 0x00798704
extern struct Sprite *DAT_00798704;
// 0x00798708
extern struct Sprite *DAT_00798708;
// 0x0079870c
extern struct Sprite *DAT_0079870c;
// 0x00798710
extern struct Sprite *DAT_00798710;
// 0x00798714
extern struct Sprite *DAT_00798714;
// 0x00798718
extern struct Sprite *DAT_00798718;
// 0x0079871c
extern struct Sprite *DAT_0079871c;
// 0x00798720
extern struct Sprite *DAT_00798720;
// 0x00798724
extern struct Sprite *DAT_00798724;
// 0x00798728
extern struct Sprite *DAT_00798728;
// 0x0079872c
extern struct Sprite *DAT_0079872c;
// 0x00798730
extern struct Sprite *DAT_00798730;
// 0x00798734
extern void *DAT_00798734;
// 0x00798738
extern int DAT_00798738;
// 0x0079873c
extern unsigned int DAT_0079873c;
// 0x00798740
extern unsigned int DAT_00798740;
// 0x00798764
extern struct Sprite *DAT_00798764;
// 0x00798768
extern int DAT_00798768;
// 0x0079876c
extern unsigned int DAT_0079876c;
// 0x00798770
extern unsigned int DAT_00798770;
// 0x00798777
extern char DAT_00798777;
// 0x00798778
extern char DAT_00798778[0x100];
// 0x00798878
extern unsigned int DAT_00798878;
// 0x0079887c
extern unsigned int DAT_0079887c;
// 0x00798880
extern unsigned int DAT_00798880;
// 0x00798884
extern unsigned int DAT_00798884;
// 0x00798888
extern unsigned int DAT_00798888;
// 0x00798890
extern void *DAT_00798890;
// 0x00798894
extern int DAT_00798894;
// 0x00798898
extern unsigned int DAT_00798898;
// 0x0079889c
extern unsigned int DAT_0079889c;
// 0x007988a0
extern unsigned int DAT_007988a0;
// 0x007988a4
extern void *DAT_007988a4;
// 0x007988a8
extern void *DAT_007988a8;
// 0x007988b0
extern void *DAT_007988b0;
// 0x007988bc
extern unsigned int DAT_007988bc;
// 0x007988c0
extern unsigned int DAT_007988c0;
// 0x007988c4
extern unsigned int DAT_007988c4;
// 0x007988c8
extern unsigned int DAT_007988c8;
// 0x007988cc
extern void *DAT_007988cc;
// 0x0079a694
extern LEGO_EXPORT unsigned int DMusicInitialised;
// 0x0079a698
extern void *DAT_0079a698;
// 0x0079a6a0
extern void *DAT_0079a6a0;
// 0x0079a6a4
extern unsigned int DAT_0079a6a4;
// 0x0079a6a8
extern unsigned int DAT_0079a6a8;
// 0x0079a6ac
extern unsigned int DAT_0079a6ac;
// 0x0079a7d0
extern unsigned int DAT_0079a7d0;
// 0x0079a7d8
extern int DAT_0079a7d8;
// 0x0079a7dc
extern int DAT_0079a7dc;
// 0x0079a7e0
extern unsigned int DAT_0079a7e0;
// 0x0079a7e4
extern unsigned int DAT_0079a7e4[20];
// 0x0079a834
extern int DAT_0079a834;
// 0x0079a838
extern int DAT_0079a838;
// 0x0079a83c
extern unsigned int DAT_0079a83c;
// 0x0079a848
extern void *DAT_0079a848;
// 0x0079a84c
extern unsigned int DAT_0079a84c;
// 0x0079a850
extern void *strings[10];
// 0x0079a878
extern unsigned int DAT_0079a878[6];
// 0x0079a890
extern unsigned int DAT_0079a890;
// 0x0079a894
extern unsigned int DAT_0079a894;
// 0x0079a898
extern unsigned int DAT_0079a898;
// 0x0079a89c
extern unsigned int DAT_0079a89c;
// 0x0079a8a0
extern unsigned int DAT_0079a8a0;
// 0x0079a8a8
extern LEGO_EXPORT void *GardenerList;
// 0x0079a8ac
extern LEGO_EXPORT void *MechanicList;
// 0x0079a8b0
extern void *DAT_0079a8b0;
// 0x0079a8b4
extern void *DAT_0079a8b4;
// 0x0079a8b8
extern int DAT_0079a8b8;
// 0x0079a8bc
extern int DAT_0079a8bc;
// 0x0079a8c0
extern void *DAT_0079a8c0;
// 0x0079a8c4
extern void *DAT_0079a8c4;
// 0x0079a8c8
extern int DAT_0079a8c8;
// 0x0079a8cc
extern int DAT_0079a8cc;
// 0x0079a8d0
extern unsigned int DAT_0079a8d0;
// 0x0079a8d4
extern void *DAT_0079a8d4;
// 0x0079abfc
extern void *DAT_0079abfc;
// 0x0079ac04
extern unsigned int DAT_0079ac04;
// 0x007caca8
extern unsigned int DAT_007caca8;
// 0x007cacac
extern unsigned int DAT_007cacac;
// 0x007cacb4
extern unsigned int DAT_007cacb4;
// 0x007cacd4
extern LEGO_EXPORT unsigned int FrameNumber;
// 0x007cacd8
extern void *DAT_007cacd8;
// 0x007cacdc
extern void *DAT_007cacdc;
// 0x007cad40
extern void *DAT_007cad40;
// 0x007cad44
extern void *DAT_007cad44;
// 0x007cad48
extern unsigned int DAT_007cad48;
// 0x007cad60
extern struct Settings DAT_007cad60;
// 0x007cae80
extern char DAT_007cae80[0x100];
// 0x007caf80
extern struct Sprite *DAT_007caf80;
// 0x007cafa0
extern void *DAT_007cafa0;
// 0x007cb140
extern void *DAT_007cb140;
// 0x007cb1c0
extern struct IconNode *DAT_007cb1c0;
// 0x007cb1c4
extern struct Sprite *DAT_007cb1c4;
// 0x007cb1e0
extern char DAT_007cb1e0[0x100];
// 0x007cb2e0
extern struct IconNode *DAT_007cb2e0;
// 0x007cb2e4
extern struct IconNode *DAT_007cb2e4;
// 0x007cb2f0
extern char DAT_007cb2f0[0x10];
// 0x007cb300
extern char DAT_007cb300[0xc];
// 0x007cb30c
extern char DAT_007cb30c[4];
// 0x007cb310
extern unsigned int DAT_007cb310;
// 0x007cb314
extern char DAT_007cb314;
// 0x007cb315
extern char DAT_007cb315;
// 0x007cb31c
extern char DAT_007cb31c;
// 0x007cb320
extern unsigned int DAT_007cb320;
// 0x007cb324
extern unsigned int DAT_007cb324;
// 0x007cb328
extern unsigned int DAT_007cb328;
// 0x007cb360
extern unsigned int DAT_007cb360;
// 0x007cb394
extern unsigned int DAT_007cb394;
// 0x007cb398
extern struct Sprite *DAT_007cb398;
// 0x007cb3a0
extern unsigned int DAT_007cb3a0;
// 0x007cb3a8
extern struct Sprite *DAT_007cb3a8;
// 0x007cb3ac
extern struct Sprite *DAT_007cb3ac;
// 0x007cb3b0
extern struct Sprite *DAT_007cb3b0;
// 0x007cb3b4
extern struct Sprite *DAT_007cb3b4;
// 0x007cb3c0
extern struct Sprite *DAT_007cb3c0;
// 0x007cb3c4
extern struct Sprite *DAT_007cb3c4;
// 0x007cb3c8
extern struct Sprite *DAT_007cb3c8;
// 0x007cb3cc
extern struct Sprite *DAT_007cb3cc;
// 0x007cb3d4
extern struct Sprite *DAT_007cb3d4;
// 0x007cb3e0
extern struct ObjTableEntry DAT_007cb3e0[128];
// 0x007cb3e2
extern struct ObjTableEntry DAT_007cb3e2[128];
// 0x007cb5e0
extern struct ObjTableEntry DAT_007cb5e0;
// 0x007cb600
extern unsigned char DAT_007cb600[1];
// 0x007fd600
extern struct SortNode *DAT_007fd600;
// 0x007fd620
extern LEGO_EXPORT void *NewObjectPtr;
// 0x007fd624
extern unsigned int DAT_007fd624;
// 0x007fd630
extern unsigned int DAT_007fd630;
// 0x007fd634
extern void *DAT_007fd634;
// 0x007fd638
extern unsigned int DAT_007fd638;
// 0x007fd640
extern struct ResVolume *DAT_007fd640[4];
// 0x007fdb88
extern unsigned int DAT_007fdb88;
// 0x007fdca0
extern int DAT_007fdca0;
// 0x007fdca4
extern unsigned int DAT_007fdca4;
// 0x007fdcc0
extern struct Sprite *DAT_007fdcc0;
// 0x007fdcc4
extern struct Sprite *DAT_007fdcc4;
// 0x007fdcc8
extern struct Sprite *DAT_007fdcc8;
// 0x007fdccc
extern struct Sprite *DAT_007fdccc;
// 0x007fdcd0
extern struct Sprite *DAT_007fdcd0;
// 0x007fdcd4
extern struct Sprite *DAT_007fdcd4;
// 0x007fdcd8
extern struct Sprite *DAT_007fdcd8;
// 0x007fdcdc
extern struct Sprite *DAT_007fdcdc;
// 0x007fdce0
extern struct Sprite *DAT_007fdce0;
// 0x007fdd00
extern unsigned int DAT_007fdd00[9];
// 0x007fdd40
extern struct Sprite *DAT_007fdd40;
// 0x007fdd44
extern struct Sprite *DAT_007fdd44;
// 0x007fdd48
extern struct Sprite *DAT_007fdd48;
// 0x007fdd4c
extern struct Sprite *DAT_007fdd4c;
// 0x007fdd50
extern struct Sprite *DAT_007fdd50;
// 0x007fdd54
extern struct Sprite *DAT_007fdd54;
// 0x007fdd58
extern struct Sprite *DAT_007fdd58;
// 0x007fdd5c
extern struct Sprite *DAT_007fdd5c;
// 0x007fdd60
extern struct Sprite *DAT_007fdd60;
// 0x007fdd70
extern struct InterfaceProfileObj *DAT_007fdd70[4];
// 0x007fdd80
extern unsigned char DAT_007fdd80;
// 0x007fdd84
extern unsigned int DAT_007fdd84;
// 0x007fdd88
extern unsigned int DAT_007fdd88;
// 0x007fdd8c
extern unsigned char DAT_007fdd8c;
// 0x007fdda0
extern unsigned char DAT_007fdda0[256];
// 0x007fdea4
extern struct InfoIcon *DAT_007fdea4;
// 0x007fdea8
extern struct InfoIcon *DAT_007fdea8;
// 0x007fdeac
extern struct Sprite *DAT_007fdeac;
// 0x007fdeb0
extern struct Sprite *DAT_007fdeb0;
// 0x007fdec0
extern unsigned int DAT_007fdec0;
// 0x007fdec4
extern void *DAT_007fdec4;
// 0x007fdec8
extern unsigned int DAT_007fdec8;
// 0x007fdecc
extern int DAT_007fdecc;
// 0x007fded0
extern int DAT_007fded0;
// 0x007fded4
extern void *DAT_007fded4;
// 0x007fdf24
extern struct Sprite *DAT_007fdf24;
// 0x007fdf74
extern unsigned int DAT_007fdf74;
// 0x007fdf78
extern int DAT_007fdf78;
// 0x007fdf7c
extern unsigned int DAT_007fdf7c;
// 0x007fdf80
extern struct InfoObjData *DAT_007fdf80;
// 0x007fdf84
extern unsigned char *DAT_007fdf84;
// 0x007fdf88
extern unsigned short DAT_007fdf88;
// 0x007fdf8c
extern unsigned int DAT_007fdf8c;
// 0x007fdf90
extern unsigned int DAT_007fdf90;
// 0x007fdf94
extern unsigned int DAT_007fdf94;
// 0x007fdf98
extern unsigned int DAT_007fdf98;
// 0x007fdf9c
extern unsigned int DAT_007fdf9c;
// 0x007fdfa0
extern unsigned int DAT_007fdfa0;
// 0x007fdfa4
extern unsigned int DAT_007fdfa4;
// 0x007fdfa8
extern unsigned int DAT_007fdfa8;
// 0x007fdfac
extern unsigned char DAT_007fdfac;
// 0x007fdfb0
extern unsigned int DAT_007fdfb0;
// 0x007fdfb4
extern unsigned int DAT_007fdfb4;
// 0x007fdfb8
extern unsigned int DAT_007fdfb8;
// 0x007fdfbc
extern unsigned int DAT_007fdfbc;
// 0x007fdfc0
extern struct InfoIcon *DAT_007fdfc0;
// 0x007fdfc4
extern struct InfoIcon *DAT_007fdfc4;
// 0x007fdfc8
extern struct Sprite *DAT_007fdfc8;
// 0x007fdfcc
extern struct InfoIcon *DAT_007fdfcc;
// 0x007fdfd0
extern struct Sprite *DAT_007fdfd0;
// 0x007fdfd8
extern struct InfoIcon *DAT_007fdfd8;
// 0x007fdfdc
extern struct InfoIcon *DAT_007fdfdc;
// 0x007fdfe0
extern struct InfoIcon *DAT_007fdfe0;
// 0x007fdfe4
extern struct Sprite *DAT_007fdfe4;
// 0x007fdfe8
extern struct InfoIcon *DAT_007fdfe8;
// 0x007fdff0
extern void *DAT_007fdff0;
// 0x007fdffc
extern unsigned int DAT_007fdffc;
// 0x007fe000
extern struct InfoIcon *DAT_007fe000;
// 0x007fe004
extern struct Sprite *DAT_007fe004;
// 0x007fe008
extern struct Sprite *DAT_007fe008;
// 0x007fe010
extern int DAT_007fe010;
// 0x007fe014
extern int DAT_007fe014;
// 0x007fe018
extern struct Sprite *DAT_007fe018;
// 0x007fe020
extern unsigned int DAT_007fe020[4];
// 0x007fe040
extern unsigned int DAT_007fe040;
// 0x007fe044
extern unsigned int DAT_007fe044;
// 0x007fe048
extern unsigned int DAT_007fe048;
// 0x007fe04c
extern unsigned int DAT_007fe04c;
// 0x007fe050
extern unsigned int DAT_007fe050;
// 0x007fe054
extern unsigned int DAT_007fe054;
// 0x007fe114
extern unsigned char DAT_007fe114;
// 0x007fe115
extern unsigned char DAT_007fe115;
// 0x007fe116
extern unsigned char DAT_007fe116;
// 0x007fe117
extern unsigned char DAT_007fe117;
// 0x007fe120
extern unsigned int DAT_007fe120[256];
// 0x007fe920
extern unsigned int DAT_007fe920;
// 0x007fe930
extern unsigned char DAT_007fe930[10];
// 0x007fe994
extern unsigned int DAT_007fe994;
// 0x007fe9a8
extern unsigned int DAT_007fe9a8;
// 0x007fe9c0
extern struct Sprite *DAT_007fe9c0[9];
// 0x007fea30
extern RECT DAT_007fea30;
// 0x007fea44
extern unsigned int DAT_007fea44;
// 0x007fea48
extern LEGO_EXPORT unsigned int FramesPerSecond;
// 0x007feb14
extern unsigned int DAT_007feb14;
// 0x007febb8
extern unsigned short DAT_007febb8;
// 0x007febc0
extern LEGO_EXPORT struct Cursor EditCursor;
// 0x008003f8
extern unsigned int DAT_008003f8;
// 0x00800400
extern LEGO_EXPORT unsigned int ObjectPartArray[1024];
// 0x00801400
extern LEGO_EXPORT struct MapElement **GameMap;
// 0x00801420
extern struct DeferredSprite DAT_00801420[100];
// 0x00801a6c
extern void *DAT_00801a6c;
// 0x00801a80
extern struct MapRect DAT_00801a80[10];
// 0x00801b20
extern unsigned int DAT_00801b20;
// 0x00801b24
extern LEGO_EXPORT unsigned int ObjectPartCount;
// 0x00801f40
extern LEGO_EXPORT struct TileSpriteEntry TileSpriteInfo[2048];
// 0x00805f48
extern unsigned int DAT_00805f48;
// 0x00805f60
extern LEGO_EXPORT void *TileSpriteArray[2048];
// 0x0080ff60
extern unsigned int DAT_0080ff60;
// 0x0080ff64
extern unsigned int DAT_0080ff64;
// 0x0080ff68
extern unsigned int DAT_0080ff68;
// 0x0080ff6c
extern void *DAT_0080ff6c;
extern unsigned int DAT_0080ff70;
// 0x0080ff74
extern LEGO_EXPORT unsigned int NEWFLC_AutoPlay;
// 0x0080ff78
extern LEGO_EXPORT unsigned char NEWFLC_PauseType;
// 0x0080ff80
extern unsigned int DAT_0080ff80;
// 0x0080ff84
extern unsigned int DAT_0080ff84;
// 0x0080ff88
extern unsigned int DAT_0080ff88;
// 0x0080ffc4
extern int DAT_0080ffc4;
// 0x0080ffcc
extern int DAT_0080ffcc;
// 0x0080ffe5
extern unsigned char DAT_0080ffe5;
// 0x0080ffa0
extern struct ScreenState DAT_0080ffa0;
// 0x008100c0
extern char DAT_008100c0[0x80];
// 0x00810140
extern unsigned int DAT_00810140;
// 0x00810148
extern struct Sprite *SPRITE_TitleScreenBk;
// 0x0081014c
extern LEGO_EXPORT unsigned char NEWFLC_ID[20];
// 0x00810160
extern LEGO_EXPORT struct Cursor QueryCursor;
// 0x008119a0
extern LEGO_EXPORT unsigned int NEWFLC_CheckDuplicate;
// 0x008119a4
extern unsigned int DAT_008119a4;
// 0x008119a8
extern LEGO_EXPORT unsigned int NEWFLC_BuffSize;
// 0x008119ac
extern LEGO_EXPORT unsigned short NEWFLC_Repeat;
// 0x008119b0
extern LEGO_EXPORT unsigned int EditMode;
// 0x008119b4
extern unsigned int DAT_008119b4;
// 0x008119b8
extern void *DAT_008119b8;
// 0x008119bc
extern unsigned int DAT_008119bc;
// 0x008139e0
extern struct Sprite *DAT_008139e0;
// 0x008139e4
extern struct Sprite *DAT_008139e4;
// 0x008139e8
extern struct Sprite *DAT_008139e8;
// 0x008139ec
extern struct Sprite *DAT_008139ec;
// 0x008139f0
extern struct Sprite *DAT_008139f0;
// 0x008139f4
extern struct Sprite *DAT_008139f4;
// 0x008139f8
extern struct Sprite *DAT_008139f8;
// 0x008139fc
extern struct Sprite *DAT_008139fc;
// 0x00813a00
extern struct Sprite *DAT_00813a00;
// 0x00813a04
extern struct Sprite *DAT_00813a04;
// 0x00813a08
extern struct Sprite *DAT_00813a08;
// 0x00813a0c
extern void *DAT_00813a0c;
// 0x00813a10
extern unsigned int DAT_00813a10;
// 0x00813a18
extern int DAT_00813a18;
// 0x00813a2c
extern int DAT_00813a2c;
// 0x00813a34
extern unsigned short DAT_00813a34;
// 0x00813a38
extern unsigned int DAT_00813a38;
// 0x00813a3c
extern unsigned int DAT_00813a3c;
// 0x00813a40
extern LEGO_EXPORT unsigned int GamePad;
// 0x00813a44
extern unsigned int DAT_00813a44;
// 0x00813a48
extern unsigned int DAT_00813a48;
// 0x00813a4c
extern unsigned int DAT_00813a4c;
// 0x00813a50
extern unsigned int DAT_00813a50;
// 0x00813a54
extern unsigned int DAT_00813a54;
// 0x00813a5c
extern unsigned int DAT_00813a5c;
// 0x00813a60
extern unsigned int DAT_00813a60;
// 0x00813a64
extern unsigned int DAT_00813a64;
// 0x00813a68
extern unsigned int DAT_00813a68;
// 0x00813a6c
extern unsigned int DAT_00813a6c;
// 0x00813a70
extern unsigned int DAT_00813a70;
// 0x00813a74
extern unsigned int DAT_00813a74;
// 0x00813a78
extern unsigned int DAT_00813a78;
// 0x00813a7c
extern unsigned int DAT_00813a7c;
// 0x00813a80
extern unsigned int DAT_00813a80;
// 0x00813a84
extern unsigned int DAT_00813a84;
// 0x00813a88
extern unsigned int DAT_00813a88;
// 0x00813a8c
extern unsigned int DAT_00813a8c;
// 0x00813a90
extern unsigned int DAT_00813a90;
// 0x00813a94
extern unsigned int DAT_00813a94;
// 0x00813a98
extern unsigned int DAT_00813a98;
// 0x00813a9c
extern unsigned int DAT_00813a9c;
// 0x00813aa0
extern unsigned int DAT_00813aa0;
// 0x00813aa4
extern unsigned int DAT_00813aa4;
// 0x00813aa8
extern unsigned int DAT_00813aa8;
// 0x00813aac
extern unsigned int DAT_00813aac;
// 0x00813ab0
extern unsigned int DAT_00813ab0;
// 0x00813ab4
extern unsigned int DAT_00813ab4;
// 0x00813ab8
extern unsigned int DAT_00813ab8;
// 0x00813abc
extern unsigned int DAT_00813abc;
// 0x00813ac0
extern unsigned int DAT_00813ac0;
// 0x00813ac4
extern unsigned int DAT_00813ac4;
// 0x00813ac8
extern unsigned int DAT_00813ac8;
// 0x00813acc
extern unsigned int DAT_00813acc;
// 0x00813ad0
extern unsigned int DAT_00813ad0;
// 0x00813ad4
extern unsigned int DAT_00813ad4;
// 0x00813ad8
extern unsigned int DAT_00813ad8;
// 0x00813adc
extern unsigned int DAT_00813adc;
// 0x00813ae0
extern unsigned int DAT_00813ae0;
// 0x00813af0
extern int DAT_00813af0;
// 0x00813af4
extern int DAT_00813af4;
// 0x00813af8
extern int DAT_00813af8;
// 0x00813afc
extern int DAT_00813afc;
// 0x00813b00
extern LEGO_EXPORT void *CONTROLLERBUFFER;
// 0x00813b04
extern char DAT_00813b04[28];
// 0x00813b20
extern unsigned char DAT_00813b20[0x300];
// 0x00813e20
extern unsigned short DAT_00813e20[256];
// 0x00814020
extern unsigned char DAT_00814020[0x8000];
// 0x0081c028
extern struct Sprite *DAT_0081c028;
// 0x0081c02c
extern struct Sprite *DAT_0081c02c;
// 0x0081c030
extern struct Sprite *DAT_0081c030;
// 0x0081c034
extern struct Sprite *DAT_0081c034;
// 0x0081c038
extern unsigned int DAT_0081c038;
// 0x0081c040
extern struct Sprite *DAT_0081c040[1];
// 0x0081c054
extern struct Sprite *DAT_0081c054[1];
// 0x0081c068
extern struct Sprite *DAT_0081c068[1];
// 0x0081c080
extern struct Sprite *DAT_0081c080;
// 0x0081c084
extern struct Sprite *DAT_0081c084;
// 0x0081c088
extern unsigned int DAT_0081c088;
// 0x0081c08c
extern void *DAT_0081c08c;
// 0x0081c090
extern void *DAT_0081c090;
// 0x0081c094
extern void *DAT_0081c094;
// 0x0081c098
extern void *DAT_0081c098;
// 0x0081c09c
extern unsigned int DAT_0081c09c;
// 0x0081c0a0
extern void *DAT_0081c0a0;
// 0x0081c0a4
extern void *DAT_0081c0a4;
// 0x0081c0c0
extern int DAT_0081c0c0[0x200];
// 0x0081c8c0
extern void *DAT_0081c8c0;
// 0x0081c8c4
extern void *DAT_0081c8c4;
// 0x0081c8c8
extern void *DAT_0081c8c8;
// 0x0081c8cc
extern void *DAT_0081c8cc;
// 0x0081c8d0
extern unsigned int DAT_0081c8d0;
// 0x0081c8d4
extern unsigned int DAT_0081c8d4;
// 0x0081c8d8
extern unsigned int DAT_0081c8d8;
// 0x0081c8dc
extern unsigned int DAT_0081c8dc;
// 0x0081cae0
extern struct Sprite *DAT_0081cae0;
// 0x0081caf0
extern void *DAT_0081caf0;
// 0x0081caf4
extern void *DAT_0081caf4;
// 0x0081cb00
extern struct Sprite *DAT_0081cb00;
// 0x0081cb04
extern struct Sprite *DAT_0081cb04;
// 0x0081cb08
extern struct Sprite *DAT_0081cb08;
// 0x0081cb0c
extern struct Sprite *DAT_0081cb0c;
// 0x0081cb14
extern struct Building *DAT_0081cb14;
// 0x0081cb18
extern struct Sprite *DAT_0081cb18;
// 0x0081cb1c
extern struct Building *DAT_0081cb1c;
// 0x0081cb20
extern struct Sprite *DAT_0081cb20;
// 0x0081cb24
extern struct Sprite *DAT_0081cb24;
// 0x0081cb28
extern struct Sprite *DAT_0081cb28;
// 0x0081cb2c
extern struct Building *DAT_0081cb2c;
// 0x0081cb30
extern struct Building *DAT_0081cb30;
// 0x0081cb34
extern struct Sprite *DAT_0081cb34;
// 0x0081cb38
extern struct Sprite *DAT_0081cb38;
// 0x0081cb3c
extern struct Building *DAT_0081cb3c;
// 0x0081cb40
extern struct Building *DAT_0081cb40;
// 0x0081cb44
extern struct Building *DAT_0081cb44;
// 0x0081cb48
extern struct Sprite *DAT_0081cb48;
// 0x0081cb4c
extern struct Building *DAT_0081cb4c;
// 0x0081cb50
extern struct Sprite *DAT_0081cb50;
// 0x0081cb54
extern struct Cursor *DAT_0081cb54;
// 0x0081cb58
extern struct JungleTileMap *DAT_0081cb58;
// 0x0081cb5c
extern struct Sprite *DAT_0081cb5c;
// 0x0081cb60
extern struct Cursor *DAT_0081cb60;
// 0x0081cb64
extern struct Cursor *DAT_0081cb64;
// 0x0081cb68
extern struct Sprite *DAT_0081cb68;
// 0x0081cb6c
extern struct Sprite *DAT_0081cb6c;
// 0x0081cb70
extern struct Cursor *DAT_0081cb70;
// 0x0081cb74
extern struct Cursor *DAT_0081cb74;
// 0x0081cb80
extern int DAT_0081cb80[0x60];
// 0x0081cd00
extern void *DAT_0081cd00;
// 0x0081cd04
extern void *DAT_0081cd04;
// 0x0081cd08
extern unsigned int DAT_0081cd08;
// 0x0081cd0c
extern unsigned int DAT_0081cd0c;
// 0x0081cd10
extern unsigned int DAT_0081cd10;
// 0x0081cd14
extern unsigned int DAT_0081cd14;
// 0x0081cd18
extern struct EateryFX *DAT_0081cd18;
// 0x0081cd1c
extern struct EateryFX *DAT_0081cd1c;
// 0x0081cd20
extern struct Sprite *DAT_0081cd20;
// 0x0081cd28
extern struct Sprite *DAT_0081cd28;
// 0x0081cd34
extern struct Sprite *DAT_0081cd34;
// 0x0081cd38
extern unsigned int DAT_0081cd38;
// 0x0081cd3c
extern unsigned int DAT_0081cd3c;
// 0x0081cd44
extern struct EateryFX *DAT_0081cd44;
// 0x0081cd48
extern struct Sprite *DAT_0081cd48;
// 0x0081cd60
extern struct Sprite *DAT_0081cd60;
// 0x0081cd64
extern struct Sprite *DAT_0081cd64;
// 0x0081cd68
extern struct Sprite *DAT_0081cd68;
// 0x0081cd6c
extern struct Sprite *DAT_0081cd6c;
// 0x0081cd70
extern struct Sprite *DAT_0081cd70;
// 0x0081cd74
extern struct Sprite *DAT_0081cd74;
// 0x0081cd78
extern struct Sprite *DAT_0081cd78;
// 0x0081cd7c
extern struct Sprite *DAT_0081cd7c;
// 0x0081cd80
extern struct Sprite *DAT_0081cd80;
// 0x0081cd84
extern struct Sprite *DAT_0081cd84;
// 0x0081cd88
extern struct Sprite *DAT_0081cd88;
// 0x0081cd8c
extern struct Sprite *DAT_0081cd8c;
// 0x0081cd90
extern struct Sprite *DAT_0081cd90;
// 0x0081cd94
extern struct Sprite *DAT_0081cd94;
// 0x0081cda0
extern struct Sprite *DAT_0081cda0;
// 0x0081cda4
extern struct Sprite *DAT_0081cda4;
// 0x0081cda8
extern struct Sprite *DAT_0081cda8;
// 0x0081cdac
extern struct Sprite *DAT_0081cdac;
// 0x0081cdb0
extern struct Sprite *DAT_0081cdb0;
// 0x0081cdb4
extern struct Sprite *DAT_0081cdb4;
// 0x0081cdb8
extern struct Sprite *DAT_0081cdb8;
// 0x0081cdbc
extern struct Sprite *DAT_0081cdbc;
// 0x0081cdc0
extern struct Sprite *DAT_0081cdc0;
// 0x0081cdc4
extern struct Sprite *DAT_0081cdc4;
// 0x0081cdc8
extern struct Sprite *DAT_0081cdc8;
// 0x0081cdcc
extern struct Sprite *DAT_0081cdcc;
// 0x0081cdd0
extern struct Sprite *DAT_0081cdd0;
// 0x0081cdd4
extern struct Sprite *DAT_0081cdd4;
// 0x0081cdd8
extern struct Sprite *DAT_0081cdd8;
// 0x0081cddc
extern struct Sprite *DAT_0081cddc;
// 0x0081cde0
extern struct EateryFX *DAT_0081cde0;
// 0x0081cde4
extern void *DAT_0081cde4;
// 0x0081cde8
extern struct Sprite *DAT_0081cde8;
// 0x0081cdec
extern void *DAT_0081cdec;
// 0x00828fe0
extern unsigned int DAT_00828fe0[2];
// 0x00829980
extern void *DAT_00829980;
// 0x0082999c
extern float DAT_0082999c;
// 0x008299d4
extern float DAT_008299d4;
// 0x008299fc
extern unsigned int DAT_008299fc;
// 0x00829a3c
extern struct ListLink DAT_00829a3c;
// 0x00829a60
extern float DAT_00829a60;
// 0x00829a80
extern struct EditFootPrint DAT_00829a80;
// 0x00829ae0
extern unsigned int DAT_00829ae0;
// 0x00829af8
extern unsigned int DAT_00829af8[3];
// 0x00829b04
extern unsigned int DAT_00829b04[3];
// 0x00829b88
extern unsigned int DAT_00829b88;
// 0x00829b8c
extern unsigned int DAT_00829b8c;
// 0x00829ba0
extern unsigned int DAT_00829ba0;
// 0x00829ba4
extern unsigned int DAT_00829ba4;
// 0x00829bec
extern void *DAT_00829bec;
// 0x00829bf0
extern void *DAT_00829bf0;
// 0x00829bf4
extern void *DAT_00829bf4;
// 0x00829bf8
extern unsigned int DAT_00829bf8;
// 0x00829c00
extern int DAT_00829c00;
// 0x00829c04
extern struct Sprite *DAT_00829c04;
// 0x00829c34
extern int DAT_00829c34;
// 0x0082ac60
extern void *DAT_0082ac60[1];
// 0x0082ad20
extern unsigned char DAT_0082ad20[0x90];
// 0x0082adb0
extern unsigned int DAT_0082adb0;
// 0x0082add0
extern unsigned int DAT_0082add0;
// 0x0082add4
extern unsigned int DAT_0082add4;
// 0x0082add8
extern unsigned int DAT_0082add8;
// 0x0082ade0
extern unsigned int DAT_0082ade0;
// 0x0082ade4
extern unsigned int DAT_0082ade4;
// 0x0082ade8
extern unsigned int DAT_0082ade8;
// 0x0082adec
extern unsigned int DAT_0082adec;
// 0x0082adf0
extern void *DAT_0082adf0;
// 0x0082adf8
extern unsigned int DAT_0082adf8;
// 0x0082ae20
extern struct Cursor DAT_0082ae20;
// 0x0082c658
extern unsigned int DAT_0082c658;
// 0x0082c668
extern struct Sprite *DAT_0082c668;
// 0x0082c678
extern void *DAT_0082c678;
// 0x0082c680
extern void *DAT_0082c680;
// 0x0082c684
extern void *DAT_0082c684;
// 0x0082c688
extern void *DAT_0082c688;
// 0x0082c690
extern unsigned int DAT_0082c690;
// 0x0082c694
extern unsigned int DAT_0082c694;
// 0x0082c6a0
extern unsigned int DAT_0082c6a0;
// 0x0082c6a4
extern unsigned int DAT_0082c6a4;
// 0x0082c6a8
extern unsigned int DAT_0082c6a8;
// 0x0082c6ac
extern unsigned short DAT_0082c6ac;
// 0x0082c6b8
extern unsigned int DAT_0082c6b8;
// 0x0082c6bc
extern unsigned int DAT_0082c6bc;
// 0x0082c6c0
extern struct Sprite *DAT_0082c6c0;
// 0x0082f760
extern struct Cursor DAT_0082f760;
// 0x00830b74
extern unsigned int DAT_00830b74;
// 0x00830f88
extern unsigned int DAT_00830f88;
// 0x00830f94
extern struct Sprite *DAT_00830f94;
// 0x00830f98
extern struct Position *DAT_00830f98;
// 0x00830fc0
extern LEGO_EXPORT struct Cursor PathCursor;
// 0x00832800
extern LEGO_EXPORT unsigned int MapStats;
// 0x00832824
extern unsigned int DAT_00832824[1];
// 0x00832828
extern unsigned int DAT_00832828[1];
// 0x0083291c
extern unsigned int DAT_0083291c;
// 0x00832920
extern unsigned int DAT_00832920;
// 0x00832924
extern unsigned int DAT_00832924;
// 0x00832934
extern int DAT_00832934;
// 0x00832938
extern int DAT_00832938;
// 0x0083293c
extern int DAT_0083293c[13];
// 0x00832970
extern int DAT_00832970;
// 0x00832974
extern unsigned int DAT_00832974;
// 0x00832978
extern unsigned int DAT_00832978;
// 0x0083297c
extern unsigned int DAT_0083297c;
// 0x00832980
extern unsigned int DAT_00832980;
// 0x00832984
extern unsigned int DAT_00832984;
// 0x0083298c
extern unsigned int DAT_0083298c;
// 0x00832990
extern unsigned int DAT_00832990;
// 0x00832994
extern unsigned int DAT_00832994;
// 0x00832998
extern char DAT_00832998[256];
// 0x00832a98
extern char DAT_00832a98[256];
// 0x00832b9c
extern unsigned int DAT_00832b9c;
// 0x00832ba0
extern unsigned int DAT_00832ba0;
extern unsigned int DAT_00832bac;
// 0x00832ba4
extern unsigned int DAT_00832ba4;
// 0x00832ba8
extern unsigned int DAT_00832ba8;
// 0x00832bd0
extern int DAT_00832bd0;
// 0x00832bd4
extern int DAT_00832bd4;
// 0x00832bd8
extern unsigned int DAT_00832bd8;
// 0x00832bdc
extern int DAT_00832bdc;
// 0x00832bf0
extern LEGO_EXPORT void *PathSprite;
