#pragma once

#include <windows.h>
#include <ddraw.h>

#include "gamemap.h"
#include "legoland.h"
#include "math.h"

struct Bloke;
struct BlokeSex0;
struct BlokeInfo {
    unsigned int sex;
    unsigned int leg;
    unsigned int arm;
    char chest[0x14];
    char face[0x14];
    struct BlokeSex0 *bloke;
};
struct RecBuf;

/* DirectInput stays private to the input subsystem (input.c) + its definitions
 * (globals.c) — both include <dinput.h> directly. globals.h only needs an opaque
 * handle to the one DInput-typed global below, so it does NOT drag <dinput.h>
 * across every TU that includes globals.h. */
struct _DIMOUSESTATE;
struct CtrlBuffer;

// ---------------------------------------------------------------------------
// Forward declarations for typed/instance globals declared below. Full struct
// definitions live in the owning translation units; these globals only need an
// incomplete type for their pointer/extern declarations. (struct Cursor's full
// layout comes from gamemap.h, used by the EditCursor / QueryCursor instances.)
// ---------------------------------------------------------------------------
struct Sprite;
struct NerpsListNode;
struct ObjectiveEvent;
/* One entry of the game's sound-effect list (GameFX), loaded by Load_FXList. */
struct FXItem {
    /* 0x00 */ char *name; /* file name under .\sfx\ */
    /* 0x04 */ unsigned char pad_4[0x8 - 0x4];
    /* 0x08 */ struct Sample *sample;
};
/* Indices into GameFX, named after the .wav each entry loads. */
enum GameFXIndex {
    FX_FLOWERS, /* Flowers.wav */
    FX_DRILL, /* RabOld\Drill.wav */
    FX_PUNCH, /* RabOld\Punch4.wav */
    FX_BUILDING1, /* Building 01.wav */
    FX_BUILDING2, /* Building 02.wav */
    FX_BUILDING3, /* Building 03.wav */
    FX_HAMMER, /* RabOld\Hammer.wav */
    FX_DRILLING, /* Drilling.wav */
    FX_BUILDING4, /* Building 04.wav */
    FX_CLICK1, /* Click01.wav */
    FX_CLICK2, /* Click02.wav */
    FX_CLICK3, /* Click03.wav */
    FX_BUTTON4, /* Button04.wav */
    FX_BUTTON14, /* Button14.wav */
    FX_RASP, /* Rasp1.wav */
    FX_GARDENER_LIFT, /* gardener lift up3.wav */
    FX_INVENTORY_IN, /* inventory slide away.wav */
    FX_INVENTORY_OUT, /* inventory slide out.wav */
    FX_MECHANIC_LIFT, /* mechanic lift2.wav */
    FX_THEME_CLICK, /* theme button click3.wav */
    FX_GARDENER_DROP, /* alroight then gardener put down.wav */
    FX_MECHANIC_DROP, /* tut tut for mechanic put down.wav */
    FX_WARNING, /* warning01.wav */
    FX_COUNT
};
struct FlumeDims {
    int field1;
    int field2;
};

struct FlumeShape {
    int count;
    struct FlumeDims *pts;
};

struct FlumeTemplate {
    int count;
    void *table;
};
struct BinVFile;
struct BalloonNode;
struct Ride;
struct Image;
struct IconNode;
struct Point;
struct DirNode;
struct JailCell;
struct FXSpriteList;
struct ResVolume;
struct RideQueueEntry;
struct ObjectiveEvent;
struct MapElement;
struct RenderItemNode;
struct WaterNode;

struct BrollyData;
struct BrollyNode;
struct SaveBlock;
struct JungleRide;
struct JungleCursor;
struct JungleFish;
struct JungleScore;
struct JungleObj;
struct JunglePath;
struct BoatRide;
struct RideNode;
struct PathNode;
struct BoatRideNode;
struct MermaidNode;
struct Ride;
struct CatapultNode;
struct Position;
struct RinData;
struct CopterNode;
struct GoldNode;
struct CarouselNode;
struct EarthNode;
struct JoustNode;
struct InterfaceIconNode;
struct IconNode;
struct InterfaceListNode;
struct MoviePool;
struct TrackElemPair;
struct PanelNode;
struct InterfaceEventNode;
struct InterfaceResearchNode;
struct InfoObjData;
struct Element;
struct SortNode;
struct SpaceTowerCar;

// Full definitions needed by typed globals defined in globals.c.
struct SpaceTowerLayout {
    /* 0x00 */ unsigned short field_0;
    /* 0x02 */ unsigned char pad_2[2];
    /* 0x04 */ struct AnimLayout *anim_layout;
};

struct SpaceTowerSeatData {
    /* 0x00 */ struct Point inner;
    /* 0x08 */ struct Point outer;
    /* 0x10 */ int direction;
};

/* An object under construction; on disk the class is saved as its LLIDB index. */
struct BuildObj {
    union {
        struct Ride *ride;
        int index;
    };
    TileId coords;
    unsigned short pad;
    int elapsed;
};
typedef struct BuildObj BuildObj;

typedef struct MapElement MapElement;

struct MapElement {
    /* 0x00 */ struct Element *field_0; /* class of the object on this tile */
    /* 0x04 */ union {
        struct {
            unsigned char field_4;
            unsigned char field_5;
        };
        TileId anchor; /* tile of the object's origin */
    };
    /* 0x06 */ TileId next; /* anchor tile of the next object on the map */
    /* 0x08 */ unsigned short field_8;
    /* 0x0a */ unsigned short field_a;
    /* 0x0c */ unsigned short flags;
    /* 0x0e */ unsigned short user_flags;
    /* 0x10 */ unsigned char field_10;
    /* 0x11 */ unsigned char durability_level;
    /* 0x12 */ unsigned short field_12;
};

struct FMat4 {
    float m[4][4];
};

struct LegoConfig {
    /* 0x00 */ unsigned short screen_width;
    /* 0x02 */ unsigned short screen_height;
    /* 0x04 */ unsigned short scroll_border_x;
    /* 0x06 */ unsigned short scroll_border_y;
    /* 0x08 */ unsigned short scroll_accel_x;
    /* 0x0a */ unsigned short scroll_accel_y;
    /* 0x0c */ unsigned short scroll_max_x;
    /* 0x0e */ unsigned short scroll_max_y;
    /* 0x10 */ unsigned short view_width;
    /* 0x12 */ unsigned short view_height;
    /* 0x14 */ unsigned short width;
    /* 0x16 */ unsigned short height;
    /* 0x18 */ unsigned short field_18;
    /* 0x1a */ unsigned short max_blokes;
    /* 0x1c */ unsigned char field_1c;
    /* 0x1d */ unsigned char pad_1d[1];
    /* 0x1e */ unsigned short field_1e;
    /* 0x20 */ unsigned short view_x;
    /* 0x22 */ unsigned short view_y;
    /* 0x24 */ unsigned short field_24;
    /* 0x26 */ unsigned char pad_26[2];
    /* 0x28 */ unsigned int level;
    /* 0x2c */ unsigned int field_2c;
    /* 0x30 */ unsigned int field_30;
    /* 0x34 */ unsigned int mechanics_enabled;
    /* 0x38 */ unsigned int gardeners_enabled;
    /* 0x3c */ int repair_orders_enabled;
    /* 0x40 */ unsigned int field_40;
};

struct ObjTableEntry {
    unsigned short key;
    unsigned short value;
};

struct ColorLutEntry {
    unsigned short shifted;
    unsigned short scaled;
    unsigned short value;
    unsigned short pad;
};

struct FreePlaySpriteSlot {
    struct Sprite *sprite0;
    struct Sprite *sprite1;
    unsigned char pad_8[0x14];
};

struct ProgressEntry {
    /* 0x00 */ const char *name0;
    /* 0x04 */ const char *name1;
    /* 0x08 */ int id;
    /* 0x0c */ int x;
    /* 0x10 */ int y;
    /* 0x14 */ struct Sprite *sprite0;
    /* 0x18 */ struct Sprite *sprite1;
};

/* 0x4beb80: progress-screen levels, then the tutorial-screen entries */
struct ProgressTables {
    /* 0x000 */ struct ProgressEntry levels[10];
    /* 0x118 */ int last_clicked;
    /* 0x11c */ int pad_11c;
    /* 0x120 */ struct ProgressEntry tutorials[5];
    /* 0x1ac */ unsigned char tail[0x14];
};

struct InfoTimedEntry {
    /* 0x00 */ unsigned int sample;
    /* 0x04 */ int interval;
    /* 0x08 */ int last_time;
};

/* Boating school: one straight step per direction (dx, dy per frame; ox, oy start offset). */
struct BoatDirStep {
    /* 0x00 */ int dx;
    /* 0x04 */ int dy;
    /* 0x08 */ int ox;
    /* 0x0c */ int oy;
};

/* Boating school: one quarter-circle turn (angle a0 -> a1 around cx, cy). */
struct BoatArc {
    /* 0x00 */ float a0;
    /* 0x04 */ float a1;
    /* 0x08 */ float cx;
    /* 0x0c */ float cy;
};

struct MapRect {
    /* 0x00 */ int x0;
    /* 0x04 */ int y0;
    /* 0x08 */ int x1;
    /* 0x0c */ int y1;
};
typedef struct MapRect MapRect;

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

/* A tile set: sprites plus per-tile-type callbacks. */
struct FXSpriteList {
    /* 0x00 */ int base;
    /* 0x04 */ int count;
    /* 0x08 */ struct Sprite **sprites;
    /* 0x0c */ int *sprite_ids;
    /* 0x10 */ int *data_10;
    /* 0x14 */ struct Element *data_14;
    /* 0x18 */ unsigned char (*get_rf_flags)(int x, int y);
    /* 0x1c */ void (*on_enter)(struct Point pos);
    /* 0x20 */ void (*on_leave)(int x, int y);
};
typedef struct FXSpriteList FXSpriteList;

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
    unsigned char pad_0[0x10]; /* the screen rectangle (a RectI) */
    /* 0x10 */ int visible; /* 0xf when the rectangle overlaps the castle view (ClipRectIToRect), else 0 */
    struct ListLink *prev;
    struct ListLink *next;
};

struct CastleFloatEnt {
    float a;
    float b;
    int c;
};

struct Int16Pair {
    short x;
    short y;
};

/* castle track-segment list node (0x60f914 table entries; also embedded in castle nodes) */
struct CastleTrackCell {
    /* 0x00 */ struct Footprint fp; /* a link of the castle cursor's footprint chain (CastleCoasterFootprint) */
    /* 0x14 */ struct Int16Pair xy;
    /* 0x18 */ unsigned int k;
    /* 0x1c */ float a;
    /* 0x20 */ float b;
};

/* 0x6102f8: three 0x58-byte ride-path objects (doubly linked via next/prev) */
/* a stretch of the castle track's curve: a piece's own (PathSeg.blob), one its class makes (method_24), or a
   template (CastleCurves) */
struct SegBlob {
    /* 0x00 */ unsigned int v[16];
    /* 0x40 */ float length; /* straight and slope stretches: the length of the xy line */
    /* 0x44 */ union {
        float start;
        unsigned int start_bits; /* passed on as raw bits by the curve integrators */
    };
    /* 0x48 */ union {
        float end;
        unsigned int end_bits;
    };
    /* 0x4c */ struct CurveVt *vt;
    /* 0x50 */ struct SegBlob *next;
    /* 0x54 */ struct SegBlob *prev;
};

/* 0x7fded4: newly unlocked objects shown by the "new objects" popup */
/* 0x7fea4c / 0x7febac: clip rectangle used by the software sprite renderer */
struct DrawClipOrigin {
    int top;
    int left;
};

struct DrawClipSize {
    int height;
    int width;
};

struct NewObjTable {
    /* 0x00 */ void *objs[20];
    /* 0x50 */ struct Sprite *sprites[20];
    /* 0xa0 */ unsigned int count;
    /* 0xa4 */ int current;
};

struct ProfileData {
    /* 0x00 */ char name[0x1e];
    /* 0x1e */ unsigned char name_len;
    /* 0x1f */ unsigned char pad_1f;
    /* 0x20 */ unsigned int age;
    /* 0x24 */ unsigned char field_24; /* checked by the script AGES command */
    /* 0x25 */ unsigned char gap_25[3];
    /* 0x28 */ unsigned int speech_volume;
    /* 0x2c */ unsigned int music_volume;
    /* 0x30 */ unsigned int fx_volume;
    /* 0x34 */ unsigned int field_34;
    /* 0x38 */ unsigned int field_38;
    /* 0x3c */ unsigned int field_3c;
    /* 0x40 */ unsigned short field_40;
    /* 0x42 */ unsigned char field_42;
    /* 0x43 */ unsigned char field_43[200];
    /* 0x10b */ unsigned char field_10b;
    /* 0x10c */ unsigned char gap_10c[4];
};

struct EditState {
    unsigned int unk0;
    unsigned int unk4;
    struct Ride *unk8; /* instance of the object being placed */
};

struct ScreenMode {
    unsigned int unk0;
    unsigned int unk4;
    unsigned int unk8;
};

struct ScreenState {
    /* 0x00 */ unsigned char gap_0[0x20];
    /* 0x20 */ unsigned int age;
    /* 0x24 */ int speech_volume;
    /* 0x28 */ int music_volume;
    /* 0x2c */ int fx_volume;
    /* 0x30 */ unsigned char flags[0x13];
    /* 0x43 */ unsigned char profile_slot;
    /* 0x44 */ unsigned char save_slot;
    /* 0x45 */ unsigned char field_45;
    /* 0x46 */ unsigned char field_46[0xca];
};

// ---------------------------------------------------------------------------
// All game globals, sorted by ascending address. Definitions (with any
// initializers) live in globals.c. This phase only collects the declarations;
// per-TU inline externs are migrated to use this header in a later phase.
// ---------------------------------------------------------------------------
/* Per object-type map statistics (0x2c bytes). The scan_* fields accumulate during DoMapAI's
   tile sweep and are latched into built/capacity/tiles/salvage when the sweep completes. */
struct MapAIClass {
    /* 0x00 */ int built;
    /* 0x04 */ int classes;
    /* 0x08 */ int capacity;
    /* 0x0c */ int tiles;
    /* 0x10 */ int salvage;
    /* 0x14 */ int percent;
    /* 0x18 */ int limit;
    /* 0x1c */ int scan_tiles;
    /* 0x20 */ int scan_built;
    /* 0x24 */ int scan_salvage;
    /* 0x28 */ int scan_capacity;
};

/* 0x832800..0x832bf0: map AI statistics (ResetMapAI clears all 0x3f0 bytes). */
struct MapStats {
    /* 0x000 */ unsigned int field_0;
    /* 0x004 */ int scan_x;
    /* 0x008 */ int scan_y;
    /* 0x00c */ int scan_stage;
    /* 0x010 */ struct MapAIClass classes[6];
    /* 0x118 */ int total_tiles;
    /* 0x11c */ unsigned int capacity;
    /* 0x120 */ unsigned int capacity_max;
    /* 0x124 */ unsigned int capacity_min;
    /* 0x128 */ int mood_threshold0;
    /* 0x12c */ int mood_threshold1;
    /* 0x130 */ int mood_threshold2;
    /* 0x134 */ int mood_threshold3;
    /* 0x138 */ int mood_threshold4;
    /* 0x13c */ int mood_delta[13];
    /* 0x170 */ int entrance_fee;
    /* 0x174 */ unsigned int brick_meter_max;
    /* 0x178 */ unsigned int timer_minutes;
    /* 0x17c */ int appraisal_fail_limit;
    /* 0x180 */ unsigned int ride_wear;
    /* 0x184 */ unsigned int field_184;
    /* 0x188 */ unsigned int field_188;
    /* 0x18c */ unsigned int power_enabled; /* script feature 4 (SetScriptFeature): power supply and demand are simulated and shown */
    /* 0x190 */ unsigned int field_190;
    /* 0x194 */ unsigned int show_capacity;
    /* 0x198 */ char field_198[256];
    /* 0x298 */ char field_298[256];
    unsigned char pad_398[0x4];
    /* 0x39c */ int appraisal_streak;
    /* 0x3a0 */ unsigned int level_end;
    /* 0x3a4 */ unsigned int script_stopped;
    /* 0x3a8 */ unsigned int field_3a8;
    /* 0x3ac */ unsigned int field_3ac;
    /* 0x3b0 */ char leave_ratings[0x19];
    /* 0x3c9 */ signed char leave_rating_index;
    unsigned char pad_3ca[0x2];
    /* 0x3cc */ int power_spare_percent;
    /* 0x3d0 */ int power_supply;
    /* 0x3d4 */ int power_demand;
    /* 0x3d8 */ unsigned int unpowered_demand;
    /* 0x3dc */ int unpowered_count;
    /* 0x3e0 */ int bridge_open[4]; /* set by OpenBridge; DrawMapTiles draws the bridge overlays */
};

struct FortRect {
    int left;
    int top;
    int right;
    int bottom;
};

// 0x004ab5f0
extern GUID IID_IDirectMusicComposer;
// 0x004ab640
extern GUID IID_IDirectMusicPerformance;
// 0x004ab6a0
extern GUID IID_IDirectMusicLoader;
// 0x004ab7b0
extern GUID GUID_Download;
// 0x004ab880
extern GUID GUID_NOTIFICATION_MEASUREANDBEAT;
// 0x004ab8a0
extern GUID GUID_NOTIFICATION_SEGMENT;
// 0x004ab900
extern GUID CLSID_DirectMusicLoader;
// 0x004ab920
extern GUID CLSID_DirectMusicComposer;
// 0x004aba00
extern GUID CLSID_DirectMusicPerformance;
// 0x004acfd0
extern GUID NullGuid;
// 0x004b4580
extern struct FortRect FortWanderRect;

// 0x004ab390
extern float FLOAT_004ab390;
extern float FLOAT_004ab468;
// 0x004ab398
extern double DOUBLE_004ab398;
// 0x004ab3dc
extern float Float2Pi;
// 0x004ab3e0
extern float FloatNegInv360;
// 0x004ab3e4
extern float Float45;
// 0x004ab3e8
extern float BoatHeadingStepDegrees;
// 0x004ab3ec
extern float Float180;
// 0x004ab3f0
extern float BoatArcScale;
// 0x004ab3f4
extern float DegToRad;
// 0x004ab3f8
extern float BoatArcStepScale;
// 0x004ab3fc
extern float Float16;
// 0x004ab478
extern double DOUBLE_004ab478;
// 0x004ab488
extern double DOUBLE_004ab488;
// 0x004ab4a0
extern float Float196;
// 0x004ab4a8
extern double JungleCruiseSeatRadiusY;
// 0x004ab4b0
extern double JungleCruiseSeatRadiusX;
// 0x004ab4b8
extern float FLOAT_004ab4b8;
// 0x004ab430
extern float TrainCarMass;
// 0x004ab43c
extern float FLOAT_004ab43c;
// 0x004ab418
extern double CastleEpsilon;
// 0x004ab424
extern float FLOAT_004ab424;
// 0x004ab428
extern float FLOAT_004ab428;
// 0x004ab42c
extern float FLOAT_004ab42c;
// 0x004ab444
extern float FLOAT_004ab444;
// 0x004ab550
extern float FLOAT_004ab550;
// 0x004b4860
extern struct Point LogFlumeDropDrawOffset;

// 0x004c2b00
extern struct FlumeShape FlumeStraightShape1;

// 0x004c2b30
extern struct FlumeDims FlumeCornerShape1Points[4];

// 0x004c2b58
extern struct FlumeShape FlumeStraightShape0;

// 0x004c2b78
extern struct FlumeDims FlumeCornerShape3Points[4];

// 0x004c2bc0
extern struct FlumeShape FlumeCornerShape1;

// 0x004c2bc8
extern struct FlumeDims FlumeCornerShape2Points[4];

// 0x004c2be8
extern struct FlumeShape FlumeCornerShape0;

// 0x004c2c08
extern struct FlumeShape FlumeCornerShape3;

// 0x004c2c10
extern struct FlumeShape FlumeCornerShape2;

// 0x004c8d58
extern struct FlumeDims FlumeStraightShape1Points[2];

// 0x004cbde8
extern struct FlumeDims FlumeCornerShape0Points[4];

// 0x004cbe38
extern struct FlumeDims FlumeStraightShape0Points[2];

// 0x0066b638
extern struct ColorLutEntry ColorLut[256];
// 0x004ab44c
extern float FLOAT_004ab44c;
// 0x004ab454
extern float FLOAT_004ab454;
// 0x004ab458
extern float FLOAT_004ab458;
// 0x004ab45c
extern float FLOAT_004ab45c;
// 0x004ab4bc
extern float RidePointOriginY;
// 0x004ab4c0
extern float RidePointOriginX;
// 0x004ab4c8
extern double DoubleZero;
// 0x004ab518
extern float FloatOnePercent;
// 0x004ab528
extern float FloatMsPerSecond;
// 0x004ab52c
extern float FLOAT_004ab52c;
// 0x004ab530
extern double DOUBLE_004ab530;
// 0x004ab538
extern double DOUBLE_004ab538;
// 0x004ab558
extern double DOUBLE_004ab558;
// 0x004ab560 c_dfDIKeyboard, 0x004ab578 c_dfDIMouse — declared in <dinput.h>, defined in globals.c
// 0x004ab5e0
extern GUID IID_IDirectMusicBand;
// 0x004ab600
extern GUID IID_IDirectMusicChordMap;
// 0x004ab610
extern GUID IID_IDirectMusicStyle;
// 0x004ab670
extern GUID IID_IDirectMusicSegment;
// 0x004ab6d0
extern GUID GUID_PerfMasterGrooveLevel;
// 0x004ab8b0
extern GUID GUID_DirectMusicAllTypes;
// 0x004ab8e0
extern GUID DirectMusicBandClassGuid;
// 0x004ab930
extern GUID DirectMusicChordMapClassGuid;
// 0x004ab980
extern GUID CLSID_DirectMusicStyle;
// 0x004ab9f0
extern GUID CLSID_DirectMusicSegment;
// 0x004ac090 GUID_SysKeyboard, 0x004ac0a0 GUID_SysMouse — declared in <dinput.h>, defined in globals.c
// 0x004acf80
extern GUID IID_IDirectDraw2_Guid;
// 0x004b4034
extern int Heading16ToDir8[16];
// 0x004b40a4
extern unsigned int CatapultSlotLayers[4];
extern int CatapultSlotOffsetsY[4];
// 0x004b40c8
extern struct FXItem Catapult_SFX[4]; /* their file names follow */
// 0x004b4140
extern struct FXItem Helicopter_SFX[4]; /* their file names follow */
// 0x004b43f8
extern struct FXItem DRIVING_SCHOOL_SFX[6]; /* their file names follow */
// 0x004b4440
extern unsigned int DrivingSchoolPart1Footprint[5];
// 0x004b4458
extern unsigned int DrivingSchoolPart2Footprint[5];
// 0x004b4470
extern unsigned int DrivingSchoolPart3Footprint[5];
// 0x004b4688
extern struct FXItem JOUST_SFX[1];
// 0x004b4728 (20 bytes: copied whole into cursors and rides, including the NULL next)
extern struct Footprint LogFlumeFootprint;
// 0x004b473c
extern int FlumeTrackMode3Sprites[4];
// 0x004b474c
extern int FlumeTrackMode1Sprites[2];
// 0x004b4754
extern int FlumeTrackMode2Sprites[4];
// 0x004b4764
extern int FlumeTrackMode4Sprite;
// 0x004b47f8
extern struct Sprite *LogFlumeFc1M1StageSprite;
// 0x004b4804
extern struct Sprite *LogFlumeFc1M2StageSprite;
// 0x004b4808
extern struct FlumeTemplate LogFlumeSpecialCorner1Stages;
// 0x004b4818
extern struct Sprite *LogFlumeFc3M1StageSprite;
// 0x004b4824
extern struct Sprite *LogFlumeFc3M2StageSprite;
// 0x004b4828
extern struct FlumeTemplate LogFlumeSpecialCorner3Stages;
// 0x004b4838
extern struct Sprite *LogFlumeHup1M1StageSprite;
// 0x004b4850
extern struct Sprite *LogFlumeHup1M2StageSprite;
// 0x004b4858
extern struct FlumeTemplate LogFlumeHoldUpStages;
// 0x004b4bd0
extern unsigned char DrivingSchoolPumpsFootprint[0x14];
// 0x004b4bf0
extern unsigned int RoadPieceFootprint[5];
// 0x004b4c04
extern int TrafficLightTimer;
// 0x004b4cb8
extern struct FXItem SAFARI_SFX[1];
// 0x004b4cc4
extern int SafariRideFrameThreshold1[8];
// 0x004b4ce4
extern int SafariRideFrameThreshold2[8];
// 0x004b4d88
extern struct FXItem SpiderRide_SFX[1];
// 0x004b4d94
struct SpiderBnv {
    char name[8];
    int tab1[16];
    int tab2[17];
};
extern struct SpiderBnv SpiderBnvInfo;
// 0x004b4e20
extern Point SpiderRideDrawAdjust;
// 0x004b78b4
extern char BoxBlokeBnvName[];
// 0x004b4f08
extern char *TempleSlidePathNames[4];
// 0x004b4f18
extern signed char TempleSlideStartFrames[4];
// 0x004b4f1c
extern signed char TempleSlideEndFrames[4];
// 0x004b4fa8
extern struct FXItem WATERWORKS_SFX[3];
// 0x004b5118
extern struct BoatDirStep BoatingSchoolDirSteps[4];
// 0x004b5158
extern struct BoatArc BoatingSchoolArcsAlt[4];
// 0x004b5198
extern struct BoatArc BoatingSchoolArcs[4];
// 0x004b51d8
extern int BoatingSchoolRiderOffsets[0x80];
// 0x004b5260
extern struct Footprint BoatingSchoolExtraFootprintTemplate;
// 0x004b5278
extern struct Footprint BoatingSchoolStartFootprintTemplate;
// 0x004b5290
extern struct Point BoatingSchoolQueueOffsets[6];
// 0x004b52c0
extern struct FXItem PTR_s_Boat_Noise_wav[2];
// 0x004b53c0
extern struct Footprint BoatingSchoolWaterFootprint;
// 0x004b53d4
extern unsigned char BoatPathTilePatterns[0x190];
// 0x004b5570
extern struct AreaNode DoorAreaList;
// 0x004b5584
extern short PieceNeighbourOffsets[4][2];
// 0x004b55f4
extern unsigned int MapObjectsEnabled;
// 0x004b5608
extern unsigned int ClipVertexStride;
// 0x004b560c
extern int ClipVertexLastField;
// 0x004b5b20
extern void *CoasterSurfaceBits;
// 0x004b5b24
extern unsigned short *CoasterDepthBuffer;
// 0x004b5b28
extern int CoasterSurfacePitch;
// 0x004b59e8
extern char SitLoGirlSitName[];
// 0x004b59f8
extern char SitLoManSitName[];
// 0x004b5abc
extern float HermiteBasis[4][4];
// 0x004b5b3c
extern struct RecBuf *DepthSpanRecordEnd;
// 0x004b5988
extern struct BlokeInfo CastleVisitorInfo;
// 0x004b5c1c
extern struct FMat4 CastleIsoProjections[2];
// 0x004b5c9c
extern unsigned int CastleShadeMax;
// 0x004b5ca0
extern unsigned int CastleLightDir;
// 0x004b5ca4
extern unsigned int CastleLightDirY;
// 0x004b5ca8
extern unsigned int CastleLightDirZ;
// 0x004b5cac
extern float TrackLightingBase;
// 0x004b5cb0
extern unsigned int TrackLightDir;
// 0x004b5cb4
extern unsigned int TrackLightDirY;
// 0x004b5cb8
extern unsigned int TrackLightDirZ;
// 0x004b5cbc
extern float ModelLightingBase;
// 0x004b5cc0
extern unsigned int ModelLightDir;
// 0x004b5cc4
extern unsigned int ModelLightDirY;
// 0x004b5cc8
extern unsigned int ModelLightDirZ;
// 0x004b5d20
extern char RollerCoasterSavePath[32];
extern struct PieceClass SquareTrackPieceClass;

// 0x004b5d58
extern struct PieceClass HeightTrack25PieceClass;

// 0x004b5d90
extern struct PieceClass HeightTrack2PieceClass;

// 0x004b5dc8
extern struct PieceClass HeightTrackPathPieceClass;
// 0x004b5e00
extern float TrackSideMidpointsUnit[4][3];
// 0x004b5e30
extern float TrackRailOffsetsUnit[4][3];
// 0x004b5e60
extern float TrackArcTangentsUnit[4][3];
// 0x004b5e90
extern float TrackArcCentresUnit[4][3];
// 0x004b5ec0
extern int TrackStraightStartSides[4];
// 0x004b5ee0
extern int TrackStraightHeightSteps[5];
// 0x004b5ef4
extern unsigned int TrackArcTangentPairs[4][2];
// 0x004b5f14
extern unsigned int TrackArcRadii[4][2];
// 0x004b61e0
extern struct CastleFloatEnt TrackTieShape[8];

// 0x004b62f0
extern unsigned int TrackTieLmsRec[4];
// 0x004b6300
extern struct LmsFile TrackTieModel; /* the track sleeper: 8 vertices (TrackTieVerts, InitTrackTieShape), 8 flat triangles */
// 0x004b6398
extern float CoasterCellEdgeMidpoints[4][3];
// 0x004b63c8
extern float SlopeRunDirections[4][3];
// 0x004b6408
extern int WheelCurvePairs[3];
// 0x004ab490
extern float FLOAT_004ab490;
// 0x004ab494
extern float FLOAT_004ab494;
// 0x004ab49c
extern float FLOAT_004ab49c;
// 0x004b64cc
extern char CarouselBlokeBoxName[12]; /* "BlokeBox??", the seat number goes at +8 */
// 0x004b64d8
extern struct FXItem CAROUSSEL_SFX[2];
// 0x004b65c0
extern int EarthSlideQueueOffsets[8];
// 0x004b6638
extern char CurrentDirPath[4];
// 0x004b6654
extern int ENTRANCE_DEST_RIGHT[2];
// 0x004b665c
extern int ENTRANCE_DEST_LEFT[2];
// 0x004b6668
extern struct FXItem ENTRANCE_SFX[1]; /* its file name follows */
// 0x004b66e8
extern struct FXItem RESTAURANT_SFX[1];
// 0x004b66f4
extern int Restaurant1CustomerPath[15 * 6];
// 0x004b685c
extern int Restaurant2DownStepsBase;
// 0x004b6860
extern int Restaurant2DownSteps[0x20];
// 0x004b68e0
extern int Restaurant2UpSteps[0x22];
// 0x004b6968
extern struct FXItem OCTOPUS_SFX[3];
// 0x004b6990
extern int OctopusCafeSpotOffsets[0x29];
// 0x004b6a34
extern int OctopusCafeEntryPath[4];
// 0x004b6a44
extern int OctopusCafeSpotIndices[0xd];
// 0x004b6a78
extern int OctopusCafeExitPath[0x30];
// 0x004b6b38
extern int OctopusCafeChairOffsets[0x2c];
// 0x004b6be8
extern int OctopusCafeChairIndices[0x5c];
// 0x004b6d58
extern unsigned char OctopusCafeDepthMap[0x120];
// 0x004b7260
extern struct Footprint JungleCruiseExtraFootprint;
// 0x004b7278
extern struct Footprint JungleCruiseStartFootprint;
// 0x004b7288
// 0x004b7148
extern struct BoatDirStep JungleCruiseDirSteps[4];
// 0x004b7188
extern struct BoatArc JungleCruiseArcs[4];
// 0x004b71c8
extern struct BoatArc JungleCruiseArcsAlt[4];
// 0x004b7290
extern struct Point JungleCruiseQueueOffsets[5];
// 0x004b7230
extern struct Footprint JungleCruiseExtraFootprintTemplate;
// 0x004b7248
extern struct Footprint JungleCruiseStartFootprintTemplate;
// 0x004b72e4
extern unsigned char JunglePathTilePatterns[0x190];
// 0x004b7478
extern struct Footprint JungleCruiseDefaultFootprint;
// 0x004b7618
extern struct FXItem SPACE_TOWER_SFX[1];
// 0x004b76b8
extern struct AnimLayout SpaceTowerFrontSeatLayout;
// 0x004b7750
extern struct AnimLayout SpaceTowerBackSeatLayout;
// 0x004b7758
extern struct SpaceTowerLayout SpaceTowerLayouts[8];
// 0x004b7798
extern struct SpaceTowerSeatData SpaceTowerSeats[4];
// 0x004b77e8
extern struct Point SpaceTowerSeatOffsets[8];
// 0x004b79d0
extern char PlaneRideManboxName[];
extern struct FXItem PlaneRideFX[2];
// 0x004b7abc
extern unsigned short PersonRenderFpuControlWord;
// 0x004b7ac0
extern unsigned char BlokeColours[0x18];
// 0x004b7ae0
extern int RideMatrixRowOrder[3];
// 0x004b7aec
extern int RideMatrixColOrder[3];
// 0x004b7af8
extern int RideMatrixColSign[3];
// 0x004b7b04
extern int RideMatrixRowSign[3];
// 0x004b7b10
extern const char Path3DFormat[];
// 0x004b7d70
extern unsigned int AdvisorFrameFormat;
// 0x004b7d74
extern unsigned int AdvisorFrameWidth;
// 0x004b7d78
extern unsigned int AdvisorFrameHeight;
// 0x004b7d7e
extern unsigned short AdvisorFrameBitCount;
// 0x004b7d84
extern unsigned int AdvisorFrameSizeImage;
// 0x004b7e9c
extern unsigned int RidePartClassNames;
// 0x004b81c0
extern char *GraphicsPath;
// 0x004b81c4
extern char *GraphicsPathPrefix;
// 0x004b81c8
extern char *GraphicsSmallPath;
// 0x004b81cc
extern char *MasksPath;
// 0x004b81d0
extern char *MasksSmallPath;
// 0x004b81d4
extern char *IconsPath;
// 0x004b81d8
extern char *ModelsPath;
// 0x004b8318
extern struct Point ParkEntranceWalkInOffset;
// 0x004b8320
extern struct Point ParkExitPos;
// 0x004b8328
extern struct Point ParkExitStep;
// 0x004b8334
extern int BlokeTirednessThresholds[4];
// 0x004b8344
extern char NextBlokeLetter;
// 0x004b8348
extern char *BlokeAiLogLinePtrs[8];
// 0x004b8368
extern void (*BlokeLongTermActionTable[26])(struct Bloke *);
// Persistent Windows 95 VWIN32 device handle used by the CD ejection path.
// 0x004b85c4
extern HANDLE VWin32DeviceHandle;
// 0x004b8710
extern struct FXItem FountainSFX[5]; /* their file names follow */
// 0x004b8750
extern struct FXItem PowerStationSFX[2];
// 0x004b8768
extern struct FXItem DINO_SFX[5];
// 0x004b87a8
extern struct FXItem MONEY_SFX[2];
// 0x004b8bbc
extern unsigned char PercentSFormat[4]; /* "%s" */
// 0x004b90f8
extern unsigned int BrickCount;
// 0x004b90fc
extern unsigned int UnlimitedBricks;
// 0x004b9210
extern int Bridge1SpriteOffsetX;
// 0x004b9214
extern int Bridge1SpriteOffsetY;
// 0x004b9218
extern int Bridge0SpriteOffsetX;
// 0x004b921c
extern int Bridge0SpriteOffsetY;
// 0x004b9220
extern LEGO_EXPORT unsigned int BGFullUpdate;
// 0x004b9228
extern struct FXItem GameFX[FX_COUNT];
// 0x004b95f0
extern struct DeferredSprite *DeferredSpriteCursor;
// 0x004b9550
extern unsigned char DirectionBits[8];
// 0x004b9558
extern unsigned int PathSquare3x3Masks[9];
// 0x004b957c
extern int PathSquare3x3OffsetsX[9];
// 0x004b95a0
extern int PathSquare3x3OffsetsY[9];
// 0x004b95c4
extern unsigned char CursorCyanColours[8];
// 0x004b95cc
extern unsigned char CursorBlueColours[8];
// 0x004b95d4
extern unsigned char CursorGreenColours[8];
// 0x004b95dc
extern unsigned char CursorRedColours[8];
// 0x004b95e8
extern int UnusedViewMapTileX;
// 0x004b95ec
extern int UnusedViewMapTileY;
// 0x004b95f4
extern int ScrollMarginTopRight;
// 0x004b95f8
extern int ScrollMarginBottomLeft;
// 0x004b95fc
extern int ScrollMarginTopLeft;
// 0x004b9600
extern int ScrollMarginBottomRight;
// 0x004b9604
extern int ScrollMaxX;
// 0x004b9608
extern int ScrollMarginLeft;
// 0x004b960c
extern int ScrollMarginTop;
// 0x004b9610
extern int ScrollMarginBottom;
// 0x004b9ca4
extern int (*BlitFrameFunc)(void);
// 0x004b9ca8
extern unsigned int OverrideFrame;
// 0x004b9d0c
extern const char BltFastNoSpriteFmt[];
// 0x004b9e5c
extern unsigned int ScriptEventCategories[71];
// 0x004b9f78
extern int AdvisorHelpRect[4];
// 0x004b9f88
extern unsigned int HelpChanged;
// 0x004b9f8c
extern UINT_PTR CurrentHelpKey; /* the speech playing: a text number or a name */
// 0x004ba87c
extern void *IconListTail;
// 0x004ba880
extern void *HelpIconListTail;
// 0x004ba884
extern unsigned int UnaffordableIconColour;
// 0x004ba8e0
extern struct InfoTimedEntry AdviceSpeechEntries[17];
// 0x004ba9ac
extern unsigned int AdviceSpeechIds[234];
// 0x004bad54
extern int mouse_granularity;
struct KeyMapping {
    /* 0x00 */ unsigned char flags;
    /* 0x01 */ signed char code;
};
// 0x004bad58
extern struct KeyMapping CheatKeyMap[0x3c];
// 0x004bafa8
extern unsigned int BuildThemeNames[20];
// 0x004baff8
extern unsigned int SelectedBuildTheme;
// 0x004baffc
extern char BuildCategoryNames[4][0x14];
// 0x004bb094
extern int InterfaceIconPos[18];
extern unsigned int LegolandThemeDeselected;
// 0x004bb098
extern unsigned int CastleThemeDeselected;
// 0x004bb09c
extern unsigned int WesternThemeDeselected;
// 0x004bb0a0
extern unsigned int AdventurersThemeDeselected;
// 0x004bb0a4
extern struct TrackElemPair BuildElementNameMap[29];
// 0x004bb18c
extern unsigned int InterfaceSlotValues[4];
// 0x004bb4dc
extern float AviAudioVolume;
// 0x004bb4e0
extern BITMAPINFOHEADER AviFrameFormat;
// 0x004bb588
// 0x004bb58c
// 0x004bb598
extern struct Point AStarStartPoint;
// 0x004bb5a0
extern int AStarGoalX;
// 0x004bb5a4
extern int AStarGoalY;
// 0x004bb5ac
extern unsigned int ScriptObjectiveKind;
// 0x004bb5b0
extern unsigned int ScriptConditionActive;
// 0x004bb5b4
extern unsigned int ScriptZoningNames;
// 0x004bb5c4
extern unsigned int ScriptThemeNames;
// 0x004bb5d8
extern unsigned int ScriptTabNames;
// 0x004bb5e0
extern unsigned int ScriptModeNames;
// 0x004bb5f4
extern unsigned int ScriptFeatureNames;
// 0x004bb624
extern unsigned int ScriptReportNames;
// 0x004bb688
extern unsigned int ScriptHapFactorNames;
// 0x004bb6bc
extern unsigned int ScriptCapacityClassNames;
// 0x004bb6d4
extern unsigned int ScriptFlashButtonNames;
// 0x004bb6f8
extern struct ScriptCommand LevelScriptCommands[0x5d];
// 0x004bcba4
extern const char *ResourceFileNames[3];
// 0x004bcbf4
extern LEGO_EXPORT struct LegoConfig *lpConfig;
// 0x004bcecc
extern char *PTR_s_Aaron_004bcecc[0x53];
// 0x004bd018
extern char *PTR_s_Abbie_004bd018[0x5a];
// 0x004bd180
extern char *PTR_s_Adams_004bd180[0x6b];
// 0x004bd32c
extern short BlokeDirVectors[8][2]; /* unit step (x, y) per direction, 8.8 fixed point */
// 0x004bd34c
extern void (*BlokeActionTable[16])(struct Bloke *);
// 0x004bdd00
extern struct HoverInfo Hover;
// 0x004bdea0
extern LEGO_EXPORT RECT SPRITE_ClipRect;
// 0x004beb80
extern struct ProgressTables ProgressScreenTables;
// 0x004bed40
extern unsigned int TutorialEndSprite0;
// 0x004bed44
extern unsigned int TutorialEndSprite1;
// 0x004bef9c
extern unsigned int FrontEndInputEnabled;
// 0x004bf670
extern unsigned int IntervalPageFirstLine;
// 0x004bf774
extern unsigned int MusicEnabled;
// 0x004bf778
extern unsigned int MusicState;
// 0x004bff28
extern const int RepairSearchOffsets[12][2];
// 0x004c10d4
extern struct NewBloke *DrivingSchoolCars;
// 0x004c10dc
extern struct Ride *CastleLevelRide;
// 0x004c10e4
extern struct Sprite *CastleLevelMatteSprite;
// 0x004c10e8
extern struct RenderItemNode *CastleLevelRiderRenderList;
// 0x004c10f0
extern void *CatapultLayer;
// 0x004c10f4
extern struct Ride *ActiveCatapultRide;
// 0x004c1100
extern struct RideSpriteInfo CatapultSpriteInfo;
// 0x004c1118
extern struct CatapultNode *CatapultNodeList;
// 0x004c1120
extern struct Sprite *CopterBaseMatteSprite;
// 0x004c1124
extern struct QueueTable *CopterQueueTables[5];
// 0x004c1138
extern void *CopterModelLayers;
// 0x004c113c
extern struct Sprite *CopterModelSprites[10];
// 0x004c1164
extern struct RenderItemNode **CoptersRenderList2;
// 0x004c1168
extern struct RenderItemNode **CoptersRenderList3;
// 0x004c1170
extern struct RideSpriteInfo CoptersSpriteInfo;
// 0x004c1188
extern struct RenderItemNode **CoptersRenderList4;
// 0x004c1190
extern struct RenderItemNode **CoptersRenderList1;
// 0x004c1194
extern struct RenderItemNode **CoptersRenderList0;
// 0x004c1198
extern struct Ride *ActiveCopterRide;
// 0x004c119c
extern struct RenderItemNode *CoptersRenderListHead0;
// 0x004c11a0
extern struct RenderItemNode *CoptersRenderListHead1;
// 0x004c11a4
extern struct RenderItemNode *CoptersRenderListHead2;
// 0x004c11a8
extern struct RenderItemNode *CoptersRenderListHead3;
// 0x004c11ac
extern struct RenderItemNode *CoptersRenderListHead4;
// 0x004c11b0
extern struct RenderItemNode *CoptersRenderListHead5;
// 0x004c11b4
extern struct CopterNode *CopterNodeList;
// 0x004c11bc
extern struct CountNode *DrivingSchoolCountList;
// 0x004c11c0
extern int DrivingSchoolDriveOnLeft;
// 0x004c11c4
extern struct RideQueueEntry *RoadBfsFrontier;
// 0x004c11c8
extern struct RideQueueEntry *RoadBfsNextFrontier;
// 0x004c11cc
extern struct Sprite *FortMaskSprite;
// 0x004c11d8
extern struct Sprite *FortLayer;
// 0x004c11dc
extern struct Ride *FortRide;
// 0x004c11e0
extern struct RenderItemNode *FortRiderRenderList;
// 0x004c11e4
extern struct Sprite *GoldRushQueueTable;
// 0x004c11e8
extern struct Sprite *GoldRushLayer;
// 0x004c11f0
extern void *GoldRushRide;
// 0x004c11f4
extern struct Sprite *GoldMaskSprite;
// 0x004c11f8
extern struct Sprite *GoldWashMatte1Sprite;
// 0x004c11fc
extern struct Sprite *GoldWashSprite;
// 0x004c1200
extern struct Sprite *GoldWashMatte2Sprite;
// 0x004c1204
extern struct GoldNode *GoldWashList;
// 0x004c1208
extern struct RenderItemNode *GoldRushBlokeRenderList;
// 0x004c1210
extern struct Sprite *ZJoustSprite;
// 0x004c1214
extern struct Sprite *JoustLayer;
// 0x004c1218
extern void *JoustRideBnv;
// 0x004c121c
extern struct Ride *JoustRide;
// 0x004c1228
extern struct RideSpriteInfo JoustSpriteInfo;
// 0x004c123c
extern void *JoustRiderSprites[2];
// 0x004c1244
extern struct Sprite *JoustFMaskSprite;
// 0x004c1248
extern struct Sprite *JoustSpecRMSprite;
// 0x004c124c
extern struct Sprite *JoustSpecLMSprite;
// 0x004c1250
extern struct JoustNode *JoustNodeList;
// 0x004c1258
extern struct Sprite *LogFlumeTunelMSprite;
// 0x004c1260
extern struct Cursor FlumeNeighbourCursor2;
// 0x004c2a94
extern struct Sprite *LogFlumeHup1M1Sprite;
// 0x004c2a98
extern struct Sprite *LogFlumeHup1M2Sprite;
// 0x004c2aa8
extern struct Footprint LogFlumeEntrancePieceFootprint;
// 0x004c2aa0
extern struct Ride *LogFlumeSpecialCorner2Ride;
// 0x004c2abc
extern struct Sprite *LogFlumeTrackSprites[10];
// 0x004c2ae4
extern struct Sprite *LogFlumeTrackSpritesEnd;
// 0x004c2ae8
extern struct QueueTable *FlumeQueueTable0;
// 0x004c2aec
extern struct Sprite *LogFlumeDrop2MSprite;
// 0x004c2af0
extern struct Sprite *LogFlumeDrop1MSprite;
// 0x004c2af4
extern unsigned int FlumeSpecialCornerIndex;
// 0x004c2af8
extern struct QueueTable *FlumeQueueTable1;
// 0x004c2afc
extern struct Element *LogFlumeImageListId;
// 0x004c2b0c
extern struct Ride *LogFlumeSpecialCorner3Ride;
// 0x004c2b10
extern struct Element *LogFlumeTrackEndyListId;
// 0x004c2b50
extern struct Element *LogFlumeHoldUpElement;
// 0x004c2b60
extern struct Ride *LogFlumeHoldUpRide;
// 0x004c2b64
extern struct Sprite *LogFlumeSplashSprite;
// 0x004c2b68
extern struct SpriteSet *LogFlumeImageList;
// 0x004c2b6c
extern struct Sprite *LogFlumeFc1M1Sprite;
// 0x004c2b70
extern struct Sprite *LogFlumeFc1M2Sprite;
// 0x004c2b98
extern struct Element *LogFlumeSpecialCorner1Element;
// 0x004c2b9c
extern struct Ride *LogFlumeEntranceRide;
// 0x004c2ba0
extern struct Element *LogFlumeSpecialCorner4Element;
// 0x004c2c18
extern struct Cursor LogFlumeEntranceCursor2;
// 0x004c2bf0
extern struct Ride *LogFlumeCsaw2Ride;
// 0x004c445c
extern struct Ride *LogFlumeSpecialCorner1Ride;
// 0x004c4460
extern struct Element *LogFlumeCsaw2Element;
// 0x004c5c90
// 0x004c74c8
// 0x004c74f4
extern struct Element *LogFlumeTrackElement;
// 0x004c74f8
extern struct Cursor LogFlumeCursor;
// 0x004c4468
extern struct Cursor FlumeConnectionCursor1;
// 0x004c5ca0
extern struct Cursor FlumeConnectionCursor2;
// 0x004c74d4
extern struct Ride *LogFlumeSpecialCorner4Ride;
// 0x004c74d8
extern struct RideSpriteInfo LogFlumeTrackSpriteInfo;
// 0x004c8d2c
extern struct Sprite *LogFlumeFc2M1Sprite;
// 0x004c8d38
extern struct Footprint LogFlumeTrackPieceFootprint;
// 0x004c8d4c
extern struct Element *LogFlumeDropElement;
// 0x004c8d50
extern struct Element *LogFlumeSpecialCorner3Element;
// 0x004c8d54
extern struct Sprite *LogFlumeEntranceLayer;
// 0x004c8d68
extern struct Sprite *LogFlumeFc3M3Sprite;
// 0x004c8d78
extern struct Cursor LogFlumeEntranceCursor;
// 0x004c8d6c
extern struct Ride *LogFlumeDropRide;
// 0x004c8d70
extern struct Sprite *LogFlumeFc4MSprite;
extern struct RenderItemNode *FlumeCornerBarrelsLowWeight;
extern struct RenderItemNode *FlumeCornerBarrelsHighWeight;
// 0x004ca5b0
extern struct Cursor FlumeNeighbourCursor1;
// 0x004cbe08
extern struct Sprite *LogFlumeFc3M2Sprite;
// 0x004cbe0c
extern struct Sprite *LogFlumeFc3M1Sprite;
// 0x004cbe10
extern struct Element *LogFlumeSpecialCorner2Element;
// 0x004cbe14
extern struct Sprite *LogFlumeCsaw2MSprite;
// 0x004cbe18
extern struct Ride *LogFlumeTunnelRide;
// 0x004cbe1c
extern struct Sprite *LogFlumeFc1M3Sprite;
// 0x004cbe20
extern struct FlumeEntry *FlumeNeighbours[4]; /* the 4 neighbours of a log flume tile (up, right, down, left) */
// 0x004cbe30
extern struct Ride *LogFlumeTrackRide;
// 0x004cbe48
extern struct Element *LogFlumeTunnelElement;
// 0x004cbe4c
extern struct Sprite *LogFlumeEnta3MSprite;
// 0x004cbe70
extern struct RenderItemNode *FlumeRiderRenderList;
// 0x004cbe74
extern struct Sprite *LogFlumeBarrelSprite;
// 0x004cbe78
extern struct Sprite *LogFlumeBarrelMSprite;
// 0x004cbe7c
extern struct Sprite *LogFlumeBarrel1Sprite;
// 0x004cbe80
extern struct Sprite *LogFlumeBarrelMatteSprite;
// 0x004cbe50
extern struct SpriteSet *LogFlumeTrackEndyList;
// 0x004cbe58
extern struct RideSpriteInfo FlumePieceSpriteInfo;
// 0x004cbe84
extern struct FlumeEntry *FlumeEntryList;
// 0x004cbe88
extern struct Sprite *LogFlumeEnta1Matte2Sprite;
// 0x004cbe8c
extern struct Sprite *LogFlumeSignSprite;
// 0x004cbe90
extern struct Sprite *LogFlumeEntrance2Sprite;
// 0x004cbe94
extern struct Sprite *LogFlumeEntrance3Sprite;
// 0x004cbe98
extern struct Sprite *LogFlumeEntrance1Sprite;
// 0x004cbe9c
extern struct Ride *DrivingSchoolPumpsRide;
// 0x004cbea4
extern struct PumpNode *PumpList;
// 0x004cbeac
extern struct RideQueueEntry *RoadEntryList;
// 0x004cbeb0
extern int TrafficLightA;
// 0x004cbeb4
extern int TrafficLightB;
// 0x004cbeb8
extern unsigned int TrafficLightPhase;
// 0x004cbec4
extern struct Ride *SafariRide;
// 0x004cbec0
extern void *SafariOffBNV;
// 0x004cbec8
extern struct Sprite *SafariLayer;
// 0x004cbecc
extern struct RenderItemNode *SafariRiderRenderList;
// 0x004cbed0
extern struct RideSpriteInfo SafariSpriteInfo;
// 0x004cbee8
extern int SafariRideDrawOffset1X;
// 0x004cbeec
extern int SafariRideDrawOffset1Y;
// 0x004cbef4
extern void *SafariRunBNV;
// 0x007988d0
extern void *DMusicStyles[600];
// 0x00799230
extern struct DirectMusicSegment *MusicIntroSegments[5];
// 0x00799244
extern struct DirectMusicSegment *MusicLoopSegments[5];
// 0x00799258
extern struct DirectMusicSegment *MusicTransitionSegments[5][5];
// 0x00799c1c
extern unsigned char *MusicFileData[35];
// 0x0079a608
extern int MusicFileSizes[35];
// 0x0079a69c
extern void *DMusicNotificationEvent;
// 0x0079a6b0
extern int ImtGetCount;
// 0x007fe9a4
extern int SpanDrawPitch;
// 0x007fea14
extern int SoftPrintHeight;
// 0x007fea18
extern unsigned char SpanStartAtOrBeforeMouse;
// 0x007fea1c
extern int SoftPrintWidth;
// 0x007fea20
extern unsigned int IndexedLLSPalette;
// 0x007fea4c
extern struct DrawClipOrigin SpriteClipOrigin;
// 0x007febac
extern struct DrawClipSize DrawClipExtent;
// 0x0082c66c
extern struct Sprite *ZSafariSprite;
// 0x0082c670
extern struct Point SafariRideDrawOffset;
// 0x004cbef8
extern void *SafariPathRunBNV;
// 0x004cbefc
extern void *SafariPathOnBNV;
// 0x004cbf00
extern void *SafariPathOffBNV;
// 0x004cbf04
extern void *SafariOnBNV[1];
// 0x004cbf08
extern struct Sprite *SafariRiderSprite;
// 0x004cbf0c
extern struct SafariNode *SafariNodeList;
// 0x004cbf10
extern void *SpiderRunBinV;
// 0x004cbf14
extern struct Sprite *SpiderHutMask1Sprite;
// 0x004cbf18
extern void *SpiderOffBinV;
// 0x004cbf1c
extern struct Sprite *SpiderHutMask2Sprite;
// 0x004cbf20
extern struct Ride *ActiveSpiderRide;
// 0x004cbf24
extern void *SpiderOnBinV;
// 0x004cbf28
extern struct Sprite *SpiderRideLayer;
// 0x0082c660
extern Point SpiderRideDrawOffset;
// 0x004cbf30
extern void *SpiderRideBinVs[2];
// 0x004cbf38
extern void *SpiderRideCarAssets[2];
// 0x004cbf40
extern struct RideSpriteInfo SpiderRideSpriteInfo;
// 0x004cbf58
extern struct SpiderNode *SpiderNodeList;
// 0x004cbf5c
extern struct Ride *TempleBuildingRide;
// 0x004cbf64
extern void *TempleLayer;
// 0x004cbf68
extern struct Sprite *TempleMatte1Sprite;
// 0x004cbf6c
extern struct Sprite *TempleMatte2Sprite;
// 0x004cbf70
extern struct RenderItemNode *TempleRiderRenderList;
// 0x004cbf7c
extern struct Sprite *TempleSlideLayer;
// 0x004cbf78
extern struct Sprite *TempSlideMatteSprite;
// 0x004cbf80
extern struct Ride *ActiveTempleSlideRide;
// 0x004cbf84
extern struct RenderItemNode *TempleSlideRiderRenderList;
// 0x004cbf88
extern unsigned int TempleSlideDrawOffset2X;
// 0x004cbf8c
extern unsigned int TempleSlideDrawOffset2Y;
// 0x004cbf98
extern struct RideSpriteInfo TempleSlideSpriteInfo;
// 0x004cbfb8
extern struct BinVFile *TempleSlideBinVs[3];
// 0x004cbfc4
extern void *TempSlideBinV;
// 0x004cbfc8
extern struct Point TempleSlideDrawOffset;
// 0x004cbfd0
extern struct Sprite *ZTempSlideSprite;
// 0x004cbfd4
extern struct SlideNode *SlideNodeList;
// 0x004cbfd8
extern struct Sprite *TopwaterSprite;
// 0x004cbfdc
extern struct Ride *ElephantFountainRide;
// 0x004cbfe0
extern struct Sprite *ShowerLayer;
// 0x004cbfe4
extern struct Element *WaterWorksEntranceElement;
// 0x004cbfe8
extern struct Element *WaterWorksImageListHandle;
// 0x004cbfec
extern struct Sprite *ElephantFountainLayer;
// 0x004cbff0
extern struct RideSpriteInfo WaterWorksSpriteInfo;
// 0x004cc008
extern struct Ride *WaterBlockRide;
// 0x004cc014
extern struct Sprite *ShowerSprite;
// 0x004cc018
extern struct SpriteSet *WaterWorksImageListData;
// 0x004cc01c
extern struct Ride *WaterWorksEntranceRide;
// 0x004cc020
extern struct Sprite *WwElsquirtSprite;
// 0x004cc024
extern struct Ride *ShowerRide;
// 0x004cc028
extern unsigned int WaterWorksSfxRefCount;
// 0x004cc02c
extern struct WaterNode *WaterBlockList;
// 0x004cc030
extern struct WaterNode *ShowerList;
// 0x004cc034
extern struct WaterNode *ElephantFountainList;
// 0x004cc03c
extern struct BoatRide *BoatRideList;
// 0x004cc048
extern struct Footprint BoatingSchoolExtraFootprint;
// 0x004cc060
extern struct Footprint BoatingSchoolStartFootprint;
// 0x004cc070
// 0x004cc074
extern struct BoatRideNode *BoatRideNodeList;
// 0x004cc078
extern struct Footprint BoatingSchoolFootprint;
// 0x004cc088
// 0x004cc08c
extern unsigned int BoatingSchoolAnimTick;
// 0x004cc090
extern struct Cursor BoatingSchoolMermaidCursors[4];
// 0x004d2164
extern struct MermaidNode *MermaidList;
// 0x004d2168
extern struct Cursor BoatingSchoolWaterCursors[4];
// 0x004d823c
extern struct PathNode *BoatPathList;
// 0x004d8240
extern struct PathNode *BoatBfsFrontier;
// 0x004d8244
extern struct PathNode *BoatBfsNextFrontier;
// 0x004d8250
struct PiecePlacement {
    unsigned int joins; /* 1: joins after the tail, 2: before the head */
    struct CastleState *owner;
    int next_spot;
    struct PathSeg *tail_anchor;
    int prev_spot;
    struct PathSeg *head_anchor;
};
extern struct PiecePlacement CoasterPlacement;
// 0x004d8268
extern unsigned int CoasterPieceCount;
// 0x004d8270
extern struct VecMath TrainVecMath;
extern float TrainInertiaHistory[64];
extern unsigned int TrainInertiaHistoryIndex;
// 0x004d88f4
extern unsigned char ShadeClampTable[0x80];
// 0x004d8974
extern unsigned char ShadeClampTableTop[0x40];
// 0x004d89b4
extern unsigned int GouraudSpanState[4];
// 0x004d89c4
extern void *ShadeClamp;
// 0x004d89c8
extern void *LtxFileTable[30];
// 0x004d8a40
extern struct LmsFile *LmsFileTable[31];
// 0x004d8abc
extern struct LmsRec *LfmFileTable[30];
// 0x004d8b34
extern unsigned int LfmFileSizes[30];
// 0x004d8bac
extern int *RollercoasterLpt;
// 0x004d8bb0
extern char EmptyString[0x100];
// 0x004dcbb8
extern float LmsLightDir[3];
// 0x004dcbd0
extern struct VecMath VecMathOps; /* .dim is the global at 0x004dcbf8 */
// 0x004dcc00
/* a vector of the castle's numerical code: count floats (the pool's are 0x54 bytes) */
struct FloatArray {
    int count;
    float data[20];
};

extern struct FloatArray VecScratchBuffers[30];
// 0x004dd5d8
extern unsigned int VecScratchTop;
// 0x004dd5e0
extern void *CurveFunctionTable[24];
// 0x004dd758
/* A text file read whole: its bytes and their count (CountTextLines and FindLineStart read both through the
 * address). */
struct CoasterTextFile {
    unsigned char *data;
    unsigned int size;
};
extern struct CoasterTextFile CoasterTxtFile;
// 0x004dd760
extern char CoasterObjBaseName[256];
// 0x004dd860
extern struct CoasterTextFile CoasterObjFile;
// 0x004dd868
extern unsigned int CoasterObjLineCount;
// 0x004dd86c
extern unsigned int CoasterTxtLineCount;
// 0x004dd870
extern unsigned char DepthSpanRecords[0x6000];
// 0x0060f908
extern int DepthSpanRecordCount;
// 0x0060f90c
extern int DepthSpanRecordsOverflow;
// 0x004e3870
extern char CoasterDepthBufferData[];
// 0x0060f914
extern struct CastleTrackCell CastleTrackCells[20];
// 0x006102f8
extern struct SegBlob CastleCurves[3];
// 0x00610a04
extern unsigned int CastleCoasterPlaced;
// 0x00610a08
extern unsigned int CastleTrackCellCount;
// 0x00610a18
extern struct RenderItemNode *CastleRiderRenderList;
// 0x00610a20 — 1/sqrt mantissa table (64 entries: {intercept, slope} pairs)
extern float rsqrtf_mantissa_table[128];
// 0x00610c40 — sqrt mantissa table (64 entries: {intercept, slope} pairs)
extern float sqrtf_mantissa_table[129];
// 0x00610e44 — sqrt exponent scale table (256 entries: sqrt(2^(e - 127)))
extern float sqrtf_exp_table[256];
// 0x00611244 — sqrtf exponent scale table (256 entries)
extern float rsqrtf_exp_table[256];
// 0x00611648
extern unsigned int CastleShadeMin;
// 0x0061164c
extern float TrackLightingOffset;
// 0x00611650
extern float ModelLightingOffset;
// 0x00611654
extern float UnusedTrackFloat;
// 0x00611658
extern Vector3 TrackRailOffsets[4];
// 0x00611688
/* castle.c lookup table: FindPieceClassForElement finds value by key; ElementPieceClassCount counts the entries */
struct KeyValuePair {
    struct Element *key;
    struct PieceClass *value;
};
extern struct KeyValuePair ElementPieceClasses[11];
// 0x006116e0
extern Vector3 TrackArcCentres[4];
// 0x00611710
extern unsigned int RailDrawOrder[16];
// 0x00611750
extern Vector3 TrackArcTangents[4];
// 0x00611780
extern unsigned char UnusedTrackScratch[0x40];
// 0x006117c0
extern Vector3 TrackSideMidpoints[34];
// 0x00611958
extern unsigned int ElementPieceClassCount;
// 0x0061195c
extern unsigned int TexturedSpanSource;
// 0x00611960
extern unsigned char TexelShadeCache[0x800];
// 0x00612160
extern unsigned int TexturedSpanState[6];
// 0x00612178
extern char RailSplitPoints[1];
// 0x006122a0
extern float CurvePlotWorldPoints[90][3];
// 0x00614858
extern struct CastleFloatEnt TrackTieVerts[8];

// 0x00616180
extern struct Cursor JungleCruiseMonkeyFishCursors[8];
// 0x00622320
extern struct Cursor JungleCruiseCursors[4];
// 0x006159c8
extern int CurvePlotScreenPoints[90][4];
// 0x00615f6c
extern unsigned int RailSplitCount;
// 0x00615f70
extern unsigned int SupportPostLmsRec[4];
// 0x00615f84
extern struct CurveWalker *RangeWalker;
extern int BisectRootCallCount;
extern int BisectRootStepCount;
extern int WalkerRangeCallCount;
extern float RangeRadius;
extern float RangeRadiusSq;
extern float RangeOuterSq;
extern float RangeInnerSq;
extern int WalkerRangeOutsideCount;
extern int WalkerRangeNormalCount;
extern int BisectRootMaxSteps;
extern float *RangeCentre;
extern int (*RangeRootFinder)(float (*fn)(unsigned int), float lo, float hi, float *out);

extern struct CurveWalker *ArcLengthWalker;
extern int ArcLengthCurvePair;
extern float ArcLengthDepth;
extern struct CastleFloatEnt SupportPostShapeTemplate[3][4];
// 0x004b6150
extern struct LmsFile SupportPostModel; /* the track support post: 12 vertices (written by DrawSupportPost), 16 flat triangles */
extern struct CastleFloatEnt SupportPostShape[3][4];
// 0x00615f90
extern int RangeWalkerSegment;
// 0x00615fd4
extern float RangeDepth;
// 0x00615f98
extern struct VecMath CurveVecMath;
// 0x00616000
extern struct LmsFile *CoasterTrainWheelLms;
// 0x00616004
extern struct LmsRec *CoasterTrainWheelLfm;
// 0x00616010
extern struct BinVFile *BalloonzBinV;
// 0x00616018
extern struct BinVFile *BalloonzBinVs[4];
// 0x0061603c
extern struct Sprite *BalloonzRiderSprites[2];
// 0x00616044
extern struct Sprite *BalloonzLayer;
// 0x00616048
extern struct Sprite *BallBaseM1Sprite;
// 0x0061604c
extern struct Sprite *BallBaseM2Sprite;
// 0x00616050
extern struct Sprite *BallBaseM3Sprite;
// 0x00616054
extern struct Sprite *BZRedCarM1Sprite;
// 0x00616058
extern struct Sprite *BZGreenCarM1Sprite;
// 0x0061605c
extern struct Sprite *BZBlueCarM1Sprite;
// 0x00616060
extern struct BalloonNode *BalloonNodeList;
// 0x00616068
extern struct Sprite *CarouselLayer;
// 0x0061606c
extern struct Sprite *CarouselEntranceMatteSprite;
// 0x00616070
extern struct Sprite *CarouselEntranceMatte2Sprite;
// 0x00616078
extern int CarouselDrawOffsetX;
// 0x0061607c
extern int CarouselDrawOffsetY;
// 0x00616080
extern void *CarouselOnBinV;
// 0x00616084
extern void *CarouselOffBinV;
// 0x0061608c
extern void *CarouselBinV;
// 0x00616090
extern void *CarouselBinVs[3];
// 0x006160b8
extern struct Sprite *ZCarouselSprite;
// 0x006160bc
extern struct Ride *ActiveCarouselRide;
// 0x006160c0
extern struct Sprite *CarouselRiderSprite;
// 0x006160c4
extern struct CarouselNode *CarouselNodeList;
// 0x006160d0
extern struct Ride *EarthSlideRide;
// 0x006160d4
extern struct RinData *EarthSlideRin;
// 0x006160d8
extern struct Sprite *EarthSlideEntranceMatteSprite;
// 0x006160e0
extern struct Sprite *EarthSlideEntranceMatte2Sprite;
// 0x006160e4
extern struct Position *EarthPos;
// 0x006160c8
extern unsigned int UnusedEarthSlideA;
// 0x006160cc
extern unsigned int UnusedEarthSlideB;
// 0x006160e8
extern struct EarthNode *EarthNodeHead;
// 0x006160ec
extern struct RenderItemNode *BlokeRenderList;
// 0x006160f4
extern struct Ride *EntranceRide;
// 0x006160f0
extern struct Sprite *EntranceLayer;
// 0x00616110
extern int Entrance1AlternateBit;
// 0x006160fc
extern struct Sprite *EntranceMatte1Sprite;
// 0x00616100
extern struct Sprite *EntranceMatte2Sprite;
// 0x00616104
extern struct Sprite *EntranceMatte3Sprite;
// 0x00616108
extern struct Sprite *EntranceMatte4Sprite;
// 0x0061610c
extern struct Sprite *Booth1Sprite;
// 0x00616118
extern struct Sprite *EateryLayerOwner;
// 0x00616120
extern struct RideSpriteInfo EaterySpriteInfo;
// 0x0061613c
extern struct Element *BrollyImagesHandle;
// 0x00616140
extern struct BrollyData *BrollyImagesData;
// 0x00616144
extern struct BrollyNode *BrollyNodeList;
// 0x00616148
extern struct SaveBlock *SaveBlockList;
// 0x0061614c
extern struct SpriteSet *HedgeImagesData;
// 0x00616150
extern struct Element *FlowerImagesHandle;
// 0x00616158
extern struct SpriteSet *FlowerImagesData;
// 0x0061615c
extern struct Element *HedgeImagesHandle;
// 0x00616164
extern struct JungleRide *JungleRideList;
// 0x00629c2c
extern struct JungleObj *JungleObjList;
// 0x00629c30
extern struct JungleFish *JungleFishList;
// 0x00629c34
extern struct JungleObj *JungleCruiseInstanceList;
// 0x00629c3c
extern struct JungleScore *JungleScoreList;
// 0x00629c40
extern struct Footprint JungleCruiseFootprint;
// 0x00629c50
// 0x00629c54
extern int JungleCruiseStep;
// 0x00629c58
extern struct Cursor JungleCruiseWaterCursors[4];
// 0x0062fd2c
extern struct JunglePath *JunglePathList;
// 0x0062fd30
extern struct JunglePath *JungleBfsFrontier;
// 0x0062fd34
extern struct JunglePath *JungleBfsNextFrontier;
// 0x0062fd3c
extern struct JailCell *JailCellList;
// 0x0062fd40
extern void *WesternTownLayer;
// 0x0062fd48
extern struct RideSpriteInfo SpaceTowerSpriteInfo; /* the space tower's sprite to draw */
// 0x0062fd60
extern struct Sprite *SpaceTowerLayers;
// 0x0062fd64
extern struct Sprite *SpaceTowerSeatMatteSprites[4];
// 0x0062fd74
extern struct Ride *SpaceTowerRideObj;
// 0x0062fd7c
extern struct Sprite *SpaceTowerMatte2Sprite;
// 0x0062fd80
extern struct Sprite *SpaceTowerMatte1Sprite;
// 0x0062fd88
extern struct Point SpaceTowerLayerOffsets[4];
// 0x0062fda8
extern struct SpaceTowerCar *SpaceTowerCarList;
// 0x0062fdb0
extern struct RideSpriteInfo SpinningBarrelsSpriteInfo; /* the spinning barrels' sprite to draw */
// 0x0062fdc8
extern void *BoxBlokesOffBNV;
// 0x0062fdcc
extern struct Sprite *SpinningBarrelsEntranceMatte2Sprite;
// 0x0062fdd0
extern struct Sprite *SpinningBarrelsEntranceMatteSprite;
// 0x0062fde4
extern struct Ride *SpinningBarrelsRide;
// 0x0062fde0
extern struct Sprite *SpinningBarrelsLayer;
// 0x0062fdd8
extern struct Point SpinningBarrelsDrawOffset;
// 0x0062fde8
extern void *SpinningBarrelsBNV;
// 0x0062fdf0
extern void *SpinningBarrelsBinVs[3];
// 0x0062fdfc
extern void *BoxBlokesOn1BNV;
// 0x0062fe00
extern void *SpinningBarrelsRiderSprites[2];
// 0x0062fe08
extern struct BarrelNode *SpinningBarrelList;
// 0x0062fe48
extern struct Sprite *PottingShedLayer;
// 0x0062fe4c
extern struct Sprite *GShedMatteSprite;
// 0x0062fe50
extern struct Sprite *MechanicsHutLayer;
// 0x0062fe54
extern struct Sprite *MechHutMaskSprite;
// 0x0062fe58
extern struct Ride *PlaneRide;
// 0x0062fe60
extern struct RideSpriteInfo PlaneRideSpriteInfo;
// 0x0062fe78
extern void *Zoomer0ffBinV;
// 0x0062fe7c
extern struct Sprite *PlaneRideLayer;
// 0x0062fe84
extern void *PlaneRideBinVs[3];
// 0x0062fe90
extern void *ZoomerideBinV;
// 0x0062fe94
extern void *Zoomer0nBinV;
// 0x0062fe98
extern struct Sprite *PlaneRideRiderSprite;
// 0x0062fe9c
extern struct PlaneRideNode *PlaneRideNodeList;
// 0x0062fea0
extern int DialogListScrollY;
// 0x0062fea4
extern int DialogSliderDragging;
// 0x0062fea8
extern int *AltManTexturesB;
// 0x0062feac
extern void *AltWomanFileData;
// 0x0062feb0
extern void *GeoffMeshes[2];
// 0x0062feb8
extern int AltManTextureCountB[1];
// 0x0062febc
extern void *ManMeshes[6];
// 0x0062fed4
extern void *WomanMeshes[6];

// 0x0062feec
extern struct RenderItemNode *RenderItemPoolNext[1]; /* the render item pool's next node */
// 0x0062fef0
extern struct RenderItemNode *RenderItem2PoolNext; /* the second pool's next node */
// 0x0062fef4
extern void *TracyWalkMesh;
// 0x0062fef8
extern int *AltWomanTexturesA;
// 0x00630100
extern void *AltManFileData;
// 0x00638358
extern unsigned short SavedFPUControlWord;
// 0x0064cd8c
extern int *AltWomanTexturesB;
// 0x00655a38
extern int *AltManTexturesA;
// 0x00655a3c
extern void *PersonListHead;
// 0x00655a4c
extern unsigned int RenderItemCount;
// 0x00655a50
extern unsigned int RenderItem2Count;
// 0x00665e8c
extern unsigned int LoadedTextureCount;
// 0x00665eec
extern int AdvisorAnimFrame;
// 0x00665f48
extern unsigned int AviAnimOpenCount;
// 0x00665f5c
extern void *CurrentAdvisorAnim;
// 0x00665f60
extern void *NextAdvisorAnim;
// 0x00665f64
extern unsigned int UnusedAdvisorVidA;
// 0x00665f68
extern unsigned int UnusedAdvisorVidB;
// 0x00665f6c
extern unsigned int UnusedAdvisorVidC;
// 0x00665fe8
extern unsigned int AdvisorAnimState;
// 0x00665fec
extern unsigned int NextAdvisorAnimState;
// 0x00665ff8
extern unsigned int ReportFlags;
// 0x00665ffc
extern unsigned int ReportLegolandZoneGoal;
// 0x00666000
extern int ReportAdventurerZoneGoal;
// 0x00666004
extern int ReportCastleZoneGoal;
// 0x00666008
extern int ReportWesternZoneGoal;
// 0x0066600c
extern int CastleScoreGoal;
// 0x00666010
extern int DrivingSchoolScoreGoal;
// 0x00666014
extern int LogFlumeScoreGoal;
// 0x00666018
extern int BoatingSchoolScoreGoal;
// 0x0066601c
extern int JungleCruiseScoreGoal;
// 0x00666020
extern int ReportType1Or3ObjectCountGoal;
// 0x00666024
extern int ReportType1Or3ObjectCountMax;
// 0x00666028
extern int ReportType1Or3ClassCountGoal;
// 0x0066602c
extern int ReportType1Or3ClassCountMax;
// 0x00666030
extern int ReportLinkedObjectPercentGoal;
// 0x00666034
extern int ReportLinkedObjectPercentMax;
// 0x00666038
extern int ReportVisitorCountGoal;
// 0x0066603c
extern int ReportVisitorCountMax;
// 0x00666040
extern int ReportFoodStallObjectCountGoal;
// 0x00666044
extern int ReportFoodStallObjectCountMax;
// 0x00666048
extern int ReportFoodStallClassCountGoal;
// 0x0066604c
extern int ReportFoodStallClassCountMax;
// 0x00666050
extern int ReportVisitorHappinessGoal;
// 0x00666054
extern int ReportVisitorHappinessMax;
// 0x00666058
extern int ReportPowerSupplyGoal;
// 0x0066605c
extern int ReportPowerSupplyMax;
// 0x00666060
extern int ReportWorkingRidesGoal;
// 0x00666064
extern int ReportWorkingRidesMax;
// 0x00666068
extern int ReportMapTileCountGoal;
// 0x0066606c
extern int ReportMapTileCountMax;
// 0x00666070
extern int ReportType2ObjectCountGoal;
// 0x00666074
extern int ReportType2ObjectCountMax;
// 0x00666078
extern int ReportType2ClassCountGoal;
// 0x0066607c
extern int ReportType2ClassCountMax;
// 0x00666080
extern int ReportSceneryCoverageGoal;
// 0x00666084
extern int ReportSceneryCoverageMax;
// 0x00666088
extern int ReportType4ObjectCountGoal;
// 0x0066608c
extern int ReportType4ObjectCountMax;
// 0x00666090
extern int ReportType4ClassCountGoal;
// 0x00666094
extern int ReportType4ClassCountMax;
// 0x00666098
extern unsigned int AppraisalDeadline;
// 0x0066609c
extern int LastAppraisalResult;
// 0x006660a0
extern int AppraisalPageCount;
// 0x006660a4
extern int AppraisalPage;
// 0x006660a8
extern struct IconNode *AppraisalNextIcon;
// 0x006660ac
extern struct IconNode *AppraisalPrevIcon;
// 0x006660b0
extern char GfxFileNameBuffer[256];
// 0x006661bc
extern int ParkVisitorCount;
// 0x006661c0
extern struct Element *SharkCafeBrollyElem;
// 0x006661c4
extern struct Element *Entrance1Elem;
// 0x006661c8
extern int VisitorSpawnCounter;
// 0x006661cc
extern char BlokeAiLogLines[8][100];
// 0x006664ec
extern int BlokeAiLogIndex;
// 0x006664f8
extern struct BuildObj BuildObjArray[256]; /* objects under construction */
// 0x006670f8
extern int BuildObjCount;
// 0x006670fc
extern unsigned int ButtonRepeatDelay;
// 0x00667104
extern unsigned int ControllersInitialized;
// 0x00667108
extern unsigned int IgnoreNextButtonRelease;
// 0x0066710c
extern unsigned int ButtonRepeatLastTick;
// 0x00667114
extern unsigned int FountainSFXRefCount;
// 0x00667118
extern unsigned int PowerStationSFXRefCount;
// 0x0066711c
extern unsigned int DinoSFXRefCount;
// 0x00667120
extern unsigned int MoneySFXLoadCount;
// 0x00667128
extern char LogBuffer[512];
// 0x0066752c
extern char ProductVersionString[0x88];
// 0x006675b4
extern unsigned int BubbleHelpGFXLoaded;
// 0x006675b8
extern volatile int TextCellCount;
// 0x006675c0
extern struct TextCell TextCells[50];
// 0x00667c00
extern int MapWorldMinX;
// 0x00667c04
extern int MapWorldMinY;
// 0x00667c08
extern int MapWorldMaxX;
// 0x00667c0c
extern int MapWorldMaxY;
// 0x00667c14
extern short MapDrawHeight;
// 0x00667c16
extern short MapDrawWidth;
// 0x00667c18
extern int MapWorldHeight;
// 0x00667c1c
extern int MapWorldWidth;
// 0x00667c20
extern int MapCenterOffsetY;
// 0x00667c10
extern void *SavedOkKeyHandler;
// 0x00667c28
extern void *SavedCancelKeyHandler;
// 0x00667c2c
extern struct Sprite *FullMapSprite;
// 0x00667c30
extern unsigned int FullMapRendered;
// 0x00667c34
extern struct Sprite *MapSpannerSprite;
// 0x00667c3c
extern int FtolScratch;
// 0x00667c54
extern LEGO_EXPORT TileId QueryObj;
// 0x00667c58
extern LEGO_EXPORT struct Ride *QueryClass;
// 0x00667c5c
extern unsigned int UnusedMapEditFlag;
// 0x00667c60
extern unsigned int PreMapScreenMode;
// 0x00667c68
extern int LastMapClickTicks;
// 0x00667c70
extern long LastMapClickX;
// 0x00667c74
extern long LastMapClickY;
// 0x00667c64
extern const char *AdvisorDebugStage;
// 0x00667c48
extern unsigned int MouseClickConsumed;
// 0x00667c4c
extern int LevelScriptLoadResult;
extern unsigned int GameStartPending;
extern unsigned int SamplesPaused;
// 0x00667c7c
extern unsigned int MapLoaded;
// 0x00667c80
extern unsigned int SaveGameLoadPending;
// 0x00667c88
extern struct Sprite *Arrow02Sprite;
// 0x00667c8c
extern struct Sprite *Arrow01Sprite;
// 0x00667c90
extern struct Sprite *Arrow04Sprite;
// 0x00667c94
extern struct Sprite *Arrow03Sprite;
// 0x00667c9c
extern void *GameMapRawBlock;
// 0x00667ca0
extern unsigned int LoadInProgress;
// 0x00667ca4
extern unsigned int LevelBaseTileId;
// 0x00667ca8
extern LEGO_EXPORT void *OverlayList;
// 0x00667cac
extern LEGO_EXPORT struct SpriteSet *OverlayILF;
// 0x00667cb0
extern struct SpriteSet *BridgesData;
// 0x00667cb4
extern LEGO_EXPORT int ScrollX;
// 0x00667cb8
extern LEGO_EXPORT int ScrollY;
// 0x00667cbc
extern LEGO_EXPORT int ScrollSpeedX;
// 0x00667cc0
extern LEGO_EXPORT int ScrollSpeedY;
// 0x00667cc4
extern int UnusedRenderViewCounter;
// 0x00667cd0
extern int BackgroundOffsetX;
// 0x00667cd4
extern int BackgroundOffsetY;
// 0x00667cd8
extern unsigned int MapBatchUpdate;
// 0x00667cdc
extern unsigned int MapBatchChanged;
// 0x00667ce0
extern int TotalCoveredSquares;
// 0x00667ce4
extern int RideCoveredSquares;
// 0x00667ce8
extern int ShopCoveredSquares;
// 0x00667cec
extern int FoodCoveredSquares;
// 0x00667cf0
extern int Type3CoveredSquares;
// 0x00667cf4
extern int PathSquareCount;
// 0x00667cf8
extern int SceneryCoveredSquares;
// 0x00667cfc
extern int UnusedCoverageCounter;
// 0x00667d00
extern int PathEdgeSquareTotal;
// 0x00667d04
extern int PathEdgeScenerySquares;
// 0x00667d08
extern int PathSceneryPercent;
// 0x00667d0c
extern int PathSceneryDirty;
// 0x00667d10
extern unsigned int PathSceneryLastUpdate;
// 0x00667d3c
extern int RectListCount;
// 0x00667d40
extern unsigned int ShowPathTileOverlay;
// 0x00667d44
extern int DeferredSpriteCount;
// 0x00667d48
extern unsigned int PathOverlayBlinkMask;
// 0x00667d4c
extern unsigned int LastMouseScrollFrameMS;
// 0x00667d50
extern unsigned int MapDataLoaded;
// 0x00667d54
extern unsigned int DamageTimerStart;
// 0x00667d58
extern int LastRideWearFrame;
// 0x00667d60
extern int LastWatchDrawTicks;
// 0x00667d68
extern unsigned int LastRenderingCompleteTick;
// 0x00667d6c
extern int WinDebugMode;
// 0x00667d70
/* DX6 DDCAPS is 0x17c; this SDK's DX5 DDCAPS is 0x16c, so each caps block has a four-DWORD DDSCAPS2 tail. */
struct DDRAWENV {
    /* 0x000 */ LPDIRECTDRAW ddraw;
    /* 0x004 */ LPDIRECTDRAW2 ddraw2;
    /* 0x008 */ DDCAPS caps;
    /* 0x174 */ unsigned int ddsCaps[4];
    /* 0x184 */ DDCAPS hel_caps;
    /* 0x2f0 */ unsigned int hel_ddsCaps[4];
    /* 0x300 */ unsigned int unk[54];
};
extern LEGO_EXPORT struct DDRAWENV DDRAWENV;
// 0x00668070
extern LPDIRECTDRAWSURFACE PrimarySurface;
// 0x00668074
extern LPDIRECTDRAWSURFACE SecondaryOffscreenSurface;
// 0x00668078
extern LPDIRECTDRAWSURFACE OffscreenSurface;
// 0x0066807c
extern LPDIRECTDRAWSURFACE renderEngine;
// 0x00668080
extern LPDIRECTDRAWCLIPPER DDrawClipper;
// 0x00668084
extern LPDIRECTDRAWPALETTE DDPalette;
// 0x00668088
extern unsigned int DisplayPixelFormat;
// 0x0066808c
extern HFONT LegoFont20Bold;
// 0x00668090
extern HFONT LegoFont24Bold;
// 0x00668094
extern HFONT LegoFont18SemiBold;
// 0x00668098
extern HFONT LegoFont28Normal;
// 0x0066809c
extern DDSURFACEDESC CurrentSurfaceDesc;
// 0x00668108
extern RECT SoftPrintClip;
// 0x00668118
extern int renderEngineTargetIdx;
// 0x0066811c
extern LPDIRECTDRAWSURFACE renderEngineTargets[10];
// 0x00668144
extern int VideoSurfaceLocked;
// 0x00668148
extern struct Sprite *CurrentPointerSprite;
// 0x0066814c
extern unsigned int CurrentPointerIndex;
// 0x00668160
extern unsigned int LLSMaskCodesLeft;
// 0x00668164
extern int RenderingStatusLockedStack[32];
// 0x006681e4
extern int RenderingStatusStackDepth;
// 0x006681e8
extern unsigned short *OverridePalette;
// 0x006681ec
extern void *MovieLastDoubledSurface;
// 0x006681f0
extern unsigned int FpsWindowStartTicks;
// 0x006681f4
extern unsigned int LastFrameTicks;
// 0x006681f8
extern int FramesThisSecond;
// 0x006681fc
extern LEGO_EXPORT unsigned int LastFrameMS;
// 0x00668200
extern unsigned int LastPresentTicks;
// 0x00668204
extern unsigned int WatchActive;
// 0x00668208
extern struct Sprite *WatchSprite;
// 0x0066820c
extern char ObjectiveTextBuffer[0x404];
// 0x00668610
extern unsigned int ScriptDirtyCategories;
// 0x00668614
extern unsigned int CurrentObjectivePromptIndex;
// 0x00668618
extern unsigned int AdvisorRepeatPending;
// 0x0066861c
extern char BriefingFileName[128];
// 0x0066869c
extern char HintsFileName[128];
// 0x0066871c
extern unsigned int BuildMenuRefreshPending;
// 0x00668720
extern unsigned int ScriptStringCount;
// 0x00668724
extern struct ObjectiveEvent *ObjectiveMessageQueue;
// 0x00668728
extern struct ObjectiveEvent *ObjectiveEventList;
// 0x0066872c
extern unsigned int ObjectiveEventTypeCounts[0x15];
// 0x00668780
extern unsigned int ObjectiveHintTimer;
// 0x00668784
extern struct ObjectiveEvent *ScriptEventList;
// 0x00668788
extern unsigned int ScriptPurgePending;
// 0x0066878c
extern int ScriptNeedRemaining;
// 0x00668790
extern unsigned int ScriptSectionStarted;
// 0x00668794
extern unsigned int ScriptEvalTime;
// 0x00668798
extern struct NerpsListNode *ScriptSectionList;
// 0x0066879c
extern struct NerpsListNode *CurrentScriptSection;
// 0x006687a0
extern unsigned int ScriptLoadErrorCount;
// 0x006687a4
extern unsigned int CurrentHelpKind;
// 0x006687a8
extern unsigned int HelpRequested;
// 0x006687ac
extern unsigned int SpeechImmediateRequest;
// 0x006687b0
extern unsigned int SpeechDelayFrames;
// 0x006687b4
extern unsigned int SpeechDebouncePending;
// 0x006687bc
extern void *OkKeyHandler;
// 0x006687c0
extern void *CancelKeyHandler;
// 0x006687c8
extern struct IconNode *IconListHead;
// 0x006687cc
extern struct IconNode *HelpIconListHead;
// 0x006687d0
extern LEGO_EXPORT void *FocussedIconPtr;
// 0x00668828
extern struct Sprite *GBarFrameSprite;
// 0x0066882c
extern struct Sprite *IfSideBUpSprite;
// 0x00668830
extern struct Sprite *IfSideBDownSprite;
// 0x00668834
extern struct Sprite *IfSidebar1Sprite;
// 0x00668840
extern short IconBBoxExtraHeight;
// 0x0066884c
extern short IconBBoxExtraWidth;
// 0x00668858
extern struct IconNode DefaultIconNode;
// 0x006688a8
extern void *NewIconGroupCallbacks;
// 0x006688ac
extern void *NewGroupRenderFunc;
// 0x006688b0
extern void *NewGroupEventHandler;
// 0x006688b4
extern unsigned int LastScrollIconTick;
// 0x006688b8
extern unsigned int NewObjectScrollHintFlags;
// 0x006688bc
extern int PowerDemandBarPos;
// 0x006688c0
extern int EnergyBarSupplyWidth;
// 0x006688c4
extern int MoneyBarShownWidth;
// 0x006688c8
extern unsigned int RenderIconsLastTicks;
// 0x006688cc
extern unsigned int RenderIconsExceptIdLastTicks;
// 0x006688d0
extern unsigned int GBarSpritesLoaded;
// 0x006688d4
extern void *IndicatorList;
// 0x006688d8
extern void *ActiveIndicators;
// 0x006688e0
extern struct Sprite *PuBgMainSprite;
// 0x006688e4
extern struct Sprite *PuBgCentreTopSprite;
// 0x006688e8
extern struct Sprite *PuBgRightTopSprite;
// 0x006688ec
extern struct Sprite *PuBgLeftMidSprite;
// 0x006688f0
extern struct Sprite *PuBgCentreMidSprite;
// 0x006688f4
extern struct Sprite *PuBgRightMidSprite;
// 0x006688f8
extern struct Sprite *PuBgLeftBtmSprite;
// 0x006688fc
extern struct Sprite *PuBgCentreBtmSprite;
// 0x00668900
extern struct Sprite *PuBgRightBtmSprite;
// 0x00668904
extern struct Sprite *CBBGLeftSprite;
// 0x00668908
extern struct Sprite *CBBGCentreSprite;
// 0x0066890c
extern struct Sprite *CBBGRightSprite;
// 0x00668910
extern struct Sprite *NewPopMockSprite;
// 0x00668914
extern struct Sprite *PuDeleteObjectOnSprite;
// 0x00668918
extern struct Sprite *PuDeleteObjectSprite;
// 0x0066891c
extern struct Sprite *PuClosePopUpOnSprite;
// 0x00668920
extern struct Sprite *PuClosePopUpSprite;
// 0x00668924
extern struct Sprite *NextIconOnSprite;
// 0x00668928
extern struct Sprite *NextIconSprite;
// 0x0066892c
extern struct Sprite *PrevIconOnSprite;
// 0x00668930
extern struct Sprite *PrevIconSprite;
// 0x00668934
extern struct Sprite *PUOKOnSprite;
// 0x00668938
extern struct Sprite *PUOKSprite;
// 0x0066893c
extern struct Sprite *CBCloseSprite;
// 0x00668940
extern struct Sprite *CBCloseOnSprite;
// 0x00668944
extern struct Sprite *PuAddGardenerOnSprite;
// 0x00668948
extern struct Sprite *PuAddGardenerSprite;
// 0x0066894c
extern struct Sprite *PuAddMechanicsOnSprite;
// 0x00668950
extern struct Sprite *PuAddMechanicsSprite;
// 0x00668954
extern unsigned int WorkerOnMouseActive;
// 0x00668958
extern struct Sprite *PopUpSpritesLoaded;
// 0x0066895c
extern int PopUpShowsBuildProgress;
// 0x00668960
extern unsigned int CurrentAdvicePriority;
// 0x00668964
extern int PopUpMessageCentrePieces;
// 0x00668968
extern char PopUpMessageText[0x400];
// 0x00668d68
extern unsigned int PopUpMessageShown;
// 0x00668d78  (DIMOUSESTATE; full type in <dinput.h>, dereferenced only by input.c)
extern struct _DIMOUSESTATE MouseState;
// 0x00668d88
extern void *dinput;
// 0x00668d8c
extern void *dinput_keyboard;
// 0x00668d90
extern void *dintput_mouse;
// 0x00668d94
extern char CheatKeyBuffer[0x14];
// 0x00668da8
extern unsigned char PrevKeyState[0x3b];
// 0x00668de4
extern unsigned char CheatKeyState[0x3b];
// 0x00668e20
extern unsigned int SavedThemeIconEnabled[4];
// 0x00668e34
extern unsigned int ResearchTabSelected;
// 0x00668e38
extern unsigned int FrontEndSubScreenActive;
// 0x00668e3c
extern void *WesternThemeIcon;
// 0x00668e40
extern struct InterfaceListNode *BuildObjectList;
// 0x00668e44
extern int BuildPanelScrollOffsets[8];
// 0x00668e64
extern unsigned char BuildPanelScrollIndex;
// 0x00668e68
extern struct Sprite *InterfaceBgSprite;
// 0x00668e6c
extern struct Sprite *NoEnergySprite;
// 0x00668e70
extern struct Sprite *BarPointerSprite;
// 0x00668e74
extern struct Sprite *AttractHighlightOnSprite;
// 0x00668e78
extern struct Sprite *AttractHighlightOffSprite;
// 0x00668e7c
extern struct Sprite *AttractNewOffSprite;
// 0x00668e80
extern struct Sprite *AttractNewOnSprite;
// 0x00668e84
extern struct Sprite *SideScrollDownLitSprite;
// 0x00668e88
extern struct Sprite *SideScrollUpLitSprite;
// 0x00668e8c
extern struct Sprite *LinkMiddleSprite;
// 0x00668e90
extern struct Sprite *LinkBottomSprite;
// 0x00668e94
extern struct Sprite *BriefIcon2Sprite;
// 0x00668e98
extern struct Sprite *BriefIconSprite;
// 0x00668e9c
extern void *BriefingIcon;
// 0x00668ea0
extern struct Sprite *ScriptEndSprite;
// 0x00668ea4
extern unsigned int InterfaceControlIconsLoaded;
// 0x00668eb0
extern struct IconNode *SelectedThemeIcon;
// 0x00668eb4
extern unsigned int ThemeIconsLoaded;
// 0x00668eb8
extern struct IconNode *ScriptEndIcon;
// 0x00668ebc
extern unsigned int GameInterfaceLoaded;
extern unsigned int ButtonFlashWasActive;
// 0x00668ed8
extern struct InterfaceResearchNode *ResearchList;
// 0x00668ee0
extern int AviAudioNeedsAcm;
// 0x00668ee8
extern struct AcmHeader AviAcmStreamHeader;
// 0x00668f3c
extern unsigned int AviSoundBytesPerFrame;
// 0x00668f44
extern int AcmStreamOpenResult;
// 0x00668f48
extern void *AviSoundBuffer;
// 0x00668f4c
extern unsigned int AviAudioBitsPerSample;
// 0x00668f50
extern unsigned int AviSoundBufferSlotCount;
// 0x00668f54
extern int AviAcmDstOffset;
// 0x00668f58
extern unsigned int AviAudioStartSample;
// 0x00668f5c
extern void *AviAcmStream;
// 0x00668f60
extern int AviAudioSamplePos;
// 0x00668f64
extern unsigned int AviAudioSampleSize;
// 0x00668f70
extern WAVEFORMATEX AviPcmFormat;
// 0x00668f84
extern void *AviAudioStream;
// 0x00668f88
extern int AviAudioEndSample;
// 0x00668f8c
extern void *AviAcmDstBase;
// 0x00668f90
extern unsigned int AviSamplesPerFrame;
// 0x00668f9c
extern int AviAudioReady;
// 0x00668fa0
extern unsigned int AviSoundBufferSlot;
extern unsigned int AviOpenCount;
// 0x00668fa4
extern unsigned int AviSoundBufferFrames;
// 0x00668fac
extern unsigned int PerfCounterState;
// 0x00668fb0
extern unsigned int MovieSkipRequested;
// 0x00668fb4
extern unsigned int AviAcmDstBytes;
// 0x00668fb8
extern struct MoviePool *ActiveMoviePool;
// 0x00668fc0
extern void *OpenPathNodeList;
// 0x00668fc4
extern struct GameMainNode *ClosedPathNodeList;
// 0x00668fcc
extern int ScriptResourceLineNumber;
// 0x00668fd0
extern char CurrentScriptFileName[128];
// 0x00669050
extern unsigned char CurrentObjectiveEventFlags;
// 0x00669054
extern unsigned int ScriptSectionTypeMask;
// 0x00669058
extern char ScriptSectionName[0x40];
// 0x00669098
extern unsigned int NextScriptSectionId;
// 0x004bc0ec
extern char LanguageName[36];
// 0x006691a0
extern unsigned int LLIDB_Capacity;
// 0x006691a4
extern unsigned int LLIDB_ElementCount;
// 0x006691a8
extern struct Element **LLIDB_Pages;
// 0x006691ac
extern struct LLSNode *LLSPlayList;
// 0x006691b0
extern int SaveFileHandle;
// 0x006691b4
extern int SavedElementCount;
// 0x006691bc
extern int MeasuredBlockStarts[16];
// 0x006691fc
extern int MeasuredBlockDepth;
// 0x00669200
extern struct Element **SavedElementTable;
// 0x00669204
extern unsigned int GameTimerMark;
// 0x00669208
extern void *g_hInstance;
// 0x0066920c
extern int g_nCmdShow;
// 0x00669210
extern HWND WndEnvHwnd;
// 0x00669238
extern unsigned int PauseGameTimerResult;
// 0x00669240
extern LEGO_EXPORT struct Ride *ObjectClassList;
// 0x00669244
struct LibraryNode;
extern struct LibraryNode *ObjectLibraryHead;
// 0x00669248
extern void *ClassRideList;
// 0x0066924c
extern struct InfoNode *ClassRideBestPtr;
// 0x00669250
extern int DirSearchWaveCount;
// 0x00669254
extern unsigned int AdjacentNodeCount;
// 0x004bcec0
extern struct Point PathGoalTile;
// 0x00669258
extern unsigned int PathVisitedBits[1152];
// 0x0066a45c
extern void *AdjacentNodes[1020];
// 0x0066b44c
extern void *BestNodeList;
// 0x0066b450
extern struct DirNode *DirSearchNodeList;
// 0x0066b454
extern struct DirNode *DirSearchGoalNode;
// 0x0066b458
extern struct DirNode *DirPathNodeList;
// 0x0066b460
extern struct Point Entrance1Point;
// 0x0066b468
extern int LastPathUpdateTime;
// 0x0066b46c
extern unsigned int PathUpdateNeeded;
// 0x0066b470
extern char VisitorNameBuffer[0x104];
// 0x0066b574
extern LEGO_EXPORT struct Bloke *FirstBloke;
// 0x0066b57c
extern struct Bloke *BlokePool; /* bloke pool */
// 0x0066b580
extern int DirPickCounts[9];
// 0x0066b5a4
extern struct SortNode *PrintListHead;
// 0x0066b5a8
extern unsigned int PrintListBytesUsed;
// 0x0066b5ac
extern unsigned int UnusedPrintListCounter;
// 0x0066b5b0
extern DDSURFACEDESC ZBufferSavedSurfaceDesc;
// 0x0066b61c
extern void *CurrentColorRamp;
// 0x0066b620
extern RECT ZBufferSavedSoftPrintClip;
// 0x0066b630
extern void *CurrentTexture;
// 0x0066be40
extern unsigned int ZBufferWidth;
// 0x0066be44
extern unsigned int ZBufferHeight;
// 0x0066be48
extern unsigned int Render3DTargetHeight;
// 0x0066be4c
extern unsigned int ZBufferStride;
// 0x0066be50
extern struct Image *ZBufferSourceImage;
// 0x0066be54
extern unsigned short ZBufferRenderPixels[0x4b002];
// 0x00701e58
extern unsigned int Render3DTargetPitch;
// 0x00701e5c
extern unsigned char *ZBuffer;
// 0x00701e60
extern unsigned int Render3DTargetWidth;
// 0x00701e64
extern struct Sprite *ZBufferImageSprite;
// 0x00701e68
extern unsigned short ZBufferImagePixels[0x4b000];
// 0x00797e68
extern unsigned char *Render3DTargetBits;
// 0x00797e6c
extern void *CursorCacheList;
// 0x00797e70
extern float ReciprocalTable[100];
// 0x00798000
extern int ScaledReciprocalTable[100];
// 0x00798190
extern void *TextureNodes[256];
// 0x00798590
extern void *ZBufferImageSpriteReady;
// 0x00798598
extern DDSURFACEDESC SavedSurfaceDesc;
// 0x00798608
extern RECT SavedSoftPrintClip;
// 0x0079861c
extern LPDIRECTDRAWSURFACE ScaledSpriteScratch;
// 0x00798620
extern int ScaledSpriteScratchMade;
// 0x00798624
extern void *MasterDirList;
// 0x00798628
extern void *MasterVolList;
// 0x0079862c
extern void *AllResVolEntries;
// 0x00798630
extern unsigned int SavedClipLeft;
// 0x00798634
extern unsigned int SavedClipTop;
// 0x00798638
extern unsigned int SavedClipRight;
// 0x0079863c
extern unsigned int SavedClipBottom;
// 0x00798648
extern unsigned int FreePlayRestoringSelection;
// 0x0079864c
extern struct IconNode *FreePlayStartIcon;
// 0x00798650
extern unsigned int FreePlaySelectedCount;
// 0x00798660
extern unsigned int ProgressLevelPicked;
// 0x00798664
extern unsigned int ProgressShowsTutorials;
// 0x00798668
extern unsigned int ProgressScreenToggled;
extern unsigned long ProgressLastClickTicks;
// 0x00798674
extern struct Sprite *PuOkOnSprite;
// 0x00798678
extern struct Sprite *PuOkSprite;
// 0x0079867c
extern struct Sprite *PopUpCloseSprite;
// 0x00798680
extern struct Sprite *PuCloseOnSprite;
// 0x00798684
extern struct Sprite *ClosePopUpSprite;
// 0x00798688
extern struct Sprite *ClosePopUpOnSprite;
// 0x0079868c
extern struct Sprite *RegDeleteOnSprite;
// 0x00798690
extern struct Sprite *RegDeleteSprite;
// 0x00798694
extern struct Sprite *RegProfileOff1Sprite;
// 0x00798698
extern struct Sprite *RegProfileOff2Sprite;
// 0x0079869c
extern struct Sprite *RegProfileOff3Sprite;
// 0x007986a0
extern struct Sprite *RegProfileOff4Sprite;
// 0x007986a4
extern struct Sprite *RegProfileOff5Sprite;
// 0x007986a8
extern struct Sprite *RegProfileOff6Sprite;
// 0x007986ac
extern struct Sprite *RegProfileOff7Sprite;
// 0x007986b0
extern struct Sprite *RegProfileOff8Sprite;
// 0x007986b4
extern struct Sprite *RegProfileOnSprite;
// 0x007986b8
extern struct Sprite *RegDeletePopUpSprite;
// 0x007986bc
extern struct Sprite *RegDiffPopUpSprite;
// 0x007986c0
extern struct Sprite *RegEasyOnSprite;
// 0x007986c4
extern struct Sprite *RegEasyOffSprite;
// 0x007986c8
extern struct Sprite *RegMidOnSprite;
// 0x007986cc
extern struct Sprite *RegMidOffSprite;
// 0x007986d0
extern struct Sprite *RegHardOnSprite;
// 0x007986d4
extern struct Sprite *RegHardOffSprite;
// 0x007986d8
extern struct IconNode *PopUpOkIcon;
// 0x007986dc
extern struct IconNode *PopUpCloseIcon;
// 0x007986e0
extern struct IconNode *AcceptIcon;
// 0x007986e4
extern unsigned int DeletePopUpShown;
// 0x007986e8
extern unsigned int NewProfilePopUpShown;
// 0x007986f0
extern unsigned int TempProfileNameSet;
// 0x007986f4
extern void *ProfileSavedCancelKeyHandler;
// 0x007986f8
extern void *ProfileSavedOkKeyHandler;
// 0x00798700
extern unsigned int NewSaveGamePopUpShown;
// 0x00798704
extern struct Sprite *RegSaveSlotOnSprite;
// 0x00798708
extern struct Sprite *RegSaveOff1Sprite;
// 0x0079870c
extern struct Sprite *RegSaveOff2Sprite;
// 0x00798710
extern struct Sprite *RegSaveOff3Sprite;
// 0x00798714
extern struct Sprite *RegSaveOff4Sprite;
// 0x00798718
extern struct Sprite *RegSaveOff5Sprite;
// 0x0079871c
extern struct Sprite *RegSaveOff6Sprite;
// 0x00798720
extern struct Sprite *RegSaveOff7Sprite;
// 0x00798724
extern struct Sprite *RegSaveOff8Sprite;
// 0x00798728
extern struct Sprite *SaveTypeNormalSprite;
// 0x0079872c
extern struct Sprite *SaveTypeFreeSprite;
// 0x00798730
extern struct Sprite *RegCornerMaskSprite;
// 0x00798734
extern void *SavedGameList;
// 0x00798738
extern int SaveNameTextWidth;
// 0x0079873c
extern void *OptionsSavedCancelKeyHandler;
// 0x00798740
extern void *OptionsSavedOkKeyHandler;
// 0x00798748
extern struct IconNode *SpeechVolumeMarkerIcon;
// 0x0079874c
extern struct IconNode *FxVolumeMarkerIcon;
// 0x00798750
extern struct IconNode *MusicVolumeMarkerIcon;
// 0x00798754
extern unsigned int ExitDialogAfterLongPlay;
// 0x00798764
extern struct Sprite *PrintInfoSprite;
// 0x00798768
extern int CertificatePrintResultTimer;
// 0x0079876c
extern unsigned int CertificatePrintCountdown;
// 0x00798770
extern unsigned int CertificateScreenActive;
// 0x00798777
extern char IntervalMovieNameEnd;
// 0x00798778
extern char IntervalMovieName[0x100];
// 0x00798878
extern unsigned int IntervalScreenIsBriefing;
// 0x0079887c
extern unsigned int IntervalTextLastLine;
// 0x00798880
extern unsigned int IntervalHintCount;
// 0x00798884
extern unsigned int IntervalHintIndex;
// 0x00798888
extern unsigned int IntervalHintBubbleEndTime;
// 0x00798890
extern void *ProfileListHead;
// 0x00798894
extern int ProfileNameTextWidth;
// 0x00798898
extern unsigned int AviLockBytes1;
// 0x0079889c
extern unsigned int AviLockBytes2;
// 0x007988a0
extern unsigned int SfxDSVolume;
// 0x007988a4
extern void *AviLockPtr1;
// 0x007988a8
extern void *AviLockPtr2;
// 0x007988b0
extern void *SoundHwnd;
// 0x007988bc
extern volatile unsigned int MusicThreadReady; /* the music thread has finished starting (polled) */
// 0x007988c0
extern unsigned int SoundAvailable;
// 0x007988c4
extern unsigned int SfxMuted;
// 0x007988c8
extern unsigned int PauseNewSamples;
// 0x007988cc
extern void *SampleListHead;
// 0x0079a694
extern LEGO_EXPORT unsigned int DMusicInitialised;
// 0x0079a698
extern void *MusicThread;
// 0x0079a6a0
extern void *MusicCommandEvent;
// 0x0079a6a4
extern unsigned int MusicCommand;
// 0x0079a6a8
extern int NextMusicTheme;
// 0x0079a6ac
extern int CurrentMusicTheme;
// 0x0079a7d0
extern unsigned int SpeechVolume;
// 0x0079a7d8
extern int SpeechRawReadPos;
// 0x0079a7dc
extern int SpeechRawWritePos;
// 0x0079a7e0
extern unsigned int SpeechChunkIndex;
// 0x0079a7e4
extern unsigned int SpeechChunkByteCounts[20];
// 0x0079a834
extern int SpeechPcmReadPos;
// 0x0079a838
extern int SpeechPcmWritePos;
// 0x0079a83c
extern unsigned int SpeechFlags;
// 0x0079a840
extern int SpeechPageIndex;
// 0x0079a844
extern int SpeechPagesQueued;
// 0x0079a848
extern void *SpeechSoundBuffer;
// 0x0079a84c
extern unsigned int SpeechState;
// 0x0079a850
extern void *strings[10];
// 0x0079a878
extern unsigned int UnusedAfterStrings[6];
// 0x0079a890
extern unsigned int GameTimerPaused;
// 0x0079a894
extern unsigned int GameTimerPausedTicks;
// 0x0079a898
extern unsigned int GameTimerBaseTicks;
// 0x0079a89c
extern unsigned int GameTimerPausedFrame;
// 0x0079a8a0
extern unsigned int GameTimerBaseFrame;
// 0x0079a8a8
extern LEGO_EXPORT struct Bloke *GardenerList;
// 0x0079a8ac
extern LEGO_EXPORT struct Bloke *MechanicList;
// 0x0079a8b0
extern struct WorkOrder *GardenerOrderHead;
// 0x0079a8b4
extern struct WorkOrder *GardenerOrderTail;
// 0x0079a8b8
extern int GardenerOrderCount;
// 0x0079a8bc
extern int GardenerCount;
// 0x0079a8c0
extern struct WorkOrder *MechanicOrderHead;
// 0x0079a8c4
extern struct WorkOrder *MechanicOrderTail;
// 0x0079a8c8
extern int MechanicOrderCount;
// 0x0079a8cc
extern int MechanicCount;
// 0x0079a8d0
extern unsigned int CastlePlacedFlag;
// 0x0079a8d4
extern struct RepairOrder *RepairOrderList;
// 0x0079abfc
extern struct Element *NormalPathTilesElement;
// 0x0079ac04
extern unsigned int SpeechDataSize;
// 0x0079ac08
extern unsigned char *SpeechAcmDstBuffer;
// 0x0079ac0c
extern void *SpeechAcmSrcBuffer;

// 0x0079ac20
extern unsigned char SpeechRawBuffer[0x10000];
// 0x007aac24
extern int SpeechConvertedUsed;
// 0x007aac40
/* The speech stream's ACMSTREAMHEADER (see struct AcmHeader; its lengths are compared signed). */
struct AcmHdr {
    unsigned int cbStruct;
    unsigned int fdwStatus;
    UINT_PTR dwUser;
    unsigned char *pbSrc;
    int cbSrcLength;
    int cbSrcLengthUsed;
    UINT_PTR dwSrcUser;
    unsigned char *pbDst;
    int cbDstLength;
    int cbDstLengthUsed;
    UINT_PTR dwDstUser;
    unsigned int dwReservedDriver[ACM_DRV_RESERVED];
};
extern struct AcmHdr SpeechAcmHeader;

// 0x007aaca0
extern unsigned char SpeechPcmBuffer[0x20000];
// 0x007caca0
extern int SpeechAcmSrcSize;
// 0x007caca4
extern int SpeechConvertedSize;
// 0x007caca8
extern unsigned int SpeechFileHandle;
// 0x007cacac
extern unsigned int SpeechBytesRemaining;
// 0x007cacb0
extern WAVEFORMATEX *SpeechSourceFormat;
// 0x007cacb8
extern void *SpeechAcmStream;
// 0x007cacc0
extern WAVEFORMATEX SpeechPcmFormat;
// 0x007cacb4
extern unsigned int SpeechDataOffset;
// 0x007cacd4
extern LEGO_EXPORT unsigned int FrameNumber;
// 0x007cacd8
extern struct DirectMusicLoader *DMusicLoader;
// 0x007cacdc
extern struct DirectMusicPerformance *DMusicPerformance;
// 0x007cace0
extern unsigned int DSoundCaps[0x18];
// 0x007cad40
extern void *DSound;
// 0x007cad44
extern struct DirectMusicComposer *DMusicComposer;
// 0x007cad48
extern unsigned int MusicThreadId;
// 0x007cad4c
extern void *DMusicSoundBuffer;
// 0x007cad60
extern struct ProfileData TempProfile;
// 0x007cae80
extern char IntervalReportSpeechPrefix[0x100];
// 0x007caf80
extern struct Sprite *RepHint1Sprite;
// 0x007cafa0
extern char *IntervalTextLines[104];
// 0x007cb140
extern char *IntervalHintLines[32];
// 0x007cb1c0
extern struct IconNode *RepHint1Icon;
// 0x007cb1c4
extern struct Sprite *RepHint2Sprite;
// 0x007cb1e0
extern char IntervalHintSpeechPrefix[0x100];
// 0x007cb2e0
extern struct IconNode *TitlePreviousPageIcon;
// 0x007cb2e4
extern struct IconNode *TitleNextPageIcon;
// 0x007cb2f0
extern struct EditState TitleSavedEditMode; /* EditMode saved by SaveScreenState */
// 0x007cb300
extern struct ScreenMode TitleSavedScreenMode; /* FrontEndScreenMode saved */
// 0x007cb30c
extern unsigned int TitleSavedScreenFlag; /* FrontEndSubScreenActive saved */
// 0x007cb310
extern unsigned int SavedGameScreenFromExitDialog;
// 0x007cb314
extern char SavedSpeechVolume;
// 0x007cb315
extern char SavedFxVolume;
// 0x007cb318
extern unsigned int ExitDialogAltText;
// 0x007cb31c
extern char SavedMusicVolume;
// 0x007cb320
extern unsigned int ExitCheckBoxShown;
// 0x007cb324
extern unsigned int SavedGameScreenFromTitle;
// 0x007cb328
extern unsigned int LoadMode;
// 0x007cb340
extern char ProfileListNewLabel[0x20];
// 0x007cb360
extern struct IconNode *DeleteIcon;
// 0x007cb380
extern struct IconNode *TutorialIcons[5];
// 0x007cb394
extern unsigned int HighestUnlockedLevelIndex;
// 0x007cb398
extern struct Sprite *FreePlayTickSprite;
// 0x007cb39c
extern struct PanelNode *FreePlayPanelList4;
// 0x007cb3a0
extern unsigned int FreePlayCostTotal;
// 0x007cb3a4
extern struct PanelNode *FreePlayPanelList2;
// 0x007cb3a8
extern struct Sprite *FreePlayDown4Sprite;
// 0x007cb3ac
extern struct Sprite *FreePlayDown2Sprite;
// 0x007cb3b0
extern struct Sprite *FreePlayDown3Sprite;
// 0x007cb3b4
extern struct Sprite *FreePlayDown1Sprite;
// 0x007cb3b8
extern struct PanelNode *FreePlayPanelList3;
// 0x007cb3bc
extern struct Element *LoadedObjDescElement;
// 0x007cb3c0
extern struct Sprite *FreePlayUp2Sprite;
// 0x007cb3c4
extern struct Sprite *FreePlayUp1Sprite;
// 0x007cb3c8
extern struct Sprite *FreePlayUp4Sprite;
// 0x007cb3cc
extern struct Sprite *FreePlayUp3Sprite;
// 0x007cb3d0
extern struct PanelNode *FreePlayPanelList1;
// 0x007cb3d4
extern struct Sprite *FreePlayCoverSprite;
// 0x007cb3e0
extern struct ObjTableEntry ObjInstanceTable[128];
// 0x007cb3e2
extern struct ObjTableEntry ObjInstanceTableCounts[128];
// 0x007cb5e0
extern struct ObjTableEntry ColorGreenBits;
// 0x007cb600
extern unsigned char PrintListPool[0x32000];
// 0x007fd600
extern struct SortNode *SortCursor;
// 0x007fd610
extern struct LibraryNode NewObjectLibraryNode;
// 0x007fd620
extern LEGO_EXPORT void *NewObjectPtr;
// 0x007fd624
extern void *PathControlObject;
// 0x007fd630
extern unsigned int MidiTimerId;
// 0x007fd634
extern void *CurrentMidiFile;
// 0x007fd638
extern HMIDIOUT MidiOutHandle;
// 0x007fd640
extern struct ResVolume *ResourceVolumes[4];
// 0x007fd660
extern struct Element *SaveGameElements[256];
// 0x007fda60
extern struct BlokeSave BlokeSaveBuffer;
// 0x007fdb84
extern int SaveGameElementCount;
// 0x007fdb88
extern unsigned int SelectElementMask;
// 0x007fdba0
extern char ObjDescSpriteName[0x100];

// 0x007fdca0
extern int SelectedElementIndex;
// 0x007fdca4
extern unsigned int ScriptPromptStringIndex;
// 0x007fdca8
extern float PerfCounterScale;
// 0x007fdcc0
extern struct Sprite *LegolandThemeOnSprite;
// 0x007fdcc4
extern struct Sprite *WesternThemeOnSprite;
// 0x007fdcc8
extern struct Sprite *CastleThemeOnSprite;
// 0x007fdccc
extern struct Sprite *AdventurersThemeOnSprite;
// 0x007fdcd0
extern struct Sprite *IfPathIconPressedSprite;
// 0x007fdcd4
extern struct Sprite *IfQueryIconPressedSprite;
// 0x007fdcd8
extern struct Sprite *IfEraserIconPressedSprite;
// 0x007fdcdc
extern struct Sprite *IfMapIconPressedSprite;
// 0x007fdce0
extern struct Sprite *IfOptionsIconPressedSprite;
// 0x007fdd00
extern unsigned int ButtonFlashStates[9];
// 0x007fdd40
extern struct Sprite *LegolandThemeOffSprite;
// 0x007fdd44
extern struct Sprite *WesternThemeOffSprite;
// 0x007fdd48
extern struct Sprite *CastleThemeOffSprite;
// 0x007fdd4c
extern struct Sprite *AdventurersThemeOffSprite;
// 0x007fdd50
extern struct Sprite *IfPathIconSprite;
// 0x007fdd54
extern struct Sprite *IfQueryiconSprite;
// 0x007fdd58
extern struct Sprite *IfEraserIconSprite;
// 0x007fdd5c
extern struct Sprite *IfMapiconSprite;
// 0x007fdd60
extern struct Sprite *IfOptionsIconSprite;
// 0x007fdd70
extern struct IconNode *ThemeIcons[4];
// 0x007fdd80
extern unsigned char BuildSidePanelState;
// 0x007fdd84
extern unsigned int BuildSidePanelSlideInPending;
// 0x007fdd88
extern unsigned int BuildSidePanelViewportReserved;
// 0x007fdd8c
extern unsigned char BuildSidePanelOriginX;
// 0x007fdda0
extern unsigned char KeyboardState[256];
// 0x007fdea4
extern struct IconNode *AddMechanicsIcon;
// 0x007fdea8
extern struct IconNode *PopUpInfoOkIcon;
// 0x007fdeac
extern struct Sprite *IFullSprite;
// 0x007fdeb0
extern struct Sprite *ObjectNoRepair1Sprite;
// 0x007fdecc
extern int PopUpInfoX;
// 0x007fded0
extern int PopUpInfoY;
// 0x007fded4
extern struct NewObjTable NewObjects;
// 0x007fdf7c
extern struct Ride *PopUpInfoRide;
// 0x007fdf80
extern struct InfoObjData *PopUpInfoData;
// 0x007fdf84
extern unsigned char *PopUpInfoTile;
// 0x007fdf88
extern unsigned short PopUpInfoHoverValue;
// 0x007fdf8c
extern void *PopUpInfoBloke; /* object the info popup is showing */
// 0x007fdf90
extern unsigned int PopUpVisitorField1c;
// 0x007fdf94
extern unsigned int PopUpVisitorField20;
// 0x007fdf98
extern unsigned int PopUpFollowingVisitor;
// 0x007fdf9c
extern int PopUpInfoSubjectType;
// 0x007fdfa0
extern unsigned int PopUpInfoState;
// 0x007fdfa4
extern unsigned int PopUpDeleteConfirm;
// 0x007fdfa8
extern unsigned int PopUpInfoResizePending;
// 0x007fdfac
extern unsigned char PopupInfoLineCount;
// 0x007fdfb0
extern struct Element *PottingShedHandle;
// 0x007fdfb4
extern struct Element *MechanicsHutHandle;
// 0x007fdfb8
extern struct Element *PathControlHandle;
// 0x007fdfbc
extern struct Element *Entrance1Handle;
// 0x007fdfc0
extern struct IconNode *ClosePopUpIcon;
// 0x007fdfc4
extern struct IconNode *NextPopUpIcon;
// 0x007fdfc8
extern struct Sprite *ISadSprite;
// 0x007fdfcc
extern struct IconNode *PopUpDeleteIcon;
// 0x007fdfd0
extern struct Sprite *IHungrySprite;
// 0x007fdfd8
extern struct IconNode *PuCornerMaskIcon;
// 0x007fdfdc
extern struct IconNode *DeleteObjectIcon;
// 0x007fdfe0
extern struct IconNode *AddGardenerIcon;
// 0x007fdfe4
extern struct Sprite *INormSprite;
// 0x007fdfe8
extern struct IconNode *PrevPopUpIcon;
// 0x007fdff0
extern struct Bloke *WorkerOnMouse;
// 0x007fdff4
extern int WorkerOldX;
// 0x007fdff8
extern int WorkerOldY;
// 0x007fdffc
extern unsigned int WorkerOnMouseType;
// 0x007fe000
extern struct IconNode *CBCloseIcon;
// 0x007fe004
extern struct Sprite *ObjectRepairOkSprite;
// 0x007fe008
extern struct Sprite *IPeckishSprite;
// 0x007fe010
extern int PopUpMessageX;
// 0x007fe014
extern int PopUpMessageY;
// 0x007fe018
extern struct Sprite *IHappySprite;
// 0x007fe020
extern RECT ScreenRect;
// 0x007fe040
extern unsigned int AdvisorHelpFlags;
// 0x007fe044
extern unsigned int AdvisorClearOnObjectiveEvent;
// 0x007fe048
extern char *AdvisorHelpText;
// 0x007fe04c
extern unsigned int AdvisorHelpStartTime;
// 0x007fe050
extern unsigned int AdvisorHelpClearTime;
// 0x007fe054
extern unsigned int LastLevelScriptRunTime;
// 0x007fe114
extern unsigned char LegolandCommonThemeCount;
// 0x007fe115
extern unsigned char WesternThemeCount;
// 0x007fe116
extern unsigned char CastleThemeCount;
// 0x007fe117
extern unsigned char AdventurersThemeCount;
// 0x007fe120
extern char *ScriptStringTable[512];
// 0x007fe920
extern unsigned int HelpShownTick;
// 0x007fe930
extern unsigned char ObjectiveCounters[10];
// 0x007fe994
extern unsigned int ScriptTimeBase;
// 0x007fe998
extern unsigned short LLSColourMask;
// 0x007fe9a8
extern unsigned short *MousePixelAddress;
// 0x007fe9c0
extern struct Sprite *PointerSprites[9];
// 0x007fea10
extern unsigned int SpanRowsLeft;
// 0x007fea30
extern RECT WatchRect;
// 0x007fea40
extern unsigned int SoftPrintSourceBits;
// 0x007fea44
extern unsigned int StoredTransparentColour;
// 0x007fea48
extern LEGO_EXPORT unsigned int FramesPerSecond;
// 0x007feb14
extern unsigned int MousePixelCovered;
// 0x007feb18
extern unsigned int SpanDstRow;
// 0x007febb8
extern unsigned short RenderOrderFirstAnchor;
// 0x007febc0
extern LEGO_EXPORT struct Cursor EditCursor;
// 0x008003f4
extern struct Element *LevelMapHandle;
// 0x008003f8
extern unsigned int BasicTileSlot0;
// 0x00800400
extern LEGO_EXPORT RECT ObjectPartArray[256];
// 0x00801400
extern LEGO_EXPORT struct MapElement **GameMap;
// 0x00801404
extern struct Element *BridgesHandle;
// 0x00801408
extern unsigned int MapRenderOrderCount;
// 0x0080140c
extern struct Element *LevelTileMapHandle;
// 0x00801410
extern struct Element *OverlayILFHandle;
// 0x00801420
extern struct DeferredSprite DeferredSprites[100];
// 0x00801a60
extern int Bridge1OverlayOffsetX;
// 0x00801a64
extern int Bridge1OverlayOffsetY;
// 0x00801a68
extern struct Element **MapObjectElements; /* the level's object classes */
// 0x00801a6c
extern int *BasicTiles1Data;
// 0x00801a70
extern struct MapTileSet **MapTileSets; /* the level's tile object sets */
// 0x00801a74
extern int BaseMapEntryCount;
// 0x00801a80
extern struct MapRect RectList[10];
// 0x00801b20
extern unsigned int BasicTileSlot1;
// 0x00801b24
extern LEGO_EXPORT unsigned int ObjectPartCount;
// 0x00801b40
extern int ObjectPartKey[256];
// 0x00801b28
extern int MapObjectElementCount;
// 0x00801f40
extern LEGO_EXPORT struct TileSpriteEntry TileSpriteInfo[2048];
// 0x00805f40
extern int Bridge0OverlayOffsetX;
// 0x00805f44
extern int Bridge0OverlayOffsetY;
// 0x00805f48
extern unsigned int BasicTileSlot3;
// 0x00805f60
extern LEGO_EXPORT struct Sprite *TileSpriteArray[2048];
// 0x00807f60
extern struct MapRenderOrderEntry MapRenderOrderList[4096];
// 0x0080ff60
extern unsigned int BasicTileSlot2;
// 0x0080ff64
extern struct Element *CastleObjElem;
// 0x0080ff68
extern unsigned int BasicTileSlot4;
// 0x0080ff6c
extern void *DragRemoveClass;
extern unsigned int MapScreenClosePending;
// 0x0080ff74
extern LEGO_EXPORT unsigned int NEWFLC_AutoPlay;
// 0x0080ff78
extern LEGO_EXPORT unsigned char NEWFLC_PauseType;
// 0x0080ff80
extern struct ScreenMode FrontEndScreenMode;
// 0x0080ffa0
extern struct ScreenState CurrentProfile;
// 0x008100c0
extern char LevelEndMovieName[0x80];
// 0x00810140
extern unsigned int LevelLoadedFromSave;
// 0x00810144
extern unsigned int QueryCursorKeepPath;
// 0x00810148
extern struct Sprite *SPRITE_TitleScreenBk;
// 0x0081014c
extern LEGO_EXPORT unsigned char NEWFLC_ID[20];
// 0x00810160
extern LEGO_EXPORT struct Cursor QueryCursor;
// 0x008119a0
extern LEGO_EXPORT unsigned int NEWFLC_CheckDuplicate;
// 0x008119a4
extern unsigned int FrameCounter;
// 0x008119a8
extern LEGO_EXPORT unsigned int NEWFLC_BuffSize;
// 0x008119ac
extern LEGO_EXPORT unsigned short NEWFLC_Repeat;
// 0x008119b0
extern LEGO_EXPORT struct EditState EditMode;
// 0x008119bc
extern unsigned int MapScreenOpenPending;
struct MapMarker {
    unsigned int x, y;
};
// 0x008119c0: the map screen's markers, 32 rows of 32. The code takes the last row for the table's end (the loop
// over the rows), clears the 32 rows at once, and writes to row 31: one array, or another linker than MSVC's puts
// the last row elsewhere (the map screen walked through other globals and hung, in the web build).
extern struct MapMarker MapSpannerMarks[32][32];
// 0x008138c0
#define MapSpannerMarksLastRow (MapSpannerMarks[31])
// 0x008139c0
extern int MapViewHeight;
// 0x008139c4
extern int MapViewWidth;
// 0x008139c8
extern int MapViewX;
// 0x008139cc
extern int MapViewY;
// 0x008139e0
extern struct Sprite *MoodIconSprites;
// 0x008139e4
extern struct Sprite *MiHungrySprite;
// 0x008139e8
extern struct Sprite *MiHappySprite;
// 0x008139ec
extern struct Sprite *MiSadSprite;
// 0x008139f0
extern struct Sprite *MiHomeSprite;
// 0x008139f4
extern struct Sprite *MiEatSprite;
// 0x008139f8
extern struct Sprite *GreatSprite;
// 0x008139fc
extern struct Sprite *PoorSprite;
// 0x00813a00
extern struct Sprite *FavouriteSprite;
// 0x00813a04
extern struct Sprite *OpinionSprite;
// 0x00813a08
extern struct Sprite *MiBoredSprite;
// 0x00813a0c
extern void *SpeechBubbleData;
// 0x00813a10
extern unsigned int DebugAllocatedBytes;
// 0x00813a18
extern unsigned long LastFrameTickCount;
// 0x00813a2c
extern int FrameTicksAccumulated;
// 0x00813a34
extern unsigned short DragQueryObjectId;
// 0x00813a38
extern unsigned int DragFootprintWidth;
// 0x00813a3c
extern unsigned int DragFootprintHeight;
// 0x00813a40
extern LEGO_EXPORT unsigned int GamePad;
// 0x00813a44
extern struct Point MousePos;
// 0x00813a4c
extern unsigned int MapLeftButtonMask;
// 0x00813a50
extern unsigned int MapLeftClickFlags;
// 0x00813a54
extern unsigned int MiddleButtonMask;
// 0x00813a5c
extern unsigned int MapRightButtonMask;
// 0x00813a60
extern unsigned int MapRightClickFlags;
// 0x00813a64
extern unsigned int MouseTileX;
// 0x00813a68
extern unsigned int MouseTileY;
// 0x00813a6c
extern int FootprintWidth;
// 0x00813a70
extern int FootprintHeight;
// 0x00813a74
extern int DragStartTileX;
// 0x00813a78
extern int DragStartTileY;
// 0x00813a7c
extern int DragEndTileX;
// 0x00813a80
extern int DragEndTileY;
// 0x00813a94
extern unsigned int PrevHeldButtons;
// 0x00813a98
extern unsigned int ScrollUpKeyMask;
// 0x00813a9c
extern unsigned int ScrollUpKeyState;
// 0x00813aa0
extern unsigned int ScrollRightKeyMask;
// 0x00813aa4
extern unsigned int ScrollRightKeyState;
// 0x00813aa8
extern unsigned int ScrollDownKeyMask;
// 0x00813aac
extern unsigned int ScrollDownKeyState;
// 0x00813ab0
extern unsigned int ScrollLeftKeyMask;
// 0x00813ab4
extern unsigned int ScrollLeftKeyState;
// 0x00813ab8
extern unsigned int WatchedButtonMask;
// 0x00813abc
extern unsigned int WatchedButtonState;
// 0x00813ac0
extern unsigned int LeftButtonMask;
// 0x00813ac4
extern unsigned int MouseClickFlags;
// 0x00813ac8
extern unsigned int RightButtonMask;
// 0x00813acc
extern unsigned int RightClickFlags;
// 0x00813ad0
extern unsigned int CancelKeyMask;
// 0x00813ad4
extern unsigned int CancelKeyState;
// 0x00813ad8
extern unsigned int OkKeyMask;
// 0x00813adc
extern unsigned int OkKeyState;
// 0x00813ae0
extern unsigned int LastPointerActivityTicks;
// 0x00813af0
extern int DragFootprintX0;
// 0x00813af4
extern int DragFootprintY0;
// 0x00813af8
extern int DragFootprintX1;
// 0x00813afc
extern int DragFootprintY1;
// 0x00813b00
extern LEGO_EXPORT struct CtrlBuffer *CONTROLLERBUFFER;
// Root of the last CDFS volume found by FindCdDriveWithVolume (four bytes, including NUL).
// 0x00813b04
extern char CdDrivePath[4];
// 0x00813b08
extern unsigned int CurrentBlokeLetter; /* letter of the bloke being processed */
// 0x00813b20
extern unsigned char PaletteReadBuffer[0x300];
// 0x00813e20
extern unsigned short Palette16Bit[256];
// 0x00814020
extern unsigned char ColourLookupTable[0x8000];
// 0x0081c028
extern struct Sprite *AppBarSprite;
// 0x0081c02c
extern struct Sprite *NextPageSprite;
// 0x0081c030
extern struct Sprite *AppBarMarkerSprite;
// 0x0081c034
extern struct Sprite *NextPageLitSprite;
// 0x0081c038
extern unsigned int AppraisalScreenActive;
// 0x0081c040
extern struct Sprite *AppTickSprites[5];
// 0x0081c054
extern struct Sprite *AppCrossSprites[5];
// 0x0081c068
extern struct Sprite *AppBulletSprites[5];
// 0x0081c07c
extern unsigned int AppraisalPageChanged;
// 0x0081c080
extern struct Sprite *PreviousPageSprite;
// 0x0081c084
extern struct Sprite *PreviousPageLitSprite;
// 0x0081c088
extern unsigned int AdvisorAnimStateParam;
// 0x0081c08c
extern void *AdBlinkAnim;
// 0x0081c090
extern void *AdWobbleAnim;
// 0x0081c094
extern void *AdLRAnim;
// 0x0081c098
extern void *AdPhoneDownAnim;
// 0x0081c09c
extern unsigned int NextAdvisorAnimStateParam;
// 0x0081c0a0
extern void *AdPhoneAnim;
// 0x0081c0a4
extern void *AdPhoneGestureAnim;
// 0x0081c0c0
extern int TextureSizes[0x200];
// 0x0081c4c0
extern unsigned int BmpPalette[0x100];
// 0x0081c8c0
extern struct LocFile *VisitorLocData;
// 0x0081c8c4
extern struct LocFile *TracyLocData;
// 0x0081c8c8
extern struct LocFile *GeoffLocData;
// 0x0081c8cc
extern void *RiderSearchCursor;
// 0x0081c8d0
extern unsigned int ViewportLeft;
// 0x0081c8d4
extern unsigned int ViewportTop;
// 0x0081c8d8
extern unsigned int ViewportRight;
// 0x0081c8dc
extern unsigned int ViewportBottom;
// 0x0081c8e0
extern char FileDialogSelectedName[512];
// 0x0081cae0
extern struct Sprite *ZoomerSprite;
// 0x0081cae8
extern int PlaneRideDrawOffsetX;
// 0x0081caec
extern int PlaneRideDrawOffsetY;
// 0x0081caf0
extern struct Ride *PottingShedRide;
// 0x0081caf4
extern struct Ride *MechanicsHutRide;
// 0x0081cb00
extern struct Sprite *SaloonMatte1Sprite;
// 0x0081cb04
extern struct Sprite *SaloonMatte2Sprite;
// 0x0081cb08
extern struct Sprite *GStoreMatteSprite;
// 0x0081cb0c
extern struct Sprite *JailCellMaskSprite;
// 0x0081cb10
extern struct Ride *ActiveWesternTownRide;
// 0x0081cb14
extern struct Ride *SheriffBuilding;
// 0x0081cb18
extern struct Sprite *LegoShop1MatteSprite;
// 0x0081cb1c
extern struct Ride *SaloonBuilding;
// 0x0081cb20
extern struct Sprite *LegoShop2MatteSprite;
// 0x0081cb24
extern struct Sprite *GStoreMatte2Sprite;
// 0x0081cb28
extern struct Sprite *ExplorersInstituteMatteSprite;
// 0x0081cb2c
extern struct Ride *BankBuilding;
// 0x0081cb30
extern struct Ride *GeneralStoreBuilding;
// 0x0081cb34
extern struct Sprite *BankMatteSprite;
// 0x0081cb38
extern struct Sprite *SherifshutMatteSprite;
// 0x0081cb3c
extern struct Ride *LegoShop1Building;
// 0x0081cb40
extern struct Ride *LegoMediaShopBuilding;
// 0x0081cb44
extern struct Ride *ExplorersInstituteBuilding;
// 0x0081cb48
extern struct Sprite *LegMediaShopMask1Sprite;
// 0x0081cb4c
extern struct Ride *LegoShop2Building;
// 0x0081cb50
extern struct Sprite *LegMediaShopMask2Sprite;
// 0x0081cb54
extern struct Ride *ActiveJungleCruiseRide;
// 0x0081cb58
extern struct TileMap *BoatingSchoolTileMap;
// 0x0081cb5c
extern struct Sprite *JungMaskSprite;
// 0x0081cb60
extern struct Ride *JungleCruiseRide;
// 0x0081cb64
extern struct Ride *JungleCruiseInstanceClass;
// 0x0081cb68
extern struct Sprite *BrijMaskSprite;
// 0x0081cb6c
extern struct Sprite *MFish2Sprite;
// 0x0081cb70
extern struct Ride *MonkeyTreeRide;
// 0x0081cb74
extern struct Ride *MFish2Ride;
// 0x0081cb80
extern struct Point JungleCruiseSeatOffsets[3][16];
// 0x0081cd00
extern struct SpriteSet *JungleCruiseBoats;
// 0x0081cd04
extern void *FlowersRide;
// 0x0081cd08
extern void *HedgeObjectClass;
// 0x0081cd0c
extern struct Ride *CastleBBQRide;
// 0x0081cd10
extern struct Sprite *CastleBBQLayer;
// 0x0081cd14
extern struct Ride *FoodcartDrinkRide;
// 0x0081cd18
extern struct Ride *SharkCafeRide;
// 0x0081cd1c
extern struct Ride *OctopusCafeRide;
// 0x0081cd20
extern struct Sprite *R2TowermSprite;
// 0x0081cd28
extern struct Sprite *RestMaskMainSprite;
// 0x0081cd2c
extern struct Sprite *EateryFxInner;
// 0x0081cd30
extern struct Ride *Restaurant2Ride;
// 0x0081cd34
extern struct Sprite *R2FdoormSprite;
// 0x0081cd38
extern struct Ride *ActiveEateryRide;
// 0x0081cd3c
extern struct Ride *FoodcartFoodRide;
// 0x0081cd40
extern struct Ride *Restaurant1Ride;
// 0x0081cd44
extern struct Ride *ChuckWagonRide;
// 0x0081cd48
extern struct Sprite *R2Fdoorm1Sprite;
// 0x0081cd60
extern struct Sprite *OctTentASprite;
// 0x0081cd64
extern struct Sprite *OctTentBSprite;
// 0x0081cd68
extern struct Sprite *OctTentCSprite;
// 0x0081cd6c
extern struct Sprite *OctTentDSprite;
// 0x0081cd70
extern struct Sprite *OctTentESprite;
// 0x0081cd74
extern struct Sprite *OctTentFSprite;
// 0x0081cd78
extern struct Sprite *OctTentGSprite;
// 0x0081cd7c
extern struct Sprite *OctTentHSprite;
// 0x0081cd80
extern struct Sprite *OctoKioskSprite;
// 0x0081cd84
extern struct Sprite *R2BdoormSprite;
// 0x0081cd88
extern struct Sprite *RestMaskLevel1Sprite;
// 0x0081cd8c
extern struct Sprite *RestMaskLevel1aaSprite;
// 0x0081cd90
extern struct Sprite *RestMaskLevel3Sprite;
// 0x0081cd94
extern struct Sprite *RestMaskLevel2Sprite;
// 0x0081cda0
extern struct Sprite *OctTabAASprite;
// 0x0081cda4
extern struct Sprite *OctTabABSprite;
// 0x0081cda8
extern struct Sprite *OctTabBASprite;
// 0x0081cdac
extern struct Sprite *OctTabBBSprite;
// 0x0081cdb0
extern struct Sprite *OctTabCASprite;
// 0x0081cdb4
extern struct Sprite *OctTabCBSprite;
// 0x0081cdb8
extern struct Sprite *OctTabDASprite;
// 0x0081cdbc
extern struct Sprite *OctTabDBSprite;
// 0x0081cdc0
extern struct Sprite *OctTabEASprite;
// 0x0081cdc4
extern struct Sprite *OctTabEBSprite;
// 0x0081cdc8
extern struct Sprite *OctTabFASprite;
// 0x0081cdcc
extern struct Sprite *OctTabFBSprite;
// 0x0081cdd0
extern struct Sprite *OctTabGASprite;
// 0x0081cdd4
extern struct Sprite *OctTabGBSprite;
// 0x0081cdd8
extern struct Sprite *OctTabHASprite;
// 0x0081cddc
extern struct Sprite *OctTabHBSprite;
// 0x0081cde0
extern struct Ride *FoodcartIcecreamRide;
// 0x0081cde4
extern struct Ride *BalloonzRide;
// 0x0081cde8
extern struct Sprite *ZBalloon2Sprite;
// 0x0081cdec
extern void *CoasterPieceToRemove;
// 0x0081ce00
extern struct Cursor FreeSpotCursors[8];
// 0x00828fe0
extern unsigned int TrackPieceCurves[2];
// 0x00829980
extern void *BasicTilesData;
// 0x00829990
extern float CastleViewDir[3];
// 0x0082999c
extern float CastleLightScale;
// 0x008299a0
extern float CastleCameraPos[3];
// 0x008299bc
extern struct FMat4 CastleViewMatrix;
// 0x008299fc
extern struct FMat4 CastleWorldToScreen;
// 0x008299ac
extern int CastleViewRect[4];
// 0x00829a3c
extern struct ListLink CastleLinkList;
// 0x00829a58
extern void (*lego_sqrtf_impl)(void);
// 0x00829a5c
extern void (*lego_rsqrtf_impl)(void);
// 0x00829a60
extern float CastleLightBias;
// 0x00829a64
extern struct Ride *HeightTrack2Ride;

// 0x00829a80
extern struct Footprint CastleCoasterFootprint;
// 0x00829abc
extern struct Element *CastleObjElement;
// 0x00829ae0
extern struct CastleState CastleCoaster;
// 0x004b5b48: the castle object's class (CastleCoaster.field_10 points here): data, then three methods the
// code calls through it (+0x1c CastleFillOpenEnd, +0x20 CastleSetHeights, +0x24 CastleGetCurve).
extern struct PieceClass CastlePieceClass;
// 0x00829bf8
extern struct Ride *CastleCoasterRide;
// 0x00829bfc
extern struct Ride *HeightTrack25Ride;

// 0x00829c00
extern struct Element *CastleDummyElement;
// 0x00829c04
extern struct Sprite *CastleMatteSprite;
// 0x00829c08
extern struct Ride *SquareTrackRide;

// 0x00829c34
extern struct Ride *CastleDummyRide;
// 0x00829c54
extern char *CoasterShadeTableData;
// 0x00829c60
extern void *CoasterShadeTables[1024];
// 0x0082ac60
extern struct FloatArray *VecScratchPool[32];
// 0x0082ad20
/* castle.c: the castle parts' handlers (a row per part class; the table ends where CastleDispatchTableEnd starts) */
struct DispatchRow {
    /* 0x00 */ struct Element *element;
    /* 0x04 */ void (*fn_4)(); /* select (edit mode) */
    /* 0x08 */ void (*fn_8)(); /* calc cursor */
    /* 0x0c */ void (*fn_c)(); /* add object */
    /* 0x10 */ void (*fn_10)(); /* dcalc cursor */
    /* 0x14 */ void (*fn_14)(); /* remove object */
};
extern struct DispatchRow CastleDispatchTable[6];
// 0x0082adb0
extern struct Element *CastleDispatchTableEnd[6];
// 0x0082add0
extern struct LmsFile *CoasterTrainCarLms[3];
// 0x0082ade0
extern struct LmsRec *CoasterTrainCarLfm[3];
// 0x0082adec
extern struct PathSeg *TrainDrawPiece;
// 0x0082adf0
extern struct Ride *BoatingSchoolWaterRide;
// 0x0082adf4
extern struct TileMap *BoatingSchoolTileMapping;
// 0x0082adf8
extern struct Ride *BoatingSchoolMermaidRide;
// 0x0082adfc
extern struct Sprite *BoatingSchoolHullMaskSprite;
// 0x0082ae00
extern void *BoatingSchoolLayer5Sprite;
// 0x0082ae20
extern struct Cursor BoatingSchoolDCursor;
// 0x0082c654
extern struct Sprite *BoatingSchoolRailmSprite;
// 0x0082c658
extern struct Ride *BoatingSchoolRide;
// 0x0082c65c
extern struct SpriteSet *BoatingSchoolBoats;
// 0x0082c668
extern struct Sprite *ZSpiderSprite;
// 0x0082c678
extern void *ZebraCrossingRide;
// 0x0082c67c
struct DSMapEntry {
    int pad_0;
    unsigned short *base;
};
extern struct DSMapEntry *DSchoolMappingData;
// 0x0082c680
struct RoadLights;
extern struct RoadLights *DrivingSchoolLightsData;
// 0x0082c684
extern void *DrivingSchoolRoadsClass;
// 0x0082c688
extern void *LogFlumeInterfaceRide;
// 0x0082c690
extern unsigned short *DSchoolRedPalette;
// 0x0082c694
extern struct Ride *DrivingSchoolRide;
// 0x0082c6b8
extern unsigned short *DSchoolYellowPalette;
// 0x0082c6bc
extern unsigned short *DSchoolBluePalette;
// 0x0082c6c0
extern struct Sprite *DSchoolMatteSprite;
// 0x0082c6e0
extern struct Cursor ExtraEditCursor2;
// 0x0082df20
extern struct Cursor ExtraEditCursor3;
// 0x0082f760
extern struct Cursor ExtraEditCursor1;
// 0x00830f94
extern struct Sprite *DSCarSprite;
// 0x00830f98
extern struct Position *CoptersPos;
// 0x00830f9c
struct DriveTable {
    unsigned char pad_0[0xc];
    int *x;
    int *y;
};
extern struct DriveTable *DSchoolBlueCarData;
// 0x00830fc0
extern LEGO_EXPORT struct Cursor PathCursor;
// 0x00832800
extern LEGO_EXPORT struct MapStats MapStats;
// 0x00832bf0
extern LEGO_EXPORT void *PathSprite;
// 0x0082c6a0
extern struct RideSpriteInfo RideSpriteInfoBuffer;
// 0x00616028
extern struct RideSpriteInfo BalloonzSpriteInfo;
// 0x006160a0
extern struct RideSpriteInfo CarouselSpriteInfo;
// 0x0062fe30
extern struct RideSpriteInfo MechanicsHutSpriteInfo;
// 0x0062fe10
extern struct RideSpriteInfo PottingShedSpriteInfo;

// 0x004ab404
extern float FLOAT_004ab404;

// 0x004b55fc
struct PlaneSet;
extern struct PlaneSet *ClipPlanes;

// 0x004d83b4
extern struct Train *DerivTrain;

// 0x004d83a0
extern struct CurveWalker DerivTrainWalker;

// 0x004b5634
extern float ScalarDerivThirdDiffWeight;

// 0x004b559c
extern float TrainCarSeatX[3];

// 0x004b55a8
extern float TrainSeatSpacing;

// 0x004b4cac
extern char SafariManboxName[];

// 0x00641000
extern int AltManBaseTextureA;

// 0x00641004
extern int PersonMeshVertexZ[3000];
// 0x00643edc
extern int PersonMeshVertices[9131];
// 0x0063810c
extern int AltManTextureCountA;

// 0x0064cd90
extern int AltManBaseTextureB;

// 0x0064cd88
extern int AltWomanTextureCountA;

// 0x0063835c
extern int AltWomanTextureCountB;

// 0x00638110
extern int AltWomanBaseTextureA;

// 0x00638108
extern int AltWomanBaseTextureB;

// 0x004b5600
extern int **ClipVertexLists[2];
// 0x004b5610
extern float DerivThirdDiffWeight;
// 0x004b5614
extern float Deriv4PointFirstOffset;
// 0x004b5618
extern float Deriv4PointOffsets[3];
// 0x004b5624
extern float Deriv4PointFirstWeight;
// 0x004b5628
extern float Deriv4PointWeights[3];
// 0x004d83c4
extern int ClipVertexPool[0x100];
// 0x004d87c4
extern int *ClipVertexPoolNext;
// 0x004d884c
extern int *ClipVertsIn[32];
// 0x004d88cc
extern struct FloatArray **DifferenceTableRows[5];

// 0x004b5660
extern float RK4Coefficients[4][2];

struct SlopeCurve;
// 0x004dd644
extern struct SlopeCurve *SplitCurveStart;
// 0x004dd648
extern struct SlopeCurve *SplitCurve;
// 0x004dd64c
extern float *SplitPointsNext;
// 0x004dd650
extern int SplitPointCount;

// 0x004ab438
extern float FLOAT_004ab438;

// 0x00610804
extern int DepthSpanRecordHistory[64];

// 0x00610500
extern int FilledTriangleHistory[64];

// 0x00610400
extern int EdgeMeshDrawCycleHistory[64];

// 0x00610704
extern int UnusedCastleCycleHistory[64];

// 0x00610604
extern int SupportPostCycleHistory[64];

// 0x006100f8
extern int LmsDrawCycleHistory[64];

// 0x0060fdf8
extern int TrainStepCycleHistory[64];

// 0x0060fef8
extern int DepthClearCycleHistory[64];

// 0x0060fbf8
extern int BisectRootCallHistory[64];

// 0x006101f8
extern int BisectRootStepHistory[64];

// 0x0060fcf8
extern int WalkerRangeCallHistory[64];

// 0x00610904
extern int TotalCastleCycleHistory[64];

// 0x00610a10
extern unsigned int CastleCounterFrame;

// 0x0060f900
extern int FilledTriangleCount;

// 0x0060f904
extern int CoasterShadeTableCount;

// 0x00579878
extern char CoasterFallbackShadeRamp[0x80];

// 0x0060f8fc
extern int EdgeMeshDrawCycles;

// 0x00611644
extern int UnusedCastleCycles;

// 0x00615f68
extern int SupportPostDrawCycles;

// 0x004dcbc8
extern int LmsDrawCycles;

// 0x004d83bc
extern int TrainStepCycles;

// 0x0060f910
extern int DepthClearCycles;

// 0x004b5964
extern unsigned int ManVisitorColours[2];
// 0x004b596c
extern const char *ManVisitorTextures[2];
// 0x004b5974
extern unsigned int GirlVisitorColours[2];
// 0x004b597c
extern const char *GirlVisitorTextures[2];

// 0x006121c8
extern float RailTubeRingPoints[6][3];
// 0x006126d8
extern float RailTubeRingNormals[6][2];
// 0x00612708
extern unsigned int RailTubeTriangles[31][36];
// 0x00613878
extern unsigned int RailTubeSegmentTriangles[36];
// 0x00613908
extern int RailTubeSegmentEdges[24][2];
// 0x006139c8
extern unsigned char RailTubeVertexBuffer[0xe90];
// 0x006148b8
extern int RailTubeEdges[546][2];

// 0x00667528
extern int ExceptionReportStarted;
// 0x004b8a88
extern int StackDumpCodeBytes;
// 0x004b8a8c
extern int StackDumpMaxWords;
// 0x004b8a90
extern int StackDumpWordsPerLine;
// 0x004b42a0
extern int CoptersMatrixRowOrder[3];
// 0x004b42ac
extern int CoptersMatrixColOrder[3];
// 0x004b42b8
extern int CoptersMatrixColSign[3];
// 0x004b42c4
extern int CoptersMatrixRowSign[3];

// Constants the original pools (strings and a float), shared by name between several functions.
// Keep these at the END of this file: MSVC6 /O2's
// register allocation depends on the order of the declarations it has seen, and declaring them
// earlier turned MusicThreadProc (sound_sfx.c, 11 KB) from 100% into 99.89% without any change
// to the function itself.
// 0x004ab384: gold-rush slot weight to map units
extern const float GoldSlotWeightScale;
// 0x004b86e0
extern const char LegoFontName[5]; /* "Lego" */
// 0x004b8a70
extern const char PathControlName[];
// 0x004bb49c
extern const char BuildMenuName[];
