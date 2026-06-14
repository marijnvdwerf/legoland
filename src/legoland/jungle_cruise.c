#include <stdlib.h>
#include "globals.h"
#include "legoland.h"

#include "gamemap.h"
#include "image_sprite.h"
#include "jungle_cruise.h"
#include "llidb.h"
#include "map_object.h"
#include "objclass.h"
#include "render3d.h"

// FUNCTION: LEGOLAND 0x00432ac0
void FUN_00432ac0(void) { STUB(); }

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
void FUN_004334c0(struct JungleRide *param_1, int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x00433840
void FUN_00433840(struct JungleRide *param_1, unsigned int param_2, unsigned int param_3) { STUB(); }

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
void FUN_00433d20(void) { STUB(); }

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
void FUN_00434100(void) { STUB(); }

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
void FUN_00434cb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00434e50
void FUN_00434e50(void) { STUB(); }

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
void FUN_00434f90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435150
void FUN_00435150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435230
void FUN_00435230(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435470
void FUN_00435470(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435750
void FUN_00435750(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435bd0
void FUN_00435bd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435c70
void FUN_00435c70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435ec0
void FUN_00435ec0(void) { STUB(); }

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
void FUN_00436470(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004365f0
void FUN_004365f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004367b0
void FUN_004367b0(int param_1, int param_2, unsigned short *param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x00436a40
void FUN_00436a40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00436dc0
void FUN_00436dc0(int param_1, int param_2, int param_3, unsigned short *param_4) { STUB(); }

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
void FUN_00437570(void) { STUB(); }
