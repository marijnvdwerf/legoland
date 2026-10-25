#include <math.h>
#include <stdlib.h>
#include "controller.h"
#include "globals.h"
#include "legoland.h"

#include <string.h>

#include "bloke.h"
#include "boating_school.h"
#include "debug_alloc.h"
#include "gamemap.h"
#include "image_sprite.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "objclass.h"
#include "print_sprite.h"
#include "render3d.h"
#include "screens.h"
#include "sound_music.h"
#include "sound_sfx.h"
#include "tilemap.h"

// FUNCTION: LEGOLAND 0x00418e60
int AddBoatRide(TileId tile, struct Bloke *bloke) {
    struct BoatRideNode *score;
    struct BoatRide *node = BoatRideList;
    struct BoatRide *fresh;

    for (score = BoatRideNodeList; score != NULL; score = score->next) {
        if (score->id == tile.id) {
            break;
        }
    }
    for (; node != NULL; node = node->next) {
        if (node->id == tile.id) {
            if (node->tile_x == score->start.pos.x && node->tile_y == score->start.pos.y) {
                return 0;
            }
            if (node->next_x == score->start.pos.x && node->next_y == score->start.pos.y) {
                return 0;
            }
            if (node->field_3e4 == 1) {
                return 0;
            }
        }
    }
    fresh = (struct BoatRide *)malloc(sizeof(struct BoatRide));
    if (fresh == NULL) {
        return 0;
    }
    fresh->next = BoatRideList;
    fresh->id = tile.id;
    fresh->tile_x = tile.pos.x - 1;
    fresh->tile_y = tile.pos.y + 5;
    fresh->next_x = tile.pos.x - 1;
    fresh->next_y = tile.pos.y + 5;
    fresh->field_3dc = 1;
    fresh->field_3e0 = rand() & 3;
    fresh->field_3e4 = 1;
    fresh->field_3e8 = (rand() & 0xf) + 4;
    fresh->bloke = bloke;
    BoatRideList = fresh;
    memset(fresh->step_xy, 0xf1, sizeof(fresh->step_xy));
    memset(fresh->step_sprite, 0, sizeof(fresh->step_sprite));
    if (fresh->field_3e0 == 3) {
        fresh->field_3e0 = 2;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00418f90
void RemoveBoatRide(struct BoatRide *param_1) {
    struct BoatRide *prev = NULL;
    struct BoatRide *node = BoatRideList;
    while (node != param_1) {
        prev = node;
        node = node->next;
        if (node == NULL) {
            return;
        }
    }
    if (node != NULL) {
        if (prev != NULL) {
            prev->next = node->next;
        } else {
            BoatRideList = node->next;
        }
        free(param_1);
    }
}

/* Draws the boating school boats (param_1 nonzero: boats at the dock, states 1 and 0x10; zero: boats out on the
 * water) with their riders turned to the boat heading, and stores each boat's screen position. At animation tick
 * 0x4f of the second docking step it hands the rider back to its AI (param_action + 1). */
// FUNCTION: LEGOLAND 0x00418fe0
void RenderBoatingSchoolBoats(int param_1) {
    struct BoatRide *ride;
    int tw;
    int th;
    int tw2;
    int dx;
    int th2;
    int dy; /* the step's y, then reused for the tile's screen x: one variable in the original */
    struct Point b;
    int sy;
    struct Person *person;
    struct Point off;
    struct Point seat;
    ride = BoatRideList;

    GetTileDimensions(&tw, &th);
    for (; ride != NULL; ride = ride->next) {
        if ((param_1 != 0 && (ride->field_3e4 == 1 || ride->field_3e4 == 0x10)) ||
            (param_1 == 0 && ride->field_3e4 != 1 && ride->field_3e4 != 0x10)) {
            dy = ride->step_xy[BoatingSchoolAnimTick * 2 + 1];
            dx = ride->step_xy[BoatingSchoolAnimTick * 2];
            GetTileDimensions(&tw2, &th2);
            b.x = (dx - dy) * tw2;
            b.y = (dx + dy) * th2;
            b.x >>= 9;
            b.y >>= 9;
            dy = (ride->tile_x - ride->tile_y) * (tw >> 1) - ((tw + 1) >> 1) - (ScrollX >> 8);
            sy = (ride->tile_x + ride->tile_y) * (th >> 1) - (ScrollY >> 8);
            off.x = BoatingSchoolBoats->offset_x[ride->step_sprite[BoatingSchoolAnimTick] & 0xff] >> 1;
            off.y = BoatingSchoolBoats->offset_y[ride->step_sprite[BoatingSchoolAnimTick] & 0xff] >> 1;
            AdjustOffsetForViewMode(&off);
            ride->screen_x = lpConfig->view_x + b.x + off.x + dy;
            ride->screen_y = lpConfig->view_y + b.y + off.y + sy;
            PrintSprite(BoatingSchoolBoats->sprites[ride->step_sprite[BoatingSchoolAnimTick] & 0xff], ride->screen_x, ride->screen_y, 0, 0);
            if (ride->bloke != 0) {
                person = Find3DPersonFromBloke(ride->bloke);
                person->field_44 = ((float)(int)ride->step_sprite[BoatingSchoolAnimTick] * BoatHeadingStepDegrees + Float45) * Float2Pi * FloatNegInv360;
                SetPersonRotation(person, &person->field_40);
                off.x = lpConfig->view_x + b.x + dy;
                off.y = lpConfig->view_y + b.y + sy;
                AdjustBlokePosition((struct Point *)&off);
                seat.x = BoatingSchoolRiderOffsets[(ride->step_sprite[BoatingSchoolAnimTick] & 0xf) * 2] + 0x44;
                seat.y = BoatingSchoolRiderOffsets[(ride->step_sprite[BoatingSchoolAnimTick] & 0xf) * 2 + 1] + 0x34;
                AdjustOffsetForViewMode(&seat);
                person->screen.x = seat.x + off.x;
                person->screen.y = seat.y + off.y;
                IP_RenderBlokeIn3DNow(ride->bloke);
                PrintSprite(BoatingSchoolBoats->sprites[(ride->step_sprite[BoatingSchoolAnimTick] + 0x30) & 0xff], ride->screen_x, ride->screen_y, 0, 0);
            }
        }
        if (ride->field_3e4 == 0x10 && ride->field_3e8 == 2 && BoatingSchoolAnimTick == 0x4f && param_1 != 0) {
            if (ride->bloke != 0) {
                (ride->bloke)->param_action++;
                ride->bloke = 0;
            }
        }
    }
}

/* Stamps every boating school boat with the id at param_1 and returns the number of boats when that id is nonzero,
 * else 0 (BoatingSchoolUpdate compares six visitors per boat against it). 100% match, so the original assigned here
 * as well. */
/* reads the id its argument starts with */
// FUNCTION: LEGOLAND 0x004192d0
unsigned int TagAndCountBoatingSchoolBoats(void *param_1) {
    unsigned short *id = (unsigned short *)param_1;
    struct BoatRide *node = BoatRideList;
    unsigned int count = 0;
    while (node != NULL) {
        unsigned short value = *id;
        node->id = value;
        if (value != 0) {
            count++;
        }
        node = node->next;
    }
    return count;
}

// FUNCTION: LEGOLAND 0x00419300
void UpdateBoatRides(void) {
    struct BoatRide *node = BoatRideList;
    struct BoatRide *cur;

    while (node != NULL) {
        node->tile_x = node->next_x;
        node->tile_y = node->next_y;
        if (node->field_3e4 == 0x10) {
            cur = node;
            node = DockBoatingSchoolBoat(node);
            if (node != cur) {
                continue;
            }
        } else {
            switch (node->field_3e4) {
            case 1:
                LaunchBoatingSchoolBoat(node);
                break;
            case 4:
                SteerBoatingSchoolBoat(node, 0);
                break;
            case 8:
                SteerBoatingSchoolBoat(node, 1);
                break;
            case 0x10:
                node = DockBoatingSchoolBoat(node);
                break;
            }
        }
        if (node == NULL) {
            return;
        }
        node = node->next;
    }
}

/* Boat state 1: leaves the dock heading south (+5 tiles in y), switches to sailing state 4 and clears the counters
 * of its boating school node. */
// FUNCTION: LEGOLAND 0x004193c0
void LaunchBoatingSchoolBoat(struct BoatRide *param_1) {
    struct BoatRideNode *node = BoatRideNodeList;
    BoatingSchoolBuildStepPath(param_1, param_1->field_3dc, 4);
    param_1->field_3e4 = 4;
    param_1->next_y = param_1->tile_y + 5;
    for (; node != NULL; node = node->next) {
        if (node->id == param_1->id) {
            node->field_c = 0;
            node->field_10 = 0;
            return;
        }
    }
}

/* Boat state 0x10, one docking step per call (countdown field_3e8 3, 2, 1): rebuilds the step path into the dock.
 * When the countdown is done it fades the rider's sounds and removes the boat, returning the next one. */
// FUNCTION: LEGOLAND 0x00419420
struct BoatRide *DockBoatingSchoolBoat(struct BoatRide *param_1) {
    int *p;
    int i;
    struct BoatRide *result;
    struct SampleSource source;

    if (param_1->field_3e8 == 0) {
        result = param_1->next;
        source.type = 1;
        source.bloke = param_1->bloke;
        UnSourceAndFadeAllSamplesFromSource(&source, -0x5a);
        RemoveBoatRide(param_1);
        return result;
    }
    BoatingSchoolBuildStepPath(param_1, 1, 4);
    param_1->field_3dc = 1;
    if (param_1->field_3e8 == 3) {
        i = 0x10;
        p = &param_1->step_xy[0x81];
        do {
            p[-1] = param_1->step_xy[0x80];
            *p = param_1->step_xy[0x81];
            p = p + 2;
            i = i - 1;
        } while (i != 0);
    } else if (param_1->field_3e8 == 2) {
        p = &param_1->step_xy[1];
        i = 0x40;
        do {
            p[-1] = param_1->step_xy[0x80];
            *p = param_1->step_xy[0x81];
            p = p + 2;
            i = i - 1;
        } while (i != 0);
    }
    if (param_1->field_3e8 != 3) {
        param_1->next_y = param_1->tile_y + 5;
    }
    if (param_1->field_3e8 == 1) {
        p = &param_1->step_xy[0x9e];
        i = 7;
        do {
            p[0] = param_1->step_xy[0x90];
            p[1] = param_1->step_xy[0x91];
            p = p + -2;
            i = i - 1;
        } while (i != 0);
    }
    param_1->field_3e8 = param_1->field_3e8 - 1;
    return param_1;
}

/* Picks the next 5-tile move of a sailing boat from the path tile's direction mask, avoiding other boats and
 * sometimes turning at random. param_2 (return state 8) restricts it to the way back. At the path end it starts
 * docking (state 0x10); with no exit it turns round. When its sailing countdown runs out it switches to state 8. */
// FUNCTION: LEGOLAND 0x00419520
void SteerBoatingSchoolBoat(struct BoatRide *ride, int param_2) {
    struct BoatRideNode *score = BoatRideNodeList;
    struct BoatRide *other = BoatRideList;
    struct PathNode *path;
    unsigned int mask;
    unsigned int free;
    struct Point d;
    int i;
    int n;
    int back;
    int step;
    int dir;

    path = FindBoatPathAt(ride->tile_x, ride->tile_y);
    for (; score != NULL; score = score->next) {
        if (path->owner.id == score->id) {
            break;
        }
    }
    mask = path->dir_mask;
    if (path->tile.id == score->start.id) {
        mask &= ~1;
    } else if (path->tile.id == score->end.id) {
        ride->field_3e4 = 0x10;
        ride->field_3e8 = 3;
        BoatingSchoolBuildStepPath(ride, ride->field_3dc, 4);
        ride->field_3dc = 1;
        ride->next_y = ride->tile_y + 5;
        return;
    }
    for (; other != NULL; other = other->next) {
        if (other == ride) {
            continue;
        }
        if ((ride->tile_x == other->tile_x && ride->tile_y - 5 == other->tile_y) || (ride->tile_x == other->next_x && ride->tile_y - 5 == other->next_y)) {
            mask &= ~1;
        }
        if ((ride->tile_x + 5 == other->tile_x && ride->tile_y == other->tile_y) || (ride->tile_x + 5 == other->next_x && ride->tile_y == other->next_y)) {
            mask &= ~2;
        }
        if ((ride->tile_x == other->tile_x && ride->tile_y + 5 == other->tile_y) || (ride->tile_x == other->next_x && ride->tile_y + 5 == other->next_y)) {
            mask &= ~4;
        }
        if ((ride->tile_x - 5 == other->tile_x && ride->tile_y == other->tile_y) || (ride->tile_x - 5 == other->next_x && ride->tile_y == other->next_y)) {
            mask &= ~8;
        }
    }
    if (param_2 != 0 && path->parent != NULL) {
        d.x = path->parent->tile.pos.x - path->tile.pos.x;
        d.y = path->parent->tile.pos.y - path->tile.pos.y;
        if (d.y != 0) {
            if (d.x < 0) {
                mask &= ~8;
            } else {
                mask &= ~2;
            }
        } else {
            mask &= ~4;
        }
    }
    if (mask == 0) {
        BoatingSchoolBuildStepPath(ride, ride->field_3dc, -1);
        ride->field_3dc = -1;
        return;
    }
    if ((rand() & 7) == 0 && (free = ~ride->field_3dc & mask) != 0) {
        for (;;) {
            for (i = 0, n = 0; i < 4; i++) {
                if ((free & (1 << i)) != 0) {
                    n++;
                }
            }
            if (n <= 1) {
                break;
            }
            free &= ~(1 << (rand() & 3));
        }
        mask = free;
    }
    for (i = 0; i < 4; i++) {
        if ((ride->field_3dc & (1 << i)) != 0) {
            break;
        }
    }
    back = (i + 2) % 4;
    dir = 1 << back;
    if ((mask & dir) == 0) {
        step = (rand() & 1) ? 1 : -1;
        dir = 1 << ((step + back) & 3);
        if ((mask & dir) == 0) {
            dir = 1 << ((back - step) & 3);
            if ((mask & dir) == 0) {
                dir = 1 << ((back + 2) % 4);
            }
        }
    }
    switch (dir) {
    case 1:
        ride->next_x = ride->tile_x;
        ride->next_y = ride->tile_y - 5;
        BoatingSchoolBuildStepPath(ride, ride->field_3dc, dir);
        ride->field_3dc = 4;
        break;
    case 2:
        ride->next_x = ride->tile_x + 5;
        ride->next_y = ride->tile_y;
        BoatingSchoolBuildStepPath(ride, ride->field_3dc, dir);
        ride->field_3dc = 8;
        break;
    case 4:
        ride->next_x = ride->tile_x;
        ride->next_y = ride->tile_y + 5;
        BoatingSchoolBuildStepPath(ride, ride->field_3dc, dir);
        ride->field_3dc = 1;
        break;
    case 8:
        ride->next_x = ride->tile_x - 5;
        ride->next_y = ride->tile_y;
        BoatingSchoolBuildStepPath(ride, ride->field_3dc, dir);
        ride->field_3dc = 2;
        break;
    }
    if (--ride->field_3e8 == 0) {
        ride->field_3e4 = 8;
    }
}

// FUNCTION: LEGOLAND 0x004198a0
void BoatingSchoolBuildStepPath(struct BoatRide *ride, int from, int to) {
    struct BoatArc *arc = NULL;
    int *p;
    int i;
    int bit;
    int idx;
    int sy;
    int sx;
    volatile int tx;
    int ty;
    unsigned int dx;
    int dy;
    float fx;
    float fy;
    float angle;
    float step;

    if (to == -1) {
        if (from == -1) {
            memset(ride->step_xy, 0, sizeof(ride->step_xy));
        } else {
            for (bit = 0; bit < 4; bit++) {
                if ((from & (1 << bit)) != 0) {
                    break;
                }
            }
            sx = (int)((float)(BoatingSchoolDirSteps[bit].ox * 40) * Float16);
            sy = (int)((float)(BoatingSchoolDirSteps[bit].oy * 40) * Float16);
            for (i = 0; i < 80; i++) {
                if (i < 40) {
                    ride->step_xy[i * 2] = (int)((float)(BoatingSchoolDirSteps[bit].dx * i) * Float16 + sx);
                    ride->step_xy[i * 2 + 1] = (int)((float)(BoatingSchoolDirSteps[bit].dy * i) * Float16 + sy);
                } else {
                    ride->step_xy[i * 2] = 0;
                    ride->step_xy[i * 2 + 1] = 0;
                }
            }
        }
    } else if (from == -1) {
        for (bit = 0; bit < 4; bit++) {
            if ((to & (1 << bit)) != 0) {
                break;
            }
        }
        idx = (bit + 2) % 4;
        for (i = 0; i < 80; i++) {
            if (i >= 40) {
                ride->step_xy[i * 2] = BoatingSchoolDirSteps[idx].dx * 16 + ride->step_xy[i * 2 - 2];
                ride->step_xy[i * 2 + 1] = BoatingSchoolDirSteps[idx].dy * 16 + ride->step_xy[i * 2 - 1];
            } else {
                ride->step_xy[i * 2] = 0;
                ride->step_xy[i * 2 + 1] = 0;
            }
        }
    } else {
        if (from == 1) {
            from = 0x11;
        }
        if (to == 1) {
            to = 0x11;
        }
        if (((unsigned)to < from ? to & (from >> 2) : to == from || from & (to >> 2)) == 0) {
            if ((to & (2 * from)) != 0) {
                arc = BoatingSchoolArcsAlt;
            } else if ((from & (to * 2)) != 0) {
                arc = BoatingSchoolArcs;
            }
            for (bit = 0; bit < 4; bit++) {
                if ((from & (1 << bit)) != 0) {
                    break;
                }
            }
            arc = arc + bit;
            step = (arc->a1 - arc->a0) * BoatArcStepScale;
            angle = arc->a0;
            ride->step_xy[0] = (int)((sin(angle * DegToRad) + arc->cx) * BoatArcScale);
            ride->step_xy[1] = (int)((cos((angle + Float180) * DegToRad) + arc->cy) * BoatArcScale);
            p = &ride->step_xy[3];
            for (i = 0x4f; i != 0; i--) {
                angle += step;
                p[-1] = (int)((sin(angle * DegToRad) + arc->cx) * BoatArcScale);
                p[0] = (int)((cos((angle + Float180) * DegToRad) + arc->cy) * BoatArcScale);
                p += 2;
            }
        } else {
            for (bit = 0; bit < 4; bit++) {
                if ((from & (1 << bit)) != 0) {
                    break;
                }
            }
            sx = (int)((float)(BoatingSchoolDirSteps[bit].ox * 40) * Float16);
            sy = (int)((float)(BoatingSchoolDirSteps[bit].oy * 40) * Float16);
            if (from == to) {
                for (i = 0; i < 80; i++) {
                    if (i < 40) {
                        ride->step_xy[i * 2] = (int)((float)(BoatingSchoolDirSteps[bit].dx * i) * Float16 + sx);
                        ride->step_xy[i * 2 + 1] = (int)((float)(BoatingSchoolDirSteps[bit].dy * i) * Float16 + sy);
                    } else {
                        ride->step_xy[i * 2] = (int)((float)(BoatingSchoolDirSteps[bit].dx * (80 - i)) * Float16 + sx);
                        ride->step_xy[i * 2 + 1] = (int)((float)(BoatingSchoolDirSteps[bit].dy * (80 - i)) * Float16 + sy);
                    }
                }
            } else {
                fx = (float)sx;
                fy = (float)sy;
                for (i = 0; i < 80; i++) {
                    ride->step_xy[i * 2] = (int)((float)(BoatingSchoolDirSteps[bit].dx * i) * Float16 + fx);
                    ride->step_xy[i * 2 + 1] = (int)((float)(i * BoatingSchoolDirSteps[bit].dy) * Float16 + fy);
                }
            }
        }
    }
    for (i = 0; i < 80; i++) {
        if (!(i < 76)) {
            ty = ride->step_xy[0x9f];
            tx = ride->step_xy[0x9e];
        } else {
            ty = ride->step_xy[(i + 4) * 2 + 1];
            tx = ride->step_xy[(i + 4) * 2];
        }
        if (i > 3) {
            dx = tx - ride->step_xy[(i - 3) * 2];
            dy = ty - ride->step_xy[(i - 3) * 2 + 1];
        } else {
            dx = tx - ride->step_xy[0];
            dy = ty - ride->step_xy[1];
        }
        ride->step_sprite[i] = ((ArcTan256(dx, dy) >> 4) + 6 & 0xf) + ride->field_3e0 * 16;
    }
}

// FUNCTION: LEGOLAND 0x00419d10
void LoadBoatingSchoolResources(Element *obj) {
    struct Element *handle;
    int i;
    struct Sprite *sprite;
    struct LLS *lls;

    Load_FXList(PTR_s_Boat_Noise_wav, 2);
    BoatingSchoolRide = obj->ride;
    BoatingSchoolRide->flags |= 0x20;
    BoatingSchoolRide->layer->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b5334
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", &handle, 0) == 0) {
        BoatingSchoolTileMapping = (struct TileMap *)LLIDB_LoadData((void *)handle);
    }
    // STRING: LEGOLAND 0x004b531c
    if (LLIDB_FindElement("BOATING SCHOOL BOATS", &handle, 0) == 0) {
        BoatingSchoolBoats = (struct SpriteSet *)LLIDB_LoadData((void *)handle);
    }
    for (i = 0; i < BoatingSchoolBoats->count; i++) {
        sprite = BoatingSchoolBoats->sprites[i & 0xff];
        LLSPlay(GetLLSForSprite((struct Sprite *)sprite), ((struct Sprite *)sprite)->image);
    }
    // STRING: LEGOLAND 0x004b530c
    BoatingSchoolHullMaskSprite = LoadSprite("bs_hullmask.lls", 1);
    // STRING: LEGOLAND 0x004b52fc
    BoatingSchoolRailmSprite = LoadSprite("bs_railm.lls", 1);
    lls = GetLLSForSprite((struct Sprite *)(BoatingSchoolLayer5Sprite = (void *)GetSpriteForLayer((struct Sprite *)BoatingSchoolRide->layer, 5)));
    LLSStop(lls);
    LLSSetFrame(lls, lls->frame_count);
    BoatingSchoolFootprint = BoatingSchoolRide->footprint;
    BoatingSchoolExtraFootprint = BoatingSchoolExtraFootprintTemplate;
    BoatingSchoolExtraFootprint.v[1] += BoatingSchoolFootprint.v[1];
    BoatingSchoolExtraFootprint.v[0] += BoatingSchoolFootprint.v[0];
    BoatingSchoolExtraFootprint.v[2] += BoatingSchoolFootprint.v[0];
    BoatingSchoolExtraFootprint.v[3] += BoatingSchoolFootprint.v[1];
    BoatingSchoolStartFootprint = BoatingSchoolStartFootprintTemplate;
    BoatingSchoolStartFootprint.v[1] += BoatingSchoolFootprint.v[3] + 1;
    BoatingSchoolStartFootprint.v[0] += BoatingSchoolFootprint.v[0];
    BoatingSchoolStartFootprint.v[2] += BoatingSchoolFootprint.v[0];
    BoatingSchoolStartFootprint.v[3] += BoatingSchoolFootprint.v[3] + 1;
}

// FUNCTION: LEGOLAND 0x00419ef0
void UnloadBoatingSchoolResources(void) {
    struct Element *handle;
    int i;
    struct Sprite *sprite;
    struct PathNode *path;

    Kill_FXList(PTR_s_Boat_Noise_wav, 2);
    for (i = 0; i < BoatingSchoolBoats->count; i++) {
        sprite = BoatingSchoolBoats->sprites[(unsigned char)i];
        LLSStop(GetLLSForSprite((struct Sprite *)sprite));
    }
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", &handle, 0) == 0) {
        LLIDB_UnLoadData(handle);
    }
    if (LLIDB_FindElement("BOATING SCHOOL BOATS", &handle, 0) == 0) {
        LLIDB_UnLoadData(handle);
    }
    while (BoatRideNodeList != NULL) {
        struct BoatRideNode *next = BoatRideNodeList->next;
        free(BoatRideNodeList);
        BoatRideNodeList = next;
    }
    while (BoatRideList != NULL) {
        RemoveBoatRide(BoatRideList);
    }
    while (BoatPathList != NULL) {
        path = BoatPathList->next;
        free(BoatPathList);
        BoatPathList = path;
    }
    KillSprite(BoatingSchoolHullMaskSprite);
    KillSprite(BoatingSchoolRailmSprite);
}

// FUNCTION: LEGOLAND 0x0041a000
void SetBoatingSchoolEditCursor(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = BoatingSchoolRide;
    DefaultCursor(&EditCursor);
    BoatingSchoolFootprint.next = &BoatingSchoolStartFootprint;
    BoatingSchoolStartFootprint.next = &BoatingSchoolExtraFootprint;
    SetEditCursorFootPrint(BoatingSchoolFootprint.v);
}

// FUNCTION: LEGOLAND 0x0041a040
void BoatingSchoolAddObject(struct Element *obj, int *coords) {
    TileId tile;
    struct BoatRideNode *score;
    int x;
    int y;

    tile.pos.x = coords[0];
    tile.pos.y = coords[1];
    score = (struct BoatRideNode *)malloc(sizeof(struct BoatRideNode));
    if (score == NULL) {
        return;
    }
    score->id = tile.id;
    score->start.pos.x = coords[0] + BoatingSchoolStartFootprint.v[0] + 2;
    score->start.pos.y = coords[1] + BoatingSchoolStartFootprint.v[1] + 2;
    score->end.pos.x = coords[0] + BoatingSchoolExtraFootprint.v[0] + 2;
    score->end.pos.y = coords[1] + BoatingSchoolExtraFootprint.v[1] + 2;
    score->connected = 0;
    score->field_c = 9999;
    score->field_10 = 0;
    score->bloke_count = 0;
    score->value = 5;
    for (x = 0; x < 5; x++) {
        score->blokes[x] = 0;
    }
    score->next = BoatRideNodeList;
    BoatRideNodeList = score;
    AddBasicObject(obj, coords);
    AddBoatPathTile(coords[0] + BoatingSchoolStartFootprint.v[0] + 2, coords[1] + BoatingSchoolStartFootprint.v[1] + 2, 1, &score->id);
    AddBoatPathTile(coords[0] + BoatingSchoolExtraFootprint.v[0] + 2, coords[1] + BoatingSchoolExtraFootprint.v[1] + 2, 4, &score->id);
    for (y = BoatingSchoolFootprint.v[1]; y <= BoatingSchoolFootprint.v[3]; y++) {
        for (x = BoatingSchoolFootprint.v[0]; x <= BoatingSchoolFootprint.v[2]; x++) {
            if (x == BoatingSchoolFootprint.v[0]) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMapping->tiles + 9);
            } else if (x == BoatingSchoolFootprint.v[2]) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMapping->tiles + 0xc);
            } else {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMapping->tiles);
            }
        }
    }
    SetMapTile(coords[0] + BoatingSchoolFootprint.v[2], coords[1] + BoatingSchoolFootprint.v[1], *BoatingSchoolTileMapping->tiles + 8);
    SetMapTile(coords[0] + BoatingSchoolFootprint.v[2], coords[1] + BoatingSchoolFootprint.v[3], *BoatingSchoolTileMapping->tiles + 7);
    SetMapTile(coords[0] + 4 + BoatingSchoolFootprint.v[0], coords[1] + BoatingSchoolFootprint.v[3], *BoatingSchoolTileMapping->tiles + 4);
    SetMapTile(coords[0] + 4 + BoatingSchoolFootprint.v[0], coords[1] + BoatingSchoolFootprint.v[1], *BoatingSchoolTileMapping->tiles + 1);
    SetMapTile(coords[0] + 5 + BoatingSchoolFootprint.v[0], coords[1] + BoatingSchoolFootprint.v[3], *BoatingSchoolTileMapping->tiles + 0xb);
    SetMapTile(coords[0] + 5 + BoatingSchoolFootprint.v[0], coords[1] + BoatingSchoolFootprint.v[1], *BoatingSchoolTileMapping->tiles + 10);
}

// FUNCTION: LEGOLAND 0x0041a2f0
void BoatingSchoolCalcCursor(struct Element *param_1, int *param_2, unsigned int param_3) {
    struct Ride *cursor = (struct Ride *)param_1->data; /* the class (as the original names it) */

    BoatingSchoolFootprint.next = &BoatingSchoolStartFootprint;
    BoatingSchoolStartFootprint.next = &BoatingSchoolExtraFootprint;
    BoatingSchoolStartFootprint.next->next = NULL;
    memcpy(EditCursor.field_1414, BoatingSchoolFootprint.v, 20);
    EditCursor.field_1830 = 0;
    ScreenToMapRef(param_2, &EditCursor.tile_x, param_3);
    PathCursor.tile_x = EditCursor.tile_x;
    PathCursor.tile_y = EditCursor.tile_y;
    PathCursor.field_1414[0] = EditCursor.field_1414[2] + 1;
    PathCursor.field_1414[1] = EditCursor.field_1414[1];
    PathCursor.field_1414[2] = PathCursor.field_1414[0];
    PathCursor.field_1414[3] = EditCursor.field_1414[3];
    PathCursor.field_1828 = 0x1008;
    PathCursor.field_1830 = 0;
    EditCursor.next = &PathCursor;
    ResetCursorBuildable(&EditCursor);
    ResetCursorBuildable(&PathCursor);
    ValidateCursor(&EditCursor, cursor);
}

// FUNCTION: LEGOLAND 0x0041a3d0
void BoatingSchoolDCalcCursor(void *param_1, struct Point *param_2) {
    struct PathNode *path = BoatPathList;
    struct MermaidNode *node = MermaidList;

    BasicObjectDCalcCursor(param_1, param_2);
    PathCursor.tile_x = QueryCursor.tile_x;
    PathCursor.tile_y = QueryCursor.tile_y;
    PathCursor.field_1414[0] = QueryCursor.field_1414[2] + 1;
    PathCursor.field_1414[2] = PathCursor.field_1414[0];
    PathCursor.field_1414[1] = QueryCursor.field_1414[1];
    PathCursor.field_1414[3] = QueryCursor.field_1414[3];
    PathCursor.field_1414[4] = 0;
    PathCursor.field_1828 = 0x1008;
    PathCursor.field_1830 = 0;
    QueryCursor.next = &PathCursor;
    QueryCursorKeepPath = 1;
    DefaultCursor(&BoatingSchoolDCursor);
    *(struct Footprint *)BoatingSchoolDCursor.field_1414 = BoatingSchoolWaterFootprint;
    for (; path != NULL; path = path->next) {
        if (path->owner.id == QueryObj.id) {
            BoatingSchoolDCursor.tile_x = path->tile.pos.x;
            BoatingSchoolDCursor.tile_y = path->tile.pos.y;
            ResetCursorBuildable(&BoatingSchoolDCursor);
            BoatingSchoolDCursor.field_1828 = 8;
            BuildCursorPtr(&BoatingSchoolDCursor, 0, 0);
            RenderCursor(&BoatingSchoolDCursor);
        }
    }
    for (; node != NULL; node = node->next) {
        if (node->owner == QueryObj.id) {
            BoatingSchoolDCursor.tile_x = node->tile.pos.x;
            BoatingSchoolDCursor.tile_y = node->tile.pos.y;
            ResetCursorBuildable(&BoatingSchoolDCursor);
            BoatingSchoolDCursor.field_1828 = 8;
            BuildCursorPtr(&BoatingSchoolDCursor, 0, 0);
            RenderCursor(&BoatingSchoolDCursor);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041a530
void BoatingSchoolRemoveObject(Element *obj, TileId tile, struct Cursor *cursor) {
    struct BoatRideNode *score = BoatRideNodeList;
    struct BoatRideNode *prev = NULL;
    struct BoatRide *ride = BoatRideList;
    struct PathNode *path;
    struct MermaidNode *mer;
    Element fake;
    int x;
    int y;
    int savedX;
    int savedY;

    StandardRemoveObject(obj, tile, cursor);
    for (y = BoatingSchoolFootprint.v[1]; y <= BoatingSchoolFootprint.v[3]; y++) {
        for (x = BoatingSchoolFootprint.v[0]; x <= BoatingSchoolFootprint.v[2]; x++) {
            RestoreBaseMap(cursor->tile_x + x, cursor->tile_y + y);
        }
    }
    while (score->id != tile.id) {
        prev = score;
        score = score->next;
        if (score == NULL) {
            return;
        }
    }
    if (score != NULL) {
        fake.ride = BoatingSchoolWaterRide;
        IncrementObjectCount(BoatingSchoolWaterRide);
        IncrementObjectCount(BoatingSchoolWaterRide);
        path = BoatPathList;
        while (path != NULL) {
            if (path->owner.id == tile.id) {
                BoatingSchoolDCursor.tile_x = path->tile.pos.x;
                BoatingSchoolDCursor.tile_y = path->tile.pos.y;
                RemoveBoatPath(&fake, path->tile, &BoatingSchoolDCursor);
                path = BoatPathList;
            } else {
                path = path->next;
            }
        }
        fake.ride = BoatingSchoolMermaidRide;
        mer = MermaidList;
        while (mer != NULL) {
            if (mer->owner == tile.id) {
                savedX = cursor->tile_x;
                savedY = cursor->tile_y;
                cursor->tile_x = mer->tile.pos.x;
                cursor->tile_y = mer->tile.pos.y;
                BoatingSchoolMermaidRemoveObject(&fake, mer->tile, cursor);
                cursor->tile_x = savedX;
                cursor->tile_y = savedY;
                mer = MermaidList;
            } else {
                mer = mer->next;
            }
        }
        if (prev != NULL) {
            prev->next = score->next;
        } else {
            BoatRideNodeList = score->next;
        }
        while (ride != NULL) {
            if (ride->id == tile.id) {
                RemoveBoatRide(ride);
                ride = BoatRideList;
            } else {
                ride = ride->next;
            }
        }
        RemoveAllBlokesFromRide(obj->ride, tile);
        free(score);
    }
}

// FUNCTION: LEGOLAND 0x0041a720
void BoatingSchoolUpdate(void) {
    struct RideNode *node = BoatingSchoolRide->riders;
    struct RideNode *next;
    struct BoatRideNode *score;
    struct Bloke *bloke;
    struct LLS *lls;
    TileId tile;
    unsigned int slot;
    int i;
    int frame;
    char dir;
    struct SampleSource source;
    struct SampleSource source2;
    struct Sample *sample;

    lls = GetLLSForSprite((struct Sprite *)BoatingSchoolLayer5Sprite);
    if (++BoatingSchoolAnimTick == 0x50) {
        BoatingSchoolAnimTick = 0;
        UpdateBoatRides();
    }
    RenderBoatingSchoolBoats(0);
    for (; node != NULL; node = next) {
        score = BoatRideNodeList;
        next = node->next;
        tile = node->tile;
        for (; score != NULL; score = score->next) {
            if (score->id == tile.id) {
                break;
            }
        }
        bloke = node->rider;
        if (bloke->low_level_action != 0) {
            continue;
        }
        switch (bloke->param_action) {
        case 0:
            slot = 4;
            for (i = 0; i < 5; i++) {
                if (score->blokes[i] == bloke) {
                    slot = i;
                    break;
                }
            }
            if (i == 5) {
                if (score->bloke_count == 5 || score->blokes[4] != 0) {
                    RemoveBlokeFromRide(BoatingSchoolRide, node);
                    break;
                }
                score->blokes[slot] = bloke;
                score->bloke_count++;
            } else {
                bloke = score->blokes[slot];
                if (score->blokes[slot - 1] != 0) {
                    break;
                }
                score->blokes[slot - 1] = bloke;
                score->blokes[slot] = 0;
                if (--slot == 0) {
                    bloke->param_action++;
                }
            }
            bloke->flags |= 8;
            bloke->dest.x = ((BoatingSchoolRide->x + tile.pos.x) << 8) + BoatingSchoolQueueOffsets[4 - slot].x;
            bloke->dest.y = ((BoatingSchoolRide->y + tile.pos.y) << 8) + BoatingSchoolQueueOffsets[4 - slot].y;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            break;
        case 1:
            if (bloke == score->blokes[0] && score->connected != 0) {
                if ((int)score->value >= (int)TagAndCountBoatingSchoolBoats(score) * 6 && AddBoatRide(tile, bloke) != 0) {
                    BlokeSitAnim(bloke);
                    BlokeSetFrame(bloke, 0);
                    score->blokes[0] = 0;
                    score->bloke_count--;
                    bloke->flags |= 0x80;
                    bloke->param_action++;
                    source.type = 1;
                    source.bloke = bloke;
                    sample = PlayInstanceOfSample(PTR_s_Boat_Noise_wav[0].sample, 1, 1, &source);
                    AdjustPSampleFreq(sample, 10);
                }
            }
            break;
        case 3:
            BlokeWalkAnim(bloke);
            BlokeSetFrame(bloke, 0);
            bloke->flags &= 0xff7f;
            bloke->pos.x = (BoatingSchoolRide->field_24 + tile.pos.x - 4) << 8;
            bloke->pos.y = (BoatingSchoolRide->field_25 + tile.pos.y + 2) << 8;
            bloke->dir = 10;
            bloke->dest.x = ((BoatingSchoolRide->field_24 + tile.pos.x) << 8) - 0xc0;
            bloke->dest.y = ((BoatingSchoolRide->field_25 + tile.pos.y) << 8) + 0x240;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->low_level_action = 7;
            bloke->target_dir = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 4:
            bloke->dest.x = ((BoatingSchoolRide->field_24 + tile.pos.x) << 8) - 0xc0;
            bloke->dest.y = ((BoatingSchoolRide->field_25 + tile.pos.y) << 8) + 0x80;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->low_level_action = 7;
            bloke->target_dir = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 5:
            bloke->dest.x = ((BoatingSchoolRide->field_24 + tile.pos.x) << 8) + 0x80;
            bloke->dest.y = ((BoatingSchoolRide->field_25 + tile.pos.y) << 8) + 0x80;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->low_level_action = 7;
            bloke->target_dir = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            source2.type = 1;
            source2.bloke = bloke;
            UnSourceAndFadeAllSamplesFromSource(&source2, -0x5a);
            break;
        case 6:
            bloke->flags &= 0xfff7;
            RemoveBlokeFromRide(BoatingSchoolRide, node);
            break;
        }
    }
    for (score = BoatRideNodeList; score != NULL; score = score->next) {
        frame = ++score->field_c;
        if (frame <= lls->frame_count) {
            if (score->field_10 == 0) {
                LLSSetFrame(lls, lls->frame_count - frame);
            } else {
                LLSSetFrame(lls, frame);
            }
        }
        if (score->field_10 == 0 && score->field_c == 100) {
            score->field_c = 0;
            score->field_10 = 1;
        }
    }
}

// FUNCTION: LEGOLAND 0x0041abd0
void RenderBoatingSchool(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    short *lls;
    struct LLS *hull;
    struct Point pos;
    struct Point offset;

    RenderBoatingSchoolBoats(1);
    pos = GetScreenCoordsForObject((TileId *)tile, ride);
    offset = GetRenderOffsetForLayer((struct Sprite *)BoatingSchoolRide->layer, 3);
    AdjustOffsetForViewMode(&offset);
    lls = (short *)GetLLSForSprite((struct Sprite *)GetSpriteForLayer((struct Sprite *)BoatingSchoolRide->layer, 3));
    hull = GetLLSForSprite((struct Sprite *)BoatingSchoolHullMaskSprite);
    LLSSetFrame(hull, *lls);
    PrintSprite(BoatingSchoolHullMaskSprite, pos.x + offset.x, pos.y + offset.y, clip, 0);
    for (; node != NULL; node = node->next) {
        if (*tile == node->tile.id && node->rider->param_action != 2) {
            IP_RenderBlokeIn3DNow(node->rider);
        }
    }
    offset = GetRenderOffsetForLayer((struct Sprite *)BoatingSchoolRide->layer, 3);
    offset.x += 0x71;
    offset.y += 0xac;
    AdjustOffsetForViewMode(&offset);
    PrintSprite(BoatingSchoolRailmSprite, pos.x + offset.x, pos.y + offset.y, clip, 0);
}

// FUNCTION: LEGOLAND 0x0041acf0
int BoatingSchool_Save(struct Element *element) {
    struct BoatRideNode *score;
    struct BoatRideNode *scoreCur;
    struct PathNode *path;
    struct PathNode *pathCur;
    struct MermaidNode *mer;
    struct MermaidNode *merCur;
    struct BoatRide *ride;
    struct BoatRide *rideCur;
    int count;
    int i;
    struct BoatRideNode scoreCopy;
    struct BoatRide rideCopy;

    count = 0;
    for (scoreCur = BoatRideNodeList; scoreCur != NULL; scoreCur = scoreCur->next) {
        count++;
    }
    score = BoatRideNodeList;
    SaveGameWrite(&count, 4);
    while (count--) {
        scoreCopy = *score;
        for (i = 0; i < 5; i++) {
            scoreCopy.blokes[i] = (struct Bloke *)(INT_PTR)GetBlokeNum(scoreCopy.blokes[i]);
        }
        SaveGameWrite(&scoreCopy, sizeof(struct BoatRideNode));
        score = score->next;
    }
    count = 0;
    for (pathCur = BoatPathList; pathCur != NULL; pathCur = pathCur->next) {
        count++;
    }
    path = BoatPathList;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(path, sizeof(struct PathNode));
        path = path->next;
    }
    count = 0;
    for (merCur = MermaidList; merCur != NULL; merCur = merCur->next) {
        count++;
    }
    mer = MermaidList;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(mer, sizeof(struct MermaidNode));
        mer = mer->next;
    }
    count = 0;
    for (rideCur = BoatRideList; rideCur != NULL; rideCur = rideCur->next) {
        count++;
    }
    ride = BoatRideList;
    SaveGameWrite(&count, 4);
    while (count--) {
        rideCopy = *ride;
        rideCopy.bloke = (struct Bloke *)(INT_PTR)GetBlokeNum(rideCopy.bloke);
        SaveGameWrite(&rideCopy, sizeof(struct BoatRide));
        ride = ride->next;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0041aee0
int BoatingSchool_Load(struct Element *element) {
    struct BoatRideNode *score;
    struct BoatRideNode *prevScore;
    struct PathNode *path;
    struct PathNode *prevPath;
    struct MermaidNode *mer;
    struct MermaidNode *prevMer;
    struct BoatRide *ride;
    struct BoatRide *prevRide;
    int count;
    int i;

    prevScore = NULL;
    prevPath = NULL;
    prevMer = NULL;
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevScore == NULL) {
            BoatRideNodeList = (struct BoatRideNode *)malloc(sizeof(struct BoatRideNode));
            prevScore = BoatRideNodeList;
        } else {
            score = (struct BoatRideNode *)malloc(sizeof(struct BoatRideNode));
            prevScore->next = score;
            prevScore = score;
        }
        SaveGameRead(prevScore, sizeof(struct BoatRideNode));
        for (i = 0; i < 5; i++) {
            prevScore->blokes[i] = GetBlokePtr((int)(INT_PTR)prevScore->blokes[i]);
        }
    }
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevPath == NULL) {
            BoatPathList = (struct PathNode *)malloc(sizeof(struct PathNode));
            prevPath = BoatPathList;
        } else {
            path = (struct PathNode *)malloc(sizeof(struct PathNode));
            prevPath->next = path;
            prevPath = path;
        }
        SaveGameRead(prevPath, sizeof(struct PathNode));
    }
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevMer == NULL) {
            MermaidList = (struct MermaidNode *)malloc(sizeof(struct MermaidNode));
            prevMer = MermaidList;
        } else {
            mer = (struct MermaidNode *)malloc(sizeof(struct MermaidNode));
            prevMer->next = mer;
            prevMer = mer;
        }
        SaveGameRead(prevMer, sizeof(struct MermaidNode));
    }
    prevRide = BoatRideList;
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevRide == NULL) {
            BoatRideList = (struct BoatRide *)malloc(sizeof(struct BoatRide));
            prevRide = BoatRideList;
        } else {
            ride = (struct BoatRide *)malloc(sizeof(struct BoatRide));
            prevRide->next = ride;
            prevRide = ride;
        }
        SaveGameRead(prevRide, sizeof(struct BoatRide));
        prevRide->bloke = GetBlokePtr((int)(INT_PTR)prevRide->bloke);
    }
    for (score = BoatRideNodeList; score != NULL; score = score->next) {
        ComputeBoatPathDistances(score->id);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0041b0d0
void AddBoatRideNodeValue(unsigned short id, unsigned int value) {
    struct BoatRideNode *node = BoatRideNodeList;
    if (node == NULL) {
        return;
    }

    while (node != NULL && node->id != id) {
        node = node->next;
    }

    if (node != NULL) {
        node->value += value;
    }
}

// FUNCTION: LEGOLAND 0x0041b100
int GetBestBoatRideScore(struct Element *dummy, int arg) {
    struct BoatRideNode *node = BoatRideNodeList;
    int result = 0;

    if (node != NULL) {
        do {
            int val = (int)node->value;
            if (val > result) {
                if (arg != 0) {
                    if (node->connected != 0) {
                        result = val;
                    }
                } else {
                    result = val;
                }
            }
            node = node->next;
        } while (node != NULL);
    }

    return result;
}

// FUNCTION: LEGOLAND 0x0041b150
LEGO_EXPORT void GetInterface(struct Element *head, struct Ride *iface) {
    void **cb = (void **)iface;
    // STRING: LEGOLAND 0x004b537c
    if (_stricmp("BOATING SCHOOL WATER", head->name) == 0) {
        cb[7] = BoatingSchoolWaterLoad;
        cb[0] = BoatingSchoolSetEditMode;
        cb[1] = BoatingSchoolWaterCalcCursor;
        cb[2] = BoatingSchoolWaterDCalcCursor;
        cb[3] = BoatingSchoolWaterAddObject;
        cb[4] = BoatingSchoolWaterRemoveObject;
        return;
    }
    if (_stricmp("BOATING SCHOOL", head->name) == 0) {
        cb[7] = LoadBoatingSchoolResources;
        cb[8] = UnloadBoatingSchoolResources;
        cb[0] = SetBoatingSchoolEditCursor;
        cb[1] = BoatingSchoolCalcCursor;
        cb[2] = BoatingSchoolDCalcCursor;
        cb[3] = BoatingSchoolAddObject;
        cb[4] = BoatingSchoolRemoveObject;
        cb[6] = BoatingSchoolUpdate;
        cb[9] = RenderBoatingSchool;
        cb[0xc] = BoatingSchool_Save;
        cb[0xb] = BoatingSchool_Load;
        cb[0xd] = GetBestBoatRideScore;
        return;
    }
    if (_stricmp("BOATING SCHOOL MERMAID", head->name) == 0) {
        cb[7] = InitBoatingSchoolMermaid;
        cb[0] = BoatingSchoolMermaidSetEditMode;
        cb[1] = BoatingSchoolMermaidCalcCursor;
        cb[2] = BoatingSchoolMermaidDCalcCursor;
        cb[3] = BoatingSchoolMermaidAddObject;
        cb[4] = BoatingSchoolMermaidRemoveObject;
    }
}

// FUNCTION: LEGOLAND 0x0041b250
void InitBoatingSchoolMermaid(Element *param_1) {
    BoatingSchoolMermaidRide = param_1->ride;
}

// FUNCTION: LEGOLAND 0x0041b260
void BoatingSchoolMermaidSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = BoatingSchoolMermaidRide;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(&BoatingSchoolMermaidRide->footprint);
}

// FUNCTION: LEGOLAND 0x0041b2a0
void BoatingSchoolMermaidAddObject(struct Element *obj, int *coords) {
    struct Ride *ride = ((Element *)obj)->ride;
    TileId tile;
    struct MermaidNode *node;
    unsigned short owner;
    int x;
    int y;
    struct SampleSource source;

    tile.pos.x = coords[0];
    tile.pos.y = coords[1];
    GetBoatPathNeighbourMask(coords[0], coords[1], &owner);
    node = (struct MermaidNode *)malloc(sizeof(struct MermaidNode));
    if (node == NULL) {
        return;
    }
    node->tile = tile;
    node->owner = owner;
    node->next = MermaidList;
    MermaidList = node;
    AddBoatRideNodeValue(owner, 1);
    AddBasicObject(obj, coords);
    for (y = ride->footprint.v[1]; y <= ride->footprint.v[3]; y++) {
        for (x = ride->footprint.v[0]; x <= ride->footprint.v[2]; x++) {
            if (x == ride->footprint.v[0]) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMapping->tiles + 9);
            } else if (x == ride->footprint.v[2]) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMapping->tiles + 0xc);
            } else if (y == ride->footprint.v[1]) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMapping->tiles + 10);
            } else if (y == ride->footprint.v[3]) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMapping->tiles + 0xb);
            } else {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMapping->tiles);
            }
        }
    }
    SetMapTile(ride->footprint.v[0] + coords[0], ride->footprint.v[1] + coords[1], *BoatingSchoolTileMapping->tiles + 5);
    SetMapTile(coords[0] + ride->footprint.v[2], ride->footprint.v[1] + coords[1], *BoatingSchoolTileMapping->tiles + 8);
    SetMapTile(ride->footprint.v[0] + coords[0], ride->footprint.v[3] + coords[1], *BoatingSchoolTileMapping->tiles + 6);
    SetMapTile(coords[0] + ride->footprint.v[2], ride->footprint.v[3] + coords[1], *BoatingSchoolTileMapping->tiles + 7);
    source.x = coords[0];
    source.type = 2;
    source.y = coords[1];
    PlayInstanceOfSample(PTR_s_Boat_Noise_wav[1].sample, 1, 1, &source);
}

// FUNCTION: LEGOLAND 0x0041b4c0
void BoatingSchoolMermaidCalcCursor(Element *obj, int *param_2, unsigned int param_3) {
    struct Ride *ride;
    unsigned int mask;
    unsigned short owner;
    int n;
    int x;
    int y;
    struct Cursor *c;

    n = 0;
    ride = obj->ride;
    memcpy(EditCursor.field_1414, &ride->footprint, sizeof(struct Footprint));
    ScreenToMapRef(param_2, &EditCursor.tile_x, param_3);
    mask = GetBoatPathNeighbourMask(EditCursor.tile_x, EditCursor.tile_y, &owner);
    EditCursor.field_1830 = n;
    if (mask == 0) {
        SetCursorBlocked(&EditCursor, 0xe);
        return;
    }
    ValidateCursor(&EditCursor, ride);
    if (IsCursorBuildable(&EditCursor) == 0) {
        return;
    }
    DefaultCursor(&BoatingSchoolMermaidCursors[0]);
    DefaultCursor(&BoatingSchoolMermaidCursors[1]);
    DefaultCursor(&BoatingSchoolMermaidCursors[2]);
    DefaultCursor(&BoatingSchoolMermaidCursors[3]);
    memcpy(BoatingSchoolMermaidCursors[0].field_1414, EditCursor.field_1414, 20);
    memcpy(BoatingSchoolMermaidCursors[1].field_1414, EditCursor.field_1414, 20);
    memcpy(BoatingSchoolMermaidCursors[2].field_1414, EditCursor.field_1414, 20);
    memcpy(BoatingSchoolMermaidCursors[3].field_1414, EditCursor.field_1414, 20);
    ResetCursorBuildable(&BoatingSchoolMermaidCursors[0]);
    ResetCursorBuildable(&BoatingSchoolMermaidCursors[1]);
    ResetCursorBuildable(&BoatingSchoolMermaidCursors[2]);
    ResetCursorBuildable(&BoatingSchoolMermaidCursors[3]);
    x = EditCursor.tile_x;
    y = EditCursor.tile_y;
    BoatingSchoolMermaidCursors[0].field_1828 = 0x2034;
    BoatingSchoolMermaidCursors[1].field_1828 = 0x2034;
    BoatingSchoolMermaidCursors[2].field_1828 = 0x2034;
    BoatingSchoolMermaidCursors[3].field_1828 = 0x2034;
    if ((mask & 1) != 0) {
        BoatingSchoolMermaidCursors[0].tile_x = x;
        BoatingSchoolMermaidCursors[0].tile_y = y - 5;
        n = 1;
    }
    if ((mask & 2) != 0) {
        BoatingSchoolMermaidCursors[n].tile_x = x + 5;
        BoatingSchoolMermaidCursors[n].tile_y = y;
        n++;
    }
    if ((mask & 4) != 0) {
        BoatingSchoolMermaidCursors[n].tile_x = x;
        BoatingSchoolMermaidCursors[n].tile_y = y + 5;
        n++;
    }
    if ((mask & 8) != 0) {
        BoatingSchoolMermaidCursors[n].tile_x = x - 5;
        BoatingSchoolMermaidCursors[n].tile_y = y;
        n++;
    }
    if (n != 0) {
        EditCursor.next = &BoatingSchoolMermaidCursors[0];
        if (n > 1) {
            c = &BoatingSchoolMermaidCursors[1];
            n--;
            do {
                c[-1].next = c;
                c++;
                n--;
            } while (n != 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041b6d0
unsigned int BoatingSchoolMermaidDCalcCursor(struct Element *param_1, struct Point *param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x0041b6f0
void BoatingSchoolMermaidRemoveObject(void *param_1, TileId tile, struct Cursor *param_3) {
    struct Ride *ride = ((struct Element *)param_1)->ride;
    struct MermaidNode *node = MermaidList;
    struct MermaidNode *prev = NULL;
    int x;
    int y;
    struct SampleSource source;

    StandardRemoveObject(param_1, tile, param_3);
    source.type = 2;
    source.x = tile.pos.x;
    source.y = tile.pos.y;
    if (CountSamplesFromSource(&source) != 1) {
        // STRING: LEGOLAND 0x004b5398
        DBPrintf("Can't find samples for mermaid\n");
    }
    UnSourceAndFadeAllSamplesFromSource(&source, -400);
    for (y = ride->footprint.v[1]; y <= ride->footprint.v[3]; y++) {
        for (x = ride->footprint.v[0]; x <= ride->footprint.v[2]; x++) {
            RestoreBaseMap(x + param_3->tile_x, y + param_3->tile_y);
        }
    }
    while (node->tile.id != tile.id) {
        prev = node;
        node = node->next;
        if (node == NULL) {
            return;
        }
    }
    if (node != NULL) {
        AddBoatRideNodeValue(node->owner, -1);
        if (prev != NULL) {
            prev->next = node->next;
        } else {
            MermaidList = node->next;
        }
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x0041b830
void BoatingSchoolWaterLoad(Element *arg) {
    struct Ride *building = arg->ride;
    BoatingSchoolWaterRide = building;
    BoatingSchoolWaterFootprint.v[1] += building->footprint.v[1];
    BoatingSchoolWaterFootprint.v[0] += building->footprint.v[0];
    BoatingSchoolWaterFootprint.v[2] += building->footprint.v[0];
    BoatingSchoolWaterFootprint.v[3] += building->footprint.v[1];
}

// FUNCTION: LEGOLAND 0x0041b880
void BoatingSchoolSetEditMode(void) {
    struct Ride *state = BoatingSchoolWaterRide;
    EditMode.unk0 = 1;
    EditMode.unk8 = state;
    memcpy(&state->footprint, &BoatingSchoolWaterFootprint, sizeof(BoatingSchoolWaterFootprint));
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x0041b8e0
void BoatingSchoolWaterAddObject(Element *obj, int *coords) {
    struct BoatRideNode *score = BoatRideNodeList;
    unsigned int mask;
    unsigned short owner;
    int x0;
    int y0;
    int x1;
    int y1;

    mask = GetBoatPathNeighbourMask(coords[0], coords[1], &owner);
    AddBoatPathTile(coords[0], coords[1], mask, &owner);
    IncrementObjectCount(obj->ride);
    AddBoatRideNodeValue(owner, 1);
    FillBoatingWaterCorners(coords[0], coords[1], &owner);
    if ((mask & 1) != 0) {
        AddBoatPathTile(coords[0], coords[1] - 5, GetBoatPathNeighbourMask(coords[0], coords[1] - 5, &owner), NULL);
        FillBoatingWaterCorners(coords[0], coords[1] - 5, &owner);
    }
    if ((mask & 2) != 0) {
        AddBoatPathTile(coords[0] + 5, coords[1], GetBoatPathNeighbourMask(coords[0] + 5, coords[1], &owner), NULL);
        FillBoatingWaterCorners(coords[0] + 5, coords[1], &owner);
    }
    if ((mask & 4) != 0) {
        AddBoatPathTile(coords[0], coords[1] + 5, GetBoatPathNeighbourMask(coords[0], coords[1] + 5, &owner), NULL);
        FillBoatingWaterCorners(coords[0], coords[1] + 5, &owner);
    }
    if ((mask & 8) != 0) {
        AddBoatPathTile(coords[0] - 5, coords[1], GetBoatPathNeighbourMask(coords[0] - 5, coords[1], &owner), NULL);
        FillBoatingWaterCorners(coords[0] - 5, coords[1], &owner);
    }
    for (; score != NULL; score = score->next) {
        if (score->id == owner) {
            x0 = score->start.pos.x;
            y0 = score->start.pos.y;
            x1 = score->end.pos.x;
            y1 = score->end.pos.y;
            score->connected = IsBoatPathConnected(x0, y0, x1, y1);
            if (score->connected != 0) {
                ComputeBoatPathDistances(owner);
            }
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x0041bab0
void FillBoatingWaterCorners(int param_1, int param_2, unsigned short *param_3) {
    struct BoatRideNode *score = BoatRideNodeList;
    struct PathNode *path;
    struct PathNode *other;
    unsigned int mask;

    path = FindBoatPathAt(param_1, param_2);
    for (; score != NULL; score = score->next) {
        if (score->id == *param_3) {
            break;
        }
    }
    if (path != NULL) {
        mask = path->dir_mask;
        if (path->tile.id == score->start.id) {
            mask = mask & 0xfffffffe;
        } else if (path->tile.id == score->end.id) {
            mask = mask & 0xfffffffb;
        }
        if ((mask & 8) != 0 && (mask & 1) != 0 &&
            (other = FindBoatPathAt(param_1, (param_2 - 5)), (*(unsigned char *)&other->dir_mask & 8) != 0)) {
            SetMapTile(param_1 - 3, param_2 - 3, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 - 2, param_2 - 3, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 - 3, param_2 - 2, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 - 2, param_2 - 2, *BoatingSchoolTileMapping->tiles);
        }
        if ((mask & 8) != 0 && (mask & 4) != 0 &&
            (other = FindBoatPathAt(param_1, (param_2 + 5)), (*(unsigned char *)&other->dir_mask & 8) != 0)) {
            SetMapTile(param_1 - 3, param_2 + 3, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 - 2, param_2 + 3, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 - 3, param_2 + 2, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 - 2, param_2 + 2, *BoatingSchoolTileMapping->tiles);
        }
        if ((mask & 2) != 0 && (mask & 1) != 0 &&
            (other = FindBoatPathAt(param_1, (param_2 - 5)), (*(unsigned char *)&other->dir_mask & 2) != 0)) {
            SetMapTile(param_1 + 3, param_2 - 3, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 + 2, param_2 - 3, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 + 3, param_2 - 2, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 + 2, param_2 - 2, *BoatingSchoolTileMapping->tiles);
        }
        if ((mask & 2) != 0 && (mask & 4) != 0 &&
            (other = FindBoatPathAt(param_1, (param_2 + 5)), (*(unsigned char *)&other->dir_mask & 2) != 0)) {
            SetMapTile(param_1 + 3, param_2 + 3, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 + 2, param_2 + 3, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 + 3, param_2 + 2, *BoatingSchoolTileMapping->tiles);
            SetMapTile(param_1 + 2, param_2 + 2, *BoatingSchoolTileMapping->tiles);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041bd40
void BoatingSchoolWaterCalcCursor(Element *obj, int *param_2, unsigned int param_3) {
    unsigned int mask;
    unsigned short owner;
    int n;
    int x;
    int y;
    int result;
    struct Cursor *c;
    struct MapRect rect;

    n = 0;
    memcpy(EditCursor.field_1414, &BoatingSchoolWaterFootprint, sizeof(BoatingSchoolWaterFootprint));
    ScreenToMapRef(param_2, &EditCursor.tile_x, param_3);
    mask = GetBoatPathNeighbourMask(EditCursor.tile_x, EditCursor.tile_y, &owner);
    EditCursor.field_1830 = n;
    if (mask == 0) {
        SetCursorBlocked(&EditCursor, 0xe);
    } else {
        ValidateCursor(&EditCursor, obj->ride);
        if (IsCursorBuildable(&EditCursor) != 0) {
            rect.x0 = EditCursor.field_1414[0] + EditCursor.tile_x;
            rect.y0 = EditCursor.field_1414[1] + EditCursor.tile_y;
            rect.x1 = EditCursor.field_1414[2] + EditCursor.tile_x;
            rect.y1 = EditCursor.field_1414[3] + EditCursor.tile_y;
            result = CheckForPeople(&rect);
            if (result != -1) {
                if (result != 1) {
                    DefaultCursor(&BoatingSchoolWaterCursors[0]);
                    DefaultCursor(&BoatingSchoolWaterCursors[1]);
                    DefaultCursor(&BoatingSchoolWaterCursors[2]);
                    DefaultCursor(&BoatingSchoolWaterCursors[3]);
                    memcpy(BoatingSchoolWaterCursors[0].field_1414, EditCursor.field_1414, 20);
                    memcpy(BoatingSchoolWaterCursors[1].field_1414, EditCursor.field_1414, 20);
                    memcpy(BoatingSchoolWaterCursors[2].field_1414, EditCursor.field_1414, 20);
                    memcpy(BoatingSchoolWaterCursors[3].field_1414, EditCursor.field_1414, 20);
                    ResetCursorBuildable(&BoatingSchoolWaterCursors[0]);
                    ResetCursorBuildable(&BoatingSchoolWaterCursors[1]);
                    ResetCursorBuildable(&BoatingSchoolWaterCursors[2]);
                    ResetCursorBuildable(&BoatingSchoolWaterCursors[3]);
                    x = EditCursor.tile_x;
                    y = EditCursor.tile_y;
                    BoatingSchoolWaterCursors[0].field_1828 = 0x2034;
                    BoatingSchoolWaterCursors[1].field_1828 = 0x2034;
                    BoatingSchoolWaterCursors[2].field_1828 = 0x2034;
                    BoatingSchoolWaterCursors[3].field_1828 = 0x2034;
                    if ((mask & 1) != 0) {
                        BoatingSchoolWaterCursors[0].tile_x = x;
                        BoatingSchoolWaterCursors[0].tile_y = y - 5;
                        n = 1;
                    }
                    if ((mask & 2) != 0) {
                        BoatingSchoolWaterCursors[n].tile_x = x + 5;
                        BoatingSchoolWaterCursors[n].tile_y = y;
                        n++;
                    }
                    if ((mask & 4) != 0) {
                        BoatingSchoolWaterCursors[n].tile_x = x;
                        BoatingSchoolWaterCursors[n].tile_y = y + 5;
                        n++;
                    }
                    if ((mask & 8) != 0) {
                        BoatingSchoolWaterCursors[n].tile_x = x - 5;
                        BoatingSchoolWaterCursors[n].tile_y = y;
                        n++;
                    }
                    if (n != 0) {
                        EditCursor.next = &BoatingSchoolWaterCursors[0];
                        if (n > 1) {
                            c = &BoatingSchoolWaterCursors[1];
                            n--;
                            do {
                                c[-1].next = c;
                                c++;
                                n--;
                            } while (n != 0);
                        }
                    }
                } else {
                    SetCursorBlocked(&EditCursor, 3);
                }
            } else {
                SetCursorBlocked(&EditCursor, 4);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0041bfb0
void BoatingSchoolWaterDCalcCursor(struct Element *param_1, int *coords) {
    struct BoatRideNode *score = BoatRideNodeList;
    struct BoatRide *ride;
    struct MapElement *elem;
    struct PathNode *path;
    TileId tile;
    Element fake;

    if (coords[0] >= 0 && coords[0] < lpConfig->width && coords[1] >= 0 && coords[1] < lpConfig->height) {
        elem = &GameMap[coords[1]][coords[0]];
    } else {
        elem = NULL;
    }
    coords[0] = elem->field_4;
    coords[1] = elem->field_5;
    tile.pos.x = coords[0];
    tile.pos.y = coords[1];
    for (; score != NULL; score = score->next) {
        if (tile.id == score->start.id || tile.id == score->end.id) {
            path = FindBoatPathAt(coords[0], coords[1]);
            QueryObj.pos.x = path->owner.pos.x;
            coords[0] = QueryObj.pos.x;
            QueryObj.pos.y = path->owner.pos.y;
            coords[1] = QueryObj.pos.y;
            memcpy(&QueryClass->footprint, &BoatingSchoolFootprint, sizeof(BoatingSchoolFootprint));
            fake.ride = BoatingSchoolRide;
            BoatingSchoolDCalcCursor(&fake, (struct Point *)coords);
            return;
        }
    }
    ride = BoatRideList;
    memcpy(&QueryClass->footprint, &BoatingSchoolWaterFootprint, sizeof(BoatingSchoolWaterFootprint));
    BasicObjectDCalcCursor(param_1, (struct Point *)coords);
    for (; ride != NULL; ride = ride->next) {
        if ((tile.pos.x == ride->tile_x && tile.pos.y == ride->tile_y) || (tile.pos.x == ride->next_x && tile.pos.y == ride->next_y)) {
            SetCursorBlocked(&QueryCursor, 1);
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x0041c130
void BoatingSchoolWaterRemoveObject(Element *obj, TileId tile, struct Cursor *cursor) {
    struct BoatRideNode *score = BoatRideNodeList;
    struct MapElement *elem;
    int ex;
    int ey;
    unsigned int mask;
    unsigned int dir;
    int x;
    int y;
    int x0;
    int y0;
    int x1;
    int y1;
    unsigned short owner;
    unsigned short other;
    Element fake;

    ex = tile.pos.x;
    ey = tile.pos.y;
    if (ex >= 0 && ex < lpConfig->width && ey >= 0 && ey < lpConfig->height) {
        elem = &GameMap[ey][ex];
    } else {
        elem = NULL;
    }
    if (elem->field_0 != BoatingSchoolWaterRide->element) {
        fake.ride = BoatingSchoolRide;
        BoatingSchoolRemoveObject(&fake, tile, cursor);
        return;
    }
    mask = GetBoatPathNeighbourMask(cursor->tile_x, cursor->tile_y, &owner);
    RemoveBoatPath(obj, tile, cursor);
    AddBoatRideNodeValue(owner, -1);
    if ((mask & 1) != 0) {
        y = cursor->tile_y - 5;
        x = cursor->tile_x;
        dir = GetBoatPathNeighbourMask(x, y, &other);
        AddBoatPathTile(x, y, dir, &owner);
        FillBoatingWaterCorners(x, y, &owner);
    }
    if ((mask & 2) != 0) {
        x = cursor->tile_x + 5;
        y = cursor->tile_y;
        dir = GetBoatPathNeighbourMask(x, y, &other);
        AddBoatPathTile(x, y, dir, &owner);
        FillBoatingWaterCorners(x, y, &owner);
    }
    if ((mask & 4) != 0) {
        y = cursor->tile_y + 5;
        x = cursor->tile_x;
        dir = GetBoatPathNeighbourMask(x, y, &other);
        AddBoatPathTile(x, y, dir, &owner);
        FillBoatingWaterCorners(x, y, &owner);
    }
    if ((mask & 8) != 0) {
        x = cursor->tile_x - 5;
        y = cursor->tile_y;
        dir = GetBoatPathNeighbourMask(x, y, &other);
        AddBoatPathTile(x, y, dir, &owner);
        FillBoatingWaterCorners(x, y, &owner);
    }
    if ((mask & 1) != 0 && (mask & 8) != 0) {
        x = cursor->tile_x - 5;
        y = cursor->tile_y - 5;
        if (FindBoatPathAt(x, y) != NULL) {
            dir = GetBoatPathNeighbourMask(x, y, &other);
            AddBoatPathTile(x, y, dir, &owner);
            FillBoatingWaterCorners(x, y, &owner);
        }
    }
    if ((mask & 1) != 0 && (mask & 2) != 0) {
        x = cursor->tile_x + 5;
        y = cursor->tile_y - 5;
        if (FindBoatPathAt(x, y) != NULL) {
            dir = GetBoatPathNeighbourMask(x, y, &other);
            AddBoatPathTile(x, y, dir, &owner);
            FillBoatingWaterCorners(x, y, &owner);
        }
    }
    if ((mask & 4) != 0 && (mask & 8) != 0) {
        x = cursor->tile_x - 5;
        y = cursor->tile_y + 5;
        if (FindBoatPathAt(x, y) != NULL) {
            dir = GetBoatPathNeighbourMask(x, y, &other);
            AddBoatPathTile(x, y, dir, &owner);
            FillBoatingWaterCorners(x, y, &owner);
        }
    }
    if ((mask & 4) != 0 && (mask & 2) != 0) {
        x = cursor->tile_x + 5;
        y = cursor->tile_y + 5;
        if (FindBoatPathAt(x, y) != NULL) {
            dir = GetBoatPathNeighbourMask(x, y, &other);
            AddBoatPathTile(x, y, dir, &owner);
            FillBoatingWaterCorners(x, y, &owner);
        }
    }
    ComputeBoatPathDistances(owner);
    for (; score != NULL; score = score->next) {
        if (score->id == owner) {
            x0 = score->start.pos.x;
            y0 = score->start.pos.y;
            x1 = score->end.pos.x;
            y1 = score->end.pos.y;
            score->connected = IsBoatPathConnected(x0, y0, x1, y1);
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x0041c4c0
void AddBoatPathTile(int x, int y, int mask, unsigned short *owner) {
    TileId tile;
    struct PathNode *node;
    struct MapElement *elem;
    int row;
    int col;
    struct Point pt;

    tile.pos.x = x;
    tile.pos.y = y;
    node = FindBoatPathAt(x, y);
    if (node == NULL) {
        node = (struct PathNode *)malloc(sizeof(struct PathNode));
        if (node == NULL) {
            return;
        }
        node->next = BoatPathList;
        node->parent = NULL;
        BoatPathList = node;
    }
    node->tile = tile;
    node->dir_mask = mask;
    if (owner != NULL) {
        node->owner.id = *owner;
    }
    BGFullUpdate = 1;
    for (row = 0; row < 5; row++) {
        for (col = 0; col < 5; col++) {
            pt.y = row + y - 2;
            pt.x = col + x - 2;
            if (pt.x >= 0 && pt.x < lpConfig->width && pt.y >= 0 && pt.y < lpConfig->height) {
                elem = &GameMap[pt.y][pt.x];
            } else {
                elem = NULL;
            }
            elem->flags = 8;
            elem->field_10 = 2;
            elem->field_0 = BoatingSchoolWaterRide->element;
            *(unsigned short *)&elem->field_4 = tile.id;
            SetMapTile(pt.x, pt.y, *BoatingSchoolTileMapping[BoatPathTilePatterns[mask * 25 + row * 5 + col] >> 8].tiles + (unsigned char)BoatPathTilePatterns[mask * 25 + row * 5 + col]);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041c620
void RemoveBoatPath(void *param_1, TileId tile, struct Cursor *param_3) {
    struct PathNode *node = BoatPathList;
    struct PathNode *prev = NULL;

    StandardRemoveObject(param_1, tile, param_3);
    while (node->tile.id != tile.id) {
        prev = node;
        node = node->next;
        if (node == NULL) {
            return;
        }
    }
    if (node != NULL) {
        if (prev != NULL) {
            prev->next = node->next;
            free(node);
            return;
        }
        BoatPathList = node->next;
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x0041c690
unsigned int GetBoatPathNeighbourMask(int x, int y, unsigned short *owner) {
    struct BoatRideNode *score;
    struct PathNode *node;
    unsigned int mask;
    int valid;
    int n;
    TileId key;

    mask = 0;
    valid = 0;
    score = BoatRideNodeList;
    node = FindBoatPathAt(x, y);
    if (node != NULL) {
        *owner = node->owner.id;
        valid = 1;
    }
    n = y - 5;
    if (x >= 0 && n >= 0 && x < lpConfig->width && n < lpConfig->height && (node = FindBoatPathAt(x, n)) != NULL) {
        if (valid) {
            if (node->owner.id == *owner) {
                mask = 1;
            }
        } else {
            mask = 1;
            *owner = node->owner.id;
            valid = 1;
        }
    }
    n = x + 5;
    if (n >= 0 && y >= 0 && n < lpConfig->width && y < lpConfig->height && (node = FindBoatPathAt(n, y)) != NULL) {
        if (valid) {
            if (node->owner.id == *owner) {
                mask |= 2;
            }
        } else {
            mask |= 2;
            *owner = node->owner.id;
            valid = 1;
        }
    }
    n = y + 5;
    if (x >= 0 && n >= 0 && x < lpConfig->width && n < lpConfig->height && (node = FindBoatPathAt(x, n)) != NULL) {
        if (valid) {
            if (node->owner.id == *owner) {
                mask |= 4;
            }
        } else {
            mask |= 4;
            *owner = node->owner.id;
            valid = 1;
        }
    }
    n = x - 5;
    if (n >= 0 && y >= 0 && n < lpConfig->width && y < lpConfig->height && (node = FindBoatPathAt(n, y)) != NULL) {
        if (valid) {
            if (node->owner.id == *owner) {
                mask |= 8;
            }
        } else {
            mask |= 8;
            *owner = node->owner.id;
        }
    }
    key.pos.x = x;
    key.pos.y = y;
    for (; score != NULL; score = score->next) {
        if (key.id == score->start.id) {
            mask |= 1;
            break;
        }
        if (key.id == score->end.id) {
            mask |= 4;
            break;
        }
    }
    return mask;
}

// FUNCTION: LEGOLAND 0x0041c890
struct PathNode *FindBoatPathAt(unsigned int a, unsigned int b) {
    struct PathNode *current;
    unsigned short key;
    unsigned char stack_key[2];

    stack_key[0] = (unsigned char)a;
    stack_key[1] = (unsigned char)b;
    key = *(unsigned short *)stack_key;

    current = BoatPathList;
    while (current != NULL && current->tile.id != key) {
        current = current->next;
    }

    return current;
}

// FUNCTION: LEGOLAND 0x0041c8c0
int IsBoatPathConnected(int a, int b, int c, int d) {
    struct PathNode *node;
    TileId key;
    int result;

    result = 0;
    for (node = BoatPathList; node != NULL; node = node->next) {
        node->visited = 0;
    }
    node = FindBoatPathAt(a, b);
    if (node == NULL) {
        return 0;
    }
    key.id = node->owner.id;
    SearchBoatPathConnected(a, b, c, d, &key, &result);
    return result;
}

/* Marks and searches connected boat-path nodes in five-tile steps for the target position, restricted to the
 * supplied owner. Stops recursively when the shared found flag is set. */
// FUNCTION: LEGOLAND 0x0041c940
void SearchBoatPathConnected(int x, int y, int tx, int ty, TileId *owner, int *found) {
    struct PathNode *node;
    struct PathNode *next;

    if (*found == 1) {
        return;
    }
    node = FindBoatPathAt(x, y);
    if (node == NULL || node->owner.id != owner->id) {
        return;
    }
    if (x == tx && y == ty) {
        *found = 1;
        return;
    }
    node->visited = 1;
    if ((node->dir_mask & 1) != 0 && (next = FindBoatPathAt(x, y - 5)) != NULL && next->visited == 0) {
        SearchBoatPathConnected(x, y - 5, tx, ty, owner, found);
    }
    if ((node->dir_mask & 2) != 0 && (next = FindBoatPathAt(x + 5, y)) != NULL && next->visited == 0) {
        SearchBoatPathConnected(x + 5, y, tx, ty, owner, found);
    }
    if ((node->dir_mask & 4) != 0 && (next = FindBoatPathAt(x, y + 5)) != NULL && next->visited == 0) {
        SearchBoatPathConnected(x, y + 5, tx, ty, owner, found);
    }
    if ((node->dir_mask & 8) != 0 && (next = FindBoatPathAt(x - 5, y)) != NULL && next->visited == 0) {
        SearchBoatPathConnected(x - 5, y, tx, ty, owner, found);
    }
}

// FUNCTION: LEGOLAND 0x0041caa0
void ComputeBoatPathDistances(unsigned short param_1) {
    struct BoatRideNode *score = BoatRideNodeList;
    struct PathNode *node;
    struct PathNode *tmp;

    for (node = BoatPathList; node != NULL; node = node->next) {
        if (node->owner.id == param_1) {
            node->parent = NULL;
        }
    }
    while (score != NULL && score->id != param_1) {
        score = score->next;
    }
    node = FindBoatPathAt(score->end.pos.x, score->end.pos.y);
    node->dist_to_end = 0;
    node->wave_next = NULL;
    BoatBfsFrontier = node;
    BoatBfsNextFrontier = NULL;
    do {
        ComputeBoatPathDistancesStep(param_1);
        tmp = BoatBfsNextFrontier;
        BoatBfsFrontier = tmp;
        BoatBfsNextFrontier = NULL;
    } while (tmp != NULL);
}

// FUNCTION: LEGOLAND 0x0041cb20
void ComputeBoatPathDistancesStep(short param_1) {
    struct PathNode *p;
    struct PathNode *n1;
    struct PathNode *n2;
    struct PathNode *n3;
    struct PathNode *n4;

    for (p = BoatBfsFrontier; p != NULL; p = p->wave_next) {
        n1 = FindBoatPathAt(p->tile.pos.x, p->tile.pos.y - 5);
        n2 = FindBoatPathAt(p->tile.pos.x + 5, p->tile.pos.y);
        n3 = FindBoatPathAt(p->tile.pos.x, p->tile.pos.y + 5);
        n4 = FindBoatPathAt(p->tile.pos.x - 5, p->tile.pos.y);
        if (n1 != NULL && (short)n1->owner.id == param_1 && n1->parent == NULL) {
            n1->parent = p;
            n1->dist_to_end = p->dist_to_end + 1;
            n1->wave_next = BoatBfsNextFrontier;
            BoatBfsNextFrontier = n1;
        }
        if (n2 != NULL && (short)n2->owner.id == param_1 && n2->parent == NULL) {
            n2->parent = p;
            n2->dist_to_end = p->dist_to_end + 1;
            n2->wave_next = BoatBfsNextFrontier;
            BoatBfsNextFrontier = n2;
        }
        if (n3 != NULL && (short)n3->owner.id == param_1 && n3->parent == NULL) {
            n3->parent = p;
            n3->dist_to_end = p->dist_to_end + 1;
            n3->wave_next = BoatBfsNextFrontier;
            BoatBfsNextFrontier = n3;
        }
        if (n4 != NULL && (short)n4->owner.id == param_1 && n4->parent == NULL) {
            n4->parent = p;
            n4->dist_to_end = p->dist_to_end + 1;
            n4->wave_next = BoatBfsNextFrontier;
            BoatBfsNextFrontier = n4;
        }
    }
}
