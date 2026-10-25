#pragma once

#include "globals.h"

struct PathSeg;
struct Bloke;
struct Point;
struct CurveVt;
struct FVec3;
struct AreaNode;
struct Cursor;

/* a piece's opening: the direction mask and where it is relative to the piece */
struct PieceDoor {
    /* 0x00 */ unsigned int mask;
    /* 0x04 */ short x;
    /* 0x06 */ short y;
};

/* A track piece type (one per piece element; see CastlePieceClass, SquareTrackPieceClass, HeightTrack*PieceClass).
 * flags bit 1: fixed-height piece (an anchor; the castle and the height blocks). f4/f8: the heights its next/prev
 * ends take (set_heights), in height steps. next_door/prev_door: where, relative to the piece, the next/previous
 * piece must sit (mask = allowed directions, see PieceNeighbourOffsets). Methods: fill_open_end makes a piece whose
 * one end is unconnected straight-through; set_heights sets the end heights on attach (plain track copies the
 * neighbour's height, anchors use f4/f8); get_curve returns the curve stretch drawn/ridden over the piece;
 * on_add/on_remove run when the piece is placed/removed (only the path height block uses them, to add/remove the
 * 2x2 path under it). */
/* a piece's class (the piece's cls; the castle's own is CastlePieceClass): data, then methods */
struct PieceClass {
    /* 0x00 */ unsigned int flags;
    /* 0x04 */ int f4;
    /* 0x08 */ int f8;
    /* 0x0c */ struct PieceDoor next_door;
    /* 0x14 */ struct PieceDoor prev_door;
    /* 0x1c */ void (*fill_open_end)(struct PathSeg *self);
    /* 0x20 */ unsigned int (*set_heights)(struct PathSeg *self, float height);
    /* 0x24 */ struct SegBlob *(*get_curve)(struct PathSeg *self);
    /* 0x28 */ void (*on_add)(struct PathSeg *self);
    /* 0x2c */ void (*on_remove)(struct PathSeg *self);
    /* 0x30 */ unsigned int field_30;
    /* 0x34 */ unsigned int field_34;
};

/* One end of a piece. end_a is the end toward prev, end_b the end toward next; dir is the direction (1 N, 2 E, 4 S,
 * 8 W) in which the neighbour lies, 0xffffffff while unconnected. */
/* one end of a piece: the direction it opens to, its height and the piece joined there */
struct PieceEnd {
    /* 0x00 */ unsigned int dir;
    /* 0x04 */ float height;
    /* 0x08 */ struct PathSeg *link;
};

/* The start of every piece and of the castle itself (CastleState.piece). In a closed ring the castle's piece is
 * also the ring's sentinel. flags bit 1: the curve and world position are stored in the piece itself (vec/blob)
 * instead of coming from get_curve. */
/* the start of every piece (and of the castle itself, CastleState.piece, the pieces' ring sentinel) */
struct PieceHead {
    /* 0x00 */ unsigned int flags;
    /* 0x04 */ union {
        struct Int16Pair pos;
        struct {
            short x;
            short y;
        };
        unsigned int xy; /* both coordinates at once, as a search key */
    };
    /* 0x08 */ struct Ride *ride;
    /* 0x0c */ struct PieceClass *cls;
    /* 0x10 */ struct CastleState *owner;
    /* 0x14 */ union {
        struct PieceEnd end_a;
        struct {
            unsigned int dir_a;
            float f18;
            struct PathSeg *prev;
        };
    };
    /* 0x20 */ union {
        struct PieceEnd end_b;
        struct {
            unsigned int dir_b;
            float f24;
            struct PathSeg *next;
        };
    };
};

/* a curve stretch's methods (one row of 8 in CurveFunctionTable per stretch kind): pairs of (point at t, derivative at t),
 * t passed as raw float bits */
struct CurveFnPair {
    void (*func)(void *base, int t, struct FVec3 *out);
    void (*fn)(void *self, int t, void *out);
};

struct CurveVt {
    union {
        struct CurveFnPair pair[4];
        struct {
            void (*side_a_pos)(struct SegBlob *self, float t, float *out); /* centre + side offset */
            void *side_a_tangent;
            void (*pos)(struct SegBlob *self, float t, float *out); /* the centre line */
            void (*tangent)(struct SegBlob *self, float t, float *out);
            void (*side_b_pos)(struct SegBlob *self, float t, float *out); /* centre - side offset */
            void *side_b_tangent;
            void *split_points; /* (self, float *out): parameters to split the stretch at for drawing; returns the count */
            unsigned int (*normal)(struct SegBlob *self, unsigned int t, float *out); /* the track's up vector */
        };
    };
};

/* walks the curve stretches along the pieces */
struct CurveWalker {
    /* 0x00 */ struct PathSeg *piece;
    /* 0x04 */ struct SegBlob *node;
    /* 0x08 */ struct FVec3 pos; /* the piece's position (GetPieceCurve) */
};

/* a castle piece (0xa4 bytes) */
struct PathSeg {
    struct PieceHead; /* MS extension: the header's fields are the piece's own */
    /* 0x2c */ struct Footprint fp; /* the piece's link of the castle cursor's footprint chain */
    /* 0x40 */ union {
        struct FVec3 vec;
        struct {
            unsigned int field_40;
            unsigned int field_44;
            unsigned int field_48;
        };
    };
    /* 0x4c */ struct SegBlob blob;
};

struct AreaNode {
    int x;
    int y;
    int w;
    int h;
    struct AreaNode *next;
};

struct DirPoints {
    unsigned int mask;
    short pt[4][2];
};

/* a train car's placement: position and orientation (the car's float[12]) */
struct CarXform {
    struct FVec3 pos;
    float m[9];
};

/* a seat of a train car (two per car) */
struct TrainSeat {
    /* 0x00 */ struct FVec3 pos;
    /* 0x0c */ struct CastleVisitor *occupant;
    /* 0x10 */ unsigned int (*method_10)(struct TrainSeat *self);
    /* 0x14 */ void (*method_14)(struct TrainSeat *self, void *arg);
    /* 0x18 */ void (*method_18)(struct TrainSeat *self);
    /* 0x1c */ void (*draw)(struct TrainSeat *self, struct CarXform *car);
};

/* a point of a train car on the track */
struct TrainWheel {
    /* 0x00 */ float t;
    /* 0x04 */ struct CurveWalker walker;
};

/* a wheel set of a train car: two points on the track and the wheels' turn */
struct TrainBogie {
    /* 0x00 */ struct TrainWheel w0;
    /* 0x18 */ float ang[2];
    /* 0x20 */ struct TrainWheel w1;
};

/* A car of the train (kind 0 head, 1 middle, 2 tail). Two bogies (each a wheel pair on the track), two seats, the
 * centre between the bogies and its potential energy. flags bit 1: drawn this frame. link is its entry in the
 * castle's dirty-rectangle list. */
/* a car of the castle's coaster train (the train's cars form a ring through prev/next) */
struct TrainCar {
    /* 0x00 */ unsigned int flags;
    /* 0x04 */ unsigned int kind; /* 0 head, 1 middle, 2 tail */
    /* 0x08 */ struct TrainBogie front;
    /* 0x40 */ struct TrainBogie rear;
    /* 0x78 */ struct TrainSeat seats[2];
    /* 0xb8 */ struct FVec3 center;
    /* 0xc4 */ float potential; /* potential energy: mass * height (PositionTrainCar) */
    /* 0xc8 */ struct ListLink link;
    /* 0xe4 */ struct TrainCar *prev;
    /* 0xe8 */ struct TrainCar *next;
};

/* the train's motion state (Train.motion) */
struct TrainMotion {
    unsigned int count; /* always 2: this is a FloatArray of [pos, energy] for the integrator */
    float pos; /* curve parameter along the current curve stretch */
    float energy; /* total energy: potential energy of the cars + kinetic energy */
};

/* vector arithmetic on FloatArrays (VecMathOps is the template; InitVecMath copies it with a dimension) */
struct VecMath {
    /* 0x00 */ void (*add)(struct FloatArray *a, struct FloatArray *b, struct FloatArray *out);
    /* 0x04 */ void (*sub)(struct FloatArray *a, struct FloatArray *b, struct FloatArray *out);
    /* 0x08 */ void (*copy)(struct FloatArray *src, struct FloatArray *dst);
    /* 0x0c */ void (*scale)(struct FloatArray *arr, float f);
    /* 0x10 */ float (*maxabs)(struct FloatArray *arr);
    /* 0x14 */ void (*blend)(struct FloatArray *a, float s, struct FloatArray *b, float t, struct FloatArray *out);
    /* 0x18 */ void (*zero)(struct FloatArray *buf, int size);
    /* 0x1c */ void (*axpy)(struct FloatArray *a, float f, struct FloatArray *out);
    /* 0x20 */ struct FloatArray **(*alloc)(unsigned int n); /* n work vectors from the pool */
    /* 0x24 */ void (*release)(struct FloatArray **h, unsigned int n);
    /* 0x28 */ unsigned int dim;
};

/* a Runge-Kutta stepper over a state vector (the train's: Train.integ, state = &motion) */
struct Integrator {
    /* 0x00 */ void (*store)(struct Integrator *self, struct FloatArray *v);
    /* 0x04 */ unsigned int (*load)(struct Integrator *self, struct FloatArray *v);
    /* 0x08 */ void (*deriv)(struct Integrator *self, float t, struct FloatArray *out);
    /* 0x0c */ struct VecMath math;
    /* 0x38 */ void *state;
    /* 0x3c */ struct Train *owner;
};

/* The castle's coaster train. flags: 2 running, 0x40 held at the start (until hold_until), and the phase: 8 in the
 * station (leaving), 4 out on the track (physics), 0x10 arriving back at the castle. walker is the head car's
 * position on the track, motion its curve parameter and total energy, stepped by integ (RK4). The cars form a ring
 * through head. */
/* the castle's coaster train (CastleState.train) */
struct Train {
    /* 0x00 */ unsigned int flags;
    /* 0x04 */ int last_step_time; /* game time of the last step */
    /* 0x08 */ int hold_until; /* game time the hold (flags & 0x40) ends */
    /* 0x0c */ struct CurveWalker walker;
    /* 0x20 */ struct TrainMotion motion;
    /* 0x2c */ struct Integrator integ; /* state &motion, owner itself */
    /* 0x6c */ struct CastleState *track;
    /* 0x70 */ struct TrainCar head;
};

struct KeyPair {
    unsigned int key;
    unsigned int val;
};

struct LmsRef {
    short idx;
    unsigned char pad_2[14];
};

struct LmsRec {
    unsigned int key;
    unsigned char pad_4[8];
};

/* A coaster model (.lms, from RollerCoaster/RollerCoaster/CreatedData; offsets are made absolute by LoadLmsFile).
 * Three triangle lists, one per shading mode; each LmsRef's idx selects the triangle's material record in the
 * model's .lfm file, whose key is an lpt colour (flat, Gouraud) or an ltx texture index (textured). DrawLmsModel
 * transforms the vertices and draws the three lists. */
/* a roller coaster mesh (.lms, CreatedData): the offsets at 0x0c..0x28 are relocated into pointers on load */
struct LmsFile {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ void *field_c;
    /* 0x10 */ void *field_10;
    /* 0x14 */ void *field_14;
    /* 0x18 */ struct LmsRef *flat_tris; /* flat_count flat-shaded triangles (lfm key: lpt colour) */
    /* 0x1c */ int flat_count;
    /* 0x20 */ struct LmsRef *gouraud_tris; /* gouraud_count Gouraud-shaded triangles (lfm key: lpt colour) */
    /* 0x24 */ int gouraud_count;
    /* 0x28 */ struct LmsRef *tex_tris; /* tex_count textured triangles (lfm key: ltx texture) */
    /* 0x2c */ int tex_count;
    /* 0x30 */ struct FVec3 box[8]; /* the bounding box's corners */
};

/* a visitor queueing for the castle (CastleState.visitors is the queue's sentinel) */
struct CastleVisitor {
    /* 0x00 */ unsigned int flags; /* 1, 2 or 4 */
    /* 0x04 */ struct LmsFile *lms; /* sitting pose */
    /* 0x08 */ struct LmsRec *field_8; /* the visitor's 3D figure (freed unless flags & 1) */
    /* 0x0c */ struct Bloke *bloke;
    /* 0x10 */ struct CastleVisitor *prev;
    /* 0x14 */ struct CastleVisitor *next;
    /* 0x18 */ struct TrainSeat *seat;
    /* 0x1c */ unsigned int time;
    /* 0x20 */ unsigned int (*draw)(struct CastleVisitor *self, struct CarXform *car); /* DrawCastleVisitor */
    /* 0x24 */ void (*unlink)(struct CastleVisitor *self); /* FreeCastleVisitor */
};

/* CastleCoaster: the castle and its track. flags: 0 no castle placed, 1 the pieces form an open chain from head
 * (prev end free) to tail (next end free), 2 they form a closed ring through the castle's own piece.
 * next_spots/prev_spots: the free cells where a piece can attach after the tail / before the head. */
/* CastleCoaster: the castle */
struct CastleState {
    /* 0x00 */ unsigned int flags; /* 2: the pieces form a closed ring */
    /* 0x04 */ struct PieceHead piece;
    /* 0x30 */ unsigned char pad_30[0xa8 - 0x30];
    /* 0xa8 */ struct PathSeg *tail;
    /* 0xac */ struct DirPoints next_spots;
    /* 0xc0 */ struct PathSeg *head;
    /* 0xc4 */ struct DirPoints prev_spots;
    /* 0xd8 */ struct Train *train;
    /* 0xdc */ unsigned char pad_dc[4];
    /* 0xe0 */ unsigned int arrive_time; /* game time the train last stopped in the station */
    /* 0xe4 */ struct CastleVisitor visitors;
    /* 0x10c */ void (*get_start)(struct CastleState *self, struct SegBlob **node, float *t, struct FVec3 *pos);
    /* 0x110 */ float (*step_leaving)(struct Train *self, float time_left); /* CastleStepLeaving */
    /* 0x114 */ float (*step_arriving)(struct Train *self, float time_left); /* CastleStepArriving */
};

struct Point;
int GetCoasterPieceMapLine(struct Point *pos, void *a2, void *a3, void *a4, void *a5);

struct ObjAtC8;
struct ListLink;
struct Int16Pair;
/* a triangle mesh: vertices, edges between two vertices, triangles of three edges (bit 31: edge reversed) */
struct MVert {
    int x;
    int y;
    int rest[3];
};
struct MEdge {
    int v0;
    int v1;
};
struct MTri {
    int e[3];
};
struct EdgeMesh {
    int f0;
    int nVerts;
    int nEdges;
    int nTris;
    struct MVert *verts;
    struct MEdge *edges;
    struct MTri *tris;
};
/* 0x004b5f60: the rail tube built for each track stretch before it is drawn. Declared here, not in globals.h:
 * the struct EdgeMesh tag there shifts MSVC codegen in MusicThreadProc. */
extern struct EdgeMesh RailTubeMesh;
struct EdgeMesh *EdgeMeshRemoveBackfaces(struct EdgeMesh *src);
struct FVec3;
struct PiecePlacement;
struct FitRes;
struct RectI;
void DrawCastleTrainOnPiece(struct PieceHead *param_1);
void AddCastleCoasterMapObjects(short *param_1);
void CreateCastleTrain(struct CastleState *param_1);
void InitCastleVisitorList(struct CastleState *param_1);
struct RecIdx;
struct RecSrc;
void ClearDepthSpans(int n, struct RecIdx *idx, struct RecSrc *src);
struct TrainSaveRecord;
struct VisitorLoadRecord;
struct VisitorSaveRecord;
struct CastleSaveInfo;
struct CastleSaveRecord;
struct PieceSaveRef;
struct BlokeSex0;
void FreeCastleVisitor(struct CastleVisitor *p);

void MapPosToWorld(const struct Int16Pair *in, float f, struct FVec3 *out);
void lego_sqrtf_init(void);
void lego_rsqrtf_init(void);
void SetTrackLighting(void);
void InitCastleLinkList(void);
void Vec3Normalize(float *v);
float lego_rsqrtf(float x);
unsigned int GetLmsBoxScreenRect(struct LmsFile *param_1, unsigned int *param_2, unsigned int *param_3, struct RectI *param_4);
void ClipRectIToCastleView(struct RectI *dst, struct RectI *src);
float lego_sqrtf(float x);
float Vec3Dot(struct FVec3 *param_1, struct FVec3 *param_2);
void DrawDottedLine(void *a, void *b, short c);
void PlotClippedPoints(int *points, int count, short c);
void RemoveAllCoasterPieces(struct CastleState *castle);
void DrawSupportPost(struct FVec3 *pos, struct FVec3 *basis);
struct PathSeg;
unsigned int CountStraightSegsForward(struct PathSeg *n, struct PathSeg **out);
void DrawTrackTie(struct FVec3 *param_1, struct FVec3 *param_2);

struct Struct42a5e0Obj;
struct Struct42a5e0Data;
struct LmsFile;

unsigned int PlaceInEmptySeatOfCar(struct TrainCar *host, void *arg);
struct TrainSeat *FindEmptySeatInCar(struct TrainCar *host);
struct CastleVisitor *FindOccupantInCar(struct TrainCar *host);
struct TrainCar *AllocTrainCar(unsigned int arg);
void FreeTrainCar(void *param_1);
struct Element;
void AddMapObjectIfEnabled(struct Element *element, int *pos);

unsigned int FindPathSegDoorPoints(struct PathSeg *obj, struct PieceDoor *door, struct DirPoints *pts);
float GetTrainCarMass(struct TrainCar *obj);
float GetTrainCarPotential(struct TrainCar *obj);
void ResetTrainToStation(struct Train *obj);
void ClearTrainCarDrawn(struct TrainCar *obj);
void InitTrainCar(struct TrainCar *param, unsigned int value);
void AddTrainCarLink(struct TrainCar *obj);
void UnlinkTrainCar(struct TrainCar *param_1);
void UpdateTrainCarBounds(struct TrainCar *obj);
unsigned int DrawTrainCarOnPiece(struct TrainCar *a, struct PathSeg *b);
unsigned int IsAreaListClear(short *pos, struct AreaNode *area);
void DrawTrainCar(struct TrainCar *a);
void DrawTrainBogie(struct TrainBogie *p);
unsigned int DrawLmsModel(struct LmsFile *a, struct LmsRec *b, void *c, void *d, unsigned int e);
void BuildUprightBasis(struct FVec3 *a, struct FVec3 *b);
void InitTrainSeat(struct TrainSeat *obj, const struct FVec3 *src);
void TestNumericDerivatives(void);
unsigned int SetCurrentDirectoryIfNotNull(const char *param);
int LoadObjAndTxtFiles(const char *s);
void *ReadFileToBuffer(const char *fileName, unsigned int *bytesReadPtr);
unsigned int GetCoasterObjLineCount(void);
void GetCoasterObjLine(unsigned int param_1, char *param_2);
struct LmsFile *LoadLmsFile(const char *name);
struct LmsRec *LoadLfmFile(const char *name, unsigned int *param2);
unsigned int GetCoasterTxtLineCount(void);
void FormatRollercoasterFileName(int value, char *buffer);
void *LoadLtxFile(const char *name);
int *LoadRollercoasterLpt(void);
void SetClipVertexFormat(unsigned int param);
struct FloatArray;
int VecDerivative(void (*fn)(float, struct FloatArray *), struct VecMath *a, float b, float c, struct FloatArray *d);
int **ClipPolygonToPlanes(int n, int **verts, int *outCount, int nPlanes, float *planes);
void ExpTestCurve(float x, struct FloatArray *out);
void InitVecMathOps(void);
int ScalarDerivative(double (*fn)(float), float p2, float p3, float *out);
float **BuildDifferenceTable(double (*fn)(float), int n, float a, float b);
double LogFloat(float param);
void FreeDifferenceTable(void *param);
float GetTrainKineticEnergy(struct Train *param_1, float param_2);
void PositionTrainCarsAndBogies(void *obj, float val, void *tmp);
void EvalTrainPotentialAndCentres(float a, struct FloatArray *out);
void PositionTrainCars(void *obj, float val, void *b);
unsigned int GetTrainCarRearPos(struct TrainCar *obj, float *param);
void FindPathPosAtRangeBehind(float *center, float r, struct CurveWalker *src, float first_hi, float x, struct CurveWalker *dst, float *out);
float CurveArcLength(struct CurveWalker *a, unsigned int hi, struct CurveWalker *b, unsigned int lo, int idx, float c);
void GetTrainCarCenter(unsigned char *obj, unsigned int *out);
int HasStraightForClosingSlope(struct PieceClass *info, struct CastleState *owner, struct PathSeg **a, struct PathSeg **b);
struct PathSeg *FindReachableAnchor(struct PathSeg *n, int x);
void InterpolateCoasterHeights(struct PathSeg *seg, struct PathSeg *end);
void OnCoasterRingClosed(struct CastleState *owner);
struct VecOps;
void IntegratorStepRK4(struct Integrator *ar, float *p, float c);
void SetTrainWheelPos(struct TrainWheel *param_1, struct CurveWalker *param_2, unsigned int param_5);
void OnCoasterRingOpened(struct CastleState *owner);
void ResetCoasterHeightsBack(struct PathSeg *n);
void ResetCoasterHeightsForward(struct PathSeg *n);
int CheckTrackPiecePlacement(struct PieceClass *info, short *pos, struct PiecePlacement *ent);
int CheckAnchorPiecePlacement(struct PieceClass *info, short *pos, struct PiecePlacement *ent);
int MatchPieceToCoasterEnds(short *pos, struct PiecePlacement *ent, struct PieceClass *info);
void RemoveMapObjectIfEnabled(struct Element *element, TileId tile, struct Cursor *cursor);
void RemovePathSegMapObject(struct PathSeg *obj);
float GetTrainParamSpeed(struct Train *param);
void GetTrainInertiaAndSlope(struct Train *obj, float *a, float *b);
struct Ride;
struct PathSeg *FindPathSegAt(struct CastleState *host, struct Ride *ride, short *pt);
void GetTrainCarCenterAndAxis(struct TrainCar *obj, float *out);
void SetClipRect(int a, int b, int c, int d);
/* the clip planes (a x + b y + c >= 0 is inside) */
struct PlaneSet {
    int count;
    float p[4][3];
};
void SetClipPlanesFromRect(const int *r, struct PlaneSet *out);
float ParamSpeedToTrackSpeed(struct Train *param, float result);
float GetTrainTrackSpeed(struct Train *param);
void PositionTrainCarAndBogies(struct TrainCar *obj, struct CurveWalker *e, float t);
void GetWalkerTangentBelow(struct CurveWalker *obj, int a, unsigned int b, float c, struct FVec3 *out);
void InitTrainBogie(struct TrainBogie *o, struct CurveWalker *src, float param_5);
void PositionTrainCar(struct TrainCar *obj, struct CurveWalker *e, float t);
void InitIntegrator(struct Integrator *param, unsigned int count);
struct LmsFile *GetLmsByName(const char *s);
struct LmsRec *GetLfmByName(const char *s);
struct FloatArray **AllocVecScratch(unsigned int param_1);
void FreeVecScratch(struct FloatArray **param_1, unsigned int param_2);
void InitVecMath(struct VecMath *dest, unsigned int value);
unsigned int FindCoasterObjLine(const char *s);
unsigned int FindCoasterTxtLine(const char *param);
void SlopeCurveNormal(float *obj, float t, float *out);
void SetCoasterShadeTable(int param1, int index);
void BuildShadeRamp(int param1, char *entry);
void CastleObjLoad(struct Element *param_1);
void CastleObjCalcCursor(struct Element *param_1, int *param_2, unsigned int param_3);
void RemoveCastleCoaster(struct Element *param_1, TileId param_2, struct Cursor *param_3);
void RemoveCastleCoasterMapObjects(struct Int16Pair *pos);
void CastleDummyRemoveObject(void);
struct SegBlob;
void GetCastleStationStart(struct CastleState *p1, struct SegBlob **p2, float *p3, struct FVec3 *p4);
float CastleStepLeaving(struct Train *param_1, float param_2);
float CastleStepArriving(struct Train *self, float value);
void UpdateCastleTrain(struct CastleState *param_1);
void DestroyCastleTrain(struct CastleState *param_1);
void StopCastleTrain(struct CastleState *this);
void UnseatAllCastleVisitors(struct CastleState *list);
void FreeAllCastleVisitors(struct CastleState *esi);
void CastleRemoveAllRiders(void);
void InitCastleView(void);
void SetModelLighting(void);
void Mat4Identity(float m[4][4]);
void Mat4Multiply(float a[4][4], float b[4][4], float out[4][4]);
unsigned int ProjectCastlePoints(float in[][3], int out[][4], int n);
unsigned int ProjectPoints(float in[][3], int out[][4], float *m, unsigned int stride, int n);
unsigned int AnyCastleLinkVisible(void);
void AddCastleLink(struct ListLink *param_1);
void UnlinkListLink(struct ListLink *param_1);
unsigned int GetBoxScreenRect(void *ptr, unsigned int *a, unsigned int *b, struct RectI *c);
unsigned int CountCastleVisitors(struct CastleState *main);
unsigned int CountCoasterPieces(struct CastleState *arg);
void InitTrackDrawing(void);
void BuildTrackPieceCurves(void);
void InitRailDrawOrder(void);
void InitCurveVecMath(void);
void LoadCoasterTrainWheelModel(void);
void SetTrainSeatOccupant(struct TrainSeat *param_1, void *param_2);
struct CastleVisitor *TakeTrainSeatOccupant(struct TrainSeat *self);
void DrawTrackPiece(void *node);
void DrawTrackStretch(struct SegBlob *a, struct FVec3 *b, unsigned int c, unsigned int d);
void InitRailTubeMesh(void);
void InitSupportPostShape(void);
float CurveTangentLength(unsigned int b);
void InitTrackTieShape(void);
unsigned int IsStraightSegment(struct PieceClass *s, unsigned int v, unsigned int c);
unsigned int CountStraightSegsBack(struct PathSeg *n, struct PathSeg **out);
unsigned int GetTrainWheelPos(struct TrainWheel *ptr, unsigned int a, float *b);

void CastleGetInterfaces(struct Element *head, struct Ride *iface);
struct Element;
struct Struct427970Src;
struct Struct427bc0Src;
struct CastleRideObj;
int CastleObj_Load(struct Element *element);
int CastleObj_Save(struct Element *element);
void SquareTrackCalcCursor(struct Element *obj, int *x, unsigned int y);
struct PieceClass *FindPieceClassForElement(struct Element *element);
void TrackPieceSetEditMode(struct Element *param_1);
void CastleRemovePieceCalcCursor(struct Element *obj, const struct Struct427970Src *src);
void RemoveQueriedCoasterPiece(void);
void RenderSquareTrack(struct Element *unused0, unsigned int unused1, unsigned int unused2, const unsigned char *src);
void CastleLoadBasicTiles(struct Element *obj);
void CastleUnloadBasicTiles(void);
void SquareTrackPieceCalcCursor(struct Element *obj, int *x, unsigned int y);
void SquareTrackAddObject(struct Element *unused, const struct Struct427bc0Src *src);
void HeightTrackCalcCursor(struct Element *obj, int *x, unsigned int y);
void HeightTrackAddObject(struct Element *obj, const struct Struct427bc0Src *src);
void SquareTrackHeightLoad(struct Element *param_1);
void SquareTrackHeight0Load(struct Element *param_1);
void SquareTrackHeightPathLoad(struct Element *param_1);
void HeightTrackPathCalcCursor(struct Element *obj, int *x, unsigned int y);
void HeightTrackPathAddObject(struct Element *obj, const struct Struct427bc0Src *src);
void UpdateCastleView(void);

struct CastleSaveRecord *ReadSaveRec(void);
void LoadCastleTrain(struct TrainSaveRecord *src, struct CastleState *obj);
void LoadCastleVisitors(struct VisitorLoadRecord *arr, int count, struct CastleState *obj);
void MeasureCastleSave(struct CastleState *param1, struct CastleSaveInfo *param2);
void WriteCastleSave(struct CastleSaveInfo *src, struct CastleSaveRecord *dst);
void SaveCastleVisitors(struct CastleState *a1, struct VisitorSaveRecord *a2);
void WriteSaveRec(unsigned int *param_1);
void SaveCoasterPieces(struct CastleState *obj, struct PieceSaveRef *out);
void SaveCastleTrain(struct Train *src, struct TrainSaveRecord *dst);
struct BlokeInfo;
struct LmsRec *BuildCastleVisitorModel(struct BlokeInfo *p);

struct BoxFace {
    short f0;
    short plane;
    short t[3];
    short u[3];
};

typedef struct BoxFace BoxFace;

struct BoxSolid {
    int f0;
    int f4;
    int f8;
    struct FVec3 *verts;
    float (*normals)[3];
    struct FVec3 *xverts;
    struct BoxFace *faceA;
    int nFaceA;
    struct BoxFace *faceB;
    int nFaceB;
    unsigned char pad_28[0x90 - 0x28];
};

extern float BoxFaceNormals[6][3];
extern struct BoxFace BoxSolidFaces[12];
extern struct BoxFace BoxSolidOriginFaces[12];
extern struct BoxSolid BoxSolidTemplate;

void InitBoxSolidToOrigin(struct BoxSolid *box, struct FVec3 *verts, struct FVec3 *xverts, int unused, float w, float h, float d);
void InitBoxSolidCentred(struct BoxSolid *box, struct FVec3 *verts, struct FVec3 *xverts, int unused, float w, float h, float d);
void FillSpansFlat(void);
void FillSpansFlatZ(void);
void FillSpansGouraud(void);
void FillSpansGouraudZ(void);
void FillSpansTexturedZ(void);
extern void *FlatSpanFillers[2];
extern void *GouraudSpanFillers[2];
extern void *TexturedSpanFillers[2];
float StepTrainToSegmentEnd(struct Train *obj, float total);
float StepTrainLeaving(struct Train *self, float time_left);
float StepTrainArriving(struct Train *self, float time_left);
void *ClipTriangle(void *verts, int *out, unsigned int code, int n);
struct SegBlob *CurveWalkerNext(struct CurveWalker *cursor);
void DrawLmsFlatTriangles(void);
void DrawLmsGouraudTriangles(void);
void DrawLmsTexturedTriangles(void);
void RecordDepthSpans(int n, int x, struct RecIdx *idx, struct RecSrc *src);
struct RecBuf;
void ClearRecordedDepthSpans(struct RecBuf *rb);
struct ClearRect;
void ClearCoasterSurfaceRect(struct ClearRect *r);
struct VideoArg;
int GetCoasterSurface(struct VideoArg *arg);
void ReleaseCoasterSurface(void);
void TransformPoints3x4(float *in, float *out, float m[4][4], int n);
void MakeMat4FromRotTrans(unsigned int *t, unsigned int *m3, unsigned int dst[4][4]);
struct Mat4x4;
void Mat3ToMat4Transpose(unsigned int *m3, struct Mat4x4 *out);
void BuildBasisFromDir(struct FVec3 *dir, struct FVec3 *basis);
unsigned int LowestSetBitIndex(unsigned int param_1);
void RasteriseTriangle(void);
void *GetLtxFileTableEntry(unsigned int param);
