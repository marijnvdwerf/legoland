#include <math.h>
#include <stdlib.h>
#include "controller.h"
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "gamemap.h"
#include "image_sprite.h"
#include "jungle_cruise.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "money.h"
#include "obj_instance.h"
#include "objclass.h"
#include "print_sprite.h"
#include "render3d.h"
#include "screens.h"
#include "tilemap.h"

/* Fills JungleCruiseSeatOffsets: for each of the 3 seats, 16 points on an ellipse (radii
 * JungleCruiseSeatRadiusX/Y), one per boat heading frame. */
// FUNCTION: LEGOLAND 0x00432ac0
void InitJungleCruiseSeatOffsets(void) {
    int i;
    float a;

    for (i = 0; i < 16; i++) {
        a = (i * BoatHeadingStepDegrees) * DegToRad;
        JungleCruiseSeatOffsets[0][i].x = (int)(sin(a) * JungleCruiseSeatRadiusX);
        JungleCruiseSeatOffsets[0][i].y = (int)(cos(a) * JungleCruiseSeatRadiusY);
        a = (i * BoatHeadingStepDegrees + Float180 - Float16) * DegToRad;
        JungleCruiseSeatOffsets[1][i].x = (int)(sin(a) * JungleCruiseSeatRadiusX);
        JungleCruiseSeatOffsets[1][i].y = (int)(cos(a) * JungleCruiseSeatRadiusY);
        a = (i * BoatHeadingStepDegrees + Float196) * DegToRad;
        JungleCruiseSeatOffsets[2][i].x = (int)(sin(a) * JungleCruiseSeatRadiusX);
        JungleCruiseSeatOffsets[2][i].y = (int)(cos(a) * JungleCruiseSeatRadiusY);
    }
}

// FUNCTION: LEGOLAND 0x00432b90
int AddJungleRide(TileId tile, struct Bloke *bloke0, struct Bloke *bloke1, struct Bloke *bloke2) {
    struct JungleScore *score;
    struct JungleRide *node = JungleRideList;
    struct JungleRide *fresh;

    for (score = JungleScoreList; score != NULL; score = score->next) {
        if (score->tile.id == tile.id) {
            break;
        }
    }
    for (; node != NULL; node = node->next) {
        if (node->tile.id == tile.id) {
            if (node->cur_x == score->start.pos.x && node->cur_y == score->start.pos.y) {
                return 0;
            }
            if (node->next_x == score->start.pos.x && node->next_y == score->start.pos.y) {
                return 0;
            }
            if (node->field_3e0 == 1) {
                return 0;
            }
        }
    }
    fresh = (struct JungleRide *)malloc(sizeof(struct JungleRide));
    if (fresh == NULL) {
        return 0;
    }
    fresh->next = JungleRideList;
    fresh->tile.id = tile.id;
    fresh->cur_x = tile.pos.x;
    fresh->cur_y = tile.pos.y + 5;
    fresh->next_x = tile.pos.x;
    fresh->next_y = tile.pos.y + 5;
    fresh->from_dir = 1;
    fresh->field_3e0 = 1;
    fresh->field_3e4 = (rand() & 0xf) + 4;
    fresh->blokes[0] = bloke0;
    fresh->blokes[1] = bloke1;
    fresh->blokes[2] = bloke2;
    JungleRideList = fresh;
    memset(fresh->step_offsets, 0xf1, sizeof(fresh->step_offsets));
    memset(fresh->step_frames, 0, sizeof(fresh->step_frames));
    return 1;
}

// FUNCTION: LEGOLAND 0x00432cb0
void RemoveJungleRide(struct JungleRide *ride) {
    struct JungleRide *prev = NULL;
    struct JungleRide *node = JungleRideList;

    while (node != ride) {
        prev = node;
        node = node->next;
        if (node == NULL) {
            return;
        }
    }
    if (node == NULL) {
        return;
    }
    if (prev != NULL) {
        prev->next = node->next;
    } else {
        JungleRideList = node->next;
    }
    free(ride);
}

// FUNCTION: LEGOLAND 0x00432d00
void DrawJungleCruiseBoats(int docked) {
    /* Draws the jungle cruise boats and their passengers. The docked pass (docked != 0) draws the boats in
     * state 1 or 0x10 or on the dock tile's column five or more rows down; the other pass draws the rest.
     * The docked pass also releases the passengers at the end of the ride. */
    struct JungleRide *ride = JungleRideList;
    int tw;
    int th;
    int tw2;
    int th2;
    struct Point off;
    struct Point fwdPos; /* passenger offset, front-to-back pass */
    struct Point backPos; /* passenger offset, back-to-front pass */
    int a;
    int b;
    struct Point base; /* screen position of the boat's tile */
    int dx;
    int dy;
    int row;
    int seat;
    int d;
    unsigned int frame; /* boat overlay sprite index */
    struct Person *person;

    GetTileDimensions(&tw, &th);
    while (ride != NULL) {
        if (docked != 0 ? (ride->field_3e0 == 1 || ride->field_3e0 == 0x10 ||
                              (ride->cur_x == ride->tile.pos.x && (int)ride->cur_y >= ride->tile.pos.y + 5))
                        : !(ride->field_3e0 == 1 || ride->field_3e0 == 0x10 ||
                              (ride->cur_x == ride->tile.pos.x && (int)ride->cur_y >= ride->tile.pos.y + 5))) {
            b = ride->step_offsets[JungleCruiseStep * 2 + 1];
            a = ride->step_offsets[JungleCruiseStep * 2];
            GetTileDimensions(&tw2, &th2);
            dx = (a - b) * tw2 >> 9;
            dy = (a + b) * th2 >> 9;
            base.x = (ride->cur_x - ride->cur_y) * (tw >> 1) - ((tw + 1) >> 1) - (ScrollX >> 8);
            base.y = (ride->cur_x + ride->cur_y) * (th >> 1) - (ScrollY >> 8);

            /* boat hull */
            off.x = JungleCruiseBoats->offset_x[ride->step_frames[JungleCruiseStep] & 0xff] >> 1;
            off.y = JungleCruiseBoats->offset_y[ride->step_frames[JungleCruiseStep] & 0xff] >> 1;
            AdjustOffsetForViewMode(&off);
            ride->screen_x = lpConfig->view_x + dx + off.x + base.x;
            ride->screen_y = lpConfig->view_y + dy + off.y + base.y;
            PrintSprite(JungleCruiseBoats->sprites[ride->step_frames[JungleCruiseStep] & 0xff], ride->screen_x,
                ride->screen_y, 0, 0);

            /* anchor for the passengers */
            off.x = lpConfig->view_x + dx + base.x;
            off.y = lpConfig->view_y + dy + base.y;
            AdjustBlokePosition(&off);

            if ((int)ride->step_frames[JungleCruiseStep] >= 4 && (int)ride->step_frames[JungleCruiseStep] < 12) {
                for (row = 0; row < 3; row++) {
                    d = 0;
                    if ((int)ride->step_frames[JungleCruiseStep] > 8) {
                        if (row == 1) {
                            d = 1;
                        } else if (row == 2) {
                            d = -1;
                        }
                    }
                    seat = row + d;
                    if (ride->blokes[seat] != NULL) {
                        person = Find3DPersonFromBloke(ride->blokes[seat]);
                        fwdPos.x = JungleCruiseSeatOffsets[seat][ride->step_frames[JungleCruiseStep] & 0xf].x + 0x20;
                        fwdPos.y = JungleCruiseSeatOffsets[seat][ride->step_frames[JungleCruiseStep] & 0xf].y + 0x18;
                        switch (seat) {
                        case 0:
                            person->field_44 = ((float)(int)ride->step_frames[JungleCruiseStep] * BoatHeadingStepDegrees +
                                                   Float45) *
                                Float2Pi * FloatNegInv360;
                            break;
                        case 1:
                            person->field_44 = ((float)(int)((ride->step_frames[JungleCruiseStep] + 6) & 0xf) *
                                                       BoatHeadingStepDegrees +
                                                   Float45) *
                                Float2Pi * FloatNegInv360;
                            fwdPos.y -= 0x10;
                            break;
                        case 2:
                            person->field_44 = ((float)(int)((ride->step_frames[JungleCruiseStep] - 6) & 0xf) *
                                                       BoatHeadingStepDegrees +
                                                   Float45) *
                                Float2Pi * FloatNegInv360;
                            fwdPos.y -= 0x10;
                            break;
                        }
                        SetPersonRotation(person, &person->field_40);
                        AdjustOffsetForViewMode(&fwdPos);
                        person->field_1c = fwdPos.x + off.x;
                        person->field_20 = fwdPos.y + off.y;
                        IP_RenderBlokeIn3DNow(ride->blokes[seat]);
                    }
                    /* boat overlays between the passenger rows */
                    if (row == 0 || row == 2) {
                        frame = row == 0 ? ride->step_frames[JungleCruiseStep] + 0x10
                                         : ride->step_frames[JungleCruiseStep] + 0x20;
                        PrintSprite(JungleCruiseBoats->sprites[frame & 0xff], ride->screen_x, ride->screen_y, 0, 0);
                    }
                }
            } else {
                for (row = 2; row >= 0; row--) {
                    d = 0;
                    if ((int)ride->step_frames[JungleCruiseStep] < 8) {
                        if (row == 1) {
                            d = 1;
                        } else if (row == 2) {
                            d = -1;
                        }
                    }
                    seat = row + d;
                    if (ride->blokes[seat] != NULL) {
                        person = Find3DPersonFromBloke(ride->blokes[seat]);
                        backPos.x = JungleCruiseSeatOffsets[seat][ride->step_frames[JungleCruiseStep] & 0xf].x + 0x20;
                        backPos.y = JungleCruiseSeatOffsets[seat][ride->step_frames[JungleCruiseStep] & 0xf].y + 0x18;
                        switch (seat) {
                        case 0:
                            person->field_44 = ((float)(int)ride->step_frames[JungleCruiseStep] * BoatHeadingStepDegrees +
                                                   Float45) *
                                Float2Pi * FloatNegInv360;
                            break;
                        case 1:
                            person->field_44 = ((float)(int)((ride->step_frames[JungleCruiseStep] + 6) & 0xf) *
                                                       BoatHeadingStepDegrees +
                                                   Float45) *
                                Float2Pi * FloatNegInv360;
                            backPos.y -= 0x10;
                            break;
                        case 2:
                            person->field_44 = ((float)(int)((ride->step_frames[JungleCruiseStep] - 6) & 0xf) *
                                                       BoatHeadingStepDegrees +
                                                   Float45) *
                                Float2Pi * FloatNegInv360;
                            backPos.y -= 0x10;
                            break;
                        }
                        SetPersonRotation(person, &person->field_40);
                        AdjustOffsetForViewMode(&backPos);
                        person->field_1c = backPos.x + off.x;
                        person->field_20 = backPos.y + off.y;
                        IP_RenderBlokeIn3DNow(ride->blokes[seat]);
                    }
                    if (row == 0 || row == 1) {
                        frame = row == 0 ? ride->step_frames[JungleCruiseStep] + 0x20
                                         : ride->step_frames[JungleCruiseStep] + 0x10;
                        PrintSprite(JungleCruiseBoats->sprites[frame & 0xff], ride->screen_x, ride->screen_y, 0, 0);
                    }
                }
            }
        }
        /* end of the ride: release the passengers */
        if (ride->field_3e0 == 0x10 && ride->field_3e4 == 2 && JungleCruiseStep == 0x4f && docked != 0 &&
            ride->blokes[0] != NULL) {
            ride->blokes[0]->param_action++;
            ride->blokes[0] = NULL;
            if (ride->blokes[1] != NULL) {
                ride->blokes[1]->param_action++;
                ride->blokes[1] = NULL;
            }
            if (ride->blokes[2] != NULL) {
                ride->blokes[2]->param_action++;
                ride->blokes[2] = NULL;
            }
        }
        ride = ride->next;
    }
}

/* Jungle cruise twin of TagAndCountBoatingSchoolBoats: stamps every boat with the id at param_1 and returns the
 * boat count when it is nonzero. */
// FUNCTION: LEGOLAND 0x004332c0
unsigned int TagAndCountJungleCruiseBoats(unsigned short *param_1) {
    struct JungleRide *node = JungleRideList;
    unsigned int count = 0;
    if (node != NULL) {
        unsigned short *src = param_1;
        while (node != NULL) {
            unsigned short value = *src;
            node->tile.id = value;
            if (value != 0) {
                count++;
            }
            node = node->next;
        }
    }
    return count;
}

// FUNCTION: LEGOLAND 0x004332f0
void UpdateJungleRides(void) {
    struct JungleRide *node = JungleRideList;
    struct JungleRide *cur;

    while (node != NULL) {
        node->cur_x = node->next_x;
        node->cur_y = node->next_y;
        if (node->field_3e0 == 0x10) {
            cur = node;
            node = DockJungleCruiseBoat(node);
            if (node != cur) {
                continue;
            }
        } else {
            switch (node->field_3e0) {
            case 1:
                LaunchJungleCruiseBoat(node);
                break;
            case 4:
                SteerJungleCruiseBoat(node, 0);
                break;
            case 8:
                SteerJungleCruiseBoat(node, 1);
                break;
            case 0x10:
                node = DockJungleCruiseBoat(node);
                break;
            }
        }
        if (node == NULL) {
            return;
        }
        node = node->next;
    }
}

/* Jungle cruise boat state 1: leaves the dock heading south and switches to sailing state 4. */
// FUNCTION: LEGOLAND 0x004333b0
void LaunchJungleCruiseBoat(struct JungleRide *param_1) {
    JungleCruiseBuildStepPath(param_1, param_1->from_dir, 4);
    param_1->field_3e0 = 4;
    param_1->next_y = param_1->cur_y + 5;
}

/* Jungle cruise boat state 0x10, one docking step per call; removes the boat when the countdown is done and returns
 * the next one. */
// FUNCTION: LEGOLAND 0x004333e0
struct JungleRide *DockJungleCruiseBoat(struct JungleRide *param_1) {
    unsigned int *p;
    int i;
    struct JungleRide *result;

    if (param_1->field_3e4 == 0) {
        result = param_1->next;
        RemoveJungleRide(param_1);
        return result;
    }
    JungleCruiseBuildStepPath(param_1, 1, 4);
    param_1->from_dir = 1;
    if (param_1->field_3e4 == 3) {
        p = &param_1->step_offsets[0x81];
        i = 0x10;
        do {
            p[-1] = param_1->step_offsets[0x80];
            *p = param_1->step_offsets[0x81];
            p = p + 2;
            i = i + -1;
        } while (i != 0);
    } else if (param_1->field_3e4 == 2) {
        p = &param_1->step_offsets[1];
        i = 0x40;
        do {
            p[-1] = param_1->step_offsets[0x80];
            *p = param_1->step_offsets[0x81];
            p = p + 2;
            i = i + -1;
        } while (i != 0);
    }
    if (param_1->field_3e4 != 3) {
        param_1->next_y = param_1->cur_y + 5;
    }
    if (param_1->field_3e4 == 1) {
        p = &param_1->step_offsets[0x9e];
        i = 7;
        do {
            p[0] = param_1->step_offsets[0x90];
            p[1] = param_1->step_offsets[0x91];
            p = p + -2;
            i = i + -1;
        } while (i != 0);
    }
    param_1->field_3e4 = param_1->field_3e4 - 1;
    return param_1;
}

/* Jungle cruise twin of SteerBoatingSchoolBoat: chooses the next 5-tile move from the path mask, avoiding other
 * boats; param_2 restricts it to the way back. */
// FUNCTION: LEGOLAND 0x004334c0
void SteerJungleCruiseBoat(struct JungleRide *ride, int param_2) {
    struct JungleScore *score = JungleScoreList;
    struct JungleRide *other = JungleRideList;
    struct JunglePath *path;
    unsigned int mask;
    unsigned int free;
    struct Point d;
    int i;
    int n;
    int back;
    int step;
    int dir;

    path = FindJunglePathAt(ride->cur_x, ride->cur_y);
    for (; score != NULL; score = score->next) {
        if (path->owner.id == score->tile.id) {
            break;
        }
    }
    mask = path->dir_mask;
    if (path->tile.id == score->start.id) {
        mask &= ~1;
    } else if (path->tile.id == score->end.id) {
        ride->field_3e0 = 0x10;
        ride->field_3e4 = 3;
        JungleCruiseBuildStepPath(ride, ride->from_dir, 4);
        ride->from_dir = 1;
        ride->next_y = ride->cur_y + 5;
        return;
    }
    for (; other != NULL; other = other->next) {
        if (other == ride) {
            continue;
        }
        if ((ride->cur_x == other->cur_x && ride->cur_y - 5 == other->cur_y) || (ride->cur_x == other->next_x && ride->cur_y - 5 == other->next_y)) {
            mask &= ~1;
        }
        if ((ride->cur_x + 5 == other->cur_x && ride->cur_y == other->cur_y) || (ride->cur_x + 5 == other->next_x && ride->cur_y == other->next_y)) {
            mask &= ~2;
        }
        if ((ride->cur_x == other->cur_x && ride->cur_y + 5 == other->cur_y) || (ride->cur_x == other->next_x && ride->cur_y + 5 == other->next_y)) {
            mask &= ~4;
        }
        if ((ride->cur_x - 5 == other->cur_x && ride->cur_y == other->cur_y) || (ride->cur_x - 5 == other->next_x && ride->cur_y == other->next_y)) {
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
        JungleCruiseBuildStepPath(ride, ride->from_dir, -1);
        ride->from_dir = -1;
        return;
    }
    if ((rand() & 7) == 0 && (free = ~ride->from_dir & mask) != 0) {
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
        if ((ride->from_dir & (1 << i)) != 0) {
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
        ride->next_x = ride->cur_x;
        ride->next_y = ride->cur_y - 5;
        JungleCruiseBuildStepPath(ride, ride->from_dir, dir);
        ride->from_dir = 4;
        break;
    case 2:
        ride->next_x = ride->cur_x + 5;
        ride->next_y = ride->cur_y;
        JungleCruiseBuildStepPath(ride, ride->from_dir, dir);
        ride->from_dir = 8;
        break;
    case 4:
        ride->next_x = ride->cur_x;
        ride->next_y = ride->cur_y + 5;
        JungleCruiseBuildStepPath(ride, ride->from_dir, dir);
        ride->from_dir = 1;
        break;
    case 8:
        ride->next_x = ride->cur_x - 5;
        ride->next_y = ride->cur_y;
        JungleCruiseBuildStepPath(ride, ride->from_dir, dir);
        ride->from_dir = 2;
        break;
    }
    if (--ride->field_3e4 == 0) {
        ride->field_3e0 = 8;
    }
}

// FUNCTION: LEGOLAND 0x00433840
void JungleCruiseBuildStepPath(struct JungleRide *ride, int from, int to) {
    struct BoatArc *arc;
    int *p;
    int i;
    int bit;
    register int idx;
    int sx;
    int sy;
    volatile int tx;
    int ty;
    unsigned int dx;
    int dy;
    float fx;
    float fy;
    float angle;
    float step;
    int ptmp73;
    arc = NULL;

    if (to == -1) {
        if (from == -1) {
            memset(ride->step_offsets, 0, sizeof(ride->step_offsets));
        } else {
            for (bit = 0; bit < 4; bit++) {
                if ((from & (1 << bit)) != 0) {
                    break;
                }
            }
            sx = (int)((float)(JungleCruiseDirSteps[bit].ox * 40) * Float16);
            sy = (int)((float)(JungleCruiseDirSteps[bit].oy * 40) * Float16);
            for (i = 0; i < 80; i++) {
                if (i < 40) {
                    ride->step_offsets[i * 2] = (int)((float)(JungleCruiseDirSteps[bit].dx * i) * Float16 + sx);
                    ride->step_offsets[i * 2 + 1] = (int)((float)(JungleCruiseDirSteps[bit].dy * i) * Float16 + sy);
                } else {
                    ride->step_offsets[i * 2] = 0;
                    ride->step_offsets[i * 2 + 1] = 0;
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
                ride->step_offsets[i * 2] = JungleCruiseDirSteps[idx].dx * 16 + ride->step_offsets[i * 2 - 2];
                ride->step_offsets[i * 2 + 1] = JungleCruiseDirSteps[idx].dy * 16 + ride->step_offsets[i * 2 - 1];
            } else {
                ride->step_offsets[2 * i] = 0;
                ride->step_offsets[i * 2 + 1] = 0;
            }
        }
    } else {
        if (from == 1) {
            from = 0x11;
        }
        if (to == 1) {
            to = 0x11;
        }
        if ((to < from ? to & (from >> 2) : to == from || from & (to >> 2)) == 0) {
            if ((to & (from * 2)) != 0) {
                arc = JungleCruiseArcs;
            } else if ((from & (to * 2)) != 0) {
                arc = JungleCruiseArcsAlt;
            }
            bit = 0;
            while (bit < 4) {
                if ((from & (1 << bit)) != 0) {
                    break;
                }
                bit++;
            }
            arc += bit;
            step = (arc->a1 - arc->a0) * BoatArcStepScale;
            angle = arc->a0;
            ride->step_offsets[0] = (int)((sin(angle * DegToRad) + arc->cx) * BoatArcScale);
            ride->step_offsets[1] = (int)((cos((angle + Float180) * DegToRad) + arc->cy) * BoatArcScale);
            p = &ride->step_offsets[3];
            for (i = 0x4f; i != 0; i--) {
                angle += step;
                p[-1] = (int)((sin(DegToRad * angle) + arc->cx) * BoatArcScale);
                p[0] = (int)((cos((angle + Float180) * DegToRad) + arc->cy) * BoatArcScale);
                p = p + 2;
            }
        } else {
            for (bit = 0; bit < 4; bit++) {
                if ((from & (1 << bit)) != 0) {
                    break;
                }
            }
            sx = (int)((float)(JungleCruiseDirSteps[bit].ox * 40) * Float16);
            sy = (int)((float)(JungleCruiseDirSteps[bit].oy * 40) * Float16);
            if (from == to) {
                for (i = 0; i < 80; i++) {
                    if (i < 40) {
                        ride->step_offsets[i * 2] = (int)((float)(JungleCruiseDirSteps[bit].dx * i) * Float16 + sx);
                        ride->step_offsets[i * 2 + 1] = (int)((float)(JungleCruiseDirSteps[bit].dy * i) * Float16 + sy);
                    } else {
                        ride->step_offsets[i * 2] = (int)((float)(JungleCruiseDirSteps[bit].dx * (80 - i)) * Float16 + sx);
                        ride->step_offsets[i * 2 + 1] = (int)((float)(JungleCruiseDirSteps[bit].dy * (80 - i)) * Float16 + sy);
                    }
                }
            } else {
                fx = (float)sx;
                fy = (float)sy;
                for (i = 0; i < 80; i++) {
                    ride->step_offsets[i * 2] = (int)((float)(JungleCruiseDirSteps[bit].dx * i) * Float16 + fx);
                    ride->step_offsets[i * 2 + 1] = (int)((float)(i * JungleCruiseDirSteps[bit].dy) * Float16 + fy);
                }
            }
        }
    }
    for (i = 0; i < 80; i++) {
        if (i < 76) {
            tx = ride->step_offsets[(i + 4) * 2];
            ty = ride->step_offsets[(i + 4) * 2 + 1];
        } else {
            tx = ride->step_offsets[0x9e];
            ty = ride->step_offsets[0x9f];
        }
        if (i > 3) {
            dx = tx - ride->step_offsets[(i - 3) * 2];
            dy = ty - ride->step_offsets[(i - 3) * 2 + 1];
        } else {
            ptmp73 = ride->step_offsets[0];
            dx = tx - ptmp73;
            dy = ty - ride->step_offsets[1];
        }
        ride->step_frames[i] = ((ArcTan256(dx, dy) >> 4) + 6 & 0xf) + 0;
    }
}

// FUNCTION: LEGOLAND 0x00433ca0
void LoadBrijMaskSprite(Element *obj) {
    struct Ride *ride = obj->ride;
    MonkeyTreeRide = ride;
    ride->flags |= 0x400;
    // STRING: LEGOLAND 0x004b720c
    BrijMaskSprite = LoadSprite("brijmask.lls", 1);
}

// FUNCTION: LEGOLAND 0x00433cd0
void KillBrijMaskSprite(void) {
    KillSprite(BrijMaskSprite);
}

// FUNCTION: LEGOLAND 0x00433ce0
void MonkeyTreeSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = MonkeyTreeRide;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(&MonkeyTreeRide->footprint);
}

// FUNCTION: LEGOLAND 0x00433d20
void MonkeyTreeAddObject(struct Element *param_1, int *param_2) {
    struct JungleObj *obj;
    unsigned char temp[2];
    unsigned short coord;
    int local_4;

    temp[0] = (unsigned char)param_2[0];
    temp[1] = (unsigned char)param_2[1];
    coord = *(unsigned short *)temp;
    GetJunglePathNeighbourMask(param_2[0], param_2[1], (unsigned short *)&local_4);
    obj = (struct JungleObj *)malloc(sizeof(struct JungleObj));
    if (obj != NULL) {
        obj->tile.id = coord;
        obj->owner = (unsigned short)local_4;
        obj->next = JungleObjList;
        JungleObjList = obj;
        AddBasicObject(param_1, param_2);
        AddJungleScore((unsigned short)local_4, 1);
    }
}

// FUNCTION: LEGOLAND 0x00433d90
void JungleCruiseMonkeyTreeCalcCursor(Element *obj, int *param_2, unsigned int param_3) {
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
    mask = GetJunglePathNeighbourMask(EditCursor.tile_x, EditCursor.tile_y, &owner);
    EditCursor.field_1830 = n;
    if (mask == 0) {
        SetCursorBlocked(&EditCursor, 0xe);
        return;
    }
    ValidateCursor(&EditCursor, ride);
    if (IsCursorBuildable(&EditCursor) == 0) {
        return;
    }
    DefaultCursor(&JungleCruiseCursors[0]);
    DefaultCursor(&JungleCruiseCursors[1]);
    DefaultCursor(&JungleCruiseCursors[2]);
    DefaultCursor(&JungleCruiseCursors[3]);
    memcpy(JungleCruiseCursors[0].field_1414, EditCursor.field_1414, 20);
    memcpy(JungleCruiseCursors[1].field_1414, EditCursor.field_1414, 20);
    memcpy(JungleCruiseCursors[2].field_1414, EditCursor.field_1414, 20);
    memcpy(JungleCruiseCursors[3].field_1414, EditCursor.field_1414, 20);
    ResetCursorBuildable(&JungleCruiseCursors[0]);
    ResetCursorBuildable(&JungleCruiseCursors[1]);
    ResetCursorBuildable(&JungleCruiseCursors[2]);
    ResetCursorBuildable(&JungleCruiseCursors[3]);
    x = EditCursor.tile_x;
    y = EditCursor.tile_y;
    JungleCruiseCursors[0].field_1828 = 0x2034;
    JungleCruiseCursors[1].field_1828 = 0x2034;
    JungleCruiseCursors[2].field_1828 = 0x2034;
    JungleCruiseCursors[3].field_1828 = 0x2034;
    if ((mask & 1) != 0) {
        JungleCruiseCursors[0].tile_x = x;
        JungleCruiseCursors[0].tile_y = y - 5;
        n = 1;
    }
    if ((mask & 2) != 0) {
        JungleCruiseCursors[n].tile_x = x + 5;
        JungleCruiseCursors[n].tile_y = y;
        n++;
    }
    if ((mask & 4) != 0) {
        JungleCruiseCursors[n].tile_x = x;
        JungleCruiseCursors[n].tile_y = y + 5;
        n++;
    }
    if ((mask & 8) != 0) {
        JungleCruiseCursors[n].tile_x = x - 5;
        JungleCruiseCursors[n].tile_y = y;
        n++;
    }
    if (n != 0) {
        EditCursor.next = &JungleCruiseCursors[0];
        if (n > 1) {
            c = &JungleCruiseCursors[1];
            n--;
            do {
                c[-1].next = c;
                c++;
                n--;
            } while (n != 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x00433fa0
unsigned int JungleCruiseMonkeyTreeDCalcCursor(struct Element *param_1, struct Point *param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00433fc0
void MonkeyTreeRemoveObject(void *param_1, TileId tile, struct Cursor *param_3) {
    struct JungleObj *node = JungleObjList;
    struct JungleObj *prev = NULL;

    StandardRemoveObject(param_1, tile, param_3);
    while (node->tile.id != tile.id) {
        prev = node;
        node = node->next;
        if (node == NULL) {
            return;
        }
    }
    if (node != NULL) {
        AddJungleScore(node->owner, -1);
        if (prev != NULL) {
            prev->next = node->next;
            free(node);
            return;
        }
        JungleObjList = node->next;
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x00434040
struct RideSpriteInfo *GetMonkeyTreeSpriteInfo(Element *obj, unsigned short param_2) {
    struct Ride *ride = obj->ride;
    RideSpriteInfoBuffer.sprite = ride->layer;
    RideSpriteInfoBuffer.x = ride->field_14;
    RideSpriteInfoBuffer.y = ride->field_18;
    RideSpriteInfoBuffer.id = param_2;
    return &RideSpriteInfoBuffer;
}

// FUNCTION: LEGOLAND 0x00434080
void LoadMFish2Sprite(Element *obj) {
    struct Ride *ride = obj->ride;
    MFish2Ride = ride;
    ride->flags |= 0x400;
    // STRING: LEGOLAND 0x004b721c
    MFish2Sprite = LoadSprite("mfish2.lls", 1);
}

// FUNCTION: LEGOLAND 0x004340b0
void KillMFish2Sprite(void) {
    KillSprite(MFish2Sprite);
}

// FUNCTION: LEGOLAND 0x004340c0
void MonkeyFishSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = MFish2Ride;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(&MFish2Ride->footprint);
}

// FUNCTION: LEGOLAND 0x00434100
void MonkeyFishAddObject(struct Element *obj, int *coords) {
    TileId tile;
    struct JungleFish *fish;
    unsigned short owner;
    int x;
    int y;

    tile.pos.x = coords[0];
    tile.pos.y = coords[1];
    if (GetJunglePathNeighbourMask(coords[0], coords[1], &owner) == 0) {
        GetJunglePathNeighbourMask(coords[0], coords[1] - 5, &owner);
    }
    fish = (struct JungleFish *)malloc(sizeof(struct JungleFish));
    if (fish == NULL) {
        return;
    }
    fish->tile = tile;
    fish->owner = owner;
    fish->next = JungleFishList;
    fish->field_4 = 0;
    JungleFishList = fish;
    AddJungleScore(fish->owner, 2);
    AddBasicObject(obj, coords);
    for (y = MFish2Ride->footprint.v[1]; y <= MFish2Ride->footprint.v[3]; y++) {
        for (x = MFish2Ride->footprint.v[0]; x <= MFish2Ride->footprint.v[2]; x++) {
            if (x == MFish2Ride->footprint.v[0]) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMap->tiles + 9);
            } else if (x == MFish2Ride->footprint.v[2]) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMap->tiles + 0xc);
            } else if (y == MFish2Ride->footprint.v[1]) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMap->tiles + 10);
            } else if (y == MFish2Ride->footprint.v[3]) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMap->tiles + 0xb);
            } else {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMap->tiles);
            }
        }
    }
    SetMapTile(MFish2Ride->footprint.v[0] + coords[0], MFish2Ride->footprint.v[1] + coords[1], *BoatingSchoolTileMap->tiles + 5);
    SetMapTile(MFish2Ride->footprint.v[2] + coords[0], MFish2Ride->footprint.v[1] + coords[1], *BoatingSchoolTileMap->tiles + 8);
    SetMapTile(MFish2Ride->footprint.v[0] + coords[0], MFish2Ride->footprint.v[3] + coords[1], *BoatingSchoolTileMap->tiles + 6);
    SetMapTile(MFish2Ride->footprint.v[2] + coords[0], MFish2Ride->footprint.v[3] + coords[1], *BoatingSchoolTileMap->tiles + 7);
}

// FUNCTION: LEGOLAND 0x00434330
void JungleCruiseMonkeyFishCalcCursor(Element *obj, int *param_2, unsigned int param_3) {
    struct Ride *ride;
    TileId owners[2];
    struct Footprint fp;
    unsigned int mask;
    int row;
    int n;
    int i;

    n = 0;
    row = 0;
    owners[0].pos.x = 0;
    owners[0].pos.y = 0;
    owners[1].pos.x = 0;
    owners[1].pos.y = 0;
    ride = obj->ride;
    memcpy(EditCursor.field_1414, &ride->footprint, sizeof(ride->footprint));
    ScreenToMapRef(param_2, &EditCursor.tile_x, param_3);
    EditCursor.field_1830 = 0;
    for (; row < 2; row++) {
        mask = GetJunglePathNeighbourMask(EditCursor.tile_x, EditCursor.tile_y - row * 5, &owners[row].id);
        if (row == 1 && owners[0].id != owners[1].id && owners[0].pos.x != 0) {
            return;
        }
        if (mask == 0 && row == 1) {
            SetCursorBlocked(&EditCursor, 0xe);
            return;
        }
        ValidateCursor(&EditCursor, ride);
        if (IsCursorBuildable(&EditCursor) != 0) {
            memcpy(&fp, EditCursor.field_1414, sizeof(fp));
            fp.v[1] += 5;
            DefaultCursor(&JungleCruiseMonkeyFishCursors[row * 4]);
            DefaultCursor(&JungleCruiseMonkeyFishCursors[row * 4 + 1]);
            DefaultCursor(&JungleCruiseMonkeyFishCursors[row * 4 + 2]);
            DefaultCursor(&JungleCruiseMonkeyFishCursors[row * 4 + 3]);
            memcpy(JungleCruiseMonkeyFishCursors[row * 4].field_1414, &fp, sizeof(fp));
            memcpy(JungleCruiseMonkeyFishCursors[row * 4 + 1].field_1414, &fp, sizeof(fp));
            memcpy(JungleCruiseMonkeyFishCursors[row * 4 + 2].field_1414, &fp, sizeof(fp));
            memcpy(JungleCruiseMonkeyFishCursors[row * 4 + 3].field_1414, &fp, sizeof(fp));
            ResetCursorBuildable(&JungleCruiseMonkeyFishCursors[row * 4]);
            ResetCursorBuildable(&JungleCruiseMonkeyFishCursors[row * 4 + 1]);
            ResetCursorBuildable(&JungleCruiseMonkeyFishCursors[row * 4 + 2]);
            ResetCursorBuildable(&JungleCruiseMonkeyFishCursors[row * 4 + 3]);
            JungleCruiseMonkeyFishCursors[row * 4].field_1828 = 0x2034;
            JungleCruiseMonkeyFishCursors[row * 4 + 1].field_1828 = 0x2034;
            JungleCruiseMonkeyFishCursors[row * 4 + 2].field_1828 = 0x2034;
            JungleCruiseMonkeyFishCursors[row * 4 + 3].field_1828 = 0x2034;
            if ((mask & 1) != 0) {
                JungleCruiseMonkeyFishCursors[n].tile_x = EditCursor.tile_x;
                JungleCruiseMonkeyFishCursors[n].tile_y = EditCursor.tile_y - (row * 5 + 5);
                n++;
            }
            if ((mask & 2) != 0) {
                JungleCruiseMonkeyFishCursors[n].tile_x = EditCursor.tile_x + 5;
                JungleCruiseMonkeyFishCursors[n].tile_y = EditCursor.tile_y - row * 5;
                n++;
            }
            if ((mask & 4) != 0) {
                JungleCruiseMonkeyFishCursors[n].tile_x = EditCursor.tile_x;
                JungleCruiseMonkeyFishCursors[n].tile_y = EditCursor.tile_y - row * 5 + 5;
                n++;
            }
            if ((mask & 8) != 0) {
                JungleCruiseMonkeyFishCursors[n].tile_x = EditCursor.tile_x - 5;
                JungleCruiseMonkeyFishCursors[n].tile_y = EditCursor.tile_y - row * 5;
                n++;
            }
            if (n != 0) {
                EditCursor.next = &JungleCruiseMonkeyFishCursors[0];
                for (i = 1; i < n; i++) {
                    JungleCruiseMonkeyFishCursors[i - 1].next = &JungleCruiseMonkeyFishCursors[i];
                }
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00434650
unsigned int JungleCruiseMonkeyFishDCalcCursor(struct Element *param_1, struct Point *param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00434670
void MonkeyFishRemoveObject(void *param_1, TileId tile, struct Cursor *param_3) {
    struct JungleFish *node = JungleFishList;
    struct JungleFish *prev = NULL;
    int x;
    int y;

    for (y = MFish2Ride->footprint.v[1]; y <= MFish2Ride->footprint.v[3]; y++) {
        for (x = MFish2Ride->footprint.v[0]; x <= MFish2Ride->footprint.v[2]; x++) {
            RestoreBaseMap(param_3->tile_x + x, param_3->tile_y + y);
        }
    }
    StandardRemoveObject(param_1, tile, param_3);
    while (node->tile.id != tile.id) {
        prev = node;
        node = node->next;
        if (node == NULL) {
            return;
        }
    }
    if (node != NULL) {
        AddJungleScore(node->owner, -2);
        if (prev != NULL) {
            prev->next = node->next;
            free(node);
            return;
        }
        JungleFishList = node->next;
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x00434740
struct RideSpriteInfo *GetMonkeyFishSpriteInfo(Element *obj, unsigned short param_2) {
    struct Ride *ride = obj->ride;
    struct JungleFish *node = JungleFishList;
    short *lls;
    struct LLS *fish_lls;

    while (node != NULL && node->tile.id != (unsigned short)param_2) {
        node = node->next;
    }

    lls = (short *)GetLLSForSprite((struct Sprite *)ride->layer);
    if (node->field_4 != 0) {
        if (*lls == 0) {
            node->field_4 = 0;
        }
    } else if (*lls == 1 && rand() % 7 == 0) {
        node->field_4 = 1;
    }

    if (node->field_4 != 0) {
        fish_lls = GetLLSForSprite((struct Sprite *)MFish2Sprite);
        LLSSetFrame(fish_lls, *lls);
        RideSpriteInfoBuffer.sprite = MFish2Sprite;
    } else {
        RideSpriteInfoBuffer.sprite = ride->layer;
    }
    RideSpriteInfoBuffer.x = ride->field_14;
    RideSpriteInfoBuffer.y = ride->field_18;
    RideSpriteInfoBuffer.id = param_2;
    return &RideSpriteInfoBuffer;
}

/* Removes a jungle cruise instance: standard object removal with a fixed cursor footprint, clears flag 0x40 on the
 * tiles beside it, and unlinks and frees its node from JungleCruiseInstanceList. */
// FUNCTION: LEGOLAND 0x00434b40
void RemoveJungleCruiseInstance(void *param_1, TileId tile, struct Cursor *param_3) {
    struct JungleObj *node = JungleCruiseInstanceList;
    struct JungleObj *prev = NULL;
    struct MapElement *elem;
    int x;
    int y;

    param_3->field_1414[1] = -1;
    param_3->field_1414[3] = 1;
    param_3->field_1414[0] = 0;
    param_3->field_1414[2] = 0;
    StandardRemoveObject(param_1, tile, param_3);
    x = param_3->tile_x - 6;
    y = param_3->tile_y;
    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
        elem = &GameMap[y][x];
    } else {
        elem = NULL;
    }
    elem->flags &= 0xffbf;
    y--;
    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
        elem = &GameMap[y][x];
    } else {
        elem = NULL;
    }
    elem->flags &= 0xffbf;
    y += 2;
    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
        elem = &GameMap[y][x];
    } else {
        elem = NULL;
    }
    elem->flags &= 0xffbf;
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
        JungleCruiseInstanceList = node->next;
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x00434cb0
void LoadJungleCruiseResources(Element *obj) {
    struct Element *handle;
    int i;
    struct Sprite *sprite;

    JungleCruiseRide = obj->ride;
    JungleCruiseRide->flags |= 0x20;
    JungleCruiseRide->layer->flags |= 0x2000;
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", &handle, 0) == 0) {
        BoatingSchoolTileMap = (struct TileMap *)LLIDB_LoadData((void *)handle);
    }
    if (LLIDB_FindElement("JUNGLE CRUISE BOATS", &handle, 0) == 0) {
        JungleCruiseBoats = (struct SpriteSet *)LLIDB_LoadData((void *)handle);
    }
    for (i = 0; i < JungleCruiseBoats->count; i++) {
        sprite = JungleCruiseBoats->sprites[(unsigned char)i];
        LLSPlay(GetLLSForSprite((struct Sprite *)sprite), ((struct Sprite *)sprite)->image);
    }
    // STRING: LEGOLAND 0x004b72b8
    JungMaskSprite = LoadSprite("jungmask.lls", 1);
    JungleCruiseFootprint = JungleCruiseRide->footprint;
    JungleCruiseExtraFootprint = JungleCruiseExtraFootprintTemplate;
    JungleCruiseExtraFootprint.v[1] += JungleCruiseFootprint.v[1];
    JungleCruiseExtraFootprint.v[0] += JungleCruiseFootprint.v[0];
    JungleCruiseExtraFootprint.v[2] += JungleCruiseFootprint.v[0];
    JungleCruiseExtraFootprint.v[3] += JungleCruiseFootprint.v[1];
    JungleCruiseStartFootprint = JungleCruiseStartFootprintTemplate;
    JungleCruiseStartFootprint.v[1] += JungleCruiseFootprint.v[3] + 1;
    JungleCruiseStartFootprint.v[0] += JungleCruiseFootprint.v[0];
    JungleCruiseStartFootprint.v[2] += JungleCruiseFootprint.v[0];
    JungleCruiseStartFootprint.v[3] += JungleCruiseFootprint.v[3] + 1;
    InitJungleCruiseSeatOffsets();
}

// FUNCTION: LEGOLAND 0x00434e50
void UnloadJungleCruiseResources(Element *obj) {
    struct Element *handle;
    int i;
    struct Sprite *sprite;
    struct JungleScore *score;
    struct JunglePath *path;

    JungleCruiseRide = obj->ride;
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", &handle, 0) == 0) {
        LLIDB_UnLoadData(handle);
    }
    for (i = 0; i < JungleCruiseBoats->count; i++) {
        sprite = JungleCruiseBoats->sprites[(unsigned char)i];
        LLSStop(GetLLSForSprite((struct Sprite *)sprite));
    }
    // STRING: LEGOLAND 0x004b72c8
    if (LLIDB_FindElement("JUNGLE CRUISE BOATS", &handle, 0) == 0) {
        LLIDB_UnLoadData(handle);
    }
    while (JungleScoreList != NULL) {
        score = JungleScoreList->next;
        free(JungleScoreList);
        JungleScoreList = score;
    }
    while (JungleRideList != NULL) {
        RemoveJungleRide(JungleRideList);
    }
    while (JunglePathList != NULL) {
        path = JunglePathList->next;
        free(JunglePathList);
        JunglePathList = path;
    }
    KillSprite(JungMaskSprite);
}

// FUNCTION: LEGOLAND 0x00434f50
void JungleCruiseSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = JungleCruiseRide;
    DefaultCursor(&EditCursor);
    JungleCruiseFootprint.next = &JungleCruiseStartFootprint;
    JungleCruiseStartFootprint.next = &JungleCruiseExtraFootprint;
    SetEditCursorFootPrint(&JungleCruiseFootprint);
}

// FUNCTION: LEGOLAND 0x00434f90
void JungleCruiseAddObject(struct Element *obj, int *coords) {
    TileId tile;
    struct JungleScore *score;
    int x;
    int y;

    tile.pos.x = coords[0];
    tile.pos.y = coords[1];
    score = (struct JungleScore *)malloc(sizeof(struct JungleScore));
    if (score == NULL) {
        return;
    }
    score->tile.id = tile.id;
    score->start.pos.x = coords[0] + JungleCruiseStartFootprint.v[0] + 2;
    score->start.pos.y = coords[1] + JungleCruiseStartFootprint.v[1] + 2;
    score->end.pos.x = coords[0] + JungleCruiseExtraFootprint.v[0] + 2;
    score->end.pos.y = coords[1] + JungleCruiseExtraFootprint.v[1] + 2;
    score->connected = 0;
    score->field_c = 9999;
    score->field_10 = 0;
    score->bloke_count = 0;
    score->timer = 150;
    score->field_40 = 3;
    for (x = 0; x < 5; x++) {
        score->blokes[x] = 0;
    }
    for (x = 0; x < 3; x++) {
        score->seats[x] = NULL;
    }
    score->next = JungleScoreList;
    JungleScoreList = score;
    AddBasicObject(obj, coords);
    AddJunglePathTile(coords[0] + JungleCruiseStartFootprint.v[0] + 2, coords[1] + JungleCruiseStartFootprint.v[1] + 2, 1, &score->tile.id);
    AddJunglePathTile(coords[0] + JungleCruiseExtraFootprint.v[0] + 2, coords[1] + JungleCruiseExtraFootprint.v[1] + 2, 4, &score->tile.id);
    for (y = JungleCruiseFootprint.v[1]; y <= JungleCruiseFootprint.v[3]; y++) {
        for (x = JungleCruiseFootprint.v[0]; x <= JungleCruiseFootprint.v[2] - 1; x++) {
            if (x == JungleCruiseFootprint.v[0]) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMap->tiles + 9);
            } else if (x == JungleCruiseFootprint.v[2] - 1) {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMap->tiles + 0xc);
            } else {
                SetMapTile(coords[0] + x, coords[1] + y, *BoatingSchoolTileMap->tiles);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00435150
void JungleCruiseCalcCursor(Element *obj, int *param_2, unsigned int param_3) {
    memcpy(EditCursor.field_1414, &JungleCruiseFootprint, sizeof(JungleCruiseFootprint));
    EditCursor.field_1830 = 0;
    JungleCruiseFootprint.next = &JungleCruiseStartFootprint;
    JungleCruiseStartFootprint.next = &JungleCruiseExtraFootprint;
    JungleCruiseExtraFootprint.v[4] = 0;
    ScreenToMapRef(param_2, &EditCursor.tile_x, param_3);
    ResetCursorBuildable(&EditCursor);
    PathCursor.tile_x = EditCursor.tile_x;
    PathCursor.tile_y = EditCursor.tile_y;
    PathCursor.field_1414[0] = EditCursor.field_1414[2] + 1;
    PathCursor.field_1414[1] = EditCursor.field_1414[1];
    PathCursor.field_1414[2] = PathCursor.field_1414[0];
    PathCursor.field_1414[3] = EditCursor.field_1414[3];
    PathCursor.field_1414[4] = 0;
    PathCursor.field_1828 = 0x1008;
    PathCursor.field_1830 = 0;
    ResetCursorBuildable(&PathCursor);
    EditCursor.next = &PathCursor;
    PathCursor.field_1830 = 0;
    ValidateCursor(&EditCursor, obj->ride);
}

// FUNCTION: LEGOLAND 0x00435230
void JungleCruiseDCalcCursor(struct Element *param_1, struct Point *param_2) {
    struct JunglePath *p1 = JunglePathList;
    struct JungleFish *p3 = JungleFishList;
    struct JungleObj *p4 = JungleCruiseInstanceList;
    struct JungleObj *p2 = JungleObjList;

    BasicObjectDCalcCursor(param_1, param_2);
    DefaultCursor(&BoatingSchoolDCursor);
    memcpy(BoatingSchoolDCursor.field_1414, &JungleCruiseDefaultFootprint, sizeof(JungleCruiseDefaultFootprint));
    for (; p1 != NULL; p1 = p1->next) {
        if (p1->owner.id == QueryObj.id) {
            BoatingSchoolDCursor.tile_x = p1->tile.pos.x;
            BoatingSchoolDCursor.tile_y = p1->tile.pos.y;
            ResetCursorBuildable(&BoatingSchoolDCursor);
            BoatingSchoolDCursor.field_1828 = 8;
            BuildCursorPtr(&BoatingSchoolDCursor, 0, 0);
            RenderCursor(&BoatingSchoolDCursor);
        }
    }
    for (; p2 != NULL; p2 = p2->next) {
        if (p2->owner == QueryObj.id) {
            BoatingSchoolDCursor.tile_x = p2->tile.pos.x;
            BoatingSchoolDCursor.tile_y = p2->tile.pos.y;
            ResetCursorBuildable(&BoatingSchoolDCursor);
            BoatingSchoolDCursor.field_1828 = 8;
            BuildCursorPtr(&BoatingSchoolDCursor, 0, 0);
            RenderCursor(&BoatingSchoolDCursor);
        }
    }
    BoatingSchoolDCursor.field_1414[1] -= 5;
    for (; p3 != NULL; p3 = p3->next) {
        if (p3->owner == QueryObj.id) {
            BoatingSchoolDCursor.tile_x = p3->tile.pos.x;
            BoatingSchoolDCursor.tile_y = p3->tile.pos.y;
            ResetCursorBuildable(&BoatingSchoolDCursor);
            BoatingSchoolDCursor.field_1828 = 8;
            BuildCursorPtr(&BoatingSchoolDCursor, 0, 0);
            RenderCursor(&BoatingSchoolDCursor);
        }
    }
    for (; p4 != NULL; p4 = p4->next) {
        if (p4->owner == QueryObj.id) {
            BoatingSchoolDCursor.field_1414[1] = -1;
            BoatingSchoolDCursor.field_1414[3] = 1;
            BoatingSchoolDCursor.field_1414[0] = 0;
            BoatingSchoolDCursor.field_1414[2] = 0;
            BoatingSchoolDCursor.tile_x = p4->tile.pos.x;
            BoatingSchoolDCursor.tile_y = p4->tile.pos.y;
            ResetCursorBuildable(&BoatingSchoolDCursor);
            BoatingSchoolDCursor.field_1828 = 8;
            BuildCursorPtr(&BoatingSchoolDCursor, 0, 0);
            RenderCursor(&BoatingSchoolDCursor);
            BoatingSchoolDCursor.tile_x = BoatingSchoolDCursor.tile_x - 6;
            BuildCursorPtr(&BoatingSchoolDCursor, 0, 0);
            RenderCursor(&BoatingSchoolDCursor);
        }
    }
}

// FUNCTION: LEGOLAND 0x00435470
void JungleCruiseRemoveObject(Element *obj, TileId tile, struct Cursor *cursor) {
    struct JungleRide *ride = JungleRideList;
    struct JungleScore *prev = NULL;
    struct JungleScore *score = JungleScoreList;
    struct JunglePath *path;
    struct JungleObj *thing;
    struct JungleFish *fish;
    Element fake;
    int x;
    int y;
    int savedX;
    int savedY;

    StandardRemoveObject(obj, tile, cursor);
    for (y = JungleCruiseFootprint.v[1]; y <= JungleCruiseFootprint.v[3]; y++) {
        for (x = JungleCruiseFootprint.v[0]; x <= JungleCruiseFootprint.v[2] - 1; x++) {
            RestoreBaseMap(cursor->tile_x + x, cursor->tile_y + y);
        }
    }
    while (score->tile.id != tile.id) {
        prev = score;
        score = score->next;
        if (score == NULL) {
            return;
        }
    }
    if (score != NULL) {
        fake.ride = ActiveJungleCruiseRide;
        IncrementObjectCount(ActiveJungleCruiseRide);
        IncrementObjectCount(ActiveJungleCruiseRide);
        memcpy(BoatingSchoolDCursor.field_1414, &JungleCruiseDefaultFootprint, sizeof(JungleCruiseDefaultFootprint));
        path = JunglePathList;
        while (path != NULL) {
            if (path->owner.id == tile.id) {
                BoatingSchoolDCursor.tile_x = path->tile.pos.x;
                BoatingSchoolDCursor.tile_y = path->tile.pos.y;
                RemoveJunglePath(&fake, path->tile, &BoatingSchoolDCursor);
                path = JunglePathList;
            } else {
                path = path->next;
            }
        }
        fake.ride = MonkeyTreeRide;
        thing = JungleObjList;
        while (thing != NULL) {
            if (thing->owner == tile.id) {
                savedX = cursor->tile_x;
                savedY = cursor->tile_y;
                cursor->tile_x = thing->tile.pos.x;
                cursor->tile_y = thing->tile.pos.y;
                MonkeyTreeRemoveObject(&fake, thing->tile, cursor);
                cursor->tile_x = savedX;
                cursor->tile_y = savedY;
                thing = JungleObjList;
            } else {
                thing = thing->next;
            }
        }
        fake.ride = MFish2Ride;
        fish = JungleFishList;
        while (fish != NULL) {
            if (fish->owner == tile.id) {
                savedX = cursor->tile_x;
                savedY = cursor->tile_y;
                cursor->tile_x = fish->tile.pos.x;
                cursor->tile_y = fish->tile.pos.y;
                MonkeyFishRemoveObject(&fake, fish->tile, cursor);
                cursor->tile_x = savedX;
                cursor->tile_y = savedY;
                fish = JungleFishList;
            } else {
                fish = fish->next;
            }
        }
        fake.ride = JungleCruiseInstanceClass;
        thing = JungleCruiseInstanceList;
        while (thing != NULL) {
            if (thing->owner == tile.id) {
                savedX = cursor->tile_x;
                savedY = cursor->tile_y;
                cursor->tile_x = thing->tile.pos.x;
                cursor->tile_y = thing->tile.pos.y;
                RemoveJungleCruiseInstance(&fake, thing->tile, cursor);
                cursor->tile_x = savedX;
                cursor->tile_y = savedY;
                thing = JungleCruiseInstanceList;
            } else {
                thing = thing->next;
            }
        }
        if (prev != NULL) {
            prev->next = score->next;
        } else {
            JungleScoreList = score->next;
        }
        while (ride != NULL) {
            if (ride->tile.id == tile.id) {
                RemoveJungleRide(ride);
                ride = JungleRideList;
            } else {
                ride = ride->next;
            }
        }
        RemoveAllBlokesFromRide(obj->ride, tile);
        free(score);
    }
}

// FUNCTION: LEGOLAND 0x00435750
void JungleCruiseUpdate(void) {
    struct RideNode *node = JungleCruiseRide->riders;
    struct RideNode *next;
    struct JungleScore *boat;
    struct JungleScore *queue;
    struct Bloke *bloke;
    struct Person *person;
    TileId tile;
    int slot;
    int i;
    char dir;
    struct Point pos;
    struct Point map;

    if (++JungleCruiseStep == 0x50) {
        JungleCruiseStep = 0;
        UpdateJungleRides();
    }
    DrawJungleCruiseBoats(0);
    /* launch boats whose seats are filled and whose wait timer ran out */
    for (boat = JungleScoreList; boat != NULL; boat = boat->next) {
        tile.id = boat->tile.id;
        if (boat->seats[0] != NULL && --boat->timer <= 0 && boat->connected != 0 &&
            boat->field_40 > (int)TagAndCountJungleCruiseBoats(&boat->tile.id) * 6 &&
            AddJungleRide(tile, boat->seats[0], boat->seats[1], boat->seats[2]) != 0) {
            boat->seats[0]->flags |= 0x80;
            boat->seats[0]->param_action++;
            BlokeSitAnim(boat->seats[0]);
            BlokeSetFrame(boat->seats[0], 0);
            if (boat->seats[1] != NULL) {
                boat->seats[1]->flags |= 0x80;
                boat->seats[1]->param_action++;
                BlokeSitAnim(boat->seats[1]);
                BlokeSetFrame(boat->seats[1], 0);
            }
            if (boat->seats[2] != NULL) {
                boat->seats[2]->flags |= 0x80;
                boat->seats[2]->param_action++;
                BlokeSitAnim(boat->seats[2]);
                BlokeSetFrame(boat->seats[2], 0);
            }
            boat->timer = 150;
            boat->seats[0] = NULL;
            boat->seats[1] = NULL;
            boat->seats[2] = NULL;
        }
    }
    /* walk every rider through the queue, the boat and the exit */
    for (; node != NULL; node = next) {
        queue = JungleScoreList;
        next = node->next;
        tile = node->tile;
        while (queue != NULL && queue->tile.id != tile.id) {
            queue = queue->next;
        }
        bloke = node->rider;
        if (bloke->low_level_action != 0) {
            continue;
        }
        switch (bloke->param_action) {
        case 0:
            /* join the queue, or move one place up it */
            slot = 4;
            for (i = 0; i < 5; i++) {
                if (queue->blokes[i] == bloke) {
                    slot = i;
                    break;
                }
            }
            if (i == 5) {
                /* queue full: the rider gives up */
                if (queue->bloke_count == 5 || queue->blokes[4] != NULL) {
                    RemoveBlokeFromRide(JungleCruiseRide, node);
                    break;
                }
                queue->blokes[slot] = bloke;
                queue->bloke_count++;
            } else {
                bloke = queue->blokes[slot];
                if (queue->blokes[slot - 1] != NULL) {
                    break;
                }
                queue->blokes[slot - 1] = bloke;
                queue->blokes[slot] = NULL;
                if (--slot == 0) {
                    bloke->param_action++;
                }
            }
            bloke->flags |= 8;
            bloke->dest.x = ((JungleCruiseRide->x + tile.pos.x) << 8) + JungleCruiseQueueOffsets[4 - slot].x;
            bloke->dest.y = ((JungleCruiseRide->y + tile.pos.y) << 8) + JungleCruiseQueueOffsets[4 - slot].y;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            break;
        case 1:
            /* head of the queue: take a free boat seat */
            if (bloke == queue->blokes[0]) {
                for (i = 0; i < 3; i++) {
                    if (queue->seats[i] == NULL) {
                        bloke->pos.x = -9999;
                        bloke->pos.y = -9999;
                        queue->seats[i] = bloke;
                        queue->blokes[0] = NULL;
                        queue->bloke_count--;
                        break;
                    }
                }
            }
            break;
        case 3:
            /* back from the cruise: step off the boat */
            pos.x = 0;
            pos.y = 0;
            BlokeWalkAnim(bloke);
            BlokeSetFrame(bloke, 0);
            person = Find3DPersonFromBloke(bloke);
            AdjustBlokePosition(&pos);
            pos.x = person->field_1c - pos.x - 0x10;
            pos.y = person->field_20 - pos.y;
            ScreenToMapRef2(&pos, &map, 0);
            bloke->pos = map;
            bloke->flags &= 0xff7f;
            bloke->dir = 10;
            bloke->dest.x = ((JungleCruiseRide->field_24 + tile.pos.x) << 8) - 0x180;
            bloke->dest.y = ((JungleCruiseRide->field_25 + tile.pos.y) << 8) + 0x80;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->low_level_action = 7;
            bloke->target_dir = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 4:
            bloke->dest.x = ((JungleCruiseRide->field_24 + tile.pos.x) << 8) + 0x80;
            bloke->dest.y = ((JungleCruiseRide->field_25 + tile.pos.y) << 8) + 0x80;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->low_level_action = 7;
            bloke->target_dir = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 5:
            bloke->flags &= 0xfff7;
            RemoveBlokeFromRide(JungleCruiseRide, node);
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x00435bd0
void RenderJungleCruise(struct Element *param_1, unsigned int param_2, unsigned int param_3, short *param_4, void *param_5, unsigned int param_6) {
    struct Ride *ride = param_1->ride;
    struct RideNode *node = ride->riders;
    short *lls1;
    struct LLS *lls2;
    struct Point coords;

    DrawJungleCruiseBoats(1);
    for (; node != NULL; node = node->next) {
        if (*param_4 == *(short *)&node->tile && node->rider->param_action != 2) {
            IP_RenderBlokeIn3DNow(node->rider);
        }
    }
    coords = GetScreenCoordsForObject((TileId *)param_4, ride);
    lls1 = (short *)GetLLSForSprite((struct Sprite *)JungleCruiseRide->layer);
    lls2 = GetLLSForSprite((struct Sprite *)JungMaskSprite);
    LLSSetFrame(lls2, *lls1);
    PrintSprite(JungMaskSprite, coords.x, coords.y, param_6, 0);
}

// FUNCTION: LEGOLAND 0x00435c70
int JungleCruise_Save(struct Element *element) {
    struct JungleScore *score;
    struct JungleScore *scoreCur;
    struct JunglePath *path;
    struct JunglePath *pathCur;
    struct JungleFish *fish;
    struct JungleFish *fishCur;
    struct JungleObj *thing;
    struct JungleObj *thingCur;
    struct JungleRide *ride;
    struct JungleRide *rideCur;
    int count;
    int i;
    struct JungleScore scoreCopy;
    struct JungleRide rideCopy;

    count = 0;
    for (scoreCur = JungleScoreList; scoreCur != NULL; scoreCur = scoreCur->next) {
        count++;
    }
    score = JungleScoreList;
    SaveGameWrite(&count, 4);
    while (count--) {
        scoreCopy = *score;
        for (i = 0; i < 5; i++) {
            scoreCopy.blokes[i] = (struct Bloke *)(INT_PTR)GetBlokeNum(scoreCopy.blokes[i]);
        }
        SaveGameWrite(&scoreCopy, sizeof(scoreCopy));
        score = score->next;
    }
    count = 0;
    for (pathCur = JunglePathList; pathCur != NULL; pathCur = pathCur->next) {
        count++;
    }
    path = JunglePathList;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(path, sizeof(*path));
        path = path->next;
    }
    count = 0;
    for (fishCur = JungleFishList; fishCur != NULL; fishCur = fishCur->next) {
        count++;
    }
    fish = JungleFishList;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(fish, sizeof(*fish));
        fish = fish->next;
    }
    count = 0;
    for (thingCur = JungleObjList; thingCur != NULL; thingCur = thingCur->next) {
        count++;
    }
    thing = JungleObjList;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(thing, sizeof(*thing));
        thing = thing->next;
    }
    count = 0;
    for (rideCur = JungleRideList; rideCur != NULL; rideCur = rideCur->next) {
        count++;
    }
    ride = JungleRideList;
    SaveGameWrite(&count, 4);
    while (count--) {
        rideCopy = *ride;
        for (i = 0; i < 3; i++) {
            rideCopy.blokes[i] = (struct Bloke *)(INT_PTR)GetBlokeNum(rideCopy.blokes[i]);
        }
        SaveGameWrite(&rideCopy, sizeof(rideCopy));
        ride = ride->next;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00435ec0
int JungleCruise_Load(struct Element *element) {
    struct JungleScore *score;
    struct JungleScore *prevScore;
    struct JunglePath *path;
    struct JunglePath *prevPath;
    struct JungleFish *fish;
    struct JungleFish *prevFish;
    struct JungleObj *thing;
    struct JungleObj *prevThing;
    struct JungleRide *ride;
    struct JungleRide *prevRide;
    int count;
    int i;

    prevScore = NULL;
    prevPath = NULL;
    prevThing = NULL;
    prevFish = NULL;
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevScore == NULL) {
            JungleScoreList = (struct JungleScore *)malloc(sizeof(struct JungleScore));
            prevScore = JungleScoreList;
        } else {
            score = (struct JungleScore *)malloc(sizeof(struct JungleScore));
            prevScore->next = score;
            prevScore = score;
        }
        SaveGameRead(prevScore, sizeof(struct JungleScore));
        for (i = 0; i < 5; i++) {
            prevScore->blokes[i] = GetBlokePtr((int)(INT_PTR)prevScore->blokes[i]);
        }
    }
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevPath == NULL) {
            JunglePathList = (struct JunglePath *)malloc(sizeof(struct JunglePath));
            prevPath = JunglePathList;
        } else {
            path = (struct JunglePath *)malloc(sizeof(struct JunglePath));
            prevPath->next = path;
            prevPath = path;
        }
        SaveGameRead(prevPath, sizeof(struct JunglePath));
    }
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevFish == NULL) {
            JungleFishList = (struct JungleFish *)malloc(sizeof(struct JungleFish));
            prevFish = JungleFishList;
        } else {
            fish = (struct JungleFish *)malloc(sizeof(struct JungleFish));
            prevFish->next = fish;
            prevFish = fish;
        }
        SaveGameRead(prevFish, sizeof(struct JungleFish));
    }
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevThing == NULL) {
            JungleObjList = (struct JungleObj *)malloc(sizeof(struct JungleObj));
            prevThing = JungleObjList;
        } else {
            thing = (struct JungleObj *)malloc(sizeof(struct JungleObj));
            prevThing->next = thing;
            prevThing = thing;
        }
        SaveGameRead(prevThing, sizeof(struct JungleObj));
    }
    prevRide = JungleRideList;
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevRide == NULL) {
            JungleRideList = (struct JungleRide *)malloc(sizeof(struct JungleRide));
            prevRide = JungleRideList;
        } else {
            ride = (struct JungleRide *)malloc(sizeof(struct JungleRide));
            prevRide->next = ride;
            prevRide = ride;
        }
        SaveGameRead(prevRide, sizeof(struct JungleRide));
        for (i = 0; i < 3; i++) {
            prevRide->blokes[i] = GetBlokePtr((int)(INT_PTR)prevRide->blokes[i]);
        }
    }
    for (score = JungleScoreList; score != NULL; score = score->next) {
        ComputeJunglePathDistances(score->tile.id);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00436130
void AddJungleScore(unsigned short param_1, int param_2) {
    struct JungleScore *node = JungleScoreList;
    while (node != NULL) {
        if (node->tile.id == param_1) {
            if (node != NULL) {
                node->field_40 += param_2;
            }
            return;
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00436160
int GetBestJungleScore(Element *obj, int param_2) {
    struct JungleScore *node;
    int best = 0;

    for (node = JungleScoreList; node != NULL; node = node->next) {
        if ((int)node->field_40 > best && (param_2 == 0 || node->connected != 0)) {
            best = node->field_40;
        }
    }
    return best;
}

// FUNCTION: LEGOLAND 0x00436190
void InitJungleCruiseWater(Element *obj) {
    ActiveJungleCruiseRide = obj->ride;
}

// FUNCTION: LEGOLAND 0x004361a0
void JungleCruiseWaterSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = ActiveJungleCruiseRide;
    ActiveJungleCruiseRide->footprint = JungleCruiseDefaultFootprint;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00436200
void JungleCruiseWaterCalcCursor(Element *obj, int *param_2, unsigned int param_3) {
    unsigned int mask;
    unsigned short owner;
    int n;
    int x;
    int y;
    int result;
    struct Cursor *c;
    struct MapRect rect;

    n = 0;
    memcpy(EditCursor.field_1414, &JungleCruiseDefaultFootprint, sizeof(JungleCruiseDefaultFootprint));
    ScreenToMapRef(param_2, &EditCursor.tile_x, param_3);
    mask = GetJunglePathNeighbourMask(EditCursor.tile_x, EditCursor.tile_y, &owner);
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
                    DefaultCursor(&JungleCruiseWaterCursors[0]);
                    DefaultCursor(&JungleCruiseWaterCursors[1]);
                    DefaultCursor(&JungleCruiseWaterCursors[2]);
                    DefaultCursor(&JungleCruiseWaterCursors[3]);
                    memcpy(JungleCruiseWaterCursors[0].field_1414, EditCursor.field_1414, 20);
                    memcpy(JungleCruiseWaterCursors[1].field_1414, EditCursor.field_1414, 20);
                    memcpy(JungleCruiseWaterCursors[2].field_1414, EditCursor.field_1414, 20);
                    memcpy(JungleCruiseWaterCursors[3].field_1414, EditCursor.field_1414, 20);
                    ResetCursorBuildable(&JungleCruiseWaterCursors[0]);
                    ResetCursorBuildable(&JungleCruiseWaterCursors[1]);
                    ResetCursorBuildable(&JungleCruiseWaterCursors[2]);
                    ResetCursorBuildable(&JungleCruiseWaterCursors[3]);
                    x = EditCursor.tile_x;
                    y = EditCursor.tile_y;
                    JungleCruiseWaterCursors[0].field_1828 = 0x2034;
                    JungleCruiseWaterCursors[1].field_1828 = 0x2034;
                    JungleCruiseWaterCursors[2].field_1828 = 0x2034;
                    JungleCruiseWaterCursors[3].field_1828 = 0x2034;
                    if ((mask & 1) != 0) {
                        JungleCruiseWaterCursors[0].tile_x = x;
                        JungleCruiseWaterCursors[0].tile_y = y - 5;
                        n = 1;
                    }
                    if ((mask & 2) != 0) {
                        JungleCruiseWaterCursors[n].tile_x = x + 5;
                        JungleCruiseWaterCursors[n].tile_y = y;
                        n++;
                    }
                    if ((mask & 4) != 0) {
                        JungleCruiseWaterCursors[n].tile_x = x;
                        JungleCruiseWaterCursors[n].tile_y = y + 5;
                        n++;
                    }
                    if ((mask & 8) != 0) {
                        JungleCruiseWaterCursors[n].tile_x = x - 5;
                        JungleCruiseWaterCursors[n].tile_y = y;
                        n++;
                    }
                    if (n != 0) {
                        EditCursor.next = &JungleCruiseWaterCursors[0];
                        if (n > 1) {
                            c = &JungleCruiseWaterCursors[1];
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

// FUNCTION: LEGOLAND 0x00436470
void JungleCruiseWaterDCalcCursor(struct Element *param_1, int *coords) {
    struct JungleScore *score = JungleScoreList;
    struct JungleRide *ride;
    struct MapElement *elem;
    struct JunglePath *path;
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
            path = FindJunglePathAt(coords[0], coords[1]);
            QueryObj.pos.x = path->owner.pos.x;
            coords[0] = QueryObj.pos.x;
            QueryObj.pos.y = path->owner.pos.y;
            coords[1] = QueryObj.pos.y;
            memcpy(&QueryClass->footprint, &JungleCruiseFootprint, sizeof(JungleCruiseFootprint));
            fake.ride = JungleCruiseRide;
            JungleCruiseDCalcCursor((struct Element *)&fake, (struct Point *)coords);
            return;
        }
    }
    ride = JungleRideList;
    memcpy(&QueryClass->footprint, &JungleCruiseDefaultFootprint, sizeof(JungleCruiseDefaultFootprint));
    BasicObjectDCalcCursor(param_1, (struct Point *)coords);
    for (; ride != NULL; ride = ride->next) {
        if ((tile.pos.x == ride->cur_x && tile.pos.y == ride->cur_y) || (tile.pos.x == ride->next_x && tile.pos.y == ride->next_y)) {
            SetCursorBlocked(&QueryCursor, 1);
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x004365f0
void JungleCruiseWaterAddObject(Element *obj, int *coords) {
    struct JungleScore *score = JungleScoreList;
    unsigned int mask;
    unsigned short owner;
    int x0;
    int y0;
    int x1;
    int y1;

    mask = GetJunglePathNeighbourMask(coords[0], coords[1], &owner);
    AddJunglePathTile(coords[0], coords[1], mask, &owner);
    IncrementObjectCount(obj->ride);
    FillJungleWaterCorners(coords[0], coords[1], &owner);
    if ((mask & 1) != 0) {
        AddJunglePathTile(coords[0], coords[1] - 5, GetJunglePathNeighbourMask(coords[0], coords[1] - 5, &owner), NULL);
        FillJungleWaterCorners(coords[0], coords[1] - 5, &owner);
    }
    if ((mask & 2) != 0) {
        AddJunglePathTile(coords[0] + 5, coords[1], GetJunglePathNeighbourMask(coords[0] + 5, coords[1], &owner), NULL);
        FillJungleWaterCorners(coords[0] + 5, coords[1], &owner);
    }
    if ((mask & 4) != 0) {
        AddJunglePathTile(coords[0], coords[1] + 5, GetJunglePathNeighbourMask(coords[0], coords[1] + 5, &owner), NULL);
        FillJungleWaterCorners(coords[0], coords[1] + 5, &owner);
    }
    if ((mask & 8) != 0) {
        AddJunglePathTile(coords[0] - 5, coords[1], GetJunglePathNeighbourMask(coords[0] - 5, coords[1], &owner), NULL);
        FillJungleWaterCorners(coords[0] - 5, coords[1], &owner);
    }
    for (; score != NULL; score = score->next) {
        if (score->tile.id == owner) {
            x0 = score->start.pos.x;
            y0 = score->start.pos.y;
            x1 = score->end.pos.x;
            y1 = score->end.pos.y;
            score->connected = AreJunglePathsConnected(x0, y0, x1, y1);
            if (score->connected != 0) {
                ComputeJunglePathDistances(owner);
            }
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x004367b0
void FillJungleWaterCorners(int param_1, int param_2, unsigned short *param_3) {
    struct JungleScore *score = JungleScoreList;
    struct JunglePath *path;
    struct JunglePath *other;
    unsigned int mask;

    path = FindJunglePathAt(param_1, param_2);
    for (; score != NULL; score = score->next) {
        if (score->tile.id == *param_3) {
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
            (other = FindJunglePathAt(param_1, (param_2 - 5)), (*(unsigned char *)&other->dir_mask & 8) != 0)) {
            SetMapTile(param_1 - 3, param_2 - 3, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 - 2, param_2 - 3, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 - 3, param_2 - 2, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 - 2, param_2 - 2, *BoatingSchoolTileMap->tiles);
        }
        if ((mask & 8) != 0 && (mask & 4) != 0 &&
            (other = FindJunglePathAt(param_1, (param_2 + 5)), (*(unsigned char *)&other->dir_mask & 8) != 0)) {
            SetMapTile(param_1 - 3, param_2 + 3, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 - 2, param_2 + 3, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 - 3, param_2 + 2, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 - 2, param_2 + 2, *BoatingSchoolTileMap->tiles);
        }
        if ((mask & 2) != 0 && (mask & 1) != 0 &&
            (other = FindJunglePathAt(param_1, (param_2 - 5)), (*(unsigned char *)&other->dir_mask & 2) != 0)) {
            SetMapTile(param_1 + 3, param_2 - 3, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 + 2, param_2 - 3, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 + 3, param_2 - 2, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 + 2, param_2 - 2, *BoatingSchoolTileMap->tiles);
        }
        if ((mask & 2) != 0 && (mask & 4) != 0 &&
            (other = FindJunglePathAt(param_1, (param_2 + 5)), (*(unsigned char *)&other->dir_mask & 2) != 0)) {
            SetMapTile(param_1 + 3, param_2 + 3, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 + 2, param_2 + 3, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 + 3, param_2 + 2, *BoatingSchoolTileMap->tiles);
            SetMapTile(param_1 + 2, param_2 + 2, *BoatingSchoolTileMap->tiles);
        }
    }
}

// FUNCTION: LEGOLAND 0x00436a40
void JungleCruiseWaterRemoveObject(Element *obj, TileId tile, struct Cursor *cursor) {
    struct JungleScore *score = JungleScoreList;
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
    if (elem->field_0 != ActiveJungleCruiseRide->element) {
        fake.ride = JungleCruiseRide;
        JungleCruiseRemoveObject(&fake, tile, cursor);
        return;
    }
    mask = GetJunglePathNeighbourMask(cursor->tile_x, cursor->tile_y, &owner);
    RemoveJunglePath(obj, tile, cursor);
    if ((mask & 1) != 0) {
        y = cursor->tile_y - 5;
        x = cursor->tile_x;
        dir = GetJunglePathNeighbourMask(x, y, &other);
        AddJunglePathTile(x, y, dir, &owner);
        FillJungleWaterCorners(x, y, &owner);
    }
    if ((mask & 2) != 0) {
        x = cursor->tile_x + 5;
        y = cursor->tile_y;
        dir = GetJunglePathNeighbourMask(x, y, &other);
        AddJunglePathTile(x, y, dir, &owner);
        FillJungleWaterCorners(x, y, &owner);
    }
    if ((mask & 4) != 0) {
        y = cursor->tile_y + 5;
        x = cursor->tile_x;
        dir = GetJunglePathNeighbourMask(x, y, &other);
        AddJunglePathTile(x, y, dir, &owner);
        FillJungleWaterCorners(x, y, &owner);
    }
    if ((mask & 8) != 0) {
        x = cursor->tile_x - 5;
        y = cursor->tile_y;
        dir = GetJunglePathNeighbourMask(x, y, &other);
        AddJunglePathTile(x, y, dir, &owner);
        FillJungleWaterCorners(x, y, &owner);
    }
    if ((mask & 1) != 0 && (mask & 8) != 0) {
        x = cursor->tile_x - 5;
        y = cursor->tile_y - 5;
        if (FindJunglePathAt(x, y) != NULL) {
            dir = GetJunglePathNeighbourMask(x, y, &other);
            AddJunglePathTile(x, y, dir, &owner);
            FillJungleWaterCorners(x, y, &owner);
        }
    }
    if ((mask & 1) != 0 && (mask & 2) != 0) {
        x = cursor->tile_x + 5;
        y = cursor->tile_y - 5;
        if (FindJunglePathAt(x, y) != NULL) {
            dir = GetJunglePathNeighbourMask(x, y, &other);
            AddJunglePathTile(x, y, dir, &owner);
            FillJungleWaterCorners(x, y, &owner);
        }
    }
    if ((mask & 4) != 0 && (mask & 8) != 0) {
        x = cursor->tile_x - 5;
        y = cursor->tile_y + 5;
        if (FindJunglePathAt(x, y) != NULL) {
            dir = GetJunglePathNeighbourMask(x, y, &other);
            AddJunglePathTile(x, y, dir, &owner);
            FillJungleWaterCorners(x, y, &owner);
        }
    }
    if ((mask & 4) != 0 && (mask & 2) != 0) {
        x = cursor->tile_x + 5;
        y = cursor->tile_y + 5;
        if (FindJunglePathAt(x, y) != NULL) {
            dir = GetJunglePathNeighbourMask(x, y, &other);
            AddJunglePathTile(x, y, dir, &owner);
            FillJungleWaterCorners(x, y, &owner);
        }
    }
    ComputeJunglePathDistances(owner);
    for (; score != NULL; score = score->next) {
        if (score->tile.id == owner) {
            x0 = score->start.pos.x;
            y0 = score->start.pos.y;
            x1 = score->end.pos.x;
            y1 = score->end.pos.y;
            score->connected = AreJunglePathsConnected(x0, y0, x1, y1);
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x00436dc0
void AddJunglePathTile(int x, int y, int mask, unsigned short *owner) {
    TileId tile;
    struct JunglePath *node;
    struct MapElement *elem;
    int row;
    int col;
    struct Point pt;

    tile.pos.x = x;
    tile.pos.y = y;
    node = FindJunglePathAt(x, y);
    if (node == NULL) {
        node = (struct JunglePath *)malloc(sizeof(struct JunglePath));
        if (node == NULL) {
            return;
        }
        node->next = JunglePathList;
        node->parent = NULL;
        JunglePathList = node;
        AddJungleScore(*owner, 1);
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
            elem->field_0 = ActiveJungleCruiseRide->element;
            *(unsigned short *)&elem->field_4 = tile.id;
            SetMapTile(pt.x, pt.y, *BoatingSchoolTileMap[JunglePathTilePatterns[mask * 25 + row * 5 + col] >> 8].tiles + (unsigned char)JunglePathTilePatterns[mask * 25 + row * 5 + col]);
        }
    }
}

// FUNCTION: LEGOLAND 0x00436f30
void RemoveJunglePath(void *param_1, TileId tile, struct Cursor *param_3) {
    struct JunglePath *node = JunglePathList;
    struct JunglePath *prev = NULL;

    StandardRemoveObject(param_1, tile, param_3);
    while (node->tile.id != tile.id) {
        prev = node;
        node = node->next;
        if (node == NULL) {
            return;
        }
    }
    if (node != NULL) {
        AddJungleScore(node->owner.id, -1);
        if (prev != NULL) {
            prev->next = node->next;
            free(node);
            return;
        }
        JunglePathList = node->next;
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x00436fb0
unsigned int GetJunglePathNeighbourMask(int x, int y, unsigned short *owner) {
    struct JungleScore *score;
    struct JunglePath *node;
    unsigned int mask;
    int valid;
    int n;
    TileId key;

    mask = 0;
    valid = 0;
    score = JungleScoreList;
    node = FindJunglePathAt(x, y);
    if (node != NULL) {
        *owner = node->owner.id;
        valid = 1;
    }
    n = y - 5;
    if (x >= 0 && n >= 0 && x < lpConfig->width && n < lpConfig->height && (node = FindJunglePathAt(x, n)) != NULL) {
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
    if (n >= 0 && y >= 0 && n < lpConfig->width && y < lpConfig->height && (node = FindJunglePathAt(n, y)) != NULL) {
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
    if (x >= 0 && n >= 0 && x < lpConfig->width && n < lpConfig->height && (node = FindJunglePathAt(x, n)) != NULL) {
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
    if (n >= 0 && y >= 0 && n < lpConfig->width && y < lpConfig->height && (node = FindJunglePathAt(n, y)) != NULL) {
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

// FUNCTION: LEGOLAND 0x004371b0
struct JunglePath *FindJunglePathAt(int x, int y) {
    struct JunglePath *node;
    TileId key;

    key.pos.x = x;
    key.pos.y = y;
    for (node = JunglePathList; node != NULL; node = node->next) {
        if (node->tile.id == key.id) {
            break;
        }
    }
    return node;
}

// FUNCTION: LEGOLAND 0x004371e0
int AreJunglePathsConnected(int a, int b, int c, int d) {
    struct JunglePath *node;
    TileId key;
    int result;

    result = 0;
    for (node = JunglePathList; node != NULL; node = node->next) {
        node->visited = 0;
    }
    node = FindJunglePathAt(a, b);
    if (node == NULL) {
        return 0;
    }
    key.id = node->owner.id;
    SearchJunglePathConnected(a, b, c, d, &key, &result);
    return result;
}

// FUNCTION: LEGOLAND 0x00437260
void SearchJunglePathConnected(int x, int y, int tx, int ty, TileId *owner, int *found) {
    struct JunglePath *node;
    struct JunglePath *next;

    if (*found == 1) {
        return;
    }
    node = FindJunglePathAt(x, y);
    if (node == NULL || node->owner.id != owner->id) {
        return;
    }
    if (x == tx && y == ty) {
        *found = 1;
        return;
    }
    node->visited = 1;
    if ((node->dir_mask & 1) != 0 && (next = FindJunglePathAt(x, y - 5)) != NULL && next->visited == 0) {
        SearchJunglePathConnected(x, y - 5, tx, ty, owner, found);
    }
    if ((node->dir_mask & 2) != 0 && (next = FindJunglePathAt(x + 5, y)) != NULL && next->visited == 0) {
        SearchJunglePathConnected(x + 5, y, tx, ty, owner, found);
    }
    if ((node->dir_mask & 4) != 0 && (next = FindJunglePathAt(x, y + 5)) != NULL && next->visited == 0) {
        SearchJunglePathConnected(x, y + 5, tx, ty, owner, found);
    }
    if ((node->dir_mask & 8) != 0 && (next = FindJunglePathAt(x - 5, y)) != NULL && next->visited == 0) {
        SearchJunglePathConnected(x - 5, y, tx, ty, owner, found);
    }
}

// FUNCTION: LEGOLAND 0x004373c0
void ComputeJunglePathDistances(unsigned short param_1) {
    struct JungleScore *score = JungleScoreList;
    struct JunglePath *node;
    struct JunglePath *tmp;

    for (node = JunglePathList; node != NULL; node = node->next) {
        if (node->owner.id == param_1) {
            node->parent = NULL;
        }
    }
    while (score != NULL && score->tile.id != param_1) {
        score = score->next;
    }
    node = FindJunglePathAt(score->end.pos.x, score->end.pos.y);
    node->field_8 = 0;
    node->bfs_next = NULL;
    JungleBfsFrontier = node;
    JungleBfsNextFrontier = NULL;
    do {
        ComputeJunglePathDistancesStep(param_1);
        tmp = JungleBfsNextFrontier;
        JungleBfsFrontier = tmp;
        JungleBfsNextFrontier = NULL;
    } while (tmp != NULL);
}

// FUNCTION: LEGOLAND 0x00437440
void ComputeJunglePathDistancesStep(short param_1) {
    struct JunglePath *p;
    struct JunglePath *n1;
    struct JunglePath *n2;
    struct JunglePath *n3;
    struct JunglePath *n4;

    for (p = JungleBfsFrontier; p != NULL; p = p->bfs_next) {
        n1 = FindJunglePathAt(p->tile.pos.x, p->tile.pos.y - 5);
        n2 = FindJunglePathAt(p->tile.pos.x + 5, p->tile.pos.y);
        n3 = FindJunglePathAt(p->tile.pos.x, p->tile.pos.y + 5);
        n4 = FindJunglePathAt(p->tile.pos.x - 5, p->tile.pos.y);
        if (n1 != NULL && (short)n1->owner.id == param_1 && n1->parent == NULL) {
            n1->parent = p;
            n1->field_8 = p->field_8 + 1;
            n1->bfs_next = JungleBfsNextFrontier;
            JungleBfsNextFrontier = n1;
        }
        if (n2 != NULL && (short)n2->owner.id == param_1 && n2->parent == NULL) {
            n2->parent = p;
            n2->field_8 = p->field_8 + 1;
            n2->bfs_next = JungleBfsNextFrontier;
            JungleBfsNextFrontier = n2;
        }
        if (n3 != NULL && (short)n3->owner.id == param_1 && n3->parent == NULL) {
            n3->parent = p;
            n3->field_8 = p->field_8 + 1;
            n3->bfs_next = JungleBfsNextFrontier;
            JungleBfsNextFrontier = n3;
        }
        if (n4 != NULL && (short)n4->owner.id == param_1 && n4->parent == NULL) {
            n4->parent = p;
            n4->field_8 = p->field_8 + 1;
            n4->bfs_next = JungleBfsNextFrontier;
            JungleBfsNextFrontier = n4;
        }
    }
}

// FUNCTION: LEGOLAND 0x00437570
void BrowseAndBuyItem(struct RideNode *node, Element *obj, TileId *tile, int sfx) {
    struct Bloke *bloke = node->rider;

    if (bloke->field_58 == 0) {
        node->rider->param_action++;
    }
    if ((bloke->field_58-- & 0x1f) == 0 && rand() % 100 <= 30) {
        BuyItem((struct Element *)obj, tile, sfx);
        node->rider->param_action++;
    }
}
