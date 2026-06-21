#include <stdlib.h>
#include "legoland.h"

#include "globals.h"
#include "llidb.h"
#include "objclass.h"
#include "pathfind.h"

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
    void *parent;
    int x;
    int y;
};

struct PathLink {
    unsigned char pad_0[4];
    struct PathLink *prev;
    int x;
    int y;
};

struct ObjData {
    unsigned char pad_0[0x3c];
    int field_3c;
    int field_40;
    unsigned char pad_44[4];
    int field_48;
};

struct ElemInfo {
    unsigned char pad_0[0xc];
    struct ObjData *obj;
};

struct MatchResult {
    unsigned char pad_0[4];
    unsigned char field_4;
    unsigned char field_5;
};

// FUNCTION: LEGOLAND 0x00481c50
LEGO_EXPORT void AddPathSquare(struct InstancePos *pos) {
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
LEGO_EXPORT void RemovePathSquare(struct InstancePos *pos) { STUB(); }

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
LEGO_EXPORT int SuggestNextMove(int *param_1, int *param_2, int *param_3) { STUB(); }

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

    node = DAT_0066b450;
    while (node != NULL) {
        next = node->next;
        free(node);
        node = next;
    }
    DAT_0066b450 = NULL;
}

// FUNCTION: LEGOLAND 0x00482210
void FUN_00482210(void) {
    struct DirNode *node;
    struct DirNode *next;

    node = DAT_0066b458;
    while (node != NULL) {
        next = node->next;
        free(node);
        node = next;
    }
    DAT_0066b458 = NULL;
}

// FUNCTION: LEGOLAND 0x00482240
void FUN_00482240(int x, int y, struct DirNode *parent) {
    struct MapElement *tile;
    struct DirNode *node;
    int word_index;
    unsigned int bit;

    if (x < 0 || x >= lpConfig->width) {
        return;
    }
    if (y < 0 || y >= lpConfig->height) {
        return;
    }

    tile = &GameMap[y][x];
    if (tile->field_10 & 0x2) {
        return;
    }

    word_index = (x >> 5) + y * 6;
    bit = 1u << (x & 0x1f);
    if (bit & DAT_00669258[word_index]) {
        return;
    }

    node = malloc(16);
    if (node == 0) {
        return;
    }

    node->next = DAT_0066b450;
    DAT_0066b450 = node;
    node->parent = parent;
    node->x = x;
    node->y = y;
    DAT_00669250++;
    DAT_00669258[word_index] |= bit;
}

// FUNCTION: LEGOLAND 0x00482300
struct DirNode *FUN_00482300(unsigned int x, unsigned int y) {
    struct DirNode *node;

    node = malloc(16);
    if (node != NULL) {
        node->next = DAT_0066b458;
        DAT_0066b458 = node;
        node->x = x;
        node->y = y;
    }
    return node;
}

// FUNCTION: LEGOLAND 0x00482330
int FUN_00482330(struct PathLink *a, struct PathLink *b, struct PathLink *c, struct PathLink *d) { STUB(); return 0; }

// FUNCTION: LEGOLAND 0x00482430
int FUN_00482430(void) {
    struct PathLink *cur;
    struct PathLink *prev;
    struct PathLink *p2;
    struct PathLink *p3;
    struct PathLink *p4;
    int dir;

    cur = (struct PathLink *)DAT_0066b454;
    p3 = 0;
    p2 = 0;
    p4 = 0;
    if (cur == 0) {
        return 0;
    }

    prev = cur->prev;
    if (prev != 0) {
        do {
            p4 = p3;
            p3 = p2;
            p2 = cur;
            cur = prev;
            prev = cur->prev;
        } while (prev != 0);
    }

    dir = FUN_00482330(cur, p2, p3, p4);
    switch (dir) {
    case 0:
        if (p2 != 0) {
            FUN_00482300(p2->x, p2->y);
        }
        return p4 == 0;
    case 1:
        FUN_00482300(p3->x, p3->y);
        return p4 == 0;
    case 2:
        FUN_00482300(p4->x, p4->y);
        break;
    }
    return p4 == 0;
}

// FUNCTION: LEGOLAND 0x004824d0
LEGO_EXPORT int PTPSuggestNextMove(int *param_1, int *param_2, int *param_3) {
    int start_x;
    int start_y;
    int goal_x;
    int goal_y;
    int wave;
    struct DirNode *node;

    start_x = param_1[0] >> 8;
    start_y = param_1[1] >> 8;
    goal_x = param_2[0] >> 8;
    goal_y = param_2[1] >> 8;

    FUN_004821e0();
    FUN_00482210();
    FUN_004821c0();

    DAT_00669250 = 0;
    DAT_0066b454 = 0;
    FUN_00482240(start_x, start_y, 0);

    wave = DAT_00669250;
    while (wave != 0) {
        node = DAT_0066b450;
        DAT_00669250 = 0;
        while (wave != 0) {
            wave--;
            if (node->x == goal_x && node->y == goal_y) {
                DAT_0066b454 = node;
                if (FUN_00482430()) {
                    param_3[0] = param_2[0];
                    param_3[1] = param_2[1];
                    FUN_004821e0();
                    FUN_00482210();
                    return 2;
                }
                param_3[0] = (DAT_0066b458->x << 8) + 0x80;
                param_3[1] = (DAT_0066b458->y << 8) + 0x80;
                FUN_004821e0();
                FUN_00482210();
                return 1;
            }
            FUN_00482240(node->x, node->y - 1, node);
            FUN_00482240(node->x + 1, node->y, node);
            FUN_00482240(node->x, node->y + 1, node);
            FUN_00482240(node->x - 1, node->y, node);
            node = node->next;
        }
        wave = DAT_00669250;
    }

    FUN_004821e0();
    FUN_00482210();
    return 0;
}

// FUNCTION: LEGOLAND 0x00482620
void FUN_00482620(int x, int y, struct DirNode *parent) {
    struct MapElement *tile;
    struct DirNode *node;
    int word_index;
    unsigned int bit;

    if (x < 0 || x >= lpConfig->width) {
        return;
    }
    if (y < 0 || y >= lpConfig->height) {
        return;
    }

    tile = &GameMap[y][x];
    if (tile->field_10 & 0x2) {
        if (!(((unsigned char *)&tile->flags)[1] & 0x8)) {
            return;
        }
    }

    word_index = (x >> 5) + y * 6;
    bit = 1u << (x & 0x1f);
    if (bit & DAT_00669258[word_index]) {
        return;
    }

    node = malloc(16);
    node->next = DAT_0066b450;
    DAT_0066b450 = node;
    node->parent = parent;
    node->x = x;
    node->y = y;
    DAT_00669250++;
    DAT_00669258[word_index] |= bit;
}

// FUNCTION: LEGOLAND 0x00482710
int FUN_00482710(int *a, int *b, int *out) {
    int start_x;
    int start_y;
    int goal_x;
    int goal_y;
    int wave;
    struct DirNode *node;

    start_x = a[0] >> 8;
    start_y = a[1] >> 8;
    goal_x = b[0] >> 8;
    goal_y = b[1] >> 8;

    FUN_004821e0();
    FUN_00482210();
    FUN_004821c0();

    DAT_00669250 = 0;
    DAT_0066b454 = 0;
    FUN_00482620(start_x, start_y, 0);

    wave = DAT_00669250;
    while (wave != 0) {
        node = DAT_0066b450;
        DAT_00669250 = 0;
        while (wave != 0) {
            wave--;
            if (node->x == goal_x && node->y == goal_y) {
                DAT_0066b454 = node;
                if (FUN_00482430()) {
                    out[0] = b[0];
                    out[1] = b[1];
                    FUN_004821e0();
                    FUN_00482210();
                    return 2;
                }
                out[0] = (DAT_0066b458->x << 8) + 0x80;
                out[1] = (DAT_0066b458->y << 8) + 0x80;
                FUN_004821e0();
                FUN_00482210();
                return 1;
            }
            FUN_00482620(node->x, node->y - 1, node);
            FUN_00482620(node->x + 1, node->y, node);
            FUN_00482620(node->x, node->y + 1, node);
            FUN_00482620(node->x - 1, node->y, node);
            node = node->next;
        }
        wave = DAT_00669250;
    }

    FUN_004821e0();
    FUN_00482210();
    return 0;
}

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
void FUN_004828f0(void) {
    struct BestNode *next;
    while (DAT_0066b44c != 0) {
        next = ((struct BestNode *)DAT_0066b44c)->next;
        free(DAT_0066b44c);
        DAT_0066b44c = next;
    }
    FUN_00482a80();
}

// FUNCTION: LEGOLAND 0x00482920
int FUN_00482920(void) {
    unsigned int count;
    struct BestNode *node;

    FUN_004828f0();

    if (SaveGameRead(&count, 4) == 0) {
        return 0;
    }

    while (count-- != 0) {
        node = malloc(0x24);
        node->next = DAT_0066b44c;
        DAT_0066b44c = node;
        if (SaveGameRead(&node->x_min, 0x14) == 0) {
            return 0;
        }
        if (SaveGameRead(&node->field_1c, 4) == 0) {
            return 0;
        }
        if (SaveGameRead(&node->field_20, 4) == 0) {
            return 0;
        }
    }

    return 1;
}

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
void FUN_00482a90(void) {
    struct MatchResult *match;
    struct ObjData *obj;

    if (DAT_0066b460 != 0) {
        return;
    }

    if (DAT_006661c4 == 0) {
        // STRING: LEGOLAND 0x004b83d0
        DAT_006661c4 = ElemID("ENTRANCE 1");
    }

    match = (struct MatchResult *)GetFirstObjectMatching(
        (struct RenderObjectVtable *)DAT_006661c4);
    obj = ((struct ElemInfo *)DAT_006661c4)->obj;

    DAT_0066b460 = match->field_4 + obj->field_3c - 1;
    DAT_0066b464 = ((obj->field_48 + obj->field_40) / 2) + match->field_5;
}

// FUNCTION: LEGOLAND 0x00482b00
unsigned int *FUN_00482b00(void) {
    return &DAT_0066b460;
}
