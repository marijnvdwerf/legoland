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

// FUNCTION: LEGOLAND 0x00432ac0
void FUN_00432ac0(void) {
    int i;
    float a;

    for (i = 0; i < 16; i++) {
        a = (i * DAT_004ab3e8) * DAT_004ab3f4;
        DAT_0081cb80[0][i].x = (int)(sin(a) * DAT_004ab4b0);
        DAT_0081cb80[0][i].y = (int)(cos(a) * DAT_004ab4a8);
        a = (i * DAT_004ab3e8 + DAT_004ab3ec - DAT_004ab3fc) * DAT_004ab3f4;
        DAT_0081cb80[1][i].x = (int)(sin(a) * DAT_004ab4b0);
        DAT_0081cb80[1][i].y = (int)(cos(a) * DAT_004ab4a8);
        a = (i * DAT_004ab3e8 + DAT_004ab4a0) * DAT_004ab3f4;
        DAT_0081cb80[2][i].x = (int)(sin(a) * DAT_004ab4b0);
        DAT_0081cb80[2][i].y = (int)(cos(a) * DAT_004ab4a8);
    }
}

// FUNCTION: LEGOLAND 0x00432b90
int FUN_00432b90(TileId tile, unsigned int bloke0, unsigned int bloke1, unsigned int bloke2) {
    struct JungleScore *score;
    struct JungleRide *node = DAT_00616164;
    struct JungleRide *fresh;

    for (score = DAT_00629c3c; score != NULL; score = score->next) {
        if (score->field_0 == tile.id) {
            break;
        }
    }
    for (; node != NULL; node = node->next) {
        if (node->tile.id == tile.id) {
            if (node->field_4 == score->start.pos.x && node->field_8 == score->start.pos.y) {
                return 0;
            }
            if (node->field_c == score->start.pos.x && node->field_10 == score->start.pos.y) {
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
    fresh->next = DAT_00616164;
    fresh->tile.id = tile.id;
    fresh->field_4 = tile.pos.x;
    fresh->field_8 = tile.pos.y + 5;
    fresh->field_c = tile.pos.x;
    fresh->field_10 = tile.pos.y + 5;
    fresh->field_3dc = 1;
    fresh->field_3e0 = 1;
    fresh->field_3e4 = (rand() & 0xf) + 4;
    fresh->blokes[0] = bloke0;
    fresh->blokes[1] = bloke1;
    fresh->blokes[2] = bloke2;
    DAT_00616164 = fresh;
    memset(fresh->field_1c, 0xf1, sizeof(fresh->field_1c));
    memset(fresh->field_29c, 0, sizeof(fresh->field_29c));
    return 1;
}

// FUNCTION: LEGOLAND 0x00432cb0
void FUN_00432cb0(struct JungleRide *ride) {
    struct JungleRide *prev = NULL;
    struct JungleRide *node = DAT_00616164;

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
        DAT_00616164 = node->next;
    }
    free(ride);
}

// FUNCTION: LEGOLAND 0x00432d00
void FUN_00432d00(int param_1) {
    struct JungleRide *ride = DAT_00616164;
    int tw;
    int tw2;
    int th2;
    int th;
    struct Point off;
    int a;
    int b;
    int dx;
    int dy;
    int baseX;
    int baseY;
    int row;
    int seat;
    int d;
    struct Sprite *sprite;
    struct Person *person;

    GetTileDimensions(&tw, &th);
    while (ride != NULL) {
        if (param_1 != 0 ? (ride->field_3e0 == 1 || ride->field_3e0 == 0x10 ||
                               (ride->field_4 == ride->tile.pos.x && (int)ride->field_8 >= ride->tile.pos.y + 5))
                         : !(ride->field_3e0 == 1 || ride->field_3e0 == 0x10 ||
                               (ride->field_4 == ride->tile.pos.x && (int)ride->field_8 >= ride->tile.pos.y + 5))) {
            b = ride->field_1c[DAT_00629c54 * 2 + 1];
            a = ride->field_1c[DAT_00629c54 * 2];
            GetTileDimensions(&tw2, &th2);
            dx = (a - b) * tw2 >> 9;
            dy = (a + b) * th2 >> 9;
            baseX = ((int)ride->field_4 - (int)ride->field_8) * (tw >> 1) - ((tw + 1) >> 1) - (ScrollX >> 8);
            baseY = ((int)ride->field_4 + (int)ride->field_8) * (th >> 1) - (ScrollY >> 8);
            off.x = DAT_0081cd00->offset_x[ride->field_29c[DAT_00629c54] & 0xff] >> 1;
            off.y = DAT_0081cd00->offset_y[ride->field_29c[DAT_00629c54] & 0xff] >> 1;
            AdjustOffsetForViewMode(&off);
            ride->field_14 = lpConfig->field_20 + dx + off.x + baseX;
            ride->field_18 = lpConfig->field_22 + dy + off.y + baseY;
            PrintSprite(DAT_0081cd00->sprites[ride->field_29c[DAT_00629c54] & 0xff], ride->field_14, ride->field_18, 0, 0);
            off.x = lpConfig->field_20 + dx + baseX;
            off.y = lpConfig->field_22 + dy + baseY;
            AdjustBlokePosition((struct BlokePos *)&off);
            if ((int)ride->field_29c[DAT_00629c54] >= 4 && (int)ride->field_29c[DAT_00629c54] < 12) {
                for (row = 0; row < 3; row++) {
                    struct Point pos;

                    d = 0;
                    if ((int)ride->field_29c[DAT_00629c54] > 8) {
                        if (row == 1) {
                            d = 1;
                        } else if (row == 2) {
                            d = -1;
                        }
                    }
                    seat = d + row;
                    if (ride->blokes[seat] != NULL) {
                        person = Find3DPersonFromBloke(ride->blokes[seat]);
                        pos.x = DAT_0081cb80[seat][ride->field_29c[DAT_00629c54] & 0xf].x + 0x20;
                        pos.y = DAT_0081cb80[seat][ride->field_29c[DAT_00629c54] & 0xf].y + 0x18;
                        switch (seat) {
                        case 0:
                            person->field_44 = ((float)(int)ride->field_29c[DAT_00629c54] * DAT_004ab3e8 + DAT_004ab3e4) * DAT_004ab3dc * DAT_004ab3e0;
                            break;
                        case 1:
                            person->field_44 = ((float)(int)((ride->field_29c[DAT_00629c54] + 6) & 0xf) * DAT_004ab3e8 + DAT_004ab3e4) * DAT_004ab3dc * DAT_004ab3e0;
                            pos.y -= 0x10;
                            break;
                        case 2:
                            person->field_44 = ((float)(int)((ride->field_29c[DAT_00629c54] - 6) & 0xf) * DAT_004ab3e8 + DAT_004ab3e4) * DAT_004ab3dc * DAT_004ab3e0;
                            pos.y -= 0x10;
                            break;
                        }
                        SetPersonRotation(person, &person->field_40);
                        AdjustOffsetForViewMode(&pos);
                        person->field_1c = pos.x + off.x;
                        person->field_20 = pos.y + off.y;
                        IP_RenderBlokeIn3DNow(ride->blokes[seat]);
                    }
                    if (row == 0 || row == 2) {
                        sprite = DAT_0081cd00->sprites[(row == 0 ? ride->field_29c[DAT_00629c54] + 0x10 : ride->field_29c[DAT_00629c54] + 0x20) & 0xff];
                        PrintSprite(sprite, ride->field_14, ride->field_18, 0, 0);
                    }
                }
            } else {
                for (row = 2; row >= 0; row--) {
                    struct Point pos;

                    d = 0;
                    if ((int)ride->field_29c[DAT_00629c54] < 8) {
                        if (row == 1) {
                            d = 1;
                        } else if (row == 2) {
                            d = -1;
                        }
                    }
                    seat = d + row;
                    if (ride->blokes[seat] != NULL) {
                        person = Find3DPersonFromBloke(ride->blokes[seat]);
                        pos.x = DAT_0081cb80[seat][ride->field_29c[DAT_00629c54] & 0xf].x + 0x20;
                        pos.y = DAT_0081cb80[seat][ride->field_29c[DAT_00629c54] & 0xf].y + 0x18;
                        switch (seat) {
                        case 0:
                            person->field_44 = ((float)(int)ride->field_29c[DAT_00629c54] * DAT_004ab3e8 + DAT_004ab3e4) * DAT_004ab3dc * DAT_004ab3e0;
                            break;
                        case 1:
                            person->field_44 = ((float)(int)((ride->field_29c[DAT_00629c54] + 6) & 0xf) * DAT_004ab3e8 + DAT_004ab3e4) * DAT_004ab3dc * DAT_004ab3e0;
                            pos.y -= 0x10;
                            break;
                        case 2:
                            person->field_44 = ((float)(int)((ride->field_29c[DAT_00629c54] - 6) & 0xf) * DAT_004ab3e8 + DAT_004ab3e4) * DAT_004ab3dc * DAT_004ab3e0;
                            pos.y -= 0x10;
                            break;
                        }
                        SetPersonRotation(person, &person->field_40);
                        AdjustOffsetForViewMode(&pos);
                        person->field_1c = pos.x + off.x;
                        person->field_20 = pos.y + off.y;
                        IP_RenderBlokeIn3DNow(ride->blokes[seat]);
                    }
                    if (row == 0 || row == 1) {
                        sprite = DAT_0081cd00->sprites[(row == 0 ? ride->field_29c[DAT_00629c54] + 0x20 : ride->field_29c[DAT_00629c54] + 0x10) & 0xff];
                        PrintSprite(sprite, ride->field_14, ride->field_18, 0, 0);
                    }
                }
            }
        }
        if (ride->field_3e0 == 0x10 && ride->field_3e4 == 2 && DAT_00629c54 == 0x4f && param_1 != 0 && ride->blokes[0] != NULL) {
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

// FUNCTION: LEGOLAND 0x004332c0
unsigned int FUN_004332c0(unsigned short *param_1) {
    struct JungleRide *node = DAT_00616164;
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
void FUN_004332f0(void) {
    struct JungleRide *node = DAT_00616164;
    struct JungleRide *cur;

    while (node != NULL) {
        node->field_4 = node->field_c;
        node->field_8 = node->field_10;
        if (node->field_3e0 == 0x10) {
            cur = node;
            node = FUN_004333e0(node);
            if (node != cur) {
                continue;
            }
        } else {
            switch (node->field_3e0) {
            case 1:
                FUN_004333b0(node);
                break;
            case 4:
                FUN_004334c0(node, 0);
                break;
            case 8:
                FUN_004334c0(node, 1);
                break;
            case 0x10:
                node = FUN_004333e0(node);
                break;
            }
        }
        if (node == NULL) {
            return;
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x004333b0
void FUN_004333b0(struct JungleRide *param_1) {
    FUN_00433840(param_1, param_1->field_3dc, 4);
    param_1->field_3e0 = 4;
    param_1->field_10 = param_1->field_8 + 5;
}

// FUNCTION: LEGOLAND 0x004333e0
struct JungleRide *FUN_004333e0(struct JungleRide *param_1) {
    unsigned int *p;
    int i;
    struct JungleRide *result;

    if (param_1->field_3e4 == 0) {
        result = param_1->next;
        FUN_00432cb0(param_1);
        return result;
    }
    FUN_00433840(param_1, 1, 4);
    param_1->field_3dc = 1;
    if (param_1->field_3e4 == 3) {
        p = &param_1->field_1c[0x81];
        i = 0x10;
        do {
            p[-1] = param_1->field_1c[0x80];
            *p = param_1->field_1c[0x81];
            p = p + 2;
            i = i + -1;
        } while (i != 0);
    } else if (param_1->field_3e4 == 2) {
        p = &param_1->field_1c[1];
        i = 0x40;
        do {
            p[-1] = param_1->field_1c[0x80];
            *p = param_1->field_1c[0x81];
            p = p + 2;
            i = i + -1;
        } while (i != 0);
    }
    if (param_1->field_3e4 != 3) {
        param_1->field_10 = param_1->field_8 + 5;
    }
    if (param_1->field_3e4 == 1) {
        p = &param_1->field_1c[0x9e];
        i = 7;
        do {
            p[0] = param_1->field_1c[0x90];
            p[1] = param_1->field_1c[0x91];
            p = p + -2;
            i = i + -1;
        } while (i != 0);
    }
    param_1->field_3e4 = param_1->field_3e4 - 1;
    return param_1;
}

// FUNCTION: LEGOLAND 0x004334c0
void FUN_004334c0(struct JungleRide *ride, int param_2) {
    struct JungleScore *score = DAT_00629c3c;
    struct JungleRide *other = DAT_00616164;
    struct JunglePath *path;
    unsigned int mask;
    unsigned int free;
    struct Point d;
    int i;
    int n;
    int back;
    int step;
    int dir;

    path = FUN_004371b0(ride->field_4, ride->field_8);
    for (; score != NULL; score = score->next) {
        if (path->owner.id == score->field_0) {
            break;
        }
    }
    mask = path->field_4;
    if (path->tile.id == score->start.id) {
        mask &= ~1;
    } else if (path->tile.id == score->end.id) {
        ride->field_3e0 = 0x10;
        ride->field_3e4 = 3;
        FUN_00433840(ride, ride->field_3dc, 4);
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
        FUN_00433840(ride, ride->field_3dc, -1);
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
        FUN_00433840(ride, ride->field_3dc, dir);
        ride->field_3dc = 4;
        break;
    case 2:
        ride->field_c = ride->field_4 + 5;
        ride->field_10 = ride->field_8;
        FUN_00433840(ride, ride->field_3dc, dir);
        ride->field_3dc = 8;
        break;
    case 4:
        ride->field_c = ride->field_4;
        ride->field_10 = ride->field_8 + 5;
        FUN_00433840(ride, ride->field_3dc, dir);
        ride->field_3dc = 1;
        break;
    case 8:
        ride->field_c = ride->field_4 - 5;
        ride->field_10 = ride->field_8;
        FUN_00433840(ride, ride->field_3dc, dir);
        ride->field_3dc = 2;
        break;
    }
    if (--ride->field_3e4 == 0) {
        ride->field_3e0 = 8;
    }
}

// FUNCTION: LEGOLAND 0x00433840
void FUN_00433840(struct JungleRide *param_1, unsigned int param_2, unsigned int param_3) {
    int *p;
    int i;
    int bit;
    unsigned int idx;
    float *table;
    int cx;
    int cy;
    int tx;
    int ty;
    int dx;
    int dy;
    int angle;

    table = NULL;
    if (param_3 == 0xffffffff) {
        if (param_2 == 0xffffffff) {
            p = param_1->field_1c;
            for (i = 0xa0; i != 0; i = i - 1) {
                *p = 0;
                p = p + 1;
            }
        } else {
            bit = 0;
            do {
                if ((param_2 & (1 << bit)) != 0) {
                    break;
                }
                bit = bit + 1;
            } while (bit < 4);
            cx = FUN_00458930((float)(DAT_004b7148[bit * 4] * 0x50) * DAT_004ab3fc);
            cy = FUN_00458930((float)(DAT_004b7148[bit * 4 + 1] * 0x50) * DAT_004ab3fc);
            p = &param_1->field_1c[1];
            i = 0;
            do {
                cx = FUN_00458930((float)(DAT_004b7148[0] * i) * DAT_004ab3fc) + cx;
                p[-1] = cx;
                cy = FUN_00458930((float)(DAT_004b7148[1] * i) * DAT_004ab3fc) + cy;
                p[0] = cy;
                i = i + 1;
                p = p + 2;
            } while (i < 0x28);
        }
    } else if (param_2 == 0xffffffff) {
        bit = 0;
        do {
            if ((param_3 & (1 << bit)) != 0) {
                break;
            }
            bit = bit + 1;
        } while (bit < 4);
        idx = (bit + 2) & 0x80000003;
        if ((int)idx < 0) {
            idx = ((idx - 1) | 0xfffffffc) + 1;
        }
        i = 0;
        p = param_1->field_1c;
        do {
            if (i < 0x28) {
                p[0] = 0;
                p[1] = 0;
            } else {
                p[0] = DAT_004b7148[idx * 4] * 0x10 + p[-2];
                p[1] = DAT_004b7148[idx * 4 + 1] * 0x10 + p[-1];
            }
            i = i + 1;
            p = p + 2;
        } while (i < 0x50);
    } else {
        if (param_2 == 1) {
            param_2 = 0x11;
        }
        if (param_3 == 1) {
            param_3 = 0x11;
        }
        if (((int)param_3 < (int)param_2 && (param_3 & ((int)param_2 >> 2)) == 0) ||
            ((int)param_3 >= (int)param_2 && param_3 != param_2 && (param_2 & ((int)param_3 >> 2)) == 0)) {
            if ((param_3 & param_2 * 2) != 0) {
                table = DAT_004b7188;
            } else if ((param_2 & param_3 * 2) != 0) {
                table = DAT_004b71c8;
            }
            bit = 0;
            do {
                if ((param_2 & (1 << bit)) != 0) {
                    break;
                }
                bit = bit + 1;
            } while (bit < 4);
            cx = FUN_00458930((float)((sin((double)table[bit * 4] * (double)DAT_004ab3f4) + (double)DAT_004ab3ec) * (double)DAT_004ab3f4));
            param_1->field_1c[0] = cx;
            cy = FUN_00458930((float)((cos((double)table[bit * 4] * (double)DAT_004ab3f4) + (double)DAT_004ab3ec) * (double)DAT_004ab3f4));
            param_1->field_1c[1] = cy;
            p = &param_1->field_1c[3];
            i = 0x4f;
            do {
                cx = FUN_00458930((float)sin((double)DAT_004ab3f4));
                p[-1] = cx;
                cy = FUN_00458930((float)cos((double)DAT_004ab3f4));
                p[0] = cy;
                p = p + 2;
                i = i - 1;
            } while (i != 0);
        } else {
            bit = 0;
            do {
                if ((param_2 & (1 << bit)) != 0) {
                    break;
                }
                bit = bit + 1;
            } while (bit < 4);
            FUN_00458930((float)0);
            FUN_00458930((float)0);
            if (param_2 == param_3) {
                i = 0x50;
                p = &param_1->field_1c[1];
                do {
                    p[-1] = FUN_00458930((float)0);
                    p[0] = FUN_00458930((float)0);
                    p = p + 2;
                    i = i - 1;
                } while (0 < i);
            } else {
                i = 0;
                p = &param_1->field_1c[1];
                do {
                    p[-1] = FUN_00458930((float)0);
                    p[0] = FUN_00458930((float)0);
                    i = i + 1;
                    p = p + 2;
                } while (i < 0x50);
            }
        }
    }
    i = 0;
    p = param_1->field_29c;
    do {
        if (i < 0x4c) {
            tx = param_1->field_1c[i * 2 + 0x1c];
            ty = param_1->field_1c[i * 2 + 0x1d];
        } else {
            tx = param_1->field_1c[0x9e];
            ty = param_1->field_1c[0x9f];
        }
        if (i < 4) {
            cx = param_1->field_1c[0];
            cy = param_1->field_1c[1];
        } else {
            cx = param_1->field_1c[i * 2];
            cy = param_1->field_1c[i * 2 + 1];
        }
        dx = tx - cx;
        dy = ty - cy;
        angle = ArcTan256(dx, dy);
        *p = (unsigned int)((angle >> 4) + 6) & 0xf;
        i = i + 1;
        p = p + 1;
    } while (i < 0x50);
}

// FUNCTION: LEGOLAND 0x00433ca0
void FUN_00433ca0(struct RideObject *obj) {
    struct Ride *ride = obj->ride;
    DAT_0081cb70 = ride;
    ride->flags |= 0x400;
    // STRING: LEGOLAND 0x004b720c
    DAT_0081cb68 = LoadSprite("brijmask.lls", 1);
}

// FUNCTION: LEGOLAND 0x00433cd0
void FUN_00433cd0(void) {
    KillSprite(DAT_0081cb68);
}

// FUNCTION: LEGOLAND 0x00433ce0
void FUN_00433ce0(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = DAT_0081cb70;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(&DAT_0081cb70->footprint);
}

// FUNCTION: LEGOLAND 0x00433d20
void FUN_00433d20(unsigned int param_1, int *param_2) {
    struct JungleObj *obj;
    unsigned char temp[2];
    unsigned short coord;
    int local_4;

    temp[0] = (unsigned char)param_2[0];
    temp[1] = (unsigned char)param_2[1];
    coord = *(unsigned short *)temp;
    FUN_00436fb0(param_2[0], param_2[1], (unsigned short *)&local_4);
    obj = (struct JungleObj *)malloc(8);
    if (obj != NULL) {
        obj->tile.id = coord;
        obj->owner = (unsigned short)local_4;
        obj->next = DAT_00629c2c;
        DAT_00629c2c = obj;
        AddBasicObject(param_1, (unsigned int)param_2);
        FUN_00436130((unsigned short)local_4, 1);
    }
}

// FUNCTION: LEGOLAND 0x00433d90
void FUN_00433d90(struct RideObject *obj, unsigned int param_2, unsigned int param_3) {
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
    mask = FUN_00436fb0(EditCursor.field_1404, EditCursor.field_1408, &owner);
    EditCursor.field_1830 = n;
    if (mask == 0) {
        FUN_0045f480(&EditCursor, 0xe);
        return;
    }
    ValidateCursor(&EditCursor, (unsigned int)ride);
    if (FUN_0045f4b0(&EditCursor) == 0) {
        return;
    }
    DefaultCursor(&DAT_00622320[0]);
    DefaultCursor(&DAT_00622320[1]);
    DefaultCursor(&DAT_00622320[2]);
    DefaultCursor(&DAT_00622320[3]);
    memcpy(DAT_00622320[0].field_1414, EditCursor.field_1414, 20);
    memcpy(DAT_00622320[1].field_1414, EditCursor.field_1414, 20);
    memcpy(DAT_00622320[2].field_1414, EditCursor.field_1414, 20);
    memcpy(DAT_00622320[3].field_1414, EditCursor.field_1414, 20);
    FUN_0045f460(&DAT_00622320[0]);
    FUN_0045f460(&DAT_00622320[1]);
    FUN_0045f460(&DAT_00622320[2]);
    FUN_0045f460(&DAT_00622320[3]);
    x = EditCursor.field_1404;
    y = EditCursor.field_1408;
    DAT_00622320[0].field_1828 = 0x2034;
    DAT_00622320[1].field_1828 = 0x2034;
    DAT_00622320[2].field_1828 = 0x2034;
    DAT_00622320[3].field_1828 = 0x2034;
    if ((mask & 1) != 0) {
        DAT_00622320[0].field_1404 = x;
        DAT_00622320[0].field_1408 = y - 5;
        n = 1;
    }
    if ((mask & 2) != 0) {
        DAT_00622320[n].field_1404 = x + 5;
        DAT_00622320[n].field_1408 = y;
        n++;
    }
    if ((mask & 4) != 0) {
        DAT_00622320[n].field_1404 = x;
        DAT_00622320[n].field_1408 = y + 5;
        n++;
    }
    if ((mask & 8) != 0) {
        DAT_00622320[n].field_1404 = x - 5;
        DAT_00622320[n].field_1408 = y;
        n++;
    }
    if (n != 0) {
        EditCursor.field_1830 = (unsigned int)&DAT_00622320[0];
        if (n > 1) {
            c = &DAT_00622320[1];
            n--;
            do {
                c[-1].field_1830 = (unsigned int)c;
                c++;
                n--;
            } while (n != 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x00433fa0
unsigned int FUN_00433fa0(unsigned int param_1, unsigned int param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00433fc0
void FUN_00433fc0(void *param_1, TileId tile, struct Cursor *param_3) {
    struct JungleObj *node = DAT_00629c2c;
    struct JungleObj *prev = NULL;

    StandardRemoveObject((struct EditObject *)param_1, tile, param_3);
    while (node->tile.id != tile.id) {
        prev = node;
        node = node->next;
        if (node == NULL) {
            return;
        }
    }
    if (node != NULL) {
        FUN_00436130(node->owner, -1);
        if (prev != NULL) {
            prev->next = node->next;
            free(node);
            return;
        }
        DAT_00629c2c = node->next;
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x00434040
struct RideSpriteInfo *FUN_00434040(struct RideObject *obj, unsigned short param_2) {
    struct Ride *ride = obj->ride;
    DAT_0082c6a0.sprite = ride->layer;
    DAT_0082c6a0.x = ride->field_14;
    DAT_0082c6a0.y = ride->field_18;
    DAT_0082c6a0.id = param_2;
    return &DAT_0082c6a0;
}

// FUNCTION: LEGOLAND 0x00434080
void FUN_00434080(struct RideObject *obj) {
    struct Ride *ride = obj->ride;
    DAT_0081cb74 = ride;
    ride->flags |= 0x400;
    // STRING: LEGOLAND 0x004b721c
    DAT_0081cb6c = LoadSprite("mfish2.lls", 1);
}

// FUNCTION: LEGOLAND 0x004340b0
void FUN_004340b0(void) {
    KillSprite(DAT_0081cb6c);
}

// FUNCTION: LEGOLAND 0x004340c0
void FUN_004340c0(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = DAT_0081cb74;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(&DAT_0081cb74->footprint);
}

// FUNCTION: LEGOLAND 0x00434100
void FUN_00434100(struct EditObject *obj, int *coords) {
    TileId tile;
    struct JungleFish *fish;
    unsigned short owner;
    int x;
    int y;

    tile.pos.x = coords[0];
    tile.pos.y = coords[1];
    if (FUN_00436fb0(coords[0], coords[1], &owner) == 0) {
        FUN_00436fb0(coords[0], coords[1] - 5, &owner);
    }
    fish = (struct JungleFish *)malloc(sizeof(struct JungleFish));
    if (fish == NULL) {
        return;
    }
    fish->tile = tile;
    fish->owner = owner;
    fish->next = DAT_00629c30;
    fish->field_4 = 0;
    DAT_00629c30 = fish;
    FUN_00436130(fish->owner, 2);
    AddBasicObject(obj, coords);
    for (y = DAT_0081cb74->footprint.v[1]; y <= DAT_0081cb74->footprint.v[3]; y++) {
        for (x = DAT_0081cb74->footprint.v[0]; x <= DAT_0081cb74->footprint.v[2]; x++) {
            if (x == DAT_0081cb74->footprint.v[0]) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0081cb58->tiles + 9);
            } else if (x == DAT_0081cb74->footprint.v[2]) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0081cb58->tiles + 0xc);
            } else if (y == DAT_0081cb74->footprint.v[1]) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0081cb58->tiles + 10);
            } else if (y == DAT_0081cb74->footprint.v[3]) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0081cb58->tiles + 0xb);
            } else {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0081cb58->tiles);
            }
        }
    }
    SetMapTile(DAT_0081cb74->footprint.v[0] + coords[0], DAT_0081cb74->footprint.v[1] + coords[1], *DAT_0081cb58->tiles + 5);
    SetMapTile(DAT_0081cb74->footprint.v[2] + coords[0], DAT_0081cb74->footprint.v[1] + coords[1], *DAT_0081cb58->tiles + 8);
    SetMapTile(DAT_0081cb74->footprint.v[0] + coords[0], DAT_0081cb74->footprint.v[3] + coords[1], *DAT_0081cb58->tiles + 6);
    SetMapTile(DAT_0081cb74->footprint.v[2] + coords[0], DAT_0081cb74->footprint.v[3] + coords[1], *DAT_0081cb58->tiles + 7);
}

// FUNCTION: LEGOLAND 0x00434330
void FUN_00434330(struct RideObject *obj, unsigned int param_2, int *param_3) {
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
    ScreenToMapRef(param_2, &EditCursor.field_1404, param_3);
    EditCursor.field_1830 = 0;
    for (; row < 2; row++) {
        mask = FUN_00436fb0(EditCursor.field_1404, EditCursor.field_1408 - row * 5, &owners[row].id);
        if (row == 1 && owners[0].id != owners[1].id && owners[0].pos.x != 0) {
            return;
        }
        if (mask == 0 && row == 1) {
            FUN_0045f480(&EditCursor, 0xe);
            return;
        }
        ValidateCursor(&EditCursor, (unsigned int)ride);
        if (FUN_0045f4b0(&EditCursor) != 0) {
            memcpy(&fp, EditCursor.field_1414, sizeof(fp));
            fp.v[1] += 5;
            DefaultCursor(&DAT_00616180[row * 4]);
            DefaultCursor(&DAT_00616180[row * 4 + 1]);
            DefaultCursor(&DAT_00616180[row * 4 + 2]);
            DefaultCursor(&DAT_00616180[row * 4 + 3]);
            memcpy(DAT_00616180[row * 4].field_1414, &fp, sizeof(fp));
            memcpy(DAT_00616180[row * 4 + 1].field_1414, &fp, sizeof(fp));
            memcpy(DAT_00616180[row * 4 + 2].field_1414, &fp, sizeof(fp));
            memcpy(DAT_00616180[row * 4 + 3].field_1414, &fp, sizeof(fp));
            FUN_0045f460(&DAT_00616180[row * 4]);
            FUN_0045f460(&DAT_00616180[row * 4 + 1]);
            FUN_0045f460(&DAT_00616180[row * 4 + 2]);
            FUN_0045f460(&DAT_00616180[row * 4 + 3]);
            DAT_00616180[row * 4].field_1828 = 0x2034;
            DAT_00616180[row * 4 + 1].field_1828 = 0x2034;
            DAT_00616180[row * 4 + 2].field_1828 = 0x2034;
            DAT_00616180[row * 4 + 3].field_1828 = 0x2034;
            if ((mask & 1) != 0) {
                DAT_00616180[n].field_1404 = EditCursor.field_1404;
                DAT_00616180[n].field_1408 = EditCursor.field_1408 - (row * 5 + 5);
                n++;
            }
            if ((mask & 2) != 0) {
                DAT_00616180[n].field_1404 = EditCursor.field_1404 + 5;
                DAT_00616180[n].field_1408 = EditCursor.field_1408 - row * 5;
                n++;
            }
            if ((mask & 4) != 0) {
                DAT_00616180[n].field_1404 = EditCursor.field_1404;
                DAT_00616180[n].field_1408 = EditCursor.field_1408 - row * 5 + 5;
                n++;
            }
            if ((mask & 8) != 0) {
                DAT_00616180[n].field_1404 = EditCursor.field_1404 - 5;
                DAT_00616180[n].field_1408 = EditCursor.field_1408 - row * 5;
                n++;
            }
            if (n != 0) {
                EditCursor.field_1830 = (unsigned int)&DAT_00616180[0];
                for (i = 1; i < n; i++) {
                    DAT_00616180[i - 1].field_1830 = (unsigned int)&DAT_00616180[i];
                }
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00434650
unsigned int FUN_00434650(unsigned int param_1, unsigned int param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00434670
void FUN_00434670(void *param_1, TileId tile, struct Cursor *param_3) {
    struct JungleFish *node = DAT_00629c30;
    struct JungleFish *prev = NULL;
    int x;
    int y;

    for (y = DAT_0081cb74->footprint.v[1]; y <= DAT_0081cb74->footprint.v[3]; y++) {
        for (x = DAT_0081cb74->footprint.v[0]; x <= DAT_0081cb74->footprint.v[2]; x++) {
            RestoreBaseMap(param_3->field_1404 + x, param_3->field_1408 + y);
        }
    }
    StandardRemoveObject((struct EditObject *)param_1, tile, param_3);
    while (node->tile.id != tile.id) {
        prev = node;
        node = node->next;
        if (node == NULL) {
            return;
        }
    }
    if (node != NULL) {
        FUN_00436130(node->owner, -2);
        if (prev != NULL) {
            prev->next = node->next;
            free(node);
            return;
        }
        DAT_00629c30 = node->next;
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x00434740
struct RideSpriteInfo *FUN_00434740(struct RideObject *obj, short param_2) {
    struct Ride *ride = obj->ride;
    struct JungleFish *node = DAT_00629c30;
    short *lls;
    struct LLS *fish_lls;

    while (node != NULL && node->tile.id != (unsigned short)param_2) {
        node = node->next;
    }

    lls = (short *)GetLLSForSprite((struct SpriteLLS *)ride->layer);
    if (node->field_4 != 0) {
        if (*lls == 0) {
            node->field_4 = 0;
        }
    } else if (*lls == 1 && rand() % 7 == 0) {
        node->field_4 = 1;
    }

    if (node->field_4 != 0) {
        fish_lls = (struct LLS *)GetLLSForSprite((struct SpriteLLS *)DAT_0081cb6c);
        LLSSetFrame(fish_lls, *lls);
        DAT_0082c6a0.sprite = DAT_0081cb6c;
    } else {
        DAT_0082c6a0.sprite = ride->layer;
    }
    DAT_0082c6a0.x = ride->field_14;
    DAT_0082c6a0.y = ride->field_18;
    DAT_0082c6a0.id = param_2;
    return &DAT_0082c6a0;
}

// FUNCTION: LEGOLAND 0x00434b40
void FUN_00434b40(void *param_1, TileId tile, struct Cursor *param_3) {
    struct JungleObj *node = DAT_00629c34;
    struct JungleObj *prev = NULL;
    struct MapElement *elem;
    int x;
    int y;

    param_3->field_1414[1] = -1;
    param_3->field_1414[3] = 1;
    param_3->field_1414[0] = 0;
    param_3->field_1414[2] = 0;
    StandardRemoveObject((struct EditObject *)param_1, tile, param_3);
    x = param_3->field_1404 - 6;
    y = param_3->field_1408;
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
        DAT_00629c34 = node->next;
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x00434cb0
void FUN_00434cb0(struct RideObject *obj) {
    unsigned int handle;
    int i;
    struct Sprite *sprite;

    DAT_0081cb60 = obj->ride;
    DAT_0081cb60->flags |= 0x20;
    DAT_0081cb60->layer->flags |= 0x2000;
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", &handle, 0) == 0) {
        DAT_0081cb58 = (struct TileMap *)LLIDB_LoadData((void *)handle);
    }
    if (LLIDB_FindElement("JUNGLE CRUISE BOATS", &handle, 0) == 0) {
        DAT_0081cd00 = (struct SpriteSet *)LLIDB_LoadData((void *)handle);
    }
    for (i = 0; i < DAT_0081cd00->count; i++) {
        sprite = DAT_0081cd00->sprites[(unsigned char)i];
        LLSPlay((struct LLS *)GetLLSForSprite((struct SpriteLLS *)sprite), *(unsigned int *)((char *)sprite + 8));
    }
    // STRING: LEGOLAND 0x004b72b8
    DAT_0081cb5c = LoadSprite("jungmask.lls", 1);
    DAT_00629c40 = DAT_0081cb60->footprint;
    DAT_004b7260 = DAT_004b7230;
    DAT_004b7260.v[1] += DAT_00629c40.v[1];
    DAT_004b7260.v[0] += DAT_00629c40.v[0];
    DAT_004b7260.v[2] += DAT_00629c40.v[0];
    DAT_004b7260.v[3] += DAT_00629c40.v[1];
    DAT_004b7278 = DAT_004b7248;
    DAT_004b7278.v[1] += DAT_00629c40.v[3] + 1;
    DAT_004b7278.v[0] += DAT_00629c40.v[0];
    DAT_004b7278.v[2] += DAT_00629c40.v[0];
    DAT_004b7278.v[3] += DAT_00629c40.v[3] + 1;
    FUN_00432ac0();
}

// FUNCTION: LEGOLAND 0x00434e50
void FUN_00434e50(struct RideObject *obj) {
    unsigned int handle;
    int i;
    struct Sprite *sprite;
    struct JungleScore *score;
    struct JunglePath *path;

    DAT_0081cb60 = obj->ride;
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", &handle, 0) == 0) {
        LLIDB_UnLoadData(handle);
    }
    for (i = 0; i < DAT_0081cd00->count; i++) {
        sprite = DAT_0081cd00->sprites[(unsigned char)i];
        LLSStop(GetLLSForSprite((struct SpriteLLS *)sprite));
    }
    // STRING: LEGOLAND 0x004b72c8
    if (LLIDB_FindElement("JUNGLE CRUISE BOATS", &handle, 0) == 0) {
        LLIDB_UnLoadData(handle);
    }
    while (DAT_00629c3c != NULL) {
        score = DAT_00629c3c->next;
        free(DAT_00629c3c);
        DAT_00629c3c = score;
    }
    while (DAT_00616164 != NULL) {
        FUN_00432cb0(DAT_00616164);
    }
    while (DAT_0062fd2c != NULL) {
        path = DAT_0062fd2c->next;
        free(DAT_0062fd2c);
        DAT_0062fd2c = path;
    }
    KillSprite(DAT_0081cb5c);
}

// FUNCTION: LEGOLAND 0x00434f50
void FUN_00434f50(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = DAT_0081cb60;
    DefaultCursor(&EditCursor);
    DAT_00629c50 = &DAT_004b7278;
    DAT_004b7288 = &DAT_004b7260;
    SetEditCursorFootPrint(&DAT_00629c40);
}

// FUNCTION: LEGOLAND 0x00434f90
void FUN_00434f90(struct EditObject *obj, int *coords) {
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
    score->field_0 = tile.id;
    score->start.pos.x = coords[0] + DAT_004b7278.v[0] + 2;
    score->start.pos.y = coords[1] + DAT_004b7278.v[1] + 2;
    score->end.pos.x = coords[0] + DAT_004b7260.v[0] + 2;
    score->end.pos.y = coords[1] + DAT_004b7260.v[1] + 2;
    score->field_8 = 0;
    score->field_c = 9999;
    score->field_10 = 0;
    score->field_14 = 0;
    score->timer = 150;
    score->field_40 = 3;
    for (x = 0; x < 5; x++) {
        score->blokes[x] = 0;
    }
    for (x = 0; x < 3; x++) {
        score->seats[x] = NULL;
    }
    score->next = DAT_00629c3c;
    DAT_00629c3c = score;
    AddBasicObject(obj, coords);
    FUN_00436dc0(coords[0] + DAT_004b7278.v[0] + 2, coords[1] + DAT_004b7278.v[1] + 2, 1, &score->field_0);
    FUN_00436dc0(coords[0] + DAT_004b7260.v[0] + 2, coords[1] + DAT_004b7260.v[1] + 2, 4, &score->field_0);
    for (y = DAT_00629c40.v[1]; y <= DAT_00629c40.v[3]; y++) {
        for (x = DAT_00629c40.v[0]; x <= DAT_00629c40.v[2] - 1; x++) {
            if (x == DAT_00629c40.v[0]) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0081cb58->tiles + 9);
            } else if (x == DAT_00629c40.v[2] - 1) {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0081cb58->tiles + 0xc);
            } else {
                SetMapTile(coords[0] + x, coords[1] + y, *DAT_0081cb58->tiles);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00435150
void FUN_00435150(struct RideObject *obj, unsigned int param_2, unsigned int param_3) {
    memcpy(EditCursor.field_1414, &DAT_00629c40, sizeof(DAT_00629c40));
    EditCursor.field_1830 = 0;
    DAT_00629c50 = &DAT_004b7278;
    DAT_004b7288 = &DAT_004b7260;
    DAT_004b7260.v[4] = 0;
    ScreenToMapRef(param_2, &EditCursor.field_1404, param_3);
    FUN_0045f460(&EditCursor);
    PathCursor.field_1404 = EditCursor.field_1404;
    PathCursor.field_1408 = EditCursor.field_1408;
    PathCursor.field_1414[0] = EditCursor.field_1414[2] + 1;
    PathCursor.field_1414[1] = EditCursor.field_1414[1];
    PathCursor.field_1414[2] = PathCursor.field_1414[0];
    PathCursor.field_1414[3] = EditCursor.field_1414[3];
    PathCursor.field_1414[4] = 0;
    PathCursor.field_1828 = 0x1008;
    PathCursor.field_1830 = 0;
    FUN_0045f460(&PathCursor);
    EditCursor.field_1830 = (unsigned int)&PathCursor;
    PathCursor.field_1830 = 0;
    ValidateCursor(&EditCursor, (unsigned int)obj->ride);
}

// FUNCTION: LEGOLAND 0x00435230
void FUN_00435230(unsigned int param_1, struct Point *param_2) {
    struct JunglePath *p1 = DAT_0062fd2c;
    struct JungleFish *p3 = DAT_00629c30;
    struct JungleObj *p4 = DAT_00629c34;
    struct JungleObj *p2 = DAT_00629c2c;

    BasicObjectDCalcCursor(param_1, param_2);
    DefaultCursor(&DAT_0082ae20);
    memcpy(DAT_0082ae20.field_1414, &DAT_004b7478, sizeof(DAT_004b7478));
    for (; p1 != NULL; p1 = p1->next) {
        if (p1->owner.id == QueryObj.id) {
            DAT_0082ae20.field_1404 = p1->tile.pos.x;
            DAT_0082ae20.field_1408 = p1->tile.pos.y;
            FUN_0045f460(&DAT_0082ae20);
            DAT_0082ae20.field_1828 = 8;
            BuildCursorPtr(&DAT_0082ae20, 0, 0);
            RenderCursor(&DAT_0082ae20);
        }
    }
    for (; p2 != NULL; p2 = p2->next) {
        if (p2->owner == QueryObj.id) {
            DAT_0082ae20.field_1404 = p2->tile.pos.x;
            DAT_0082ae20.field_1408 = p2->tile.pos.y;
            FUN_0045f460(&DAT_0082ae20);
            DAT_0082ae20.field_1828 = 8;
            BuildCursorPtr(&DAT_0082ae20, 0, 0);
            RenderCursor(&DAT_0082ae20);
        }
    }
    DAT_0082ae20.field_1414[1] -= 5;
    for (; p3 != NULL; p3 = p3->next) {
        if (p3->owner == QueryObj.id) {
            DAT_0082ae20.field_1404 = p3->tile.pos.x;
            DAT_0082ae20.field_1408 = p3->tile.pos.y;
            FUN_0045f460(&DAT_0082ae20);
            DAT_0082ae20.field_1828 = 8;
            BuildCursorPtr(&DAT_0082ae20, 0, 0);
            RenderCursor(&DAT_0082ae20);
        }
    }
    for (; p4 != NULL; p4 = p4->next) {
        if (p4->owner == QueryObj.id) {
            DAT_0082ae20.field_1414[1] = -1;
            DAT_0082ae20.field_1414[3] = 1;
            DAT_0082ae20.field_1414[0] = 0;
            DAT_0082ae20.field_1414[2] = 0;
            DAT_0082ae20.field_1404 = p4->tile.pos.x;
            DAT_0082ae20.field_1408 = p4->tile.pos.y;
            FUN_0045f460(&DAT_0082ae20);
            DAT_0082ae20.field_1828 = 8;
            BuildCursorPtr(&DAT_0082ae20, 0, 0);
            RenderCursor(&DAT_0082ae20);
            DAT_0082ae20.field_1404 = DAT_0082ae20.field_1404 - 6;
            BuildCursorPtr(&DAT_0082ae20, 0, 0);
            RenderCursor(&DAT_0082ae20);
        }
    }
}

// FUNCTION: LEGOLAND 0x00435470
void FUN_00435470(struct RideObject *obj, TileId tile, struct Cursor *cursor) {
    struct JungleRide *ride = DAT_00616164;
    struct JungleScore *prev = NULL;
    struct JungleScore *score = DAT_00629c3c;
    struct JunglePath *path;
    struct JungleObj *thing;
    struct JungleFish *fish;
    struct RideObject fake;
    int x;
    int y;
    int savedX;
    int savedY;

    StandardRemoveObject((struct EditObject *)obj, tile, cursor);
    for (y = DAT_00629c40.v[1]; y <= DAT_00629c40.v[3]; y++) {
        for (x = DAT_00629c40.v[0]; x <= DAT_00629c40.v[2] - 1; x++) {
            RestoreBaseMap(cursor->field_1404 + x, cursor->field_1408 + y);
        }
    }
    while (score->field_0 != tile.id) {
        prev = score;
        score = score->next;
        if (score == NULL) {
            return;
        }
    }
    if (score != NULL) {
        fake.ride = DAT_0081cb54;
        IncrementObjectCount(DAT_0081cb54);
        IncrementObjectCount(DAT_0081cb54);
        memcpy(DAT_0082ae20.field_1414, &DAT_004b7478, sizeof(DAT_004b7478));
        path = DAT_0062fd2c;
        while (path != NULL) {
            if (path->owner.id == tile.id) {
                DAT_0082ae20.field_1404 = path->tile.pos.x;
                DAT_0082ae20.field_1408 = path->tile.pos.y;
                FUN_00436f30(&fake, path->tile, &DAT_0082ae20);
                path = DAT_0062fd2c;
            } else {
                path = path->next;
            }
        }
        fake.ride = DAT_0081cb70;
        thing = DAT_00629c2c;
        while (thing != NULL) {
            if (thing->owner == tile.id) {
                savedX = cursor->field_1404;
                savedY = cursor->field_1408;
                cursor->field_1404 = thing->tile.pos.x;
                cursor->field_1408 = thing->tile.pos.y;
                FUN_00433fc0(&fake, thing->tile, cursor);
                cursor->field_1404 = savedX;
                cursor->field_1408 = savedY;
                thing = DAT_00629c2c;
            } else {
                thing = thing->next;
            }
        }
        fake.ride = DAT_0081cb74;
        fish = DAT_00629c30;
        while (fish != NULL) {
            if (fish->owner == tile.id) {
                savedX = cursor->field_1404;
                savedY = cursor->field_1408;
                cursor->field_1404 = fish->tile.pos.x;
                cursor->field_1408 = fish->tile.pos.y;
                FUN_00434670(&fake, fish->tile, cursor);
                cursor->field_1404 = savedX;
                cursor->field_1408 = savedY;
                fish = DAT_00629c30;
            } else {
                fish = fish->next;
            }
        }
        fake.ride = DAT_0081cb64;
        thing = DAT_00629c34;
        while (thing != NULL) {
            if (thing->owner == tile.id) {
                savedX = cursor->field_1404;
                savedY = cursor->field_1408;
                cursor->field_1404 = thing->tile.pos.x;
                cursor->field_1408 = thing->tile.pos.y;
                FUN_00434b40(&fake, thing->tile, cursor);
                cursor->field_1404 = savedX;
                cursor->field_1408 = savedY;
                thing = DAT_00629c34;
            } else {
                thing = thing->next;
            }
        }
        if (prev != NULL) {
            prev->next = score->next;
        } else {
            DAT_00629c3c = score->next;
        }
        while (ride != NULL) {
            if (ride->tile.id == tile.id) {
                FUN_00432cb0(ride);
                ride = DAT_00616164;
            } else {
                ride = ride->next;
            }
        }
        RemoveAllBlokesFromRide(obj->ride, *(unsigned int *)&tile);
        free(score);
    }
}

// FUNCTION: LEGOLAND 0x00435750
void FUN_00435750(void) {
    struct RideNode *node = DAT_0081cb60->riders;
    struct RideNode *next;
    struct JungleScore *score;
    struct Bloke *bloke;
    struct Person *person;
    TileId tile;
    int slot;
    int i;
    char dir;
    struct Point pos;
    struct Point map;

    if (++DAT_00629c54 == 0x50) {
        DAT_00629c54 = 0;
        FUN_004332f0();
    }
    FUN_00432d00(0);
    for (score = DAT_00629c3c; score != NULL; score = score->next) {
        tile.id = score->field_0;
        if (score->seats[0] != NULL && --score->timer <= 0 && score->field_8 != 0 &&
            score->field_40 > (int)FUN_004332c0(&score->field_0) * 6 &&
            FUN_00432b90(tile, (unsigned int)score->seats[0], (unsigned int)score->seats[1], (unsigned int)score->seats[2]) != 0) {
            score->seats[0]->flags |= 0x80;
            score->seats[0]->param_action++;
            BlokeSitAnim(score->seats[0]);
            BlokeSetFrame(score->seats[0], 0);
            if (score->seats[1] != NULL) {
                score->seats[1]->flags |= 0x80;
                score->seats[1]->param_action++;
                BlokeSitAnim(score->seats[1]);
                BlokeSetFrame(score->seats[1], 0);
            }
            if (score->seats[2] != NULL) {
                score->seats[2]->flags |= 0x80;
                score->seats[2]->param_action++;
                BlokeSitAnim(score->seats[2]);
                BlokeSetFrame(score->seats[2], 0);
            }
            score->timer = 150;
            score->seats[0] = NULL;
            score->seats[1] = NULL;
            score->seats[2] = NULL;
        }
    }
    for (; node != NULL; node = next) {
        score = DAT_00629c3c;
        next = node->next;
        tile = node->tile;
        for (; score != NULL; score = score->next) {
            if (score->field_0 == tile.id) {
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
                if (score->blokes[i] == bloke) {
                    slot = i;
                    break;
                }
            }
            if (i == 5) {
                if (score->field_14 == 5 || score->blokes[4] != NULL) {
                    RemoveBlokeFromRide(DAT_0081cb60, node);
                    break;
                }
                score->blokes[slot] = bloke;
                score->field_14++;
            } else {
                bloke = score->blokes[slot];
                if (score->blokes[slot - 1] != NULL) {
                    break;
                }
                score->blokes[slot - 1] = bloke;
                score->blokes[slot] = NULL;
                if (--slot == 0) {
                    bloke->param_action++;
                }
            }
            bloke->flags |= 8;
            bloke->dest.x = ((DAT_0081cb60->x + tile.pos.x) << 8) + DAT_004b7290[4 - slot].x;
            bloke->dest.y = ((DAT_0081cb60->y + tile.pos.y) << 8) + DAT_004b7290[4 - slot].y;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->field_73 = dir + 0x10;
            bloke->field_e = 7;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            break;
        case 1:
            if (bloke == score->blokes[0]) {
                for (i = 0; i < 3; i++) {
                    if (score->seats[i] == NULL) {
                        bloke->pos.x = -9999;
                        bloke->pos.y = -9999;
                        score->seats[i] = bloke;
                        score->blokes[0] = NULL;
                        score->field_14--;
                        break;
                    }
                }
            }
            break;
        case 3:
            pos.x = 0;
            pos.y = 0;
            BlokeWalkAnim(bloke);
            BlokeSetFrame(bloke, 0);
            person = Find3DPersonFromBloke((unsigned int)bloke);
            AdjustBlokePosition((struct BlokePos *)&pos);
            pos.x = person->field_1c - pos.x - 0x10;
            pos.y = person->field_20 - pos.y;
            ScreenToMapRef2(&pos, &map, 0);
            bloke->flags &= 0xff7f;
            bloke->pos = map;
            bloke->field_72 = 10;
            bloke->dest.x = ((DAT_0081cb60->field_24 + tile.pos.x) << 8) - 0x180;
            bloke->dest.y = ((DAT_0081cb60->field_25 + tile.pos.y) << 8) + 0x80;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->field_e = 7;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 4:
            bloke->dest.x = ((DAT_0081cb60->field_24 + tile.pos.x) << 8) + 0x80;
            bloke->dest.y = ((DAT_0081cb60->field_25 + tile.pos.y) << 8) + 0x80;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->field_e = 7;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 5:
            bloke->flags &= 0xfff7;
            RemoveBlokeFromRide(DAT_0081cb60, node);
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x00435bd0
void FUN_00435bd0(int param_1, unsigned int param_2, unsigned int param_3, short *param_4, unsigned int param_5, unsigned int param_6) {
    int ride = *(int *)(param_1 + 0xc);
    unsigned int *node = *(unsigned int **)(ride + 0xcc);
    short *lls1;
    unsigned int lls2;
    struct Point coords;

    FUN_00432d00(1);
    for (; node != NULL; node = (unsigned int *)*node) {
        if (*param_4 == *(short *)(node + 3) && *(char *)(node[2] + 0x60) != '\x02') {
            IP_RenderBlokeIn3DNow((struct Bloke *)node[2]);
        }
    }
    coords = GetScreenCoordsForObject((unsigned char *)param_4, (void *)ride);
    lls1 = (short *)GetLLSForSprite((struct SpriteLLS *)DAT_0081cb60->layer);
    lls2 = GetLLSForSprite((struct SpriteLLS *)DAT_0081cb5c);
    LLSSetFrame((struct LLS *)lls2, *lls1);
    PrintSprite(DAT_0081cb5c, coords.x, coords.y, param_6, 0);
}

// FUNCTION: LEGOLAND 0x00435c70
int FUN_00435c70(void) {
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
    for (scoreCur = DAT_00629c3c; scoreCur != NULL; scoreCur = scoreCur->next) {
        count++;
    }
    score = DAT_00629c3c;
    SaveGameWrite(&count, 4);
    while (count--) {
        scoreCopy = *score;
        for (i = 0; i < 5; i++) {
            scoreCopy.blokes[i] = (struct Bloke *)GetBlokeNum(scoreCopy.blokes[i]);
        }
        SaveGameWrite(&scoreCopy, sizeof(scoreCopy));
        score = score->next;
    }
    count = 0;
    for (pathCur = DAT_0062fd2c; pathCur != NULL; pathCur = pathCur->next) {
        count++;
    }
    path = DAT_0062fd2c;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(path, sizeof(*path));
        path = path->next;
    }
    count = 0;
    for (fishCur = DAT_00629c30; fishCur != NULL; fishCur = fishCur->next) {
        count++;
    }
    fish = DAT_00629c30;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(fish, sizeof(*fish));
        fish = fish->next;
    }
    count = 0;
    for (thingCur = DAT_00629c2c; thingCur != NULL; thingCur = thingCur->next) {
        count++;
    }
    thing = DAT_00629c2c;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(thing, sizeof(*thing));
        thing = thing->next;
    }
    count = 0;
    for (rideCur = DAT_00616164; rideCur != NULL; rideCur = rideCur->next) {
        count++;
    }
    ride = DAT_00616164;
    SaveGameWrite(&count, 4);
    while (count--) {
        rideCopy = *ride;
        for (i = 0; i < 3; i++) {
            rideCopy.blokes[i] = GetBlokeNum((struct Bloke *)rideCopy.blokes[i]);
        }
        SaveGameWrite(&rideCopy, sizeof(rideCopy));
        ride = ride->next;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00435ec0
int FUN_00435ec0(void) {
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
            DAT_00629c3c = (struct JungleScore *)malloc(sizeof(struct JungleScore));
            prevScore = DAT_00629c3c;
        } else {
            score = (struct JungleScore *)malloc(sizeof(struct JungleScore));
            prevScore->next = score;
            prevScore = score;
        }
        SaveGameRead(prevScore, sizeof(struct JungleScore));
        for (i = 0; i < 5; i++) {
            prevScore->blokes[i] = GetBlokePtr((int)prevScore->blokes[i]);
        }
    }
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevPath == NULL) {
            DAT_0062fd2c = (struct JunglePath *)malloc(sizeof(struct JunglePath));
            prevPath = DAT_0062fd2c;
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
            DAT_00629c30 = (struct JungleFish *)malloc(sizeof(struct JungleFish));
            prevFish = DAT_00629c30;
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
            DAT_00629c2c = (struct JungleObj *)malloc(sizeof(struct JungleObj));
            prevThing = DAT_00629c2c;
        } else {
            thing = (struct JungleObj *)malloc(sizeof(struct JungleObj));
            prevThing->next = thing;
            prevThing = thing;
        }
        SaveGameRead(prevThing, sizeof(struct JungleObj));
    }
    prevRide = DAT_00616164;
    SaveGameRead(&count, 4);
    while (count--) {
        if (prevRide == NULL) {
            DAT_00616164 = (struct JungleRide *)malloc(sizeof(struct JungleRide));
            prevRide = DAT_00616164;
        } else {
            ride = (struct JungleRide *)malloc(sizeof(struct JungleRide));
            prevRide->next = ride;
            prevRide = ride;
        }
        SaveGameRead(prevRide, sizeof(struct JungleRide));
        for (i = 0; i < 3; i++) {
            prevRide->blokes[i] = (unsigned int)GetBlokePtr(prevRide->blokes[i]);
        }
    }
    for (score = DAT_00629c3c; score != NULL; score = score->next) {
        FUN_004373c0(score->field_0);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00436130
void FUN_00436130(unsigned short param_1, unsigned int param_2) {
    struct JungleScore *node = DAT_00629c3c;
    while (node != NULL) {
        if (node->field_0 == param_1) {
            if (node != NULL) {
                node->field_40 += param_2;
            }
            return;
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00436160
int FUN_00436160(struct RideObject *obj, int param_2) {
    struct JungleScore *node;
    int best = 0;

    for (node = DAT_00629c3c; node != NULL; node = node->next) {
        if ((int)node->field_40 > best && (param_2 == 0 || node->field_8 != 0)) {
            best = node->field_40;
        }
    }
    return best;
}

// FUNCTION: LEGOLAND 0x00436190
void FUN_00436190(struct RideObject *obj) {
    DAT_0081cb54 = obj->ride;
}

// FUNCTION: LEGOLAND 0x004361a0
void FUN_004361a0(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = DAT_0081cb54;
    DAT_0081cb54->footprint = DAT_004b7478;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint((char *)EditMode.unk8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00436200
void FUN_00436200(struct RideObject *obj, unsigned int param_2, unsigned int param_3) {
    unsigned int mask;
    unsigned short owner;
    int n;
    int x;
    int y;
    int result;
    struct Cursor *c;
    struct MapRect rect;

    n = 0;
    memcpy(EditCursor.field_1414, &DAT_004b7478, sizeof(DAT_004b7478));
    ScreenToMapRef(param_2, &EditCursor.field_1404, param_3);
    mask = FUN_00436fb0(EditCursor.field_1404, EditCursor.field_1408, &owner);
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
                    DefaultCursor(&DAT_00629c58[0]);
                    DefaultCursor(&DAT_00629c58[1]);
                    DefaultCursor(&DAT_00629c58[2]);
                    DefaultCursor(&DAT_00629c58[3]);
                    memcpy(DAT_00629c58[0].field_1414, EditCursor.field_1414, 20);
                    memcpy(DAT_00629c58[1].field_1414, EditCursor.field_1414, 20);
                    memcpy(DAT_00629c58[2].field_1414, EditCursor.field_1414, 20);
                    memcpy(DAT_00629c58[3].field_1414, EditCursor.field_1414, 20);
                    FUN_0045f460(&DAT_00629c58[0]);
                    FUN_0045f460(&DAT_00629c58[1]);
                    FUN_0045f460(&DAT_00629c58[2]);
                    FUN_0045f460(&DAT_00629c58[3]);
                    x = EditCursor.field_1404;
                    y = EditCursor.field_1408;
                    DAT_00629c58[0].field_1828 = 0x2034;
                    DAT_00629c58[1].field_1828 = 0x2034;
                    DAT_00629c58[2].field_1828 = 0x2034;
                    DAT_00629c58[3].field_1828 = 0x2034;
                    if ((mask & 1) != 0) {
                        DAT_00629c58[0].field_1404 = x;
                        DAT_00629c58[0].field_1408 = y - 5;
                        n = 1;
                    }
                    if ((mask & 2) != 0) {
                        DAT_00629c58[n].field_1404 = x + 5;
                        DAT_00629c58[n].field_1408 = y;
                        n++;
                    }
                    if ((mask & 4) != 0) {
                        DAT_00629c58[n].field_1404 = x;
                        DAT_00629c58[n].field_1408 = y + 5;
                        n++;
                    }
                    if ((mask & 8) != 0) {
                        DAT_00629c58[n].field_1404 = x - 5;
                        DAT_00629c58[n].field_1408 = y;
                        n++;
                    }
                    if (n != 0) {
                        EditCursor.field_1830 = (unsigned int)&DAT_00629c58[0];
                        if (n > 1) {
                            c = &DAT_00629c58[1];
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

// FUNCTION: LEGOLAND 0x00436470
void FUN_00436470(unsigned int param_1, int *coords) {
    struct JungleScore *score = DAT_00629c3c;
    struct JungleRide *ride;
    struct MapElement *elem;
    struct JunglePath *path;
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
            path = FUN_004371b0(coords[0], coords[1]);
            QueryObj.pos.x = path->owner.pos.x;
            coords[0] = QueryObj.pos.x;
            QueryObj.pos.y = path->owner.pos.y;
            coords[1] = QueryObj.pos.y;
            memcpy(&QueryClass->footprint, &DAT_00629c40, sizeof(DAT_00629c40));
            fake.ride = DAT_0081cb60;
            FUN_00435230((unsigned int)&fake, (struct Point *)coords);
            return;
        }
    }
    ride = DAT_00616164;
    memcpy(&QueryClass->footprint, &DAT_004b7478, sizeof(DAT_004b7478));
    BasicObjectDCalcCursor(param_1, (struct Point *)coords);
    for (; ride != NULL; ride = ride->next) {
        if ((tile.pos.x == ride->field_4 && tile.pos.y == ride->field_8) || (tile.pos.x == ride->field_c && tile.pos.y == ride->field_10)) {
            FUN_0045f480(&QueryCursor, 1);
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x004365f0
void FUN_004365f0(struct RideObject *obj, int *coords) {
    struct JungleScore *score = DAT_00629c3c;
    unsigned int mask;
    unsigned short owner;
    int x0;
    int y0;
    int x1;
    int y1;

    mask = FUN_00436fb0(coords[0], coords[1], &owner);
    FUN_00436dc0(coords[0], coords[1], mask, &owner);
    IncrementObjectCount(obj->ride);
    FUN_004367b0(coords[0], coords[1], &owner);
    if ((mask & 1) != 0) {
        FUN_00436dc0(coords[0], coords[1] - 5, FUN_00436fb0(coords[0], coords[1] - 5, &owner), NULL);
        FUN_004367b0(coords[0], coords[1] - 5, &owner);
    }
    if ((mask & 2) != 0) {
        FUN_00436dc0(coords[0] + 5, coords[1], FUN_00436fb0(coords[0] + 5, coords[1], &owner), NULL);
        FUN_004367b0(coords[0] + 5, coords[1], &owner);
    }
    if ((mask & 4) != 0) {
        FUN_00436dc0(coords[0], coords[1] + 5, FUN_00436fb0(coords[0], coords[1] + 5, &owner), NULL);
        FUN_004367b0(coords[0], coords[1] + 5, &owner);
    }
    if ((mask & 8) != 0) {
        FUN_00436dc0(coords[0] - 5, coords[1], FUN_00436fb0(coords[0] - 5, coords[1], &owner), NULL);
        FUN_004367b0(coords[0] - 5, coords[1], &owner);
    }
    for (; score != NULL; score = score->next) {
        if (score->field_0 == owner) {
            x0 = score->start.pos.x;
            y0 = score->start.pos.y;
            x1 = score->end.pos.x;
            y1 = score->end.pos.y;
            score->field_8 = FUN_004371e0(x0, y0, x1, y1);
            if (score->field_8 != 0) {
                FUN_004373c0(owner);
            }
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x004367b0
void FUN_004367b0(int param_1, int param_2, unsigned short *param_3) {
    struct JungleScore *score = DAT_00629c3c;
    struct JunglePath *path;
    int other;
    unsigned int mask;

    path = FUN_004371b0(param_1, param_2);
    for (; score != NULL; score = score->next) {
        if (score->field_0 == *param_3) {
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
            (other = (int)FUN_004371b0(param_1, (param_2 - 5)), (*(unsigned char *)(other + 4) & 8) != 0)) {
            SetMapTile(param_1 - 3, param_2 - 3, *DAT_0081cb58->tiles);
            SetMapTile(param_1 - 2, param_2 - 3, *DAT_0081cb58->tiles);
            SetMapTile(param_1 - 3, param_2 - 2, *DAT_0081cb58->tiles);
            SetMapTile(param_1 - 2, param_2 - 2, *DAT_0081cb58->tiles);
        }
        if ((mask & 8) != 0 && (mask & 4) != 0 &&
            (other = (int)FUN_004371b0(param_1, (param_2 + 5)), (*(unsigned char *)(other + 4) & 8) != 0)) {
            SetMapTile(param_1 - 3, param_2 + 3, *DAT_0081cb58->tiles);
            SetMapTile(param_1 - 2, param_2 + 3, *DAT_0081cb58->tiles);
            SetMapTile(param_1 - 3, param_2 + 2, *DAT_0081cb58->tiles);
            SetMapTile(param_1 - 2, param_2 + 2, *DAT_0081cb58->tiles);
        }
        if ((mask & 2) != 0 && (mask & 1) != 0 &&
            (other = (int)FUN_004371b0(param_1, (param_2 - 5)), (*(unsigned char *)(other + 4) & 2) != 0)) {
            SetMapTile(param_1 + 3, param_2 - 3, *DAT_0081cb58->tiles);
            SetMapTile(param_1 + 2, param_2 - 3, *DAT_0081cb58->tiles);
            SetMapTile(param_1 + 3, param_2 - 2, *DAT_0081cb58->tiles);
            SetMapTile(param_1 + 2, param_2 - 2, *DAT_0081cb58->tiles);
        }
        if ((mask & 2) != 0 && (mask & 4) != 0 &&
            (other = (int)FUN_004371b0(param_1, (param_2 + 5)), (*(unsigned char *)(other + 4) & 2) != 0)) {
            SetMapTile(param_1 + 3, param_2 + 3, *DAT_0081cb58->tiles);
            SetMapTile(param_1 + 2, param_2 + 3, *DAT_0081cb58->tiles);
            SetMapTile(param_1 + 3, param_2 + 2, *DAT_0081cb58->tiles);
            SetMapTile(param_1 + 2, param_2 + 2, *DAT_0081cb58->tiles);
        }
    }
}

// FUNCTION: LEGOLAND 0x00436a40
void FUN_00436a40(struct RideObject *obj, TileId tile, struct Cursor *cursor) {
    struct JungleScore *score = DAT_00629c3c;
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
    if (elem->field_0 != DAT_0081cb54->field_c4) {
        fake.ride = DAT_0081cb60;
        FUN_00435470(&fake, tile, cursor);
        return;
    }
    mask = FUN_00436fb0(cursor->field_1404, cursor->field_1408, &owner);
    FUN_00436f30(obj, tile, cursor);
    if ((mask & 1) != 0) {
        y = cursor->field_1408 - 5;
        x = cursor->field_1404;
        dir = FUN_00436fb0(x, y, &other);
        FUN_00436dc0(x, y, dir, &owner);
        FUN_004367b0(x, y, &owner);
    }
    if ((mask & 2) != 0) {
        x = cursor->field_1404 + 5;
        y = cursor->field_1408;
        dir = FUN_00436fb0(x, y, &other);
        FUN_00436dc0(x, y, dir, &owner);
        FUN_004367b0(x, y, &owner);
    }
    if ((mask & 4) != 0) {
        y = cursor->field_1408 + 5;
        x = cursor->field_1404;
        dir = FUN_00436fb0(x, y, &other);
        FUN_00436dc0(x, y, dir, &owner);
        FUN_004367b0(x, y, &owner);
    }
    if ((mask & 8) != 0) {
        x = cursor->field_1404 - 5;
        y = cursor->field_1408;
        dir = FUN_00436fb0(x, y, &other);
        FUN_00436dc0(x, y, dir, &owner);
        FUN_004367b0(x, y, &owner);
    }
    if ((mask & 1) != 0 && (mask & 8) != 0) {
        x = cursor->field_1404 - 5;
        y = cursor->field_1408 - 5;
        if (FUN_004371b0(x, y) != NULL) {
            dir = FUN_00436fb0(x, y, &other);
            FUN_00436dc0(x, y, dir, &owner);
            FUN_004367b0(x, y, &owner);
        }
    }
    if ((mask & 1) != 0 && (mask & 2) != 0) {
        x = cursor->field_1404 + 5;
        y = cursor->field_1408 - 5;
        if (FUN_004371b0(x, y) != NULL) {
            dir = FUN_00436fb0(x, y, &other);
            FUN_00436dc0(x, y, dir, &owner);
            FUN_004367b0(x, y, &owner);
        }
    }
    if ((mask & 4) != 0 && (mask & 8) != 0) {
        x = cursor->field_1404 - 5;
        y = cursor->field_1408 + 5;
        if (FUN_004371b0(x, y) != NULL) {
            dir = FUN_00436fb0(x, y, &other);
            FUN_00436dc0(x, y, dir, &owner);
            FUN_004367b0(x, y, &owner);
        }
    }
    if ((mask & 4) != 0 && (mask & 2) != 0) {
        x = cursor->field_1404 + 5;
        y = cursor->field_1408 + 5;
        if (FUN_004371b0(x, y) != NULL) {
            dir = FUN_00436fb0(x, y, &other);
            FUN_00436dc0(x, y, dir, &owner);
            FUN_004367b0(x, y, &owner);
        }
    }
    FUN_004373c0(owner);
    for (; score != NULL; score = score->next) {
        if (score->field_0 == owner) {
            x0 = score->start.pos.x;
            y0 = score->start.pos.y;
            x1 = score->end.pos.x;
            y1 = score->end.pos.y;
            score->field_8 = FUN_004371e0(x0, y0, x1, y1);
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x00436dc0
void FUN_00436dc0(int x, int y, int mask, unsigned short *owner) {
    TileId tile;
    struct JunglePath *node;
    struct MapElement *elem;
    int row;
    int col;
    struct Point pt;

    tile.pos.x = x;
    tile.pos.y = y;
    node = FUN_004371b0(x, y);
    if (node == NULL) {
        node = (struct JunglePath *)malloc(sizeof(struct JunglePath));
        if (node == NULL) {
            return;
        }
        node->next = DAT_0062fd2c;
        node->field_18 = NULL;
        DAT_0062fd2c = node;
        FUN_00436130(*owner, 1);
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
            elem->field_0 = DAT_0081cb54->field_c4;
            *(unsigned short *)&elem->field_4 = tile.id;
            SetMapTile(pt.x, pt.y, *DAT_0081cb58[DAT_004b72e4[mask * 25 + row * 5 + col] >> 8].tiles + (unsigned char)DAT_004b72e4[mask * 25 + row * 5 + col]);
        }
    }
}

// FUNCTION: LEGOLAND 0x00436f30
void FUN_00436f30(void *param_1, TileId tile, struct Cursor *param_3) {
    struct JunglePath *node = DAT_0062fd2c;
    struct JunglePath *prev = NULL;

    StandardRemoveObject((struct EditObject *)param_1, tile, param_3);
    while (node->tile.id != tile.id) {
        prev = node;
        node = node->next;
        if (node == NULL) {
            return;
        }
    }
    if (node != NULL) {
        FUN_00436130(node->owner.id, -1);
        if (prev != NULL) {
            prev->next = node->next;
            free(node);
            return;
        }
        DAT_0062fd2c = node->next;
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x00436fb0
unsigned int FUN_00436fb0(int x, int y, unsigned short *owner) {
    struct JungleScore *score;
    struct JunglePath *node;
    unsigned int mask;
    int valid;
    int n;
    TileId key;

    mask = 0;
    valid = 0;
    score = DAT_00629c3c;
    node = FUN_004371b0(x, y);
    if (node != NULL) {
        *owner = node->owner.id;
        valid = 1;
    }
    n = y - 5;
    if (x >= 0 && n >= 0 && x < lpConfig->width && n < lpConfig->height && (node = FUN_004371b0(x, n)) != NULL) {
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
    if (n >= 0 && y >= 0 && n < lpConfig->width && y < lpConfig->height && (node = FUN_004371b0(n, y)) != NULL) {
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
    if (x >= 0 && n >= 0 && x < lpConfig->width && n < lpConfig->height && (node = FUN_004371b0(x, n)) != NULL) {
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
    if (n >= 0 && y >= 0 && n < lpConfig->width && y < lpConfig->height && (node = FUN_004371b0(n, y)) != NULL) {
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
struct JunglePath *FUN_004371b0(int x, int y) {
    struct JunglePath *node;
    TileId key;

    key.pos.x = x;
    key.pos.y = y;
    for (node = DAT_0062fd2c; node != NULL; node = node->next) {
        if (node->tile.id == key.id) {
            break;
        }
    }
    return node;
}

// FUNCTION: LEGOLAND 0x004371e0
int FUN_004371e0(int a, int b, int c, int d) {
    struct JunglePath *node;
    TileId key;
    int result;

    result = 0;
    for (node = DAT_0062fd2c; node != NULL; node = node->next) {
        node->field_c = 0;
    }
    node = FUN_004371b0(a, b);
    if (node == NULL) {
        return 0;
    }
    key.id = node->owner.id;
    FUN_00437260(a, b, c, d, &key, &result);
    return result;
}

// FUNCTION: LEGOLAND 0x00437260
void FUN_00437260(int x, int y, int tx, int ty, TileId *owner, int *found) {
    struct JunglePath *node;
    struct JunglePath *next;

    if (*found == 1) {
        return;
    }
    node = FUN_004371b0(x, y);
    if (node == NULL || node->owner.id != owner->id) {
        return;
    }
    if (x == tx && y == ty) {
        *found = 1;
        return;
    }
    node->field_c = 1;
    if ((node->field_4 & 1) != 0 && (next = FUN_004371b0(x, y - 5)) != NULL && next->field_c == 0) {
        FUN_00437260(x, y - 5, tx, ty, owner, found);
    }
    if ((node->field_4 & 2) != 0 && (next = FUN_004371b0(x + 5, y)) != NULL && next->field_c == 0) {
        FUN_00437260(x + 5, y, tx, ty, owner, found);
    }
    if ((node->field_4 & 4) != 0 && (next = FUN_004371b0(x, y + 5)) != NULL && next->field_c == 0) {
        FUN_00437260(x, y + 5, tx, ty, owner, found);
    }
    if ((node->field_4 & 8) != 0 && (next = FUN_004371b0(x - 5, y)) != NULL && next->field_c == 0) {
        FUN_00437260(x - 5, y, tx, ty, owner, found);
    }
}

// FUNCTION: LEGOLAND 0x004373c0
void FUN_004373c0(unsigned short param_1) {
    struct JungleScore *score = DAT_00629c3c;
    struct JunglePath *node;
    struct JunglePath *tmp;

    for (node = DAT_0062fd2c; node != NULL; node = node->next) {
        if (node->owner.id == param_1) {
            node->field_18 = NULL;
        }
    }
    while (score != NULL && score->field_0 != param_1) {
        score = score->next;
    }
    node = FUN_004371b0(score->end.pos.x, score->end.pos.y);
    node->field_8 = 0;
    node->field_14 = NULL;
    DAT_0062fd30 = node;
    DAT_0062fd34 = NULL;
    do {
        FUN_00437440(param_1);
        tmp = DAT_0062fd34;
        DAT_0062fd30 = tmp;
        DAT_0062fd34 = NULL;
    } while (tmp != NULL);
}

// FUNCTION: LEGOLAND 0x00437440
void FUN_00437440(short param_1) {
    struct JunglePath *p;
    struct JunglePath *n1;
    struct JunglePath *n2;
    struct JunglePath *n3;
    struct JunglePath *n4;

    for (p = DAT_0062fd30; p != NULL; p = p->field_14) {
        n1 = FUN_004371b0(p->tile.pos.x, p->tile.pos.y - 5);
        n2 = FUN_004371b0(p->tile.pos.x + 5, p->tile.pos.y);
        n3 = FUN_004371b0(p->tile.pos.x, p->tile.pos.y + 5);
        n4 = FUN_004371b0(p->tile.pos.x - 5, p->tile.pos.y);
        if (n1 != NULL && (short)n1->owner.id == param_1 && n1->field_18 == NULL) {
            n1->field_18 = p;
            n1->field_8 = p->field_8 + 1;
            n1->field_14 = DAT_0062fd34;
            DAT_0062fd34 = n1;
        }
        if (n2 != NULL && (short)n2->owner.id == param_1 && n2->field_18 == NULL) {
            n2->field_18 = p;
            n2->field_8 = p->field_8 + 1;
            n2->field_14 = DAT_0062fd34;
            DAT_0062fd34 = n2;
        }
        if (n3 != NULL && (short)n3->owner.id == param_1 && n3->field_18 == NULL) {
            n3->field_18 = p;
            n3->field_8 = p->field_8 + 1;
            n3->field_14 = DAT_0062fd34;
            DAT_0062fd34 = n3;
        }
        if (n4 != NULL && (short)n4->owner.id == param_1 && n4->field_18 == NULL) {
            n4->field_18 = p;
            n4->field_8 = p->field_8 + 1;
            n4->field_14 = DAT_0062fd34;
            DAT_0062fd34 = n4;
        }
    }
}

// FUNCTION: LEGOLAND 0x00437570
void FUN_00437570(struct RideNode *node, struct RideObject *obj, TileId *tile, int sfx) {
    struct Bloke *bloke = node->rider;

    if (bloke->field_58 == 0) {
        bloke->param_action++;
    }
    if ((bloke->field_58-- & 0x1f) == 0 && rand() % 100 <= 30) {
        BuyItem((struct BuyItemArg *)obj, tile, sfx);
        node->rider->param_action++;
    }
}
