#pragma once

#include <windows.h>

#include "gamemap.h"

// ---------------------------------------------------------------------------
// Forward declarations for typed/instance globals declared below. Full struct
// definitions live in the owning translation units; these globals only need an
// incomplete type for their pointer/extern declarations. (struct Cursor's full
// layout comes from gamemap.h, used by the EditCursor / QueryCursor instances.)
// ---------------------------------------------------------------------------
struct Sprite;
struct CursorSource;
struct Building;
struct JailCell;
struct LegoConfig;
struct RideQueueEntry;
struct ObjectiveEvent;
struct MapElement;
struct WaterContext;
struct WaterSub;
struct EateryFX;
struct BlokeNode;
struct SaveBlock;
struct JungleNode;
struct JungleCursor;
struct JungleFish;
struct JungleScore;
struct CatapultNode;
struct Position;
struct RinData;
struct CopterNode;
struct GoldNode;
struct CarouselNode;
struct JoustNode;
struct InterfaceIconNode;
struct InterfaceProfileObj;
struct InterfaceListNode;
struct InterfaceQueryNode;
struct InterfaceEventNode;
struct InterfaceResearchNode;
struct InfoIcon;

// Full definitions needed by typed globals defined in globals.c.
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

// ---------------------------------------------------------------------------
// All game globals, sorted by ascending address. Definitions (with any
// initializers) live in globals.c. This phase only collects the declarations;
// per-TU inline externs are migrated to use this header in a later phase.
// ---------------------------------------------------------------------------
// GLOBAL: LEGOLAND 0x004ab390
extern float FLOAT_004ab390;
// GLOBAL: LEGOLAND 0x004ab3d0
extern float DAT_004ab3d0;
// GLOBAL: LEGOLAND 0x004ab430
extern float DAT_004ab430;
// GLOBAL: LEGOLAND 0x004ab454
extern float FLOAT_004ab454;
// GLOBAL: LEGOLAND 0x004ab458
extern float FLOAT_004ab458;
// GLOBAL: LEGOLAND 0x004ab45c
extern float FLOAT_004ab45c;
// GLOBAL: LEGOLAND 0x004ab480
extern float FLOAT_004ab480;
// GLOBAL: LEGOLAND 0x004ab6d0
extern GUID GUID_PerfMasterGrooveLevel;
// GLOBAL: LEGOLAND 0x004ab8b0
extern GUID DAT_004ab8b0;
// GLOBAL: LEGOLAND 0x004b40c8
extern unsigned char Catapult_SFX[0x70];
// GLOBAL: LEGOLAND 0x004b4140
extern unsigned char Helicopter_SFX[0x70];
// GLOBAL: LEGOLAND 0x004b43f8
extern unsigned char DRIVING_SCHOOL_SFX[0xa8];
// GLOBAL: LEGOLAND 0x004b4688
extern unsigned char JOUST_SFX[12];
// GLOBAL: LEGOLAND 0x004b4728
extern unsigned int DAT_004b4728;
// GLOBAL: LEGOLAND 0x004b472c
extern unsigned int DAT_004b472c;
// GLOBAL: LEGOLAND 0x004b4730
extern unsigned int DAT_004b4730;
// GLOBAL: LEGOLAND 0x004b4734
extern unsigned int DAT_004b4734;
// GLOBAL: LEGOLAND 0x004b47f8
extern unsigned int DAT_004b47f8;
// GLOBAL: LEGOLAND 0x004b4804
extern unsigned int DAT_004b4804;
// GLOBAL: LEGOLAND 0x004b4838
extern unsigned int DAT_004b4838;
// GLOBAL: LEGOLAND 0x004b4850
extern unsigned int DAT_004b4850;
// GLOBAL: LEGOLAND 0x004b4bd0
extern unsigned char DAT_004b4bd0[0x14];
// GLOBAL: LEGOLAND 0x004b4bf0
extern unsigned int DAT_004b4bf0[5];
// GLOBAL: LEGOLAND 0x004b4cb8
extern unsigned char SAFARI_SFX[8];
// GLOBAL: LEGOLAND 0x004b4d88
extern unsigned char SpiderRide_SFX[8];
// GLOBAL: LEGOLAND 0x004b4d90
extern void *DAT_004b4d90;
// GLOBAL: LEGOLAND 0x004b4fa8
extern unsigned char WATERWORKS_SFX[28];
// GLOBAL: LEGOLAND 0x004b53c0
extern unsigned int DAT_004b53c0[5];
// GLOBAL: LEGOLAND 0x004b55f4
extern unsigned int DAT_004b55f4;
// GLOBAL: LEGOLAND 0x004b5608
extern unsigned int DAT_004b5608;
// GLOBAL: LEGOLAND 0x004b560c
extern unsigned int DAT_004b560c;
// GLOBAL: LEGOLAND 0x004b5b20
extern void *DAT_004b5b20;
// GLOBAL: LEGOLAND 0x004b5b28
extern int DAT_004b5b28;
// GLOBAL: LEGOLAND 0x004b5b4c
extern int DAT_004b5b4c;
// GLOBAL: LEGOLAND 0x004b5b50
extern int DAT_004b5b50;
// GLOBAL: LEGOLAND 0x004b5c9c
extern unsigned int DAT_004b5c9c;
// GLOBAL: LEGOLAND 0x004b5ca0
extern unsigned int DAT_004b5ca0;
// GLOBAL: LEGOLAND 0x004b5ca4
extern unsigned int DAT_004b5ca4;
// GLOBAL: LEGOLAND 0x004b5ca8
extern unsigned int DAT_004b5ca8;
// GLOBAL: LEGOLAND 0x004b5cbc
extern float DAT_004b5cbc;
// GLOBAL: LEGOLAND 0x004b5cc0
extern unsigned int DAT_004b5cc0;
// GLOBAL: LEGOLAND 0x004b5cc4
extern unsigned int DAT_004b5cc4;
// GLOBAL: LEGOLAND 0x004b5cc8
extern unsigned int DAT_004b5cc8;
// GLOBAL: LEGOLAND 0x004b5dc8
extern const unsigned char DAT_004b5dc8[1];
// GLOBAL: LEGOLAND 0x004b5f60
extern unsigned char DAT_004b5f60[1];
// GLOBAL: LEGOLAND 0x004b62f0
extern unsigned int DAT_004b62f0;
// GLOBAL: LEGOLAND 0x004b6300
extern unsigned int DAT_004b6300;
// GLOBAL: LEGOLAND 0x004b64d8
extern unsigned char CAROUSSEL_SFX[12];
// GLOBAL: LEGOLAND 0x004b6668
extern unsigned char ENTRANCE_SFX[0x1c];
// GLOBAL: LEGOLAND 0x004b66e8
extern unsigned char RESTAURANT_SFX[0x1c];
// GLOBAL: LEGOLAND 0x004b6968
extern unsigned char OCTOPUS_SFX[0x1c];
// GLOBAL: LEGOLAND 0x004b7260
extern unsigned char DAT_004b7260[0x18];
// GLOBAL: LEGOLAND 0x004b7278
extern unsigned char DAT_004b7278[0x10];
// GLOBAL: LEGOLAND 0x004b7288
extern void *DAT_004b7288;
// GLOBAL: LEGOLAND 0x004b7618
extern const unsigned char SPACE_TOWER_SFX[15];
// GLOBAL: LEGOLAND 0x004b7620
extern void *DAT_004b7620;
// GLOBAL: LEGOLAND 0x004b79d0
extern unsigned char DAT_004b79d0[0x18];
// GLOBAL: LEGOLAND 0x004b7d74
extern unsigned int DAT_004b7d74;
// GLOBAL: LEGOLAND 0x004b7d78
extern unsigned int DAT_004b7d78;
// GLOBAL: LEGOLAND 0x004b7d7e
extern unsigned short DAT_004b7d7e;
// GLOBAL: LEGOLAND 0x004b7d84
extern unsigned int DAT_004b7d84;
// GLOBAL: LEGOLAND 0x004b7e9c
extern unsigned int DAT_004b7e9c;
// GLOBAL: LEGOLAND 0x004b8334
extern int DAT_004b8334;
// GLOBAL: LEGOLAND 0x004b8340
extern int DAT_004b8340;
// GLOBAL: LEGOLAND 0x004b8344
extern int DAT_004b8344;
// GLOBAL: LEGOLAND 0x004b8368
extern void (*PTR_Bloke_DoNothing_004b8368[16])(void *);
// GLOBAL: LEGOLAND 0x004b8710
extern unsigned char DAT_004b8710[0x40];
// GLOBAL: LEGOLAND 0x004b8750
extern unsigned char DAT_004b8750[0x18];
// GLOBAL: LEGOLAND 0x004b8768
extern unsigned char DINO_SFX[0x18];
// GLOBAL: LEGOLAND 0x004b87a8
extern unsigned char MONEY_SFX[12];
// GLOBAL: LEGOLAND 0x004b8bbc
extern unsigned char DAT_004b8bbc[1];
// GLOBAL: LEGOLAND 0x004b90f8
extern unsigned int DAT_004b90f8;
// GLOBAL: LEGOLAND 0x004b90fc
extern unsigned int DAT_004b90fc;
// GLOBAL: LEGOLAND 0x004b9228
extern const unsigned char DAT_004b9228[1];
// GLOBAL: LEGOLAND 0x004b929c
extern void *DAT_004b929c;
// GLOBAL: LEGOLAND 0x004b92c0
extern void *PTR_004b92c0;
// GLOBAL: LEGOLAND 0x004b9550
extern unsigned char DAT_004b9550[8];
// GLOBAL: LEGOLAND 0x004b9ca8
extern unsigned int DAT_004b9ca8;
// GLOBAL: LEGOLAND 0x004b9e5c
extern unsigned int DAT_004b9e5c[1];
// GLOBAL: LEGOLAND 0x004b9f78
extern unsigned int DAT_004b9f78[4];
// GLOBAL: LEGOLAND 0x004b9f88
extern unsigned int DAT_004b9f88;
// GLOBAL: LEGOLAND 0x004b9f8c
extern unsigned int DAT_004b9f8c;
// GLOBAL: LEGOLAND 0x004ba87c
extern void *PTR_some_list_head_004ba87c;
// GLOBAL: LEGOLAND 0x004ba9ac
extern unsigned int DAT_004ba9ac[1];
// GLOBAL: LEGOLAND 0x004bad54
extern int mouse_granularity;
// GLOBAL: LEGOLAND 0x004bafa8
extern unsigned int DAT_004bafa8[20];
// GLOBAL: LEGOLAND 0x004baff8
extern unsigned int DAT_004baff8;
// GLOBAL: LEGOLAND 0x004bb094
extern unsigned int DAT_004bb094;
// GLOBAL: LEGOLAND 0x004bb098
extern unsigned int DAT_004bb098;
// GLOBAL: LEGOLAND 0x004bb09c
extern unsigned int DAT_004bb09c;
// GLOBAL: LEGOLAND 0x004bb0a0
extern unsigned int DAT_004bb0a0;
// GLOBAL: LEGOLAND 0x004bb18c
extern unsigned int DAT_004bb18c[4];
// GLOBAL: LEGOLAND 0x004bb5ac
extern unsigned int DAT_004bb5ac;
// GLOBAL: LEGOLAND 0x004bb5b0
extern unsigned int DAT_004bb5b0;
// GLOBAL: LEGOLAND 0x004bb688
extern unsigned int DAT_004bb688;
// GLOBAL: LEGOLAND 0x004bb6bc
extern unsigned int DAT_004bb6bc;
// GLOBAL: LEGOLAND 0x004bb6f8
extern unsigned int DAT_004bb6f8;
// GLOBAL: LEGOLAND 0x004bcbf4
extern struct LegoConfig *lpConfig;
// GLOBAL: LEGOLAND 0x004bdd00
extern unsigned int DAT_004bdd00;
// GLOBAL: LEGOLAND 0x004bdd04
extern unsigned int DAT_004bdd04;
// GLOBAL: LEGOLAND 0x004bdea0
extern struct ClipRect SPRITE_ClipRect;
// GLOBAL: LEGOLAND 0x004beb94
extern struct FreePlaySpriteSlot DAT_004beb94[10];
// GLOBAL: LEGOLAND 0x004beba0
extern unsigned char DAT_004beba0;
// GLOBAL: LEGOLAND 0x004becac
extern unsigned int DAT_004becac;
// GLOBAL: LEGOLAND 0x004becb0
extern int DAT_004becb0[1];
// GLOBAL: LEGOLAND 0x004bef9c
extern unsigned int DAT_004bef9c;
// GLOBAL: LEGOLAND 0x004bf670
extern unsigned int DAT_004bf670;
// GLOBAL: LEGOLAND 0x004bf774
extern unsigned int DAT_004bf774;
// GLOBAL: LEGOLAND 0x004bf778
extern unsigned int DAT_004bf778;
// GLOBAL: LEGOLAND 0x004c10d4
extern void *DAT_004c10d4;
// GLOBAL: LEGOLAND 0x004c10dc
extern void *DAT_004c10dc;
// GLOBAL: LEGOLAND 0x004c10e4
extern unsigned int DAT_004c10e4;
// GLOBAL: LEGOLAND 0x004c10f0
extern void *DAT_004c10f0;
// GLOBAL: LEGOLAND 0x004c10f4
extern void *DAT_004c10f4;
// GLOBAL: LEGOLAND 0x004c1100
extern void *DAT_004c1100;
// GLOBAL: LEGOLAND 0x004c1104
extern unsigned int DAT_004c1104;
// GLOBAL: LEGOLAND 0x004c1108
extern unsigned int DAT_004c1108;
// GLOBAL: LEGOLAND 0x004c110c
extern unsigned short DAT_004c110c;
// GLOBAL: LEGOLAND 0x004c1118
extern struct CatapultNode *DAT_004c1118;
// GLOBAL: LEGOLAND 0x004c1120
extern struct Sprite *DAT_004c1120;
// GLOBAL: LEGOLAND 0x004c1124
extern int DAT_004c1124[6];
// GLOBAL: LEGOLAND 0x004c113c
extern struct Sprite *DAT_004c113c[10];
// GLOBAL: LEGOLAND 0x004c1164
extern struct Sprite *DAT_004c1164;
// GLOBAL: LEGOLAND 0x004c1198
extern void *DAT_004c1198;
// GLOBAL: LEGOLAND 0x004c11b4
extern struct CopterNode *DAT_004c11b4;
// GLOBAL: LEGOLAND 0x004c11bc
extern void *DAT_004c11bc;
// GLOBAL: LEGOLAND 0x004c11c4
extern unsigned int DAT_004c11c4;
// GLOBAL: LEGOLAND 0x004c11c8
extern unsigned int DAT_004c11c8;
// GLOBAL: LEGOLAND 0x004c11cc
extern unsigned int DAT_004c11cc;
// GLOBAL: LEGOLAND 0x004c11dc
extern unsigned int DAT_004c11dc;
// GLOBAL: LEGOLAND 0x004c11e4
extern struct Sprite *DAT_004c11e4;
// GLOBAL: LEGOLAND 0x004c11f0
extern void *DAT_004c11f0;
// GLOBAL: LEGOLAND 0x004c11f4
extern struct Sprite *DAT_004c11f4;
// GLOBAL: LEGOLAND 0x004c11f8
extern struct Sprite *DAT_004c11f8;
// GLOBAL: LEGOLAND 0x004c11fc
extern struct Sprite *DAT_004c11fc;
// GLOBAL: LEGOLAND 0x004c1200
extern struct Sprite *DAT_004c1200;
// GLOBAL: LEGOLAND 0x004c1204
extern struct GoldNode *DAT_004c1204;
// GLOBAL: LEGOLAND 0x004c1218
extern void *DAT_004c1218;
// GLOBAL: LEGOLAND 0x004c121c
extern unsigned int DAT_004c121c;
// GLOBAL: LEGOLAND 0x004c1228
extern unsigned int DAT_004c1228;
// GLOBAL: LEGOLAND 0x004c122c
extern unsigned int DAT_004c122c;
// GLOBAL: LEGOLAND 0x004c1230
extern unsigned int DAT_004c1230;
// GLOBAL: LEGOLAND 0x004c1234
extern unsigned short DAT_004c1234;
// GLOBAL: LEGOLAND 0x004c1240
extern unsigned int DAT_004c1240;
// GLOBAL: LEGOLAND 0x004c1244
extern unsigned int DAT_004c1244;
// GLOBAL: LEGOLAND 0x004c1248
extern unsigned int DAT_004c1248;
// GLOBAL: LEGOLAND 0x004c124c
extern unsigned int DAT_004c124c;
// GLOBAL: LEGOLAND 0x004c1250
extern struct JoustNode *DAT_004c1250;
// GLOBAL: LEGOLAND 0x004c1258
extern struct Sprite *DAT_004c1258;
// GLOBAL: LEGOLAND 0x004c2a88
extern unsigned int DAT_004c2a88;
// GLOBAL: LEGOLAND 0x004c2a94
extern unsigned int DAT_004c2a94;
// GLOBAL: LEGOLAND 0x004c2a98
extern unsigned int DAT_004c2a98;
// GLOBAL: LEGOLAND 0x004c2aa0
extern struct CursorSource *DAT_004c2aa0;
// GLOBAL: LEGOLAND 0x004c2abc
extern struct Sprite *DAT_004c2abc;
// GLOBAL: LEGOLAND 0x004c2ae4
extern struct Sprite *DAT_004c2ae4;
// GLOBAL: LEGOLAND 0x004c2ae8
extern void *DAT_004c2ae8;
// GLOBAL: LEGOLAND 0x004c2aec
extern unsigned int DAT_004c2aec;
// GLOBAL: LEGOLAND 0x004c2af0
extern unsigned int DAT_004c2af0;
// GLOBAL: LEGOLAND 0x004c2af4
extern unsigned int DAT_004c2af4;
// GLOBAL: LEGOLAND 0x004c2af8
extern void *DAT_004c2af8;
// GLOBAL: LEGOLAND 0x004c2afc
extern unsigned int DAT_004c2afc;
// GLOBAL: LEGOLAND 0x004c2b0c
extern struct CursorSource *DAT_004c2b0c;
// GLOBAL: LEGOLAND 0x004c2b10
extern unsigned int DAT_004c2b10;
// GLOBAL: LEGOLAND 0x004c2b50
extern void *DAT_004c2b50;
// GLOBAL: LEGOLAND 0x004c2b60
extern struct CursorSource *DAT_004c2b60;
// GLOBAL: LEGOLAND 0x004c2b64
extern unsigned int DAT_004c2b64;
// GLOBAL: LEGOLAND 0x004c2b6c
extern unsigned int DAT_004c2b6c;
// GLOBAL: LEGOLAND 0x004c2b70
extern unsigned int DAT_004c2b70;
// GLOBAL: LEGOLAND 0x004c2b98
extern unsigned int DAT_004c2b98;
// GLOBAL: LEGOLAND 0x004c2b9c
extern unsigned int DAT_004c2b9c;
// GLOBAL: LEGOLAND 0x004c2ba0
extern void *DAT_004c2ba0;
// GLOBAL: LEGOLAND 0x004c2bf0
extern struct CursorSource *DAT_004c2bf0;
// GLOBAL: LEGOLAND 0x004c445c
extern struct CursorSource *DAT_004c445c;
// GLOBAL: LEGOLAND 0x004c4460
extern void *DAT_004c4460;
// GLOBAL: LEGOLAND 0x004c5c90
extern unsigned int DAT_004c5c90;
// GLOBAL: LEGOLAND 0x004c74c8
extern unsigned int DAT_004c74c8;
// GLOBAL: LEGOLAND 0x004c74d4
extern struct CursorSource *DAT_004c74d4;
// GLOBAL: LEGOLAND 0x004c8d2c
extern struct Sprite *DAT_004c8d2c;
// GLOBAL: LEGOLAND 0x004c8d4c
extern void *DAT_004c8d4c;
// GLOBAL: LEGOLAND 0x004c8d68
extern struct Sprite *DAT_004c8d68;
// GLOBAL: LEGOLAND 0x004c8d6c
extern struct CursorSource *DAT_004c8d6c;
// GLOBAL: LEGOLAND 0x004c8d70
extern struct Sprite *DAT_004c8d70;
// GLOBAL: LEGOLAND 0x004cbdd8
extern unsigned int DAT_004cbdd8;
// GLOBAL: LEGOLAND 0x004cbe08
extern struct Sprite *DAT_004cbe08;
// GLOBAL: LEGOLAND 0x004cbe0c
extern struct Sprite *DAT_004cbe0c;
// GLOBAL: LEGOLAND 0x004cbe10
extern void *DAT_004cbe10;
// GLOBAL: LEGOLAND 0x004cbe14
extern unsigned int DAT_004cbe14;
// GLOBAL: LEGOLAND 0x004cbe18
extern struct CursorSource *DAT_004cbe18;
// GLOBAL: LEGOLAND 0x004cbe1c
extern struct Sprite *DAT_004cbe1c;
// GLOBAL: LEGOLAND 0x004cbe20
extern unsigned int DAT_004cbe20;
// GLOBAL: LEGOLAND 0x004cbe24
extern unsigned int DAT_004cbe24;
// GLOBAL: LEGOLAND 0x004cbe28
extern unsigned int DAT_004cbe28;
// GLOBAL: LEGOLAND 0x004cbe2c
extern unsigned int DAT_004cbe2c;
// GLOBAL: LEGOLAND 0x004cbe48
extern void *DAT_004cbe48;
// GLOBAL: LEGOLAND 0x004cbe4c
extern struct Sprite *DAT_004cbe4c;
// GLOBAL: LEGOLAND 0x004cbe74
extern struct Sprite *DAT_004cbe74;
// GLOBAL: LEGOLAND 0x004cbe78
extern struct Sprite *DAT_004cbe78;
// GLOBAL: LEGOLAND 0x004cbe7c
extern struct Sprite *DAT_004cbe7c;
// GLOBAL: LEGOLAND 0x004cbe80
extern struct Sprite *DAT_004cbe80;
// GLOBAL: LEGOLAND 0x004cbe84
extern void *DAT_004cbe84;
// GLOBAL: LEGOLAND 0x004cbe88
extern struct Sprite *DAT_004cbe88;
// GLOBAL: LEGOLAND 0x004cbe8c
extern struct Sprite *DAT_004cbe8c;
// GLOBAL: LEGOLAND 0x004cbe90
extern struct Sprite *DAT_004cbe90;
// GLOBAL: LEGOLAND 0x004cbe94
extern struct Sprite *DAT_004cbe94;
// GLOBAL: LEGOLAND 0x004cbe98
extern struct Sprite *DAT_004cbe98;
// GLOBAL: LEGOLAND 0x004cbe9c
extern unsigned int DAT_004cbe9c;
// GLOBAL: LEGOLAND 0x004cbea4
extern void *DAT_004cbea4;
// GLOBAL: LEGOLAND 0x004cbeac
extern struct RideQueueEntry *DAT_004cbeac;
// GLOBAL: LEGOLAND 0x004cbeb0
extern int DAT_004cbeb0;
// GLOBAL: LEGOLAND 0x004cbeb4
extern int DAT_004cbeb4;
// GLOBAL: LEGOLAND 0x004cbec4
extern unsigned int DAT_004cbec4;
// GLOBAL: LEGOLAND 0x004cbed0
extern unsigned int DAT_004cbed0;
// GLOBAL: LEGOLAND 0x004cbed4
extern unsigned int DAT_004cbed4;
// GLOBAL: LEGOLAND 0x004cbed8
extern unsigned int DAT_004cbed8;
// GLOBAL: LEGOLAND 0x004cbedc
extern unsigned short DAT_004cbedc;
// GLOBAL: LEGOLAND 0x004cbef8
extern void *DAT_004cbef8;
// GLOBAL: LEGOLAND 0x004cbefc
extern void *DAT_004cbefc;
// GLOBAL: LEGOLAND 0x004cbf00
extern void *DAT_004cbf00;
// GLOBAL: LEGOLAND 0x004cbf08
extern unsigned int DAT_004cbf08;
// GLOBAL: LEGOLAND 0x004cbf0c
extern void *DAT_004cbf0c;
// GLOBAL: LEGOLAND 0x004cbf10
extern void *DAT_004cbf10;
// GLOBAL: LEGOLAND 0x004cbf14
extern struct Sprite *DAT_004cbf14;
// GLOBAL: LEGOLAND 0x004cbf18
extern void *DAT_004cbf18;
// GLOBAL: LEGOLAND 0x004cbf1c
extern struct Sprite *DAT_004cbf1c;
// GLOBAL: LEGOLAND 0x004cbf20
extern unsigned int DAT_004cbf20;
// GLOBAL: LEGOLAND 0x004cbf24
extern void *DAT_004cbf24;
// GLOBAL: LEGOLAND 0x004cbf40
extern unsigned int DAT_004cbf40;
// GLOBAL: LEGOLAND 0x004cbf44
extern unsigned int DAT_004cbf44;
// GLOBAL: LEGOLAND 0x004cbf48
extern unsigned int DAT_004cbf48;
// GLOBAL: LEGOLAND 0x004cbf4c
extern unsigned short DAT_004cbf4c;
// GLOBAL: LEGOLAND 0x004cbf58
extern void *DAT_004cbf58;
// GLOBAL: LEGOLAND 0x004cbf5c
extern void *DAT_004cbf5c;
// GLOBAL: LEGOLAND 0x004cbf68
extern unsigned int DAT_004cbf68;
// GLOBAL: LEGOLAND 0x004cbf6c
extern unsigned int DAT_004cbf6c;
// GLOBAL: LEGOLAND 0x004cbf78
extern unsigned int DAT_004cbf78;
// GLOBAL: LEGOLAND 0x004cbf80
extern unsigned int DAT_004cbf80;
// GLOBAL: LEGOLAND 0x004cbf98
extern unsigned int DAT_004cbf98;
// GLOBAL: LEGOLAND 0x004cbf9c
extern unsigned int DAT_004cbf9c;
// GLOBAL: LEGOLAND 0x004cbfa0
extern unsigned int DAT_004cbfa0;
// GLOBAL: LEGOLAND 0x004cbfa4
extern unsigned short DAT_004cbfa4;
// GLOBAL: LEGOLAND 0x004cbfc4
extern void *DAT_004cbfc4;
// GLOBAL: LEGOLAND 0x004cbfd0
extern unsigned int DAT_004cbfd0;
// GLOBAL: LEGOLAND 0x004cbfd4
extern void *DAT_004cbfd4;
// GLOBAL: LEGOLAND 0x004cbfd8
extern unsigned int DAT_004cbfd8;
// GLOBAL: LEGOLAND 0x004cbfdc
extern struct WaterContext *DAT_004cbfdc;
// GLOBAL: LEGOLAND 0x004cbfe0
extern struct WaterSub *DAT_004cbfe0;
// GLOBAL: LEGOLAND 0x004cbfe4
extern unsigned int DAT_004cbfe4;
// GLOBAL: LEGOLAND 0x004cbfe8
extern unsigned int DAT_004cbfe8;
// GLOBAL: LEGOLAND 0x004cbfec
extern unsigned int DAT_004cbfec;
// GLOBAL: LEGOLAND 0x004cbff0
extern unsigned int DAT_004cbff0;
// GLOBAL: LEGOLAND 0x004cbff4
extern unsigned int DAT_004cbff4;
// GLOBAL: LEGOLAND 0x004cbff8
extern unsigned int DAT_004cbff8;
// GLOBAL: LEGOLAND 0x004cbffc
extern unsigned short DAT_004cbffc;
// GLOBAL: LEGOLAND 0x004cc000
extern unsigned int DAT_004cc000;
// GLOBAL: LEGOLAND 0x004cc014
extern unsigned int DAT_004cc014;
// GLOBAL: LEGOLAND 0x004cc01c
extern unsigned int DAT_004cc01c;
// GLOBAL: LEGOLAND 0x004cc020
extern unsigned int DAT_004cc020;
// GLOBAL: LEGOLAND 0x004cc024
extern struct WaterContext *DAT_004cc024;
// GLOBAL: LEGOLAND 0x004cc028
extern unsigned int DAT_004cc028;
// GLOBAL: LEGOLAND 0x004cc02c
extern void *DAT_004cc02c;
// GLOBAL: LEGOLAND 0x004cc030
extern void *DAT_004cc030;
// GLOBAL: LEGOLAND 0x004cc034
extern void *DAT_004cc034;
// GLOBAL: LEGOLAND 0x004cc03c
extern void *DAT_004cc03c;
// GLOBAL: LEGOLAND 0x004cc048
extern unsigned int DAT_004cc048;
// GLOBAL: LEGOLAND 0x004cc060
extern unsigned int DAT_004cc060;
// GLOBAL: LEGOLAND 0x004cc070
extern void *DAT_004cc070;
// GLOBAL: LEGOLAND 0x004cc074
extern void *DAT_004cc074;
// GLOBAL: LEGOLAND 0x004cc078
extern unsigned int DAT_004cc078;
// GLOBAL: LEGOLAND 0x004cc088
extern void *DAT_004cc088;
// GLOBAL: LEGOLAND 0x004d823c
extern void *DAT_004d823c;
// GLOBAL: LEGOLAND 0x004d8250
extern const unsigned char DAT_004d8250[1];
// GLOBAL: LEGOLAND 0x004d8268
extern unsigned int DAT_004d8268;
// GLOBAL: LEGOLAND 0x004d8270
extern unsigned int DAT_004d8270;
// GLOBAL: LEGOLAND 0x004d88f4
extern unsigned char DAT_004d88f4[0x80];
// GLOBAL: LEGOLAND 0x004d8934
extern unsigned char DAT_004d8934[1];
// GLOBAL: LEGOLAND 0x004d8974
extern unsigned char DAT_004d8974[0x40];
// GLOBAL: LEGOLAND 0x004d89c4
extern void *DAT_004d89c4;
// GLOBAL: LEGOLAND 0x004d89c8
extern unsigned int DAT_004d89c8[1];
// GLOBAL: LEGOLAND 0x004d8a40
extern unsigned int DAT_004d8a40[1];
// GLOBAL: LEGOLAND 0x004d8abc
extern unsigned int DAT_004d8abc[1];
// GLOBAL: LEGOLAND 0x004d8b34
extern unsigned int DAT_004d8b34[1];
// GLOBAL: LEGOLAND 0x004d8bac
extern unsigned int DAT_004d8bac;
// GLOBAL: LEGOLAND 0x004d8bb0
extern char DAT_004d8bb0[0x100];
// GLOBAL: LEGOLAND 0x004dcbd0
extern void *DAT_004dcbd0[10];
// GLOBAL: LEGOLAND 0x004dcbf8
extern unsigned int DAT_004dcbf8;
// GLOBAL: LEGOLAND 0x004dcc00
extern unsigned char DAT_004dcc00[1];
// GLOBAL: LEGOLAND 0x004dd5d8
extern unsigned int DAT_004dd5d8;
// GLOBAL: LEGOLAND 0x004dd5e0
extern void *DAT_004dd5e0[24];
// GLOBAL: LEGOLAND 0x004dd758
extern unsigned int DAT_004dd758;
// GLOBAL: LEGOLAND 0x004dd760
extern char DAT_004dd760[1];
// GLOBAL: LEGOLAND 0x004dd860
extern unsigned int DAT_004dd860;
// GLOBAL: LEGOLAND 0x004dd868
extern unsigned int DAT_004dd868;
// GLOBAL: LEGOLAND 0x004dd86c
extern unsigned int DAT_004dd86c;
// GLOBAL: LEGOLAND 0x0060f924
extern unsigned int DAT_0060f924[1];
// GLOBAL: LEGOLAND 0x0060f938
extern unsigned int DAT_0060f938;
// GLOBAL: LEGOLAND 0x006102f8
extern unsigned int DAT_006102f8;
// GLOBAL: LEGOLAND 0x006103a8
extern unsigned int DAT_006103a8;
// GLOBAL: LEGOLAND 0x00610a04
extern unsigned int DAT_00610a04;
// GLOBAL: LEGOLAND 0x00610a08
extern unsigned int DAT_00610a08;
// GLOBAL: LEGOLAND 0x00611648
extern unsigned int DAT_00611648;
// GLOBAL: LEGOLAND 0x00611650
extern float DAT_00611650;
// GLOBAL: LEGOLAND 0x00611688
extern unsigned int DAT_00611688;
// GLOBAL: LEGOLAND 0x0061168c
extern unsigned int DAT_0061168c;
// GLOBAL: LEGOLAND 0x00611958
extern unsigned int DAT_00611958;
// GLOBAL: LEGOLAND 0x00612178
extern char DAT_00612178[1];
// GLOBAL: LEGOLAND 0x00615f6c
extern unsigned int DAT_00615f6c;
// GLOBAL: LEGOLAND 0x00615f98
extern unsigned int DAT_00615f98;
// GLOBAL: LEGOLAND 0x00616000
extern unsigned int DAT_00616000;
// GLOBAL: LEGOLAND 0x00616004
extern unsigned int DAT_00616004;
// GLOBAL: LEGOLAND 0x00616018
extern void *DAT_00616018;
// GLOBAL: LEGOLAND 0x00616028
extern unsigned int DAT_00616028;
// GLOBAL: LEGOLAND 0x0061602c
extern unsigned int DAT_0061602c;
// GLOBAL: LEGOLAND 0x00616030
extern unsigned int DAT_00616030;
// GLOBAL: LEGOLAND 0x00616034
extern unsigned short DAT_00616034;
// GLOBAL: LEGOLAND 0x00616048
extern unsigned int DAT_00616048;
// GLOBAL: LEGOLAND 0x0061604c
extern unsigned int DAT_0061604c;
// GLOBAL: LEGOLAND 0x00616050
extern unsigned int DAT_00616050;
// GLOBAL: LEGOLAND 0x00616054
extern unsigned int DAT_00616054;
// GLOBAL: LEGOLAND 0x00616058
extern unsigned int DAT_00616058;
// GLOBAL: LEGOLAND 0x0061605c
extern unsigned int DAT_0061605c;
// GLOBAL: LEGOLAND 0x00616060
extern void *DAT_00616060;
// GLOBAL: LEGOLAND 0x0061606c
extern unsigned int DAT_0061606c;
// GLOBAL: LEGOLAND 0x00616070
extern unsigned int DAT_00616070;
// GLOBAL: LEGOLAND 0x00616090
extern void *DAT_00616090;
// GLOBAL: LEGOLAND 0x00616094
extern void *DAT_00616094;
// GLOBAL: LEGOLAND 0x00616098
extern void *DAT_00616098;
// GLOBAL: LEGOLAND 0x006160a0
extern unsigned int DAT_006160a0;
// GLOBAL: LEGOLAND 0x006160a4
extern unsigned int DAT_006160a4;
// GLOBAL: LEGOLAND 0x006160a8
extern unsigned int DAT_006160a8;
// GLOBAL: LEGOLAND 0x006160ac
extern unsigned short DAT_006160ac;
// GLOBAL: LEGOLAND 0x006160bc
extern unsigned int DAT_006160bc;
// GLOBAL: LEGOLAND 0x006160c0
extern unsigned int DAT_006160c0;
// GLOBAL: LEGOLAND 0x006160c4
extern struct CarouselNode *DAT_006160c4;
// GLOBAL: LEGOLAND 0x006160d0
extern unsigned int DAT_006160d0;
// GLOBAL: LEGOLAND 0x006160d4
extern struct RinData *DAT_006160d4;
// GLOBAL: LEGOLAND 0x006160d8
extern struct Sprite *DAT_006160d8;
// GLOBAL: LEGOLAND 0x006160e0
extern struct Sprite *DAT_006160e0;
// GLOBAL: LEGOLAND 0x006160e4
extern struct Position *DAT_006160e4;
// GLOBAL: LEGOLAND 0x006160e8
extern void *DAT_006160e8;
// GLOBAL: LEGOLAND 0x006160f4
extern unsigned int DAT_006160f4;
// GLOBAL: LEGOLAND 0x006160fc
extern unsigned int DAT_006160fc;
// GLOBAL: LEGOLAND 0x00616100
extern unsigned int DAT_00616100;
// GLOBAL: LEGOLAND 0x00616104
extern unsigned int DAT_00616104;
// GLOBAL: LEGOLAND 0x00616108
extern unsigned int DAT_00616108;
// GLOBAL: LEGOLAND 0x0061610c
extern unsigned int DAT_0061610c;
// GLOBAL: LEGOLAND 0x00616120
extern unsigned int DAT_00616120;
// GLOBAL: LEGOLAND 0x00616124
extern unsigned int DAT_00616124;
// GLOBAL: LEGOLAND 0x00616128
extern unsigned int DAT_00616128;
// GLOBAL: LEGOLAND 0x0061612c
extern unsigned short DAT_0061612c;
// GLOBAL: LEGOLAND 0x0061613c
extern unsigned int DAT_0061613c;
// GLOBAL: LEGOLAND 0x00616144
extern struct BlokeNode *DAT_00616144;
// GLOBAL: LEGOLAND 0x00616148
extern struct SaveBlock *DAT_00616148;
// GLOBAL: LEGOLAND 0x0061614c
extern unsigned int DAT_0061614c;
// GLOBAL: LEGOLAND 0x00616150
extern unsigned int DAT_00616150;
// GLOBAL: LEGOLAND 0x00616158
extern unsigned int DAT_00616158;
// GLOBAL: LEGOLAND 0x0061615c
extern unsigned int DAT_0061615c;
// GLOBAL: LEGOLAND 0x00616164
extern struct JungleNode *DAT_00616164;
// GLOBAL: LEGOLAND 0x00629c30
extern struct JungleFish *DAT_00629c30;
// GLOBAL: LEGOLAND 0x00629c3c
extern struct JungleScore *DAT_00629c3c;
// GLOBAL: LEGOLAND 0x00629c40
extern unsigned char DAT_00629c40[16];
// GLOBAL: LEGOLAND 0x00629c50
extern void *DAT_00629c50;
// GLOBAL: LEGOLAND 0x0062fd2c
extern unsigned int DAT_0062fd2c;
// GLOBAL: LEGOLAND 0x0062fd3c
extern struct JailCell *DAT_0062fd3c;
// GLOBAL: LEGOLAND 0x0062fd68
extern void *DAT_0062fd68;
// GLOBAL: LEGOLAND 0x0062fd6c
extern void *DAT_0062fd6c;
// GLOBAL: LEGOLAND 0x0062fd74
extern void *DAT_0062fd74;
// GLOBAL: LEGOLAND 0x0062fd7c
extern void *DAT_0062fd7c;
// GLOBAL: LEGOLAND 0x0062fd80
extern void *DAT_0062fd80;
// GLOBAL: LEGOLAND 0x0062fda8
extern void *DAT_0062fda8;
// GLOBAL: LEGOLAND 0x0062fdb0
extern unsigned int DAT_0062fdb0;
// GLOBAL: LEGOLAND 0x0062fdb4
extern unsigned int DAT_0062fdb4;
// GLOBAL: LEGOLAND 0x0062fdb8
extern unsigned int DAT_0062fdb8;
// GLOBAL: LEGOLAND 0x0062fdbc
extern unsigned short DAT_0062fdbc;
// GLOBAL: LEGOLAND 0x0062fdc8
extern void *DAT_0062fdc8;
// GLOBAL: LEGOLAND 0x0062fdcc
extern void *DAT_0062fdcc;
// GLOBAL: LEGOLAND 0x0062fdd0
extern void *DAT_0062fdd0;
// GLOBAL: LEGOLAND 0x0062fde4
extern unsigned int DAT_0062fde4;
// GLOBAL: LEGOLAND 0x0062fde8
extern void *DAT_0062fde8;
// GLOBAL: LEGOLAND 0x0062fdfc
extern void *DAT_0062fdfc;
// GLOBAL: LEGOLAND 0x0062fe04
extern void *DAT_0062fe04;
// GLOBAL: LEGOLAND 0x0062fe08
extern void *DAT_0062fe08;
// GLOBAL: LEGOLAND 0x0062fe10
extern unsigned int DAT_0062fe10;
// GLOBAL: LEGOLAND 0x0062fe14
extern unsigned int DAT_0062fe14;
// GLOBAL: LEGOLAND 0x0062fe18
extern unsigned int DAT_0062fe18;
// GLOBAL: LEGOLAND 0x0062fe1c
extern unsigned short DAT_0062fe1c;
// GLOBAL: LEGOLAND 0x0062fe48
extern unsigned int DAT_0062fe48;
// GLOBAL: LEGOLAND 0x0062fe4c
extern unsigned int DAT_0062fe4c;
// GLOBAL: LEGOLAND 0x0062fe50
extern unsigned int DAT_0062fe50;
// GLOBAL: LEGOLAND 0x0062fe54
extern unsigned int DAT_0062fe54;
// GLOBAL: LEGOLAND 0x0062fe58
extern unsigned int DAT_0062fe58;
// GLOBAL: LEGOLAND 0x0062fe60
extern unsigned int DAT_0062fe60;
// GLOBAL: LEGOLAND 0x0062fe64
extern unsigned int DAT_0062fe64;
// GLOBAL: LEGOLAND 0x0062fe68
extern unsigned int DAT_0062fe68;
// GLOBAL: LEGOLAND 0x0062fe6c
extern unsigned short DAT_0062fe6c;
// GLOBAL: LEGOLAND 0x0062fe78
extern void *DAT_0062fe78;
// GLOBAL: LEGOLAND 0x0062fe90
extern void *DAT_0062fe90;
// GLOBAL: LEGOLAND 0x0062fe94
extern void *DAT_0062fe94;
// GLOBAL: LEGOLAND 0x0062fe9c
extern void *DAT_0062fe9c;
// GLOBAL: LEGOLAND 0x0062fea8
extern void *DAT_0062fea8;
// GLOBAL: LEGOLAND 0x0062feac
extern void *DAT_0062feac;
// GLOBAL: LEGOLAND 0x0062feb0
extern unsigned int DAT_0062feb0[2];
// GLOBAL: LEGOLAND 0x0062feb8
extern unsigned int DAT_0062feb8[1];
// GLOBAL: LEGOLAND 0x0062febc
extern unsigned int DAT_0062febc[6];
// GLOBAL: LEGOLAND 0x0062fed4
extern unsigned int DAT_0062fed4[6];
// GLOBAL: LEGOLAND 0x0062feec
extern unsigned int DAT_0062feec[1];
// GLOBAL: LEGOLAND 0x0062fef0
extern unsigned int DAT_0062fef0;
// GLOBAL: LEGOLAND 0x0062fef4
extern void *DAT_0062fef4;
// GLOBAL: LEGOLAND 0x0062fef8
extern void *DAT_0062fef8;
// GLOBAL: LEGOLAND 0x00630100
extern void *DAT_00630100;
// GLOBAL: LEGOLAND 0x00630108
extern unsigned int DAT_00630108[1];
// GLOBAL: LEGOLAND 0x00638218
extern unsigned int DAT_00638218[1];
// GLOBAL: LEGOLAND 0x0064cd8c
extern void *DAT_0064cd8c;
// GLOBAL: LEGOLAND 0x00655a38
extern void *DAT_00655a38;
// GLOBAL: LEGOLAND 0x00655a3c
extern void *DAT_00655a3c;
// GLOBAL: LEGOLAND 0x00655a4c
extern unsigned int DAT_00655a4c;
// GLOBAL: LEGOLAND 0x00655a50
extern unsigned int DAT_00655a50;
// GLOBAL: LEGOLAND 0x00665e8c
extern unsigned int DAT_00665e8c;
// GLOBAL: LEGOLAND 0x00665f60
extern unsigned int DAT_00665f60;
// GLOBAL: LEGOLAND 0x00665fe8
extern unsigned int DAT_00665fe8;
// GLOBAL: LEGOLAND 0x00665fec
extern unsigned int DAT_00665fec;
// GLOBAL: LEGOLAND 0x00665ff8
extern unsigned int DAT_00665ff8;
// GLOBAL: LEGOLAND 0x00665ffc
extern unsigned int DAT_00665ffc;
// GLOBAL: LEGOLAND 0x00666000
extern unsigned int DAT_00666000;
// GLOBAL: LEGOLAND 0x00666004
extern unsigned int DAT_00666004;
// GLOBAL: LEGOLAND 0x00666008
extern unsigned int DAT_00666008;
// GLOBAL: LEGOLAND 0x0066600c
extern unsigned int DAT_0066600c;
// GLOBAL: LEGOLAND 0x00666010
extern unsigned int DAT_00666010;
// GLOBAL: LEGOLAND 0x00666014
extern unsigned int DAT_00666014;
// GLOBAL: LEGOLAND 0x00666018
extern unsigned int DAT_00666018;
// GLOBAL: LEGOLAND 0x0066601c
extern unsigned int DAT_0066601c;
// GLOBAL: LEGOLAND 0x00666020
extern unsigned int DAT_00666020;
// GLOBAL: LEGOLAND 0x00666024
extern unsigned int DAT_00666024;
// GLOBAL: LEGOLAND 0x00666030
extern unsigned int DAT_00666030;
// GLOBAL: LEGOLAND 0x00666034
extern unsigned int DAT_00666034;
// GLOBAL: LEGOLAND 0x00666038
extern unsigned int DAT_00666038;
// GLOBAL: LEGOLAND 0x0066603c
extern unsigned int DAT_0066603c;
// GLOBAL: LEGOLAND 0x00666040
extern unsigned int DAT_00666040;
// GLOBAL: LEGOLAND 0x00666044
extern unsigned int DAT_00666044;
// GLOBAL: LEGOLAND 0x00666048
extern unsigned int DAT_00666048;
// GLOBAL: LEGOLAND 0x0066604c
extern unsigned int DAT_0066604c;
// GLOBAL: LEGOLAND 0x00666050
extern unsigned int DAT_00666050;
// GLOBAL: LEGOLAND 0x00666054
extern unsigned int DAT_00666054;
// GLOBAL: LEGOLAND 0x00666058
extern unsigned int DAT_00666058;
// GLOBAL: LEGOLAND 0x0066605c
extern unsigned int DAT_0066605c;
// GLOBAL: LEGOLAND 0x00666060
extern unsigned int DAT_00666060;
// GLOBAL: LEGOLAND 0x00666064
extern unsigned int DAT_00666064;
// GLOBAL: LEGOLAND 0x00666068
extern unsigned int DAT_00666068;
// GLOBAL: LEGOLAND 0x0066606c
extern unsigned int DAT_0066606c;
// GLOBAL: LEGOLAND 0x00666070
extern unsigned int DAT_00666070;
// GLOBAL: LEGOLAND 0x00666074
extern unsigned int DAT_00666074;
// GLOBAL: LEGOLAND 0x00666078
extern unsigned int DAT_00666078;
// GLOBAL: LEGOLAND 0x0066607c
extern unsigned int DAT_0066607c;
// GLOBAL: LEGOLAND 0x00666080
extern unsigned int DAT_00666080;
// GLOBAL: LEGOLAND 0x00666084
extern unsigned int DAT_00666084;
// GLOBAL: LEGOLAND 0x00666088
extern unsigned int DAT_00666088;
// GLOBAL: LEGOLAND 0x0066608c
extern unsigned int DAT_0066608c;
// GLOBAL: LEGOLAND 0x00666090
extern unsigned int DAT_00666090;
// GLOBAL: LEGOLAND 0x00666094
extern unsigned int DAT_00666094;
// GLOBAL: LEGOLAND 0x00666098
extern unsigned int DAT_00666098;
// GLOBAL: LEGOLAND 0x006660a8
extern unsigned int DAT_006660a8;
// GLOBAL: LEGOLAND 0x006660ac
extern unsigned int DAT_006660ac;
// GLOBAL: LEGOLAND 0x006661bc
extern int DAT_006661bc;
// GLOBAL: LEGOLAND 0x006661c4
extern unsigned int DAT_006661c4;
// GLOBAL: LEGOLAND 0x006661c8
extern int DAT_006661c8;
// GLOBAL: LEGOLAND 0x006664f8
extern unsigned int DAT_006664f8[768];
// GLOBAL: LEGOLAND 0x006670f8
extern unsigned int DAT_006670f8;
// GLOBAL: LEGOLAND 0x00667104
extern unsigned int DAT_00667104;
// GLOBAL: LEGOLAND 0x00667114
extern unsigned int DAT_00667114;
// GLOBAL: LEGOLAND 0x00667118
extern unsigned int DAT_00667118;
// GLOBAL: LEGOLAND 0x0066711c
extern unsigned int DAT_0066711c;
// GLOBAL: LEGOLAND 0x00667120
extern unsigned int DAT_00667120;
// GLOBAL: LEGOLAND 0x00667128
extern char DAT_00667128[512];
// GLOBAL: LEGOLAND 0x006675b4
extern unsigned int DAT_006675b4;
// GLOBAL: LEGOLAND 0x00667c10
extern unsigned int DAT_00667c10;
// GLOBAL: LEGOLAND 0x00667c28
extern unsigned int DAT_00667c28;
// GLOBAL: LEGOLAND 0x00667c2c
extern struct Sprite *DAT_00667c2c;
// GLOBAL: LEGOLAND 0x00667c30
extern unsigned int DAT_00667c30;
// GLOBAL: LEGOLAND 0x00667c34
extern unsigned int DAT_00667c34;
// GLOBAL: LEGOLAND 0x00667c7c
extern unsigned int DAT_00667c7c;
// GLOBAL: LEGOLAND 0x00667c88
extern unsigned int DAT_00667c88;
// GLOBAL: LEGOLAND 0x00667c8c
extern unsigned int DAT_00667c8c;
// GLOBAL: LEGOLAND 0x00667c90
extern unsigned int DAT_00667c90;
// GLOBAL: LEGOLAND 0x00667c94
extern unsigned int DAT_00667c94;
// GLOBAL: LEGOLAND 0x00667c9c
extern void *DAT_00667c9c;
// GLOBAL: LEGOLAND 0x00667ca4
extern unsigned int DAT_00667ca4;
// GLOBAL: LEGOLAND 0x00667ca8
extern void *OverlayList;
// GLOBAL: LEGOLAND 0x00667cac
extern unsigned int OverlayILF;
// GLOBAL: LEGOLAND 0x00667cb4
extern int ScrollX;
// GLOBAL: LEGOLAND 0x00667cb8
extern int ScrollY;
// GLOBAL: LEGOLAND 0x00667cd8
extern unsigned int DAT_00667cd8;
// GLOBAL: LEGOLAND 0x00667ce0
extern int DAT_00667ce0;
// GLOBAL: LEGOLAND 0x00667ce4
extern int DAT_00667ce4;
// GLOBAL: LEGOLAND 0x00667ce8
extern int DAT_00667ce8;
// GLOBAL: LEGOLAND 0x00667cec
extern int DAT_00667cec;
// GLOBAL: LEGOLAND 0x00667cf0
extern int DAT_00667cf0;
// GLOBAL: LEGOLAND 0x00667cf4
extern int DAT_00667cf4;
// GLOBAL: LEGOLAND 0x00667cf8
extern int DAT_00667cf8;
// GLOBAL: LEGOLAND 0x00667cfc
extern int DAT_00667cfc;
// GLOBAL: LEGOLAND 0x00667d08
extern int DAT_00667d08;
// GLOBAL: LEGOLAND 0x00667d10
extern unsigned int DAT_00667d10;
// GLOBAL: LEGOLAND 0x00667d54
extern unsigned int DAT_00667d54;
// GLOBAL: LEGOLAND 0x00667d58
extern int DAT_00667d58;
// GLOBAL: LEGOLAND 0x00667d70
extern unsigned int DDRAWENV[246];
// GLOBAL: LEGOLAND 0x00668070
extern void *DAT_00668070;
// GLOBAL: LEGOLAND 0x0066807c
extern void *renderEngine;
// GLOBAL: LEGOLAND 0x00668084
extern unsigned int DAT_00668084;
// GLOBAL: LEGOLAND 0x00668088
extern unsigned int DAT_00668088;
// GLOBAL: LEGOLAND 0x0066808c
extern void *PTR_0066808c;
// GLOBAL: LEGOLAND 0x00668090
extern void *PTR_00668090;
// GLOBAL: LEGOLAND 0x00668094
extern void *PTR_00668094;
// GLOBAL: LEGOLAND 0x00668098
extern void *PTR_00668098;
// GLOBAL: LEGOLAND 0x00668118
extern int renderEngineTargetIdx;
// GLOBAL: LEGOLAND 0x0066811c
extern void *renderEngineTargets[1];
// GLOBAL: LEGOLAND 0x00668148
extern unsigned int DAT_00668148;
// GLOBAL: LEGOLAND 0x0066814c
extern unsigned int DAT_0066814c;
// GLOBAL: LEGOLAND 0x006681e8
extern unsigned int DAT_006681e8;
// GLOBAL: LEGOLAND 0x00668204
extern unsigned int DAT_00668204;
// GLOBAL: LEGOLAND 0x00668208
extern unsigned int DAT_00668208;
// GLOBAL: LEGOLAND 0x00668610
extern unsigned int DAT_00668610;
// GLOBAL: LEGOLAND 0x00668614
extern unsigned int DAT_00668614;
// GLOBAL: LEGOLAND 0x00668618
extern unsigned int DAT_00668618;
// GLOBAL: LEGOLAND 0x0066861c
extern unsigned char DAT_0066861c;
// GLOBAL: LEGOLAND 0x0066869b
extern unsigned char DAT_0066869b;
// GLOBAL: LEGOLAND 0x0066869c
extern char DAT_0066869c[128];
// GLOBAL: LEGOLAND 0x0066871b
extern unsigned char DAT_0066871b;
// GLOBAL: LEGOLAND 0x0066871c
extern unsigned int DAT_0066871c;
// GLOBAL: LEGOLAND 0x00668724
extern struct ObjectiveEvent *DAT_00668724;
// GLOBAL: LEGOLAND 0x00668728
extern struct ObjectiveEvent *DAT_00668728;
// GLOBAL: LEGOLAND 0x0066872c
extern unsigned int DAT_0066872c[64];
// GLOBAL: LEGOLAND 0x00668780
extern unsigned int DAT_00668780;
// GLOBAL: LEGOLAND 0x00668784
extern void *DAT_00668784;
// GLOBAL: LEGOLAND 0x00668788
extern unsigned int DAT_00668788;
// GLOBAL: LEGOLAND 0x0066878c
extern int DAT_0066878c;
// GLOBAL: LEGOLAND 0x00668790
extern unsigned int DAT_00668790;
// GLOBAL: LEGOLAND 0x00668794
extern unsigned int DAT_00668794;
// GLOBAL: LEGOLAND 0x00668798
extern void *DAT_00668798;
// GLOBAL: LEGOLAND 0x0066879c
extern unsigned int DAT_0066879c;
// GLOBAL: LEGOLAND 0x006687a4
extern unsigned int DAT_006687a4;
// GLOBAL: LEGOLAND 0x006687a8
extern unsigned int DAT_006687a8;
// GLOBAL: LEGOLAND 0x006687ac
extern unsigned int DAT_006687ac;
// GLOBAL: LEGOLAND 0x006687b0
extern unsigned int DAT_006687b0;
// GLOBAL: LEGOLAND 0x006687bc
extern unsigned int DAT_006687bc;
// GLOBAL: LEGOLAND 0x006687c0
extern unsigned int DAT_006687c0;
// GLOBAL: LEGOLAND 0x006687c8
extern struct InterfaceIconNode *DAT_006687c8;
// GLOBAL: LEGOLAND 0x006687cc
extern struct InterfaceIconNode *DAT_006687cc;
// GLOBAL: LEGOLAND 0x006687d0
extern void *FocussedIconPtr;
// GLOBAL: LEGOLAND 0x00668828
extern void *DAT_00668828;
// GLOBAL: LEGOLAND 0x0066882c
extern void *DAT_0066882c;
// GLOBAL: LEGOLAND 0x00668830
extern void *DAT_00668830;
// GLOBAL: LEGOLAND 0x00668834
extern void *DAT_00668834;
// GLOBAL: LEGOLAND 0x006688a8
extern void *DAT_006688a8;
// GLOBAL: LEGOLAND 0x006688ac
extern void *DAT_006688ac;
// GLOBAL: LEGOLAND 0x006688b0
extern void *DAT_006688b0;
// GLOBAL: LEGOLAND 0x006688d0
extern unsigned int DAT_006688d0;
// GLOBAL: LEGOLAND 0x006688d4
extern void *DAT_006688d4;
// GLOBAL: LEGOLAND 0x006688d8
extern void *DAT_006688d8;
// GLOBAL: LEGOLAND 0x006688e0
extern struct Sprite *DAT_006688e0;
// GLOBAL: LEGOLAND 0x006688e4
extern struct Sprite *DAT_006688e4;
// GLOBAL: LEGOLAND 0x006688e8
extern struct Sprite *DAT_006688e8;
// GLOBAL: LEGOLAND 0x006688ec
extern struct Sprite *DAT_006688ec;
// GLOBAL: LEGOLAND 0x006688f0
extern struct Sprite *DAT_006688f0;
// GLOBAL: LEGOLAND 0x006688f4
extern struct Sprite *DAT_006688f4;
// GLOBAL: LEGOLAND 0x006688f8
extern struct Sprite *DAT_006688f8;
// GLOBAL: LEGOLAND 0x006688fc
extern struct Sprite *DAT_006688fc;
// GLOBAL: LEGOLAND 0x00668900
extern struct Sprite *DAT_00668900;
// GLOBAL: LEGOLAND 0x00668904
extern struct Sprite *DAT_00668904;
// GLOBAL: LEGOLAND 0x00668908
extern struct Sprite *DAT_00668908;
// GLOBAL: LEGOLAND 0x0066890c
extern struct Sprite *DAT_0066890c;
// GLOBAL: LEGOLAND 0x00668914
extern struct Sprite *DAT_00668914;
// GLOBAL: LEGOLAND 0x00668918
extern struct Sprite *DAT_00668918;
// GLOBAL: LEGOLAND 0x0066891c
extern struct Sprite *DAT_0066891c;
// GLOBAL: LEGOLAND 0x00668920
extern struct Sprite *DAT_00668920;
// GLOBAL: LEGOLAND 0x00668924
extern struct Sprite *DAT_00668924;
// GLOBAL: LEGOLAND 0x00668928
extern struct Sprite *DAT_00668928;
// GLOBAL: LEGOLAND 0x0066892c
extern struct Sprite *DAT_0066892c;
// GLOBAL: LEGOLAND 0x00668930
extern struct Sprite *DAT_00668930;
// GLOBAL: LEGOLAND 0x00668934
extern struct Sprite *DAT_00668934;
// GLOBAL: LEGOLAND 0x00668938
extern struct Sprite *DAT_00668938;
// GLOBAL: LEGOLAND 0x0066893c
extern struct Sprite *DAT_0066893c;
// GLOBAL: LEGOLAND 0x00668940
extern struct Sprite *DAT_00668940;
// GLOBAL: LEGOLAND 0x00668944
extern struct Sprite *DAT_00668944;
// GLOBAL: LEGOLAND 0x00668948
extern struct Sprite *DAT_00668948;
// GLOBAL: LEGOLAND 0x0066894c
extern struct Sprite *DAT_0066894c;
// GLOBAL: LEGOLAND 0x00668950
extern struct Sprite *DAT_00668950;
// GLOBAL: LEGOLAND 0x00668954
extern unsigned int DAT_00668954;
// GLOBAL: LEGOLAND 0x00668958
extern struct Sprite *DAT_00668958;
// GLOBAL: LEGOLAND 0x00668960
extern unsigned int DAT_00668960;
// GLOBAL: LEGOLAND 0x00668d68
extern unsigned int DAT_00668d68;
// GLOBAL: LEGOLAND 0x00668d78
extern unsigned char DAT_00668d78[16];
// GLOBAL: LEGOLAND 0x00668d80
extern int DAT_00668d80;
// GLOBAL: LEGOLAND 0x00668d88
extern void *dinput;
// GLOBAL: LEGOLAND 0x00668d8c
extern void *dinput_keyboard;
// GLOBAL: LEGOLAND 0x00668d90
extern void *dintput_mouse;
// GLOBAL: LEGOLAND 0x00668e20
extern unsigned int DAT_00668e20[4];
// GLOBAL: LEGOLAND 0x00668e34
extern unsigned int DAT_00668e34;
// GLOBAL: LEGOLAND 0x00668e38
extern unsigned int DAT_00668e38;
// GLOBAL: LEGOLAND 0x00668e40
extern struct InterfaceListNode *DAT_00668e40;
// GLOBAL: LEGOLAND 0x00668e68
extern unsigned int DAT_00668e68;
// GLOBAL: LEGOLAND 0x00668e6c
extern unsigned int DAT_00668e6c;
// GLOBAL: LEGOLAND 0x00668e70
extern unsigned int DAT_00668e70;
// GLOBAL: LEGOLAND 0x00668e74
extern unsigned int DAT_00668e74;
// GLOBAL: LEGOLAND 0x00668e78
extern unsigned int DAT_00668e78;
// GLOBAL: LEGOLAND 0x00668e7c
extern unsigned int DAT_00668e7c;
// GLOBAL: LEGOLAND 0x00668e80
extern unsigned int DAT_00668e80;
// GLOBAL: LEGOLAND 0x00668e84
extern unsigned int DAT_00668e84;
// GLOBAL: LEGOLAND 0x00668e88
extern unsigned int DAT_00668e88;
// GLOBAL: LEGOLAND 0x00668e8c
extern unsigned int DAT_00668e8c;
// GLOBAL: LEGOLAND 0x00668e90
extern unsigned int DAT_00668e90;
// GLOBAL: LEGOLAND 0x00668e94
extern unsigned int DAT_00668e94;
// GLOBAL: LEGOLAND 0x00668e98
extern unsigned int DAT_00668e98;
// GLOBAL: LEGOLAND 0x00668e9c
extern void *DAT_00668e9c;
// GLOBAL: LEGOLAND 0x00668ea0
extern unsigned int DAT_00668ea0;
// GLOBAL: LEGOLAND 0x00668ea4
extern unsigned int DAT_00668ea4;
// GLOBAL: LEGOLAND 0x00668eb0
extern unsigned int DAT_00668eb0;
// GLOBAL: LEGOLAND 0x00668eb4
extern unsigned int DAT_00668eb4;
// GLOBAL: LEGOLAND 0x00668eb8
extern unsigned int DAT_00668eb8;
// GLOBAL: LEGOLAND 0x00668ebc
extern unsigned int DAT_00668ebc;
// GLOBAL: LEGOLAND 0x00668ed8
extern struct InterfaceResearchNode *DAT_00668ed8;
// GLOBAL: LEGOLAND 0x00668fc0
extern void *DAT_00668fc0;
// GLOBAL: LEGOLAND 0x00668fc4
extern struct InterfaceQueryNode *DAT_00668fc4;
// GLOBAL: LEGOLAND 0x00669050
extern unsigned char DAT_00669050;
// GLOBAL: LEGOLAND 0x00669054
extern unsigned int DAT_00669054;
// GLOBAL: LEGOLAND 0x00669058
extern char DAT_00669058[0x100];
// GLOBAL: LEGOLAND 0x00669098
extern unsigned int DAT_00669098;
// GLOBAL: LEGOLAND 0x006691a4
extern unsigned int DAT_006691a4;
// GLOBAL: LEGOLAND 0x006691a8
extern unsigned int **DAT_006691a8;
// GLOBAL: LEGOLAND 0x006691b0
extern int DAT_006691b0;
// GLOBAL: LEGOLAND 0x00669200
extern unsigned int *DAT_00669200;
// GLOBAL: LEGOLAND 0x00669204
extern unsigned int DAT_00669204;
// GLOBAL: LEGOLAND 0x00669208
extern void *g_hInstance;
// GLOBAL: LEGOLAND 0x00669210
extern void *DAT_00669210;
// GLOBAL: LEGOLAND 0x00669240
extern void *ObjectClassList;
// GLOBAL: LEGOLAND 0x00669248
extern void *DAT_00669248;
// GLOBAL: LEGOLAND 0x0066924c
extern unsigned int DAT_0066924c;
// GLOBAL: LEGOLAND 0x00669258
extern unsigned int DAT_00669258[1152];
// GLOBAL: LEGOLAND 0x0066b44c
extern void *DAT_0066b44c;
// GLOBAL: LEGOLAND 0x0066b450
extern void *DAT_0066b450;
// GLOBAL: LEGOLAND 0x0066b458
extern void *DAT_0066b458;
// GLOBAL: LEGOLAND 0x0066b460
extern unsigned int DAT_0066b460;
// GLOBAL: LEGOLAND 0x0066b468
extern unsigned int DAT_0066b468;
// GLOBAL: LEGOLAND 0x0066b46c
extern unsigned int DAT_0066b46c;
// GLOBAL: LEGOLAND 0x0066b574
extern void *FirstBloke;
// GLOBAL: LEGOLAND 0x0066b57c
extern void *DAT_0066b57c;
// GLOBAL: LEGOLAND 0x0066b5a4
extern void *DAT_0066b5a4;
// GLOBAL: LEGOLAND 0x0066b5a8
extern unsigned int DAT_0066b5a8;
// GLOBAL: LEGOLAND 0x0066b61c
extern unsigned int DAT_0066b61c;
// GLOBAL: LEGOLAND 0x0066b630
extern unsigned int DAT_0066b630;
// GLOBAL: LEGOLAND 0x0066be40
extern unsigned int DAT_0066be40;
// GLOBAL: LEGOLAND 0x0066be44
extern unsigned int DAT_0066be44;
// GLOBAL: LEGOLAND 0x0066be48
extern unsigned int DAT_0066be48;
// GLOBAL: LEGOLAND 0x0066be4c
extern unsigned int DAT_0066be4c;
// GLOBAL: LEGOLAND 0x00701e58
extern unsigned int DAT_00701e58;
// GLOBAL: LEGOLAND 0x00701e5c
extern unsigned int DAT_00701e5c;
// GLOBAL: LEGOLAND 0x00701e60
extern unsigned int DAT_00701e60;
// GLOBAL: LEGOLAND 0x00797e68
extern unsigned int DAT_00797e68;
// GLOBAL: LEGOLAND 0x00797e6c
extern void *DAT_00797e6c;
// GLOBAL: LEGOLAND 0x00798190
extern void *DAT_00798190[256];
// GLOBAL: LEGOLAND 0x00798590
extern void *DAT_00798590;
// GLOBAL: LEGOLAND 0x00798624
extern void *DAT_00798624;
// GLOBAL: LEGOLAND 0x00798628
extern void *DAT_00798628;
// GLOBAL: LEGOLAND 0x00798630
extern unsigned int DAT_00798630;
// GLOBAL: LEGOLAND 0x00798634
extern unsigned int DAT_00798634;
// GLOBAL: LEGOLAND 0x00798638
extern unsigned int DAT_00798638;
// GLOBAL: LEGOLAND 0x0079863c
extern unsigned int DAT_0079863c;
// GLOBAL: LEGOLAND 0x00798648
extern unsigned int DAT_00798648;
// GLOBAL: LEGOLAND 0x00798660
extern unsigned int DAT_00798660;
// GLOBAL: LEGOLAND 0x00798664
extern unsigned int DAT_00798664;
// GLOBAL: LEGOLAND 0x00798668
extern unsigned int DAT_00798668;
// GLOBAL: LEGOLAND 0x00798674
extern struct Sprite *DAT_00798674;
// GLOBAL: LEGOLAND 0x00798678
extern struct Sprite *DAT_00798678;
// GLOBAL: LEGOLAND 0x0079867c
extern struct Sprite *DAT_0079867c;
// GLOBAL: LEGOLAND 0x00798680
extern struct Sprite *DAT_00798680;
// GLOBAL: LEGOLAND 0x00798684
extern struct Sprite *DAT_00798684;
// GLOBAL: LEGOLAND 0x00798688
extern struct Sprite *DAT_00798688;
// GLOBAL: LEGOLAND 0x0079868c
extern struct Sprite *DAT_0079868c;
// GLOBAL: LEGOLAND 0x00798690
extern struct Sprite *DAT_00798690;
// GLOBAL: LEGOLAND 0x00798694
extern struct Sprite *DAT_00798694;
// GLOBAL: LEGOLAND 0x00798698
extern struct Sprite *DAT_00798698;
// GLOBAL: LEGOLAND 0x0079869c
extern struct Sprite *DAT_0079869c;
// GLOBAL: LEGOLAND 0x007986a0
extern struct Sprite *DAT_007986a0;
// GLOBAL: LEGOLAND 0x007986a4
extern struct Sprite *DAT_007986a4;
// GLOBAL: LEGOLAND 0x007986a8
extern struct Sprite *DAT_007986a8;
// GLOBAL: LEGOLAND 0x007986ac
extern struct Sprite *DAT_007986ac;
// GLOBAL: LEGOLAND 0x007986b0
extern struct Sprite *DAT_007986b0;
// GLOBAL: LEGOLAND 0x007986b4
extern struct Sprite *DAT_007986b4;
// GLOBAL: LEGOLAND 0x007986b8
extern unsigned int DAT_007986b8;
// GLOBAL: LEGOLAND 0x007986bc
extern struct Sprite *DAT_007986bc;
// GLOBAL: LEGOLAND 0x007986c0
extern struct Sprite *DAT_007986c0;
// GLOBAL: LEGOLAND 0x007986c4
extern struct Sprite *DAT_007986c4;
// GLOBAL: LEGOLAND 0x007986c8
extern struct Sprite *DAT_007986c8;
// GLOBAL: LEGOLAND 0x007986cc
extern struct Sprite *DAT_007986cc;
// GLOBAL: LEGOLAND 0x007986d0
extern struct Sprite *DAT_007986d0;
// GLOBAL: LEGOLAND 0x007986d4
extern struct Sprite *DAT_007986d4;
// GLOBAL: LEGOLAND 0x007986d8
extern unsigned int DAT_007986d8;
// GLOBAL: LEGOLAND 0x007986dc
extern unsigned int DAT_007986dc;
// GLOBAL: LEGOLAND 0x007986e0
extern unsigned int DAT_007986e0;
// GLOBAL: LEGOLAND 0x007986e4
extern unsigned int DAT_007986e4;
// GLOBAL: LEGOLAND 0x007986e8
extern unsigned int DAT_007986e8;
// GLOBAL: LEGOLAND 0x007986f4
extern unsigned int DAT_007986f4;
// GLOBAL: LEGOLAND 0x007986f8
extern unsigned int DAT_007986f8;
// GLOBAL: LEGOLAND 0x00798704
extern struct Sprite *DAT_00798704;
// GLOBAL: LEGOLAND 0x00798708
extern struct Sprite *DAT_00798708;
// GLOBAL: LEGOLAND 0x0079870c
extern struct Sprite *DAT_0079870c;
// GLOBAL: LEGOLAND 0x00798710
extern struct Sprite *DAT_00798710;
// GLOBAL: LEGOLAND 0x00798714
extern struct Sprite *DAT_00798714;
// GLOBAL: LEGOLAND 0x00798718
extern struct Sprite *DAT_00798718;
// GLOBAL: LEGOLAND 0x0079871c
extern struct Sprite *DAT_0079871c;
// GLOBAL: LEGOLAND 0x00798720
extern struct Sprite *DAT_00798720;
// GLOBAL: LEGOLAND 0x00798724
extern struct Sprite *DAT_00798724;
// GLOBAL: LEGOLAND 0x00798728
extern struct Sprite *DAT_00798728;
// GLOBAL: LEGOLAND 0x0079872c
extern struct Sprite *DAT_0079872c;
// GLOBAL: LEGOLAND 0x00798730
extern struct Sprite *DAT_00798730;
// GLOBAL: LEGOLAND 0x00798734
extern void *DAT_00798734;
// GLOBAL: LEGOLAND 0x0079873c
extern unsigned int DAT_0079873c;
// GLOBAL: LEGOLAND 0x00798740
extern unsigned int DAT_00798740;
// GLOBAL: LEGOLAND 0x00798764
extern unsigned int DAT_00798764;
// GLOBAL: LEGOLAND 0x00798768
extern unsigned int DAT_00798768;
// GLOBAL: LEGOLAND 0x0079876c
extern unsigned int DAT_0079876c;
// GLOBAL: LEGOLAND 0x00798770
extern unsigned int DAT_00798770;
// GLOBAL: LEGOLAND 0x00798778
extern char DAT_00798778[0x100];
// GLOBAL: LEGOLAND 0x00798878
extern unsigned int DAT_00798878;
// GLOBAL: LEGOLAND 0x0079887c
extern unsigned int DAT_0079887c;
// GLOBAL: LEGOLAND 0x00798880
extern unsigned int DAT_00798880;
// GLOBAL: LEGOLAND 0x00798890
extern void *DAT_00798890;
// GLOBAL: LEGOLAND 0x007988a0
extern unsigned int DAT_007988a0;
// GLOBAL: LEGOLAND 0x007988bc
extern unsigned int DAT_007988bc;
// GLOBAL: LEGOLAND 0x007988c0
extern unsigned int DAT_007988c0;
// GLOBAL: LEGOLAND 0x007988c4
extern unsigned int DAT_007988c4;
// GLOBAL: LEGOLAND 0x007988c8
extern unsigned int DAT_007988c8;
// GLOBAL: LEGOLAND 0x007988cc
extern void *DAT_007988cc;
// GLOBAL: LEGOLAND 0x0079a694
extern unsigned int DAT_0079a694;
// GLOBAL: LEGOLAND 0x0079a698
extern void *DAT_0079a698;
// GLOBAL: LEGOLAND 0x0079a6a0
extern void *DAT_0079a6a0;
// GLOBAL: LEGOLAND 0x0079a6a4
extern unsigned int DAT_0079a6a4;
// GLOBAL: LEGOLAND 0x0079a6a8
extern unsigned int DAT_0079a6a8;
// GLOBAL: LEGOLAND 0x0079a6ac
extern unsigned int DAT_0079a6ac;
// GLOBAL: LEGOLAND 0x0079a7c0
extern void *DAT_0079a7c0;
// GLOBAL: LEGOLAND 0x0079a7c4
extern void *DAT_0079a7c4;
// GLOBAL: LEGOLAND 0x0079a7c8
extern unsigned int DAT_0079a7c8;
// GLOBAL: LEGOLAND 0x0079a7cc
extern unsigned int DAT_0079a7cc;
// GLOBAL: LEGOLAND 0x0079a7d0
extern unsigned int DAT_0079a7d0;
// GLOBAL: LEGOLAND 0x0079a7d8
extern int DAT_0079a7d8;
// GLOBAL: LEGOLAND 0x0079a7dc
extern int DAT_0079a7dc;
// GLOBAL: LEGOLAND 0x0079a7e0
extern unsigned int DAT_0079a7e0;
// GLOBAL: LEGOLAND 0x0079a7e4
extern unsigned int DAT_0079a7e4[19];
// GLOBAL: LEGOLAND 0x0079a7e8
extern unsigned int DAT_0079a7e8[19];
// GLOBAL: LEGOLAND 0x0079a830
extern unsigned int DAT_0079a830;
// GLOBAL: LEGOLAND 0x0079a834
extern int DAT_0079a834;
// GLOBAL: LEGOLAND 0x0079a838
extern int DAT_0079a838;
// GLOBAL: LEGOLAND 0x0079a83c
extern unsigned int DAT_0079a83c;
// GLOBAL: LEGOLAND 0x0079a848
extern void *DAT_0079a848;
// GLOBAL: LEGOLAND 0x0079a84c
extern unsigned int DAT_0079a84c;
// GLOBAL: LEGOLAND 0x0079a850
extern void *strings[10];
// GLOBAL: LEGOLAND 0x0079a890
extern unsigned int DAT_0079a890;
// GLOBAL: LEGOLAND 0x0079a894
extern unsigned int DAT_0079a894;
// GLOBAL: LEGOLAND 0x0079a898
extern unsigned int DAT_0079a898;
// GLOBAL: LEGOLAND 0x0079a89c
extern unsigned int DAT_0079a89c;
// GLOBAL: LEGOLAND 0x0079a8a0
extern unsigned int DAT_0079a8a0;
// GLOBAL: LEGOLAND 0x0079a8a8
extern void *DAT_0079a8a8;
// GLOBAL: LEGOLAND 0x0079a8ac
extern void *DAT_0079a8ac;
// GLOBAL: LEGOLAND 0x0079a8b0
extern void *DAT_0079a8b0;
// GLOBAL: LEGOLAND 0x0079a8b4
extern void *DAT_0079a8b4;
// GLOBAL: LEGOLAND 0x0079a8b8
extern int DAT_0079a8b8;
// GLOBAL: LEGOLAND 0x0079a8bc
extern int DAT_0079a8bc;
// GLOBAL: LEGOLAND 0x0079a8c0
extern void *DAT_0079a8c0;
// GLOBAL: LEGOLAND 0x0079a8c4
extern void *DAT_0079a8c4;
// GLOBAL: LEGOLAND 0x0079a8c8
extern int DAT_0079a8c8;
// GLOBAL: LEGOLAND 0x0079a8cc
extern int DAT_0079a8cc;
// GLOBAL: LEGOLAND 0x0079a8d0
extern unsigned int DAT_0079a8d0;
// GLOBAL: LEGOLAND 0x0079a8d4
extern void *DAT_0079a8d4;
// GLOBAL: LEGOLAND 0x0079abfc
extern void *DAT_0079abfc;
// GLOBAL: LEGOLAND 0x0079ac04
extern unsigned int DAT_0079ac04;
// GLOBAL: LEGOLAND 0x007caca8
extern unsigned int DAT_007caca8;
// GLOBAL: LEGOLAND 0x007cacac
extern unsigned int DAT_007cacac;
// GLOBAL: LEGOLAND 0x007cacb4
extern unsigned int DAT_007cacb4;
// GLOBAL: LEGOLAND 0x007cacd8
extern void *DAT_007cacd8;
// GLOBAL: LEGOLAND 0x007cacdc
extern void *DAT_007cacdc;
// GLOBAL: LEGOLAND 0x007cad40
extern void *DAT_007cad40;
// GLOBAL: LEGOLAND 0x007cad44
extern void *DAT_007cad44;
// GLOBAL: LEGOLAND 0x007cad48
extern unsigned int DAT_007cad48;
// GLOBAL: LEGOLAND 0x007cad60
extern unsigned char DAT_007cad60[0x110];
// GLOBAL: LEGOLAND 0x007cad80
extern unsigned int DAT_007cad80;
// GLOBAL: LEGOLAND 0x007cae80
extern char DAT_007cae80[0x100];
// GLOBAL: LEGOLAND 0x007caf80
extern unsigned int DAT_007caf80;
// GLOBAL: LEGOLAND 0x007cafa0
extern void *DAT_007cafa0;
// GLOBAL: LEGOLAND 0x007cb140
extern void *DAT_007cb140;
// GLOBAL: LEGOLAND 0x007cb1c4
extern unsigned int DAT_007cb1c4;
// GLOBAL: LEGOLAND 0x007cb1e0
extern char DAT_007cb1e0[0x100];
// GLOBAL: LEGOLAND 0x007cb2e0
extern unsigned int DAT_007cb2e0;
// GLOBAL: LEGOLAND 0x007cb2f0
extern char DAT_007cb2f0[0x10];
// GLOBAL: LEGOLAND 0x007cb300
extern char DAT_007cb300[0xc];
// GLOBAL: LEGOLAND 0x007cb30c
extern char DAT_007cb30c[0x18];
// GLOBAL: LEGOLAND 0x007cb310
extern unsigned int DAT_007cb310;
// GLOBAL: LEGOLAND 0x007cb314
extern char DAT_007cb314;
// GLOBAL: LEGOLAND 0x007cb315
extern char DAT_007cb315;
// GLOBAL: LEGOLAND 0x007cb31c
extern char DAT_007cb31c;
// GLOBAL: LEGOLAND 0x007cb320
extern unsigned int DAT_007cb320;
// GLOBAL: LEGOLAND 0x007cb324
extern unsigned int DAT_007cb324;
// GLOBAL: LEGOLAND 0x007cb328
extern unsigned int DAT_007cb328;
// GLOBAL: LEGOLAND 0x007cb360
extern unsigned int DAT_007cb360;
// GLOBAL: LEGOLAND 0x007cb394
extern unsigned int DAT_007cb394;
// GLOBAL: LEGOLAND 0x007cb398
extern struct Sprite *DAT_007cb398;
// GLOBAL: LEGOLAND 0x007cb3a0
extern unsigned int DAT_007cb3a0;
// GLOBAL: LEGOLAND 0x007cb3a8
extern struct Sprite *DAT_007cb3a8;
// GLOBAL: LEGOLAND 0x007cb3ac
extern struct Sprite *DAT_007cb3ac;
// GLOBAL: LEGOLAND 0x007cb3b0
extern struct Sprite *DAT_007cb3b0;
// GLOBAL: LEGOLAND 0x007cb3b4
extern struct Sprite *DAT_007cb3b4;
// GLOBAL: LEGOLAND 0x007cb3c0
extern struct Sprite *DAT_007cb3c0;
// GLOBAL: LEGOLAND 0x007cb3c4
extern struct Sprite *DAT_007cb3c4;
// GLOBAL: LEGOLAND 0x007cb3c8
extern struct Sprite *DAT_007cb3c8;
// GLOBAL: LEGOLAND 0x007cb3cc
extern struct Sprite *DAT_007cb3cc;
// GLOBAL: LEGOLAND 0x007cb3d4
extern struct Sprite *DAT_007cb3d4;
// GLOBAL: LEGOLAND 0x007cb3e0
extern struct ObjTableEntry DAT_007cb3e0[128];
// GLOBAL: LEGOLAND 0x007cb3e2
extern struct ObjTableEntry DAT_007cb3e2[128];
// GLOBAL: LEGOLAND 0x007cb5e0
extern struct ObjTableEntry DAT_007cb5e0;
// GLOBAL: LEGOLAND 0x007cb600
extern unsigned char DAT_007cb600[1];
// GLOBAL: LEGOLAND 0x007fd624
extern unsigned int DAT_007fd624;
// GLOBAL: LEGOLAND 0x007fd630
extern unsigned int DAT_007fd630;
// GLOBAL: LEGOLAND 0x007fd634
extern void *DAT_007fd634;
// GLOBAL: LEGOLAND 0x007fd638
extern unsigned int DAT_007fd638;
// GLOBAL: LEGOLAND 0x007fdca4
extern unsigned int DAT_007fdca4;
// GLOBAL: LEGOLAND 0x007fdcc0
extern unsigned int DAT_007fdcc0;
// GLOBAL: LEGOLAND 0x007fdcc4
extern unsigned int DAT_007fdcc4;
// GLOBAL: LEGOLAND 0x007fdcc8
extern unsigned int DAT_007fdcc8;
// GLOBAL: LEGOLAND 0x007fdccc
extern unsigned int DAT_007fdccc;
// GLOBAL: LEGOLAND 0x007fdcd0
extern unsigned int DAT_007fdcd0;
// GLOBAL: LEGOLAND 0x007fdcd4
extern unsigned int DAT_007fdcd4;
// GLOBAL: LEGOLAND 0x007fdcd8
extern unsigned int DAT_007fdcd8;
// GLOBAL: LEGOLAND 0x007fdcdc
extern unsigned int DAT_007fdcdc;
// GLOBAL: LEGOLAND 0x007fdce0
extern unsigned int DAT_007fdce0;
// GLOBAL: LEGOLAND 0x007fdd00
extern unsigned int DAT_007fdd00[9];
// GLOBAL: LEGOLAND 0x007fdd40
extern unsigned int DAT_007fdd40;
// GLOBAL: LEGOLAND 0x007fdd44
extern unsigned int DAT_007fdd44;
// GLOBAL: LEGOLAND 0x007fdd48
extern unsigned int DAT_007fdd48;
// GLOBAL: LEGOLAND 0x007fdd4c
extern unsigned int DAT_007fdd4c;
// GLOBAL: LEGOLAND 0x007fdd50
extern unsigned int DAT_007fdd50;
// GLOBAL: LEGOLAND 0x007fdd54
extern unsigned int DAT_007fdd54;
// GLOBAL: LEGOLAND 0x007fdd58
extern unsigned int DAT_007fdd58;
// GLOBAL: LEGOLAND 0x007fdd5c
extern unsigned int DAT_007fdd5c;
// GLOBAL: LEGOLAND 0x007fdd60
extern unsigned int DAT_007fdd60;
// GLOBAL: LEGOLAND 0x007fdd70
extern struct InterfaceProfileObj *DAT_007fdd70[4];
// GLOBAL: LEGOLAND 0x007fdd80
extern unsigned char DAT_007fdd80;
// GLOBAL: LEGOLAND 0x007fdd84
extern unsigned int DAT_007fdd84;
// GLOBAL: LEGOLAND 0x007fdd88
extern unsigned int DAT_007fdd88;
// GLOBAL: LEGOLAND 0x007fdd8c
extern unsigned char DAT_007fdd8c;
// GLOBAL: LEGOLAND 0x007fdda0
extern unsigned char DAT_007fdda0[256];
// GLOBAL: LEGOLAND 0x007fddca
extern unsigned char DAT_007fddca;
// GLOBAL: LEGOLAND 0x007fddd6
extern unsigned char DAT_007fddd6;
// GLOBAL: LEGOLAND 0x007fdea4
extern struct InfoIcon *DAT_007fdea4;
// GLOBAL: LEGOLAND 0x007fdea8
extern struct InfoIcon *DAT_007fdea8;
// GLOBAL: LEGOLAND 0x007fdeac
extern struct Sprite *DAT_007fdeac;
// GLOBAL: LEGOLAND 0x007fdeb0
extern struct Sprite *DAT_007fdeb0;
// GLOBAL: LEGOLAND 0x007fded4
extern unsigned int DAT_007fded4;
// GLOBAL: LEGOLAND 0x007fdf74
extern unsigned int DAT_007fdf74;
// GLOBAL: LEGOLAND 0x007fdf78
extern int DAT_007fdf78;
// GLOBAL: LEGOLAND 0x007fdf7c
extern unsigned int DAT_007fdf7c;
// GLOBAL: LEGOLAND 0x007fdf84
extern unsigned int DAT_007fdf84;
// GLOBAL: LEGOLAND 0x007fdf8c
extern unsigned int DAT_007fdf8c;
// GLOBAL: LEGOLAND 0x007fdf98
extern unsigned int DAT_007fdf98;
// GLOBAL: LEGOLAND 0x007fdf9c
extern unsigned int DAT_007fdf9c;
// GLOBAL: LEGOLAND 0x007fdfa0
extern unsigned int DAT_007fdfa0;
// GLOBAL: LEGOLAND 0x007fdfa4
extern unsigned int DAT_007fdfa4;
// GLOBAL: LEGOLAND 0x007fdfa8
extern unsigned int DAT_007fdfa8;
// GLOBAL: LEGOLAND 0x007fdfac
extern unsigned char DAT_007fdfac;
// GLOBAL: LEGOLAND 0x007fdfc0
extern struct InfoIcon *DAT_007fdfc0;
// GLOBAL: LEGOLAND 0x007fdfc4
extern struct InfoIcon *DAT_007fdfc4;
// GLOBAL: LEGOLAND 0x007fdfc8
extern struct Sprite *DAT_007fdfc8;
// GLOBAL: LEGOLAND 0x007fdfcc
extern struct InfoIcon *DAT_007fdfcc;
// GLOBAL: LEGOLAND 0x007fdfd0
extern struct Sprite *DAT_007fdfd0;
// GLOBAL: LEGOLAND 0x007fdfd8
extern struct InfoIcon *DAT_007fdfd8;
// GLOBAL: LEGOLAND 0x007fdfdc
extern struct InfoIcon *DAT_007fdfdc;
// GLOBAL: LEGOLAND 0x007fdfe0
extern struct InfoIcon *DAT_007fdfe0;
// GLOBAL: LEGOLAND 0x007fdfe4
extern struct Sprite *DAT_007fdfe4;
// GLOBAL: LEGOLAND 0x007fdfe8
extern struct InfoIcon *DAT_007fdfe8;
// GLOBAL: LEGOLAND 0x007fdff0
extern void *DAT_007fdff0;
// GLOBAL: LEGOLAND 0x007fdffc
extern unsigned int DAT_007fdffc;
// GLOBAL: LEGOLAND 0x007fe000
extern struct InfoIcon *DAT_007fe000;
// GLOBAL: LEGOLAND 0x007fe004
extern struct Sprite *DAT_007fe004;
// GLOBAL: LEGOLAND 0x007fe008
extern struct Sprite *DAT_007fe008;
// GLOBAL: LEGOLAND 0x007fe018
extern struct Sprite *DAT_007fe018;
// GLOBAL: LEGOLAND 0x007fe020
extern unsigned int DAT_007fe020[4];
// GLOBAL: LEGOLAND 0x007fe040
extern unsigned int DAT_007fe040;
// GLOBAL: LEGOLAND 0x007fe044
extern unsigned int DAT_007fe044;
// GLOBAL: LEGOLAND 0x007fe048
extern unsigned int DAT_007fe048;
// GLOBAL: LEGOLAND 0x007fe04c
extern unsigned int DAT_007fe04c;
// GLOBAL: LEGOLAND 0x007fe050
extern unsigned int DAT_007fe050;
// GLOBAL: LEGOLAND 0x007fe054
extern unsigned int DAT_007fe054;
// GLOBAL: LEGOLAND 0x007fe114
extern unsigned char DAT_007fe114;
// GLOBAL: LEGOLAND 0x007fe115
extern unsigned char DAT_007fe115;
// GLOBAL: LEGOLAND 0x007fe116
extern unsigned char DAT_007fe116;
// GLOBAL: LEGOLAND 0x007fe117
extern unsigned char DAT_007fe117;
// GLOBAL: LEGOLAND 0x007fe120
extern unsigned int DAT_007fe120[256];
// GLOBAL: LEGOLAND 0x007fe920
extern unsigned int DAT_007fe920;
// GLOBAL: LEGOLAND 0x007fe930
extern unsigned char DAT_007fe930[10];
// GLOBAL: LEGOLAND 0x007fe9a8
extern unsigned int DAT_007fe9a8;
// GLOBAL: LEGOLAND 0x007fe9c0
extern unsigned int DAT_007fe9c0[1];
// GLOBAL: LEGOLAND 0x007feb14
extern unsigned int DAT_007feb14;
// GLOBAL: LEGOLAND 0x007febc0
extern struct Cursor EditCursor;
// GLOBAL: LEGOLAND 0x00801400
extern struct MapElement **DAT_00801400;
// GLOBAL: LEGOLAND 0x00805f60
extern void *TileSpriteArray[256];
// GLOBAL: LEGOLAND 0x0080ff64
extern unsigned int DAT_0080ff64;
// GLOBAL: LEGOLAND 0x0080ff80
extern unsigned int DAT_0080ff80;
// GLOBAL: LEGOLAND 0x0080ff84
extern unsigned int DAT_0080ff84;
// GLOBAL: LEGOLAND 0x0080ff88
extern unsigned int DAT_0080ff88;
// GLOBAL: LEGOLAND 0x0080ffa0
extern unsigned int DAT_0080ffa0[68];
// GLOBAL: LEGOLAND 0x0080ffc4
extern unsigned int DAT_0080ffc4;
// GLOBAL: LEGOLAND 0x0080ffc8
extern unsigned int DAT_0080ffc8;
// GLOBAL: LEGOLAND 0x0080ffcc
extern unsigned int DAT_0080ffcc;
// GLOBAL: LEGOLAND 0x0080ffd0
extern unsigned char DAT_0080ffd0[4];
// GLOBAL: LEGOLAND 0x0080ffd4
extern unsigned char DAT_0080ffd4[0xf];
// GLOBAL: LEGOLAND 0x0080ffd9
extern unsigned char DAT_0080ffd9;
// GLOBAL: LEGOLAND 0x0080ffe3
extern unsigned char DAT_0080ffe3;
// GLOBAL: LEGOLAND 0x0080ffe4
extern unsigned char DAT_0080ffe4;
// GLOBAL: LEGOLAND 0x00810140
extern unsigned int DAT_00810140;
// GLOBAL: LEGOLAND 0x00810148
extern unsigned int SPRITE_TitleScreenBk;
// GLOBAL: LEGOLAND 0x00810160
extern struct Cursor QueryCursor;
// GLOBAL: LEGOLAND 0x008119a0
extern unsigned int NEWFLC_CheckDuplicate;
// GLOBAL: LEGOLAND 0x008119a4
extern unsigned int DAT_008119a4;
// GLOBAL: LEGOLAND 0x008119a8
extern unsigned int NEWFLC_BuffSize;
// GLOBAL: LEGOLAND 0x008119ac
extern unsigned short NEWFLC_Repeat;
// GLOBAL: LEGOLAND 0x008119b0
extern unsigned int EditMode;
// GLOBAL: LEGOLAND 0x008119b4
extern unsigned int DAT_008119b4;
// GLOBAL: LEGOLAND 0x008119b8
extern void *DAT_008119b8;
// GLOBAL: LEGOLAND 0x008139e0
extern void *DAT_008139e0;
// GLOBAL: LEGOLAND 0x008139e4
extern void *DAT_008139e4;
// GLOBAL: LEGOLAND 0x008139e8
extern void *DAT_008139e8;
// GLOBAL: LEGOLAND 0x008139ec
extern void *DAT_008139ec;
// GLOBAL: LEGOLAND 0x008139f0
extern void *DAT_008139f0;
// GLOBAL: LEGOLAND 0x008139f4
extern void *DAT_008139f4;
// GLOBAL: LEGOLAND 0x008139f8
extern void *DAT_008139f8;
// GLOBAL: LEGOLAND 0x008139fc
extern void *DAT_008139fc;
// GLOBAL: LEGOLAND 0x00813a00
extern void *DAT_00813a00;
// GLOBAL: LEGOLAND 0x00813a04
extern void *DAT_00813a04;
// GLOBAL: LEGOLAND 0x00813a08
extern void *DAT_00813a08;
// GLOBAL: LEGOLAND 0x00813a0c
extern void *DAT_00813a0c;
// GLOBAL: LEGOLAND 0x00813a10
extern unsigned int DAT_00813a10;
// GLOBAL: LEGOLAND 0x00813a40
extern unsigned int GamePad;
// GLOBAL: LEGOLAND 0x00813a44
extern unsigned int DAT_00813a44;
// GLOBAL: LEGOLAND 0x00813a48
extern unsigned int DAT_00813a48;
// GLOBAL: LEGOLAND 0x00813b00
extern void *CONTROLLERBUFFER;
// GLOBAL: LEGOLAND 0x00814020
extern unsigned char DAT_00814020[1];
// GLOBAL: LEGOLAND 0x0081c028
extern unsigned int DAT_0081c028;
// GLOBAL: LEGOLAND 0x0081c02c
extern unsigned int DAT_0081c02c;
// GLOBAL: LEGOLAND 0x0081c030
extern unsigned int DAT_0081c030;
// GLOBAL: LEGOLAND 0x0081c034
extern unsigned int DAT_0081c034;
// GLOBAL: LEGOLAND 0x0081c038
extern unsigned int DAT_0081c038;
// GLOBAL: LEGOLAND 0x0081c040
extern unsigned int DAT_0081c040[1];
// GLOBAL: LEGOLAND 0x0081c054
extern unsigned int DAT_0081c054[1];
// GLOBAL: LEGOLAND 0x0081c068
extern unsigned int DAT_0081c068[1];
// GLOBAL: LEGOLAND 0x0081c080
extern unsigned int DAT_0081c080;
// GLOBAL: LEGOLAND 0x0081c084
extern unsigned int DAT_0081c084;
// GLOBAL: LEGOLAND 0x0081c088
extern unsigned int DAT_0081c088;
// GLOBAL: LEGOLAND 0x0081c08c
extern void *DAT_0081c08c;
// GLOBAL: LEGOLAND 0x0081c090
extern void *DAT_0081c090;
// GLOBAL: LEGOLAND 0x0081c094
extern void *DAT_0081c094;
// GLOBAL: LEGOLAND 0x0081c098
extern void *DAT_0081c098;
// GLOBAL: LEGOLAND 0x0081c09c
extern unsigned int DAT_0081c09c;
// GLOBAL: LEGOLAND 0x0081c0a0
extern void *DAT_0081c0a0;
// GLOBAL: LEGOLAND 0x0081c0a4
extern void *DAT_0081c0a4;
// GLOBAL: LEGOLAND 0x0081c8c0
extern void *DAT_0081c8c0;
// GLOBAL: LEGOLAND 0x0081c8c4
extern void *DAT_0081c8c4;
// GLOBAL: LEGOLAND 0x0081c8c8
extern void *DAT_0081c8c8;
// GLOBAL: LEGOLAND 0x0081c8cc
extern void *DAT_0081c8cc;
// GLOBAL: LEGOLAND 0x0081c8d0
extern unsigned int DAT_0081c8d0;
// GLOBAL: LEGOLAND 0x0081c8d4
extern unsigned int DAT_0081c8d4;
// GLOBAL: LEGOLAND 0x0081c8d8
extern unsigned int DAT_0081c8d8;
// GLOBAL: LEGOLAND 0x0081c8dc
extern unsigned int DAT_0081c8dc;
// GLOBAL: LEGOLAND 0x0081cae0
extern unsigned int DAT_0081cae0;
// GLOBAL: LEGOLAND 0x0081caf0
extern void *DAT_0081caf0;
// GLOBAL: LEGOLAND 0x0081caf4
extern void *DAT_0081caf4;
// GLOBAL: LEGOLAND 0x0081cb00
extern unsigned int DAT_0081cb00;
// GLOBAL: LEGOLAND 0x0081cb04
extern unsigned int DAT_0081cb04;
// GLOBAL: LEGOLAND 0x0081cb08
extern unsigned int DAT_0081cb08;
// GLOBAL: LEGOLAND 0x0081cb0c
extern unsigned int DAT_0081cb0c;
// GLOBAL: LEGOLAND 0x0081cb14
extern struct Building *DAT_0081cb14;
// GLOBAL: LEGOLAND 0x0081cb18
extern unsigned int DAT_0081cb18;
// GLOBAL: LEGOLAND 0x0081cb1c
extern struct Building *DAT_0081cb1c;
// GLOBAL: LEGOLAND 0x0081cb20
extern unsigned int DAT_0081cb20;
// GLOBAL: LEGOLAND 0x0081cb24
extern unsigned int DAT_0081cb24;
// GLOBAL: LEGOLAND 0x0081cb28
extern unsigned int DAT_0081cb28;
// GLOBAL: LEGOLAND 0x0081cb2c
extern struct Building *DAT_0081cb2c;
// GLOBAL: LEGOLAND 0x0081cb30
extern struct Building *DAT_0081cb30;
// GLOBAL: LEGOLAND 0x0081cb34
extern unsigned int DAT_0081cb34;
// GLOBAL: LEGOLAND 0x0081cb38
extern unsigned int DAT_0081cb38;
// GLOBAL: LEGOLAND 0x0081cb3c
extern struct Building *DAT_0081cb3c;
// GLOBAL: LEGOLAND 0x0081cb40
extern struct Building *DAT_0081cb40;
// GLOBAL: LEGOLAND 0x0081cb44
extern struct Building *DAT_0081cb44;
// GLOBAL: LEGOLAND 0x0081cb48
extern unsigned int DAT_0081cb48;
// GLOBAL: LEGOLAND 0x0081cb4c
extern struct Building *DAT_0081cb4c;
// GLOBAL: LEGOLAND 0x0081cb50
extern unsigned int DAT_0081cb50;
// GLOBAL: LEGOLAND 0x0081cb54
extern unsigned int DAT_0081cb54;
// GLOBAL: LEGOLAND 0x0081cb60
extern void *DAT_0081cb60;
// GLOBAL: LEGOLAND 0x0081cb68
extern unsigned int DAT_0081cb68;
// GLOBAL: LEGOLAND 0x0081cb6c
extern unsigned int DAT_0081cb6c;
// GLOBAL: LEGOLAND 0x0081cb70
extern struct JungleCursor *DAT_0081cb70;
// GLOBAL: LEGOLAND 0x0081cb74
extern unsigned int DAT_0081cb74;
// GLOBAL: LEGOLAND 0x0081cd04
extern void *DAT_0081cd04;
// GLOBAL: LEGOLAND 0x0081cd08
extern unsigned int DAT_0081cd08;
// GLOBAL: LEGOLAND 0x0081cd0c
extern unsigned int DAT_0081cd0c;
// GLOBAL: LEGOLAND 0x0081cd10
extern unsigned int DAT_0081cd10;
// GLOBAL: LEGOLAND 0x0081cd14
extern unsigned int DAT_0081cd14;
// GLOBAL: LEGOLAND 0x0081cd18
extern struct EateryFX *DAT_0081cd18;
// GLOBAL: LEGOLAND 0x0081cd1c
extern struct EateryFX *DAT_0081cd1c;
// GLOBAL: LEGOLAND 0x0081cd20
extern unsigned int DAT_0081cd20;
// GLOBAL: LEGOLAND 0x0081cd28
extern unsigned int DAT_0081cd28;
// GLOBAL: LEGOLAND 0x0081cd34
extern unsigned int DAT_0081cd34;
// GLOBAL: LEGOLAND 0x0081cd38
extern unsigned int DAT_0081cd38;
// GLOBAL: LEGOLAND 0x0081cd3c
extern unsigned int DAT_0081cd3c;
// GLOBAL: LEGOLAND 0x0081cd44
extern struct EateryFX *DAT_0081cd44;
// GLOBAL: LEGOLAND 0x0081cd48
extern unsigned int DAT_0081cd48;
// GLOBAL: LEGOLAND 0x0081cd60
extern void *DAT_0081cd60;
// GLOBAL: LEGOLAND 0x0081cd64
extern void *DAT_0081cd64;
// GLOBAL: LEGOLAND 0x0081cd68
extern void *DAT_0081cd68;
// GLOBAL: LEGOLAND 0x0081cd6c
extern void *DAT_0081cd6c;
// GLOBAL: LEGOLAND 0x0081cd70
extern void *DAT_0081cd70;
// GLOBAL: LEGOLAND 0x0081cd74
extern void *DAT_0081cd74;
// GLOBAL: LEGOLAND 0x0081cd78
extern void *DAT_0081cd78;
// GLOBAL: LEGOLAND 0x0081cd7c
extern void *DAT_0081cd7c;
// GLOBAL: LEGOLAND 0x0081cd80
extern void *DAT_0081cd80;
// GLOBAL: LEGOLAND 0x0081cd84
extern unsigned int DAT_0081cd84;
// GLOBAL: LEGOLAND 0x0081cd88
extern unsigned int DAT_0081cd88;
// GLOBAL: LEGOLAND 0x0081cd8c
extern unsigned int DAT_0081cd8c;
// GLOBAL: LEGOLAND 0x0081cd90
extern unsigned int DAT_0081cd90;
// GLOBAL: LEGOLAND 0x0081cd94
extern unsigned int DAT_0081cd94;
// GLOBAL: LEGOLAND 0x0081cda0
extern void *DAT_0081cda0;
// GLOBAL: LEGOLAND 0x0081cda4
extern void *DAT_0081cda4;
// GLOBAL: LEGOLAND 0x0081cda8
extern void *DAT_0081cda8;
// GLOBAL: LEGOLAND 0x0081cdac
extern void *DAT_0081cdac;
// GLOBAL: LEGOLAND 0x0081cdb0
extern void *DAT_0081cdb0;
// GLOBAL: LEGOLAND 0x0081cdb4
extern void *DAT_0081cdb4;
// GLOBAL: LEGOLAND 0x0081cdb8
extern void *DAT_0081cdb8;
// GLOBAL: LEGOLAND 0x0081cdbc
extern void *DAT_0081cdbc;
// GLOBAL: LEGOLAND 0x0081cdc0
extern void *DAT_0081cdc0;
// GLOBAL: LEGOLAND 0x0081cdc4
extern void *DAT_0081cdc4;
// GLOBAL: LEGOLAND 0x0081cdc8
extern void *DAT_0081cdc8;
// GLOBAL: LEGOLAND 0x0081cdcc
extern void *DAT_0081cdcc;
// GLOBAL: LEGOLAND 0x0081cdd0
extern void *DAT_0081cdd0;
// GLOBAL: LEGOLAND 0x0081cdd4
extern void *DAT_0081cdd4;
// GLOBAL: LEGOLAND 0x0081cdd8
extern void *DAT_0081cdd8;
// GLOBAL: LEGOLAND 0x0081cddc
extern void *DAT_0081cddc;
// GLOBAL: LEGOLAND 0x0081cde0
extern struct EateryFX *DAT_0081cde0;
// GLOBAL: LEGOLAND 0x0081cde4
extern void *DAT_0081cde4;
// GLOBAL: LEGOLAND 0x0081cde8
extern unsigned int DAT_0081cde8;
// GLOBAL: LEGOLAND 0x0081cdec
extern void *DAT_0081cdec;
// GLOBAL: LEGOLAND 0x00828fe0
extern unsigned int DAT_00828fe0[2];
// GLOBAL: LEGOLAND 0x00829980
extern void *DAT_00829980;
// GLOBAL: LEGOLAND 0x0082999c
extern float DAT_0082999c;
// GLOBAL: LEGOLAND 0x008299d4
extern float DAT_008299d4;
// GLOBAL: LEGOLAND 0x008299fc
extern unsigned int DAT_008299fc;
// GLOBAL: LEGOLAND 0x00829a3c
extern unsigned char DAT_00829a3c[0x1c];
// GLOBAL: LEGOLAND 0x00829a50
extern void *DAT_00829a50;
// GLOBAL: LEGOLAND 0x00829a54
extern void *DAT_00829a54;
// GLOBAL: LEGOLAND 0x00829a60
extern float DAT_00829a60;
// GLOBAL: LEGOLAND 0x00829a80
extern char DAT_00829a80[20];
// GLOBAL: LEGOLAND 0x00829a90
extern unsigned int DAT_00829a90;
// GLOBAL: LEGOLAND 0x00829ae0
extern unsigned int DAT_00829ae0;
// GLOBAL: LEGOLAND 0x00829af8
extern unsigned int DAT_00829af8[3];
// GLOBAL: LEGOLAND 0x00829b04
extern unsigned int DAT_00829b04[3];
// GLOBAL: LEGOLAND 0x00829b88
extern unsigned int DAT_00829b88;
// GLOBAL: LEGOLAND 0x00829b8c
extern unsigned int DAT_00829b8c;
// GLOBAL: LEGOLAND 0x00829ba0
extern unsigned int DAT_00829ba0;
// GLOBAL: LEGOLAND 0x00829ba4
extern unsigned int DAT_00829ba4;
// GLOBAL: LEGOLAND 0x00829bec
extern void *DAT_00829bec;
// GLOBAL: LEGOLAND 0x00829bf0
extern void *DAT_00829bf0;
// GLOBAL: LEGOLAND 0x00829bf4
extern void *DAT_00829bf4;
// GLOBAL: LEGOLAND 0x00829bf8
extern unsigned int DAT_00829bf8;
// GLOBAL: LEGOLAND 0x00829c04
extern unsigned int DAT_00829c04;
// GLOBAL: LEGOLAND 0x0082ac60
extern void *DAT_0082ac60[1];
// GLOBAL: LEGOLAND 0x0082ad20
extern unsigned char DAT_0082ad20[0x90];
// GLOBAL: LEGOLAND 0x0082adb0
extern unsigned int DAT_0082adb0;
// GLOBAL: LEGOLAND 0x0082add0
extern unsigned int DAT_0082add0;
// GLOBAL: LEGOLAND 0x0082add4
extern unsigned int DAT_0082add4;
// GLOBAL: LEGOLAND 0x0082add8
extern unsigned int DAT_0082add8;
// GLOBAL: LEGOLAND 0x0082ade0
extern unsigned int DAT_0082ade0;
// GLOBAL: LEGOLAND 0x0082ade4
extern unsigned int DAT_0082ade4;
// GLOBAL: LEGOLAND 0x0082ade8
extern unsigned int DAT_0082ade8;
// GLOBAL: LEGOLAND 0x0082adec
extern unsigned int DAT_0082adec;
// GLOBAL: LEGOLAND 0x0082adf0
extern void *DAT_0082adf0;
// GLOBAL: LEGOLAND 0x0082adf8
extern unsigned int DAT_0082adf8;
// GLOBAL: LEGOLAND 0x0082c658
extern unsigned int DAT_0082c658;
// GLOBAL: LEGOLAND 0x0082c668
extern struct Sprite *DAT_0082c668;
// GLOBAL: LEGOLAND 0x0082c678
extern void *DAT_0082c678;
// GLOBAL: LEGOLAND 0x0082c680
extern void *DAT_0082c680;
// GLOBAL: LEGOLAND 0x0082c684
extern void *DAT_0082c684;
// GLOBAL: LEGOLAND 0x0082c688
extern unsigned int DAT_0082c688;
// GLOBAL: LEGOLAND 0x0082c690
extern unsigned int DAT_0082c690;
// GLOBAL: LEGOLAND 0x0082c694
extern unsigned int DAT_0082c694;
// GLOBAL: LEGOLAND 0x0082c6a0
extern unsigned int DAT_0082c6a0;
// GLOBAL: LEGOLAND 0x0082c6a4
extern unsigned int DAT_0082c6a4;
// GLOBAL: LEGOLAND 0x0082c6a8
extern unsigned int DAT_0082c6a8;
// GLOBAL: LEGOLAND 0x0082c6ac
extern unsigned short DAT_0082c6ac;
// GLOBAL: LEGOLAND 0x0082c6b8
extern unsigned int DAT_0082c6b8;
// GLOBAL: LEGOLAND 0x0082c6bc
extern unsigned int DAT_0082c6bc;
// GLOBAL: LEGOLAND 0x0082c6c0
extern unsigned int DAT_0082c6c0;
// GLOBAL: LEGOLAND 0x0082f760
extern struct Cursor DAT_0082f760;
// GLOBAL: LEGOLAND 0x00830b74
extern unsigned int DAT_00830b74;
// GLOBAL: LEGOLAND 0x00830f88
extern unsigned int DAT_00830f88;
// GLOBAL: LEGOLAND 0x00830f94
extern unsigned int DAT_00830f94;
// GLOBAL: LEGOLAND 0x00830f98
extern struct Position *DAT_00830f98;
// GLOBAL: LEGOLAND 0x00832824
extern unsigned int DAT_00832824[1];
// GLOBAL: LEGOLAND 0x00832828
extern unsigned int DAT_00832828[1];
// GLOBAL: LEGOLAND 0x0083291c
extern unsigned int DAT_0083291c;
// GLOBAL: LEGOLAND 0x00832920
extern unsigned int DAT_00832920;
// GLOBAL: LEGOLAND 0x00832924
extern unsigned int DAT_00832924;
// GLOBAL: LEGOLAND 0x00832934
extern int DAT_00832934;
// GLOBAL: LEGOLAND 0x00832938
extern int DAT_00832938;
// GLOBAL: LEGOLAND 0x0083293c
extern int DAT_0083293c[13];
// GLOBAL: LEGOLAND 0x00832970
extern int DAT_00832970;
// GLOBAL: LEGOLAND 0x00832974
extern unsigned int DAT_00832974;
// GLOBAL: LEGOLAND 0x00832978
extern unsigned int DAT_00832978;
// GLOBAL: LEGOLAND 0x0083297c
extern unsigned int DAT_0083297c;
// GLOBAL: LEGOLAND 0x00832980
extern unsigned int DAT_00832980;
// GLOBAL: LEGOLAND 0x00832990
extern unsigned int DAT_00832990;
// GLOBAL: LEGOLAND 0x00832994
extern unsigned int DAT_00832994;
// GLOBAL: LEGOLAND 0x00832998
extern char DAT_00832998[256];
// GLOBAL: LEGOLAND 0x00832a98
extern char DAT_00832a98[256];
// GLOBAL: LEGOLAND 0x00832b9c
extern unsigned int DAT_00832b9c;
// GLOBAL: LEGOLAND 0x00832ba0
extern unsigned int DAT_00832ba0;
// GLOBAL: LEGOLAND 0x00832ba4
extern unsigned int DAT_00832ba4;
// GLOBAL: LEGOLAND 0x00832ba8
extern unsigned int DAT_00832ba8;
// GLOBAL: LEGOLAND 0x00832bd0
extern int DAT_00832bd0;
// GLOBAL: LEGOLAND 0x00832bd4
extern int DAT_00832bd4;
// GLOBAL: LEGOLAND 0x00832bd8
extern unsigned int DAT_00832bd8;
// GLOBAL: LEGOLAND 0x00832bdc
extern int DAT_00832bdc;
// GLOBAL: LEGOLAND 0x00832bf0
extern void *PathSprite;
