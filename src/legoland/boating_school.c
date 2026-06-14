#include <stdlib.h>
#include <math.h>
#include "globals.h"
#include "legoland.h"

#include <string.h>

#include "boating_school.h"
#include "debug_alloc.h"
#include "gamemap.h"
#include "image_sprite.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "obj_instance.h"
#include "objclass.h"
#include "print_sprite.h"
#include "render3d.h"
#include "ride_interfaces.h"
#include "sound_music.h"
#include "tilemap.h"

// FUNCTION: LEGOLAND 0x00418e60
int FUN_00418e60(unsigned int param_1, unsigned int param_2) {
    short id = (short)param_1;
    struct RideNode *score;
    struct BoatRide *node;
    struct BoatRide *fresh;
    short *fill;
    int i;
    unsigned int r;

    for (score = DAT_004cc074; node = DAT_004cc03c, score != NULL && score->id != (unsigned short)id;
         score = score->next) {
    }
    while (node != NULL) {
        if (node->id == (unsigned short)id) {
            if (node->field_4 == score->field_2 && node->field_8 == score->field_3) {
                return 0;
            }
            if (node->field_c == score->field_2 && node->field_10 == score->field_3) {
                return 0;
            }
            if (node->field_3e4 == 1) {
                return 0;
            }
        }
        node = node->next;
    }
    fresh = (struct BoatRide *)malloc(sizeof(struct BoatRide));
    if (fresh == NULL) {
        return 0;
    }
    fresh->id = id;
    fresh->next = DAT_004cc03c;
    i = (param_1 >> 8 & 0xff) + 5;
    fresh->field_4 = (param_1 & 0xff) - 1;
    fresh->field_8 = i;
    fresh->field_c = (param_1 & 0xff) - 1;
    fresh->field_10 = i;
    fresh->field_3dc = 1;
    fresh->field_3e4 = 1;
    r = rand();
    fresh->field_3e0 = r & 3;
    r = rand();
    fresh->field_3ec = param_2;
    fresh->field_3e8 = (r & 0xf) + 4;
    fill = (short *)fresh->field_1c;
    DAT_004cc03c = fresh;
    for (i = 0xa0; i != 0; i = i - 1) {
        fill[0] = -0xe0f;
        fill[1] = -0xe0f;
        fill = fill + 2;
    }
    fill = (short *)fresh->field_29c;
    for (i = 0x50; i != 0; i = i - 1) {
        fill[0] = 0;
        fill[1] = 0;
        fill = fill + 2;
    }
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
    if (node == NULL) {
        return;
    }
    if (prev != NULL) {
        prev->next = node->next;
        free(param_1);
        return;
    }
    DAT_004cc03c = node->next;
    free(param_1);
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
    int baseX;
    int baseY;
    int local_18;
    int local_14;
    int person;

    GetTileDimensions(&tw, &th);
    while (ride != NULL) {
        int mode = ride->field_3e4;
        if (param_1 == 0) {
            if (mode != 1 && mode != 0x10) {
                goto next;
            }
        } else if (mode != 1 && mode != 0x10) {
            goto next;
        }
        dy = ride->field_1c[DAT_004cc08c * 2 + 1];
        dx = ride->field_1c[DAT_004cc08c * 2];
        GetTileDimensions(&tw2, &th2);
        baseX = (dx - dy) * tw2 >> 9;
        baseY = (dx + dy) * th2 >> 9;
        local_18 = ((ride->field_4 - ride->field_8) * (tw >> 1) - ((tw + 1) >> 1)) - (ScrollX >> 8);
        local_14 = (ride->field_4 + ride->field_8) * (th >> 1) - (ScrollY >> 8);
        dx = *(int *)(*(int *)((char *)DAT_0082c65c + 0xc) + (ride->field_29c[DAT_004cc08c] & 0xff) * 4) >> 1;
        dy = *(int *)(*(int *)((char *)DAT_0082c65c + 0x10) + (ride->field_29c[DAT_004cc08c] & 0xff) * 4) >> 1;
        AdjustOffsetForViewMode((struct AdjustStruct *)&dx);
        ride->field_14 = (unsigned int)lpConfig->field_20 + baseX + dx + local_18;
        ride->field_18 = (unsigned int)lpConfig->field_22 + baseY + dy + local_14;
        PrintSprite(*(struct Sprite **)(*(int *)((char *)DAT_0082c65c + 8) + (ride->field_29c[DAT_004cc08c] & 0xff) * 4),
                    ride->field_14, ride->field_18, 0, 0);
        if (ride->field_3ec != 0) {
            person = Find3DPersonFromBloke(ride->field_3ec);
            *(float *)(person + 0x44) = ((float)(int)ride->field_29c[DAT_004cc08c] * DAT_004ab3e8 + DAT_004ab3e4) * DAT_004ab3e0 * DAT_004ab3dc;
            SetPersonRotation(person, person + 0x40);
            tw2 = (unsigned int)lpConfig->field_20 + baseX + local_18;
            th2 = (unsigned int)lpConfig->field_22 + baseY + local_14;
            AdjustBlokePosition((struct BlokePos *)&tw2);
            local_18 = DAT_004b51d8[(ride->field_29c[DAT_004cc08c] & 0xf) * 8] + 0x44;
            local_14 = DAT_004b51d8[(ride->field_29c[DAT_004cc08c] & 0xf) * 8 + 1] + 0x34;
            AdjustOffsetForViewMode((struct AdjustStruct *)&local_18);
            *(int *)(person + 0x1c) = local_18 + tw2;
            *(int *)(person + 0x20) = local_14 + th2;
            IP_RenderBlokeIn3DNow((struct Bloke *)ride->field_3ec);
            PrintSprite(*(struct Sprite **)(*(int *)((char *)DAT_0082c65c + 8) + ((ride->field_29c[DAT_004cc08c] + 0x30) & 0xff) * 4),
                        ride->field_14, ride->field_18, 0, 0);
        }
    next:
        if (ride->field_3e4 == 0x10 && ride->field_3e8 == 2 && DAT_004cc08c == 0x4f && param_1 != 0 && ride->field_3ec != 0) {
            *(char *)(ride->field_3ec + 0x60) += 1;
            ride->field_3ec = 0;
        }
        ride = ride->next;
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
    while (node != NULL) {
        struct BoatRide *cur = node;
        node->field_4 = node->field_c;
        node->field_8 = node->field_10;
        if (node->field_3e4 == 0x10) {
            cur = FUN_00419420(node);
            if (cur != node) {
                node = cur;
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
            }
        }
        if (cur == NULL) {
            return;
        }
        node = cur->next;
    }
}

// FUNCTION: LEGOLAND 0x004193c0
void FUN_004193c0(struct BoatRide *param_1) {
    struct RideNode *node = DAT_004cc074;
    FUN_004198a0(param_1, param_1->field_3dc, 4);
    param_1->field_3e4 = 4;
    param_1->field_10 = param_1->field_8 + 5;
    if (node != NULL) {
        while (node->id != param_1->id) {
            node = node->next;
            if (node == NULL) {
                return;
            }
        }
        node->field_c = 0;
        node->field_10 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00419420
struct BoatRide *FUN_00419420(struct BoatRide *param_1) {
    int *p;
    int i;
    struct BoatRide *result;
    struct SampleParams params;

    if (param_1->field_3e8 != 0) {
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
    params.field_8 = param_1->field_3ec;
    result = param_1->next;
    params.field_0 = 1;
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffffa6);
    FUN_00418f90(param_1);
    return result;
}

// FUNCTION: LEGOLAND 0x00419520
void FUN_00419520(struct BoatRide *param_1, int param_2) {
    struct RideNode *score = DAT_004cc074;
    struct BoatRide *other = DAT_004cc03c;
    struct PathNode *path;
    unsigned int mask;
    unsigned int rnd;
    int bit;
    int count;
    int dir;
    unsigned int sel;
    unsigned char d;
    char step;

    path = FUN_0041c890(param_1->field_4, param_1->field_8);
    if (score != NULL) {
        do {
            if (path->field_2 == score->id) {
                break;
            }
            score = score->next;
        } while (score != NULL);
    }
    mask = path->field_4;
    if (*(short *)&path->x == *(short *)&score->field_2) {
        mask = mask & 0xfffffffe;
    } else if (*(short *)&path->x == *(short *)&score->field_4) {
        param_1->field_3e4 = 0x10;
        param_1->field_3e8 = 3;
        FUN_004198a0(param_1, param_1->field_3dc, 4);
        param_1->field_3dc = 1;
        param_1->field_10 = param_1->field_8 + 5;
        return;
    }
    for (; other != NULL; other = other->next) {
        if (other != param_1) {
            if ((param_1->field_4 == other->field_4 && param_1->field_8 - 5 == other->field_8) ||
                (param_1->field_4 == other->field_c && param_1->field_8 - 5 == other->field_10)) {
                mask = mask & 0xfffffffe;
            }
            if ((param_1->field_4 + 5 == other->field_4 && param_1->field_8 == other->field_8) ||
                (param_1->field_4 + 5 == other->field_c && param_1->field_8 == other->field_10)) {
                mask = mask & 0xfffffffd;
            }
            if ((param_1->field_4 == other->field_4 && param_1->field_8 + 5 == other->field_8) ||
                (param_1->field_4 == other->field_c && param_1->field_8 + 5 == other->field_10)) {
                mask = mask & 0xfffffffb;
            }
            if ((param_1->field_4 - 5 == other->field_4 && param_1->field_8 == other->field_8) ||
                (param_1->field_4 - 5 == other->field_c && param_1->field_8 == other->field_10)) {
                mask = mask & 0xfffffff7;
            }
        }
    }
    if (param_2 != 0 && path->field_18 != NULL) {
        if (((unsigned char *)path->field_18)[1] == path->y) {
            mask = mask & 0xfffffffb;
        } else if ((int)((unsigned int)((unsigned char *)path->field_18)[0] - (unsigned int)path->x) < 0) {
            mask = mask & 0xfffffff7;
        } else {
            mask = mask & 0xfffffffd;
        }
    }
    if (mask == 0) {
        FUN_004198a0(param_1, param_1->field_3dc, 0xffffffff);
        param_1->field_3dc = 0xffffffff;
        return;
    }
    rnd = rand();
    if ((rnd & 7) == 0 && (rnd = ~param_1->field_3dc & mask) != 0) {
        while (1) {
            mask = rnd;
            bit = 0;
            count = 0;
            do {
                if ((mask & (1 << bit)) != 0) {
                    count = count + 1;
                }
                bit = bit + 1;
            } while (bit < 4);
            if (count < 2) {
                break;
            }
            d = rand();
            rnd = mask & ~(1 << (d & 3));
        }
    }
    bit = 0;
    do {
        if ((param_1->field_3dc & (1 << bit)) != 0) {
            break;
        }
        bit = bit + 1;
    } while (bit < 4);
    sel = (bit + 2) & 0x80000003;
    if ((int)sel < 0) {
        sel = ((sel - 1) | 0xfffffffc) + 1;
    }
    dir = 1 << ((unsigned char)sel & 0x1f);
    if ((mask & dir) == 0) {
        d = rand();
        step = (-((d & 1) != 0) & 2) - 1;
        dir = 1 << ((step + (char)sel) & 3);
        if ((mask & dir) == 0) {
            dir = 1 << (((char)sel - step) & 3);
            if ((mask & dir) == 0) {
                sel = (sel + 2) & 0x80000003;
                if ((int)sel < 0) {
                    sel = ((sel - 1) | 0xfffffffc) + 1;
                }
                dir = 1 << ((unsigned char)sel & 0x1f);
            }
        }
    }
    switch (dir) {
    case 1:
        param_1->field_c = param_1->field_4;
        param_1->field_10 = param_1->field_8 - 5;
        FUN_004198a0(param_1, param_1->field_3dc, dir);
        param_1->field_3dc = 4;
        break;
    case 2:
        param_1->field_c = param_1->field_4 + 5;
        param_1->field_10 = param_1->field_8;
        FUN_004198a0(param_1, param_1->field_3dc, dir);
        param_1->field_3dc = 8;
        break;
    case 4:
        param_1->field_c = param_1->field_4;
        param_1->field_10 = param_1->field_8 + 5;
        FUN_004198a0(param_1, param_1->field_3dc, dir);
        param_1->field_3dc = 1;
        break;
    case 8:
        param_1->field_c = param_1->field_4 - 5;
        param_1->field_10 = param_1->field_8;
        FUN_004198a0(param_1, param_1->field_3dc, dir);
        param_1->field_3dc = 2;
        break;
    }
    bit = param_1->field_3e8 - 1;
    param_1->field_3e8 = bit;
    if (bit == 0) {
        param_1->field_3e4 = 8;
    }
}

// FUNCTION: LEGOLAND 0x004198a0
void FUN_004198a0(struct BoatRide *param_1, unsigned int param_2, unsigned int param_3) {
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
            FUN_00458930((float)0);
            FUN_00458930((float)0);
            i = 0;
            p = &param_1->field_1c[1];
            do {
                if (i < 0x28) {
                    p[-1] = FUN_00458930((float)0);
                    p[0] = FUN_00458930((float)0);
                } else {
                    p[-1] = 0;
                    p[0] = 0;
                }
                i = i + 1;
                p = p + 2;
            } while (i < 0x50);
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
                p[0] = DAT_004b5118[idx * 4] * 0x10 + p[-2];
                p[1] = DAT_004b5118[idx * 4 + 1] * 0x10 + p[-1];
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
                table = DAT_004b5158;
            } else if ((param_2 & param_3 * 2) != 0) {
                table = DAT_004b5198;
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
    p = &param_1->field_4;
    do {
        if (i < 0x4c) {
            tx = p[0xe];
            ty = p[0xf];
        } else {
            tx = param_1->field_1c[0x9e];
            ty = param_1->field_1c[0x9f];
        }
        if (i < 4) {
            cx = param_1->field_1c[0];
            cy = param_1->field_1c[1];
        } else {
            cx = p[0];
            cy = p[1];
        }
        dx = tx - cx;
        dy = ty - cy;
        angle = ArcTan256(dx, dy);
        param_1->field_29c[i] = ((angle >> 4) + 6 & 0xf) + param_1->field_3e0 * 0x10;
        i = i + 1;
        p = p + 2;
    } while (i < 0x50);
}

// FUNCTION: LEGOLAND 0x00419d10
void FUN_00419d10(struct BoatHolder *param_1) {
    void *handle;
    unsigned int i;
    int sprite;
    int lls;
    int *src;
    int *dst;
    int absorbed;

    Load_FXList(PTR_s_Boat_Noise_wav, 2);
    DAT_0082c658 = param_1->cursor;
    DAT_0082c658->field_1c |= 0x20;
    *(unsigned int *)((char *)DAT_0082c658->field_64 + 0x10) |= 0x2000;
    // STRING: LEGOLAND 0x004b5334
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", (unsigned int *)&handle, 0) == 0) {
        DAT_0082adf4 = (struct BoatTileMap *)LLIDB_LoadData(handle);
    }
    // STRING: LEGOLAND 0x004b531c
    if (LLIDB_FindElement("BOATING SCHOOL BOATS", (unsigned int *)&handle, 0) == 0) {
        DAT_0082c65c = LLIDB_LoadData(handle);
    }
    i = 0;
    if (0 < *(int *)((char *)DAT_0082c65c + 4)) {
        do {
            sprite = *(int *)(*(int *)((char *)DAT_0082c65c + 8) + (i & 0xff) * 4);
            lls = GetLLSForSprite((struct SpriteLLS *)sprite);
            LLSPlay((struct LLS *)lls, *(unsigned int *)(sprite + 8));
            i = i + 1;
        } while ((int)i < *(int *)((char *)DAT_0082c65c + 4));
    }
    // STRING: LEGOLAND 0x004b530c
    DAT_0082adfc = LoadSprite("bs_hullmask.lls", 1);
    // STRING: LEGOLAND 0x004b52fc
    DAT_0082c654 = LoadSprite("bs_railm.lls", 1);
    DAT_0082ae00 = GetSpriteForLayer((void *)DAT_0082c658->field_64, 5);
    lls = GetLLSForSprite((struct SpriteLLS *)DAT_0082ae00);
    LLSStop((struct LLS *)lls);
    LLSSetFrame((struct LLS *)lls, *(short *)(lls + 0x10));
    src = (int *)&DAT_0082c658->field_3c;
    dst = DAT_004cc078;
    for (i = 5; i != 0; i = i - 1) {
        *dst = *src;
        src = src + 1;
        dst = dst + 1;
    }
    src = DAT_004b5260;
    dst = DAT_004cc048;
    for (i = 5; absorbed = DAT_004cc078[0], i != 0; i = i - 1) {
        *dst = *src;
        src = src + 1;
        dst = dst + 1;
    }
    DAT_004cc048[1] = DAT_004b5260[1] + DAT_004cc078[1];
    DAT_004cc048[3] += DAT_004cc078[1];
    DAT_004cc048[0] += DAT_004cc078[0];
    DAT_004cc048[2] += DAT_004cc078[0];
    src = DAT_004b5278;
    dst = DAT_004cc060;
    for (i = 5; i != 0; i = i - 1) {
        *dst = *src;
        src = src + 1;
        dst = dst + 1;
    }
    DAT_004cc060[0] += absorbed;
    DAT_004cc060[1] = DAT_004b5278[1] + 1 + DAT_004cc078[3];
    DAT_004cc060[2] += absorbed;
    DAT_004cc060[3] += 1 + DAT_004cc078[3];
}

// FUNCTION: LEGOLAND 0x00419ef0
void FUN_00419ef0(void) {
    void *handle;
    unsigned int i;
    struct BoatRide *ride;
    struct PathNode *path;

    Kill_FXList(PTR_s_Boat_Noise_wav, 2);
    i = 0;
    if (0 < *(int *)((char *)DAT_0082c65c + 4)) {
        do {
            LLSStop(GetLLSForSprite(*(struct SpriteLLS **)(*(int *)((char *)DAT_0082c65c + 8) + (i & 0xff) * 4)));
            i = i + 1;
        } while ((int)i < *(int *)((char *)DAT_0082c65c + 4));
    }
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", (unsigned int *)&handle, 0) == 0) {
        LLIDB_UnLoadData((unsigned int)handle);
    }
    if (LLIDB_FindElement("BOATING SCHOOL BOATS", (unsigned int *)&handle, 0) == 0) {
        LLIDB_UnLoadData((unsigned int)handle);
    }
    while (DAT_004cc074 != NULL) {
        struct RideNode *next = DAT_004cc074->next;
        free(DAT_004cc074);
        DAT_004cc074 = next;
    }
    DAT_004cc074 = NULL;
    while (DAT_004cc03c != NULL) {
        FUN_00418f90(DAT_004cc03c);
    }
    DAT_004cc03c = NULL;
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
    EditMode = 1;
    DAT_008119b8 = DAT_0082c658;
    DefaultCursor(&EditCursor);
    DAT_004cc088 = DAT_004cc060;
    DAT_004cc070 = DAT_004cc048;
    SetEditCursorFootPrint(DAT_004cc078);
}

// FUNCTION: LEGOLAND 0x0041a040
void FUN_0041a040(unsigned int param_1, int *param_2) {
    struct RideNode *score;
    unsigned char temp[2];
    int x;
    int y;
    short tile;

    temp[0] = (unsigned char)param_2[0];
    temp[1] = (unsigned char)param_2[1];
    score = (struct RideNode *)malloc(sizeof(struct RideNode));
    if (score == NULL) {
        return;
    }
    score->id = *(unsigned short *)temp;
    score->field_2 = (unsigned char)param_2[0] + (char)DAT_004cc060[0] + 2;
    score->field_3 = (unsigned char)param_2[1] + (char)DAT_004cc060[1] + 2;
    score->field_4 = (unsigned char)param_2[0] + (char)DAT_004cc048[0] + 2;
    score->field_8 = 0;
    score->field_c = 9999;
    score->field_10 = 0;
    score->field_5 = (unsigned char)param_2[1] + (char)DAT_004cc048[1] + 2;
    score->field_14 = 0;
    score->value = 5;
    score->blokes[0] = 0;
    score->blokes[1] = 0;
    score->blokes[2] = 0;
    score->blokes[3] = 0;
    score->blokes[4] = 0;
    score->next = DAT_004cc074;
    DAT_004cc074 = score;
    AddBasicObject(param_1, (unsigned int)param_2);
    FUN_0041c4c0(param_2[0] + 2 + DAT_004cc060[0], param_2[1] + 2 + DAT_004cc060[1], 1, &score->id);
    FUN_0041c4c0(param_2[0] + 2 + DAT_004cc048[0], param_2[1] + 2 + DAT_004cc048[1], 4, &score->id);
    y = DAT_004cc078[1];
    if (DAT_004cc078[1] <= DAT_004cc078[3]) {
        do {
            x = DAT_004cc078[0];
            if (DAT_004cc078[0] <= DAT_004cc078[2]) {
                do {
                    if (x == DAT_004cc078[0]) {
                        tile = *DAT_0082adf4->tiles + 9;
                    } else if (x == DAT_004cc078[2]) {
                        tile = *DAT_0082adf4->tiles + 0xc;
                    } else {
                        tile = *DAT_0082adf4->tiles;
                    }
                    SetMapTile(param_2[0] + x, param_2[1] + y, tile);
                    x = x + 1;
                } while (x <= DAT_004cc078[2]);
            }
            y = y + 1;
        } while (y <= DAT_004cc078[3]);
    }
    SetMapTile(param_2[0] + DAT_004cc078[2], param_2[1] + DAT_004cc078[1], *DAT_0082adf4->tiles + 8);
    SetMapTile(param_2[0] + DAT_004cc078[2], param_2[1] + DAT_004cc078[3], *DAT_0082adf4->tiles + 7);
    SetMapTile(param_2[0] + 4 + DAT_004cc078[0], param_2[1] + DAT_004cc078[3], *DAT_0082adf4->tiles + 4);
    SetMapTile(param_2[0] + 4 + DAT_004cc078[0], param_2[1] + DAT_004cc078[1], *DAT_0082adf4->tiles + 1);
    SetMapTile(param_2[0] + 5 + DAT_004cc078[0], param_2[1] + DAT_004cc078[3], *DAT_0082adf4->tiles + 0xb);
    SetMapTile(param_2[0] + 5 + DAT_004cc078[0], param_2[1] + DAT_004cc078[1], *DAT_0082adf4->tiles + 10);
}

// FUNCTION: LEGOLAND 0x0041a2f0
void FUN_0041a2f0(int param_1, unsigned int param_2, unsigned int param_3) {
    struct Cursor *cursor = *(struct Cursor **)(param_1 + 0xc);

    DAT_004cc088 = DAT_004cc060;
    *(struct Footprint *)EditCursor.field_1414 = *(struct Footprint *)DAT_004cc078;
    DAT_004cc070 = DAT_004cc048;
    DAT_004cc048[4] = 0;
    EditCursor.field_1830 = 0;
    ScreenToMapRef(param_2, &EditCursor.field_1404, param_3);
    PathCursor.field_1404 = EditCursor.field_1404;
    PathCursor.field_1408 = EditCursor.field_1408;
    PathCursor.field_1414[0] = EditCursor.field_1414[2] + 1;
    PathCursor.field_1414[1] = EditCursor.field_1414[1];
    PathCursor.field_1414[3] = EditCursor.field_1414[3];
    PathCursor.field_1828 = 0x1008;
    PathCursor.field_1830 = 0;
    EditCursor.field_1830 = (unsigned int)&PathCursor;
    PathCursor.field_1414[2] = PathCursor.field_1414[0];
    FUN_0045f460(&EditCursor);
    FUN_0045f460(&PathCursor);
    ValidateCursor(&EditCursor, (unsigned int)cursor);
}

// FUNCTION: LEGOLAND 0x0041a3d0
void FUN_0041a3d0(void *param_1, unsigned int param_2) {
    struct PathNode *path = DAT_004d823c;
    struct MermaidNode *node = DAT_004d2164;

    BasicObjectDCalcCursor((unsigned int)param_1, param_2);
    PathCursor.field_1408 = QueryCursor.field_1408;
    PathCursor.field_1414[0] = QueryCursor.field_1414[2] + 1;
    PathCursor.field_1404 = QueryCursor.field_1404;
    PathCursor.field_1414[3] = QueryCursor.field_1414[3];
    PathCursor.field_1414[1] = QueryCursor.field_1414[1];
    PathCursor.field_1414[4] = 0;
    PathCursor.field_1828 = 0x1008;
    PathCursor.field_1830 = 0;
    QueryCursor.field_1830 = (unsigned int)&PathCursor;
    DAT_00810144 = 1;
    PathCursor.field_1414[2] = PathCursor.field_1414[0];
    DefaultCursor(&DAT_0082ae20);
    *(struct Footprint *)DAT_0082ae20.field_1414 = *(struct Footprint *)DAT_004b53c0;
    for (; path != NULL; path = path->next) {
        if (path->field_2 == QueryObj) {
            DAT_0082ae20.field_1404 = path->x;
            DAT_0082ae20.field_1408 = path->y;
            FUN_0045f460(&DAT_0082ae20);
            DAT_0082ae20.field_1828 = 8;
            BuildCursorPtr(&DAT_0082ae20, 0, 0);
            RenderCursor(&DAT_0082ae20);
        }
    }
    for (; node != NULL; node = node->next) {
        if (node->field_2 == QueryObj) {
            DAT_0082ae20.field_1404 = (unsigned char)node->field_0;
            DAT_0082ae20.field_1408 = (unsigned char)(node->field_0 >> 8);
            FUN_0045f460(&DAT_0082ae20);
            DAT_0082ae20.field_1828 = 8;
            BuildCursorPtr(&DAT_0082ae20, 0, 0);
            RenderCursor(&DAT_0082ae20);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041a530
void FUN_0041a530(int param_1, unsigned int param_2, int param_3) {
    struct RideNode *score = DAT_004cc074;
    struct BoatRide *ride = DAT_004cc03c;
    struct RideNode *prev = NULL;
    struct PathNode *path;
    struct MermaidNode *mer;
    short id = (short)param_2;
    int x;
    int y;
    unsigned int savedX;
    unsigned int savedY;
    void *local_8;
    unsigned char local_14[12];

    StandardRemoveObject(param_1, param_2, param_3);
    y = DAT_004cc078[1];
    if (DAT_004cc078[1] <= DAT_004cc078[3]) {
        do {
            x = DAT_004cc078[0];
            if (DAT_004cc078[0] <= DAT_004cc078[2]) {
                do {
                    RestoreBaseMap(*(int *)(param_3 + 0x1404) + x, *(int *)(param_3 + 0x1408) + y);
                    x = x + 1;
                } while (x <= DAT_004cc078[2]);
            }
            y = y + 1;
        } while (y <= DAT_004cc078[3]);
    }
    while (score->id == (unsigned short)id ? 1 : (prev = score, score = score->next, score != NULL)) {
        if (score->id == (unsigned short)id) {
            break;
        }
    }
    if (score->id != (unsigned short)id) {
        return;
    }
    local_8 = DAT_0082adf0;
    IncrementObjectCount(DAT_0082adf0);
    IncrementObjectCount(DAT_0082adf0);
    path = DAT_004d823c;
    while (path != NULL) {
        if (path->field_2 == id) {
            DAT_0082ae20.field_1404 = path->x;
            DAT_0082ae20.field_1408 = path->y;
            FUN_0041c620(local_14, (unsigned short)path->field_2, &DAT_0082ae20);
            path = DAT_004d823c;
        } else {
            path = path->next;
        }
    }
    mer = DAT_004d2164;
    while (mer != NULL) {
        if (mer->field_2 == id) {
            savedX = *(unsigned int *)(param_3 + 0x1404);
            savedY = *(unsigned int *)(param_3 + 0x1408);
            *(unsigned int *)(param_3 + 0x1404) = (unsigned char)mer->field_0;
            *(unsigned int *)(param_3 + 0x1408) = (unsigned char)(mer->field_0 >> 8);
            FUN_0041b6f0(local_14, mer->field_2, (struct Cursor *)param_3);
            *(unsigned int *)(param_3 + 0x1404) = savedX;
            *(unsigned int *)(param_3 + 0x1408) = savedY;
            mer = DAT_004d2164;
        } else {
            mer = mer->next;
        }
    }
    if (prev == NULL) {
        DAT_004cc074 = score->next;
    } else {
        prev->next = score->next;
    }
    while (ride != NULL) {
        if (ride->id == id) {
            FUN_00418f90(ride);
            ride = DAT_004cc03c;
        } else {
            ride = ride->next;
        }
    }
    RemoveAllBlokesFromRide(*(unsigned int *)(param_1 + 0xc), param_2);
    free(score);
}

// FUNCTION: LEGOLAND 0x0041a720
void FUN_0041a720(void) {
    struct RideNode **bloke_list;
    struct RideNode *node;
    int lls;
    int bloke;
    unsigned short id;
    unsigned char idhi;
    struct RideNode *score;
    int slot;
    int frame;
    struct SampleParams params;
    struct Sample *sample;

    bloke_list = *(struct RideNode ***)((char *)DAT_0082c658 + 0xcc);
    lls = GetLLSForSprite(DAT_0082ae00);
    DAT_004cc08c = DAT_004cc08c + 1;
    if (DAT_004cc08c == 0x50) {
        DAT_004cc08c = 0;
        FUN_00419300();
    }
    FUN_00418fe0(0);
    while (bloke_list != NULL) {
        struct RideNode **next_bloke = (struct RideNode **)*bloke_list;
        id = *(unsigned short *)((char *)bloke_list + 0xc);
        idhi = (unsigned char)(id >> 8);
        score = DAT_004cc074;
        while (score != NULL && score->id != id) {
            score = score->next;
        }
        bloke = ((int *)bloke_list)[2];
        if (*(short *)(bloke + 0xe) == 0) {
            switch (*(unsigned char *)(bloke + 0x60)) {
            case 0:
                slot = 0;
                while (slot < 5) {
                    if (score->blokes[slot] == (unsigned int)bloke) {
                        break;
                    }
                    slot = slot + 1;
                }
                if (slot == 5) {
                    slot = 4;
                    if (score->field_14 == 5 || score->blokes[4] != 0) {
                        goto remove;
                    }
                    score->blokes[slot] = bloke;
                    score->field_14 = score->field_14 + 1;
                } else {
                    if (score->blokes[slot] != 0 && slot != 0) {
                    }
                    if (slot != 0) {
                    }
                    {
                        int prevbloke = score->blokes[slot];
                        if (score->blokes[slot - 1] != 0) {
                            break;
                        }
                        score->blokes[slot - 1] = prevbloke;
                        score->blokes[slot] = 0;
                        slot = slot - 1;
                        if (slot == 0) {
                            *(char *)(prevbloke + 0x60) += 1;
                        }
                        bloke = prevbloke;
                    }
                }
                *(unsigned char *)(bloke + 0x62) |= 8;
                *(unsigned int *)(bloke + 0x24) =
                    ((int)*(char *)((char *)DAT_0082c658 + 0xc) + (id & 0xff)) * 0x100 + DAT_004b5290[(4 - slot) * 2];
                frame = ((int)*(char *)((char *)DAT_0082c658 + 0x10) + (idhi & 0xff)) * 0x100 + DAT_004b5290[(4 - slot) * 2 + 1];
                *(int *)(bloke + 0x28) = frame;
                {
                    char dir = CalcMoveLine(*(int *)(bloke + 0x68), *(int *)(bloke + 0x6c), *(int *)(bloke + 0x24), frame, bloke + 0x98);
                    *(unsigned char *)(bloke + 0x73) = dir + 0x10;
                    *(short *)(bloke + 0xe) = 7;
                    NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                }
                break;
            case 1:
                if (bloke == (int)score->blokes[0] && score->field_2 != 0 &&
                    (int)FUN_004192d0((struct BoatRide *)score) * 6 <= (int)score->blokes[3] &&
                    FUN_00418e60((id & 0xff) | (idhi << 8) | (id & 0xffff0000), bloke) != 0) {
                    BlokeSitAnim(bloke);
                    BlokeSetFrame(bloke, 0);
                    score->blokes[0] = 0;
                    score->field_14 = score->field_14 - 1;
                    *(unsigned char *)(bloke + 0x62) |= 0x80;
                    *(char *)(bloke + 0x60) += 1;
                    params.field_0 = 1;
                    params.field_8 = bloke;
                    sample = PlayInstanceOfSample(*(void **)(PTR_s_Boat_Noise_wav + 8), 1, 1, &params);
                    AdjustPSampleFreq(sample, 10);
                }
                break;
            case 3:
                BlokeWalkAnim(bloke);
                BlokeSetFrame(bloke, 0);
                *(unsigned short *)(bloke + 0x62) &= 0xff7f;
                *(unsigned int *)(bloke + 0x68) = (*(char *)((char *)DAT_0082c658 + 0x24) - 4 + (id & 0xff)) * 0x100;
                *(unsigned char *)(bloke + 0x72) = 10;
                *(unsigned int *)(bloke + 0x6c) = (*(char *)((char *)DAT_0082c658 + 0x25) + 2 + (idhi & 0xff)) * 0x100;
                *(unsigned int *)(bloke + 0x24) = ((int)*(char *)((char *)DAT_0082c658 + 0x24) + (id & 0xff)) * 0x100 - 0xc0;
                frame = ((int)*(char *)((char *)DAT_0082c658 + 0x25) + (idhi & 0xff)) * 0x100 + 0x240;
                *(int *)(bloke + 0x28) = frame;
                {
                    char dir = CalcMoveLine(*(int *)(bloke + 0x68), *(int *)(bloke + 0x6c), *(int *)(bloke + 0x24), frame, bloke + 0x98);
                    *(short *)(bloke + 0xe) = 7;
                    *(unsigned char *)(bloke + 0x73) = dir + 0x10;
                    NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                }
                *(char *)(bloke + 0x60) += 1;
                break;
            case 4:
                *(unsigned int *)(bloke + 0x24) = ((int)*(char *)((char *)DAT_0082c658 + 0x24) + (id & 0xff)) * 0x100 - 0xc0;
                frame = ((int)*(char *)((char *)DAT_0082c658 + 0x25) + (idhi & 0xff)) * 0x100 + 0x80;
                *(int *)(bloke + 0x28) = frame;
                {
                    char dir = CalcMoveLine(*(int *)(bloke + 0x68), *(int *)(bloke + 0x6c), *(int *)(bloke + 0x24), frame, bloke + 0x98);
                    *(short *)(bloke + 0xe) = 7;
                    *(unsigned char *)(bloke + 0x73) = dir + 0x10;
                    NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                }
                *(char *)(bloke + 0x60) += 1;
                break;
            case 5:
                *(unsigned int *)(bloke + 0x24) = ((int)*(char *)((char *)DAT_0082c658 + 0x24) + (id & 0xff)) * 0x100 + 0x80;
                frame = ((int)*(char *)((char *)DAT_0082c658 + 0x25) + (idhi & 0xff)) * 0x100 + 0x80;
                *(int *)(bloke + 0x28) = frame;
                {
                    char dir = CalcMoveLine(*(int *)(bloke + 0x68), *(int *)(bloke + 0x6c), *(int *)(bloke + 0x24), frame, bloke + 0x98);
                    *(short *)(bloke + 0xe) = 7;
                    *(unsigned char *)(bloke + 0x73) = dir + 0x10;
                    NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                }
                params.field_0 = 1;
                *(char *)(bloke + 0x60) += 1;
                params.field_8 = bloke;
                UnSourceAndFadeAllSamplesFromSource(&params, 0xffffffa6);
                break;
            case 6:
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
            remove:
                RemoveBlokeFromRide(DAT_0082c658, bloke_list);
            }
        }
        bloke_list = next_bloke;
    }
    if (bloke_list == NULL) {
        for (node = DAT_004cc074; node != NULL; node = node->next) {
            frame = node->field_c + 1;
            node->field_c = frame;
            if (frame <= *(short *)(lls + 0x10)) {
                if (node->field_10 == 0) {
                    frame = *(short *)(lls + 0x10) - frame;
                }
                LLSSetFrame((struct LLS *)lls, frame);
            }
            if (node->field_10 == 0 && node->field_c == 100) {
                node->field_c = 0;
                node->field_10 = 1;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0041abd0
void FUN_0041abd0(int param_1, unsigned int param_2, unsigned int param_3, short *param_4, void *param_5, unsigned int param_6) {
    unsigned int **bloke_list;
    int cursor;
    short *lls;
    union {
        __int64 q;
        int i[2];
    } coords;
    union {
        __int64 q;
        int i[2];
    } offset;

    cursor = *(int *)(param_1 + 0xc);
    bloke_list = *(unsigned int ***)(cursor + 0xcc);
    FUN_00418fe0(1);
    coords.q = GetScreenCoordsForObject((unsigned char *)param_4, (void *)cursor);
    offset.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0082c658->field_64, 3);
    AdjustOffsetForViewMode((struct AdjustStruct *)&offset);
    lls = (short *)GetLLSForSprite((struct SpriteLLS *)GetSpriteForLayer((struct LayerContainer *)DAT_0082c658->field_64, 3));
    LLSSetFrame((struct LLS *)GetLLSForSprite((struct SpriteLLS *)DAT_0082adfc), *lls);
    PrintSprite(DAT_0082adfc, offset.i[0] + coords.i[0], offset.i[1] + coords.i[1], param_6, 0);
    for (; bloke_list != NULL; bloke_list = (unsigned int **)*bloke_list) {
        if (*param_4 == *(short *)(bloke_list + 3) && *(char *)(((int *)bloke_list)[2] + 0x60) != 2) {
            IP_RenderBlokeIn3DNow((struct Bloke *)((int *)bloke_list)[2]);
        }
    }
    offset.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0082c658->field_64, 3);
    offset.i[0] = offset.i[0] + 0x71;
    offset.i[1] = offset.i[1] + 0xac;
    AdjustOffsetForViewMode((struct AdjustStruct *)&offset);
    PrintSprite(DAT_0082c654, offset.i[0] + coords.i[0], offset.i[1] + coords.i[1], param_6, 0);
}

// FUNCTION: LEGOLAND 0x0041acf0
int FUN_0041acf0(void) {
    struct RideNode *score;
    struct PathNode *path;
    struct MermaidNode *mer;
    struct BoatRide *ride;
    int count;
    int i;
    struct RideNode scoreCopy;
    struct BoatRide rideCopy;

    count = 0;
    for (score = DAT_004cc074; score != NULL; score = score->next) {
        count = count + 1;
    }
    SaveGameWrite(&count, 4);
    score = DAT_004cc074;
    while (count != 0) {
        count = count - 1;
        scoreCopy = *score;
        for (i = 0; i < 5; i = i + 1) {
            scoreCopy.blokes[i] = GetBlokeNum(scoreCopy.blokes[i]);
        }
        SaveGameWrite(&scoreCopy, 0x34);
        score = score->next;
    }
    count = 0;
    for (path = DAT_004d823c; path != NULL; path = path->next) {
        count = count + 1;
    }
    SaveGameWrite(&count, 4);
    path = DAT_004d823c;
    while (count != 0) {
        count = count - 1;
        SaveGameWrite(path, 0x1c);
        path = path->next;
    }
    count = 0;
    for (mer = DAT_004d2164; mer != NULL; mer = mer->next) {
        count = count + 1;
    }
    SaveGameWrite(&count, 4);
    mer = DAT_004d2164;
    while (count != 0) {
        count = count - 1;
        SaveGameWrite(mer, 8);
        mer = mer->next;
    }
    count = 0;
    for (ride = DAT_004cc03c; ride != NULL; ride = ride->next) {
        count = count + 1;
    }
    SaveGameWrite(&count, 4);
    ride = DAT_004cc03c;
    while (count != 0) {
        count = count - 1;
        rideCopy = *ride;
        rideCopy.field_3ec = GetBlokeNum(rideCopy.field_3ec);
        SaveGameWrite(&rideCopy, 0x3f4);
        ride = ride->next;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0041aee0
int FUN_0041aee0(void) {
    struct RideNode *score;
    struct RideNode *prevScore;
    struct PathNode *path;
    struct PathNode *prevPath;
    struct MermaidNode *mer;
    struct MermaidNode *prevMer;
    struct BoatRide *ride;
    struct BoatRide *prevRide;
    int count;
    int i;

    prevScore = NULL;
    SaveGameRead(&count, 4);
    while (count != 0) {
        count = count - 1;
        if (prevScore == NULL) {
            score = (struct RideNode *)malloc(sizeof(struct RideNode));
            DAT_004cc074 = score;
        } else {
            score = (struct RideNode *)malloc(sizeof(struct RideNode));
            prevScore->next = score;
        }
        SaveGameRead(score, 0x34);
        for (i = 0; i < 5; i = i + 1) {
            score->blokes[i] = GetBlokePtr(score->blokes[i]);
        }
        prevScore = score;
    }
    prevPath = NULL;
    SaveGameRead(&count, 4);
    while (count != 0) {
        count = count - 1;
        if (prevPath == NULL) {
            path = (struct PathNode *)malloc(0x1c);
            DAT_004d823c = path;
        } else {
            path = (struct PathNode *)malloc(0x1c);
            prevPath->next = path;
        }
        SaveGameRead(path, 0x1c);
        prevPath = path;
    }
    prevMer = NULL;
    SaveGameRead(&count, 4);
    while (count != 0) {
        count = count - 1;
        if (prevMer == NULL) {
            mer = (struct MermaidNode *)malloc(8);
            DAT_004d2164 = mer;
        } else {
            mer = (struct MermaidNode *)malloc(8);
            prevMer->next = mer;
        }
        SaveGameRead(mer, 8);
        prevMer = mer;
    }
    prevRide = NULL;
    SaveGameRead(&count, 4);
    while (count != 0) {
        count = count - 1;
        if (prevRide == NULL) {
            ride = (struct BoatRide *)malloc(sizeof(struct BoatRide));
            DAT_004cc03c = ride;
        } else {
            ride = (struct BoatRide *)malloc(sizeof(struct BoatRide));
            prevRide->next = ride;
        }
        SaveGameRead(ride, 0x3f4);
        ride->field_3ec = GetBlokePtr(ride->field_3ec);
        prevRide = ride;
    }
    for (score = DAT_004cc074; score != NULL; score = score->next) {
        FUN_0041caa0(score->id);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0041b0d0
void FUN_0041b0d0(unsigned short id, unsigned int value) {
    struct RideNode *node = DAT_004cc074;
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
    struct RideNode *node = DAT_004cc074;
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
    DAT_0082adf8 = param_1->building;
}

// FUNCTION: LEGOLAND 0x0041b260
void FUN_0041b260(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0082adf8;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(DAT_0082adf8->footprint);
}

// FUNCTION: LEGOLAND 0x0041b2a0
void FUN_0041b2a0(int param_1, int *param_2) {
    struct Cursor *cursor = *(struct Cursor **)(param_1 + 0xc);
    struct MermaidNode *node;
    unsigned char temp[2];
    int x;
    int y;
    short tile;
    struct SampleParams params;
    unsigned short coord;

    temp[0] = (unsigned char)param_2[0];
    temp[1] = (unsigned char)param_2[1];
    coord = *(unsigned short *)temp;
    FUN_0041c690(param_2[0], param_2[1], (unsigned short *)&param_2);
    node = (struct MermaidNode *)malloc(8);
    if (node == NULL) {
        return;
    }
    node->field_0 = coord;
    node->field_2 = (unsigned short)(unsigned int)param_2;
    node->next = DAT_004d2164;
    DAT_004d2164 = node;
    FUN_0041b0d0((unsigned short)(unsigned int)param_2, 1);
    AddBasicObject(param_1, (unsigned int)param_2);
    y = cursor->field_3c.v[1];
    if (cursor->field_3c.v[1] <= cursor->field_3c.v[3]) {
        do {
            x = cursor->field_3c.v[0];
            if (cursor->field_3c.v[0] <= cursor->field_3c.v[2]) {
                do {
                    if (x == cursor->field_3c.v[0]) {
                        tile = *DAT_0082adf4->tiles + 9;
                    } else if (x == cursor->field_3c.v[2]) {
                        tile = *DAT_0082adf4->tiles + 0xc;
                    } else if (y == cursor->field_3c.v[1]) {
                        tile = *DAT_0082adf4->tiles + 10;
                    } else if (y == cursor->field_3c.v[3]) {
                        tile = *DAT_0082adf4->tiles + 0xb;
                    } else {
                        tile = *DAT_0082adf4->tiles;
                    }
                    SetMapTile(param_2[0] + x, param_2[1] + y, tile);
                    x = x + 1;
                } while (x <= cursor->field_3c.v[2]);
            }
            y = y + 1;
        } while (y <= cursor->field_3c.v[3]);
    }
    SetMapTile(cursor->field_3c.v[0] + param_2[0], cursor->field_3c.v[1] + param_2[1], *DAT_0082adf4->tiles + 5);
    SetMapTile(param_2[0] + cursor->field_3c.v[2], cursor->field_3c.v[1] + param_2[1], *DAT_0082adf4->tiles + 8);
    SetMapTile(cursor->field_3c.v[0] + param_2[0], cursor->field_3c.v[3] + param_2[1], *DAT_0082adf4->tiles + 6);
    SetMapTile(param_2[0] + cursor->field_3c.v[2], cursor->field_3c.v[3] + param_2[1], *DAT_0082adf4->tiles + 7);
    params.field_8 = param_2[0];
    params.field_c = param_2[1];
    params.field_0 = 2;
    PlayInstanceOfSample(*(void **)(PTR_s_Boat_Noise_wav + 0x14), 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x0041b4c0
void FUN_0041b4c0(int param_1, unsigned int param_2, unsigned int param_3) {
    struct Cursor *cursor = *(struct Cursor **)(param_1 + 0xc);
    unsigned int mask;
    int valid;
    int x;
    int y;
    unsigned int n;
    int i;

    *(struct Footprint *)EditCursor.field_1414 = cursor->field_3c;
    ScreenToMapRef(param_2, &EditCursor.field_1404, param_3);
    mask = FUN_0041c690(EditCursor.field_1404, EditCursor.field_1408, (unsigned short *)&param_1);
    EditCursor.field_1830 = 0;
    if (mask == 0) {
        FUN_0045f480(&EditCursor, 0xe);
        return;
    }
    ValidateCursor(&EditCursor, (unsigned int)cursor);
    valid = FUN_0045f4b0(&EditCursor);
    if (valid == 0) {
        return;
    }
    DefaultCursor(&DAT_004cc090[0]);
    DefaultCursor(&DAT_004cc090[1]);
    DefaultCursor(&DAT_004cc090[2]);
    DefaultCursor(&DAT_004cc090[3]);
    *(struct Footprint *)DAT_004cc090[0].field_1414 = *(struct Footprint *)EditCursor.field_1414;
    *(struct Footprint *)DAT_004cc090[1].field_1414 = *(struct Footprint *)EditCursor.field_1414;
    *(struct Footprint *)DAT_004cc090[2].field_1414 = *(struct Footprint *)EditCursor.field_1414;
    *(struct Footprint *)DAT_004cc090[3].field_1414 = *(struct Footprint *)EditCursor.field_1414;
    FUN_0045f460(&DAT_004cc090[0]);
    FUN_0045f460(&DAT_004cc090[1]);
    FUN_0045f460(&DAT_004cc090[2]);
    FUN_0045f460(&DAT_004cc090[3]);
    y = EditCursor.field_1408;
    x = EditCursor.field_1404;
    DAT_004cc090[0].field_1828 = 0x2034;
    DAT_004cc090[1].field_1828 = 0x2034;
    DAT_004cc090[2].field_1828 = 0x2034;
    DAT_004cc090[3].field_1828 = 0x2034;
    if ((mask & 1) != 0) {
        DAT_004cc090[0].field_1408 = EditCursor.field_1408 - 5;
        DAT_004cc090[0].field_1404 = EditCursor.field_1404;
    }
    n = mask & 1;
    if ((mask & 2) != 0) {
        DAT_004cc090[n].field_1404 = EditCursor.field_1404 + 5;
        DAT_004cc090[n].field_1408 = y;
        n = n + 1;
    }
    if ((mask & 4) != 0) {
        DAT_004cc090[n].field_1404 = x;
        DAT_004cc090[n].field_1408 = y + 5;
        n = n + 1;
    }
    if ((mask & 8) != 0) {
        DAT_004cc090[n].field_1404 = x - 5;
        DAT_004cc090[n].field_1408 = y;
        n = n + 1;
    }
    if (n != 0) {
        EditCursor.field_1830 = (unsigned int)&DAT_004cc090[0];
        if (1 < n) {
            struct Cursor *c = &DAT_004cc090[1];
            i = n - 1;
            do {
                c[-1].field_1830 = (unsigned int)c;
                c = c + 1;
                i = i - 1;
            } while (i != 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041b6d0
unsigned int FUN_0041b6d0(unsigned int param_1, unsigned int param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x0041b6f0
void FUN_0041b6f0(void *param_1, unsigned short param_2, struct Cursor *param_3) {
    struct Cursor *cursor = *(struct Cursor **)((char *)param_1 + 0xc);
    struct MermaidNode *node = DAT_004d2164;
    struct MermaidNode *prev = NULL;
    struct MermaidNode *nxt;
    int x;
    int y;
    int n;
    struct SampleParams params;

    StandardRemoveObject((unsigned int)param_1, param_2, (unsigned int)param_3);
    params.field_8 = param_2 & 0xff;
    params.field_c = param_2 >> 8 & 0xff;
    params.field_0 = 2;
    n = CountSamplesFromSource(&params);
    if (n != 1) {
        // STRING: LEGOLAND 0x004b5398
        DBPrintf("Can't find samples for mermaid\n");
    }
    UnSourceAndFadeAllSamplesFromSource(&params, 0xfffffe70);
    y = cursor->field_3c.v[1];
    if (cursor->field_3c.v[1] <= cursor->field_3c.v[3]) {
        do {
            x = cursor->field_3c.v[0];
            if (cursor->field_3c.v[0] <= cursor->field_3c.v[2]) {
                do {
                    RestoreBaseMap(x + param_3->field_1404, y + param_3->field_1408);
                    x = x + 1;
                } while (x <= cursor->field_3c.v[2]);
            }
            y = y + 1;
        } while (y <= cursor->field_3c.v[3]);
    }
    while (node->field_2 != param_2) {
        nxt = node->next;
        if (nxt == NULL) {
            return;
        }
        prev = node;
        node = nxt;
    }
    if (node != NULL) {
        FUN_0041b0d0(node->field_2, 0xffffffff);
        if (prev != NULL) {
            prev->next = node->next;
            free(node);
            return;
        }
        DAT_004d2164 = node->next;
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x0041b830
void FUN_0041b830(struct RideObject *arg) {
    struct RideBuilding *building = arg->building;
    DAT_0082adf0 = building;
    DAT_004b53c0[1] += building->footprint[1];
    DAT_004b53c0[0] += building->footprint[0];
    DAT_004b53c0[2] += building->footprint[0];
    DAT_004b53c0[3] += building->footprint[1];
}

// FUNCTION: LEGOLAND 0x0041b880
void FUN_0041b880(void) {
    struct RideBuilding *state = DAT_0082adf0;
    EditMode = 1;
    DAT_008119b8 = state;
    memcpy(state->footprint, DAT_004b53c0, 20);
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x0041b8e0
void FUN_0041b8e0(int param_1, int *param_2) {
    int *piVar1 = param_2;
    struct RideNode *score = DAT_004cc074;
    unsigned int mask;
    int dir;

    mask = FUN_0041c690(*param_2, param_2[1], (unsigned short *)&param_2);
    FUN_0041c4c0(*piVar1, piVar1[1], mask, (unsigned short *)&param_2);
    IncrementObjectCount(*(void **)(param_1 + 0xc));
    FUN_0041b0d0((unsigned short)(unsigned int)param_2, 1);
    FUN_0041bab0(*piVar1, piVar1[1], (unsigned short *)&param_2);
    if ((mask & 1) != 0) {
        dir = FUN_0041c690(*piVar1, piVar1[1] - 5, (unsigned short *)&param_2);
        FUN_0041c4c0(*piVar1, piVar1[1] - 5, dir, (unsigned short *)&param_2);
        FUN_0041bab0(*piVar1, piVar1[1] - 5, (unsigned short *)&param_2);
    }
    if ((mask & 2) != 0) {
        dir = FUN_0041c690(*piVar1 + 5, piVar1[1], (unsigned short *)&param_2);
        FUN_0041c4c0(*piVar1 + 5, piVar1[1], dir, (unsigned short *)&param_2);
        FUN_0041bab0(*piVar1 + 5, piVar1[1], (unsigned short *)&param_2);
    }
    if ((mask & 4) != 0) {
        dir = FUN_0041c690(*piVar1, piVar1[1] + 5, (unsigned short *)&param_2);
        FUN_0041c4c0(*piVar1, piVar1[1] + 5, dir, (unsigned short *)&param_2);
        FUN_0041bab0(*piVar1, piVar1[1] + 5, (unsigned short *)&param_2);
    }
    if ((mask & 8) != 0) {
        dir = FUN_0041c690(*piVar1 - 5, piVar1[1], (unsigned short *)&param_2);
        FUN_0041c4c0(*piVar1 - 5, piVar1[1], dir, (unsigned short *)&param_2);
        FUN_0041bab0(*piVar1 - 5, piVar1[1], (unsigned short *)&param_2);
    }
    if (score != NULL) {
        while (score->id != (unsigned short)(unsigned int)param_2) {
            score = score->next;
            if (score == NULL) {
                return;
            }
        }
        dir = FUN_0041c8c0(score->field_2, score->field_3, score->field_4, score->field_5);
        score->field_8 = dir;
        if (dir != 0) {
            FUN_0041caa0((unsigned short)(unsigned int)param_2);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041bab0
void FUN_0041bab0(int param_1, int param_2, unsigned short *param_3) {
    struct RideNode *score = DAT_004cc074;
    struct PathNode *path;
    int other;
    unsigned int mask;

    path = FUN_0041c890(param_1, param_2);
    if (score != NULL) {
        do {
            if (score->id == *param_3) {
                break;
            }
            score = score->next;
        } while (score != NULL);
    }
    if (path != NULL) {
        mask = path->field_4;
        if (*(short *)&path->x == *(short *)&score->field_2) {
            mask = mask & 0xfffffffe;
        } else if (*(short *)&path->x == *(short *)&score->field_4) {
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
void FUN_0041bd40(int param_1, unsigned int param_2, unsigned int param_3) {
    unsigned int mask;
    int valid;
    int x;
    int y;
    unsigned int n;
    int i;
    int local_10;
    int local_c;
    int local_8;
    int local_4;
    int result;

    *(struct Footprint *)EditCursor.field_1414 = *(struct Footprint *)DAT_004b53c0;
    ScreenToMapRef(param_2, &EditCursor.field_1404, param_3);
    mask = FUN_0041c690(EditCursor.field_1404, EditCursor.field_1408, (unsigned short *)&param_3);
    EditCursor.field_1830 = 0;
    if (mask == 0) {
        FUN_0045f480(&EditCursor, 0xe);
        return;
    }
    ValidateCursor(&EditCursor, *(unsigned int *)(param_1 + 0xc));
    valid = FUN_0045f4b0(&EditCursor);
    if (valid == 0) {
        return;
    }
    local_10 = EditCursor.field_1414[0] + EditCursor.field_1404;
    local_c = EditCursor.field_1414[1] + EditCursor.field_1408;
    local_8 = EditCursor.field_1414[2] + EditCursor.field_1404;
    local_4 = EditCursor.field_1414[3] + EditCursor.field_1408;
    result = CheckForPeople(&local_10);
    if (result == -1) {
        FUN_0045f480(&EditCursor, 4);
        return;
    }
    if (result == 1) {
        FUN_0045f480(&EditCursor, 3);
        return;
    }
    DefaultCursor(&DAT_004d2168[0]);
    DefaultCursor(&DAT_004d2168[1]);
    DefaultCursor(&DAT_004d2168[2]);
    DefaultCursor(&DAT_004d2168[3]);
    *(struct Footprint *)DAT_004d2168[0].field_1414 = *(struct Footprint *)EditCursor.field_1414;
    *(struct Footprint *)DAT_004d2168[1].field_1414 = *(struct Footprint *)EditCursor.field_1414;
    *(struct Footprint *)DAT_004d2168[2].field_1414 = *(struct Footprint *)EditCursor.field_1414;
    *(struct Footprint *)DAT_004d2168[3].field_1414 = *(struct Footprint *)EditCursor.field_1414;
    FUN_0045f460(&DAT_004d2168[0]);
    FUN_0045f460(&DAT_004d2168[1]);
    FUN_0045f460(&DAT_004d2168[2]);
    FUN_0045f460(&DAT_004d2168[3]);
    y = EditCursor.field_1408;
    x = EditCursor.field_1404;
    DAT_004d2168[0].field_1828 = 0x2034;
    DAT_004d2168[1].field_1828 = 0x2034;
    DAT_004d2168[2].field_1828 = 0x2034;
    DAT_004d2168[3].field_1828 = 0x2034;
    if ((mask & 1) != 0) {
        DAT_004d2168[0].field_1408 = EditCursor.field_1408 - 5;
        DAT_004d2168[0].field_1404 = EditCursor.field_1404;
    }
    n = mask & 1;
    if ((mask & 2) != 0) {
        DAT_004d2168[n].field_1404 = EditCursor.field_1404 + 5;
        DAT_004d2168[n].field_1408 = y;
        n = n + 1;
    }
    if ((mask & 4) != 0) {
        DAT_004d2168[n].field_1404 = x;
        DAT_004d2168[n].field_1408 = y + 5;
        n = n + 1;
    }
    if ((mask & 8) != 0) {
        DAT_004d2168[n].field_1404 = x - 5;
        DAT_004d2168[n].field_1408 = y;
        n = n + 1;
    }
    if (n == 0) {
        return;
    }
    EditCursor.field_1830 = (unsigned int)&DAT_004d2168[0];
    if (n < 2) {
        EditCursor.field_1830 = (unsigned int)&DAT_004d2168[0];
        return;
    }
    {
        struct Cursor *c = &DAT_004d2168[1];
        i = n - 1;
        do {
            c[-1].field_1830 = (unsigned int)c;
            c = c + 1;
            i = i - 1;
        } while (i != 0);
    }
}

// FUNCTION: LEGOLAND 0x0041bfb0
void FUN_0041bfb0(unsigned int param_1, unsigned int *param_2) {
    struct RideNode *score = DAT_004cc074;
    struct BoatRide *ride;
    unsigned char x;
    unsigned char y;
    int *tile;
    int *prev_tile;
    unsigned char temp[2];

    if ((int)*param_2 < 0 || (int)(unsigned int)lpConfig->width <= (int)*param_2 ||
        (int)param_2[1] < 0 || (int)(unsigned int)lpConfig->height <= (int)param_2[1]) {
        tile = NULL;
    } else {
        tile = (int *)((char *)GameMap[param_2[1]] + *param_2 * 0x14);
    }
    x = *(unsigned char *)((char *)tile + 4);
    *param_2 = x;
    y = *(unsigned char *)((char *)tile + 5);
    param_2[1] = y;
    ride = DAT_004cc03c;
    if (score != NULL) {
        do {
            if (*(short *)temp == *(short *)&score->field_2 || *(short *)temp == *(short *)&score->field_4) {
                struct PathNode *p;
                temp[0] = x;
                temp[1] = y;
                p = FUN_0041c890(*param_2, y);
                QueryObj = p->x;
                *param_2 = p->x;
                QueryObj = (unsigned short)((QueryObj & 0xff) | (p->y << 8));
                param_2[1] = p->y;
                *(struct Footprint *)((char *)QueryClass + 0x3c) = *(struct Footprint *)DAT_004cc078;
                FUN_0041a3d0((void *)temp, param_1);
                return;
            }
            score = score->next;
        } while (score != NULL);
    }
    (void)temp;
    *(struct Footprint *)((char *)QueryClass + 0x3c) = *(struct Footprint *)DAT_004b53c0;
    BasicObjectDCalcCursor(param_1, (unsigned int)param_2);
    if (tile != NULL) {
        unsigned int key = (unsigned int)x | ((unsigned int)y << 8);
        key = key & 0xff;
        while ((key != ride->field_4 || (unsigned int)y != ride->field_8) &&
               (key != ride->field_c || (unsigned int)y != ride->field_10)) {
            ride = ride->next;
            if (ride == NULL) {
                return;
            }
        }
        FUN_0045f480(&QueryCursor, 1);
    }
}

// FUNCTION: LEGOLAND 0x0041c130
void FUN_0041c130(void *param_1, unsigned int param_2, struct Cursor *param_3) {
    struct RideNode *score = DAT_004cc074;
    struct RideNode *find = DAT_004cc074;
    int *tile;
    unsigned int mask;
    unsigned int u1;
    unsigned int u2;
    unsigned int u3;
    unsigned int u9;
    int x;
    int y;
    int other;
    unsigned char local_14[12];
    unsigned int local_param2 = param_2;

    if ((param_2 & 0xff) < (unsigned int)lpConfig->width && (param_2 >> 8 & 0xff) < (unsigned int)lpConfig->height) {
        tile = (int *)((char *)GameMap[param_2 >> 8 & 0xff] + (param_2 & 0xff) * 0x14);
    } else {
        tile = NULL;
    }
    if (*tile != (int)DAT_0082adf0->footprint[(0xc4 - 0x3c) / 4]) {
        *(struct Footprint *)((char *)QueryClass + 0x3c) = *(struct Footprint *)DAT_004b53c0;
        FUN_0041a530((int)local_14, local_param2, (int)param_3);
        return;
    }
    u9 = FUN_0041c690(param_3->field_1404, param_3->field_1408, (unsigned short *)&param_2);
    FUN_0041c620(param_1, (unsigned short)local_param2, param_3);
    FUN_0041b0d0((unsigned short)(unsigned int)param_2, 0xffffffff);
    u1 = u9 & 1;
    if (u1 != 0) {
        y = param_3->field_1408 - 5;
        x = param_3->field_1404;
        mask = FUN_0041c690(x, y, (unsigned short *)&param_3);
        FUN_0041c4c0(x, y, mask, (unsigned short *)&param_2);
        FUN_0041bab0(x, y, (unsigned short *)&param_2);
    }
    u2 = u9 & 2;
    if (u2 != 0) {
        y = param_3->field_1408;
        x = param_3->field_1404 + 5;
        mask = FUN_0041c690(x, y, (unsigned short *)&param_3);
        FUN_0041c4c0(x, y, mask, (unsigned short *)&param_2);
        FUN_0041bab0(x, y, (unsigned short *)&param_2);
    }
    u3 = u9 & 4;
    if (u3 != 0) {
        y = param_3->field_1408 + 5;
        x = param_3->field_1404;
        mask = FUN_0041c690(x, y, (unsigned short *)&param_3);
        FUN_0041c4c0(x, y, mask, (unsigned short *)&param_2);
        FUN_0041bab0(x, y, (unsigned short *)&param_2);
    }
    u9 = u9 & 8;
    if (u9 != 0) {
        y = param_3->field_1408;
        x = param_3->field_1404 - 5;
        mask = FUN_0041c690(x, y, (unsigned short *)&param_3);
        FUN_0041c4c0(x, y, mask, (unsigned short *)&param_2);
        FUN_0041bab0(x, y, (unsigned short *)&param_2);
    }
    if (u1 != 0) {
        if (u9 != 0) {
            y = param_3->field_1408 - 5;
            x = param_3->field_1404 - 5;
            other = (int)FUN_0041c890(x, y);
            if (other != 0) {
                mask = FUN_0041c690(x, y, (unsigned short *)&param_3);
                FUN_0041c4c0(x, y, mask, (unsigned short *)&param_2);
                FUN_0041bab0(x, y, (unsigned short *)&param_2);
            }
        }
        if (u1 != 0 && u2 != 0) {
            y = param_3->field_1408 - 5;
            x = param_3->field_1404 + 5;
            other = (int)FUN_0041c890(x, y);
            if (other != 0) {
                mask = FUN_0041c690(x, y, (unsigned short *)&param_3);
                FUN_0041c4c0(x, y, mask, (unsigned short *)&param_2);
                FUN_0041bab0(x, y, (unsigned short *)&param_2);
            }
        }
    }
    if (u3 != 0 && u9 != 0) {
        y = param_3->field_1408 + 5;
        x = param_3->field_1404 - 5;
        other = (int)FUN_0041c890(x, y);
        if (other != 0) {
            mask = FUN_0041c690(x, y, (unsigned short *)&param_3);
            FUN_0041c4c0(x, y, mask, (unsigned short *)&param_2);
            FUN_0041bab0(x, y, (unsigned short *)&param_2);
        }
    }
    if (u3 != 0 && u2 != 0) {
        y = param_3->field_1408 + 5;
        x = param_3->field_1404 + 5;
        other = (int)FUN_0041c890(x, y);
        if (other != 0) {
            mask = FUN_0041c690(x, y, (unsigned short *)&param_3);
            FUN_0041c4c0(x, y, mask, (unsigned short *)&param_2);
            FUN_0041bab0(x, y, (unsigned short *)&param_2);
        }
    }
    FUN_0041caa0((unsigned short)(unsigned int)param_2);
    if (score != NULL) {
        while (find->id != (unsigned short)(unsigned int)param_2) {
            find = find->next;
            if (find == NULL) {
                return;
            }
        }
        find->value = FUN_0041c8c0(find->field_2, find->field_3, find->field_4, find->field_5);
    }
}

// FUNCTION: LEGOLAND 0x0041c4c0
void FUN_0041c4c0(int param_1, int param_2, int param_3, unsigned short *param_4) {
    struct PathNode *node;
    int *tile;
    int row;
    int col;
    int cx;
    unsigned char *map;
    unsigned short coord;

    coord = (unsigned short)((unsigned char)param_1 | ((unsigned char)param_2 << 8));
    node = FUN_0041c890(param_1, param_2);
    if (node == NULL) {
        node = (struct PathNode *)malloc(0x1c);
        if (node == NULL) {
            return;
        }
        node->field_c = 0;
        node->next = DAT_004d823c;
        DAT_004d823c = node;
    }
    *(unsigned short *)node = coord;
    node->field_4 = param_3;
    if (param_4 != NULL) {
        node->field_2 = *param_4;
    }
    BGFullUpdate = 1;
    map = &DAT_004b53d4[param_3 * 0x19];
    row = 0;
    do {
        int yy = param_2 - 2 + row;
        cx = param_1 - 2;
        do {
            if (cx < 0 || (int)(unsigned int)lpConfig->width <= cx || yy < 0 || (int)(unsigned int)lpConfig->height <= yy) {
                tile = NULL;
            } else {
                tile = (int *)((char *)GameMap[yy] + cx * 0x14);
            }
            *(unsigned short *)((char *)tile + 0xc) = 8;
            *(unsigned char *)((char *)tile + 0x10) = 2;
            *tile = (int)DAT_0082adf0->footprint[(0xc4 - 0x3c) / 4];
            *(unsigned short *)((char *)tile + 4) = coord;
            SetMapTile(cx, yy, *DAT_0082adf4->tiles + *map);
            cx = cx + 1;
            map = map + 1;
        } while ((2 - param_1) + cx < 5);
        row = row + 1;
    } while (row < 5);
}

// FUNCTION: LEGOLAND 0x0041c620
void FUN_0041c620(void *param_1, unsigned short param_2, struct Cursor *param_3) {
    struct PathNode *node = DAT_004d823c;
    struct PathNode *prev = NULL;
    struct PathNode *nxt;
    short key;

    StandardRemoveObject((unsigned int)param_1, param_2, (unsigned int)param_3);
    key = *(short *)node;
    while (key != (short)param_2) {
        nxt = node->next;
        if (nxt == NULL) {
            return;
        }
        prev = node;
        node = nxt;
        key = *(short *)nxt;
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
unsigned int FUN_0041c690(int param_1, int param_2, unsigned short *param_3) {
    struct RideNode *score = DAT_004cc074;
    struct PathNode *node;
    int n;
    unsigned int mask;
    int valid;
    short key;

    mask = 0;
    node = FUN_0041c890(param_1, param_2);
    valid = node != NULL;
    if (valid) {
        *param_3 = node->field_2;
    }
    n = param_2 - 5;
    if (-1 < param_1 && -1 < n && param_1 < (int)(unsigned int)lpConfig->width && n < (int)(unsigned int)lpConfig->height &&
        (node = FUN_0041c890(param_1, n), node != NULL)) {
        if (valid) {
            if (node->field_2 == *param_3) {
                mask = 1;
            }
        } else {
            mask = 1;
            *param_3 = node->field_2;
            valid = 1;
        }
    }
    n = param_1 + 5;
    if (-1 < n && -1 < param_2 && n < (int)(unsigned int)lpConfig->width && param_2 < (int)(unsigned int)lpConfig->height &&
        (node = FUN_0041c890(n, param_2), node != NULL)) {
        if (valid) {
            if (node->field_2 == *param_3) {
                mask = mask | 2;
            }
        } else {
            mask = mask | 2;
            *param_3 = node->field_2;
            valid = 1;
        }
    }
    n = param_2 + 5;
    if (-1 < param_1 && -1 < n && param_1 < (int)(unsigned int)lpConfig->width && n < (int)(unsigned int)lpConfig->height &&
        (node = FUN_0041c890(param_1, n), node != NULL)) {
        if (valid) {
            if (node->field_2 == *param_3) {
                mask = mask | 4;
            }
        } else {
            mask = mask | 4;
            *param_3 = node->field_2;
            valid = 1;
        }
    }
    n = param_1 - 5;
    if (-1 < n && -1 < param_2 && n < (int)(unsigned int)lpConfig->width && param_2 < (int)(unsigned int)lpConfig->height &&
        (node = FUN_0041c890(n, param_2), node != NULL)) {
        if (valid) {
            if (node->field_2 == *param_3) {
                mask = mask | 8;
            }
        } else {
            mask = mask | 8;
            *param_3 = node->field_2;
        }
    }
    key = (short)((unsigned char)param_1 | ((unsigned char)param_2 << 8));
    if (score != NULL) {
        while (1) {
            if (key == (short)score->id) {
                return mask | 1;
            }
            if (key == *(short *)&score->field_4) {
                break;
            }
            score = score->next;
            if (score == NULL) {
                return mask;
            }
        }
        mask = mask | 4;
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
    while (current != NULL && *(unsigned short *)current != key) {
        current = current->next;
    }

    return current;
}

// FUNCTION: LEGOLAND 0x0041c8c0
int FUN_0041c8c0(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
    struct PathNode *node;
    short key;
    int result;

    result = 0;
    for (node = DAT_004d823c; node != NULL; node = node->next) {
        node->field_c = 0;
    }
    node = FUN_0041c890(a, b);
    if (node != NULL) {
        key = node->field_2;
        FUN_0041c940(a, b, c, d, &key, &result);
        return result;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0041c940
void FUN_0041c940(int param_1, int param_2, int param_3, int param_4, short *param_5, int *param_6) {
    struct PathNode *node;
    struct PathNode *neighbour;

    if (*param_6 != 1) {
        while (1) {
            node = FUN_0041c890(param_1, param_2);
            if (node == NULL) {
                return;
            }
            if (node->field_2 != *param_5) {
                return;
            }
            if (param_1 == param_3 && param_2 == param_4) {
                break;
            }
            node->field_c = 1;
            if ((node->field_4 & 1) != 0 &&
                (neighbour = FUN_0041c890(param_1, (param_2 - 5)), neighbour != NULL) && neighbour->field_c == 0) {
                FUN_0041c940(param_1, param_2 - 5, param_3, param_4, param_5, param_6);
            }
            if ((node->field_4 & 2) != 0 &&
                (neighbour = FUN_0041c890((param_1 + 5), param_2), neighbour != NULL) && neighbour->field_c == 0) {
                FUN_0041c940(param_1 + 5, param_2, param_3, param_4, param_5, param_6);
            }
            if ((node->field_4 & 4) != 0 &&
                (neighbour = FUN_0041c890(param_1, (param_2 + 5)), neighbour != NULL) && neighbour->field_c == 0) {
                FUN_0041c940(param_1, param_2 + 5, param_3, param_4, param_5, param_6);
            }
            if ((node->field_4 & 8) == 0) {
                return;
            }
            param_1 = param_1 - 5;
            node = FUN_0041c890(param_1, param_2);
            if (node == NULL) {
                return;
            }
            if (node->field_c != 0) {
                return;
            }
            if (*param_6 == 1) {
                return;
            }
        }
        *param_6 = 1;
    }
}

// FUNCTION: LEGOLAND 0x0041caa0
void FUN_0041caa0(unsigned short param_1) {
    struct RideNode *score = DAT_004cc074;
    struct PathNode *node;
    int again;

    for (node = DAT_004d823c; node != NULL; node = node->next) {
        if (node->field_2 == param_1) {
            node->field_18 = NULL;
        }
    }
    while (score != NULL && score->id != param_1) {
        score = score->next;
    }
    DAT_004d8240 = (struct RideNode *)FUN_0041c890(score->field_4, score->field_5);
    *(unsigned int *)((char *)DAT_004d8240 + 8) = 0;
    *(unsigned int *)((char *)DAT_004d8240 + 0x14) = 0;
    do {
        DAT_004d8244 = NULL;
        FUN_0041cb20((short)param_1);
        again = DAT_004d8244 != NULL;
        DAT_004d8240 = DAT_004d8244;
        DAT_004d8244 = NULL;
    } while (again);
}

// FUNCTION: LEGOLAND 0x0041cb20
void FUN_0041cb20(short param_1) {
    struct PathNode *p;
    int n1;
    int n2;
    int n3;
    int n4;

    for (p = (struct PathNode *)DAT_004d8240; p != NULL; p = *(struct PathNode **)((char *)p + 0x14)) {
        n1 = (int)FUN_0041c890(p->x, p->y - 5);
        n2 = (int)FUN_0041c890(p->x + 5, p->y);
        n3 = (int)FUN_0041c890(p->x, p->y + 5);
        n4 = (int)FUN_0041c890(p->x - 5, p->y);
        if (n1 != 0 && *(short *)(n1 + 2) == param_1 && *(int *)(n1 + 0x18) == 0) {
            *(struct PathNode **)(n1 + 0x18) = p;
            *(int *)(n1 + 8) = *(int *)((char *)p + 8) + 1;
            *(struct RideNode **)(n1 + 0x14) = DAT_004d8244;
            DAT_004d8244 = (struct RideNode *)n1;
        }
        if (n2 != 0 && *(short *)(n2 + 2) == param_1 && *(int *)(n2 + 0x18) == 0) {
            *(struct PathNode **)(n2 + 0x18) = p;
            *(int *)(n2 + 8) = *(int *)((char *)p + 8) + 1;
            *(struct RideNode **)(n2 + 0x14) = DAT_004d8244;
            DAT_004d8244 = (struct RideNode *)n2;
        }
        if (n3 != 0 && *(short *)(n3 + 2) == param_1 && *(int *)(n3 + 0x18) == 0) {
            *(struct PathNode **)(n3 + 0x18) = p;
            *(int *)(n3 + 8) = *(int *)((char *)p + 8) + 1;
            *(struct RideNode **)(n3 + 0x14) = DAT_004d8244;
            DAT_004d8244 = (struct RideNode *)n3;
        }
        if (n4 != 0 && *(short *)(n4 + 2) == param_1 && *(int *)(n4 + 0x18) == 0) {
            *(struct PathNode **)(n4 + 0x18) = p;
            *(int *)(n4 + 8) = *(int *)((char *)p + 8) + 1;
            *(struct RideNode **)(n4 + 0x14) = DAT_004d8244;
            DAT_004d8244 = (struct RideNode *)n4;
        }
    }
}
