#include <stdlib.h>
#include <math.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "gamemap.h"
#include "image_sprite.h"
#include "jungle_cruise.h"
#include "llidb.h"
#include "map_object.h"
#include "objclass.h"
#include "render3d.h"

// FUNCTION: LEGOLAND 0x00432ac0
void FUN_00432ac0(void) {
    int *p = &DAT_0081cb80[1];
    int i = 0;
    do {
        double angle = (double)i * (double)DAT_004ab3e8 * (double)DAT_004ab3f4;
        p[-1] = FUN_00458930((float)sin(angle));
        p[0] = FUN_00458930((float)cos(angle));
        angle = (sin(angle) + (double)DAT_004ab3ec - (double)DAT_004ab3fc) * (double)DAT_004ab3f4;
        p[0x1f] = FUN_00458930((float)sin(angle));
        p[0x20] = FUN_00458930((float)cos(angle));
        angle = (cos(angle) + (double)DAT_004ab4a0) * (double)DAT_004ab3f4;
        p[0x3f] = FUN_00458930((float)sin(angle));
        p[0x40] = FUN_00458930((float)cos(angle));
        p = p + 2;
        i = i + 1;
    } while (p < &DAT_0081cb80[0x21]);
}

// FUNCTION: LEGOLAND 0x00432b90
int FUN_00432b90(unsigned int param_1, void *param_2, void *param_3, void *param_4) {
    short id = (short)param_1;
    struct JungleScore *score;
    struct JungleRide *node;
    struct JungleRide *fresh;
    short *fill;
    int i;
    unsigned int r;

    for (score = DAT_00629c3c; node = DAT_00616164, score != NULL && score->field_0 != (unsigned short)id;
         score = score->next) {
    }
    while (node != NULL) {
        if (node->field_0 == (unsigned short)id) {
            if (node->field_4 == score->field_2 && node->field_8 == score->field_3) {
                return 0;
            }
            if (node->field_c == score->field_2 && node->field_10 == score->field_3) {
                return 0;
            }
            if (node->field_3e0 == 1) {
                return 0;
            }
        }
        node = node->next;
    }
    fresh = (struct JungleRide *)malloc(sizeof(struct JungleRide));
    if (fresh == NULL) {
        return 0;
    }
    fresh->next = DAT_00616164;
    i = (param_1 >> 8 & 0xff) + 5;
    fresh->field_0 = id;
    fresh->field_4 = param_1 & 0xff;
    fresh->field_8 = i;
    fresh->field_c = param_1 & 0xff;
    fresh->field_10 = i;
    fresh->field_3dc = 1;
    fresh->field_3e0 = 1;
    r = rand();
    fresh->blokes[2] = (unsigned int)param_4;
    fresh->blokes[0] = (unsigned int)param_2;
    fresh->field_3e4 = (r & 0xf) + 4;
    fresh->blokes[1] = (unsigned int)param_3;
    fill = (short *)fresh->field_1c;
    DAT_00616164 = fresh;
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
    return 1;
}

// FUNCTION: LEGOLAND 0x00432cb0
void FUN_00432cb0(struct JungleRide *param_1) {
    struct JungleRide *prev = NULL;
    struct JungleRide *node = DAT_00616164;
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
    DAT_00616164 = node->next;
    free(param_1);
}

// FUNCTION: LEGOLAND 0x00432d00
void FUN_00432d00(int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x004332c0
unsigned int FUN_004332c0(unsigned short *param_1) {
    struct JungleRide *node = DAT_00616164;
    unsigned int count = 0;
    if (node != NULL) {
        unsigned short *src = param_1;
        while (node != NULL) {
            unsigned short value = *src;
            node->field_0 = value;
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
    while (node != NULL) {
        struct JungleRide *cur = node;
        node->field_4 = node->field_c;
        node->field_8 = node->field_10;
        if (node->field_3e0 == 0x10) {
            cur = FUN_004333e0(node);
            if (cur != node) {
                node = cur;
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
            }
        }
        if (cur == NULL) {
            return;
        }
        node = cur->next;
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
void FUN_004334c0(struct JungleRide *param_1, int param_2) {
    struct JungleScore *score = DAT_00629c3c;
    struct JungleRide *other = DAT_00616164;
    struct JunglePath *path;
    unsigned int mask;
    unsigned int rnd;
    int bit;
    int count;
    int dir;
    unsigned int sel;
    unsigned char d;
    char step;

    path = FUN_004371b0((unsigned char)param_1->field_4, (unsigned char)param_1->field_8);
    if (score != NULL) {
        do {
            if (path->field_2 == score->field_0) {
                break;
            }
            score = score->next;
        } while (score != NULL);
    }
    mask = path->field_4;
    if (*(short *)&path->x == *(short *)&score->field_2) {
        mask = mask & 0xfffffffe;
    } else if (*(short *)&path->x == *(short *)&score->field_4) {
        param_1->field_3e0 = 0x10;
        param_1->field_3e4 = 3;
        FUN_00433840(param_1, param_1->field_3dc, 4);
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
        FUN_00433840(param_1, param_1->field_3dc, 0xffffffff);
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
        FUN_00433840(param_1, param_1->field_3dc, dir);
        param_1->field_3dc = 4;
        break;
    case 2:
        param_1->field_c = param_1->field_4 + 5;
        param_1->field_10 = param_1->field_8;
        FUN_00433840(param_1, param_1->field_3dc, dir);
        param_1->field_3dc = 8;
        break;
    case 4:
        param_1->field_c = param_1->field_4;
        param_1->field_10 = param_1->field_8 + 5;
        FUN_00433840(param_1, param_1->field_3dc, dir);
        param_1->field_3dc = 1;
        break;
    case 8:
        param_1->field_c = param_1->field_4 - 5;
        param_1->field_10 = param_1->field_8;
        FUN_00433840(param_1, param_1->field_3dc, dir);
        param_1->field_3dc = 2;
        break;
    }
    bit = param_1->field_3e4 - 1;
    param_1->field_3e4 = bit;
    if (bit == 0) {
        param_1->field_3e0 = 8;
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
void FUN_00433ca0(struct JungleHolder *param_1) {
    struct Cursor *cursor = param_1->cursor;
    DAT_0081cb70 = cursor;
    cursor->field_1c |= 0x400;
    // STRING: LEGOLAND 0x004b720c
    DAT_0081cb68 = LoadSprite("brijmask.lls", 1);
}

// FUNCTION: LEGOLAND 0x00433cd0
void FUN_00433cd0(void) {
    KillSprite(DAT_0081cb68);
}

// FUNCTION: LEGOLAND 0x00433ce0
void FUN_00433ce0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb70;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(&DAT_0081cb70->field_3c);
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
        obj->field_0 = coord;
        obj->field_2 = (unsigned short)local_4;
        obj->next = DAT_00629c2c;
        DAT_00629c2c = obj;
        AddBasicObject(param_1, (unsigned int)param_2);
        FUN_00436130((unsigned short)local_4, 1);
    }
}

// FUNCTION: LEGOLAND 0x00433d90
void FUN_00433d90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00433fa0
unsigned int FUN_00433fa0(unsigned int param_1, unsigned int param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00433fc0
void FUN_00433fc0(void *param_1, unsigned short param_2, void *param_3) {
    struct JungleObj *node = DAT_00629c2c;
    struct JungleObj *prev = NULL;
    struct JungleObj *nxt;
    StandardRemoveObject((unsigned int)param_1, param_2, (unsigned int)param_3);
    while (node->field_0 != param_2) {
        nxt = node->next;
        if (nxt == NULL) {
            return;
        }
        prev = node;
        node = nxt;
    }
    if (node != NULL) {
        FUN_00436130(node->field_2, 0xffffffff);
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
unsigned int *FUN_00434040(struct JungleHolder *param_1, unsigned short param_2) {
    struct Cursor *cursor = param_1->cursor;
    DAT_0082c6a0 = cursor->field_64;
    DAT_0082c6a4 = cursor->field_14;
    DAT_0082c6a8 = cursor->field_18;
    DAT_0082c6ac = param_2;
    return &DAT_0082c6a0;
}

// FUNCTION: LEGOLAND 0x00434080
void FUN_00434080(struct JungleHolder *param_1) {
    struct Cursor *cursor = param_1->cursor;
    DAT_0081cb74 = cursor;
    cursor->field_1c |= 0x400;
    // STRING: LEGOLAND 0x004b721c
    DAT_0081cb6c = LoadSprite("mfish2.lls", 1);
}

// FUNCTION: LEGOLAND 0x004340b0
void FUN_004340b0(void) {
    KillSprite(DAT_0081cb6c);
}

// FUNCTION: LEGOLAND 0x004340c0
void FUN_004340c0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb74;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(&DAT_0081cb74->field_3c);
}

// FUNCTION: LEGOLAND 0x00434100
void FUN_00434100(unsigned int param_1, int *param_2) {
    struct JungleFish *fish;
    unsigned char temp[2];
    unsigned short coord;
    short local_2;
    int x;
    int y;
    short tile;
    struct Cursor *cursor;

    temp[0] = (unsigned char)param_2[0];
    temp[1] = (unsigned char)param_2[1];
    coord = *(unsigned short *)temp;
    if (FUN_00436fb0(param_2[0], param_2[1], (unsigned short *)&local_2) == 0) {
        FUN_00436fb0(param_2[0], param_2[1] - 5, (unsigned short *)&local_2);
    }
    fish = (struct JungleFish *)malloc(0xc);
    if (fish == NULL) {
        return;
    }
    fish->field_0 = coord;
    fish->field_2 = local_2;
    fish->next = DAT_00629c30;
    fish->field_4 = 0;
    DAT_00629c30 = fish;
    FUN_00436130(fish->field_2, 2);
    AddBasicObject(param_1, (unsigned int)param_2);
    y = DAT_0081cb74->field_3c.v[1];
    cursor = DAT_0081cb74;
    if (y <= DAT_0081cb74->field_3c.v[3]) {
        do {
            x = cursor->field_3c.v[0];
            if (x <= cursor->field_3c.v[2]) {
                do {
                    if (x == cursor->field_3c.v[0]) {
                        tile = DAT_0081cb58->tiles[0] + 9;
                        SetMapTile(param_2[0] + x, param_2[1] + y, tile);
                    } else if (x == cursor->field_3c.v[2]) {
                        tile = DAT_0081cb58->tiles[0] + 0xc;
                        SetMapTile(param_2[0] + x, param_2[1] + y, tile);
                    } else if (y == cursor->field_3c.v[1]) {
                        tile = DAT_0081cb58->tiles[0] + 10;
                        SetMapTile(param_2[0] + x, param_2[1] + y, tile);
                    } else if (y == cursor->field_3c.v[3]) {
                        tile = DAT_0081cb58->tiles[0] + 0xb;
                        SetMapTile(param_2[0] + x, param_2[1] + y, tile);
                    } else {
                        tile = DAT_0081cb58->tiles[0];
                        SetMapTile(param_2[0] + x, param_2[1] + y, tile);
                    }
                    x = x + 1;
                    cursor = DAT_0081cb74;
                } while (x <= DAT_0081cb74->field_3c.v[2]);
            }
            y = y + 1;
        } while (y <= cursor->field_3c.v[3]);
    }
    SetMapTile(cursor->field_3c.v[0] + param_2[0], cursor->field_3c.v[1] + param_2[1], DAT_0081cb58->tiles[0] + 5);
    SetMapTile(DAT_0081cb74->field_3c.v[2] + param_2[0], DAT_0081cb74->field_3c.v[1] + param_2[1], DAT_0081cb58->tiles[0] + 8);
    SetMapTile(DAT_0081cb74->field_3c.v[0] + param_2[0], DAT_0081cb74->field_3c.v[3] + param_2[1], DAT_0081cb58->tiles[0] + 6);
    SetMapTile(DAT_0081cb74->field_3c.v[2] + param_2[0], DAT_0081cb74->field_3c.v[3] + param_2[1], DAT_0081cb58->tiles[0] + 7);
}

// FUNCTION: LEGOLAND 0x00434330
void FUN_00434330(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00434650
unsigned int FUN_00434650(unsigned int param_1, unsigned int param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00434670
void FUN_00434670(void *param_1, unsigned short param_2, struct Cursor *param_3) {
    struct JungleFish *node = DAT_00629c30;
    struct JungleFish *prev = NULL;
    struct JungleFish *nxt;
    int x;
    int y;

    y = DAT_0081cb74->field_3c.v[1];
    if (y <= DAT_0081cb74->field_3c.v[3]) {
        do {
            x = DAT_0081cb74->field_3c.v[0];
            if (x <= DAT_0081cb74->field_3c.v[2]) {
                do {
                    RestoreBaseMap(param_3->field_1404 + x, param_3->field_1408 + y);
                    x = x + 1;
                } while (x <= DAT_0081cb74->field_3c.v[2]);
            }
            y = y + 1;
        } while (y <= DAT_0081cb74->field_3c.v[3]);
    }
    StandardRemoveObject((unsigned int)param_1, param_2, (unsigned int)param_3);
    while (node->field_0 != param_2) {
        nxt = node->next;
        if (nxt == NULL) {
            return;
        }
        prev = node;
        node = nxt;
    }
    if (node != NULL) {
        FUN_00436130(node->field_2, 0xfffffffe);
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
unsigned int *FUN_00434740(struct JungleHolder *param_1, short param_2) {
    struct Cursor *cursor = param_1->cursor;
    struct JungleFish *node = DAT_00629c30;
    short *lls;

    while (node != NULL && node->field_0 != (unsigned short)param_2) {
        node = node->next;
    }

    lls = (short *)GetLLSForSprite((struct SpriteLLS *)cursor->field_64);
    if (node->field_4 != 0) {
        if (*lls == 0) {
            node->field_4 = 0;
        }
    } else if (*lls == 1 && rand() % 7 == 0) {
        node->field_4 = 1;
    }

    if (node->field_4 != 0) {
        LLSSetFrame((struct LLS *)GetLLSForSprite((struct SpriteLLS *)DAT_0081cb6c), *lls);
        DAT_0082c6a0 = (unsigned int)DAT_0081cb6c;
    } else {
        DAT_0082c6a0 = cursor->field_64;
    }
    DAT_0082c6a4 = cursor->field_14;
    DAT_0082c6ac = (unsigned short)param_2;
    DAT_0082c6a8 = cursor->field_18;
    return (unsigned int *)&DAT_0082c6a0;
}

// FUNCTION: LEGOLAND 0x00434b40
void FUN_00434b40(void *param_1, unsigned short param_2, struct Cursor *param_3) {
    struct JungleFish *node = DAT_00629c34;
    struct JungleFish *prev;
    int x;
    int y;
    struct MapElement *tile;

    param_3->field_1414[1] = 0xffffffff;
    param_3->field_1414[3] = 1;
    param_3->field_1414[0] = 0;
    param_3->field_1414[2] = 0;
    StandardRemoveObject((unsigned int)param_1, param_2, (unsigned int)param_3);
    y = param_3->field_1408;
    x = param_3->field_1404 - 6;
    if (x < 0 || (int)(unsigned int)lpConfig->width <= x || y < 0 || (int)(unsigned int)lpConfig->height <= y) {
        tile = NULL;
    } else {
        tile = (struct MapElement *)((char *)GameMap[y] + x * 0x14);
    }
    tile->flags &= 0xffbf;
    y = y - 1;
    if (x < 0 || (int)(unsigned int)lpConfig->width <= x || y < 0 || (int)(unsigned int)lpConfig->height <= y) {
        tile = NULL;
    } else {
        tile = (struct MapElement *)((char *)GameMap[y] + x * 0x14);
    }
    tile->flags &= 0xffbf;
    y = y + 2;
    if (x < 0 || (int)(unsigned int)lpConfig->width <= x || y < 0 || (int)(unsigned int)lpConfig->height <= y) {
        tile = NULL;
    } else {
        tile = (struct MapElement *)((char *)GameMap[y] + x * 0x14);
    }
    tile->flags &= 0xffbf;
    if (node->field_0 == param_2) {
        prev = NULL;
    } else {
        do {
            prev = node;
            node = node->next;
            if (node == NULL) {
                return;
            }
        } while (node->field_0 != param_2);
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
void FUN_00434cb0(struct JungleHolder *param_1) {
    void *handle;
    unsigned int i;
    int sprite;
    unsigned int lls;

    DAT_0081cb60 = param_1->cursor;
    DAT_0081cb60->field_1c |= 0x20;
    *(unsigned int *)((char *)DAT_0081cb60->field_64 + 0x10) |= 0x2000;
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", (unsigned int *)&handle, 0) == 0) {
        DAT_0081cb58 = (struct JungleTileMap *)LLIDB_LoadData(handle);
    }
    if (LLIDB_FindElement("JUNGLE CRUISE BOATS", (unsigned int *)&handle, 0) == 0) {
        DAT_0081cd00 = LLIDB_LoadData(handle);
    }
    i = 0;
    if (0 < *(int *)((char *)DAT_0081cd00 + 4)) {
        do {
            sprite = *(int *)(*(int *)((char *)DAT_0081cd00 + 8) + (i & 0xff) * 4);
            lls = GetLLSForSprite((struct SpriteLLS *)sprite);
            LLSPlay((struct LLS *)lls, *(unsigned int *)(sprite + 8));
            i = i + 1;
        } while ((int)i < *(int *)((char *)DAT_0081cd00 + 4));
    }
    // STRING: LEGOLAND 0x004b72b8
    DAT_0081cb5c = LoadSprite("jungmask.lls", 1);
    *(struct Footprint *)DAT_00629c40 = DAT_0081cb60->field_3c;
    *(struct Footprint *)DAT_004b7260 = DAT_004b7230;
    DAT_004b7260[1] = DAT_004b7230.v[1] + DAT_00629c40[1];
    DAT_004b7260[3] += DAT_00629c40[1];
    DAT_004b7260[0] += DAT_00629c40[0];
    DAT_004b7260[2] += DAT_00629c40[0];
    *(struct Footprint *)DAT_004b7278 = DAT_004b7248;
    DAT_004b7278[0] += DAT_00629c40[0];
    DAT_004b7278[1] = DAT_004b7248.v[1] + 1 + DAT_00629c40[3];
    DAT_004b7278[2] += DAT_00629c40[0];
    DAT_004b7278[3] += DAT_00629c40[3];
    FUN_00432ac0();
}

// FUNCTION: LEGOLAND 0x00434e50
void FUN_00434e50(struct JungleHolder *param_1) {
    void *handle;
    unsigned int i;
    struct JungleScore *score;
    struct JunglePath *path;

    DAT_0081cb60 = param_1->cursor;
    // STRING: LEGOLAND 0x004b5334
    if (LLIDB_FindElement("BOATING SCHOOL TILE MAPPING", (unsigned int *)&handle, 0) == 0) {
        LLIDB_UnLoadData((unsigned int)handle);
    }
    i = 0;
    if (0 < *(int *)((char *)DAT_0081cd00 + 4)) {
        do {
            LLSStop(GetLLSForSprite(*(struct SpriteLLS **)(*(int *)((char *)DAT_0081cd00 + 8) + (i & 0xff) * 4)));
            i = i + 1;
        } while ((int)i < *(int *)((char *)DAT_0081cd00 + 4));
    }
    // STRING: LEGOLAND 0x004b72c8
    if (LLIDB_FindElement("JUNGLE CRUISE BOATS", (unsigned int *)&handle, 0) == 0) {
        LLIDB_UnLoadData((unsigned int)handle);
    }
    while (DAT_00629c3c != NULL) {
        score = DAT_00629c3c->next;
        free(DAT_00629c3c);
        DAT_00629c3c = score;
    }
    DAT_00629c3c = NULL;
    while (DAT_00616164 != NULL) {
        FUN_00432cb0(DAT_00616164);
    }
    DAT_00616164 = NULL;
    while (DAT_0062fd2c != NULL) {
        path = DAT_0062fd2c->next;
        free(DAT_0062fd2c);
        DAT_0062fd2c = path;
    }
    KillSprite(DAT_0081cb5c);
}

// FUNCTION: LEGOLAND 0x00434f50
void FUN_00434f50(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb60;
    DefaultCursor(&EditCursor);
    DAT_00629c50 = &DAT_004b7278;
    DAT_004b7288 = &DAT_004b7260;
    SetEditCursorFootPrint(&DAT_00629c40);
}

// FUNCTION: LEGOLAND 0x00434f90
void FUN_00434f90(unsigned int param_1, int *param_2) {
    struct JungleScore *score;
    unsigned char temp[2];
    int x;
    int y;
    int tile;
    int i;

    temp[0] = (unsigned char)param_2[0];
    temp[1] = (unsigned char)param_2[1];
    score = (struct JungleScore *)malloc(0x44);
    if (score == NULL) {
        return;
    }
    score->field_0 = *(unsigned short *)temp;
    score->field_2 = (unsigned char)param_2[0] + (char)DAT_004b7278[0] + 2;
    score->field_3 = (unsigned char)param_2[1] + (char)DAT_004b7278[1] + 2;
    score->field_4 = (unsigned char)param_2[0] + (char)DAT_004b7260[0] + 2;
    score->field_8 = 0;
    score->field_c = 9999;
    score->field_10 = 0;
    score->field_5 = (unsigned char)param_2[1] + (char)DAT_004b7260[1] + 2;
    score->field_14 = 0;
    score->field_2c = 0x96;
    score->field_40 = 3;
    score->blokes[0] = 0;
    score->blokes[1] = 0;
    score->blokes[2] = 0;
    score->blokes[3] = 0;
    score->blokes[4] = 0;
    score->field_30[0] = 0;
    score->field_30[1] = 0;
    score->next = DAT_00629c3c;
    DAT_00629c3c = score;
    AddBasicObject(param_1, (unsigned int)param_2);
    FUN_00436dc0(param_2[0] + 2 + DAT_004b7278[0], param_2[1] + 2 + DAT_004b7278[1], 1, &score->field_0);
    FUN_00436dc0(param_2[0] + 2 + DAT_004b7260[0], param_2[1] + 2 + DAT_004b7260[1], 4, &score->field_0);
    y = DAT_00629c40[1];
    if (DAT_00629c40[1] <= DAT_00629c40[3]) {
        do {
            i = DAT_00629c40[2] - 1;
            x = DAT_00629c40[0];
            if (DAT_00629c40[0] <= i) {
                do {
                    if (x == DAT_00629c40[0]) {
                        tile = DAT_0081cb58->tiles[0] + 9;
                    } else if (x == i) {
                        tile = DAT_0081cb58->tiles[0] + 0xc;
                    } else {
                        tile = DAT_0081cb58->tiles[0];
                    }
                    SetMapTile(param_2[0] + x, param_2[1] + y, tile);
                    x = x + 1;
                    i = DAT_00629c40[2] - 1;
                } while (x <= i);
            }
            y = y + 1;
        } while (y <= DAT_00629c40[3]);
    }
}

// FUNCTION: LEGOLAND 0x00435150
void FUN_00435150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435230
void FUN_00435230(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435470
void FUN_00435470(void *param_1, unsigned int param_2, struct Cursor *param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x00435750
void FUN_00435750(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435bd0
void FUN_00435bd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435c70
int FUN_00435c70(void) {
    struct JungleScore *score;
    struct JungleObj *obj;
    struct JungleFish *fish;
    struct JunglePath *path;
    struct JungleRide *ride;
    struct JungleScore scoreCopy;
    struct JungleRide rideCopy;
    int count;
    int i;
    unsigned int *blokes;

    score = DAT_00629c3c;
    count = 0;
    for (; score != NULL; score = score->next) {
        count = count + 1;
    }
    SaveGameWrite(&count, 4);
    score = DAT_00629c3c;
    path = DAT_0062fd2c;
    while (count-- != 0) {
        scoreCopy = *score;
        blokes = scoreCopy.blokes;
        i = 5;
        do {
            *blokes = GetBlokeNum(*blokes);
            blokes = blokes + 1;
            i = i - 1;
        } while (i != 0);
        SaveGameWrite(&scoreCopy, 0x44);
        score = score->next;
        path = DAT_0062fd2c;
    }
    count = 0;
    for (; path != NULL; path = path->next) {
        count = count + 1;
    }
    SaveGameWrite(&count, 4);
    path = DAT_0062fd2c;
    fish = DAT_00629c30;
    while (count-- != 0) {
        SaveGameWrite(path, 0x1c);
        path = path->next;
        fish = DAT_00629c30;
    }
    count = 0;
    for (; fish != NULL; fish = fish->next) {
        count = count + 1;
    }
    SaveGameWrite(&count, 4);
    fish = DAT_00629c30;
    obj = DAT_00629c2c;
    while (count-- != 0) {
        SaveGameWrite(fish, 0xc);
        fish = fish->next;
        obj = DAT_00629c2c;
    }
    count = 0;
    for (; obj != NULL; obj = obj->next) {
        count = count + 1;
    }
    SaveGameWrite(&count, 4);
    obj = DAT_00629c2c;
    ride = DAT_00616164;
    while (count-- != 0) {
        SaveGameWrite(obj, 8);
        obj = obj->next;
        ride = DAT_00616164;
    }
    count = 0;
    for (; ride != NULL; ride = ride->next) {
        count = count + 1;
    }
    SaveGameWrite(&count, 4);
    ride = DAT_00616164;
    while (count-- != 0) {
        rideCopy = *ride;
        blokes = rideCopy.blokes;
        i = 3;
        do {
            *blokes = GetBlokeNum(*blokes);
            blokes = blokes + 1;
            i = i - 1;
        } while (i != 0);
        SaveGameWrite(&rideCopy, 0x3f8);
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
    struct JungleObj *obj;
    struct JungleObj *prevObj;
    struct JungleRide *ride;
    struct JungleRide *prevRide;
    unsigned int *blokes;
    int count;
    int i;

    prevScore = NULL;
    SaveGameRead(&count, 4);
    while (count-- != 0) {
        if (prevScore == NULL) {
            score = (struct JungleScore *)malloc(0x44);
            DAT_00629c3c = score;
        } else {
            score = (struct JungleScore *)malloc(0x44);
            prevScore->next = score;
        }
        SaveGameRead(score, 0x44);
        blokes = score->blokes;
        i = 5;
        do {
            *blokes = GetBlokePtr(*blokes);
            blokes = blokes + 1;
            i = i - 1;
        } while (i != 0);
        prevScore = score;
    }
    SaveGameRead(&count, 4);
    prevPath = NULL;
    while (count-- != 0) {
        if (prevPath == NULL) {
            path = (struct JunglePath *)malloc(0x1c);
            DAT_0062fd2c = path;
        } else {
            path = (struct JunglePath *)malloc(0x1c);
            prevPath->next = path;
        }
        SaveGameRead(path, 0x1c);
        prevPath = path;
    }
    SaveGameRead(&count, 4);
    prevFish = NULL;
    while (count-- != 0) {
        if (prevFish == NULL) {
            fish = (struct JungleFish *)malloc(0xc);
            DAT_00629c30 = fish;
        } else {
            fish = (struct JungleFish *)malloc(0xc);
            prevFish->next = fish;
        }
        SaveGameRead(fish, 0xc);
        prevFish = fish;
    }
    SaveGameRead(&count, 4);
    prevObj = NULL;
    while (count-- != 0) {
        if (prevObj == NULL) {
            obj = (struct JungleObj *)malloc(8);
            DAT_00629c2c = obj;
        } else {
            obj = (struct JungleObj *)malloc(8);
            prevObj->next = obj;
        }
        SaveGameRead(obj, 8);
        prevObj = obj;
    }
    SaveGameRead(&count, 4);
    prevRide = NULL;
    while (count-- != 0) {
        if (prevRide == NULL) {
            ride = (struct JungleRide *)malloc(sizeof(struct JungleRide));
            DAT_00616164 = ride;
        } else {
            ride = (struct JungleRide *)malloc(sizeof(struct JungleRide));
            prevRide->next = ride;
        }
        SaveGameRead(ride, 0x3f8);
        blokes = ride->blokes;
        i = 3;
        do {
            *blokes = GetBlokePtr(*blokes);
            blokes = blokes + 1;
            i = i - 1;
        } while (i != 0);
        prevRide = ride;
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
void FUN_00436160(unsigned int param_1, int param_2) {
    struct JungleScore *node;
    int best = 0;
    for (node = DAT_00629c3c; node != NULL; node = node->next) {
        if (best < (int)node->field_40 && (param_2 == 0 || node->field_8 != 0)) {
            best = node->field_40;
        }
    }
}

// FUNCTION: LEGOLAND 0x00436190
void FUN_00436190(struct JungleHolder *param_1) {
    DAT_0081cb54 = param_1->cursor;
}

// FUNCTION: LEGOLAND 0x004361a0
void FUN_004361a0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb54;
    DAT_0081cb54->field_3c = DAT_004b7478;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00436200
void FUN_00436200(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00436470
void FUN_00436470(unsigned int param_1, unsigned int *param_2) {
    struct JungleScore *score = DAT_00629c3c;
    struct JungleRide *ride;
    struct MapElement *tile;
    struct JunglePath *path;
    unsigned char bx;
    unsigned char by;
    unsigned int x;
    unsigned int y;
    unsigned char local_14[12];
    struct Cursor *local_8;

    x = param_2[0];
    y = param_2[1];
    if ((int)x < 0 || (int)(unsigned int)lpConfig->width <= (int)x || (int)y < 0 || (int)(unsigned int)lpConfig->height <= (int)y) {
        tile = NULL;
    } else {
        tile = (struct MapElement *)((char *)GameMap[y] + x * 0x14);
    }
    bx = ((unsigned char *)tile)[4];
    param_2[0] = bx;
    by = ((unsigned char *)tile)[5];
    param_2[1] = by;
    ride = DAT_00616164;
    if (score != NULL) {
        do {
            if ((short)((bx) | (by << 8)) == *(short *)&score->field_2 || (short)((bx) | (by << 8)) == *(short *)&score->field_4) {
                path = FUN_004371b0(param_2[0], by);
                QueryObj = path->field_2;
                param_2[0] = (unsigned char)QueryObj;
                param_2[1] = (unsigned char)(QueryObj >> 8);
                *(struct Footprint *)((char *)QueryClass + 0x3c) = DAT_004b7478;
                local_8 = DAT_0081cb60;
                FUN_00435230(local_14, param_2);
                return;
            }
            score = score->next;
        } while (score != NULL);
    }
    *(struct Footprint *)((char *)QueryClass + 0x3c) = DAT_004b7478;
    BasicObjectDCalcCursor(param_1, (unsigned int)param_2);
    if (ride != NULL) {
        x = (unsigned char)((bx) | (by << 8)) & 0xff;
        while ((x != ride->field_4 || (unsigned int)by != ride->field_8) &&
               (x != ride->field_c || (unsigned int)by != ride->field_10)) {
            ride = ride->next;
            if (ride == NULL) {
                return;
            }
        }
        FUN_0045f480(&QueryCursor, 1);
    }
}

// FUNCTION: LEGOLAND 0x004365f0
void FUN_004365f0(unsigned int param_1, int *param_2) {
    struct JungleScore *score = DAT_00629c3c;
    unsigned int mask;
    unsigned short coord;
    int result;

    mask = FUN_00436fb0(param_2[0], param_2[1], (unsigned short *)&coord);
    FUN_00436dc0(param_2[0], param_2[1], mask, &coord);
    IncrementObjectCount(*(unsigned int *)(param_1 + 0xc));
    FUN_004367b0(param_2[0], param_2[1], &coord);
    if ((mask & 1) != 0) {
        result = FUN_00436fb0(param_2[0], param_2[1] - 5, &coord);
        FUN_00436dc0(param_2[0], param_2[1] - 5, result, &coord);
        FUN_004367b0(param_2[0], param_2[1] - 5, &coord);
    }
    if ((mask & 2) != 0) {
        result = FUN_00436fb0(param_2[0] + 5, param_2[1], &coord);
        FUN_00436dc0(param_2[0] + 5, param_2[1], result, &coord);
        FUN_004367b0(param_2[0] + 5, param_2[1], &coord);
    }
    if ((mask & 4) != 0) {
        result = FUN_00436fb0(param_2[0], param_2[1] + 5, &coord);
        FUN_00436dc0(param_2[0], param_2[1] + 5, result, &coord);
        FUN_004367b0(param_2[0], param_2[1] + 5, &coord);
    }
    if ((mask & 8) != 0) {
        result = FUN_00436fb0(param_2[0] - 5, param_2[1], &coord);
        FUN_00436dc0(param_2[0] - 5, param_2[1], result, &coord);
        FUN_004367b0(param_2[0] - 5, param_2[1], &coord);
    }
    if (score != NULL) {
        while (score->field_0 != coord) {
            score = score->next;
            if (score == NULL) {
                return;
            }
        }
        result = FUN_004371e0(score->field_2, score->field_3, score->field_4, score->field_5);
        score->field_8 = result;
        if (result != 0) {
            FUN_004373c0(coord);
        }
    }
}

// FUNCTION: LEGOLAND 0x004367b0
void FUN_004367b0(int param_1, int param_2, unsigned short *param_3) {
    struct JungleScore *score = DAT_00629c3c;
    struct JunglePath *p;
    struct JunglePath *adj;
    unsigned int mask;

    p = FUN_004371b0(param_1, param_2);
    if (score != NULL) {
        do {
            if (score->field_0 == *param_3) {
                break;
            }
            score = score->next;
        } while (score != NULL);
    }
    if (p == NULL) {
        return;
    }
    mask = p->field_4;
    if (*(short *)&p->x == *(short *)&score->field_2) {
        mask = mask & 0xfffffffe;
    } else if (*(short *)&p->x == *(short *)&score->field_4) {
        mask = mask & 0xfffffffb;
    }
    if ((mask & 8) != 0 && (mask & 1) != 0 && (adj = FUN_004371b0(param_1, param_2 - 5)) != NULL && (adj->field_4 & 8) != 0) {
        SetMapTile(param_1 - 3, param_2 - 3, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 - 2, param_2 - 3, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 - 3, param_2 - 2, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 - 2, param_2 - 2, DAT_0081cb58->tiles[0]);
    }
    if ((mask & 8) != 0 && (mask & 4) != 0 && (adj = FUN_004371b0(param_1, param_2 + 5)) != NULL && (adj->field_4 & 8) != 0) {
        SetMapTile(param_1 - 3, param_2 + 3, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 - 2, param_2 + 3, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 - 3, param_2 + 2, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 - 2, param_2 + 2, DAT_0081cb58->tiles[0]);
    }
    if ((mask & 2) != 0 && (mask & 1) != 0 && (adj = FUN_004371b0(param_1, param_2 - 5)) != NULL && (adj->field_4 & 2) != 0) {
        SetMapTile(param_1 + 3, param_2 - 3, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 + 2, param_2 - 3, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 + 3, param_2 - 2, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 + 2, param_2 - 2, DAT_0081cb58->tiles[0]);
    }
    if ((mask & 2) != 0 && (mask & 4) != 0 && (adj = FUN_004371b0(param_1, param_2 + 5)) != NULL && (adj->field_4 & 2) != 0) {
        SetMapTile(param_1 + 3, param_2 + 3, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 + 2, param_2 + 3, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 + 3, param_2 + 2, DAT_0081cb58->tiles[0]);
        SetMapTile(param_1 + 2, param_2 + 2, DAT_0081cb58->tiles[0]);
    }
}

// FUNCTION: LEGOLAND 0x00436a40
void FUN_00436a40(void *param_1, unsigned int param_2, struct Cursor *param_3) {
    struct JungleScore *score = DAT_00629c3c;
    struct JungleScore *cur = DAT_00629c3c;
    struct MapElement *tile;
    unsigned int mask;
    unsigned short coord;
    unsigned int result;
    int x;
    int y;
    int yc;

    yc = (param_2 >> 8) & 0xff;
    if ((param_2 & 0xff) < (unsigned int)lpConfig->width && (unsigned int)yc < (unsigned int)lpConfig->height) {
        tile = (struct MapElement *)((char *)GameMap[yc] + (param_2 & 0xff) * 0x14);
    } else {
        tile = NULL;
    }
    if (tile->field_0 != DAT_0081cb54->field_c4) {
        FUN_00435470(param_1, param_2, param_3);
        return;
    }
    mask = FUN_00436fb0(param_3->field_1404, param_3->field_1408, &coord);
    FUN_00436f30(param_1, (unsigned short)param_2, param_3);
    if ((mask & 1) != 0) {
        x = param_3->field_1404;
        y = param_3->field_1408 - 5;
        result = FUN_00436fb0(x, y, &coord);
        FUN_00436dc0(x, y, result, &coord);
        FUN_004367b0(x, y, &coord);
    }
    if ((mask & 2) != 0) {
        y = param_3->field_1408;
        x = param_3->field_1404 + 5;
        result = FUN_00436fb0(x, y, &coord);
        FUN_00436dc0(x, y, result, &coord);
        FUN_004367b0(x, y, &coord);
    }
    if ((mask & 4) != 0) {
        x = param_3->field_1404;
        y = param_3->field_1408 + 5;
        result = FUN_00436fb0(x, y, &coord);
        FUN_00436dc0(x, y, result, &coord);
        FUN_004367b0(x, y, &coord);
    }
    if ((mask & 8) != 0) {
        y = param_3->field_1408;
        x = param_3->field_1404 - 5;
        result = FUN_00436fb0(x, y, &coord);
        FUN_00436dc0(x, y, result, &coord);
        FUN_004367b0(x, y, &coord);
    }
    if ((mask & 1) != 0) {
        if ((mask & 8) != 0) {
            y = param_3->field_1408 - 5;
            x = param_3->field_1404 - 5;
            if (FUN_004371b0(x, y) != NULL) {
                result = FUN_00436fb0(x, y, &coord);
                FUN_00436dc0(x, y, result, &coord);
                FUN_004367b0(x, y, &coord);
            }
        }
        if ((mask & 2) != 0) {
            y = param_3->field_1408 - 5;
            x = param_3->field_1404 + 5;
            if (FUN_004371b0(x, y) != NULL) {
                result = FUN_00436fb0(x, y, &coord);
                FUN_00436dc0(x, y, result, &coord);
                FUN_004367b0(x, y, &coord);
            }
        }
    }
    if ((mask & 4) != 0 && (mask & 8) != 0) {
        y = param_3->field_1408 + 5;
        x = param_3->field_1404 - 5;
        if (FUN_004371b0(x, y) != NULL) {
            result = FUN_00436fb0(x, y, &coord);
            FUN_00436dc0(x, y, result, &coord);
            FUN_004367b0(x, y, &coord);
        }
    }
    if ((mask & 4) != 0 && (mask & 2) != 0) {
        y = param_3->field_1408 + 5;
        x = param_3->field_1404 + 5;
        if (FUN_004371b0(x, y) != NULL) {
            result = FUN_00436fb0(x, y, &coord);
            FUN_00436dc0(x, y, result, &coord);
            FUN_004367b0(x, y, &coord);
        }
    }
    FUN_004373c0(coord);
    if (score != NULL) {
        while (cur->field_0 != coord) {
            cur = cur->next;
            if (cur == NULL) {
                return;
            }
        }
        result = FUN_004371e0(cur->field_2, cur->field_3, cur->field_4, cur->field_5);
        cur->field_8 = result;
    }
}

// FUNCTION: LEGOLAND 0x00436dc0
void FUN_00436dc0(int param_1, int param_2, int param_3, unsigned short *param_4) {
    struct JunglePath *path;
    struct MapElement *tile;
    unsigned char x;
    unsigned char y;
    unsigned char *row;
    int j;
    int ty;
    int tx;
    unsigned short coord;

    coord = (unsigned short)((param_1 & 0xff) | (param_2 << 8));
    path = FUN_004371b0(param_1, param_2);
    if (path == NULL) {
        path = (struct JunglePath *)malloc(sizeof(struct JunglePath));
        if (path == NULL) {
            return;
        }
        path->field_c = 0;
        path->next = DAT_0062fd2c;
        DAT_0062fd2c = path;
        FUN_00436130(*param_4, 1);
    }
    *(unsigned short *)&path->x = coord;
    path->field_4 = param_3;
    if (param_4 != NULL) {
        path->field_2 = *param_4;
    }
    BGFullUpdate = 1;
    row = &DAT_004b72e4[param_3 * 0x19];
    j = 0;
    do {
        ty = param_2 - 2 + j;
        tx = param_1 - 2;
        do {
            if (tx < 0 || (int)(unsigned int)lpConfig->width <= tx || ty < 0 || (int)(unsigned int)lpConfig->height <= ty) {
                tile = NULL;
            } else {
                tile = &GameMap[ty][tx];
            }
            tile->flags = 8;
            tile->field_10 = 2;
            tile->field_0 = DAT_0081cb54->field_c4;
            tile->field_4 = coord;
            SetMapTile(tx, ty, DAT_0081cb58->tiles[0] + (unsigned short)*row);
            tx = tx + 1;
            row = row + 1;
        } while ((2 - param_1) + tx < 5);
        j = j + 1;
    } while (j < 5);
}

// FUNCTION: LEGOLAND 0x00436f30
void FUN_00436f30(void *param_1, unsigned short param_2, struct Cursor *param_3) {
    struct JunglePath *node = DAT_0062fd2c;
    struct JunglePath *prev = NULL;
    struct JunglePath *nxt;
    StandardRemoveObject((unsigned int)param_1, param_2, (unsigned int)param_3);
    while (*(unsigned short *)&node->x != param_2) {
        nxt = node->next;
        if (nxt == NULL) {
            return;
        }
        prev = node;
        node = nxt;
    }
    if (node != NULL) {
        FUN_00436130(node->field_2, 0xffffffff);
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
unsigned int FUN_00436fb0(int param_1, int param_2, unsigned short *param_3) {
    struct JungleScore *score = DAT_00629c3c;
    struct JunglePath *p;
    unsigned int result = 0;
    int found;
    short id;

    p = FUN_004371b0(param_1, param_2);
    found = p != NULL;
    if (found) {
        *param_3 = p->field_2;
    }
    if (param_1 >= 0 && param_2 - 5 >= 0 && param_1 < lpConfig->width && param_2 - 5 < lpConfig->height &&
        (p = FUN_004371b0(param_1, param_2 - 5)) != NULL) {
        if (found) {
            if (p->field_2 == *param_3) {
                result = 1;
            }
        } else {
            result = 1;
            *param_3 = p->field_2;
            found = 1;
        }
    }
    if (param_1 + 5 >= 0 && param_2 >= 0 && param_1 + 5 < lpConfig->width && param_2 < lpConfig->height &&
        (p = FUN_004371b0(param_1 + 5, param_2)) != NULL) {
        if (found) {
            if (p->field_2 == *param_3) {
                result = result | 2;
            }
        } else {
            result = result | 2;
            *param_3 = p->field_2;
            found = 1;
        }
    }
    if (param_1 >= 0 && param_2 + 5 >= 0 && param_1 < lpConfig->width && param_2 + 5 < lpConfig->height &&
        (p = FUN_004371b0(param_1, param_2 + 5)) != NULL) {
        if (found) {
            if (p->field_2 == *param_3) {
                result = result | 4;
            }
        } else {
            result = result | 4;
            *param_3 = p->field_2;
            found = 1;
        }
    }
    if (param_1 - 5 >= 0 && param_2 >= 0 && param_1 - 5 < lpConfig->width && param_2 < lpConfig->height &&
        (p = FUN_004371b0(param_1 - 5, param_2)) != NULL) {
        if (found) {
            if (p->field_2 == *param_3) {
                result = result | 8;
            }
        } else {
            result = result | 8;
            *param_3 = p->field_2;
        }
    }
    id = (short)((param_1 & 0xff) | (param_2 << 8));
    if (score != NULL) {
        while (1) {
            if (id == *(short *)&score->field_2) {
                return result | 1;
            }
            if (id == *(short *)&score->field_4) {
                break;
            }
            score = score->next;
            if (score == NULL) {
                return result;
            }
        }
        result = result | 4;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x004371b0
struct JunglePath *FUN_004371b0(unsigned char param_1, unsigned char param_2) {
    struct JunglePath *entry;
    unsigned short combined;
    unsigned char temp[2];

    temp[0] = param_1;
    temp[1] = param_2;
    combined = *(unsigned short *)temp;

    entry = DAT_0062fd2c;
    if (entry != NULL) {
        while (1) {
            if (*(unsigned short *)&entry->x == combined) {
                break;
            }
            entry = entry->next;
            if (entry == NULL) {
                break;
            }
        }
    }
    return entry;
}

// FUNCTION: LEGOLAND 0x004371e0
unsigned int FUN_004371e0(unsigned char param_1, unsigned char param_2, unsigned char param_3, unsigned char param_4) {
    struct JunglePath *path;
    int local_4;
    unsigned short local_6;

    local_4 = 0;
    for (path = DAT_0062fd2c; path != NULL; path = path->next) {
        path->field_c = 0;
    }
    path = FUN_004371b0(param_1, param_2);
    if (path == NULL) {
        return 0;
    }
    local_6 = path->field_2;
    FUN_00437260(param_1, param_2, param_3, param_4, &local_6, &local_4);
    return local_4;
}

// FUNCTION: LEGOLAND 0x00437260
void FUN_00437260(int param_1, int param_2, int param_3, int param_4, unsigned short *param_5, int *param_6) {
    struct JunglePath *cur;
    struct JunglePath *adj;

    if (*param_6 == 1) {
        return;
    }
    while (1) {
        cur = FUN_004371b0(param_1, param_2);
        if (cur == NULL) {
            return;
        }
        if (cur->field_2 != *param_5) {
            return;
        }
        if (param_1 == param_3 && param_2 == param_4) {
            break;
        }
        cur->field_c = 1;
        if ((cur->field_4 & 1) != 0 && (adj = FUN_004371b0(param_1, param_2 - 5)) != NULL && adj->field_c == 0) {
            FUN_00437260(param_1, param_2 - 5, param_3, param_4, param_5, param_6);
        }
        if ((cur->field_4 & 2) != 0 && (adj = FUN_004371b0(param_1 + 5, param_2)) != NULL && adj->field_c == 0) {
            FUN_00437260(param_1 + 5, param_2, param_3, param_4, param_5, param_6);
        }
        if ((cur->field_4 & 4) != 0 && (adj = FUN_004371b0(param_1, param_2 + 5)) != NULL && adj->field_c == 0) {
            FUN_00437260(param_1, param_2 + 5, param_3, param_4, param_5, param_6);
        }
        if ((cur->field_4 & 8) == 0) {
            return;
        }
        param_1 = param_1 - 5;
        cur = FUN_004371b0(param_1, param_2);
        if (cur == NULL) {
            return;
        }
        if (cur->field_c != 0) {
            return;
        }
        if (*param_6 == 1) {
            return;
        }
    }
    *param_6 = 1;
}

// FUNCTION: LEGOLAND 0x004373c0
void FUN_004373c0(unsigned short param_1) {
    struct JungleScore *score = DAT_00629c3c;
    struct JunglePath *path;
    int again;

    for (path = DAT_0062fd2c; path != NULL; path = path->next) {
        if (path->field_2 == param_1) {
            path->field_18 = NULL;
        }
    }
    while (score != NULL && score->field_0 != param_1) {
        score = score->next;
    }
    DAT_0062fd30 = FUN_004371b0(score->field_4, score->field_5);
    DAT_0062fd30->field_8 = 0;
    DAT_0062fd30->field_14 = NULL;
    do {
        DAT_0062fd34 = NULL;
        FUN_00437440(param_1);
        again = DAT_0062fd34 != NULL;
        DAT_0062fd30 = DAT_0062fd34;
        DAT_0062fd34 = NULL;
    } while (again);
}

// FUNCTION: LEGOLAND 0x00437440
void FUN_00437440(short param_1) {
    struct JunglePath *cur;
    struct JunglePath *a0;
    struct JunglePath *a1;
    struct JunglePath *a2;
    struct JunglePath *a3;

    for (cur = DAT_0062fd30; cur != NULL; cur = cur->field_14) {
        a0 = FUN_004371b0(cur->x, cur->y - 5);
        a1 = FUN_004371b0(cur->x + 5, cur->y);
        a2 = FUN_004371b0(cur->x, cur->y + 5);
        a3 = FUN_004371b0(cur->x - 5, cur->y);
        if (a0 != NULL && a0->field_2 == (unsigned short)param_1 && a0->field_18 == NULL) {
            a0->field_18 = cur;
            a0->field_8 = cur->field_8 + 1;
            a0->field_14 = DAT_0062fd34;
            DAT_0062fd34 = a0;
        }
        if (a1 != NULL && a1->field_2 == (unsigned short)param_1 && a1->field_18 == NULL) {
            a1->field_18 = cur;
            a1->field_8 = cur->field_8 + 1;
            a1->field_14 = DAT_0062fd34;
            DAT_0062fd34 = a1;
        }
        if (a2 != NULL && a2->field_2 == (unsigned short)param_1 && a2->field_18 == NULL) {
            a2->field_18 = cur;
            a2->field_8 = cur->field_8 + 1;
            a2->field_14 = DAT_0062fd34;
            DAT_0062fd34 = a2;
        }
        if (a3 != NULL && a3->field_2 == (unsigned short)param_1 && a3->field_18 == NULL) {
            a3->field_18 = cur;
            a3->field_8 = cur->field_8 + 1;
            a3->field_14 = DAT_0062fd34;
            DAT_0062fd34 = a3;
        }
    }
}

// FUNCTION: LEGOLAND 0x00437570
int FUN_00437570(struct JungleScore *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4) {
    struct Bloke *bloke;
    unsigned int v;
    int r;
    int result;

    bloke = (struct Bloke *)param_1->field_8;
    if (bloke->field_58 == 0) {
        bloke->param_action = bloke->param_action + 1;
    }
    v = bloke->field_58;
    bloke->field_58 = v - 1;
    result = (int)bloke;
    if ((v & 0x1f) == 0) {
        r = rand();
        result = r / 100;
        if (r % 100 < 0x1f) {
            BuyItem(param_2, param_3, param_4);
            bloke = (struct Bloke *)param_1->field_8;
            result = (int)bloke;
            bloke->param_action = bloke->param_action + 1;
        }
    }
    return result;
}
