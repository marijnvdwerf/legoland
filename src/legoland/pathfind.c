#include "legoland.h"
#include "crt.h"

#include "pathfind.h"
#include "objclass.h"
#include "llidb.h"
#include "globals.h"

struct BestNode {
    struct BestNode *next;
    unsigned int pad_4;
    unsigned int x_min;
    unsigned int y_min;
    unsigned int x_max;
    unsigned int y_max;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
};

struct PathQuery {
    int x;
    int y;
};

struct PathBox {
    unsigned char pad_0[8];
    int x_min;
    int y_min;
    int x_max;
    int y_max;
    unsigned char pad_18[4];
    int dist_sq;
};

struct DirNode {
    struct DirNode *next;
    unsigned char pad_4[4];
    unsigned int x;
    unsigned int y;
};


// FUNCTION: LEGOLAND 0x00481c50
void AddPathSquare(struct InstancePos *pos) {
    struct BestNode *node;
    unsigned int x;
    unsigned int y;

    if (FUN_00481790(pos)) {
        return;
    }

    node = FUN_00481730();
    x = pos->x;
    node->x_max = x;
    node->x_min = x;
    y = pos->y;
    node->y_max = y;
    node->y_min = y;
    FUN_00481b10(node);
}

// FUNCTION: LEGOLAND 0x00481c90
void RemovePathSquare(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481e60
void FUN_00481e60(struct PathQuery *query, struct PathBox *box) {
    int px = query->x;
    int lower_x = (box->x_min << 8) + 128;
    int upper_x = (box->x_max << 8) + 128;
    int lower_y = (box->y_min << 8) + 128;
    int upper_y = (box->y_max << 8) + 128;
    int clamped_x;
    int clamped_y;
    int py;
    int dx;
    int dy;

    if (px < lower_x) {
        clamped_x = lower_x;
    } else if (px > upper_x) {
        clamped_x = upper_x;
    } else {
        clamped_x = px;
    }

    py = query->y;
    if (py < lower_y) {
        clamped_y = lower_y;
    } else if (py > upper_y) {
        clamped_y = upper_y;
    } else {
        clamped_y = py;
    }

    dx = clamped_x - px;
    dy = clamped_y - query->y;
    box->dist_sq = (dx * dx) + (dy * dy);
}

// FUNCTION: LEGOLAND 0x00481ee0
void FUN_00481ee0(void) {
    struct BestNode *node;

    node = (struct BestNode *)DAT_0066b44c;
    if (node) {
        while (node) {
            node->field_20 = node->field_20 & 0xfffffffe;
            node = node->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x00481f00
void FUN_00481f00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482050
void SuggestNextMove(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004821c0
void FUN_004821c0(void) {
    int i;

    for (i = 0; i < 1152; i = i + 1) {
        DAT_00669258[i] = 0;
    }
}

// FUNCTION: LEGOLAND 0x004821e0
void FUN_004821e0(void) {
    struct DirNode *node;
    struct DirNode *next;

    node = (struct DirNode *)DAT_0066b450;
    while (node != NULL) {
        next = node->next;
        FUN_0049e4d0(node);
        node = next;
    }
    DAT_0066b450 = NULL;
}

// FUNCTION: LEGOLAND 0x00482210
void FUN_00482210(void) {
    struct DirNode *node;
    struct DirNode *next;

    node = (struct DirNode *)DAT_0066b458;
    while (node != NULL) {
        next = node->next;
        FUN_0049e4d0(node);
        node = next;
    }
    DAT_0066b458 = NULL;
}

// FUNCTION: LEGOLAND 0x00482240
void FUN_00482240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482300
struct DirNode *FUN_00482300(unsigned int x, unsigned int y) {
    struct DirNode *node;

    node = _malloc(16);
    if (node != NULL) {
        node->next = DAT_0066b458;
        DAT_0066b458 = node;
        node->x = x;
        node->y = y;
    }
    return node;
}

// FUNCTION: LEGOLAND 0x00482330
void FUN_00482330(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482430
void FUN_00482430(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004824d0
void PTPSuggestNextMove(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482620
void FUN_00482620(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482710
void FUN_00482710(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482860
unsigned int FUN_00482860(void) {
    unsigned int count;
    struct BestNode *node;

    count = 0;
    if (DAT_0066b44c != NULL) {
        node = (struct BestNode *)DAT_0066b44c;
        while (node != NULL) {
            count++;
            node = node->next;
        }
    }

    if (SaveGameWrite(&count, 4) == 0) {
        return 0;
    }

    node = (struct BestNode *)DAT_0066b44c;
    while (node != NULL) {
        if (SaveGameWrite(&node->x_min, 20) == 0) {
            return 0;
        }
        if (SaveGameWrite(&node->field_1c, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(&node->field_20, 4) == 0) {
            return 0;
        }
        node = node->next;
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x004828f0
void FUN_004828f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482920
void FUN_00482920(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004829c0
void FUN_004829c0(struct BestNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00482a40
void FUN_00482a40(struct InstancePos *pos) {
    struct BestNode *node;
    struct BestNode *found;

    if (DAT_0066b44c != NULL) {
        node = (struct BestNode *)DAT_0066b44c;
        while (node != NULL) {
            node->field_20 &= 0xfffffffd;
            node = node->next;
        }
    }

    found = FUN_00481790(pos);
    if (found != NULL) {
        FUN_004829c0(found);
    }
}

// FUNCTION: LEGOLAND 0x00482a80
void FUN_00482a80(void) {
    DAT_0066b460 = 0;
    DAT_006661c4 = 0;
}

// FUNCTION: LEGOLAND 0x00482a90
void FUN_00482a90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00482b00
unsigned int *FUN_00482b00(void) {
    return &DAT_0066b460;
}
