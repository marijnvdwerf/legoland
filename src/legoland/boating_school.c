#include <math.h>
#include <stdlib.h>
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
int FUN_00418e60(TileId tile, unsigned int bloke) {
    struct BoatRideNode *score;
    struct BoatRide *node = DAT_004cc03c;
    struct BoatRide *fresh;

    for (score = DAT_004cc074; score != NULL; score = score->next) {
        if (score->id == tile.id) {
            break;
        }
    }
    for (; node != NULL; node = node->next) {
        if (node->id == tile.id) {
            if (node->field_4 == score->start.pos.x && node->field_8 == score->start.pos.y) {
                return 0;
            }
            if (node->field_c == score->start.pos.x && node->field_10 == score->start.pos.y) {
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
    fresh->next = DAT_004cc03c;
    fresh->id = tile.id;
    fresh->field_4 = tile.pos.x - 1;
    fresh->field_8 = tile.pos.y + 5;
    fresh->field_c = tile.pos.x - 1;
    fresh->field_10 = tile.pos.y + 5;
    fresh->field_3dc = 1;
    fresh->field_3e0 = rand() & 3;
    fresh->field_3e4 = 1;
    fresh->field_3e8 = (rand() & 0xf) + 4;
    fresh->field_3ec = bloke;
    DAT_004cc03c = fresh;
    memset(fresh->field_1c, 0xf1, sizeof(fresh->field_1c));
    memset(fresh->field_29c, 0, sizeof(fresh->field_29c));
    if (fresh->field_3e0 == 3) {
        fresh->field_3e0 = 2;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00418f90
void FUN_00418f90(struct BoatRide *param_1) {
    struct BoatRide *prev = NULL;
    struct BoatRide *node = DAT_004cc03c;
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
            DAT_004cc03c = node->next;
        }
        free(param_1);
    }
}

// FUNCTION: LEGOLAND 0x00418fe0
void FUN_00418fe0(int param_1) {
    struct BoatRide *ride = DAT_004cc03c;
    int tw;
    int th;
    int tw2;
    int th2;
    int dx;
    int dy;
    int bx;
    int by;
    int sx;
    int sy;
    int person;
    struct Point off;
    struct Point seat;

    GetTileDimensions(&tw, &th);
    for (; ride != NULL; ride = ride->next) {
        if ((param_1 != 0 && (ride->field_3e4 == 1 || ride->field_3e4 == 0x10)) ||
            (param_1 == 0 && ride->field_3e4 != 1 && ride->field_3e4 != 0x10)) {
            dy = ride->field_1c[DAT_004cc08c * 2 + 1];
            dx = ride->field_1c[DAT_004cc08c * 2];
            GetTileDimensions(&tw2, &th2);
            bx = (dx - dy) * tw2 >> 9;
            by = (dx + dy) * th2 >> 9;
            sx = (ride->field_4 - ride->field_8) * (tw >> 1) - ((tw + 1) >> 1) - (ScrollX >> 8);
            sy = (ride->field_4 + ride->field_8) * (th >> 1) - (ScrollY >> 8);
            off.x = DAT_0082c65c->offset_x[ride->field_29c[DAT_004cc08c] & 0xff] >> 1;
            off.y = DAT_0082c65c->offset_y[ride->field_29c[DAT_004cc08c] & 0xff] >> 1;
            AdjustOffsetForViewMode(&off);
            ride->field_14 = lpConfig->field_20 + bx + off.x + sx;
            ride->field_18 = lpConfig->field_22 + by + off.y + sy;
            PrintSprite(DAT_0082c65c->sprites[ride->field_29c[DAT_004cc08c] & 0xff], ride->field_14, ride->field_18, 0, 0);
            if (ride->field_3ec != 0) {
                person = (int)Find3DPersonFromBloke(ride->field_3ec);
                *(float *)(person + 0x44) = ((float)(int)ride->field_29c[DAT_004cc08c] * DAT_004ab3e8 + DAT_004ab3e4) * DAT_004ab3dc * DAT_004ab3e0;
                SetPersonRotation((struct Person *)person, (float *)(person + 0x40));
                off.x = lpConfig->field_20 + bx + sx;
                off.y = lpConfig->field_22 + by + sy;
                AdjustBlokePosition((struct BlokePos *)&off);
                seat.x = DAT_004b51d8[(ride->field_29c[DAT_004cc08c] & 0xf) * 2] + 0x44;
                seat.y = DAT_004b51d8[(ride->field_29c[DAT_004cc08c] & 0xf) * 2 + 1] + 0x34;
                AdjustOffsetForViewMode(&seat);
                *(int *)(person + 0x1c) = seat.x + off.x;
                *(int *)(person + 0x20) = seat.y + off.y;
                IP_RenderBlokeIn3DNow((struct Bloke *)ride->field_3ec);
                PrintSprite(DAT_0082c65c->sprites[(ride->field_29c[DAT_004cc08c] + 0x30) & 0xff], ride->field_14, ride->field_18, 0, 0);
            }
        }
        if (ride->field_3e4 == 0x10 && ride->field_3e8 == 2 && DAT_004cc08c == 0x4f && param_1 != 0 && ride->field_3ec != 0) {
            ((struct Bloke *)ride->field_3ec)->param_action++;
            ride->field_3ec = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x004192d0
unsigned int FUN_004192d0(struct BoatRide *param_1) {
    unsigned short *id = (unsigned short *)param_1;
    struct BoatRide *node = DAT_004cc03c;
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
void FUN_00419300(void) {
    struct BoatRide *node = DAT_004cc03c;
    struct BoatRide *cur;

    while (node != NULL) {
        node->field_4 = node->field_c;
        node->field_8 = node->field_10;
        if (node->field_3e4 == 0x10) {
            cur = node;
            node = FUN_00419420(node);
            if (node != cur) {
                continue;
            }
        } else {
            switch (node->field_3e4) {
            case 1:
                FUN_004193c0(node);
                break;
            case 4:
                FUN_00419520(node, 0);
                break;
            case 8:
                FUN_00419520(node, 1);
                break;
            case 0x10:
                node = FUN_00419420(node);
                break;
            }
        }
        if (node == NULL) {
            return;
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x004193c0
void FUN_004193c0(struct BoatRide *param_1) {
    struct BoatRideNode *node = DAT_004cc074;
    FUN_004198a0(param_1, param_1->field_3dc, 4);
    param_1->field_3e4 = 4;
    param_1->field_10 = param_1->field_8 + 5;
    for (; node != NULL; node = node->next) {
        if (node->id == param_1->id) {
            node->field_c = 0;
            node->field_10 = 0;
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x00419420
struct BoatRide *FUN_00419420(struct BoatRide *param_1) {
    int *p;
    int i;
    struct BoatRide *result;
    struct SampleSource source;

    if (param_1->field_3e8 == 0) {
        result = param_1->next;
        source.type = 1;
        source.field_4 = (void *)param_1->field_3ec;
        UnSourceAndFadeAllSamplesFromSource(&source, -0x5a);
        FUN_00418f90(param_1);
        return result;
    }
    FUN_004198a0(param_1, 1, 4);
    param_1->field_3dc = 1;
    if (param_1->field_3e8 == 3) {
        i = 0x10;
        p = &param_1->field_1c[0x81];
        do {
            p[-1] = param_1->field_1c[0x80];
            *p = param_1->field_1c[0x81];
            p = p + 2;
            i = i - 1;
        } while (i != 0);
    } else if (param_1->field_3e8 == 2) {
        p = &param_1->field_1c[1];
        i = 0x40;
        do {
            p[-1] = param_1->field_1c[0x80];
            *p = param_1->field_1c[0x81];
            p = p + 2;
            i = i - 1;
        } while (i != 0);
    }
    if (param_1->field_3e8 != 3) {
        param_1->field_10 = param_1->field_8 + 5;
    }
    if (param_1->field_3e8 == 1) {
        p = &param_1->field_1c[0x9e];
        i = 7;
        do {
            p[0] = param_1->field_1c[0x90];
            p[1] = param_1->field_1c[0x91];
            p = p + -2;
            i = i - 1;
        } while (i != 0);
    }
    param_1->field_3e8 = param_1->field_3e8 - 1;
    return param_1;
}

// FUNCTION: LEGOLAND 0x00419520
void FUN_00419520(struct BoatRide *ride, int param_2) {
    struct BoatRideNode *score = DAT_004cc074;
    struct BoatRide *other = DAT_004cc03c;
    struct PathNode *path;
    unsigned int mask;
    unsigned int free;
    struct Point d;
    int i;
    int n;
    int back;
    int step;
    int dir;

    path = FUN_0041c890(ride->field_4, ride->field_8);
    for (; score != NULL; score = score->next) {
        if (path->owner.id == score->id) {
            break;
        }
    }
    mask = path->field_4;
    if (path->tile.id == score->start.id) {
        mask &= ~1;
    } else if (path->tile.id == score->end.id) {
        ride->field_3e4 = 0x10;
        ride->field_3e8 = 3;
        FUN_004198a0(ride, ride->field_3dc, 4);
        ride->field_3dc = 1;
        ride->field_10 = ride->field_8 + 5;
        return;
    }
    for (; other != NULL; other = other->next) {
        if (other == ride) {
            continue;
        }
        if ((ride->field_4 == other->field_4 && ride->field_8 - 5 == other->field_8) || (ride->field_4 == other->field_c && ride->field_8 - 5 == other->field_10)) {
            mask &= ~1;
        }
        if ((ride->field_4 + 5 == other->field_4 && ride->field_8 == other->field_8) || (ride->field_4 + 5 == other->field_c && ride->field_8 == other->field_10)) {
            mask &= ~2;
        }
        if ((ride->field_4 == other->field_4 && ride->field_8 + 5 == other->field_8) || (ride->field_4 == other->field_c && ride->field_8 + 5 == other->field_10)) {
            mask &= ~4;
        }
        if ((ride->field_4 - 5 == other->field_4 && ride->field_8 == other->field_8) || (ride->field_4 - 5 == other->field_c && ride->field_8 == other->field_10)) {
            mask &= ~8;
        }
    }
    if (param_2 != 0 && path->field_18 != NULL) {
        d.x = path->field_18->tile.pos.x - path->tile.pos.x;
        d.y = path->field_18->tile.pos.y - path->tile.pos.y;
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
        FUN_004198a0(ride, ride->field_3dc, -1);
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
        ride->field_c = ride->field_4;
        ride->field_10 = ride->field_8 - 5;
        FUN_004198a0(ride, ride->field_3dc, dir);
        ride->field_3dc = 4;
        break;
    case 2:
        ride->field_c = ride->field_4 + 5;
        ride->field_10 = ride->field_8;
        FUN_004198a0(ride, ride->field_3dc, dir);
        ride->field_3dc = 8;
        break;
    case 4:
        ride->field_c = ride->field_4;
        ride->field_10 = ride->field_8 + 5;
        FUN_004198a0(ride, ride->field_3dc, dir);
        ride->field_3dc = 1;
        break;
    case 8:
        ride->field_c = ride->field_4 - 5;
        ride->field_10 = ride->field_8;
        FUN_004198a0(ride, ride->field_3dc, dir);
        ride->field_3dc = 2;
        break;
    }
    if (--ride->field_3e8 == 0) {
        ride->field_3e4 = 8;
    }
}

// FUNCTION: LEGOLAND 0x004198a0
void FUN_004198a0(struct BoatRide *ride, int from, int to) {
    struct BoatArc *arc = NULL;
    int *p;
    int i;
    int bit;
    int idx;
    int sx;
    int sy;
    int tx;
    int ty;
    int dx;
    int dy;
    float fx;
    float fy;
    float angle;
    float step;

    if (to == -1) {
        if (from == -1) {
            memset(ride->field_1c, 0, sizeof(ride->field_1c));
        } else {
            for (bit = 0; bit < 4; bit++) {
                if ((from & (1 << bit)) != 0) {
                    break;
                }
            }
            sx = (int)((float)(DAT_004b5118[bit].ox * 40) * DAT_004ab3fc);
            sy = (int)((float)(DAT_004b5118[bit].oy * 40) * DAT_004ab3fc);
            for (i = 0; i < 80; i++) {
                if (i < 40) {
                    ride->field_1c[i * 2] = (int)((float)(DAT_004b5118[bit].dx * i) * DAT_004ab3fc + sx);
                    ride->field_1c[i * 2 + 1] = (int)((float)(DAT_004b5118[bit].dy * i) * DAT_004ab3fc + sy);
                } else {
                    ride->field_1c[i * 2] = 0;
                    ride->field_1c[i * 2 + 1] = 0;
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
                ride->field_1c[i * 2] = DAT_004b5118[idx].dx * 16 + ride->field_1c[i * 2 - 2];
                ride->field_1c[i * 2 + 1] = DAT_004b5118[idx].dy * 16 + ride->field_1c[i * 2 - 1];
            } else {
                ride->field_1c[i * 2] = 0;
                ride->field_1c[i * 2 + 1] = 0;
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
                arc = DAT_004b5158;
            } else if ((from & (to * 2)) != 0) {
                arc = DAT_004b5198;
            }
            for (bit = 0; bit < 4; bit++) {
                if ((from & (1 << bit)) != 0) {
                    break;
                }
            }
            arc += bit;
            step = (arc->a1 - arc->a0) * DAT_004ab3f8;
            angle = arc->a0;
            ride->field_1c[0] = (int)((sin(angle * DAT_004ab3f4) + arc->cx) * DAT_004ab3f0);
            ride->field_1c[1] = (int)((cos((angle + DAT_004ab3ec) * DAT_004ab3f4) + arc->cy) * DAT_004ab3f0);
            p = &ride->field_1c[3];
            for (i = 0x4f; i != 0; i--) {
                angle += step;
                p[-1] = (int)((sin(angle * DAT_004ab3f4) + arc->cx) * DAT_004ab3f0);
                p[0] = (int)((cos((angle + DAT_004ab3ec) * DAT_004ab3f4) + arc->cy) * DAT_004ab3f0);
                p += 2;
            }
        } else {
            for (bit = 0; bit < 4; bit++) {
                if ((from & (1 << bit)) != 0) {
                    break;
                }
            }
            sx = (int)((float)(DAT_004b5118[bit].ox * 40) * DAT_004ab3fc);
            sy = (int)((float)(DAT_004b5118[bit].oy * 40) * DAT_004ab3fc);
            if (from == to) {
                for (i = 0; i < 80; i++) {
                    if (i < 40) {
                        ride->field_1c[i * 2] = (int)((float)(DAT_004b5118[bit].dx * i) * DAT_004ab3fc + sx);
                        ride->field_1c[i * 2 + 1] = (int)((float)(DAT_004b5118[bit].dy * i) * DAT_004ab3fc + sy);
                    } else {
                        ride->field_1c[i * 2] = (int)((float)(DAT_004b5118[bit].dx * (80 - i)) * DAT_004ab3fc + sx);
                        ride->field_1c[i * 2 + 1] = (int)((float)(DAT_004b5118[bit].dy * (80 - i)) * DAT_004ab3fc + sy);
                    }
                }
            } else {
                fx = (float)sx;
                fy = (float)sy;
                for (i = 0; i < 80; i++) {
                    ride->field_1c[i * 2] = (int)((float)(DAT_004b5118[bit].dx * i) * DAT_004ab3fc + fx);
                    ride->field_1c[i * 2 + 1] = (int)((float)(i * DAT_004b5118[bit].dy) * DAT_004ab3fc + fy);
                }
            }
        }
    }
    for (i = 0; i < 80; i++) {
        if (i < 76) {
            tx = ride->field_1c[(i + 4) * 2];
            ty = ride->field_1c[(i + 4) * 2 + 1];
        } else {
            tx = ride->field_1c[0x9e];
            ty = ride->field_1c[0x9f];
        }
        if (i > 3) {
            dx = tx - ride->field_1c[(i - 3) * 2];
            dy = ty - ride->field_1c[(i - 3) * 2 + 1];
        } else {
            dx = tx - ride->field_1c[0];
            dy = ty - ride->field_1c[1];
        }
        ride->field_29c[i] = ((ArcTan256(dx, dy) >> 4) + 6 & 0xf) + ride->field_3e0 * 16;
    }
}

// FUNCTION: LEGOLAND 0x00419d10
void FUN_00419d10(struct RideObject *obj) {
    unsigned int handle;
    int i;
    struct Sprite *sprite;
    int lls;

    Load_FXList(PTR_s_Boat_Noise_wav, 2);
    DAT_0082c658 = obj->ride;
    DAT_0082c658->flags |= 0x20;
    DAT_0082c658->layer->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b5334
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", &handle, 0) == 0) {
        DAT_0082adf4 = (struct BoatTileMap *)LLIDB_LoadData((void *)handle);
    }
    // STRING: LEGOLAND 0x004b531c
    if (LLIDB_FindElement("BOATING SCHOOL BOATS", &handle, 0) == 0) {
        DAT_0082c65c = (struct SpriteSet *)LLIDB_LoadData((void *)handle);
    }
    for (i = 0; i < DAT_0082c65c->count; i++) {
        sprite = DAT_0082c65c->sprites[i & 0xff];
        LLSPlay((struct LLS *)GetLLSForSprite((struct SpriteLLS *)sprite), *(unsigned int *)((char *)sprite + 8));
    }
    // STRING: LEGOLAND 0x004b530c
    DAT_0082adfc = LoadSprite("bs_hullmask.lls", 1);
    // STRING: LEGOLAND 0x004b52fc
    DAT_0082c654 = LoadSprite("bs_railm.lls", 1);
    lls = GetLLSForSprite((struct SpriteLLS *)(DAT_0082ae00 = (void *)GetSpriteForLayer((struct LayerContainer *)DAT_0082c658->layer, 5)));
    LLSStop(lls);
    LLSSetFrame((struct LLS *)lls, *(short *)(lls + 0x10));
    DAT_004cc078 = DAT_0082c658->footprint;
    DAT_004cc048 = DAT_004b5260;
    DAT_004cc048.v[1] += DAT_004cc078.v[1];
    DAT_004cc048.v[0] += DAT_004cc078.v[0];
    DAT_004cc048.v[2] += DAT_004cc078.v[0];
    DAT_004cc048.v[3] += DAT_004cc078.v[1];
    DAT_004cc060 = DAT_004b5278;
    DAT_004cc060.v[1] += DAT_004cc078.v[3] + 1;
    DAT_004cc060.v[0] += DAT_004cc078.v[0];
    DAT_004cc060.v[2] += DAT_004cc078.v[0];
    DAT_004cc060.v[3] += DAT_004cc078.v[3] + 1;
}

// FUNCTION: LEGOLAND 0x00419ef0
void FUN_00419ef0(void) {
    unsigned int handle;
    int i;
    struct Sprite *sprite;
    struct PathNode *path;

    Kill_FXList(PTR_s_Boat_Noise_wav, 2);
    for (i = 0; i < DAT_0082c65c->count; i++) {
        sprite = DAT_0082c65c->sprites[(unsigned char)i];
        LLSStop(GetLLSForSprite((struct SpriteLLS *)sprite));
    }
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", &handle, 0) == 0) {
        LLIDB_UnLoadData(handle);
    }
    if (LLIDB_FindElement("BOATING SCHOOL BOATS", &handle, 0) == 0) {
        LLIDB_UnLoadData(handle);
    }
    while (DAT_004cc074 != NULL) {
        struct BoatRideNode *next = DAT_004cc074->next;
        free(DAT_004cc074);
        DAT_004cc074 = next;
    }
    while (DAT_004cc03c != NULL) {
        FUN_00418f90(DAT_004cc03c);
    }
    while (DAT_004d823c != NULL) {
        path = DAT_004d823c->next;
        free(DAT_004d823c);
        DAT_004d823c = path;
    }
    KillSprite(DAT_0082adfc);
    KillSprite(DAT_0082c654);
}

// FUNCTION: LEGOLAND 0x0041a000
void FUN_0041a000(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = DAT_0082c658;
    DefaultCursor(&EditCursor);
    DAT_004cc088 = DAT_004cc060.v;
    DAT_004cc070 = DAT_004cc048.v;
    SetEditCursorFootPrint(DAT_004cc078.v);
}

// FUNCTION: LEGOLAND 0x0041a040
void FUN_0041a040(struct EditObject *obj, int *coords) {
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
    score->start.pos.x = coords[0] + DAT_004cc060.v[0] + 2;
    score->start.pos.y = coords[1] + DAT_004cc060.v[1] + 2;
    score->end.pos.x = coords[0] + DAT_004cc048.v[0] + 2;
    score->end.pos.y = coords[1] + DAT_004cc048.v[1] + 2;
    score->field_8 = 0;
    score->field_c = 9999;
    score->field_10 = 0;
    score->field_14 = 0;
    score->value = 5;
    for (x = 0; x < 5; x++) {
        score->blokes[x] = 0;
    }
    score->next = DAT_004cc074;
    DAT_004cc074 = score;
    AddBasicObject(obj, coords);
    FUN_0041c4c0(coords[0] + DAT_004cc060.v[0] + 2, coords[1] + DAT_004cc060.v[1] + 2, 1, &score->id);
    FUN_0041c4c0(coords[0] + DAT_004cc048.v[0] + 2, coords[1] + DAT_004cc048.v[1] + 2, 4, &score->id);
    for (y = DAT_004cc078.v[1]; y <= DAT_004cc078.v[3]; y++) {
        for (x = DAT_004cc078.v[0]; x <= DAT_004cc078.v[2]; x++) {
            if (x == DAT_004cc078.v[0]) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0082adf4->tiles + 9);
            } else if (x == DAT_004cc078.v[2]) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0082adf4->tiles + 0xc);
            } else {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0082adf4->tiles);
            }
        }
    }
    SetMapTile(coords[0] + DAT_004cc078.v[2], coords[1] + DAT_004cc078.v[1], *DAT_0082adf4->tiles + 8);
    SetMapTile(coords[0] + DAT_004cc078.v[2], coords[1] + DAT_004cc078.v[3], *DAT_0082adf4->tiles + 7);
    SetMapTile(coords[0] + 4 + DAT_004cc078.v[0], coords[1] + DAT_004cc078.v[3], *DAT_0082adf4->tiles + 4);
    SetMapTile(coords[0] + 4 + DAT_004cc078.v[0], coords[1] + DAT_004cc078.v[1], *DAT_0082adf4->tiles + 1);
    SetMapTile(coords[0] + 5 + DAT_004cc078.v[0], coords[1] + DAT_004cc078.v[3], *DAT_0082adf4->tiles + 0xb);
    SetMapTile(coords[0] + 5 + DAT_004cc078.v[0], coords[1] + DAT_004cc078.v[1], *DAT_0082adf4->tiles + 10);
}

// FUNCTION: LEGOLAND 0x0041a2f0
void FUN_0041a2f0(int param_1, unsigned int param_2, unsigned int param_3) {
    struct Cursor *cursor = *(struct Cursor **)(param_1 + 0xc);

    DAT_004cc088 = DAT_004cc060.v;
    DAT_004cc070 = DAT_004cc048.v;
    DAT_004cc070[4] = 0;
    memcpy(EditCursor.field_1414, DAT_004cc078.v, 20);
    EditCursor.field_1830 = 0;
    ScreenToMapRef(param_2, &EditCursor.field_1404, param_3);
    PathCursor.field_1404 = EditCursor.field_1404;
    PathCursor.field_1408 = EditCursor.field_1408;
    PathCursor.field_1414[0] = EditCursor.field_1414[2] + 1;
    PathCursor.field_1414[1] = EditCursor.field_1414[1];
    PathCursor.field_1414[2] = PathCursor.field_1414[0];
    PathCursor.field_1414[3] = EditCursor.field_1414[3];
    PathCursor.field_1828 = 0x1008;
    PathCursor.field_1830 = 0;
    EditCursor.field_1830 = (unsigned int)&PathCursor;
    FUN_0045f460(&EditCursor);
    FUN_0045f460(&PathCursor);
    ValidateCursor(&EditCursor, (unsigned int)cursor);
}

// FUNCTION: LEGOLAND 0x0041a3d0
void FUN_0041a3d0(void *param_1, unsigned int param_2) {
    struct PathNode *path = DAT_004d823c;
    struct MermaidNode *node = DAT_004d2164;

    BasicObjectDCalcCursor((unsigned int)param_1, param_2);
    PathCursor.field_1404 = QueryCursor.field_1404;
    PathCursor.field_1408 = QueryCursor.field_1408;
    PathCursor.field_1414[0] = QueryCursor.field_1414[2] + 1;
    PathCursor.field_1414[2] = PathCursor.field_1414[0];
    PathCursor.field_1414[1] = QueryCursor.field_1414[1];
    PathCursor.field_1414[3] = QueryCursor.field_1414[3];
    PathCursor.field_1414[4] = 0;
    PathCursor.field_1828 = 0x1008;
    PathCursor.field_1830 = 0;
    QueryCursor.field_1830 = (unsigned int)&PathCursor;
    DAT_00810144 = 1;
    DefaultCursor(&DAT_0082ae20);
    *(struct Footprint *)DAT_0082ae20.field_1414 = DAT_004b53c0;
    for (; path != NULL; path = path->next) {
        if (path->owner.id == QueryObj) {
            DAT_0082ae20.field_1404 = path->tile.pos.x;
            DAT_0082ae20.field_1408 = path->tile.pos.y;
            FUN_0045f460(&DAT_0082ae20);
            DAT_0082ae20.field_1828 = 8;
            BuildCursorPtr(&DAT_0082ae20, 0, 0);
            RenderCursor(&DAT_0082ae20);
        }
    }
    for (; node != NULL; node = node->next) {
        if (node->field_2 == QueryObj) {
            DAT_0082ae20.field_1404 = node->tile.pos.x;
            DAT_0082ae20.field_1408 = node->tile.pos.y;
            FUN_0045f460(&DAT_0082ae20);
            DAT_0082ae20.field_1828 = 8;
            BuildCursorPtr(&DAT_0082ae20, 0, 0);
            RenderCursor(&DAT_0082ae20);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041a530
void FUN_0041a530(struct RideObject *obj, TileId tile, struct Cursor *cursor) {
    struct BoatRideNode *score = DAT_004cc074;
    struct BoatRideNode *prev = NULL;
    struct BoatRide *ride = DAT_004cc03c;
    struct PathNode *path;
    struct MermaidNode *mer;
    struct RideObject fake;
    int x;
    int y;
    int savedX;
    int savedY;

    StandardRemoveObject((struct EditObject *)obj, tile, cursor);
    for (y = DAT_004cc078.v[1]; y <= DAT_004cc078.v[3]; y++) {
        for (x = DAT_004cc078.v[0]; x <= DAT_004cc078.v[2]; x++) {
            RestoreBaseMap(cursor->field_1404 + x, cursor->field_1408 + y);
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
        fake.ride = DAT_0082adf0;
        IncrementObjectCount(DAT_0082adf0);
        IncrementObjectCount(DAT_0082adf0);
        path = DAT_004d823c;
        while (path != NULL) {
            if (path->owner.id == tile.id) {
                DAT_0082ae20.field_1404 = path->tile.pos.x;
                DAT_0082ae20.field_1408 = path->tile.pos.y;
                FUN_0041c620(&fake, path->tile, &DAT_0082ae20);
                path = DAT_004d823c;
            } else {
                path = path->next;
            }
        }
        fake.ride = DAT_0082adf8;
        mer = DAT_004d2164;
        while (mer != NULL) {
            if (mer->field_2 == tile.id) {
                savedX = cursor->field_1404;
                savedY = cursor->field_1408;
                cursor->field_1404 = mer->tile.pos.x;
                cursor->field_1408 = mer->tile.pos.y;
                FUN_0041b6f0(&fake, mer->tile, cursor);
                cursor->field_1404 = savedX;
                cursor->field_1408 = savedY;
                mer = DAT_004d2164;
            } else {
                mer = mer->next;
            }
        }
        if (prev != NULL) {
            prev->next = score->next;
        } else {
            DAT_004cc074 = score->next;
        }
        while (ride != NULL) {
            if (ride->id == tile.id) {
                FUN_00418f90(ride);
                ride = DAT_004cc03c;
            } else {
                ride = ride->next;
            }
        }
        RemoveAllBlokesFromRide(obj->ride, *(unsigned int *)&tile);
        free(score);
    }
}

// FUNCTION: LEGOLAND 0x0041a720
void FUN_0041a720(void) {
    struct RideNode *node = DAT_0082c658->riders;
    struct RideNode *next;
    struct BoatRideNode *score;
    struct Bloke *bloke;
    struct LLS *lls;
    TileId tile;
    int slot;
    int i;
    int frame;
    char dir;
    struct SampleSource source;
    struct SampleSource source2;
    struct Sample *sample;

    lls = (struct LLS *)GetLLSForSprite((struct SpriteLLS *)DAT_0082ae00);
    if (++DAT_004cc08c == 0x50) {
        DAT_004cc08c = 0;
        FUN_00419300();
    }
    FUN_00418fe0(0);
    for (; node != NULL; node = next) {
        score = DAT_004cc074;
        next = node->next;
        tile = node->tile;
        for (; score != NULL; score = score->next) {
            if (score->id == tile.id) {
                break;
            }
        }
        bloke = node->rider;
        if (bloke->field_e != 0) {
            continue;
        }
        switch (bloke->param_action) {
        case 0:
            slot = 4;
            for (i = 0; i < 5; i++) {
                if (score->blokes[i] == (unsigned int)bloke) {
                    slot = i;
                    break;
                }
            }
            if (i == 5) {
                if (score->field_14 == 5 || score->blokes[4] != 0) {
                    RemoveBlokeFromRide(DAT_0082c658, node);
                    break;
                }
                score->blokes[slot] = (unsigned int)bloke;
                score->field_14++;
            } else {
                bloke = (struct Bloke *)score->blokes[slot];
                if (score->blokes[slot - 1] != 0) {
                    break;
                }
                score->blokes[slot - 1] = (unsigned int)bloke;
                score->blokes[slot] = 0;
                if (--slot == 0) {
                    bloke->param_action++;
                }
            }
            bloke->flags |= 8;
            bloke->dest.x = ((DAT_0082c658->x + tile.pos.x) << 8) + DAT_004b5290[(4 - slot) * 2];
            bloke->dest.y = ((DAT_0082c658->y + tile.pos.y) << 8) + DAT_004b5290[(4 - slot) * 2 + 1];
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->field_73 = dir + 0x10;
            bloke->field_e = 7;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            break;
        case 1:
            if (bloke == (struct Bloke *)score->blokes[0] && score->field_8 != 0 && (int)score->value >= (int)FUN_004192d0((struct BoatRide *)score) * 6 && FUN_00418e60(tile, (unsigned int)bloke) != 0) {
                BlokeSitAnim(bloke);
                BlokeSetFrame(bloke, 0);
                score->blokes[0] = 0;
                score->field_14--;
                bloke->flags |= 0x80;
                bloke->param_action++;
                source.type = 1;
                source.field_4 = bloke;
                sample = PlayInstanceOfSample(*(void **)(PTR_s_Boat_Noise_wav + 8), 1, 1, &source);
                AdjustPSampleFreq(sample, 10);
            }
            break;
        case 3:
            BlokeWalkAnim(bloke);
            BlokeSetFrame(bloke, 0);
            bloke->flags &= 0xff7f;
            bloke->pos.x = (DAT_0082c658->field_24 + tile.pos.x - 4) << 8;
            bloke->pos.y = (DAT_0082c658->field_25 + tile.pos.y + 2) << 8;
            bloke->field_72 = 10;
            bloke->dest.x = ((DAT_0082c658->field_24 + tile.pos.x) << 8) - 0xc0;
            bloke->dest.y = ((DAT_0082c658->field_25 + tile.pos.y) << 8) + 0x240;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->field_e = 7;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 4:
            bloke->dest.x = ((DAT_0082c658->field_24 + tile.pos.x) << 8) - 0xc0;
            bloke->dest.y = ((DAT_0082c658->field_25 + tile.pos.y) << 8) + 0x80;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->field_e = 7;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 5:
            bloke->dest.x = ((DAT_0082c658->field_24 + tile.pos.x) << 8) + 0x80;
            bloke->dest.y = ((DAT_0082c658->field_25 + tile.pos.y) << 8) + 0x80;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->field_e = 7;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            source2.type = 1;
            source2.field_4 = bloke;
            UnSourceAndFadeAllSamplesFromSource(&source2, -0x5a);
            break;
        case 6:
            bloke->flags &= 0xfff7;
            RemoveBlokeFromRide(DAT_0082c658, node);
            break;
        }
    }
    for (score = DAT_004cc074; score != NULL; score = score->next) {
        frame = ++score->field_c;
        if (frame <= *(short *)((char *)lls + 0x10)) {
            if (score->field_10 == 0) {
                LLSSetFrame(lls, *(short *)((char *)lls + 0x10) - frame);
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
void FUN_0041abd0(struct RideObject *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    short *lls;
    struct LLS *hull;
    struct Point pos;
    struct Point offset;

    FUN_00418fe0(1);
    pos = GetScreenCoordsForObject((unsigned char *)tile, ride);
    offset = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0082c658->layer, 3);
    AdjustOffsetForViewMode(&offset);
    lls = (short *)GetLLSForSprite((struct SpriteLLS *)GetSpriteForLayer((struct LayerContainer *)DAT_0082c658->layer, 3));
    hull = (struct LLS *)GetLLSForSprite((struct SpriteLLS *)DAT_0082adfc);
    LLSSetFrame(hull, *lls);
    PrintSprite(DAT_0082adfc, pos.x + offset.x, pos.y + offset.y, clip, 0);
    for (; node != NULL; node = node->next) {
        if (*tile == node->tile.id && node->rider->param_action != 2) {
            IP_RenderBlokeIn3DNow(node->rider);
        }
    }
    offset = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0082c658->layer, 3);
    offset.x += 0x71;
    offset.y += 0xac;
    AdjustOffsetForViewMode(&offset);
    PrintSprite(DAT_0082c654, pos.x + offset.x, pos.y + offset.y, clip, 0);
}

// FUNCTION: LEGOLAND 0x0041acf0
int FUN_0041acf0(void) {
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
    for (scoreCur = DAT_004cc074; scoreCur != NULL; scoreCur = scoreCur->next) {
        count++;
    }
    score = DAT_004cc074;
    SaveGameWrite(&count, 4);
    while (count--) {
        scoreCopy = *score;
        for (i = 0; i < 5; i++) {
            scoreCopy.blokes[i] = GetBlokeNum(scoreCopy.blokes[i]);
        }
        SaveGameWrite(&scoreCopy, 0x34);
        score = score->next;
    }
    count = 0;
    for (pathCur = DAT_004d823c; pathCur != NULL; pathCur = pathCur->next) {
        count++;
    }
    path = DAT_004d823c;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(path, 0x1c);
        path = path->next;
    }
    count = 0;
    for (merCur = DAT_004d2164; merCur != NULL; merCur = merCur->next) {
        count++;
    }
    mer = DAT_004d2164;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(mer, 8);
        mer = mer->next;
    }
    count = 0;
    for (rideCur = DAT_004cc03c; rideCur != NULL; rideCur = rideCur->next) {
        count++;
    }
    ride = DAT_004cc03c;
    SaveGameWrite(&count, 4);
    while (count--) {
        rideCopy = *ride;
        rideCopy.field_3ec = GetBlokeNum(rideCopy.field_3ec);
        SaveGameWrite(&rideCopy, 0x3f4);
        ride = ride->next;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0041aee0
int FUN_0041aee0(void) {
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
            DAT_004cc074 = (struct BoatRideNode *)malloc(sizeof(struct BoatRideNode));
            prevScore = DAT_004cc074;
        } else {
            score = (struct BoatRideNode *)malloc(sizeof(struct BoatRideNode));
            prevScore->next = score;
            prevScore = score;
        }
        SaveGameRead(prevScore, 0x34);
        for (i = 0; i < 5; i++) {
            prevScore->blokes[i] = GetBlokePtr(prevScore->blokes[i]);
        }
    }
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevPath == NULL) {
            DAT_004d823c = (struct PathNode *)malloc(0x1c);
            prevPath = DAT_004d823c;
        } else {
            path = (struct PathNode *)malloc(0x1c);
            prevPath->next = path;
            prevPath = path;
        }
        SaveGameRead(prevPath, 0x1c);
    }
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevMer == NULL) {
            DAT_004d2164 = (struct MermaidNode *)malloc(8);
            prevMer = DAT_004d2164;
        } else {
            mer = (struct MermaidNode *)malloc(8);
            prevMer->next = mer;
            prevMer = mer;
        }
        SaveGameRead(prevMer, 8);
    }
    prevRide = DAT_004cc03c;
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevRide == NULL) {
            DAT_004cc03c = (struct BoatRide *)malloc(sizeof(struct BoatRide));
            prevRide = DAT_004cc03c;
        } else {
            ride = (struct BoatRide *)malloc(sizeof(struct BoatRide));
            prevRide->next = ride;
            prevRide = ride;
        }
        SaveGameRead(prevRide, 0x3f4);
        prevRide->field_3ec = GetBlokePtr(prevRide->field_3ec);
    }
    for (score = DAT_004cc074; score != NULL; score = score->next) {
        FUN_0041caa0(score->id);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0041b0d0
void FUN_0041b0d0(unsigned short id, unsigned int value) {
    struct BoatRideNode *node = DAT_004cc074;
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
int FUN_0041b100(int dummy, int arg) {
    struct BoatRideNode *node = DAT_004cc074;
    int result = 0;

    if (node != NULL) {
        do {
            int val = (int)node->value;
            if (val > result) {
                if (arg != 0) {
                    if (node->field_8 != 0) {
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
LEGO_EXPORT void GetInterface(struct ClassNode *head, struct CallbackTable *iface) {
    void **cb = (void **)iface;
    // STRING: LEGOLAND 0x004b537c
    if (_stricmp("BOATING SCHOOL WATER", head->name) == 0) {
        cb[7] = FUN_0041b830;
        cb[0] = FUN_0041b880;
        cb[1] = FUN_0041bd40;
        cb[2] = FUN_0041bfb0;
        cb[3] = FUN_0041b8e0;
        cb[4] = FUN_0041c130;
        return;
    }
    if (_stricmp("BOATING SCHOOL", head->name) == 0) {
        cb[7] = FUN_00419d10;
        cb[8] = FUN_00419ef0;
        cb[0] = FUN_0041a000;
        cb[1] = FUN_0041a2f0;
        cb[2] = FUN_0041a3d0;
        cb[3] = FUN_0041a040;
        cb[4] = FUN_0041a530;
        cb[6] = FUN_0041a720;
        cb[9] = FUN_0041abd0;
        cb[0xc] = FUN_0041acf0;
        cb[0xb] = FUN_0041aee0;
        cb[0xd] = FUN_0041b100;
        return;
    }
    if (_stricmp("BOATING SCHOOL MERMAID", head->name) == 0) {
        cb[7] = FUN_0041b250;
        cb[0] = FUN_0041b260;
        cb[1] = FUN_0041b4c0;
        cb[2] = FUN_0041b6d0;
        cb[3] = FUN_0041b2a0;
        cb[4] = FUN_0041b6f0;
    }
}

// FUNCTION: LEGOLAND 0x0041b250
void FUN_0041b250(struct RideObject *param_1) {
    DAT_0082adf8 = param_1->ride;
}

// FUNCTION: LEGOLAND 0x0041b260
void FUN_0041b260(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = DAT_0082adf8;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(&DAT_0082adf8->footprint);
}

// FUNCTION: LEGOLAND 0x0041b2a0
void FUN_0041b2a0(struct EditObject *obj, int *coords) {
    struct Ride *ride = ((struct RideObject *)obj)->ride;
    TileId tile;
    struct MermaidNode *node;
    unsigned short owner;
    int x;
    int y;
    struct SampleSource source;

    tile.pos.x = coords[0];
    tile.pos.y = coords[1];
    FUN_0041c690(coords[0], coords[1], &owner);
    node = (struct MermaidNode *)malloc(8);
    if (node == NULL) {
        return;
    }
    node->tile = tile;
    node->field_2 = owner;
    node->next = DAT_004d2164;
    DAT_004d2164 = node;
    FUN_0041b0d0(owner, 1);
    AddBasicObject(obj, coords);
    for (y = ride->footprint.v[1]; y <= ride->footprint.v[3]; y++) {
        for (x = ride->footprint.v[0]; x <= ride->footprint.v[2]; x++) {
            if (x == ride->footprint.v[0]) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0082adf4->tiles + 9);
            } else if (x == ride->footprint.v[2]) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0082adf4->tiles + 0xc);
            } else if (y == ride->footprint.v[1]) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0082adf4->tiles + 10);
            } else if (y == ride->footprint.v[3]) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0082adf4->tiles + 0xb);
            } else {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0082adf4->tiles);
            }
        }
    }
    SetMapTile(ride->footprint.v[0] + coords[0], ride->footprint.v[1] + coords[1], *DAT_0082adf4->tiles + 5);
    SetMapTile(coords[0] + ride->footprint.v[2], ride->footprint.v[1] + coords[1], *DAT_0082adf4->tiles + 8);
    SetMapTile(ride->footprint.v[0] + coords[0], ride->footprint.v[3] + coords[1], *DAT_0082adf4->tiles + 6);
    SetMapTile(coords[0] + ride->footprint.v[2], ride->footprint.v[3] + coords[1], *DAT_0082adf4->tiles + 7);
    source.field_8 = coords[0];
    source.type = 2;
    source.field_c = coords[1];
    PlayInstanceOfSample(*(void **)(PTR_s_Boat_Noise_wav + 0x14), 1, 1, &source);
}

// FUNCTION: LEGOLAND 0x0041b4c0
void FUN_0041b4c0(struct RideObject *obj, unsigned int param_2, unsigned int param_3) {
    struct Ride *ride;
    unsigned int mask;
    unsigned short owner;
    int n;
    int x;
    int y;
    struct Cursor *c;

    n = 0;
    ride = obj->ride;
    memcpy(EditCursor.field_1414, &ride->footprint, 20);
    ScreenToMapRef(param_2, &EditCursor.field_1404, param_3);
    mask = FUN_0041c690(EditCursor.field_1404, EditCursor.field_1408, &owner);
    EditCursor.field_1830 = n;
    if (mask == 0) {
        FUN_0045f480(&EditCursor, 0xe);
        return;
    }
    ValidateCursor(&EditCursor, (unsigned int)ride);
    if (FUN_0045f4b0(&EditCursor) == 0) {
        return;
    }
    DefaultCursor(&DAT_004cc090[0]);
    DefaultCursor(&DAT_004cc090[1]);
    DefaultCursor(&DAT_004cc090[2]);
    DefaultCursor(&DAT_004cc090[3]);
    memcpy(DAT_004cc090[0].field_1414, EditCursor.field_1414, 20);
    memcpy(DAT_004cc090[1].field_1414, EditCursor.field_1414, 20);
    memcpy(DAT_004cc090[2].field_1414, EditCursor.field_1414, 20);
    memcpy(DAT_004cc090[3].field_1414, EditCursor.field_1414, 20);
    FUN_0045f460(&DAT_004cc090[0]);
    FUN_0045f460(&DAT_004cc090[1]);
    FUN_0045f460(&DAT_004cc090[2]);
    FUN_0045f460(&DAT_004cc090[3]);
    x = EditCursor.field_1404;
    y = EditCursor.field_1408;
    DAT_004cc090[0].field_1828 = 0x2034;
    DAT_004cc090[1].field_1828 = 0x2034;
    DAT_004cc090[2].field_1828 = 0x2034;
    DAT_004cc090[3].field_1828 = 0x2034;
    if ((mask & 1) != 0) {
        DAT_004cc090[0].field_1404 = x;
        DAT_004cc090[0].field_1408 = y - 5;
        n = 1;
    }
    if ((mask & 2) != 0) {
        DAT_004cc090[n].field_1404 = x + 5;
        DAT_004cc090[n].field_1408 = y;
        n++;
    }
    if ((mask & 4) != 0) {
        DAT_004cc090[n].field_1404 = x;
        DAT_004cc090[n].field_1408 = y + 5;
        n++;
    }
    if ((mask & 8) != 0) {
        DAT_004cc090[n].field_1404 = x - 5;
        DAT_004cc090[n].field_1408 = y;
        n++;
    }
    if (n != 0) {
        EditCursor.field_1830 = (unsigned int)&DAT_004cc090[0];
        if (n > 1) {
            c = &DAT_004cc090[1];
            n--;
            do {
                c[-1].field_1830 = (unsigned int)c;
                c++;
                n--;
            } while (n != 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041b6d0
unsigned int FUN_0041b6d0(unsigned int param_1, unsigned int param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x0041b6f0
void FUN_0041b6f0(void *param_1, TileId tile, struct Cursor *param_3) {
    struct Cursor *cursor = *(struct Cursor **)((char *)param_1 + 0xc);
    struct MermaidNode *node = DAT_004d2164;
    struct MermaidNode *prev = NULL;
    int x;
    int y;
    struct SampleSource source;

    StandardRemoveObject((struct EditObject *)param_1, tile, param_3);
    source.type = 2;
    source.field_8 = tile.pos.x;
    source.field_c = tile.pos.y;
    if (CountSamplesFromSource(&source) != 1) {
        // STRING: LEGOLAND 0x004b5398
        DBPrintf("Can't find samples for mermaid\n");
    }
    UnSourceAndFadeAllSamplesFromSource(&source, -400);
    for (y = cursor->field_3c.v[1]; y <= cursor->field_3c.v[3]; y++) {
        for (x = cursor->field_3c.v[0]; x <= cursor->field_3c.v[2]; x++) {
            RestoreBaseMap(x + param_3->field_1404, y + param_3->field_1408);
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
        FUN_0041b0d0(node->field_2, -1);
        if (prev != NULL) {
            prev->next = node->next;
        } else {
            DAT_004d2164 = node->next;
        }
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x0041b830
void FUN_0041b830(struct RideObject *arg) {
    struct Ride *building = arg->ride;
    DAT_0082adf0 = building;
    DAT_004b53c0.v[1] += building->footprint.v[1];
    DAT_004b53c0.v[0] += building->footprint.v[0];
    DAT_004b53c0.v[2] += building->footprint.v[0];
    DAT_004b53c0.v[3] += building->footprint.v[1];
}

// FUNCTION: LEGOLAND 0x0041b880
void FUN_0041b880(void) {
    struct Ride *state = DAT_0082adf0;
    EditMode.unk0 = 1;
    EditMode.unk8 = state;
    memcpy(&state->footprint, &DAT_004b53c0, sizeof(DAT_004b53c0));
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint((char *)EditMode.unk8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x0041b8e0
void FUN_0041b8e0(struct RideObject *obj, int *coords) {
    struct BoatRideNode *score = DAT_004cc074;
    unsigned int mask;
    unsigned short owner;
    int x0;
    int y0;
    int x1;
    int y1;

    mask = FUN_0041c690(coords[0], coords[1], &owner);
    FUN_0041c4c0(coords[0], coords[1], mask, &owner);
    IncrementObjectCount(obj->ride);
    FUN_0041b0d0(owner, 1);
    FUN_0041bab0(coords[0], coords[1], &owner);
    if ((mask & 1) != 0) {
        FUN_0041c4c0(coords[0], coords[1] - 5, FUN_0041c690(coords[0], coords[1] - 5, &owner), NULL);
        FUN_0041bab0(coords[0], coords[1] - 5, &owner);
    }
    if ((mask & 2) != 0) {
        FUN_0041c4c0(coords[0] + 5, coords[1], FUN_0041c690(coords[0] + 5, coords[1], &owner), NULL);
        FUN_0041bab0(coords[0] + 5, coords[1], &owner);
    }
    if ((mask & 4) != 0) {
        FUN_0041c4c0(coords[0], coords[1] + 5, FUN_0041c690(coords[0], coords[1] + 5, &owner), NULL);
        FUN_0041bab0(coords[0], coords[1] + 5, &owner);
    }
    if ((mask & 8) != 0) {
        FUN_0041c4c0(coords[0] - 5, coords[1], FUN_0041c690(coords[0] - 5, coords[1], &owner), NULL);
        FUN_0041bab0(coords[0] - 5, coords[1], &owner);
    }
    for (; score != NULL; score = score->next) {
        if (score->id == owner) {
            x0 = score->start.pos.x;
            y0 = score->start.pos.y;
            x1 = score->end.pos.x;
            y1 = score->end.pos.y;
            score->field_8 = FUN_0041c8c0(x0, y0, x1, y1);
            if (score->field_8 != 0) {
                FUN_0041caa0(owner);
            }
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x0041bab0
void FUN_0041bab0(int param_1, int param_2, unsigned short *param_3) {
    struct BoatRideNode *score = DAT_004cc074;
    struct PathNode *path;
    int other;
    unsigned int mask;

    path = FUN_0041c890(param_1, param_2);
    for (; score != NULL; score = score->next) {
        if (score->id == *param_3) {
            break;
        }
    }
    if (path != NULL) {
        mask = path->field_4;
        if (path->tile.id == score->start.id) {
            mask = mask & 0xfffffffe;
        } else if (path->tile.id == score->end.id) {
            mask = mask & 0xfffffffb;
        }
        if ((mask & 8) != 0 && (mask & 1) != 0 &&
            (other = (int)FUN_0041c890(param_1, (param_2 - 5)), (*(unsigned char *)(other + 4) & 8) != 0)) {
            SetMapTile(param_1 - 3, param_2 - 3, *DAT_0082adf4->tiles);
            SetMapTile(param_1 - 2, param_2 - 3, *DAT_0082adf4->tiles);
            SetMapTile(param_1 - 3, param_2 - 2, *DAT_0082adf4->tiles);
            SetMapTile(param_1 - 2, param_2 - 2, *DAT_0082adf4->tiles);
        }
        if ((mask & 8) != 0 && (mask & 4) != 0 &&
            (other = (int)FUN_0041c890(param_1, (param_2 + 5)), (*(unsigned char *)(other + 4) & 8) != 0)) {
            SetMapTile(param_1 - 3, param_2 + 3, *DAT_0082adf4->tiles);
            SetMapTile(param_1 - 2, param_2 + 3, *DAT_0082adf4->tiles);
            SetMapTile(param_1 - 3, param_2 + 2, *DAT_0082adf4->tiles);
            SetMapTile(param_1 - 2, param_2 + 2, *DAT_0082adf4->tiles);
        }
        if ((mask & 2) != 0 && (mask & 1) != 0 &&
            (other = (int)FUN_0041c890(param_1, (param_2 - 5)), (*(unsigned char *)(other + 4) & 2) != 0)) {
            SetMapTile(param_1 + 3, param_2 - 3, *DAT_0082adf4->tiles);
            SetMapTile(param_1 + 2, param_2 - 3, *DAT_0082adf4->tiles);
            SetMapTile(param_1 + 3, param_2 - 2, *DAT_0082adf4->tiles);
            SetMapTile(param_1 + 2, param_2 - 2, *DAT_0082adf4->tiles);
        }
        if ((mask & 2) != 0 && (mask & 4) != 0 &&
            (other = (int)FUN_0041c890(param_1, (param_2 + 5)), (*(unsigned char *)(other + 4) & 2) != 0)) {
            SetMapTile(param_1 + 3, param_2 + 3, *DAT_0082adf4->tiles);
            SetMapTile(param_1 + 2, param_2 + 3, *DAT_0082adf4->tiles);
            SetMapTile(param_1 + 3, param_2 + 2, *DAT_0082adf4->tiles);
            SetMapTile(param_1 + 2, param_2 + 2, *DAT_0082adf4->tiles);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041bd40
void FUN_0041bd40(struct RideObject *obj, unsigned int param_2, unsigned int param_3) {
    unsigned int mask;
    unsigned short owner;
    int n;
    int x;
    int y;
    int result;
    struct Cursor *c;
    struct MapRect rect;

    n = 0;
    memcpy(EditCursor.field_1414, &DAT_004b53c0, sizeof(DAT_004b53c0));
    ScreenToMapRef(param_2, &EditCursor.field_1404, param_3);
    mask = FUN_0041c690(EditCursor.field_1404, EditCursor.field_1408, &owner);
    EditCursor.field_1830 = n;
    if (mask == 0) {
        FUN_0045f480(&EditCursor, 0xe);
    } else {
        ValidateCursor(&EditCursor, (unsigned int)obj->ride);
        if (FUN_0045f4b0(&EditCursor) != 0) {
            rect.x0 = EditCursor.field_1414[0] + EditCursor.field_1404;
            rect.y0 = EditCursor.field_1414[1] + EditCursor.field_1408;
            rect.x1 = EditCursor.field_1414[2] + EditCursor.field_1404;
            rect.y1 = EditCursor.field_1414[3] + EditCursor.field_1408;
            result = CheckForPeople(&rect);
            if (result != -1) {
                if (result != 1) {
                    DefaultCursor(&DAT_004d2168[0]);
                    DefaultCursor(&DAT_004d2168[1]);
                    DefaultCursor(&DAT_004d2168[2]);
                    DefaultCursor(&DAT_004d2168[3]);
                    memcpy(DAT_004d2168[0].field_1414, EditCursor.field_1414, 20);
                    memcpy(DAT_004d2168[1].field_1414, EditCursor.field_1414, 20);
                    memcpy(DAT_004d2168[2].field_1414, EditCursor.field_1414, 20);
                    memcpy(DAT_004d2168[3].field_1414, EditCursor.field_1414, 20);
                    FUN_0045f460(&DAT_004d2168[0]);
                    FUN_0045f460(&DAT_004d2168[1]);
                    FUN_0045f460(&DAT_004d2168[2]);
                    FUN_0045f460(&DAT_004d2168[3]);
                    x = EditCursor.field_1404;
                    y = EditCursor.field_1408;
                    DAT_004d2168[0].field_1828 = 0x2034;
                    DAT_004d2168[1].field_1828 = 0x2034;
                    DAT_004d2168[2].field_1828 = 0x2034;
                    DAT_004d2168[3].field_1828 = 0x2034;
                    if ((mask & 1) != 0) {
                        DAT_004d2168[0].field_1404 = x;
                        DAT_004d2168[0].field_1408 = y - 5;
                        n = 1;
                    }
                    if ((mask & 2) != 0) {
                        DAT_004d2168[n].field_1404 = x + 5;
                        DAT_004d2168[n].field_1408 = y;
                        n++;
                    }
                    if ((mask & 4) != 0) {
                        DAT_004d2168[n].field_1404 = x;
                        DAT_004d2168[n].field_1408 = y + 5;
                        n++;
                    }
                    if ((mask & 8) != 0) {
                        DAT_004d2168[n].field_1404 = x - 5;
                        DAT_004d2168[n].field_1408 = y;
                        n++;
                    }
                    if (n != 0) {
                        EditCursor.field_1830 = (unsigned int)&DAT_004d2168[0];
                        if (n > 1) {
                            c = &DAT_004d2168[1];
                            n--;
                            do {
                                c[-1].field_1830 = (unsigned int)c;
                                c++;
                                n--;
                            } while (n != 0);
                        }
                    }
                } else {
                    FUN_0045f480(&EditCursor, 3);
                }
            } else {
                FUN_0045f480(&EditCursor, 4);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0041bfb0
void FUN_0041bfb0(unsigned int param_1, int *coords) {
    struct BoatRideNode *score = DAT_004cc074;
    struct BoatRide *ride;
    struct MapElement *elem;
    struct PathNode *path;
    TileId tile;
    struct RideObject fake;

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
            path = FUN_0041c890(coords[0], coords[1]);
            ((TileId *)&QueryObj)->pos.x = path->owner.pos.x;
            coords[0] = ((TileId *)&QueryObj)->pos.x;
            ((TileId *)&QueryObj)->pos.y = path->owner.pos.y;
            coords[1] = ((TileId *)&QueryObj)->pos.y;
            memcpy((char *)QueryClass + 0x3c, DAT_004cc078.v, 20);
            fake.ride = DAT_0082c658;
            FUN_0041a3d0(&fake, (unsigned int)coords);
            return;
        }
    }
    ride = DAT_004cc03c;
    memcpy((char *)QueryClass + 0x3c, &DAT_004b53c0, sizeof(DAT_004b53c0));
    BasicObjectDCalcCursor(param_1, (unsigned int)coords);
    for (; ride != NULL; ride = ride->next) {
        if ((tile.pos.x == ride->field_4 && tile.pos.y == ride->field_8) || (tile.pos.x == ride->field_c && tile.pos.y == ride->field_10)) {
            FUN_0045f480(&QueryCursor, 1);
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x0041c130
void FUN_0041c130(struct RideObject *obj, TileId tile, struct Cursor *cursor) {
    struct BoatRideNode *score = DAT_004cc074;
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
    struct RideObject fake;

    ex = tile.pos.x;
    ey = tile.pos.y;
    if (ex >= 0 && ex < lpConfig->width && ey >= 0 && ey < lpConfig->height) {
        elem = &GameMap[ey][ex];
    } else {
        elem = NULL;
    }
    if (elem->field_0 != DAT_0082adf0->field_c4) {
        fake.ride = DAT_0082c658;
        FUN_0041a530(&fake, tile, cursor);
        return;
    }
    mask = FUN_0041c690(cursor->field_1404, cursor->field_1408, &owner);
    FUN_0041c620(obj, tile, cursor);
    FUN_0041b0d0(owner, -1);
    if ((mask & 1) != 0) {
        y = cursor->field_1408 - 5;
        x = cursor->field_1404;
        dir = FUN_0041c690(x, y, &other);
        FUN_0041c4c0(x, y, dir, &owner);
        FUN_0041bab0(x, y, &owner);
    }
    if ((mask & 2) != 0) {
        x = cursor->field_1404 + 5;
        y = cursor->field_1408;
        dir = FUN_0041c690(x, y, &other);
        FUN_0041c4c0(x, y, dir, &owner);
        FUN_0041bab0(x, y, &owner);
    }
    if ((mask & 4) != 0) {
        y = cursor->field_1408 + 5;
        x = cursor->field_1404;
        dir = FUN_0041c690(x, y, &other);
        FUN_0041c4c0(x, y, dir, &owner);
        FUN_0041bab0(x, y, &owner);
    }
    if ((mask & 8) != 0) {
        x = cursor->field_1404 - 5;
        y = cursor->field_1408;
        dir = FUN_0041c690(x, y, &other);
        FUN_0041c4c0(x, y, dir, &owner);
        FUN_0041bab0(x, y, &owner);
    }
    if ((mask & 1) != 0 && (mask & 8) != 0) {
        x = cursor->field_1404 - 5;
        y = cursor->field_1408 - 5;
        if (FUN_0041c890(x, y) != NULL) {
            dir = FUN_0041c690(x, y, &other);
            FUN_0041c4c0(x, y, dir, &owner);
            FUN_0041bab0(x, y, &owner);
        }
    }
    if ((mask & 1) != 0 && (mask & 2) != 0) {
        x = cursor->field_1404 + 5;
        y = cursor->field_1408 - 5;
        if (FUN_0041c890(x, y) != NULL) {
            dir = FUN_0041c690(x, y, &other);
            FUN_0041c4c0(x, y, dir, &owner);
            FUN_0041bab0(x, y, &owner);
        }
    }
    if ((mask & 4) != 0 && (mask & 8) != 0) {
        x = cursor->field_1404 - 5;
        y = cursor->field_1408 + 5;
        if (FUN_0041c890(x, y) != NULL) {
            dir = FUN_0041c690(x, y, &other);
            FUN_0041c4c0(x, y, dir, &owner);
            FUN_0041bab0(x, y, &owner);
        }
    }
    if ((mask & 4) != 0 && (mask & 2) != 0) {
        x = cursor->field_1404 + 5;
        y = cursor->field_1408 + 5;
        if (FUN_0041c890(x, y) != NULL) {
            dir = FUN_0041c690(x, y, &other);
            FUN_0041c4c0(x, y, dir, &owner);
            FUN_0041bab0(x, y, &owner);
        }
    }
    FUN_0041caa0(owner);
    for (; score != NULL; score = score->next) {
        if (score->id == owner) {
            x0 = score->start.pos.x;
            y0 = score->start.pos.y;
            x1 = score->end.pos.x;
            y1 = score->end.pos.y;
            score->field_8 = FUN_0041c8c0(x0, y0, x1, y1);
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x0041c4c0
void FUN_0041c4c0(int x, int y, int mask, unsigned short *owner) {
    TileId tile;
    struct PathNode *node;
    struct MapElement *elem;
    int row;
    int col;
    struct Point pt;

    tile.pos.x = x;
    tile.pos.y = y;
    node = FUN_0041c890(x, y);
    if (node == NULL) {
        node = (struct PathNode *)malloc(0x1c);
        if (node == NULL) {
            return;
        }
        node->next = DAT_004d823c;
        node->field_18 = NULL;
        DAT_004d823c = node;
    }
    node->tile = tile;
    node->field_4 = mask;
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
            elem->field_0 = DAT_0082adf0->field_c4;
            *(unsigned short *)&elem->field_4 = tile.id;
            SetMapTile(pt.x, pt.y, *DAT_0082adf4[DAT_004b53d4[mask * 25 + row * 5 + col] >> 8].tiles + (unsigned char)DAT_004b53d4[mask * 25 + row * 5 + col]);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041c620
void FUN_0041c620(void *param_1, TileId tile, struct Cursor *param_3) {
    struct PathNode *node = DAT_004d823c;
    struct PathNode *prev = NULL;

    StandardRemoveObject((struct EditObject *)param_1, tile, param_3);
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
        DAT_004d823c = node->next;
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x0041c690
unsigned int FUN_0041c690(int x, int y, unsigned short *owner) {
    struct BoatRideNode *score;
    struct PathNode *node;
    unsigned int mask;
    int valid;
    int n;
    TileId key;

    mask = 0;
    valid = 0;
    score = DAT_004cc074;
    node = FUN_0041c890(x, y);
    if (node != NULL) {
        *owner = node->owner.id;
        valid = 1;
    }
    n = y - 5;
    if (x >= 0 && n >= 0 && x < lpConfig->width && n < lpConfig->height && (node = FUN_0041c890(x, n)) != NULL) {
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
    if (n >= 0 && y >= 0 && n < lpConfig->width && y < lpConfig->height && (node = FUN_0041c890(n, y)) != NULL) {
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
    if (x >= 0 && n >= 0 && x < lpConfig->width && n < lpConfig->height && (node = FUN_0041c890(x, n)) != NULL) {
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
    if (n >= 0 && y >= 0 && n < lpConfig->width && y < lpConfig->height && (node = FUN_0041c890(n, y)) != NULL) {
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
struct PathNode *FUN_0041c890(unsigned int a, unsigned int b) {
    struct PathNode *current;
    unsigned short key;
    unsigned char stack_key[2];

    stack_key[0] = (unsigned char)a;
    stack_key[1] = (unsigned char)b;
    key = *(unsigned short *)stack_key;

    current = DAT_004d823c;
    while (current != NULL && current->tile.id != key) {
        current = current->next;
    }

    return current;
}

// FUNCTION: LEGOLAND 0x0041c8c0
int FUN_0041c8c0(int a, int b, int c, int d) {
    struct PathNode *node;
    TileId key;
    int result;

    result = 0;
    for (node = DAT_004d823c; node != NULL; node = node->next) {
        node->field_c = 0;
    }
    node = FUN_0041c890(a, b);
    if (node == NULL) {
        return 0;
    }
    key.id = node->owner.id;
    FUN_0041c940(a, b, c, d, &key, &result);
    return result;
}

// FUNCTION: LEGOLAND 0x0041c940
void FUN_0041c940(int x, int y, int tx, int ty, TileId *owner, int *found) {
    struct PathNode *node;
    struct PathNode *next;

    if (*found == 1) {
        return;
    }
    node = FUN_0041c890(x, y);
    if (node == NULL || node->owner.id != owner->id) {
        return;
    }
    if (x == tx && y == ty) {
        *found = 1;
        return;
    }
    node->field_c = 1;
    if ((node->field_4 & 1) != 0 && (next = FUN_0041c890(x, y - 5)) != NULL && next->field_c == 0) {
        FUN_0041c940(x, y - 5, tx, ty, owner, found);
    }
    if ((node->field_4 & 2) != 0 && (next = FUN_0041c890(x + 5, y)) != NULL && next->field_c == 0) {
        FUN_0041c940(x + 5, y, tx, ty, owner, found);
    }
    if ((node->field_4 & 4) != 0 && (next = FUN_0041c890(x, y + 5)) != NULL && next->field_c == 0) {
        FUN_0041c940(x, y + 5, tx, ty, owner, found);
    }
    if ((node->field_4 & 8) != 0 && (next = FUN_0041c890(x - 5, y)) != NULL && next->field_c == 0) {
        FUN_0041c940(x - 5, y, tx, ty, owner, found);
    }
}

// FUNCTION: LEGOLAND 0x0041caa0
void FUN_0041caa0(unsigned short param_1) {
    struct BoatRideNode *score = DAT_004cc074;
    struct PathNode *node;
    struct PathNode *tmp;

    for (node = DAT_004d823c; node != NULL; node = node->next) {
        if (node->owner.id == param_1) {
            node->field_18 = NULL;
        }
    }
    while (score != NULL && score->id != param_1) {
        score = score->next;
    }
    node = FUN_0041c890(score->end.pos.x, score->end.pos.y);
    node->field_8 = 0;
    node->field_14 = NULL;
    DAT_004d8240 = node;
    DAT_004d8244 = NULL;
    do {
        FUN_0041cb20(param_1);
        tmp = DAT_004d8244;
        DAT_004d8240 = tmp;
        DAT_004d8244 = NULL;
    } while (tmp != NULL);
}

// FUNCTION: LEGOLAND 0x0041cb20
void FUN_0041cb20(short param_1) {
    struct PathNode *p;
    struct PathNode *n1;
    struct PathNode *n2;
    struct PathNode *n3;
    struct PathNode *n4;

    for (p = DAT_004d8240; p != NULL; p = p->field_14) {
        n1 = FUN_0041c890(p->tile.pos.x, p->tile.pos.y - 5);
        n2 = FUN_0041c890(p->tile.pos.x + 5, p->tile.pos.y);
        n3 = FUN_0041c890(p->tile.pos.x, p->tile.pos.y + 5);
        n4 = FUN_0041c890(p->tile.pos.x - 5, p->tile.pos.y);
        if (n1 != NULL && (short)n1->owner.id == param_1 && n1->field_18 == NULL) {
            n1->field_18 = p;
            n1->field_8 = p->field_8 + 1;
            n1->field_14 = DAT_004d8244;
            DAT_004d8244 = n1;
        }
        if (n2 != NULL && (short)n2->owner.id == param_1 && n2->field_18 == NULL) {
            n2->field_18 = p;
            n2->field_8 = p->field_8 + 1;
            n2->field_14 = DAT_004d8244;
            DAT_004d8244 = n2;
        }
        if (n3 != NULL && (short)n3->owner.id == param_1 && n3->field_18 == NULL) {
            n3->field_18 = p;
            n3->field_8 = p->field_8 + 1;
            n3->field_14 = DAT_004d8244;
            DAT_004d8244 = n3;
        }
        if (n4 != NULL && (short)n4->owner.id == param_1 && n4->field_18 == NULL) {
            n4->field_18 = p;
            n4->field_8 = p->field_8 + 1;
            n4->field_14 = DAT_004d8244;
            DAT_004d8244 = n4;
        }
    }
}
