#include "globals.h"
#include <dinput.h> /* this TU defines the DInput data symbols (c_dfDI*, GUID_Sys*, MouseState) */
#include "bloke.h"
#include "castle.h"
#include "icon.h"
#include "interface.h"
#include "math.h"
#include "objclass.h"
#include "popupinfo.h"
#include "space_tower.h"

// GLOBAL: LEGOLAND 0x004ab444
float FLOAT_004ab444;

// GLOBAL: LEGOLAND 0x004ab550
float FLOAT_004ab550;

// GLOBAL: LEGOLAND 0x004ab5f0
GUID IID_IDirectMusicComposer = {0xd2ac28bf, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab640
GUID IID_IDirectMusicPerformance = {0x07d43d03, 0x6523, 0x11d2, {0x87, 0x1d, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab6a0
GUID IID_IDirectMusicLoader = {0x2ffaaca2, 0x5dca, 0x11d2, {0xaf, 0xa6, 0x00, 0xaa, 0x00, 0x24, 0xd8, 0xb6}};

// GLOBAL: LEGOLAND 0x004ab7b0
GUID GUID_Download = {0xd2ac28a7, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab880
GUID GUID_NOTIFICATION_MEASUREANDBEAT = {0xd2ac289a, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab8a0
GUID GUID_NOTIFICATION_SEGMENT = {0xd2ac2899, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab900
GUID CLSID_DirectMusicLoader = {0xd2ac2892, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab920
GUID CLSID_DirectMusicComposer = {0xd2ac2890, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004aba00
GUID CLSID_DirectMusicPerformance = {0xd2ac2881, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004acfd0
GUID NullGuid = {0x00000000, 0x0000, 0x0000, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

// GLOBAL: LEGOLAND 0x004b4860
struct Point LogFlumeDropDrawOffset;

// GLOBAL: LEGOLAND 0x004c2b00
struct FlumeShape FlumeStraightShape1;

// GLOBAL: LEGOLAND 0x004c2b30
struct FlumeDims FlumeCornerShape1Points[4];

// GLOBAL: LEGOLAND 0x004c2b58
struct FlumeShape FlumeStraightShape0;

// GLOBAL: LEGOLAND 0x004c2b78
struct FlumeDims FlumeCornerShape3Points[4];

// GLOBAL: LEGOLAND 0x004c2bc0
struct FlumeShape FlumeCornerShape1;

// GLOBAL: LEGOLAND 0x004c2bc8
struct FlumeDims FlumeCornerShape2Points[4];

// GLOBAL: LEGOLAND 0x004c2be8
struct FlumeShape FlumeCornerShape0;

// GLOBAL: LEGOLAND 0x004c2c08
struct FlumeShape FlumeCornerShape3;

// GLOBAL: LEGOLAND 0x004c2c10
struct FlumeShape FlumeCornerShape2;

// GLOBAL: LEGOLAND 0x004c8d58
struct FlumeDims FlumeStraightShape1Points[2];

// GLOBAL: LEGOLAND 0x004cbde8
struct FlumeDims FlumeCornerShape0Points[4];

// GLOBAL: LEGOLAND 0x004cbe38
struct FlumeDims FlumeStraightShape0Points[2];

// GLOBAL: LEGOLAND 0x0066b638
struct ColorLutEntry ColorLut[256];

// GLOBAL: LEGOLAND 0x004ab468
float FLOAT_004ab468;

// GLOBAL: LEGOLAND 0x004ab384
const float GoldSlotWeightScale = 512.0f;

// GLOBAL: LEGOLAND 0x004ab390
float FLOAT_004ab390;

// GLOBAL: LEGOLAND 0x004ab398
double DOUBLE_004ab398;

// GLOBAL: LEGOLAND 0x004ab3dc
float Float2Pi;

// GLOBAL: LEGOLAND 0x004ab3e0
float FloatNegInv360;

// GLOBAL: LEGOLAND 0x004ab3e4
float Float45;

/* Degrees per boat heading frame (16 frames). */
// GLOBAL: LEGOLAND 0x004ab3e8
float BoatHeadingStepDegrees;

// GLOBAL: LEGOLAND 0x004ab3ec
float Float180;

/* 640.0: scale from unit arc coordinates to boat step offsets. */
// GLOBAL: LEGOLAND 0x004ab3f0
float BoatArcScale;

// GLOBAL: LEGOLAND 0x004ab3f4
float DegToRad;

// GLOBAL: LEGOLAND 0x004ab3f8
float BoatArcStepScale;

// GLOBAL: LEGOLAND 0x004ab3fc
float Float16;

// GLOBAL: LEGOLAND 0x004ab478
double DOUBLE_004ab478;

// GLOBAL: LEGOLAND 0x004ab488
double DOUBLE_004ab488;

// GLOBAL: LEGOLAND 0x004ab4a0
float Float196;

// GLOBAL: LEGOLAND 0x004ab4a8
double JungleCruiseSeatRadiusY;

// GLOBAL: LEGOLAND 0x004ab4b0
double JungleCruiseSeatRadiusX;

// GLOBAL: LEGOLAND 0x004ab4b8
float FLOAT_004ab4b8;

// GLOBAL: LEGOLAND 0x004ab430
float TrainCarMass;

// GLOBAL: LEGOLAND 0x004ab43c
float FLOAT_004ab43c;

// GLOBAL: LEGOLAND 0x004ab418
double CastleEpsilon;

// GLOBAL: LEGOLAND 0x004ab424
float FLOAT_004ab424;

// GLOBAL: LEGOLAND 0x004ab428
float FLOAT_004ab428;

// GLOBAL: LEGOLAND 0x004ab42c
float FLOAT_004ab42c;

// GLOBAL: LEGOLAND 0x004ab44c
float FLOAT_004ab44c;

// GLOBAL: LEGOLAND 0x004ab454
float FLOAT_004ab454;

// GLOBAL: LEGOLAND 0x004ab458
float FLOAT_004ab458;

// GLOBAL: LEGOLAND 0x004ab45c
float FLOAT_004ab45c;

// GLOBAL: LEGOLAND 0x004ab4bc
float RidePointOriginY;

// GLOBAL: LEGOLAND 0x004ab4c0
float RidePointOriginX;

// GLOBAL: LEGOLAND 0x004ab4c8
double DoubleZero;

// GLOBAL: LEGOLAND 0x004ab518
float FloatOnePercent;

// GLOBAL: LEGOLAND 0x004ab528
float FloatMsPerSecond;

// GLOBAL: LEGOLAND 0x004ab52c
float FLOAT_004ab52c;

// GLOBAL: LEGOLAND 0x004ab530
double DOUBLE_004ab530;

// GLOBAL: LEGOLAND 0x004ab538
double DOUBLE_004ab538;

// GLOBAL: LEGOLAND 0x004ab558
double DOUBLE_004ab558;

// GLOBAL: LEGOLAND 0x004ab560
const DIDATAFORMAT c_dfDIKeyboard;

// GLOBAL: LEGOLAND 0x004ab578
const DIDATAFORMAT c_dfDIMouse;

// GLOBAL: LEGOLAND 0x004ab5e0
GUID IID_IDirectMusicBand = {0xd2ac28c0, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab600
GUID IID_IDirectMusicChordMap = {0xd2ac28be, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab610
GUID IID_IDirectMusicStyle = {0xd2ac28bd, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab670
GUID IID_IDirectMusicSegment = {0xf96029a2, 0x4282, 0x11d2, {0x87, 0x17, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab6d0
GUID GUID_PerfMasterGrooveLevel = {0xd2ac28b2, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab8b0
GUID GUID_DirectMusicAllTypes = {0xd2ac2893, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab8e0
GUID DirectMusicBandClassGuid = {0x79ba9e00, 0xb6ee, 0x11d1, {0x86, 0xbe, 0x00, 0xc0, 0x4f, 0xbf, 0x8f, 0xef}};

// GLOBAL: LEGOLAND 0x004ab930
GUID DirectMusicChordMapClassGuid = {0xd2ac288f, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab980
GUID CLSID_DirectMusicStyle = {0xd2ac288a, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ab9f0
GUID CLSID_DirectMusicSegment = {0xd2ac2882, 0xb39b, 0x11d1, {0x87, 0x04, 0x00, 0x60, 0x08, 0x93, 0xb1, 0xbd}};

// GLOBAL: LEGOLAND 0x004ac090
const GUID GUID_SysKeyboard = {0x6f1d2b61, 0xd5a0, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};

// GLOBAL: LEGOLAND 0x004ac0a0
const GUID GUID_SysMouse = {0x6f1d2b60, 0xd5a0, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};

// GLOBAL: LEGOLAND 0x004acf80
GUID IID_IDirectDraw2_Guid = {0xb3a6f3e0, 0x2b43, 0x11cf, {0xa2, 0xde, 0x00, 0xaa, 0x00, 0xb9, 0x33, 0x56}};

/* Maps a 16-step heading to the 8-way direction used by roads and traffic lights. */
// GLOBAL: LEGOLAND 0x004b4034
int Heading16ToDir8[16];

// GLOBAL: LEGOLAND 0x004b40a4
unsigned int CatapultSlotLayers[4];

// GLOBAL: LEGOLAND 0x004b40b4
int CatapultSlotOffsetsY[4];

// GLOBAL: LEGOLAND 0x004b40c8
struct FXItem Catapult_SFX[4];

// GLOBAL: LEGOLAND 0x004b4140
struct FXItem Helicopter_SFX[4];

/* CoptersPlaceRider axis remap: row order, column order (y/z swapped) and signs, converting the ride matrix into the person matrix. */
// GLOBAL: LEGOLAND 0x004b42a0
int CoptersMatrixRowOrder[3] = {0, 1, 2};

// GLOBAL: LEGOLAND 0x004b42ac
int CoptersMatrixColOrder[3] = {0, 2, 1};

// GLOBAL: LEGOLAND 0x004b42b8
int CoptersMatrixColSign[3] = {1, -1, -1};

// GLOBAL: LEGOLAND 0x004b42c4
int CoptersMatrixRowSign[3] = {-1, 1, 1};

// GLOBAL: LEGOLAND 0x004b43f8
struct FXItem DRIVING_SCHOOL_SFX[6];

// GLOBAL: LEGOLAND 0x004b4440
unsigned int DrivingSchoolPart1Footprint[5];

// GLOBAL: LEGOLAND 0x004b4458
unsigned int DrivingSchoolPart2Footprint[5];

// GLOBAL: LEGOLAND 0x004b4470
unsigned int DrivingSchoolPart3Footprint[5];

// GLOBAL: LEGOLAND 0x004b4580
struct FortRect FortWanderRect = {-1, -3, 3, 3};

// GLOBAL: LEGOLAND 0x004b4688
struct FXItem JOUST_SFX[1];

// GLOBAL: LEGOLAND 0x004b4728
struct Footprint LogFlumeFootprint;
// 0x008003e8 is EditCursor.field_1828 — see struct Cursor in gamemap.h.

// GLOBAL: LEGOLAND 0x004b473c
int FlumeTrackMode3Sprites[4];

// GLOBAL: LEGOLAND 0x004b474c
int FlumeTrackMode1Sprites[2];

// GLOBAL: LEGOLAND 0x004b4754
int FlumeTrackMode2Sprites[4];

// GLOBAL: LEGOLAND 0x004b4764
int FlumeTrackMode4Sprite;

// GLOBAL: LEGOLAND 0x004b47f8
struct Sprite *LogFlumeFc1M1StageSprite;

// GLOBAL: LEGOLAND 0x004b4804
struct Sprite *LogFlumeFc1M2StageSprite;

// GLOBAL: LEGOLAND 0x004b4808
struct FlumeTemplate LogFlumeSpecialCorner1Stages;

// GLOBAL: LEGOLAND 0x004b4818
struct Sprite *LogFlumeFc3M1StageSprite;

// GLOBAL: LEGOLAND 0x004b4824
struct Sprite *LogFlumeFc3M2StageSprite;

// GLOBAL: LEGOLAND 0x004b4828
struct FlumeTemplate LogFlumeSpecialCorner3Stages;

// GLOBAL: LEGOLAND 0x004b4838
struct Sprite *LogFlumeHup1M1StageSprite;

// GLOBAL: LEGOLAND 0x004b4850
struct Sprite *LogFlumeHup1M2StageSprite;

// GLOBAL: LEGOLAND 0x004b4858
struct FlumeTemplate LogFlumeHoldUpStages;

// GLOBAL: LEGOLAND 0x004b4bd0
unsigned char DrivingSchoolPumpsFootprint[0x14];

// GLOBAL: LEGOLAND 0x004b4bf0
unsigned int RoadPieceFootprint[5];

// GLOBAL: LEGOLAND 0x004b4c04
int TrafficLightTimer;

// GLOBAL: LEGOLAND 0x004b4c08
unsigned short RoadPieceTileTable[15][4];

// GLOBAL: LEGOLAND 0x004b4cb8
struct FXItem SAFARI_SFX[1];

// GLOBAL: LEGOLAND 0x004b4cc4
int SafariRideFrameThreshold1[8] = {0x42, 0x42, 0x2f, 0x2f, 0x50, 0x50, 0x30, 0x30};

// GLOBAL: LEGOLAND 0x004b4ce4
int SafariRideFrameThreshold2[8] = {0x3f, 0x3f, 0x3f, 0x3f, 0x30, 0x30, 0x20, 0x20};

// GLOBAL: LEGOLAND 0x004b4d88
struct FXItem SpiderRide_SFX[1];

// GLOBAL: LEGOLAND 0x004b4d94
struct SpiderBnv SpiderBnvInfo = {"manbox??", {0, 0x20, 0x1c, 0x14, 0x18, 0x0c, 0x10, 0x14, 0x0c, 0x14, 0x18, 0x1c, 0x21, 0x24, 0x21, 0x24}, {0x20, 0x14, 0x18, 0x20, 0x18, 0x20, 0x20, 0x18, 0x20, 0x14, 0x18, 0x10, 0x0c, 0x08, 0x0c, 0x12, 0x14}};

// GLOBAL: LEGOLAND 0x004b4e20
Point SpiderRideDrawAdjust;

// GLOBAL: LEGOLAND 0x004b4f08
char *TempleSlidePathNames[4] = {
    // STRING: LEGOLAND 0x004b4f44
    "manbox01",
    // STRING: LEGOLAND 0x004b4f38
    "manbox02",
    // STRING: LEGOLAND 0x004b4f2c
    "manbox03",
    // STRING: LEGOLAND 0x004b4f20
    "manbox04",
};

/* Per-lane BNV path frame at which a temple slide rider starts sliding (speed 0x18). */
// GLOBAL: LEGOLAND 0x004b4f18
signed char TempleSlideStartFrames[4] = {0x40, 0x45, 0x3f, 0x3c};

/* Per-lane BNV path frame at which a temple slide rider reaches the end of the slide. */
// GLOBAL: LEGOLAND 0x004b4f1c
signed char TempleSlideEndFrames[4] = {0x51, 0x53, 0x53, 0x50};

// GLOBAL: LEGOLAND 0x004b4fa8
struct FXItem WATERWORKS_SFX[3];

// GLOBAL: LEGOLAND 0x004b5118
struct BoatDirStep BoatingSchoolDirSteps[4];

// GLOBAL: LEGOLAND 0x004b5158
struct BoatArc BoatingSchoolArcsAlt[4];

// GLOBAL: LEGOLAND 0x004b5198
struct BoatArc BoatingSchoolArcs[4];

// GLOBAL: LEGOLAND 0x004b51d8
int BoatingSchoolRiderOffsets[0x80];

// GLOBAL: LEGOLAND 0x004b5260
struct Footprint BoatingSchoolExtraFootprintTemplate;

// GLOBAL: LEGOLAND 0x004b5278
struct Footprint BoatingSchoolStartFootprintTemplate;

// GLOBAL: LEGOLAND 0x004b5290
struct Point BoatingSchoolQueueOffsets[6];

// GLOBAL: LEGOLAND 0x004b52c0
struct FXItem PTR_s_Boat_Noise_wav[2];

// GLOBAL: LEGOLAND 0x004b53c0
struct Footprint BoatingSchoolWaterFootprint;

// GLOBAL: LEGOLAND 0x004b53d4
unsigned char BoatPathTilePatterns[0x190];

// GLOBAL: LEGOLAND 0x004b5570
struct AreaNode DoorAreaList;

// GLOBAL: LEGOLAND 0x004b5584
short PieceNeighbourOffsets[4][2] = {{0, -2}, {2, 0}, {0, 2}, {-2, 0}};

// GLOBAL: LEGOLAND 0x004b55f4
unsigned int MapObjectsEnabled;

// GLOBAL: LEGOLAND 0x004b5608
unsigned int ClipVertexStride;

// GLOBAL: LEGOLAND 0x004b560c
int ClipVertexLastField;

// GLOBAL: LEGOLAND 0x005798fc
unsigned short CoasterSurfaceData[640 * 480]; /* the renderer's 16-bit frame until GetCoasterSurface locks a surface */

// GLOBAL: LEGOLAND 0x004b5b20
void *CoasterSurfaceBits;

// GLOBAL: LEGOLAND 0x004b5b24
unsigned short *CoasterDepthBuffer;

// GLOBAL: LEGOLAND 0x004b5b28
int CoasterSurfacePitch;

// GLOBAL: LEGOLAND 0x004b59e8
char SitLoGirlSitName[16] = "sit.logirlsit";

// GLOBAL: LEGOLAND 0x004bc0ec
char LanguageName[36] = "english";

// GLOBAL: LEGOLAND 0x004b59f8
char SitLoManSitName[] = "sit.lomansit";

// GLOBAL: LEGOLAND 0x004b5abc
float HermiteBasis[4][4] = {{-2.0f, 2.0f, 1.0f, 1.0f}, {3.0f, -3.0f, -1.0f, -2.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 0.0f}};

// GLOBAL: LEGOLAND 0x004b5b3c
struct RecBuf *DepthSpanRecordEnd;

// GLOBAL: LEGOLAND 0x004b5988
struct BlokeInfo CastleVisitorInfo;

// GLOBAL: LEGOLAND 0x004b5964
unsigned int ManVisitorColours[2] = {0x191919, 0xf11a22};

// GLOBAL: LEGOLAND 0x004b596c
const char *ManVisitorTextures[2] = {
    // STRING: LEGOLAND 0x004b59d8
    "Chest visitor1",
    // STRING: LEGOLAND 0x004b59d0
    "Face01"};

// GLOBAL: LEGOLAND 0x004b5974
unsigned int GirlVisitorColours[2] = {0xf11a22, 0x8b4a};

// GLOBAL: LEGOLAND 0x004b597c
const char *GirlVisitorTextures[2] = {
    // STRING: LEGOLAND 0x004b59c0
    "Chest girly2",
    // STRING: LEGOLAND 0x004b59d0
    "Face01"};

// GLOBAL: LEGOLAND 0x004b5c1c
struct FMat4 CastleIsoProjections[2] = {{{{1.60334f, -1.60334f, 0.0f, 0.0f}, {0.801688f, 0.801688f, 1.96416f, 0.0f}, {3.19995f, 3.19995f, 0.0f, 32767.5f}, {0.0f, 0.0f, 0.0f, 1.0f}}}, {{{1.60334f, -1.60334f, 0.0f, 0.0f}, {0.801688f, 0.801688f, 1.96416f, 0.0f}, {3.19995f, 3.19995f, 0.0f, 32767.5f}, {0.0f, 0.0f, 0.0f, 1.0f}}}};

// GLOBAL: LEGOLAND 0x004b5c9c
unsigned int CastleShadeMax = 0x427c0000 /* 63.0f */;

// GLOBAL: LEGOLAND 0x004b5ca0
unsigned int CastleLightDir;

// GLOBAL: LEGOLAND 0x004b5ca4
unsigned int CastleLightDirY;

// GLOBAL: LEGOLAND 0x004b5ca8
unsigned int CastleLightDirZ = 0xbf800000 /* -1.0f */;

// GLOBAL: LEGOLAND 0x004b5cac
float TrackLightingBase = 63.0f;

// GLOBAL: LEGOLAND 0x004b5cb0
unsigned int TrackLightDir;

// GLOBAL: LEGOLAND 0x004b5cb4
unsigned int TrackLightDirY;

// GLOBAL: LEGOLAND 0x004b5cb8
unsigned int TrackLightDirZ = 0xbf800000 /* -1.0f */;

// GLOBAL: LEGOLAND 0x004b5cbc
float ModelLightingBase = 40.0f;

// GLOBAL: LEGOLAND 0x004b5cc0
unsigned int ModelLightDir = 0x3e70a3d7 /* 0.235f */;

// GLOBAL: LEGOLAND 0x004b5cc4
unsigned int ModelLightDirY = 0x3eb0a3d7 /* 0.345f */;

// GLOBAL: LEGOLAND 0x004b5cc8
unsigned int ModelLightDirZ = 0xbf800000 /* -1.0f */;

// GLOBAL: LEGOLAND 0x004b5cf4
char RollerCoasterSavePath[32] = "RollerCoaster\\RollerCoaster.sav";

// GLOBAL: LEGOLAND 0x004b5d20
struct PieceClass SquareTrackPieceClass;

// GLOBAL: LEGOLAND 0x004b5d58
struct PieceClass HeightTrack25PieceClass;

// GLOBAL: LEGOLAND 0x004b5d90
struct PieceClass HeightTrack2PieceClass;

// GLOBAL: LEGOLAND 0x004b5dc8
struct PieceClass HeightTrackPathPieceClass;

// GLOBAL: LEGOLAND 0x004b5e00
float TrackSideMidpointsUnit[4][3] = {{1.0f, 0.0f, 0.0f}, {2.0f, 1.0f, 0.0f}, {1.0f, 2.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};

// GLOBAL: LEGOLAND 0x004b5e30
float TrackRailOffsetsUnit[4][3] = {{0.5f, 0.0f, 0.0f}, {-0.5f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.0f, -0.5f, 0.0f}};

// GLOBAL: LEGOLAND 0x004b5e60
float TrackArcTangentsUnit[4][3] = {{1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};

// GLOBAL: LEGOLAND 0x004b5e90
float TrackArcCentresUnit[4][3] = {{0.0f, 0.0f, 0.0f}, {2.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 0.0f}, {0.0f, 2.0f, 0.0f}};

// GLOBAL: LEGOLAND 0x004b5ec0
int TrackStraightStartSides[4] = {0, 2, 1, 3};

// GLOBAL: LEGOLAND 0x004b5ee0
int TrackStraightHeightSteps[5] = {-4, -2, 0, 2, 4};

// GLOBAL: LEGOLAND 0x004b5ef4
unsigned int TrackArcTangentPairs[4][2] = {{0, 3}, {2, 3}, {2, 1}, {0, 1}};

// GLOBAL: LEGOLAND 0x004b5f14
unsigned int TrackArcRadii[4][2] = {{0x3fc00000, 0x3f000000}, {0x3f000000, 0x3fc00000}, {0x3fc00000, 0x3f000000}, {0x3f000000, 0x3fc00000}};

// GLOBAL: LEGOLAND 0x004b5f60
struct EdgeMesh RailTubeMesh;

// GLOBAL: LEGOLAND 0x004b6010
unsigned short SupportPostModelTriangles[16][8]; /* SupportPostModel's 16 triangles */

/* Face normals of SupportPostModel (its field_10 points here). */
// GLOBAL: LEGOLAND 0x004b6110
float SupportPostModelNormals[16];

// GLOBAL: LEGOLAND 0x004b6150
struct LmsFile SupportPostModel = {0};

// GLOBAL: LEGOLAND 0x004b61e0
struct CastleFloatEnt TrackTieShape[8] = {0}; /* a unit box; c holds z as float bits */

/* Face normals of TrackTieModel (its field_10 points here). */
// GLOBAL: LEGOLAND 0x004b6240
float TrackTieModelNormals[12];

// GLOBAL: LEGOLAND 0x004b6270
unsigned short TrackTieModelTriangles[8][8]; /* TrackTieModel's 8 triangles */

// GLOBAL: LEGOLAND 0x004b62f0
unsigned int TrackTieLmsRec[4];

// GLOBAL: LEGOLAND 0x004b6300
struct LmsFile TrackTieModel;

/* Edge midpoints of a 40-unit coaster cell per door direction (N, E, S, W), used by BuildSlopeRun. */
// GLOBAL: LEGOLAND 0x004b6398
float CoasterCellEdgeMidpoints[4][3] = {{20.0f, 0.0f, 0.0f}, {40.0f, 20.0f, 0.0f}, {20.0f, 40.0f, 0.0f}, {0.0f, 20.0f, 0.0f}};

// GLOBAL: LEGOLAND 0x004b63c8
float SlopeRunDirections[4][3] = {{10.0f, 0.0f, 0.0f}, {0.0f, 10.0f, 0.0f}, {-10.0f, 0.0f, 0.0f}, {0.0f, -10.0f, 0.0f}};
// GLOBAL: LEGOLAND 0x004b6408
int WheelCurvePairs[3] = {0, 2, 1};

// GLOBAL: LEGOLAND 0x004ab490
float FLOAT_004ab490;

// GLOBAL: LEGOLAND 0x004ab494
float FLOAT_004ab494;

// GLOBAL: LEGOLAND 0x004ab49c
float FLOAT_004ab49c;

/* the carousel seat path name; the riders' code writes the seat number over "??" (+8) before each lookup */
// GLOBAL: LEGOLAND 0x004b64cc
char CarouselBlokeBoxName[12] = "BlokeBox??";

// GLOBAL: LEGOLAND 0x004b64d8
struct FXItem CAROUSSEL_SFX[2];

// GLOBAL: LEGOLAND 0x004b65c0
int EarthSlideQueueOffsets[8] = {0, 4, 0, 3, 0, 2, 0, 1};

// GLOBAL: LEGOLAND 0x004b6638
char CurrentDirPath[4] = ".";

// GLOBAL: LEGOLAND 0x004b6654
int ENTRANCE_DEST_RIGHT[2] = {0x978, 0xd78};

// GLOBAL: LEGOLAND 0x004b665c
int ENTRANCE_DEST_LEFT[2] = {0x878, 0xc78};

// GLOBAL: LEGOLAND 0x004b6668
struct FXItem ENTRANCE_SFX[1];

// GLOBAL: LEGOLAND 0x004b66e8
struct FXItem RESTAURANT_SFX[1];

/* Restaurant1 customer walk paths: 3 seats x 5 steps x 6 ints (tile dx, tile dy, sub-tile x, sub-tile y,
 * face flag, frame). */
// GLOBAL: LEGOLAND 0x004b66f4
int Restaurant1CustomerPath[15 * 6];

// GLOBAL: LEGOLAND 0x004b685c
int Restaurant2DownStepsBase;

// GLOBAL: LEGOLAND 0x004b6860
int Restaurant2DownSteps[0x20];

// GLOBAL: LEGOLAND 0x004b68e0
int Restaurant2UpSteps[0x22];

// GLOBAL: LEGOLAND 0x004b6968
struct FXItem OCTOPUS_SFX[3];

// GLOBAL: LEGOLAND 0x004b6990
int OctopusCafeSpotOffsets[0x29];

// GLOBAL: LEGOLAND 0x004b6a34
int OctopusCafeEntryPath[4];

// GLOBAL: LEGOLAND 0x004b6a44
int OctopusCafeSpotIndices[0xd];

// GLOBAL: LEGOLAND 0x004b6a78
int OctopusCafeExitPath[0x30];

// GLOBAL: LEGOLAND 0x004b6b38
int OctopusCafeChairOffsets[0x2c];

// GLOBAL: LEGOLAND 0x004b6be8
int OctopusCafeChairIndices[0x5c];

/* Octopus cafe depth layer per tile of its 11-wide footprint (stride 4), stored in bloke->field_37 for draw order. */
// GLOBAL: LEGOLAND 0x004b6d58
unsigned char OctopusCafeDepthMap[0x120];

// GLOBAL: LEGOLAND 0x004b7260
struct Footprint JungleCruiseExtraFootprint;

// GLOBAL: LEGOLAND 0x004b7278
struct Footprint JungleCruiseStartFootprint;

// GLOBAL: LEGOLAND 0x004b7148
struct BoatDirStep JungleCruiseDirSteps[4];

// GLOBAL: LEGOLAND 0x004b7188
struct BoatArc JungleCruiseArcs[4];

// GLOBAL: LEGOLAND 0x004b71c8
struct BoatArc JungleCruiseArcsAlt[4];

/* Jungle cruise queue walk offsets (1/256 tile) from the ride origin, indexed 4 - slot. */
// GLOBAL: LEGOLAND 0x004b7290
struct Point JungleCruiseQueueOffsets[5];

// GLOBAL: LEGOLAND 0x004b7230
struct Footprint JungleCruiseExtraFootprintTemplate;

// GLOBAL: LEGOLAND 0x004b7248
struct Footprint JungleCruiseStartFootprintTemplate;

// GLOBAL: LEGOLAND 0x004b72e4
unsigned char JunglePathTilePatterns[0x190];

// GLOBAL: LEGOLAND 0x004b7478
struct Footprint JungleCruiseDefaultFootprint;

// GLOBAL: LEGOLAND 0x004b7618
struct FXItem SPACE_TOWER_SFX[1];

/* Space tower seat layouts: riders on this one are drawn in front of the seats, the next one behind. */
// GLOBAL: LEGOLAND 0x004b76b8
struct AnimLayout SpaceTowerFrontSeatLayout;

// GLOBAL: LEGOLAND 0x004b7750
struct AnimLayout SpaceTowerBackSeatLayout;

// GLOBAL: LEGOLAND 0x004b7758
struct SpaceTowerLayout SpaceTowerLayouts[8];

// GLOBAL: LEGOLAND 0x004b7798
struct SpaceTowerSeatData SpaceTowerSeats[4];

/* Space tower seat offsets in tiles, indexed by the rider's seat (field_36). */
// GLOBAL: LEGOLAND 0x004b77e8
struct Point SpaceTowerSeatOffsets[8];

// GLOBAL: LEGOLAND 0x004b78b4
char BoxBlokeBnvName[] = "BoxBloke??";

// GLOBAL: LEGOLAND 0x004b79bc
char PlaneRideManboxName[] = "manbox??";

// GLOBAL: LEGOLAND 0x004b79d0
struct FXItem PlaneRideFX[2];

/* FPU control word loaded while a 3D person is rendered. */
// GLOBAL: LEGOLAND 0x004b7abc
unsigned short PersonRenderFpuControlWord;

// GLOBAL: LEGOLAND 0x004b7ac0
unsigned char BlokeColours[0x18];

/* Put3DBlokeOnRide axis remap tables (same scheme as the copters ones). */
// GLOBAL: LEGOLAND 0x004b7ae0
int RideMatrixRowOrder[3] = {0, 1, 2};

// GLOBAL: LEGOLAND 0x004b7aec
int RideMatrixColOrder[3] = {0, 2, 1};

// GLOBAL: LEGOLAND 0x004b7af8
int RideMatrixColSign[3] = {1, -1, -1};

// GLOBAL: LEGOLAND 0x004b7b04
int RideMatrixRowSign[3] = {-1, 1, 1};

// GLOBAL: LEGOLAND 0x004b7b10
const char Path3DFormat[] = ".\\3ddata\\new\\%s\\%s";

/* BITMAPINFOHEADER passed to AVIStreamGetFrameOpen for the advisor video (InitAdvisorFrameFormat: 112x96, 16 bpp). */
// GLOBAL: LEGOLAND 0x004b7d70
unsigned int AdvisorFrameFormat;

// GLOBAL: LEGOLAND 0x004b7d74
unsigned int AdvisorFrameWidth;

// GLOBAL: LEGOLAND 0x004b7d78
unsigned int AdvisorFrameHeight;

// GLOBAL: LEGOLAND 0x004b7d7e
unsigned short AdvisorFrameBitCount;

// GLOBAL: LEGOLAND 0x004b7d84
unsigned int AdvisorFrameSizeImage;

/* Start of the 22 element names IsRidePartClass accepts. */
// GLOBAL: LEGOLAND 0x004b7e9c
unsigned int RidePartClassNames;

// GLOBAL: LEGOLAND 0x004b81c0
// STRING: LEGOLAND 0x004b8260
char *GraphicsPath = ".\\graphics\\";

// GLOBAL: LEGOLAND 0x004b81c4
char *GraphicsPathPrefix = ".\\graphics\\";

// GLOBAL: LEGOLAND 0x004b81c8
// STRING: LEGOLAND 0x004b824c
char *GraphicsSmallPath = ".\\graphics\\small\\";

// GLOBAL: LEGOLAND 0x004b81cc
// STRING: LEGOLAND 0x004b8238
char *MasksPath = ".\\graphics\\masks\\";

// GLOBAL: LEGOLAND 0x004b81d0
// STRING: LEGOLAND 0x004b8220
char *MasksSmallPath = ".\\graphics\\masks\\small\\";

// GLOBAL: LEGOLAND 0x004b81d4
// STRING: LEGOLAND 0x004b820c
char *IconsPath = ".\\graphics\\icons\\";

// GLOBAL: LEGOLAND 0x004b81d8
// STRING: LEGOLAND 0x004b81f8
char *ModelsPath = ".\\graphics\\models\\";

// GLOBAL: LEGOLAND 0x004b8318
struct Point ParkEntranceWalkInOffset; /* offset of the park entrance walk-in start */

// GLOBAL: LEGOLAND 0x004b8320
struct Point ParkExitPos; /* park exit (goal of leaving blokes) */

// GLOBAL: LEGOLAND 0x004b8328
struct Point ParkExitStep; /* step past the exit */

// GLOBAL: LEGOLAND 0x004b8334
int BlokeTirednessThresholds[4] = {0}; /* bloke tiredness thresholds */

// GLOBAL: LEGOLAND 0x004b8344
char NextBlokeLetter = 0;

// GLOBAL: LEGOLAND 0x004b8348
char *BlokeAiLogLinePtrs[8] = {0};

/* Long-term (high-level) action handlers, indexed by bloke->action (NewLongTermAction / DoHighLevelAI).
 * 26 entries in the original: visitors 0-15, workers 16-25 (idle, build, repair, picked up). */
// GLOBAL: LEGOLAND 0x004b8368
void (*BlokeLongTermActionTable[26])(struct Bloke *) = {0};

// Persistent VWIN32 handle for EjectCdDrive; invalid until opened and invalidated after closing.
// GLOBAL: LEGOLAND 0x004b85c4
HANDLE VWin32DeviceHandle = INVALID_HANDLE_VALUE;

// GLOBAL: LEGOLAND 0x004b8710
struct FXItem FountainSFX[5];

// GLOBAL: LEGOLAND 0x004b8750
struct FXItem PowerStationSFX[2];

// GLOBAL: LEGOLAND 0x004b8768
struct FXItem DINO_SFX[5];

// GLOBAL: LEGOLAND 0x004b87a8
struct FXItem MONEY_SFX[2];

// GLOBAL: LEGOLAND 0x004b8bbc
unsigned char PercentSFormat[4]; /* "%s" */

// GLOBAL: LEGOLAND 0x004b90f8
unsigned int BrickCount;

// GLOBAL: LEGOLAND 0x004b90fc
unsigned int UnlimitedBricks;

// GLOBAL: LEGOLAND 0x004b9210
int Bridge1SpriteOffsetX;

// GLOBAL: LEGOLAND 0x004b9214
int Bridge1SpriteOffsetY;

// GLOBAL: LEGOLAND 0x004b9218
int Bridge0SpriteOffsetX;

// GLOBAL: LEGOLAND 0x004b921c
int Bridge0SpriteOffsetY;

// GLOBAL: LEGOLAND 0x004b9220
LEGO_EXPORT unsigned int BGFullUpdate;

// GLOBAL: LEGOLAND 0x004b9228
struct FXItem GameFX[FX_COUNT];

/* Direction (0-7) to direction-bit table used by Dir_To_Bit and Bit_To_Dir. */
// GLOBAL: LEGOLAND 0x004b9550
unsigned char DirectionBits[8];

/* Path 3x3 square masks tried by FindPathSquare3x3, with the matching x/y origin offsets after them. */
// GLOBAL: LEGOLAND 0x004b9558
unsigned int PathSquare3x3Masks[9] = {
    0x01ce7000,
    0x00e73800,
    0x00739c00,
    0x000e7380,
    0x000739c0,
    0x00039ce0,
    0x0000739c,
    0x000039ce,
    0x00001ce7,
};

// GLOBAL: LEGOLAND 0x004b957c
int PathSquare3x3OffsetsX[9] = {-2, -1, 0, -2, -1, 0, -2, -1, 0};

// GLOBAL: LEGOLAND 0x004b95a0
int PathSquare3x3OffsetsY[9] = {-2, -2, -2, -1, -1, -1, 0, 0, 0};

/* Cursor outline colour pairs (r, g, b, pad) x 2: cyan for cursor flag 2; blue, green and red follow. */
// GLOBAL: LEGOLAND 0x004b95c4
unsigned char CursorCyanColours[8];

// GLOBAL: LEGOLAND 0x004b95cc
unsigned char CursorBlueColours[8];

// GLOBAL: LEGOLAND 0x004b95d4
unsigned char CursorGreenColours[8];

// GLOBAL: LEGOLAND 0x004b95dc
unsigned char CursorRedColours[8];

// GLOBAL: LEGOLAND 0x004b95e8
int UnusedViewMapTileX;

// GLOBAL: LEGOLAND 0x004b95ec
int UnusedViewMapTileY;

// GLOBAL: LEGOLAND 0x004b95f0
struct DeferredSprite *DeferredSpriteCursor;

// GLOBAL: LEGOLAND 0x004b95f4
int ScrollMarginTopRight;

// GLOBAL: LEGOLAND 0x004b95f8
int ScrollMarginBottomLeft;

// GLOBAL: LEGOLAND 0x004b95fc
int ScrollMarginTopLeft;

// GLOBAL: LEGOLAND 0x004b9600
int ScrollMarginBottomRight;

// GLOBAL: LEGOLAND 0x004b9604
int ScrollMaxX;

// GLOBAL: LEGOLAND 0x004b9608
int ScrollMarginLeft;

// GLOBAL: LEGOLAND 0x004b960c
int ScrollMarginTop;

// GLOBAL: LEGOLAND 0x004b9610
int ScrollMarginBottom;

// GLOBAL: LEGOLAND 0x004b9ca4
int (*BlitFrameFunc)(void);

// GLOBAL: LEGOLAND 0x004b9ca8
unsigned int OverrideFrame;

// GLOBAL: LEGOLAND 0x004b9d0c
const char BltFastNoSpriteFmt[] = "BltFast:Sprite Not Available:%s\n";

// GLOBAL: LEGOLAND 0x004b9e5c
unsigned int ScriptEventCategories[71];

// GLOBAL: LEGOLAND 0x004b9f78
int AdvisorHelpRect[4] = {0x1f4, 0x190, 0x280, 0x1e0};

// GLOBAL: LEGOLAND 0x004b9f88
unsigned int HelpChanged;

// GLOBAL: LEGOLAND 0x004b9f8c
UINT_PTR CurrentHelpKey = (UINT_PTR)-1; /* nothing spoken yet */

// GLOBAL: LEGOLAND 0x004ba87c
void *IconListTail;

// GLOBAL: LEGOLAND 0x004ba880
void *HelpIconListTail;

/* Print colour/flag for build icons the player cannot afford (cost above the brick count). */
// GLOBAL: LEGOLAND 0x004ba884
unsigned int UnaffordableIconColour = 0xff000000;

/* Advice speech table indexed by advice id (TryPlayAdviceSpeech). */
// GLOBAL: LEGOLAND 0x004ba8e0
struct InfoTimedEntry AdviceSpeechEntries[17];

// GLOBAL: LEGOLAND 0x004ba9ac
unsigned int AdviceSpeechIds[234];

// GLOBAL: LEGOLAND 0x004bad54
int mouse_granularity;

// GLOBAL: LEGOLAND 0x004bad58
struct KeyMapping CheatKeyMap[0x3c];

/* Four fixed-width theme names passed to the element database when constructing the build menu. */
// GLOBAL: LEGOLAND 0x004bafa8
unsigned int BuildThemeNames[20];

/* Selected build-theme index: 0 common/Legoland, 1 western, 2 castle, 3 adventurers, 5 none. */
// GLOBAL: LEGOLAND 0x004baff8
unsigned int SelectedBuildTheme;

/* Fixed-width build-category names used to order the themed object menu. */
// GLOBAL: LEGOLAND 0x004baffc
char BuildCategoryNames[4][0x14];

// GLOBAL: LEGOLAND 0x004bb04c
int InterfaceIconPos[18];

/* Legoland theme icon is in its deselected state when nonzero. */
// GLOBAL: LEGOLAND 0x004bb094
unsigned int LegolandThemeDeselected;

/* Castle theme icon is in its deselected state when nonzero. */
// GLOBAL: LEGOLAND 0x004bb098
unsigned int CastleThemeDeselected;

/* Western theme icon is in its deselected state when nonzero. */
// GLOBAL: LEGOLAND 0x004bb09c
unsigned int WesternThemeDeselected;

/* Adventurers theme icon is in its deselected state when nonzero. */
// GLOBAL: LEGOLAND 0x004bb0a0
unsigned int AdventurersThemeDeselected;

/* Pairs of object name and build element name (NULL = itself) used by SelectMatchingBuildElement. */
// GLOBAL: LEGOLAND 0x004bb0a4
struct TrackElemPair BuildElementNameMap[29];

// GLOBAL: LEGOLAND 0x004bb18c
unsigned int InterfaceSlotValues[4];

// GLOBAL: LEGOLAND 0x004bb4dc
float AviAudioVolume;

// GLOBAL: LEGOLAND 0x004bb4e0
BITMAPINFOHEADER AviFrameFormat;

// GLOBAL: LEGOLAND 0x004bb598
struct Point AStarStartPoint;

// GLOBAL: LEGOLAND 0x004bb5a0
int AStarGoalX;

// GLOBAL: LEGOLAND 0x004bb5a4
int AStarGoalY;

/* Script objective kind selecting the initial event flag (1, 2, or 4). */
// GLOBAL: LEGOLAND 0x004bb5ac
unsigned int ScriptObjectiveKind;

// GLOBAL: LEGOLAND 0x004bb5b0
unsigned int ScriptConditionActive;

// GLOBAL: LEGOLAND 0x004bb5b4
unsigned int ScriptZoningNames;

// GLOBAL: LEGOLAND 0x004bb5c4
unsigned int ScriptThemeNames;

// GLOBAL: LEGOLAND 0x004bb5d8
unsigned int ScriptTabNames;

// GLOBAL: LEGOLAND 0x004bb5e0
unsigned int ScriptModeNames;

// GLOBAL: LEGOLAND 0x004bb5f4
unsigned int ScriptFeatureNames;

/* Start of the 25 report names matched by the script REPORT command. */
// GLOBAL: LEGOLAND 0x004bb624
unsigned int ScriptReportNames;

// GLOBAL: LEGOLAND 0x004bb688
unsigned int ScriptHapFactorNames;

// GLOBAL: LEGOLAND 0x004bb6bc
unsigned int ScriptCapacityClassNames;

/* Start of the 9 button names matched by the script FLASHBUTTON command (bit index = name index). */
// GLOBAL: LEGOLAND 0x004bb6d4
unsigned int ScriptFlashButtonNames;

// GLOBAL: LEGOLAND 0x004bb6f8
struct ScriptCommand LevelScriptCommands[0x5d];

// GLOBAL: LEGOLAND 0x004b86e0
const char LegoFontName[] = "Lego";

// GLOBAL: LEGOLAND 0x004bb49c
const char BuildMenuName[] = "BUILD MENU";

// GLOBAL: LEGOLAND 0x004b8a70
const char PathControlName[] = "PATH CONTROL";

// GLOBAL: LEGOLAND 0x004bcba4
const char *ResourceFileNames[3] = {
    // STRING: LEGOLAND 0x004bcc18
    "Legoland.res",
    // STRING: LEGOLAND 0x004bcc08
    "Graphics2.res",
    // STRING: LEGOLAND 0x004bcbf8
    "Graphics1.res",
};

// GLOBAL: LEGOLAND 0x004bcbf4
LEGO_EXPORT struct LegoConfig *lpConfig;

// GLOBAL: LEGOLAND 0x004bcec0
struct Point PathGoalTile;

// GLOBAL: LEGOLAND 0x004bcecc
char *PTR_s_Aaron_004bcecc[0x53];

// GLOBAL: LEGOLAND 0x004bd018
char *PTR_s_Abbie_004bd018[0x5a];

// GLOBAL: LEGOLAND 0x004bd180
char *PTR_s_Adams_004bd180[0x6b];

// GLOBAL: LEGOLAND 0x004bd32c
short BlokeDirVectors[8][2] = {
    {-181, -181},
    {0, -256},
    {181, -181},
    {256, 0},
    {181, 181},
    {0, 256},
    {-181, 181},
    {-256, 0},
};

// GLOBAL: LEGOLAND 0x004bd34c
BlokeAction BlokeActionTable[16] = {
    LogBlokeRethinking,
    BlokeActionWait,
    BlokeActionWalkPath,
    BlokeActionWalkPathStraight,
    BlokeActionRandomWalk,
    BlokeActionTurn,
    BlokeActionMoveToDestOnPath,
    BlokeActionMoveToDest,
    BlokeActionBounce,
    BlokeActionPassThroughTile,
    BlokeActionWalkUntilPath,
    BlokeActionMoveToDestAvoiding,
    BlokeActionMoveToDestStayOnPath,
    BlokeActionWaitWhileHovered,
    BlokeActionRandomWalkDelayed,
    BlokeActionMoveUntilPath,
};

// GLOBAL: LEGOLAND 0x004bdd00
struct HoverInfo Hover;

// GLOBAL: LEGOLAND 0x004bdea0
LEGO_EXPORT RECT SPRITE_ClipRect;

// GLOBAL: LEGOLAND 0x004beb80
struct ProgressTables ProgressScreenTables;

// GLOBAL: LEGOLAND 0x004bed40
unsigned int TutorialEndSprite0;

// GLOBAL: LEGOLAND 0x004bed44
unsigned int TutorialEndSprite1;

/* Allows front-end click handlers; disabled while a confirmation checkbox/dialog owns input. */
// GLOBAL: LEGOLAND 0x004bef9c
unsigned int FrontEndInputEnabled;

/* First interval report line on the current page (14 lines per page, lines start at 1). */
// GLOBAL: LEGOLAND 0x004bf670
unsigned int IntervalPageFirstLine;

// GLOBAL: LEGOLAND 0x004bf774
unsigned int MusicEnabled;

// GLOBAL: LEGOLAND 0x004bf778
unsigned int MusicState;

/* Twelve tile offsets FindNearbyRepairTarget checks around a worker. */
// GLOBAL: LEGOLAND 0x004bff28
const int RepairSearchOffsets[12][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {0, 2}, {0, -2},
    {2, 0}, {-2, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

// GLOBAL: LEGOLAND 0x004c10d4
struct NewBloke *DrivingSchoolCars;

// GLOBAL: LEGOLAND 0x004c10dc
struct Ride *CastleLevelRide;

// GLOBAL: LEGOLAND 0x004c10e4
struct Sprite *CastleLevelMatteSprite;

// GLOBAL: LEGOLAND 0x004c10e8
struct RenderItemNode *CastleLevelRiderRenderList;

// GLOBAL: LEGOLAND 0x004c10f0
void *CatapultLayer;

// GLOBAL: LEGOLAND 0x004c10f4
struct Ride *ActiveCatapultRide;

// GLOBAL: LEGOLAND 0x004c1100
struct RideSpriteInfo CatapultSpriteInfo;

// GLOBAL: LEGOLAND 0x004c1118
struct CatapultNode *CatapultNodeList;

// GLOBAL: LEGOLAND 0x004c1120
struct Sprite *CopterBaseMatteSprite;

// GLOBAL: LEGOLAND 0x004c1124
struct QueueTable *CopterQueueTables[5];

// GLOBAL: LEGOLAND 0x004c1138
void *CopterModelLayers;

// GLOBAL: LEGOLAND 0x004c113c
struct Sprite *CopterModelSprites[10];

// GLOBAL: LEGOLAND 0x004c1164
struct RenderItemNode **CoptersRenderList2;

// GLOBAL: LEGOLAND 0x004c1168
struct RenderItemNode **CoptersRenderList3;

// GLOBAL: LEGOLAND 0x004c1170
struct RideSpriteInfo CoptersSpriteInfo;

// GLOBAL: LEGOLAND 0x004c1188
struct RenderItemNode **CoptersRenderList4;

// GLOBAL: LEGOLAND 0x004c1190
struct RenderItemNode **CoptersRenderList1;

// GLOBAL: LEGOLAND 0x004c1194
struct RenderItemNode **CoptersRenderList0;

// GLOBAL: LEGOLAND 0x004c1198
struct Ride *ActiveCopterRide;

/* Copters render list heads; the five list pointers after them in CoptersLoad point here, and CoptersRender clears them each frame. */
// GLOBAL: LEGOLAND 0x004c119c
struct RenderItemNode *CoptersRenderListHead0;

// GLOBAL: LEGOLAND 0x004c11a0
struct RenderItemNode *CoptersRenderListHead1;

// GLOBAL: LEGOLAND 0x004c11a4
struct RenderItemNode *CoptersRenderListHead2;

// GLOBAL: LEGOLAND 0x004c11a8
struct RenderItemNode *CoptersRenderListHead3;

// GLOBAL: LEGOLAND 0x004c11ac
struct RenderItemNode *CoptersRenderListHead4;

// GLOBAL: LEGOLAND 0x004c11b0
struct RenderItemNode *CoptersRenderListHead5;

// GLOBAL: LEGOLAND 0x004c11b4
struct CopterNode *CopterNodeList;

// GLOBAL: LEGOLAND 0x004c11bc
struct CountNode *DrivingSchoolCountList;

/* Driving school cars keep to the left lane: they spawn on the left, turn right wide and left tight; the traffic
 * light sprites in UpdateTrafficLights depend on it too. */
// GLOBAL: LEGOLAND 0x004c11c0
int DrivingSchoolDriveOnLeft;

// GLOBAL: LEGOLAND 0x004c11c4
struct RideQueueEntry *RoadBfsFrontier;

// GLOBAL: LEGOLAND 0x004c11c8
struct RideQueueEntry *RoadBfsNextFrontier;

// GLOBAL: LEGOLAND 0x004c11cc
struct Sprite *FortMaskSprite;

// GLOBAL: LEGOLAND 0x004c11d8
struct Sprite *FortLayer;

// GLOBAL: LEGOLAND 0x004c11dc
struct Ride *FortRide;

// GLOBAL: LEGOLAND 0x004c11e0
struct RenderItemNode *FortRiderRenderList;

// GLOBAL: LEGOLAND 0x004c11e4
struct Sprite *GoldRushQueueTable;

// GLOBAL: LEGOLAND 0x004c11e8
struct Sprite *GoldRushLayer;

// GLOBAL: LEGOLAND 0x004c11f0
void *GoldRushRide;

// GLOBAL: LEGOLAND 0x004c11f4
struct Sprite *GoldMaskSprite;

// GLOBAL: LEGOLAND 0x004c11f8
struct Sprite *GoldWashMatte1Sprite;

// GLOBAL: LEGOLAND 0x004c11fc
struct Sprite *GoldWashSprite;

// GLOBAL: LEGOLAND 0x004c1200
struct Sprite *GoldWashMatte2Sprite;

// GLOBAL: LEGOLAND 0x004c1204
struct GoldNode *GoldWashList;

// GLOBAL: LEGOLAND 0x004c1208
struct RenderItemNode *GoldRushBlokeRenderList;

// GLOBAL: LEGOLAND 0x004c1210
struct Sprite *ZJoustSprite;

// GLOBAL: LEGOLAND 0x004c1214
struct Sprite *JoustLayer;

// GLOBAL: LEGOLAND 0x004c1218
void *JoustRideBnv;

// GLOBAL: LEGOLAND 0x004c121c
struct Ride *JoustRide;

// GLOBAL: LEGOLAND 0x004c1228
struct RideSpriteInfo JoustSpriteInfo;

// GLOBAL: LEGOLAND 0x004c123c
void *JoustRiderSprites[2]; /* rider sprite by field_30: [1] is the joust z-sprite */

// GLOBAL: LEGOLAND 0x004c1244
struct Sprite *JoustFMaskSprite;

// GLOBAL: LEGOLAND 0x004c1248
struct Sprite *JoustSpecRMSprite;

// GLOBAL: LEGOLAND 0x004c124c
struct Sprite *JoustSpecLMSprite;

// GLOBAL: LEGOLAND 0x004c1250
struct JoustNode *JoustNodeList;

// GLOBAL: LEGOLAND 0x004c1258
struct Sprite *LogFlumeTunelMSprite;

// GLOBAL: LEGOLAND 0x004c1260
struct Cursor FlumeNeighbourCursor2;

// GLOBAL: LEGOLAND 0x004c2a94
struct Sprite *LogFlumeHup1M1Sprite;

// GLOBAL: LEGOLAND 0x004c2a98
struct Sprite *LogFlumeHup1M2Sprite;

// GLOBAL: LEGOLAND 0x004c2aa8
struct Footprint LogFlumeEntrancePieceFootprint;

// GLOBAL: LEGOLAND 0x004c2aa0
struct Ride *LogFlumeSpecialCorner2Ride;

// GLOBAL: LEGOLAND 0x004c2abc
struct Sprite *LogFlumeTrackSprites[10];

// GLOBAL: LEGOLAND 0x004c2ae4
struct Sprite *LogFlumeTrackSpritesEnd;

// GLOBAL: LEGOLAND 0x004c2ae8
struct QueueTable *FlumeQueueTable0;

// GLOBAL: LEGOLAND 0x004c2aec
struct Sprite *LogFlumeDrop2MSprite;

// GLOBAL: LEGOLAND 0x004c2af0
struct Sprite *LogFlumeDrop1MSprite;

// GLOBAL: LEGOLAND 0x004c2af4
unsigned int FlumeSpecialCornerIndex;

// GLOBAL: LEGOLAND 0x004c2af8
struct QueueTable *FlumeQueueTable1;

// GLOBAL: LEGOLAND 0x004c2afc
struct Element *LogFlumeImageListId;

// GLOBAL: LEGOLAND 0x004c2b0c
struct Ride *LogFlumeSpecialCorner3Ride;

// GLOBAL: LEGOLAND 0x004c2b10
struct Element *LogFlumeTrackEndyListId;

// GLOBAL: LEGOLAND 0x004c2b50
struct Element *LogFlumeHoldUpElement;

// GLOBAL: LEGOLAND 0x004c2b60
struct Ride *LogFlumeHoldUpRide;

// GLOBAL: LEGOLAND 0x004c2b64
struct Sprite *LogFlumeSplashSprite;

// GLOBAL: LEGOLAND 0x004c2b68
struct SpriteSet *LogFlumeImageList;

// GLOBAL: LEGOLAND 0x004c2b6c
struct Sprite *LogFlumeFc1M1Sprite;

// GLOBAL: LEGOLAND 0x004c2b70
struct Sprite *LogFlumeFc1M2Sprite;

// GLOBAL: LEGOLAND 0x004c2b98
struct Element *LogFlumeSpecialCorner1Element;

// GLOBAL: LEGOLAND 0x004c2b9c
struct Ride *LogFlumeEntranceRide;

// GLOBAL: LEGOLAND 0x004c2c18
struct Cursor LogFlumeEntranceCursor2;

// GLOBAL: LEGOLAND 0x004c2ba0
struct Element *LogFlumeSpecialCorner4Element;

// GLOBAL: LEGOLAND 0x004c2bf0
struct Ride *LogFlumeCsaw2Ride;

// GLOBAL: LEGOLAND 0x004c445c
struct Ride *LogFlumeSpecialCorner1Ride;

// GLOBAL: LEGOLAND 0x004c4460
struct Element *LogFlumeCsaw2Element;

// GLOBAL: LEGOLAND 0x004c4468
struct Cursor FlumeConnectionCursor1;

// GLOBAL: LEGOLAND 0x004c5ca0
struct Cursor FlumeConnectionCursor2;

// GLOBAL: LEGOLAND 0x004c74f4
struct Element *LogFlumeTrackElement;

// GLOBAL: LEGOLAND 0x004c74f8
struct Cursor LogFlumeCursor;

// GLOBAL: LEGOLAND 0x004c74d4
struct Ride *LogFlumeSpecialCorner4Ride;

// GLOBAL: LEGOLAND 0x004c74d8
struct RideSpriteInfo LogFlumeTrackSpriteInfo;

// GLOBAL: LEGOLAND 0x004c8d2c
struct Sprite *LogFlumeFc2M1Sprite;

// GLOBAL: LEGOLAND 0x004c8d38
struct Footprint LogFlumeTrackPieceFootprint;

// GLOBAL: LEGOLAND 0x004c8d4c
struct Element *LogFlumeDropElement;

// GLOBAL: LEGOLAND 0x004c8d50
struct Element *LogFlumeSpecialCorner3Element;

// GLOBAL: LEGOLAND 0x004c8d54
struct Sprite *LogFlumeEntranceLayer;

// GLOBAL: LEGOLAND 0x004c8d78
struct Cursor LogFlumeEntranceCursor;

// GLOBAL: LEGOLAND 0x004c8d68
struct Sprite *LogFlumeFc3M3Sprite;

// GLOBAL: LEGOLAND 0x004c8d6c
struct Ride *LogFlumeDropRide;

// GLOBAL: LEGOLAND 0x004c8d70
struct Sprite *LogFlumeFc4MSprite;

// GLOBAL: LEGOLAND 0x004c8d74
struct RenderItemNode *FlumeCornerBarrelsLowWeight;

// GLOBAL: LEGOLAND 0x004ca5ac
struct RenderItemNode *FlumeCornerBarrelsHighWeight;

/* Cursor appended for the first occupied flume neighbour (AppendFlumeNeighbourCursors). */
// GLOBAL: LEGOLAND 0x004ca5b0
struct Cursor FlumeNeighbourCursor1;

// GLOBAL: LEGOLAND 0x004cbe08
struct Sprite *LogFlumeFc3M2Sprite;

// GLOBAL: LEGOLAND 0x004cbe0c
struct Sprite *LogFlumeFc3M1Sprite;

// GLOBAL: LEGOLAND 0x004cbe10
struct Element *LogFlumeSpecialCorner2Element;

// GLOBAL: LEGOLAND 0x004cbe14
struct Sprite *LogFlumeCsaw2MSprite;

// GLOBAL: LEGOLAND 0x004cbe18
struct Ride *LogFlumeTunnelRide;

// GLOBAL: LEGOLAND 0x004cbe1c
struct Sprite *LogFlumeFc1M3Sprite;

// GLOBAL: LEGOLAND 0x004cbe20
struct FlumeEntry *FlumeNeighbours[4]; /* the 4 neighbours of a log flume tile (up, right, down, left) */

// GLOBAL: LEGOLAND 0x004cbe30
struct Ride *LogFlumeTrackRide;

// GLOBAL: LEGOLAND 0x004cbe48
struct Element *LogFlumeTunnelElement;

// GLOBAL: LEGOLAND 0x004cbe4c
struct Sprite *LogFlumeEnta3MSprite;

// GLOBAL: LEGOLAND 0x004cbe50
struct SpriteSet *LogFlumeTrackEndyList;

// GLOBAL: LEGOLAND 0x004cbe58
struct RideSpriteInfo FlumePieceSpriteInfo;

// GLOBAL: LEGOLAND 0x004cbe70
struct RenderItemNode *FlumeRiderRenderList;

// GLOBAL: LEGOLAND 0x004cbe74
struct Sprite *LogFlumeBarrelSprite;

// GLOBAL: LEGOLAND 0x004cbe78
struct Sprite *LogFlumeBarrelMSprite;

// GLOBAL: LEGOLAND 0x004cbe7c
struct Sprite *LogFlumeBarrel1Sprite;

// GLOBAL: LEGOLAND 0x004cbe80
struct Sprite *LogFlumeBarrelMatteSprite;

// GLOBAL: LEGOLAND 0x004cbe84
struct FlumeEntry *FlumeEntryList;

// GLOBAL: LEGOLAND 0x004cbe88
struct Sprite *LogFlumeEnta1Matte2Sprite;

// GLOBAL: LEGOLAND 0x004cbe8c
struct Sprite *LogFlumeSignSprite;

// GLOBAL: LEGOLAND 0x004cbe90
struct Sprite *LogFlumeEntrance2Sprite;

// GLOBAL: LEGOLAND 0x004cbe94
struct Sprite *LogFlumeEntrance3Sprite;

// GLOBAL: LEGOLAND 0x004cbe98
struct Sprite *LogFlumeEntrance1Sprite;

// GLOBAL: LEGOLAND 0x004cbe9c
struct Ride *DrivingSchoolPumpsRide;

// GLOBAL: LEGOLAND 0x004cbea4
struct PumpNode *PumpList;

// GLOBAL: LEGOLAND 0x004cbeac
struct RideQueueEntry *RoadEntryList;

// GLOBAL: LEGOLAND 0x004cbeb0
int TrafficLightA;

// GLOBAL: LEGOLAND 0x004cbeb4
int TrafficLightB;

// GLOBAL: LEGOLAND 0x004cbeb8
unsigned int TrafficLightPhase;

// GLOBAL: LEGOLAND 0x004cbec4
struct Ride *SafariRide;

// GLOBAL: LEGOLAND 0x004cbec0
void *SafariOffBNV;

// GLOBAL: LEGOLAND 0x004cbec8
struct Sprite *SafariLayer;

// GLOBAL: LEGOLAND 0x004cbecc
struct RenderItemNode *SafariRiderRenderList;

// GLOBAL: LEGOLAND 0x004cbed0
struct RideSpriteInfo SafariSpriteInfo;

// GLOBAL: LEGOLAND 0x004cbee8
int SafariRideDrawOffset1X;

// GLOBAL: LEGOLAND 0x004cbeec
int SafariRideDrawOffset1Y;

// GLOBAL: LEGOLAND 0x004cbef4
void *SafariRunBNV;

// GLOBAL: LEGOLAND 0x007988d0
void *DMusicStyles[600];

// GLOBAL: LEGOLAND 0x00799230
struct DirectMusicSegment *MusicIntroSegments[5]; /* seg<theme>1.sgt */

// GLOBAL: LEGOLAND 0x00799244
struct DirectMusicSegment *MusicLoopSegments[5]; /* seg<theme>2.sgt */

// GLOBAL: LEGOLAND 0x00799258
struct DirectMusicSegment *MusicTransitionSegments[5][5]; /* [from][to] */

// GLOBAL: LEGOLAND 0x00799c1c
unsigned char *MusicFileData[35];

// GLOBAL: LEGOLAND 0x0079a608
int MusicFileSizes[35];

// GLOBAL: LEGOLAND 0x0079a69c
void *DMusicNotificationEvent;

/* Number of IMT_GET music segment loads, printed when one fails. */
// GLOBAL: LEGOLAND 0x0079a6b0
int ImtGetCount;

// GLOBAL: LEGOLAND 0x007fe9a4
int SpanDrawPitch;

// GLOBAL: LEGOLAND 0x007fea14
int SoftPrintHeight;

// GLOBAL: LEGOLAND 0x007fea18
unsigned char SpanStartAtOrBeforeMouse;

// GLOBAL: LEGOLAND 0x007fea1c
int SoftPrintWidth;

/* 16-bit colour table the indexed LLS blitters look pixel bytes up in. */
// GLOBAL: LEGOLAND 0x007fea20
unsigned int IndexedLLSPalette;

// GLOBAL: LEGOLAND 0x007fea4c
struct DrawClipOrigin SpriteClipOrigin;

// GLOBAL: LEGOLAND 0x007febac
struct DrawClipSize DrawClipExtent;

// GLOBAL: LEGOLAND 0x0082c66c
struct Sprite *ZSafariSprite;

// GLOBAL: LEGOLAND 0x0082c670
struct Point SafariRideDrawOffset;

/* Safari BNV per rider path index (0 run, 1 on, 2 off), read as an array starting here. */
// GLOBAL: LEGOLAND 0x004cbef8
void *SafariPathRunBNV;

// GLOBAL: LEGOLAND 0x004cbefc
void *SafariPathOnBNV;

// GLOBAL: LEGOLAND 0x004cbf00
void *SafariPathOffBNV;

// GLOBAL: LEGOLAND 0x004cbf04
void *SafariOnBNV[1];

// GLOBAL: LEGOLAND 0x004cbf08
struct Sprite *SafariRiderSprite;

// GLOBAL: LEGOLAND 0x004cbf0c
struct SafariNode *SafariNodeList;

// GLOBAL: LEGOLAND 0x004cbf10
void *SpiderRunBinV;

// GLOBAL: LEGOLAND 0x004cbf14
struct Sprite *SpiderHutMask1Sprite;

// GLOBAL: LEGOLAND 0x004cbf18
void *SpiderOffBinV;

// GLOBAL: LEGOLAND 0x004cbf1c
struct Sprite *SpiderHutMask2Sprite;

// GLOBAL: LEGOLAND 0x004cbf20
struct Ride *ActiveSpiderRide;

// GLOBAL: LEGOLAND 0x004cbf24
void *SpiderOnBinV;

// GLOBAL: LEGOLAND 0x004cbf28
struct Sprite *SpiderRideLayer;

// GLOBAL: LEGOLAND 0x0082c660
Point SpiderRideDrawOffset;

// GLOBAL: LEGOLAND 0x004cbf30
void *SpiderRideBinVs[2];

// GLOBAL: LEGOLAND 0x004cbf38
void *SpiderRideCarAssets[2];

// GLOBAL: LEGOLAND 0x004cbf40
struct RideSpriteInfo SpiderRideSpriteInfo;

// GLOBAL: LEGOLAND 0x004cbf58
struct SpiderNode *SpiderNodeList;

// GLOBAL: LEGOLAND 0x004cbf5c
struct Ride *TempleBuildingRide;

// GLOBAL: LEGOLAND 0x004cbf64
void *TempleLayer;

// GLOBAL: LEGOLAND 0x004cbf68
struct Sprite *TempleMatte1Sprite;

// GLOBAL: LEGOLAND 0x004cbf6c
struct Sprite *TempleMatte2Sprite;

// GLOBAL: LEGOLAND 0x004cbf70
struct RenderItemNode *TempleRiderRenderList;

// GLOBAL: LEGOLAND 0x004cbf7c
struct Sprite *TempleSlideLayer;

// GLOBAL: LEGOLAND 0x004cbf78
struct Sprite *TempSlideMatteSprite;

// GLOBAL: LEGOLAND 0x004cbf80
struct Ride *ActiveTempleSlideRide;

// GLOBAL: LEGOLAND 0x004cbf84
struct RenderItemNode *TempleSlideRiderRenderList;

// GLOBAL: LEGOLAND 0x004cbf88
unsigned int TempleSlideDrawOffset2X;

// GLOBAL: LEGOLAND 0x004cbf8c
unsigned int TempleSlideDrawOffset2Y;

// GLOBAL: LEGOLAND 0x004cbf98
struct RideSpriteInfo TempleSlideSpriteInfo;

// GLOBAL: LEGOLAND 0x004cbfb8
struct BinVFile *TempleSlideBinVs[3];

// GLOBAL: LEGOLAND 0x004cbfc4
void *TempSlideBinV;

/* Initialised so both land in .data in this order: LoadTempleSlide reaches ZTempSlideSprite as the word after
 * TempleSlideDrawOffset, as in the original. */
// GLOBAL: LEGOLAND 0x004cbfc8
struct Point TempleSlideDrawOffset = {0};

// GLOBAL: LEGOLAND 0x004cbfd0
struct Sprite *ZTempSlideSprite = NULL;

// GLOBAL: LEGOLAND 0x004cbfd4
struct SlideNode *SlideNodeList;

// GLOBAL: LEGOLAND 0x004cbfd8
struct Sprite *TopwaterSprite;

// GLOBAL: LEGOLAND 0x004cbfdc
struct Ride *ElephantFountainRide;

// GLOBAL: LEGOLAND 0x004cbfe0
struct Sprite *ShowerLayer;

// GLOBAL: LEGOLAND 0x004cbfe4
struct Element *WaterWorksEntranceElement;

// GLOBAL: LEGOLAND 0x004cbfe8
struct Element *WaterWorksImageListHandle;

// GLOBAL: LEGOLAND 0x004cbfec
struct Sprite *ElephantFountainLayer;

// GLOBAL: LEGOLAND 0x004cbff0
struct RideSpriteInfo WaterWorksSpriteInfo;

// GLOBAL: LEGOLAND 0x004cc008
struct Ride *WaterBlockRide;

// GLOBAL: LEGOLAND 0x004cc014
struct Sprite *ShowerSprite;

// GLOBAL: LEGOLAND 0x004cc018
struct SpriteSet *WaterWorksImageListData;

// GLOBAL: LEGOLAND 0x004cc01c
struct Ride *WaterWorksEntranceRide;

// GLOBAL: LEGOLAND 0x004cc020
struct Sprite *WwElsquirtSprite;

// GLOBAL: LEGOLAND 0x004cc024
struct Ride *ShowerRide;

// GLOBAL: LEGOLAND 0x004cc028
unsigned int WaterWorksSfxRefCount;

// GLOBAL: LEGOLAND 0x004cc02c
struct WaterNode *WaterBlockList;

// GLOBAL: LEGOLAND 0x004cc030
struct WaterNode *ShowerList;

// GLOBAL: LEGOLAND 0x004cc034
struct WaterNode *ElephantFountainList;
// 0x007fffc4, 0x007fffd4, 0x008003f0 are EditCursor fields (field_1404,
// field_1414[5], field_1830) — see struct Cursor in gamemap.h.

struct InterfaceIconNode;
struct InterfaceListNode;
struct InterfaceEventNode;
struct InterfaceResearchNode;

// GLOBAL: LEGOLAND 0x004cc03c
struct BoatRide *BoatRideList;

// GLOBAL: LEGOLAND 0x004cc048
struct Footprint BoatingSchoolExtraFootprint;

// GLOBAL: LEGOLAND 0x004cc060
struct Footprint BoatingSchoolStartFootprint;

// GLOBAL: LEGOLAND 0x004cc074
struct BoatRideNode *BoatRideNodeList;

// GLOBAL: LEGOLAND 0x004cc078
struct Footprint BoatingSchoolFootprint;

// GLOBAL: LEGOLAND 0x004cc08c
unsigned int BoatingSchoolAnimTick;

// GLOBAL: LEGOLAND 0x004cc090
struct Cursor BoatingSchoolMermaidCursors[4];

// GLOBAL: LEGOLAND 0x004d2164
struct MermaidNode *MermaidList;

// GLOBAL: LEGOLAND 0x004d2168
struct Cursor BoatingSchoolWaterCursors[4];

// GLOBAL: LEGOLAND 0x004d823c
struct PathNode *BoatPathList;

// GLOBAL: LEGOLAND 0x004d8240
struct PathNode *BoatBfsFrontier;

// GLOBAL: LEGOLAND 0x004d8244
struct PathNode *BoatBfsNextFrontier;

// GLOBAL: LEGOLAND 0x004d8250
struct PiecePlacement CoasterPlacement;

// GLOBAL: LEGOLAND 0x004d8268
unsigned int CoasterPieceCount;

// GLOBAL: LEGOLAND 0x004d8270
struct VecMath TrainVecMath;

// GLOBAL: LEGOLAND 0x004d829c
float TrainInertiaHistory[64];

// GLOBAL: LEGOLAND 0x004d83c0
unsigned int TrainInertiaHistoryIndex;

// GLOBAL: LEGOLAND 0x004d88f4
unsigned char ShadeClampTable[0x80];

// GLOBAL: LEGOLAND 0x004d8974
unsigned char ShadeClampTableTop[0x40];

// GLOBAL: LEGOLAND 0x004d89b4
unsigned int GouraudSpanState[4];

// GLOBAL: LEGOLAND 0x004d89c4
void *ShadeClamp;

// GLOBAL: LEGOLAND 0x004d89c8
void *LtxFileTable[30];

// GLOBAL: LEGOLAND 0x004d8a40
struct LmsFile *LmsFileTable[31];

// GLOBAL: LEGOLAND 0x004d8abc
struct LmsRec *LfmFileTable[30];

// GLOBAL: LEGOLAND 0x004d8b34
unsigned int LfmFileSizes[30];

// GLOBAL: LEGOLAND 0x004d8bac
int *RollercoasterLpt;

/* 256 zero bytes in .bss, used as an empty string. */
// GLOBAL: LEGOLAND 0x004d8bb0
char EmptyString[0x100];

// GLOBAL: LEGOLAND 0x004dcbb8
float LmsLightDir[3];

// GLOBAL: LEGOLAND 0x004dcbd0
struct VecMath VecMathOps;

// GLOBAL: LEGOLAND 0x004dcc00
struct FloatArray VecScratchBuffers[30];

// GLOBAL: LEGOLAND 0x004dd5d8
unsigned int VecScratchTop;

// GLOBAL: LEGOLAND 0x004dd5e0
void *CurveFunctionTable[24];

// GLOBAL: LEGOLAND 0x004dd758
struct CoasterTextFile CoasterTxtFile;

// GLOBAL: LEGOLAND 0x004dd760
char CoasterObjBaseName[256];

// GLOBAL: LEGOLAND 0x004dd860
struct CoasterTextFile CoasterObjFile;

// GLOBAL: LEGOLAND 0x004dd868
unsigned int CoasterObjLineCount;

// GLOBAL: LEGOLAND 0x004dd86c
unsigned int CoasterTxtLineCount;

// GLOBAL: LEGOLAND 0x004dd870
unsigned char DepthSpanRecords[0x6000];

// GLOBAL: LEGOLAND 0x0060f908
int DepthSpanRecordCount;

// GLOBAL: LEGOLAND 0x0060f90c
int DepthSpanRecordsOverflow;

// GLOBAL: LEGOLAND 0x004e3870
char CoasterDepthBufferData[640 * 480 * 2]; /* the castle view's 16-bit depth buffer (CoasterDepthBuffer) */

// GLOBAL: LEGOLAND 0x0060f914
struct CastleTrackCell CastleTrackCells[20];

// GLOBAL: LEGOLAND 0x006102f8
struct SegBlob CastleCurves[3];

// GLOBAL: LEGOLAND 0x00610a04
unsigned int CastleCoasterPlaced;

// GLOBAL: LEGOLAND 0x00610a08
unsigned int CastleTrackCellCount;

// GLOBAL: LEGOLAND 0x00610a18
struct RenderItemNode *CastleRiderRenderList;

// GLOBAL: LEGOLAND 0x00610a20
float rsqrtf_mantissa_table[128];

// GLOBAL: LEGOLAND 0x00611244
float rsqrtf_exp_table[256];

/* Castle shade range for the current lighting (SetTrackLighting / SetModelLighting): min here, max in the next global. */
// GLOBAL: LEGOLAND 0x00611648
unsigned int CastleShadeMin;

// GLOBAL: LEGOLAND 0x0061164c
float TrackLightingOffset = 0;

// GLOBAL: LEGOLAND 0x00611650
float ModelLightingOffset = 0;

/* Only touched through BuildTrackPieceCurves biased addressing of TrackRailOffsets; never read. */
// GLOBAL: LEGOLAND 0x00611654
float UnusedTrackFloat = 0;

// GLOBAL: LEGOLAND 0x00611658
Vector3 TrackRailOffsets[4] = {0};

// GLOBAL: LEGOLAND 0x00611688
struct KeyValuePair ElementPieceClasses[11] = {0};

// GLOBAL: LEGOLAND 0x006116e0
Vector3 TrackArcCentres[4] = {0};

// GLOBAL: LEGOLAND 0x00611710
unsigned int RailDrawOrder[16] = {0};

// GLOBAL: LEGOLAND 0x00611750
Vector3 TrackArcTangents[4] = {0};

/* Never referenced. */
// GLOBAL: LEGOLAND 0x00611780
unsigned char UnusedTrackScratch[0x40] = {0};

/* Track geometry (BuildTrackPieceCurves, x 20): side midpoints of a cell, rail offsets, arc tangents and arc centres, from the *Unit tables. */
// GLOBAL: LEGOLAND 0x006117c0
Vector3 TrackSideMidpoints[34] = {0};

// GLOBAL: LEGOLAND 0x00611958
unsigned int ElementPieceClassCount;

// GLOBAL: LEGOLAND 0x0061195c
unsigned int TexturedSpanSource;

/* One texel looked up through each coaster shade table (FillSpansTexturedZ). */
// GLOBAL: LEGOLAND 0x00611960
unsigned char TexelShadeCache[0x800];

// GLOBAL: LEGOLAND 0x00612160
unsigned int TexturedSpanState[6];

// GLOBAL: LEGOLAND 0x00612178
char RailSplitPoints[1];

// GLOBAL: LEGOLAND 0x006122a0
float CurvePlotWorldPoints[90][3];

// GLOBAL: LEGOLAND 0x006139c8
unsigned char RailTubeVertexBuffer[0xe90] = {0};

// GLOBAL: LEGOLAND 0x00614858
struct CastleFloatEnt TrackTieVerts[8] = {0};

// GLOBAL: LEGOLAND 0x00616180
struct Cursor JungleCruiseMonkeyFishCursors[8];

// GLOBAL: LEGOLAND 0x00622320
struct Cursor JungleCruiseCursors[4];

// GLOBAL: LEGOLAND 0x006159c8
int CurvePlotScreenPoints[90][4];

// GLOBAL: LEGOLAND 0x00615f6c
unsigned int RailSplitCount;

// GLOBAL: LEGOLAND 0x00615f70
unsigned int SupportPostLmsRec[4];

// GLOBAL: LEGOLAND 0x00615f80
struct CurveWalker *ArcLengthWalker;

// GLOBAL: LEGOLAND 0x00615ff0
int ArcLengthCurvePair;

// GLOBAL: LEGOLAND 0x00615ff4
float ArcLengthDepth;

// GLOBAL: LEGOLAND 0x004b5f80
struct CastleFloatEnt SupportPostShapeTemplate[3][4];

// GLOBAL: LEGOLAND 0x006121c8
float RailTubeRingPoints[6][3] = {0};

// GLOBAL: LEGOLAND 0x00612210
struct CastleFloatEnt SupportPostShape[3][4] = {0};

// GLOBAL: LEGOLAND 0x00615fc4
int BisectRootCallCount;

// GLOBAL: LEGOLAND 0x00615fc8
int BisectRootStepCount;

/* Per-frame castle counters (WalkerRangeFn calls, BisectRoot calls/steps), copied to 64-frame rings by RecordCastleFrameCounters. */
// GLOBAL: LEGOLAND 0x00615fcc
int WalkerRangeCallCount;

// GLOBAL: LEGOLAND 0x00615fd0
float RangeRadius;

// GLOBAL: LEGOLAND 0x00615fd8
float RangeRadiusSq;

// GLOBAL: LEGOLAND 0x00615fdc
float RangeOuterSq;

// GLOBAL: LEGOLAND 0x00615fe0
float RangeInnerSq;

// GLOBAL: LEGOLAND 0x00615fe4
int WalkerRangeOutsideCount;

// GLOBAL: LEGOLAND 0x00615fe8
int WalkerRangeNormalCount;

// GLOBAL: LEGOLAND 0x00615fec
int BisectRootMaxSteps;

// GLOBAL: LEGOLAND 0x00615f8c
float *RangeCentre;

// GLOBAL: LEGOLAND 0x004b63fc
int (*RangeRootFinder)(float (*fn)(unsigned int), float lo, float hi, float *out);

// GLOBAL: LEGOLAND 0x00615f84
struct CurveWalker *RangeWalker;

// GLOBAL: LEGOLAND 0x00615f90
int RangeWalkerSegment;

// GLOBAL: LEGOLAND 0x00615fd4
float RangeDepth;

// GLOBAL: LEGOLAND 0x00615f98
struct VecMath CurveVecMath;

// GLOBAL: LEGOLAND 0x00616000
struct LmsFile *CoasterTrainWheelLms;

// GLOBAL: LEGOLAND 0x00616004
struct LmsRec *CoasterTrainWheelLfm;

// GLOBAL: LEGOLAND 0x00616010
struct BinVFile *BalloonzBinV;

// GLOBAL: LEGOLAND 0x00616018
struct BinVFile *BalloonzBinVs[4];

// GLOBAL: LEGOLAND 0x0061603c
struct Sprite *BalloonzRiderSprites[2]; /* rider sprite by field_30: [1] is ZBalloon2Sprite */

// GLOBAL: LEGOLAND 0x00616044
struct Sprite *BalloonzLayer;

// GLOBAL: LEGOLAND 0x00616048
struct Sprite *BallBaseM1Sprite;

// GLOBAL: LEGOLAND 0x0061604c
struct Sprite *BallBaseM2Sprite;

// GLOBAL: LEGOLAND 0x00616050
struct Sprite *BallBaseM3Sprite;

// GLOBAL: LEGOLAND 0x00616054
struct Sprite *BZRedCarM1Sprite;

// GLOBAL: LEGOLAND 0x00616058
struct Sprite *BZGreenCarM1Sprite;

// GLOBAL: LEGOLAND 0x0061605c
struct Sprite *BZBlueCarM1Sprite;

// GLOBAL: LEGOLAND 0x00616060
struct BalloonNode *BalloonNodeList;

// GLOBAL: LEGOLAND 0x00616068
struct Sprite *CarouselLayer;

// GLOBAL: LEGOLAND 0x0061606c
struct Sprite *CarouselEntranceMatteSprite;

// GLOBAL: LEGOLAND 0x00616070
struct Sprite *CarouselEntranceMatte2Sprite;

// GLOBAL: LEGOLAND 0x00616078
int CarouselDrawOffsetX;

// GLOBAL: LEGOLAND 0x0061607c
int CarouselDrawOffsetY;

// GLOBAL: LEGOLAND 0x00616080
void *CarouselOnBinV;

// GLOBAL: LEGOLAND 0x00616084
void *CarouselOffBinV;

// GLOBAL: LEGOLAND 0x0061608c
void *CarouselBinV;

// GLOBAL: LEGOLAND 0x00616090
void *CarouselBinVs[3]; /* carousel BinVs (on, ride, off), indexed by the saved path index */

// GLOBAL: LEGOLAND 0x006160b8
struct Sprite *ZCarouselSprite;

// GLOBAL: LEGOLAND 0x006160bc
struct Ride *ActiveCarouselRide;

// GLOBAL: LEGOLAND 0x006160c0
struct Sprite *CarouselRiderSprite;

// GLOBAL: LEGOLAND 0x006160c4
struct CarouselNode *CarouselNodeList;

// GLOBAL: LEGOLAND 0x006160c8
unsigned int UnusedEarthSlideA;

// GLOBAL: LEGOLAND 0x006160cc
unsigned int UnusedEarthSlideB;

// GLOBAL: LEGOLAND 0x006160d0
struct Ride *EarthSlideRide;

// GLOBAL: LEGOLAND 0x006160d4
struct RinData *EarthSlideRin;

// GLOBAL: LEGOLAND 0x006160d8
struct Sprite *EarthSlideEntranceMatteSprite;

// GLOBAL: LEGOLAND 0x006160e0
struct Sprite *EarthSlideEntranceMatte2Sprite;

// GLOBAL: LEGOLAND 0x006160e4
struct Position *EarthPos;

// GLOBAL: LEGOLAND 0x006160e8
struct EarthNode *EarthNodeHead;

// GLOBAL: LEGOLAND 0x006160ec
struct RenderItemNode *BlokeRenderList;

// GLOBAL: LEGOLAND 0x006160f4
struct Ride *EntranceRide;

// GLOBAL: LEGOLAND 0x006160f0
struct Sprite *EntranceLayer;

// GLOBAL: LEGOLAND 0x00616110
int Entrance1AlternateBit;

// GLOBAL: LEGOLAND 0x006160fc
struct Sprite *EntranceMatte1Sprite;

// GLOBAL: LEGOLAND 0x00616100
struct Sprite *EntranceMatte2Sprite;

// GLOBAL: LEGOLAND 0x00616104
struct Sprite *EntranceMatte3Sprite;

// GLOBAL: LEGOLAND 0x00616108
struct Sprite *EntranceMatte4Sprite;

// GLOBAL: LEGOLAND 0x0061610c
struct Sprite *Booth1Sprite;

// GLOBAL: LEGOLAND 0x00616118
struct Sprite *EateryLayerOwner;

// GLOBAL: LEGOLAND 0x00616120
struct RideSpriteInfo EaterySpriteInfo;

// GLOBAL: LEGOLAND 0x0061613c
struct Element *BrollyImagesHandle;

// GLOBAL: LEGOLAND 0x00616140
struct BrollyData *BrollyImagesData;

// GLOBAL: LEGOLAND 0x00616144
struct BrollyNode *BrollyNodeList;

// GLOBAL: LEGOLAND 0x00616148
struct SaveBlock *SaveBlockList;

// GLOBAL: LEGOLAND 0x0061614c
struct SpriteSet *HedgeImagesData;

// GLOBAL: LEGOLAND 0x00616150
struct Element *FlowerImagesHandle;

// GLOBAL: LEGOLAND 0x00616158
struct SpriteSet *FlowerImagesData;

// GLOBAL: LEGOLAND 0x0061615c
struct Element *HedgeImagesHandle;

// GLOBAL: LEGOLAND 0x00616164
struct JungleRide *JungleRideList;

// GLOBAL: LEGOLAND 0x00629c2c
struct JungleObj *JungleObjList;

// GLOBAL: LEGOLAND 0x00629c30
struct JungleFish *JungleFishList;

// GLOBAL: LEGOLAND 0x00629c34
struct JungleObj *JungleCruiseInstanceList;

// GLOBAL: LEGOLAND 0x00629c3c
struct JungleScore *JungleScoreList;

// GLOBAL: LEGOLAND 0x00629c40
struct Footprint JungleCruiseFootprint;

// GLOBAL: LEGOLAND 0x00629c54
int JungleCruiseStep;

// GLOBAL: LEGOLAND 0x00629c58
struct Cursor JungleCruiseWaterCursors[4];

// GLOBAL: LEGOLAND 0x0062fd2c
struct JunglePath *JunglePathList;

// GLOBAL: LEGOLAND 0x0062fd30
struct JunglePath *JungleBfsFrontier;

// GLOBAL: LEGOLAND 0x0062fd34
struct JunglePath *JungleBfsNextFrontier;

// GLOBAL: LEGOLAND 0x0062fd3c
struct JailCell *JailCellList;

// GLOBAL: LEGOLAND 0x0062fd40
void *WesternTownLayer;

// GLOBAL: LEGOLAND 0x0062fd48
struct RideSpriteInfo SpaceTowerSpriteInfo;

// GLOBAL: LEGOLAND 0x0062fd60
struct Sprite *SpaceTowerLayers;

// GLOBAL: LEGOLAND 0x0062fd64
struct Sprite *SpaceTowerSeatMatteSprites[4];

// GLOBAL: LEGOLAND 0x0062fd74
struct Ride *SpaceTowerRideObj;

// GLOBAL: LEGOLAND 0x0062fd7c
struct Sprite *SpaceTowerMatte2Sprite;

// GLOBAL: LEGOLAND 0x0062fd80
struct Sprite *SpaceTowerMatte1Sprite;

// GLOBAL: LEGOLAND 0x0062fd88
struct Point SpaceTowerLayerOffsets[4];

// GLOBAL: LEGOLAND 0x0062fda8
struct SpaceTowerCar *SpaceTowerCarList;

// GLOBAL: LEGOLAND 0x0062fdb0
struct RideSpriteInfo SpinningBarrelsSpriteInfo;

// GLOBAL: LEGOLAND 0x0062fdc8
void *BoxBlokesOffBNV;

// GLOBAL: LEGOLAND 0x0062fdcc
struct Sprite *SpinningBarrelsEntranceMatte2Sprite;

// GLOBAL: LEGOLAND 0x0062fdd0
struct Sprite *SpinningBarrelsEntranceMatteSprite;

// GLOBAL: LEGOLAND 0x0062fde4
struct Ride *SpinningBarrelsRide;

// GLOBAL: LEGOLAND 0x0062fde0
struct Sprite *SpinningBarrelsLayer;

// GLOBAL: LEGOLAND 0x0062fdd8
struct Point SpinningBarrelsDrawOffset;

// GLOBAL: LEGOLAND 0x0062fde8
void *SpinningBarrelsBNV;

// GLOBAL: LEGOLAND 0x0062fdf0
void *SpinningBarrelsBinVs[3];

// GLOBAL: LEGOLAND 0x0062fdfc
void *BoxBlokesOn1BNV;

// GLOBAL: LEGOLAND 0x0062fe00
void *SpinningBarrelsRiderSprites[2]; /* rider sprite by field_30: [1] is ZSpinningBarrelsSprite */

// GLOBAL: LEGOLAND 0x0062fe08
struct BarrelNode *SpinningBarrelList;

// GLOBAL: LEGOLAND 0x0062fe48
struct Sprite *PottingShedLayer;

// GLOBAL: LEGOLAND 0x0062fe4c
struct Sprite *GShedMatteSprite;

// GLOBAL: LEGOLAND 0x0062fe50
struct Sprite *MechanicsHutLayer;

// GLOBAL: LEGOLAND 0x0062fe54
struct Sprite *MechHutMaskSprite;

// GLOBAL: LEGOLAND 0x0062fe58
struct Ride *PlaneRide;

// GLOBAL: LEGOLAND 0x0062fe60
struct RideSpriteInfo PlaneRideSpriteInfo;

// GLOBAL: LEGOLAND 0x0062fe78
void *Zoomer0ffBinV;

// GLOBAL: LEGOLAND 0x0062fe7c
struct Sprite *PlaneRideLayer;

// GLOBAL: LEGOLAND 0x0062fe98
struct Sprite *PlaneRideRiderSprite;

// GLOBAL: LEGOLAND 0x0062fe84
void *PlaneRideBinVs[3];

// GLOBAL: LEGOLAND 0x0062fe90
void *ZoomerideBinV;

// GLOBAL: LEGOLAND 0x0062fe94
void *Zoomer0nBinV;

// GLOBAL: LEGOLAND 0x0062fe9c
struct PlaneRideNode *PlaneRideNodeList;

// GLOBAL: LEGOLAND 0x0062fea0
int DialogListScrollY;

// GLOBAL: LEGOLAND 0x0062fea4
int DialogSliderDragging;

// GLOBAL: LEGOLAND 0x0062fea8
int *AltManTexturesB;

// GLOBAL: LEGOLAND 0x0062feac
void *AltWomanFileData;

// GLOBAL: LEGOLAND 0x0062feb0
void *GeoffMeshes[2];

// GLOBAL: LEGOLAND 0x0062feb8
int AltManTextureCountB[1];

// GLOBAL: LEGOLAND 0x0062febc
void *ManMeshes[6];

// GLOBAL: LEGOLAND 0x0062fed4
void *WomanMeshes[6];

// GLOBAL: LEGOLAND 0x0062feec
struct RenderItemNode *RenderItemPoolNext[1];

// GLOBAL: LEGOLAND 0x0062fef0
struct RenderItemNode *RenderItem2PoolNext;

// GLOBAL: LEGOLAND 0x0062fef4
void *TracyWalkMesh;

// GLOBAL: LEGOLAND 0x0062fef8
int *AltWomanTexturesA;

// GLOBAL: LEGOLAND 0x00630100
void *AltManFileData;

// GLOBAL: LEGOLAND 0x00638358
unsigned short SavedFPUControlWord;

// GLOBAL: LEGOLAND 0x0064cd8c
int *AltWomanTexturesB;

// GLOBAL: LEGOLAND 0x00655a38
int *AltManTexturesA;

// GLOBAL: LEGOLAND 0x00655a3c
void *PersonListHead;

// GLOBAL: LEGOLAND 0x00655a4c
unsigned int RenderItemCount;

// GLOBAL: LEGOLAND 0x00655a50
unsigned int RenderItem2Count;

// GLOBAL: LEGOLAND 0x00665e8c
unsigned int LoadedTextureCount;

// GLOBAL: LEGOLAND 0x00665eec
int AdvisorAnimFrame;

// GLOBAL: LEGOLAND 0x00665f48
unsigned int AviAnimOpenCount;

// GLOBAL: LEGOLAND 0x00665f5c
void *CurrentAdvisorAnim;

// GLOBAL: LEGOLAND 0x00665f60
void *NextAdvisorAnim;

// GLOBAL: LEGOLAND 0x00665f64
unsigned int UnusedAdvisorVidA;

// GLOBAL: LEGOLAND 0x00665f68
unsigned int UnusedAdvisorVidB;

// GLOBAL: LEGOLAND 0x00665f6c
unsigned int UnusedAdvisorVidC;

// GLOBAL: LEGOLAND 0x00665fe8
unsigned int AdvisorAnimState;

// GLOBAL: LEGOLAND 0x00665fec
unsigned int NextAdvisorAnimState;

// GLOBAL: LEGOLAND 0x00665ff8
unsigned int ReportFlags;

// GLOBAL: LEGOLAND 0x00665ffc
unsigned int ReportLegolandZoneGoal;

// GLOBAL: LEGOLAND 0x00666000
int ReportAdventurerZoneGoal;

// GLOBAL: LEGOLAND 0x00666004
int ReportCastleZoneGoal;

// GLOBAL: LEGOLAND 0x00666008
int ReportWesternZoneGoal;

// GLOBAL: LEGOLAND 0x0066600c
int CastleScoreGoal;

// GLOBAL: LEGOLAND 0x00666010
int DrivingSchoolScoreGoal;

// GLOBAL: LEGOLAND 0x00666014
int LogFlumeScoreGoal;

// GLOBAL: LEGOLAND 0x00666018
int BoatingSchoolScoreGoal;

// GLOBAL: LEGOLAND 0x0066601c
int JungleCruiseScoreGoal;

// GLOBAL: LEGOLAND 0x00666020
int ReportType1Or3ObjectCountGoal;

// GLOBAL: LEGOLAND 0x00666024
int ReportType1Or3ObjectCountMax;

// GLOBAL: LEGOLAND 0x00666028
int ReportType1Or3ClassCountGoal;

// GLOBAL: LEGOLAND 0x0066602c
int ReportType1Or3ClassCountMax;

// GLOBAL: LEGOLAND 0x00666030
int ReportLinkedObjectPercentGoal;

// GLOBAL: LEGOLAND 0x00666034
int ReportLinkedObjectPercentMax;

// GLOBAL: LEGOLAND 0x00666038
int ReportVisitorCountGoal;

// GLOBAL: LEGOLAND 0x0066603c
int ReportVisitorCountMax;

// GLOBAL: LEGOLAND 0x00666040
int ReportFoodStallObjectCountGoal;

// GLOBAL: LEGOLAND 0x00666044
int ReportFoodStallObjectCountMax;

// GLOBAL: LEGOLAND 0x00666048
int ReportFoodStallClassCountGoal;

// GLOBAL: LEGOLAND 0x0066604c
int ReportFoodStallClassCountMax;

// GLOBAL: LEGOLAND 0x00666050
int ReportVisitorHappinessGoal;

// GLOBAL: LEGOLAND 0x00666054
int ReportVisitorHappinessMax;

// GLOBAL: LEGOLAND 0x00666058
int ReportPowerSupplyGoal;

// GLOBAL: LEGOLAND 0x0066605c
int ReportPowerSupplyMax;

// GLOBAL: LEGOLAND 0x00666060
int ReportWorkingRidesGoal;

// GLOBAL: LEGOLAND 0x00666064
int ReportWorkingRidesMax;

// GLOBAL: LEGOLAND 0x00666068
int ReportMapTileCountGoal;

// GLOBAL: LEGOLAND 0x0066606c
int ReportMapTileCountMax;

// GLOBAL: LEGOLAND 0x00666070
int ReportType2ObjectCountGoal;

// GLOBAL: LEGOLAND 0x00666074
int ReportType2ObjectCountMax;

// GLOBAL: LEGOLAND 0x00666078
int ReportType2ClassCountGoal;

// GLOBAL: LEGOLAND 0x0066607c
int ReportType2ClassCountMax;

// GLOBAL: LEGOLAND 0x00666080
int ReportSceneryCoverageGoal;

// GLOBAL: LEGOLAND 0x00666084
int ReportSceneryCoverageMax;

// GLOBAL: LEGOLAND 0x00666088
int ReportType4ObjectCountGoal;

// GLOBAL: LEGOLAND 0x0066608c
int ReportType4ObjectCountMax;

// GLOBAL: LEGOLAND 0x00666090
int ReportType4ClassCountGoal;

// GLOBAL: LEGOLAND 0x00666094
int ReportType4ClassCountMax;

// GLOBAL: LEGOLAND 0x00666098
unsigned int AppraisalDeadline;

/* Result of the last RunAppraisal started by CheckAppraisalDue. */
// GLOBAL: LEGOLAND 0x0066609c
int LastAppraisalResult;

// GLOBAL: LEGOLAND 0x006660a0
int AppraisalPageCount;

// GLOBAL: LEGOLAND 0x006660a4
int AppraisalPage;

// GLOBAL: LEGOLAND 0x006660a8
struct IconNode *AppraisalNextIcon;

// GLOBAL: LEGOLAND 0x006660ac
struct IconNode *AppraisalPrevIcon;

// GLOBAL: LEGOLAND 0x006660b0
char GfxFileNameBuffer[256];

// GLOBAL: LEGOLAND 0x006661bc
int ParkVisitorCount;

// GLOBAL: LEGOLAND 0x006661c0
struct Element *SharkCafeBrollyElem;

// GLOBAL: LEGOLAND 0x006661c4
struct Element *Entrance1Elem;

// GLOBAL: LEGOLAND 0x006661c8
int VisitorSpawnCounter;

// GLOBAL: LEGOLAND 0x006661cc
char BlokeAiLogLines[8][100];

// GLOBAL: LEGOLAND 0x006664ec
int BlokeAiLogIndex;

// GLOBAL: LEGOLAND 0x006664f8
struct BuildObj BuildObjArray[256] = {0};

// GLOBAL: LEGOLAND 0x006670f8
int BuildObjCount = 0;

// GLOBAL: LEGOLAND 0x006670fc
unsigned int ButtonRepeatDelay;

// GLOBAL: LEGOLAND 0x00667104
unsigned int ControllersInitialized;

/* Set by SelectMatchingBuildElement; ReadGameButtons then swallows the next button release. */
// GLOBAL: LEGOLAND 0x00667108
unsigned int IgnoreNextButtonRelease;

// GLOBAL: LEGOLAND 0x0066710c
unsigned int ButtonRepeatLastTick;

// GLOBAL: LEGOLAND 0x00667114
unsigned int FountainSFXRefCount;

// GLOBAL: LEGOLAND 0x00667118
unsigned int PowerStationSFXRefCount;

// GLOBAL: LEGOLAND 0x0066711c
unsigned int DinoSFXRefCount;

// GLOBAL: LEGOLAND 0x00667120
unsigned int MoneySFXLoadCount;

// GLOBAL: LEGOLAND 0x00667128
char LogBuffer[512];

// GLOBAL: LEGOLAND 0x0066752c
char ProductVersionString[0x88];

// GLOBAL: LEGOLAND 0x006675b4
unsigned int BubbleHelpGFXLoaded;

// GLOBAL: LEGOLAND 0x006675b8
volatile int TextCellCount;

// GLOBAL: LEGOLAND 0x006675c0
struct TextCell TextCells[50];

// GLOBAL: LEGOLAND 0x00667c00
int MapWorldMinX;

// GLOBAL: LEGOLAND 0x00667c04
int MapWorldMinY;

// GLOBAL: LEGOLAND 0x00667c08
int MapWorldMaxX;

// GLOBAL: LEGOLAND 0x00667c0c
int MapWorldMaxY;

// GLOBAL: LEGOLAND 0x00667c14
short MapDrawHeight;

// GLOBAL: LEGOLAND 0x00667c16
short MapDrawWidth;

// GLOBAL: LEGOLAND 0x00667c18
int MapWorldHeight;

// GLOBAL: LEGOLAND 0x00667c1c
int MapWorldWidth;

// GLOBAL: LEGOLAND 0x00667c20
int MapCenterOffsetY;

// GLOBAL: LEGOLAND 0x00667c10
void *SavedOkKeyHandler;

// GLOBAL: LEGOLAND 0x00667c28
void *SavedCancelKeyHandler;

// GLOBAL: LEGOLAND 0x00667c2c
struct Sprite *FullMapSprite;

// GLOBAL: LEGOLAND 0x00667c30
unsigned int FullMapRendered;

// GLOBAL: LEGOLAND 0x00667c34
struct Sprite *MapSpannerSprite;

// GLOBAL: LEGOLAND 0x00667c3c
int FtolScratch;

// GLOBAL: LEGOLAND 0x00667c40
const char *AdvisorDebugStage;

/* Cleared by ReadGameButtons every frame; set once something has handled this frame's mouse click, so GameProc skips the focussed icon. */
// GLOBAL: LEGOLAND 0x00667c48
unsigned int MouseClickConsumed;

/* Return value of the last LoadLevelScript call; not read elsewhere. */
// GLOBAL: LEGOLAND 0x00667c4c
int LevelScriptLoadResult;

// GLOBAL: LEGOLAND 0x00667c54
LEGO_EXPORT TileId QueryObj;

// GLOBAL: LEGOLAND 0x00667c58
LEGO_EXPORT struct Ride *QueryClass;

// GLOBAL: LEGOLAND 0x00667c5c
unsigned int UnusedMapEditFlag;

// GLOBAL: LEGOLAND 0x00667c60
unsigned int PreMapScreenMode;

// GLOBAL: LEGOLAND 0x00667c68
int LastMapClickTicks;

// GLOBAL: LEGOLAND 0x00667c70
long LastMapClickX;

// GLOBAL: LEGOLAND 0x00667c74
long LastMapClickY;

/* Set by SavedGameLoadClicked: the next UpdateApplicationFrame loads the pending save (or starts the level) and clears it. */
// GLOBAL: LEGOLAND 0x00667c64
unsigned int GameStartPending;

// GLOBAL: LEGOLAND 0x00667c78
unsigned int SamplesPaused;

// GLOBAL: LEGOLAND 0x00667c7c
unsigned int MapLoaded;

/* Set by SavedGameLoadClicked; UpdateApplicationFrame then loads the chosen save game and clears it. */
// GLOBAL: LEGOLAND 0x00667c80
unsigned int SaveGameLoadPending;

// GLOBAL: LEGOLAND 0x00667c88
struct Sprite *Arrow02Sprite;

// GLOBAL: LEGOLAND 0x00667c8c
struct Sprite *Arrow01Sprite;

// GLOBAL: LEGOLAND 0x00667c90
struct Sprite *Arrow04Sprite;

// GLOBAL: LEGOLAND 0x00667c94
struct Sprite *Arrow03Sprite;

// GLOBAL: LEGOLAND 0x00667c9c
void *GameMapRawBlock;

// GLOBAL: LEGOLAND 0x00667ca0
unsigned int LoadInProgress;

// GLOBAL: LEGOLAND 0x00667ca4
unsigned int LevelBaseTileId;

// GLOBAL: LEGOLAND 0x00667ca8
LEGO_EXPORT void *OverlayList;

// GLOBAL: LEGOLAND 0x00667cac
LEGO_EXPORT struct SpriteSet *OverlayILF;

// GLOBAL: LEGOLAND 0x00667cb0
struct SpriteSet *BridgesData;

// GLOBAL: LEGOLAND 0x00667cb4
LEGO_EXPORT int ScrollX;

// GLOBAL: LEGOLAND 0x00667cb8
LEGO_EXPORT int ScrollY;

// GLOBAL: LEGOLAND 0x00667cbc
LEGO_EXPORT int ScrollSpeedX;

// GLOBAL: LEGOLAND 0x00667cc0
LEGO_EXPORT int ScrollSpeedY;

// GLOBAL: LEGOLAND 0x00667cc4
int UnusedRenderViewCounter;

// GLOBAL: LEGOLAND 0x00667cd0
int BackgroundOffsetX;

// GLOBAL: LEGOLAND 0x00667cd4
int BackgroundOffsetY;

/* Set while many objects are built/removed at once (drag removal, UnloadMap): per-object sounds and
 * CalculateMapRenderOrder are skipped and MapBatchChanged records that something changed. */
// GLOBAL: LEGOLAND 0x00667cd8
unsigned int MapBatchUpdate;

// GLOBAL: LEGOLAND 0x00667cdc
unsigned int MapBatchChanged;

// GLOBAL: LEGOLAND 0x00667ce0
int TotalCoveredSquares;

// GLOBAL: LEGOLAND 0x00667ce4
int RideCoveredSquares;

// GLOBAL: LEGOLAND 0x00667ce8
int ShopCoveredSquares;

// GLOBAL: LEGOLAND 0x00667cec
int FoodCoveredSquares;

// GLOBAL: LEGOLAND 0x00667cf0
int Type3CoveredSquares;

// GLOBAL: LEGOLAND 0x00667cf4
int PathSquareCount;

// GLOBAL: LEGOLAND 0x00667cf8
int SceneryCoveredSquares;

// GLOBAL: LEGOLAND 0x00667cfc
int UnusedCoverageCounter;

// GLOBAL: LEGOLAND 0x00667d00
int PathEdgeSquareTotal;

// GLOBAL: LEGOLAND 0x00667d04
int PathEdgeScenerySquares;

// GLOBAL: LEGOLAND 0x00667d08
int PathSceneryPercent;

// GLOBAL: LEGOLAND 0x00667d0c
int PathSceneryDirty;

// GLOBAL: LEGOLAND 0x00667d10
unsigned int PathSceneryLastUpdate;

/* Number of rectangles in RectList (AddRectToRectList / SubtractRectFromRectList). */
// GLOBAL: LEGOLAND 0x00667d3c
int RectListCount;

/* RenderView draws the path tile overlay while demolishing or placing path. */
// GLOBAL: LEGOLAND 0x00667d40
unsigned int ShowPathTileOverlay;

// GLOBAL: LEGOLAND 0x00667d44
int DeferredSpriteCount;

// GLOBAL: LEGOLAND 0x00667d48
unsigned int PathOverlayBlinkMask;

// GLOBAL: LEGOLAND 0x00667d4c
unsigned int LastMouseScrollFrameMS;

// GLOBAL: LEGOLAND 0x00667d50
unsigned int MapDataLoaded;

// GLOBAL: LEGOLAND 0x00667d54
unsigned int DamageTimerStart;

/* Game timer frame of the last ride wear tick (IsRideWearDue). */
// GLOBAL: LEGOLAND 0x00667d58
int LastRideWearFrame;

// GLOBAL: LEGOLAND 0x00667d60
int LastWatchDrawTicks;

// GLOBAL: LEGOLAND 0x00667d68
unsigned int LastRenderingCompleteTick;

// GLOBAL: LEGOLAND 0x00667d6c
int WinDebugMode;

// GLOBAL: LEGOLAND 0x00667d70
LEGO_EXPORT struct DDRAWENV DDRAWENV;

// GLOBAL: LEGOLAND 0x00668070
LPDIRECTDRAWSURFACE PrimarySurface;

// GLOBAL: LEGOLAND 0x00668074
LPDIRECTDRAWSURFACE SecondaryOffscreenSurface;

// GLOBAL: LEGOLAND 0x00668078
LPDIRECTDRAWSURFACE OffscreenSurface;

// GLOBAL: LEGOLAND 0x0066807c
LPDIRECTDRAWSURFACE renderEngine;

// GLOBAL: LEGOLAND 0x00668080
LPDIRECTDRAWCLIPPER DDrawClipper;

// GLOBAL: LEGOLAND 0x00668084
LPDIRECTDRAWPALETTE DDPalette;

// GLOBAL: LEGOLAND 0x00668088
unsigned int DisplayPixelFormat;

// GLOBAL: LEGOLAND 0x0066808c
HFONT LegoFont20Bold;

// GLOBAL: LEGOLAND 0x00668090
HFONT LegoFont24Bold;

// GLOBAL: LEGOLAND 0x00668094
HFONT LegoFont18SemiBold;

// GLOBAL: LEGOLAND 0x00668098
HFONT LegoFont28Normal;

// GLOBAL: LEGOLAND 0x0066809c
DDSURFACEDESC CurrentSurfaceDesc;

// GLOBAL: LEGOLAND 0x00668108
RECT SoftPrintClip;

// GLOBAL: LEGOLAND 0x00668118
int renderEngineTargetIdx;

// GLOBAL: LEGOLAND 0x0066811c
LPDIRECTDRAWSURFACE renderEngineTargets[10];

// GLOBAL: LEGOLAND 0x00668144
int VideoSurfaceLocked;

// GLOBAL: LEGOLAND 0x00668148
struct Sprite *CurrentPointerSprite;

// GLOBAL: LEGOLAND 0x0066814c
unsigned int CurrentPointerIndex;

/* Hand-written LLS blitters: 2-bit mask codes left in the current mask dword (reloaded at 16). */
// GLOBAL: LEGOLAND 0x00668160
unsigned int LLSMaskCodesLeft;

// GLOBAL: LEGOLAND 0x00668164
int RenderingStatusLockedStack[32];

// GLOBAL: LEGOLAND 0x006681e4
int RenderingStatusStackDepth;

// GLOBAL: LEGOLAND 0x006681e8
unsigned short *OverridePalette;

// GLOBAL: LEGOLAND 0x006681ec
void *MovieLastDoubledSurface;

// GLOBAL: LEGOLAND 0x006681f0
unsigned int FpsWindowStartTicks;

// GLOBAL: LEGOLAND 0x006681f4
unsigned int LastFrameTicks;

// GLOBAL: LEGOLAND 0x006681f8
int FramesThisSecond;

// GLOBAL: LEGOLAND 0x006681fc
LEGO_EXPORT unsigned int LastFrameMS;

// GLOBAL: LEGOLAND 0x00668200
unsigned int LastPresentTicks;

// GLOBAL: LEGOLAND 0x00668204
unsigned int WatchActive;

// GLOBAL: LEGOLAND 0x00668208
struct Sprite *WatchSprite;

// GLOBAL: LEGOLAND 0x0066820c
char ObjectiveTextBuffer[0x404];

// GLOBAL: LEGOLAND 0x00668610
unsigned int ScriptDirtyCategories;

// GLOBAL: LEGOLAND 0x00668614
unsigned int CurrentObjectivePromptIndex;

/* Repeated-objective request allowing the next advisor message to bypass its ordinary delay. */
// GLOBAL: LEGOLAND 0x00668618
unsigned int AdvisorRepeatPending;

// GLOBAL: LEGOLAND 0x0066861c
char BriefingFileName[128];

// GLOBAL: LEGOLAND 0x0066869c
char HintsFileName[128];

/* Object reward/availability change requires rebuilding the menu and updating new-object information. */
// GLOBAL: LEGOLAND 0x0066871c
unsigned int BuildMenuRefreshPending;

// GLOBAL: LEGOLAND 0x00668720
unsigned int ScriptStringCount;

/* Advisor messages from objectives, sorted by sort_key; ShowNextObjectiveMessage pops the head. */
// GLOBAL: LEGOLAND 0x00668724
struct ObjectiveEvent *ObjectiveMessageQueue;

// GLOBAL: LEGOLAND 0x00668728
struct ObjectiveEvent *ObjectiveEventList;

// GLOBAL: LEGOLAND 0x0066872c
unsigned int ObjectiveEventTypeCounts[0x15];

// GLOBAL: LEGOLAND 0x00668780
unsigned int ObjectiveHintTimer;

// GLOBAL: LEGOLAND 0x00668784
struct ObjectiveEvent *ScriptEventList;

// GLOBAL: LEGOLAND 0x00668788
unsigned int ScriptPurgePending;

// GLOBAL: LEGOLAND 0x0066878c
int ScriptNeedRemaining;

// GLOBAL: LEGOLAND 0x00668790
unsigned int ScriptSectionStarted;

// GLOBAL: LEGOLAND 0x00668794
unsigned int ScriptEvalTime;

// GLOBAL: LEGOLAND 0x00668798
struct NerpsListNode *ScriptSectionList;

// GLOBAL: LEGOLAND 0x0066879c
struct NerpsListNode *CurrentScriptSection;

// GLOBAL: LEGOLAND 0x006687a0
unsigned int ScriptLoadErrorCount;

// GLOBAL: LEGOLAND 0x006687a4
unsigned int CurrentHelpKind;

// GLOBAL: LEGOLAND 0x006687a8
unsigned int HelpRequested;

/* Explicit speech request bypasses the normal delay/debounce checks and is consumed at playback. */
// GLOBAL: LEGOLAND 0x006687ac
unsigned int SpeechImmediateRequest;

/* Frame countdown before normal help speech processing resumes after a screen transition. */
// GLOBAL: LEGOLAND 0x006687b0
unsigned int SpeechDelayFrames;

/* Help speech is waiting for its 500 ms debounce interval; checked by volume controls. */
// GLOBAL: LEGOLAND 0x006687b4
unsigned int SpeechDebouncePending;

// GLOBAL: LEGOLAND 0x006687bc
void *OkKeyHandler;

// GLOBAL: LEGOLAND 0x006687c0
void *CancelKeyHandler;

// GLOBAL: LEGOLAND 0x006687c8
struct IconNode *IconListHead;

/* Second icon list (help icons): RenderHelpIcons draws it, GetIconAtPos searches it after IconListHead. Nothing in the game appends to it. */
// GLOBAL: LEGOLAND 0x006687cc
struct IconNode *HelpIconListHead;

// GLOBAL: LEGOLAND 0x006687d0
LEGO_EXPORT void *FocussedIconPtr;

// GLOBAL: LEGOLAND 0x00668828
struct Sprite *GBarFrameSprite;

// GLOBAL: LEGOLAND 0x0066882c
struct Sprite *IfSideBUpSprite;

// GLOBAL: LEGOLAND 0x00668830
struct Sprite *IfSideBDownSprite;

// GLOBAL: LEGOLAND 0x00668834
struct Sprite *IfSidebar1Sprite;

// GLOBAL: LEGOLAND 0x00668840
short IconBBoxExtraHeight;

// GLOBAL: LEGOLAND 0x0066884c
short IconBBoxExtraWidth;

// GLOBAL: LEGOLAND 0x00668858
struct IconNode DefaultIconNode;

// GLOBAL: LEGOLAND 0x006688a8
void *NewIconGroupCallbacks;

// GLOBAL: LEGOLAND 0x006688ac
void *NewGroupRenderFunc;

// GLOBAL: LEGOLAND 0x006688b0
void *NewGroupEventHandler;

// GLOBAL: LEGOLAND 0x006688b4
unsigned int LastScrollIconTick;

/* Bits 1/2 indicate new object icons clipped above/below the build panel, for scroll-arrow hints. */
// GLOBAL: LEGOLAND 0x006688b8
unsigned int NewObjectScrollHintFlags;

// GLOBAL: LEGOLAND 0x006688bc
int PowerDemandBarPos;

// GLOBAL: LEGOLAND 0x006688c0
int EnergyBarSupplyWidth;

/* Money bar width on screen; RenderMoneyBar moves it 6 pixels per frame towards the real fill. */
// GLOBAL: LEGOLAND 0x006688c4
int MoneyBarShownWidth;

// GLOBAL: LEGOLAND 0x006688c8
unsigned int RenderIconsLastTicks;

/* Previous tick for the build-panel animation in the icon renderer that excludes one ID. */
// GLOBAL: LEGOLAND 0x006688cc
unsigned int RenderIconsExceptIdLastTicks;

// GLOBAL: LEGOLAND 0x006688d0
unsigned int GBarSpritesLoaded;

// GLOBAL: LEGOLAND 0x006688d4
void *IndicatorList;

// GLOBAL: LEGOLAND 0x006688d8
void *ActiveIndicators;

// GLOBAL: LEGOLAND 0x006688e0
struct Sprite *PuBgMainSprite;

// GLOBAL: LEGOLAND 0x006688e4
struct Sprite *PuBgCentreTopSprite;

// GLOBAL: LEGOLAND 0x006688e8
struct Sprite *PuBgRightTopSprite;

// GLOBAL: LEGOLAND 0x006688ec
struct Sprite *PuBgLeftMidSprite;

// GLOBAL: LEGOLAND 0x006688f0
struct Sprite *PuBgCentreMidSprite;

// GLOBAL: LEGOLAND 0x006688f4
struct Sprite *PuBgRightMidSprite;

// GLOBAL: LEGOLAND 0x006688f8
struct Sprite *PuBgLeftBtmSprite;

// GLOBAL: LEGOLAND 0x006688fc
struct Sprite *PuBgCentreBtmSprite;

// GLOBAL: LEGOLAND 0x00668900
struct Sprite *PuBgRightBtmSprite;

// GLOBAL: LEGOLAND 0x00668904
struct Sprite *CBBGLeftSprite;

// GLOBAL: LEGOLAND 0x00668908
struct Sprite *CBBGCentreSprite;

// GLOBAL: LEGOLAND 0x0066890c
struct Sprite *CBBGRightSprite;

// GLOBAL: LEGOLAND 0x00668910
struct Sprite *NewPopMockSprite;

// GLOBAL: LEGOLAND 0x00668914
struct Sprite *PuDeleteObjectOnSprite;

// GLOBAL: LEGOLAND 0x00668918
struct Sprite *PuDeleteObjectSprite;

// GLOBAL: LEGOLAND 0x0066891c
struct Sprite *PuClosePopUpOnSprite;

// GLOBAL: LEGOLAND 0x00668920
struct Sprite *PuClosePopUpSprite;

// GLOBAL: LEGOLAND 0x00668924
struct Sprite *NextIconOnSprite;

// GLOBAL: LEGOLAND 0x00668928
struct Sprite *NextIconSprite;

// GLOBAL: LEGOLAND 0x0066892c
struct Sprite *PrevIconOnSprite;

// GLOBAL: LEGOLAND 0x00668930
struct Sprite *PrevIconSprite;

// GLOBAL: LEGOLAND 0x00668934
struct Sprite *PUOKOnSprite;

// GLOBAL: LEGOLAND 0x00668938
struct Sprite *PUOKSprite;

// GLOBAL: LEGOLAND 0x0066893c
struct Sprite *CBCloseSprite;

// GLOBAL: LEGOLAND 0x00668940
struct Sprite *CBCloseOnSprite;

// GLOBAL: LEGOLAND 0x00668944
struct Sprite *PuAddGardenerOnSprite;

// GLOBAL: LEGOLAND 0x00668948
struct Sprite *PuAddGardenerSprite;

// GLOBAL: LEGOLAND 0x0066894c
struct Sprite *PuAddMechanicsOnSprite;

// GLOBAL: LEGOLAND 0x00668950
struct Sprite *PuAddMechanicsSprite;

// GLOBAL: LEGOLAND 0x00668954
unsigned int WorkerOnMouseActive;

// GLOBAL: LEGOLAND 0x00668958
struct Sprite *PopUpSpritesLoaded;

/* Set for the 0x104 pop-up (object under construction): the pop-up draws its build progress bar. */
// GLOBAL: LEGOLAND 0x0066895c
int PopUpShowsBuildProgress;

/* Priority of the advice speech playing now; TryPlayAdviceSpeech only interrupts it with a higher one. */
// GLOBAL: LEGOLAND 0x00668960
unsigned int CurrentAdvicePriority;

// GLOBAL: LEGOLAND 0x00668964
int PopUpMessageCentrePieces;

// GLOBAL: LEGOLAND 0x00668968
char PopUpMessageText[0x400];

// GLOBAL: LEGOLAND 0x00668d68
unsigned int PopUpMessageShown;

// GLOBAL: LEGOLAND 0x00668d78
DIMOUSESTATE MouseState;

// GLOBAL: LEGOLAND 0x00668d88
void *dinput;

// GLOBAL: LEGOLAND 0x00668d8c
void *dinput_keyboard;

// GLOBAL: LEGOLAND 0x00668d90
void *dintput_mouse;

// GLOBAL: LEGOLAND 0x00668d94
char CheatKeyBuffer[0x14];

// GLOBAL: LEGOLAND 0x00668da8
unsigned char PrevKeyState[0x3b];

// GLOBAL: LEGOLAND 0x00668de4
unsigned char CheatKeyState[0x3b];

// GLOBAL: LEGOLAND 0x00668e20
unsigned int SavedThemeIconEnabled[4];

/* Build menu tab: 0 = Build, nonzero = Research (ScriptTabNames; SELECTTAB events wait for it). Saved with the game. */
// GLOBAL: LEGOLAND 0x00668e34
unsigned int ResearchTabSelected;

/* Front end shows a sub-screen (interval report, title movie, end of game): only icon group 7-14 is drawn and clicked. */
// GLOBAL: LEGOLAND 0x00668e38
unsigned int FrontEndSubScreenActive;

// GLOBAL: LEGOLAND 0x00668e3c
void *WesternThemeIcon;

/* Linked list of parent and child object classes used to construct build-panel icons. */
// GLOBAL: LEGOLAND 0x00668e40
struct InterfaceListNode *BuildObjectList;

/* Per-theme build/research panel vertical scroll offsets. */
// GLOBAL: LEGOLAND 0x00668e44
int BuildPanelScrollOffsets[8];

/* Scroll-offset index for the active panel: theme index for build, theme plus four for research. */
// GLOBAL: LEGOLAND 0x00668e64
unsigned char BuildPanelScrollIndex;

// GLOBAL: LEGOLAND 0x00668e68
struct Sprite *InterfaceBgSprite;

// GLOBAL: LEGOLAND 0x00668e6c
struct Sprite *NoEnergySprite;

// GLOBAL: LEGOLAND 0x00668e70
struct Sprite *BarPointerSprite;

// GLOBAL: LEGOLAND 0x00668e74
struct Sprite *AttractHighlightOnSprite;

// GLOBAL: LEGOLAND 0x00668e78
struct Sprite *AttractHighlightOffSprite;

// GLOBAL: LEGOLAND 0x00668e7c
struct Sprite *AttractNewOffSprite;

// GLOBAL: LEGOLAND 0x00668e80
struct Sprite *AttractNewOnSprite;

// GLOBAL: LEGOLAND 0x00668e84
struct Sprite *SideScrollDownLitSprite;

// GLOBAL: LEGOLAND 0x00668e88
struct Sprite *SideScrollUpLitSprite;

// GLOBAL: LEGOLAND 0x00668e8c
struct Sprite *LinkMiddleSprite;

// GLOBAL: LEGOLAND 0x00668e90
struct Sprite *LinkBottomSprite;

// GLOBAL: LEGOLAND 0x00668e94
struct Sprite *BriefIcon2Sprite;

// GLOBAL: LEGOLAND 0x00668e98
struct Sprite *BriefIconSprite;

// GLOBAL: LEGOLAND 0x00668e9c
void *BriefingIcon;

// GLOBAL: LEGOLAND 0x00668ea0
struct Sprite *ScriptEndSprite;

// GLOBAL: LEGOLAND 0x00668ea4
unsigned int InterfaceControlIconsLoaded;

// GLOBAL: LEGOLAND 0x00668eb0
struct IconNode *SelectedThemeIcon;

// GLOBAL: LEGOLAND 0x00668eb4
unsigned int ThemeIconsLoaded;

// GLOBAL: LEGOLAND 0x00668eb8
struct IconNode *ScriptEndIcon;

/* Set while the in-game interface icon groups are loaded. */
// GLOBAL: LEGOLAND 0x00668ebc
unsigned int GameInterfaceLoaded;

/* Whether a flashing button was lit last frame; FX_WARNING plays when one lights up again. */
// GLOBAL: LEGOLAND 0x00668ec0
unsigned int ButtonFlashWasActive;

// GLOBAL: LEGOLAND 0x00668ed8
struct InterfaceResearchNode *ResearchList;

// GLOBAL: LEGOLAND 0x00668ee0
int AviAudioNeedsAcm;

// GLOBAL: LEGOLAND 0x00668ee8
struct AcmHeader AviAcmStreamHeader;

// GLOBAL: LEGOLAND 0x00668f3c
unsigned int AviSoundBytesPerFrame;

// GLOBAL: LEGOLAND 0x00668f44
int AcmStreamOpenResult;

// GLOBAL: LEGOLAND 0x00668f48
void *AviSoundBuffer;

// GLOBAL: LEGOLAND 0x00668f4c
unsigned int AviAudioBitsPerSample;

// GLOBAL: LEGOLAND 0x00668f50
unsigned int AviSoundBufferSlotCount;

// GLOBAL: LEGOLAND 0x00668f54
int AviAcmDstOffset;

// GLOBAL: LEGOLAND 0x00668f58
unsigned int AviAudioStartSample;

// GLOBAL: LEGOLAND 0x00668f5c
void *AviAcmStream;

// GLOBAL: LEGOLAND 0x00668f60
int AviAudioSamplePos;

// GLOBAL: LEGOLAND 0x00668f64
unsigned int AviAudioSampleSize;

// GLOBAL: LEGOLAND 0x00668f70
WAVEFORMATEX AviPcmFormat;

// GLOBAL: LEGOLAND 0x00668f84
void *AviAudioStream;

// GLOBAL: LEGOLAND 0x00668f88
int AviAudioEndSample;

// GLOBAL: LEGOLAND 0x00668f8c
void *AviAcmDstBase;

// GLOBAL: LEGOLAND 0x00668f90
unsigned int AviSamplesPerFrame;

// GLOBAL: LEGOLAND 0x00668f9c
int AviAudioReady;

// GLOBAL: LEGOLAND 0x00668fa0
unsigned int AviSoundBufferSlot;

// GLOBAL: LEGOLAND 0x00668f98
unsigned int AviOpenCount;

// GLOBAL: LEGOLAND 0x00668fa4
unsigned int AviSoundBufferFrames;

// GLOBAL: LEGOLAND 0x00668fac
unsigned int PerfCounterState;

/* Set when Cancel interrupts a movie; PlayMovie then skips further movies until a call with param_3 resets it. */
// GLOBAL: LEGOLAND 0x00668fb0
unsigned int MovieSkipRequested;

// GLOBAL: LEGOLAND 0x00668fb4
unsigned int AviAcmDstBytes;

// GLOBAL: LEGOLAND 0x00668fb8
struct MoviePool *ActiveMoviePool;

// GLOBAL: LEGOLAND 0x00668fc0
void *OpenPathNodeList;

// GLOBAL: LEGOLAND 0x00668fc4
struct GameMainNode *ClosedPathNodeList;

/* Number of resource-script lines read by the current parse. */
// GLOBAL: LEGOLAND 0x00668fcc
int ScriptResourceLineNumber;

// GLOBAL: LEGOLAND 0x00668fd0
char CurrentScriptFileName[128];

// GLOBAL: LEGOLAND 0x00669050
unsigned char CurrentObjectiveEventFlags;

/* Current script section type bitmask used to gate allowed commands. */
// GLOBAL: LEGOLAND 0x00669054
unsigned int ScriptSectionTypeMask;

/* Current script section name supplied with its type mask. */
// GLOBAL: LEGOLAND 0x00669058
char ScriptSectionName[0x40];

// GLOBAL: LEGOLAND 0x00669098
unsigned int NextScriptSectionId;

// GLOBAL: LEGOLAND 0x006691a0
unsigned int LLIDB_Capacity;

// GLOBAL: LEGOLAND 0x006691a4
unsigned int LLIDB_ElementCount;

// GLOBAL: LEGOLAND 0x006691a8
struct Element **LLIDB_Pages;

// GLOBAL: LEGOLAND 0x006691ac
struct LLSNode *LLSPlayList;

// GLOBAL: LEGOLAND 0x006691b0
int SaveFileHandle;

// GLOBAL: LEGOLAND 0x006691b4
int SavedElementCount;

// GLOBAL: LEGOLAND 0x006691bc
int MeasuredBlockStarts[16];

// GLOBAL: LEGOLAND 0x006691fc
int MeasuredBlockDepth;

// GLOBAL: LEGOLAND 0x00669200
struct Element **SavedElementTable;

// GLOBAL: LEGOLAND 0x00669204
unsigned int GameTimerMark;

// GLOBAL: LEGOLAND 0x00669208
void *g_hInstance;

// GLOBAL: LEGOLAND 0x0066920c
int g_nCmdShow;

// GLOBAL: LEGOLAND 0x00669210
HWND WndEnvHwnd;

// GLOBAL: LEGOLAND 0x00669238
unsigned int PauseGameTimerResult;

// GLOBAL: LEGOLAND 0x00669240
LEGO_EXPORT struct Ride *ObjectClassList;

// GLOBAL: LEGOLAND 0x00669244
struct LibraryNode *ObjectLibraryHead;

// GLOBAL: LEGOLAND 0x00669248
void *ClassRideList;

// GLOBAL: LEGOLAND 0x0066924c
struct InfoNode *ClassRideBestPtr;

// GLOBAL: LEGOLAND 0x00669250
int DirSearchWaveCount;

// GLOBAL: LEGOLAND 0x00669254
unsigned int AdjacentNodeCount;

// GLOBAL: LEGOLAND 0x00669258
unsigned int PathVisitedBits[1152];

// GLOBAL: LEGOLAND 0x0066a45c
void *AdjacentNodes[1020];

// GLOBAL: LEGOLAND 0x0066b44c
void *BestNodeList;

// GLOBAL: LEGOLAND 0x0066b450
struct DirNode *DirSearchNodeList;

// GLOBAL: LEGOLAND 0x0066b454
struct DirNode *DirSearchGoalNode;

// GLOBAL: LEGOLAND 0x0066b458
struct DirNode *DirPathNodeList;

// GLOBAL: LEGOLAND 0x0066b460
struct Point Entrance1Point; /* park entrance tile */

// GLOBAL: LEGOLAND 0x0066b468
int LastPathUpdateTime;

// GLOBAL: LEGOLAND 0x0066b46c
unsigned int PathUpdateNeeded;

// GLOBAL: LEGOLAND 0x0066b470
char VisitorNameBuffer[0x104];

// GLOBAL: LEGOLAND 0x0066b574
LEGO_EXPORT struct Bloke *FirstBloke;

// GLOBAL: LEGOLAND 0x0066b57c
struct Bloke *BlokePool;

// GLOBAL: LEGOLAND 0x0066b580
int DirPickCounts[9];

// GLOBAL: LEGOLAND 0x0066b5a4
struct SortNode *PrintListHead;

// GLOBAL: LEGOLAND 0x0066b5a8
unsigned int PrintListBytesUsed;

// GLOBAL: LEGOLAND 0x0066b5ac
unsigned int UnusedPrintListCounter;

// GLOBAL: LEGOLAND 0x0066b5b0
DDSURFACEDESC ZBufferSavedSurfaceDesc;

// GLOBAL: LEGOLAND 0x0066b61c
void *CurrentColorRamp;

// GLOBAL: LEGOLAND 0x0066b620
RECT ZBufferSavedSoftPrintClip;

// GLOBAL: LEGOLAND 0x0066b630
void *CurrentTexture;

// GLOBAL: LEGOLAND 0x0066be40
unsigned int ZBufferWidth;

// GLOBAL: LEGOLAND 0x0066be44
unsigned int ZBufferHeight;

// GLOBAL: LEGOLAND 0x0066be48
unsigned int Render3DTargetHeight;

// GLOBAL: LEGOLAND 0x0066be4c
unsigned int ZBufferStride;

// GLOBAL: LEGOLAND 0x0066be50
struct Image *ZBufferSourceImage;

// GLOBAL: LEGOLAND 0x0066be54
unsigned short ZBufferRenderPixels[0x4b002];

// GLOBAL: LEGOLAND 0x00701e58
unsigned int Render3DTargetPitch;

// GLOBAL: LEGOLAND 0x00701e5c
unsigned char *ZBuffer;

// GLOBAL: LEGOLAND 0x00701e60
unsigned int Render3DTargetWidth;

// GLOBAL: LEGOLAND 0x00701e64
struct Sprite *ZBufferImageSprite;

// GLOBAL: LEGOLAND 0x00701e68
unsigned short ZBufferImagePixels[0x4b000];

/* 3D render target: address of the first pixel (SetRender3DTarget), with pitch, width and height after it. */
// GLOBAL: LEGOLAND 0x00797e68
unsigned char *Render3DTargetBits;

// GLOBAL: LEGOLAND 0x00797e6c
void *CursorCacheList;

// GLOBAL: LEGOLAND 0x00797e70
float ReciprocalTable[100];

// GLOBAL: LEGOLAND 0x00798000
int ScaledReciprocalTable[100];

// GLOBAL: LEGOLAND 0x00798190
void *TextureNodes[256];

/* Set once InitZBufferImageSprite has run; also the end marker of the 256 texture node slots before it. */
// GLOBAL: LEGOLAND 0x00798590
void *ZBufferImageSpriteReady;

// GLOBAL: LEGOLAND 0x00798598
DDSURFACEDESC SavedSurfaceDesc;

// GLOBAL: LEGOLAND 0x00798608
RECT SavedSoftPrintClip;

// GLOBAL: LEGOLAND 0x0079861c
LPDIRECTDRAWSURFACE ScaledSpriteScratch;

// GLOBAL: LEGOLAND 0x00798620
int ScaledSpriteScratchMade;

// GLOBAL: LEGOLAND 0x00798624
void *MasterDirList;

// GLOBAL: LEGOLAND 0x00798628
void *MasterVolList;

// GLOBAL: LEGOLAND 0x0079862c
void *AllResVolEntries;

// GLOBAL: LEGOLAND 0x00798630
unsigned int SavedClipLeft;

// GLOBAL: LEGOLAND 0x00798634
unsigned int SavedClipTop;

// GLOBAL: LEGOLAND 0x00798638
unsigned int SavedClipRight;

// GLOBAL: LEGOLAND 0x0079863c
unsigned int SavedClipBottom;

/* Set while RestoreFreePlaySelection replays clicks; FreePlayObjectIconHandler then skips the click sound. */
// GLOBAL: LEGOLAND 0x00798648
unsigned int FreePlayRestoringSelection;

/* Free play Start icon: disabled (flag 0x400) until at least one object is selected. */
// GLOBAL: LEGOLAND 0x0079864c
struct IconNode *FreePlayStartIcon;

/* Number of objects selected on the free play screen; enables/disables the Start icon at 1/0. */
// GLOBAL: LEGOLAND 0x00798650
unsigned int FreePlaySelectedCount;

// GLOBAL: LEGOLAND 0x00798660
unsigned int ProgressLevelPicked;

// GLOBAL: LEGOLAND 0x00798664
unsigned int ProgressShowsTutorials;

// GLOBAL: LEGOLAND 0x0079866c
unsigned long ProgressLastClickTicks;

// GLOBAL: LEGOLAND 0x00798668
unsigned int ProgressScreenToggled;

// GLOBAL: LEGOLAND 0x00798674
struct Sprite *PuOkOnSprite;

// GLOBAL: LEGOLAND 0x00798678
struct Sprite *PuOkSprite;

// GLOBAL: LEGOLAND 0x0079867c
struct Sprite *PopUpCloseSprite;

// GLOBAL: LEGOLAND 0x00798680
struct Sprite *PuCloseOnSprite;

// GLOBAL: LEGOLAND 0x00798684
struct Sprite *ClosePopUpSprite;

// GLOBAL: LEGOLAND 0x00798688
struct Sprite *ClosePopUpOnSprite;

// GLOBAL: LEGOLAND 0x0079868c
struct Sprite *RegDeleteOnSprite;

// GLOBAL: LEGOLAND 0x00798690
struct Sprite *RegDeleteSprite;

// GLOBAL: LEGOLAND 0x00798694
struct Sprite *RegProfileOff1Sprite;

// GLOBAL: LEGOLAND 0x00798698
struct Sprite *RegProfileOff2Sprite;

// GLOBAL: LEGOLAND 0x0079869c
struct Sprite *RegProfileOff3Sprite;

// GLOBAL: LEGOLAND 0x007986a0
struct Sprite *RegProfileOff4Sprite;

// GLOBAL: LEGOLAND 0x007986a4
struct Sprite *RegProfileOff5Sprite;

// GLOBAL: LEGOLAND 0x007986a8
struct Sprite *RegProfileOff6Sprite;

// GLOBAL: LEGOLAND 0x007986ac
struct Sprite *RegProfileOff7Sprite;

// GLOBAL: LEGOLAND 0x007986b0
struct Sprite *RegProfileOff8Sprite;

// GLOBAL: LEGOLAND 0x007986b4
struct Sprite *RegProfileOnSprite;

// GLOBAL: LEGOLAND 0x007986b8
struct Sprite *RegDeletePopUpSprite;

// GLOBAL: LEGOLAND 0x007986bc
struct Sprite *RegDiffPopUpSprite;

// GLOBAL: LEGOLAND 0x007986c0
struct Sprite *RegEasyOnSprite;

// GLOBAL: LEGOLAND 0x007986c4
struct Sprite *RegEasyOffSprite;

// GLOBAL: LEGOLAND 0x007986c8
struct Sprite *RegMidOnSprite;

// GLOBAL: LEGOLAND 0x007986cc
struct Sprite *RegMidOffSprite;

// GLOBAL: LEGOLAND 0x007986d0
struct Sprite *RegHardOnSprite;

// GLOBAL: LEGOLAND 0x007986d4
struct Sprite *RegHardOffSprite;

// GLOBAL: LEGOLAND 0x007986d8
struct IconNode *PopUpOkIcon;

// GLOBAL: LEGOLAND 0x007986dc
struct IconNode *PopUpCloseIcon;

// GLOBAL: LEGOLAND 0x007986e0
struct IconNode *AcceptIcon;

// GLOBAL: LEGOLAND 0x007986e4
unsigned int DeletePopUpShown;

// GLOBAL: LEGOLAND 0x007986e8
unsigned int NewProfilePopUpShown;

// GLOBAL: LEGOLAND 0x007986f0
unsigned int TempProfileNameSet;

// GLOBAL: LEGOLAND 0x007986f4
void *ProfileSavedCancelKeyHandler;

// GLOBAL: LEGOLAND 0x007986f8
void *ProfileSavedOkKeyHandler;

// GLOBAL: LEGOLAND 0x00798700
unsigned int NewSaveGamePopUpShown;

// GLOBAL: LEGOLAND 0x00798704
struct Sprite *RegSaveSlotOnSprite;

// GLOBAL: LEGOLAND 0x00798708
struct Sprite *RegSaveOff1Sprite;

// GLOBAL: LEGOLAND 0x0079870c
struct Sprite *RegSaveOff2Sprite;

// GLOBAL: LEGOLAND 0x00798710
struct Sprite *RegSaveOff3Sprite;

// GLOBAL: LEGOLAND 0x00798714
struct Sprite *RegSaveOff4Sprite;

// GLOBAL: LEGOLAND 0x00798718
struct Sprite *RegSaveOff5Sprite;

// GLOBAL: LEGOLAND 0x0079871c
struct Sprite *RegSaveOff6Sprite;

// GLOBAL: LEGOLAND 0x00798720
struct Sprite *RegSaveOff7Sprite;

// GLOBAL: LEGOLAND 0x00798724
struct Sprite *RegSaveOff8Sprite;

// GLOBAL: LEGOLAND 0x00798728
struct Sprite *SaveTypeNormalSprite;

// GLOBAL: LEGOLAND 0x0079872c
struct Sprite *SaveTypeFreeSprite;

// GLOBAL: LEGOLAND 0x00798730
struct Sprite *RegCornerMaskSprite;

// GLOBAL: LEGOLAND 0x00798734
void *SavedGameList;

// GLOBAL: LEGOLAND 0x00798738
int SaveNameTextWidth;

/* Saved cancel-key callback restored when leaving the options-related screen. */
// GLOBAL: LEGOLAND 0x0079873c
void *OptionsSavedCancelKeyHandler;

/* Saved OK-key callback restored when leaving the options-related screen. */
// GLOBAL: LEGOLAND 0x00798740
void *OptionsSavedOkKeyHandler;

// GLOBAL: LEGOLAND 0x00798748
struct IconNode *SpeechVolumeMarkerIcon;

// GLOBAL: LEGOLAND 0x0079874c
struct IconNode *FxVolumeMarkerIcon;

// GLOBAL: LEGOLAND 0x00798750
struct IconNode *MusicVolumeMarkerIcon;

/* The exit dialog was opened after more than 60 s of play: big pop-up with the extra text 0x884. */
// GLOBAL: LEGOLAND 0x00798754
unsigned int ExitDialogAfterLongPlay;

// GLOBAL: LEGOLAND 0x00798764
struct Sprite *PrintInfoSprite;

// GLOBAL: LEGOLAND 0x00798768
int CertificatePrintResultTimer;

/* Frames left before the certificate is printed (2 after the click); the "printing" note shows meanwhile. */
// GLOBAL: LEGOLAND 0x0079876c
unsigned int CertificatePrintCountdown;

// GLOBAL: LEGOLAND 0x00798770
unsigned int CertificateScreenActive;

// GLOBAL: LEGOLAND 0x00798777
char IntervalMovieNameEnd;

// GLOBAL: LEGOLAND 0x00798778
char IntervalMovieName[0x100];

/* 1 while the interval screen shows the level briefing (accept button text 0x244), 0 for interval reports. */
// GLOBAL: LEGOLAND 0x00798878
unsigned int IntervalScreenIsBriefing;

// GLOBAL: LEGOLAND 0x0079887c
unsigned int IntervalTextLastLine;

// GLOBAL: LEGOLAND 0x00798880
unsigned int IntervalHintCount;

// GLOBAL: LEGOLAND 0x00798884
unsigned int IntervalHintIndex;

// GLOBAL: LEGOLAND 0x00798888
unsigned int IntervalHintBubbleEndTime;

// GLOBAL: LEGOLAND 0x00798890
void *ProfileListHead;

// GLOBAL: LEGOLAND 0x00798894
int ProfileNameTextWidth;

// GLOBAL: LEGOLAND 0x00798898
unsigned int AviLockBytes1;

// GLOBAL: LEGOLAND 0x0079889c
unsigned int AviLockBytes2;

// GLOBAL: LEGOLAND 0x007988a0
unsigned int SfxDSVolume;

// GLOBAL: LEGOLAND 0x007988a4
void *AviLockPtr1;

// GLOBAL: LEGOLAND 0x007988a8
void *AviLockPtr2;

// GLOBAL: LEGOLAND 0x007988b0
void *SoundHwnd;

// GLOBAL: LEGOLAND 0x007988bc
volatile unsigned int MusicThreadReady;

// GLOBAL: LEGOLAND 0x007988c0
unsigned int SoundAvailable;

// GLOBAL: LEGOLAND 0x007988c4
unsigned int SfxMuted;

/* When set, newly played sample instances are immediately paused. */
// GLOBAL: LEGOLAND 0x007988c8
unsigned int PauseNewSamples;

// GLOBAL: LEGOLAND 0x007988cc
void *SampleListHead;

// GLOBAL: LEGOLAND 0x0079a694
LEGO_EXPORT unsigned int DMusicInitialised;

// GLOBAL: LEGOLAND 0x0079a698
void *MusicThread;

// GLOBAL: LEGOLAND 0x0079a6a0
void *MusicCommandEvent;

// GLOBAL: LEGOLAND 0x0079a6a4
unsigned int MusicCommand;

// GLOBAL: LEGOLAND 0x0079a6a8
int NextMusicTheme;

// GLOBAL: LEGOLAND 0x0079a6ac
int CurrentMusicTheme;

// GLOBAL: LEGOLAND 0x0079a7d0
unsigned int SpeechVolume;

// GLOBAL: LEGOLAND 0x0079a7d8
int SpeechRawReadPos;

// GLOBAL: LEGOLAND 0x0079a7dc
int SpeechRawWritePos;

// GLOBAL: LEGOLAND 0x0079a7e0
unsigned int SpeechChunkIndex;

// GLOBAL: LEGOLAND 0x0079a7e4
unsigned int SpeechChunkByteCounts[20];

// GLOBAL: LEGOLAND 0x0079a834
int SpeechPcmReadPos;

// GLOBAL: LEGOLAND 0x0079a838
int SpeechPcmWritePos;

// GLOBAL: LEGOLAND 0x0079a83c
unsigned int SpeechFlags;

// GLOBAL: LEGOLAND 0x0079a840
int SpeechPageIndex;

/* Speech buffer pages (4 KB each) holding audio that has not played yet; the stream stops when it reaches 0. */
// GLOBAL: LEGOLAND 0x0079a844
int SpeechPagesQueued;

// GLOBAL: LEGOLAND 0x0079a848
void *SpeechSoundBuffer;

// GLOBAL: LEGOLAND 0x0079a84c
unsigned int SpeechState;

// GLOBAL: LEGOLAND 0x0079a850
void *strings[10];

// GLOBAL: LEGOLAND 0x0079a878
unsigned int UnusedAfterStrings[6];

// GLOBAL: LEGOLAND 0x0079a890
unsigned int GameTimerPaused;

// GLOBAL: LEGOLAND 0x0079a894
unsigned int GameTimerPausedTicks;

// GLOBAL: LEGOLAND 0x0079a898
unsigned int GameTimerBaseTicks;

// GLOBAL: LEGOLAND 0x0079a89c
unsigned int GameTimerPausedFrame;

// GLOBAL: LEGOLAND 0x0079a8a0
unsigned int GameTimerBaseFrame;

// GLOBAL: LEGOLAND 0x0079a8a8
LEGO_EXPORT struct Bloke *GardenerList;

// GLOBAL: LEGOLAND 0x0079a8ac
LEGO_EXPORT struct Bloke *MechanicList;

// GLOBAL: LEGOLAND 0x0079a8b0
struct WorkOrder *GardenerOrderHead;

// GLOBAL: LEGOLAND 0x0079a8b4
struct WorkOrder *GardenerOrderTail;

// GLOBAL: LEGOLAND 0x0079a8b8
int GardenerOrderCount;

// GLOBAL: LEGOLAND 0x0079a8bc
int GardenerCount;

// GLOBAL: LEGOLAND 0x0079a8c0
struct WorkOrder *MechanicOrderHead;

// GLOBAL: LEGOLAND 0x0079a8c4
struct WorkOrder *MechanicOrderTail;

// GLOBAL: LEGOLAND 0x0079a8c8
int MechanicOrderCount;

// GLOBAL: LEGOLAND 0x0079a8cc
int MechanicCount;

// GLOBAL: LEGOLAND 0x0079a8d0
unsigned int CastlePlacedFlag;

// GLOBAL: LEGOLAND 0x0079a8d4
struct RepairOrder *RepairOrderList;

// GLOBAL: LEGOLAND 0x0079abfc
struct Element *NormalPathTilesElement;

// GLOBAL: LEGOLAND 0x0079ac04
unsigned int SpeechDataSize;

// GLOBAL: LEGOLAND 0x0079ac08
unsigned char *SpeechAcmDstBuffer;

// GLOBAL: LEGOLAND 0x0079ac0c
void *SpeechAcmSrcBuffer;

// GLOBAL: LEGOLAND 0x0079ac20
unsigned char SpeechRawBuffer[0x10000];

// GLOBAL: LEGOLAND 0x007aac24
int SpeechConvertedUsed;

// GLOBAL: LEGOLAND 0x007aac40
struct AcmHdr SpeechAcmHeader;

// GLOBAL: LEGOLAND 0x007aaca0
unsigned char SpeechPcmBuffer[0x20000];

// GLOBAL: LEGOLAND 0x007caca0
int SpeechAcmSrcSize;

// GLOBAL: LEGOLAND 0x007caca4
int SpeechConvertedSize;

// GLOBAL: LEGOLAND 0x007caca8
unsigned int SpeechFileHandle;

// GLOBAL: LEGOLAND 0x007cacac
unsigned int SpeechBytesRemaining;

// GLOBAL: LEGOLAND 0x007cacb0
WAVEFORMATEX *SpeechSourceFormat;

// GLOBAL: LEGOLAND 0x007cacb8
void *SpeechAcmStream;

// GLOBAL: LEGOLAND 0x007cacc0
WAVEFORMATEX SpeechPcmFormat;

// GLOBAL: LEGOLAND 0x007cacb4
unsigned int SpeechDataOffset;

// GLOBAL: LEGOLAND 0x007cacd4
LEGO_EXPORT unsigned int FrameNumber;

// GLOBAL: LEGOLAND 0x007cacd8
struct DirectMusicLoader *DMusicLoader;

// GLOBAL: LEGOLAND 0x007cacdc
struct DirectMusicPerformance *DMusicPerformance;

// GLOBAL: LEGOLAND 0x007cace0
unsigned int DSoundCaps[0x18];

// GLOBAL: LEGOLAND 0x007cad40
void *DSound;

// GLOBAL: LEGOLAND 0x007cad44
struct DirectMusicComposer *DMusicComposer;

// GLOBAL: LEGOLAND 0x007cad48
unsigned int MusicThreadId;

// GLOBAL: LEGOLAND 0x007cad4c
void *DMusicSoundBuffer;

// GLOBAL: LEGOLAND 0x007cad60
struct ProfileData TempProfile;

// GLOBAL: LEGOLAND 0x007cae80
char IntervalReportSpeechPrefix[0x100];

// GLOBAL: LEGOLAND 0x007caf80
struct Sprite *RepHint1Sprite;

// GLOBAL: LEGOLAND 0x007cafa0
char *IntervalTextLines[104]; /* text lines (ReadResourceLines, up to 100) */

// GLOBAL: LEGOLAND 0x007cb140
char *IntervalHintLines[32]; /* text lines (ReadResourceLines, up to 32) */

// GLOBAL: LEGOLAND 0x007cb1c0
struct IconNode *RepHint1Icon;

// GLOBAL: LEGOLAND 0x007cb1c4
struct Sprite *RepHint2Sprite;

// GLOBAL: LEGOLAND 0x007cb1e0
char IntervalHintSpeechPrefix[0x100];

// GLOBAL: LEGOLAND 0x007cb2e0
struct IconNode *TitlePreviousPageIcon;

// GLOBAL: LEGOLAND 0x007cb2e4
struct IconNode *TitleNextPageIcon;

// GLOBAL: LEGOLAND 0x007cb2f0
struct EditState TitleSavedEditMode; /* EditMode saved by SaveScreenState */

// GLOBAL: LEGOLAND 0x007cb300
struct ScreenMode TitleSavedScreenMode; /* FrontEndScreenMode saved */

// GLOBAL: LEGOLAND 0x007cb30c
unsigned int TitleSavedScreenFlag; /* FrontEndSubScreenActive saved */

/* The saved-game screen was opened from the exit check box Save button, so Accept saves. */
// GLOBAL: LEGOLAND 0x007cb310
unsigned int SavedGameScreenFromExitDialog;

// GLOBAL: LEGOLAND 0x007cb314
char SavedSpeechVolume;

// GLOBAL: LEGOLAND 0x007cb315
char SavedFxVolume;

// GLOBAL: LEGOLAND 0x007cb318
unsigned int ExitDialogAltText;

// GLOBAL: LEGOLAND 0x007cb31c
char SavedMusicVolume;

// GLOBAL: LEGOLAND 0x007cb320
unsigned int ExitCheckBoxShown;

// GLOBAL: LEGOLAND 0x007cb324
unsigned int SavedGameScreenFromTitle;

// GLOBAL: LEGOLAND 0x007cb328
unsigned int LoadMode;

// GLOBAL: LEGOLAND 0x007cb340
char ProfileListNewLabel[0x20];

// GLOBAL: LEGOLAND 0x007cb360
struct IconNode *DeleteIcon;

// GLOBAL: LEGOLAND 0x007cb380
struct IconNode *TutorialIcons[5];

/* Highest unlocked profile level slot, zero-based; used to choose the progress-screen level. */
// GLOBAL: LEGOLAND 0x007cb394
unsigned int HighestUnlockedLevelIndex;

// GLOBAL: LEGOLAND 0x007cb398
struct Sprite *FreePlayTickSprite;

// GLOBAL: LEGOLAND 0x007cb39c
struct PanelNode *FreePlayPanelList4;

// GLOBAL: LEGOLAND 0x007cb3a0
unsigned int FreePlayCostTotal;

// GLOBAL: LEGOLAND 0x007cb3a4
struct PanelNode *FreePlayPanelList2;

// GLOBAL: LEGOLAND 0x007cb3a8
struct Sprite *FreePlayDown4Sprite;

// GLOBAL: LEGOLAND 0x007cb3ac
struct Sprite *FreePlayDown2Sprite;

// GLOBAL: LEGOLAND 0x007cb3b0
struct Sprite *FreePlayDown3Sprite;

// GLOBAL: LEGOLAND 0x007cb3b4
struct Sprite *FreePlayDown1Sprite;

// GLOBAL: LEGOLAND 0x007cb3b8
struct PanelNode *FreePlayPanelList3;

// GLOBAL: LEGOLAND 0x007cb3bc
struct Element *LoadedObjDescElement;

// GLOBAL: LEGOLAND 0x007cb3c0
struct Sprite *FreePlayUp2Sprite;

// GLOBAL: LEGOLAND 0x007cb3c4
struct Sprite *FreePlayUp1Sprite;

// GLOBAL: LEGOLAND 0x007cb3c8
struct Sprite *FreePlayUp4Sprite;

// GLOBAL: LEGOLAND 0x007cb3cc
struct Sprite *FreePlayUp3Sprite;

// GLOBAL: LEGOLAND 0x007cb3d0
struct PanelNode *FreePlayPanelList1;

// GLOBAL: LEGOLAND 0x007cb3d4
struct Sprite *FreePlayCoverSprite;

// GLOBAL: LEGOLAND 0x007cb3e0
struct ObjTableEntry ObjInstanceTable[128];

/* Instance counts of ObjInstanceTable, viewed 2 bytes into each entry. */
// GLOBAL: LEGOLAND 0x007cb3e2
struct ObjTableEntry ObjInstanceTableCounts[128];

/* Green channel bits of the display (6 for RGB565, 5 for RGB555), set by InitRender3D; obj_instance.c also uses its address as the end of ObjInstanceTable. */
// GLOBAL: LEGOLAND 0x007cb5e0
struct ObjTableEntry ColorGreenBits;

// GLOBAL: LEGOLAND 0x007cb600
unsigned char PrintListPool[0x32000];

// GLOBAL: LEGOLAND 0x007fd600
struct SortNode *SortCursor;

/* Library node filled while an object DLL is loaded (NewObjectPtr points here). */
// GLOBAL: LEGOLAND 0x007fd610
struct LibraryNode NewObjectLibraryNode;

// GLOBAL: LEGOLAND 0x007fd620
LEGO_EXPORT void *NewObjectPtr;

// GLOBAL: LEGOLAND 0x007fd624
void *PathControlObject;

// GLOBAL: LEGOLAND 0x007fd630
unsigned int MidiTimerId;

// GLOBAL: LEGOLAND 0x007fd634
void *CurrentMidiFile;

// GLOBAL: LEGOLAND 0x007fd638
HMIDIOUT MidiOutHandle;

// GLOBAL: LEGOLAND 0x007fd640
struct ResVolume *ResourceVolumes[4];

// GLOBAL: LEGOLAND 0x007fd660
struct Element *SaveGameElements[256];

// GLOBAL: LEGOLAND 0x007fda60
struct BlokeSave BlokeSaveBuffer;

// GLOBAL: LEGOLAND 0x007fdb84
int SaveGameElementCount;

// GLOBAL: LEGOLAND 0x007fdb88
unsigned int SelectElementMask;

// GLOBAL: LEGOLAND 0x007fdba0
char ObjDescSpriteName[0x100];

// GLOBAL: LEGOLAND 0x007fdca0
int SelectedElementIndex;

/* Script string of the last PROMPT command; AddScriptEvent stores it in each new event (field_40). */
// GLOBAL: LEGOLAND 0x007fdca4
unsigned int ScriptPromptStringIndex;

// GLOBAL: LEGOLAND 0x007fdca8
float PerfCounterScale;

// GLOBAL: LEGOLAND 0x007fdcc0
struct Sprite *LegolandThemeOnSprite;

// GLOBAL: LEGOLAND 0x007fdcc4
struct Sprite *WesternThemeOnSprite;

// GLOBAL: LEGOLAND 0x007fdcc8
struct Sprite *CastleThemeOnSprite;

// GLOBAL: LEGOLAND 0x007fdccc
struct Sprite *AdventurersThemeOnSprite;

// GLOBAL: LEGOLAND 0x007fdcd0
struct Sprite *IfPathIconPressedSprite;

// GLOBAL: LEGOLAND 0x007fdcd4
struct Sprite *IfQueryIconPressedSprite;

// GLOBAL: LEGOLAND 0x007fdcd8
struct Sprite *IfEraserIconPressedSprite;

// GLOBAL: LEGOLAND 0x007fdcdc
struct Sprite *IfMapIconPressedSprite;

// GLOBAL: LEGOLAND 0x007fdce0
struct Sprite *IfOptionsIconPressedSprite;

// GLOBAL: LEGOLAND 0x007fdd00
unsigned int ButtonFlashStates[9];

// GLOBAL: LEGOLAND 0x007fdd40
struct Sprite *LegolandThemeOffSprite;

// GLOBAL: LEGOLAND 0x007fdd44
struct Sprite *WesternThemeOffSprite;

// GLOBAL: LEGOLAND 0x007fdd48
struct Sprite *CastleThemeOffSprite;

// GLOBAL: LEGOLAND 0x007fdd4c
struct Sprite *AdventurersThemeOffSprite;

// GLOBAL: LEGOLAND 0x007fdd50
struct Sprite *IfPathIconSprite;

// GLOBAL: LEGOLAND 0x007fdd54
struct Sprite *IfQueryiconSprite;

// GLOBAL: LEGOLAND 0x007fdd58
struct Sprite *IfEraserIconSprite;

// GLOBAL: LEGOLAND 0x007fdd5c
struct Sprite *IfMapiconSprite;

// GLOBAL: LEGOLAND 0x007fdd60
struct Sprite *IfOptionsIconSprite;

/* Theme icons in ScriptThemeNames order: Legoland, Western, Castle, Adventurers. */
// GLOBAL: LEGOLAND 0x007fdd70
struct IconNode *ThemeIcons[4];

/* Build panel slide state: 0 opening, 1 closing, 2 closed, 3 open. */
// GLOBAL: LEGOLAND 0x007fdd80
unsigned char BuildSidePanelState;

/* Newly constructed build panel must begin offscreen and slide into view. */
// GLOBAL: LEGOLAND 0x007fdd84
unsigned int BuildSidePanelSlideInPending;

/* Map viewport currently reserves the 121-pixel strip occupied by the open build panel. */
// GLOBAL: LEGOLAND 0x007fdd88
unsigned int BuildSidePanelViewportReserved;

/* Build panel horizontal origin/position byte (3 open; 0x86 encodes the closed negative offset). */
// GLOBAL: LEGOLAND 0x007fdd8c
unsigned char BuildSidePanelOriginX;

// GLOBAL: LEGOLAND 0x007fdda0
unsigned char KeyboardState[256];

// GLOBAL: LEGOLAND 0x007fdea4
struct IconNode *AddMechanicsIcon;

// GLOBAL: LEGOLAND 0x007fdea8
struct IconNode *PopUpInfoOkIcon;

// GLOBAL: LEGOLAND 0x007fdeac
struct Sprite *IFullSprite;

// GLOBAL: LEGOLAND 0x007fdeb0
struct Sprite *ObjectNoRepair1Sprite;

// GLOBAL: LEGOLAND 0x007fdec0
struct HoverInfo PopUpInfoHover;

// GLOBAL: LEGOLAND 0x007fdecc
int PopUpInfoX;

// GLOBAL: LEGOLAND 0x007fded0
int PopUpInfoY;

// GLOBAL: LEGOLAND 0x007fded4
struct NewObjTable NewObjects;

// GLOBAL: LEGOLAND 0x007fdf7c
struct Ride *PopUpInfoRide;

// GLOBAL: LEGOLAND 0x007fdf80
struct InfoObjData *PopUpInfoData;

// GLOBAL: LEGOLAND 0x007fdf84
unsigned char *PopUpInfoTile;

// GLOBAL: LEGOLAND 0x007fdf88
unsigned short PopUpInfoHoverValue;

// GLOBAL: LEGOLAND 0x007fdf8c
void *PopUpInfoBloke;

// GLOBAL: LEGOLAND 0x007fdf90
unsigned int PopUpVisitorField1c;

// GLOBAL: LEGOLAND 0x007fdf94
unsigned int PopUpVisitorField20;

// GLOBAL: LEGOLAND 0x007fdf98
unsigned int PopUpFollowingVisitor;

/* Hover type of the pop-up subject (0x103 object, 0x306 visitor, ...); 10 and 0x14 mark a potting shed / mechanics hut click that bought a worker. */
// GLOBAL: LEGOLAND 0x007fdf9c
int PopUpInfoSubjectType;

/* 0 = no pop-up, 1 = object/visitor info pop-up, 2 = new objects pop-up. */
// GLOBAL: LEGOLAND 0x007fdfa0
unsigned int PopUpInfoState;

// GLOBAL: LEGOLAND 0x007fdfa4
unsigned int PopUpDeleteConfirm;

// GLOBAL: LEGOLAND 0x007fdfa8
unsigned int PopUpInfoResizePending;

// GLOBAL: LEGOLAND 0x007fdfac
unsigned char PopupInfoLineCount;

// GLOBAL: LEGOLAND 0x007fdfb0
struct Element *PottingShedHandle;

// GLOBAL: LEGOLAND 0x007fdfb4
struct Element *MechanicsHutHandle;

// GLOBAL: LEGOLAND 0x007fdfb8
struct Element *PathControlHandle;

// GLOBAL: LEGOLAND 0x007fdfbc
struct Element *Entrance1Handle;

// GLOBAL: LEGOLAND 0x007fdfc0
struct IconNode *ClosePopUpIcon;

// GLOBAL: LEGOLAND 0x007fdfc4
struct IconNode *NextPopUpIcon;

// GLOBAL: LEGOLAND 0x007fdfc8
struct Sprite *ISadSprite;

// GLOBAL: LEGOLAND 0x007fdfcc
struct IconNode *PopUpDeleteIcon;

// GLOBAL: LEGOLAND 0x007fdfd0
struct Sprite *IHungrySprite;

// GLOBAL: LEGOLAND 0x007fdfd8
struct IconNode *PuCornerMaskIcon;

// GLOBAL: LEGOLAND 0x007fdfdc
struct IconNode *DeleteObjectIcon;

// GLOBAL: LEGOLAND 0x007fdfe0
struct IconNode *AddGardenerIcon;

// GLOBAL: LEGOLAND 0x007fdfe4
struct Sprite *INormSprite;

// GLOBAL: LEGOLAND 0x007fdfe8
struct IconNode *PrevPopUpIcon;

// GLOBAL: LEGOLAND 0x007fdff0
struct Bloke *WorkerOnMouse;

// GLOBAL: LEGOLAND 0x007fdff4
int WorkerOldX;

// GLOBAL: LEGOLAND 0x007fdff8
int WorkerOldY;

// GLOBAL: LEGOLAND 0x007fdffc
unsigned int WorkerOnMouseType;

// GLOBAL: LEGOLAND 0x007fe000
struct IconNode *CBCloseIcon;

// GLOBAL: LEGOLAND 0x007fe004
struct Sprite *ObjectRepairOkSprite;

// GLOBAL: LEGOLAND 0x007fe008
struct Sprite *IPeckishSprite;

// GLOBAL: LEGOLAND 0x007fe010
int PopUpMessageX;

// GLOBAL: LEGOLAND 0x007fe014
int PopUpMessageY;

// GLOBAL: LEGOLAND 0x007fe018
struct Sprite *IHappySprite;

// GLOBAL: LEGOLAND 0x007fe020
RECT ScreenRect;

// GLOBAL: LEGOLAND 0x007fe040
unsigned int AdvisorHelpFlags;

/* Active advisor message may clear early when a nonzero-type objective event is pending. */
// GLOBAL: LEGOLAND 0x007fe044
unsigned int AdvisorClearOnObjectiveEvent;

// GLOBAL: LEGOLAND 0x007fe048
char *AdvisorHelpText;

// GLOBAL: LEGOLAND 0x007fe04c
unsigned int AdvisorHelpStartTime;

// GLOBAL: LEGOLAND 0x007fe050
unsigned int AdvisorHelpClearTime;

/* Last game timer value at which the periodic level-script poll ran. */
// GLOBAL: LEGOLAND 0x007fe054
unsigned int LastLevelScriptRunTime;

// GLOBAL: LEGOLAND 0x007fe114
unsigned char LegolandCommonThemeCount;

// GLOBAL: LEGOLAND 0x007fe115
unsigned char WesternThemeCount;

// GLOBAL: LEGOLAND 0x007fe116
unsigned char CastleThemeCount;

// GLOBAL: LEGOLAND 0x007fe117
unsigned char AdventurersThemeCount;

// GLOBAL: LEGOLAND 0x007fe120
char *ScriptStringTable[512];

// GLOBAL: LEGOLAND 0x007fe920
unsigned int HelpShownTick;

// GLOBAL: LEGOLAND 0x007fe930
unsigned char ObjectiveCounters[10];

/* Game timer at SaveScripts/LoadScripts time; objective event timestamps are saved relative to it. */
// GLOBAL: LEGOLAND 0x007fe994
unsigned int ScriptTimeBase;

/* Colour every LLS pixel is ANDed with in SoftPrint_XBltFast (nearest colour of the tint), or ~nearest(15,15,15) for the half-bright draw. */
// GLOBAL: LEGOLAND 0x007fe998
unsigned short LLSColourMask;

/* Address of the surface pixel under the mouse; the span drawers compare their write pointer against it. */
// GLOBAL: LEGOLAND 0x007fe9a8
unsigned short *MousePixelAddress;

// GLOBAL: LEGOLAND 0x007fe9c0
struct Sprite *PointerSprites[9];

// GLOBAL: LEGOLAND 0x007fea10
unsigned int SpanRowsLeft;

// GLOBAL: LEGOLAND 0x007fea30
RECT WatchRect;

// GLOBAL: LEGOLAND 0x007fea40
unsigned int SoftPrintSourceBits;

// GLOBAL: LEGOLAND 0x007fea44
unsigned int StoredTransparentColour;

// GLOBAL: LEGOLAND 0x007fea48
LEGO_EXPORT unsigned int FramesPerSecond;

/* Set by the sprite/span drawers when they draw over the pixel under the mouse (hover hit test, e.g. a person under the cursor). */
// GLOBAL: LEGOLAND 0x007feb14
unsigned int MousePixelCovered;

/* Hand-written blitters: surface address of the current row; SpanRowsLeft counts the rows left. */
// GLOBAL: LEGOLAND 0x007feb18
unsigned int SpanDstRow;

/* Anchor tile id of the first object in the map render order chain. */
// GLOBAL: LEGOLAND 0x007febb8
unsigned short RenderOrderFirstAnchor;

// GLOBAL: LEGOLAND 0x007febc0
LEGO_EXPORT struct Cursor EditCursor;

// GLOBAL: LEGOLAND 0x008003f4
struct Element *LevelMapHandle;

/* Basic tile slots 0-4 (offsets from the first BASIC TILES 1 sprite), set by LoadMapTiles; slot 3 is the red marker tile. */
// GLOBAL: LEGOLAND 0x008003f8
unsigned int BasicTileSlot0;

// GLOBAL: LEGOLAND 0x00800400
LEGO_EXPORT RECT ObjectPartArray[256];

// GLOBAL: LEGOLAND 0x00801400
LEGO_EXPORT struct MapElement **GameMap;

// GLOBAL: LEGOLAND 0x00801404
struct Element *BridgesHandle;

// GLOBAL: LEGOLAND 0x00801408
unsigned int MapRenderOrderCount;

// GLOBAL: LEGOLAND 0x0080140c
struct Element *LevelTileMapHandle;

// GLOBAL: LEGOLAND 0x00801410
struct Element *OverlayILFHandle;

/* Sprites queued during map drawing and printed afterwards by DrawDeferredSprites. */
// GLOBAL: LEGOLAND 0x00801420
struct DeferredSprite DeferredSprites[100];

// GLOBAL: LEGOLAND 0x00801a60
int Bridge1OverlayOffsetX;

// GLOBAL: LEGOLAND 0x00801a64
int Bridge1OverlayOffsetY;

// GLOBAL: LEGOLAND 0x00801a68
struct Element **MapObjectElements;

// GLOBAL: LEGOLAND 0x00801a6c
int *BasicTiles1Data;

// GLOBAL: LEGOLAND 0x00801a70
struct MapTileSet **MapTileSets;

// GLOBAL: LEGOLAND 0x00801a74
int BaseMapEntryCount;

// GLOBAL: LEGOLAND 0x00801a80
struct MapRect RectList[10];

// GLOBAL: LEGOLAND 0x00801b20
unsigned int BasicTileSlot1;

// GLOBAL: LEGOLAND 0x00801b24
LEGO_EXPORT unsigned int ObjectPartCount;

// GLOBAL: LEGOLAND 0x00801b28
int MapObjectElementCount;

// GLOBAL: LEGOLAND 0x00801b40
int ObjectPartKey[256];

// GLOBAL: LEGOLAND 0x00801f40
LEGO_EXPORT struct TileSpriteEntry TileSpriteInfo[2048];

// GLOBAL: LEGOLAND 0x00805f40
int Bridge0OverlayOffsetX;

// GLOBAL: LEGOLAND 0x00805f44
int Bridge0OverlayOffsetY;

// GLOBAL: LEGOLAND 0x00805f48
unsigned int BasicTileSlot3;

// GLOBAL: LEGOLAND 0x00805f60
LEGO_EXPORT struct Sprite *TileSpriteArray[2048];

// GLOBAL: LEGOLAND 0x00807f60
struct MapRenderOrderEntry MapRenderOrderList[4096];

// GLOBAL: LEGOLAND 0x0080ff60
unsigned int BasicTileSlot2;

// GLOBAL: LEGOLAND 0x0080ff64
struct Element *CastleObjElem;

// GLOBAL: LEGOLAND 0x0080ff68
unsigned int BasicTileSlot4;

// GLOBAL: LEGOLAND 0x0080ff6c
void *DragRemoveClass;

// GLOBAL: LEGOLAND 0x0080ff70
unsigned int MapScreenClosePending;

// GLOBAL: LEGOLAND 0x0080ff74
LEGO_EXPORT unsigned int NEWFLC_AutoPlay;

// GLOBAL: LEGOLAND 0x0080ff78
LEGO_EXPORT unsigned char NEWFLC_PauseType;

/* Current front-end screen transition/mode state, saved separately around title and options screens. */
// GLOBAL: LEGOLAND 0x0080ff80
struct ScreenMode FrontEndScreenMode;

// GLOBAL: LEGOLAND 0x0080ffa0
struct ScreenState CurrentProfile;

// GLOBAL: LEGOLAND 0x008100c0
char LevelEndMovieName[0x80];

/* Set by LoadGame, cleared by LoadBaseMap: the current level came from a saved game. */
// GLOBAL: LEGOLAND 0x00810140
unsigned int LevelLoadedFromSave;

/* Set when a class CalcCursor (boating school D) built QueryCursor itself: removal then keeps its field_1830 and the path under it. */
// GLOBAL: LEGOLAND 0x00810144
unsigned int QueryCursorKeepPath;

// GLOBAL: LEGOLAND 0x00810148
struct Sprite *SPRITE_TitleScreenBk;

// GLOBAL: LEGOLAND 0x0081014c
LEGO_EXPORT unsigned char NEWFLC_ID[20];

// GLOBAL: LEGOLAND 0x00810160
LEGO_EXPORT struct Cursor QueryCursor;

// GLOBAL: LEGOLAND 0x008119a0
LEGO_EXPORT unsigned int NEWFLC_CheckDuplicate;

// GLOBAL: LEGOLAND 0x008119a4
unsigned int FrameCounter;

// GLOBAL: LEGOLAND 0x008119a8
LEGO_EXPORT unsigned int NEWFLC_BuffSize;

// GLOBAL: LEGOLAND 0x008119ac
LEGO_EXPORT unsigned short NEWFLC_Repeat;

// GLOBAL: LEGOLAND 0x008119b0
LEGO_EXPORT struct EditState EditMode;

// GLOBAL: LEGOLAND 0x008119bc
unsigned int MapScreenOpenPending;

// GLOBAL: LEGOLAND 0x008119c0
struct MapMarker MapSpannerMarks[32][32]; /* one table: its last row is MapSpannerMarksLastRow (globals.h) */

// GLOBAL: LEGOLAND 0x008139c0
int MapViewHeight;

// GLOBAL: LEGOLAND 0x008139c4
int MapViewWidth;

// GLOBAL: LEGOLAND 0x008139c8
int MapViewX;

// GLOBAL: LEGOLAND 0x008139cc
int MapViewY;

// GLOBAL: LEGOLAND 0x008139e0
struct Sprite *MoodIconSprites;

// GLOBAL: LEGOLAND 0x008139e4
struct Sprite *MiHungrySprite;

// GLOBAL: LEGOLAND 0x008139e8
struct Sprite *MiHappySprite;

// GLOBAL: LEGOLAND 0x008139ec
struct Sprite *MiSadSprite;

// GLOBAL: LEGOLAND 0x008139f0
struct Sprite *MiHomeSprite;

// GLOBAL: LEGOLAND 0x008139f4
struct Sprite *MiEatSprite;

// GLOBAL: LEGOLAND 0x008139f8
struct Sprite *GreatSprite;

// GLOBAL: LEGOLAND 0x008139fc
struct Sprite *PoorSprite;

// GLOBAL: LEGOLAND 0x00813a00
struct Sprite *FavouriteSprite;

// GLOBAL: LEGOLAND 0x00813a04
struct Sprite *OpinionSprite;

// GLOBAL: LEGOLAND 0x00813a08
struct Sprite *MiBoredSprite;

// GLOBAL: LEGOLAND 0x00813a0c
void *SpeechBubbleData;

// GLOBAL: LEGOLAND 0x00813a10
unsigned int DebugAllocatedBytes;

// GLOBAL: LEGOLAND 0x00813a18
unsigned long LastFrameTickCount;

// GLOBAL: LEGOLAND 0x00813a2c
int FrameTicksAccumulated;

/* Tile id of the object queried when a demolish drag starts (BeginTileSelectionDrag); not read elsewhere. */
// GLOBAL: LEGOLAND 0x00813a34
unsigned short DragQueryObjectId;

// GLOBAL: LEGOLAND 0x00813a38
unsigned int DragFootprintWidth;

// GLOBAL: LEGOLAND 0x00813a3c
unsigned int DragFootprintHeight;

// GLOBAL: LEGOLAND 0x00813a40
LEGO_EXPORT unsigned int GamePad;

// GLOBAL: LEGOLAND 0x00813a44
struct Point MousePos;

// GLOBAL: LEGOLAND 0x00813a4c
unsigned int MapLeftButtonMask;

// GLOBAL: LEGOLAND 0x00813a50
unsigned int MapLeftClickFlags;

// GLOBAL: LEGOLAND 0x00813a54
unsigned int MiddleButtonMask;

// GLOBAL: LEGOLAND 0x00813a5c
unsigned int MapRightButtonMask;

// GLOBAL: LEGOLAND 0x00813a60
unsigned int MapRightClickFlags;

// GLOBAL: LEGOLAND 0x00813a64
unsigned int MouseTileX;

// GLOBAL: LEGOLAND 0x00813a68
unsigned int MouseTileY;

// GLOBAL: LEGOLAND 0x00813a6c
int FootprintWidth;

// GLOBAL: LEGOLAND 0x00813a70
int FootprintHeight;

// GLOBAL: LEGOLAND 0x00813a74
int DragStartTileX;

// GLOBAL: LEGOLAND 0x00813a78
int DragStartTileY;

// GLOBAL: LEGOLAND 0x00813a7c
int DragEndTileX;

// GLOBAL: LEGOLAND 0x00813a80
int DragEndTileY;

// GLOBAL: LEGOLAND 0x00813a84
struct MapRect SelectionRect;

// GLOBAL: LEGOLAND 0x00813a94
unsigned int PrevHeldButtons;

// GLOBAL: LEGOLAND 0x00813a98
unsigned int ScrollUpKeyMask;

// GLOBAL: LEGOLAND 0x00813a9c
unsigned int ScrollUpKeyState;

// GLOBAL: LEGOLAND 0x00813aa0
unsigned int ScrollRightKeyMask;

// GLOBAL: LEGOLAND 0x00813aa4
unsigned int ScrollRightKeyState;

// GLOBAL: LEGOLAND 0x00813aa8
unsigned int ScrollDownKeyMask;

// GLOBAL: LEGOLAND 0x00813aac
unsigned int ScrollDownKeyState;

// GLOBAL: LEGOLAND 0x00813ab0
unsigned int ScrollLeftKeyMask;

// GLOBAL: LEGOLAND 0x00813ab4
unsigned int ScrollLeftKeyState;

// GLOBAL: LEGOLAND 0x00813ab8
unsigned int WatchedButtonMask;

/* Bits for WatchedButtonMask (button mask) this frame: 1 pressed, 2 released, 4 held, 8 repeat. */
// GLOBAL: LEGOLAND 0x00813abc
unsigned int WatchedButtonState;

// GLOBAL: LEGOLAND 0x00813ac0
unsigned int LeftButtonMask;

// GLOBAL: LEGOLAND 0x00813ac4
unsigned int MouseClickFlags;

// GLOBAL: LEGOLAND 0x00813ac8
unsigned int RightButtonMask;

// GLOBAL: LEGOLAND 0x00813acc
unsigned int RightClickFlags;

// GLOBAL: LEGOLAND 0x00813ad0
unsigned int CancelKeyMask;

// GLOBAL: LEGOLAND 0x00813ad4
unsigned int CancelKeyState;

// GLOBAL: LEGOLAND 0x00813ad8
unsigned int OkKeyMask;

// GLOBAL: LEGOLAND 0x00813adc
unsigned int OkKeyState;

/* GetTicks() of the last mouse movement or GamePad 0x10 input; after 2000 ms without one ReadGameButtons sets GamePad 0x200. */
// GLOBAL: LEGOLAND 0x00813ae0
unsigned int LastPointerActivityTicks;

// GLOBAL: LEGOLAND 0x00813af0
int DragFootprintX0;

// GLOBAL: LEGOLAND 0x00813af4
int DragFootprintY0;

// GLOBAL: LEGOLAND 0x00813af8
int DragFootprintX1;

// GLOBAL: LEGOLAND 0x00813afc
int DragFootprintY1;

// GLOBAL: LEGOLAND 0x00813b00
LEGO_EXPORT struct CtrlBuffer *CONTROLLERBUFFER;

// Last matching CD-ROM root, including its trailing backslash and NUL terminator.
// GLOBAL: LEGOLAND 0x00813b04
char CdDrivePath[4];

// GLOBAL: LEGOLAND 0x00813b08
unsigned int CurrentBlokeLetter;

// GLOBAL: LEGOLAND 0x00813b20
unsigned char PaletteReadBuffer[0x300] = {0};

// GLOBAL: LEGOLAND 0x00813e20
unsigned short Palette16Bit[256] = {0};

// GLOBAL: LEGOLAND 0x00814020
unsigned char ColourLookupTable[0x8000];

// GLOBAL: LEGOLAND 0x0081c028
struct Sprite *AppBarSprite;

// GLOBAL: LEGOLAND 0x0081c02c
struct Sprite *NextPageSprite;

// GLOBAL: LEGOLAND 0x0081c030
struct Sprite *AppBarMarkerSprite;

// GLOBAL: LEGOLAND 0x0081c034
struct Sprite *NextPageLitSprite;

// GLOBAL: LEGOLAND 0x0081c038
unsigned int AppraisalScreenActive;

// GLOBAL: LEGOLAND 0x0081c040
struct Sprite *AppTickSprites[5];

// GLOBAL: LEGOLAND 0x0081c054
struct Sprite *AppCrossSprites[5];

// GLOBAL: LEGOLAND 0x0081c068
struct Sprite *AppBulletSprites[5];

// GLOBAL: LEGOLAND 0x0081c07c
unsigned int AppraisalPageChanged;

// GLOBAL: LEGOLAND 0x0081c080
struct Sprite *PreviousPageSprite;

// GLOBAL: LEGOLAND 0x0081c084
struct Sprite *PreviousPageLitSprite;

// GLOBAL: LEGOLAND 0x0081c088
unsigned int AdvisorAnimStateParam;

// GLOBAL: LEGOLAND 0x0081c08c
void *AdBlinkAnim;

// GLOBAL: LEGOLAND 0x0081c090
void *AdWobbleAnim;

// GLOBAL: LEGOLAND 0x0081c094
void *AdLRAnim;

// GLOBAL: LEGOLAND 0x0081c098
void *AdPhoneDownAnim;

// GLOBAL: LEGOLAND 0x0081c09c
unsigned int NextAdvisorAnimStateParam;

// GLOBAL: LEGOLAND 0x0081c0a0
void *AdPhoneAnim;

// GLOBAL: LEGOLAND 0x0081c0a4
void *AdPhoneGestureAnim;

// GLOBAL: LEGOLAND 0x0081c0c0
int TextureSizes[0x200];

// GLOBAL: LEGOLAND 0x0081c4c0
unsigned int BmpPalette[0x100];

// GLOBAL: LEGOLAND 0x0081c8c0
struct LocFile *VisitorLocData;

// GLOBAL: LEGOLAND 0x0081c8c4
struct LocFile *TracyLocData;

// GLOBAL: LEGOLAND 0x0081c8c8
struct LocFile *GeoffLocData;

// GLOBAL: LEGOLAND 0x0081c8cc
void *RiderSearchCursor;

// GLOBAL: LEGOLAND 0x0081c8d0
unsigned int ViewportLeft;

// GLOBAL: LEGOLAND 0x0081c8d4
unsigned int ViewportTop;

// GLOBAL: LEGOLAND 0x0081c8d8
unsigned int ViewportRight;

// GLOBAL: LEGOLAND 0x0081c8dc
unsigned int ViewportBottom;

/* Name chosen in the file list dialog; also the directory it enters when a folder is picked. */
// GLOBAL: LEGOLAND 0x0081c8e0
char FileDialogSelectedName[512];

// GLOBAL: LEGOLAND 0x0081cae0
struct Sprite *ZoomerSprite;

// GLOBAL: LEGOLAND 0x0081cae8
int PlaneRideDrawOffsetX;

// GLOBAL: LEGOLAND 0x0081caec
int PlaneRideDrawOffsetY;

// GLOBAL: LEGOLAND 0x0081caf0
struct Ride *PottingShedRide;

// GLOBAL: LEGOLAND 0x0081caf4
struct Ride *MechanicsHutRide;

// GLOBAL: LEGOLAND 0x0081cb00
struct Sprite *SaloonMatte1Sprite;

// GLOBAL: LEGOLAND 0x0081cb04
struct Sprite *SaloonMatte2Sprite;

// GLOBAL: LEGOLAND 0x0081cb08
struct Sprite *GStoreMatteSprite;

// GLOBAL: LEGOLAND 0x0081cb0c
struct Sprite *JailCellMaskSprite;

// GLOBAL: LEGOLAND 0x0081cb10
struct Ride *ActiveWesternTownRide;

// GLOBAL: LEGOLAND 0x0081cb14
struct Ride *SheriffBuilding;

// GLOBAL: LEGOLAND 0x0081cb18
struct Sprite *LegoShop1MatteSprite;

// GLOBAL: LEGOLAND 0x0081cb1c
struct Ride *SaloonBuilding;

// GLOBAL: LEGOLAND 0x0081cb20
struct Sprite *LegoShop2MatteSprite;

// GLOBAL: LEGOLAND 0x0081cb24
struct Sprite *GStoreMatte2Sprite;

// GLOBAL: LEGOLAND 0x0081cb28
struct Sprite *ExplorersInstituteMatteSprite;

// GLOBAL: LEGOLAND 0x0081cb2c
struct Ride *BankBuilding;

// GLOBAL: LEGOLAND 0x0081cb30
struct Ride *GeneralStoreBuilding;

// GLOBAL: LEGOLAND 0x0081cb34
struct Sprite *BankMatteSprite;

// GLOBAL: LEGOLAND 0x0081cb38
struct Sprite *SherifshutMatteSprite;

// GLOBAL: LEGOLAND 0x0081cb3c
struct Ride *LegoShop1Building;

// GLOBAL: LEGOLAND 0x0081cb40
struct Ride *LegoMediaShopBuilding;

// GLOBAL: LEGOLAND 0x0081cb44
struct Ride *ExplorersInstituteBuilding;

// GLOBAL: LEGOLAND 0x0081cb48
struct Sprite *LegMediaShopMask1Sprite;

// GLOBAL: LEGOLAND 0x0081cb4c
struct Ride *LegoShop2Building;

// GLOBAL: LEGOLAND 0x0081cb50
struct Sprite *LegMediaShopMask2Sprite;

// GLOBAL: LEGOLAND 0x0081cb54
struct Ride *ActiveJungleCruiseRide;

// GLOBAL: LEGOLAND 0x0081cb58
struct TileMap *BoatingSchoolTileMap;

// GLOBAL: LEGOLAND 0x0081cb5c
struct Sprite *JungMaskSprite;

// GLOBAL: LEGOLAND 0x0081cb60
struct Ride *JungleCruiseRide;

/* Class passed when JungleCruiseRemoveObject removes the jungle cruise instances; never assigned in the game code. */
// GLOBAL: LEGOLAND 0x0081cb64
struct Ride *JungleCruiseInstanceClass;

// GLOBAL: LEGOLAND 0x0081cb68
struct Sprite *BrijMaskSprite;

// GLOBAL: LEGOLAND 0x0081cb6c
struct Sprite *MFish2Sprite;

// GLOBAL: LEGOLAND 0x0081cb70
struct Ride *MonkeyTreeRide;

// GLOBAL: LEGOLAND 0x0081cb74
struct Ride *MFish2Ride;

/* Rider screen offsets per seat (3) and boat heading frame (16), built by the seat ellipse initialiser. */
// GLOBAL: LEGOLAND 0x0081cb80
struct Point JungleCruiseSeatOffsets[3][16];

// GLOBAL: LEGOLAND 0x0081cd00
struct SpriteSet *JungleCruiseBoats;

// GLOBAL: LEGOLAND 0x0081cd04
void *FlowersRide;

// GLOBAL: LEGOLAND 0x0081cd08
void *HedgeObjectClass;

// GLOBAL: LEGOLAND 0x0081cd0c
struct Ride *CastleBBQRide;

// GLOBAL: LEGOLAND 0x0081cd10
struct Sprite *CastleBBQLayer;

// GLOBAL: LEGOLAND 0x0081cd14
struct Ride *FoodcartDrinkRide;

// GLOBAL: LEGOLAND 0x0081cd18
struct Ride *SharkCafeRide;

// GLOBAL: LEGOLAND 0x0081cd1c
struct Ride *OctopusCafeRide;

// GLOBAL: LEGOLAND 0x0081cd20
struct Sprite *R2TowermSprite;

// GLOBAL: LEGOLAND 0x0081cd28
struct Sprite *RestMaskMainSprite;

// GLOBAL: LEGOLAND 0x0081cd2c
struct Sprite *EateryFxInner;

// GLOBAL: LEGOLAND 0x0081cd30
struct Ride *Restaurant2Ride;

// GLOBAL: LEGOLAND 0x0081cd34
struct Sprite *R2FdoormSprite;

// GLOBAL: LEGOLAND 0x0081cd38
struct Ride *ActiveEateryRide;

// GLOBAL: LEGOLAND 0x0081cd3c
struct Ride *FoodcartFoodRide;

// GLOBAL: LEGOLAND 0x0081cd40
struct Ride *Restaurant1Ride;

// GLOBAL: LEGOLAND 0x0081cd44
struct Ride *ChuckWagonRide;

// GLOBAL: LEGOLAND 0x0081cd48
struct Sprite *R2Fdoorm1Sprite;

// GLOBAL: LEGOLAND 0x0081cd60
struct Sprite *OctTentASprite;

// GLOBAL: LEGOLAND 0x0081cd64
struct Sprite *OctTentBSprite;

// GLOBAL: LEGOLAND 0x0081cd68
struct Sprite *OctTentCSprite;

// GLOBAL: LEGOLAND 0x0081cd6c
struct Sprite *OctTentDSprite;

// GLOBAL: LEGOLAND 0x0081cd70
struct Sprite *OctTentESprite;

// GLOBAL: LEGOLAND 0x0081cd74
struct Sprite *OctTentFSprite;

// GLOBAL: LEGOLAND 0x0081cd78
struct Sprite *OctTentGSprite;

// GLOBAL: LEGOLAND 0x0081cd7c
struct Sprite *OctTentHSprite;

// GLOBAL: LEGOLAND 0x0081cd80
struct Sprite *OctoKioskSprite;

// GLOBAL: LEGOLAND 0x0081cd84
struct Sprite *R2BdoormSprite;

// GLOBAL: LEGOLAND 0x0081cd88
struct Sprite *RestMaskLevel1Sprite;

// GLOBAL: LEGOLAND 0x0081cd8c
struct Sprite *RestMaskLevel1aaSprite;

// GLOBAL: LEGOLAND 0x0081cd90
struct Sprite *RestMaskLevel3Sprite;

// GLOBAL: LEGOLAND 0x0081cd94
struct Sprite *RestMaskLevel2Sprite;

// GLOBAL: LEGOLAND 0x0081cda0
struct Sprite *OctTabAASprite;

// GLOBAL: LEGOLAND 0x0081cda4
struct Sprite *OctTabABSprite;

// GLOBAL: LEGOLAND 0x0081cda8
struct Sprite *OctTabBASprite;

// GLOBAL: LEGOLAND 0x0081cdac
struct Sprite *OctTabBBSprite;

// GLOBAL: LEGOLAND 0x0081cdb0
struct Sprite *OctTabCASprite;

// GLOBAL: LEGOLAND 0x0081cdb4
struct Sprite *OctTabCBSprite;

// GLOBAL: LEGOLAND 0x0081cdb8
struct Sprite *OctTabDASprite;

// GLOBAL: LEGOLAND 0x0081cdbc
struct Sprite *OctTabDBSprite;

// GLOBAL: LEGOLAND 0x0081cdc0
struct Sprite *OctTabEASprite;

// GLOBAL: LEGOLAND 0x0081cdc4
struct Sprite *OctTabEBSprite;

// GLOBAL: LEGOLAND 0x0081cdc8
struct Sprite *OctTabFASprite;

// GLOBAL: LEGOLAND 0x0081cdcc
struct Sprite *OctTabFBSprite;

// GLOBAL: LEGOLAND 0x0081cdd0
struct Sprite *OctTabGASprite;

// GLOBAL: LEGOLAND 0x0081cdd4
struct Sprite *OctTabGBSprite;

// GLOBAL: LEGOLAND 0x0081cdd8
struct Sprite *OctTabHASprite;

// GLOBAL: LEGOLAND 0x0081cddc
struct Sprite *OctTabHBSprite;

// GLOBAL: LEGOLAND 0x0081cde0
struct Ride *FoodcartIcecreamRide;

// GLOBAL: LEGOLAND 0x0081cde4
struct Ride *BalloonzRide;

// GLOBAL: LEGOLAND 0x0081cde8
struct Sprite *ZBalloon2Sprite;

// GLOBAL: LEGOLAND 0x0081cdec
void *CoasterPieceToRemove;

// GLOBAL: LEGOLAND 0x0081ce00
struct Cursor FreeSpotCursors[8];

// GLOBAL: LEGOLAND 0x00828fe0
unsigned int TrackPieceCurves[2];

// GLOBAL: LEGOLAND 0x00829980
void *BasicTilesData;

// GLOBAL: LEGOLAND 0x00829990
float CastleViewDir[3];

// GLOBAL: LEGOLAND 0x0082999c
float CastleLightScale;

// GLOBAL: LEGOLAND 0x008299a0
float CastleCameraPos[3];

// GLOBAL: LEGOLAND 0x008299bc
struct FMat4 CastleViewMatrix;

// GLOBAL: LEGOLAND 0x008299fc
struct FMat4 CastleWorldToScreen;

// GLOBAL: LEGOLAND 0x008299ac
int CastleViewRect[4]; /* view rect: left, top, right, bottom */

// GLOBAL: LEGOLAND 0x00829a3c
struct ListLink CastleLinkList;

// GLOBAL: LEGOLAND 0x00829a58
void (*lego_sqrtf_impl)(void);

// GLOBAL: LEGOLAND 0x00829a5c
void (*lego_rsqrtf_impl)(void);

// GLOBAL: LEGOLAND 0x00829a60
float CastleLightBias;

// GLOBAL: LEGOLAND 0x00829a64
struct Ride *HeightTrack2Ride;

// GLOBAL: LEGOLAND 0x00829a80
struct Footprint CastleCoasterFootprint;

// GLOBAL: LEGOLAND 0x00829abc
struct Element *CastleObjElement;

// One object of 0x118 bytes (0x10c of data, then three methods): the castle code reaches its parts both by name and through struct
// views of its address (sentinel list node at +4, anims, list hosts), so they must keep these offsets.
// GLOBAL: LEGOLAND 0x00829ae0
struct CastleState CastleCoaster;

// GLOBAL: LEGOLAND 0x004b5b48
struct PieceClass CastlePieceClass;

// GLOBAL: LEGOLAND 0x00829bf8
struct Ride *CastleCoasterRide;

// GLOBAL: LEGOLAND 0x00829bfc
struct Ride *HeightTrack25Ride;

// GLOBAL: LEGOLAND 0x00829c00
struct Element *CastleDummyElement;

// GLOBAL: LEGOLAND 0x00829c04
struct Sprite *CastleMatteSprite;

// GLOBAL: LEGOLAND 0x00829c08
struct Ride *SquareTrackRide;

// GLOBAL: LEGOLAND 0x00829c34
struct Ride *CastleDummyRide;

// GLOBAL: LEGOLAND 0x00829c54
char *CoasterShadeTableData;

// GLOBAL: LEGOLAND 0x00829c60
void *CoasterShadeTables[1024];

// GLOBAL: LEGOLAND 0x0082ac60
struct FloatArray *VecScratchPool[32]; /* InitVecMath fills 30 */

// GLOBAL: LEGOLAND 0x0082ad20
struct DispatchRow CastleDispatchTable[6];

// GLOBAL: LEGOLAND 0x0082adb0
struct Element *CastleDispatchTableEnd[6];

// GLOBAL: LEGOLAND 0x0082add0
struct LmsFile *CoasterTrainCarLms[3]; /* head, mid, tail; indexed by car type */

// GLOBAL: LEGOLAND 0x0082ade0
struct LmsRec *CoasterTrainCarLfm[3]; /* head, mid, tail; indexed by car type */

// GLOBAL: LEGOLAND 0x0082adec
struct PathSeg *TrainDrawPiece;

// GLOBAL: LEGOLAND 0x0082adf0
struct Ride *BoatingSchoolWaterRide;

// GLOBAL: LEGOLAND 0x0082adf4
struct TileMap *BoatingSchoolTileMapping;

// GLOBAL: LEGOLAND 0x0082adf8
struct Ride *BoatingSchoolMermaidRide;

// GLOBAL: LEGOLAND 0x0082adfc
struct Sprite *BoatingSchoolHullMaskSprite;

// GLOBAL: LEGOLAND 0x0082ae00
void *BoatingSchoolLayer5Sprite;

// GLOBAL: LEGOLAND 0x0082ae20
struct Cursor BoatingSchoolDCursor;

// GLOBAL: LEGOLAND 0x0082c654
struct Sprite *BoatingSchoolRailmSprite;

// GLOBAL: LEGOLAND 0x0082c658
struct Ride *BoatingSchoolRide;

// GLOBAL: LEGOLAND 0x0082c65c
struct SpriteSet *BoatingSchoolBoats;

// GLOBAL: LEGOLAND 0x0082c668
struct Sprite *ZSpiderSprite;

// GLOBAL: LEGOLAND 0x0082c678
void *ZebraCrossingRide;

// GLOBAL: LEGOLAND 0x0082c67c
struct DSMapEntry *DSchoolMappingData;

// GLOBAL: LEGOLAND 0x0082c680
struct RoadLights *DrivingSchoolLightsData;

// GLOBAL: LEGOLAND 0x0082c684
void *DrivingSchoolRoadsClass;

// GLOBAL: LEGOLAND 0x0082c688
void *LogFlumeInterfaceRide;

// GLOBAL: LEGOLAND 0x0082c690
unsigned short *DSchoolRedPalette;

// GLOBAL: LEGOLAND 0x0082c694
struct Ride *DrivingSchoolRide;

// GLOBAL: LEGOLAND 0x0082c6b8
unsigned short *DSchoolYellowPalette;

// GLOBAL: LEGOLAND 0x0082c6bc
unsigned short *DSchoolBluePalette;

// GLOBAL: LEGOLAND 0x0082c6c0
struct Sprite *DSchoolMatteSprite;

// GLOBAL: LEGOLAND 0x0082c6e0
struct Cursor ExtraEditCursor2;

// GLOBAL: LEGOLAND 0x0082df20
struct Cursor ExtraEditCursor3;

/* Extra cursor parts chained after EditCursor by the driving school, pumps and road edit modes. */
// GLOBAL: LEGOLAND 0x0082f760
struct Cursor ExtraEditCursor1;

// 0x00830b64/68/74, 0x00830f88/90 are ExtraEditCursor1 cursor fields.
// 0x00811564, 0x00811568, 0x00811988, 0x00811574 are QueryCursor fields
// (field_1404, field_1408, field_1828, field_1414[5]) — see struct Cursor.
// 0x007fffc8 is EditCursor.field_1408 — see struct Cursor in gamemap.h.

// GLOBAL: LEGOLAND 0x00830f94
struct Sprite *DSCarSprite;

// GLOBAL: LEGOLAND 0x00830f9c
struct DriveTable *DSchoolBlueCarData;

// GLOBAL: LEGOLAND 0x00830f98
struct Position *CoptersPos;

// GLOBAL: LEGOLAND 0x00830fc0
LEGO_EXPORT struct Cursor PathCursor;

// GLOBAL: LEGOLAND 0x00832800
LEGO_EXPORT struct MapStats MapStats;

// GLOBAL: LEGOLAND 0x00832bf0
LEGO_EXPORT void *PathSprite;

// GLOBAL: LEGOLAND 0x0082c6a0
struct RideSpriteInfo RideSpriteInfoBuffer;

// GLOBAL: LEGOLAND 0x00616028
struct RideSpriteInfo BalloonzSpriteInfo;

// GLOBAL: LEGOLAND 0x006160a0
struct RideSpriteInfo CarouselSpriteInfo;

// GLOBAL: LEGOLAND 0x0062fe30
struct RideSpriteInfo MechanicsHutSpriteInfo;

// GLOBAL: LEGOLAND 0x0062fe10
struct RideSpriteInfo PottingShedSpriteInfo;

// GLOBAL: LEGOLAND 0x004ab404
float FLOAT_004ab404;

// GLOBAL: LEGOLAND 0x0082ace0
struct PlaneSet ClipPlaneSet;

// GLOBAL: LEGOLAND 0x004b55fc
struct PlaneSet *ClipPlanes;

// GLOBAL: LEGOLAND 0x004d83b4
struct Train *DerivTrain;

// GLOBAL: LEGOLAND 0x004d83a0
struct CurveWalker DerivTrainWalker;

// GLOBAL: LEGOLAND 0x004b5634
float ScalarDerivThirdDiffWeight = 0.16666667f;

// GLOBAL: LEGOLAND 0x004b559c
float TrainCarSeatX[3] = {0.0f, 4.0f, 8.0f};

// GLOBAL: LEGOLAND 0x004b55a8
float TrainSeatSpacing = 16.0f;

// GLOBAL: LEGOLAND 0x004b4cac
char SafariManboxName[] = "manbox??";

/* Alt textures for men (altman.txt): base texture A index, then alternatives; B likewise. CreatePersonFaceList swaps base for a random alternative. */
// GLOBAL: LEGOLAND 0x00641000
int AltManBaseTextureA;

/* DrawPersonMesh: Z-buffer value per vertex (person layer field_34 in the top byte, scaled depth below). */
// GLOBAL: LEGOLAND 0x00641004
int PersonMeshVertexZ[3000];

/* DrawPersonMesh: transformed vertices (x, y, z 16.16), 12 bytes each, vertex 1 at +0xc. */
// GLOBAL: LEGOLAND 0x00643edc
int PersonMeshVertices[9131];

// GLOBAL: LEGOLAND 0x0063810c
int AltManTextureCountA;

// GLOBAL: LEGOLAND 0x0064cd90
int AltManBaseTextureB;

// GLOBAL: LEGOLAND 0x0064cd88
int AltWomanTextureCountA;

// GLOBAL: LEGOLAND 0x0063835c
int AltWomanTextureCountB;

// GLOBAL: LEGOLAND 0x00638110
int AltWomanBaseTextureA;

// GLOBAL: LEGOLAND 0x00638108
int AltWomanBaseTextureB;

// GLOBAL: LEGOLAND 0x004d87cc
int *ClipVertsOut[32];

// GLOBAL: LEGOLAND 0x004b5600
int **ClipVertexLists[2];

// GLOBAL: LEGOLAND 0x004b5610
float DerivThirdDiffWeight = 0.16666667f;

// GLOBAL: LEGOLAND 0x004b5614
float Deriv4PointFirstOffset = -1.0f;

// GLOBAL: LEGOLAND 0x004b5618
float Deriv4PointOffsets[3] = {-0.33333334f, 0.33333334f, 1.0f};

// GLOBAL: LEGOLAND 0x004b5624
float Deriv4PointFirstWeight = 0.0625f;

// GLOBAL: LEGOLAND 0x004b5628
float Deriv4PointWeights[3] = {-1.6875f, 1.6875f, -0.0625f};

// GLOBAL: LEGOLAND 0x004d83c4
int ClipVertexPool[0x100];

// GLOBAL: LEGOLAND 0x004d87c4
int *ClipVertexPoolNext;

// GLOBAL: LEGOLAND 0x004d884c
int *ClipVertsIn[32];

// GLOBAL: LEGOLAND 0x004d88cc
struct FloatArray **DifferenceTableRows[5];

// GLOBAL: LEGOLAND 0x004b5660
float RK4Coefficients[4][2] = {{0.0f, 0.1666666f}, {0.5f, 0.3333333f}, {0.5f, 0.3333333f}, {1.0f, 0.1666666f}};

// GLOBAL: LEGOLAND 0x004dd644
struct SlopeCurve *SplitCurveStart;

// GLOBAL: LEGOLAND 0x004dd648
struct SlopeCurve *SplitCurve;

// GLOBAL: LEGOLAND 0x004dd64c
float *SplitPointsNext;

// GLOBAL: LEGOLAND 0x004dd650
int SplitPointCount;

// GLOBAL: LEGOLAND 0x004ab438
float FLOAT_004ab438;

// GLOBAL: LEGOLAND 0x00610804
int DepthSpanRecordHistory[64];

// GLOBAL: LEGOLAND 0x00610500
int FilledTriangleHistory[64];

// GLOBAL: LEGOLAND 0x00610400
int EdgeMeshDrawCycleHistory[64];

// GLOBAL: LEGOLAND 0x00610704
int UnusedCastleCycleHistory[64];

// GLOBAL: LEGOLAND 0x00610604
int SupportPostCycleHistory[64];

// GLOBAL: LEGOLAND 0x006100f8
int LmsDrawCycleHistory[64];

// GLOBAL: LEGOLAND 0x0060fdf8
int TrainStepCycleHistory[64];

// GLOBAL: LEGOLAND 0x0060fef8
int DepthClearCycleHistory[64];

// GLOBAL: LEGOLAND 0x0060fbf8
int BisectRootCallHistory[64];

// GLOBAL: LEGOLAND 0x006101f8
int BisectRootStepHistory[64];

// GLOBAL: LEGOLAND 0x0060fcf8
int WalkerRangeCallHistory[64];

// GLOBAL: LEGOLAND 0x00610904
int TotalCastleCycleHistory[64];

/* Frame index of the castle profiling history rings (RecordCastleFrameCounters, & 0x3f). */
// GLOBAL: LEGOLAND 0x00610a10
unsigned int CastleCounterFrame;

// GLOBAL: LEGOLAND 0x0060f900
int FilledTriangleCount;

// GLOBAL: LEGOLAND 0x0060f904
int CoasterShadeTableCount;

/* Single white shade ramp used for every coaster shade table when their allocation fails. */
// GLOBAL: LEGOLAND 0x00579878
char CoasterFallbackShadeRamp[0x80];

// GLOBAL: LEGOLAND 0x0060f8fc
int EdgeMeshDrawCycles;

/* Castle cycle counter that is recorded and cleared each frame but never incremented in the game code. */
// GLOBAL: LEGOLAND 0x00611644
int UnusedCastleCycles;

/* DrawSupportPost cycle count for this frame; RecordCastleFrameCounters stores it in the SupportPostCycleHistory ring and clears it. */
// GLOBAL: LEGOLAND 0x00615f68
int SupportPostDrawCycles;

// GLOBAL: LEGOLAND 0x004dcbc8
int LmsDrawCycles;

// GLOBAL: LEGOLAND 0x004d83bc
int TrainStepCycles;

// GLOBAL: LEGOLAND 0x0060f910
int DepthClearCycles;

// GLOBAL: LEGOLAND 0x006126d8
float RailTubeRingNormals[6][2];

/* Rail tube triangle indices: 30 segments x 12 triangles (high bit = edge flag); row 30 is reused by DrawSupportPost as vertex scratch. */
// GLOBAL: LEGOLAND 0x00612708
unsigned int RailTubeTriangles[31][36];

// GLOBAL: LEGOLAND 0x00613878
unsigned int RailTubeSegmentTriangles[36];

// GLOBAL: LEGOLAND 0x00613908
int RailTubeSegmentEdges[24][2];

// GLOBAL: LEGOLAND 0x006148b8
int RailTubeEdges[546][2];

// GLOBAL: LEGOLAND 0x00667528
int ExceptionReportStarted;

/* Number of code bytes at CS:EIP written to the exception log (16). */
// GLOBAL: LEGOLAND 0x004b8a88
int StackDumpCodeBytes = 0x10;

// GLOBAL: LEGOLAND 0x004b8a8c
int StackDumpMaxWords = 0x800;

/* Stack words per line in the exception log stack dump. */
// GLOBAL: LEGOLAND 0x004b8a90
int StackDumpWordsPerLine = 8;
