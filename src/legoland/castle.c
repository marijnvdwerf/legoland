/* castle.c: the Castle roller coaster (the "CASTLE OBJ" ride and its SQUARE_TRACK* pieces).
 *
 * The player places the castle, then lays track pieces (2x2 cells each) from its two doors: plain track (turns and
 * straights) and height blocks (fixed-height anchors, one of them also puts path under itself). CastleCoaster holds
 * the pieces as an open chain (head..castle..tail) until both ends meet, which closes the ring. Heights between
 * anchors are interpolated: every straight run becomes one slope stretch, turns stay level.
 *
 * Every piece's track is a list of curve stretches (lines, slopes, quarter arcs; CurveFunctionTable holds their
 * methods). The train (three cars, two seats each) runs on them by energy conservation: its state [curve parameter,
 * total energy] is stepped with RK4, speed = sqrt(2 (E - PE) / inertia), both terms numeric derivatives of the
 * cars' potential energy and centres; the rear wheels sit where the track is one car length behind the front ones
 * (bisection). The castle's own track (CastleCurves) holds the station: the train starts on its last stretch and is
 * braked to a stop there when it comes back. Visitors queue (CastleVisitor), board when someone waited 5 s or more
 * than 3 wait, and leave after the ride.
 *
 * Everything is drawn by the castle's own software 3D renderer into the locked 16-bit screen: models (.lms meshes,
 * .lfm materials, .ltx textures, Rollercoaster.lpt palette with 64-step shade ramps) are projected with an
 * isometric matrix matching the game map, clipped, and scan-converted by hand-written span fillers (flat, Gouraud,
 * textured; depth-tested twins use a 640x480 16-bit depth buffer cleared span by span). Track is drawn as hexagonal
 * rail tubes with sleepers and support posts. lego_sqrtf / lego_rsqrtf are table-driven sqrt and 1/sqrt. Many
 * functions time themselves with rdtsc into profiling counters (RecordCastleFrameCounters). */

#include <windows.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include <float.h>
#include <math.h>
#include "bloke.h"
#include "castle.h"
#include "debug_alloc.h"
#include "draw.h"
#include "gamemap.h"
#include "interface.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "obj_instance.h"
#include "objclass.h"
#include "path_control.h"
#include "print_sprite.h"
#include "render3d.h"
#include "screens.h"
#include "tilemap.h"
#include "timer.h"

struct ClearRect {
    int left;
    int top;
    int right;
    int bottom;
};

struct EdgeVert {
    int f[5];
};

struct EdgePair {
    int a;
    int b;
};

struct EdgeObj {
    unsigned short color;
    unsigned short pad_2;
    int npts;
    int nedges;
    struct EdgePair edges[12];
    float pts[8][3];
};

struct EdgeSet {
    unsigned char pad_0[8];
    int count;
    unsigned char pad_c[4];
    struct EdgeVert *base;
    struct EdgePair *pairs;
};

#include "image_sprite.h"
/* Returns the opposite of a direction bit (N 1 <-> S 4, E 2 <-> W 8). An unset end (0xffffffff) gives N. */
// FUNCTION: LEGOLAND 0x0041cc50
unsigned int GetOppositeDirection(unsigned int dir) {
    if (dir == 1) return 4;
    if (dir == 4) return 1;
    if (dir == 2) return 8;
    if (dir == 8) return 2;
    if (dir == 0xffffffff) return 1;
    return 0;
}

/* Returns 1 << index: direction index 0..3 to direction bit. */
// FUNCTION: LEGOLAND 0x0041cc90
unsigned int BitFromIndex(unsigned int shift) {
    return 1U << shift;
}

/* Direction bit (1, 2, 4, 8) to its index 0..3. */
// FUNCTION: LEGOLAND 0x0041cca0
unsigned int DirMaskToIndex(unsigned int dir) {
    if (dir == 1) return 0;
    if (dir == 4) return 2;
    if (dir == 2) return 1;
    if (dir == 8) return 3;
    return 0;
}

/* The ride's footprint rectangle moved to map cell pt: out = {x0, y0, x1, y1}. */
// FUNCTION: LEGOLAND 0x0041cce0
void GetFootprintRectAt(short *pt, struct Ride *ride, int *out) {
    int *rect = ride->footprint.v;
    out[0] = pt[0] + rect[0];
    out[2] = pt[0] + rect[2];
    out[1] = pt[1] + rect[1];
    out[3] = pt[1] + rect[3];
}

/* IsAreaListClear on the area list at pos. */
// FUNCTION: LEGOLAND 0x0041cd20
unsigned int IsAreaListClearAt(short *a, struct AreaNode *b) {
    return IsAreaListClear(a, b);
}

/* Index (0..3) of the DirPoints entry at cell *val whose direction bit is set in rec[0], or -1. */
// FUNCTION: LEGOLAND 0x0041cd40
int FindDoorPointIndex(unsigned int *val, unsigned int *rec) {
    int i;
    unsigned int bit = 1;

    for (i = 0; i <= 3; i++) {
        if ((bit & rec[0]) != 0 && *val == rec[i + 1]) {
            return i;
        }
        bit <<= 1;
    }
    return -1;
}

/* Fills out with the cells next to pos (two map cells away, PieceNeighbourOffsets) in each direction of mask, and
 * sets the direction's bit in out->mask when that 2x2 area is free. */
// FUNCTION: LEGOLAND 0x0041cd80
void FindOpenDoorPoints(short *pos, struct DirPoints *out, unsigned int mask) {
    int i;
    unsigned int bit = 1;

    out->mask = 0;
    for (i = 0; i * 4 + 4 <= 16; i++) {
        if ((mask & bit) != 0) {
            out->pt[i][0] = pos[0] + PieceNeighbourOffsets[i][0];
            out->pt[i][1] = pos[1] + PieceNeighbourOffsets[i][1];
            if (IsAreaListClearAt(&out->pt[i][0], &DoorAreaList) == 1) {
                out->mask |= bit;
            }
        }
        bit <<= 1;
    }
}

/* Clears a piece end: no neighbour, direction unset, height 0. */
// FUNCTION: LEGOLAND 0x0041ce10
void ResetPieceEnd(struct PieceEnd *anim) {
    anim->link = 0;
    anim->dir = 0xffffffff;
    anim->height = 0;
}

/* Clears a piece header: flags, position, class, owner and both ends. */
// FUNCTION: LEGOLAND 0x0041ce30
void InitPieceHead(struct PieceHead *pair) {
    pair->flags = 0;
    pair->x = 0;
    pair->y = 0;
    pair->cls = 0;
    pair->owner = 0;
    ResetPieceEnd(&pair->end_b);
    ResetPieceEnd(&pair->end_a);
}

/* Index 0..15 of the piece's (prev direction, next direction) pair, prev in bits 0-1 and next in bits 2-3; an unset
 * end counts as straight through. Selects the piece's curve shape (GetTrackPieceCurveIndex, DrawTrackPiece). */
// FUNCTION: LEGOLAND 0x0041ce60
unsigned int GetPieceHeadDirPairIndex(struct PieceHead *pair) {
    unsigned int a = pair->end_a.dir;
    unsigned int lo;
    unsigned int b;

    if (a == 0xffffffff) {
        a = GetOppositeDirection(pair->end_b.dir);
    }
    lo = DirMaskToIndex(a);
    b = pair->end_b.dir;
    if (b == 0xffffffff) {
        b = GetOppositeDirection(pair->end_a.dir);
    }
    return (DirMaskToIndex(b) << 2) | lo;
}

/* Puts the piece's element on the map at the piece's cell (if map objects are enabled). */
// FUNCTION: LEGOLAND 0x0041ceb0
void AddPathSegMapObject(struct PathSeg *obj) {
    int pt[2];

    pt[0] = obj->x;
    pt[1] = obj->y;
    AddMapObjectIfEnabled(obj->ride->element, pt);
}

/* Number of pieces from node back along prev until a piece with flags & 6 (or the chain end). */
// FUNCTION: LEGOLAND 0x0041cee0
unsigned int CountPrevSegs(struct PathSeg *node) {
    unsigned int count = 0;
    while (node != NULL) {
        if (node->flags & 0x6) break;
        node = node->prev;
        count++;
    }
    return count;
}

/* Number of pieces from node forward along next until a piece with flags & 6 (or the chain end). */
// FUNCTION: LEGOLAND 0x0041cf00
unsigned int CountNextSegs(struct PathSeg *node) {
    unsigned int count = 0;
    while (node != NULL) {
        if (node->flags & 0x6) break;
        node = node->next;
        count++;
    }
    return count;
}

/* Counts the pieces before (into out_a) and after (into out_b) src; 0xffffffff where there is no neighbour. */
// FUNCTION: LEGOLAND 0x0041cf20
void CountPathSegsAround(struct PathSeg *src, unsigned int *out_b, unsigned int *out_a) {
    *out_a = 0xffffffff;
    *out_b = 0xffffffff;
    if (src->prev != NULL) {
        *out_a = CountPrevSegs(src->prev);
    }
    if (src->next != NULL) {
        *out_b = CountNextSegs(src->next);
    }
}

/* For an open chain: pieces after the head (out_b) and before the tail (out_a); both 0 for a ring. */
// FUNCTION: LEGOLAND 0x0041cf70
void CountCastleCoasterSegs(struct CastleState *src, int *out_a, int *out_b) {
    *out_a = 0;
    *out_b = 0;
    if (src->flags == 2) return;
    *out_b = CountNextSegs(src->head);
    *out_a = CountPrevSegs(src->tail);
}

/* Calls the piece's fill_open_end method (makes an end that has no neighbour straight-through). */
// FUNCTION: LEGOLAND 0x0041cfc0
void PieceFillOpenEnd(struct PathSeg *host) {
    host->cls->fill_open_end(host);
}

/* Calls the piece's set_heights method with the height of the neighbour it was just attached to. */
// FUNCTION: LEGOLAND 0x0041cfd0
unsigned int PieceSetHeights(struct PathSeg *host, float arg) {
    return host->cls->set_heights(host, arg);
}

/* Returns the curve stretch for the piece and its world position in *pos: stored in the piece when flags & 1, else
 * the cell position at the piece's prev-end height and the class's get_curve. */
// FUNCTION: LEGOLAND 0x0041cff0
struct SegBlob *GetPieceCurve(struct PathSeg *obj, struct FVec3 *pos) {
    if (obj->flags & 1) {
        *pos = obj->vec;
        return &obj->blob;
    }
    MapPosToWorld(&obj->pos, obj->f18, pos);
    return obj->cls->get_curve(obj);
}

/* Finds the piece at map cell *key (x and y packed as one dword): around the ring when closed, else from the head
 * along next. */
// FUNCTION: LEGOLAND 0x0041d060
struct PathSeg *FindPathSegByXY(struct CastleState *host, unsigned int *key) {
    if (host->flags & 0x2) {
        struct PathSeg *current = (struct PathSeg *)&host->piece;
        unsigned int search = *key;
        if (current->xy == search) {
            return current;
        }
        while (1) {
            current = current->next;
            if (current == (struct PathSeg *)&host->piece) {
                break;
            }
            if (current->xy == search) {
                return current;
            }
        }
    } else {
        struct PathSeg *current = host->head;
        unsigned int search = *key;
        if (current->xy == search) {
            return current;
        }
        while (1) {
            current = current->next;
            if (current == NULL) {
                break;
            }
            if (current->xy == search) {
                return current;
            }
        }
    }
    return NULL;
}

/* 1 when map cell pt lies inside the piece's footprint. */
// FUNCTION: LEGOLAND 0x0041d0b0
int IsPointInPathSegFootprint(struct PathSeg *obj, short *pt) {
    int r[5];

    GetFootprintRectAt(&obj->x, obj->ride, r);
    if (pt[0] >= r[0] && pt[0] <= r[2] && pt[1] >= r[1] && pt[1] <= r[3]) {
        return 1;
    }
    return 0;
}

/* Finds the piece of the given ride type whose footprint contains map cell pt (ring or head to tail). */
// FUNCTION: LEGOLAND 0x0041d100
struct PathSeg *FindPathSegAt(struct CastleState *host, struct Ride *ride, short *pt) {
    struct PathSeg *node;

    if (host->flags & 2) {
        node = (struct PathSeg *)&host->piece;
        do {
            if (node->ride == ride && IsPointInPathSegFootprint(node, pt)) {
                return node;
            }
            node = node->next;
        } while (node != (struct PathSeg *)&host->piece);
        return 0;
    }
    node = host->head;
    do {
        if (node->ride == ride && IsPointInPathSegFootprint(node, pt)) {
            return node;
        }
        node = node->next;
    } while (node);
    return 0;
}

/* The free cells where a piece can follow obj (its next door). */
// FUNCTION: LEGOLAND 0x0041d170
unsigned int FindPathSegDoorAPoints(struct PathSeg *obj, struct DirPoints *pts) {
    return FindPathSegDoorPoints(obj, &obj->cls->next_door, pts);
}

/* The free cells where a piece can precede obj (its prev door). */
// FUNCTION: LEGOLAND 0x0041d190
unsigned int FindPathSegDoorBPoints(struct PathSeg *obj, struct DirPoints *pts) {
    return FindPathSegDoorPoints(obj, &obj->cls->prev_door, pts);
}

/* Empties a DirPoints set. */
// FUNCTION: LEGOLAND 0x0041d1b0
void ClearDirPoints(struct DirPoints *pts) {
    pts->mask = 0;
}

/* Empty in the original (does nothing). */
/* Empty in the release build; called with both free-cell sets after every attach and removal (probably a stripped
 * debug display). */
// FUNCTION: LEGOLAND 0x0041d1c0
void Empty_0041d1c0(void *unused) {
}

/* Fills pts with the free cells around the given door of the piece. */
// FUNCTION: LEGOLAND 0x0041d1d0
unsigned int FindPathSegDoorPoints(struct PathSeg *obj, struct PieceDoor *door, struct DirPoints *pts) {
    short sum[2];

    ClearDirPoints(pts);
    sum[0] = obj->x + door->x;
    sum[1] = obj->y + door->y;
    FindOpenDoorPoints(sum, pts, door->mask);
}

/* Checks a piece of class info placed at pos against both open ends of CastleCoaster: next_spot = the tail's free
 * cell its prev door sits on, prev_spot = the head's free cell its next door sits on (-1 if none), and points the
 * tail/head end at it. Returns 1 if it connects to either end. Never matches a closed ring. */
// FUNCTION: LEGOLAND 0x0041d210
int MatchPieceToCoasterEnds(short *pos, struct PiecePlacement *ent, struct PieceClass *info) {
    short p[2];
    int r;

    if (CastleCoaster.flags != 2) {
        ent->owner = (struct CastleState *)&CastleCoaster;
        p[0] = info->prev_door.x + pos[0];
        p[1] = info->prev_door.y + pos[1];
        r = FindDoorPointIndex((unsigned int *)p, &CastleCoaster.next_spots.mask);
        ent->next_spot = r;
        if (r != -1) {
            CastleCoaster.tail->dir_b = BitFromIndex(r);
        }
        p[0] = info->next_door.x + pos[0];
        p[1] = info->next_door.y + pos[1];
        r = FindDoorPointIndex((unsigned int *)p, &CastleCoaster.prev_spots.mask);
        ent->prev_spot = r;
        if (r != -1) {
            CastleCoaster.head->dir_a = BitFromIndex(r);
        }
        if (ent->next_spot != -1 || ent->prev_spot != -1) {
            return 1;
        }
    }
    return 0;
}

/* Placement test for a fixed-height piece (class flags & 1): on each end it connects to, the previous anchor must
 * be reachable, i.e. a straight run in between to slope over or the same height (FindReachableAnchor). Stores those
 * anchors in ent. */
// FUNCTION: LEGOLAND 0x0041d2e0
int CheckAnchorPiecePlacement(struct PieceClass *info, short *pos, struct PiecePlacement *ent) {
    if (MatchPieceToCoasterEnds(pos, ent, info)) {
        if (ent->next_spot != -1) {
            ent->tail_anchor = FindReachableAnchor(ent->owner->tail, info->f8);
            if (!ent->tail_anchor) {
                return 0;
            }
        }
        if (ent->prev_spot != -1) {
            ent->head_anchor = FindReachableAnchor(ent->owner->head, info->f4);
            if (!ent->head_anchor) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

/* Placement test for a plain track piece: connecting to one end is always allowed; closing the ring needs at least
 * one straight piece between the two anchors to absorb the height difference. */
// FUNCTION: LEGOLAND 0x0041d350
int CheckTrackPiecePlacement(struct PieceClass *info, short *pos, struct PiecePlacement *ent) {
    if (MatchPieceToCoasterEnds(pos, ent, info)) {
        if (ent->next_spot == -1 || ent->prev_spot == -1) {
            return 1;
        }
        return HasStraightForClosingSlope(info, ent->owner, &ent->tail_anchor, &ent->head_anchor) != 0;
    }
    return 0;
}

/* Runs the placement test for a piece of class cls at pos and returns CoasterPlacement with joins = 1 (joins after
 * the tail) | 2 (joins before the head), or 0 if it cannot be placed there. */
// FUNCTION: LEGOLAND 0x0041d3b0
struct PiecePlacement *CheckCoasterPiecePlacement(struct PieceClass *cls, struct Int16Pair *pos) {
    int r;

    if (cls->flags & 1) {
        r = CheckAnchorPiecePlacement(cls, &pos->x, &CoasterPlacement);
    } else {
        r = CheckTrackPiecePlacement(cls, &pos->x, &CoasterPlacement);
    }
    CoasterPlacement.joins = 0;
    if (r) {
        if (CoasterPlacement.next_spot != -1) {
            CoasterPlacement.joins = 1;
        }
        if (CoasterPlacement.prev_spot != -1) {
            CoasterPlacement.joins |= 2;
        }
        return &CoasterPlacement;
    }
    CoasterPlacement.joins = 0;
    return &CoasterPlacement;
}

/* Makes b follow a (a->next = b, b->prev = a). */
// FUNCTION: LEGOLAND 0x0041d430
void LinkPathSegs(struct PathSeg *a, struct PathSeg *b) {
    a->next = b;
    b->prev = a;
}

/* Links a new piece into the castle's track at the end(s) found by MatchPieceToCoasterEnds, sets the joined ends'
 * directions and heights, updates tail/head and their free cells, closes the ring (flags 2) when both ends join,
 * puts the element on the map and stores the piece's footprint relative to the castle. */
// FUNCTION: LEGOLAND 0x0041d440
void AttachPathSegToCoaster(struct PathSeg *obj, struct PiecePlacement *ent) {
    struct CastleState *owner = ent->owner;
    int *r;

    obj->owner = owner;
    if (ent->next_spot != -1) {
        LinkPathSegs(owner->tail, obj);
        owner->tail->dir_b = BitFromIndex(ent->next_spot);
        obj->dir_a = GetOppositeDirection(owner->tail->dir_b);
        PieceSetHeights(obj, owner->tail->f24);
        owner->tail = obj;
        FindPathSegDoorAPoints(obj, &owner->next_spots);
    }
    if (ent->prev_spot != -1) {
        LinkPathSegs(obj, owner->head);
        owner->head->dir_a = BitFromIndex(ent->prev_spot);
        obj->dir_b = GetOppositeDirection(owner->head->dir_a);
        PieceSetHeights(obj, owner->head->f18);
        owner->head = obj;
        FindPathSegDoorBPoints(obj, &owner->prev_spots);
    }
    if (ent->next_spot != -1 && ent->prev_spot != -1) {
        owner->flags = 2;
        OnCoasterRingClosed(owner);
    } else {
        PieceFillOpenEnd((struct PathSeg *)obj);
        Empty_0041d1c0(&owner->next_spots);
        Empty_0041d1c0(&owner->prev_spots);
    }
    AddPathSegMapObject(obj);
    r = obj->ride->footprint.v;
    obj->fp.v[0] = r[0] - owner->piece.x + obj->x;
    obj->fp.v[2] = r[2] - owner->piece.x + obj->x;
    obj->fp.v[1] = r[1] - owner->piece.y + obj->y;
    obj->fp.v[3] = r[3] - owner->piece.y + obj->y;
    obj->fp.next = 0;
}

/* Allocates and links a plain track piece; when it closes the ring, the heights between the two anchors are
 * interpolated along the straight pieces. */
// FUNCTION: LEGOLAND 0x0041d5b0
struct PathSeg *NewTrackPiece(struct Ride *ride, struct PieceClass *cls, struct Int16Pair *pos, struct PiecePlacement *ent) {
    struct PathSeg *obj = MemAlloc(sizeof(struct PathSeg), 0, 0, 0);

    if (obj == NULL) {
        return 0;
    }
    InitPieceHead((struct PieceHead *)obj);
    obj->ride = ride;
    obj->owner = ent->owner;
    obj->pos = *pos;
    obj->cls = cls;
    AttachPathSegToCoaster(obj, ent);
    if (ent->next_spot != -1 && ent->prev_spot != -1) {
        InterpolateCoasterHeights(ent->tail_anchor, ent->head_anchor);
    }
    obj->cls->on_add(obj);
    return obj;
}

/* Allocates and links a fixed-height piece; the heights between it and the anchor before/after it are interpolated
 * along the straight pieces. */
// FUNCTION: LEGOLAND 0x0041d630
struct PathSeg *NewAnchorPiece(struct Ride *ride, struct PieceClass *cls, struct Int16Pair *pos, struct PiecePlacement *ent) {
    struct PathSeg *obj = MemAlloc(sizeof(struct PathSeg), 0, 0, 0);

    if (obj == NULL) {
        return 0;
    }
    InitPieceHead((struct PieceHead *)obj);
    obj->ride = ride;
    obj->owner = ent->owner;
    obj->pos = *pos;
    obj->cls = cls;
    AttachPathSegToCoaster(obj, ent);
    if (ent->next_spot != -1) {
        InterpolateCoasterHeights(ent->tail_anchor, obj);
    }
    if (ent->prev_spot != -1) {
        InterpolateCoasterHeights(obj, ent->head_anchor);
    }
    obj->cls->on_add(obj);
    return obj;
}

/* Sets CoasterPieceCount. */
// FUNCTION: LEGOLAND 0x0041d6c0
void SetCoasterPieceCount(unsigned int value) {
    CoasterPieceCount = value;
}

/* Adds delta to CoasterPieceCount and returns the new count. */
// FUNCTION: LEGOLAND 0x0041d6d0
unsigned int AddCoasterPieceCount(unsigned int delta) {
    CoasterPieceCount += delta;
    return CoasterPieceCount;
}

/* Returns CoasterPieceCount. */
// FUNCTION: LEGOLAND 0x0041d6f0
unsigned int GetCoasterPieceCount(void) {
    return CoasterPieceCount;
}

/* Places a piece of class cls at pos if it connects to the track (NewAnchorPiece for fixed-height classes, else
 * NewTrackPiece); NULL otherwise. */
// FUNCTION: LEGOLAND 0x0041d700
struct PathSeg *AddCoasterPiece(struct Ride *ride, struct PieceClass *cls, struct Int16Pair *pos) {
    struct PiecePlacement *result = CheckCoasterPiecePlacement(cls, pos);
    if (result->joins != 0) {
        AddCoasterPieceCount(1);
        if ((cls->flags & 1) != 0) {
            return NewAnchorPiece(ride, cls, pos, &CoasterPlacement);
        } else {
            return NewTrackPiece(ride, cls, pos, &CoasterPlacement);
        }
    }
    return 0;
}

/* Removes the piece's element from the map at the piece's cell. */
// FUNCTION: LEGOLAND 0x0041d760
void RemovePathSegMapObject(struct PathSeg *obj) {
    TileId tile;
    struct Point pt;

    tile.pos.x = (unsigned char)obj->x;
    pt.x = obj->x;
    tile.pos.y = (unsigned char)obj->y;
    pt.y = obj->y;
    BasicObjectDCalcCursor(obj->ride->element, &pt);
    RemoveMapObjectIfEnabled(obj->ride->element, tile, &QueryCursor);
}

/* 1 if the piece may be removed: the track is a closed ring, or it is the tail or the head. */
// FUNCTION: LEGOLAND 0x0041d7c0
unsigned int IsCoasterEndPiece(struct PathSeg *value) {
    if (CastleCoaster.flags != 2) {
        if (value != CastleCoaster.tail) {
            if (value != CastleCoaster.head) {
                return 0;
            }
        }
    }
    return 1;
}

/* Removes a piece: off the map, out of the chain (opening the ring if it was closed, else moving the tail or head
 * back), resets the heights of the straight run behind/ahead of the new end(s), makes the new end(s)
 * straight-through, runs the class's on_remove and frees it. */
// FUNCTION: LEGOLAND 0x0041d7f0
void RemoveCoasterPiece(void *arg) {
    struct PathSeg *obj = arg;
    struct CastleState *owner = obj->owner;

    if (obj != NULL) {
        RemovePathSegMapObject(obj);
        AddCoasterPieceCount(-1);
        if (owner->flags == 2) {
            ResetPieceEnd((struct PieceEnd *)&obj->next->dir_a);
            ResetPieceEnd((struct PieceEnd *)&obj->prev->dir_b);
            owner->tail = obj->prev;
            FindPathSegDoorAPoints(obj->prev, &CastleCoaster.next_spots);
            owner->head = obj->next;
            FindPathSegDoorBPoints(obj->next, &CastleCoaster.prev_spots);
            ResetCoasterHeightsBack((struct PathSeg *)owner->tail);
            ResetCoasterHeightsForward((struct PathSeg *)owner->head);
            PieceFillOpenEnd((struct PathSeg *)owner->tail);
            PieceFillOpenEnd((struct PathSeg *)owner->head);
            owner->flags = 1;
            OnCoasterRingOpened(owner);
        } else if (obj == owner->tail) {
            ResetPieceEnd((struct PieceEnd *)&obj->prev->dir_b);
            owner->tail = obj->prev;
            FindPathSegDoorAPoints(obj->prev, &CastleCoaster.next_spots);
            ResetCoasterHeightsBack((struct PathSeg *)owner->tail);
            PieceFillOpenEnd((struct PathSeg *)owner->tail);
        } else {
            ResetPieceEnd((struct PieceEnd *)&obj->next->dir_a);
            owner->head = obj->next;
            FindPathSegDoorBPoints(obj->next, &CastleCoaster.prev_spots);
            ResetCoasterHeightsForward((struct PathSeg *)owner->head);
            PieceFillOpenEnd((struct PathSeg *)owner->head);
        }
        Empty_0041d1c0(&CastleCoaster.next_spots.mask);
        Empty_0041d1c0(&CastleCoaster.prev_spots.mask);
        ((struct PieceClass *)obj->cls)->on_remove(obj);
        MemFree(obj);
    }
}

/* Puts the train at curve parameter val on the walker's stretch: the head car there, every following car 30 units
 * (to 4.8 along the curve) behind the previous car's rear wheels, and resets all bogies. */
// FUNCTION: LEGOLAND 0x0041d950
void PositionTrainCarsAndBogies(void *obj, float val, void *b) {
    struct Train *host = obj;
    struct TrainCar *head;
    struct TrainCar *cur;
    struct TrainCar *next;
    struct CurveWalker elem;
    struct CurveWalker out;
    unsigned int tmp[3];
    float t;
    float f;

    next = host->head.next;
    head = &host->head;
    cur = head;
    host->motion.pos = val;
    host->walker = *(struct CurveWalker *)b;
    PositionTrainCarAndBogies(head, (struct CurveWalker *)b, val);
    while (next != head) {
        t = cur->rear.w0.t;
        elem = cur->rear.w0.walker;
        GetTrainCarRearPos(cur, (float *)tmp);
        FindPathPosAtRangeBehind((float *)tmp, 30.0f, &elem, t, 4.8f, &out, &f);
        PositionTrainCarAndBogies(next, &out, f);
        cur = next;
        next = next->next;
    }
}

/* Same as PositionTrainCarsAndBogies without resetting the bogies (used while evaluating the physics). */
// FUNCTION: LEGOLAND 0x0041da10
void PositionTrainCars(void *obj, float val, void *b) {
    struct Train *host = obj;
    struct TrainCar *head;
    struct TrainCar *cur;
    struct TrainCar *next;
    struct CurveWalker elem;
    struct CurveWalker out;
    unsigned int tmp[3];
    float t;
    float f;

    next = host->head.next;
    head = &host->head;
    cur = head;
    host->motion.pos = val;
    host->walker = *(struct CurveWalker *)b;
    PositionTrainCar(head, (struct CurveWalker *)b, val);
    while (next != head) {
        t = cur->rear.w0.t;
        elem = cur->rear.w0.walker;
        GetTrainCarRearPos(cur, (float *)tmp);
        FindPathPosAtRangeBehind((float *)tmp, 30.0f, &elem, t, 4.8f, &out, &f);
        PositionTrainCar(next, &out, f);
        cur = next;
        next = next->next;
    }
}

/* Sets the train's total energy. */
// FUNCTION: LEGOLAND 0x0041dad0
void SetTrainEnergy(struct Train *holder, float value) {
    holder->motion.energy = value;
}

/* Sum of the cars' potential energies. */
// FUNCTION: LEGOLAND 0x0041dae0
float SumTrainPotential(struct Train *obj) {
    float sum = 0.0f;
    struct TrainCar *head = &obj->head;
    struct TrainCar *cur = head;

    do {
        sum += GetTrainCarPotential(cur);
        cur = cur->next;
    } while (cur != head);
    return sum;
}

/* VecDerivative callback: positions DerivTrain at curve parameter a (from DerivTrainWalker) and returns [total
 * potential energy, centre of car 1 (x, y, z), centre of car 2, ...]. */
// FUNCTION: LEGOLAND 0x0041db20
void EvalTrainPotentialAndCentres(float a, struct FloatArray *out) {
    struct TrainCar *node;
    float *p;
    unsigned int n;

    n = 1;
    node = &DerivTrain->head;
    PositionTrainCars(DerivTrain, a, &DerivTrainWalker);
    out->data[0] = 0.0f;
    p = &out->data[1];
    do {
        out->data[0] += GetTrainCarPotential(node);
        GetTrainCarCenter((unsigned char *)node, (unsigned int *)p);
        node = node->next;
        n += 3;
        p += 3;
    } while (node != &DerivTrain->head);
    out->count = n;
}

/* Numerically differentiates the train's potential energy and car centres with respect to the curve parameter at
 * its current position: *b = dPE/ds, *a = effective inertia sum(mass * |d centre/ds|^2) (scaled). Restores the
 * train and logs *a in TrainInertiaHistory. */
// FUNCTION: LEGOLAND 0x0041db90
void GetTrainInertiaAndSlope(struct Train *obj, float *a, float *b) {
    struct TrainCar *node;
    struct CurveWalker saved;
    float savedval;
    struct FloatArray out;
    float *p;
    float sum;
    volatile float x; /* volatile: the original re-reads this from its stack slot */
    int k;

    saved = obj->walker;
    DerivTrain = obj;
    savedval = obj->motion.pos;
    DerivTrainWalker = obj->walker;
    VecDerivative(EvalTrainPotentialAndCentres, &TrainVecMath, savedval, 0.1f, &out);
    *b = out.data[0];
    *a = 0.0f;
    p = &out.data[1];
    node = &obj->head;
    do {
        sum = FLOAT_004ab390;
        for (k = 0; k < 3; k++) {
            x = *p++;
            sum += x * x;
        }
        *a += GetTrainCarMass(node) * sum * 2.5201562e-06f;
        node = node->next;
    } while (node != &obj->head);
    obj->walker = saved;
    obj->motion.pos = savedval;
    TrainInertiaHistory[TrainInertiaHistoryIndex & 0x3f] = *a;
    TrainInertiaHistoryIndex++;
}

/* Speed along the curve parameter from energy conservation: sqrt(2 * (energy - potential) / inertia), 0 if the
 * train has not enough energy. */
// FUNCTION: LEGOLAND 0x0041dca0
float GetTrainParamSpeed(struct Train *param) {
    float a;
    float b;
    float t;
    float r;

    t = SumTrainPotential(param);
    GetTrainInertiaAndSlope(param, &a, &b);
    r = (param->motion.energy - t) * 2.0f / a;
    if (r > FLOAT_004ab390) {
        return (float)sqrt(r);
    }
    return FLOAT_004ab390;
}

/* Curve-parameter speed to speed along the track: times the length of the centre line's tangent at the train's
 * position, times 0.0015875 (a unit scale). */
// FUNCTION: LEGOLAND 0x0041dd00
float ParamSpeedToTrackSpeed(struct Train *param, float result) {
    struct FVec3 v;
    float t;

    GetWalkerTangentBelow(&param->walker, 1, *(unsigned int *)&param->motion.pos, 0.0f, &v);
    t = lego_sqrtf(Vec3Dot(&v, &v));
    t = t * result;
    return t * FLOAT_004ab404;
}

/* The train's speed along the track (GetTrainParamSpeed times the tangent length and unit scale). */
// FUNCTION: LEGOLAND 0x0041dd50
float GetTrainTrackSpeed(struct Train *param) {
    float result = GetTrainParamSpeed(param);
    return ParamSpeedToTrackSpeed(param, result);
}

/* Sum of the cars' masses. */
// FUNCTION: LEGOLAND 0x0041dd70
float SumTrainCarMasses(struct Train *obj) {
    float sum = 0.0f;
    struct TrainCar *head = &obj->head;
    struct TrainCar *cur = head;

    do {
        sum += GetTrainCarMass(cur);
        cur = cur->next;
    } while (cur != head);
    return sum;
}

/* Kinetic energy 0.5 * total mass * speed^2. */
// FUNCTION: LEGOLAND 0x0041ddb0
float GetTrainKineticEnergy(struct Train *param_1, float param_2) {
    float result = SumTrainCarMasses(param_1);
    result = result * param_2;
    result = result * param_2;
    result = result * 0.5f;
    return result;
}

/* The train integrator's store: puts the cars at state[0] and sets the energy to state[1]. */
// FUNCTION: LEGOLAND 0x0041ddd0
void TrainIntegratorStore(struct Integrator *a, struct FloatArray *b) {
    struct Train *e = a->owner;

    e->motion.count = 2;
    PositionTrainCars(e, b->data[0], &e->walker);
    SetTrainEnergy(e, b->data[1]);
}

/* The train integrator's derivative: [lego_sqrtf(2 * (energy - potential) / inertia), 0]; when the train is
 * about to stall (track speed < 0.05) the energy instead grows by dPE/ds, which keeps it climbing. */
// FUNCTION: LEGOLAND 0x0041de10
void TrainIntegratorDeriv(struct Integrator *obj, float unused, struct FloatArray *out) {
    struct Train *e = obj->owner;
    float a;
    float b;
    float t;

    out->count = 2;
    t = SumTrainPotential(e);
    GetTrainInertiaAndSlope(e, &a, &b);
    t = (e->motion.energy - t) * 2.0f / a;
    if (t > FLOAT_004ab390) {
        t = lego_sqrtf(t);
    } else {
        t = FLOAT_004ab390;
    }
    out->data[0] = t;
    out->data[1] = 0.0f;
    if (b > FLT_MIN) {
        if (ParamSpeedToTrackSpeed(e, t) < 0.05) {
            out->data[1] = b;
        }
    }
}

/* Sets up the train's 2-value integrator (state = motion: curve parameter and energy). */
// FUNCTION: LEGOLAND 0x0041dec0
void InitTrainIntegrator(struct Train *self) {
    InitIntegrator(&self->integ, 2);
    self->integ.store = TrainIntegratorStore;
    self->integ.state = &self->motion;
    self->integ.owner = self;
    self->integ.deriv = TrainIntegratorDeriv;
    self->motion.count = 2;
}

struct BisectOut {
    float p;
    unsigned int q;
    unsigned int r;
    unsigned int w;
};

/* Bisects the time step in [0, t] after which the train just passes the end of its current curve stretch (to
 * 0.001), restarting from the saved state each try; leaves the train stepped by it and returns it. */
// FUNCTION: LEGOLAND 0x0041df00
float BisectTrainSegmentEnd(struct Train *obj, float t) {
    struct BisectOut s;
    struct TrainMotion anim;
    float lo;
    float hi;

    /* bisection on t in [0, t]: t itself holds the midpoint being tried */
    anim = obj->motion;
    lo = 0.0f;
    hi = t;
    s.p = 0.0f;
    s.q = anim.count;
    while (hi - lo > CastleEpsilon) {
        t = (hi + lo) * 0.5f;
        PositionTrainCars(obj, anim.pos, &obj->walker);
        SetTrainEnergy((struct Train *)obj, anim.energy);
        IntegratorStepRK4(&obj->integ, &s.p, t);
        if (obj->motion.pos > obj->walker.node->end) {
            hi = t;
        } else {
            lo = t;
        }
    }
    PositionTrainCars(obj, anim.pos, &obj->walker);
    SetTrainEnergy((struct Train *)obj, anim.energy);
    lo = (hi + lo) * 0.5f;
    IntegratorStepRK4(&obj->integ, &s.p, lo);
    return lo;
}

/* Physics step of up to `total` seconds in RK4 steps of at most 1/70 s; stops exactly at the end of the current
 * curve stretch (BisectTrainSegmentEnd). Returns the time used. */
// FUNCTION: LEGOLAND 0x0041e000
float StepTrainToSegmentEnd(struct Train *obj, float total) {
    struct BisectOut s;
    struct TrainMotion anim;
    int i = 0;
    int n = (int)ceil(total * 70.0f);
    float dt = total / n;
    float acc = 0.0f;

    s.p = 0.0f;
    for (i = 0; i < n; i++) {
        anim = obj->motion;
        s.q = anim.count;
        IntegratorStepRK4(&obj->integ, &s.p, dt);
        if (obj->motion.pos > obj->walker.node->end) {
            PositionTrainCars(obj, anim.pos, &obj->walker);
            SetTrainEnergy((struct Train *)obj, anim.energy);
            return BisectTrainSegmentEnd(obj, dt) + acc;
        }
        acc += dt;
    }
    return total;
}

/* Steps the train while leaving the castle (the track's step_leaving). Returns the time used. */
// FUNCTION: LEGOLAND 0x0041e0e0
float StepTrainLeaving(struct Train *self, float time_left) {
    return self->track->step_leaving(self, time_left);
}

/* Steps the train while arriving (the track's step_arriving), keeping the integrator's derivative function. Returns
 * the time used. */
// FUNCTION: LEGOLAND 0x0041e100
float StepTrainArriving(struct Train *self, float time_left) {
    void (*saved)(struct Integrator *, float, struct FloatArray *) = self->integ.deriv;
    float used = self->track->step_arriving(self, time_left);

    self->integ.deriv = saved;
    return used;
}

/* Advances the train by the game time since its last step (in seconds, at most 0.8): runs the phase's step function
 * until the time is used up, moving the walker to the next curve stretch whenever one ends; leaving the castle
 * switches phase 8 -> 4, reaching the castle's piece again 4 -> 0x10. Adds its rdtsc cycle count to
 * TrainStepCycles. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x0041e130
__declspec(naked) void StepTrainForFrame(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x14
        push ebx
        push esi
        push edi
        call GetGameTimer
        mov esi, dword ptr [ebp + 8]
        mov dword ptr [ebp - 0xc], eax
        fild dword ptr [ebp - 0xc]
        mov eax, dword ptr [esi + 0x6c]
        mov dword ptr [ebp + 8], 0
        mov dword ptr [ebp - 0x14], eax
        fisub dword ptr [esi + 4]
        fmul dword ptr [FLOAT_004ab42c]
        fst dword ptr [ebp - 4]
        fcomp dword ptr [FLOAT_004ab428]
        fnstsw ax
        test ah, 0x41
        jne L41e174
        mov dword ptr [ebp - 4], 0x3f4ccccd
L41e174:
        push eax
        push edx
        rdtsc
        mov dword ptr [ebp - 8], eax
        pop edx
        pop eax
        fld dword ptr [ebp - 4]
        fsub dword ptr [FLOAT_004ab424]
        lea ebx, [esi + 0xc]
        fstp dword ptr [ebp - 0x10]
L41e18c:
        mov eax, dword ptr [esi]
        mov edi, dword ptr [ebx]
        test al, 8
        je L41e1a6
        fld dword ptr [ebp - 4]
        fsub dword ptr [ebp + 8]
        push ecx
        fstp dword ptr [esp]
        push esi
        call StepTrainLeaving
        jmp L41e1c1
L41e1a6:
        fld dword ptr [ebp - 4]
        fsub dword ptr [ebp + 8]
        push ecx
        test al, 0x10
        fstp dword ptr [esp]
        push esi
        je L41e1bc
        call StepTrainArriving
        jmp L41e1c1
L41e1bc:
        call StepTrainToSegmentEnd
L41e1c1:
        fadd dword ptr [ebp + 8]
        add esp, 8
        fstp dword ptr [ebp + 8]
        fld dword ptr [ebp + 8]
        fcomp dword ptr [ebp - 0x10]
        fnstsw ax
        test ah, 1
        je L41e216
        push ebx
        call CurveWalkerNext
        mov ecx, dword ptr [esi + 0x10]
        add esp, 4
        mov edx, dword ptr [ecx + 0x44]
        mov ecx, dword ptr [ebx]
        cmp edi, ecx
        mov dword ptr [esi + 0x24], edx
        je L41e18c
        mov eax, dword ptr [esi]
        test al, 8
        je L41e1fd
        and al, 0xf7
        or al, 4
        mov dword ptr [esi], eax
        jmp L41e18c
L41e1fd:
        test al, 4
        je L41e18c
        mov edx, dword ptr [ebp - 0x14]
        add edx, 4
        cmp ecx, edx
        jne L41e18c
        and al, 0xfb
        or al, 0x10
        mov dword ptr [esi], eax
        jmp L41e18c
L41e216:
        mov eax, dword ptr [ebp - 0xc]
        mov dword ptr [esi + 4], eax
        push eax
        push edx
        rdtsc
        sub eax, dword ptr [ebp - 8]
        mov dword ptr [ebp - 8], eax
        pop edx
        pop eax
        mov ecx, dword ptr [ebp - 8]
        mov eax, dword ptr [TrainStepCycles]
        add eax, ecx
        pop edi
        pop esi
        mov dword ptr [TrainStepCycles], eax
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Ends the start hold once the game time passes hold_until; records the step time. */
// FUNCTION: LEGOLAND 0x0041e240
void UpdateTrainStartDelay(struct Train *timed) {
    int now = GetGameTimer();
    if (now > timed->hold_until) {
        timed->flags &= ~0x40;
    }
    timed->last_step_time = now;
}

/* Seats arg in the first car with a free seat; 0 if the train is full. */
// FUNCTION: LEGOLAND 0x0041e260
unsigned int PlaceInEmptyTrainSeat(struct Train *host, void *arg) {
    struct TrainCar *node = &host->head;

    while (PlaceInEmptySeatOfCar(node, arg) == 0) {
        node = node->next;
        if (node == &host->head) {
            return 0;
        }
    }
    return 1;
}

/* First free seat of the train, or NULL. */
// FUNCTION: LEGOLAND 0x0041e2b0
struct TrainSeat *FindEmptyTrainSeat(struct Train *host) {
    struct TrainCar *node = &host->head;
    struct TrainSeat *r;

    while ((r = FindEmptySeatInCar(node)) == 0) {
        node = node->next;
        if (node == &host->head) {
            return 0;
        }
    }
    return r;
}

/* Takes the occupant out of the first occupied seat (NULL if the train is empty). */
// FUNCTION: LEGOLAND 0x0041e2f0
struct CastleVisitor *FindTrainOccupant(struct Train *host) {
    struct TrainCar *node = &host->head;
    struct CastleVisitor *r;
    while ((r = FindOccupantInCar(node)) == 0) {
        node = node->next;
        if (node == &host->head) {
            return 0;
        }
    }
    return r;
}

/* Calls visit for every car of the train's ring, head first. */
// FUNCTION: LEGOLAND 0x0041e330
void ForEachRingNode(struct Train *host, void (*visit)(struct TrainCar *car)) {
    struct TrainCar *start = &host->head;
    struct TrainCar *node = start;
    do {
        visit(node);
        node = node->next;
    } while (node != start);
}

/* Adds every car to the castle's dirty-rectangle list. */
// FUNCTION: LEGOLAND 0x0041e360
void AddAllTrainCarLinks(struct Train *host) {
    ForEachRingNode(host, AddTrainCarLink);
}

/* Removes every car from the castle's dirty-rectangle list. */
// FUNCTION: LEGOLAND 0x0041e380
void UnlinkAllTrainCars(struct Train *host) {
    ForEachRingNode(host, UnlinkTrainCar);
}

/* Recomputes every car's screen rectangle. */
// FUNCTION: LEGOLAND 0x0041e3a0
void UpdateAllTrainCarBounds(struct Train *host) {
    ForEachRingNode(host, UpdateTrainCarBounds);
}

/* DrawTrainCarOnPiece for TrainDrawPiece (ForEachRingNode callback). */
// FUNCTION: LEGOLAND 0x0041e3c0
unsigned int DrawTrainCarOnDrawPiece(struct TrainCar *param) {
    return DrawTrainCarOnPiece(param, TrainDrawPiece);
}

/* Draws the cars that stand on the given piece and were not drawn yet this frame. */
// FUNCTION: LEGOLAND 0x0041e3e0
void DrawTrainCarsOnPiece(struct Train *host, struct PathSeg *value) {
    TrainDrawPiece = value;
    ForEachRingNode(host, (void (*)(struct TrainCar *))DrawTrainCarOnDrawPiece);
}

/* Clears every car's drawn flag. */
// FUNCTION: LEGOLAND 0x0041e400
void ClearAllTrainCarsDrawn(struct Train *host) {
    ForEachRingNode(host, ClearTrainCarDrawn);
}

/* Allocates a car of the given kind and inserts it right after the head car. */
// FUNCTION: LEGOLAND 0x0041e420
void AddTrainCar(struct Train *self, unsigned int flag) {
    struct TrainCar *node = AllocTrainCar(flag);
    struct TrainCar *head;

    if (node != NULL) {
        self->head.next->prev = node;
        head = &((struct Train *)self)->head;
        node->next = head->next;
        head->next = node;
        node->prev = head;
    }
}

/* Unlinks and frees a car (not the head car, which is part of the Train). */
// FUNCTION: LEGOLAND 0x0041e460
void RemoveTrainCar(struct TrainCar *node, struct Train *host) {
    if (node != &host->head) {
        node->next->prev = node->prev;
        node->prev->next = node->next;
        FreeTrainCar(node);
    }
}

/* 1 if the train is running (flags & 2). */
// FUNCTION: LEGOLAND 0x0041e4a0
unsigned int IsTrainRunning(struct Train *obj) {
    if (*(signed char *)&obj->flags & 2) {
        return 1;
    }
    return 0;
}

/* 1 while the train is held at the start (flags & 0x40). */
// FUNCTION: LEGOLAND 0x0041e4b0
unsigned int IsTrainStartDelayed(struct Train *obj) {
    if (*(signed char *)&obj->flags & 0x40) {
        return 1;
    }
    return 0;
}

/* Starts the train, optionally held for delay ms. */
// FUNCTION: LEGOLAND 0x0041e4c0
unsigned int StartTrain(struct Train *obj, unsigned int delay) {
    obj->flags |= 2;
    if (delay != 0) {
        obj->flags |= 0x40;
        obj->hold_until = GetGameTimer() + delay;
        return obj->hold_until;
    }
    obj->flags &= ~0x40;
    return obj->flags;
}

/* Stops the train and puts it back in the station. */
// FUNCTION: LEGOLAND 0x0041e4f0
void StopTrain(struct Train *obj) {
    obj->flags &= 0xfffffffd;
    ResetTrainToStation(obj);
}

/* Puts the train in the station phase at the castle's start position (the track's get_start), stamps the step time
 * and gives it potential energy plus the kinetic energy of speed 0.1. */
// FUNCTION: LEGOLAND 0x0041e500
void ResetTrainToStation(struct Train *obj) {
    struct CurveWalker t;
    float slot;

    obj->flags = 8;
    t.piece = (struct PathSeg *)&obj->track->piece;
    obj->track->get_start(obj->track, &t.node, &slot, &t.pos);
    PositionTrainCarsAndBogies(obj, slot, &t);
    obj->last_step_time = GetGameTimer();
    obj->motion.energy = SumTrainPotential(obj);
    obj->motion.energy = GetTrainKineticEnergy(obj, 0.1f) + obj->motion.energy;
}

/* Allocates a train for the track: head car, then middle and tail car, its integrator, in the station, cars added
 * to the dirty-rectangle list. */
// FUNCTION: LEGOLAND 0x0041e570
struct Train *CreateTrain(struct CastleState *arg) {
    struct Train *self = MemAlloc(sizeof(struct Train), 0, 0, 0);
    if (self == NULL) {
        return NULL;
    }
    self->track = arg;
    InitTrainCar(&self->head, 0);
    AddTrainCar(self, 2);
    AddTrainCar(self, 1);
    self->flags = 0;
    InitTrainIntegrator((struct Train *)self);
    ResetTrainToStation(self);
    AddAllTrainCarLinks((struct Train *)self);
    return self;
}

/* Unlinks and frees every car and the train, clears *param_1. */
// FUNCTION: LEGOLAND 0x0041e5d0
void DestroyTrain(struct Train **param_1) {
    struct Train *host = (struct Train *)*param_1;
    struct TrainCar *node = &host->head;
    struct TrainCar *next;

    UnlinkAllTrainCars(host);
    do {
        next = node->next;
        RemoveTrainCar(node, host);
        node = next;
    } while (next != &host->head);
    MemFree(*param_1);
    *param_1 = NULL;
}

/* TrainVecMath: vector ops of dimension 10 (energy + 3 car centres) for the derivatives. */
// FUNCTION: LEGOLAND 0x0041e620
void InitTrainVecMath(void) {
    InitVecMath(&TrainVecMath, 10);
}

/* Clears the car's drawn flag. */
// FUNCTION: LEGOLAND 0x0041e630
void ClearTrainCarDrawn(struct TrainCar *obj) {
    obj->flags &= 0xfffffffe;
}

/* Sets or clears bit 1 (drawn) of a car's flags. */
// FUNCTION: LEGOLAND 0x0041e640
void SetTrainCarDrawnFlag(unsigned int *flags, unsigned int set) {
    if (set == 1) {
        *flags |= 1;
    } else {
        *flags &= 0xfffffffe;
    }
}

/* 1 if the car was drawn this frame. */
// FUNCTION: LEGOLAND 0x0041e660
unsigned int IsTrainCarDrawn(struct TrainCar *a) {
    if (*(signed char *)&a->flags & 1) {
        return 1;
    }
    return 0;
}

/* 1 if either bogie's front wheel is on the given piece. */
// FUNCTION: LEGOLAND 0x0041e670
unsigned int IsTrainCarOnPiece(struct TrainCar *obj, struct PathSeg *id) {
    if (obj->front.w0.walker.piece == id) {
        return 1;
    }
    return obj->rear.w0.walker.piece == id;
}

/* Initialises a car of the given kind as a ring of one: its two seats at x = TrainCarSeatX[kind] and 16 units
 * behind, 4 units up. */
// FUNCTION: LEGOLAND 0x0041e6a0
void InitTrainCar(struct TrainCar *param, unsigned int value) {
    float v[3];

    v[1] = 0.0f;
    v[2] = -4.0f;
    param->kind = value;
    v[0] = TrainCarSeatX[value];
    InitTrainSeat(&param->seats[0], (const struct FVec3 *)v);
    v[0] = v[0] - TrainSeatSpacing;
    InitTrainSeat(&param->seats[1], (const struct FVec3 *)v);
    param->next = param;
    param->prev = param;
    param->flags = 0;
}

/* Seats arg in the car's first free seat; 0 if both are taken. */
// FUNCTION: LEGOLAND 0x0041e720
unsigned int PlaceInEmptySeatOfCar(struct TrainCar *host, void *arg) {
    struct TrainSeat *slot = &host->seats[0];
    int i = 0;
    while (i <= 1) {
        if (slot->method_10(slot) == 0) {
            slot->method_14(slot, arg);
            return 1;
        }
        i++;
        slot++;
    }
    return 0;
}

/* The car's first free seat, or NULL. */
// FUNCTION: LEGOLAND 0x0041e760
struct TrainSeat *FindEmptySeatInCar(struct TrainCar *host) {
    struct TrainSeat *slot = &host->seats[0];
    int i = 0;
    while (i <= 1) {
        if (slot->method_10(slot) == 0) {
            return slot;
        }
        i++;
        slot++;
    }
    return NULL;
}

/* Takes the occupant out of the car's first occupied seat, or NULL. */
// FUNCTION: LEGOLAND 0x0041e790
struct CastleVisitor *FindOccupantInCar(struct TrainCar *host) {
    struct TrainSeat *slot = &host->seats[0];
    int i = 0;

    while (i <= 1) {
        if (slot->method_10(slot) != 0) {
            return TakeTrainSeatOccupant(slot);
        }
        i++;
        slot++;
    }
    return 0;
}

/* Frees the occupants of both seats of the car (the seats' method_18). */
// FUNCTION: LEGOLAND 0x0041e7c0
void FreeCarSeatOccupants(struct TrainCar *host) {
    struct TrainSeat *slot = &host->seats[0];
    int i = 2;
    do {
        slot->method_18(slot);
        slot++;
        i--;
    } while (i != 0);
}

/* Every car weighs TrainCarMass (0.1). */
// FUNCTION: LEGOLAND 0x0041e7e0
float GetTrainCarMass(struct TrainCar *obj) {
    return TrainCarMass;
}

/* Copies the car's centre. */
// FUNCTION: LEGOLAND 0x0041e7f0
void GetTrainCarCenter(unsigned char *obj, unsigned int *out) {
    obj += offsetof(struct TrainCar, center);
    out[0] = *(unsigned int *)obj;
    out[1] = *(unsigned int *)(obj + 4);
    out[2] = *(unsigned int *)(obj + 8);
}

/* The car's potential energy (computed by PositionTrainCar). */
// FUNCTION: LEGOLAND 0x0041e810
float GetTrainCarPotential(struct TrainCar *obj) {
    return obj->potential;
}

/* Puts the car's front wheels at parameter t on the walker's stretch and its rear wheels 30 units behind; the
 * centre is their midpoint and the potential energy mass * centre.z * -0.001349. */
// FUNCTION: LEGOLAND 0x0041e820
void PositionTrainCar(struct TrainCar *obj, struct CurveWalker *e, float t) {
    float vA[3];
    float vB[3];
    struct CurveWalker k;
    float f;

    SetTrainWheelPos(&obj->front.w0, e, *(unsigned int *)&t);
    GetTrainWheelPos(&obj->front.w0, 2, vA);
    FindPathPosAtRangeBehind(vA, 30.0f, &obj->front.w0.walker, obj->front.w0.t, 4.8f, &k, &f);
    SetTrainWheelPos(&obj->rear.w0, &k, *(unsigned int *)&f);
    GetTrainWheelPos(&obj->rear.w0, 2, vB);
    obj->center.x = (vA[0] + vB[0]) * 0.5f;
    obj->center.y = (vA[1] + vB[1]) * 0.5f;
    obj->center.z = (vA[2] + vB[2]) * 0.5f;
    obj->potential = GetTrainCarMass(obj) * obj->center.z * -0.00134937500115484f;
}

/* PositionTrainCar, then re-initialises both bogies at their wheels' positions. */
// FUNCTION: LEGOLAND 0x0041e8f0
void PositionTrainCarAndBogies(struct TrainCar *obj, struct CurveWalker *e, float t) {
    PositionTrainCar(obj, e, t);
    InitTrainBogie(&obj->front, &obj->front.w0.walker, obj->front.w0.t);
    InitTrainBogie(&obj->rear, &obj->rear.w0.walker, obj->rear.w0.t);
}

/* World position of the car's rear wheels. */
// FUNCTION: LEGOLAND 0x0041e930
unsigned int GetTrainCarRearPos(struct TrainCar *obj, float *param) {
    return GetTrainWheelPos(&obj->rear.w0, 2, param);
}

/* Adds the car to the castle's dirty-rectangle list. */
// FUNCTION: LEGOLAND 0x0041e950
void AddTrainCarLink(struct TrainCar *obj) {
    AddCastleLink(&obj->link);
}

/* Removes the car from the castle's dirty-rectangle list. */
// FUNCTION: LEGOLAND 0x0041e970
void UnlinkTrainCar(struct TrainCar *param_1) {
    UnlinkListLink(&param_1->link);
}

struct RectI {
    int var_0;
    int var_4;
    int var_8;
    int var_c;
};

/* Projects the car model's bounding box at the car's position and stores the clipped screen rectangle in its list
 * entry. */
// FUNCTION: LEGOLAND 0x0041e990
void UpdateTrainCarBounds(struct TrainCar *obj) {
    struct {
        struct RectI r;
        unsigned int loc1[3];
        unsigned int b[9];
    } s;

    GetTrainCarCenterAndAxis(obj, (float *)s.loc1);
    GetLmsBoxScreenRect(CoasterTrainCarLms[0], s.loc1, s.b, &s.r);
    ClipRectIToCastleView((struct RectI *)&obj->link, &s.r);
}

/* out[0..2] = the car's centre, out[3..11] = its upright basis from the front-to-rear wheel axis. */
// FUNCTION: LEGOLAND 0x0041e9e0
void GetTrainCarCenterAndAxis(struct TrainCar *obj, float *out) {
    float a[3];
    float b[3];

    GetTrainWheelPos(&obj->front.w0, 2, a);
    GetTrainWheelPos(&obj->rear.w0, 2, b);
    out[0] = (a[0] + b[0]) * 0.5f;
    out[1] = (a[1] + b[1]) * 0.5f;
    out[2] = (a[2] + b[2]) * 0.5f;
    out[3] = a[0] - b[0];
    out[4] = a[1] - b[1];
    out[5] = a[2] - b[2];
    BuildUprightBasis((struct FVec3 *)(out + 3), (struct FVec3 *)(out + 3));
}

/* Draws the car: both bogies, the car model (head/mid/tail by kind) and the two seats with their occupants. */
// FUNCTION: LEGOLAND 0x0041ea70
void DrawTrainCar(struct TrainCar *a) {
    struct CarXform x;

    SetModelLighting();
    DrawTrainBogie(&a->front);
    DrawTrainBogie(&a->rear);
    GetTrainCarCenterAndAxis(a, &x.pos.x);
    DrawLmsModel(CoasterTrainCarLms[a->kind], CoasterTrainCarLfm[a->kind], &x.pos, x.m, 0);
    a->seats[0].draw(&a->seats[0], &x);
    a->seats[1].draw(&a->seats[1], &x);
}

/* Draws the car if it is on the given piece and not drawn yet this frame. */
// FUNCTION: LEGOLAND 0x0041eaf0
unsigned int DrawTrainCarOnPiece(struct TrainCar *a, struct PathSeg *b) {
    if (IsTrainCarDrawn(a) == 0) {
        if (IsTrainCarOnPiece(a, b) != 0) {
            DrawTrainCar(a);
            SetTrainCarDrawnFlag(&a->flags, 1);
        }
    }
}

/* Allocates and initialises a car of the given kind. */
// FUNCTION: LEGOLAND 0x0041eb30
struct TrainCar *AllocTrainCar(unsigned int arg) {
    struct TrainCar *obj = MemAlloc(sizeof(struct TrainCar), 0, 0, 0);
    if (obj == NULL) {
        return NULL;
    }
    InitTrainCar(obj, arg);
    return obj;
}

/* Frees a car. */
// FUNCTION: LEGOLAND 0x0041eb60
void FreeTrainCar(void *param_1) {
    MemFree(param_1);
}

/* Looks up the head/mid/tail car meshes (.lms) and textures (.lfm) by name. */
// FUNCTION: LEGOLAND 0x0041eb70
void LoadCoasterTrainCarModels(void) {
    // STRING: LEGOLAND 0x004b55d8
    CoasterTrainCarLms[0] = GetLmsByName("coastertrain.headcar");
    // STRING: LEGOLAND 0x004b55c4
    CoasterTrainCarLms[1] = GetLmsByName("coastertrain.midcar");
    // STRING: LEGOLAND 0x004b55ac
    CoasterTrainCarLms[2] = GetLmsByName("coastertrain.tailcar");
    CoasterTrainCarLfm[0] = GetLfmByName("coastertrain.headcar");
    CoasterTrainCarLfm[1] = GetLfmByName("coastertrain.midcar");
    CoasterTrainCarLfm[2] = GetLfmByName("coastertrain.tailcar");
}

/* Index of the dispatch row whose element belongs to ride type arg (0 if none). */
// FUNCTION: LEGOLAND 0x0041ebd0
unsigned int FindCastleDispatchIndex(struct Ride *arg) {
    unsigned int index = 0;
    struct DispatchRow *row = CastleDispatchTable;
    while ((INT_PTR)row < (INT_PTR)&CastleDispatchTableEnd) {
        struct Element *target = row->element;
        if (target != NULL && arg == target->ride) {
            return index;
        }
        index++;
        row++;
    }
    return 0;
}

/* Ride type of the element in dispatch row param. */
// FUNCTION: LEGOLAND 0x0041ec00
struct Ride *GetCastleDispatchRide(unsigned int param) {
    struct Element *target = CastleDispatchTable[param].element;
    return target->ride;
}

/* Index of the dispatch row for element param (0 if none). */
// FUNCTION: LEGOLAND 0x0041ec20
unsigned int CastleDispatchIndex(struct Element *param) {
    unsigned int index = 0;
    struct DispatchRow *row = CastleDispatchTable;
    while ((INT_PTR)row < (INT_PTR)&CastleDispatchTableEnd) {
        if (param == row->element) {
            return index;
        }
        row++;
        index++;
    }
    return 0;
}

/* Element of dispatch row param. */
// FUNCTION: LEGOLAND 0x0041ec40
struct Element *GetCastleDispatchElement(unsigned int param) {
    return CastleDispatchTable[param].element;
}

/* Calls the element's SetEditMode handler from CastleDispatchTable. */
// FUNCTION: LEGOLAND 0x0041ec50
void CastleDispatchSetEditMode(struct Element *obj) {
    CastleDispatchTable[CastleDispatchIndex(obj)].fn_4(obj);
}

/* Calls the element's CalcCursor handler from CastleDispatchTable. */
// FUNCTION: LEGOLAND 0x0041ec70
void CastleDispatchCalcCursor(struct Element *obj, struct Point *a, unsigned int b) {
    CastleDispatchTable[CastleDispatchIndex(obj)].fn_8(obj, a, b);
}

/* Calls the AddObject handler of dispatch row idx with the cell p (converted to ints). */
// FUNCTION: LEGOLAND 0x0041eca0
void CastleDispatchAddObjectByIndex(unsigned int idx, short *p) {
    int v[2];

    v[0] = p[0];
    v[1] = p[1];
    CastleDispatchTable[idx].fn_c(CastleDispatchTable[idx].element, v);
}

/* Calls the element's AddObject handler from CastleDispatchTable. */
// FUNCTION: LEGOLAND 0x0041ece0
void CastleDispatchAddObject(struct Element *obj, int *a) {
    ((void (*)(struct Element *, int *)) * (void (**)())((char *)CastleDispatchTable + CastleDispatchIndex(obj) * sizeof(struct DispatchRow) + offsetof(struct DispatchRow, fn_c)))(obj, a);
}

/* Calls the element's DCalcCursor handler from CastleDispatchTable. */
// FUNCTION: LEGOLAND 0x0041ed00
void CastleDispatchDCalcCursor(struct Element *obj, struct Point *a) {
    ((void (*)(struct Element *, struct Point *)) * (void (**)())((char *)CastleDispatchTable + CastleDispatchIndex(obj) * sizeof(struct DispatchRow) + offsetof(struct DispatchRow, fn_10)))(obj, a);
}

/* Calls the element's RemoveObject handler from CastleDispatchTable. */
// FUNCTION: LEGOLAND 0x0041ed50
void CastleDispatchRemoveObject(struct Element *obj, TileId a, struct Cursor *b) {
    ((void (*)(struct Element *, TileId, struct Cursor *)) * (void (**)())((char *)CastleDispatchTable + CastleDispatchIndex(obj) * sizeof(struct DispatchRow) + offsetof(struct DispatchRow, fn_14)))(obj, a, b);
}

/* Turns putting coaster pieces on the game map on or off (off while loading). */
// FUNCTION: LEGOLAND 0x0041ed80
void SetMapObjectsEnabled(unsigned int param) {
    MapObjectsEnabled = param;
}

/* AddBasicObject when map objects are enabled. */
// FUNCTION: LEGOLAND 0x0041ed90
void AddMapObjectIfEnabled(struct Element *element, int *pos) {
    if (MapObjectsEnabled != 0) {
        AddBasicObject(element, pos);
    }
}

/* StandardRemoveObject when map objects are enabled. */
// FUNCTION: LEGOLAND 0x0041edb0
void RemoveMapObjectIfEnabled(struct Element *element, TileId tile, struct Cursor *cursor) {
    if (MapObjectsEnabled != 0) {
        StandardRemoveObject(element, tile, cursor);
    }
}

struct KindInfo {
    unsigned char pad_0[0x1c];
    unsigned int field_1c;
};

struct KindOwner {
    unsigned char pad_0[0xc];
    struct KindInfo *field_c;
};

struct KindObj {
    struct KindOwner *owner;
    unsigned char pad_4[0x8];
    unsigned short flags;
};

/* 1 if a map cell blocks a coaster piece: off the map, or occupied by anything other than plain path, a 0x800 cell
 * or an object class with flag 0x200000. */
// FUNCTION: LEGOLAND 0x0041ede0
unsigned int IsMapElementBlocking(struct KindObj *obj) {
    struct KindOwner *owner;
    struct KindInfo *info;
    unsigned short flags;

    if (obj == NULL) {
        return 1;
    }
    flags = obj->flags;
    if ((flags & 0x8f8) == 0) {
        return 0;
    }
    if (flags & 0x40) {
        return 1;
    }
    owner = obj->owner;
    if (owner != NULL) {
        info = owner->field_c;
    } else {
        info = NULL;
    }
    if ((flags & 0x8) && owner != NULL && info == PathControlObject) {
        return 0;
    }
    if (flags & 0x800) {
        return 0;
    }
    if ((flags & 0x88) && info != NULL && (info->field_1c & 0x200000)) {
        return 0;
    }
    return 1;
}

/* 1 if no cell of the area list (rectangles x..w-1, y..h-1) moved to pos is blocked; always 1 while map objects are
 * disabled. */
// FUNCTION: LEGOLAND 0x0041ee40
unsigned int IsAreaListClear(short *pos, struct AreaNode *b) {
    struct AreaNode *node;
    struct MapElement *elem;
    int x;
    int y;

    if (MapObjectsEnabled != 0) {
        node = b;
        do {
            for (y = node->y + pos[1]; y < node->h + pos[1]; y++) {
                for (x = node->x + pos[0]; x < node->w + pos[0]; x++) {
                    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                        elem = &GameMap[y][x];
                    } else {
                        elem = NULL;
                    }
                    if (IsMapElementBlocking((struct KindObj *)elem) != 0) {
                        return 0;
                    }
                }
            }
            node = node->next;
        } while (node != NULL);
    }
    return 1;
}

/* Does nothing but return 1. */
/* Returns 1. */
// FUNCTION: LEGOLAND 0x0041ef00
int ReturnOne_0041ef00(void) {
    return 1;
}

/* Sets the four screen clip planes to the rectangle x0..x1, y0..y1. */
// FUNCTION: LEGOLAND 0x0041ef20
void SetClipRect(int a, int b, int c, int d) {
    struct RectI r;

    r.var_0 = b;
    r.var_4 = a;
    r.var_c = c;
    r.var_8 = d;
    SetClipPlanesFromRect((int *)&r, ClipPlanes);
}

/* Clips a triangle against the screen planes it is not already inside of: code bits 1|2 = inside both x planes, 4|8
 * = inside both y planes (0xf: no clipping). n = number of interpolated vertex fields. Returns the clipped
 * polygon's vertex list, the vertex count in *out. */
// FUNCTION: LEGOLAND 0x0041ef60
void *ClipTriangle(void *verts, int *out, unsigned int code, int n) {
    int sel = 0;

    if (code == 0xf) {
        *out = 3;
        return verts;
    }
    SetClipVertexFormat(n + 1);
    if ((char)(code & 3) < 3) {
        sel = 1;
    }
    if ((char)(code & 0xc) < 0xc) {
        sel |= 2;
    }
    switch (sel) {
    case 3:
        return ClipPolygonToPlanes(3, verts, out, 4, ClipPlanes->p[0]);
    case 2:
        return ClipPolygonToPlanes(3, verts, out, 2, ClipPlanes->p[2]);
    case 1:
        return ClipPolygonToPlanes(3, verts, out, 2, ClipPlanes->p[0]);
    }
    return out;
}

/* Clipped vertices are 28 bytes (7 ints); fields 0..n are interpolated at the new vertices. */
// FUNCTION: LEGOLAND 0x0041f030
void SetClipVertexFormat(unsigned int param) {
    ClipVertexStride = 0x1c;
    ClipVertexLastField = param;
}

union FloatBits {
    float f;
    unsigned int u;
};

struct FDist {
    union FloatBits mag;
    unsigned int sign;
};

/* One Sutherland-Hodgman pass: clips polygon src (n vertices) against plane pl (inside where pl[2] - (v[2] * pl[0]
 * + v[1] * pl[1]) >= 0) into dst, taking new vertices from *cursor. Returns the new vertex count. */
// FUNCTION: LEGOLAND 0x0041f050
int ClipPolygonToPlane(int n, int **src, int **dst, int **cursor, float *pl) {
    struct FDist m, pm;
    int count = 0;
    int *pool = *cursor;
    int i, k;
    int *prev, *cur;
    float t;
    int code;

    src[n] = src[0];
    cur = src[0];
    m.mag.f = pl[2] - (cur[2] * pl[0] + cur[1] * pl[1]);
    code = m.mag.u & 0x80000000;
    m.mag.u &= 0x7fffffff;
    for (i = 1; i <= n; i++) {
        pm.mag = m.mag;
        prev = cur;
        cur = src[i];
        code = (code >> 1) & 0x40000000;
        m.mag.f = pl[2] - (cur[2] * pl[0] + cur[1] * pl[1]);
        code |= m.mag.u & 0x80000000;
        m.mag.u &= 0x7fffffff;
        switch (code) {
        case 0x40000000:
            t = pm.mag.f / (pm.mag.f + m.mag.f);
            *dst++ = prev;
            for (k = 0; k <= ClipVertexLastField; k++) {
                pool[k] = prev[k] + (int)((cur[k] - prev[k]) * t);
            }
            *dst++ = pool;
            pool = (int *)((char *)pool + ClipVertexStride);
            count += 2;
            break;
        case 0xc0000000:
            *dst++ = prev;
            count++;
            break;
        case 0x80000000:
            t = m.mag.f / (pm.mag.f + m.mag.f);
            for (k = 0; k <= ClipVertexLastField; k++) {
                pool[k] = cur[k] + (int)((prev[k] - cur[k]) * t);
            }
            *dst++ = pool;
            pool = (int *)((char *)pool + ClipVertexStride);
            count++;
            break;
        }
    }
    *cursor = pool;
    return count;
}

/* Clips a polygon against nPlanes planes, ping-ponging between the two ClipVertexLists; NULL (count 0) when nothing
 * is left. */
// FUNCTION: LEGOLAND 0x0041f2b0
int **ClipPolygonToPlanes(int n, int **verts, int *outCount, int nPlanes, float *planes) {
    int i;

    for (i = 0; i < n; i++) {
        ClipVertsIn[i] = verts[i];
    }
    ClipVertexPoolNext = ClipVertexPool;
    for (i = 0; i < nPlanes; i++) {
        n = ClipPolygonToPlane(n, ClipVertexLists[i & 1], ClipVertexLists[(i - 1) & 1], &ClipVertexPoolNext, planes);
        if (n == 0) {
            *outCount = 0;
            return NULL;
        }
        planes += 3;
    }
    *outCount = n;
    return ClipVertexLists[i & 1];
}

struct Pair {
    unsigned int field_0;
    unsigned int field_4;
};

/* ClipPolygonToPlanes with a PlaneSet. */
// FUNCTION: LEGOLAND 0x0041f350
int **ClipPolygonToPlaneSet(int param1, int **param2, int *param3, struct PlaneSet *param4) {
    return ClipPolygonToPlanes(param1, param2, param3, param4->count, param4->p[0]);
}

/* The four planes of rectangle r = {y0, x0, y1, x1}: x >= x0, x <= x1, y >= y0, y <= y1. */
// FUNCTION: LEGOLAND 0x0041f380
void SetClipPlanesFromRect(const int *r, struct PlaneSet *out) {
    out->count = 4;
    out->p[0][0] = 1.0f;
    out->p[0][1] = 0.0f;
    out->p[0][2] = (float)r[1];
    out->p[1][0] = -1.0f;
    out->p[1][1] = 0.0f;
    out->p[1][2] = -(float)r[3];
    out->p[2][0] = 0.0f;
    out->p[2][1] = 1.0f;
    out->p[2][2] = (float)r[0];
    out->p[3][0] = 0.0f;
    out->p[3][1] = -1.0f;
    out->p[3][2] = -(float)r[2];
}

/* Forward difference table of fn sampled at n + 1 points spaced b around a: row k holds the k-th differences
 * (vector version; the rows share one VecMath scratch block). */
// FUNCTION: LEGOLAND 0x0041f3e0
struct FloatArray ***BuildVecDifferenceTable(void (*fn)(float, struct FloatArray *), struct VecMath *ar, int n, float a, float b) {
    int cnt = n + 1;
    struct FloatArray **blk;
    int i;
    int j;
    int k;

    blk = ar->alloc((cnt + 1) * cnt >> 1);
    if (blk == NULL) {
        return NULL;
    }
    for (i = 0; i <= n; i++) {
        DifferenceTableRows[i] = blk;
        blk += cnt - i;
    }
    a -= (n >> 1) * b;
    for (i = 0; i <= n; i++) {
        fn(a, DifferenceTableRows[0][i]);
        a += b;
    }
    for (k = 1; k <= n; k++) {
        for (j = 0; j < n - k + 1; j++) {
            ar->sub(DifferenceTableRows[k - 1][j + 1], DifferenceTableRows[k - 1][j], DifferenceTableRows[k][j]);
        }
    }
    return DifferenceTableRows;
}

/* Releases a vector difference table's scratch block. */
// FUNCTION: LEGOLAND 0x0041f4c0
unsigned int FreeVecDifferenceTable(struct FloatArray ***param1, struct VecMath *param2, int param3) {
    int n = param3 + 1;
    int tri = ((n + 1) * n) >> 1;
    return ((unsigned int (*)(struct FloatArray **, unsigned int))param2->release)(*param1, tri);
}

/* Derivative of fn at b from its difference table with spacing c / 2 (central first differences corrected by the
 * third differences, weight 1/6). Used for the train's slope and inertia. */
// FUNCTION: LEGOLAND 0x0041f4e0
int VecDerivative(void (*fn)(float, struct FloatArray *), struct VecMath *a, float b, float c, struct FloatArray *d) {
    struct VecMath *ar = a;
    struct FloatArray ***t;
    struct FloatArray **h;
    struct FloatArray *p;
    struct FloatArray *q;

    c *= 0.5f;
    t = BuildVecDifferenceTable(fn, ar, 4, b, c);
    h = ar->alloc(2);
    p = h[0];
    q = h[1];
    if (t == NULL) {
        return 0;
    }
    ar->add(t[1][1], t[1][2], p);
    ar->add(t[3][0], t[3][1], q);
    ar->scale(q, DerivThirdDiffWeight);
    ar->add(p, q, d);
    ar->scale(d, 0.5f / c);
    ar->release(h, 2);
    FreeVecDifferenceTable(t, ar, 4);
    return 1;
}

/* Derivative of fn at b by the 4-point rule over b +- c/2, b +- c/6: (f(b-3d) - 27 f(b-d) + 27 f(b+d) - f(b+3d)) /
 * 48d with d = c/6. */
// FUNCTION: LEGOLAND 0x0041f5a0
int VecDerivative4Point(void (*fn)(float, struct FloatArray *), struct VecMath *a, float b, float c, struct FloatArray *d) {
    struct VecMath *ar = a;
    struct FloatArray **h;
    struct FloatArray *q;
    struct FloatArray *r;
    int i;

    c *= 0.5f;
    h = ar->alloc(2);
    q = h[1];
    r = d;
    fn(Deriv4PointFirstOffset * c + b, r);
    ar->scale(r, Deriv4PointFirstWeight);
    for (i = 0; i <= 2; i++) {
        fn(c * Deriv4PointOffsets[i] + b, q);
        ar->scale(q, Deriv4PointWeights[i]);
        ar->add(q, r, r);
    }
    ar->scale(r, 1.0f / c);
    ar->release(h, 2);
    return 1;
}

/* Scalar version of BuildVecDifferenceTable (one MemAlloc block: row pointers, then the triangle). */
// FUNCTION: LEGOLAND 0x0041f650
float **BuildDifferenceTable(double (*fn)(float), int n, float a, float b) {
    int cnt = n + 1;
    float **t;
    float *p;
    int i;
    int j;
    int k;

    /* the rows' pointers, then the triangle of cnt * (cnt + 1) / 2 floats */
    t = (float **)MemAlloc(((cnt + 1) * cnt >> 1) * sizeof(float) + cnt * sizeof(float *), 0, 0, 0);
    if (t == NULL) {
        return NULL;
    }
    p = (float *)(t + n + 1);
    for (i = 0; i <= n; i++) {
        t[i] = p;
        p += n + 1 - i;
    }
    a -= (n >> 1) * b;
    for (i = 0; i <= n; i++) {
        t[0][i] = (float)fn(a);
        a += b;
    }
    for (k = 1; k <= n; k++) {
        for (j = 0; j < n - k + 1; j++) {
            t[k][j] = t[k - 1][j + 1] - t[k - 1][j];
        }
    }
    return t;
}

/* Frees a difference table. */
// FUNCTION: LEGOLAND 0x0041f710
void FreeDifferenceTable(void *param) {
    MemFree(param);
}

/* Scalar version of VecDerivative. */
// FUNCTION: LEGOLAND 0x0041f720
int ScalarDerivative(double (*fn)(float), float p2, float p3, float *out) {
    float **r;

    p3 *= 0.5f;
    r = BuildDifferenceTable(fn, 4, p2, p3);
    if (!r) {
        return 0;
    }
    p3 = ((r[1][2] + r[1][1]) - (r[3][1] + r[3][0]) * ScalarDerivThirdDiffWeight) * 0.5f / p3;
    FreeDifferenceTable(r);
    *out = p3;
    return 1;
}

/* Test function for the derivatives: (e^x, 2 e^x, 2 e^2x). */
// FUNCTION: LEGOLAND 0x0041f790
void ExpTestCurve(float x, struct FloatArray *out) {
    double e;
    e = exp(x);
    out->count = 3;
    out->data[0] = (float)e;
    out->data[1] = (float)(2.0 * e);
    out->data[2] = (float)(2.0 * exp(2.0 * x));
}

/* log(x) (test function for ScalarDerivative). */
// FUNCTION: LEGOLAND 0x0041f7e0
double LogFloat(float param) {
    return log(param);
}

/* Debug test of ScalarDerivative and VecDerivative; nothing uses the results. */
// FUNCTION: LEGOLAND 0x0041f7f0
void TestNumericDerivatives(void) {
    float buf;
    struct VecMath x;
    struct FloatArray y;

    ScalarDerivative(LogFloat, 3.0f, 1.0f, &buf);
    InitVecMathOps();
    InitVecMath(&x, 3);
    VecDerivative(ExpTestCurve, &x, 3.0f, 0.01f, &y);
}

/* Steps the walker to the next curve stretch, moving on to the next piece's first stretch at the end of a piece. */
// FUNCTION: LEGOLAND 0x0041f850
struct SegBlob *CurveWalkerNext(struct CurveWalker *cursor) {
    struct SegBlob *result = cursor->node->next;
    if (result == 0) {
        struct PathSeg *piece = cursor->piece->next;
        cursor->piece = piece;
        result = GetPieceCurve(piece, &cursor->pos);
    }
    cursor->node = result;
    return result;
}

/* Steps the walker to the previous curve stretch, moving back to the previous piece's last stretch. */
// FUNCTION: LEGOLAND 0x0041f880
void CurveWalkerPrev(struct CurveWalker *walker) {
    struct SegBlob *node = walker->node->prev;
    if (node != NULL) {
        walker->node = node;
        return;
    }
    walker->piece = walker->piece->prev;
    walker->node = GetPieceCurve(walker->piece, &walker->pos);
    if (walker->node->next == NULL) {
        return;
    }
    do {
        walker->node = walker->node->next;
    } while (walker->node->next != NULL);
}

/* Span filler (FlatSpanFillers[0]): fills the rows of a scan-converted triangle with one colour from its shade
 * table, no depth test. Counts FilledTriangleCount. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x0041f8d0
__declspec(naked) void FillSpansFlat(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x60
        mov edx, dword ptr [ebp + 0x10]
        push ebx
        push esi
        mov esi, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [ebp + 0x18]
        mov ebx, dword ptr [ebp + 8]
        mov eax, dword ptr [esi]
        push edi
        mov dword ptr [ebp + 0x14], eax
        mov eax, dword ptr [esi + edx*8 - 4]
        lea eax, [eax + eax*2]
        shl eax, 4
        inc word ptr [eax + ecx + 2]
        mov ebx, dword ptr [ebx*4 + CoasterShadeTables]
        lea eax, [eax + ecx + 2]
        mov edi, dword ptr [CoasterSurfaceBits]
        mov eax, dword ptr [esi + edx*8 - 4]
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + ecx + 2]
        mov dword ptr [ebp - 4], eax
        mov eax, dword ptr [CoasterDepthBuffer]
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [CoasterSurfacePitch]
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [FilledTriangleCount]
        inc eax
        mov dword ptr [FilledTriangleCount], eax
        mov eax, dword ptr [ebp + 0xc]
        mov eax, dword ptr [eax]
        mov ax, word ptr [ebx + eax*2]
        mov ebx, dword ptr [ebp + 0x14]
        mov dword ptr [ebp + 0x10], eax
        mov eax, dword ptr [esi + edx*8 - 4]
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + ecx + 2]
        mov dword ptr [esi + edx*8], eax
        mov edx, dword ptr [ebp - 8]
        imul edx, ebx
        lea eax, [edi + edx*2]
        mov dword ptr [ebp - 0x10], eax
L41f964:
        mov eax, dword ptr [esi + 4]
        add esi, 8
        lea eax, [eax + eax*2]
        shl eax, 4
        add eax, ecx
        mov edx, dword ptr [eax + 4]
        test edx, edx
        mov edx, dword ptr [eax + 8]
        mov eax, dword ptr [eax + 0x1c]
        je L41f989
        sub edx, eax
        mov dword ptr [ebp - 0x24], eax
        mov dword ptr [ebp - 0x38], edx
        jmp L41f991
L41f989:
        sub edx, eax
        mov dword ptr [ebp - 0x4c], eax
        mov dword ptr [ebp - 0x60], edx
L41f991:
        cmp ebx, dword ptr [esi]
        jge L41f9ee
L41f995:
        inc dword ptr [ebp + 0x14]
        mov eax, dword ptr [ebp - 0x60]
        mov ebx, dword ptr [ebp - 0x38]
        add eax, dword ptr [ebp - 0x4c]
        add ebx, dword ptr [ebp - 0x24]
        mov dword ptr [ebp - 0x60], eax
        mov dword ptr [ebp - 0x38], ebx
        mov ecx, ebx
        sub ecx, eax
        cmp ecx, 0x8000
        jns L41f9b8
        jmp L41f9d4
L41f9b8:
        sar eax, 0x10
        sar ebx, 0x10
        mov edi, dword ptr [ebp - 0x10]
        xchg eax, ebx
        mov dx, word ptr [ebp + 0x10]
        sub ebx, eax
        lea edi, [edi + eax*2]
L41f9cb:
        mov word ptr [edi + ebx*2], dx
        add ebx, 1
        jle L41f9cb
L41f9d4:
        mov ecx, dword ptr [ebp - 8]
        mov edx, dword ptr [ebp + 0x14]
        lea eax, [ecx + ecx]
        mov ecx, dword ptr [ebp - 0x10]
        add ecx, eax
        mov eax, dword ptr [esi]
        cmp edx, eax
        mov dword ptr [ebp - 0x10], ecx
        jl L41f995
        mov ecx, dword ptr [ebp + 0x18]
L41f9ee:
        mov ebx, dword ptr [ebp + 0x14]
        mov eax, dword ptr [ebp - 4]
        cmp ebx, eax
        jl L41f964
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Flat span filler that writes the interpolated depth without testing it. Not referenced. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x0041fa10
__declspec(naked) void FillSpansFlatWriteZ(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x64
        mov ecx, dword ptr [ebp + 0x14]
        push ebx
        push esi
        mov esi, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ecx]
        mov edx, dword ptr [ebp + 0x18]
        mov dword ptr [ebp - 4], eax
        mov eax, dword ptr [ecx + esi*8 - 4]
        push edi
        lea eax, [eax + eax*2]
        shl eax, 4
        inc word ptr [eax + edx + 2]
        mov edi, dword ptr [CoasterDepthBuffer]
        lea eax, [eax + edx + 2]
        mov eax, dword ptr [ecx + esi*8 - 4]
        mov esi, dword ptr [CoasterSurfaceBits]
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + edx + 2]
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [CoasterSurfacePitch]
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 0xc]
        mov ebx, dword ptr [eax + 4]
        mov dword ptr [ebp + 0xc], ebx
        mov ebx, dword ptr [FilledTriangleCount]
        inc ebx
        mov dword ptr [FilledTriangleCount], ebx
        mov ebx, dword ptr [ebp + 8]
        mov eax, dword ptr [eax]
        mov ebx, dword ptr [ebx*4 + CoasterShadeTables]
        mov ax, word ptr [ebx + eax*2]
        mov ebx, dword ptr [ebp + 0x10]
        mov dword ptr [ebp + 8], eax
        mov eax, dword ptr [ecx + ebx*8 - 4]
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + edx + 2]
        mov dword ptr [ecx + ebx*8], eax
        mov eax, dword ptr [ebp - 0xc]
        imul eax, dword ptr [ebp - 4]
        shl eax, 1
        add esi, eax
        add edi, eax
        mov dword ptr [ebp - 0x14], esi
        mov dword ptr [ebp - 0x10], edi
L41fab1:
        mov eax, dword ptr [ecx + 4]
        add ecx, 8
        mov dword ptr [ebp + 0x14], ecx
        lea eax, [eax + eax*2]
        shl eax, 4
        add eax, edx
        mov esi, dword ptr [eax + 4]
        test esi, esi
        mov esi, dword ptr [eax + 8]
        je L41fad9
        mov eax, dword ptr [eax + 0x1c]
        sub esi, eax
        mov dword ptr [ebp - 0x28], eax
        mov dword ptr [ebp - 0x3c], esi
        jmp L41faf2
L41fad9:
        mov edi, dword ptr [eax + 0x1c]
        mov ebx, dword ptr [eax + 0xc]
        mov eax, dword ptr [eax + 0x20]
        sub esi, edi
        sub ebx, eax
        mov dword ptr [ebp - 0x50], edi
        mov dword ptr [ebp - 0x48], eax
        mov dword ptr [ebp - 0x64], esi
        mov dword ptr [ebp - 0x5c], ebx
L41faf2:
        mov eax, dword ptr [ebp - 4]
        mov esi, dword ptr [ecx]
        cmp eax, esi
        jge L41fb84
        mov ecx, dword ptr [ebp - 0xc]
        lea edx, [ecx + ecx]
        mov dword ptr [ebp + 0x10], edx
L41fb08:
        inc dword ptr [ebp - 4]
        mov eax, dword ptr [ebp - 0x64]
        mov edx, dword ptr [ebp - 0x5c]
        mov ebx, dword ptr [ebp - 0x3c]
        add eax, dword ptr [ebp - 0x50]
        add edx, dword ptr [ebp - 0x48]
        add ebx, dword ptr [ebp - 0x28]
        mov dword ptr [ebp - 0x64], eax
        mov dword ptr [ebp - 0x5c], edx
        mov dword ptr [ebp - 0x3c], ebx
        mov ecx, ebx
        sub ecx, eax
        cmp ecx, 0x8000
        jns L41fb34
        jmp L41fb62
L41fb34:
        sar eax, 0x10
        sar ebx, 0x10
        mov edi, dword ptr [ebp - 0x14]
        mov esi, dword ptr [ebp - 0x10]
        xchg eax, ebx
        sub ebx, eax
        lea edi, [edi + eax*2]
        lea esi, [esi + eax*2]
        mov ax, word ptr [ebp + 8]
L41fb4d:
        mov ecx, edx
        mov word ptr [edi + ebx*2], ax
        add edx, dword ptr [ebp + 0xc]
        sar ecx, 0x10
        mov word ptr [esi + ebx*2], cx
        add ebx, 1
        jle L41fb4d
L41fb62:
        mov eax, dword ptr [ebp + 0x10]
        mov edi, dword ptr [ebp - 0x14]
        mov esi, dword ptr [ebp - 0x10]
        mov ecx, dword ptr [ebp + 0x14]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [ebp - 4]
        mov edx, dword ptr [ecx]
        cmp eax, edx
        mov dword ptr [ebp - 0x14], edi
        mov dword ptr [ebp - 0x10], esi
        jl L41fb08
        mov edx, dword ptr [ebp + 0x18]
L41fb84:
        mov eax, dword ptr [ebp - 4]
        mov esi, dword ptr [ebp - 8]
        cmp eax, esi
        jl L41fab1
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Span filler (FlatSpanFillers[1]): flat colour, writing a pixel only where the 16-bit interpolated depth is >= the
 * depth buffer's, and then the depth too. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x0041fba0
__declspec(naked) void FillSpansFlatZ(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x64
        mov ecx, dword ptr [ebp + 0x14]
        push ebx
        push esi
        mov esi, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ecx]
        mov edx, dword ptr [ebp + 0x18]
        mov dword ptr [ebp - 4], eax
        mov eax, dword ptr [ecx + esi*8 - 4]
        push edi
        lea eax, [eax + eax*2]
        shl eax, 4
        inc word ptr [eax + edx + 2]
        mov edi, dword ptr [CoasterDepthBuffer]
        lea eax, [eax + edx + 2]
        mov eax, dword ptr [ecx + esi*8 - 4]
        mov esi, dword ptr [CoasterSurfaceBits]
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + edx + 2]
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [CoasterSurfacePitch]
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 0xc]
        mov ebx, dword ptr [eax + 4]
        mov dword ptr [ebp + 0xc], ebx
        mov ebx, dword ptr [FilledTriangleCount]
        inc ebx
        mov dword ptr [FilledTriangleCount], ebx
        mov ebx, dword ptr [ebp + 8]
        mov eax, dword ptr [eax]
        mov ebx, dword ptr [ebx*4 + CoasterShadeTables]
        mov ax, word ptr [ebx + eax*2]
        mov ebx, dword ptr [ebp + 0x10]
        mov dword ptr [ebp + 8], eax
        mov eax, dword ptr [ecx + ebx*8 - 4]
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + edx + 2]
        mov dword ptr [ecx + ebx*8], eax
        mov eax, dword ptr [ebp - 0xc]
        imul eax, dword ptr [ebp - 4]
        shl eax, 1
        add esi, eax
        add edi, eax
        mov dword ptr [ebp - 0x14], esi
        mov dword ptr [ebp - 0x10], edi
L41fc41:
        mov eax, dword ptr [ecx + 4]
        add ecx, 8
        mov dword ptr [ebp + 0x14], ecx
        lea eax, [eax + eax*2]
        shl eax, 4
        add eax, edx
        mov esi, dword ptr [eax + 4]
        test esi, esi
        mov esi, dword ptr [eax + 8]
        je L41fc69
        mov eax, dword ptr [eax + 0x1c]
        sub esi, eax
        mov dword ptr [ebp - 0x28], eax
        mov dword ptr [ebp - 0x3c], esi
        jmp L41fc82
L41fc69:
        mov edi, dword ptr [eax + 0x1c]
        mov ebx, dword ptr [eax + 0xc]
        mov eax, dword ptr [eax + 0x20]
        sub esi, edi
        sub ebx, eax
        mov dword ptr [ebp - 0x50], edi
        mov dword ptr [ebp - 0x48], eax
        mov dword ptr [ebp - 0x64], esi
        mov dword ptr [ebp - 0x5c], ebx
L41fc82:
        mov eax, dword ptr [ebp - 4]
        mov esi, dword ptr [ecx]
        cmp eax, esi
        jge L41fd1a
        mov ecx, dword ptr [ebp - 0xc]
        lea edx, [ecx + ecx]
        mov dword ptr [ebp + 0x10], edx
L41fc98:
        inc dword ptr [ebp - 4]
        mov eax, dword ptr [ebp - 0x64]
        mov edx, dword ptr [ebp - 0x5c]
        mov ebx, dword ptr [ebp - 0x3c]
        add eax, dword ptr [ebp - 0x50]
        add edx, dword ptr [ebp - 0x48]
        add ebx, dword ptr [ebp - 0x28]
        mov dword ptr [ebp - 0x64], eax
        mov dword ptr [ebp - 0x5c], edx
        mov dword ptr [ebp - 0x3c], ebx
        mov ecx, ebx
        sub ecx, eax
        cmp ecx, 0x8000
        jns L41fcc4
        jmp L41fcf8
L41fcc4:
        sar eax, 0x10
        sar ebx, 0x10
        mov edi, dword ptr [ebp - 0x14]
        mov esi, dword ptr [ebp - 0x10]
        xchg eax, ebx
        sub ebx, eax
        lea edi, [edi + eax*2]
        lea esi, [esi + eax*2]
        mov ax, word ptr [ebp + 8]
L41fcdd:
        mov ecx, edx
        sar ecx, 0x10
        cmp cx, word ptr [esi + ebx*2]
        jb L41fcf0
        mov word ptr [edi + ebx*2], ax
        mov word ptr [esi + ebx*2], cx
L41fcf0:
        add edx, dword ptr [ebp + 0xc]
        add ebx, 1
        jle L41fcdd
L41fcf8:
        mov eax, dword ptr [ebp + 0x10]
        mov edi, dword ptr [ebp - 0x14]
        mov esi, dword ptr [ebp - 0x10]
        mov ecx, dword ptr [ebp + 0x14]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [ebp - 4]
        mov edx, dword ptr [ecx]
        cmp eax, edx
        mov dword ptr [ebp - 0x14], edi
        mov dword ptr [ebp - 0x10], esi
        jl L41fc98
        mov edx, dword ptr [ebp + 0x18]
L41fd1a:
        mov eax, dword ptr [ebp - 4]
        mov esi, dword ptr [ebp - 8]
        cmp eax, esi
        jl L41fc41
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Builds the 192-byte shade clamp table: ShadeClamp[i] = i clamped to 0..63 for i in -64..127. */
// FUNCTION: LEGOLAND 0x0041fd30
void InitShadeClampTable(void) {
    int i;
    for (i = 0; i < 16; i++) {
        ((unsigned int *)ShadeClampTable)[i] = 0;
    }
    ShadeClamp = &ShadeClampTable[0x40];
    for (i = 0x40; i <= 0x7f; i++) {
        ShadeClampTable[i] = (unsigned char)(i - 0x40);
    }
    for (i = 0; i < 16; i++) {
        ((unsigned int *)ShadeClampTableTop)[i] = 0x3f3f3f3f;
    }
}

/* Span filler (GouraudSpanFillers[0]): interpolates the shade along each span, clamps it through ShadeClamp and
 * looks the colour up in the triangle's shade table; no depth test. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x0041fd80
__declspec(naked) void FillSpansGouraud(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x64
        mov ecx, dword ptr [ebp + 0x14]
        push ebx
        push esi
        mov esi, dword ptr [ebp + 0x10]
        mov edx, dword ptr [ebp + 0x18]
        mov ebx, dword ptr [ecx]
        mov eax, dword ptr [ecx + esi*8 - 4]
        push edi
        mov dword ptr [ebp - 4], ebx
        mov dword ptr [ebp + 0x10], 0
        lea eax, [eax + eax*2]
        shl eax, 4
        inc word ptr [eax + edx + 2]
        mov edi, dword ptr [CoasterSurfaceBits]
        lea eax, [eax + edx + 2]
        mov eax, dword ptr [ecx + esi*8 - 4]
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + edx + 2]
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [CoasterDepthBuffer]
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [CoasterSurfacePitch]
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 8]
        mov eax, dword ptr [eax*4 + CoasterShadeTables]
        mov dword ptr [GouraudSpanState + 0xc], eax
        mov eax, dword ptr [FilledTriangleCount]
        inc eax
        mov dword ptr [FilledTriangleCount], eax
        mov eax, dword ptr [ecx + esi*8 - 4]
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + edx + 2]
        mov dword ptr [ecx + esi*8], eax
        mov esi, dword ptr [ebp + 0xc]
        mov eax, dword ptr [esi + 4]
        test eax, eax
        jge L41fe1b
        neg eax
        mov dword ptr [esi + 4], eax
        mov dword ptr [ebp + 0x10], 1
L41fe1b:
        mov eax, dword ptr [esi + 4]
        sar eax, 0x10
        mov dword ptr [GouraudSpanState], eax
        mov eax, dword ptr [esi + 4]
        shl eax, 0x10
        mov dword ptr [GouraudSpanState + 0x8], eax
        mov eax, dword ptr [ebp - 0xc]
        imul eax, ebx
        lea eax, [edi + eax*2]
        mov dword ptr [ebp - 0x14], eax
L41fe3d:
        mov eax, dword ptr [ecx + 4]
        add ecx, 8
        mov dword ptr [ebp + 0x14], ecx
        lea eax, [eax + eax*2]
        shl eax, 4
        add eax, edx
        mov esi, dword ptr [eax + 4]
        mov edi, dword ptr [eax + 0x1c]
        test esi, esi
        mov esi, dword ptr [eax + 8]
        je L41fe73
        mov ebx, dword ptr [eax + 0xc]
        mov eax, dword ptr [eax + 0x20]
        sub esi, edi
        sub ebx, eax
        mov dword ptr [ebp - 0x28], edi
        mov dword ptr [ebp - 0x24], eax
        mov dword ptr [ebp - 0x3c], esi
        mov dword ptr [ebp - 0x38], ebx
        jmp L41fe89
L41fe73:
        mov ebx, dword ptr [eax + 0xc]
        mov eax, dword ptr [eax + 0x20]
        sub esi, edi
        sub ebx, eax
        mov dword ptr [ebp - 0x50], edi
        mov dword ptr [ebp - 0x4c], eax
        mov dword ptr [ebp - 0x64], esi
        mov dword ptr [ebp - 0x60], ebx
L41fe89:
        mov eax, dword ptr [ebp - 4]
        mov esi, dword ptr [ecx]
        cmp eax, esi
        jge L41ff64
        mov ecx, dword ptr [ebp - 0xc]
        lea edx, [ecx + ecx]
        mov dword ptr [ebp + 8], edx
L41fe9f:
        inc dword ptr [ebp - 4]
        mov eax, dword ptr [ebp - 0x64]
        mov ebx, dword ptr [ebp - 0x3c]
        add eax, dword ptr [ebp - 0x50]
        add ebx, dword ptr [ebp - 0x28]
        mov dword ptr [ebp - 0x64], eax
        mov dword ptr [ebp - 0x3c], ebx
        mov ecx, ebx
        sub ecx, eax
        cmp ecx, 0x8000
        jns L41fecb
        mov ecx, dword ptr [ebp - 0x60]
        add ecx, dword ptr [ebp - 0x4c]
        mov dword ptr [ebp - 0x60], ecx
        jmp L41ff46
L41fecb:
        sar eax, 0x10
        sar ebx, 0x10
        mov edi, dword ptr [ebp - 0x14]
        xchg eax, ebx
        sub ebx, eax
        lea edi, [edi + eax*2]
        mov ecx, dword ptr [ebp - 0x60]
        xor edx, edx
        add ecx, dword ptr [ebp - 0x4c]
        mov esi, dword ptr [GouraudSpanState + 0xc]
        mov dword ptr [ebp - 0x60], ecx
        ror ecx, 0x10
        mov eax, ecx
        and ecx, 0xffff
        and eax, 0xffff0000
        add ecx, dword ptr [ShadeClamp]
        cmp edx, dword ptr [ebp + 0x10]
        jne L41ff27
        xor edx, edx
L41ff08:
        mov dl, byte ptr [ecx]
        mov dx, word ptr [esi + edx*2]
        add eax, dword ptr [GouraudSpanState + 0x8]
        mov word ptr [edi + ebx*2], dx
        adc ecx, dword ptr [GouraudSpanState]
        xor edx, edx
        add ebx, 1
        jle L41ff08
        jmp L41ff46
L41ff27:
        xor edx, edx
L41ff29:
        mov dl, byte ptr [ecx]
        mov dx, word ptr [esi + edx*2]
        add eax, dword ptr [GouraudSpanState + 0x8]
        mov word ptr [edi + ebx*2], dx
        sbb ecx, dword ptr [GouraudSpanState]
        xor edx, edx
        add ebx, 1
        jle L41ff29
L41ff46:
        mov eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp - 0x14]
        mov edx, dword ptr [ebp + 0x14]
        add ecx, eax
        mov dword ptr [ebp - 0x14], ecx
        mov ecx, dword ptr [ebp - 4]
        cmp ecx, dword ptr [edx]
        jl L41fe9f
        mov ecx, edx
        mov edx, dword ptr [ebp + 0x18]
L41ff64:
        mov eax, dword ptr [ebp - 4]
        mov esi, dword ptr [ebp - 8]
        cmp eax, esi
        jl L41fe3d
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Span filler (GouraudSpanFillers[1]): Gouraud shading with the depth test of FillSpansFlatZ. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x0041ff80
__declspec(naked) void FillSpansGouraudZ(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x64
        mov ecx, dword ptr [ebp + 0x14]
        push ebx
        push esi
        mov esi, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ecx]
        mov edx, dword ptr [ebp + 0x18]
        mov dword ptr [ebp - 4], eax
        mov eax, dword ptr [ecx + esi*8 - 4]
        push edi
        mov dword ptr [ebp + 0x10], 0
        lea eax, [eax + eax*2]
        shl eax, 4
        inc word ptr [eax + edx + 2]
        mov edi, dword ptr [CoasterSurfaceBits]
        lea eax, [eax + edx + 2]
        mov ebx, dword ptr [CoasterDepthBuffer]
        mov eax, dword ptr [ecx + esi*8 - 4]
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + edx + 2]
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [CoasterSurfacePitch]
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 8]
        mov eax, dword ptr [eax*4 + CoasterShadeTables]
        mov dword ptr [GouraudSpanState + 0xc], eax
        mov eax, dword ptr [FilledTriangleCount]
        inc eax
        mov dword ptr [FilledTriangleCount], eax
        mov eax, dword ptr [ecx + esi*8 - 4]
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + edx + 2]
        mov dword ptr [ecx + esi*8], eax
        mov eax, dword ptr [ebp + 0xc]
        mov esi, dword ptr [eax + 4]
        test esi, esi
        jge L420019
        neg esi
        mov dword ptr [eax + 4], esi
        mov dword ptr [ebp + 0x10], 1
L420019:
        mov esi, dword ptr [eax + 4]
        sar esi, 0x10
        mov dword ptr [GouraudSpanState], esi
        mov esi, dword ptr [eax + 4]
        and esi, 0xffffff00
        shl esi, 0x10
        mov dword ptr [GouraudSpanState + 0x8], esi
        mov eax, dword ptr [eax + 8]
        sar eax, 8
        and eax, 0xffffff
        or esi, eax
        mov eax, dword ptr [ebp - 0xc]
        imul eax, dword ptr [ebp - 4]
        shl eax, 1
        add edi, eax
        add ebx, eax
        mov dword ptr [GouraudSpanState + 0x8], esi
        mov dword ptr [ebp - 0x14], edi
        mov dword ptr [ebp - 0x10], ebx
L42005d:
        mov eax, dword ptr [ecx + 4]
        add ecx, 8
        mov dword ptr [ebp + 0x14], ecx
        lea eax, [eax + eax*2]
        shl eax, 4
        add eax, edx
        mov esi, dword ptr [eax + 4]
        mov edi, dword ptr [eax + 0x1c]
        test esi, esi
        mov esi, dword ptr [eax + 8]
        je L420093
        mov ebx, dword ptr [eax + 0xc]
        mov eax, dword ptr [eax + 0x20]
        sub esi, edi
        sub ebx, eax
        mov dword ptr [ebp - 0x28], edi
        mov dword ptr [ebp - 0x24], eax
        mov dword ptr [ebp - 0x3c], esi
        mov dword ptr [ebp - 0x38], ebx
        jmp L4200c0
L420093:
        mov ebx, dword ptr [eax + 0xc]
        sub esi, edi
        mov dword ptr [ebp - 0x60], ebx
        mov ebx, dword ptr [eax + 0x20]
        mov dword ptr [ebp - 0x50], edi
        mov edi, dword ptr [ebp - 0x60]
        mov dword ptr [ebp - 0x4c], ebx
        mov ebx, dword ptr [eax + 0x10]
        mov eax, dword ptr [eax + 0x24]
        mov dword ptr [ebp - 0x64], esi
        mov esi, dword ptr [ebp - 0x4c]
        mov dword ptr [ebp - 0x48], eax
        sub edi, esi
        sub ebx, eax
        mov dword ptr [ebp - 0x60], edi
        mov dword ptr [ebp - 0x5c], ebx
L4200c0:
        mov eax, dword ptr [ebp - 4]
        mov esi, dword ptr [ecx]
        cmp eax, esi
        jge L4201e4
        mov ecx, dword ptr [ebp - 0xc]
        lea edx, [ecx + ecx]
        mov dword ptr [ebp + 8], edx
L4200d6:
        inc dword ptr [ebp - 4]
        mov eax, dword ptr [ebp - 0x64]
        mov edx, dword ptr [ebp - 0x5c]
        mov ebx, dword ptr [ebp - 0x3c]
        add eax, dword ptr [ebp - 0x50]
        add edx, dword ptr [ebp - 0x48]
        add ebx, dword ptr [ebp - 0x28]
        mov dword ptr [ebp - 0x64], eax
        mov dword ptr [ebp - 0x5c], edx
        mov dword ptr [ebp - 0x3c], ebx
        mov ecx, ebx
        sub ecx, eax
        cmp ecx, 0x8000
        jns L42010e
        mov ecx, dword ptr [ebp - 0x60]
        add ecx, dword ptr [ebp - 0x4c]
        mov dword ptr [ebp - 0x60], ecx
        jmp L4201be
L42010e:
        sar eax, 0x10
        sar ebx, 0x10
        mov edi, dword ptr [ebp - 0x14]
        mov esi, dword ptr [ebp - 0x10]
        xchg eax, ebx
        sub ebx, eax
        lea edi, [edi + eax*2]
        lea esi, [esi + eax*2]
        mov ecx, dword ptr [ebp - 0x60]
        mov eax, edx
        add ecx, dword ptr [ebp - 0x4c]
        sar eax, 8
        mov dword ptr [ebp - 0x60], ecx
        and eax, 0xffffff
        xor edx, edx
        sar ecx, 0x10
        add ecx, dword ptr [ShadeClamp]
        cmp edx, dword ptr [ebp + 0x10]
        jne L420184
        push ebp
        mov ebp, dword ptr [GouraudSpanState + 0xc]
        xor edx, edx
L42014f:
        mov edx, eax
        sar edx, 8
        cmp dx, word ptr [esi + ebx*2]
        jb L42016b
        mov word ptr [esi + ebx*2], dx
        xor edx, edx
        mov dl, byte ptr [ecx]
        mov dx, word ptr [ebp + edx*2]
        mov word ptr [edi + ebx*2], dx
L42016b:
        add eax, dword ptr [GouraudSpanState + 0x8]
        adc ecx, dword ptr [GouraudSpanState]
        and eax, 0xfeffffff
        add ebx, 1
        jle L42014f
        pop ebp
        jmp L4201be
L420184:
        push ebp
        mov ebp, dword ptr [GouraudSpanState + 0xc]
L42018b:
        mov edx, eax
        sar edx, 8
        cmp dx, word ptr [esi + ebx*2]
        jb L4201a7
        mov word ptr [esi + ebx*2], dx
        xor edx, edx
        mov dl, byte ptr [ecx]
        mov dx, word ptr [ebp + edx*2]
        mov word ptr [edi + ebx*2], dx
L4201a7:
        add eax, dword ptr [GouraudSpanState + 0x8]
        sbb ecx, dword ptr [GouraudSpanState]
        and eax, 0xfeffffff
        add ebx, 1
        jle L42018b
        pop ebp
L4201be:
        mov eax, dword ptr [ebp + 8]
        mov edi, dword ptr [ebp - 0x14]
        mov esi, dword ptr [ebp - 0x10]
        mov ecx, dword ptr [ebp + 0x14]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [ebp - 4]
        mov edx, dword ptr [ecx]
        cmp eax, edx
        mov dword ptr [ebp - 0x14], edi
        mov dword ptr [ebp - 0x10], esi
        jl L4200d6
        mov edx, dword ptr [ebp + 0x18]
L4201e4:
        mov eax, dword ptr [ebp - 4]
        mov esi, dword ptr [ebp - 8]
        cmp eax, esi
        jl L42005d
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

#pragma optimize("", off)
/* Simpson integration of fn over [a, b] (floats passed as bits), doubling the number of intervals until two
 * estimates agree to tol (relative). */
// FUNCTION: LEGOLAND 0x00420200
float IntegrateSimpson(float (*fn)(unsigned int), unsigned int a, unsigned int b, float tol) {
    int it2;
    int iib;
    float hh_;
    float s_;
    float add0;
    float pr0;
    float area0;
    float strideb;
    float cxxb;

    it2 = 1;
    hh_ = *(float *)&b - *(float *)&a;
    s_ = fn(a) + fn(b);
    add0 = 0;
    area0 = s_ * hh_ * 0.5f * FLOAT_004ab43c;
    do {
        pr0 = area0;
        s_ = s_ - 2.0f * add0;
        add0 = 0;
        strideb = hh_;
        hh_ = 0.5f * hh_;
        cxxb = hh_ + *(float *)&a;
        for (iib = 1; iib <= it2; iib++) {
            add0 = fn(*(unsigned int *)&cxxb) + add0;
            cxxb = cxxb + strideb;
        }
        s_ = 4.0f * add0 + s_;
        area0 = s_ * hh_;
        it2 = it2 + it2;
    } while (fabs(area0 - pr0) > tol * fabs(pr0));
    return FLOAT_004ab438 * area0;
}
#pragma optimize("", on)

/* One classic 4th-order Runge-Kutta step of size c from time *p (coefficients RK4Coefficients); advances *p. */
// FUNCTION: LEGOLAND 0x00420310
void IntegratorStepRK4(struct Integrator *ar, float *p, float c) {
    struct FloatArray **h;
    struct FloatArray *v0;
    struct FloatArray *v1;
    struct FloatArray *v2;
    struct FloatArray *v3;
    int i;

    h = ar->math.alloc(4);
    v0 = h[0];
    v1 = h[1];
    v2 = h[2];
    v3 = h[3];
    ar->math.zero(v2, ar->math.dim);
    ar->load(ar, v0);
    ar->load(ar, v1);
    for (i = 0; i <= 3; i++) {
        ar->math.blend(v0, 1.0f, v2, RK4Coefficients[i][0], v3);
        ar->store(ar, v3);
        ar->deriv(ar, *p + c * RK4Coefficients[i][0], v2);
        ar->math.scale(v2, c);
        ar->math.axpy(v2, RK4Coefficients[i][1], v1);
    }
    ar->store(ar, v1);
    *p += c;
    ar->math.release(h, 4);
}

/* Default integrator store: copies param into the state array. */
// FUNCTION: LEGOLAND 0x004203d0
void IntegratorStoreState(struct Integrator *self, struct FloatArray *param) {
    self->math.copy(param, self->state);
}

/* Default integrator load: copies the state array into param. */
// FUNCTION: LEGOLAND 0x004203f0
unsigned int IntegratorLoadState(struct Integrator *self, struct FloatArray *param) {
    return ((unsigned int (*)(void *, struct FloatArray *))self->math.copy)(self->state, param);
}

/* Sets up an integrator for 2-value states with the default load/store. */
// FUNCTION: LEGOLAND 0x00420410
void InitIntegrator(struct Integrator *param, unsigned int count) {
    struct Integrator *host = param;
    InitVecMath(&host->math, 2);
    host->store = IntegratorStoreState;
    host->load = IntegratorLoadState;
}

/* Loads the coaster's palette (Rollercoaster.lpt), the object and texture name lists, then every .lms model, .lfm
 * material file and .ltx texture named in them. */
// FUNCTION: LEGOLAND 0x00420440
void LoadRollercoasterFiles(void) {
    char buf[256];
    int i;
    int n;

    RollercoasterLpt = LoadRollercoasterLpt();
    // STRING: LEGOLAND 0x004b56a0
    SetCurrentDirectoryIfNotNull("RollerCoaster\\RollerCoaster\\CreatedData");
    // STRING: LEGOLAND 0x004b5690
    LoadObjAndTxtFiles("ROLLERCOASTER");
    // STRING: LEGOLAND 0x004b5684
    SetCurrentDirectoryIfNotNull("..\\..\\..");
    n = GetCoasterObjLineCount();
    for (i = 0; i < n; i++) {
        GetCoasterObjLine(i, buf);
        LmsFileTable[i] = LoadLmsFile(buf);
    }
    n = GetCoasterObjLineCount();
    for (i = 0; i < n; i++) {
        GetCoasterObjLine(i, buf);
        LfmFileTable[i] = LoadLfmFile(buf, &LfmFileSizes[i]);
    }
    n = GetCoasterTxtLineCount();
    for (i = 0; i < n; i++) {
        FormatRollercoasterFileName(i, buf);
        LtxFileTable[i] = LoadLtxFile(buf);
    }
}

/* SetCurrentDirectoryA unless param is NULL. */
// FUNCTION: LEGOLAND 0x00420530
unsigned int SetCurrentDirectoryIfNotNull(const char *param) {
    if (param == NULL) {
        return 0;
    }
    return SetCurrentDirectoryA(param);
}

/* Reads a whole file from RollerCoaster/RollerCoaster/CreatedData into a new block (size in *sizeOut); NULL on any
 * failure. */
// FUNCTION: LEGOLAND 0x00420550
void *LoadCreatedDataFile(const char *fileName, unsigned int *sizeOut) {
    HANDLE hFile;
    unsigned int fileSize;
    void *buffer;
    DWORD bytesRead;

    if (fileName == 0) {
        return 0;
    }

    // STRING: LEGOLAND 0x004b56a0
    SetCurrentDirectoryIfNotNull("RollerCoaster\\RollerCoaster\\CreatedData");
    hFile = CreateFileA(fileName, 0x80000000, 0x1, 0, 0x3, 0x8000000, 0);
    if (hFile == (HANDLE)-1) {
        // STRING: LEGOLAND 0x004b5684
        SetCurrentDirectoryIfNotNull("..\\..\\..");
        return 0;
    }

    fileSize = GetFileSize(hFile, 0);
    buffer = MemAlloc(fileSize, 0, (unsigned int)(UINT_PTR)EmptyString, 0);
    if (buffer == 0) {
        CloseHandle(hFile);
        SetCurrentDirectoryIfNotNull("..\\..\\..");
        return 0;
    }

    ReadFile(hFile, buffer, fileSize, &bytesRead, 0);
    if (bytesRead != fileSize) {
        MemFree(buffer);
        CloseHandle(hFile);
        SetCurrentDirectoryIfNotNull("..\\..\\..");
        return 0;
    }

    CloseHandle(hFile);
    SetCurrentDirectoryIfNotNull("..\\..\\..");
    if (sizeOut != 0) {
        *sizeOut = fileSize;
    }
    return buffer;
}

/* Loads name.lms and turns its six section offsets into pointers. */
// FUNCTION: LEGOLAND 0x00420640
struct LmsFile *LoadLmsFile(const char *name) {
    char buffer[256];
    struct LmsFile *f;
    // STRING: LEGOLAND 0x004b56c8
    wsprintfA(buffer, "%s.lms", name);
    f = LoadCreatedDataFile(buffer, 0);
    if (f != 0) {
        *(UINT_PTR *)&f->field_c += (UINT_PTR)f;
        *(UINT_PTR *)&f->field_14 += (UINT_PTR)f;
        *(UINT_PTR *)&f->field_10 += (UINT_PTR)f;
        *(UINT_PTR *)&f->flat_tris += (UINT_PTR)f;
        *(UINT_PTR *)&f->gouraud_tris += (UINT_PTR)f;
        *(UINT_PTR *)&f->tex_tris += (UINT_PTR)f;
        return f;
    }
    return 0;
}

/* The loaded .lms of the named coaster object. */
// FUNCTION: LEGOLAND 0x004206b0
struct LmsFile *GetLmsByName(const char *s) {
    unsigned int index = FindCoasterObjLine(s);
    if (index != 0xffffffff) {
        return LmsFileTable[index];
    }
    return 0;
}

/* Loads name.lfm (the model's per-triangle material records). */
// FUNCTION: LEGOLAND 0x004206d0
struct LmsRec *LoadLfmFile(const char *name, unsigned int *param2) {
    char buffer[256];
    // STRING: LEGOLAND 0x004b56d0
    wsprintfA(buffer, "%s.lfm", name);
    return LoadCreatedDataFile(buffer, param2);
}

/* The loaded .lfm of the named coaster object. */
// FUNCTION: LEGOLAND 0x00420710
struct LmsRec *GetLfmByName(const char *s) {
    unsigned int index = FindCoasterObjLine(s);
    if (index != 0xffffffff) {
        return LfmFileTable[index];
    }
    return 0;
}

/* Size of the named object's .lfm. */
// FUNCTION: LEGOLAND 0x00420730
unsigned int GetLfmSizeByName(const char *s) {
    unsigned int index = FindCoasterObjLine(s);
    if (index != 0xffffffff) {
        return LfmFileSizes[index];
    }
    return 0;
}

/* Loads name.ltx (a texture). */
// FUNCTION: LEGOLAND 0x00420750
void *LoadLtxFile(const char *name) {
    char buffer[256];
    // STRING: LEGOLAND 0x004b56d8
    wsprintfA(buffer, "%s.ltx", name);
    return LoadCreatedDataFile(buffer, 0);
}

/* The loaded .ltx at index param. */
// FUNCTION: LEGOLAND 0x00420780
void *GetLtxFileTableEntry(unsigned int param) {
    return LtxFileTable[param];
}

/* Index of the named texture in the coaster's texture list (the LtxFileTable index). */
// FUNCTION: LEGOLAND 0x00420790
unsigned int FindLtxIndexByName(const char *param) {
    return FindCoasterTxtLine(param);
}

/* Loads Rollercoaster.lpt: a count, then the palette's 24-bit colours. */
// FUNCTION: LEGOLAND 0x004207a0
int *LoadRollercoasterLpt(void) {
    // STRING: LEGOLAND 0x004b56e0
    return LoadCreatedDataFile("Rollercoaster.lpt", 0);
}

/* The loaded palette (Rollercoaster.lpt). */
// FUNCTION: LEGOLAND 0x004207c0
int *GetRollercoasterLpt(void) {
    return RollercoasterLpt;
}

/* Palette index of the 24-bit colour key, or -1. */
// FUNCTION: LEGOLAND 0x004207d0
int FindRollercoasterLptEntry(unsigned int key) {
    int *table = RollercoasterLpt;
    int count = table[0];
    int i;
    int *p = table + 1;
    for (i = 0; i < count; i++) {
        if (((*p++ ^ key) & 0xffffff) == 0) {
            return i;
        }
    }
    return -1;
}

/* Draws an LmsFile's flat triangles from the transformed vertices: back-face culls each, then RasteriseTriangle
 * with FlatSpanFillers and the triangle's colour from the .lfm. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00420810
__declspec(naked) void DrawLmsFlatTriangles(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0xcc
        push ebx
        push esi
        push edi
        push eax
        push edx
        rdtsc
        mov dword ptr [ebp - 8], eax
        pop edx
        pop eax
        mov edx, dword ptr [ebp + 0x10]
        mov edi, dword ptr [ebp + 8]
        xor ecx, ecx
        xor eax, eax
        cmp edx, ecx
        lea edx, [ebp - 0xcc]
        setne al
        mov dword ptr [ebp - 0x4c], eax
        lea eax, [ebp - 0xb0]
        mov dword ptr [ebp - 0x20], eax
        mov eax, dword ptr [edi + 0x1c]
        mov dword ptr [ebp - 0x24], edx
        lea edx, [ebp - 0x94]
        cmp eax, ecx
        mov dword ptr [ebp - 0x1c], edx
        mov dword ptr [ebp - 0x14], offset FlatSpanFillers
        mov dword ptr [ebp + 0x10], ecx
        jle L4209f1
        mov dword ptr [ebp - 0xc], ecx
L42086a:
        add ecx, dword ptr [edi + 0x18]
        movsx edx, word ptr [ecx + 6]
        movsx eax, word ptr [ecx + 4]
        mov dword ptr [ebp - 0x58], edx
        mov dword ptr [ebp - 0x5c], eax
        mov dword ptr [ebp - 0x50], eax
        shl edx, 4
        movsx esi, word ptr [ecx + 8]
        shl eax, 4
        mov edi, edx
        mov edx, eax
        mov dword ptr [ebp - 0x54], esi
        mov eax, dword ptr [edx + EmptyString + 0x8]
        mov ebx, dword ptr [edi + EmptyString + 0x8]
        sub ebx, eax
        mov eax, dword ptr [edx + EmptyString + 0xc]
        mov dword ptr [ebp - 4], ebx
        mov ebx, dword ptr [edi + EmptyString + 0xc]
        fild dword ptr [ebp - 4]
        sub ebx, eax
        shl esi, 4
        fstp dword ptr [ebp - 0x38]
        mov dword ptr [ebp - 4], ebx
        mov ebx, dword ptr [esi + EmptyString + 0x8]
        fild dword ptr [ebp - 4]
        sub ebx, dword ptr [edx + EmptyString + 0x8]
        fstp dword ptr [ebp - 0x30]
        mov dword ptr [ebp - 4], ebx
        mov ebx, dword ptr [esi + EmptyString + 0xc]
        fild dword ptr [ebp - 4]
        sub ebx, eax
        mov dword ptr [ebp - 4], ebx
        fstp dword ptr [ebp - 0x34]
        fild dword ptr [ebp - 4]
        fst dword ptr [ebp - 0x2c]
        fmul dword ptr [ebp - 0x38]
        fld dword ptr [ebp - 0x34]
        fmul dword ptr [ebp - 0x30]
        fsubp st(1), st
        fst dword ptr [ebp - 0x28]
        fcomp dword ptr [FLOAT_004ab390]
        fnstsw ax
        test ah, 0x41
        jne L4209d3
        mov eax, dword ptr [edx + EmptyString + 0x14]
        mov edi, dword ptr [edi + EmptyString + 0x14]
        mov esi, dword ptr [esi + EmptyString + 0x14]
        mov edx, eax
        or eax, edi
        and edx, 0xff
        or eax, esi
        and edx, edi
        mov dword ptr [ebp - 0x40], eax
        and eax, 0xf
        and edx, esi
        cmp al, 0xf
        mov dword ptr [ebp - 0x44], edx
        jne L4209d3
        movsx eax, word ptr [ecx + 2]
        fld dword ptr [LmsLightDir + 0x8]
        mov edi, dword ptr [ebp + 8]
        lea eax, [eax + eax*2]
        mov edx, dword ptr [edi + 0x10]
        fmul dword ptr [edx + eax*4 + 8]
        fld dword ptr [LmsLightDir + 0x4]
        fmul dword ptr [edx + eax*4 + 4]
        lea eax, [edx + eax*4]
        faddp st(1), st
        fld dword ptr [LmsLightDir]
        fmul dword ptr [eax]
        faddp st(1), st
        fadd dword ptr [CastleLightBias]
        fstp dword ptr [ebp - 4]
        fld dword ptr [ebp - 4]
        fistp dword ptr [ebp - 0x10]
        mov eax, dword ptr [ebp - 0x10]
        mov edx, dword ptr [ebp + 0xc]
        mov dword ptr [ebp - 0x3c], eax
        mov esi, 3
        movsx eax, word ptr [ecx]
        lea ecx, [eax + eax*2]
        mov eax, dword ptr [edx + ecx*4]
        lea edx, [ebp - 0x5c]
        mov dword ptr [ebp - 0x48], eax
        lea ecx, [ebp - 0xc4]
L42099a:
        mov eax, dword ptr [edx]
        add edx, 4
        shl eax, 4
        add ecx, 0x1c
        dec esi
        mov ebx, dword ptr [eax + EmptyString + 0xc]
        mov dword ptr [ecx - 0x20], ebx
        mov ebx, dword ptr [eax + EmptyString + 0x8]
        mov eax, dword ptr [eax + EmptyString + 0x10]
        mov dword ptr [ecx - 0x1c], ebx
        mov dword ptr [ecx - 0x18], eax
        jne L42099a
        lea ecx, [ebp - 0x4c]
        push ecx
        push 2
        call RasteriseTriangle
        add esp, 8
        jmp L4209d6
L4209d3:
        mov edi, dword ptr [ebp + 8]
L4209d6:
        mov eax, dword ptr [ebp + 0x10]
        mov ecx, dword ptr [ebp - 0xc]
        mov edx, dword ptr [edi + 0x1c]
        inc eax
        add ecx, 0x10
        cmp eax, edx
        mov dword ptr [ebp + 0x10], eax
        mov dword ptr [ebp - 0xc], ecx
        jl L42086a
L4209f1:
        push eax
        push edx
        rdtsc
        sub eax, dword ptr [ebp - 8]
        mov dword ptr [ebp - 8], eax
        pop edx
        pop eax
        mov edx, dword ptr [ebp - 8]
        mov eax, dword ptr [LmsDrawCycles]
        add eax, edx
        pop edi
        pop esi
        mov dword ptr [LmsDrawCycles], eax
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Draws an LmsFile's Gouraud triangles (GouraudSpanFillers). */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00420a20
__declspec(naked) void DrawLmsGouraudTriangles(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0xd0
        push ebx
        push esi
        push edi
        push eax
        push edx
        rdtsc
        mov dword ptr [ebp - 4], eax
        pop edx
        pop eax
        mov esi, dword ptr [ebp + 0x10]
        xor edx, edx
        xor eax, eax
        cmp esi, edx
        setne al
        mov dword ptr [ebp - 0x50], eax
        lea ecx, [ebp - 0xd0]
        lea eax, [ebp - 0xb4]
        mov dword ptr [ebp - 0x28], ecx
        mov dword ptr [ebp - 0x24], eax
        mov eax, dword ptr [ebp + 8]
        lea ecx, [ebp - 0x98]
        mov dword ptr [ebp - 0x18], offset GouraudSpanFillers
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [eax + 0x24]
        cmp ecx, edx
        mov dword ptr [ebp - 8], edx
        jle L420c10
        mov dword ptr [ebp - 0xc], edx
L420a7a:
        mov ebx, dword ptr [eax + 0x20]
        movsx eax, word ptr [edx + ebx + 4]
        movsx ecx, word ptr [edx + ebx + 6]
        add edx, ebx
        mov dword ptr [ebp - 0x5c], ecx
        mov dword ptr [ebp - 0x60], eax
        mov dword ptr [ebp - 0x54], eax
        shl ecx, 4
        movsx esi, word ptr [edx + 8]
        shl eax, 4
        mov edi, ecx
        mov ecx, eax
        mov dword ptr [ebp - 0x58], esi
        mov eax, dword ptr [ecx + EmptyString + 0x8]
        mov ebx, dword ptr [edi + EmptyString + 0x8]
        sub ebx, eax
        mov eax, dword ptr [ecx + EmptyString + 0xc]
        mov dword ptr [ebp + 0x10], ebx
        mov ebx, dword ptr [edi + EmptyString + 0xc]
        fild dword ptr [ebp + 0x10]
        sub ebx, eax
        shl esi, 4
        fstp dword ptr [ebp - 0x3c]
        mov dword ptr [ebp + 0x10], ebx
        mov ebx, dword ptr [esi + EmptyString + 0x8]
        fild dword ptr [ebp + 0x10]
        sub ebx, dword ptr [ecx + EmptyString + 0x8]
        fstp dword ptr [ebp - 0x34]
        mov dword ptr [ebp + 0x10], ebx
        mov ebx, dword ptr [esi + EmptyString + 0xc]
        fild dword ptr [ebp + 0x10]
        sub ebx, eax
        mov dword ptr [ebp + 0x10], ebx
        fstp dword ptr [ebp - 0x38]
        fild dword ptr [ebp + 0x10]
        fst dword ptr [ebp - 0x30]
        fmul dword ptr [ebp - 0x3c]
        fld dword ptr [ebp - 0x38]
        fmul dword ptr [ebp - 0x34]
        fsubp st(1), st
        fst dword ptr [ebp - 0x2c]
        fcomp dword ptr [FLOAT_004ab390]
        fnstsw ax
        test ah, 0x41
        jne L420bf2
        mov eax, dword ptr [ecx + EmptyString + 0x14]
        mov edi, dword ptr [edi + EmptyString + 0x14]
        mov esi, dword ptr [esi + EmptyString + 0x14]
        mov ecx, eax
        or eax, edi
        and ecx, 0xff
        or eax, esi
        and ecx, edi
        mov dword ptr [ebp - 0x44], eax
        and eax, 0xf
        and ecx, esi
        cmp al, 0xf
        mov dword ptr [ebp - 0x48], ecx
        jne L420bf2
        lea edi, [ebp - 0x60]
        lea ecx, [ebp - 0xc8]
        lea esi, [edx + 0xa]
        mov dword ptr [ebp + 0x10], 3
L420b5f:
        movsx eax, word ptr [esi]
        fld dword ptr [LmsLightDir + 0x8]
        mov ebx, dword ptr [ebp + 8]
        lea eax, [eax + eax*2]
        mov ebx, dword ptr [ebx + 0x14]
        fmul dword ptr [ebx + eax*4 + 8]
        fld dword ptr [LmsLightDir + 0x4]
        fmul dword ptr [ebx + eax*4 + 4]
        lea eax, [ebx + eax*4]
        faddp st(1), st
        fld dword ptr [LmsLightDir]
        fmul dword ptr [eax]
        faddp st(1), st
        fadd dword ptr [CastleLightBias]
        fstp dword ptr [ebp - 0x10]
        fld dword ptr [ebp - 0x10]
        fistp dword ptr [ebp - 0x14]
        mov eax, dword ptr [edi]
        add esi, 2
        shl eax, 4
        add edi, 4
        add ecx, 0x1c
        mov ebx, dword ptr [eax + EmptyString + 0xc]
        mov dword ptr [ecx - 0x20], ebx
        mov ebx, dword ptr [eax + EmptyString + 0x8]
        mov eax, dword ptr [eax + EmptyString + 0x10]
        mov dword ptr [ecx - 0x1c], ebx
        mov ebx, dword ptr [ebp - 0x14]
        mov dword ptr [ecx - 0x18], ebx
        mov dword ptr [ecx - 0x14], eax
        mov eax, dword ptr [ebp + 0x10]
        dec eax
        mov dword ptr [ebp + 0x10], eax
        jne L420b5f
        movsx eax, word ptr [edx]
        mov edx, dword ptr [ebp + 0xc]
        lea ecx, [eax + eax*2]
        mov eax, dword ptr [edx + ecx*4]
        lea ecx, [ebp - 0x50]
        push ecx
        push 3
        mov dword ptr [ebp - 0x4c], eax
        call RasteriseTriangle
        add esp, 8
L420bf2:
        mov eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp - 8]
        mov edx, dword ptr [ebp - 0xc]
        inc ecx
        mov esi, dword ptr [eax + 0x24]
        add edx, 0x10
        cmp ecx, esi
        mov dword ptr [ebp - 8], ecx
        mov dword ptr [ebp - 0xc], edx
        jl L420a7a
L420c10:
        push eax
        push edx
        rdtsc
        sub eax, dword ptr [ebp - 4]
        mov dword ptr [ebp - 4], eax
        pop edx
        pop eax
        mov edx, dword ptr [ebp - 4]
        mov eax, dword ptr [LmsDrawCycles]
        add eax, edx
        pop edi
        pop esi
        mov dword ptr [LmsDrawCycles], eax
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Draws an LmsFile's textured triangles (TexturedSpanFillers). */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00420c40
__declspec(naked) void DrawLmsTexturedTriangles(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0xd0
        push ebx
        push esi
        push edi
        push eax
        push edx
        rdtsc
        mov dword ptr [ebp - 8], eax
        pop edx
        pop eax
        mov edx, dword ptr [ebp + 0x10]
        xor esi, esi
        xor eax, eax
        cmp edx, esi
        setne al
        mov dword ptr [ebp - 0x50], eax
        lea eax, [ebp - 0x98]
        mov dword ptr [ebp - 0x20], eax
        mov eax, dword ptr [ebp + 8]
        lea ecx, [ebp - 0xd0]
        lea edx, [ebp - 0xb4]
        mov dword ptr [ebp - 0x28], ecx
        mov ecx, dword ptr [eax + 0x2c]
        cmp ecx, esi
        mov dword ptr [ebp - 0x24], edx
        mov dword ptr [ebp - 0x18], offset TexturedSpanFillers
        mov dword ptr [ebp - 0x10], esi
        jle L420e5f
        mov dword ptr [ebp - 0xc], esi
L420c9a:
        mov ebx, dword ptr [eax + 0x28]
        movsx eax, word ptr [esi + ebx + 4]
        movsx ecx, word ptr [esi + ebx + 6]
        add esi, ebx
        mov dword ptr [ebp - 0x5c], ecx
        mov dword ptr [ebp - 0x60], eax
        mov dword ptr [ebp - 0x54], eax
        shl ecx, 4
        movsx edx, word ptr [esi + 8]
        shl eax, 4
        mov edi, ecx
        mov ecx, eax
        mov dword ptr [ebp - 0x58], edx
        mov eax, dword ptr [ecx + EmptyString + 0x8]
        mov ebx, dword ptr [edi + EmptyString + 0x8]
        sub ebx, eax
        mov eax, dword ptr [ecx + EmptyString + 0xc]
        mov dword ptr [ebp + 0x10], ebx
        mov ebx, dword ptr [edi + EmptyString + 0xc]
        fild dword ptr [ebp + 0x10]
        sub ebx, eax
        shl edx, 4
        fstp dword ptr [ebp - 0x3c]
        mov dword ptr [ebp + 0x10], ebx
        mov ebx, dword ptr [edx + EmptyString + 0x8]
        fild dword ptr [ebp + 0x10]
        sub ebx, dword ptr [ecx + EmptyString + 0x8]
        fstp dword ptr [ebp - 0x34]
        mov dword ptr [ebp + 0x10], ebx
        mov ebx, dword ptr [edx + EmptyString + 0xc]
        fild dword ptr [ebp + 0x10]
        sub ebx, eax
        mov dword ptr [ebp + 0x10], ebx
        fstp dword ptr [ebp - 0x38]
        fild dword ptr [ebp + 0x10]
        fst dword ptr [ebp - 0x30]
        fmul dword ptr [ebp - 0x3c]
        fld dword ptr [ebp - 0x38]
        fmul dword ptr [ebp - 0x34]
        fsubp st(1), st
        fst dword ptr [ebp - 0x2c]
        fcomp dword ptr [FLOAT_004ab390]
        fnstsw ax
        test ah, 0x41
        jne L420e41
        mov eax, dword ptr [ecx + EmptyString + 0x14]
        mov edi, dword ptr [edi + EmptyString + 0x14]
        mov edx, dword ptr [edx + EmptyString + 0x14]
        mov ecx, eax
        or eax, edi
        and ecx, 0xff
        or eax, edx
        and ecx, edi
        mov dword ptr [ebp - 0x44], eax
        and eax, 0xf
        and ecx, edx
        cmp al, 0xf
        mov dword ptr [ebp - 0x48], ecx
        jne L420e41
        movsx eax, word ptr [esi + 2]
        fld dword ptr [LmsLightDir + 0x8]
        mov edx, dword ptr [ebp + 8]
        lea ecx, [eax + eax*2]
        mov eax, dword ptr [edx + 0x10]
        fmul dword ptr [eax + ecx*4 + 8]
        fld dword ptr [LmsLightDir + 0x4]
        fmul dword ptr [eax + ecx*4 + 4]
        lea eax, [eax + ecx*4]
        faddp st(1), st
        fld dword ptr [LmsLightDir]
        fmul dword ptr [eax]
        faddp st(1), st
        fadd dword ptr [CastleLightBias]
        fstp dword ptr [ebp + 0x10]
        fld dword ptr [ebp + 0x10]
        fistp dword ptr [ebp - 0x14]
        mov ecx, dword ptr [ebp - 0x14]
        mov ebx, dword ptr [ebp + 0xc]
        mov dword ptr [ebp - 0x40], ecx
        xor edi, edi
        movsx eax, word ptr [esi]
        lea ecx, [ebp - 0x60]
        mov dword ptr [ebp + 0x10], edi
        lea edx, [eax + eax*2]
        mov dword ptr [ebp - 4], ecx
        mov eax, dword ptr [ebx + edx*4]
        mov dword ptr [ebp - 0x4c], eax
        lea eax, [ebp - 0xc8]
L420dd1:
        mov edx, dword ptr [ebp - 4]
        add eax, 0x1c
        mov ecx, dword ptr [edx]
        shl ecx, 4
        mov edx, dword ptr [ecx + EmptyString + 0xc]
        mov dword ptr [eax - 0x20], edx
        mov edx, dword ptr [ecx + EmptyString + 0x8]
        mov dword ptr [eax - 0x1c], edx
        mov ecx, dword ptr [ecx + EmptyString + 0x10]
        movsx edx, word ptr [esi]
        lea edx, [edx + edx*2]
        lea edi, [edi + edx*2]
        xor edx, edx
        mov dl, byte ptr [ebx + edi*2 + 4]
        mov edi, dword ptr [ebp + 0x10]
        mov dword ptr [eax - 0x18], edx
        movsx edx, word ptr [esi]
        lea edx, [edx + edx*2]
        lea edi, [edi + edx*2]
        xor edx, edx
        mov dl, byte ptr [ebx + edi*2 + 5]
        mov edi, dword ptr [ebp + 0x10]
        mov dword ptr [eax - 0x14], edx
        mov dword ptr [eax - 0x10], ecx
        mov ecx, dword ptr [ebp - 4]
        inc edi
        add ecx, 4
        cmp edi, 2
        mov dword ptr [ebp + 0x10], edi
        mov dword ptr [ebp - 4], ecx
        jle L420dd1
        lea edx, [ebp - 0x50]
        push edx
        push 4
        call RasteriseTriangle
        add esp, 8
L420e41:
        mov eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp - 0x10]
        mov esi, dword ptr [ebp - 0xc]
        inc ecx
        mov edx, dword ptr [eax + 0x2c]
        add esi, 0x10
        cmp ecx, edx
        mov dword ptr [ebp - 0x10], ecx
        mov dword ptr [ebp - 0xc], esi
        jl L420c9a
L420e5f:
        push eax
        push edx
        rdtsc
        sub eax, dword ptr [ebp - 8]
        mov dword ptr [ebp - 8], eax
        pop edx
        pop eax
        mov eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [LmsDrawCycles]
        add ecx, eax
        pop edi
        pop esi
        mov dword ptr [LmsDrawCycles], ecx
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Draws a coaster model at position c with rotation d: builds the model-to-screen matrix, transforms the vertices,
 * then draws the flat, Gouraud and textured triangle lists into the locked coaster surface. Adds its rdtsc cycles
 * to LmsDrawCycles. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00420e90
__declspec(naked) unsigned int DrawLmsModel(struct LmsFile *a, struct LmsRec *b, void *c, void *d, unsigned int e) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x9c
        push ebx
        push esi
        push edi
        push eax
        push edx
        rdtsc
        mov dword ptr [ebp - 4], eax
        pop edx
        pop eax
        mov esi, dword ptr [ebp + 8]
        test esi, esi
        je L420fa7
        mov edi, dword ptr [ebp + 0xc]
        test edi, edi
        je L420fa7
        mov ebx, dword ptr [ebp + 0x14]
        lea eax, [ebp - 0x5c]
        push eax
        push ebx
        call Mat3ToMat4Transpose
        lea ecx, [ebp - 0x5c]
        push 1
        push ecx
        push offset LmsLightDir
        push offset CastleLightDir
        call TransformPoints3x4
        fld dword ptr [LmsLightDir]
        fmul dword ptr [CastleLightScale]
        mov eax, dword ptr [ebp + 0x10]
        lea edx, [ebp - 0x5c]
        push edx
        push ebx
        push eax
        fstp dword ptr [LmsLightDir]
        fld dword ptr [LmsLightDir + 0x4]
        fmul dword ptr [CastleLightScale]
        fstp dword ptr [LmsLightDir + 0x4]
        fld dword ptr [LmsLightDir + 0x8]
        fmul dword ptr [CastleLightScale]
        fstp dword ptr [LmsLightDir + 0x8]
        call MakeMat4FromRotTrans
        lea ecx, [ebp - 0x9c]
        lea edx, [ebp - 0x5c]
        push ecx
        push edx
        push offset CastleWorldToScreen
        call Mat4Multiply
        mov eax, dword ptr [esi]
        mov edx, dword ptr [esi + 0xc]
        push eax
        lea ecx, [ebp - 0x9c]
        push 0x10
        push ecx
        push offset EmptyString + 0x8
        push edx
        call ProjectPoints
        add esp, 0x44
        push eax
        push edx
        rdtsc
        sub eax, dword ptr [ebp - 4]
        mov dword ptr [ebp - 4], eax
        pop edx
        pop eax
        mov eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [LmsDrawCycles]
        add ecx, eax
        mov dword ptr [LmsDrawCycles], ecx
        lea ecx, [ebp - 0x1c]
        push ecx
        call GetCoasterSurface
        add esp, 4
        test eax, eax
        je L420fa7
        mov ebx, dword ptr [ebp + 0x18]
        push ebx
        push edi
        push esi
        call DrawLmsFlatTriangles
        push ebx
        push edi
        push esi
        call DrawLmsGouraudTriangles
        push ebx
        push edi
        push esi
        call DrawLmsTexturedTriangles
        lea edx, [ebp - 0x1c]
        push edx
        call ReleaseCoasterSurface
        add esp, 0x28
L420fa7:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Screen rectangle of the model's bounding box at the given position and basis. */
// FUNCTION: LEGOLAND 0x00420fb0
unsigned int GetLmsBoxScreenRect(struct LmsFile *param_1, unsigned int *param_2, unsigned int *param_3, struct RectI *param_4) {
    return GetBoxScreenRect(param_1->box, param_2, param_3, param_4);
}

// GLOBAL: LEGOLAND 0x004b5648
void *FlatSpanFillers[2] = {FillSpansFlat, FillSpansFlatZ};

// GLOBAL: LEGOLAND 0x004b5658
void *GouraudSpanFillers[2] = {FillSpansGouraud, FillSpansGouraudZ};

// GLOBAL: LEGOLAND 0x004b5700
float BoxFaceNormals[6][3] = {
    {0.0f, 0.0f, -1.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
};

// GLOBAL: LEGOLAND 0x004b5748
struct BoxFace BoxSolidFaces[12] = {
    {0, 0, {0, 1, 2}, {0, 0, 0}},
    {0, 0, {0, 2, 3}, {0, 0, 0}},
    {0, 1, {7, 6, 5}, {0, 0, 0}},
    {0, 1, {7, 5, 4}, {0, 0, 0}},
    {0, 2, {0, 5, 1}, {0, 0, 0}},
    {0, 2, {0, 4, 5}, {0, 0, 0}},
    {0, 3, {2, 7, 3}, {0, 0, 0}},
    {0, 3, {2, 6, 7}, {0, 0, 0}},
    {0, 4, {1, 6, 2}, {0, 0, 0}},
    {0, 3, {1, 5, 6}, {0, 0, 0}},
    {0, 5, {3, 4, 0}, {0, 0, 0}},
    {0, 3, {3, 7, 4}, {0, 0, 0}},
};

// GLOBAL: LEGOLAND 0x004b5808
BoxFace BoxSolidOriginFaces[12] = {
    {0, 0, {0, 1, 2}, {0, 0, 0}},
    {0, 0, {0, 2, 3}, {0, 0, 0}},
    {0, 1, {7, 6, 5}, {0, 0, 0}},
    {0, 1, {7, 5, 4}, {0, 0, 0}},
    {0, 2, {0, 5, 1}, {0, 0, 0}},
    {0, 2, {0, 4, 5}, {0, 0, 0}},
    {0, 3, {2, 7, 3}, {0, 0, 0}},
    {0, 3, {2, 6, 7}, {0, 0, 0}},
    {0, 4, {1, 6, 2}, {0, 0, 0}},
    {0, 3, {1, 5, 6}, {0, 0, 0}},
    {0, 5, {3, 4, 0}, {0, 0, 0}},
    {0, 3, {3, 7, 4}, {0, 0, 0}},
};

// GLOBAL: LEGOLAND 0x004b58c8
struct BoxSolid BoxSolidTemplate = {8, 0, 12, NULL, BoxFaceNormals, NULL, BoxSolidFaces, 12};

// GLOBAL: LEGOLAND 0x004b5f50
void *TexturedSpanFillers[2] = {FillSpansTexturedZ, FillSpansTexturedZ};

/* Makes a BoxSolid from BoxSolidTemplate: a box +-w by +-h from z = -d up to 0; with xverts, also transforms the
 * corners. */
// FUNCTION: LEGOLAND 0x00420fd0
void InitBoxSolidToOrigin(struct BoxSolid *box, struct FVec3 *verts, struct FVec3 *xverts, int unused, float w, float h, float d) {
    int i;

    *box = BoxSolidTemplate;
    box->verts = verts;
    box->xverts = xverts;
    box->verts[0].x = w;
    box->verts[0].y = -h;
    box->verts[0].z = -d;
    box->verts[1].x = w;
    box->verts[1].y = h;
    box->verts[1].z = -d;
    box->verts[2].x = -w;
    box->verts[2].y = h;
    box->verts[2].z = -d;
    box->verts[3].x = -w;
    box->verts[3].y = -h;
    box->verts[3].z = -d;
    for (i = 0; i <= 3; i++) {
        box->verts[i + 4] = box->verts[i];
        box->verts[i + 4].z = 0.0f;
    }
    if (xverts != NULL) {
        for (i = 0; i <= 7; i++) {
            struct FVec3 tmp = box->verts[i];
            Vec3Normalize(&tmp.x);
            box->xverts[i] = tmp;
        }
        for (i = 0; i < 12; i++) {
            BoxSolidOriginFaces[i].plane = -1;
            BoxSolidOriginFaces[i].u[0] = BoxSolidOriginFaces[i].t[0];
            BoxSolidOriginFaces[i].u[1] = BoxSolidOriginFaces[i].t[1];
            BoxSolidOriginFaces[i].u[2] = BoxSolidOriginFaces[i].t[2];
        }
        box->nFaceA = 0;
        box->nFaceB = 12;
        box->faceB = BoxSolidOriginFaces;
    }
}

/* Like InitBoxSolidToOrigin but centred: z from -d to d. */
// FUNCTION: LEGOLAND 0x00421130
void InitBoxSolidCentred(struct BoxSolid *box, struct FVec3 *verts, struct FVec3 *xverts, int unused, float w, float h, float d) {
    int i;

    *box = BoxSolidTemplate;
    box->verts = verts;
    box->xverts = xverts;
    box->verts[0].x = w;
    box->verts[0].y = -h;
    box->verts[0].z = -d;
    box->verts[1].x = w;
    box->verts[1].y = h;
    box->verts[1].z = -d;
    box->verts[2].x = -w;
    box->verts[2].y = h;
    box->verts[2].z = -d;
    box->verts[3].x = -w;
    box->verts[3].y = -h;
    box->verts[3].z = -d;
    for (i = 0; i <= 3; i++) {
        box->verts[i + 4] = box->verts[i];
        box->verts[i + 4].z = d;
    }
    if (xverts != NULL) {
        for (i = 0; i <= 7; i++) {
            struct FVec3 tmp = box->verts[i];
            Vec3Normalize(&tmp.x);
            box->xverts[i] = tmp;
        }
        for (i = 0; i < 12; i++) {
            BoxSolidOriginFaces[i].plane = -1;
            BoxSolidOriginFaces[i].u[0] = BoxSolidOriginFaces[i].t[0];
            BoxSolidOriginFaces[i].u[1] = BoxSolidOriginFaces[i].t[1];
            BoxSolidOriginFaces[i].u[2] = BoxSolidOriginFaces[i].t[2];
        }
        box->nFaceA = 0;
        box->nFaceB = 12;
        box->faceB = BoxSolidOriginFaces;
    }
}

/* out = a + b. */
// FUNCTION: LEGOLAND 0x004212a0
void FloatArrayAdd(struct FloatArray *a, struct FloatArray *b, struct FloatArray *out) {
    int i;
    out->count = a->count;
    for (i = 0; i < a->count; i++) {
        float t = b->data[i];
        out->data[i] = t + a->data[i];
    }
}

/* out = a - b. */
// FUNCTION: LEGOLAND 0x004212e0
void FloatArraySub(struct FloatArray *a, struct FloatArray *b, struct FloatArray *out) {
    int i;
    out->count = a->count;
    for (i = 0; i < a->count; i++) {
        out->data[i] = a->data[i] - b->data[i];
    }
}

/* dst = src. */
// FUNCTION: LEGOLAND 0x00421320
void CopyFloatArray(struct FloatArray *src, struct FloatArray *dst) {
    memcpy(dst, src, sizeof(struct FloatArray));
}

/* arr *= f. */
// FUNCTION: LEGOLAND 0x00421340
void FloatArrayScale(struct FloatArray *arr, float f) {
    int i;
    for (i = 0; i < arr->count; i++) {
        arr->data[i] = f * arr->data[i];
    }
}

/* Largest absolute element. */
// FUNCTION: LEGOLAND 0x00421360
float FloatArrayMaxAbs(struct FloatArray *arr) {
    float max = FLOAT_004ab390;
    int i;
    for (i = 0; i < arr->count; i++) {
        float v = (float)fabs(arr->data[i]);
        if (v > max) {
            max = v;
        }
    }
    return max;
}

/* out = s * a + t * b. */
// FUNCTION: LEGOLAND 0x004213a0
void BlendFloatArrays(struct FloatArray *a, float s, struct FloatArray *b, float t, struct FloatArray *out) {
    int i;

    out->count = a->count;
    for (i = 0; i < a->count; i++) {
        out->data[i] = a->data[i] * s + b->data[i] * t;
    }
    out->count = a->count;
}

/* Zeroes size elements. */
// FUNCTION: LEGOLAND 0x00421400
void ZeroIntBuffer(struct FloatArray *buf, int size) {
    int i;
    if (size > 0) {
        for (i = 0; i < size; i++) {
            *(int *)&buf->data[i] = 0;
        }
    }
    buf->count = size;
}

/* out += f * a. */
// FUNCTION: LEGOLAND 0x00421430
void FloatArrayAddScaled(struct FloatArray *a, float f, struct FloatArray *out) {
    int i;
    out->count = a->count;
    for (i = 0; i < a->count; i++) {
        out->data[i] += f * a->data[i];
    }
}

/* Fills VecMathOps with the FloatArray functions and the scratch pool's pointer table. */
// FUNCTION: LEGOLAND 0x00421470
void InitVecMathOps(void) {
    struct FloatArray **table;
    struct FloatArray *cur;

    VecMathOps.add = FloatArrayAdd;
    VecMathOps.sub = FloatArraySub;
    VecMathOps.copy = CopyFloatArray;
    VecMathOps.scale = FloatArrayScale;
    VecMathOps.maxabs = FloatArrayMaxAbs;
    VecMathOps.blend = BlendFloatArrays;
    VecMathOps.zero = ZeroIntBuffer;
    VecMathOps.axpy = FloatArrayAddScaled;
    VecMathOps.alloc = AllocVecScratch;
    VecMathOps.release = FreeVecScratch;

    table = VecScratchPool;
    cur = VecScratchBuffers;
    while ((INT_PTR)cur < (INT_PTR)&VecScratchTop) {
        *table = cur;
        cur++;
        table += 1;
    }
}

/* Takes param_1 arrays from the scratch stack. */
// FUNCTION: LEGOLAND 0x004214f0
struct FloatArray **AllocVecScratch(unsigned int param_1) {
    unsigned int index = VecScratchTop;
    VecScratchTop = VecScratchTop + param_1;
    return &VecScratchPool[index];
}

/* Gives param_2 arrays back to the scratch stack (LIFO). */
// FUNCTION: LEGOLAND 0x00421510
void FreeVecScratch(struct FloatArray **param_1, unsigned int param_2) {
    VecScratchTop = VecScratchTop - param_2;
}

/* dest = VecMathOps with dimension value. */
// FUNCTION: LEGOLAND 0x00421540
void InitVecMath(struct VecMath *dest, unsigned int value) {
    VecMathOps.dim = value;
    memcpy(dest, &VecMathOps, sizeof(struct VecMath));
}

/* Draws a seated visitor (the castle visitor's draw method). */
// FUNCTION: LEGOLAND 0x00421560
unsigned int DrawCastleVisitor(struct CastleVisitor *param_2, struct CarXform *param_1) {
    SetModelLighting();
    return DrawLmsModel(param_2->lms, param_2->field_8, param_1, &param_1->m, 0);
}

/* Puts the visitor in the seat. */
// FUNCTION: LEGOLAND 0x00421590
void SeatCastleVisitor(struct CastleVisitor *arg1, struct TrainSeat *arg2) {
    SetTrainSeatOccupant(arg2, arg1);
    arg1->seat = arg2;
}

/* Takes the visitor out of its seat. */
// FUNCTION: LEGOLAND 0x004215b0
void UnseatCastleVisitor(struct CastleVisitor *param_1) {
    struct TrainSeat *temp = param_1->seat;
    TakeTrainSeatOccupant(temp);
    param_1->seat = 0;
}

/* Allocates a castle visitor for a bloke: the sitting man/girl model and a copy of its materials recoloured for the
 * bloke (flags 1 when that fails). */
// FUNCTION: LEGOLAND 0x004215d0
struct CastleVisitor *CreateCastleVisitor(struct BlokeInfo *a1) {
    struct CastleVisitor *self = MemAlloc(sizeof(struct CastleVisitor), 0, 0, 0);
    if (self == NULL) {
        return NULL;
    }
    self->flags = 0;
    if (a1->sex == 0) {
        self->lms = GetLmsByName(SitLoManSitName);
        self->field_8 = GetLfmByName(SitLoManSitName);
    } else {
        self->lms = GetLmsByName(SitLoGirlSitName);
        self->field_8 = GetLfmByName(SitLoGirlSitName);
    }
    self->field_8 = BuildCastleVisitorModel(a1);
    if (self->field_8 == 0) {
        self->flags = 1;
    }
    self->draw = DrawCastleVisitor;
    self->unlink = FreeCastleVisitor;
    self->bloke = (struct Bloke *)a1->bloke;
    return self;
}

/* Copies the sitting model's .lfm and swaps the template chest/face textures for the bloke's, and the template
 * leg/arm colours for the bloke's. */
// FUNCTION: LEGOLAND 0x00421660
struct LmsRec *BuildCastleVisitorModel(struct BlokeInfo *p) {
    struct LmsFile *lms;
    struct LmsRec *src;
    struct LmsRec *dst;
    unsigned int *colours;
    unsigned int size;
    const char **names;
    struct KeyPair colPairs[2];
    struct KeyPair namePairs[2];
    char *nm;
    unsigned int *cp;
    int i;
    int k;
    struct LmsRef *r;

    if (p->sex == 0) {
        lms = GetLmsByName(SitLoManSitName);
        src = GetLfmByName(SitLoManSitName);
        size = GetLfmSizeByName(SitLoManSitName);
        names = ManVisitorTextures;
        colours = ManVisitorColours;
    } else {
        lms = GetLmsByName(SitLoGirlSitName);
        src = GetLfmByName(SitLoGirlSitName);
        size = GetLfmSizeByName(SitLoGirlSitName);
        names = GirlVisitorTextures;
        colours = GirlVisitorColours;
    }
    dst = MemAlloc(size, 0, 0, 0);
    if (dst == NULL) {
        return 0;
    }
    for (i = 0, nm = p->chest; i < 2; i++) {
        namePairs[i].key = FindLtxIndexByName(*names++);
        namePairs[i].val = FindLtxIndexByName(nm);
        nm += 0x14;
    }
    cp = &p->leg;
    for (i = 0; i < 2; i++) {
        colPairs[i].key = FindRollercoasterLptEntry(*colours++);
        colPairs[i].val = FindRollercoasterLptEntry(*cp);
        cp++;
    }
    memcpy(dst, src, size);
    for (i = 0; i < lms->tex_count; i++) {
        unsigned int key;
        r = lms->tex_tris + i;
        key = src[r->idx].key;
        for (k = 0; k <= 1; k++) {
            if (namePairs[k].key == key) {
                key = namePairs[k].val;
                break;
            }
        }
        dst[r->idx].key = key;
    }
    for (i = 0; i < lms->flat_count; i++) {
        unsigned int key;
        r = lms->flat_tris + i;
        key = src[r->idx].key;
        for (k = 0; k <= 1; k++) {
            if (colPairs[k].key == key) {
                key = colPairs[k].val;
                break;
            }
        }
        dst[r->idx].key = key;
    }
    for (i = 0; i < lms->gouraud_count; i++) {
        unsigned int key;
        r = lms->gouraud_tris + i;
        key = src[r->idx].key;
        for (k = 0; k <= 1; k++) {
            if (colPairs[k].key == key) {
                key = colPairs[k].val;
                break;
            }
        }
        dst[r->idx].key = key;
    }
    return dst;
}

/* Fills CastleVisitorInfo with the bloke's sex, leg and arm colours and chest and face textures. */
// FUNCTION: LEGOLAND 0x00421890
struct BlokeInfo *GetBlokeAppearance(struct BlokeSex0 *bloke) {
    CastleVisitorInfo.bloke = bloke;
    CastleVisitorInfo.sex = GetSexOfBloke(bloke) != 0;
    CastleVisitorInfo.leg = GetLegColourOfBloke(bloke) & 0xffffff;
    CastleVisitorInfo.arm = GetArmColourOfBloke(bloke) & 0xffffff;
    strcpy(CastleVisitorInfo.chest, GetChestTextureNameOfBloke(bloke));
    strcpy(CastleVisitorInfo.face, GetFaceTextureNameOfBloke(bloke));
    return &CastleVisitorInfo;
}

/* Queues a new visitor for the bloke at the end of the castle's visitor list (waiting, flags 1). */
// FUNCTION: LEGOLAND 0x00421930
struct CastleVisitor *AddCastleVisitor(struct Bloke *param, struct CastleState *timer) {
    struct CastleVisitor *node;
    struct CastleVisitor *old_head;

    node = CreateCastleVisitor(GetBlokeAppearance((struct BlokeSex0 *)param));

    old_head = timer->visitors.prev;
    old_head->next = node;
    old_head = timer->visitors.prev;
    node->prev = old_head;
    node->next = (struct CastleVisitor *)&timer->visitors;
    timer->visitors.prev = node;

    node->time = GetGameTimer();
    node->flags = 1;

    return node;
}

/* Unlinks and frees a visitor (and its recoloured materials unless they failed to build). */
// FUNCTION: LEGOLAND 0x00421980
void FreeCastleVisitor(struct CastleVisitor *p) {
    p->prev->next = p->next;
    p->next->prev = p->prev;

    if ((p->flags & 1U) == 0U) {
        MemFree(p->field_8);
    }

    MemFree(p);
}

struct Vec5 {
    float field_0;
    float field_4;
    unsigned int field_8;
    float field_c;
    float field_10;
};

struct Vec3 {
    float field_0;
    float field_4;
    unsigned int field_8;
};

/* Line stretch, centre at t: start + t * dir in x/y, constant height. */
// FUNCTION: LEGOLAND 0x004219c0
void LineCurvePos(struct Vec5 *arg1, float float_arg, struct Vec3 *arg3) {
    arg3->field_0 = arg1->field_0 + (float_arg * arg1->field_c);
    arg3->field_4 = arg1->field_4 + (float_arg * arg1->field_10);
    arg3->field_8 = arg1->field_8;
}

struct Words3 {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
};

/* Tangent of a line stretch: its constant direction. */
// FUNCTION: LEGOLAND 0x004219f0
void LineCurveTangent(unsigned char *param_1, unsigned int param_2, struct Words3 *param_3) {
    param_1 = param_1 + 12;
    param_3->field_0 = *(unsigned int *)param_1;
    param_3->field_4 = *(unsigned int *)(param_1 + 4);
    param_3->field_8 = *(unsigned int *)(param_1 + 8);
}

struct Vec7 {
    float field_0;
    float field_4;
    unsigned int field_8;
    float field_c;
    float field_10;
    unsigned char pad_14[4];
    float field_18;
    float field_1c;
};

/* Line stretch, side A at t: the centre plus the side offset. */
// FUNCTION: LEGOLAND 0x00421a10
void LineCurveSideAPos(struct Vec7 *s1, float arg2, struct Vec3 *s2) {
    s2->field_0 = arg2 * s1->field_c + s1->field_18 + s1->field_0;
    s2->field_4 = arg2 * s1->field_10 + s1->field_1c + s1->field_4;
    s2->field_8 = s1->field_8;
}

/* Line stretch, side B at t: the centre minus the side offset. */
// FUNCTION: LEGOLAND 0x00421a40
void LineCurveSideBPos(struct Vec7 *a, float c, struct Vec3 *b) {
    b->field_0 = c * a->field_c + a->field_0 - a->field_18;
    b->field_4 = c * a->field_10 + a->field_4 - a->field_1c;
    b->field_8 = a->field_8;
}

struct Struct1a70A {
    unsigned char pad_0[0x44];
    unsigned int field_44;
    unsigned int field_48;
};

/* Line stretch split points: just its two ends. Returns 2. */
// FUNCTION: LEGOLAND 0x00421a70
int LineCurveSplitPoints(struct Struct1a70A *param_1, struct Pair *param_2) {
    param_2->field_0 = param_1->field_44;
    param_2->field_4 = param_1->field_48;
    return 2;
}

struct Struct1a90 {
    unsigned int field_0;
    unsigned int field_4;
    float field_8;
};

/* Line stretch normal: straight up (0, 0, 1). */
// FUNCTION: LEGOLAND 0x00421a90
void LineCurveNormal(unsigned int param_1, unsigned int param_2, struct Struct1a90 *param_3) {
    param_3->field_0 = 0;
    param_3->field_4 = 0;
    param_3->field_8 = 1.0f;
}

struct LineCurve {
    struct Words3 start;
    float dir[3];
    struct Words3 side;
    float height[4];
    unsigned char pad_34[0xc];
    float length;
    unsigned int start_t;
    float end_t;
    void **vt;
    struct SegBlob *next;
    struct SegBlob *prev;
};

/* Makes a straight stretch from a to b with side offset c: t runs 0..1, length is the 3D distance. */
// FUNCTION: LEGOLAND 0x00421ab0
void InitLineCurve(struct LineCurve *obj, float *a, float *b, struct Words3 *c) {
    float sum = FLOAT_004ab390;
    float d;
    int i;

    for (i = 0; i < 3; i++) {
        d = b[i] - a[i];
        obj->dir[i] = d;
        sum += d * d;
    }
    obj->length = (float)sqrt(sum);
    obj->start = *(struct Words3 *)a;
    obj->side = *c;
    obj->vt = &CurveFunctionTable[8];
    obj->start_t = 0;
    obj->end_t = 1.0f;
    obj->next = NULL;
    obj->prev = NULL;
}

/* Arc stretch, centre at angle t: cos t * axis0 + sin t * axis1 + centre (x/y; z from the centre). */
// FUNCTION: LEGOLAND 0x00421b40
void ArcCurvePos(float m[3][3], float angle, float out[3]) {
    int j;
    float s = (float)sin(angle);
    float c = (float)cos(angle);
    for (j = 0; j < 2; j++) {
        out[j] = c * m[0][j] + s * m[1][j] + m[2][j];
    }
    out[2] = m[2][2];
}

/* Arc stretch tangent at angle t: -sin t * axis0 + cos t * axis1 (level). */
// FUNCTION: LEGOLAND 0x00421b90
void ArcCurveTangent(float m[3][3], float angle, float out[3]) {
    int j;
    float s = (float)sin(angle);
    angle = (float)cos(angle);
    s = -s;
    for (j = 0; j < 2; j++) {
        out[j] = angle * m[1][j] + s * m[0][j];
    }
    out[2] = 0.0f;
}

/* Arc stretch, side A at angle t: the centre circle scaled by side_a_scale. */
// FUNCTION: LEGOLAND 0x00421be0
void ArcCurveSideAPos(float m[12], float angle, float out[3]) {
    int j;
    float s = (float)sin(angle) * m[9];
    float c = (float)cos(angle) * m[9];
    for (j = 0; j < 2; j++) {
        out[j] = c * m[j] + s * m[j + 3] + m[j + 6];
    }
    out[2] = m[8];
}

/* Arc stretch, side B at angle t: the centre circle scaled by side_b_scale. */
// FUNCTION: LEGOLAND 0x00421c30
void ArcCurveSideBPos(float m[12], float angle, float out[3]) {
    int j;
    float s = (float)sin(angle) * m[10];
    float c = (float)cos(angle) * m[10];
    for (j = 0; j < 2; j++) {
        out[j] = c * m[j] + s * m[j + 3] + m[j + 6];
    }
    out[2] = m[8];
}

struct Floats6 {
    float field_0;
    float field_4;
    float field_8;
    float field_c;
    float field_10;
    float field_14;
};

/* Arc stretch split points: 0 to pi/2 in five steps. Returns 6. */
// FUNCTION: LEGOLAND 0x00421c80
unsigned int GetQuarterCircleAngles(unsigned int param_1, struct Floats6 *ptr_0) {
    ptr_0->field_0 = 0.0f;
    ptr_0->field_4 = 0.31415900588035583f;
    ptr_0->field_8 = 0.6283180117607117f;
    ptr_0->field_c = 0.9424769878387451f;
    ptr_0->field_10 = 1.2566360235214233f;
    ptr_0->field_14 = 1.5707950592041016f;
    return 6;
}

/* Arc stretch normal: straight up (0, 0, 1). */
// FUNCTION: LEGOLAND 0x00421cc0
void ArcCurveNormal(int param_1, int param_2, struct Struct1a90 *param_3) {
    param_3->field_0 = 0;
    param_3->field_4 = 0;
    param_3->field_8 = 1.0f;
}

struct ArcCurve {
    struct Words3 axis0;
    struct Words3 axis1;
    struct Words3 centre;
    unsigned int side_a_scale;
    unsigned int side_b_scale;
    unsigned char pad_2c[0x18];
    unsigned int start_t;
    float end_t;
    void **vt;
    struct SegBlob *next;
    struct SegBlob *prev;
};

/* Makes a quarter-circle stretch (t = 0..pi/2) from its two axes (radius vectors at 0 and 90 degrees), its centre
 * and the side radius factors. */
// FUNCTION: LEGOLAND 0x00421ce0
void InitArcCurve(struct Words3 *a, struct Words3 *b, struct Words3 *c, struct ArcCurve *obj, unsigned int d, unsigned int e) {
    obj->axis0 = *a;
    obj->axis1 = *b;
    obj->centre = *c;
    obj->vt = &CurveFunctionTable[16];
    obj->side_a_scale = d;
    obj->side_b_scale = e;
    obj->start_t = 0;
    obj->end_t = 1.5707950592041016f;
    obj->next = NULL;
    obj->prev = NULL;
}

struct Struct1d60 {
    float field_0;
    float field_4;
    unsigned char pad_8[4];
    float field_c;
    float field_10;
    unsigned char pad_14[16];
    float field_24;
    float field_28;
    float field_2c;
    float field_30;
};

struct Floats3 {
    float field_0;
    float field_4;
    float field_8;
};

/* Slope stretch, centre at t: the xy line, height from the cubic in t. */
// FUNCTION: LEGOLAND 0x00421d60
void SlopeCurvePos(struct Struct1d60 *a, float b, struct Floats3 *out) {
    out->field_0 = a->field_0 + b * a->field_c;
    out->field_4 = a->field_4 + b * a->field_10;
    out->field_8 = a->field_30 + b * (a->field_2c + b * (a->field_28 + b * a->field_24));
}

/* Slope stretch tangent at t: (dx, dy, 3 a t^2 + 2 b t + c). */
// FUNCTION: LEGOLAND 0x00421da0
void SlopeCurveTangent(unsigned char *param_1, float t, struct Words3 *out) {
    float *f = (float *)param_1;
    float v;
    *out = *(struct Words3 *)(param_1 + 12);
    v = t * f[9];
    v = v * FLOAT_004ab43c;
    v = v + (f[10] + f[10]);
    *(float *)&out->field_8 = v * t + f[11];
}

/* Slope stretch, side A at t: the centre plus the side offset in x/y. */
// FUNCTION: LEGOLAND 0x00421df0
void SlopeCurveSideAPos(float *a, float x, float *out) {
    out[0] = x * a[3] + a[6] + a[0];
    out[1] = x * a[4] + a[7] + a[1];
    out[2] = ((x * a[9] + a[10]) * x + a[11]) * x + a[12];
}

struct SlopeCurve {
    float x0;
    float y0;
    float pad_8;
    float dx;
    float dy;
    float pad_14;
    float side_x;
    float side_y;
    float pad_20;
    float coef_t3;
    float coef_t2;
    float coef_t1;
    float coef_t0;
    unsigned char pad_34[0x44 - 0x34];
    float t_start;
    float t_end;
};

/* Slope stretch, side B at t: the centre minus the side offset in x/y. */
// FUNCTION: LEGOLAND 0x00421e40
void SlopeCurveSideBPos(struct SlopeCurve *ptr1, float multiplier, struct Floats3 *ptr2) {
    float temp;

    ptr2->field_0 = multiplier * ptr1->dx + ptr1->x0 - ptr1->side_x;
    ptr2->field_4 = multiplier * ptr1->dy + ptr1->y0 - ptr1->side_y;

    temp = multiplier * ptr1->coef_t3 + ptr1->coef_t2;
    temp = temp * multiplier + ptr1->coef_t1;
    temp = temp * multiplier + ptr1->coef_t0;
    ptr2->field_8 = temp;
}

/* Recursively adds split points between (x0, y0) and (x1, y1) where the slope's height cubic is furthest (more than
 * 1 unit) from the straight chord, so the drawn segments follow the curve. */
// FUNCTION: LEGOLAND 0x00421e90
void SubdivideCurveAtMaxError(float x0, float y0, float x1, float y1) {
    float roots[2];
    float b;
    float by;
    float bx;
    float m;
    float best = 1.17549435e-38f;
    struct SlopeCurve *o = SplitCurve;
    float *r = roots;
    int i = 2;

    m = (y1 - y0) / (x1 - x0);
    b = y0 - m * x0;
    {
        float a;
        volatile float b2;
        volatile float sq;
        float c;
        a = o->coef_t3;
        a = a * FLOAT_004ab43c;
        b2 = o->coef_t2 + o->coef_t2;
        c = o->coef_t1 - m;
        sq = (float)sqrt(b2 * b2 - c * a * 4.0f);
        roots[0] = (sq - b2) / (a + a);
        roots[1] = (-b2 - sq) / (a + a);
    }
    for (; i > 0; i--, r++) {
        float y;
        float d;
        if (*r >= x0 && *r <= x1) {
            y = o->coef_t3;
            y = y * *r + o->coef_t2;
            y = y * *r + o->coef_t1;
            y = y * *r + o->coef_t0;
            d = (float)fabs(y - m * *r - b);
            if (d > best) {
                bx = *r;
                by = y;
                best = d;
            }
        }
    }
    if (fabs(best) > 1.0) {
        *SplitPointsNext++ = bx;
        SplitPointCount++;
        SubdivideCurveAtMaxError(x0, y0, bx, by);
        SubdivideCurveAtMaxError(bx, by, x1, y1);
    }
}

/* Slope stretch split points: its ends plus the points SubdivideCurveAtMaxError adds, sorted. Returns the count. */
// FUNCTION: LEGOLAND 0x00422000
int BuildCurveSplitPoints(struct SlopeCurve *o, float *out) {
    int i;
    int j;
    float t;

    SplitCurveStart = o;
    SplitCurve = o;
    SplitPointCount = 2;
    SplitPointsNext = out;
    *SplitPointsNext = o->t_start;
    SplitPointsNext++;
    *SplitPointsNext = o->t_end;
    SplitPointsNext++;
    SubdivideCurveAtMaxError(o->t_start, ((o->t_start * o->coef_t3 + o->coef_t2) * o->t_start + o->coef_t1) * o->t_start + o->coef_t0, o->t_end, ((o->t_end * o->coef_t3 + o->coef_t2) * o->t_end + o->coef_t1) * o->t_end + o->coef_t0);
    for (i = SplitPointCount - 1; i >= 0; i--) {
        for (j = 0; j < i; j++) {
            if (out[j] > out[j + 1]) {
                t = out[j];
                out[j] = out[j + 1];
                out[j + 1] = t;
            }
        }
    }
    return SplitPointCount;
}

struct V3 {
    float x, y, z;
};

/* Slope stretch normal at t: the tangent, transformed (Vec3Normalize), turned 90 degrees in the plane of its larger
 * horizontal component. */
// FUNCTION: LEGOLAND 0x004220e0
void SlopeCurveNormal(float *obj, float t, float *out) {
    float c, v;

    *(struct V3 *)out = *(struct V3 *)(obj + 3);
    v = t * obj[9];
    out[2] = (v * FLOAT_004ab43c + obj[10] * 2) * t + obj[11];
    Vec3Normalize(out);
    if (fabs(out[1]) < CastleEpsilon) {
        c = out[2];
        if (*(unsigned int *)&out[0] & 0x80000000) {
            out[2] = -out[0];
            out[0] = c;
        } else {
            out[2] = out[0];
            out[0] = -c;
        }
    } else {
        c = out[2];
        if (*(unsigned int *)&out[1] & 0x80000000) {
            out[2] = -out[1];
            out[1] = c;
        } else {
            out[2] = out[1];
            out[1] = -c;
        }
    }
}

/* Makes a slope stretch from a1 to a2: a straight xy line whose height follows the Hermite cubic from a1.z to a2.z
 * with level ends. */
// FUNCTION: LEGOLAND 0x00422180
void InitSlopeCurve(float *a1, float *a2, struct Words3 *a3, struct LineCurve *a4) {
    float q[4];
    float sum;
    float *p;
    float *out;
    int j;

    q[0] = a2[2];
    q[1] = a1[2];
    q[2] = 0.0f;
    q[3] = 0.0f;
    a2[2] = 0.0f;
    a1[2] = 0.0f;
    InitLineCurve(a4, a1, a2, a3);
    p = &HermiteBasis[0][0];
    out = a4->height;
    do {
        sum = FLOAT_004ab390;
        for (j = 0; j < 4; j++) {
            sum += q[j] * *p++;
        }
        *out++ = sum;
    } while ((INT_PTR)p <= (INT_PTR)&HermiteBasis[3][0]);
    a4->vt = &CurveFunctionTable[0];
}

/* Fills CurveFunctionTable: rows of 8 for slope (0), line (8) and arc (16) stretches: side A position and tangent,
 * centre position and tangent, side B position and tangent, split points, normal. */
// FUNCTION: LEGOLAND 0x00422210
void InitCurveFunctionTable(void) {
    CurveFunctionTable[0] = SlopeCurveSideAPos;
    CurveFunctionTable[1] = SlopeCurveTangent;
    CurveFunctionTable[3] = SlopeCurveTangent;
    CurveFunctionTable[5] = SlopeCurveTangent;
    CurveFunctionTable[9] = LineCurveTangent;
    CurveFunctionTable[11] = LineCurveTangent;
    CurveFunctionTable[13] = LineCurveTangent;
    CurveFunctionTable[2] = SlopeCurvePos;
    CurveFunctionTable[4] = SlopeCurveSideBPos;
    CurveFunctionTable[6] = BuildCurveSplitPoints;
    CurveFunctionTable[7] = SlopeCurveNormal;
    CurveFunctionTable[8] = LineCurveSideAPos;
    CurveFunctionTable[10] = LineCurvePos;
    CurveFunctionTable[12] = LineCurveSideBPos;
    CurveFunctionTable[14] = LineCurveSplitPoints;
    CurveFunctionTable[15] = LineCurveNormal;
    CurveFunctionTable[16] = ArcCurveSideAPos;
    CurveFunctionTable[17] = ArcCurveTangent;
    CurveFunctionTable[18] = ArcCurvePos;
    CurveFunctionTable[19] = ArcCurveTangent;
    CurveFunctionTable[20] = ArcCurveSideBPos;
    CurveFunctionTable[21] = ArcCurveTangent;
    CurveFunctionTable[22] = GetQuarterCircleAngles;
    CurveFunctionTable[23] = ArcCurveNormal;
}

/* 1 if the text is at a CR LF. */
// FUNCTION: LEGOLAND 0x004222f0
int IsCrLf(unsigned short *param_1) {
    return param_1[0] == 0xa0d;
}

/* Copies text up to the next CR LF; returns the end of the copy (not terminated). */
// FUNCTION: LEGOLAND 0x00422300
unsigned char *CopyTextLine(unsigned char *src, unsigned char *dst) {
    if (IsCrLf((unsigned short *)src) == 0) {
        do {
            *dst = *src;
            dst++;
            src++;
        } while (IsCrLf((unsigned short *)src) == 0);
        return dst;
    }
    return dst;
}

/* Start of line `count` of a CR LF text file, or NULL. */
// FUNCTION: LEGOLAND 0x00422340
unsigned char *FindLineStart(struct CoasterTextFile *range, int count) {
    int n = 0;
    unsigned char *p = range->data;
    unsigned char *end = p + range->size;

    if (count == 0) {
        return p;
    }
    while (p < end - 1) {
        if (IsCrLf((unsigned short *)p) != 0) {
            n++;
            p += 2;
            if (n == count) {
                return p;
            }
        } else {
            p++;
        }
    }
    return 0;
}

/* Copies line a3 of the text file into a2 (terminated); 0 if there is no such line. */
// FUNCTION: LEGOLAND 0x00422390
unsigned int GetTextLine(struct CoasterTextFile *a1, char *a2, unsigned int a3) {
    unsigned char *result1 = FindLineStart(a1, a3);
    if (result1 == 0)
        return 0;
    {
        unsigned char *p = CopyTextLine(result1, (unsigned char *)a2);
        p[0] = 0;
    }
    return 1;
}

/* Number of CR LF line ends in the text file. */
// FUNCTION: LEGOLAND 0x004223c0
int CountTextLines(struct CoasterTextFile *range) {
    unsigned char *p = range->data;
    unsigned char *end = p + range->size;
    int n = 0;

    while (p < end - 1) {
        if (IsCrLf((unsigned short *)p) != 0) {
            n++;
            p += 2;
        } else {
            p++;
        }
    }
    return n;
}

/* Index of the line equal (case-insensitively) to param_2, or -1. */
// FUNCTION: LEGOLAND 0x00422400
int FindTextLine(struct CoasterTextFile *param_1, const char *param_2) {
    char buf[0x100];
    int i = 0;

    while (GetTextLine(param_1, buf, i) != 0) {
        if (_strcmpi(param_2, buf) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

/* Reads a whole file into a new block (size in *bytesReadPtr); NULL on failure. */
// FUNCTION: LEGOLAND 0x00422470
void *ReadFileToBuffer(const char *fileName, unsigned int *bytesReadPtr) {
    HANDLE hFile;
    unsigned int fileSize;
    void *buffer;
    DWORD bytesRead;

    if (fileName == 0) {
        return 0;
    }

    hFile = CreateFileA(fileName, 0x80000000, 0x1, 0, 0x3, 0x8000000, 0);
    if (hFile == (HANDLE)-1) {
        return 0;
    }

    fileSize = GetFileSize(hFile, 0);
    buffer = MemAlloc(fileSize, 0, (unsigned int)(UINT_PTR)EmptyString, 0);
    if (buffer == 0) {
        CloseHandle(hFile);
        return 0;
    }

    ReadFile(hFile, buffer, fileSize, &bytesRead, 0);
    if (bytesRead != fileSize) {
        MemFree(buffer);
        CloseHandle(hFile);
        return 0;
    }

    CloseHandle(hFile);
    *bytesReadPtr = fileSize;
    return buffer;
}

/* Index of the named object in ROLLERCOASTER.obj (the coaster's model list). */
// FUNCTION: LEGOLAND 0x00422590
unsigned int FindCoasterObjLine(const char *s) {
    return FindTextLine(&CoasterObjFile, s);
}

/* Copies line param_1 of the model list. */
// FUNCTION: LEGOLAND 0x004225b0
void GetCoasterObjLine(unsigned int param_1, char *param_2) {
    GetTextLine(&CoasterObjFile, param_2, param_1);
}

/* Number of models in the model list. */
// FUNCTION: LEGOLAND 0x004225d0
unsigned int GetCoasterObjLineCount(void) {
    return CoasterObjLineCount;
}

/* Index of the named texture in ROLLERCOASTER.txt (the coaster's texture list). */
// FUNCTION: LEGOLAND 0x00422600
unsigned int FindCoasterTxtLine(const char *param) {
    return FindTextLine(&CoasterTxtFile, param);
}

/* Texture file name for index value: the base name and the 4-digit index (e.g. ROLLERCOASTER0003). */
// FUNCTION: LEGOLAND 0x00422620
void FormatRollercoasterFileName(int value, char *buffer) {
    // STRING: LEGOLAND 0x004b5afc
    wsprintfA(buffer, "%s%04d", CoasterObjBaseName, value);
}

/* Number of textures in the texture list. */
// FUNCTION: LEGOLAND 0x00422640
unsigned int GetCoasterTxtLineCount(void) {
    return CoasterTxtLineCount;
}

/* Loads name.obj (model names) and name.txt (texture names) and counts their lines. */
// FUNCTION: LEGOLAND 0x004226c0
int LoadObjAndTxtFiles(const char *name) {
    char buffer[256];

    // STRING: LEGOLAND 0x004b5b04
    wsprintfA(buffer, "%s.obj", name);
    CoasterObjFile.data = ReadFileToBuffer(buffer, &CoasterObjFile.size);
    if (CoasterObjFile.data == 0) {
        return 0;
    }
    // STRING: LEGOLAND 0x004b5b0c
    wsprintfA(buffer, "%s.txt", name);
    CoasterTxtFile.data = ReadFileToBuffer(buffer, &CoasterTxtFile.size);
    if (CoasterTxtFile.data == 0) {
        MemFree(CoasterObjFile.data);
        return 0;
    }
    CoasterObjLineCount = CountTextLines(&CoasterObjFile);
    CoasterTxtLineCount = CountTextLines(&CoasterTxtFile);
    strcpy(CoasterObjBaseName, name);
    return 1;
}

/* Frees the model and texture name lists. */
// FUNCTION: LEGOLAND 0x004227a0
void FreeObjAndTxtFiles(void) {
    MemFree(CoasterTxtFile.data);
    MemFree(CoasterObjFile.data);
}

/* Copy of an edge mesh without its back-facing triangles (negative screen-space winding) and the edges and vertices
 * only they used; indices are renumbered. Edge references in triangles carry bit 31 for reversed edges. */
// FUNCTION: LEGOLAND 0x004227c0
struct EdgeMesh *EdgeMeshRemoveBackfaces(struct EdgeMesh *src) {
    int *triFlag;
    volatile int ptmp45;
    int nV = 0;
    int *edgeFlag;
    int j;
    int i;
    int removedTris = 0;
    int *vertFlag;
    int k;
    struct EdgeMesh *out = NULL;
    int nEdges = src->nEdges;
    int removedEdges = 0;
    int removedVerts = 0;

    for (i = 0; i < src->nTris; i++) {
        for (k = 0; k < 3; k++) {
            struct MEdge *ed = &src->edges[src->tris[i].e[k] & 0x7fffffff];
            if (ed->v0 > nV) {
                nV = ed->v0;
            }
            if (ed->v1 > nV) {
                nV = ed->v1;
            }
        }
    }
    nV++;
    triFlag = (int *)malloc(src->nTris * 4);
    edgeFlag = (int *)malloc(nEdges * 4);
    vertFlag = (int *)malloc(nV * 4);
    if (triFlag != NULL) {
        if (edgeFlag != NULL && vertFlag != NULL) {
            memset(triFlag, 0, src->nTris * 4);
            memset(edgeFlag, 0, nEdges * 4);
            memset(vertFlag, 0, nV * 4);
            for (i = 0; i < src->nTris; i++) {
                int v[3];
                float z;
                struct MVert *p0;
                struct MVert *p1;
                struct MVert *p2;
                double dx1;
                double dy1;
                double dx2;
                double dy2;
                unsigned int bits;
                for (k = 0; k < 3; k++) {
                    int e = src->tris[i].e[k];
                    if (e & 0x80000000) {
                        v[k] = src->edges[e & 0x7fffffff].v1;
                    } else {
                        ptmp45 = src->edges[e & 0x7fffffff].v0;
                        v[k] = ptmp45;
                    }
                }
                p0 = &src->verts[v[0]];
                p1 = &src->verts[v[1]];
                dx1 = p1->x - p0->x;
                dy1 = p1->y - p0->y;
                p2 = &src->verts[v[2]];
                dx2 = p2->x - p0->x;
                dy2 = p2->y - p0->y;
                z = dy2 * dx1 - dx2 * dy1;
                bits = *(unsigned int *)&z;
                if (bits & 0x80000000) {
                    triFlag[i] = 1;
                    removedTris++;
                }
            }
            for (i = 0; i < src->nTris; i++) {
                if (triFlag[i] != 0) {
                    for (k = 0; k < 3; k++) {
                        int found = 0;
                        for (j = 0; j < src->nTris; j++) {
                            if (triFlag[j] == 0) {
                                if (((src->tris[j].e[0] ^ ((int *)src->tris)[i * 3 + k]) & 0x7fffffff) == 0 || ((src->tris[j].e[1] ^ ((int *)src->tris)[i * 3 + k]) & 0x7fffffff) == 0 || ((src->tris[j].e[2] ^ ((int *)src->tris)[i * 3 + k]) & 0x7fffffff) == 0) {
                                    found = 1;
                                    break;
                                }
                            }
                        }
                        if (!found) {
                            edgeFlag[((int *)src->tris)[i * 3 + k] & 0x7fffffff] = 1;
                        }
                    }
                }
            }
            i = 0;
            while (i < nEdges) {
                if (edgeFlag[i] != 0) {
                    int *pv = &src->edges[i].v0;
                    for (k = 0; k < 2; k++) {
                        int found = 0;
                        for (j = 0; j < nEdges; j++) {
                            if (edgeFlag[j] == 0) {
                                if (*pv == src->edges[j].v0 || *pv == src->edges[j].v1) {
                                    found = 1;
                                    break;
                                }
                            }
                        }
                        if (!found) {
                            vertFlag[*pv] = 1;
                        }
                        pv++;
                    }
                }
                i++;
            }
            for (i = 0; i < nEdges; i++) {
                if (edgeFlag[i] != 0) {
                    removedEdges++;
                }
            }
            for (i = 0; i < nV; i++) {
                if (vertFlag[i] != 0) {
                    removedVerts++;
                }
            }
            out = (struct EdgeMesh *)malloc((nEdges - removedEdges) * 8 + (src->nTris - removedTris) * 12 + (nV - removedVerts) * 20 + 0x1c);
            if (out != NULL) {
                out->nEdges = nEdges - removedEdges - 1;
                out->nTris = src->nTris - removedTris;
                out->nVerts = nV - removedVerts - 1;
                out->edges = (struct MEdge *)(out + 1);
                out->tris = (struct MTri *)(out->edges + (nEdges - removedEdges));
                out->verts = (struct MVert *)(out->tris + (src->nTris - removedTris));
                j = 0;
                for (i = 0; i < nV; i++) {
                    if (vertFlag[i] == 0) {
                        out->verts[j] = src->verts[i];
                        vertFlag[i] = i - j;
                        j++;
                    }
                }
                for (i = 0, j = 0; nEdges > i; i++) {
                    if (edgeFlag[i] == 0) {
                        out->edges[j].v0 = src->edges[i].v0 - vertFlag[src->edges[i].v0];
                        out->edges[j].v1 = src->edges[i].v1 - vertFlag[src->edges[i].v1];
                        edgeFlag[i] = i - j;
                        j++;
                    }
                }
                j = 0;
                for (i = 0; i < src->nTris; i++) {
                    if (triFlag[i] == 0) {
                        int e = src->tris[i].e[0];
                        if (0x80000000 & e) {
                            out->tris[j].e[0] = (e - edgeFlag[e & 0x7fffffff]) | 0x80000000;
                        } else {
                            out->tris[j].e[0] = e - edgeFlag[e];
                        }
                        e = src->tris[i].e[1];
                        if (e & 0x80000000) {
                            out->tris[j].e[1] = (e - edgeFlag[e & 0x7fffffff]) | 0x80000000;
                        } else {
                            out->tris[j].e[1] = e - edgeFlag[e];
                        }
                        e = src->tris[i].e[2];
                        if (e & 0x80000000) {
                            out->tris[j].e[2] = (e - edgeFlag[e & 0x7fffffff]) | 0x80000000;
                        } else {
                            out->tris[j].e[2] = e - edgeFlag[e];
                        }
                        j++;
                    }
                }
            }
        }
        free(triFlag);
    }
    if (edgeFlag != NULL) {
        free(edgeFlag);
    }
    if (vertFlag != NULL) {
        free(vertFlag);
    }
    return out;
}

/* Points shade table `index` at its 128-byte slot and builds the 64-colour ramp for 24-bit colour param1 in it. */
// FUNCTION: LEGOLAND 0x00422e10
void SetCoasterShadeTable(int param1, int index) {
    char *p = CoasterShadeTableData + (index << 7);
    CoasterShadeTables[index] = p;
    BuildShadeRamp(param1, p);
}

/* Builds a 64-step shade ramp for a 24-bit colour in the display's 15/16-bit format: 33 steps from black up to the
 * colour, then 32 from the colour up to white. */
// FUNCTION: LEGOLAND 0x00422e40
void BuildShadeRamp(int param1, char *entry) {
    unsigned short *out;
    unsigned int gbits;
    unsigned int rshift;
    float rf, gf, bf;
    float dr, dg, db;
    double ar;
    double ag;
    double ab;
    int i;

    if (DisplayPixelFormat == 2) {
        gbits = 6;
        rshift = 11;
    } else {
        gbits = 5;
        rshift = 10;
    }
    ar = FLOAT_004ab390;
    ag = FLOAT_004ab390;
    ab = FLOAT_004ab390;
    rf = (float)(((unsigned int)param1 >> 19) & 0x1f);
    dr = rf * 0.03125f;
    gf = (float)(((unsigned int)param1 & 0xff00) >> (16 - gbits));
    dg = gf * 0.03125f;
    bf = (float)(((unsigned int)param1 >> 3) & 0x1f);
    db = bf * 0.03125f;
    out = (unsigned short *)entry;
    for (i = 0; i < 33; i++) {
        *out++ = (unsigned short)(((short)(int)ar << rshift) | ((int)ag << 5) | (int)ab);
        ar += dr;
        ag += dg;
        ab += db;
    }
    ar = rf;
    ag = gf;
    ab = bf;
    dr = (float)(FLOAT_004ab444 - ar) * 0.032258064f;
    dg = (float)((float)((1 << gbits) - 1) - ag) * 0.032258064f;
    db = (float)(FLOAT_004ab444 - ab) * 0.032258064f;
    out = (unsigned short *)entry + 32;
    for (i = 0; i < 32; i++) {
        *out++ = (unsigned short)(((short)(int)ar << rshift) | ((int)ag << 5) | (int)ab);
        ar += dr;
        ag += dg;
        ab += db;
    }
}

/* Builds a shade table for every palette colour (Rollercoaster.lpt); if out of memory, every table is a white ramp. */
// FUNCTION: LEGOLAND 0x00422fe0
void BuildCoasterShadeTables(void) {
    int *p = GetRollercoasterLpt();
    int n = *p++;
    int i;

    CoasterShadeTableCount = n;
    CoasterShadeTableData = MemAlloc(n << 7, 0, 0, 0);
    if (CoasterShadeTableData == NULL) {
        BuildShadeRamp(0xffffff, CoasterFallbackShadeRamp);
        for (i = 0; i < 1024; i++) {
            CoasterShadeTables[i] = CoasterFallbackShadeRamp;
        }
        return;
    }
    for (i = 0; i < n; i++) {
        SetCoasterShadeTable(*p++, i);
    }
}

#pragma pack(push, 2)
struct RecEnt {
    short w;
    int d;
    short s0n;
};

struct RecBuf {
    int n;
    int x;
    short s0_first;
    struct RecEnt ent[1];
};
#pragma pack(pop)

struct RecIdx {
    int v;
    int k;
};

struct RecSrc {
    int f0;
    int flag;
    int fx;
    int f0c;
    int f10;
    int f14;
    int f18;
    int d;
    int f20;
    int f24;
    int f28;
    int f2c;
};

/* Clears the depth buffer for a new frame: completely after a span record overflow, else only the dirty rectangles
 * and the spans recorded while drawing the last frame. Times itself into DepthClearCycles. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00423140
__declspec(naked) void ClearCoasterDepth(int param_1) {
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        push esi
        push edi
        mov edi, offset DepthSpanRecords
        push eax
        push edx
        rdtsc
        mov dword ptr [ebp - 4], eax
        pop edx
        pop eax
        mov eax, dword ptr [DepthSpanRecordsOverflow]
        test eax, eax
        je L423177
        mov ecx, 0x25800
        xor eax, eax
        mov edi, offset CoasterDepthBufferData
        mov dword ptr [CastleTrackCells], 0xa
        rep stosd
        jmp L4231c6
L423177:
        mov esi, dword ptr [CastleLinkList + 0x18]
        cmp esi, offset CastleLinkList
        je L4231a0
L423185:
        mov eax, dword ptr [esi + 0x10]
        test eax, eax
        je L423195
        push esi
        call ClearCoasterSurfaceRect
        add esp, 4
L423195:
        mov esi, dword ptr [esi + 0x18]
        cmp esi, offset CastleLinkList
        jne L423185
L4231a0:
        cmp dword ptr [DepthSpanRecordEnd], offset DepthSpanRecords
        je L4231c6
L4231ac:
        mov ecx, dword ptr [edi]
        mov eax, edi
        push eax
        lea edi, [edi + ecx*8 + 8]
        call ClearRecordedDepthSpans
        mov eax, dword ptr [DepthSpanRecordEnd]
        add esp, 4
        cmp edi, eax
        jne L4231ac
L4231c6:
        mov dword ptr [DepthSpanRecordEnd], offset DepthSpanRecords
        mov dword ptr [DepthSpanRecordsOverflow], 0
        mov dword ptr [DepthSpanRecordCount], 0
        push eax
        push edx
        rdtsc
        sub eax, dword ptr [ebp - 4]
        mov dword ptr [ebp - 4], eax
        pop edx
        pop eax
        mov edx, dword ptr [ebp - 4]
        pop edi
        mov dword ptr [DepthClearCycles], edx
        pop esi
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Records the spans of a triangle just drawn (so the next frame can clear exactly those depth pixels); sets
 * DepthSpanRecordsOverflow when the record buffer is full. */
// FUNCTION: LEGOLAND 0x00423200
void RecordDepthSpans(int n, int x, struct RecIdx *idx, struct RecSrc *src) {
    short *p = (short *)DepthSpanRecordEnd;
    struct RecBuf *saved = DepthSpanRecordEnd;
    int i;

    DepthSpanRecordCount++;
    if ((char *)p + 0x10 > CoasterDepthBufferData) {
        DepthSpanRecordsOverflow = 1;
        return;
    }
    if (n) {
        *(int *)p = n;
        saved->x = x;
        i = 0;
        if (n > 0) {
            p = (short *)saved + 5;
            for (; i < n; i++) {
                struct RecSrc *s = &src[idx->k];
                p[-1] = s->fx >> 16;
                *(int *)(p + 1) = s->d;
                p[0] = (short)idx->v;
                idx++;
                if (s->flag == 1) {
                    p[0] = -p[0];
                }
                p += 4;
            }
        }
        DepthSpanRecordEnd = (struct RecBuf *)((char *)saved + i * 8 + 8);
    }
}

/* Rebuilds a recorded triangle's edges and clears its spans (ClearDepthSpans). */
// FUNCTION: LEGOLAND 0x004232b0
void ClearRecordedDepthSpans(struct RecBuf *rb) {
    /* The edge tables are one block: the fill reads the last line from the high half of src[n - 1].f0, and
     * writes idx[n].v (src[0] when n == 8). */
    struct {
        struct RecIdx idx[8];
        struct RecSrc src[8];
    } poly;
    int n = rb->n;
    int i;

    for (i = 0; i < n; i++) {
        struct RecSrc *s = &poly.src[i];
        struct RecEnt *e = &rb->ent[i];
        short w = e->w;
        short w2 = e->w;

        if (w < 0) {
            s->flag = 1;
            poly.idx[i].v = -w;
        } else {
            s->flag = 0;
            poly.idx[i].v = w2;
        }
        poly.idx[i].k = i;
        s->fx = e[-1].s0n << 16;
        s->d = rb->ent[i].d;
    }
    ((short *)&poly.src[i - 1].f0)[1] = (short)(rb->x - 1);
    ClearDepthSpans(n, poly.idx, poly.src);
}

/* Span filler that writes depth 0 over a triangle's spans in the depth buffer. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00423350
__declspec(naked) void ClearDepthSpans(int n, struct RecIdx *idx, struct RecSrc *src) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x60
        mov edx, dword ptr [ebp + 8]
        push ebx
        push esi
        mov esi, dword ptr [ebp + 0xc]
        mov ecx, dword ptr [ebp + 0x10]
        push edi
        mov eax, dword ptr [esi]
        mov dword ptr [ebp - 4], 0
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [esi + edx*8 - 4]
        lea eax, [eax + eax*2]
        shl eax, 4
        inc word ptr [eax + ecx + 2]
        mov edi, dword ptr [CoasterDepthBuffer]
        lea eax, [eax + ecx + 2]
        mov ebx, dword ptr [CoasterSurfacePitch]
        mov eax, dword ptr [esi + edx*8 - 4]
        mov dword ptr [ebp - 8], ebx
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + ecx + 2]
        mov dword ptr [ebp + 8], eax
        mov eax, dword ptr [CoasterSurfaceBits]
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [FilledTriangleCount]
        inc eax
        mov dword ptr [FilledTriangleCount], eax
        mov eax, dword ptr [esi + edx*8 - 4]
        lea eax, [eax + eax*2]
        shl eax, 4
        movsx eax, word ptr [eax + ecx + 2]
        mov dword ptr [esi + edx*8], eax
        mov edx, ebx
        mov ebx, dword ptr [ebp + 0xc]
        imul edx, ebx
        lea eax, [edi + edx*2]
        mov dword ptr [ebp - 0xc], eax
L4233d5:
        mov eax, dword ptr [esi + 4]
        add esi, 8
        lea eax, [eax + eax*2]
        shl eax, 4
        add eax, ecx
        mov edx, dword ptr [eax + 4]
        test edx, edx
        mov edx, dword ptr [eax + 8]
        mov eax, dword ptr [eax + 0x1c]
        je L4233fa
        sub edx, eax
        mov dword ptr [ebp - 0x24], eax
        mov dword ptr [ebp - 0x38], edx
        jmp L423402
L4233fa:
        sub edx, eax
        mov dword ptr [ebp - 0x4c], eax
        mov dword ptr [ebp - 0x60], edx
L423402:
        cmp ebx, dword ptr [esi]
        jge L42345f
L423406:
        inc dword ptr [ebp + 0xc]
        mov eax, dword ptr [ebp - 0x60]
        mov ebx, dword ptr [ebp - 0x38]
        add eax, dword ptr [ebp - 0x4c]
        add ebx, dword ptr [ebp - 0x24]
        mov dword ptr [ebp - 0x60], eax
        mov dword ptr [ebp - 0x38], ebx
        mov ecx, ebx
        sub ecx, eax
        cmp ecx, 0x8000
        jns L423429
        jmp L423445
L423429:
        sar eax, 0x10
        sar ebx, 0x10
        mov edi, dword ptr [ebp - 0xc]
        xchg eax, ebx
        mov dx, word ptr [ebp - 4]
        sub ebx, eax
        lea edi, [edi + eax*2]
L42343c:
        mov word ptr [edi + ebx*2], dx
        add ebx, 1
        jle L42343c
L423445:
        mov ecx, dword ptr [ebp - 8]
        mov edx, dword ptr [ebp + 0xc]
        lea eax, [ecx + ecx]
        mov ecx, dword ptr [ebp - 0xc]
        add ecx, eax
        mov eax, dword ptr [esi]
        cmp edx, eax
        mov dword ptr [ebp - 0xc], ecx
        jl L423406
        mov ecx, dword ptr [ebp + 0x10]
L42345f:
        mov ebx, dword ptr [ebp + 0xc]
        mov eax, dword ptr [ebp + 8]
        cmp ebx, eax
        jl L4233d5
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Zeroes the depth buffer inside the rectangle (inclusive). */
// FUNCTION: LEGOLAND 0x00423480
void ClearCoasterSurfaceRect(struct ClearRect *r) {
    unsigned short *row = CoasterDepthBuffer + r->top * CoasterSurfacePitch;
    int y;
    int x;
    unsigned short *p;

    for (y = r->top; y <= r->bottom; y++) {
        p = row + r->left;
        for (x = r->left; x <= r->right; x++) {
            *p++ = 0;
        }
        row += CoasterSurfacePitch;
    }
}

/* Draws an edge mesh's triangles (RasteriseTriangle) into the locked coaster surface. Times itself into
 * EdgeMeshDrawCycles. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x004234e0
__declspec(naked) void DrawEdgeMeshTriangles(void *param1) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0xe4
        push ebx
        push esi
        lea eax, [ebp - 0x74]
        push edi
        push eax
        call GetCoasterSurface
        add esp, 4
        push eax
        push edx
        rdtsc
        mov dword ptr [ebp - 8], eax
        pop edx
        pop eax
        lea edx, [ebp - 0xc8]
        lea ecx, [ebp - 0xe4]
        mov dword ptr [ebp - 0x20], edx
        mov edx, dword ptr [ebp + 8]
        lea eax, [ebp - 0xac]
        mov dword ptr [ebp - 0x24], ecx
        mov ecx, dword ptr [edx + 0xc]
        mov dword ptr [ebp - 0x1c], eax
        xor eax, eax
        mov dword ptr [ebp - 0x4c], 1
        cmp ecx, eax
        mov dword ptr [ebp - 0x14], offset GouraudSpanFillers
        mov dword ptr [ebp - 0x10], eax
        jle L4236ad
        mov dword ptr [ebp - 0xc], eax
L423540:
        mov ecx, dword ptr [ebp - 0xc]
        mov esi, dword ptr [edx + 0x18]
        add ecx, esi
        lea esi, [ebp - 0x5c]
        sub esi, ecx
        mov edi, 3
L423552:
        mov eax, dword ptr [ecx]
        test eax, 0x80000000
        je L423569
        mov ebx, dword ptr [edx + 0x14]
        and eax, 0x7fffffff
        mov eax, dword ptr [ebx + eax*8 + 4]
        jmp L423574
L423569:
        mov ebx, dword ptr [edx + 0x14]
        and eax, 0x7fffffff
        mov eax, dword ptr [ebx + eax*8]
L423574:
        mov dword ptr [esi + ecx], eax
        add ecx, 4
        dec edi
        jne L423552
        mov ecx, dword ptr [ebp - 0x58]
        mov eax, dword ptr [ebp - 0x5c]
        mov esi, dword ptr [edx + 0x10]
        mov dword ptr [ebp - 0x50], eax
        lea ecx, [ecx + ecx*4]
        lea eax, [eax + eax*4]
        mov ebx, dword ptr [esi + eax*4]
        lea edi, [esi + ecx*4]
        lea ecx, [esi + eax*4]
        mov eax, dword ptr [edi]
        sub eax, ebx
        mov dword ptr [ebp - 4], eax
        fild dword ptr [ebp - 4]
        fstp dword ptr [ebp - 0x38]
        mov eax, dword ptr [edi + 4]
        mov ebx, dword ptr [ecx + 4]
        sub eax, ebx
        mov dword ptr [ebp - 4], eax
        mov eax, dword ptr [ebp - 0x54]
        fild dword ptr [ebp - 4]
        lea eax, [eax + eax*4]
        fstp dword ptr [ebp - 0x30]
        lea ebx, [esi + eax*4]
        mov eax, dword ptr [esi + eax*4]
        sub eax, dword ptr [ecx]
        mov dword ptr [ebp - 4], eax
        fild dword ptr [ebp - 4]
        fstp dword ptr [ebp - 0x34]
        mov eax, dword ptr [ebx + 4]
        sub eax, dword ptr [ecx + 4]
        mov dword ptr [ebp - 4], eax
        fild dword ptr [ebp - 4]
        fst dword ptr [ebp - 0x2c]
        fmul dword ptr [ebp - 0x38]
        fld dword ptr [ebp - 0x34]
        fmul dword ptr [ebp - 0x30]
        fsubp st(1), st
        fst dword ptr [ebp - 0x28]
        fcomp dword ptr [FLOAT_004ab390]
        fnstsw ax
        test ah, 0x41
        jne L423692
        mov dword ptr [ebp - 0x44], 0xff
        mov dword ptr [ebp - 0x40], 0
        mov edx, dword ptr [ecx + 0xc]
        mov dword ptr [ebp - 0x40], edx
        mov eax, dword ptr [ecx + 0xc]
        and eax, 0xff
        mov dword ptr [ebp - 0x44], eax
        mov ecx, dword ptr [edi + 0xc]
        or edx, ecx
        mov dword ptr [ebp - 0x40], edx
        mov ecx, dword ptr [edi + 0xc]
        and eax, ecx
        mov dword ptr [ebp - 0x44], eax
        mov ecx, dword ptr [ebx + 0xc]
        or edx, ecx
        mov dword ptr [ebp - 0x40], edx
        mov edi, dword ptr [ebx + 0xc]
        and edx, 0xf
        and eax, edi
        cmp dl, 0xf
        mov dword ptr [ebp - 0x44], eax
        jne L42368f
        lea edx, [ebp - 0x5c]
        lea eax, [ebp - 0xdc]
        mov edi, 3
L423650:
        mov ecx, dword ptr [edx]
        add edx, 4
        add eax, 0x1c
        dec edi
        lea ecx, [ecx + ecx*4]
        mov ebx, dword ptr [esi + ecx*4 + 4]
        lea ecx, [esi + ecx*4]
        mov dword ptr [eax - 0x20], ebx
        mov ebx, dword ptr [ecx]
        mov dword ptr [eax - 0x1c], ebx
        mov ebx, dword ptr [ecx + 0x10]
        mov dword ptr [eax - 0x18], ebx
        mov ecx, dword ptr [ecx + 8]
        mov dword ptr [eax - 0x14], ecx
        jne L423650
        mov edx, dword ptr [ebp + 8]
        lea ecx, [ebp - 0x4c]
        push ecx
        push 3
        mov eax, dword ptr [edx]
        mov dword ptr [ebp - 0x48], eax
        call RasteriseTriangle
        add esp, 8
L42368f:
        mov edx, dword ptr [ebp + 8]
L423692:
        mov eax, dword ptr [ebp - 0x10]
        mov esi, dword ptr [ebp - 0xc]
        mov ecx, dword ptr [edx + 0xc]
        inc eax
        add esi, 0xc
        cmp eax, ecx
        mov dword ptr [ebp - 0x10], eax
        mov dword ptr [ebp - 0xc], esi
        jl L423540
L4236ad:
        push eax
        push edx
        rdtsc
        sub eax, dword ptr [ebp - 8]
        mov dword ptr [ebp - 8], eax
        pop edx
        pop eax
        mov edx, dword ptr [ebp - 8]
        mov esi, dword ptr [EdgeMeshDrawCycles]
        lea eax, [ebp - 0x74]
        add esi, edx
        push eax
        mov dword ptr [EdgeMeshDrawCycles], esi
        call ReleaseCoasterSurface
        add esp, 4
        mov eax, 1
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Switches the FPU to single precision for the renderer; returns the old control word. */
// FUNCTION: LEGOLAND 0x004236f0
unsigned short FPUSetSinglePrecision(void) {
    unsigned int control_word;
    unsigned short replacement;
    unsigned char exception_masks;

    __asm {
        fstcw word ptr control_word
    }
    exception_masks = control_word;
    if ((control_word & 0x300) != 0 || (exception_masks & 0x3f) != 0x3f || (control_word & 0xc00) != 0) {
        __asm {
            mov ax, word ptr control_word
            and ax, 0xfcff
            or ax, 0x3f
            and ax, 0xf3ff
            mov replacement, ax
            fldcw replacement
        }
    }
    return control_word;
}

/* Restores the FPU control word. */
// FUNCTION: LEGOLAND 0x00423730
void FPURestoreControlWord(unsigned short control_word) {
    __asm {
        fldcw control_word
    }
}

/* Builds the palette's shade tables and the shade clamp table. */
// FUNCTION: LEGOLAND 0x00423740
void InitCoasterShading(void) {
    BuildCoasterShadeTables();
    InitShadeClampTable();
}

/* Locks the video surface for the renderer: CoasterSurfaceBits and the pitch in pixels. */
// FUNCTION: LEGOLAND 0x00423760
int GetCoasterSurface(struct VideoArg *arg) {
    int result = GetVideoSurface(arg);
    if (result == 0) {
        return 0;
    }
    CoasterSurfaceBits = arg->bits;
    CoasterSurfacePitch = arg->pitch >> 1;
    return 1;
}

/* Empty: the counterpart of GetCoasterSurface (the surface is unlocked elsewhere). */
// FUNCTION: LEGOLAND 0x00423790
void ReleaseCoasterSurface(void) {
}

/* Debug: draws an edge set as dotted lines (white). */
// FUNCTION: LEGOLAND 0x004237a0
void DrawEdgeSet(struct EdgeSet *set) {
    int i;

    for (i = 0; i < set->count; i++) {
        DrawDottedLine(&set->base[set->pairs[i].a], &set->base[set->pairs[i].b], -1);
    }
}

/* Debug: a dotted line of 30 points between two projected points. */
// FUNCTION: LEGOLAND 0x004237f0
void DrawDottedLine(void *a, void *b, short c) {
    int *pa = (int *)a;
    int *pb = (int *)b;
    int pts[30][4];
    float x, y, dx, dy;
    int i;

    x = (float)pa[0];
    y = (float)pa[1];
    dx = (float)(pb[0] - pa[0]) * FLOAT_004ab44c;
    dy = (float)(pb[1] - pa[1]) * FLOAT_004ab44c;
    for (i = 0; i < 30; i++) {
        pts[i][0] = (int)x;
        pts[i][1] = (int)y;
        x += dx;
        y += dy;
    }
    PlotClippedPoints(&pts[0][0], 30, c);
}

/* Debug: plots points (16-bit colour c) on the screen surface inside the castle view. */
// FUNCTION: LEGOLAND 0x004238a0
void PlotClippedPoints(int *points, int count, short c) {
    struct VideoArg surf;
    int x;
    int y;

    if (GetSprite(&surf, NULL)) {
        while (count-- > 0) {
            x = points[0];
            if (x > CastleViewRect[0] && x < CastleViewRect[2]) {
                y = points[1];
                if (y > CastleViewRect[1] && y < CastleViewRect[3]) {
                    ((unsigned short *)surf.bits)[x + (surf.pitch >> 1) * y] = c;
                }
            }
            points += 4;
        }
        ReleaseSprite(&surf);
    }
}

/* 1 while the castle is on the map. */
// FUNCTION: LEGOLAND 0x00423930
unsigned int IsCastleCoasterPlaced(void) {
    return CastleCoasterPlaced;
}

/* The castle's fill_open_end: resets its directions (prev E, next W) and its heights from CastlePieceClass. */
// FUNCTION: LEGOLAND 0x00423940
void CastleFillOpenEnd(struct PathSeg *arg1) {
    arg1->dir_a = 2;
    arg1->f18 = (float)CastlePieceClass.f8;
    arg1->dir_b = 8;
    arg1->f24 = (float)CastlePieceClass.f4;
}

/* The castle's set_heights: always the CastlePieceClass heights. */
// FUNCTION: LEGOLAND 0x00423970
void CastleSetHeights(struct PathSeg *param_1) {
    param_1->f18 = (float)CastlePieceClass.f8;
    param_1->f24 = (float)CastlePieceClass.f4;
}

/* The castle's get_curve: its own fixed curve stretch CastleCurves. */
// FUNCTION: LEGOLAND 0x00423990
struct SegBlob *CastleGetCurve(void) {
    return CastleCurves;
}

/* Empty in the original (does nothing). */
/* Empty. */
// FUNCTION: LEGOLAND 0x004239a0
void Empty_004239a0(void) {
}

/* Map cell of the castle's next door (where the first track piece leaves it), relative to a. */
// FUNCTION: LEGOLAND 0x004239b0
void GetCastleCoasterDoorA(struct Int16Pair *a, struct Int16Pair *b) {
    unsigned char *p = (unsigned char *)&CastleCoasterRide->footprint;
    b->x = *(unsigned short *)(p + 8) + a->x;
    b->y = *(unsigned short *)(p + 4) + a->y - 2;
}

/* Map cell of the castle's prev door (where the last track piece enters it), relative to a. */
// FUNCTION: LEGOLAND 0x004239e0
void GetCastleCoasterDoorB(struct Int16Pair *a, struct Int16Pair *b) {
    unsigned char *global_ptr = (unsigned char *)&CastleCoasterRide->footprint;
    b->x = *(unsigned short *)global_ptr + a->x - 2;
    b->y = *(unsigned short *)(global_ptr + 0xc) + a->y;
}

/* Builds the castle's own track: CastleCurves = a straight in from the prev door, a quarter arc and a straight out
 * to the next door; CastleTrackCells lists every 2x2 cell it crosses with its stretch (k) and parameter range,
 * chained as footprints. */
// FUNCTION: LEGOLAND 0x00423a10
void BuildCastleTrack(void) {
    struct Int16Pair origin;
    struct Int16Pair q;
    struct Int16Pair p;
    struct Int16Pair r;
    struct Int16Pair pt;
    struct Int16Pair pt1;
    struct FVec3 v1;
    struct FVec3 v2;
    struct FVec3 v3;
    struct FVec3 c;
    float t;
    float step;
    int n;
    int i;
    int cnt = 0;

    origin.x = 0;
    origin.y = 0;
    GetCastleCoasterDoorB(&origin, &q);
    GetCastleCoasterDoorA(&origin, &p);
    r.x = q.x;
    r.y = p.y;
    pt.x = r.x + 1;
    pt.y = q.y + 2;
    pt1.x = r.x + 1;
    pt1.y = r.y + 2;
    MapPosToWorld(&pt, 0.0f, &v1);
    MapPosToWorld(&pt1, 0.0f, &v2);
    c.x = -10.0f;
    c.y = 0.0f;
    c.z = 0.0f;
    InitLineCurve((struct LineCurve *)&CastleCurves[0], &v1.x, &v2.x, (struct Words3 *)&c);
    n = (q.y - r.y) >> 1;
    t = CastleCurves[0].start;
    pt = q;
    step = (CastleCurves[0].end - CastleCurves[0].start) / n;
    for (i = 0; i < n; i++) {
        CastleTrackCells[cnt].xy = pt;
        pt.y -= 2;
        CastleTrackCells[cnt].k = 0;
        CastleTrackCells[cnt].a = t;
        t += step;
        CastleTrackCells[cnt].b = t;
        cnt++;
    }
    pt.x = -1;
    pt.y = 0;
    MapPosToWorld(&pt, 0.0f, &v1);
    pt.x = 0;
    pt.y = -1;
    MapPosToWorld(&pt, 0.0f, &v2);
    pt.x = r.x + 2;
    pt.y = r.y + 2;
    MapPosToWorld(&pt, 0.0f, &v3);
    InitArcCurve((struct Words3 *)&v1, (struct Words3 *)&v2, (struct Words3 *)&v3, (struct ArcCurve *)&CastleCurves[1], 0x3fc00000, 0x3f000000);
    CastleTrackCells[cnt].xy = r;
    CastleTrackCells[cnt].k = 1;
    CastleTrackCells[cnt].a = CastleCurves[1].start;
    CastleTrackCells[cnt].b = CastleCurves[1].end;
    cnt++;
    pt.x = r.x + 2;
    pt.y = r.y + 1;
    pt1.x = p.x + 2;
    pt1.y = p.y + 1;
    MapPosToWorld(&pt, 0.0f, &v1);
    MapPosToWorld(&pt1, 0.0f, &v2);
    c.x = 0.0f;
    c.y = -10.0f;
    c.z = 0.0f;
    InitLineCurve((struct LineCurve *)&CastleCurves[2], &v1.x, &v2.x, (struct Words3 *)&c);
    CastleCurves[0].prev = 0;
    CastleCurves[0].next = &CastleCurves[1];
    CastleCurves[1].prev = &CastleCurves[0];
    CastleCurves[1].next = &CastleCurves[2];
    CastleCurves[2].prev = &CastleCurves[1];
    CastleCurves[2].next = 0;
    n = (p.x - r.x) >> 1;
    t = CastleCurves[2].start;
    step = (CastleCurves[2].end - CastleCurves[2].start) / n;
    pt = r;
    pt.x = r.x + 2;
    for (i = 0; i < n; i++) {
        CastleTrackCells[cnt].xy = pt;
        pt.x += 2;
        CastleTrackCells[cnt].k = 2;
        CastleTrackCells[cnt].a = t;
        t += step;
        CastleTrackCells[cnt].b = t;
        cnt++;
    }
    for (i = 0; i < cnt; i++) {
        CastleTrackCells[i + 1].fp.v[0] = CastleTrackCells[i].xy.x;
        CastleTrackCells[i + 1].fp.v[1] = CastleTrackCells[i].xy.y;
        CastleTrackCells[i + 1].fp.v[2] = CastleTrackCells[i].xy.x + 1;
        CastleTrackCells[i + 1].fp.v[3] = CastleTrackCells[i].xy.y + 1;
        CastleTrackCells[i + 1].fp.next = &CastleTrackCells[i + 2].fp;
    }
    CastleTrackCellCount = cnt;
    CastleTrackCells[i].fp.next = 0;
}

/* Sets CastlePieceClass's doors from the castle's footprint and builds its track. */
// FUNCTION: LEGOLAND 0x00423d40
void InitCastleCoasterDoors(void) {
    struct Int16Pair a;
    struct Int16Pair b;
    struct Int16Pair c;

    a.x = 0;
    a.y = 0;
    GetCastleCoasterDoorA(&a, &b);
    GetCastleCoasterDoorB(&a, &c);
    CastlePieceClass.next_door.x = b.x;
    CastlePieceClass.next_door.y = b.y;
    CastlePieceClass.prev_door.x = c.x;
    CastlePieceClass.prev_door.y = c.y;
    BuildCastleTrack();
}

/* Sets the castle's track methods (station start, leaving, arriving) and its doors. */
// FUNCTION: LEGOLAND 0x00423db0
void InitCastleCoaster(void) {
    CastleCoaster.get_start = GetCastleStationStart;
    CastleCoaster.step_leaving = CastleStepLeaving;
    CastleCoaster.step_arriving = CastleStepArriving;
    InitCastleCoasterDoors();
}

/* Cursor footprint for placing the castle: the ride's footprint plus its track cells. */
// FUNCTION: LEGOLAND 0x00423de0
void BuildCastleBaseFootprint(void) {
    struct Ride *v0 = CastleCoasterRide;
    memcpy(&CastleCoasterFootprint, &v0->footprint, sizeof(struct Footprint));
    CastleCoasterFootprint.next = &CastleTrackCells[1].fp;
    CastleTrackCells[CastleTrackCellCount].fp.next = 0;
}

/* Footprint of the castle with all its pieces (for the query cursor). */
// FUNCTION: LEGOLAND 0x00423e20
void BuildCastleCoasterFootprint(void) {
    struct PathSeg *n;
    struct Footprint *tail;

    CastleCoasterFootprint = CastleCoasterRide->footprint;
    CastleCoasterFootprint.next = &CastleTrackCells[1].fp;
    CastleTrackCells[CastleTrackCellCount].fp.next = 0;
    tail = &CastleTrackCells[CastleTrackCellCount].fp;
    if (CastleCoaster.flags == 2) {
        for (n = (struct PathSeg *)CastleCoaster.piece.next; n != (struct PathSeg *)&CastleCoaster.piece.flags; n = n->next) {
            tail->next = &n->fp;
            tail = &n->fp;
        }
        tail->next = 0;
        return;
    }
    for (n = (struct PathSeg *)CastleCoaster.piece.next; n != 0; n = n->next) {
        tail->next = &n->fp;
        tail = &n->fp;
    }
    for (n = (struct PathSeg *)CastleCoaster.piece.prev; n != 0; n = n->prev) {
        tail->next = &n->fp;
        tail = &n->fp;
    }
    tail->next = 0;
}

/* Removes every track piece (around the ring, or from head and tail back to the castle). */
// FUNCTION: LEGOLAND 0x00423ec0
void RemoveAllCoasterPieces(struct CastleState *castle) {
    struct PathSeg *n;
    struct PathSeg *next;
    struct PathSeg *end;

    if (CastleCoaster.flags == 2) {
        n = (struct PathSeg *)CastleCoaster.piece.next;
        while (n != (struct PathSeg *)&CastleCoaster.piece.flags) {
            next = n->next;
            RemoveCoasterPiece(n);
            n = next;
        }
        return;
    }
    n = (struct PathSeg *)CastleCoaster.head;
    end = (struct PathSeg *)&castle->piece;
    while (n != end) {
        next = n->next;
        RemoveCoasterPiece(n);
        n = next;
    }
    n = (struct PathSeg *)CastleCoaster.tail;
    while (n != end) {
        next = n->prev;
        RemoveCoasterPiece(n);
        n = next;
    }
}

/* For the map screen: twice the height of the piece's middle (*a2), and the next piece's cell and height (a3, *a4);
 * *a6 = 1 for the first piece after the castle. */
// FUNCTION: LEGOLAND 0x00423f40
int GetPieceMapLine(struct Point *pos, float *a2, struct Point *a3, float *a4, struct PathSeg *node, int *a6) {
    struct FVec3 a;
    struct FVec3 b;
    struct SegBlob *c;
    struct PathSeg *n2;

    *a6 = node->prev == (struct PathSeg *)&CastleCoaster.piece.flags;
    c = GetPieceCurve(node, &a);
    c->vt->pos(c, (c->end + c->start) * 0.5f, &b.x);
    b.z += a.z;
    *a2 = b.z * -2.0f;
    n2 = node->next;
    if (n2 != 0 && n2 != (struct PathSeg *)&CastleCoaster.piece.flags) {
        c = GetPieceCurve(n2, &a);
        c->vt->pos(c, (c->end + c->start) * 0.5f, &b.x);
        b.z += a.z;
        *a4 = b.z * -2.0f;
        a3->x = n2->x;
        a3->y = n2->y;
        return 1;
    }
    *a3 = *pos;
    if (node->next == (struct PathSeg *)&CastleCoaster.piece.flags) {
        a3->y -= 16;
    }
    *a4 = 0.0f;
    return 1;
}

/* GetPieceMapLine for the piece at map cell pos; 0 if there is none. */
// FUNCTION: LEGOLAND 0x00424050
int GetCoasterPieceMapLine(struct Point *pos, void *a2, void *a3, void *a4, void *a5) {
    struct PathSeg *n;
    struct PathSeg *next;

    if (CastleCoaster.flags == 2) {
        n = (struct PathSeg *)CastleCoaster.piece.next;
        while (n != (struct PathSeg *)&CastleCoaster.piece.flags) {
            next = n->next;
            if (n->x == pos->x && n->y == pos->y) {
                return GetPieceMapLine(pos, a2, a3, a4, n, a5);
            }
            n = next;
        }
        return 0;
    }
    n = (struct PathSeg *)CastleCoaster.head;
    while (n != (struct PathSeg *)&CastleCoaster.piece.flags) {
        next = n->next;
        if (n->x == pos->x && n->y == pos->y) {
            return GetPieceMapLine(pos, a2, a3, a4, n, a5);
        }
        n = next;
    }
    n = (struct PathSeg *)CastleCoaster.tail;
    while (n != (struct PathSeg *)&CastleCoaster.piece.flags) {
        next = n->prev;
        if (n->x == pos->x && n->y == pos->y) {
            return GetPieceMapLine(pos, a2, a3, a4, n, a5);
        }
        n = next;
    }
    return 0;
}

/* &CastleCoaster while the castle is placed, else NULL. */
// FUNCTION: LEGOLAND 0x00424140
void *GetPlacedCastleCoaster(void) {
    return CastleCoasterPlaced ? &CastleCoaster : 0;
}

/* Castle object load: loads the matte sprite, the models, shading, curve and train tables. */
// FUNCTION: LEGOLAND 0x00424150
void CastleObjLoad(struct Element *param_1) {
    CastleCoasterPlaced = 0;
    ReturnOne_00477400();
    CastleObjElement = param_1;
    CastleCoasterRide = param_1->ride;
    if (CastleCoasterRide->layer != NULL) {
        CastleCoasterRide->layer->flags |= 0x2000;
    }
    CastleCoasterRide->flags |= 0x20;
    CastleMatteSprite = LoadSprite("Castle Matte.lls", 1);
    InitCastleView();
    InitVecMathOps();
    LoadRollercoasterFiles();
    InitCoasterShading();
    InitCurveFunctionTable();
    InitTrackDrawing();
    ReturnOne_0041ef00();
    InitCurveVecMath();
    InitCastleCoaster();
    InitTrainVecMath();
    LoadCoasterTrainCarModels();
    LoadCoasterTrainWheelModel();
}

/* Castle object unload: frees the matte sprite and resets the track state. */
// FUNCTION: LEGOLAND 0x004241e0
void CastleObjUnload(void) {
    KillSprite(CastleMatteSprite);
    CastleCoaster.next_spots.mask = 0;
    CastleCoaster.prev_spots.mask = 0;
    ClearDirPoints(&CastleCoaster.next_spots);
    ClearDirPoints(&CastleCoaster.prev_spots);
    CastleCoaster.tail = 0;
    CastleCoaster.head = 0;
    ResetPieceEnd((struct PieceEnd *)&CastleCoaster.piece.end_a);
    ResetPieceEnd((struct PieceEnd *)&CastleCoaster.piece.end_b);
    CastleCoaster.flags = 0;
}

/* The castle's SetEditMode handler: castle placement cursor unless one is already placed. */
// FUNCTION: LEGOLAND 0x00424240
unsigned int CastleObjSetEditMode(void) {
    if (CastleCoasterPlaced != 0) {
        return CastleCoasterPlaced;
    }
    EditMode.unk0 = 1;
    EditMode.unk8 = CastleCoasterRide;
    DefaultCursor(&EditCursor);
    BuildCastleBaseFootprint();
    return ((unsigned int (*)(const char *))SetEditCursorFootPrint)((const char *)&CastleCoasterFootprint); /* TODO: fold — SetEditCursorFootPrint leaves memcpy's eax, used here as uint return */
}

/* The castle's CalcCursor handler: castle footprint at the mouse; blocked if one is already placed. */
// FUNCTION: LEGOLAND 0x00424280
void CastleObjCalcCursor(struct Element *param_1, int *param_2, unsigned int param_3) {
    struct Cursor *cursor;

    BuildCastleBaseFootprint();
    SetEditCursorFootPrint(&CastleCoasterFootprint);
    ScreenToMapRef((int *)param_2, &EditCursor.tile_x, param_3);
    EditCursor.field_1830 = 0;
    cursor = GetOrAddPathCursor(&EditCursor);
    if (cursor != NULL) {
        cursor->field_1414[1]++;
        cursor->field_1414[0]++;
    }
    ResetCursorBuildable(&EditCursor);
    ValidateCursor(&EditCursor, param_1->data);
    if (CastleCoasterPlaced != 0) {
        SetCursorBlocked(&EditCursor, 0xf);
        PropagateCursorBlocked(&EditCursor);
    }
}

/* The castle's AddObject handler: puts the castle on the map with its track cells, makes it the only piece of an
 * open chain (head = tail = castle), creates the train and the visitor queue. */
// FUNCTION: LEGOLAND 0x00424320
void PlaceCastleCoaster(struct Element *obj, short *pt) {
    short v[2];

    SetCoasterPieceCount(0);
    AddMapObjectIfEnabled(obj, (int *)pt);
    v[0] = pt[0];
    v[1] = pt[2];
    AddCastleCoasterMapObjects(v);
    InitPieceHead(&CastleCoaster.piece);
    CastleCoaster.flags = 1;
    CastleCoaster.piece.x = pt[0];
    CastleCoaster.piece.y = pt[2];
    CastleCoaster.piece.cls = &CastlePieceClass;
    CastleCoaster.piece.owner = &CastleCoaster;
    CastleCoaster.piece.ride = CastleCoasterRide;
    CastleCoaster.tail = (struct PathSeg *)&CastleCoaster.piece;
    CastleCoaster.head = (struct PathSeg *)&CastleCoaster.piece;
    CastleCoaster.next_spots.mask = 0;
    CastleCoaster.prev_spots.mask = 0;
    FindPathSegDoorAPoints((struct PathSeg *)&CastleCoaster.piece, &CastleCoaster.next_spots);
    FindPathSegDoorBPoints((struct PathSeg *)&CastleCoaster.piece, &CastleCoaster.prev_spots);
    Empty_0041d1c0(&CastleCoaster.next_spots.mask);
    Empty_0041d1c0(&CastleCoaster.prev_spots.mask);
    PieceFillOpenEnd((struct PathSeg *)&CastleCoaster.piece.flags);
    PieceSetHeights((struct PathSeg *)&CastleCoaster.piece, 0);
    CreateCastleTrain((struct CastleState *)&CastleCoaster);
    InitCastleVisitorList((struct CastleState *)&CastleCoaster);
    CastleCoaster.piece.flags |= 6;
    CastleCoasterPlaced = 1;
}

/* The castle's DCalcCursor handler: query cursor over the castle and all its pieces. */
// FUNCTION: LEGOLAND 0x00424440
void CastleObjDCalcCursor(struct Element *param_1, struct Point *param_2) {
    int x;
    int y;

    BuildCastleCoasterFootprint();
    x = CastleCoaster.piece.x;
    memcpy(QueryCursor.field_1414, &CastleCoasterFootprint, sizeof(struct Footprint));
    y = CastleCoaster.piece.y;
    QueryCursor.tile_x = x;
    QueryCursor.tile_y = y;
    // STRING: LEGOLAND 0x004b5b7c
    if ((void *)QueryClass != ElemID("CASTLE_DUMMY")->data) {
        ResetCursorBuildable(&QueryCursor);
        return;
    }
    QueryCursor.field_140c = 0;
}

/* The castle's RemoveObject handler: stops the train, removes every piece, the train, the visitors and riders, and
 * the castle with its track cells. */
// FUNCTION: LEGOLAND 0x004244b0
void RemoveCastleCoaster(struct Element *param_1, TileId param_2, struct Cursor *param_3) {
    struct Cursor cursor;
    TileId tile;
    struct Element *handle;

    if (CastleCoasterPlaced != 0) {
        tile.pos.x = CastleCoaster.piece.x;
        tile.pos.y = CastleCoaster.piece.y;
        cursor.tile_x = tile.pos.x;
        cursor.tile_y = tile.pos.y;
        cursor.footprint = CastleCoasterRide->footprint;
        cursor.field_1414[4] = 0;
        StopCastleTrain((struct CastleState *)&CastleCoaster);
        RemoveAllCoasterPieces(&CastleCoaster);
        ClearDirPoints(&CastleCoaster.next_spots);
        ClearDirPoints(&CastleCoaster.prev_spots);
        CastleCoaster.tail = 0;
        CastleCoaster.head = 0;
        DestroyCastleTrain((struct CastleState *)&CastleCoaster);
        FreeAllCastleVisitors((struct CastleState *)&CastleCoaster);
        CastleRemoveAllRiders();
        handle = GetCastleDispatchElement(0);
        RemoveMapObjectIfEnabled(handle, tile, &cursor);
        RemoveCastleCoasterMapObjects(&CastleCoaster.piece.pos);
        CastleCoasterPlaced = 0;
        Empty_004775f0();
        FreeMoviePool();
        SetCoasterPieceCount(0);
    }
}

/* Puts a CASTLE_DUMMY on every cell of the castle's own track. */
// FUNCTION: LEGOLAND 0x004245b0
void AddCastleCoasterMapObjects(short *param_1) {
    int pt[2];
    int i;

    for (i = 0; i < (int)CastleTrackCellCount; i++) {
        short *q = &CastleTrackCells[i].xy.y;
        pt[0] = q[-1] + param_1[0];
        pt[1] = param_1[1] + q[0];
        AddMapObjectIfEnabled(CastleDummyElement, pt);
    }
    AddCoasterPieceCount(CastleTrackCellCount);
}

/* Removes the CASTLE_DUMMY from every cell of the castle's own track. */
// FUNCTION: LEGOLAND 0x00424620
void RemoveCastleCoasterMapObjects(struct Int16Pair *pos) {
    struct Cursor cursor;
    TileId tile;
    int i;

    for (i = 0; i < (int)CastleTrackCellCount; i++) {
        tile.pos.x = CastleTrackCells[i].xy.x + pos->x;
        tile.pos.y = CastleTrackCells[i].xy.y + pos->y;
        cursor.tile_x = tile.pos.x;
        cursor.tile_y = tile.pos.y;
        cursor.footprint = CastleDummyRide->footprint;
        cursor.field_1414[4] = 0;
        RemoveMapObjectIfEnabled(CastleDummyElement, tile, &cursor);
    }
    AddCoasterPieceCount(-(int)CastleTrackCellCount);
}

/* The castle's cb_c0: the number of pieces, 0 when param_2 is set and the track is not a closed ring. */
// FUNCTION: LEGOLAND 0x004246e0
unsigned int CastleObjPieceCount(struct Element *param_1, unsigned int param_2) {
    if (param_2 != 0 && CastleCoaster.flags != 2) {
        return 0;
    }
    return GetCoasterPieceCount();
}

/* Renders a CASTLE_DUMMY cell: the castle track stretch over that cell, then the train cars on it. */
// FUNCTION: LEGOLAND 0x00424700
void RenderCastleDummy(struct Element *a1, unsigned int a2, unsigned int a3, unsigned char *p) {
    struct FVec3 buf;
    struct SegBlob obj;
    short key[2];
    int want;
    int i;

    MapPosToWorld(&CastleCoaster.piece.pos, (float)CastlePieceClass.f8, &buf);
    key[0] = p[0];
    key[1] = p[1];
    want = *(int *)key;
    for (i = 0; i < (int)CastleTrackCellCount; i++) {
        key[0] = CastleTrackCells[i].xy.x + CastleCoaster.piece.x;
        key[1] = CastleTrackCells[i].xy.y + CastleCoaster.piece.y;
        if (want == *(int *)key) {
            obj = ((struct SegBlob *)&CastleCurves)[CastleTrackCells[i].k];
            obj.start = CastleTrackCells[i].a;
            obj.end = CastleTrackCells[i].b;
            DrawTrackStretch(&obj, (struct FVec3 *)&buf, 1, 0);
            break;
        }
    }
    DrawCastleTrainOnPiece((struct PieceHead *)&CastleCoaster.piece.flags);
}

/* The castle dummy's RemoveObject handler: removes the whole castle. */
// FUNCTION: LEGOLAND 0x00424800
void CastleDummyRemoveObject(void) {
    TileId tile;

    tile.pos.x = 0;
    tile.pos.y = 0;
    RemoveCastleCoaster(0, tile, 0);
}

/* The castle dummy's DCalcCursor handler: the castle's query cursor. */
// FUNCTION: LEGOLAND 0x00424820
void CastleDummyDCalcCursor(void) {
    CastleObjDCalcCursor(0, 0);
}

/* CASTLE_DUMMY load: remembers its element and ride. */
// FUNCTION: LEGOLAND 0x00424830
void CastleDummyLoad(struct Element *param_1) {
    struct Ride *inner;

    inner = param_1->ride;
    CastleDummyElement = param_1;
    CastleDummyRide = inner;
    inner->layer->flags |= 0x2000;
}

/* The castle's get_start: the train starts on the castle's last stretch at parameter 0.1. */
// FUNCTION: LEGOLAND 0x00424850
void GetCastleStationStart(struct CastleState *p1, struct SegBlob **p2, float *p3, struct FVec3 *p4) {
    *p2 = &CastleCurves[2];
    *(unsigned int *)p3 = 0x3dcccccd;
    MapPosToWorld(&p1->piece.pos, (float)CastlePieceClass.f8, p4);
}

/* The castle's step_leaving: a normal physics step. */
// FUNCTION: LEGOLAND 0x00424890
float CastleStepLeaving(struct Train *param_1, float param_2) {
    return StepTrainToSegmentEnd(param_1, param_2);
}

/* Integrator derivative while arriving: the normal one plus a braking term that brings the train to a stop at the
 * station. */
// FUNCTION: LEGOLAND 0x004248b0
void CastleBrakingDeriv(struct Integrator *obj, float unused, struct FloatArray *out) {
    struct Train *e = obj->owner;
    struct CastleState *sub = e->track;
    struct FVec3 v;
    void *s[2]; /* the piece and stretch of a CurveWalker (the rest is not read) */
    float t;
    float r;

    TrainIntegratorDeriv(obj, unused, out);
    t = GetTrainTrackSpeed(e);
    MapPosToWorld(&sub->piece.pos, (float)CastlePieceClass.f8, &v);
    r = t * t;
    s[0] = &sub->piece;
    s[1] = &CastleCurves[2];
    r = -(r / (CurveArcLength((struct CurveWalker *)s, 0x3dcccccd, &e->walker, *(unsigned int *)&e->motion.pos, 1, 0) * FLOAT_004ab404 * 2));

    out->data[1] = SumTrainCarMasses(e) * r * t;
}

/* 1 when the train is on the castle's last stretch past parameter 0.099 (back at the station). */
// FUNCTION: LEGOLAND 0x00424960
int HasTrainReachedStation(struct Train *param_1) {
    if (param_1->walker.node != &CastleCurves[2]) {
        return 0;
    }
    if (param_1->motion.pos > FLOAT_004ab454) {
        return 1;
    }
    return 0;
}

/* The castle's step_arriving: stops the train once it reaches the station (recording arrive_time), else steps it
 * with the braking derivative. */
// FUNCTION: LEGOLAND 0x00424990
float CastleStepArriving(struct Train *self, float value) {
    if (HasTrainReachedStation(self) != 0) {
        StopCastleTrain(self->track);
        self->track->arrive_time = GetGameTimer();
        return value;
    }
    self->integ.deriv = CastleBrakingDeriv;
    return StepTrainToSegmentEnd(self, value);
}

/* Creates the castle's train. */
// FUNCTION: LEGOLAND 0x004249e0
void CreateCastleTrain(struct CastleState *param_1) {
    param_1->train = CreateTrain(param_1);
    param_1->arrive_time = 0;
}

/* Frees the castle's train. */
// FUNCTION: LEGOLAND 0x00424a00
void DestroyCastleTrain(struct CastleState *param_1) {
    DestroyTrain(&param_1->train);
}

/* Draws the train cars standing on the given piece. */
// FUNCTION: LEGOLAND 0x00424a20
void DrawCastleTrainOnPiece(struct PieceHead *param_1) {
    struct Train *obj = param_1->owner->train;
    if (AnyCastleLinkVisible() != 0) {
        DrawTrainCarsOnPiece(obj, (struct PathSeg *)param_1);
    }
}

/* Per frame: clears the drawn flags, steps the train if it runs (ending its start hold), updates the cars' screen
 * rectangles. */
// FUNCTION: LEGOLAND 0x00424a50
void UpdateCastleTrain(struct CastleState *param_1) {
    struct Train *host = (struct Train *)param_1->train;

    ClearAllTrainCarsDrawn(host);
    if (IsTrainRunning(param_1->train) != 0) {
        if (IsTrainStartDelayed(param_1->train) != 0) {
            UpdateTrainStartDelay((struct Train *)host);
        }
        StepTrainForFrame(host);
    }
    UpdateAllTrainCarBounds(host);
}

/* Stops the train and unseats its riders. */
// FUNCTION: LEGOLAND 0x00424ab0
void StopCastleTrain(struct CastleState *this) {
    if (IsTrainRunning(this->train) != 0) {
        StopTrain(this->train);
        UnseatAllCastleVisitors((struct CastleState *)this);
    }
}

/* Puts the train in the station and starts it after param_2 ms. */
// FUNCTION: LEGOLAND 0x00424ae0
void StartCastleTrain(struct CastleState *param_1, unsigned int param_2) {
    ResetTrainToStation(param_1->train);
    StartTrain((struct Train *)param_1->train, param_2);
}

/* Empties the visitor queue. */
// FUNCTION: LEGOLAND 0x00424b10
void InitCastleVisitorList(struct CastleState *param_1) {
    struct CastleVisitor *addr = &param_1->visitors;
    param_1->visitors.next = addr;
    param_1->visitors.prev = addr;
}

/* First waiting visitor (flags 1), or NULL. */
// FUNCTION: LEGOLAND 0x00424b30
struct CastleVisitor *FindWaitingCastleVisitor(struct CastleState *list) {
    struct CastleVisitor *node = list->visitors.next;
    struct CastleVisitor *list_end = (struct CastleVisitor *)&list->visitors;
    while (node != list_end && node->flags != 1) {
        node = node->next;
    }
    return (node == list_end) ? 0 : node;
}

/* First seated visitor (flags 2), or NULL. */
// FUNCTION: LEGOLAND 0x00424b60
struct CastleVisitor *FindSeatedCastleVisitor(struct CastleState *list) {
    struct CastleVisitor *node = list->visitors.next;
    struct CastleVisitor *list_end = (struct CastleVisitor *)&list->visitors;
    while (node != list_end && node->flags != 2) {
        node = node->next;
    }
    return (node == list_end) ? 0 : node;
}

/* First finished visitor (flags 4), or NULL. */
// FUNCTION: LEGOLAND 0x00424b90
void *FindFinishedCastleVisitor(struct CastleState *list) {
    struct CastleVisitor *cur = list->visitors.next;
    struct CastleVisitor *end = (struct CastleVisitor *)&list->visitors;

    while (cur != end && cur->flags != 4) {
        cur = cur->next;
    }
    return (cur == end) ? 0 : cur;
}

/* Longest time (ms) any waiting visitor has waited. */
// FUNCTION: LEGOLAND 0x00424bc0
unsigned int GetLongestCastleVisitorWait(struct CastleState *ctx) {
    unsigned int result = 0;
    unsigned int t = GetGameTimer();
    struct CastleVisitor *cur = ctx->visitors.next;
    struct CastleVisitor *end = (struct CastleVisitor *)&ctx->visitors;

    if (cur != end) {
        do {
            if (cur->flags == 1) {
                unsigned int v = t - cur->time;
                if ((int)v > (int)result) {
                    result = v;
                }
            }
            cur = cur->next;
        } while (cur != end);
    }
    return result;
}

/* Number of waiting visitors. */
// FUNCTION: LEGOLAND 0x00424c10
unsigned int CountWaitingCastleVisitors(struct CastleState *ctx) {
    unsigned int result = 0;
    struct CastleVisitor *cur = ctx->visitors.next;
    struct CastleVisitor *end = (struct CastleVisitor *)&ctx->visitors;
    while (cur != end) {
        if (cur->flags == 1) {
            ++result;
        }
        cur = cur->next;
    }
    return result;
}

/* The train leaves when someone has waited over 5 s or more than 3 visitors wait. */
// FUNCTION: LEGOLAND 0x00424c40
int ShouldCastleTrainDepart(struct CastleState *arg) {
    int temp1 = GetLongestCastleVisitorWait(arg);
    int temp2 = CountWaitingCastleVisitors(arg);
    if (temp1 > 0x1388 || temp2 > 3) {
        return 1;
    }
    return 0;
}

/* With a closed ring and the train idle (or still held): seats waiting visitors until the train is full and starts
 * it after 3 s. Without a ring, visitors waiting over 5 s give up (flags 4). */
// FUNCTION: LEGOLAND 0x00424c70
void BoardCastleVisitors(struct CastleState *param_1) {
    struct CastleState *host = (struct CastleState *)param_1;
    struct CastleVisitor *node;
    struct CastleVisitor *cur;
    struct TrainSeat *ring;
    int found;
    int now = GetGameTimer();

    if (CastleCoaster.flags == 2) {
        if (IsTrainRunning(param_1->train) != 0) {
            if (IsTrainStartDelayed(param_1->train) == 0) {
                return;
            }
        }
        if (ShouldCastleTrainDepart(host) == 0) {
            return;
        }
        if (FindWaitingCastleVisitor(host) == 0) {
            return;
        }
        found = 0;
        while ((node = FindWaitingCastleVisitor(host)) != 0) {
            ring = FindEmptyTrainSeat(param_1->train);
            if (ring == 0) {
                break;
            }
            SeatCastleVisitor((struct CastleVisitor *)node, ring);
            node->flags = 2;
            found = 1;
        }
        if (found != 0) {
            StartCastleTrain(param_1, 0xbb8);
        }
        return;
    }
    for (cur = (struct CastleVisitor *)host->visitors.next; cur != (struct CastleVisitor *)&host->visitors; cur = cur->next) {
        if (now - (int)cur->time > 0x1388) {
            cur->flags = 4;
        }
    }
}

/* Unseats every rider; they are finished (flags 4). */
// FUNCTION: LEGOLAND 0x00424d80
void UnseatAllCastleVisitors(struct CastleState *list) {
    struct CastleVisitor *current = FindSeatedCastleVisitor(list);
    while (current != NULL) {
        UnseatCastleVisitor((struct CastleVisitor *)current);
        current->flags = 4;
        current = FindSeatedCastleVisitor(list);
    }
}

/* Sends every finished visitor's bloke on (action 33) and frees the visitor. */
// FUNCTION: LEGOLAND 0x00424dc0
void ReleaseFinishedCastleVisitors(struct CastleState *arg) {
    struct CastleVisitor *current = FindFinishedCastleVisitor(arg);
    if (current != NULL) {
        while (current != NULL) {
            struct Bloke *next = current->bloke;
            next->param_action = 33;
            FreeCastleVisitor((struct CastleVisitor *)current);
            current = FindFinishedCastleVisitor(arg);
        }
    }
}

/* Frees every visitor. */
// FUNCTION: LEGOLAND 0x00424df0
void FreeAllCastleVisitors(struct CastleState *esi) {
    struct CastleVisitor *current = esi->visitors.next;
    struct CastleVisitor *target = (struct CastleVisitor *)&esi->visitors;
    while (current != target) {
        FreeCastleVisitor((struct CastleVisitor *)current);
        current = esi->visitors.next;
    }
}

/* Takes every bloke off the castle ride. */
// FUNCTION: LEGOLAND 0x00424e20
void CastleRemoveAllRiders(void) {
    struct Ride *ride = GetCastleDispatchRide(0);
    struct RideNode *node = ride->riders;
    if (node != 0) {
        do {
            struct RideNode *next_node = node->next;
            node->rider->flags &= 0xfff7;
            RemoveBlokeFromRide(ride, node);
            node = next_node;
        } while (node != 0);
    }
}

/* Called when the track becomes a closed ring. Empty. */
// FUNCTION: LEGOLAND 0x00424e60
void OnCoasterRingClosed(struct CastleState *owner) {
}

/* Called when the closed ring is broken: stops the train. */
// FUNCTION: LEGOLAND 0x00424e70
void OnCoasterRingOpened(struct CastleState *owner) {
    StopCastleTrain((struct CastleState *)&CastleCoaster);
}

#pragma optimize("", off)
/* Debug profiling: stores this frame's counters (spans, triangles, cycles per renderer stage) in 64-entry history
 * rings and resets them. */
// FUNCTION: LEGOLAND 0x00424e80
void RecordCastleFrameCounters(void) {
    DepthSpanRecordHistory[CastleCounterFrame & 0x3f] = DepthSpanRecordCount;
    FilledTriangleHistory[CastleCounterFrame & 0x3f] = FilledTriangleCount;
    EdgeMeshDrawCycleHistory[CastleCounterFrame & 0x3f] = EdgeMeshDrawCycles;
    UnusedCastleCycleHistory[CastleCounterFrame & 0x3f] = UnusedCastleCycles;
    SupportPostCycleHistory[CastleCounterFrame & 0x3f] = SupportPostDrawCycles;
    LmsDrawCycleHistory[CastleCounterFrame & 0x3f] = LmsDrawCycles;
    TrainStepCycleHistory[CastleCounterFrame & 0x3f] = TrainStepCycles;
    DepthClearCycleHistory[CastleCounterFrame & 0x3f] = DepthClearCycles;
    BisectRootCallHistory[CastleCounterFrame & 0x3f] = BisectRootCallCount;
    BisectRootStepHistory[CastleCounterFrame & 0x3f] = BisectRootStepCount;
    WalkerRangeCallHistory[CastleCounterFrame & 0x3f] = WalkerRangeCallCount;
    TotalCastleCycleHistory[CastleCounterFrame & 0x3f] = UnusedCastleCycleHistory[CastleCounterFrame & 0x3f] + EdgeMeshDrawCycleHistory[CastleCounterFrame & 0x3f] + TrainStepCycleHistory[CastleCounterFrame & 0x3f] + SupportPostCycleHistory[CastleCounterFrame & 0x3f] + LmsDrawCycleHistory[CastleCounterFrame & 0x3f] + DepthClearCycleHistory[CastleCounterFrame & 0x3f];
    DepthSpanRecordCount = 0;
    FilledTriangleCount = 0;
    EdgeMeshDrawCycles = 0;
    UnusedCastleCycles = 0;
    SupportPostDrawCycles = 0;
    LmsDrawCycles = 0;
    TrainStepCycles = 0;
    DepthClearCycles = 0;
    BisectRootCallCount = 0;
    BisectRootStepCount = 0;
    WalkerRangeCallCount = 0;
}
#pragma optimize("", on)

#pragma optimize("", off)
/* Castle object render: draws its riders on this tile that are walking in or out, then the castle matte sprite. */
// FUNCTION: LEGOLAND 0x00425050
void RenderCastleObj(Element *obj, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int clip) {
    struct Ride *r;
    struct RideNode *walk;
    struct Bloke *rider;
    struct Point off;
    struct Point pos;

    r = obj->ride;
    RenderItems_New();
    CastleRiderRenderList = NULL;
    walk = r->riders;
    while (walk != NULL) {
        if (tile->id == walk->tile.id) {
            rider = walk->rider;
            if (rider->param_action < 0x10) {
                AddBlokeToRenderList(&CastleRiderRenderList, walk, walk->person->field_20);
            }
            if (rider->param_action >= 0x21) {
                AddBlokeToRenderList(&CastleRiderRenderList, walk, walk->person->field_20);
            }
        }
        walk = walk->next;
    }
    RenderBlokeList((struct RenderItemNode **)&CastleRiderRenderList);
    if (CastleMatteSprite != NULL) {
        pos = GetScreenCoordsForObject(tile, r);
        off = GetRenderOffsetForLayer(r->layer, 2);
        AdjustOffsetForViewMode(&off);
        PrintSprite(CastleMatteSprite, pos.x + off.x, pos.y + off.y, clip, 0);
    }
}
#pragma optimize("", on)

#pragma optimize("", off)
/* Per frame: profiling, view and depth buffer reset, then boarding, the train and releasing finished visitors. */
// FUNCTION: LEGOLAND 0x00425170
void CastleCoasterUpdate(Element *obj) {
    RecordCastleFrameCounters();
    UpdateCastleView();
    ClearCoasterDepth(0);
    if (CastleCoasterPlaced != 0) {
        BoardCastleVisitors((struct CastleState *)&CastleCoaster);
        UpdateCastleTrain((struct CastleState *)&CastleCoaster);
        ReleaseFinishedCastleVisitors((struct CastleState *)&CastleCoaster);
    }
}
#pragma optimize("", on)

#pragma optimize("", off)
/* Castle object pre-render: refreshes the free cells at both ends of the track, then moves each rider: walk to the
 * entrance (0, 1), queue as a visitor (0x10 -> 0x20, waiting for the ride), walk back out once released (0x21,
 * 0x22) and leave the ride (0x40). */
// FUNCTION: LEGOLAND 0x004251c0
void CastleObjUpdate(Element *obj) {
    /* /Od build: stack-slot order follows the (hashed) local names; these single-letter
     * names reproduce the original layout (a at ebp-4 ... i at ebp-0x24). */
    RideNode *a; /* current rider node */
    TileId *b; /* &a->tile */
    struct CastleState *c; /* CastleCoaster */
    Ride *d; /* the castle ride */
    unsigned int e; /* tile y */
    unsigned int f; /* tile x */
    Bloke *g; /* rider */
    RideNode *h; /* next rider node */
    struct CastleVisitor *i; /* timer node (unused) */

    d = obj->ride;
    a = d->riders;
    c = &CastleCoaster;
    if (c->tail != NULL) {
        FindPathSegDoorAPoints(c->tail, &c->next_spots);
    }
    if (c->head != NULL) {
        FindPathSegDoorBPoints(c->head, &c->prev_spots);
    }
    while (a != NULL) {
        h = a->next;
        b = &a->tile;
        f = b->pos.x + d->x;
        e = b->pos.y + d->y;
        g = a->rider;
        switch (g->param_action) {
        case 0:
            g->flags |= 8;
            if (g->low_level_action == 0) {
                g->dest.x = (f - 8) << 8;
                g->dest.y = (e - 1) << 8;
                g->target_dir = ((CalcMoveLineIntFn)CalcMoveLine)(g->pos, g->dest, &g->nav) + 0x10;
                g->low_level_action = 7;
                NewDirForAction(g, (g->target_dir >> 5) + 3);
                g->param_action++;
            }
            break;
        case 1:
            if (g->low_level_action == 0) {
                g->param_action = 0x10;
            }
            break;
        case 0x10:
            i = AddCastleVisitor(g, &CastleCoaster);
            g->param_action = 0x20;
            break;
        case 0x21:
            if (g->low_level_action == 0) {
                g->dest.x = (f << 8) + 0x80;
                g->dest.y = (e << 8) + 0x80;
                g->target_dir = ((CalcMoveLineIntFn)CalcMoveLine)(g->pos, g->dest, &g->nav) + 0x10;
                g->low_level_action = 7;
                NewDirForAction(g, (g->target_dir >> 5) + 3);
                g->param_action++;
            }
            break;
        case 0x22:
            if (g->low_level_action == 0) {
                g->param_action = 0x40;
            }
            break;
        case 0x40:
            g->flags &= 0xfff7;
            RemoveBlokeFromRide(d, a);
            break;
        }
        a = h;
    }
    CastleCoasterUpdate(obj);
}
#pragma optimize("", on)

#pragma optimize("", off)
/* Sets the ride callbacks of the castle's elements (castle, dummy, square track, the three height blocks, coaster
 * load/save) and fills their CastleDispatchTable rows. */
// FUNCTION: LEGOLAND 0x004254d0
void CastleGetInterfaces(struct Element *head, struct Ride *obj) {
    // STRING: LEGOLAND 0x004b5c0c
    if (_stricmp("CASTLE OBJ", head->name) == 0) {
        obj->cb_8c = CastleDispatchSetEditMode;
        obj->cb_90 = CastleDispatchCalcCursor;
        obj->cb_94 = CastleDispatchDCalcCursor;
        obj->cb_98 = CastleDispatchAddObject;
        obj->cb_9c = CastleDispatchRemoveObject;
        obj->cb_a4 = CastleObjLoad;
        obj->cb_ac = CastleObjUnload;
        obj->cb_pre_render = CastleObjUpdate;
        obj->cb_b0 = RenderCastleObj;
        obj->save_hook = CastleObj_Save;
        obj->load_hook = CastleObj_Load;
        obj->cb_c0 = (int (*)())CastleObjPieceCount;
        CastleDispatchTable[0].element = head;
        CastleDispatchTable[0].fn_4 = (void (*)())CastleObjSetEditMode;
        CastleDispatchTable[0].fn_8 = CastleObjCalcCursor;
        CastleDispatchTable[0].fn_10 = CastleObjDCalcCursor;
        CastleDispatchTable[0].fn_c = PlaceCastleCoaster;
        CastleDispatchTable[0].fn_14 = RemoveCastleCoaster;
        CastleDispatchTableEnd[2] = head;
    }
    // STRING: LEGOLAND 0x004b5b7c
    else if (_stricmp("CASTLE_DUMMY", head->name) == 0) {
        obj->cb_8c = CastleDispatchSetEditMode;
        obj->cb_90 = CastleDispatchCalcCursor;
        obj->cb_94 = CastleDispatchDCalcCursor;
        obj->cb_98 = CastleDispatchAddObject;
        obj->cb_9c = CastleDispatchRemoveObject;
        obj->cb_a4 = CastleDummyLoad;
        obj->cb_b0 = RenderCastleDummy;
        CastleDispatchTable[1].fn_10 = CastleDummyDCalcCursor;
        CastleDispatchTable[1].fn_14 = CastleDummyRemoveObject;
        CastleDispatchTableEnd[3] = head;
    }
    // STRING: LEGOLAND 0x004b5bfc
    else if (_stricmp("SQUARE_TRACK", head->name) == 0) {
        obj->cb_8c = CastleDispatchSetEditMode;
        obj->cb_90 = CastleDispatchCalcCursor;
        obj->cb_94 = CastleDispatchDCalcCursor;
        obj->cb_98 = CastleDispatchAddObject;
        obj->cb_9c = CastleDispatchRemoveObject;
        obj->cb_a4 = CastleLoadBasicTiles;
        obj->cb_ac = CastleUnloadBasicTiles;
        obj->cb_b0 = RenderSquareTrack;
        obj->cb_90 = SquareTrackCalcCursor;
        CastleDispatchTable[2].element = head;
        CastleDispatchTable[2].fn_4 = TrackPieceSetEditMode;
        CastleDispatchTable[2].fn_8 = SquareTrackPieceCalcCursor;
        CastleDispatchTable[2].fn_10 = CastleRemovePieceCalcCursor;
        CastleDispatchTable[2].fn_c = SquareTrackAddObject;
        CastleDispatchTable[2].fn_14 = RemoveQueriedCoasterPiece;
        CastleDispatchTableEnd[0] = head;
    }
    // STRING: LEGOLAND 0x004b5be8
    else if (_stricmp("SQUARE_TRACK_HEIGHT", head->name) == 0) {
        obj->cb_8c = CastleDispatchSetEditMode;
        obj->cb_90 = CastleDispatchCalcCursor;
        obj->cb_94 = CastleDispatchDCalcCursor;
        obj->cb_98 = CastleDispatchAddObject;
        obj->cb_9c = CastleDispatchRemoveObject;
        obj->cb_a4 = SquareTrackHeightLoad;
        obj->cb_b0 = RenderSquareTrack;
        CastleDispatchTable[3].element = head;
        CastleDispatchTable[3].fn_4 = TrackPieceSetEditMode;
        CastleDispatchTable[3].fn_8 = HeightTrackCalcCursor;
        CastleDispatchTable[3].fn_10 = CastleRemovePieceCalcCursor;
        CastleDispatchTable[3].fn_c = HeightTrackAddObject;
        CastleDispatchTable[3].fn_14 = RemoveQueriedCoasterPiece;
        CastleDispatchTableEnd[5] = head;
    }
    // STRING: LEGOLAND 0x004b5bd0
    else if (_stricmp("SQUARE_TRACK_HEIGHT_0", head->name) == 0) {
        obj->cb_8c = CastleDispatchSetEditMode;
        obj->cb_90 = CastleDispatchCalcCursor;
        obj->cb_94 = CastleDispatchDCalcCursor;
        obj->cb_98 = CastleDispatchAddObject;
        obj->cb_9c = CastleDispatchRemoveObject;
        obj->cb_a4 = SquareTrackHeight0Load;
        obj->cb_b0 = RenderSquareTrack;
        CastleDispatchTable[4].element = head;
        CastleDispatchTable[4].fn_4 = TrackPieceSetEditMode;
        CastleDispatchTable[4].fn_8 = HeightTrackCalcCursor;
        CastleDispatchTable[4].fn_10 = CastleRemovePieceCalcCursor;
        CastleDispatchTable[4].fn_c = HeightTrackAddObject;
        CastleDispatchTable[4].fn_14 = RemoveQueriedCoasterPiece;
        CastleDispatchTableEnd[4] = head;
    }
    // STRING: LEGOLAND 0x004b5bb4
    else if (_stricmp("SQUARE_TRACK_HEIGHT_PATH", head->name) == 0) {
        obj->cb_8c = CastleDispatchSetEditMode;
        obj->cb_90 = CastleDispatchCalcCursor;
        obj->cb_94 = CastleDispatchDCalcCursor;
        obj->cb_98 = CastleDispatchAddObject;
        obj->cb_9c = CastleDispatchRemoveObject;
        obj->cb_a4 = SquareTrackHeightPathLoad;
        obj->cb_b0 = RenderSquareTrack;
        CastleDispatchTable[5].element = head;
        CastleDispatchTable[5].fn_4 = TrackPieceSetEditMode;
        CastleDispatchTable[5].fn_8 = HeightTrackPathCalcCursor;
        CastleDispatchTable[5].fn_10 = CastleRemovePieceCalcCursor;
        CastleDispatchTable[5].fn_c = HeightTrackPathAddObject;
        CastleDispatchTable[5].fn_14 = RemoveQueriedCoasterPiece;
        CastleDispatchTableEnd[1] = head;
    }
    // STRING: LEGOLAND 0x004b5ba0
    else if (_stricmp("ROLLER_COASTER_LOAD", head->name) == 0) {
        obj->cb_8c = CastleDispatchSetEditMode;
        obj->cb_90 = CastleDispatchCalcCursor;
        obj->cb_94 = CastleDispatchDCalcCursor;
        obj->cb_98 = CastleDispatchAddObject;
        obj->cb_9c = CastleDispatchRemoveObject;
        obj->cb_8c = (void (*)())CastleObj_Load;
    }
    // STRING: LEGOLAND 0x004b5b8c
    else if (_stricmp("ROLLER_COASTER_SAVE", head->name) == 0) {
        obj->cb_8c = CastleDispatchSetEditMode;
        obj->cb_90 = CastleDispatchCalcCursor;
        obj->cb_94 = CastleDispatchDCalcCursor;
        obj->cb_98 = CastleDispatchAddObject;
        obj->cb_9c = CastleDispatchRemoveObject;
        obj->cb_8c = (void (*)())CastleObj_Save;
    }
}
#pragma optimize("", on)

/* Sets up the castle's 3D view: the isometric projections (full and half scale), the sqrt tables, the view
 * direction, the two light directions, then the track lighting and the dirty-rectangle list. */
// FUNCTION: LEGOLAND 0x00425a50
void InitCastleView(void) {
    CastleIsoProjections[0].m[0][0] = 1.60334f;
    CastleIsoProjections[0].m[0][1] = -1.60334f;
    CastleIsoProjections[0].m[0][2] = 0.0f;
    CastleIsoProjections[0].m[0][3] = 0.0f;
    CastleIsoProjections[0].m[1][0] = 0.801688f;
    CastleIsoProjections[0].m[1][1] = 0.801688f;
    CastleIsoProjections[0].m[1][2] = 1.96416f;
    CastleIsoProjections[0].m[1][3] = 0.0f;
    CastleIsoProjections[0].m[2][0] = 3.19995f;
    CastleIsoProjections[0].m[2][1] = 3.19995f;
    CastleIsoProjections[0].m[2][2] = 0.0f;
    CastleIsoProjections[0].m[2][3] = 32767.5f;
    CastleIsoProjections[0].m[3][0] = 0.0f;
    CastleIsoProjections[0].m[3][1] = 0.0f;
    CastleIsoProjections[0].m[3][2] = 0.0f;
    CastleIsoProjections[0].m[3][3] = 1.0f;
    CastleIsoProjections[1] = CastleIsoProjections[0];
    *(volatile float *)&CastleIsoProjections[1].m[0][0] *= 0.5f;
    CastleIsoProjections[1].m[0][1] *= 0.5f;
    CastleIsoProjections[1].m[1][0] *= 0.5f;
    CastleIsoProjections[1].m[1][1] *= 0.5f;
    CastleIsoProjections[1].m[1][2] *= 0.5f;
    lego_sqrtf_init();
    lego_rsqrtf_init();
    CastleViewDir[0] = 1.0f;
    CastleViewDir[1] = 1.0f;
    CastleViewDir[2] = CastleIsoProjections[0].m[1][0] * -2.0f / CastleIsoProjections[0].m[1][2];
    Vec3Normalize(CastleViewDir);
    Vec3Normalize((float *)&TrackLightDir);
    Vec3Normalize((float *)&ModelLightDir);
    SetTrackLighting();
    InitCastleLinkList();
}

/* Selects the track lighting: CastleLightDir = TrackLightDir with its shade range. */
// FUNCTION: LEGOLAND 0x00425bd0
void SetTrackLighting(void) {
    float f;
    unsigned int i1;
    unsigned int i2;
    unsigned int i3;

    f = TrackLightingBase;
    f = f + TrackLightingOffset;
    f = f * 0.5f;

    i1 = TrackLightDir;
    i2 = TrackLightDirY;
    i3 = TrackLightDirZ;
    CastleLightDir = i1;

    memcpy(&i1, &TrackLightingOffset, sizeof(i1));
    CastleLightDirY = i2;
    memcpy(&i2, &TrackLightingBase, sizeof(i2));
    CastleLightDirZ = i3;

    CastleShadeMin = i1;
    CastleShadeMax = i2;
    CastleLightBias = f;

    f = TrackLightingBase;
    f = f - TrackLightingOffset;
    f = f * 0.5f;
    CastleLightScale = f;
}

/* Selects the model lighting (train, visitors): CastleLightDir = ModelLightDir with its shade range. */
// FUNCTION: LEGOLAND 0x00425c40
void SetModelLighting(void) {
    float f;
    unsigned int i1;
    unsigned int i2;
    unsigned int i3;

    f = ModelLightingBase;
    f = f + ModelLightingOffset;
    f = f * 0.5f;

    i1 = ModelLightDir;
    i2 = ModelLightDirY;
    i3 = ModelLightDirZ;
    CastleLightDir = i1;

    memcpy(&i1, &ModelLightingOffset, sizeof(i1));
    CastleLightDirY = i2;
    memcpy(&i2, &ModelLightingBase, sizeof(i2));
    CastleLightDirZ = i3;

    if (f == f) {
        CastleShadeMin = i1;
        CastleShadeMax = i2;
        CastleLightBias = f;
    }

    f = ModelLightingBase;
    f = f - ModelLightingOffset;
    f = f * 0.5f;
    CastleLightScale = f;
}

/* Map cell (x, y) and height f to castle world coordinates. */
// FUNCTION: LEGOLAND 0x00425cb0
void MapPosToWorld(const struct Int16Pair *in, float f, struct FVec3 *out) {
    short temp;
    float temp_f;
    temp = in->x;
    out->x = (float)temp * FLOAT_004ab45c;
    temp = in->y;
    out->y = (float)temp * FLOAT_004ab45c;
    temp_f = f;
    out->z = temp_f * FLOAT_004ab458;
}

/* result = a x b. */
// FUNCTION: LEGOLAND 0x00425cf0
void Vec3Cross(const struct FVec3 *a, const struct FVec3 *b, struct FVec3 *result) {
    result->x = b->z * a->y - a->z * b->y;
    result->y = a->z * b->x - a->x * b->z;
    result->z = a->x * b->y - a->y * b->x;
}

/* a . b. */
// FUNCTION: LEGOLAND 0x00425d30
float Vec3Dot(struct FVec3 *param_1, struct FVec3 *param_2) {
    return param_1->z * param_2->z + param_1->y * param_2->y + param_1->x * param_2->x;
}

/* Scales v to unit length (lego_rsqrtf). */
// FUNCTION: LEGOLAND 0x00425d50
void Vec3Normalize(float *v) {
    float sum = FLOAT_004ab390;
    float s;
    int i;

    for (i = 0; i < 3; i++) {
        sum += v[i] * v[i];
    }
    s = lego_rsqrtf(sum);
    for (i = 0; i < 3; i++) {
        v[i] *= s;
    }
}

/* 1 if the two vectors are equal. */
// FUNCTION: LEGOLAND 0x00425da0
unsigned int Vec3Equal(const float *a, const float *b) {
    if (a[0] != b[0]) {
        return 0;
    }
    if (a[1] != b[1]) {
        return 0;
    }
    if (a[2] != b[2]) {
        return 0;
    }
    return 1;
}

/* Inverts a 2x2 matrix in place. */
// FUNCTION: LEGOLAND 0x00425de0
void Mat2Invert(float *a) {
    float inv = 1.0f / (a[0] * a[3] - a[1] * a[2]);
    float t = a[0];

    a[0] = inv * a[3];
    a[3] = t * inv;
    a[1] = -(inv * a[1]);
    a[2] = -(inv * a[2]);
}

/* Per frame: puts the camera on the world point under the centre of the game view (inverting the projection's 2x2
 * part) and rebuilds CastleWorldToScreen and the clip rectangle. */
// FUNCTION: LEGOLAND 0x00425e20
void UpdateCastleView(void) {
    struct FMat4 m;
    float a[4];
    struct Point pt;
    struct Point ref;
    int bounds[4];

    int mid;
    float dx;
    float fx, fy, fm, fb;
    float dy;

    CastleViewMatrix = CastleIsoProjections[1];
    a[0] = CastleViewMatrix.m[0][0];
    a[1] = CastleViewMatrix.m[0][1];
    a[2] = CastleViewMatrix.m[1][0];
    a[3] = CastleViewMatrix.m[1][1];
    Mat2Invert(a);
    pt.x = (lpConfig->view_width >> 1) + lpConfig->view_x;
    pt.y = (lpConfig->view_height >> 1) + lpConfig->view_y;
    CastleViewMatrix.m[0][3] = (float)pt.x;
    CastleViewMatrix.m[1][3] = (float)pt.y;
    ScreenToMapRef(&pt.x, &ref.x, 0);
    GetTileBounds(&ref, bounds);
    CastleCameraPos[2] = 0.0f;
    mid = (bounds[2] + bounds[0]) >> 1;
    fx = (float)pt.x;
    fm = (float)mid;
    dx = fx - fm;
    fy = (float)pt.y;
    fb = (float)bounds[1];
    dy = fy - fb;
    CastleCameraPos[0] = dy * a[1] + dx * a[0];
    CastleCameraPos[1] = dy * a[3] + dx * a[2];
    CastleCameraPos[0] += (float)ref.x * FLOAT_004ab45c;
    CastleCameraPos[1] += (float)ref.y * FLOAT_004ab45c;
    CastleViewRect[0] = lpConfig->view_x;
    CastleViewRect[1] = lpConfig->view_y;
    CastleViewRect[2] = lpConfig->view_width + lpConfig->view_x;
    CastleViewRect[3] = lpConfig->view_height + lpConfig->view_y;
    Mat4Identity(m.m);
    m.m[0][3] = -CastleCameraPos[0];
    m.m[1][3] = -CastleCameraPos[1];
    m.m[2][3] = -CastleCameraPos[2];
    Mat4Multiply(CastleViewMatrix.m, m.m, CastleWorldToScreen.m);
    SetClipRect(CastleViewRect[0], CastleViewRect[1], CastleViewRect[2], CastleViewRect[3]);
}

/* Like UpdateCastleView but with the camera at the origin; flag selects the half-scale projection. Not called. */
// FUNCTION: LEGOLAND 0x00426000
void SetCastleViewAtOrigin(int flag) {
    struct FMat4 m;

    if (flag) {
        CastleViewMatrix = CastleIsoProjections[1];
    } else {
        CastleViewMatrix = CastleIsoProjections[0];
    }
    CastleViewMatrix.m[0][3] = 0.0f;
    CastleViewMatrix.m[1][3] = 0.0f;
    CastleViewRect[0] = lpConfig->view_x;
    CastleViewRect[1] = lpConfig->view_y;
    CastleViewRect[2] = lpConfig->view_width + lpConfig->view_x;
    CastleViewRect[3] = lpConfig->view_height + lpConfig->view_y;
    CastleCameraPos[0] = 0.0f;
    CastleCameraPos[1] = 0.0f;
    CastleCameraPos[2] = 0.0f;
    Mat4Identity(m.m);
    m.m[0][3] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][3] = 0.0f;
    Mat4Multiply(CastleViewMatrix.m, m.m, CastleWorldToScreen.m);
}

/* Screen y per unit of height in the current projection. Not called. */
// FUNCTION: LEGOLAND 0x004260e0
float GetCastleViewHeightScale(void) {
    return CastleViewMatrix.m[1][2];
}

/* m = identity. */
// FUNCTION: LEGOLAND 0x004260f0
void Mat4Identity(float m[4][4]) {
    int i;
    int j;

    for (i = 0; i <= 3; i++) {
        for (j = 0; j <= 3; j++) {
            m[i][j] = 0.0f;
            if (i == j) {
                m[i][j] = 1.0f;
            }
        }
    }
}

/* out = a * b (4x4). */
// FUNCTION: LEGOLAND 0x00426120
void Mat4Multiply(float a[4][4], float b[4][4], float out[4][4]) {
    int i;
    int j;
    int k;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            float sum = FLOAT_004ab390;
            for (k = 0; k < 4; k++) {
                sum += a[i][k] * b[k][j];
            }
            out[i][j] = sum;
        }
    }
}

struct Mat4x4 {
    unsigned int m[4][4];
};

/* param_1 = transpose of param_2. */
// FUNCTION: LEGOLAND 0x00426190
void Mat4Transpose(struct Mat4x4 *param_2, struct Mat4x4 *param_1) {
    int i;
    int j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            param_1->m[i][j] = param_2->m[j][i];
        }
    }
}

/* Transforms n points by the 3x4 part of m. */
// FUNCTION: LEGOLAND 0x004261c0
void TransformPoints3x4(float *in, float *out, float m[4][4], int n) {
    int j;
    int k;

    while (n-- > 0) {
        for (j = 0; j < 3; j++) {
            float sum = FLOAT_004ab390;
            for (k = 0; k < 3; k++) {
                sum += in[k] * m[j][k];
            }
            out[j] = sum + m[j][3];
        }
        out += 3;
        in += 3;
    }
}

/* ProjectPoints with CastleWorldToScreen. */
// FUNCTION: LEGOLAND 0x00426230
unsigned int ProjectCastlePoints(float in[][3], int out[][4], int n) {
    return ProjectPoints(in, out, &CastleWorldToScreen.m[0][0], 0x10, n);
}

/* Projects n points with matrix m to integer screen coordinates (16.16) and depth, with their clip code: bits 1, 2,
 * 4, 8 = inside the left, right, top, bottom planes, 0xf0 = depth in range. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00426250
__declspec(naked) unsigned int ProjectPoints(float in[][3], int out[][4], float *m, unsigned int stride, int n) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        mov eax, dword ptr [ebp + 0x18]
        mov ecx, eax
        dec eax
        test ecx, ecx
        jle L42639c
        push ebx
        mov ebx, dword ptr [ebp + 0xc]
        inc eax
        push esi
        push edi
        mov dword ptr [ebp - 8], eax
L42626e:
        mov edx, dword ptr [CastleLinkList + 0x18]
        mov edi, ebx
        mov dword ptr [ebp + 0xc], edx
        mov edx, dword ptr [ebp + 0x10]
        mov dword ptr [ebp - 4], 3
L426283:
        fld dword ptr [FLOAT_004ab390]
        mov ecx, dword ptr [ebp + 8]
        mov eax, edx
        mov esi, 3
L426293:
        fld dword ptr [ecx]
        fmul dword ptr [eax]
        add eax, 4
        add ecx, 4
        dec esi
        faddp st(1), st
        jne L426293
        fadd dword ptr [edx + 0xc]
        fstp dword ptr [ebp + 0x18]
        fld dword ptr [ebp + 0x18]
        fistp dword ptr [ebp + 0x18]
        mov eax, dword ptr [ebp + 0x18]
        add edx, 0x10
        mov dword ptr [edi], eax
        mov eax, dword ptr [ebp - 4]
        add edi, 4
        dec eax
        mov dword ptr [ebp - 4], eax
        jne L426283
        mov ecx, dword ptr [ebx]
        mov dword ptr [ebx + 0xc], 0
        cmp ecx, dword ptr [CastleViewRect]
        jge L4262da
        mov eax, dword ptr [ebx + 0xc]
        or al, 2
        jmp L4262ee
L4262da:
        mov edx, dword ptr [ebx]
        mov eax, dword ptr [CastleViewRect + 0x8]
        cmp edx, eax
        mov eax, dword ptr [ebx + 0xc]
        jg L4262ec
        or al, 3
        jmp L4262ee
L4262ec:
        or al, 1
L4262ee:
        mov dword ptr [ebx + 0xc], eax
        mov eax, dword ptr [ebx + 4]
        cmp eax, dword ptr [CastleViewRect + 0x4]
        jge L426303
        mov eax, dword ptr [ebx + 0xc]
        or al, 8
        jmp L426316
L426303:
        mov ecx, dword ptr [CastleViewRect + 0xc]
        cmp eax, ecx
        mov eax, dword ptr [ebx + 0xc]
        jg L426314
        or al, 0xc
        jmp L426316
L426314:
        or al, 4
L426316:
        mov ecx, dword ptr [ebp + 0xc]
        mov dword ptr [ebx + 0xc], eax
        cmp ecx, offset CastleLinkList
        je L42637e
        mov esi, dword ptr [ebx]
        mov edi, dword ptr [ebx + 4]
        jmp L42632e
L42632b:
        mov ecx, dword ptr [ebp + 0xc]
L42632e:
        cmp esi, dword ptr [ecx]
        jge L426339
        mov eax, 2
        jmp L426348
L426339:
        mov edx, dword ptr [ecx + 8]
        xor eax, eax
        cmp esi, edx
        setg al
        dec eax
        and eax, 2
        inc eax
L426348:
        cmp edi, dword ptr [ecx + 4]
        jge L426351
        or al, 8
        jmp L42635c
L426351:
        cmp edi, dword ptr [ecx + 0xc]
        jg L42635a
        or al, 0xc
        jmp L42635c
L42635a:
        or al, 4
L42635c:
        mov edx, dword ptr [ecx + 0x10]
        and edx, eax
        cmp edx, 0xf
        je L426376
        mov ecx, dword ptr [ecx + 0x18]
        cmp ecx, offset CastleLinkList
        mov dword ptr [ebp + 0xc], ecx
        jne L42632b
        jmp L42637e
L426376:
        mov eax, dword ptr [ebx + 0xc]
        or al, 0xf0
        mov dword ptr [ebx + 0xc], eax
L42637e:
        mov edx, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp + 0x14]
        mov eax, dword ptr [ebp - 8]
        add edx, 0xc
        add ebx, ecx
        dec eax
        mov dword ptr [ebp + 8], edx
        mov dword ptr [ebp - 8], eax
        jne L42626e
        pop edi
        pop esi
        pop ebx
L42639c:
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Screen rectangle of n points projected with m. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x004263a0
__declspec(naked) void GetPointsScreenRect(void *pts, unsigned int m[4][4], int n, struct RectI *out) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x14
        mov ecx, dword ptr [ebp + 0x14]
        mov eax, 0x80000000
        mov dword ptr [ecx + 8], eax
        mov dword ptr [ecx + 0xc], eax
        mov eax, dword ptr [ebp + 0x10]
        mov dword ptr [ecx], 0x7fffffff
        mov dword ptr [ecx + 4], 0x7fffffff
        mov ecx, eax
        dec eax
        test ecx, ecx
        jle L426458
        push ebx
        inc eax
        push esi
        push edi
        mov dword ptr [ebp - 4], eax
L4263d6:
        mov edx, dword ptr [ebp + 0xc]
        lea edi, [ebp - 0x14]
        mov ebx, 2
L4263e1:
        fld dword ptr [FLOAT_004ab390]
        mov ecx, dword ptr [ebp + 8]
        mov eax, edx
        mov esi, 3
L4263f1:
        fld dword ptr [ecx]
        fmul dword ptr [eax]
        add eax, 4
        add ecx, 4
        dec esi
        faddp st(1), st
        jne L4263f1
        fadd dword ptr [edx + 0xc]
        fstp dword ptr [ebp + 0x10]
        fld dword ptr [ebp + 0x10]
        fistp dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebp + 0x10]
        add edx, 0x10
        mov dword ptr [edi], eax
        add edi, 4
        dec ebx
        jne L4263e1
        mov ecx, dword ptr [ebp + 0x14]
        mov eax, dword ptr [ebp - 0x14]
        cmp eax, dword ptr [ecx + 8]
        jle L426428
        mov dword ptr [ecx + 8], eax
L426428:
        cmp eax, dword ptr [ecx]
        jge L42642e
        mov dword ptr [ecx], eax
L42642e:
        mov eax, dword ptr [ebp - 0x10]
        mov edx, dword ptr [ecx + 0xc]
        cmp eax, edx
        jle L42643b
        mov dword ptr [ecx + 0xc], eax
L42643b:
        cmp eax, dword ptr [ecx + 4]
        jge L426443
        mov dword ptr [ecx + 4], eax
L426443:
        mov ecx, dword ptr [ebp + 8]
        mov eax, dword ptr [ebp - 4]
        add ecx, 0xc
        dec eax
        mov dword ptr [ebp + 8], ecx
        mov dword ptr [ebp - 4], eax
        jne L4263d6
        pop edi
        pop esi
        pop ebx
L426458:
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Transposed 3x3 part of a 4x4 matrix. */
// FUNCTION: LEGOLAND 0x00426460
void Mat4ToMat3Transpose(unsigned int *dst, void *src) {
    unsigned int i;
    unsigned int j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            dst[j] = *(unsigned int *)((unsigned char *)src + i * 4 + j * 16);
        }
        dst += 3;
    }
}

/* 4x4 matrix from a transposed 3x3 rotation (no translation). */
// FUNCTION: LEGOLAND 0x00426490
void Mat3ToMat4Transposed(unsigned int *src, unsigned int dst[4][4]) {
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            dst[j][i] = src[i * 3 + j];
        }
        dst[3][i] = 0;
    }
    dst[2][3] = 0;
    dst[1][3] = 0;
    dst[0][3] = 0;
    dst[3][3] = 0x3f800000;
}

/* 4x4 matrix from a 3x3 rotation and a translation t. */
// FUNCTION: LEGOLAND 0x004264e0
void MakeMat4FromRotTrans(unsigned int *t, unsigned int *m3, unsigned int dst[4][4]) {
    Mat3ToMat4Transposed(m3, dst);
    dst[0][3] = t[0];
    dst[1][3] = t[1];
    dst[2][3] = t[2];
}

/* Mat3ToMat4Transposed, transposed back. */
// FUNCTION: LEGOLAND 0x00426510
void Mat3ToMat4Transpose(unsigned int *m3, struct Mat4x4 *out) {
    float t[3];
    unsigned int dst[4][4];
    t[0] = 0.0f;
    t[1] = 0.0f;
    t[2] = 0.0f;
    MakeMat4FromRotTrans((unsigned int *)t, m3, dst);
    Mat4Transpose((struct Mat4x4 *)dst, out);
}

/* Orthonormal basis with basis[0] along dir and basis[2] as close to up as possible. */
// FUNCTION: LEGOLAND 0x00426560
void BuildBasisFromDir(struct FVec3 *dir, struct FVec3 *basis) {
    int i;

    basis[2].x = 0.0f;
    basis[2].y = 0.0f;
    basis[2].z = 1.0f;
    Vec3Cross(&basis[2], dir, &basis[1]);
    Vec3Cross(dir, &basis[1], &basis[2]);
    basis[0] = *dir;
    for (i = 0; i < 3; i++) {
        Vec3Normalize(&basis[i].x);
    }
}

/* Clips rectangle a to b; 0 if they do not overlap, else 0xf. */
// FUNCTION: LEGOLAND 0x004265d0
int ClipRectIToRect(struct RectI *a, struct RectI *b) {
    if (a->var_0 > b->var_8) {
        return 0;
    }
    if (a->var_8 < b->var_0) {
        return 0;
    }
    if (a->var_4 > b->var_c) {
        return 0;
    }
    if (a->var_c < b->var_4) {
        return 0;
    }
    if (a->var_0 < b->var_0) {
        a->var_0 = b->var_0;
    }
    if (a->var_8 > b->var_8) {
        a->var_8 = b->var_8;
    }
    if (a->var_4 < b->var_4) {
        a->var_4 = b->var_4;
    }
    if (a->var_c > b->var_c) {
        a->var_c = b->var_c;
    }
    return 0xF;
}

/* 1 if any rectangle on the castle's dirty list is on screen. */
// FUNCTION: LEGOLAND 0x00426650
unsigned int AnyCastleLinkVisible(void) {
    struct ListLink *n = (struct ListLink *)CastleLinkList.next;

    while (n != (struct ListLink *)&CastleLinkList) {
        if (n->visible == 0xf) {
            return 1;
        }
        n = n->next;
    }
    return 0;
}

/* Adds a rectangle to the castle's dirty list. */
// FUNCTION: LEGOLAND 0x004266b0
void AddCastleLink(struct ListLink *param_1) {
    CastleLinkList.next->prev = param_1;
    param_1->next = CastleLinkList.next;
    CastleLinkList.next = param_1;
    param_1->prev = &CastleLinkList;
}

/* Removes a rectangle from the castle's dirty list. */
// FUNCTION: LEGOLAND 0x004266e0
void UnlinkListLink(struct ListLink *param_1) {
    struct ListLink *temp_ptr_1;
    struct ListLink *temp_ptr_2;

    temp_ptr_1 = param_1->next;
    temp_ptr_2 = param_1->prev;
    temp_ptr_1->prev = temp_ptr_2;

    temp_ptr_1 = param_1->prev;
    temp_ptr_2 = param_1->next;
    temp_ptr_1->next = temp_ptr_2;
}

/* Stores src clipped to the castle view in the list entry, with its visibility. */
// FUNCTION: LEGOLAND 0x00426700
void ClipRectIToCastleView(struct RectI *dst, struct RectI *src) {
    *dst = *src;
    ((struct ListLink *)dst)->visible = ClipRectIToRect(dst, (struct RectI *)&CastleViewRect);
}

/* Empties the castle's dirty list. */
// FUNCTION: LEGOLAND 0x00426740
void InitCastleLinkList(void) {
    CastleLinkList.next = &CastleLinkList;
    CastleLinkList.prev = &CastleLinkList;
}

/* Screen rectangle of an 8-corner box at position a with rotation b. */
// FUNCTION: LEGOLAND 0x00426750
unsigned int GetBoxScreenRect(void *ptr, unsigned int *a, unsigned int *b, struct RectI *c) {
    unsigned int m[4][4];
    unsigned int m2[4][4];

    MakeMat4FromRotTrans(a, b, m);
    Mat4Multiply(CastleWorldToScreen.m, (float (*)[4])m, (float (*)[4])m2);
    GetPointsScreenRect(ptr, m2, 8, c);
}

/* Draws an edge object (a debug wire box) as dotted lines. */
// FUNCTION: LEGOLAND 0x004267b0
void DrawEdgeObj(unsigned int *t, unsigned int *m3, struct EdgeObj *obj) {
    unsigned int m[4][4];
    int xf[8][4];
    float pts[8][3];
    int i;

    MakeMat4FromRotTrans(t, m3, m);
    TransformPoints3x4(&obj->pts[0][0], &pts[0][0], (float (*)[4])m, obj->npts);
    ProjectCastlePoints(pts, xf, obj->npts);
    for (i = 0; i < obj->nedges; i++) {
        DrawDottedLine(xf[obj->edges[i].a], xf[obj->edges[i].b], obj->color);
    }
}

/* Makes a wire box +-x by +-y, 0 to -z (8 corners, 12 edges). */
// FUNCTION: LEGOLAND 0x00426850
void InitEdgeObjBox(float x, float y, float z, struct EdgeObj *obj) {
    int i;
    obj->npts = 8;
    obj->nedges = 12;
    obj->pts[0][0] = x;
    obj->pts[0][1] = -y;
    obj->pts[0][2] = 0.0f;
    obj->pts[1][0] = x;
    obj->pts[1][1] = y;
    obj->pts[1][2] = 0.0f;
    obj->pts[2][0] = -x;
    obj->pts[2][1] = y;
    obj->pts[3][0] = -x;
    obj->pts[3][1] = -y;
    obj->pts[3][2] = 0.0f;
    for (i = 0; i < 4; i++) {
        *(struct FVec3 *)obj->pts[i + 4] = *(struct FVec3 *)obj->pts[i];
        obj->pts[i + 4][2] = -z;
    }
    obj->edges[0].a = 0;
    obj->edges[0].b = 1;
    obj->edges[1].a = 1;
    obj->edges[1].b = 2;
    obj->edges[2].a = 2;
    obj->edges[2].b = 3;
    obj->edges[3].a = 3;
    obj->edges[3].b = 0;
    for (i = 0; i < 4; i++) {
        obj->edges[i + 4].a = obj->edges[i].a + 4;
        obj->edges[i + 4].b = obj->edges[i].b + 4;
    }
    obj->edges[8].a = 0;
    obj->edges[8].b = 4;
    obj->edges[9].a = 1;
    obj->edges[9].b = 5;
    obj->edges[10].a = 1;
    obj->edges[10].b = 5;
    obj->edges[11].a = 3;
    obj->edges[11].b = 7;
}

/* Fast 1 / sqrt(x) (through lego_rsqrtf_impl, HASM_lego_rsqrtf). */
// FUNCTION: LEGOLAND 0x00426960
float lego_rsqrtf(float x) {
    __asm {
        fld x
        call dword ptr [lego_rsqrtf_impl]
        fstp x
    }
    return x;
}

/* Table-driven 1 / sqrt(x): one linear step on the mantissa (rsqrt mantissa table) times the exponent's 1 /
 * sqrt(2^e) (rsqrtf_exp_table). Takes and returns ST(0). */
// FUNCTION: LEGOLAND 0x00426980
__declspec(naked) void HASM_lego_rsqrtf(void) {
    __asm {
        fstp dword ptr [esp - 10h]
        mov dword ptr [esp - 8], ebx
        mov dword ptr [esp - 0Ch], edx
        mov edx, dword ptr [esp - 10h]
        mov ebx, 007FFFFFh
        mov dword ptr [esp - 4], eax
        mov eax, edx
        shr eax, 11h
        and ebx, edx
        and eax, 3Fh
        or  ebx, 3F800000h
        mov dword ptr [esp - 10h], ebx
        mov ebx, dword ptr [esp - 8]
        fld dword ptr [esp - 10h]
        fmul dword ptr [eax*8 + rsqrtf_mantissa_table + 4]
        shr edx, 17h
        fadd dword ptr [eax*8 + rsqrtf_mantissa_table]
        mov eax, dword ptr [esp - 4]
        fmul dword ptr [edx*4 + rsqrtf_exp_table]
        mov edx, dword ptr [esp - 0Ch]
        ret
    }
}

/* Builds the 1 / sqrt tables and installs HASM_lego_rsqrtf. */
// FUNCTION: LEGOLAND 0x004269e0
void lego_rsqrtf_init(void) {
    int i;
    float x;
    struct {
        float a;
        float b;
    } *t = (void *)&CastleRiderRenderList;

    for (i = 0; i < 64; i++) {
        if (i == 29) {
            i = 29;
        }
        x = (float)sqrt((float)i * 0.015625f + 1.0f);
        t[i + 1].a = FLOAT_004ab43c / (x + x);
        t[i + 1].b = FLOAT_004ab468 / (x * x * x * 2.0f);
    }
    rsqrtf_exp_table[0] = 1.0f;
    for (i = 1; i <= 255; i++) {
        rsqrtf_exp_table[i] = 1.0f / (float)sqrt(pow(2.0, i - 127));
    }
    __asm {
        push eax
        lea eax, HASM_lego_rsqrtf
        mov lego_rsqrtf_impl, eax
        pop eax
    }
}

/* Fast sqrt(x) (through lego_sqrtf_impl, HASM_lego_sqrtf). */
// FUNCTION: LEGOLAND 0x00426a90
float lego_sqrtf(float x) {
    __asm {
        fld x
        call dword ptr [lego_sqrtf_impl]
        fstp x
    }
    return x;
}

/* Table-driven sqrt(x), like HASM_lego_rsqrtf. Takes and returns ST(0). */
// FUNCTION: LEGOLAND 0x00426ab0
__declspec(naked) void HASM_lego_sqrtf(void) {
    __asm {
        fstp dword ptr [esp - 10h]
        mov dword ptr [esp - 8], ebx
        mov dword ptr [esp - 0Ch], edx
        mov edx, dword ptr [esp - 10h]
        mov ebx, 007FFFFFh
        mov dword ptr [esp - 4], eax
        mov eax, edx
        shr eax, 11h
        and ebx, edx
        and eax, 3Fh
        or  ebx, 3F800000h
        mov dword ptr [esp - 10h], ebx
        mov ebx, dword ptr [esp - 8]
        fld dword ptr [esp - 10h]
        fmul dword ptr [eax*8 + sqrtf_mantissa_table + 4]
        shr edx, 17h
        fadd dword ptr [eax*8 + sqrtf_mantissa_table]
        mov eax, dword ptr [esp - 4]
        fmul dword ptr [edx*4 + sqrtf_exp_table]
        mov edx, dword ptr [esp - 0Ch]
        ret
    }
}

/* Lookup tables of the inverse square root (lego_sqrtf): 64 (x/2, 1/(2x)) mantissa pairs, then one
 * factor per float exponent. The exponent table starts 0x204 bytes after the mantissa table (one unused
 * float between them): lego_sqrtf_init's loop end pointer is exactly sqrtf_exp_table. Defined here,
 * initialised, so the two stay in this order and adjacent as in the original. */
// GLOBAL: LEGOLAND 0x00610c40
float sqrtf_mantissa_table[129] = {0};

// GLOBAL: LEGOLAND 0x00610e44
float sqrtf_exp_table[256] = {0};

/* Builds the sqrt tables and installs HASM_lego_sqrtf. */
// FUNCTION: LEGOLAND 0x00426b10
void lego_sqrtf_init(void) {
    int i;
    int j;
    float x;

    for (i = 0; i < 64; i++) {
        x = (float)sqrt((float)i * 0.015625f + 1.0f);
        sqrtf_mantissa_table[i * 2] = x * 0.5f;
        sqrtf_mantissa_table[i * 2 + 1] = 0.5f / x;
    }
    for (j = 0; j <= 255; j++) {
        sqrtf_exp_table[j] = (float)sqrt(pow(2.0, j - 127));
    }
    __asm {
        push eax
        lea eax, HASM_lego_sqrtf
        mov lego_sqrtf_impl, eax
        pop eax
    }
}

/* Adds base to a saved offset unless it is 0 (pointer fix-up after loading). */
// FUNCTION: LEGOLAND 0x00426ba0
void RelocatePointer(UINT_PTR *param_1, UINT_PTR param_2) {
    if (*param_1 != 0) {
        *param_1 = *param_1 + param_2;
    }
}

/* Subtracts base from a pointer unless it is NULL (offset for saving). */
// FUNCTION: LEGOLAND 0x00426bc0
void UnrelocatePointer(UINT_PTR *param_1, UINT_PTR param_2) {
    UINT_PTR var_0 = *param_1;
    if (var_0 != 0) {
        *param_1 = var_0 - param_2;
    }
}

struct TrackEntry {
    unsigned int var_0;
    short var_4;
    short var_6;
};

struct TrackList {
    unsigned int var_0;
    unsigned int var_4;
    unsigned int var_8;
    struct TrackEntry *var_c;
};

/* Debug: prints a track list. */
// FUNCTION: LEGOLAND 0x00426be0
void PrintTrackList(struct TrackList *edi) {
    int i;
    for (i = 0; i < (int)edi->var_8; i += 1) {
        struct TrackEntry *ptr = edi->var_c + i;
        // STRING: LEGOLAND 0x004b5cd0
        DBPrintf("Track %2x, Type %2x at (%2x, %2x)\n", i, ptr->var_0, ptr->var_4, ptr->var_6);
    }
}

struct CastleSaveInfo {
    struct CastleState *castle;
    unsigned int size;
    unsigned int piece_count;
    unsigned int train_running;
    unsigned int visitor_count;
};

struct PieceSaveRef {
    unsigned int kind; /* CastleDispatchTable index of the piece element */
    unsigned int xy; /* its map cell, x and y packed */
};

struct TrainSaveRecord {
    unsigned int flags;
    float pos;
    float energy;
    unsigned int since_step; /* ms since the last step */
    unsigned int hold_left; /* ms left of the start hold */
    unsigned int field_14; /* field_14..field_1c: the saved walker (SaveCurveWalker) */
    unsigned int field_18;
    unsigned int field_1c;
};

struct CastleSaveRecord {
    unsigned int size;
    unsigned int field_4;
    int piece_count;
    struct PieceSaveRef *pieces;
    unsigned int train_count;
    struct TrainSaveRecord *train;
    unsigned int visitor_count;
    struct VisitorLoadRecord *visitors;
};

/* Loads the castle from the save game: re-places every piece through its dispatch row (map objects off), then the
 * train and the visitors. */
// FUNCTION: LEGOLAND 0x00426c20
int CastleObj_Load(struct Element *element) {
    struct CastleSaveRecord *rec = ReadSaveRec();
    void *obj;
    int i;

    if (rec == 0) {
        return 0;
    }
    if (rec == (struct CastleSaveRecord *)-1) {
        return 1;
    }
    RelocatePointer((UINT_PTR *)&rec->pieces, (UINT_PTR)rec);
    RelocatePointer((UINT_PTR *)&rec->train, (UINT_PTR)rec);
    RelocatePointer((UINT_PTR *)&rec->visitors, (UINT_PTR)rec);
    SetMapObjectsEnabled(0);
    for (i = 0; i < rec->piece_count; i++) {
        CastleDispatchAddObjectByIndex(rec->pieces[i].kind, (short *)&rec->pieces[i].xy);
    }
    obj = GetPlacedCastleCoaster();
    if (rec->train != 0) {
        LoadCastleTrain(rec->train, obj);
    }
    if (rec->visitors != 0) {
        LoadCastleVisitors(rec->visitors, rec->visitor_count, obj);
    }
    MemFree(rec);
    SetMapObjectsEnabled(1);
    return 1;
}

/* Saves the castle (one block: pieces, the train if it runs, the visitors), or a 0 if none is placed. */
// FUNCTION: LEGOLAND 0x00426ce0
int CastleObj_Save(struct Element *element) {
    struct CastleSaveInfo info;
    unsigned int zero;
    struct CastleSaveRecord *rec;
    struct CastleState *obj = GetPlacedCastleCoaster();

    if (obj != 0) {
        MeasureCastleSave(obj, &info);
        rec = MemAlloc(info.size, 0, 0, 0);
        if (rec == 0) {
            return 0;
        }
        WriteCastleSave(&info, rec);
        UnrelocatePointer((UINT_PTR *)&rec->pieces, (UINT_PTR)rec);
        UnrelocatePointer((UINT_PTR *)&rec->train, (UINT_PTR)rec);
        UnrelocatePointer((UINT_PTR *)&rec->visitors, (UINT_PTR)rec);
        WriteSaveRec((unsigned int *)rec);
        MemFree(rec);
        return 1;
    }
    zero = 0;
    SaveGameWrite(&zero, 4);
    return 1;
}

/* Counts what the save needs and its size. */
// FUNCTION: LEGOLAND 0x00426d80
void MeasureCastleSave(struct CastleState *param1, struct CastleSaveInfo *param2) {
    param2->castle = param1;
    param2->piece_count = CountCoasterPieces(param1);
    if (IsTrainRunning(param1->train) != 0) {
        param2->train_running = 1;
    } else {
        param2->train_running = 0;
    }
    param2->visitor_count = CountCastleVisitors((struct CastleState *)param1);
    param2->size = (param2->piece_count + param2->train_running * 4 + 4 + param2->visitor_count) * 8;
}

/* Fills the save block: the pieces, the train record when it runs and the visitor records. */
// FUNCTION: LEGOLAND 0x00426de0
void WriteCastleSave(struct CastleSaveInfo *src, struct CastleSaveRecord *dst) {
    unsigned char *end;

    dst->size = src->size;
    dst->piece_count = src->piece_count;
    dst->pieces = (struct PieceSaveRef *)&dst[1];
    SaveCoasterPieces(src->castle, dst->pieces);
    end = (unsigned char *)(dst->pieces + dst->piece_count);
    if (IsTrainRunning(src->castle->train) != 0) {
        struct Train *t = (struct Train *)src->castle->train;
        dst->train_count = 1;
        dst->train = (struct TrainSaveRecord *)end;
        SaveCastleTrain(t, (struct TrainSaveRecord *)end);
        end = (unsigned char *)(dst->train + dst->train_count);
    } else {
        dst->train = 0;
    }
    dst->visitor_count = src->visitor_count;
    if (dst->visitor_count != 0) {
        dst->visitors = (struct VisitorLoadRecord *)end;
        SaveCastleVisitors((struct CastleState *)src->castle, (struct VisitorSaveRecord *)end);
    } else {
        dst->visitors = 0;
    }
}

/* A piece's save reference: its element kind and map cell. */
// FUNCTION: LEGOLAND 0x00426e80
void SavePathSegRef(struct PathSeg *src, struct PieceSaveRef *dst) {
    dst->kind = FindCastleDispatchIndex(src->ride);
    dst->xy = src->xy;
}

/* The piece at the saved map cell. */
// FUNCTION: LEGOLAND 0x00426ea0
struct PathSeg *LoadPathSegRef(unsigned char *param_1, struct CastleState *param_2) {
    return FindPathSegByXY(param_2, (unsigned int *)(param_1 + 4));
}

/* Saves a walker: its piece and the index of its stretch in the piece (the index is not used when loading). */
// FUNCTION: LEGOLAND 0x00426ec0
void SaveCurveWalker(struct CurveWalker *arg0, unsigned int *arg1) {
    struct FVec3 local;
    struct SegBlob *cur;
    struct SegBlob *end;
    unsigned int count = 0;

    SavePathSegRef(arg0->piece, (struct PieceSaveRef *)arg1);
    cur = GetPieceCurve(arg0->piece, &local);
    end = arg0->node;
    while (cur != end) {
        cur = cur->next;
        if (cur == NULL)
            break;
        count++;
    }
    arg1[2] = count;
}

/* Restores a walker on its piece's first stretch. */
// FUNCTION: LEGOLAND 0x00426f10
void LoadCurveWalker(unsigned char *a, struct CurveWalker *b, struct CastleState *c) {
    b->piece = LoadPathSegRef(a, c);
    b->node = GetPieceCurve(b->piece, &b->pos);
}

/* Writes the train's save record: flags, position and energy, the time since its last step and the time left of its
 * start hold (so both survive the save as offsets). */
// FUNCTION: LEGOLAND 0x00426f40
void SaveCastleTrain(struct Train *src, struct TrainSaveRecord *dst) {
    SaveCurveWalker(&src->walker, &dst->field_14);
    dst->flags = src->flags;
    dst->pos = src->motion.pos;
    dst->energy = src->motion.energy;
    dst->since_step = GetGameTimer() - src->last_step_time;
    dst->hold_left = src->hold_until - GetGameTimer();
}

/* Restores the train from its save record (inverse of SaveCastleTrain). */
// FUNCTION: LEGOLAND 0x00426f90
void LoadCastleTrain(struct TrainSaveRecord *src, struct CastleState *obj) {
    struct Train *dst = obj->train;

    dst->flags = src->flags;
    LoadCurveWalker((unsigned char *)&src->field_14, (struct CurveWalker *)&dst->walker, (struct CastleState *)obj);
    PositionTrainCars(dst, src->pos, &dst->walker);
    SetTrainEnergy(dst, src->energy);
    dst->last_step_time = GetGameTimer() - src->since_step;
    dst->hold_until = GetGameTimer() + src->hold_left;
}

/* Index of the bloke in the castle ride's rider list, or -1. */
// FUNCTION: LEGOLAND 0x00426ff0
int FindCastleRiderIndex(struct Bloke *param) {
    int i = 0;
    struct RideNode *n = GetCastleDispatchRide(0)->riders;

    while (n != 0) {
        if (param == n->rider) {
            return i;
        }
        n = n->next;
        i++;
    }
    return -1;
}

/* The rider at index, or NULL. */
// FUNCTION: LEGOLAND 0x00427020
struct Bloke *GetCastleRiderByIndex(unsigned int index) {
    unsigned int i = 0;
    struct RideNode *n = GetCastleDispatchRide(0)->riders;

    while (n != 0) {
        if (index == i) {
            return n->rider;
        }
        n = n->next;
        i++;
    }
    return 0;
}

struct VisitorSaveRecord {
    unsigned int flags;
    unsigned int rider;
};

/* A visitor's save record: its state and rider index. */
// FUNCTION: LEGOLAND 0x00427050
void SaveCastleVisitor(struct CastleVisitor *param1, struct VisitorSaveRecord *param2) {
    param2->flags = param1->flags;
    param2->rider = FindCastleRiderIndex(param1->bloke);
}

struct VisitorLoadRecord {
    unsigned int flags;
    unsigned int rider;
};

/* Re-queues a saved visitor, seating it if it was on the train. */
// FUNCTION: LEGOLAND 0x00427070
void LoadCastleVisitor(struct VisitorLoadRecord *param_1, struct CastleState *param_2) {
    struct CastleVisitor *node;
    unsigned int kind;
    struct Bloke *index = GetCastleRiderByIndex(param_1->rider);
    node = AddCastleVisitor(index, param_2);
    kind = param_1->flags;
    node->flags = kind;
    if (kind == 2) {
        SeatCastleVisitor((struct CastleVisitor *)node, FindEmptyTrainSeat((struct Train *)param_2->train));
    }
}

/* Saves every visitor. */
// FUNCTION: LEGOLAND 0x004270c0
void SaveCastleVisitors(struct CastleState *a1, struct VisitorSaveRecord *a2) {
    struct CastleVisitor *node = a1->visitors.next;
    struct CastleVisitor *end = &a1->visitors;
    if (node != end) {
        struct VisitorSaveRecord *out = a2;
        do {
            struct VisitorSaveRecord *cur = out;
            out = out + 1;
            SaveCastleVisitor(node, cur);
            node = node->next;
        } while (node != end);
    }
}

/* Restores count visitors. */
// FUNCTION: LEGOLAND 0x00427100
void LoadCastleVisitors(struct VisitorLoadRecord *arr, int count, struct CastleState *obj) {
    for (; count > 0; count--) {
        LoadCastleVisitor(arr++, obj);
    }
}

/* Number of visitors in the queue. */
// FUNCTION: LEGOLAND 0x00427130
unsigned int CountCastleVisitors(struct CastleState *main) {
    unsigned int counter = 0;
    struct CastleVisitor *node = main->visitors.next;
    struct CastleVisitor *end = &main->visitors;
    while (node != end) {
        node = node->next;
        counter++;
    }
    return counter;
}

/* Number of pieces including the castle. */
// FUNCTION: LEGOLAND 0x00427150
unsigned int CountCoasterPieces(struct CastleState *arg) {
    unsigned int count = 1;

    if (arg->flags == 2) {
        struct PathSeg *next = arg->piece.next;
        struct PathSeg *end = (struct PathSeg *)&arg->piece;
        if (next != end) {
            do {
                next = next->next;
                count++;
            } while (next != end);
        }
        return count;
    }

    if (arg->piece.next != NULL) {
        struct PathSeg *curr = arg->piece.next;
        do {
            curr = curr->next;
            count++;
        } while (curr != NULL);
    }

    if (arg->piece.prev != NULL) {
        struct PathSeg *curr = arg->piece.prev;
        do {
            curr = curr->prev;
            count++;
        } while (curr != NULL);
    }

    return count;
}

/* Saves the castle and every piece (ring order, or after then before the castle). */
// FUNCTION: LEGOLAND 0x00427190
void SaveCoasterPieces(struct CastleState *obj, struct PieceSaveRef *out) {
    struct PathSeg *cur;

    out->kind = FindCastleDispatchIndex(obj->piece.ride);
    out->xy = obj->piece.xy;
    cur = obj->piece.next;
    out++;
    if (obj->flags == 2) {
        struct PathSeg *end = (struct PathSeg *)&obj->piece;
        if (cur != end) {
            do {
                SavePathSegRef(cur, out++);
                cur = cur->next;
            } while (cur != end);
        }
        return;
    }
    while (cur != 0) {
        SavePathSegRef(cur, out++);
        cur = cur->next;
    }
    cur = obj->piece.prev;
    while (cur != 0) {
        SavePathSegRef(cur, out++);
        cur = cur->prev;
    }
}

/* Writes a castle save record (its first dword is its size) to the save game. */
// FUNCTION: LEGOLAND 0x00427220
void WriteSaveRec(unsigned int *param_1) {
    SaveGameWrite(param_1, *param_1);
}

/* Reads a castle save record from the save game; (CastleSaveRecord *)-1 when the castle was not placed, NULL when
 * out of memory. */
// FUNCTION: LEGOLAND 0x00427240
struct CastleSaveRecord *ReadSaveRec(void) {
    unsigned int size;
    struct CastleSaveRecord *rec;

    SaveGameRead(&size, 4);
    if (size == 0) {
        return (struct CastleSaveRecord *)-1;
    }
    rec = MemAlloc(size, 0, 0, 0);
    if (rec == 0) {
        return rec;
    }
    rec->size = size;
    SaveGameRead(&rec->field_4, size - 4);
    return rec;
}

/* Writes a block (first dword = size) to RollerCoasterSavePath. */
// FUNCTION: LEGOLAND 0x004272a0
int SaveRollerCoasterFile(unsigned int *data) {
    unsigned int size = *data;
    DWORD written;
    HANDLE hFile;

    if (RollerCoasterSavePath && data) {
        hFile = CreateFileA(RollerCoasterSavePath, 0x40000000, 0, 0, 2, 0x8000000, 0);
        if (hFile != (HANDLE)-1) {
            WriteFile(hFile, data, size, &written, 0);
            if (written != size) {
                CloseHandle(hFile);
            } else {
                CloseHandle(hFile);
                return 1;
            }
        }
    }
    return 0;
}

/* Reads RollerCoasterSavePath into a new block. */
// FUNCTION: LEGOLAND 0x00427310
void *LoadRollerCoasterFile(void) {
    DWORD bytesRead;
    HANDLE hFile;
    unsigned int fileSize;
    void *buffer;

    if (!RollerCoasterSavePath) {
        return 0;
    }
    hFile = CreateFileA(RollerCoasterSavePath, 0x80000000, 1, 0, 3, 0x8000000, 0);
    if (hFile == (HANDLE)-1) {
        return 0;
    }
    fileSize = GetFileSize(hFile, 0);
    buffer = MemAlloc(fileSize, 0, (unsigned int)(UINT_PTR)EmptyString, 0);
    if (buffer == 0) {
        CloseHandle(hFile);
        return 0;
    }
    ReadFile(hFile, buffer, fileSize, &bytesRead, 0);
    if (bytesRead != fileSize) {
        MemFree(buffer);
        CloseHandle(hFile);
        return 0;
    }
    CloseHandle(hFile);
    return buffer;
}

/* 1 if the seat has an occupant (the seat's method_10). */
// FUNCTION: LEGOLAND 0x004273c0
unsigned char IsTrainSeatOccupied(struct TrainSeat *param_1) {
    return (unsigned char)(param_1->occupant != 0);
}

/* Sets the seat's occupant (method_14). */
// FUNCTION: LEGOLAND 0x004273d0
void SetTrainSeatOccupant(struct TrainSeat *param_1, void *param_2) {
    param_1->occupant = param_2;
}

/* Takes the occupant out of the seat. */
// FUNCTION: LEGOLAND 0x004273e0
struct CastleVisitor *TakeTrainSeatOccupant(struct TrainSeat *self) {
    struct CastleVisitor *value = self->occupant;
    self->occupant = 0;
    return value;
}

/* Frees the seat's occupant (method_18). */
// FUNCTION: LEGOLAND 0x004273f0
void UnlinkSeatOccupant(struct TrainSeat *param_1) {
    struct CastleVisitor *obj = param_1->occupant;
    if (obj != NULL) {
        obj->unlink(obj);
    }
}

/* Draws the seat's occupant at the seat's position in the car's frame (the seat's draw). */
// FUNCTION: LEGOLAND 0x00427410
void DrawTrainSeat(struct TrainSeat *obj, struct CarXform *xf) {
    struct FVec3 old;

    if (obj->occupant != 0) {
        old = xf->pos;
        xf->pos.x = xf->m[6] * obj->pos.z + xf->m[0] * obj->pos.x + xf->m[3] * obj->pos.y + old.x;
        xf->pos.y = xf->m[7] * obj->pos.z + xf->m[1] * obj->pos.x + xf->m[4] * obj->pos.y + old.y;
        xf->pos.z = xf->m[8] * obj->pos.z + xf->m[2] * obj->pos.x + xf->m[5] * obj->pos.y + old.z;
        obj->occupant->draw(obj->occupant, xf);
        xf->pos = old;
    }
}

/* Initialises a seat at pos with its methods, empty. */
// FUNCTION: LEGOLAND 0x004274b0
void InitTrainSeat(struct TrainSeat *obj, const struct FVec3 *src) {
    obj->pos = *src;
    obj->occupant = 0;
    /* the original stores this byte-returning function in an unsigned int slot (callers test eax) */
    obj->method_10 = (unsigned int (*)(struct TrainSeat *))IsTrainSeatOccupied;
    obj->method_14 = SetTrainSeatOccupant;
    obj->method_18 = UnlinkSeatOccupant;
    obj->draw = DrawTrainSeat;
}

struct MapHeader {
    unsigned short field_0;
};

struct Struct4274f0Coord {
    short field_0;
    short field_2;
};

/* Sets the 2x2 map cells at coord to the first BASIC TILES 1 tile (when map objects are enabled). Not referenced. */
// FUNCTION: LEGOLAND 0x004274f0
void SetCell2x2ToBasicTile(struct Struct4274f0Coord *coord) {
    if (MapObjectsEnabled != 0) {
        SetMapTile(coord->field_0, coord->field_2, ((struct MapHeader *)BasicTilesData)->field_0 + 1);
        SetMapTile(coord->field_0 + 1, coord->field_2, ((struct MapHeader *)BasicTilesData)->field_0 + 1);
        SetMapTile(coord->field_0, coord->field_2 + 1, ((struct MapHeader *)BasicTilesData)->field_0 + 1);
        SetMapTile(coord->field_0 + 1, coord->field_2 + 1, ((struct MapHeader *)BasicTilesData)->field_0 + 1);
    }
}

/* on_remove of the track pieces: nothing. */
// FUNCTION: LEGOLAND 0x004275b0
void TrackPieceOnRemove(void) {}

/* on_add of the track pieces: nothing. */
// FUNCTION: LEGOLAND 0x004275c0
void TrackPieceOnAdd(void) {}

/* SQUARE_TRACK ride CalcCursor: ghost cursors on every free cell at both ends, the cursor snapped to a free cell
 * within one cell of the mouse, blocked (0xe) if the piece cannot connect there; always blocked with a closed ring. */
// FUNCTION: LEGOLAND 0x004275d0
void SquareTrackCalcCursor(Element *obj, int *x, unsigned int y) {
    struct PiecePlacement *result;
    struct PieceClass *key;
    unsigned int bit;
    unsigned int *mask;
    struct Cursor *c;
    struct Ride *ride = obj->ride;
    struct Footprint *fp = &ride->footprint;
    int n = 0;
    int i;
    int found = 0;

    EditCursor.footprint = *fp;
    EditCursor.field_1830 = 0;
    ScreenToMapRef((int *)x, &EditCursor.tile_x, y);
    if (CastleCoaster.flags != 2) {
        bit = 1;
        for (i = 0; i <= 3; i++) {
            if ((bit & CastleCoaster.next_spots.mask) && EditCursor.tile_x - CastleCoaster.next_spots.pt[i][0] >= -1 && EditCursor.tile_x - CastleCoaster.next_spots.pt[i][0] <= 1 && (int)EditCursor.tile_y - CastleCoaster.next_spots.pt[i][1] >= -1 && (int)EditCursor.tile_y - CastleCoaster.next_spots.pt[i][1] <= 1) {
                EditCursor.tile_x = CastleCoaster.next_spots.pt[i][0];
                EditCursor.tile_y = CastleCoaster.next_spots.pt[i][1];
                found = 1;
                break;
            }
            bit <<= 1;
        }
        if (!found) {
            bit = 1;
            for (i = 0; i <= 3; i++) {
                if ((bit & CastleCoaster.prev_spots.mask) && EditCursor.tile_x - CastleCoaster.prev_spots.pt[i][0] >= -1 && EditCursor.tile_x - CastleCoaster.prev_spots.pt[i][0] <= 1 && (int)EditCursor.tile_y - CastleCoaster.prev_spots.pt[i][1] >= -1 && (int)EditCursor.tile_y - CastleCoaster.prev_spots.pt[i][1] <= 1) {
                    EditCursor.tile_x = CastleCoaster.prev_spots.pt[i][0];
                    EditCursor.tile_y = CastleCoaster.prev_spots.pt[i][1];
                    found = 1;
                    break;
                }
                bit <<= 1;
            }
        }
        ValidateCursor(&EditCursor, ride);
        bit = 1;
        for (i = 0; i <= 3; i++) {
            if (bit & CastleCoaster.next_spots.mask) {
                DefaultCursor(&FreeSpotCursors[n]);
                FreeSpotCursors[n].next = EditCursor.next;
                EditCursor.next = &FreeSpotCursors[n];
                FreeSpotCursors[n].tile_x = CastleCoaster.next_spots.pt[i][0];
                FreeSpotCursors[n].tile_y = CastleCoaster.next_spots.pt[i][1];
                FreeSpotCursors[n].footprint = *fp;
                FreeSpotCursors[n].field_1828 = 0x2032;
                ResetCursorBuildable(&FreeSpotCursors[n]);
                n++;
            }
            bit <<= 1;
        }
        bit = 1;
        for (i = 0; i <= 3; i++) {
            if (bit & CastleCoaster.prev_spots.mask) {
                DefaultCursor(&FreeSpotCursors[n]);
                FreeSpotCursors[n].next = EditCursor.next;
                EditCursor.next = &FreeSpotCursors[n];
                FreeSpotCursors[n].tile_x = CastleCoaster.prev_spots.pt[i][0];
                FreeSpotCursors[n].tile_y = CastleCoaster.prev_spots.pt[i][1];
                FreeSpotCursors[n].footprint = *fp;
                FreeSpotCursors[n].field_1828 = 0x2032;
                ResetCursorBuildable(&FreeSpotCursors[n]);
                n++;
            }
            bit <<= 1;
        }
        {
            struct Int16Pair pair;
            pair.x = (unsigned short)EditCursor.tile_x;
            pair.y = (unsigned short)EditCursor.tile_y;
            key = FindPieceClassForElement(obj);
            if (key == 0) {
                key = &SquareTrackPieceClass;
            }
            result = (struct PiecePlacement *)CheckCoasterPiecePlacement(key, &pair);
            if (result->joins == 0) {
                SetCursorBlocked(&EditCursor, 0xe);
            }
        }
        if (IsCursorBuildable(&EditCursor)) {
            SetCursorBlocked(&EditCursor, 0xe);
            bit = 1;
            mask = &CastleCoaster.next_spots.mask;
            c = FreeSpotCursors;
            for (i = 0; i < 8; i++) {
                if (i == 4) {
                    bit = 1;
                    mask = &CastleCoaster.prev_spots.mask;
                }
                if (bit & *mask) {
                    if (EditCursor.tile_x == c->tile_x && EditCursor.tile_y == c->tile_y) {
                        ResetCursorBuildable(&EditCursor);
                        return;
                    }
                    c++;
                }
                bit <<= 1;
            }
        }
    } else {
        SetCursorBlocked(&EditCursor, 0xe);
    }
}

/* SetEditMode handler of all track elements: build mode with the element's footprint. */
// FUNCTION: LEGOLAND 0x00427940
void TrackPieceSetEditMode(struct Element *param_1) {
    struct Ride *value = param_1->ride;
    EditMode.unk0 = 1;
    EditMode.unk8 = value;
    SetEditCursorFootPrint(&value->footprint);
    EditCursor.field_1830 = 0;
}

struct Struct427970Src {
    short field_0;
    short pad_2;
    short field_4;
};

/* DCalcCursor handler of all track elements: only the head or tail piece (or any piece of a closed ring) can be
 * removed; remembers it in CoasterPieceToRemove. */
// FUNCTION: LEGOLAND 0x00427970
void CastleRemovePieceCalcCursor(Element *obj, const struct Struct427970Src *src) {
    struct Ride *key = obj->ride;
    struct Int16Pair pair;
    struct PathSeg *node;

    BasicObjectDCalcCursor(obj, (struct Point *)src);
    pair.x = src->field_0;
    pair.y = src->field_4;
    node = FindPathSegAt((struct CastleState *)&CastleCoaster, key, (short *)&pair);
    if (IsCoasterEndPiece(node) == 0) {
        SetCursorBlocked(&QueryCursor, 1);
        CoasterPieceToRemove = 0;
    } else {
        ResetCursorBuildable(&QueryCursor);
        CoasterPieceToRemove = node;
    }
}

/* RemoveObject handler of all track elements: removes CoasterPieceToRemove. */
// FUNCTION: LEGOLAND 0x004279f0
void RemoveQueriedCoasterPiece(void) {
    RemoveCoasterPiece(CoasterPieceToRemove);
}

/* Render of a track cell: draws the piece's track, then the train cars on it. */
// FUNCTION: LEGOLAND 0x00427a00
void RenderSquareTrack(struct Element *unused0, unsigned int unused1, unsigned int unused2, const unsigned char *src) {
    struct PieceHead *node;
    struct Int16Pair tile;
    tile.x = src[0];
    tile.y = src[1];
    node = (struct PieceHead *)FindPathSegByXY((struct CastleState *)&CastleCoaster, (unsigned int *)&tile);
    DrawTrackPiece(node);
    DrawCastleTrainOnPiece(node);
}

struct Struct427a40 {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
    unsigned int field_24;
};

/* fill_open_end of plain track: the unconnected end continues straight on at the same height. */
// FUNCTION: LEGOLAND 0x00427a40
void SquareTrackFillOpenEnd(struct Struct427a40 *param_1) {
    if (param_1->field_20 == 0xffffffff) {
        param_1->field_20 = GetOppositeDirection(param_1->field_14);
        param_1->field_24 = param_1->field_18;
    } else if (param_1->field_14 == 0xffffffff) {
        param_1->field_14 = GetOppositeDirection(param_1->field_20);
        param_1->field_18 = param_1->field_24;
    }
}

struct Struct427a80 {
    unsigned char pad_0[0x18];
    unsigned int field_18;
    unsigned char pad_1c[0x24 - 0x1c];
    float field_24;
};

/* set_heights of plain track: both ends take the height of the neighbour it was attached to. */
// FUNCTION: LEGOLAND 0x00427a80
void SquareTrackSetHeights(struct Struct427a80 *param_1, float param_2) {
    param_1->field_24 = param_2;
    param_1->field_18 = *(unsigned int *)&param_2;
}

/* SQUARE_TRACK load: remembers its ride, loads BASIC TILES 1 and builds the piece curves and rail draw order. */
// FUNCTION: LEGOLAND 0x00427aa0
void CastleLoadBasicTiles(Element *obj) {
    struct Element *handle;

    SquareTrackRide = obj->ride;
    SquareTrackRide->layer->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b5f34
    if (LLIDB_FindElement("BASIC TILES 1", &handle, 0) == 0) {
        BasicTilesData = LLIDB_LoadData((void *)handle);
    }
    BuildTrackPieceCurves();
    InitRailDrawOrder();
}

/* SQUARE_TRACK unload: frees BASIC TILES 1. */
// FUNCTION: LEGOLAND 0x00427af0
void CastleUnloadBasicTiles(void) {
    struct Element *handle;
    unsigned int packed;
    // STRING: LEGOLAND 0x004b5f34
    if (LLIDB_FindElement("BASIC TILES 1", &handle, 0) == 0) {
        LLIDB_UnLoadData(handle);
    }
}

/* CalcCursor handler of SQUARE_TRACK: buildable where a plain piece connects. */
// FUNCTION: LEGOLAND 0x00427b20
void SquareTrackPieceCalcCursor(Element *obj, int *x, unsigned int y) {
    struct Int16Pair tile;
    struct PiecePlacement *result;

    SetEditCursorFootPrint(&obj->ride->footprint);
    ScreenToMapRef((int *)x, &EditCursor.tile_x, y);
    ValidateCursor(&EditCursor, obj->data);
    EditCursor.field_1828 |= 8;
    tile.x = (unsigned short)EditCursor.tile_x;
    tile.y = (unsigned short)EditCursor.tile_y;
    result = CheckCoasterPiecePlacement(&SquareTrackPieceClass, &tile);
    if (result->joins != 0) {
        ResetCursorBuildable(&EditCursor);
    } else {
        SetCursorBlocked(&EditCursor, 0xe);
    }
}

struct Struct427bc0Src {
    short field_0;
    short pad_2;
    short field_4;
};

/* AddObject handler of SQUARE_TRACK: adds a plain piece (flags 6). */
// FUNCTION: LEGOLAND 0x00427bc0
void SquareTrackAddObject(struct Element *unused, const struct Struct427bc0Src *src) {
    struct Int16Pair pair;
    struct PathSeg *entry;
    pair.x = src->field_0;
    pair.y = src->field_4;
    entry = AddCoasterPiece(SquareTrackRide, &SquareTrackPieceClass, &pair);
    if (entry != NULL) {
        entry->flags |= 6;
    }
}

/* The piece class registered for a height block element, or NULL. */
// FUNCTION: LEGOLAND 0x00427c00
struct PieceClass *FindPieceClassForElement(struct Element *key) {
    int i;
    for (i = 0; i < (int)ElementPieceClassCount; i++) {
        if (ElementPieceClasses[i].key == key)
            return ElementPieceClasses[i].value;
    }
    return 0;
}

struct Struct427c30 {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x20 - 0x1c];
    unsigned int field_20;
    unsigned int field_24;
};

/* fill_open_end of the height blocks (same as plain track). */
// FUNCTION: LEGOLAND 0x00427c30
void HeightTrackFillOpenEnd(struct Struct427c30 *param_1) {
    if (param_1->field_20 == 0xffffffff) {
        param_1->field_20 = GetOppositeDirection(param_1->field_14);
        param_1->field_24 = param_1->field_18;
        return;
    }
    if (param_1->field_14 == 0xffffffff) {
        param_1->field_14 = GetOppositeDirection(param_1->field_20);
        param_1->field_18 = param_1->field_24;
    }
}

struct Struct427c70Inner {
    unsigned char pad_0[4];
    int field_4;
    int field_8;
};

struct Struct427c70 {
    unsigned char pad_0[0xc];
    struct Struct427c70Inner *field_c;
    unsigned char pad_10[0x18 - 0x10];
    float field_18;
    unsigned char pad_1c[0x24 - 0x1c];
    float field_24;
};

/* set_heights of the height blocks: their own fixed heights from the class (f4 next end, f8 prev end). */
// FUNCTION: LEGOLAND 0x00427c70
void HeightTrackSetHeights(struct Struct427c70 *param_1) {
    struct Struct427c70Inner *inner = param_1->field_c;
    param_1->field_24 = (float)inner->field_4;
    param_1->field_18 = (float)inner->field_8;
}

/* CalcCursor handler of SQUARE_TRACK_HEIGHT(_0): ghost cursors on the free cells; buildable where the block's class
 * connects. */
// FUNCTION: LEGOLAND 0x00427c90
void HeightTrackCalcCursor(Element *obj, int *x, unsigned int y) {
    struct Int16Pair pair;
    struct PiecePlacement *result;
    struct PieceClass *key;
    unsigned int bit;
    int n;
    struct Ride *ride = obj->ride;
    int i;

    SetEditCursorFootPrint(&ride->footprint);
    EditCursor.field_1830 = 0;
    ScreenToMapRef((int *)x, &EditCursor.tile_x, y);
    n = 0;
    bit = 1;
    for (i = 0; i <= 3; i++) {
        if (bit & CastleCoaster.next_spots.mask) {
            DefaultCursor(&FreeSpotCursors[n]);
            FreeSpotCursors[n].next = EditCursor.next;
            EditCursor.next = &FreeSpotCursors[n];
            FreeSpotCursors[n].tile_x = CastleCoaster.next_spots.pt[i][0];
            FreeSpotCursors[n].tile_y = CastleCoaster.next_spots.pt[i][1];
            FreeSpotCursors[n].footprint = ride->footprint;
            FreeSpotCursors[n].field_1828 = 0x2032;
            ResetCursorBuildable(&FreeSpotCursors[n]);
            n++;
        }
        bit <<= 1;
    }
    bit = 1;
    for (i = 0; i <= 3; i++) {
        if (bit & CastleCoaster.prev_spots.mask) {
            DefaultCursor(&FreeSpotCursors[n]);
            FreeSpotCursors[n].next = EditCursor.next;
            EditCursor.next = &FreeSpotCursors[n];
            FreeSpotCursors[n].tile_x = CastleCoaster.prev_spots.pt[i][0];
            FreeSpotCursors[n].tile_y = CastleCoaster.prev_spots.pt[i][1];
            FreeSpotCursors[n].footprint = ride->footprint;
            FreeSpotCursors[n].field_1828 = 0x2032;
            ResetCursorBuildable(&FreeSpotCursors[n]);
            n++;
        }
        bit <<= 1;
    }
    ValidateCursor(&EditCursor, obj->data);
    pair.x = (unsigned short)EditCursor.tile_x;
    pair.y = (unsigned short)EditCursor.tile_y;
    EditCursor.field_1828 |= 8;
    key = FindPieceClassForElement(obj);
    if (key != 0) {
        result = CheckCoasterPiecePlacement(key, &pair);
        if (result->joins != 0) {
            ResetCursorBuildable(&EditCursor);
        } else {
            SetCursorBlocked(&EditCursor, 0xe);
        }
    }
}

/* AddObject handler of SQUARE_TRACK_HEIGHT(_0): adds a piece of the element's class (flags 6). */
// FUNCTION: LEGOLAND 0x00427ea0
void HeightTrackAddObject(Element *obj, const struct Struct427bc0Src *src) {
    struct Int16Pair pair;
    struct Ride *ride = obj->ride;
    struct PieceClass *key;

    pair.x = src->field_0;
    pair.y = src->field_4;
    key = FindPieceClassForElement(obj);
    if (key != 0) {
        AddCoasterPiece(ride, key, &pair)->flags |= 6;
    }
}

/* SQUARE_TRACK_HEIGHT load: registers it with HeightTrack25PieceClass (height 25). */
// FUNCTION: LEGOLAND 0x00427ef0
void SquareTrackHeightLoad(Element *param_1) {
    struct Ride *a = param_1->ride;
    HeightTrack25Ride = a;
    a->layer->flags |= 0x2000;
    ElementPieceClasses[ElementPieceClassCount].key = param_1;
    ElementPieceClasses[ElementPieceClassCount].value = &HeightTrack25PieceClass;
    ElementPieceClassCount++;
}

/* SQUARE_TRACK_HEIGHT_0 load: registers it with HeightTrack2PieceClass (height 2). */
// FUNCTION: LEGOLAND 0x00427f30
void SquareTrackHeight0Load(Element *param_1) {
    struct Ride *a = param_1->ride;
    HeightTrack2Ride = a;
    a->layer->flags |= 0x2000;
    ElementPieceClasses[ElementPieceClassCount].key = param_1;
    ElementPieceClasses[ElementPieceClassCount].value = &HeightTrack2PieceClass;
    ElementPieceClassCount++;
}

struct Struct427f70Arg {
    unsigned char pad_0[4];
    short field_4;
    short field_6;
};

/* on_remove of the path height block: removes the roller coaster path from its 2x2 cells. */
// FUNCTION: LEGOLAND 0x00427f70
void RemoveRollerCoasterPath2x2(const struct Struct427f70Arg *arg) {
    int coords[2];

    coords[0] = arg->field_4;
    coords[1] = arg->field_6;
    RemoveRollerCoasterPath(coords);

    coords[0] = arg->field_4 + 1;
    coords[1] = arg->field_6;
    RemoveRollerCoasterPath(coords);

    coords[0] = arg->field_4 + 1;
    coords[1] = arg->field_6 + 1;
    RemoveRollerCoasterPath(coords);

    coords[0] = arg->field_4;
    coords[1] = arg->field_6 + 1;
    RemoveRollerCoasterPath(coords);
}

struct Struct427ff0Point {
    unsigned short pad_0;
    unsigned short pad_2;
    short x;
    short y;
};

/* on_add of the path height block: puts roller coaster path on its 2x2 cells. */
// FUNCTION: LEGOLAND 0x00427ff0
void AddRollerCoasterPath2x2(struct Struct427ff0Point *point) {
    int coords[2];

    coords[0] = point->x;
    coords[1] = point->y;
    AddRollerCoasterPath(coords);

    coords[0] = point->x + 1;
    coords[1] = point->y;
    AddRollerCoasterPath(coords);

    coords[0] = point->x + 1;
    coords[1] = point->y + 1;
    AddRollerCoasterPath(coords);

    coords[0] = point->x;
    coords[1] = point->y + 1;
    AddRollerCoasterPath(coords);
}

/* SQUARE_TRACK_HEIGHT_PATH load: registers it with HeightTrackPathPieceClass. */
// FUNCTION: LEGOLAND 0x00428070
void SquareTrackHeightPathLoad(Element *param_1) {
    struct Ride *a = param_1->ride;
    a->layer->flags |= 0x2000;
    ElementPieceClasses[ElementPieceClassCount].key = param_1;
    ElementPieceClasses[ElementPieceClassCount].value = &HeightTrackPathPieceClass;
    ElementPieceClassCount++;
}

/* Placement-cursor callback for SQUARE_TRACK_HEIGHT_PATH. Add cursors for the enabled
 * points in both point sets, then validate the footprint and the selected track piece. */
// FUNCTION: LEGOLAND 0x004280b0
void HeightTrackPathCalcCursor(Element *obj, int *x, unsigned int y) {
    int total;
    struct Int16Pair pair;
    int a;
    int b;
    struct PiecePlacement *result;
    struct PieceClass *key;
    unsigned int bit;
    int n;
    struct Ride *ride;
    struct Footprint *fp;
    int i;

    /* Capture the ride before clearing the cursor chain, and calculate the footprint
     * address afterward. This order reproduces MSVC6's original setup instructions. */
    ride = obj->ride;
    total = 0;
    EditCursor.field_1830 = 0;
    fp = &ride->footprint;
    SetEditCursorFootPrint(fp);
    ScreenToMapRef((int *)x, &EditCursor.tile_x, y);
    n = 0;
    bit = 1;
    for (i = 0; i <= 3; i++) {
        if (bit & CastleCoaster.next_spots.mask) {
            DefaultCursor(&FreeSpotCursors[n]);
            FreeSpotCursors[n].next = EditCursor.next;
            EditCursor.next = &FreeSpotCursors[n];
            FreeSpotCursors[n].tile_x = CastleCoaster.next_spots.pt[i][0];
            FreeSpotCursors[n].tile_y = CastleCoaster.next_spots.pt[i][1];
            FreeSpotCursors[n].footprint = *fp;
            FreeSpotCursors[n].field_1828 = 0x2032;
            ResetCursorBuildable(&FreeSpotCursors[n]);
            n++;
        }
        bit <<= 1;
    }
    bit = 1;
    for (i = 0; i <= 3; i++) {
        if (bit & CastleCoaster.prev_spots.mask) {
            DefaultCursor(&FreeSpotCursors[n]);
            FreeSpotCursors[n].next = EditCursor.next;
            EditCursor.next = &FreeSpotCursors[n];
            FreeSpotCursors[n].tile_x = CastleCoaster.prev_spots.pt[i][0];
            FreeSpotCursors[n].tile_y = CastleCoaster.prev_spots.pt[i][1];
            FreeSpotCursors[n].footprint = *fp;
            FreeSpotCursors[n].field_1828 = 0x2032;
            ResetCursorBuildable(&FreeSpotCursors[n]);
            n++;
        }
        bit <<= 1;
    }
    ValidateCursor(&EditCursor, obj->data);
    pair.x = (unsigned short)EditCursor.tile_x;
    pair.y = (unsigned short)EditCursor.tile_y;
    EditCursor.field_1828 |= 8;
    key = FindPieceClassForElement(obj);
    if (key != 0) {
        result = CheckCoasterPiecePlacement(key, &pair);
        if (result->joins == 0) {
            SetCursorBlocked(&EditCursor, 0xe);
        } else {
            CountCastleCoasterSegs(result->owner, &a, &b);
            if (a >= 0) {
                total = a;
            }
            if (b >= 0) {
                total += b;
            }
            if (total >= 2) {
                SetCursorBlocked(&EditCursor, 0xb);
            } else {
                ResetCursorBuildable(&EditCursor);
            }
        }
    }
}

/* AddObject handler of SQUARE_TRACK_HEIGHT_PATH: adds a piece of its class with flags 6 cleared. */
// FUNCTION: LEGOLAND 0x00428300
void HeightTrackPathAddObject(Element *obj, const struct Struct427bc0Src *src) {
    struct Int16Pair pair;
    struct Ride *ride = obj->ride;
    struct PieceClass *key;

    pair.x = src->field_0;
    pair.y = src->field_4;
    key = FindPieceClassForElement(obj);
    if (key != 0) {
        AddCoasterPiece(ride, key, &pair)->flags &= ~6;
    }
}

/* A straight piece stretch from the middle of side a to the middle of side b, dropping c * 6, with the rail offset
 * of direction d. */
// FUNCTION: LEGOLAND 0x00428350
void InitStraightPieceCurve(int a, int b, int c, struct LineCurve *obj, int d) {
    Vector3 pt;

    pt.x = TrackSideMidpoints[b].x;
    pt.y = TrackSideMidpoints[b].y;
    pt.z = TrackSideMidpoints[b].z - (float)(c * 6);
    InitLineCurve(obj, &TrackSideMidpoints[a].x, &pt.x, (struct Words3 *)&TrackRailOffsets[d]);
}

/* Index of the piece's stretch in TrackPieceCurves: 0..7 for the eight turns, else 8 + 5 * direction + (height
 * change / 2 + 2) for straights. */
// FUNCTION: LEGOLAND 0x004283c0
unsigned int GetTrackPieceCurveIndex(struct PieceHead *pair) {
    unsigned int kind = GetPieceHeadDirPairIndex(pair);
    int v = (((int)pair->end_b.height - (int)pair->end_a.height) >> 1) + 2;

    if (pair->end_a.dir == GetOppositeDirection(pair->end_b.dir)) {
        if (kind == 8) {
            v += kind;
        } else if (kind == 2) {
            v += 0xd;
        } else if (kind == 0xd) {
            v += 0x12;
        } else if (kind == 7) {
            v += 0x17;
        }
        return v;
    }
    switch (kind) {
    case 12:
        return 0;
    case 3:
        return 1;
    case 4:
        return 2;
    case 1:
        return 3;
    case 6:
        return 4;
    case 9:
        return 5;
    case 14:
        return 6;
    case 11:
        return 7;
    }
    return 0xffffffff;
}

/* Builds TrackPieceCurves: the eight quarter-circle turns (two per corner), then for each of the four directions
 * five straights with drops -24, -12, 0, 12, 24. */
// FUNCTION: LEGOLAND 0x004284d0
void BuildTrackPieceCurves(void) {
    struct ArcCurve *base;
    struct ArcCurve *cur;
    struct Words3 *p;
    struct Words3 *q;
    int x;
    unsigned int y;
    int j;
    unsigned int i;
    unsigned int k;
    int n;
    n = 0;

    for (i = 0; i <= 3; i++) {
        TrackSideMidpoints[i].x = TrackSideMidpointsUnit[i][0] * 20.0f;
        TrackSideMidpoints[i].y = TrackSideMidpointsUnit[i][1] * 20.0f;
        TrackSideMidpoints[i].z = TrackSideMidpointsUnit[i][2] * 20.0f;
    }
    for (i = 0; i <= 3; i++) {
        TrackRailOffsets[i].x = TrackRailOffsetsUnit[i][0] * 20.0f;
        TrackRailOffsets[i].y = TrackRailOffsetsUnit[i][1] * 20.0f;
        TrackRailOffsets[i].z = TrackRailOffsetsUnit[i][2] * 20.0f;
    }
    for (i = 0; i <= 3; i++) {
        TrackArcTangents[i].x = TrackArcTangentsUnit[i][0] * 20.0f;
        TrackArcTangents[i].y = TrackArcTangentsUnit[i][1] * 20.0f;
        TrackArcTangents[i].z = TrackArcTangentsUnit[i][2] * 20.0f;
    }
    for (i = 0; i <= 3; i++) {
        TrackArcCentres[i].x = TrackArcCentresUnit[i][0] * 20.0f;
        TrackArcCentres[i].y = TrackArcCentresUnit[i][1] * 20.0f;
        TrackArcCentres[i].z = TrackArcCentresUnit[i][2] * 20.0f;
    }
    for (i = 0; i <= 3; i++) {
        q = (struct Words3 *)&TrackArcTangents[TrackArcTangentPairs[i][1]];
        y = TrackArcRadii[i][1];
        p = (struct Words3 *)&TrackArcTangents[TrackArcTangentPairs[i][0]];
        x = TrackArcRadii[i][0];
        InitArcCurve(p, q, (struct Words3 *)&TrackArcCentres[i], &((struct ArcCurve *)TrackPieceCurves)[n++], x, y);
        if (5 == n) { /* the original has a test with an empty body here (cmp n, 5; jne +0) */
            n = 5;
        }
        InitArcCurve(q, p, (struct Words3 *)&TrackArcCentres[i], &((struct ArcCurve *)TrackPieceCurves)[n++], y, x);
    }
    base = &((struct ArcCurve *)TrackPieceCurves)[n];
    for (j = 0; j <= 3; j++) {
        cur = base;
        base += 5;
        for (k = 0; k < 5; k++) {
            InitStraightPieceCurve(TrackStraightStartSides[j], (TrackStraightStartSides[j] - 2) & 3, TrackStraightHeightSteps[k], (struct LineCurve *)cur++, j);
        }
    }
}

struct Struct4286e0Element {
    unsigned int field_0;
    unsigned int field_4;
};

/* get_curve of the track pieces: the piece's stretch in TrackPieceCurves. */
// FUNCTION: LEGOLAND 0x004286e0
struct Struct4286e0Element *GetTrackPieceCurve(void *param1) {
    unsigned int result = GetTrackPieceCurveIndex(param1);
    unsigned int index = result + (result + result * 4) * 2;
    return &((struct Struct4286e0Element *)TrackPieceCurves)[index];
}

/* Draws a piece's track: rails, sleeper, and a support post only when the piece has flags & 6 (not under path
 * blocks). */
// FUNCTION: LEGOLAND 0x00428700
void DrawTrackPiece(void *node) {
    struct FVec3 buf;
    unsigned int idx = GetPieceHeadDirPairIndex((struct PieceHead *)node);
    struct SegBlob *r = GetPieceCurve(node, &buf);
    if (*(unsigned char *)node & 6) {
        DrawTrackStretch(r, &buf, RailDrawOrder[idx], 0);
    } else {
        DrawTrackStretch(r, &buf, RailDrawOrder[idx], 1);
    }
}

struct DirRule {
    unsigned int from;
    unsigned int to;
    unsigned int kind;
};

struct AnimPairBuf {
    struct PieceHead pair;
    unsigned char pad_2c[0xa4 - 0x2c];
};

/* Fills RailDrawOrder: for each (prev, next) direction pair, which rail DrawTrackStretch draws first (1 side A, 2
 * side B); reversing the pair swaps it. */
// FUNCTION: LEGOLAND 0x00428750
void InitRailDrawOrder(void) {
    int n;
    struct DirRule table[6];
    struct AnimPairBuf buf;

    table[0].from = 1;
    table[0].to = 4;
    table[0].kind = 2;
    table[1].from = 2;
    table[1].to = 8;
    table[1].kind = 2;
    table[2].from = 1;
    table[2].to = 2;
    table[2].kind = 1;
    table[3].from = 2;
    table[3].to = 4;
    table[3].kind = 2;
    table[4].from = 4;
    table[4].to = 8;
    table[4].kind = 1;
    table[5].from = 8;
    table[5].to = 1;
    table[5].kind = 1;
    for (n = 0; n < 6; n++) {
        unsigned int idx;

        buf.pair.end_a.dir = table[n].from;
        buf.pair.end_b.dir = table[n].to;
        RailDrawOrder[GetPieceHeadDirPairIndex(&buf.pair)] = table[n].kind;
        buf.pair.end_a.dir = table[n].to;
        buf.pair.end_b.dir = table[n].from;
        idx = GetPieceHeadDirPairIndex(&buf.pair);
        if (table[n].kind == 2) {
            RailDrawOrder[idx] = 1;
        } else {
            RailDrawOrder[idx] = 2;
        }
    }
}

/* Index of the lowest set bit (0 for 0). */
// FUNCTION: LEGOLAND 0x00428840
unsigned int LowestSetBitIndex(unsigned int param_1) {
    unsigned int mask = 1;
    unsigned int result = 0;
    if ((param_1 & 1) == 0) {
        while ((param_1 & mask) == 0) {
            mask = mask * 2;
            result = result + 1;
        }
    }
    return result;
}

/* Span filler (TexturedSpanFillers, both entries): affine texture mapping from the triangle's .ltx texture, shaded
 * through its shade table, with the depth test of FillSpansFlatZ. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00428860
__declspec(naked) void FillSpansTexturedZ(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x70
        mov eax, dword ptr [ebp + 0x14]
        mov edx, dword ptr [ebp + 0x10]
        push ebx
        push esi
        mov ecx, dword ptr [eax]
        push edi
        mov dword ptr [ebp - 4], ecx
        lea ecx, [eax + edx*8 - 4]
        mov edx, dword ptr [ebp + 0x18]
        mov dword ptr [ebp - 8], ecx
        mov eax, dword ptr [ecx]
        lea eax, [eax + eax*2]
        shl eax, 4
        inc word ptr [eax + edx + 2]
        lea eax, [eax + edx + 2]
        mov eax, dword ptr [ecx]
        lea ecx, [eax + eax*2]
        mov eax, dword ptr [CoasterSurfaceBits]
        shl ecx, 4
        mov dword ptr [ebp - 0x20], eax
        mov eax, dword ptr [ebp + 8]
        movsx edx, word ptr [ecx + edx + 2]
        mov ecx, dword ptr [CoasterDepthBuffer]
        mov dword ptr [ebp - 0x14], edx
        mov edx, dword ptr [CoasterSurfacePitch]
        push eax
        mov dword ptr [ebp - 0x1c], ecx
        mov dword ptr [ebp - 0x18], edx
        call GetLtxFileTableEntry
        mov esi, eax
        add esp, 4
        test esi, esi
        je L428b5c
        mov ecx, dword ptr [esi]
        push ecx
        call LowestSetBitIndex
        mov edx, dword ptr [esi + 4]
        mov edi, eax
        push edx
        call LowestSetBitIndex
        add esi, 8
        mov ecx, 8
        mov dword ptr [ebp + 8], esi
        sub ecx, edi
        mov esi, 0xff000000
        mov edx, ecx
        sar esi, cl
        sub edx, eax
        mov dword ptr [ebp - 0xc], ecx
        add edx, 8
        add esp, 8
        mov dword ptr [ebp - 0x10], edx
        mov dword ptr [TexturedSpanSource], esi
        mov esi, dword ptr [ebp + 0xc]
        mov ebx, dword ptr [esi + 4]
        sar ebx, cl
        mov ecx, edx
        mov edx, 1
        mov dword ptr [TexturedSpanState], ebx
        mov ebx, dword ptr [esi + 8]
        shl ebx, 8
        sar ebx, cl
        mov dword ptr [TexturedSpanState + 0x4], ebx
        mov ecx, dword ptr [esi + 0xc]
        mov dword ptr [TexturedSpanState + 0xc], ecx
        lea ecx, [eax + edi]
        mov eax, dword ptr [FilledTriangleCount]
        shl edx, cl
        dec edx
        inc eax
        mov dword ptr [FilledTriangleCount], eax
        mov eax, dword ptr [ebp - 8]
        mov dword ptr [TexturedSpanState + 0x14], edx
        mov edx, dword ptr [ebp + 0x18]
        mov eax, dword ptr [eax]
        lea ecx, [eax + eax*2]
        shl ecx, 4
        movsx eax, word ptr [ecx + edx + 2]
        mov ecx, dword ptr [ebp + 0x10]
        mov edx, dword ptr [ebp + 0x14]
        mov dword ptr [edx + ecx*8], eax
        mov eax, dword ptr [ebp - 0x18]
        imul eax, dword ptr [ebp - 4]
        mov edx, dword ptr [ebp - 0x20]
        mov ecx, dword ptr [ebp - 0x1c]
        shl eax, 1
        add edx, eax
        add ecx, eax
        mov dword ptr [ebp - 0x20], edx
        mov edx, dword ptr [CoasterShadeTableCount]
        test edx, edx
        mov dword ptr [ebp - 0x1c], ecx
        jle L4289ad
        mov ecx, offset TexelShadeCache
        mov eax, offset CoasterShadeTables
L428998:
        mov edi, dword ptr [esi]
        mov ebx, dword ptr [eax]
        add eax, 4
        add ecx, 2
        mov di, word ptr [ebx + edi*2]
        dec edx
        mov word ptr [ecx - 2], di
        jne L428998
L4289ad:
        mov dword ptr [ebp + 0x10], offset TexelShadeCache
L4289b4:
        mov ecx, dword ptr [ebp + 0x14]
        mov esi, dword ptr [ebp + 0x18]
        mov eax, dword ptr [ecx + 4]
        add ecx, 8
        mov dword ptr [ebp + 0x14], ecx
        lea eax, [eax + eax*2]
        shl eax, 4
        add eax, esi
        mov ecx, dword ptr [eax + 4]
        test ecx, ecx
        mov ecx, dword ptr [eax + 8]
        je L4289e2
        mov eax, dword ptr [eax + 0x1c]
        sub ecx, eax
        mov dword ptr [ebp - 0x34], eax
        mov dword ptr [ebp - 0x48], ecx
        jmp L428a20
L4289e2:
        mov ebx, dword ptr [eax + 0x10]
        mov edx, dword ptr [eax + 0x1c]
        mov esi, dword ptr [eax + 0xc]
        mov edi, dword ptr [eax + 0x20]
        mov dword ptr [ebp - 0x60], ebx
        mov ebx, dword ptr [eax + 0x24]
        sub ecx, edx
        mov dword ptr [ebp - 0x5c], edx
        mov edx, dword ptr [ebp - 0x60]
        mov dword ptr [ebp - 0x4c], ebx
        mov ebx, dword ptr [eax + 0x14]
        mov eax, dword ptr [eax + 0x28]
        mov dword ptr [ebp - 0x70], ecx
        mov ecx, dword ptr [ebp - 0x4c]
        sub esi, edi
        sub edx, ecx
        sub ebx, eax
        mov dword ptr [ebp - 0x50], edi
        mov dword ptr [ebp - 0x54], eax
        mov dword ptr [ebp - 0x64], esi
        mov dword ptr [ebp - 0x60], edx
        mov dword ptr [ebp - 0x68], ebx
L428a20:
        mov eax, dword ptr [ebp + 0x14]
        mov edx, dword ptr [ebp - 4]
        cmp edx, dword ptr [eax]
        jge L428b4e
        mov ecx, dword ptr [ebp - 0x18]
        lea edx, [ecx + ecx]
        mov dword ptr [ebp + 0xc], edx
L428a37:
        inc dword ptr [ebp - 4]
        mov eax, dword ptr [ebp - 0x70]
        mov ebx, dword ptr [ebp - 0x48]
        add eax, dword ptr [ebp - 0x5c]
        add ebx, dword ptr [ebp - 0x34]
        mov dword ptr [ebp - 0x70], eax
        mov dword ptr [ebp - 0x48], ebx
        mov ecx, ebx
        sub ecx, eax
        cmp ecx, 0x8000
        jns L428a78
        mov eax, dword ptr [ebp - 0x64]
        mov ebx, dword ptr [ebp - 0x60]
        mov ecx, dword ptr [ebp - 0x68]
        add eax, dword ptr [ebp - 0x50]
        add ebx, dword ptr [ebp - 0x4c]
        add ecx, dword ptr [ebp - 0x54]
        mov dword ptr [ebp - 0x64], eax
        mov dword ptr [ebp - 0x60], ebx
        mov dword ptr [ebp - 0x68], ecx
        jmp L428b2b
L428a78:
        sar eax, 0x10
        sar ebx, 0x10
        mov edi, dword ptr [ebp - 0x20]
        mov esi, dword ptr [ebp - 0x1c]
        xchg eax, ebx
        sub ebx, eax
        lea edi, [edi + eax*2]
        lea esi, [esi + eax*2]
        push ebx
        mov eax, dword ptr [ebp - 0x64]
        mov edx, dword ptr [ebp - 0x60]
        mov ebx, dword ptr [ebp - 0x68]
        add eax, dword ptr [ebp - 0x50]
        add edx, dword ptr [ebp - 0x4c]
        add ebx, dword ptr [ebp - 0x54]
        mov dword ptr [ebp - 0x64], eax
        mov dword ptr [ebp - 0x60], edx
        mov dword ptr [ebp - 0x68], ebx
        mov ecx, dword ptr [ebp - 0x10]
        shl edx, 8
        shr edx, cl
        mov ecx, dword ptr [ebp - 0xc]
        sar eax, cl
        push ebp
        sub esp, 8
        mov ecx, esi
        sub ecx, dword ptr [ebp + 8]
        mov dword ptr [esp + 4], ecx
        mov ecx, esi
        sub ecx, dword ptr [ebp + 0x10]
        sar ecx, 1
        mov dword ptr [esp], ecx
        mov ebp, ebx
        mov ebx, dword ptr [esp + 0xc]
L428ad3:
        mov ecx, ebp
        sar ecx, 0x10
        cmp cx, word ptr [esi + ebx*2]
        jb L428b0d
        mov word ptr [esi + ebx*2], cx
        mov ecx, dword ptr [TexturedSpanSource]
        and ecx, edx
        or ecx, eax
        shr ecx, 0x10
        and ecx, dword ptr [TexturedSpanState + 0x14]
        sub ecx, dword ptr [esp + 4]
        mov cl, byte ptr [esi + ecx]
        and ecx, 0xff
        sub ecx, dword ptr [esp]
        mov cx, word ptr [esi + ecx*2]
        mov word ptr [edi + ebx*2], cx
L428b0d:
        add eax, dword ptr [TexturedSpanState]
        add edx, dword ptr [TexturedSpanState + 0x4]
        add ebp, dword ptr [TexturedSpanState + 0xc]
        add ebx, 1
        jle L428ad3
        add esp, 8
        pop ebp
        add esp, 4
L428b2b:
        mov eax, dword ptr [ebp + 0xc]
        mov edi, dword ptr [ebp - 0x20]
        mov esi, dword ptr [ebp - 0x1c]
        mov ecx, dword ptr [ebp + 0x14]
        add edi, eax
        add esi, eax
        mov eax, dword ptr [ebp - 4]
        mov edx, dword ptr [ecx]
        cmp eax, edx
        mov dword ptr [ebp - 0x20], edi
        mov dword ptr [ebp - 0x1c], esi
        jl L428a37
L428b4e:
        mov edx, dword ptr [ebp - 4]
        mov eax, dword ptr [ebp - 0x14]
        cmp edx, eax
        jl L4289b4
L428b5c:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Builds the rail tube mesh and the post and sleeper shapes. */
// FUNCTION: LEGOLAND 0x00428b70
void InitTrackDrawing(void) {
    InitRailTubeMesh();
    InitSupportPostShape();
    InitTrackTieShape();
}

/* Debug: plots 30 points of the stretch's centre and both sides. */
// FUNCTION: LEGOLAND 0x00428b80
void PlotCurvePoints(struct SegBlob *curve, float *off) {
    float step = (curve->end - curve->start) * FLOAT_004ab44c;
    float t = curve->start;
    int i;
    int k = 0;

    for (i = 0; i < 30; i++) {
        curve->vt->pos(curve, t, CurvePlotWorldPoints[k]);
        CurvePlotWorldPoints[k][0] += off[0];
        CurvePlotWorldPoints[k][1] += off[1];
        CurvePlotWorldPoints[k][2] += off[2];
        k++;
        curve->vt->side_a_pos(curve, t, CurvePlotWorldPoints[k]);
        CurvePlotWorldPoints[k][0] += off[0];
        CurvePlotWorldPoints[k][1] += off[1];
        CurvePlotWorldPoints[k][2] += off[2];
        k++;
        curve->vt->side_b_pos(curve, t, CurvePlotWorldPoints[k]);
        CurvePlotWorldPoints[k][0] += off[0];
        CurvePlotWorldPoints[k][1] += off[1];
        CurvePlotWorldPoints[k][2] += off[2];
        k++;
        t = t + step;
    }
    ProjectCastlePoints(CurvePlotWorldPoints, CurvePlotScreenPoints, 0x5a);
    PlotClippedPoints(&CurvePlotScreenPoints[0][0], 0x5a, -1);
}

/* Builds the rail tube along one side of a stretch: at each split point a hexagonal ring (radius 1.8) around the
 * side's position, oriented by its tangent. Times itself into SupportPostDrawCycles. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00428cb0
__declspec(naked) void BuildRailTubeMesh(void *p, unsigned int a, unsigned int b, unsigned int c, const char *name) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0xc8
        mov eax, dword ptr [ebp + 0x14]
        push ebx
        push esi
        push edi
        lea ecx, [eax - 1]
        mov dword ptr [ebp - 0x18], ecx
        push eax
        push edx
        rdtsc
        mov dword ptr [ebp - 4], eax
        pop edx
        pop eax
        test ecx, ecx
        jl L428e2a
        mov edx, dword ptr [ebp + 0x18]
        mov edi, dword ptr [ebp + 8]
        mov ebx, offset RailTubeVertexBuffer
        inc ecx
        mov dword ptr [ebp + 8], edx
        mov dword ptr [ebp + 0x14], ebx
        mov dword ptr [ebp + 0x18], ecx
        jmp L428cf0
L428ced:
        mov ebx, dword ptr [ebp + 0x14]
L428cf0:
        mov eax, dword ptr [ebp + 8]
        mov edx, dword ptr [edi + 0x4c]
        mov ecx, dword ptr [eax]
        lea eax, [ebp - 0x48]
        mov esi, ecx
        mov dword ptr [ebp - 0x14], ecx
        mov ecx, dword ptr [ebp + 0x10]
        push eax
        push esi
        push edi
        call dword ptr [edx + ecx*8 + 4]
        lea edx, [ebp - 0x3c]
        lea eax, [ebp - 0x48]
        push edx
        push eax
        call BuildBasisFromDir
        mov ecx, dword ptr [edi + 0x4c]
        mov eax, dword ptr [ebp + 0x10]
        lea edx, [ebp - 0x10]
        push edx
        push esi
        push edi
        call dword ptr [ecx + eax*8]
        mov eax, dword ptr [ebp + 0xc]
        lea ecx, [ebp - 0x88]
        fld dword ptr [ebp - 0x10]
        fadd dword ptr [eax]
        lea edx, [ebp - 0x3c]
        push ecx
        push edx
        fstp dword ptr [ebp - 0x10]
        fld dword ptr [ebp - 0xc]
        fadd dword ptr [eax + 4]
        fstp dword ptr [ebp - 0xc]
        fld dword ptr [ebp - 8]
        fadd dword ptr [eax + 8]
        lea eax, [ebp - 0x10]
        push eax
        fstp dword ptr [ebp - 8]
        call MakeMat4FromRotTrans
        lea ecx, [ebp - 0xc8]
        lea edx, [ebp - 0x88]
        push ecx
        push edx
        push offset CastleWorldToScreen
        call Mat4Multiply
        push 6
        lea eax, [ebp - 0xc8]
        push 0x14
        push eax
        push ebx
        push offset RailTubeRingPoints
        call ProjectPoints
        fld dword ptr [CastleLightDirZ]
        fmul dword ptr [ebp - 0x28]
        fld dword ptr [CastleLightDirY]
        fmul dword ptr [ebp - 0x2c]
        add esp, 0x4c
        mov esi, offset RailTubeRingNormals
        add ebx, 0x10
        faddp st(1), st
        fld dword ptr [CastleLightDir]
        fmul dword ptr [ebp - 0x30]
        faddp st(1), st
        fmul dword ptr [CastleLightScale]
        fld dword ptr [CastleLightDirZ]
        fmul dword ptr [ebp - 0x1c]
        fld dword ptr [CastleLightDirY]
        fmul dword ptr [ebp - 0x20]
        faddp st(1), st
        fld dword ptr [CastleLightDir]
        fmul dword ptr [ebp - 0x24]
        faddp st(1), st
        fmul dword ptr [CastleLightScale]
        fld dword ptr [CastleLightBias]
L428de0:
        fld st(1)
        fmul dword ptr [esi + 4]
        fld st(3)
        fmul dword ptr [esi]
        faddp st(1), st
        fadd st, st(1)
        call _ftol
        mov dword ptr [ebx], eax
        add esi, 8
        add ebx, 0x14
        cmp esi, offset RailTubeTriangles
        jl L428de0
        mov edx, dword ptr [ebp + 0x14]
        mov ecx, dword ptr [ebp + 8]
        mov eax, dword ptr [ebp + 0x18]
        add edx, 0x78
        fstp st(0)
        add ecx, 4
        dec eax
        fstp st(0)
        fstp st(0)
        mov dword ptr [ebp + 0x14], edx
        mov dword ptr [ebp + 8], ecx
        mov dword ptr [ebp + 0x18], eax
        jne L428ced
        mov ecx, dword ptr [ebp - 0x18]
L428e2a:
        lea edx, [ecx + ecx*8]
        lea ecx, [ecx + ecx*2]
        shl ecx, 2
        lea eax, [edx + edx + 6]
        mov dword ptr [RailTubeMesh + 0xc], ecx
        mov dword ptr [RailTubeMesh + 0x8], eax
        push eax
        push edx
        rdtsc
        sub eax, dword ptr [ebp - 4]
        mov dword ptr [ebp - 4], eax
        pop edx
        pop eax
        mov edx, dword ptr [ebp - 4]
        mov eax, dword ptr [SupportPostDrawCycles]
        add eax, edx
        pop edi
        mov dword ptr [SupportPostDrawCycles], eax
        pop esi
        mov eax, offset RailTubeMesh
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Draws one rail of a stretch: computes the split points (vt split_points), builds and draws the tube. */
// FUNCTION: LEGOLAND 0x00428e70
void DrawTrackRail(struct SegBlob *p, const struct FVec3 *a, unsigned int b) {
    unsigned short handle = FPUSetSinglePrecision();
    unsigned int result = ((unsigned int (**)(void *, const char *))p->vt)[6](p, (const char *)RailSplitPoints);
    RailSplitCount = result;
    BuildRailTubeMesh(p, a, b, result, RailSplitPoints);
    DrawEdgeMeshTriangles(&RailTubeMesh);
    FPURestoreControlWord(handle);
}

/* Draws the other rail with the split points of the last DrawTrackRail. */
// FUNCTION: LEGOLAND 0x00428ec0
void DrawTrackRailSameSplit(struct SegBlob *p, const struct FVec3 *a, unsigned int b) {
    unsigned short handle = FPUSetSinglePrecision();
    BuildRailTubeMesh(p, a, b, RailSplitCount, RailSplitPoints);
    DrawEdgeMeshTriangles(&RailTubeMesh);
    FPURestoreControlWord(handle);
}

/* Builds the rail tube mesh's edges and triangles: 30 hexagonal rings joined by 12 triangles each, and the hexagon
 * profile. */
// FUNCTION: LEGOLAND 0x00428f00
void InitRailTubeMesh(void) {
    int i;
    int j;
    int n;
    int t = 0;
    int e;
    int v;
    int k;
    int cnt;
    int m;
    int k6;
    int k18;
    unsigned int *g;
    unsigned int *out;
    float ang;
    float s;

    {
        int *end = &RailTubeSegmentEdges[0][1]; /* the edge's second vertex */
        for (i = 0; i <= 4; i++, end += 2) {
            end[-1] = i;
            end[0] = i + 1;
        }
    }
    RailTubeSegmentEdges[5][0] = i;
    RailTubeSegmentEdges[5][1] = 0;
    n = 6;
    for (j = 0; j <= 4; j++) {
        RailTubeSegmentEdges[n][0] = j;
        RailTubeSegmentEdges[n][1] = j + 6;
        n++;
        RailTubeSegmentEdges[n][0] = j;
        RailTubeSegmentEdges[n][1] = j + 7;
        n++;
    }
    RailTubeSegmentEdges[n][0] = j;
    RailTubeSegmentEdges[n][1] = j + 6;
    n++;
    RailTubeSegmentEdges[n][0] = j;
    RailTubeSegmentEdges[n][1] = 6;
    n++;
    for (j = 0; j <= 4; j++) {
        RailTubeSegmentEdges[n][0] = j + 6;
        RailTubeSegmentEdges[n][1] = j + 7;
        n++;
    }
    RailTubeSegmentEdges[n][0] = j + 6;
    RailTubeSegmentEdges[n][1] = 6;

    v = 6;
    for (e = 18; e < 23; e++) {
        RailTubeSegmentTriangles[t * 3] = e - 18 + 0x80000000;
        RailTubeSegmentTriangles[t * 3 + 1] = v + 1;
        RailTubeSegmentTriangles[t * 3 + 2] = v + 2 + 0x80000000;
        t++;
        RailTubeSegmentTriangles[t * 3] = v;
        RailTubeSegmentTriangles[t * 3 + 1] = e;
        RailTubeSegmentTriangles[t * 3 + 2] = v + 1 + 0x80000000;
        t++;
        v += 2;
    }
    RailTubeSegmentTriangles[t * 3] = 0x80000005;
    RailTubeSegmentTriangles[t * 3 + 1] = v + 1;
    RailTubeSegmentTriangles[t * 3 + 2] = 0x80000006;
    t++;
    RailTubeSegmentTriangles[t * 3] = v;
    RailTubeSegmentTriangles[t * 3 + 1] = e;
    RailTubeSegmentTriangles[t * 3 + 2] = v + 1 + 0x80000000;

    n = 0;
    g = RailTubeTriangles[0];
    for (k = 0; k < 30; k++) {
        k6 = k * 6;
        k18 = k * 18;
        cnt = 18;
        if (k == 29) {
            cnt = 24;
        }
        for (m = 0; m < cnt; m++) {
            RailTubeEdges[n][0] = RailTubeSegmentEdges[m][0] + k6;
            RailTubeEdges[n][1] = RailTubeSegmentEdges[m][1] + k6;
            n++;
        }
        out = g;
        g += 36;
        for (i = 0; i < 12; i++) {
            for (j = 0; j < 3; j++) {
                unsigned int x = RailTubeSegmentTriangles[i * 3 + j];
                *out++ = ((x & 0x7fffffff) + k18) | (x & 0x80000000);
            }
        }
    }

    ang = FLOAT_004ab390;
    for (i = 0; i < 6; i++) {
        RailTubeRingPoints[i][0] = 0.0f;
        RailTubeRingPoints[i][1] = (float)cos(ang) * 1.8f;
        s = (float)sin(ang);
        RailTubeRingPoints[i][2] = s * 1.8f;
        RailTubeRingNormals[i][0] = (float)cos(ang);
        RailTubeRingNormals[i][1] = s;
        ang += 1.0472f;
    }
}

/* Draws the sleeper and (unless flag is 1) the support post at the middle of a stretch, the post behind or in front
 * depending on which way the track faces; above a height limit only the sleeper. */
// FUNCTION: LEGOLAND 0x00429150
void DrawTrackSupport(struct SegBlob *curve, float *off, int flag) {
    struct FVec3 pos;
    struct FVec3 dir;
    struct FVec3 basis[3];
    float t = (curve->end + curve->start) * 0.5f;

    curve->vt->tangent(curve, t, &dir.x);
    BuildBasisFromDir(&dir, basis);
    curve->vt->pos(curve, t, &pos.x);
    pos.x += off[0];
    pos.y += off[1];
    pos.z += off[2];
    if (pos.z > DOUBLE_004ab478) {
        DrawTrackTie(&pos, basis);
        return;
    }
    if (Vec3Dot((struct FVec3 *)CastleViewDir, &basis[2]) < FLOAT_004ab390) {
        if (flag != 1) {
            DrawSupportPost(&pos, basis);
        }
        DrawTrackTie(&pos, basis);
    } else {
        DrawTrackTie(&pos, basis);
        if (flag != 1) {
            DrawSupportPost(&pos, basis);
        }
    }
}

/* Scales the support post's shape. */
// FUNCTION: LEGOLAND 0x00429270
void InitSupportPostShape(void) {
    int i;

    for (i = 0; i <= 3; i++) {
        RailTubeRingPoints[6 + i][1] = RailTubeRingPoints[6 + i][0] = SupportPostShapeTemplate[0][i].a * 5.0f;
    }
    for (i = 0; i <= 3; i++) {
        SupportPostShape[1][i].a = SupportPostShapeTemplate[1][i].a * 1.5f;
        SupportPostShape[1][i].b = SupportPostShapeTemplate[1][i].b * 1.5f;
    }
    for (i = 0; i <= 3; i++) {
        SupportPostShape[2][i].a = SupportPostShapeTemplate[2][i].a * 1.5f;
        SupportPostShape[2][i].b = SupportPostShapeTemplate[2][i].b * 1.5f;
    }
}

/* Draws a support post from the ground (z 0) up to the plane of the track at pos. */
// FUNCTION: LEGOLAND 0x004292f0
void DrawSupportPost(struct FVec3 *pos, struct FVec3 *basis) {
    float ident[9];
    float zero[3];
    struct FVec3 *n;
    struct FVec3 *src;
    struct FVec3 *dst;
    float fx;
    float fy;
    float d;
    int t;
    int i;

    n = basis + 2;
    __asm {
        push eax
        push edx
        rdtsc
        mov t, eax
        pop edx
        pop eax
    }
    src = (struct FVec3 *)SupportPostShape;
    dst = (struct FVec3 *)RailTubeTriangles[30];
    fx = (float)((int)pos->x * DOUBLE_004ab488) * 5.0f;
    fy = (float)((int)pos->y * DOUBLE_004ab488) * 5.0f;
    for (i = 0; i <= 3; i++) {
        dst[i].x = fx + src[i].x;
        dst[i].y = fy + src[i].y;
        *(int *)&dst[i].z = *(int *)&src[i].z;
    }
    for (i = 4; i <= 7; i++) {
        dst[i].x = src[i].x + pos->x;
        dst[i].y = src[i].y + pos->y;
        dst[i].z = 0.0f;
    }
    d = Vec3Dot(pos, n);
    for (i = 8; i <= 11; i++) {
        dst[i].x = src[i].x + pos->x;
        dst[i].y = src[i].y + pos->y;
        dst[i].z = (d - n->x * dst[i].x - dst[i].y * n->y) / n->z;
    }
    __asm {
        push eax
        push edx
        rdtsc
        sub eax, t
        mov t, eax
        pop edx
        pop eax
    }
    SupportPostDrawCycles += t;
    zero[0] = 0.0f;
    zero[1] = 0.0f;
    zero[2] = 0.0f;
    ident[0] = 1.0f;
    ident[1] = 0.0f;
    ident[2] = 0.0f;
    ident[3] = 0.0f;
    ident[4] = 1.0f;
    ident[5] = 0.0f;
    ident[6] = 0.0f;
    ident[7] = 0.0f;
    ident[8] = 1.0f;
    DrawLmsModel(&SupportPostModel, (struct LmsRec *)SupportPostLmsRec, zero, ident, 1);
}

/* Draws the sleeper model at pos with the track's basis. */
// FUNCTION: LEGOLAND 0x00429490
void DrawTrackTie(struct FVec3 *param_1, struct FVec3 *param_2) {
    DrawLmsModel(&TrackTieModel, (struct LmsRec *)TrackTieLmsRec, param_1, param_2, 1);
}

/* Scales the sleeper's vertices (2.5 along the track, 8.2 across). */
// FUNCTION: LEGOLAND 0x004294b0
void InitTrackTieShape(void) {
    int i;

    for (i = 0; i < 8; i++) {
        TrackTieVerts[i].a = TrackTieShape[i].a * 2.5f;
        TrackTieVerts[i].b = TrackTieShape[i].b * 8.2f;
        TrackTieVerts[i].c = TrackTieShape[i].c;
    }
}

/* Draws one track stretch with the track lighting: the back rail, the sleeper and post, the front rail (c = 1: side
 * A is the back rail). d = 1: no post. */
// FUNCTION: LEGOLAND 0x004294f0
void DrawTrackStretch(struct SegBlob *a, struct FVec3 *b, unsigned int c, unsigned int d) {
    SetTrackLighting();
    if (c == 1) {
        DrawTrackRail(a, b, 0);
        DrawTrackSupport(a, &b->x, d);
        DrawTrackRailSameSplit(a, b, 2);
    } else {
        DrawTrackRail(a, b, 2);
        DrawTrackSupport(a, &b->x, d);
        DrawTrackRailSameSplit(a, b, 0);
    }
}

/* Gives `count` straight pieces from seg on one slope stretch rising dh from height h: each piece stores its share
 * of the parameter range (flags | 1). */
// FUNCTION: LEGOLAND 0x00429560
void BuildSlopeRun(struct PathSeg *seg, struct PathSeg *end, int count, float h, float dh) {
    float t;
    float step;
    struct FVec3 b;
    struct FVec3 a;
    struct SegBlob blob;
    unsigned int di = DirMaskToIndex(seg->dir_a);
    unsigned int dout = DirMaskToIndex(end->dir_b);
    int i;

    step = 1.0f / (float)count;
    t = 0.0f;
    MapPosToWorld(&seg->pos, h, &a);
    a.x += CoasterCellEdgeMidpoints[di][0];
    a.y += CoasterCellEdgeMidpoints[di][1];
    MapPosToWorld(&end->pos, h + dh, &b);
    b.x += CoasterCellEdgeMidpoints[dout][0];
    b.y += CoasterCellEdgeMidpoints[dout][1];
    InitSlopeCurve(&a.x, &b.x, (struct Words3 *)SlopeRunDirections[di], (struct LineCurve *)&blob);
    for (i = 0; i < count; i++) {
        seg->flags |= 1;
        seg->blob = blob;
        seg->blob.start = t;
        t += step;
        seg->blob.end = t;
        seg->field_40 = 0;
        seg->field_44 = 0;
        seg->field_48 = 0;
        seg = seg->next;
    }
}

/* Gives every piece from n up to the next straight one (or end) the height v at both ends: curves stay level.
 * Returns the piece it stopped at. */
// FUNCTION: LEGOLAND 0x00429690
struct PathSeg *SetHeightUntilStraight(struct PathSeg *n, float v, struct PathSeg *end) {
    while (!IsStraightSegment(n->cls, n->dir_a, n->dir_b) && n != end) {
        n->f24 = n->f18 = v;
        n = n->next;
    }
    return n;
}

/* Last piece of the straight run starting after param_1; *param_2 = its length. */
// FUNCTION: LEGOLAND 0x004296f0
struct PathSeg *FindStraightRunEnd(struct PathSeg *param_1, int *param_2) {
    struct PathSeg *node = param_1->next;
    *param_2 = 1;

    if (!IsStraightSegment(node->cls, node->dir_a, node->dir_b)) {
        return node->prev;
    }

    do {
        ++(*param_2);
        node = node->next;
    } while (IsStraightSegment(node->cls, node->dir_a, node->dir_b));

    return node->prev;
}

/* Interpolates the heights between two anchors: every straight run becomes a slope taking its share of the height
 * difference, the curves in between stay level. */
// FUNCTION: LEGOLAND 0x00429750
void InterpolateCoasterHeights(struct PathSeg *seg, struct PathSeg *end) {
    struct PathSeg *n;
    struct PathSeg *node;
    unsigned int count;
    float base;
    float step;

    count = CountStraightSegsForward(seg->next, &n);
    base = (float)(int)seg->cls->f4;
    if (count) {
        step = (float)(int)(end->cls->f8 - seg->cls->f4) / (int)count;
        n = seg->next;
        while (n != end) {
            if (IsStraightSegment(n->cls, n->dir_a, n->dir_b)) {
                int k;
                float h;
                node = FindStraightRunEnd(n, &k);
                h = (float)k * step;
                BuildSlopeRun(n, node, k, base, h);
                base = h + base;
                n = node->next;
            } else {
                n = SetHeightUntilStraight(n, base, end);
            }
        }
    }
}

/* From an open end piece, walks back (tail) or forward (head) to the nearest fixed-height piece. Returns it if a
 * straight piece lies in between (a slope can reach any height) or its facing end already has height x; else NULL. */
// FUNCTION: LEGOLAND 0x00429840
struct PathSeg *FindReachableAnchor(struct PathSeg *n, int x) {
    struct PathSeg *r;
    unsigned int ok;

    if (n->next == 0) {
        ok = CountStraightSegsBack(n, &n);
        r = n;
        if (ok || (float)x == r->f24) {
            return r;
        }
        return 0;
    } else {
        ok = CountStraightSegsForward(n, &n);
        r = n;
        if (ok || (float)x == r->f18) {
            return r;
        }
        return 0;
    }
}

/* For a plain piece that would close the ring: 1 if the straight pieces before the tail, after the head, plus the
 * new piece itself if straight, give at least one piece to build the slope on. */
// FUNCTION: LEGOLAND 0x004298a0
int HasStraightForClosingSlope(struct PieceClass *info, struct CastleState *owner, struct PathSeg **a, struct PathSeg **b) {
    int n = CountStraightSegsBack(owner->tail, a) + CountStraightSegsForward(owner->head, b);
    unsigned int da = GetOppositeDirection(owner->tail->dir_b);
    unsigned int db = GetOppositeDirection(owner->head->dir_a);

    if (IsStraightSegment(info, da, db)) {
        n++;
    }
    return n > 0;
}

/* 1 for a plain piece (class flags 0) going straight through. */
// FUNCTION: LEGOLAND 0x00429910
unsigned int IsStraightSegment(struct PieceClass *s, unsigned int v, unsigned int c) {
    unsigned int flag = (s->flags == 0);
    flag &= 1;
    if (flag) {
        if (c == GetOppositeDirection(v)) {
            return 1;
        }
    }
    return 0;
}

/* Straight pieces from n back to the previous anchor (stored in *out). */
// FUNCTION: LEGOLAND 0x00429940
unsigned int CountStraightSegsBack(struct PathSeg *n, struct PathSeg **out) {
    unsigned int count = 0;

    while (!(n->cls->flags & 1)) {
        if (IsStraightSegment(n->cls, n->dir_a, n->dir_b)) {
            count++;
        }
        n = n->prev;
    }
    *out = n;
    return count;
}

/* Straight pieces from n forward to the next anchor (stored in *out). */
// FUNCTION: LEGOLAND 0x00429990
unsigned int CountStraightSegsForward(struct PathSeg *n, struct PathSeg **out) {
    unsigned int count = 0;

    while (!(n->cls->flags & 1)) {
        if (IsStraightSegment(n->cls, n->dir_a, n->dir_b)) {
            count++;
        }
        n = n->next;
    }
    *out = n;
    return count;
}

/* Sets every piece after the anchor behind n, up to n, flat at that anchor's height (stored curves dropped). */
// FUNCTION: LEGOLAND 0x004299e0
void ResetCoasterHeightsBack(struct PathSeg *n) {
    struct PathSeg *p = n;
    float v;

    while (!(p->cls->flags & 1)) {
        p = p->prev;
    }
    v = (float)(int)p->cls->f4;
    p = p->next;
    while (p != n->next) {
        unsigned int fl = p->flags & ~1u;
        p->f18 = v;
        p->f24 = v;
        p->flags = fl;
        p = p->next;
    }
}

/* Sets every piece before the anchor ahead of n, down to n, flat at that anchor's height. */
// FUNCTION: LEGOLAND 0x00429a30
void ResetCoasterHeightsForward(struct PathSeg *n) {
    struct PathSeg *p = n;
    float v;

    while (!(p->cls->flags & 1)) {
        p = p->next;
    }
    v = (float)(int)p->cls->f8;
    p = p->prev;
    while (p != n->prev) {
        unsigned int fl = p->flags & ~1u;
        p->f18 = v;
        p->f24 = v;
        p->flags = fl;
        p = p->prev;
    }
}

/* World position of the walker's stretch at t: point pair param0 (0 side A, 1 centre, 2 side B). */
// FUNCTION: LEGOLAND 0x00429a80
void GetWalkerPoint(struct CurveWalker *a, int param0, int param2, struct FVec3 *b) {
    struct SegBlob *base = a->node;
    base->vt->pair[param0].func(base, param2, b);
    b->x += a->pos.x;
    b->y += a->pos.y;
    b->z += a->pos.z;
}

/* Tangent of the walker's stretch at t for point pair b. */
// FUNCTION: LEGOLAND 0x00429ac0
void GetWalkerTangent(struct CurveWalker *a, int b, int c, void *d) {
    struct SegBlob *obj = a->node;
    obj->vt->pair[b].fn(obj, c, d);
}

/* Basis from direction a with z kept up: b[0] = a, b[1] = up x a, b[2] = a x b[1], normalised. */
// FUNCTION: LEGOLAND 0x00429af0
void BuildUprightBasis(struct FVec3 *a, struct FVec3 *b) {
    struct FVec3 *m = b;
    struct FVec3 *v = a;
    int i;

    m[2].x = 0.0f;
    m[2].y = 0.0f;
    m[2].z = 1.0f;
    Vec3Cross(&m[2], v, &m[1]);
    Vec3Cross(v, &m[1], &m[2]);
    m[0] = *v;
    for (i = 0; i < 3; i++) {
        Vec3Normalize(&m[i].x);
    }
}

/* Basis of the track at t (from the centre tangent). */
// FUNCTION: LEGOLAND 0x00429b60
void GetWalkerBasis(struct CurveWalker *a, int b, void *out) {
    int local[3];
    GetWalkerTangent(a, 1, b, local);
    BuildUprightBasis((struct FVec3 *)&local[0], out);
}

/* The track's normal at t. */
// FUNCTION: LEGOLAND 0x00429b90
unsigned int GetWalkerNormal(struct CurveWalker *param_1, unsigned int param_2, unsigned int param_3, float *param_4) {
    struct SegBlob *inner = param_1->node;
    return inner->vt->normal(inner, param_3, param_4);
}

/* World position at t of point pair b, `scale` below the track along its normal. */
// FUNCTION: LEGOLAND 0x00429bb0
void GetWalkerPointBelow(struct CurveWalker *a, int b, int c, float scale, float *d) {
    float v[3];

    GetWalkerPoint(a, b, c, (struct FVec3 *)d);
    GetWalkerNormal(a, b, c, v);
    d[0] = d[0] - v[0] * scale;
    d[1] = d[1] - v[1] * scale;
    d[2] = d[2] - v[2] * scale;
}

struct Out429c60 {
    int type;
    struct FVec3 v;
    unsigned char pad[0x54 - 0x10];
};

/* VecDerivative callback: GetWalkerPointBelow at t. */
// FUNCTION: LEGOLAND 0x00429c10
void WalkerPointBelowFn(float t, struct FloatArray *out) {
    struct FVec3 v;

    /* the integrator's time comes in as a float; its bits are the walker's position (GetWalkerTangentBelow passed an int) */
    GetWalkerPointBelow(RangeWalker, RangeWalkerSegment, *(int *)&t, RangeDepth, (float *)&v);
    out->data[0] = v.x;
    out->data[1] = v.y;
    out->data[2] = v.z;
    out->count = 3;
}

/* Tangent at b of the point c below point pair a: numerically when c is not 0, else the stretch's own tangent. */
// FUNCTION: LEGOLAND 0x00429c60
void GetWalkerTangentBelow(struct CurveWalker *obj, int a, unsigned int b, float c, struct FVec3 *out) {
    struct Out429c60 local;

    if (c != FLOAT_004ab390) {
        RangeWalker = obj;
        RangeWalkerSegment = a;
        RangeDepth = c;
        VecDerivative(WalkerPointBelowFn, &CurveVecMath, *(float *)&b, 0.01f, (struct FloatArray *)&local);
        out->x = local.v.x;
        out->y = local.v.y;
        out->z = local.v.z;
    } else {
        GetWalkerTangent(obj, a, b, out);
    }
}

/* Root function for the range search: squared distance from RangeCentre to the point RangeDepth below the track at
 * x, minus RangeRadius^2 (with quick answers far outside or inside). */
// FUNCTION: LEGOLAND 0x00429cf0
float WalkerRangeFn(unsigned int x) {
    float v[3];
    float w[3];
    float *c;
    float d;

    WalkerRangeCallCount++;
    GetWalkerPoint(RangeWalker, 1, x, (struct FVec3 *)v);
    c = RangeCentre;
    v[0] = v[0] - c[0];
    v[1] = v[1] - c[1];
    v[2] = v[2] - c[2];
    if ((d = v[2] * v[2] + v[1] * v[1] + v[0] * v[0]) > RangeOuterSq) {
        WalkerRangeOutsideCount++;
        return 1.0f;
    }
    if (RangeRadius > RangeDepth && d < RangeInnerSq) {
        return FLOAT_004ab468;
    }
    GetWalkerNormal(RangeWalker, 1, x, w);
    WalkerRangeNormalCount++;
    v[0] = v[0] - RangeDepth * w[0];
    v[1] = v[1] - RangeDepth * w[1];
    v[2] = v[2] - RangeDepth * w[2];
    return v[2] * v[2] + v[1] * v[1] + v[0] * v[0] - RangeRadiusSq;
}

/* Bisection for a sign change of fn in [lo, hi] to 0.005; 0 if fn has the same sign at both ends. */
// FUNCTION: LEGOLAND 0x00429e20
int BisectRoot(float (*fn)(unsigned int), float lo, float hi, float *out) {
    float fa;
    int n;

    BisectRootCallCount++;
    BisectRootStepCount++;
    n = 1;
    fa = fn(*(unsigned int *)&lo);
    {
        float fb = fn(*(unsigned int *)&hi);
        if (!((*(unsigned int *)&fa ^ *(unsigned int *)&fb) & 0x80000000)) {
            return 0;
        }
    }
    while (hi - lo > 0.005f) {
        float mid = (lo + hi) * 0.5f;
        float fb = fn(*(unsigned int *)&mid);
        if ((*(unsigned int *)&fb ^ *(unsigned int *)&fa) & 0x80000000) {
            hi = mid;
        } else {
            lo = mid;
            fa = fb;
        }
        BisectRootStepCount++;
        n++;
    }
    *out = (lo + hi) * 0.5f;
    if (n > BisectRootMaxSteps) {
        BisectRootMaxSteps = n;
    }
    return 1;
}

/* Finds the track position behind src (stretch by stretch backwards) whose point x below the track is r away from
 * center: used to put the rear wheels one car length behind the front ones. */
// FUNCTION: LEGOLAND 0x00429f30
void FindPathPosAtRangeBehind(float *center, float r, struct CurveWalker *src, float first_hi, float x, struct CurveWalker *dst, float *out) {
    struct CurveWalker cur;
    float lo;
    float hi;
    int found;

    cur = *src;
    lo = *(float *)&cur.node->start_bits;
    hi = first_hi;
    RangeRadiusSq = r * r;
    RangeCentre = center;
    RangeRadius = r;
    RangeDepth = x;
    RangeWalker = &cur;
    RangeOuterSq = (r + x) * (r + x);
    RangeInnerSq = (r - x) * (r - x);
    found = RangeRootFinder(WalkerRangeFn, lo, hi, out);
    while (!found) {
        CurveWalkerPrev(&cur);
        lo = *(float *)&cur.node->start_bits;
        hi = *(float *)&cur.node->end_bits;
        found = RangeRootFinder(WalkerRangeFn, lo, hi, out);
    }
    *dst = cur;
}

/* Like FindPathPosAtRangeBehind, searching forwards. */
// FUNCTION: LEGOLAND 0x0042a020
void FindPathPosAtRangeAhead(float *center, float r, struct CurveWalker *src, float first_lo, float x, struct CurveWalker *dst, float *out) {
    struct CurveWalker cur;
    float start;
    float end;
    int found;

    cur = *src;
    start = first_lo;
    end = *(float *)&cur.node->end_bits;
    RangeRadiusSq = r * r;
    RangeCentre = center;
    RangeRadius = r;
    RangeDepth = x;
    RangeWalker = &cur;
    RangeOuterSq = (r + x) * (r + x);
    RangeInnerSq = (r - x) * (r - x);
    found = RangeRootFinder(WalkerRangeFn, start, end, out);
    while (!found) {
        CurveWalkerNext(&cur);
        start = *(float *)&cur.node->start_bits;
        end = *(float *)&cur.node->end_bits;
        found = RangeRootFinder(WalkerRangeFn, start, end, out);
    }
    *dst = cur;
}

/* 1 if both walkers are on the same piece, stretch and position. */
// FUNCTION: LEGOLAND 0x0042a110
int CurveWalkersEqual(struct CurveWalker *p1, struct CurveWalker *p2) {
    if (p1->piece != p2->piece) {
        return 0;
    }
    if (p1->node != p2->node) {
        return 0;
    }
    return Vec3Equal(&p1->pos.x, &p2->pos.x) != 0;
}

/* Integrand of CurveArcLength: length of the tangent at b. */
// FUNCTION: LEGOLAND 0x0042a150
float CurveTangentLength(unsigned int b) {
    float sum = 0.0f;
    float v[3];
    int i;

    GetWalkerTangentBelow(ArcLengthWalker, ArcLengthCurvePair, b, ArcLengthDepth, (struct FVec3 *)v);
    for (i = 0; i < 3; i++) {
        sum += v[i] * v[i];
    }
    return (float)sqrt(sum);
}

/* Arc length along the track from walker b at lo to walker a at hi (Simpson per stretch). */
// FUNCTION: LEGOLAND 0x0042a1b0
float CurveArcLength(struct CurveWalker *a, unsigned int hi, struct CurveWalker *b, unsigned int lo, int idx, float c) {
    struct CurveWalker cur;
    float acc;

    ArcLengthCurvePair = idx;
    ArcLengthDepth = c;
    if (CurveWalkersEqual(a, b)) {
        ArcLengthWalker = a;
        return IntegrateSimpson(CurveTangentLength, lo, hi, 0.01f);
    }
    cur = *b;
    ArcLengthWalker = &cur;
    acc = IntegrateSimpson(CurveTangentLength, lo, cur.node->end_bits, 0.01f);
    CurveWalkerNext(&cur);
    while (!CurveWalkersEqual(&cur, a)) {
        ArcLengthWalker = &cur;
        acc = IntegrateSimpson(CurveTangentLength, cur.node->start_bits, cur.node->end_bits, 0.01f) + acc;
        CurveWalkerNext(&cur);
    }
    ArcLengthWalker = &cur;
    return IntegrateSimpson(CurveTangentLength, cur.node->start_bits, hi, 0.01f) + acc;
}

/* CurveVecMath: vector ops of dimension 3. */
// FUNCTION: LEGOLAND 0x0042a2e0
void InitCurveVecMath(void) {
    InitVecMath(&CurveVecMath, 3);
}

/* Rasterises one projected triangle: clips it to the view when its clip code says so, sets up the edges and calls
 * the span filler table's entry (depth-tested when all its vertices have depth, code 0xf0). */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x0042a2f0
__declspec(naked) void RasteriseTriangle(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x200
        push ebx
        push esi
        push edi
        mov edi, dword ptr [ebp + 0xc]
        mov ecx, 3
        mov dword ptr [ebp - 0x14], 0
        mov edx, dword ptr [edi + 0xc]
        lea esi, [edi + 0x28]
        and edx, 0xf0
        mov eax, esi
        cmp dl, 0xf0
        mov dword ptr [ebp - 4], ecx
        mov dword ptr [ebp - 0x40], 0x47800000
        lea ebx, [ebp - 0x80]
        mov dword ptr [ebp - 0x18], eax
        je L42a33e
        mov edx, dword ptr [ebp + 8]
        mov dword ptr [ebp - 0x20], 0
        dec edx
        mov dword ptr [ebp + 8], edx
        jmp L42a345
L42a33e:
        mov dword ptr [ebp - 0x20], 1
L42a345:
        mov edx, dword ptr [edi + 8]
        and edx, 0xf
        cmp edx, 0xf
        je L42a377
        mov ecx, dword ptr [ebp + 8]
        mov dword ptr [ebp - 4], 0
        push ecx
        push edx
        lea edx, [ebp - 4]
        push edx
        push eax
        call ClipTriangle
        mov ecx, dword ptr [ebp - 4]
        add esp, 0x10
        test ecx, ecx
        mov dword ptr [ebp - 0x18], eax
        je L42a5d0
L42a377:
        mov edx, dword ptr [eax]
        mov dword ptr [eax + ecx*4], edx
        mov edx, dword ptr [ebp + 8]
        mov ecx, dword ptr [edi + 0x10]
        cmp edx, 1
        mov dword ptr [ebp - 0x34], ecx
        jle L42a409
        fld dword ptr [FLOAT_004ab49c]
        fdiv dword ptr [edi + 0x24]
        mov ecx, 0xc
        lea eax, [ebp - 0x30]
        sub eax, ecx
        dec edx
        mov dword ptr [ebp - 0xc], eax
        mov dword ptr [ebp - 8], edx
        fstp dword ptr [ebp - 0x1c]
L42a3a7:
        mov edx, dword ptr [esi]
        mov eax, dword ptr [esi + 4]
        mov edx, dword ptr [edx + ecx]
        mov eax, dword ptr [ecx + eax]
        sub eax, edx
        mov dword ptr [ebp - 0x3c], eax
        mov eax, dword ptr [esi + 8]
        fild dword ptr [ebp - 0x3c]
        mov eax, dword ptr [ecx + eax]
        sub eax, edx
        fmul dword ptr [edi + 0x20]
        mov dword ptr [ebp - 0x38], eax
        fild dword ptr [ebp - 0x38]
        fmul dword ptr [edi + 0x1c]
        fsubp st(1), st
        fmul dword ptr [ebp - 0x1c]
        fstp dword ptr [ebp - 0x24]
        fld dword ptr [ebp - 0x24]
        fistp dword ptr [ebp - 0x10]
        mov eax, dword ptr [ebp - 0x10]
        mov edx, dword ptr [ebp - 0xc]
        cmp eax, 0xffe00000
        mov dword ptr [edx + ecx], eax
        jl L42a3f3
        cmp eax, 0x200000
        jle L42a3fa
L42a3f3:
        mov dword ptr [ebp - 0x10], 0
L42a3fa:
        mov eax, dword ptr [ebp - 8]
        add ecx, 4
        dec eax
        mov dword ptr [ebp - 8], eax
        jne L42a3a7
        mov eax, dword ptr [ebp - 0x18]
L42a409:
        mov ecx, dword ptr [ebp - 4]
        mov dword ptr [ebp - 0x1c], 0
        test ecx, ecx
        jle L42a5d0
        lea edx, [ebp - 0x200]
        mov dword ptr [ebp - 0x10], eax
        jmp L42a429
L42a426:
        mov eax, dword ptr [ebp - 0x10]
L42a429:
        mov esi, dword ptr [eax]
        mov ecx, dword ptr [eax + 4]
        mov edi, dword ptr [ecx + 4]
        mov eax, dword ptr [esi + 4]
        sub edi, eax
        mov dword ptr [ebp - 0xc], edi
        je L42a518
        fild dword ptr [ebp - 0xc]
        mov eax, edx
        fstp dword ptr [ebp - 0xc]
        fld dword ptr [ebp - 0x40]
        fdiv dword ptr [ebp - 0xc]
        test edi, edi
        mov edi, dword ptr [ebp - 0x14]
        mov dword ptr [ebx + 4], edi
        jge L42a4cf
        inc edi
        add edx, 0x30
        mov dword ptr [ebp - 0x14], edi
        mov edi, dword ptr [ecx + 4]
        mov dword ptr [ebx], edi
        mov dword ptr [eax + 4], 0
        mov di, word ptr [ecx + 4]
        mov dword ptr [ebp - 0xc], edx
        mov word ptr [eax], di
        mov di, word ptr [esi + 4]
        add ebx, 8
        mov word ptr [eax + 2], di
        fstp dword ptr [ebp - 0x24]
        mov edi, dword ptr [ebp + 8]
        test edi, edi
        jle L42a518
        lea edi, [eax + 0x1c]
        sub eax, ecx
        lea edx, [ecx + 8]
        sub esi, ecx
        mov ecx, dword ptr [ebp + 8]
        mov dword ptr [ebp - 0x18], ecx
L42a49d:
        mov ecx, dword ptr [edx]
        shl ecx, 0x10
        mov dword ptr [eax + edx], ecx
        mov ecx, dword ptr [edx]
        sub ecx, dword ptr [esi + edx]
        mov dword ptr [ebp - 8], ecx
        fild dword ptr [ebp - 8]
        fmul dword ptr [ebp - 0x24]
        fistp dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 8]
        add edx, 4
        mov dword ptr [edi], ecx
        mov ecx, dword ptr [ebp - 0x18]
        add edi, 4
        dec ecx
        mov dword ptr [ebp - 0x18], ecx
        jne L42a49d
        mov edx, dword ptr [ebp - 0xc]
        jmp L42a518
L42a4cf:
        inc edi
        add edx, 0x30
        mov dword ptr [ebp - 0x14], edi
        mov edi, dword ptr [esi + 4]
        mov dword ptr [ebx], edi
        mov dword ptr [eax + 4], 1
        mov di, word ptr [esi + 4]
        add ebx, 8
        mov word ptr [eax], di
        mov di, word ptr [ecx + 4]
        mov word ptr [eax + 2], di
        fstp dword ptr [ebp - 0x38]
        mov edi, dword ptr [esi + 8]
        shl edi, 0x10
        mov dword ptr [eax + 8], edi
        mov ecx, dword ptr [ecx + 8]
        sub ecx, dword ptr [esi + 8]
        mov dword ptr [ebp - 8], ecx
        fild dword ptr [ebp - 8]
        fmul dword ptr [ebp - 0x38]
        fistp dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 8]
        mov dword ptr [eax + 0x1c], ecx
L42a518:
        mov eax, dword ptr [ebp - 0x1c]
        mov esi, dword ptr [ebp - 0x10]
        mov ecx, dword ptr [ebp - 4]
        inc eax
        add esi, 4
        cmp eax, ecx
        mov dword ptr [ebp - 0x1c], eax
        mov dword ptr [ebp - 0x10], esi
        jl L42a426
        mov ebx, dword ptr [ebp - 0x14]
        test ebx, ebx
        je L42a5d0
        lea edi, [ebx - 1]
        test edi, edi
        jl L42a578
L42a545:
        test edi, edi
        jle L42a575
        lea eax, [ebp - 0x78]
        mov esi, edi
L42a54e:
        mov edx, dword ptr [eax - 8]
        mov ecx, dword ptr [eax]
        cmp edx, ecx
        jle L42a56c
        mov ebx, dword ptr [eax]
        mov ecx, edx
        mov edx, dword ptr [eax - 4]
        mov dword ptr [eax - 8], ebx
        mov ebx, dword ptr [eax + 4]
        mov dword ptr [eax - 4], ebx
        mov dword ptr [eax], ecx
        mov dword ptr [eax + 4], edx
L42a56c:
        add eax, 8
        dec esi
        jne L42a54e
        mov ebx, dword ptr [ebp - 0x14]
L42a575:
        dec edi
        jns L42a545
L42a578:
        mov esi, dword ptr [ebp + 0xc]
        test byte ptr [esi], 1
        je L42a5b0
        cmp dword ptr [ebp - 0x20], 1
        jne L42a5b0
        lea eax, [ebp - 0x200]
        lea ecx, [ebp - 0x80]
        push eax
        mov eax, dword ptr [ebp + ebx*8 - 0x84]
        push ecx
        lea edx, [eax + eax*2]
        shl edx, 4
        movsx eax, word ptr [ebp + edx - 0x1fe]
        push eax
        push ebx
        call RecordDepthSpans
        add esp, 0x10
L42a5b0:
        mov ecx, dword ptr [esi + 0x38]
        lea edx, [ebp - 0x200]
        lea eax, [ebp - 0x80]
        push edx
        push eax
        mov eax, dword ptr [esi + 4]
        lea edx, [ebp - 0x34]
        push ebx
        push edx
        mov edx, dword ptr [ebp - 0x20]
        push eax
        call dword ptr [ecx + edx*4]
        add esp, 0x14
L42a5d0:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

static __inline void Init42a5e0(struct TrainWheel *id, struct CurveWalker *src, float v) {
    id->walker = *src;
    id->t = v;
}

/* Puts both wheel pairs of a bogie at t on the walker's stretch. */
// FUNCTION: LEGOLAND 0x0042a5e0
void InitTrainBogie(struct TrainBogie *o, struct CurveWalker *src, float param_5) {
    Init42a5e0(&o->w0, src, param_5);
    Init42a5e0(&o->w1, src, param_5);
}

/* Sets the wheel pair's walker and parameter. */
// FUNCTION: LEGOLAND 0x0042a620
void SetTrainWheelPos(struct TrainWheel *param_1, struct CurveWalker *param_2, unsigned int param_5) {
    memcpy(&param_1->walker, param_2, sizeof(struct CurveWalker));
    *(unsigned int *)&param_1->t = param_5;
}

/* World position of wheel a (0 side A, 1 side B, 2 between them), 4.8 below the track. */
// FUNCTION: LEGOLAND 0x0042a640
unsigned int GetTrainWheelPos(struct TrainWheel *ptr, unsigned int a, float *b) {
    GetWalkerPointBelow(&ptr->walker, WheelCurvePairs[a], *(int *)&ptr->t, 4.8f, b);
}

/* Wheel spin per frame: always 0. */
// FUNCTION: LEGOLAND 0x0042a670
float GetWheelSpinStep(struct TrainBogie *p, int i) {
    return FLOAT_004ab390;
}

/* Draws a bogie's two wheels on the track with the model lighting. */
// FUNCTION: LEGOLAND 0x0042a680
void DrawTrainBogie(struct TrainBogie *p) {
    struct TrainBogie *o = p;
    float E[4][4];
    unsigned int B[4][4];
    unsigned int A[9];
    unsigned int G[9];
    float D[4][4];
    float C[3];
    int i;

    SetModelLighting();
    GetWalkerBasis(&o->w0.walker, *(int *)&o->w0.t, A);
    Mat3ToMat4Transposed(A, B);
    for (i = 0; i <= 1; i++) {
        o->ang[i] = GetWheelSpinStep(p, i) + o->ang[i];
        GetTrainWheelPos(&p->w0, i, C);
        Mat4Identity(D);
        D[0][0] = (float)sin(o->ang[i]);
        D[2][0] = -(D[0][2] = (float)cos(o->ang[i]));
        D[2][2] = (float)sin(o->ang[i]);
        Mat4Multiply((float (*)[4])B, D, E);
        Mat4ToMat3Transpose(G, E);
        DrawLmsModel(CoasterTrainWheelLms, CoasterTrainWheelLfm, C, G, 0);
    }
    o->w1.t = o->w0.t;
    o->w1.walker = o->w0.walker;
}

/* Looks up the wheel model (coastertrain.wheel01). */
// FUNCTION: LEGOLAND 0x0042a780
void LoadCoasterTrainWheelModel(void) {
    // STRING: LEGOLAND 0x004b6414
    const char *str = "coastertrain.wheel01";
    CoasterTrainWheelLms = GetLmsByName(str);
    CoasterTrainWheelLfm = GetLfmByName(str);
}
