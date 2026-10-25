#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bloke.h"
#include "bricks.h"
#include "debug_alloc.h"
#include "globals.h"
#include "legoland.h"
#include "math.h"

#include "bricks.h"
#include "clipping.h"
#include "gamemap.h"
#include "llidb.h"
#include "log_flume.h"
#include "man3d.h"
#include "map_object.h"
#include "objclass.h"
#include "print_sprite.h"
#include "render3d.h"
#include "ride_queue.h"
#include "tilemap.h"

struct Sprite;

#include "image_sprite.h"

struct Particle {
    int x;
    int y;
    struct FlumeEntry *node; /* the slot owner: the next piece of the chain */
    float scale;
    int z;
    float w;
    int a;
    int b;
    int c;
};

struct WalkNode {
    struct WalkNode *var_0;
    struct FlumeSlot *var_4;
    struct WalkNode *var_8;
};

struct SubBuf {
    unsigned char b0;
    unsigned char b1;
};

struct LinkList {
    struct FlumeEntry *field_0;
    struct FlumeEntry *field_4;
    struct FlumeEntry *field_8;
    struct FlumeEntry *field_c;
};

struct InputBuffer {
    unsigned char flags;
    unsigned char pad_0[3];
    unsigned char var_4;
    unsigned char pad_4[3];
    unsigned char var_8;
    unsigned char pad_8[3];
    unsigned char var_c;
    unsigned char pad_c[3];
    unsigned char var_10;
    unsigned char pad_10[3];
    unsigned char var_14;
    unsigned char pad_14[3];
    unsigned char var_18;
    unsigned char pad_18[3];
    unsigned char var_1c;
    unsigned char pad_1c[3];
    unsigned char var_20;
};

struct ListNode {
    struct ListNode *prev;
    struct ListNode *next;
    struct ListNode *up;
    struct ListNode *down;
};

struct StateSlots {
    struct FlumeEntry *slot0;
    struct FlumeEntry *slot1;
    struct FlumeEntry *slot2;
    struct FlumeEntry *slot3;
};

typedef void (*FlumeCallback)(TileId tile, unsigned int *res);
typedef void (*FlumeEdgeFn)(struct FlumeEntry *, int *);
typedef int (*FlumeCheckFn)(struct FlumeEntry **);

struct FlumeRect {
    unsigned int var_0;
    unsigned int var_4;
    unsigned int var_8;
    unsigned int var_c;
};

struct FlumeBytes {
    unsigned char b0;
    unsigned char pad_1[3];
    unsigned char b4;
};

struct FlumeRunNode {
    struct FlumeRunNode *prev;
    struct FlumeRunNode *next;
};

struct FlumeStage {
    unsigned char pad_0[4];
    int count;
    struct Sprite *sprite;
};

struct FlumeStageList {
    unsigned char pad_0[4];
    unsigned char *stages;
};

struct FlumeNode;

// GLOBAL: LEGOLAND 0x004b4798
static struct PathPair Flume_PathPairs0[4] = {{0, -1}, {3, 0}, {0, -2}, {-4, 0}};
// GLOBAL: LEGOLAND 0x004b47b8
struct PathTable FlumePathTable0 = {4, Flume_PathPairs0};
// GLOBAL: LEGOLAND 0x004b47c0
static struct PathPair Flume_PathPairs1[5] = {{0, 0}, {0, 1}, {3, 0}, {0, 3}, {-4, 0}};
// GLOBAL: LEGOLAND 0x004b47e8
struct PathTable FlumePathTable1 = {5, Flume_PathPairs1};

// GLOBAL: LEGOLAND 0x004b4768
const char *LogFlumeTrackSpriteFiles[10] = {
    // STRING: LEGOLAND 0x004b48d4
    "fc1a_m.lls",
    // STRING: LEGOLAND 0x004b48c8
    "fc2a_m.lls",
    // STRING: LEGOLAND 0x004b48bc
    "fc3a_m.lls",
    // STRING: LEGOLAND 0x004b48b0
    "fc4a_m.lls",
    // STRING: LEGOLAND 0x004b48a4
    "fs1_m.lls",
    // STRING: LEGOLAND 0x004b4898
    "fs2_m.lls",
    // STRING: LEGOLAND 0x004b488c
    "fe2_m.lls",
    // STRING: LEGOLAND 0x004b4880
    "fe3_m.lls",
    // STRING: LEGOLAND 0x004b4874
    "fe4_m.lls",
    // STRING: LEGOLAND 0x004b4868
    "fe1_m.lls",
};

// FUNCTION: LEGOLAND 0x00408e40
void AddFlumeEntry(TileId tile) {
    struct FlumeEntry *entry = malloc(sizeof(struct FlumeEntry));
    if (entry != NULL) {
        memset(entry, 0, sizeof(struct FlumeEntry));
        entry->tile = tile;
        entry->next = FlumeEntryList;
        FlumeEntryList = entry;
    }
}

// FUNCTION: LEGOLAND 0x00408e80
void RemoveFlumeEntry(struct FlumeEntry *entry) {
    struct FlumeEntry *prev;
    struct FlumeEntry *cur;

    if (FlumeEntryList == entry) {
        FlumeEntryList = entry->next;
    } else {
        cur = FlumeEntryList->next;
        prev = FlumeEntryList;
        while (cur != entry) {
            prev = prev->next;
            if (prev == NULL) {
                break;
            }
            cur = prev->next;
        }
        if (prev != NULL) {
            prev->next = entry->next;
        }
    }
    free(entry);
}

// FUNCTION: LEGOLAND 0x00408ec0
struct FlumeEntry *FindFlumeEntryByTile(TileId *tile) {
    struct FlumeEntry *cur = FlumeEntryList;
    if (cur == NULL) {
        return NULL;
    }
    while (memcmp(&cur->tile, tile, sizeof(TileId)) != 0) {
        cur = cur->next;
        if (cur == NULL) {
            return NULL;
        }
    }
    return cur;
}

// FUNCTION: LEGOLAND 0x00408ef0
struct FlumeEntry *FindFlumeSubEntryByTile(TileId *tile) {
    struct FlumeEntry *outer = FlumeEntryList;
    struct FlumeEntry *cur;

    if (outer == NULL) {
        return NULL;
    }
    do {
        cur = outer->sub;
        while (cur != NULL) {
            if (memcmp(&cur->tile, tile, sizeof(TileId)) == 0) {
                return cur;
            }
            cur = cur->next;
        }
        outer = outer->next;
    } while (outer != NULL);
    return NULL;
}

// FUNCTION: LEGOLAND 0x00408f30
struct FlumeEntry *FindFlumePieceAtTile(struct SubBuf *buf) {
    struct FlumeEntry *outer;
    struct FlumeEntry *mid;
    struct FlumeEntry *cur;

    outer = FlumeEntryList;
    if (FlumeEntryList != NULL) {
        for (; outer != NULL; outer = outer->next) {
            for (mid = outer->sub; mid != NULL; mid = mid->next) {
                cur = mid->sub2;
                if (cur == NULL) {
                    if (memcmp(&mid->tile, buf, sizeof(TileId)) == 0) {
                        return mid;
                    }
                } else {
                    do {
                        if (memcmp(&cur->tile, buf, sizeof(TileId)) == 0) {
                            return cur;
                        }
                        cur = cur->next;
                    } while (cur != NULL);
                }
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00408f90
struct FlumeEntry *FindFlumeTrackTileAt(int x, int y) {
    struct FlumeEntry *outer;
    struct FlumeEntry *cur;
    int h = LogFlumeFootprint.y1 - LogFlumeFootprint.y0;
    int w = LogFlumeFootprint.x1 - LogFlumeFootprint.x0;

    outer = FlumeEntryList;
    while (outer != NULL) {
        cur = outer->sub;
        while (cur != NULL) {
            if (x >= cur->tile.pos.x && x <= cur->tile.pos.x + w && y >= cur->tile.pos.y && y <= cur->tile.pos.y + h) {
                return cur;
            }
            cur = cur->next;
        }
        outer = outer->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00409010
void *AllocFlumeNode(void) {
    /* a track node: FlumeEntry callers use only the FlumeNode head */
    void *block = malloc(sizeof(struct FlumeNode));
    if (block != NULL) {
        memset(block, 0, sizeof(struct FlumeNode));
    }
    return block;
}

// FUNCTION: LEGOLAND 0x00409040
void InsertNodeBefore(struct FlumeEntry *node, struct FlumeEntry *insert) {
    if (node->field_c != NULL) {
        node->field_c->parent8 = insert;
    }
    insert->parent8 = node;
    if (node->field_c != NULL) {
        // STRING: LEGOLAND 0x004b48e0
        printf("bug");
    }
    node->field_c = insert;
}

// FUNCTION: LEGOLAND 0x00409080
void LinkNodeAfter(struct FlumeEntry *node, struct FlumeEntry *insert) {
    if (node->parent8 != NULL) {
        node->parent8->field_c = insert;
    }
    insert->field_c = node;
    if (node->parent8 != NULL) {
        printf("bug");
    }
    node->parent8 = insert;
}

// FUNCTION: LEGOLAND 0x004090c0
void InsertNodeBetween(struct FlumeEntry *node1, struct FlumeEntry *node2, struct FlumeEntry *node3) {
    node3->field_c = node1;
    node3->parent8 = node2;
    node1->parent8 = node3;
    node2->field_c = node3;
}

// FUNCTION: LEGOLAND 0x004090e0
struct FlumeEntry *FindFlumeChainEnd(struct FlumeEntry *node) {
    struct FlumeEntry *start = node;

    while (node->field_c != NULL) {
        node = node->field_c;
        if (node == start) {
            return start;
        }
        if (node == NULL) {
            return NULL;
        }
    }
    return node;
}

// FUNCTION: LEGOLAND 0x00409110
void ReverseNodeList(struct FlumeEntry *container) {
    struct FlumeEntry *node = FindFlumeChainEnd(container);
    struct FlumeEntry *next;
    struct FlumeEntry *prev;
    if (node != NULL) {
        do {
            next = node->parent8;
            prev = node->field_c;
            node->parent8 = prev;
            node->field_c = next;
            node = next;
        } while (next != NULL);
    }
}

// FUNCTION: LEGOLAND 0x00409140
int IsJoinedToFlume(struct FlumeEntry *container) {
    struct FlumeEntry *node = FindFlumeChainEnd(container);
    struct FlumeEntry *head;
    if (node == NULL) {
        return 0;
    }
    head = container->parent->parent8;
    while (node != NULL) {
        if (node == head) {
            return 1;
        }
        node = node->parent8;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00409170
void AppendListNode(struct FlumeEntry *container, struct ListNode *node) {
    node->prev = container->list_tail;
    node->next = NULL;
    if (container->list_tail != NULL) {
        container->list_tail->next = node;
    }
    container->list_tail = node;
}

// FUNCTION: LEGOLAND 0x004091a0
void UnlinkFlumeSubNode(struct FlumeEntry *container, struct ListNode *node) {
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node == container->list_tail) {
        container->list_tail = node->prev;
    }
    if (node->down != NULL) {
        node->down->up = NULL;
    }
    if (node->up != NULL) {
        node->up->down = NULL;
    }
}

// FUNCTION: LEGOLAND 0x004091f0
void PushFlumeListNode(struct FlumeEntry *container, struct ListNode *node) {
    struct ListNode *head = container->list_head;
    node->next = NULL;
    node->prev = head;
    head = container->list_head;
    if (head != NULL) {
        head->next = node;
    }
    container->list_head = node;
}

// FUNCTION: LEGOLAND 0x00409220
void UnlinkFlumeEntry(struct FlumeEntry *container, struct ListNode *node) {
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node == container->list_head) {
        container->list_head = node->prev;
    }
    if (node->down != NULL) {
        node->down->up = NULL;
    }
    if (node->up != NULL) {
        node->up->down = NULL;
    }
}

// FUNCTION: LEGOLAND 0x00409270
void FreeFlumeEntry(struct FlumeEntry *other, struct FlumeEntry *container) {
    struct ListNode *node = container->list_tail;
    struct ListNode *prev;
    while (node != NULL) {
        prev = node->prev;
        UnlinkFlumeSubNode(container, node);
        free(node);
        node = prev;
    }
    UnlinkFlumeEntry(other, (struct ListNode *)container);
    free(container);
}

// FUNCTION: LEGOLAND 0x004092b0
int GetFlumeSlotDirection(struct FlumeSlot *holder) {
    struct FlumeEntry *e = holder->owner;
    int mode = e->mode;
    TileId t;
    int dx;
    int dy;

    if (mode > 0) {
        if (mode > 2) {
            if (mode == 3) {
                switch (e->submode) {
                case 1:
                    return 3;
                case 0:
                    return 1;
                case 2:
                    return 5;
                case 3:
                    return 7;
                }
            }
        } else {
            t = e->parent8->tile;
            dx = t.pos.x - e->tile.pos.x;
            dy = t.pos.y - e->tile.pos.y;
            if (dx < 0) {
                return 7;
            } else if (dx > 0) {
                return 3;
            } else if (dy >= 0 && dy > 0) {
                return 5;
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00409360
void FindFlumeNeighbours(TileId tile) {
    struct SubBuf buf;
    int w = LogFlumeFootprint.x1 - LogFlumeFootprint.x0;
    int h = LogFlumeFootprint.y1 - LogFlumeFootprint.y0;

    buf.b0 = tile.pos.x;
    buf.b1 = tile.pos.y - h;
    FlumeNeighbours[0] = FindFlumePieceAtTile(&buf);
    buf.b0 = tile.pos.x + w;
    buf.b1 = tile.pos.y;
    FlumeNeighbours[1] = FindFlumePieceAtTile(&buf);
    buf.b0 = tile.pos.x;
    buf.b1 = tile.pos.y + h;
    FlumeNeighbours[2] = FindFlumePieceAtTile(&buf);
    buf.b0 = tile.pos.x - w;
    buf.b1 = tile.pos.y;
    FlumeNeighbours[3] = FindFlumePieceAtTile(&buf);
}

// FUNCTION: LEGOLAND 0x00409410
int FlumeNeighbourMask(struct FlumeEntry **ctx) {
    int result = 0;
    if (ctx[0] != 0) {
        result = 1;
    }
    if (ctx[1] != 0) {
        result |= 0x4;
    }
    if (ctx[2] != 0) {
        result |= 0x10;
    }
    if (ctx[3] != 0) {
        result |= 0x40;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00409440
void GetFlumeNeighbours(TileId param_1, void **param_2) {
    FlumeNeighbours[0] = 0;
    FlumeNeighbours[1] = 0;
    FlumeNeighbours[2] = 0;
    FlumeNeighbours[3] = 0;
    FindFlumeNeighbours(param_1);
    *param_2 = FlumeNeighbours;
}

// FUNCTION: LEGOLAND 0x00409470
unsigned int CountFlumeNeighbours(unsigned int *list) {
    unsigned int result = 0;
    if (FlumeNeighbours[0] != 0) {
        result = 1;
    }
    if (FlumeNeighbours[1] != 0) {
        result++;
    }
    if (FlumeNeighbours[2] != 0) {
        result++;
    }
    if (FlumeNeighbours[3] != 0) {
        result++;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x004094b0
void DropForeignFlumeTrackNeighbours(struct FlumeEntry *arg, struct FlumeEntry **slots) {
    if (slots[0] != NULL && slots[0]->parent != arg) {
        slots[0] = NULL;
    }
    if (slots[1] != NULL && slots[1]->parent != arg) {
        slots[1] = NULL;
    }
    if (slots[2] != NULL && slots[2]->parent != arg) {
        slots[2] = NULL;
    }
    if (slots[3] != NULL && slots[3]->parent != arg) {
        slots[3] = NULL;
    }
}

// FUNCTION: LEGOLAND 0x00409510
void DropNonEndFlumeTrackNeighbours(struct StateSlots *slots) {
    if (slots->slot0 != NULL && slots->slot0->mode != 3 && slots->slot0->mode != 4) {
        slots->slot0 = NULL;
    }
    if (slots->slot1 != NULL && slots->slot1->mode != 3 && slots->slot1->mode != 4) {
        slots->slot1 = NULL;
    }
    if (slots->slot2 != NULL && slots->slot2->mode != 3 && slots->slot2->mode != 4) {
        slots->slot2 = NULL;
    }
    if (slots->slot3 != NULL && slots->slot3->mode != 3 && slots->slot3->mode != 4) {
        slots->slot3 = NULL;
    }
}

/* Log flume track cursor test: true when the neighbour connection mask is a dead end, straight or corner (no T or
 * cross) and every present neighbour piece is in mode 3 or 4. */
// FUNCTION: LEGOLAND 0x00409580
int CanPlaceLogFlumeTrack(int code, struct StateSlots *slots) {
    int result = 0;
    if (code == 1 || code == 4 || code == 0x10 || code == 0x40 || code == 0x11 || code == 0x44 || code == 5 || code == 0x14 || code == 0x50 || code == 0x41) {
        result = 1;
        if (slots->slot0 != NULL && slots->slot0->mode != 3 && slots->slot0->mode != 4) {
            result = 0;
        }
        if (slots->slot1 != NULL && slots->slot1->mode != 3 && slots->slot1->mode != 4) {
            result = 0;
        }
        if (slots->slot2 != NULL && slots->slot2->mode != 3 && slots->slot2->mode != 4) {
            result = 0;
        }
        if (slots->slot3 != NULL && slots->slot3->mode != 3 && slots->slot3->mode != 4) {
            result = 0;
        }
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00409620
void JoinFlumeNeighbour0(struct FlumeEntry *node) {
    if (node->mode == 3) {
        if (node->submode == 2) {
            node->mode = 1;
            node->submode = 0;
            return;
        }
        if (node->submode == 1) {
            node->mode = 2;
            node->submode = 2;
            return;
        }
        if (node->submode == 3) {
            node->mode = 2;
            node->submode = 1;
            return;
        }
    }
    if (node->mode == 4) {
        node->mode = 3;
        node->submode = 0;
    }
}

// FUNCTION: LEGOLAND 0x00409680
void JoinFlumeNeighbour2(struct FlumeEntry *node) {
    if (node->mode == 3) {
        if (node->submode == 0) {
            node->mode = 1;
            node->submode = 0;
            return;
        }
        if (node->submode == 1) {
            node->mode = 2;
            node->submode = 3;
            return;
        }
        if (node->submode == 3) {
            node->mode = 2;
            node->submode = 0;
            return;
        }
    }
    if (node->mode == 4) {
        node->mode = 3;
        node->submode = 2;
    }
}

// FUNCTION: LEGOLAND 0x004096e0
void JoinFlumeNeighbour1(struct FlumeEntry *node) {
    if (node->mode == 3) {
        if (node->submode == node->mode) {
            node->mode = 1;
            node->submode = 1;
            return;
        }
        if (node->submode == 0) {
            node->mode = 2;
            node->submode = 2;
            return;
        }
        if (node->submode == 2) {
            node->mode = node->submode;
            node->submode = 3;
            return;
        }
    }
    if (node->mode == 4) {
        node->mode = 3;
        node->submode = 1;
    }
}

// FUNCTION: LEGOLAND 0x00409740
void JoinFlumeNeighbour3(struct FlumeEntry *node) {
    if (node->mode == 3) {
        if (node->submode == 1) {
            node->mode = 1;
            node->submode = 1;
            return;
        }
        if (node->submode == 0) {
            node->mode = 2;
            node->submode = 1;
            return;
        }
        if (node->submode == 2) {
            node->mode = 2;
            node->submode = 0;
            return;
        }
    }
    if (node->mode == 4) {
        node->mode = 3;
        node->submode = 3;
    }
}

// FUNCTION: LEGOLAND 0x004097a0
void SetFlumePieceShapeFromNeighbours(struct FlumeEntry *entry, struct StateSlots *slots) {
    int mask = FlumeNeighbourMask(&slots->slot0);
    struct FlumeEntry *sub;

    if (entry != NULL) {
        sub = entry->sub2;
        if (sub == NULL) {
            switch (mask) {
            case 0x01:
            case 0x04:
            case 0x10:
            case 0x40:
                entry->mode = 3;
                break;
            case 0x11:
            case 0x44:
                entry->mode = 1;
                break;
            case 0x05:
            case 0x14:
            case 0x41:
            case 0x50:
                entry->mode = 2;
                break;
            }
        }
        switch (mask) {
        case 0x01:
            if (sub == NULL) {
                entry->submode = 2;
            }
            JoinFlumeNeighbour0(slots->slot0);
            break;
        case 0x10:
            if (sub == NULL) {
                entry->submode = 0;
            }
            JoinFlumeNeighbour2(slots->slot2);
            break;
        case 0x04:
            if (sub == NULL) {
                entry->submode = 3;
            }
            JoinFlumeNeighbour1(slots->slot1);
            break;
        case 0x40:
            if (sub == NULL) {
                entry->submode = 1;
            }
            JoinFlumeNeighbour3(slots->slot3);
            break;
        case 0x11:
            if (sub == NULL) {
                entry->submode = 0;
            }
            JoinFlumeNeighbour0(slots->slot0);
            JoinFlumeNeighbour2(slots->slot2);
            break;
        case 0x44:
            if (sub == NULL) {
                entry->submode = 1;
            }
            JoinFlumeNeighbour1(slots->slot1);
            JoinFlumeNeighbour3(slots->slot3);
            break;
        case 0x05:
            if (sub == NULL) {
                entry->submode = 0;
            }
            JoinFlumeNeighbour0(slots->slot0);
            JoinFlumeNeighbour1(slots->slot1);
            break;
        case 0x14:
            if (sub == NULL) {
                entry->submode = 1;
            }
            JoinFlumeNeighbour1(slots->slot1);
            JoinFlumeNeighbour2(slots->slot2);
            break;
        case 0x50:
            if (sub == NULL) {
                entry->submode = 2;
            }
            JoinFlumeNeighbour2(slots->slot2);
            JoinFlumeNeighbour3(slots->slot3);
            break;
        case 0x41:
            if (sub == NULL) {
                entry->submode = 3;
            }
            JoinFlumeNeighbour3(slots->slot3);
            JoinFlumeNeighbour0(slots->slot0);
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x00409a50
void MakeFlumePieceStraight(struct FlumeEntry *node) {
    switch (node->submode) {
    case 0:
    case 2:
        node->submode = 0;
        break;
    case 1:
    case 3:
        node->submode = 1;
        break;
    }
    node->mode = 1;
}

// FUNCTION: LEGOLAND 0x00409a90
void JoinFlumeNeighbourShapes(void *edi_ptr[4], struct FlumeEntry *esi_ptr[4]) {
    int flags = FlumeNeighbourMask(esi_ptr);
    if (flags & 0x1) {
        MakeFlumePieceStraight(edi_ptr[0]);
        JoinFlumeNeighbour0(esi_ptr[0]);
    }
    if (flags & 0x4) {
        MakeFlumePieceStraight(edi_ptr[1]);
        JoinFlumeNeighbour1(esi_ptr[1]);
    }
    if (flags & 0x10) {
        MakeFlumePieceStraight(edi_ptr[2]);
        JoinFlumeNeighbour2(esi_ptr[2]);
    }
    if (flags & 0x40) {
        MakeFlumePieceStraight(edi_ptr[3]);
        JoinFlumeNeighbour3(esi_ptr[3]);
    }
}

// FUNCTION: LEGOLAND 0x00409b10
void OrientFlumeChainsForJoin(struct FlumeEntry *a, struct FlumeEntry *b) {
    int a_ok = IsJoinedToFlume(a);
    int b_ok = IsJoinedToFlume(b);

    if (a_ok != 0 && b_ok != 0) {
        // STRING: LEGOLAND 0x004b48e4
        printf("both pieces are joined to log flume - error");
    } else if (a_ok == 0 && b_ok == 0) {
        ReverseNodeList(a);
    } else if (a_ok != 0 && b_ok == 0) {
        ReverseNodeList(b);
    } else {
        ReverseNodeList(a);
    }
}

// FUNCTION: LEGOLAND 0x00409b70
void JoinOppositeFlumeNodes(int i, int j, struct FlumeEntry **nodes, struct FlumeEntry *insert) {
    int a_ok;
    int b_ok;

    // STRING: LEGOLAND 0x004b4910
    printf("going opposite directions!!!!");
    a_ok = IsJoinedToFlume(nodes[i]);
    b_ok = IsJoinedToFlume(nodes[j]);
    if (a_ok != 0 && b_ok != 0) {
        // STRING: LEGOLAND 0x004b48e4
        printf("both pieces are joined to log flume - error");
        return;
    } else if (a_ok == 0 && b_ok == 0) {
        ReverseNodeList(nodes[i]);
    } else if (a_ok != 0 && b_ok == 0) {
        ReverseNodeList(nodes[j]);
    } else {
        ReverseNodeList(nodes[i]);
    }
    if (nodes[i]->parent8 == NULL && nodes[j]->field_c == NULL) {
        InsertNodeBetween(nodes[i], nodes[j], insert);
    } else {
        InsertNodeBetween(nodes[j], nodes[i], insert);
    }
}

// FUNCTION: LEGOLAND 0x00409c20
void InsertFlumePieceIntoChain(struct FlumeEntry *entry, struct FlumeEntry **list) {
    struct FlumeEntry *insert = entry;
    struct FlumeEntry **nodes = list;
    struct FlumeEntry *a;

    switch (FlumeNeighbourMask(list)) {
    case 0x01:
        a = nodes[0];
        if (a->field_c == NULL) {
            InsertNodeBefore(a, insert);
        } else {
            LinkNodeAfter(a, insert);
        }
        break;
    case 0x10:
        a = nodes[2];
        if (a->field_c == NULL) {
            InsertNodeBefore(a, insert);
        } else {
            LinkNodeAfter(a, insert);
        }
        break;
    case 0x04:
        a = nodes[1];
        if (a->field_c == NULL) {
            InsertNodeBefore(a, insert);
        } else {
            LinkNodeAfter(a, insert);
        }
        break;
    case 0x40:
        a = nodes[3];
        if (a->field_c == NULL) {
            InsertNodeBefore(a, insert);
        } else {
            LinkNodeAfter(a, insert);
        }
        break;
    case 0x11:
        a = nodes[0];
        if ((a->parent8 != NULL && nodes[2]->parent8 != NULL && a->field_c == NULL && nodes[2]->field_c == NULL) || (a->parent8 == NULL && nodes[2]->parent8 == NULL && a->field_c != NULL && nodes[2]->field_c != NULL)) {
            JoinOppositeFlumeNodes(0, 2, nodes, insert);
            return;
        }
        if (a->parent8 == NULL && nodes[2]->field_c == NULL) {
            InsertNodeBetween(a, nodes[2], insert);
        } else {
            InsertNodeBetween(nodes[2], a, insert);
        }
        return;
    case 0x44:
        a = nodes[1];
        if ((a->parent8 != NULL && nodes[3]->parent8 != NULL && a->field_c == NULL && nodes[3]->field_c == NULL) || (a->parent8 == NULL && nodes[3]->parent8 == NULL && a->field_c != NULL && nodes[3]->field_c != NULL)) {
            JoinOppositeFlumeNodes(1, 3, nodes, insert);
            return;
        }
        if (a->field_c == NULL && nodes[3]->parent8 == NULL) {
            InsertNodeBetween(nodes[3], a, insert);
        } else {
            InsertNodeBetween(a, nodes[3], insert);
        }
        return;
    case 0x05:
        a = nodes[0];
        if ((a->parent8 != NULL && nodes[1]->parent8 != NULL && a->field_c == NULL && nodes[1]->field_c == NULL) || (a->parent8 == NULL && nodes[1]->parent8 == NULL && a->field_c != NULL && nodes[1]->field_c != NULL)) {
            JoinOppositeFlumeNodes(0, 1, nodes, insert);
            return;
        }
        if (nodes[1]->field_c == NULL && a->parent8 == NULL) {
            InsertNodeBetween(a, nodes[1], insert);
        } else {
            InsertNodeBetween(nodes[1], a, insert);
        }
        return;
    case 0x14:
        a = nodes[1];
        if ((a->parent8 != NULL && nodes[2]->parent8 != NULL && a->field_c == NULL && nodes[2]->field_c == NULL) || (a->parent8 == NULL && nodes[2]->parent8 == NULL && a->field_c != NULL && nodes[2]->field_c != NULL)) {
            JoinOppositeFlumeNodes(1, 2, nodes, insert);
            return;
        }
        if (nodes[2]->field_c == NULL && a->parent8 == NULL) {
            InsertNodeBetween(a, nodes[2], insert);
        } else {
            InsertNodeBetween(nodes[2], a, insert);
        }
        return;
    case 0x50:
        a = nodes[2];
        if ((a->parent8 != NULL && nodes[3]->parent8 != NULL && a->field_c == NULL && nodes[3]->field_c == NULL) || (a->parent8 == NULL && nodes[3]->parent8 == NULL && a->field_c != NULL && nodes[3]->field_c != NULL)) {
            JoinOppositeFlumeNodes(2, 3, nodes, insert);
            return;
        }
        if (nodes[3]->field_c == NULL && a->parent8 == NULL) {
            InsertNodeBetween(a, nodes[3], insert);
        } else {
            InsertNodeBetween(nodes[3], a, insert);
        }
        return;
    case 0x41:
        a = nodes[3];
        if ((a->parent8 != NULL && nodes[0]->parent8 != NULL && a->field_c == NULL && nodes[0]->field_c == NULL) || (a->parent8 != NULL && nodes[0]->parent8 != NULL && a->field_c == NULL && nodes[0]->field_c == NULL)) {
            JoinOppositeFlumeNodes(3, 0, nodes, insert);
            return;
        }
        if (nodes[0]->field_c == NULL && a->parent8 == NULL) {
            InsertNodeBetween(a, nodes[0], insert);
        } else {
            InsertNodeBetween(nodes[0], a, insert);
        }
        return;
    }
}

// FUNCTION: LEGOLAND 0x0040a010
void JoinFlumeChains(struct FlumeEntry *a, struct FlumeEntry *b) {
    if ((a->parent8 != NULL && b->parent8 != NULL && a->field_c == NULL && b->field_c == NULL) ||
        (a->parent8 == NULL && b->parent8 == NULL && a->field_c != NULL && b->field_c != NULL)) {
        OrientFlumeChainsForJoin(a, b);
    }
    if (a->parent8 == NULL) {
        LinkNodeAfter(a, b);
    } else {
        InsertNodeBefore(a, b);
    }
}

// FUNCTION: LEGOLAND 0x0040a080
void JoinFlumeNeighbourChains(struct FlumeEntry **b, struct FlumeEntry **a) {
    int flags = FlumeNeighbourMask(a);

    if (flags & 1) {
        JoinFlumeChains(a[0], b[0]);
    }
    if (flags & 4) {
        JoinFlumeChains(a[1], b[1]);
    }
    if (flags & 0x10) {
        JoinFlumeChains(a[2], b[2]);
    }
    if (flags & 0x40) {
        JoinFlumeChains(a[3], b[3]);
    }
}

// FUNCTION: LEGOLAND 0x0040a0f0
void DetachFlumeNeighbour0(struct FlumeEntry *node) {
    if (node == NULL) {
        return;
    }
    if (node->mode == 3 && node->submode == 0) {
        node->mode = 4;
        return;
    }
    if (node->mode == 1 && node->submode == 0) {
        node->mode = 3;
        node->submode = 2;
        return;
    }
    if (node->mode == 2) {
        if (node->submode == 2) {
            node->mode = 3;
            node->submode = 1;
            return;
        } else if (node->submode == 1) {
            node->mode = 3;
            node->submode = 3;
        }
    }
}

// FUNCTION: LEGOLAND 0x0040a160
void DetachFlumeNeighbour1(struct FlumeEntry *node) {
    if (node != NULL) {
        if (node->mode == 3 && node->submode == 1) {
            node->mode = 4;
        } else if (node->mode == 1 && node->submode == 1) {
            node->mode = 3;
            node->submode = 3;
        } else if (node->mode == 2) {
            if (node->submode == 3) {
                node->mode = 3;
                node->submode = 2;
            } else if (node->submode == 2) {
                node->mode = 3;
                node->submode = 0;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0040a1d0
void DetachFlumeNeighbour2(struct FlumeEntry *node) {
    if (node == NULL) {
        return;
    }
    if (node->mode == 3 && node->submode == 2) {
        node->mode = 4;
        return;
    }
    if (node->mode == 1 && node->submode == 0) {
        node->mode = 3;
        node->submode = 0;
        return;
    }
    if (node->mode == 2) {
        if (node->submode == 3) {
            node->mode = 3;
            node->submode = 1;
            return;
        }
        if (node->submode == 0) {
            node->mode = 3;
            node->submode = 3;
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x0040a230
void DetachFlumeNeighbour3(struct FlumeEntry *node) {
    if (node == NULL) {
        return;
    }
    if (node->mode == 3 && node->submode == 3) {
        node->mode = 4;
    } else if (node->mode == 1 && node->submode == 1) {
        node->mode = 3;
        node->submode = 1;
    } else if (node->mode == 2) {
        if (node->submode == 0) {
            node->mode = 3;
            node->submode = 2;
        } else if (node->submode == 1) {
            node->mode = 3;
            node->submode = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x0040a2a0
void DetachFlumeNeighbourShapes(void *other, struct FlumeEntry **ctx) {
    FlumeNeighbourMask(ctx);
    if (other != NULL) {
        DetachFlumeNeighbour0(ctx[0]);
        DetachFlumeNeighbour1(ctx[1]);
        DetachFlumeNeighbour2(ctx[2]);
        DetachFlumeNeighbour3(ctx[3]);
    }
}

// FUNCTION: LEGOLAND 0x0040a2e0
void LogFlumeEntranceLoad(Element *elem) {
    struct LLS *lls;

    LogFlumeEntranceRide = elem->ride;
    LogFlumeEntranceRide->flags |= 0x20;
    LogFlumeEntranceLayer = LogFlumeEntranceRide->layer;
    if (LogFlumeEntranceLayer != NULL) {
        LogFlumeEntranceLayer->flags |= 0x2000;
    }
    FlumeQueueTable0 = BuildQueueTable(&FlumePathTable0);
    FlumeQueueTable1 = BuildQueueTable(&FlumePathTable1);
    // STRING: LEGOLAND 0x004b49c8
    LogFlumeBarrelSprite = LoadSprite("lf_barrel.lls", 1);
    // STRING: LEGOLAND 0x004b49b8
    LogFlumeBarrelMSprite = LoadSprite("lf_barrel_m.lls", 1);
    // STRING: LEGOLAND 0x004b49a8
    LogFlumeBarrel1Sprite = LoadSprite("lf_barrel1.lls", 1);
    // STRING: LEGOLAND 0x004b4998
    LogFlumeBarrelMatteSprite = LoadSprite("barrelmatte.lls", 1);
    if (LogFlumeBarrelSprite != NULL) {
        lls = GetLLSForSprite((struct Sprite *)LogFlumeBarrelSprite);
        if (lls != NULL) {
            LLSPlay(lls, LogFlumeBarrelSprite->image);
        }
    }
    // STRING: LEGOLAND 0x004b4984
    LogFlumeEnta1Matte2Sprite = LoadSprite("lf_enta1_matte2.lls", 1);
    // STRING: LEGOLAND 0x004b4978
    LogFlumeSignSprite = LoadSprite("lf_sign.lls", 1);
    // STRING: LEGOLAND 0x004b4964
    LogFlumeEntrance1Sprite = LoadSprite("lf_entrance1.lls", 1);
    // STRING: LEGOLAND 0x004b4950
    LogFlumeEntrance2Sprite = LoadSprite("lf_entrance2.lls", 1);
    // STRING: LEGOLAND 0x004b493c
    LogFlumeEntrance3Sprite = LoadSprite("lf_entrance3.lls", 1);
    // STRING: LEGOLAND 0x004b4930
    LogFlumeEnta3MSprite = LoadSprite("enta3_m.lls", 1);
}

// FUNCTION: LEGOLAND 0x0040a410
void LogFlumeEntranceUnload(void) {
    struct FlumeEntry *current;

    if (LogFlumeEnta3MSprite) {
        KillSprite(LogFlumeEnta3MSprite);
    }
    if (LogFlumeEntrance3Sprite) {
        KillSprite(LogFlumeEntrance3Sprite);
    }
    if (LogFlumeEntrance2Sprite) {
        KillSprite(LogFlumeEntrance2Sprite);
    }
    if (LogFlumeEntrance1Sprite) {
        KillSprite(LogFlumeEntrance1Sprite);
    }
    if (LogFlumeSignSprite) {
        KillSprite(LogFlumeSignSprite);
    }
    if (LogFlumeEnta1Matte2Sprite) {
        KillSprite(LogFlumeEnta1Matte2Sprite);
    }
    if (LogFlumeBarrelSprite) {
        KillSprite(LogFlumeBarrelSprite);
    }
    if (LogFlumeBarrelMSprite) {
        KillSprite(LogFlumeBarrelMSprite);
    }
    if (FlumeQueueTable1) {
        FreeIfNotNull(FlumeQueueTable1);
    }
    if (FlumeQueueTable0) {
        FreeIfNotNull(FlumeQueueTable0);
    }
    if (LogFlumeBarrel1Sprite) {
        KillSprite(LogFlumeBarrel1Sprite);
    }
    if (LogFlumeBarrelMatteSprite) {
        KillSprite(LogFlumeBarrelMatteSprite);
    }

    current = FlumeEntryList;
    if (current != NULL) {
        while (current != NULL) {
            struct FlumeEntry *next = current->next;
            struct FlumeEntry *chain = current->sub;
            while (chain != NULL) {
                struct FlumeEntry *next_chain = chain->next;
                free(chain);
                chain = next_chain;
            }
            free(current);
            current = next;
        }
        FlumeEntryList = current;
        return;
    }
    FlumeEntryList = NULL;
}

// FUNCTION: LEGOLAND 0x0040a540
void LogFlumeEntranceSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = LogFlumeEntranceRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

/* Places the entrance's log particles at the footprint's bottom-left edge midpoint (GetFlumeEdgeMidBottomLeft),
 * linking each to successive nodes, with z 0, scale 1, w 0.1 and cleared state. */
// FUNCTION: LEGOLAND 0x0040a580
void ResetLogFlumeEntranceParticles(struct FlumeEntry *param) {
    int i;
    struct Particle *p;
    struct FlumeEntry *node;
    struct FlumeDims d;

    node = param->field_8;
    i = 0;
    if ((int)param->count > 0) {
        /* the particles overlay the slots, 0xc bytes in (x86 layout) */
        p = (struct Particle *)((unsigned char *)param->slots + 0xc);
        do {
            p->node = node;
            d = GetFlumeEdgeMidBottomLeft();
            p->x = d.field1;
            p->y = d.field2;
            p->z = 0;
            p->scale = 1.0f;
            p->w = 0.1f;
            p[-1].a = 0;
            p[-1].b = 0;
            node = node->field_c;
            i++;
            p++;
        } while (i < (int)param->count);
    }
}

/* Sets a new log flume entrance to 4 particles (field_d0 = 6) and places them with ResetLogFlumeEntranceParticles. */
// FUNCTION: LEGOLAND 0x0040a5d0
void InitLogFlumeEntranceParticles(struct FlumeEntry *param) {
    if (param != NULL) {
        param->count = 4;
        param->field_d0 = 6;
        ResetLogFlumeEntranceParticles(param);
    }
}

// FUNCTION: LEGOLAND 0x0040a600
void LogFlumeEntranceAddObject(Element *elem, int *pt) {
    TileId t;
    struct FlumeEntry *entry;
    struct FlumeEntry *mid;
    struct FlumeEntry *node;
    struct FlumeEntry *prev;
    struct Ride *ride;
    int pos[2];
    int coords[2];
    int h;
    int midY;
    int last;
    int count;
    int i;

    int node_y;
    t.pos.x = pt[0];
    t.pos.y = pt[1];
    ride = elem->ride;
    AddBasicObject(elem, pt);
    AddFlumeEntry(t);
    entry = FindFlumeEntryByTile(&t);
    if (entry != NULL) {
        h = LogFlumeFootprint.y1 - LogFlumeFootprint.y0;
        midY = t.pos.y + ride->footprint.y0 + ((ride->footprint.y1 - ride->footprint.y0) >> 1);
        pos[0] = t.pos.x + ride->footprint.x0 + 1;
        entry->sub2 = (struct FlumeEntry *)FlumeQueueTable0;
        pos[1] = t.pos.y + ride->footprint.y0 - h;
        node = AllocFlumeNode();
        if (NULL != node) {
            node->mode = 3;
            node->submode = 0;
            node->ride = LogFlumeTrackRide;
            node->parent = entry;
            node->link28 = NULL;
            node->flags10d |= 3;
            node->tile.pos.x = pos[0] - LogFlumeFootprint.x0;
            node->tile.pos.y = pos[1] - LogFlumeFootprint.y0;
        }
        PushFlumeListNode((struct FlumeEntry *)entry, (struct ListNode *)node);
        coords[0] = node->tile.pos.x;
        coords[1] = node->tile.pos.y;
        memcpy(&LogFlumeTrackRide->footprint, &LogFlumeFootprint, sizeof(struct Footprint));
        LogFlumeTrackRide->footprint.x1--;
        LogFlumeTrackRide->footprint.y1--;
        AddBasicObject(LogFlumeTrackElement, coords);
        mid = AllocFlumeNode();
        mid->parent = entry;
        mid->ride = LogFlumeEntranceRide;
        mid->tile = entry->tile;
        PushFlumeListNode((struct FlumeEntry *)entry, (struct ListNode *)mid);
        entry->link = mid;
        LinkNodeAfter((struct FlumeEntry *)node, (struct FlumeEntry *)mid);
        count = (ride->footprint.y1 - ride->footprint.y0 + 1) / h;
        prev = node;
        coords[1] = pos[1] + h;
        last = count - 1;
        for (i = 0; i < count; i++) {
            node = AllocFlumeNode();
            if (node != NULL) {
                node->mode = 1;
                node->submode = 0;
                node->link28 = mid;
                node->ride = LogFlumeTrackRide;
                node->parent = entry;
                node->flags10d = node->flags10d | 7;
                node->tile.pos.x = pos[0] - LogFlumeFootprint.x0;
                node->tile.pos.y = coords[1] - LogFlumeFootprint.y0;
            }
            if (coords[1] <= midY) {
                if (coords[1] + h >= midY) {
                    entry->field_8 = node;
                    entry->field_c = prev;
                }
            }
            AppendListNode((struct FlumeEntry *)mid, (struct ListNode *)node);
            if (0 == i) {
                mid->link30 = node;
            }
            if (i == last) {
                mid->link34 = node;
            }
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
            prev = node;
            coords[1] += h;
        }
        coords[0] = t.pos.x + ride->footprint.x0 + 1;
        coords[1] = t.pos.y + ride->footprint.y1 - 1 + h;
        node = AllocFlumeNode();
        if (node != NULL) {
            node->mode = 3;
            node->ride = LogFlumeTrackRide;
            node->submode = 2;
            node->flags10d |= 3;
            node->link28 = NULL;
            node->parent = entry;
            node->tile.pos.x = coords[0] - LogFlumeFootprint.x0;
            node->tile.pos.y = coords[1] - LogFlumeFootprint.y0;
        }
        PushFlumeListNode((struct FlumeEntry *)entry, (struct ListNode *)node);
        node_y = node->tile.pos.y;
        coords[0] = node->tile.pos.x;
        coords[1] = node_y;
        AddBasicObject(LogFlumeTrackElement, coords);
        LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
        InitLogFlumeEntranceParticles((struct FlumeEntry *)entry);
    }
}

// FUNCTION: LEGOLAND 0x0040a930
void LogFlumeEntranceCalcCursor(Element *elem, int *param_2, unsigned int param_3) {
    struct Ride *ride;
    int x;
    int h;
    unsigned int y;

    unsigned int fy0;
    int fx0;
    int fy1;
    ride = elem->ride;
    fy1 = LogFlumeFootprint.y1;
    h = fy1 - LogFlumeFootprint.y0;

    ScreenToMapRef(param_2, &EditCursor.tile_x, param_3);
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&ride->footprint);
    PathCursor.tile_x = EditCursor.tile_x;
    PathCursor.tile_y = EditCursor.tile_y;
    PathCursor.footprint.y0 = EditCursor.footprint.y0 - 1;
    PathCursor.footprint.x0 = EditCursor.footprint.x0 + 3;
    EditCursor.next = &LogFlumeEntranceCursor;
    LogFlumeEntranceCursor.next = &LogFlumeEntranceCursor2;
    LogFlumeEntranceCursor2.next = &PathCursor;
    PathCursor.next = NULL;
    PathCursor.footprint.x1 = EditCursor.footprint.x1 + 1;
    PathCursor.footprint.y1 = 1 + EditCursor.footprint.y1;
    PathCursor.footprint.next = NULL;
    PathCursor.field_1828 = 0x1000;
    ResetCursorBuildable(&PathCursor);
    memcpy(LogFlumeEntranceCursor.footprint.v, &LogFlumeFootprint, sizeof(struct Footprint));
    LogFlumeEntranceCursor.footprint.x1 = LogFlumeFootprint.x1 - 1;
    LogFlumeEntranceCursor.footprint.y1 = LogFlumeEntranceCursor.footprint.y1 - 1;
    ResetCursorBuildable(&LogFlumeEntranceCursor);
    y = EditCursor.tile_y;
    x = EditCursor.tile_x;
    fx0 = LogFlumeEntranceRide->footprint.x0;
    LogFlumeEntranceCursor.tile_x = fx0 + x;
    fy0 = LogFlumeEntranceRide->footprint.y0;
    LogFlumeEntranceCursor.tile_y = fy0 + y - h;
    LogFlumeEntranceCursor.tile_x++;
    LogFlumeEntranceCursor2.tile_x = LogFlumeEntranceRide->footprint.x0 + x;
    LogFlumeEntranceCursor2.tile_y = LogFlumeEntranceRide->footprint.y1 + y - 1 + h;
    LogFlumeEntranceCursor2.tile_x++;
    memcpy(LogFlumeEntranceCursor2.footprint.v, LogFlumeEntranceCursor.footprint.v, 20);
    ResetCursorBuildable(&LogFlumeEntranceCursor2);
    ValidateCursor(&EditCursor, ride);
    PropagateCursorBlocked(&EditCursor);
}

// FUNCTION: LEGOLAND 0x0040aac0
void LogFlumeEntranceDCalcCursor(struct Element *param_1, struct Point *param_2) {
    struct FlumeEntry *entry;
    struct FlumeEntry *cur;

    BasicObjectDCalcCursor(param_1, param_2);
    DefaultCursor(&LogFlumeCursor);
    entry = FindFlumeEntryByTile(&QueryObj);
    if (entry != NULL) {
        for (cur = entry->sub; cur != NULL; cur = cur->next) {
            if (cur->field_28 != -1) {
                GetFlumePieceFootprint(cur, (struct Footprint **)&param_1, (TileId *)&param_2);
                if (param_1 == 0) {
                    continue;
                }
                memcpy(LogFlumeCursor.footprint.v, (void *)param_1, sizeof(struct Footprint)); /* param_1 holds the footprint */
                LogFlumeCursor.tile_x = cur->tile.pos.x;
                LogFlumeCursor.tile_y = cur->tile.pos.y;
            }
            LogFlumeCursor.field_1828 = 0x18;
            ResetCursorBuildable(&LogFlumeCursor);
            BuildCursorPtr(&LogFlumeCursor, 0, 0);
            RenderCursor(&LogFlumeCursor);
        }
        PathCursor.tile_x = LogFlumeCursor.tile_x;
        PathCursor.tile_y = LogFlumeCursor.tile_y;
        PathCursor.footprint.y0 = LogFlumeCursor.footprint.y0 - 1;
        PathCursor.footprint.x0 = LogFlumeCursor.footprint.x0 + 3;
        PathCursor.footprint.x1 = LogFlumeCursor.footprint.x1 + 1;
        PathCursor.footprint.y1 = LogFlumeCursor.footprint.y1 + 1;
        PathCursor.footprint.next = NULL;
        PathCursor.field_1828 = 0x1000;
        PathCursor.next = LogFlumeCursor.next;
        LogFlumeCursor.next = &PathCursor;
    }
}

// FUNCTION: LEGOLAND 0x0040abf0
void LogFlumeEntranceRemoveObject(Element *obj, TileId tile, struct Cursor *cursor_arg) {
    struct Cursor cursor;
    struct FlumeEntry *entry;
    struct FlumeEntry *cur;
    struct FlumeEntry *next;

    StandardRemoveObject(obj, tile, cursor_arg);
    entry = FindFlumeEntryByTile(&tile);
    if (entry != NULL) {
        cur = entry->sub;
        if (cur != NULL) {
            do {
                next = cur->next;
                memcpy(&LogFlumeTrackRide->footprint, &LogFlumeFootprint, sizeof(struct Footprint));
                LogFlumeTrackRide->footprint.x1--;
                LogFlumeTrackRide->footprint.y1--;
                cursor.tile_x = cur->tile.pos.x + LogFlumeFootprint.x0;
                cursor.tile_y = cur->tile.pos.y + LogFlumeFootprint.x1;
                memcpy(cursor.footprint.v, &LogFlumeTrackRide->footprint, sizeof(struct Footprint));
                StandardRemoveObject(LogFlumeTrackRide->element, cur->tile, &cursor);
                if (cur->flags10 & 2) {
                    UseBricks(GetObjCost(LogFlumeTrackRide));
                }
                FreeFlumeEntry((struct FlumeEntry *)entry, (struct FlumeEntry *)cur);
                cur = next;
            } while (next != NULL);
        }
        FreeAllQueueNodes((struct Queue *)&entry->sub2);
        RemoveFlumeEntry(entry);
        LogFlumeTrackRide->field_8++;
    }
    RemoveAllBlokesFromRide(obj->ride, tile);
}

// FUNCTION: LEGOLAND 0x0040ad50
int GetFlumeTrackSpriteIndex(struct FlumeEntry *node) {
    int result = 0;

    if (node != NULL) {
        switch (node->mode) {
        case 1:
            return FlumeTrackMode1Sprites[node->submode];
        case 2:
            return FlumeTrackMode2Sprites[node->submode];
        case 3:
            return FlumeTrackMode3Sprites[node->submode];
        case 4:
            result = FlumeTrackMode4Sprite;
        }
    }
    return result;
}

/* Maps screen y across the rect's vertical screen span (scaled) to 32..224, clamped. Nothing calls it. */
// FUNCTION: LEGOLAND 0x0040adb0
int ScaleYAcrossFlumeRect(TileId tile, struct FlumeRect *rect, int param_3, int y, float scale) {
    int bounds[4];
    struct Point pt;
    int top;
    float f;
    RECT rc;

    rc.left = rect->var_0 + tile.pos.x;
    rc.top = rect->var_4 + tile.pos.y;
    rc.right = rect->var_8 + tile.pos.x;
    rc.bottom = rect->var_c + tile.pos.y;
    pt.x = rc.left;
    pt.y = rc.top;
    GetTileBounds(&pt, bounds);
    top = bounds[1];
    pt.x = rc.right;
    pt.y = rc.bottom;
    GetTileBounds(&pt, bounds);
    f = (float)(y - top) / ((float)(bounds[3] - top + 1) * scale);
    if (f < FLOAT_004ab390) {
        f = FLOAT_004ab390;
    } else if (f > 1.0f) {
        f = 1.0f;
    }
    return 32 - (int)(f * -192.0f);
}

// FUNCTION: LEGOLAND 0x0040ae90
void DrawLogFlumeBarrel(struct FlumeSlot *slot, struct FlumeEntry *other, int drawBarrel) {
    /* Draws one log-flume barrel (and its rider, if any). The clip rect is narrowed to the
       edge of the neighbouring track tile `other` so the barrel is hidden behind it. */
    struct FlumeEntry *owner = slot->owner;
    struct Sprite *spriteA;
    struct Sprite *spriteB;
    RECT saved;
    RECT clip;
    struct Point p;

    if (slot->flags & 4) {
        spriteA = LogFlumeBarrelMatteSprite;
        spriteB = LogFlumeBarrel1Sprite;
    } else {
        spriteA = LogFlumeBarrelMSprite;
        spriteB = LogFlumeBarrelSprite;
    }
    GetClipping(&saved);
    clip = saved;
    if (other != owner) {
        int w;
        int h;
        int bounds[4];

        GetTileDimensions(&w, &h);
        if (owner->tile.pos.x == other->tile.pos.x) {
            p.x = other->tile.pos.x + 1;
            p.y = other->tile.pos.y;
            GetTileBounds(&p, bounds);
            if (bounds[2] < clip.right) {
                clip.right = bounds[2];
            }
            p.x = other->tile.pos.x + 1;
            p.y = other->tile.pos.y + 1;
            GetTileBounds(&p, bounds);
            if ((w >> 1) + bounds[0] > clip.left) {
                clip.left = (w >> 1) + bounds[0];
            }
        } else {
            p.x = other->tile.pos.x;
            p.y = other->tile.pos.y + 1;
            GetTileBounds(&p, bounds);
            if (bounds[0] + 1 > clip.left) {
                clip.left = bounds[0] + 1;
            }
            p.x = other->tile.pos.x + 1;
            p.y = other->tile.pos.y + 1;
            GetTileBounds(&p, bounds);
            if ((w >> 1) + bounds[0] + 1 < clip.right) {
                clip.right = (w >> 1) + bounds[0] + 1;
            }
        }
    }
    SetClipping(&clip);
    if (spriteB != NULL) {
        int w;
        int h;
        struct Point pos;
        struct Point tile;
        int bounds[4];
        struct Point q;

        pos.x = slot->x;
        pos.y = slot->y;
        GetTileDimensions(&w, &h);
        w <<= 1;
        h <<= 1;
        pos.x -= w >> 1;
        tile.x = owner->tile.pos.x;
        tile.y = owner->tile.pos.y;
        GetTileBounds(&tile, bounds);
        q.x = bounds[0];
        q.y = bounds[1];
        if (owner->link28 != NULL) {
            GetFlumePieceScreenPos(owner->link28);
        }
        if (owner->link28 == NULL) {
            GetFlumePieceScreenPos(owner);
        }
        p = pos;
        AdjustOffsetForViewMode(&p);
        /* centre the barrel sprite horizontally; lift it by 3/4 of its height plus half the
           slot's bob offset */
        pos.x = p.x - (spriteB->width >> 1);
        p.y -= (int)(((short)spriteB->height * 0.75f) + (int)(slot->field_1c >> 1));
        p.x = pos.x;
        if (drawBarrel != 0) {
            struct RideNode *node;

            PrintSprite(spriteB, p.x + q.x, p.y + q.y, 0, 0);
            node = slot->busy;
            if (node != NULL && (node->rider->flags & 0x80)) {
                struct Person *person = node->person;

                if (person != NULL) {
                    struct Point bp;

                    bp.x = p.x + (spriteB->width >> 1);
                    bp.y = p.y + ((short)spriteB->height >> 2) + ((short)spriteB->height >> 1);
                    AdjustBlokePosition(&bp);
                    SetPersonPosition(person, bp.x + q.x, bp.y + q.y);
                    SetPersonDirection(person, GetFlumeSlotDirection((struct FlumeSlot *)slot));
                    IP_RenderBlokeIn3DNow((slot->busy)->rider);
                }
            }
        }
        if (spriteA != NULL) {
            int frame;
            struct LLS *lls;

            frame = 0;
            lls = GetLLSForSprite((struct Sprite *)spriteB);
            if (lls != NULL) {
                frame = lls->frame;
            }
            lls = GetLLSForSprite((struct Sprite *)spriteA);
            if (lls != NULL) {
                LLSSetFrame(lls, frame);
            }
            PrintSprite(spriteA, p.x + q.x, p.y + q.y, 0, 0);
        }
    }
    SetClipping(&saved);
}

// FUNCTION: LEGOLAND 0x0040b210
int IsFlumeSlotOnPiece(struct FlumeSlot *self, struct FlumeEntry *other) {
    struct FlumeEntry *link = self->owner;
    if (link == other) {
        return 1;
    }
    if (self->weight >= DOUBLE_004ab398 && link->parent8 == other) {
        return 1;
    }
    if (self->weight < DOUBLE_004ab398 && link->field_c == other) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040b270
int IsFlumeSlotOwnedBy(struct FlumeSlot *param_1, void *param_2) {
    return (void *)param_1->owner == param_2;
}

/* Walks the flume run in either direction, draws barrel slots on each piece, and interleaves the stage sprites at
 * their recorded counts. */
// FUNCTION: LEGOLAND 0x0040b290
void DrawFlumeBarrelsByStage(struct FlumeEntry *entry, int x, int y, struct FlumeStageList *list, int reverse) {
    struct FlumeEntry *set = (struct FlumeEntry *)entry->slotset;
    int i = 0;
    int j;
    int off;
    struct FlumeRunNode *node;
    struct FlumeStage *stage;

    if (reverse == 0) {
        node = entry->first;
    } else {
        node = entry->last;
    }
    stage = (struct FlumeStage *)list->stages;
    if (node != NULL) {
        off = 0;
        do {
            for (j = 0; j < set->count; j++) {
                if (IsFlumeSlotOnPiece(&set->slots[j], (struct FlumeEntry *)node)) {
                    DrawLogFlumeBarrel(&set->slots[j], (struct FlumeEntry *)node, 1);
                }
            }
            if (reverse == 0) {
                node = node->next;
            } else {
                node = node->prev;
            }
            i++;
            if (i >= stage->count) {
                if (stage->sprite != NULL) {
                    PrintSprite(stage->sprite, x, y, 0, 0);
                }
                off += sizeof(struct FlumeStage);
                stage = (struct FlumeStage *)(list->stages + off);
            }
        } while (node != NULL);
    }
    if (stage->sprite != NULL) {
        PrintSprite(stage->sprite, x, y, 0, 0);
    }
}

// FUNCTION: LEGOLAND 0x0040b390
int DrawFlumeBarrelsOnPiece(struct FlumeEntry *entry) {
    struct FlumeEntry *set = (struct FlumeEntry *)entry->slotset;
    int matched = 0;
    int i;
    struct FlumeEntry *node;

    for (i = 0; i < set->count; i++) {
        node = entry->sub2;
        if (node != NULL) {
            do {
                if (IsFlumeSlotOnPiece(&set->slots[i], (struct FlumeEntry *)node)) {
                    DrawLogFlumeBarrel(&set->slots[i], node, 1);
                    matched++;
                }
                node = node->next;
            } while (node != NULL);
        } else {
            if (IsFlumeSlotOnPiece(&set->slots[i], (struct FlumeEntry *)entry)) {
                DrawLogFlumeBarrel(&set->slots[i], entry, 1);
                matched++;
            }
        }
    }
    return matched;
}

// FUNCTION: LEGOLAND 0x0040b420
void RenderLogFlumeEntrance(Element *elem, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = elem->ride;
    struct Point base;
    struct FlumeEntry *entry;
    struct Point coords;
    struct Point pos;
    struct RideNode *node;
    struct LLS *lls;
    struct Sprite *sprite;
    int ok;

    base.x = ride->x + tile->pos.x;
    base.y = ride->y + tile->pos.y;
    entry = FindFlumeEntryByTile(tile);
    coords = GetScreenCoordsForObject(tile, ride);
    if (entry != NULL) {
        ok = DrawFlumeBarrelsOnPiece(entry->link);
    }
    if (ok) {
        struct Point off;
        int frame;
        lls = GetLLSForSprite((struct Sprite *)GetSpriteForLayer(ride->layer, 0));
        if (lls != NULL) {
            frame = lls->frame;
        }
        lls = GetLLSForSprite((struct Sprite *)LogFlumeEnta3MSprite);
        if (lls != NULL) {
            LLSSetFrame(lls, frame);
        }
        pos = GetScreenCoordsForObject(tile, ride);
        off = GetRenderOffsetForLayer(ride->layer, 0);
        AdjustOffsetForViewMode(&off);
        if (LogFlumeEnta3MSprite != NULL) {
            PrintSprite(LogFlumeEnta3MSprite, pos.x + off.x, pos.y + off.y, clip, 0);
        }
    }
    {
        struct Point off;
        sprite = GetSpriteForLayer(ride->layer, 1);
        pos = GetScreenCoordsForObject(tile, ride);
        off = GetRenderOffsetForLayer(ride->layer, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite(sprite, pos.x + off.x, pos.y + off.y, clip, 0);
    }

    RenderItems_New();
    FlumeRiderRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && !(node->rider->flags & 0x80) && node->rider->pos.x <= (base.x << 8) - 0x280) {
            AddBlokeToRenderList(&FlumeRiderRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&FlumeRiderRenderList);

    RenderItems_New();
    FlumeRiderRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && !(node->rider->flags & 0x80) && node->rider->pos.x > (base.x << 8) - 0x280 && (node->rider->pos.y >> 8) <= base.y - 9) {
            AddBlokeToRenderList(&FlumeRiderRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&FlumeRiderRenderList);
    if (LogFlumeEntrance3Sprite != NULL) {
        struct Point off;
        int frame;
        off.x = -100;
        off.y = -0xd1;
        lls = NULL;
        sprite = GetSpriteForLayer(ride->layer, 2);
        if (sprite != NULL) {
            lls = GetLLSForSprite((struct Sprite *)sprite);
        }
        if (lls != NULL) {
            frame = lls->frame;
        }
        AdjustOffsetForViewMode(&off);
        lls = GetLLSForSprite((struct Sprite *)LogFlumeEntrance3Sprite);
        if (lls != NULL) {
            LLSSetFrame(lls, frame);
        }
        PrintSprite(LogFlumeEntrance3Sprite, coords.x + off.x, coords.y + off.y, clip, 0);
    }

    RenderItems_New();
    FlumeRiderRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && !(node->rider->flags & 0x80) && node->rider->pos.x > (base.x << 8) - 0x280) {
            int y = node->rider->pos.y >> 8;
            if (y == base.y - 6 || y == base.y - 7 || y == base.y - 8) {
                AddBlokeToRenderList(&FlumeRiderRenderList, node, node->person->field_20);
            }
        }
    }
    RenderBlokeList((struct RenderItemNode **)&FlumeRiderRenderList);
    if (LogFlumeEntrance1Sprite != NULL) {
        struct Point off;
        int frame;
        off.x = 8;
        off.y = -0x71;
        lls = NULL;
        sprite = GetSpriteForLayer(ride->layer, 2);
        if (sprite != NULL) {
            lls = GetLLSForSprite((struct Sprite *)sprite);
        }
        if (lls != NULL) {
            frame = lls->frame;
        }
        AdjustOffsetForViewMode(&off);
        lls = GetLLSForSprite((struct Sprite *)LogFlumeEntrance1Sprite);
        if (lls != NULL) {
            LLSSetFrame(lls, frame);
        }
        PrintSprite(LogFlumeEntrance1Sprite, coords.x + off.x, coords.y + off.y, clip, 0);
    }

    RenderItems_New();
    FlumeRiderRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && !(node->rider->flags & 0x80) && node->rider->pos.x > (base.x << 8) - 0x280 && (node->rider->pos.y >> 8) == base.y - 3) {
            AddBlokeToRenderList(&FlumeRiderRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&FlumeRiderRenderList);
    if (LogFlumeEntrance2Sprite != NULL) {
        struct Point off;
        int frame;
        off.x = -100;
        off.y = -0xd1;
        lls = NULL;
        sprite = GetSpriteForLayer(ride->layer, 2);
        if (sprite != NULL) {
            lls = GetLLSForSprite((struct Sprite *)sprite);
        }
        if (lls != NULL) {
            frame = lls->frame;
        }
        AdjustOffsetForViewMode(&off);
        lls = GetLLSForSprite((struct Sprite *)LogFlumeEntrance2Sprite);
        if (lls != NULL) {
            LLSSetFrame(lls, frame);
        }
        PrintSprite(LogFlumeEntrance2Sprite, coords.x + off.x, coords.y + off.y, clip, 0);
    }

    RenderItems_New();
    FlumeRiderRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && !(node->rider->flags & 0x80) && node->rider->pos.x > (base.x << 8) - 0x280) {
            int y = node->rider->pos.y >> 8;
            if (y == base.y - 2 || y == base.y - 1 || y == base.y) {
                AddBlokeToRenderList(&FlumeRiderRenderList, node, node->person->field_20);
            }
        }
    }
    RenderBlokeList((struct RenderItemNode **)&FlumeRiderRenderList);
    if (LogFlumeSignSprite != NULL) {
        struct Point off;
        int frame;
        off.x = -100;
        off.y = -0xd1;
        AdjustOffsetForViewMode(&off);
        lls = NULL;
        sprite = GetSpriteForLayer(ride->layer, 2);
        if (sprite != NULL) {
            lls = GetLLSForSprite((struct Sprite *)sprite);
        }
        if (lls != NULL) {
            frame = lls->frame;
        }
        lls = GetLLSForSprite((struct Sprite *)LogFlumeSignSprite);
        if (lls != NULL) {
            LLSSetFrame(lls, frame);
        }
        PrintSprite(LogFlumeSignSprite, coords.x + off.x, coords.y + off.y, clip, 0);
    }
    if (LogFlumeEnta1Matte2Sprite != NULL) {
        struct Point off;
        off = GetRenderOffsetForLayer(ride->layer, 2);
        AdjustOffsetForViewMode(&off);
        PrintSprite(LogFlumeEnta1Matte2Sprite, coords.x + off.x, coords.y + off.y, clip, 0);
    }
}

// FUNCTION: LEGOLAND 0x0040ba80
int IsFlumeLoopClosed(struct FlumeEntry *arg) {
    struct FlumeEntry *current = arg->parent8;
    if (current != NULL) {
        struct FlumeEntry *target = arg->field_c;
        while (current != NULL) {
            current = current->parent8;
            if (current == target) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040bab0
int IsFlumeSlotClearAhead(struct FlumeEntry *set, int index) {
    struct FlumeSlot *slot = &set->slots[index];
    struct FlumeSlot *other;
    struct FlumeEntry *link;
    float weight;
    float f;
    int i;

    if (slot != NULL) {
        link = slot->owner;
        if (slot->owner->parent8 != NULL) {
            weight = slot->weight;
            for (i = 0, other = set->slots; i < set->count; i++, other++) {
                if (i != index) {
                    if (other->owner == link || other->owner == link->parent8) {
                        if (other->owner == link) {
                            f = other->weight - weight;
                        }
                        if (other->owner == link->parent8) {
                            f = (float)(other->weight + 1.0 - weight);
                        }
                        if (f > FLOAT_004ab390 && f < 0.8) {
                            return 0;
                        }
                    }
                }
            }
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040bb50
int FindFreeFlumeStationSlot(struct FlumeEntry *set, struct FlumeSlot **out) {
    int i;
    int count = set->count;
    struct FlumeSlot *slot;

    for (i = 0; i < count; i++) {
        slot = &set->slots[i];
        if (slot->owner == set->parent8->parent8 && slot->busy == 0 && !(slot->flags & 1)) {
            *out = slot;
            return 1;
        }
    }
    *out = NULL;
    return 0;
}

// FUNCTION: LEGOLAND 0x0040bbb0
void UpdateFlumeSlot(struct FlumeEntry *set, int index) {
    struct FlumeSlot *slot = &set->slots[index];

    if (slot != NULL) {
        if (slot->flags & 2) {
            UpdateFlumeDropSlot((struct FlumeSlot *)slot);
            return;
        }
        if (slot->flags & 1) {
            if (slot->owner->parent8 != NULL) {
                if (IsFlumeSlotClearAhead(set, index)) {
                    if (AdvanceFlumeMover((struct FlumeSlot *)slot)) {
                        slot->owner = slot->owner->parent8;
                        if (IsFlumeSlotOnDrop(slot)) {
                            slot->flags |= 2;
                            return;
                        }
                        if (slot->owner == set->field_c->parent8) {
                            if (slot->busy != NULL) {
                                slot->busy->rider->param_action++;
                                slot->busy = NULL;
                                slot->timer = 0x32;
                                slot->flags &= ~1;
                            }
                        }
                        if (slot->owner == set->parent8->parent8) {
                            slot->timer = 0x32;
                            slot->flags &= ~1;
                        }
                    }
                }
            }
        } else {
            slot->timer--;
            if (slot->timer < 0) {
                if (!(set->flags & 1)) {
                    if (slot->owner->parent8 != NULL) {
                        if (IsFlumeSlotClearAhead(set, index)) {
                            slot->flags |= 1;
                            if (AdvanceFlumeMover((struct FlumeSlot *)slot)) {
                                slot->owner = slot->owner->parent8;
                                if (slot->owner == set->field_c->parent8) {
                                    if (slot->busy != NULL) {
                                        slot->busy->rider->param_action++;
                                        slot->busy = NULL;
                                        slot->timer = 0x32;
                                        slot->flags &= ~1;
                                    }
                                }
                                if (slot->owner == set->parent8->parent8) {
                                    slot->timer = 0x32;
                                    slot->flags &= ~1;
                                }
                            }
                        }
                    }
                }
                slot->timer = 1;
            }
        }
    }
}

/* Advances one boat slot when the piece ahead is clear, releases its rider at the exit, and maintains the stop
 * timer and movement flag. */
// FUNCTION: LEGOLAND 0x0040bd40
void UpdateFlumeBoatSlot(struct FlumeEntry *set, int index) {
    struct FlumeSlot *slot = &set->slots[index];

    if (slot != NULL) {
        if (slot->flags & 1) {
            if (slot->owner == set->field_c) {
                if (slot->busy != NULL) {
                    slot->busy->rider->param_action++;
                    slot->busy = NULL;
                }
            }
            if (slot->owner == set->parent8) {
                slot->timer = 0x32;
                slot->flags &= ~1;
            } else if (slot->owner->parent8 != NULL) {
                if (IsFlumeSlotClearAhead(set, index)) {
                    slot->owner = slot->owner->parent8;
                }
            }
        } else {
            slot->timer--;
            if (slot->timer < 0) {
                if (!(set->flags & 1)) {
                    if (slot->owner->parent8 != NULL) {
                        if (IsFlumeSlotClearAhead(set, index)) {
                            slot->flags |= 1;
                            slot->owner = slot->owner->parent8;
                        }
                    }
                }
                slot->timer = 1;
            }
        }
    }
}

/* Per-entry update used by LogFlumeEntranceUpdate: advance its boat slots, hand
   over queued riders, and update the queue's admission flag and walking positions. */
// FUNCTION: LEGOLAND 0x0040be00
void UpdateLogFlumeEntranceEntry(struct FlumeEntry *set) {
    struct FlumeSlot *slot;
    struct RideNode *rider_node; /* the rider the queue hands over */
    int slot_index;

    /* Flag 2 expires after the entry's configured tick limit. */
    if (set->flags & 2) {
        if (++set->ticks >= set->field_28) {
            set->flags &= ~2;
        }
    }
    /* Only circulate boats on connected track; service the slots every third update. */
    if (IsFlumeLoopClosed((struct FlumeEntry *)set)) {
        set->timer--;
        if (set->timer < 0) {
            set->timer = 2;
            for (slot_index = 0; slot_index < set->count; slot_index++) {
                UpdateFlumeSlot(set, slot_index);
            }
            if (!(set->flags & 1)) {
                slot = NULL;
                if (FindFreeFlumeStationSlot(set, &slot)) {
                    /* Transfer the next ready queued rider to an available boarding slot. */
                    rider_node = NULL;
                    set->target = slot;
                    if (QueueHasNodes(&set->queue)) {
                        if (IsQueueHeadAtFront(&set->queue)) {
                            set->flags |= 1;
                            LeaveQueueHead(&set->queue, (struct RideNode **)&rider_node);
                            slot->busy = rider_node;
                        }
                    }
                }
            }
        }
    } else {
        ResetLogFlumeEntranceParticles((struct FlumeEntry *)set);
    }
    {
        /* Calculate both world coordinates before the queue call. The Point preserves
           MSVC6's original register allocation; a cached ride pointer does not. */
        Point queue_position;
        queue_position.x = LogFlumeEntranceRide->x + set->tile.pos.x;
        queue_position.y = LogFlumeEntranceRide->y + set->tile.pos.y;
        StepQueueRiders(&set->queue, queue_position.x, queue_position.y);
    }
    /* Prevent new arrivals while the entrance queue is full. */
    if (IsQueueFull(&set->queue)) {
        Ride_SetFlagToNotLetAnyoneOn(&set->tile);
    } else {
        Ride_ClearFlagToNotLetAnyoneOn(&set->tile);
    }
    /* Advance the entry's phase modulo 16. */
    set->submode = (set->submode + 1) & 0xf;
}

// FUNCTION: LEGOLAND 0x0040bf50
void UpdateAllLogFlumeEntrances(void) {
    struct FlumeEntry *current = FlumeEntryList;
    while (current != NULL) {
        UpdateLogFlumeEntranceEntry((struct FlumeEntry *)current);
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0040bf70
void LogFlumeEntranceUpdate(Element *obj) {
    int target_dir;
    Ride *ride = obj->ride;
    RideNode *elem;
    Bloke *bloke;
    RideNode *next;
    TileId *tile;
    struct FlumeEntry *entry;
    unsigned int x;
    char dir;
    unsigned int y;
    int v;
    elem = ride->riders;

    UpdateAllLogFlumeEntrances();
    while (NULL != elem) {
        bloke = elem->rider;
        tile = &elem->tile;
        x = ride->field_24 + tile->pos.x;
        next = elem->next;
        y = ride->field_25 + tile->pos.y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                entry = FindFlumeEntryByTile(tile);
                if (entry != NULL) {
                    JoinQueue(&entry->queue, (struct RideNode *)elem);
                    if (IsQueueFull(&entry->queue)) {
                        Ride_SetFlagToNotLetAnyoneOn(&entry->tile);
                    }
                }
                break;
            case 2:
                v = (tile->pos.x - 2) << 8;
                bloke->dest.x = v;
                v = tile->pos.y << 8;
                bloke->dest.y = v;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 3:
                v = (tile->pos.x << 8) - 0x280;
                bloke->dest.x = v;
                v = (tile->pos.y << 8) + 0x80;
                bloke->dest.y = v;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                bloke->flags = bloke->flags | 0x80;
                BlokeWalkAnim(bloke);
                BlokeSetFrame(bloke, 0);
                entry = FindFlumeEntryByTile(tile);
                if (entry != NULL) {
                    if (NULL != entry->target) {
                        *(int *)entry->target = 1;
                        entry->target = NULL;
                    }
                    entry->flags &= ~1;
                }
                bloke->param_action++;
                break;
            case 6:
                bloke->flags &= ~0x80;
                BlokeWalkAnim(bloke);
                BlokeSetFrame(bloke, 0);
                bloke->pos.x = (tile->pos.x << 8) - 0x280;
                bloke->pos.y = (tile->pos.y << 8) - 0x80;
                bloke->dir = 3;
                bloke->dest.x = (tile->pos.x - 2) << 8;
                bloke->dest.y = (tile->pos.y << 8) - 0x100;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                target_dir = bloke->target_dir;
                NewDirForAction(bloke, (target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 7:
                v = tile->pos.x << 8;
                bloke->dest.x = v;
                v = (tile->pos.y << 8) - 0x100;
                bloke->dest.y = v;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = 0x10 + dir;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 8:
                StartQueueTableBackward((struct QueueTable *)FlumeQueueTable1, (struct Bloke *)bloke);
                break;
            case 9:
                MoveBlokeAlongQueueTable((struct QueueTable *)FlumeQueueTable1, x, y, bloke);
                break;
            case 10:
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = ((tile->pos.y + ride->field_25) << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 11:
                RemoveBlokeFromRide(ride, elem);
                bloke->flags = bloke->flags & (~8);
                break;
            }
        }
        elem = next;
    }
}

/* Retargets slots owned by this entry or its sub-piece chain to the next or alternate entry, preferring that
 * destination's sub-piece. Returns zero if no destination is found. */
// FUNCTION: LEGOLAND 0x0040c250
int TransferFlumeSlotsToNextPiece(struct FlumeEntry *entry) {
    struct FlumeEntry *set = (struct FlumeEntry *)entry->slotset;
    /* Original bug: target is read uninitialised when entry has no alt/next (its stack slot then holds entry). */
    struct FlumeEntry *target;
    struct FlumeEntry *tmp;
    struct FlumeEntry *node;
    int i;

    tmp = entry->alt;
    if (tmp == NULL) {
        tmp = entry->next;
    }
    if (tmp != NULL) {
        target = tmp;
    }
    if (target == NULL) {
        return 0;
    }
    tmp = target->sub2;
    if (tmp != NULL) {
        target = tmp;
    }
    for (i = 0; i < set->count; i++) {
        node = entry->sub2;
        if (node == NULL) {
            if (!IsFlumeSlotOwnedBy(&set->slots[i], entry)) {
                continue;
            }
        } else {
            do {
                if (IsFlumeSlotOwnedBy(&set->slots[i], node)) {
                    break;
                }
                node = node->next;
            } while (node != NULL);
            if (node == NULL) {
                continue;
            }
        }
        set->slots[i].owner = (struct FlumeEntry *)target;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0040c2e0
int HasFlumeBarrelOnPiece(struct FlumeEntry *owner) {
    struct FlumeEntry *set = owner->parent;
    int i;
    struct FlumeEntry *node;

    for (i = 0; i < set->count; i++) {
        node = owner->sub2;
        if (node == NULL) {
            if (IsFlumeSlotOnPiece(&set->slots[i], (struct FlumeEntry *)owner)) {
                return 1;
            }
        } else {
            do {
                if (IsFlumeSlotOnPiece(&set->slots[i], (struct FlumeEntry *)node)) {
                    return 1;
                }
                node = node->next;
            } while (node != NULL);
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040c350
void LogFlumeTrackLoad(Element *elem) {
    int i;

    LogFlumeTrackRide = elem->ride;
    LogFlumeTrackRide->flags |= 0x400;
    LogFlumeTrackRide->field_18 = 0;
    LogFlumeTrackRide->field_14 = 0;
    LogFlumeTrackElement = elem;
    // STRING: LEGOLAND 0x004b4a0c
    if (LLIDB_FindElement("LOG FLUME IMAGE LIST", &LogFlumeImageListId, 0) == 0) {
        LogFlumeImageList = LLIDB_LoadData((void *)LogFlumeImageListId);
    }
    // STRING: LEGOLAND 0x004b49f0
    if (LLIDB_FindElement("LOG FLUME TRACK ENDY LIST", &LogFlumeTrackEndyListId, 0) == 0) {
        LogFlumeTrackEndyList = LLIDB_LoadData((void *)LogFlumeTrackEndyListId);
    }
    InitLogFlumeShapes();
    for (i = 0; i < 10; i++) {
        LogFlumeTrackSprites[i] = LoadSprite(LogFlumeTrackSpriteFiles[i], 1);
    }
    // STRING: LEGOLAND 0x004b49e4
    LogFlumeFc1M3Sprite = LoadSprite("fc1_m3.lls", 1);
    // STRING: LEGOLAND 0x004b49d8
    LogFlumeFc3M3Sprite = LoadSprite("fc3_m3.lls", 1);
}

// FUNCTION: LEGOLAND 0x0040c430
void LogFlumeTrackUnload(void) {
    struct Sprite **ptr;

    LLIDB_UnLoadData(LogFlumeImageListId);

    ptr = LogFlumeTrackSprites;
    while ((INT_PTR)ptr < (INT_PTR)&LogFlumeTrackSpritesEnd) {
        if (*ptr != NULL) {
            KillSprite(*ptr);
        }
        ptr++;
    }

    if (LogFlumeFc1M3Sprite != NULL) {
        KillSprite(LogFlumeFc1M3Sprite);
    }
    if (LogFlumeFc3M3Sprite != NULL) {
        KillSprite(LogFlumeFc3M3Sprite);
    }

    LLIDB_UnLoadData(LogFlumeTrackEndyListId);
}

// FUNCTION: LEGOLAND 0x0040c4a0
void LogFlumeTrackCalcCursor(Element *elem, int *param_2, unsigned int param_3) {
    struct Ride *ride = elem->ride;
    struct MapRect rect;
    TileId t;
    struct FlumeEntry **list;
    struct FlumeEntry *p;
    struct FlumeEntry *key;
    int r;
    int cost;
    int code;

    memcpy(&EditCursor.footprint, &LogFlumeFootprint, sizeof(struct Footprint));
    EditCursor.footprint.x1 = LogFlumeFootprint.x1 - 1;
    EditCursor.footprint.y1--;
    ScreenToMapRef(param_2, &EditCursor.tile_x, param_3);
    ResetCursorBuildable(&EditCursor);
    EditCursor.next = NULL;
    ValidateCursor(&EditCursor, ride);
    if (IsCursorBuildable(&EditCursor)) {
        rect.x0 = EditCursor.footprint.x0 + EditCursor.tile_x;
        rect.y0 = EditCursor.tile_y + EditCursor.footprint.y0;
        rect.x1 = EditCursor.footprint.x1 + EditCursor.tile_x;
        rect.y1 = EditCursor.footprint.y1 + EditCursor.tile_y;
        r = CheckForPeople(&rect);
        switch (r) {
        case -1:
            SetCursorBlocked(&EditCursor, 4);
            break;
        case 1:
            SetCursorBlocked(&EditCursor, 3);
            break;
        }
    }
    cost = GetObjCost(ride);
    if (GetBrickCount() < cost) {
        SetCursorBlocked(&EditCursor, 2);
    }
    if (IsCursorBuildable(&EditCursor)) {
        t.pos.x = LogFlumeFootprint.x0 + EditCursor.tile_x;
        t.pos.y = LogFlumeFootprint.y0 + EditCursor.tile_y;
        GetFlumeNeighbours(t, (void **)&list);
        DropNonEndFlumeTrackNeighbours((struct StateSlots *)list);
        if (CountFlumeNeighbours((unsigned int *)list) == 0) {
            SetCursorBlocked(&EditCursor, 0xe);
        } else {
            SetCursorBlocked(&EditCursor, 0xe);
            p = list[0];
            if (p != NULL) {
                key = p->parent;
            } else {
                p = list[1];
                if (p != NULL) {
                    key = p->parent;
                } else {
                    p = list[2];
                    if (p != NULL) {
                        key = p->parent;
                    } else {
                        p = list[3];
                        if (p != NULL) {
                            key = p->parent;
                        }
                    }
                }
            }
            DropForeignFlumeTrackNeighbours(key, list);
            code = FlumeNeighbourMask(list);
            if (CanPlaceLogFlumeTrack(code, (struct StateSlots *)list)) {
                AppendFlumeNeighbourCursors((struct FlumeEntry **)list, &EditCursor);
                ResetCursorBuildable(&EditCursor);
            }
        }
    }
    PropagateCursorBlocked(&EditCursor);
}

// FUNCTION: LEGOLAND 0x0040c6c0
void LogFlumeTrackDCalcCursor(struct Element *unused, struct Point *pt) {
    struct FlumeEntry *entry = FindFlumePieceAt(pt->x, pt->y);
    unsigned int v;

    if (entry != NULL) {
        SetCursorBlocked(&QueryCursor, 1);
        QueryCursor.tile_x = entry->tile.pos.x;
        QueryCursor.tile_y = entry->tile.pos.y;
        v = LogFlumeFootprint.x1;
        memcpy(&QueryCursor.footprint, &LogFlumeFootprint, sizeof(struct Footprint));
        QueryCursor.footprint.x1 = v - 1;
        QueryCursor.footprint.y1 = QueryCursor.footprint.y1 - 1;
        QueryCursor.field_1828 = 8;
        if (IsJoinedToFlume((struct FlumeEntry *)entry)) {
            if ((entry->field_c == NULL || entry->field_8 == NULL || IsFlumeLoopClosed((struct FlumeEntry *)entry->parent)) && !(entry->flags10d & 1)) {
                ResetCursorBuildable(&QueryCursor);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0040c780
void LogFlumeTrackAddObject(struct Element *unused, struct Point *pt) {
    TileId t;
    int coords[2];
    struct FlumeEntry *entry;
    struct FlumeEntry **list;
    struct FlumeEntry *p;
    struct FlumeEntry *key;

    t.pos.x = pt->x;
    t.pos.y = pt->y;
    coords[0] = t.pos.x;
    coords[1] = t.pos.y;
    entry = AllocFlumeNode();
    if (entry != NULL) {
        entry->ride = LogFlumeTrackRide;
        entry->link28 = NULL;
        entry->tile.pos.x = LogFlumeFootprint.x0 + coords[0];
        entry->tile.pos.y = LogFlumeFootprint.y0 + coords[1];
        GetFlumeNeighbours(t, (void **)&list);
        DropNonEndFlumeTrackNeighbours((struct StateSlots *)list);
        p = list[0];
        if (p != NULL) {
            key = p->parent;
        } else {
            p = list[1];
            if (p != NULL) {
                key = p->parent;
            } else {
                p = list[2];
                if (p != NULL) {
                    key = p->parent;
                } else {
                    p = list[3];
                    if (p != NULL) {
                        key = p->parent;
                    }
                }
            }
        }
        AddFlumeScore((struct FlumeEntry *)key, 1);
        if (key != 0) {
            entry->parent = (struct FlumeEntry *)key;
            DropForeignFlumeTrackNeighbours(key, list);
            PushFlumeListNode((struct FlumeEntry *)key, (struct ListNode *)entry);
            memcpy(&LogFlumeTrackRide->footprint, &LogFlumeFootprint, sizeof(struct Footprint));
            LogFlumeTrackRide->footprint.x1--;
            LogFlumeTrackRide->footprint.y1--;
            AddBasicObject(LogFlumeTrackElement, coords);
            InsertFlumePieceIntoChain(entry, list);
            SetFlumePieceShapeFromNeighbours(entry, (struct StateSlots *)list);
        }
    }
}

// FUNCTION: LEGOLAND 0x0040c8d0
void LogFlumeTrackRemoveObject(Element *elem, TileId tile, struct Cursor *cursor) {
    struct FlumeEntry *entry;

    memcpy(&LogFlumeTrackRide->footprint, &LogFlumeFootprint, sizeof(struct Footprint));
    --LogFlumeTrackRide->footprint.x1;
    --LogFlumeTrackRide->footprint.y1;
    StandardRemoveObject(elem, tile, cursor);
    entry = FindFlumeSubEntryByTile(&tile);
    AddFlumeScore((struct FlumeEntry *)entry->parent, -1);
    if (entry != NULL) {
        GetFlumeNeighbours(tile, (void **)&cursor);
        DropUnlinkedFlumeNeighbours((struct FlumeEntry *)entry, (struct LinkList *)cursor);
        DetachFlumeNeighbourShapes(entry, (struct FlumeEntry **)cursor);
        FreeFlumeEntry((struct FlumeEntry *)entry->parent, (struct FlumeEntry *)entry);
    }
}

// FUNCTION: LEGOLAND 0x0040c970
struct RideSpriteInfo *GetLogFlumeTrackSpriteInfo(struct Element *unused, TileId tile) {
    struct FlumeEntry *entry;
    struct Sprite *spr;
    struct LLS *lls;
    int idx;

    LogFlumeTrackSpriteInfo.id = tile.id;
    entry = FindFlumePieceAtTile((struct SubBuf *)&tile);
    if (entry != NULL) {
        if (entry->flags10d & 4) {
            return NULL;
        }
        idx = GetFlumeTrackSpriteIndex((struct FlumeEntry *)entry);
        spr = LogFlumeImageList->sprites[(unsigned char)idx];
        LogFlumeTrackSpriteInfo.sprite = spr;
        LogFlumeTrackSpriteInfo.x = LogFlumeImageList->offset_x[(unsigned char)idx] >> 1;
        LogFlumeTrackSpriteInfo.y = LogFlumeImageList->offset_y[(unsigned char)idx] >> 1;
        LogFlumeTrackSpriteInfo.field_10 = 0;
        lls = GetLLSForSprite((struct Sprite *)spr);
        if (lls != NULL) {
            LLSStop(lls);
            LLSSetFrame(lls, entry->parent->submode);
        }
        ((struct Sprite *)LogFlumeTrackSpriteInfo.sprite)->flags |= 0x2000;
        LogFlumeTrackSpriteInfo.field_10 = 0;
    }
    return &LogFlumeTrackSpriteInfo;
}

// FUNCTION: LEGOLAND 0x0040ca30
void DrawFlumeBarrelList(void *a1, struct FlumeEntry *a2) {
    struct WalkNode *node = *(struct WalkNode **)a1;
    if (node != NULL) {
        do {
            if (node->var_4 != 0) {
                DrawLogFlumeBarrel(node->var_4, a2, 1);
            }
            node = node->var_8;
        } while (node != NULL);
    }
}

/* A corner of the log flume (track sprites fc1-fc4; this draws corners 0 and 2, which have the extra matte
 * fc1_m3/fc3_m3): the boats on the corner go behind or in front of the matte by their weight along the track,
 * then the corner track sprite. arg (the caller's clip) is used only for corner 0 when the next piece is in
 * another column. */
// FUNCTION: LEGOLAND 0x0040ca60
int RenderLogFlumeCorner(struct FlumeEntry *entry, int arg) {
    int count = 0;
    struct FlumeEntry *par = entry->parent;
    int i;
    struct FlumeSlot *slot;
    double w;
    struct FlumeEntry *next;
    unsigned char thisX;
    struct Sprite *spr;
    struct Point pos;
    int idx;

    RenderItems2_New();
    FlumeCornerBarrelsLowWeight = NULL;
    FlumeCornerBarrelsHighWeight = NULL;
    /* sort the slots at this corner into the lists drawn before / after the corner sprite */
    for (i = 0; i < par->count; i++) {
        slot = &par->slots[i];
        if (IsFlumeSlotOnPiece(slot, (struct FlumeEntry *)entry)) {
            w = slot->weight;
            if (slot->owner != entry) {
                if (entry->field_c == slot->owner) {
                    w = slot->weight - 1.0;
                }
                if (entry->field_8 == slot->owner) {
                    w = slot->weight + 1.0;
                }
            }
            if (w <= DOUBLE_004ab398) {
                RenderItem2_AddItem(&FlumeCornerBarrelsLowWeight, slot, 0);
            } else {
                RenderItem2_AddItem(&FlumeCornerBarrelsHighWeight, slot, 0);
            }
            count++;
        }
    }
    if (count != 0) {
        next = entry->parent8;
        pos = GetFlumePieceScreenPos(entry);
        spr = LogFlumeFc1M3Sprite;
        if (entry->submode != 0) {
            spr = LogFlumeFc3M3Sprite;
        }
        thisX = entry->tile.pos.x;
        if (entry->submode == 0) {
            if (next->tile.pos.x != thisX) {
                DrawFlumeBarrelList(&FlumeCornerBarrelsLowWeight, entry);
                if (spr != NULL) {
                    PrintSprite(spr, pos.x, pos.y, arg, 0);
                }
                DrawFlumeBarrelList(&FlumeCornerBarrelsHighWeight, entry);
            } else {
                DrawFlumeBarrelList(&FlumeCornerBarrelsHighWeight, entry);
                if (spr != NULL) {
                    PrintSprite(spr, pos.x, pos.y, 0, 0);
                }
                DrawFlumeBarrelList(&FlumeCornerBarrelsLowWeight, entry);
            }
        } else if (next->tile.pos.x != thisX) {
            DrawFlumeBarrelList(&FlumeCornerBarrelsHighWeight, entry);
            if (spr != NULL) {
                PrintSprite(spr, pos.x, pos.y, 0, 0);
            }
            DrawFlumeBarrelList(&FlumeCornerBarrelsLowWeight, entry);
        } else {
            DrawFlumeBarrelList(&FlumeCornerBarrelsLowWeight, entry);
            if (spr != NULL) {
                PrintSprite(spr, pos.x, pos.y, 0, 0);
            }
            DrawFlumeBarrelList(&FlumeCornerBarrelsHighWeight, entry);
        }
        idx = GetFlumeTrackSpriteIndex((struct FlumeEntry *)entry);
        pos = GetFlumePieceScreenPos(entry);
        spr = LogFlumeTrackSprites[idx];
        if (spr != NULL) {
            PrintSprite(spr, pos.x, pos.y, 0, 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x0040cc00
void DrawFlumePieceWithBarrels(struct FlumeEntry *entry, int arg) {
    int idx;
    struct Point pos;
    struct Sprite *spr;

    if (DrawFlumeBarrelsOnPiece(entry)) {
        idx = GetFlumeTrackSpriteIndex((struct FlumeEntry *)entry);
        pos = GetFlumePieceScreenPos(entry);
        spr = LogFlumeTrackSprites[idx];
        if (spr != NULL) {
            PrintSprite(spr, pos.x, pos.y, arg, 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x0040cc50
int RenderLogFlumeTrack(struct Element *a, int b, int c, TileId *tile, void *e, int arg) {
    struct FlumeEntry *entry = FindFlumeSubEntryByTile(tile);
    if (entry != NULL) {
        if (entry->mode == 2 && (entry->submode == 0 || entry->submode == 2)) {
            return RenderLogFlumeCorner(entry, arg);
        }
        DrawFlumePieceWithBarrels(entry, arg);
    }
}

// FUNCTION: LEGOLAND 0x0040cca0
void DrawFlumeTrackEnd(struct FlumeEntry *node) {
    struct FlumeEntry *entry = (struct FlumeEntry *)node;
    struct Point pos;
    struct Point off;
    struct Sprite *spr;
    int idx;

    entry->ride->field_18 = 0;
    entry->ride->field_14 = 0;
    pos = GetScreenCoordsForObject(&entry->tile, entry->ride);
    switch (entry->submode) {
    case 0:
        idx = 0;
        break;
    case 1:
        idx = 1;
        break;
    case 2:
        idx = 2;
        break;
    case 3:
        idx = 3;
        break;
    }
    spr = LogFlumeTrackEndyList->sprites[(unsigned char)idx];
    off.x = LogFlumeTrackEndyList->offset_x[(unsigned char)idx] >> 1;
    off.y = LogFlumeTrackEndyList->offset_y[(unsigned char)idx] >> 1;
    AdjustOffsetForViewMode(&off);
    if (spr != NULL) {
        PrintSprite(spr, pos.x + off.x, pos.y + off.y, 0, 0);
    }
}

// FUNCTION: LEGOLAND 0x0040cd70
void DrawFlumeTrackEnds(struct FlumeEntry *p, int param1) {
    struct FlumeEntry *first;
    struct FlumeEntry *second;
    unsigned int phase;

    if (p->sub2 == 0) {
        return;
    }

    first = p->link30;
    second = p->link34;

    if (first->mode == 3) {
        phase = first->submode;
        if (param1 != 0) {
            if (phase != 0 && phase != 3) {
                DrawFlumeTrackEnd(first);
            }
        } else {
            if (phase != 1 && phase != 2) {
                DrawFlumeTrackEnd(first);
            }
        }
    }

    if (second->mode == 3) {
        phase = second->submode;
        if (param1 != 0) {
            if (phase != 0 && phase != 3) {
                DrawFlumeTrackEnd(second);
            }
        } else {
            if (phase != 1 && phase != 2) {
                DrawFlumeTrackEnd(second);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0040cdf0
int DrawFlumeBarrelsAtTile(TileId *tile) {
    int result = 0;
    struct FlumeEntry *entry = FindFlumeSubEntryByTile(tile);
    if (entry != NULL) {
        result = DrawFlumeBarrelsOnPiece(entry);
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0040ce20
void FindFlumeConnectionNeighbours(struct InputBuffer *esi) {
    unsigned int eax = LogFlumeFootprint.x1;
    unsigned int edx = LogFlumeFootprint.y0;
    unsigned int ebx = LogFlumeFootprint.y1;
    unsigned int local_8 = eax - LogFlumeFootprint.x0;
    struct SubBuf buf;

    ebx = ebx - edx;

    FlumeNeighbours[0] = 0;
    FlumeNeighbours[1] = 0;
    FlumeNeighbours[2] = 0;
    FlumeNeighbours[3] = 0;

    if (esi->flags & 1) {
        buf.b0 = esi->var_4;
        buf.b1 = esi->var_8 - (unsigned char)ebx;
        FlumeNeighbours[0] = FindFlumePieceAtTile(&buf);
    }
    if (esi->flags & 2) {
        buf.b0 = esi->var_c + (unsigned char)local_8;
        buf.b1 = esi->var_10;
        FlumeNeighbours[1] = FindFlumePieceAtTile(&buf);
    }
    if (esi->flags & 4) {
        buf.b0 = esi->var_14;
        buf.b1 = esi->var_18 + (unsigned char)ebx;
        FlumeNeighbours[2] = FindFlumePieceAtTile(&buf);
    }
    if (esi->flags & 8) {
        buf.b0 = esi->var_1c - (unsigned char)local_8;
        buf.b1 = esi->var_20;
        FlumeNeighbours[3] = FindFlumePieceAtTile(&buf);
    }
}

// FUNCTION: LEGOLAND 0x0040cf10
void GetFlumeConnectionNeighbours(struct InputBuffer *param_1, struct FlumeEntry ***param_2) {
    FindFlumeConnectionNeighbours(param_1);
    *param_2 = FlumeNeighbours;
}

// FUNCTION: LEGOLAND 0x0040cf30
unsigned int CountFlumeNeighbourSlots(struct FlumeEntry **param_1) {
    unsigned int count = 0;
    int i;
    for (i = 0; i < 4; i++) {
        if (param_1[i] != NULL) {
            count++;
        }
    }
    return count;
}

// FUNCTION: LEGOLAND 0x0040cf50
void DropForeignFlumeNeighbours(struct FlumeEntry *arg, struct FlumeEntry **slots) {
    int i;
    for (i = 0; i < 4; i++) {
        if (slots[i] != NULL && slots[i]->parent != arg) {
            slots[i] = NULL;
        }
    }
}

// FUNCTION: LEGOLAND 0x0040cf80
struct FlumeEntry *GetNeighbourFlumeOwner(struct FlumeEntry **arg) {
    int i;
    for (i = 0; i < 4; i++) {
        if (arg[i] != NULL) {
            return arg[i]->parent;
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0040cfa0
void DropNonEndFlumeNeighbours(struct FlumeEntry *(*arr)[4]) {
    int i;
    for (i = 0; i < 4; i++) {
        struct FlumeEntry *p = (*arr)[i];
        if (p != NULL) {
            if (p->mode != 3 && p->mode != 4) {
                (*arr)[i] = NULL;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0040cfd0
struct Point GetFlumePieceScreenPos(struct FlumeEntry *entry) {
    struct Point pos;
    struct Point off;
    int idx;

    if (entry->ride == LogFlumeEntranceRide) {
        return GetScreenCoordsForObject(&entry->parent->tile, LogFlumeEntranceRide);
    }
    if (entry->ride == LogFlumeTrackRide) {
        entry->ride->field_18 = 0;
        entry->ride->field_14 = 0;
        pos = GetScreenCoordsForObject(&entry->tile, entry->ride);
        idx = GetFlumeTrackSpriteIndex((struct FlumeEntry *)entry);
        off.x = LogFlumeImageList->offset_x[(unsigned char)idx] >> 1;
        off.y = LogFlumeImageList->offset_y[(unsigned char)idx] >> 1;
        AdjustOffsetForViewMode(&off);
        pos.x += off.x;
        pos.y += off.y;
        return pos;
    }
    return GetScreenCoordsForObject(&entry->tile, entry->ride);
}

// FUNCTION: LEGOLAND 0x0040d090
void GetFlumePieceFootprint(struct FlumeEntry *entry, struct Footprint **out, TileId *tile) {
    struct FlumeEntry *cur = entry->link28;
    void *r;
    unsigned int v;

    if (cur == NULL || cur == (struct FlumeEntry *)-1) {
        cur = entry;
    }
    tile->pos.x = cur->tile.pos.x;
    tile->pos.y = cur->tile.pos.y;
    if (cur->link28 == (struct FlumeEntry *)-1) {
        tile->pos.x = cur->parent->tile.pos.x;
        tile->pos.y = cur->parent->tile.pos.y;
        LogFlumeEntrancePieceFootprint = LogFlumeEntranceRide->footprint;
        LogFlumeEntrancePieceFootprint.x1 = LogFlumeEntrancePieceFootprint.x0 + (LogFlumeFootprint.x1 - LogFlumeFootprint.x0) * 2;
        *out = &LogFlumeEntrancePieceFootprint;
        return;
    }
    r = cur->ride;
    if (r == LogFlumeTrackRide) {
        v = LogFlumeFootprint.x1;
        memcpy(&LogFlumeTrackPieceFootprint, &LogFlumeFootprint, sizeof(struct Footprint));
        LogFlumeTrackPieceFootprint.x1 = v - 1;
        LogFlumeTrackPieceFootprint.y1 = LogFlumeTrackPieceFootprint.y1 - 1;
        *out = &LogFlumeTrackPieceFootprint;
    } else if (r == LogFlumeDropRide) {
        *out = (struct Footprint *)LogFlumeDropRide->footprint.v;
    } else if (r == LogFlumeHoldUpRide) {
        *out = (struct Footprint *)LogFlumeHoldUpRide->footprint.v;
    } else if (r == LogFlumeSpecialCorner1Ride) {
        *out = (struct Footprint *)LogFlumeSpecialCorner1Ride->footprint.v;
    } else if (r == LogFlumeSpecialCorner2Ride) {
        *out = (struct Footprint *)LogFlumeSpecialCorner2Ride->footprint.v;
    } else if (r == LogFlumeSpecialCorner3Ride) {
        *out = (struct Footprint *)LogFlumeSpecialCorner3Ride->footprint.v;
    } else if (r == LogFlumeSpecialCorner4Ride) {
        *out = (struct Footprint *)LogFlumeSpecialCorner4Ride->footprint.v;
    } else if (r == LogFlumeTunnelRide) {
        *out = (struct Footprint *)LogFlumeTunnelRide->footprint.v;
    } else if (r == LogFlumeCsaw2Ride) {
        *out = (struct Footprint *)LogFlumeCsaw2Ride->footprint.v;
    } else {
        *out = NULL;
    }
}

// FUNCTION: LEGOLAND 0x0040d210
struct FlumeEntry *FindFlumePieceAt(int x, int y) {
    struct FlumeEntry *outer;
    struct FlumeEntry *cur;
    struct Footprint *fp;
    TileId t;
    int tx;
    int ty;

    outer = FlumeEntryList;
    if (FlumeEntryList != NULL) {
        for (; outer != NULL; outer = outer->next) {
            cur = outer->sub;
            while (cur != NULL) {
                GetFlumePieceFootprint(cur, &fp, &t);
                if (fp != NULL) {
                    tx = t.pos.x;
                    ty = t.pos.y;
                    if (x >= fp->x0 + tx && x <= fp->x1 + tx && y >= fp->y0 + ty && y <= fp->y1 + ty) {
                        return cur;
                    }
                } else {
                    // STRING: LEGOLAND 0x004b4a24
                    DBPrintf("Something wrong in the log flume track\n");
                }
                cur = cur->next;
            }
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0040d2d0
void SetFlumeQueryCursor(struct Point *pt) {
    struct FlumeEntry *entry = FindFlumePieceAt(pt->x, pt->y);
    struct Footprint *fp;
    TileId t;

    if (entry != NULL) {
        QueryCursor.tile_x = entry->tile.pos.x;
        QueryCursor.tile_y = entry->tile.pos.y;
        GetFlumePieceFootprint(entry, &fp, &t);
        QueryCursor.footprint = *fp;
        QueryCursor.field_1828 = 8;
        SetCursorBlocked(&QueryCursor, 1);
        if (IsJoinedToFlume((struct FlumeEntry *)entry->sub2)) {
            if (entry->link30->field_c == NULL || entry->link30->field_8 == NULL || entry->link34->field_c == NULL || entry->link34->field_8 == NULL || IsFlumeLoopClosed((struct FlumeEntry *)entry->parent)) {
                if (!(entry->flags10d & 1) && !HasFlumeBarrelOnPiece((struct FlumeEntry *)entry)) {
                    ResetCursorBuildable(&QueryCursor);
                }
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0040d3b0
unsigned int SetFlumeEditCursor(void *param_1, unsigned int *param_2) {
    EditMode.unk0 = 1;
    EditMode.unk8 = param_1;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 8;
    BuildCursorPtr(&EditCursor, 0x8f8, 0);
    SetEditCursorFootPrint(param_2);
    FlumeNeighbourCursor1.field_1828 = 0x2034;
    FlumeNeighbourCursor2.field_1828 = 0x2034;
    FlumeConnectionCursor1.field_1828 = 0x2034;
    FlumeConnectionCursor2.field_1828 = 0x2034;
    return 0x2034;
}

/* Initializes two linked cursors from the connection mask and coordinates, using the inset log-flume footprint, and
 * resets their buildability checks. */
// FUNCTION: LEGOLAND 0x0040d420
void BuildFlumeConnectionCursors(unsigned int *res) {
    unsigned int flags = res[0];
    struct Cursor *cur = NULL;
    int n;

    FlumeConnectionCursor1.next = &FlumeConnectionCursor2;
    memcpy(&FlumeConnectionCursor1.footprint, &LogFlumeFootprint, sizeof(struct Footprint));
    FlumeConnectionCursor1.footprint.x1 = LogFlumeFootprint.x1 - 1;
    FlumeConnectionCursor1.footprint.y1 = FlumeConnectionCursor1.footprint.y1 - 1;
    FlumeConnectionCursor2.next = NULL;
    FlumeConnectionCursor2.footprint = FlumeConnectionCursor1.footprint;
    for (n = 2; n != 0; n--) {
        if (cur == NULL) {
            cur = &FlumeConnectionCursor1;
        } else {
            cur = &FlumeConnectionCursor2;
        }
        if (flags & 1) {
            cur->tile_x = res[1];
            cur->tile_y = res[2];
            flags &= ~1;
        } else if (flags & 2) {
            cur->tile_x = res[3];
            cur->tile_y = res[4];
            flags &= ~2;
        } else if (flags & 4) {
            cur->tile_x = res[5];
            cur->tile_y = res[6];
            flags &= ~4;
        } else if (flags & 8) {
            cur->tile_x = res[7];
            cur->tile_y = res[8];
            flags &= ~8;
        }
        ResetCursorBuildable(cur);
    }
}

/* Appends up to two cursors for the occupied flume neighbours, copying each piece footprint and tile position and
 * resetting buildability checks. */
// FUNCTION: LEGOLAND 0x0040d520
void AppendFlumeNeighbourCursors(struct FlumeEntry **list, struct Cursor *first) {
    int flags;
    struct Cursor *cur;
    int i;

    cur = NULL;
    flags = FlumeNeighbourMask(list);
    FlumeNeighbourCursor1.next = NULL;
    FlumeNeighbourCursor2.next = NULL;
    first->next = &FlumeNeighbourCursor1;
    i = 0;
    do {
        if (cur == NULL) {
            cur = &FlumeNeighbourCursor1;
        } else {
            cur->next = &FlumeNeighbourCursor2;
            cur = &FlumeNeighbourCursor2;
        }
        if (flags & 1) {
            struct Footprint *fp;
            TileId t;
            GetFlumePieceFootprint(list[0], &fp, &t);
            cur->tile_x = t.pos.x;
            cur->tile_y = t.pos.y;
            cur->footprint = *fp;
            flags &= ~1;
        } else if (flags & 4) {
            struct Footprint *fp;
            TileId t;
            GetFlumePieceFootprint(list[1], &fp, &t);
            cur->tile_x = t.pos.x;
            cur->tile_y = t.pos.y;
            cur->footprint = *fp;
            flags &= ~4;
        } else if (flags & 0x10) {
            struct Footprint *fp;
            TileId t;
            GetFlumePieceFootprint(list[2], &fp, &t);
            cur->tile_x = t.pos.x;
            cur->tile_y = t.pos.y;
            cur->footprint = *fp;
            flags &= ~0x10;
        } else if (flags & 0x40) {
            struct Footprint *fp;
            TileId t;
            GetFlumePieceFootprint(list[3], &fp, &t);
            cur->tile_x = t.pos.x;
            cur->tile_y = t.pos.y;
            cur->footprint = *fp;
            flags &= ~0x40;
        }
        cur->field_1828 = 0x2010;
        ResetCursorBuildable(cur);
    } while (flags != 0 && ++i < 2);
}

// FUNCTION: LEGOLAND 0x0040d6f0
unsigned int LogFlumePieceCalcCursor(struct Ride *param_1, int *param_2, unsigned int param_3, unsigned int *param_4, FlumeCallback param_5, FlumeCheckFn param_6) {
    union {
        TileId t;
        struct FlumeEntry **list;
    } u;

    unsigned int buf[9];

    struct MapRect rect;
    int cost;
    struct FlumeEntry *key;
    int r;

    EditCursor.footprint = *(struct Footprint *)param_4;
    ScreenToMapRef((int *)param_2, &EditCursor.tile_x, param_3);
    EditCursor.field_1830 = 0;
    ResetCursorBuildable(&EditCursor);
    ValidateCursor(&EditCursor, param_1);
    u.t.pos.x = (unsigned char)EditCursor.tile_x;
    u.t.pos.y = (unsigned char)EditCursor.tile_y;
    param_5(u.t, buf);
    BuildFlumeConnectionCursors(buf);
    EditCursor.next = &FlumeConnectionCursor1;
    cost = GetObjCost((struct Ride *)param_1);
    if (GetBrickCount() < cost) {
        SetCursorBlocked(&EditCursor, 2);
    }
    if (IsCursorBuildable(&EditCursor)) {
        u.t.pos.x = (unsigned char)EditCursor.tile_x;
        u.t.pos.y = (unsigned char)EditCursor.tile_y;
        param_5(u.t, buf);
        GetFlumeConnectionNeighbours((struct InputBuffer *)buf, &u.list);
        DropNonEndFlumeNeighbours((struct FlumeEntry * (*)[4]) u.list);
        if (CountFlumeNeighbourSlots(u.list) == 0) {
            SetCursorBlocked(&EditCursor, 0xe);
        } else {
            ResetCursorBuildable(&EditCursor);
            key = GetNeighbourFlumeOwner((struct FlumeEntry **)u.list);
            DropForeignFlumeNeighbours(key, (struct FlumeEntry **)u.list);
            if (CountFlumeNeighbourSlots(u.list) == 0) {
                SetCursorBlocked(&EditCursor, 0xe);
            } else if (param_6(u.list)) {
                ResetCursorBuildable(&EditCursor);
                AppendFlumeNeighbourCursors((struct FlumeEntry **)u.list, EditCursor.next->next);
            } else {
                SetCursorBlocked(&EditCursor, 0xd);
            }
        }
    }
    if (IsCursorBuildable(&EditCursor)) {
        int cx = EditCursor.tile_x;
        int cy = EditCursor.tile_y;
        rect.x0 = EditCursor.footprint.x0 + cx;
        rect.y0 = EditCursor.footprint.y0 + cy;
        rect.x1 = EditCursor.footprint.x1 + cx;
        rect.y1 = EditCursor.footprint.y1 + cy;
        r = CheckForPeople(&rect);
        switch (r) {
        case -1:
            SetCursorBlocked(&EditCursor, 4);
            break;
        case 1:
            SetCursorBlocked(&EditCursor, 3);
            break;
        }
    }
    PropagateCursorBlocked(&EditCursor);
}

// FUNCTION: LEGOLAND 0x0040d900
void LogFlumePieceAddObject(unsigned int param_1, unsigned int *param_2, Element *param_3, void (*param_4)(), FlumeCallback param_5, FlumeEdgeFn param_6) {
    struct FlumeEntry *entry = AllocFlumeNode();
    unsigned int buf[9];
    struct FlumeEntry **list;
    int coords[4];
    struct FlumeEntry *key;

    if (entry != NULL) {
        entry->tile.pos.x = (unsigned char)param_1;
        entry->tile.pos.y = ((unsigned char *)&param_1)[1];
        entry->ride = ((Element *)param_3)->ride;
        entry->link28 = NULL;
        ((void (*)(unsigned int, unsigned int *))param_5)(param_1, buf);
        GetFlumeConnectionNeighbours((struct InputBuffer *)buf, &list);
        DropNonEndFlumeNeighbours((struct FlumeEntry * (*)[4]) list);
        key = GetNeighbourFlumeOwner((struct FlumeEntry **)list);
        AddFlumeScore((struct FlumeEntry *)key, 3);
        entry->parent = (struct FlumeEntry *)key;
        DropForeignFlumeNeighbours(key, (struct FlumeEntry **)list);
        ((void (*)(struct FlumeEntry *))param_4)(entry);
        PushFlumeListNode((struct FlumeEntry *)key, (struct ListNode *)entry);
        coords[1] = ((unsigned char *)&param_1)[1];
        coords[0] = (unsigned char)param_1;
        memcpy(&((Element *)param_3)->ride->footprint, param_2, sizeof(struct Footprint));
        AddBasicObject((Element *)param_3, coords);
        param_6((struct FlumeEntry *)entry, coords);
        JoinFlumeNeighbourChains((struct FlumeEntry **)coords, (struct FlumeEntry **)list);
        JoinFlumeNeighbourShapes((void **)coords, (struct FlumeEntry **)list);
    }
}

// FUNCTION: LEGOLAND 0x0040da10
void DropUnlinkedFlumeNeighbours(struct FlumeEntry *a, struct LinkList *list) {
    struct FlumeEntry *node;

    if (list->field_0 != NULL) {
        node = list->field_0;
        if (a->sub2 == 0) {
            if (node->field_8 != a && node->field_c != a) {
                list->field_0 = NULL;
            }
        } else {
            if (node->field_8 != a->link30 && node->field_c != a->link30 &&
                node->field_8 != a->link34 && node->field_c != a->link34) {
                list->field_0 = NULL;
            }
        }
    }

    if (list->field_4 != NULL) {
        node = list->field_4;
        if (a->sub2 == 0) {
            if (node->field_8 != a && node->field_c != a) {
                list->field_4 = NULL;
            }
        } else {
            if (node->field_8 != a->link30 && node->field_c != a->link30 &&
                node->field_8 != a->link34 && node->field_c != a->link34) {
                list->field_4 = NULL;
            }
        }
    }

    if (list->field_c != NULL) {
        node = list->field_c;
        if (a->sub2 == 0) {
            if (node->field_8 != a && node->field_c != a) {
                list->field_c = NULL;
            }
        } else {
            if (node->field_8 != a->link30 && node->field_c != a->link30 &&
                node->field_8 != a->link34 && node->field_c != a->link34) {
                list->field_c = NULL;
            }
        }
    }

    if (list->field_8 != NULL) {
        node = list->field_8;
        if (a->sub2 == 0) {
            if (node->field_8 != a && node->field_c != a) {
                list->field_8 = NULL;
            }
        } else {
            if (node->field_8 != a->link30 && node->field_c != a->link30 &&
                node->field_8 != a->link34 && node->field_c != a->link34) {
                list->field_8 = NULL;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0040db00
void LogFlumePieceRemoveObject(struct Element *param_1, TileId param_2, struct Cursor *param_3, FlumeCallback param_4) {
    struct FlumeEntry *entry = FindFlumeSubEntryByTile(&param_2);
    struct Footprint *fp;
    TileId t;
    struct InputBuffer buf;
    struct FlumeEntry **list;

    if (entry != NULL) {
        AddFlumeScore((struct FlumeEntry *)entry->parent, -3);
        GetFlumePieceFootprint(entry, &fp, &t);
        param_3->footprint = *fp;
        StandardRemoveObject(param_1, t, param_3);
        param_4(t, (unsigned int *)&buf);
        GetFlumeConnectionNeighbours(&buf, &list);
        DropUnlinkedFlumeNeighbours((struct FlumeEntry *)entry, (struct LinkList *)list);
        DetachFlumeNeighbourShapes(entry, (struct FlumeEntry **)list);
        FreeFlumeEntry((struct FlumeEntry *)entry->parent, (struct FlumeEntry *)entry);
    }
}

// FUNCTION: LEGOLAND 0x0040dbb0
void LogFlumeTrackSetEditMode(void) {
    unsigned int local[5];
    unsigned int v = LogFlumeFootprint.x1;

    memcpy(local, &LogFlumeFootprint, sizeof(local));
    local[2] = v - 1;
    local[3] = local[3] - 1;
    SetFlumeEditCursor(LogFlumeTrackRide, local);
}

// FUNCTION: LEGOLAND 0x0040dc00
void BuildFlumeSpecialCornerNodes(struct FlumeEntry *entry) {
    unsigned int w = LogFlumeFootprint.x1 - LogFlumeFootprint.x0;
    unsigned int h = LogFlumeFootprint.y1 - LogFlumeFootprint.y0;
    TileId t;
    struct FlumeEntry *n;
    struct FlumeEntry *prev;

    t.pos.x = entry->tile.pos.x;
    t.pos.y = entry->tile.pos.y;

    if (entry != NULL) {
        entry->submode = FlumeSpecialCornerIndex;
        switch (FlumeSpecialCornerIndex) {
        case 0:
            t.pos.x += LogFlumeSpecialCorner1Ride->footprint.v[0];
            t.pos.y += LogFlumeSpecialCorner1Ride->footprint.v[1];
            t.pos.x += 3;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 3;
                n->submode = 0;
                n->ride = LogFlumeTrackRide;
                n->slotset = entry->slotset;
                n->flags10d |= 4;
                n->link28 = entry;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            entry->link30 = n;
            prev = n;
            t.pos.y += h;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 1;
                n->submode = 0;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.y += h;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 2;
                n->submode = 0;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.x += w;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 1;
                n->submode = 1;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.x += w;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 3;
                n->submode = 1;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            break;
        case 1:
            t.pos.x += LogFlumeSpecialCorner2Ride->footprint.v[0];
            t.pos.y += LogFlumeSpecialCorner2Ride->footprint.v[1];
            t.pos.x += LogFlumeSpecialCorner2Ride->footprint.v[2] - LogFlumeSpecialCorner2Ride->footprint.v[0] - w + 1;
            t.pos.y += 2;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 3;
                n->submode = 1;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            prev = n;
            entry->link30 = n;
            t.pos.x -= w;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 1;
                n->submode = 1;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.x -= w;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 2;
                n->submode = 1;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.y += h;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 1;
                n->submode = 0;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.y += h;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 3;
                n->submode = 2;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            break;
        case 2:
            t.pos.x += LogFlumeSpecialCorner3Ride->footprint.v[0];
            t.pos.y += LogFlumeSpecialCorner3Ride->footprint.v[1];
            t.pos.x += 4;
            t.pos.y += LogFlumeSpecialCorner3Ride->footprint.v[3] - LogFlumeSpecialCorner3Ride->footprint.v[1] - h + 1;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 3;
                n->submode = 2;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            prev = n;
            entry->link30 = n;
            t.pos.y -= h;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 1;
                n->submode = 0;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.y -= h;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 2;
                n->submode = 2;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.x -= w;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 1;
                n->submode = 1;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.x -= w;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 3;
                n->submode = 3;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            break;
        case 3:
            t.pos.x += LogFlumeSpecialCorner4Ride->footprint.v[0];
            t.pos.y += LogFlumeSpecialCorner4Ride->footprint.v[1] + 4;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 3;
                n->submode = 3;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            entry->link30 = n;
            prev = n;
            t.pos.x += w;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 1;
                n->submode = 1;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.x += w;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 2;
                n->submode = 3;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.y -= h;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 1;
                n->submode = 0;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
            LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
            prev = n;
            t.pos.y -= h;
            n = AllocFlumeNode();
            if (n != NULL) {
                n->mode = 3;
                n->submode = 0;
                n->slotset = entry->slotset;
                n->link28 = entry;
                n->ride = LogFlumeTrackRide;
                n->flags10d |= 4;
                n->tile.pos.x = t.pos.x;
                n->tile.pos.y = t.pos.y;
            }
            break;
        default:
            return;
        }
        AppendListNode((struct FlumeEntry *)entry, (struct ListNode *)n);
        LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)n);
        entry->link34 = n;
    }
}

// FUNCTION: LEGOLAND 0x0040e340
void GetFlumeSpecialCornerEndNodes(struct FlumeEntry *node, int *out) {
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    switch (node->submode) {
    case 0:
        out[0] = node->a_30;
        out[1] = node->b_34;
        break;
    case 1:
        out[1] = node->a_30;
        out[2] = node->b_34;
        break;
    case 2:
        out[2] = node->a_30;
        out[3] = node->b_34;
        break;
    case 3:
        out[3] = node->a_30;
        out[0] = node->b_34;
        break;
    }
}

// FUNCTION: LEGOLAND 0x0040e3b0
int CanPlaceFlumeSpecialCorner(struct FlumeEntry **ctx) {
    int flags = FlumeNeighbourMask(ctx);

    switch (FlumeSpecialCornerIndex) {
    case 0:
        if (flags == 5 || flags == 1 || flags == 4) {
            return 1;
        }
        break;
    case 1:
        if (flags == 0x14 || flags == 4 || flags == 0x10) {
            return 1;
        }
        break;
    case 2:
        if (flags == 0x50 || flags == 0x10 || flags == 0x40) {
            return 1;
        }
        break;
    case 3:
        if (flags == 0x41 || flags == 0x40 || flags == 1) {
            return 1;
        }
        break;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040e440
void GetFlumeSpecialCornerConnections(TileId p, unsigned int *result) {
    unsigned int w = LogFlumeFootprint.x1 - LogFlumeFootprint.x0;
    unsigned int h = LogFlumeFootprint.y1 - LogFlumeFootprint.y0;

    switch (FlumeSpecialCornerIndex) {
    case 0:
        p.pos.x += (unsigned char)LogFlumeSpecialCorner1Ride->footprint.v[0];
        p.pos.y += (unsigned char)LogFlumeSpecialCorner1Ride->footprint.v[1];
        result[0] = 3;
        result[1] = p.pos.x + 3;
        result[2] = p.pos.y;
        result[3] = LogFlumeSpecialCorner1Ride->footprint.v[2] - LogFlumeSpecialCorner1Ride->footprint.v[0] - w + p.pos.x + 1;
        result[4] = p.pos.y + 4;
        break;
    case 1:
        p.pos.x += (unsigned char)LogFlumeSpecialCorner2Ride->footprint.v[0];
        p.pos.y += (unsigned char)LogFlumeSpecialCorner2Ride->footprint.v[1];
        result[0] = 6;
        result[3] = LogFlumeSpecialCorner2Ride->footprint.v[2] - LogFlumeSpecialCorner2Ride->footprint.v[0] - w + p.pos.x + 1;
        result[4] = p.pos.y + 2;
        result[5] = p.pos.x + 2;
        result[6] = LogFlumeSpecialCorner2Ride->footprint.v[3] - LogFlumeSpecialCorner2Ride->footprint.v[1] - h + p.pos.y + 1;
        break;
    case 2:
        p.pos.x += (unsigned char)LogFlumeSpecialCorner3Ride->footprint.v[0];
        p.pos.y += (unsigned char)LogFlumeSpecialCorner3Ride->footprint.v[1];
        result[0] = 12;
        result[5] = p.pos.x + 4;
        result[6] = LogFlumeSpecialCorner3Ride->footprint.v[3] - LogFlumeSpecialCorner3Ride->footprint.v[1] - h + p.pos.y + 1;
        result[7] = p.pos.x;
        result[8] = p.pos.y + 3;
        break;
    case 3:
        p.pos.x += (unsigned char)LogFlumeSpecialCorner4Ride->footprint.v[0];
        p.pos.y += (unsigned char)LogFlumeSpecialCorner4Ride->footprint.v[1];
        result[0] = 9;
        result[7] = p.pos.x;
        result[8] = p.pos.y + 4;
        result[1] = p.pos.x + 4;
        result[2] = p.pos.y;
        break;
    }
}

// FUNCTION: LEGOLAND 0x0040e630
void LogFlumeSpecialCorner1SetEditMode(void) {
    unsigned int local[5];
    memcpy(local, LogFlumeSpecialCorner1Ride->footprint.v, 20);
    SetFlumeEditCursor(LogFlumeSpecialCorner1Ride, local);
}

// FUNCTION: LEGOLAND 0x0040e660
void LogFlumeSpecialCorner2SetEditMode(void) {
    struct Ride *src = LogFlumeSpecialCorner2Ride;
    unsigned int local[5];
    memcpy(local, src->footprint.v, sizeof(local));
    SetFlumeEditCursor(src, local);
}

// FUNCTION: LEGOLAND 0x0040e690
void LogFlumeSpecialCorner3SetEditMode(void) {
    struct Ride *src = LogFlumeSpecialCorner3Ride;
    unsigned int local[5];
    memcpy(local, src->footprint.v, sizeof(local));
    SetFlumeEditCursor(src, local);
}

// FUNCTION: LEGOLAND 0x0040e6c0
void LogFlumeSpecialCorner4SetEditMode(void) {
    struct Ride *src = LogFlumeSpecialCorner4Ride;
    unsigned int local[5];
    memcpy(local, src->footprint.v, sizeof(local));
    SetFlumeEditCursor(src, local);
}

// FUNCTION: LEGOLAND 0x0040e6f0
void LogFlumeSpecialCorner1CalcCursor(struct Element *param_1, struct Point *param_2, unsigned int param_3) {
    struct Ride *src = LogFlumeSpecialCorner1Ride;
    unsigned int local[5];
    FlumeSpecialCornerIndex = 0;
    memcpy(local, src->footprint.v, 20);
    LogFlumePieceCalcCursor(src, &param_2->x, param_3, local, GetFlumeSpecialCornerConnections, CanPlaceFlumeSpecialCorner);
}

// FUNCTION: LEGOLAND 0x0040e740
void LogFlumeSpecialCorner2CalcCursor(struct Element *param_1, struct Point *param_2, unsigned int param_3) {
    struct Ride *src = LogFlumeSpecialCorner2Ride;
    unsigned int local[5];
    FlumeSpecialCornerIndex = 1;
    memcpy(local, src->footprint.v, 20);
    LogFlumePieceCalcCursor(src, &param_2->x, param_3, local, GetFlumeSpecialCornerConnections, CanPlaceFlumeSpecialCorner);
}

// FUNCTION: LEGOLAND 0x0040e790
void LogFlumeSpecialCorner3CalcCursor(struct Element *param_1, struct Point *param_2, unsigned int param_3) {
    struct Ride *src = LogFlumeSpecialCorner3Ride;
    unsigned int local[5];
    FlumeSpecialCornerIndex = 2;
    memcpy(local, src->footprint.v, 20);
    LogFlumePieceCalcCursor(src, &param_2->x, param_3, local, GetFlumeSpecialCornerConnections, CanPlaceFlumeSpecialCorner);
}

// FUNCTION: LEGOLAND 0x0040e7e0
void LogFlumeSpecialCorner4CalcCursor(struct Element *param_1, struct Point *param_2, unsigned int param_3) {
    struct Ride *src = LogFlumeSpecialCorner4Ride;
    unsigned int local[5];
    FlumeSpecialCornerIndex = 3;
    memcpy(local, src->footprint.v, 20);
    LogFlumePieceCalcCursor(src, &param_2->x, param_3, local, GetFlumeSpecialCornerConnections, CanPlaceFlumeSpecialCorner);
}

// FUNCTION: LEGOLAND 0x0040e830
void LogFlumeSpecialCorner1DCalcCursor(struct Element *param_1, struct Point *param_2) {
    FlumeSpecialCornerIndex = 0;
    SetFlumeQueryCursor(param_2);
}

// FUNCTION: LEGOLAND 0x0040e850
void LogFlumeSpecialCorner2DCalcCursor(struct Element *param_1, struct Point *param_2) {
    FlumeSpecialCornerIndex = 1;
    SetFlumeQueryCursor(param_2);
}

// FUNCTION: LEGOLAND 0x0040e870
void LogFlumeSpecialCorner3DCalcCursor(struct Element *dummy, struct Point *param_1) {
    FlumeSpecialCornerIndex = 2;
    SetFlumeQueryCursor(param_1);
}

// FUNCTION: LEGOLAND 0x0040e890
void LogFlumeSpecialCorner4DCalcCursor(struct Element *param_1, struct Point *param_2) {
    FlumeSpecialCornerIndex = 3;
    SetFlumeQueryCursor(param_2);
}

// FUNCTION: LEGOLAND 0x0040e8b0
void LogFlumeSpecialCorner1LoadSprites(struct Element *obj_ptr) {
    struct Ride *resA;
    struct Sprite *resB;

    LogFlumeSpecialCorner1Ride = (struct Ride *)obj_ptr->data;
    resA = (struct Ride *)LogFlumeSpecialCorner1Ride;
    resA->flags |= 0x400;

    resA = (struct Ride *)LogFlumeSpecialCorner1Ride;
    if (resA != NULL) {
        resB = resA->layer;
        if (resB != NULL) {
            resB->flags |= 0x2000;
        }
    }

    LogFlumeSpecialCorner1Element = obj_ptr;

    // STRING: LEGOLAND 0x004b4a58
    LogFlumeFc1M1Sprite = LoadSprite("fc1_m1.lls", 1);
    // STRING: LEGOLAND 0x004b4a4c
    LogFlumeFc1M2Sprite = LoadSprite("fc1_m2.lls", 1);

    LogFlumeFc1M1StageSprite = LogFlumeFc1M1Sprite;
    LogFlumeFc1M2StageSprite = LogFlumeFc1M2Sprite;
}

// FUNCTION: LEGOLAND 0x0040e920
void LogFlumeSpecialCorner2LoadSprites(struct Element *obj_ptr) {
    struct Ride *resA;
    struct Sprite *resB;

    LogFlumeSpecialCorner2Ride = (struct Ride *)obj_ptr->data;
    resA = (struct Ride *)LogFlumeSpecialCorner2Ride;
    resA->flags |= 0x400;

    resA = (struct Ride *)LogFlumeSpecialCorner2Ride;
    if (resA != NULL) {
        resB = resA->layer;
        if (resB != NULL) {
            resB->flags |= 0x2000;
        }
    }

    LogFlumeSpecialCorner2Element = obj_ptr;
    // STRING: LEGOLAND 0x004b4a64
    LogFlumeFc2M1Sprite = LoadSprite("fc2_m1.lls", 1);
}

// FUNCTION: LEGOLAND 0x0040e970
void LogFlumeSpecialCorner3LoadSprites(struct Element *obj_ptr) {
    struct Ride *resA;
    struct Sprite *resB;

    LogFlumeSpecialCorner3Ride = (struct Ride *)obj_ptr->data;
    resA = (struct Ride *)LogFlumeSpecialCorner3Ride;
    resA->flags |= 0x400;

    resA = (struct Ride *)LogFlumeSpecialCorner3Ride;
    if (resA != NULL) {
        resB = resA->layer;
        if (resB != NULL) {
            resB->flags |= 0x2000;
        }
    }

    LogFlumeSpecialCorner3Element = obj_ptr;

    // STRING: LEGOLAND 0x004b4a7c
    LogFlumeFc3M1Sprite = LoadSprite("fc3_m1.lls", 1);
    // STRING: LEGOLAND 0x004b4a70
    LogFlumeFc3M2Sprite = LoadSprite("fc3_m2.lls", 1);

    LogFlumeFc3M1StageSprite = LogFlumeFc3M1Sprite;
    LogFlumeFc3M2StageSprite = LogFlumeFc3M2Sprite;
}

// FUNCTION: LEGOLAND 0x0040e9e0
void LogFlumeSpecialCorner4LoadSprites(struct Element *obj_ptr) {
    struct Ride *resA;
    struct Sprite *resB;

    LogFlumeSpecialCorner4Ride = (struct Ride *)obj_ptr->data;
    resA = (struct Ride *)LogFlumeSpecialCorner4Ride;
    resA->flags |= 0x400;
    LogFlumeSpecialCorner4Element = obj_ptr;

    resA = (struct Ride *)LogFlumeSpecialCorner4Ride;
    if (resA != NULL) {
        resB = resA->layer;
        if (resB != NULL) {
            resB->flags |= 0x2000;
        }
    }

    // STRING: LEGOLAND 0x004b4a88
    LogFlumeFc4MSprite = LoadSprite("fc4_m.lls", 1);
}

// FUNCTION: LEGOLAND 0x0040ea30
void LogFlumeSpecialCorner1UnloadSprites(void) {
    if (LogFlumeFc1M1Sprite != 0) {
        KillSprite(LogFlumeFc1M1Sprite);
    }
    if (LogFlumeFc1M2Sprite != 0) {
        KillSprite(LogFlumeFc1M2Sprite);
    }
}

// FUNCTION: LEGOLAND 0x0040ea60
void LogFlumeSpecialCorner2UnloadSprites(void) {
    if (LogFlumeFc2M1Sprite != NULL) {
        KillSprite(LogFlumeFc2M1Sprite);
    }
}

// FUNCTION: LEGOLAND 0x0040ea80
void LogFlumeSpecialCorner3UnloadSprites(void) {
    if (LogFlumeFc3M1Sprite != NULL) {
        KillSprite(LogFlumeFc3M1Sprite);
    }
    if (LogFlumeFc3M2Sprite != NULL) {
        KillSprite(LogFlumeFc3M2Sprite);
    }
}

// FUNCTION: LEGOLAND 0x0040eab0
void LogFlumeSpecialCorner4UnloadSprites(void) {
    if (LogFlumeFc4MSprite != NULL) {
        KillSprite(LogFlumeFc4MSprite);
    }
}

// FUNCTION: LEGOLAND 0x0040ead0
void LogFlumeSpecialCorner1AddObject(struct Element *param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    unsigned int local[5];
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    memcpy(local, LogFlumeSpecialCorner1Ride->footprint.v, 20);
    FlumeSpecialCornerIndex = 0;
    LogFlumePieceAddObject(packed, local, LogFlumeSpecialCorner1Element, BuildFlumeSpecialCornerNodes, GetFlumeSpecialCornerConnections, GetFlumeSpecialCornerEndNodes);
}

// FUNCTION: LEGOLAND 0x0040eb40
void LogFlumeSpecialCorner2AddObject(struct Element *param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    unsigned int local[5];
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    memcpy(local, LogFlumeSpecialCorner2Ride->footprint.v, 20);
    FlumeSpecialCornerIndex = 1;
    LogFlumePieceAddObject(packed, local, LogFlumeSpecialCorner2Element, BuildFlumeSpecialCornerNodes, GetFlumeSpecialCornerConnections, GetFlumeSpecialCornerEndNodes);
}

// FUNCTION: LEGOLAND 0x0040ebb0
void LogFlumeSpecialCorner3AddObject(struct Element *param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    unsigned int local[5];
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    memcpy(local, LogFlumeSpecialCorner3Ride->footprint.v, 20);
    FlumeSpecialCornerIndex = 2;
    LogFlumePieceAddObject(packed, local, LogFlumeSpecialCorner3Element, BuildFlumeSpecialCornerNodes, GetFlumeSpecialCornerConnections, GetFlumeSpecialCornerEndNodes);
}

// FUNCTION: LEGOLAND 0x0040ec20
void LogFlumeSpecialCorner4AddObject(struct Element *param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    unsigned int local[5];
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    memcpy(local, LogFlumeSpecialCorner4Ride->footprint.v, 20);
    FlumeSpecialCornerIndex = 3;
    LogFlumePieceAddObject(packed, local, LogFlumeSpecialCorner4Element, BuildFlumeSpecialCornerNodes, GetFlumeSpecialCornerConnections, GetFlumeSpecialCornerEndNodes);
}

// FUNCTION: LEGOLAND 0x0040ec90
void LogFlumeSpecialCorner1RemoveObject(struct Element *param_1, TileId param_2, struct Cursor *param_3) {
    FlumeSpecialCornerIndex = 0;
    LogFlumePieceRemoveObject(param_1, param_2, param_3, GetFlumeSpecialCornerConnections);
}

// FUNCTION: LEGOLAND 0x0040ecc0
void LogFlumeSpecialCorner2RemoveObject(struct Element *param_1, TileId param_2, struct Cursor *param_3) {
    FlumeSpecialCornerIndex = 1;
    LogFlumePieceRemoveObject(param_1, param_2, param_3, GetFlumeSpecialCornerConnections);
}

// FUNCTION: LEGOLAND 0x0040ecf0
void LogFlumeSpecialCorner3RemoveObject(struct Element *param_1, TileId param_2, struct Cursor *param_3) {
    FlumeSpecialCornerIndex = 2;
    LogFlumePieceRemoveObject(param_1, param_2, param_3, GetFlumeSpecialCornerConnections);
}

// FUNCTION: LEGOLAND 0x0040ed20
void LogFlumeSpecialCorner4RemoveObject(struct Element *param_1, TileId param_2, struct Cursor *param_3) {
    FlumeSpecialCornerIndex = 3;
    LogFlumePieceRemoveObject(param_1, param_2, param_3, GetFlumeSpecialCornerConnections);
}

// FUNCTION: LEGOLAND 0x0040ed50
struct RideSpriteInfo *GetFlumePieceSpriteInfo(struct Element *arg1, TileId arg2) {
    struct Ride *src = arg1->ride;
    struct FlumeEntry *entry = FindFlumeSubEntryByTile(&arg2);
    if (entry != NULL) {
        DrawFlumeTrackEnds((struct FlumeEntry *)entry, 0);
        FlumePieceSpriteInfo.id = arg2.id;
        FlumePieceSpriteInfo.sprite = src->layer;
        FlumePieceSpriteInfo.x = src->field_14;
        FlumePieceSpriteInfo.y = src->field_18;
        FlumePieceSpriteInfo.field_10 = 0;
    }
    return &FlumePieceSpriteInfo;
}

// FUNCTION: LEGOLAND 0x0040edb0
void RenderLogFlumeSpecialCorner1(Element *elem, unsigned int param_2, unsigned int param_3, TileId *tile) {
    struct Ride *ride = elem->ride;
    struct FlumeEntry *entry;
    struct LLS *lls;
    struct Point pos;
    int frame = 0;

    FlumeSpecialCornerIndex = 0;
    pos = GetScreenCoordsForObject(tile, ride);
    lls = GetLLSForSprite((struct Sprite *)ride->layer);
    if (lls != NULL) {
        frame = lls->frame;
    }
    lls = GetLLSForSprite((struct Sprite *)LogFlumeFc1M1Sprite);
    if (lls != NULL) {
        LLSSetFrame(lls, frame);
    }
    lls = GetLLSForSprite((struct Sprite *)LogFlumeFc1M2Sprite);
    if (lls != NULL) {
        LLSSetFrame(lls, frame);
    }
    DrawFlumeBarrelsAtTile(tile);
    entry = FindFlumeSubEntryByTile(tile);
    if (entry != NULL) {
        DrawFlumeBarrelsByStage(entry, pos.x, pos.y, (struct FlumeStageList *)&LogFlumeSpecialCorner1Stages, 0);
        DrawFlumeTrackEnds((struct FlumeEntry *)entry, 1);
    }
}

// FUNCTION: LEGOLAND 0x0040ee60
void RenderLogFlumeSpecialCorner2(Element *elem, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = elem->ride;
    struct FlumeEntry *entry;
    struct LLS *lls;
    struct Point pos;
    int frame;

    FlumeSpecialCornerIndex = 1;
    entry = FindFlumeSubEntryByTile(tile);
    if (DrawFlumeBarrelsAtTile(tile)) {
        frame = 0;
        lls = GetLLSForSprite((struct Sprite *)ride->layer);
        if (lls != NULL) {
            frame = lls->frame;
        }
        lls = GetLLSForSprite((struct Sprite *)LogFlumeFc2M1Sprite);
        if (lls != NULL) {
            LLSSetFrame(lls, frame);
        }
        pos = GetScreenCoordsForObject(tile, ride);
        if (LogFlumeFc2M1Sprite != NULL) {
            PrintSprite(LogFlumeFc2M1Sprite, pos.x, pos.y, clip, 0);
        }
    }
    DrawFlumeTrackEnds((struct FlumeEntry *)entry, 1);
}

// FUNCTION: LEGOLAND 0x0040ef00
void RenderLogFlumeSpecialCorner3(Element *elem, unsigned int param_2, unsigned int param_3, TileId *tile) {
    struct Ride *ride = elem->ride;
    struct FlumeEntry *entry;
    struct LLS *lls;
    struct Point pos;
    int frame;

    FlumeSpecialCornerIndex = 2;
    frame = 0;
    lls = GetLLSForSprite((struct Sprite *)ride->layer);
    if (lls != NULL) {
        frame = lls->frame;
    }
    lls = GetLLSForSprite((struct Sprite *)LogFlumeFc3M1Sprite);
    if (lls != NULL) {
        LLSSetFrame(lls, frame);
    }
    lls = GetLLSForSprite((struct Sprite *)LogFlumeFc3M2Sprite);
    if (lls != NULL) {
        LLSSetFrame(lls, frame);
    }
    pos = GetScreenCoordsForObject(tile, ride);
    DrawFlumeBarrelsAtTile(tile);
    entry = FindFlumeSubEntryByTile(tile);
    if (entry != NULL) {
        DrawFlumeBarrelsByStage(entry, pos.x, pos.y, (struct FlumeStageList *)&LogFlumeSpecialCorner3Stages, 1);
        DrawFlumeTrackEnds((struct FlumeEntry *)entry, 1);
    }
}

// FUNCTION: LEGOLAND 0x0040efb0
void RenderLogFlumeSpecialCorner4(Element *elem, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = elem->ride;
    struct FlumeEntry *entry;
    struct LLS *lls;
    struct Point pos;
    int frame;

    FlumeSpecialCornerIndex = 3;
    entry = FindFlumeSubEntryByTile(tile);
    if (DrawFlumeBarrelsAtTile(tile)) {
        frame = 0;
        lls = GetLLSForSprite((struct Sprite *)ride->layer);
        if (lls != NULL) {
            frame = lls->frame;
        }
        lls = GetLLSForSprite((struct Sprite *)LogFlumeFc4MSprite);
        if (lls != NULL) {
            LLSSetFrame(lls, frame);
        }
        pos = GetScreenCoordsForObject(tile, ride);
        if (LogFlumeFc4MSprite != NULL) {
            PrintSprite(LogFlumeFc4MSprite, pos.x, pos.y, clip, 0);
        }
        DrawFlumeTrackEnds((struct FlumeEntry *)entry, 1);
    }
}

static __inline struct FlumeNode *NewFlumeNode(struct FlumeNode *parent, int mode, int submode, unsigned char y, unsigned char x) {
    struct FlumeNode *node = AllocFlumeNode();
    if (node != NULL) {
        node->mode = mode;
        node->submode = submode;
        node->owner = parent->owner;
        node->entry = parent;
        node->ride = (struct Ride *)LogFlumeTrackRide;
        node->flags |= 4;
        node->tile.pos.x = x;
        node->tile.pos.y = y;
    }
    return node;
}

// FUNCTION: LEGOLAND 0x0040f050
void BuildLogFlumeTunnelNodes(struct FlumeNode *parent) {
    struct FlumeNode *node;
    struct FlumeNode *prev;
    TileId pos;
    int w = LogFlumeFootprint.x1 - LogFlumeFootprint.x0;
    int h = LogFlumeFootprint.y1 - LogFlumeFootprint.y0;

    pos.pos.x = ((struct Ride *)LogFlumeTunnelRide)->footprint.x0 + parent->tile.pos.x + 6;
    pos.pos.y = ((struct Ride *)LogFlumeTunnelRide)->footprint.y0 + parent->tile.pos.y;
    node = NewFlumeNode(parent, 3, 0, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    parent->field_30 = node;
    pos.pos.y += h;
    prev = NewFlumeNode(parent, 1, 0, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)prev);
    LinkNodeAfter((struct FlumeEntry *)node, (struct FlumeEntry *)prev);
    pos.pos.y += h;
    node = NewFlumeNode(parent, 2, 3, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    pos.pos.x -= w;
    prev = node;
    node = NewFlumeNode(parent, 1, 1, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    pos.pos.x -= w;
    prev = node;
    node = NewFlumeNode(parent, 2, 1, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    pos.pos.y += h;
    prev = node;
    node = NewFlumeNode(parent, 1, 0, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    pos.pos.y += h;
    prev = node;
    node = NewFlumeNode(parent, 3, 2, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    parent->field_34 = node;
}

// FUNCTION: LEGOLAND 0x0040f300
void GetLogFlumeTunnelEndNodes(struct FlumeEntry *src, struct FlumeRect *dst) {
    dst->var_0 = 0;
    dst->var_4 = 0;
    dst->var_8 = 0;
    dst->var_c = 0;
    dst->var_0 = src->a_30;
    dst->var_8 = src->b_34;
}

// FUNCTION: LEGOLAND 0x0040f330
int CanPlaceLogFlumeTunnel(struct FlumeEntry **param_1) {
    int flags = FlumeNeighbourMask(param_1);
    if (flags == 0x11 || flags == 0x1 || flags == 0x10) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040f360
void GetLogFlumeTunnelConnections(TileId p, unsigned int *result) {
    unsigned int w = LogFlumeFootprint.y1 - LogFlumeFootprint.y0;

    p.pos.x += (unsigned char)LogFlumeTunnelRide->footprint.v[0];
    p.pos.y += (unsigned char)LogFlumeTunnelRide->footprint.v[1];
    result[0] = 5;
    result[1] = p.pos.x + 6;
    result[2] = p.pos.y;
    result[5] = p.pos.x + 2;
    result[6] = LogFlumeTunnelRide->footprint.v[3] - LogFlumeTunnelRide->footprint.v[1] - w + p.pos.y + 1;
}

// FUNCTION: LEGOLAND 0x0040f3e0
void LogFlumeTunnelLoadSprites(struct Element *obj_ptr) {
    struct Ride *resA;
    struct Sprite *resB;

    LogFlumeTunnelRide = (struct Ride *)obj_ptr->data;
    resA = (struct Ride *)LogFlumeTunnelRide;
    resA->flags |= 0x400;
    LogFlumeTunnelElement = obj_ptr;

    resA = (struct Ride *)LogFlumeTunnelRide;
    if (resA != NULL) {
        resB = resA->layer;
        if (resB != NULL) {
            resB->flags |= 0x2000;
        }
    }

    // STRING: LEGOLAND 0x004b4a94
    LogFlumeTunelMSprite = LoadSprite("tunel_m.lls", 1);
}

// FUNCTION: LEGOLAND 0x0040f430
void LogFlumeTunnelUnloadSprites(void) {
    if (LogFlumeTunelMSprite != NULL) {
        KillSprite(LogFlumeTunelMSprite);
    }
}

// FUNCTION: LEGOLAND 0x0040f450
void RenderLogFlumeTunnel(Element *elem, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = elem->ride;
    struct FlumeEntry *entry;
    struct LLS *lls;
    struct Point pos;
    int frame;

    entry = FindFlumeSubEntryByTile(tile);
    if (DrawFlumeBarrelsAtTile(tile)) {
        if (LogFlumeTunelMSprite != NULL) {
            lls = GetLLSForSprite((struct Sprite *)ride->layer);
            frame = lls != NULL ? lls->frame : frame;
            lls = GetLLSForSprite((struct Sprite *)LogFlumeTunelMSprite);
            if (lls != NULL) {
                LLSSetFrame(lls, frame);
            }
        }
        pos = GetFlumePieceScreenPos(entry);
        if (LogFlumeTunelMSprite != NULL) {
            PrintSprite(LogFlumeTunelMSprite, pos.x, pos.y, clip, 0);
        }
    }
    DrawFlumeTrackEnds((struct FlumeEntry *)entry, 1);
}

// FUNCTION: LEGOLAND 0x0040f4f0
void LogFlumeTunnelSetEditMode(void) {
    SetFlumeEditCursor(LogFlumeTunnelRide, LogFlumeTunnelRide->footprint.v);
}

// FUNCTION: LEGOLAND 0x0040f510
void LogFlumeTunnelCalcCursor(struct Element *param_1, struct Point *param_2, unsigned int param_3) {
    LogFlumePieceCalcCursor(LogFlumeTunnelRide, &param_2->x, param_3, LogFlumeTunnelRide->footprint.v, GetLogFlumeTunnelConnections, CanPlaceLogFlumeTunnel);
}

// FUNCTION: LEGOLAND 0x0040f540
void LogFlumeTunnelAddObject(struct Element *param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    LogFlumePieceAddObject(packed, LogFlumeTunnelRide->footprint.v, LogFlumeTunnelElement, BuildLogFlumeTunnelNodes, GetLogFlumeTunnelConnections, (FlumeEdgeFn)GetLogFlumeTunnelEndNodes);
}

// FUNCTION: LEGOLAND 0x0040f580
void LogFlumeTunnelRemoveObject(struct Element *param_1, TileId param_2, struct Cursor *param_3) {
    LogFlumePieceRemoveObject(param_1, param_2, param_3, GetLogFlumeTunnelConnections);
}

// FUNCTION: LEGOLAND 0x0040f5a0
void LogFlumeTunnelDCalcCursor(struct Element *param_1, struct Point *param_2) {
    SetFlumeQueryCursor(param_2);
}

// FUNCTION: LEGOLAND 0x0040f5b0
void BuildLogFlumeCsaw2Nodes(struct FlumeNode *param_1) {
    struct FlumeNode *parent = param_1;
    struct FlumeNode *node;
    struct FlumeNode *prev;
    struct FlumeNode *last;
    TileId pos;
    int w = LogFlumeFootprint.x1 - LogFlumeFootprint.x0;
    int h = LogFlumeFootprint.y1 - LogFlumeFootprint.y0;
    int i;

    pos.pos.x = ((struct Ride *)LogFlumeCsaw2Ride)->footprint.x0 + parent->tile.pos.x;
    pos.pos.y = ((struct Ride *)LogFlumeCsaw2Ride)->footprint.y0 + parent->tile.pos.y + 3;
    node = NewFlumeNode(parent, 3, 3, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    parent->field_30 = node;
    prev = node;
    for (i = 4; i != 0; i--) {
        pos.pos.x += w;
        node = NewFlumeNode(parent, 1, 1, pos.pos.y, pos.pos.x);
        AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
        LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
        prev = node;
    }
    pos.pos.x += w;
    node = NewFlumeNode(parent, 2, 3, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    prev = node;
    pos.pos.y -= h;
    node = NewFlumeNode(parent, 2, 1, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    prev = node;
    pos.pos.x += w;
    last = NewFlumeNode(parent, 3, 1, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)last);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)last);
    parent->field_34 = last;
}

// FUNCTION: LEGOLAND 0x0040f7d0
void GetLogFlumeCsawEndNodes(struct FlumeEntry *src, struct FlumeRect *dst) {
    dst->var_0 = 0;
    dst->var_4 = 0;
    dst->var_8 = 0;
    dst->var_c = 0;
    dst->var_c = src->a_30;
    dst->var_4 = src->b_34;
}

// FUNCTION: LEGOLAND 0x0040f800
unsigned int CanPlaceLogFlumeCsaw(struct FlumeEntry **param_1) {
    int flags = FlumeNeighbourMask(param_1);
    if (flags == 0x44 || flags == 0x4 || flags == 0x40) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040f830
void GetLogFlumeCsaw2Connections(TileId p, unsigned int *result) {
    unsigned int w = LogFlumeFootprint.x1 - LogFlumeFootprint.x0;

    p.pos.x += (unsigned char)LogFlumeCsaw2Ride->footprint.v[0];
    p.pos.y += (unsigned char)LogFlumeCsaw2Ride->footprint.v[1];
    result[0] = 10;
    result[7] = p.pos.x;
    result[8] = p.pos.y + 3;
    result[3] = LogFlumeCsaw2Ride->footprint.v[2] - LogFlumeCsaw2Ride->footprint.v[0] - w + p.pos.x + 1;
    result[4] = p.pos.y + 1;
}

// FUNCTION: LEGOLAND 0x0040f8b0
struct Sprite *LoadLogFlumeCsaw2(struct Element *obj_ptr) {
    struct Ride *resA;
    struct Sprite *resB;

    LogFlumeCsaw2Ride = (struct Ride *)obj_ptr->data;
    resA = (struct Ride *)LogFlumeCsaw2Ride;
    resA->flags |= 0x400;
    LogFlumeCsaw2Element = obj_ptr;

    resA = (struct Ride *)LogFlumeCsaw2Ride;
    if (resA != NULL) {
        resB = resA->layer;
        if (resB != NULL) {
            resB->flags |= 0x2000;
        }
    }

    // STRING: LEGOLAND 0x004b4aa0
    LogFlumeCsaw2MSprite = LoadSprite("csaw2_m.lls", 1);
    return LogFlumeCsaw2MSprite;
}

// FUNCTION: LEGOLAND 0x0040f900
void LogFlumeCsawUnloadSprites(void) {
    if (LogFlumeCsaw2MSprite != 0) {
        KillSprite(LogFlumeCsaw2MSprite);
    }
}

// FUNCTION: LEGOLAND 0x0040f920
void RenderLogFlumeCsaw(Element *elem, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = elem->ride;
    struct FlumeEntry *entry;
    struct LLS *lls;
    struct Point off;
    struct Point pos;
    int frame;

    entry = FindFlumeSubEntryByTile(tile);
    if (DrawFlumeBarrelsAtTile(tile)) {
        if (LogFlumeCsaw2MSprite != NULL) {
            lls = GetLLSForSprite((struct Sprite *)GetSpriteForLayer(ride->layer, 1));
            if (lls != NULL) {
                frame = lls->frame;
            }
            lls = GetLLSForSprite((struct Sprite *)LogFlumeCsaw2MSprite);
            if (lls != NULL) {
                LLSSetFrame(lls, frame);
            }
        }
        off = GetRenderOffsetForLayer(ride->layer, 1);
        pos = GetFlumePieceScreenPos(entry);
        AdjustOffsetForViewMode(&off);
        if (LogFlumeCsaw2MSprite != NULL) {
            PrintSprite(LogFlumeCsaw2MSprite, pos.x + off.x, pos.y + off.y, clip, 0);
        }
    }
    DrawFlumeTrackEnds((struct FlumeEntry *)entry, 1);
}

// FUNCTION: LEGOLAND 0x0040fa00
void LogFlumeCsawSetEditMode(void) {
    SetFlumeEditCursor(LogFlumeCsaw2Ride, LogFlumeCsaw2Ride->footprint.v);
}

// FUNCTION: LEGOLAND 0x0040fa20
void LogFlumeCsawCalcCursor(struct Element *param_1, struct Point *param_2, unsigned int param_3) {
    struct Ride *src = LogFlumeCsaw2Ride;
    LogFlumePieceCalcCursor(src, &param_2->x, param_3, src->footprint.v, GetLogFlumeCsaw2Connections, (FlumeCheckFn)CanPlaceLogFlumeCsaw);
}

// FUNCTION: LEGOLAND 0x0040fa50
void LogFlumeCsawDCalcCursor(struct Element *param_1, struct Point *param_2) {
    SetFlumeQueryCursor(param_2);
}

// FUNCTION: LEGOLAND 0x0040fa60
void LogFlumeCsawAddObject(struct Element *param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    FlumeSpecialCornerIndex = 0;
    LogFlumePieceAddObject(packed, LogFlumeCsaw2Ride->footprint.v, LogFlumeCsaw2Element, BuildLogFlumeCsaw2Nodes, GetLogFlumeCsaw2Connections, (FlumeEdgeFn)GetLogFlumeCsawEndNodes);
}

// FUNCTION: LEGOLAND 0x0040fab0
void LogFlumeCsawRemoveObject(struct Element *param_1, TileId param_2, struct Cursor *param_3) {
    LogFlumePieceRemoveObject(param_1, param_2, param_3, GetLogFlumeCsaw2Connections);
}

#define NEW_FLUME_NODE(node, parent, mode_, submode_, y_, x_) \
    do { \
        (node) = AllocFlumeNode(); \
        if ((node) != NULL) { \
            (node)->mode = (mode_); \
            (node)->submode = (submode_); \
            (node)->owner = (parent)->owner; \
            (node)->entry = (parent); \
            (node)->ride = (struct Ride *)LogFlumeTrackRide; \
            (node)->flags |= 4; \
            (node)->tile.pos.x = (x_); \
            (node)->tile.pos.y = (y_); \
        } \
    } while (0)

// FUNCTION: LEGOLAND 0x0040fad0
void BuildLogFlumeHoldUpNodes(struct FlumeNode *parent) {
    struct FlumeNode *node;
    struct FlumeNode *prev;
    TileId pos;
    int w = LogFlumeFootprint.x1 - LogFlumeFootprint.x0;
    int h = LogFlumeFootprint.y1 - LogFlumeFootprint.y0;
    int i;

    pos.pos.x = ((struct Ride *)LogFlumeHoldUpRide)->footprint.x0 + parent->tile.pos.x;
    pos.pos.y = ((struct Ride *)LogFlumeHoldUpRide)->footprint.y0 + parent->tile.pos.y + 9;
    NEW_FLUME_NODE(node, parent, 3, 3, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    parent->field_30 = node;
    prev = node;
    pos.pos.x += w;
    NEW_FLUME_NODE(node, parent, 1, 1, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    prev = node;
    pos.pos.x += w;
    NEW_FLUME_NODE(node, parent, 2, 3, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    prev = node;
    pos.pos.y -= h;
    NEW_FLUME_NODE(node, parent, 1, 0, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    prev = node;
    pos.pos.y -= h;
    NEW_FLUME_NODE(node, parent, 2, 1, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    prev = node;
    for (i = 2; i != 0; i--) {
        pos.pos.x += w;
        NEW_FLUME_NODE(node, parent, 1, 1, pos.pos.y, pos.pos.x);
        AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
        LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
        prev = node;
    }
    pos.pos.x += w;
    NEW_FLUME_NODE(node, parent, 2, 2, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    prev = node;
    pos.pos.y += h;
    NEW_FLUME_NODE(node, parent, 2, 0, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    prev = node;
    pos.pos.x += w;
    NEW_FLUME_NODE(node, parent, 3, 1, pos.pos.y, pos.pos.x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    parent->field_34 = node;
}

// FUNCTION: LEGOLAND 0x0040fe50
void GetLogFlumeHoldUpEndNodes(struct FlumeEntry *src, struct FlumeRect *dst) {
    dst->var_0 = 0;
    dst->var_4 = 0;
    dst->var_8 = 0;
    dst->var_c = 0;
    dst->var_c = src->a_30;
    dst->var_4 = src->b_34;
}

// FUNCTION: LEGOLAND 0x0040fe80
unsigned int CanPlaceLogFlumeHoldUp(struct FlumeEntry **param_1) {
    int flags = FlumeNeighbourMask(param_1);
    if (flags == 0x44 || flags == 0x4 || flags == 0x40) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040feb0
void GetLogFlumeHoldUpConnections(TileId p, unsigned int *result) {
    unsigned int w = LogFlumeFootprint.x1 - LogFlumeFootprint.x0;

    p.pos.x += (unsigned char)LogFlumeHoldUpRide->footprint.v[0];
    p.pos.y += (unsigned char)LogFlumeHoldUpRide->footprint.v[1];
    result[0] = 10;
    result[7] = p.pos.x;
    result[8] = p.pos.y + 9;
    result[3] = LogFlumeHoldUpRide->footprint.v[2] - LogFlumeHoldUpRide->footprint.v[0] - w + p.pos.x + 1;
    result[4] = p.pos.y + 7;
}

// FUNCTION: LEGOLAND 0x0040ff30
void LogFlumeHoldUpLoadSprites(struct Element *obj_ptr) {
    struct Ride *resA;
    struct Sprite *resB;

    LogFlumeHoldUpRide = (struct Ride *)obj_ptr->data;
    resA = (struct Ride *)LogFlumeHoldUpRide;
    resA->flags |= 0x400;

    resA = (struct Ride *)LogFlumeHoldUpRide;
    LogFlumeHoldUpElement = obj_ptr;
    if (resA != NULL) {
        resB = resA->layer;
        if (resB != NULL) {
            resB->flags |= 0x2000;
        }
    }

    // STRING: LEGOLAND 0x004b4ab8
    LogFlumeHup1M1Sprite = LoadSprite("hup1_m1.lls", 1);
    // STRING: LEGOLAND 0x004b4aac
    LogFlumeHup1M2Sprite = LoadSprite("hup1_m2.lls", 1);
    LogFlumeHup1M1StageSprite = LogFlumeHup1M1Sprite;
    LogFlumeHup1M2StageSprite = LogFlumeHup1M2Sprite;
}

// FUNCTION: LEGOLAND 0x0040ffa0
void LogFlumeHoldUpUnloadSprites(void) {
    if (LogFlumeHup1M1Sprite != 0) {
        KillSprite(LogFlumeHup1M1Sprite);
    }
    if (LogFlumeHup1M2Sprite != 0) {
        KillSprite(LogFlumeHup1M2Sprite);
    }
}

// FUNCTION: LEGOLAND 0x0040ffd0
void RenderLogFlumeHoldUp(Element *elem, unsigned int param_2, unsigned int param_3, TileId *tile) {
    struct Ride *ride = elem->ride;
    struct FlumeEntry *entry;
    struct LLS *lls;
    struct Sprite *sprite;
    struct Point pos;
    struct Point off;
    int frame;

    frame = 0;
    sprite = GetSpriteForLayer(ride->layer, 1);
    if (sprite != NULL) {
        lls = GetLLSForSprite((struct Sprite *)sprite);
        if (lls != NULL) {
            frame = lls->frame;
        }
    }
    lls = GetLLSForSprite((struct Sprite *)LogFlumeHup1M1Sprite);
    if (lls != NULL) {
        LLSSetFrame(lls, frame);
    }
    lls = GetLLSForSprite((struct Sprite *)LogFlumeHup1M2Sprite);
    if (lls != NULL) {
        LLSSetFrame(lls, frame);
    }
    pos = GetScreenCoordsForObject(tile, ride);
    off = GetRenderOffsetForLayer(ride->layer, 1);
    AdjustOffsetForViewMode(&off);
    pos.x += off.x;
    pos.y += off.y;
    DrawFlumeBarrelsAtTile(tile);
    entry = FindFlumeSubEntryByTile(tile);
    if (entry != NULL) {
        DrawFlumeBarrelsByStage(entry, pos.x, pos.y, (struct FlumeStageList *)&LogFlumeHoldUpStages, 0);
        DrawFlumeTrackEnds((struct FlumeEntry *)entry, 1);
    }
}

// FUNCTION: LEGOLAND 0x004100b0
void LogFlumeHoldUpSetEditMode(void) {
    SetFlumeEditCursor(LogFlumeHoldUpRide, LogFlumeHoldUpRide->footprint.v);
}

// FUNCTION: LEGOLAND 0x004100d0
unsigned int LogFlumeHoldUpCalcCursor(struct Element *param_1, struct Point *param_2, unsigned int param_3) {
    struct Ride *src = LogFlumeHoldUpRide;
    return LogFlumePieceCalcCursor(src, &param_2->x, param_3, src->footprint.v, (FlumeCallback)GetLogFlumeHoldUpConnections, (FlumeCheckFn)CanPlaceLogFlumeHoldUp);
}

// FUNCTION: LEGOLAND 0x00410100
void LogFlumeHoldUpDCalcCursor(struct Element *param_1, struct Point *param_2) {
    SetFlumeQueryCursor(param_2);
}

// FUNCTION: LEGOLAND 0x00410110
void LogFlumeHoldUpAddObject(struct Element *param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    FlumeSpecialCornerIndex = 0;
    LogFlumePieceAddObject(packed, LogFlumeHoldUpRide->footprint.v, LogFlumeHoldUpElement, BuildLogFlumeHoldUpNodes, (FlumeCallback)GetLogFlumeHoldUpConnections, (FlumeEdgeFn)GetLogFlumeHoldUpEndNodes);
}

// FUNCTION: LEGOLAND 0x00410160
void LogFlumeHoldUpRemoveObject(struct Element *param_1, TileId param_2, struct Cursor *param_3) {
    LogFlumePieceRemoveObject(param_1, param_2, param_3, (FlumeCallback)GetLogFlumeHoldUpConnections);
}

// FUNCTION: LEGOLAND 0x00410180
void BuildLogFlumeDropNodes(struct FlumeNode *param_1) {
    struct FlumeNode *parent = param_1;
    struct FlumeNode *node;
    struct FlumeNode *prev;
    unsigned char x;
    unsigned char y;
    int h = LogFlumeFootprint.y1 - LogFlumeFootprint.y0;
    int i;

    y = ((struct Ride *)LogFlumeDropRide)->footprint.y0 + parent->tile.pos.y;
    x = ((struct Ride *)LogFlumeDropRide)->footprint.x0 + parent->tile.pos.x + 2;
    node = NewFlumeNode(parent, 3, 0, y, x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    parent->field_30 = node;
    prev = node;
    for (i = 0; i < 0x18 / h - 2; i++) {
        y += h;
        node = NewFlumeNode(parent, 1, 0, y, x);
        AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
        LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
        prev = node;
    }
    y += h;
    node = NewFlumeNode(parent, 3, 2, y, x);
    AppendListNode((struct FlumeEntry *)parent, (struct ListNode *)node);
    LinkNodeAfter((struct FlumeEntry *)prev, (struct FlumeEntry *)node);
    parent->field_34 = node;
}

// FUNCTION: LEGOLAND 0x004102e0
void GetLogFlumeDropEndNodes(struct FlumeEntry *src, struct FlumeRect *dst) {
    dst->var_0 = 0;
    dst->var_4 = 0;
    dst->var_8 = 0;
    dst->var_c = 0;
    dst->var_0 = src->a_30;
    dst->var_8 = src->b_34;
}

// FUNCTION: LEGOLAND 0x00410310
int CanPlaceLogFlumeDrop(struct FlumeEntry **param) {
    int result = FlumeNeighbourMask(param);
    if (result != 0x11 && result != 0x1 && result != 0x10) {
        return 0;
    }
    if (result & 0x1) {
        if (param[0]->field_8 != 0) {
            return 0;
        }
    }
    if (result & 0x10) {
        if (param[2]->field_c != 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00410360
void GetLogFlumeDropConnections(TileId p, unsigned int *result) {
    unsigned int w = LogFlumeFootprint.y1 - LogFlumeFootprint.y0;

    p.pos.x += (unsigned char)LogFlumeDropRide->footprint.v[0];
    p.pos.y += (unsigned char)LogFlumeDropRide->footprint.v[1];
    result[0] = 5;
    result[1] = p.pos.x + 2;
    result[2] = p.pos.y;
    result[5] = p.pos.x + 2;
    result[6] = LogFlumeDropRide->footprint.v[3] - LogFlumeDropRide->footprint.v[1] - w + p.pos.y + 1;
}

// FUNCTION: LEGOLAND 0x004103e0
struct Sprite *LoadLogFlumeDrop(struct Element *obj_ptr) {
    struct Ride *resA;
    struct Sprite *resB;

    LogFlumeDropRide = (struct Ride *)obj_ptr->data;
    resA = (struct Ride *)LogFlumeDropRide;
    resA->flags |= 0x400;

    resA = (struct Ride *)LogFlumeDropRide;
    LogFlumeDropElement = obj_ptr;
    if (resA != NULL) {
        resB = resA->layer;
        if (resB != NULL) {
            resB->flags |= 0x2000;
        }
    }

    // STRING: LEGOLAND 0x004b4ae0
    LogFlumeDrop1MSprite = LoadSprite("drop1_m.lls", 1);
    // STRING: LEGOLAND 0x004b4ad4
    LogFlumeDrop2MSprite = LoadSprite("drop2_m.lls", 1);
    // STRING: LEGOLAND 0x004b4ac4
    LogFlumeSplashSprite = LoadSprite("lf_splash.lls", 1);

    return LogFlumeSplashSprite;
}

// FUNCTION: LEGOLAND 0x00410450
void LogFlumeDropUnloadSprites(void) {
    if (LogFlumeSplashSprite != 0) {
        KillSprite(LogFlumeSplashSprite);
        LogFlumeSplashSprite = 0;
    }
    if (LogFlumeDrop1MSprite != 0) {
        KillSprite(LogFlumeDrop1MSprite);
        LogFlumeDrop1MSprite = 0;
    }
    if (LogFlumeDrop2MSprite != 0) {
        KillSprite(LogFlumeDrop2MSprite);
        LogFlumeDrop2MSprite = 0;
    }
}

// FUNCTION: LEGOLAND 0x004104b0
void RenderLogFlumeDrop(Element *elem, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = elem->ride;
    struct FlumeEntry *entry;
    struct Sprite *sprite;
    struct LLS *lls;
    struct Point off;
    struct Point pos;
    int frame;

    entry = FindFlumeSubEntryByTile(tile);
    if (DrawFlumeBarrelsAtTile(tile) && entry != NULL) {
        if (LogFlumeDrop1MSprite != NULL && LogFlumeDrop2MSprite != NULL) {
            frame = 0;
            sprite = GetSpriteForLayer(ride->layer, 0);
            if (sprite != NULL) {
                lls = GetLLSForSprite((struct Sprite *)sprite);
                if (lls != NULL) {
                    frame = lls->frame;
                }
            }
            lls = GetLLSForSprite((struct Sprite *)LogFlumeDrop1MSprite);
            if (lls != NULL) {
                LLSSetFrame(lls, frame);
            }
            sprite = GetSpriteForLayer(ride->layer, 1);
            if (sprite != NULL) {
                lls = GetLLSForSprite((struct Sprite *)sprite);
                if (lls != NULL) {
                    frame = lls->frame;
                }
            }
            lls = GetLLSForSprite((struct Sprite *)LogFlumeDrop2MSprite);
            if (lls != NULL) {
                LLSSetFrame(lls, frame);
            }
        }
        pos = GetFlumePieceScreenPos(entry);
        if (LogFlumeDrop1MSprite != NULL) {
            off = GetRenderOffsetForLayer(ride->layer, 0);
            AdjustOffsetForViewMode(&off);
            PrintSprite(LogFlumeDrop1MSprite, pos.x + off.x, pos.y + off.y, clip, 0);
        }
        if (LogFlumeDrop2MSprite != NULL) {
            off = GetRenderOffsetForLayer(ride->layer, 1);
            AdjustOffsetForViewMode(&off);
            PrintSprite(LogFlumeDrop2MSprite, pos.x + off.x, pos.y + off.y, clip, 0);
        }
        if (entry->field_24 != NULL && (entry->field_24->flags & 2)) {
            pos = GetFlumePieceScreenPos(entry);
            off = GetRenderOffsetForLayer(ride->layer, 1);
            off.x += LogFlumeDropDrawOffset.x;
            off.y += LogFlumeDropDrawOffset.y;
            AdjustOffsetForViewMode(&off);
            if (LogFlumeSplashSprite != NULL) {
                frame = ((struct FlumeSlot *)entry->field_24)[1].timer; /* the next slot */
                lls = GetLLSForSprite((struct Sprite *)LogFlumeSplashSprite);
                if (lls != NULL) {
                    LLSStop((struct LLS *)lls);
                    LLSSetFrame(lls, frame);
                }
                PrintSprite(LogFlumeSplashSprite, pos.x + off.x, pos.y + off.y, clip, 0);
            }
        }
    }
    DrawFlumeTrackEnds((struct FlumeEntry *)entry, 1);
}

// FUNCTION: LEGOLAND 0x004106e0
unsigned int LogFlumeDropSetEditMode(void) {
    return SetFlumeEditCursor(LogFlumeDropRide, LogFlumeDropRide->footprint.v);
}

// FUNCTION: LEGOLAND 0x00410700
unsigned int LogFlumeDropCalcCursor(struct Element *param_1, struct Point *param_2, unsigned int param_3) {
    struct Ride *src = LogFlumeDropRide;
    return LogFlumePieceCalcCursor(src, &param_2->x, param_3, src->footprint.v, GetLogFlumeDropConnections, (FlumeCheckFn)CanPlaceLogFlumeDrop);
}

// FUNCTION: LEGOLAND 0x00410730
void LogFlumeDropDCalcCursor(struct Element *param_1, struct Point *param_2) {
    SetFlumeQueryCursor(param_2);
}

// FUNCTION: LEGOLAND 0x00410740
void LogFlumeDropAddObject(struct Element *param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    FlumeSpecialCornerIndex = 0;
    LogFlumePieceAddObject(packed, LogFlumeDropRide->footprint.v, LogFlumeDropElement, BuildLogFlumeDropNodes, GetLogFlumeDropConnections, (FlumeEdgeFn)GetLogFlumeDropEndNodes);
}

// FUNCTION: LEGOLAND 0x00410790
void LogFlumeDropRemoveObject(struct Element *param_1, TileId param_2, struct Cursor *param_3) {
    LogFlumePieceRemoveObject(param_1, param_2, param_3, GetLogFlumeDropConnections);
}

// FUNCTION: LEGOLAND 0x004107b0
int FlumeNodeToIndex(struct FlumeNode *node, struct FlumeNode *target) {
    int idx = 1;
    int r;

    while (node != NULL) {
        if (node == target) {
            return idx;
        }
        r = FlumeNodeToIndex(node->field_2c, target);
        if (r != -1) {
            return (r << 16) | idx;
        }
        node = node->next;
        idx++;
    }
    return -1;
}

// FUNCTION: LEGOLAND 0x00410800
void SaveFlumeNodes(struct FlumeEntry *owner, struct FlumeNode *node) {
    int marker = 1;
    int end = 0;

    if (node != NULL) {
        do {
            struct FlumeNode buf;
            int len;

            SaveGameWrite(&marker, 4);
            buf = *node;
            buf.field_8 = (struct FlumeNode *)(INT_PTR)FlumeNodeToIndex((struct FlumeNode *)owner->sub, buf.field_8);
            buf.field_c = (struct FlumeNode *)(INT_PTR)FlumeNodeToIndex((struct FlumeNode *)owner->sub, buf.field_c);
            buf.field_30 = (struct FlumeNode *)(INT_PTR)FlumeNodeToIndex((struct FlumeNode *)owner->sub, buf.field_30);
            buf.field_34 = (struct FlumeNode *)(INT_PTR)FlumeNodeToIndex((struct FlumeNode *)owner->sub, buf.field_34);
            SaveGameWrite(&buf, sizeof(buf));
            len = strlen(buf.ride->element->name);
            SaveGameWrite(&len, 4);
            SaveGameWrite(buf.ride->element->name, len);
            SaveFlumeNodes(owner, node->field_2c);
            node = node->next;
        } while (node != NULL);
    }
    SaveGameWrite(&end, 4);
}

// FUNCTION: LEGOLAND 0x00410910
int GetFlumeTargetSlotIndex(struct FlumeEntry *entry) {
    int count = 0;
    void *target = entry->target;
    char *current = (char *)entry->slots;

    while (count < 4) {
        if (target == (void *)current) {
            return count;
        }
        count = count + 1;
        current = current + 0x24;
    }
    return -1;
}

// FUNCTION: LEGOLAND 0x00410930
int LogFlumeEntrance_Save(struct Element *element) {
    struct FlumeEntry *entry = FlumeEntryList;
    int marker = 1;
    int end = 0;
    int i;

    if (entry != NULL) {
        do {
            struct FlumeEntry buf;
            SaveGameWrite(&marker, 4);
            SaveFlumeNodes(entry, (struct FlumeNode *)entry->sub);
            buf = *entry;
            buf.field_8 = (void *)(INT_PTR)FlumeNodeToIndex((struct FlumeNode *)buf.sub, buf.field_8);
            buf.field_c = (void *)(INT_PTR)FlumeNodeToIndex((struct FlumeNode *)buf.sub, (struct FlumeNode *)buf.field_c);
            buf.link = (struct FlumeEntry *)(INT_PTR)FlumeNodeToIndex((struct FlumeNode *)buf.sub, (struct FlumeNode *)buf.link);
            buf.target = (void *)(INT_PTR)GetFlumeTargetSlotIndex(entry);
            SaveQueue((struct QueueNode *)((struct Ride *)LogFlumeEntranceRide)->riders, &buf.queue);
            for (i = 0; i < 4; i++) {
                buf.slots[i].owner = (struct FlumeEntry *)(INT_PTR)FlumeNodeToIndex((struct FlumeNode *)buf.sub, (struct FlumeNode *)buf.slots[i].owner);
                buf.slots[i].busy = (struct RideNode *)(UINT_PTR)CountQueueNodesBetween((struct QueueNode *)((struct Ride *)LogFlumeEntranceRide)->riders, (struct QueueNode *)buf.slots[i].busy);
            }
            SaveGameWrite(&buf, sizeof(buf));
            entry = entry->next;
        } while (entry != NULL);
    }
    SaveGameWrite(&end, 4);
    return 1;
}

// FUNCTION: LEGOLAND 0x00410a50
struct FlumeNode *LoadFlumeNodes(struct FlumeEntry *owner, struct FlumeEntry *entry) {
    struct FlumeNode *head = NULL;
    struct FlumeNode *cur = NULL;
    struct FlumeNode *prev = NULL;
    int marker = 1;
    int len;
    struct Element *elem;
    char name[0x200];

    SaveGameRead(&marker, 4);
    while (marker != 0) {
        if (cur == NULL) {
            cur = malloc(sizeof(struct FlumeNode));
            head = cur;
        } else {
            cur->next = malloc(sizeof(struct FlumeNode));
            cur = cur->next;
        }
        SaveGameRead(cur, sizeof(struct FlumeNode));
        cur->prev = prev;
        cur->owner = owner;
        cur->entry = (struct FlumeNode *)entry;
        SaveGameRead(&len, 4);
        SaveGameRead(name, len);
        name[len] = 0;
        if (name[0] != 0) {
            LLIDB_FindElement(name, &elem, 0);
            cur->ride = ((struct Element *)elem)->ride;
        }
        cur->field_2c = LoadFlumeNodes(owner, (struct FlumeEntry *)cur);
        SaveGameRead(&marker, 4);
        prev = cur;
    }
    return head;
}

// FUNCTION: LEGOLAND 0x00410b60
void *IndexToFlumeNode(void *base, unsigned int packed) {
    unsigned int hi;
    unsigned int lo;
    unsigned int i;
    struct FlumeNode *n;

    lo = packed & 0xffff;
    hi = packed >> 16;

    if (lo == 0) {
        return NULL;
    }
    if (lo == 0xffff) {
        return NULL;
    }
    lo--;
    if (lo != 0) {
        i = lo;
        n = (struct FlumeNode *)base;
        do {
            n = n->next;
        } while (--i != 0);
    } else {
        n = (struct FlumeNode *)base;
    }
    if (hi == 0 || hi == 0xffff) {
        return n;
    }
    n = n->field_2c;
    while (--hi != 0) {
        n = n->next;
    }
    return n;
}

// FUNCTION: LEGOLAND 0x00410bb0
void ResolveFlumeNodeLinks(void *arg0, struct FlumeNode *arg1) {
    struct FlumeNode *current;

    if (arg1 == NULL) {
        return;
    }

    current = arg1;
    do {
        current->field_8 = IndexToFlumeNode(arg0, (unsigned int)(UINT_PTR)current->field_8);
        current->field_c = IndexToFlumeNode(arg0, (unsigned int)(UINT_PTR)current->field_c);
        current->field_30 = IndexToFlumeNode(arg0, (unsigned int)(UINT_PTR)current->field_30);
        current->field_34 = IndexToFlumeNode(arg0, (unsigned int)(UINT_PTR)current->field_34);
        ResolveFlumeNodeLinks(arg0, current->field_2c);
        current = current->next;
    } while (current != NULL);
}

// FUNCTION: LEGOLAND 0x00410c10
int LogFlumeEntrance_Load(struct Element *element) {
    struct FlumeEntry *cur = NULL;
    struct FlumeNode *head;
    struct Queue queue;
    struct FlumeSlot *slot;
    int marker = 1;
    int i;

    SaveGameRead(&marker, 4);
    while (marker != 0) {
        if (cur == NULL) {
            cur = malloc(sizeof(struct FlumeEntry));
            FlumeEntryList = cur;
            cur->next = NULL;
        } else {
            cur->next = malloc(sizeof(struct FlumeEntry));
            cur = cur->next;
        }
        head = LoadFlumeNodes(cur, NULL);
        ResolveFlumeNodeLinks(head, head);
        LoadQueueNodes((struct QueueNode *)((struct Ride *)LogFlumeEntranceRide)->riders, &queue);
        SaveGameRead(cur, sizeof(struct FlumeEntry));
        cur->sub = (struct FlumeEntry *)head;
        cur->queue = queue;
        cur->field_8 = IndexToFlumeNode(head, (unsigned int)(UINT_PTR)cur->field_8);
        cur->field_c = IndexToFlumeNode(cur->sub, (unsigned int)(UINT_PTR)cur->field_c);
        cur->link = IndexToFlumeNode(cur->sub, (unsigned int)(UINT_PTR)cur->link);
        cur->target = &cur->slots[(INT_PTR)cur->target];
        slot = cur->slots;
        for (i = 4; i != 0; i--) {
            slot->owner = IndexToFlumeNode(cur->sub, (unsigned int)(UINT_PTR)slot->owner);
            slot->busy = (struct RideNode *)GetNthNextQueueNode((struct QueueNode *)((struct Ride *)LogFlumeEntranceRide)->riders, (int)(INT_PTR)slot->busy);
            slot++;
        }
        SaveGameRead(&marker, 4);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00410d60
void LogFlume_GetInterfaces(struct Element *flume, struct Ride *vtbl) {
    // STRING: LEGOLAND 0x004b4bb4
    if (_stricmp("LOG FLUME ENTRANCE", flume->name) == 0) {
        vtbl->cb_a4 = LogFlumeEntranceLoad;
        vtbl->cb_8c = LogFlumeEntranceSetEditMode;
        vtbl->cb_90 = LogFlumeEntranceCalcCursor;
        vtbl->cb_94 = LogFlumeEntranceDCalcCursor;
        vtbl->cb_98 = LogFlumeEntranceAddObject;
        vtbl->cb_9c = LogFlumeEntranceRemoveObject;
        vtbl->cb_pre_render = LogFlumeEntranceUpdate;
        vtbl->cb_b0 = RenderLogFlumeEntrance;
        vtbl->cb_ac = LogFlumeEntranceUnload;
        vtbl->save_hook = LogFlumeEntrance_Save;
        vtbl->load_hook = LogFlumeEntrance_Load;
        vtbl->cb_c0 = GetBestFlumeScore;
        return;
    }
    // STRING: LEGOLAND 0x004b4ba4
    if (_stricmp("LOG FLUME TRACK", flume->name) == 0) {
        LogFlumeInterfaceRide = flume->ride;
        vtbl->cb_a4 = LogFlumeTrackLoad;
        vtbl->cb_8c = LogFlumeTrackSetEditMode;
        vtbl->cb_sprite = (struct RideSpriteInfo * (*)(struct Element *, unsigned short)) GetLogFlumeTrackSpriteInfo; /* takes the tile id as a TileId */
        vtbl->cb_b0 = (void (*)())RenderLogFlumeTrack;
        vtbl->cb_90 = LogFlumeTrackCalcCursor;
        vtbl->cb_94 = LogFlumeTrackDCalcCursor;
        vtbl->cb_98 = LogFlumeTrackAddObject;
        vtbl->cb_9c = LogFlumeTrackRemoveObject;
        vtbl->cb_ac = LogFlumeTrackUnload;
        return;
    }
    // STRING: LEGOLAND 0x004b4b88
    if (_stricmp("LOG FLUME SPECIAL CORNER 1", flume->name) == 0) {
        vtbl->cb_a4 = LogFlumeSpecialCorner1LoadSprites;
        vtbl->cb_8c = LogFlumeSpecialCorner1SetEditMode;
        vtbl->cb_sprite = (struct RideSpriteInfo * (*)(struct Element *, unsigned short)) GetFlumePieceSpriteInfo; /* takes the tile id as a TileId */
        vtbl->cb_b0 = RenderLogFlumeSpecialCorner1;
        vtbl->cb_90 = LogFlumeSpecialCorner1CalcCursor;
        vtbl->cb_94 = LogFlumeSpecialCorner1DCalcCursor;
        vtbl->cb_98 = LogFlumeSpecialCorner1AddObject;
        vtbl->cb_9c = LogFlumeSpecialCorner1RemoveObject;
        vtbl->cb_ac = LogFlumeSpecialCorner1UnloadSprites;
        return;
    }
    // STRING: LEGOLAND 0x004b4b6c
    if (_stricmp("LOG FLUME SPECIAL CORNER 2", flume->name) == 0) {
        vtbl->cb_a4 = LogFlumeSpecialCorner2LoadSprites;
        vtbl->cb_8c = LogFlumeSpecialCorner2SetEditMode;
        vtbl->cb_sprite = (struct RideSpriteInfo * (*)(struct Element *, unsigned short)) GetFlumePieceSpriteInfo; /* takes the tile id as a TileId */
        vtbl->cb_b0 = RenderLogFlumeSpecialCorner2;
        vtbl->cb_90 = LogFlumeSpecialCorner2CalcCursor;
        vtbl->cb_94 = LogFlumeSpecialCorner2DCalcCursor;
        vtbl->cb_98 = LogFlumeSpecialCorner2AddObject;
        vtbl->cb_9c = LogFlumeSpecialCorner2RemoveObject;
        vtbl->cb_ac = LogFlumeSpecialCorner2UnloadSprites;
        return;
    }
    // STRING: LEGOLAND 0x004b4b50
    if (_stricmp("LOG FLUME SPECIAL CORNER 3", flume->name) == 0) {
        vtbl->cb_a4 = LogFlumeSpecialCorner3LoadSprites;
        vtbl->cb_8c = LogFlumeSpecialCorner3SetEditMode;
        vtbl->cb_sprite = (struct RideSpriteInfo * (*)(struct Element *, unsigned short)) GetFlumePieceSpriteInfo; /* takes the tile id as a TileId */
        vtbl->cb_b0 = RenderLogFlumeSpecialCorner3;
        vtbl->cb_90 = LogFlumeSpecialCorner3CalcCursor;
        vtbl->cb_94 = LogFlumeSpecialCorner3DCalcCursor;
        vtbl->cb_98 = LogFlumeSpecialCorner3AddObject;
        vtbl->cb_9c = LogFlumeSpecialCorner3RemoveObject;
        vtbl->cb_ac = LogFlumeSpecialCorner3UnloadSprites;
        return;
    }
    // STRING: LEGOLAND 0x004b4b34
    if (_stricmp("LOG FLUME SPECIAL CORNER 4", flume->name) == 0) {
        vtbl->cb_a4 = LogFlumeSpecialCorner4LoadSprites;
        vtbl->cb_8c = LogFlumeSpecialCorner4SetEditMode;
        vtbl->cb_sprite = (struct RideSpriteInfo * (*)(struct Element *, unsigned short)) GetFlumePieceSpriteInfo; /* takes the tile id as a TileId */
        vtbl->cb_b0 = RenderLogFlumeSpecialCorner4;
        vtbl->cb_90 = LogFlumeSpecialCorner4CalcCursor;
        vtbl->cb_94 = LogFlumeSpecialCorner4DCalcCursor;
        vtbl->cb_98 = LogFlumeSpecialCorner4AddObject;
        vtbl->cb_9c = LogFlumeSpecialCorner4RemoveObject;
        vtbl->cb_ac = LogFlumeSpecialCorner4UnloadSprites;
        return;
    }
    // STRING: LEGOLAND 0x004b4b24
    if (_stricmp("LOG FLUME CSAW", flume->name) == 0) {
        vtbl->cb_a4 = (RideCallback)LoadLogFlumeCsaw2;
        vtbl->cb_8c = LogFlumeCsawSetEditMode;
        vtbl->cb_sprite = (struct RideSpriteInfo * (*)(struct Element *, unsigned short)) GetFlumePieceSpriteInfo; /* takes the tile id as a TileId */
        vtbl->cb_b0 = RenderLogFlumeCsaw;
        vtbl->cb_90 = LogFlumeCsawCalcCursor;
        vtbl->cb_94 = LogFlumeCsawDCalcCursor;
        vtbl->cb_98 = LogFlumeCsawAddObject;
        vtbl->cb_9c = LogFlumeCsawRemoveObject;
        vtbl->cb_ac = LogFlumeCsawUnloadSprites;
        return;
    }
    // STRING: LEGOLAND 0x004b4b10
    if (_stricmp("LOG FLUME TUNNEL", flume->name) == 0) {
        vtbl->cb_a4 = LogFlumeTunnelLoadSprites;
        vtbl->cb_8c = LogFlumeTunnelSetEditMode;
        vtbl->cb_sprite = (struct RideSpriteInfo * (*)(struct Element *, unsigned short)) GetFlumePieceSpriteInfo; /* takes the tile id as a TileId */
        vtbl->cb_b0 = RenderLogFlumeTunnel;
        vtbl->cb_90 = LogFlumeTunnelCalcCursor;
        vtbl->cb_94 = LogFlumeTunnelDCalcCursor;
        vtbl->cb_98 = LogFlumeTunnelAddObject;
        vtbl->cb_9c = LogFlumeTunnelRemoveObject;
        vtbl->cb_ac = LogFlumeTunnelUnloadSprites;
        return;
    }
    // STRING: LEGOLAND 0x004b4b00
    if (_stricmp("LOG FLUME DROP", flume->name) == 0) {
        vtbl->cb_a4 = (RideCallback)LoadLogFlumeDrop;
        vtbl->cb_8c = (void (*)())LogFlumeDropSetEditMode;
        vtbl->cb_sprite = (struct RideSpriteInfo * (*)(struct Element *, unsigned short)) GetFlumePieceSpriteInfo; /* takes the tile id as a TileId */
        vtbl->cb_b0 = RenderLogFlumeDrop;
        vtbl->cb_90 = (void (*)())LogFlumeDropCalcCursor;
        vtbl->cb_94 = LogFlumeDropDCalcCursor;
        vtbl->cb_98 = LogFlumeDropAddObject;
        vtbl->cb_9c = LogFlumeDropRemoveObject;
        vtbl->cb_ac = LogFlumeDropUnloadSprites;
        return;
    }
    // STRING: LEGOLAND 0x004b4aec
    if (_stricmp("LOG FLUME HOLD UP", flume->name) == 0) {
        vtbl->cb_a4 = LogFlumeHoldUpLoadSprites;
        vtbl->cb_8c = LogFlumeHoldUpSetEditMode;
        vtbl->cb_sprite = (struct RideSpriteInfo * (*)(struct Element *, unsigned short)) GetFlumePieceSpriteInfo; /* takes the tile id as a TileId */
        vtbl->cb_b0 = RenderLogFlumeHoldUp;
        vtbl->cb_90 = (void (*)())LogFlumeHoldUpCalcCursor;
        vtbl->cb_94 = LogFlumeHoldUpDCalcCursor;
        vtbl->cb_98 = LogFlumeHoldUpAddObject;
        vtbl->cb_9c = LogFlumeHoldUpRemoveObject;
        vtbl->cb_ac = LogFlumeHoldUpUnloadSprites;
    }
}

/* Screen offset (3w, h) of the log flume footprint, w/h = GetTileDimensions: the middle of the top-right edge. With
 * the other three GetFlumeEdgeMid* points it gives the ends of the straight and corner paths built by
 * InitLogFlumeShapes. */
// FUNCTION: LEGOLAND 0x00411220
struct FlumeDims GetFlumeEdgeMidTopRight(void) {
    int dim1;
    int dim2;
    struct FlumeDims result;

    GetTileDimensions(&dim1, &dim2);

    dim1 = dim1 << 1;
    dim2 = dim2 << 1;

    result.field1 = (dim1 >> 1) + dim1;
    result.field2 = dim2 >> 1;

    return result;
}

/* Screen offset (3w, 3h): middle of the footprint's bottom-right edge (see GetFlumeEdgeMidTopRight). */
// FUNCTION: LEGOLAND 0x00411250
struct FlumeDims GetFlumeEdgeMidBottomRight(void) {
    int dim1;
    int dim2;
    int val1;
    int val2;
    struct FlumeDims result;

    GetTileDimensions(&dim1, &dim2);

    dim1 = dim1 << 1;
    dim2 = dim2 << 1;

    val1 = dim1 >> 1;
    val2 = dim2 >> 1;

    val1 = val1 + dim1;
    val2 = val2 + dim2;

    result.field1 = val1;
    result.field2 = val2;

    return result;
}

/* Screen offset (w, h): middle of the footprint's top-left edge (see GetFlumeEdgeMidTopRight). */
// FUNCTION: LEGOLAND 0x00411290
struct FlumeDims GetFlumeEdgeMidTopLeft(void) {
    int dim1;
    int dim2;
    struct FlumeDims result;

    GetTileDimensions(&dim1, &dim2);

    dim1 = dim1 << 1;
    dim2 = dim2 << 1;

    result.field1 = dim1 >> 1;
    result.field2 = dim2 >> 1;

    return result;
}

/* Screen offset (w, 3h): middle of the footprint's bottom-left edge (see GetFlumeEdgeMidTopRight). Also the start
 * position of an entrance's log particles. */
// FUNCTION: LEGOLAND 0x004112c0
struct FlumeDims GetFlumeEdgeMidBottomLeft(void) {
    int dim1;
    int dim2;
    struct FlumeDims result;

    GetTileDimensions(&dim1, &dim2);

    dim1 = dim1 << 1;
    dim2 = dim2 << 1;

    result.field1 = dim1 >> 1;
    result.field2 = (dim2 >> 1) + dim2;

    return result;
}

// FUNCTION: LEGOLAND 0x004112f0
struct FlumeDims InterpolateFlumeShape(struct FlumeShape *shape, float t, int reverse) {
    float step = 1.0f / (shape->count - 1);
    int idx = (int)floor(t / step);
    int next = idx + 1;
    struct FlumeDims p0;
    struct FlumeDims p1;
    struct FlumeDims result;

    if (reverse == 0) {
        p0 = shape->pts[idx];
        p1 = shape->pts[next];
    } else {
        p0 = shape->pts[shape->count - idx - 1];
        p1 = shape->pts[shape->count - next - 1];
    }
    result.field1 = (int)((p1.field1 - p0.field1) * ((t - idx * step) / step) + p0.field1);
    result.field2 = (int)((p1.field2 - p0.field2) * ((t - idx * step) / step) + p0.field2);
    return result;
}

// FUNCTION: LEGOLAND 0x004113d0
void InitLogFlumeShapes(void) {
    int w;
    int h;
    int dx;
    int dy;

    GetTileDimensions(&w, &h);
    w = w << 1;
    h = h << 1;
    dy = h >> 2;
    dx = w >> 2;

    FlumeStraightShape0.count = 2;
    FlumeStraightShape0.pts = FlumeStraightShape0Points;
    FlumeStraightShape0.pts[0] = GetFlumeEdgeMidTopRight();
    FlumeStraightShape0.pts[1] = GetFlumeEdgeMidBottomLeft();

    FlumeStraightShape1.count = 2;
    FlumeStraightShape1.pts = FlumeStraightShape1Points;
    FlumeStraightShape1.pts[0] = GetFlumeEdgeMidBottomRight();
    FlumeStraightShape1.pts[1] = GetFlumeEdgeMidTopLeft();

    FlumeCornerShape0.count = 4;
    FlumeCornerShape0.pts = FlumeCornerShape0Points;
    FlumeCornerShape0.pts[0] = GetFlumeEdgeMidTopRight();
    FlumeCornerShape0.pts[3] = GetFlumeEdgeMidBottomRight();
    FlumeCornerShape0.pts[1].field1 = FlumeCornerShape0.pts[0].field1 - dx;
    FlumeCornerShape0.pts[1].field2 = FlumeCornerShape0.pts[0].field2 + dy;
    FlumeCornerShape0.pts[2].field1 = FlumeCornerShape0.pts[3].field1 - dx;
    FlumeCornerShape0.pts[2].field2 = FlumeCornerShape0.pts[3].field2 - dy;

    FlumeCornerShape1.count = 4;
    FlumeCornerShape1.pts = FlumeCornerShape1Points;
    FlumeCornerShape1.pts[0] = GetFlumeEdgeMidBottomRight();
    FlumeCornerShape1.pts[3] = GetFlumeEdgeMidBottomLeft();
    FlumeCornerShape1.pts[1].field1 = FlumeCornerShape1.pts[0].field1 - dx;
    FlumeCornerShape1.pts[1].field2 = FlumeCornerShape1.pts[0].field2 - dy;
    FlumeCornerShape1.pts[2].field1 = FlumeCornerShape1.pts[3].field1 + dx;
    FlumeCornerShape1.pts[2].field2 = FlumeCornerShape1.pts[3].field2 - dy;

    FlumeCornerShape2.count = 4;
    FlumeCornerShape2.pts = FlumeCornerShape2Points;
    FlumeCornerShape2.pts[0] = GetFlumeEdgeMidBottomLeft();
    FlumeCornerShape2.pts[3] = GetFlumeEdgeMidTopLeft();
    FlumeCornerShape2.pts[1].field1 = FlumeCornerShape2.pts[0].field1 + dx;
    FlumeCornerShape2.pts[1].field2 = FlumeCornerShape2.pts[0].field2 - dy;
    FlumeCornerShape2.pts[2].field1 = FlumeCornerShape2.pts[3].field1 + dx;
    FlumeCornerShape2.pts[2].field2 = FlumeCornerShape2.pts[3].field2 + dy;

    FlumeCornerShape3.count = 4;
    FlumeCornerShape3.pts = FlumeCornerShape3Points;
    FlumeCornerShape3.pts[0] = GetFlumeEdgeMidTopLeft();
    FlumeCornerShape3.pts[3] = GetFlumeEdgeMidTopRight();
    FlumeCornerShape3.pts[1].field1 = FlumeCornerShape3.pts[0].field1 + dx;
    FlumeCornerShape3.pts[1].field2 = FlumeCornerShape3.pts[0].field2 + dy;
    FlumeCornerShape3.pts[2].field1 = FlumeCornerShape3.pts[3].field1 - dx;
    FlumeCornerShape3.pts[2].field2 = FlumeCornerShape3.pts[3].field2 + dy;
}

// FUNCTION: LEGOLAND 0x00411650
int IsFlumeSlotOnDrop(struct FlumeSlot *slot) {
    struct FlumeEntry *p = ((struct FlumeEntry *)slot->owner)->link28;

    if (p != NULL && p != (struct FlumeEntry *)-1 && p->ride == LogFlumeDropRide) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00411680
int AdvanceFlumeMover(struct FlumeSlot *mover) {
    struct FlumeNode *node = (struct FlumeNode *)mover->owner;
    struct FlumeNode *next;
    struct FlumeShape *shape = NULL;
    struct FlumeDims r;
    TileId tile;
    float f;
    int flag;
    int rev;
    int dir;
    int dx;
    int dy;

    flag = 0;
    rev = 0;
    f = mover->weight + mover->field_20;
    if (f > 1.0) {
        f -= 1.0f;
        node = (struct FlumeNode *)node->field_8;
        flag = 1;
    }
    next = (struct FlumeNode *)node->field_8;
    if (next == NULL) {
        return 0;
    }
    /* as in the original, dir is left unset when next is on the same tile (dx == dy == 0) */
    tile = next->tile;
    dx = tile.pos.x - node->tile.pos.x;
    dy = tile.pos.y - node->tile.pos.y;
    if (dx < 0) {
        dir = 7;
    }
    if (dx > 0) {
        dir = 3;
    }
    if (dy < 0) {
        dir = 1;
    }
    if (dy > 0) {
        dir = 5;
    }
    switch (node->mode) {
    case 1:
        if (node->submode == 1) {
            shape = &FlumeStraightShape1;
            if (dir == 3) {
                rev = 1;
            }
        }
        if (node->submode == 0) {
            shape = &FlumeStraightShape0;
            if (dir == 1) {
                rev = 1;
            }
        }
        break;
    case 2:
        if (node->submode == 3) {
            shape = &FlumeCornerShape3;
            if (dir == 7) {
                rev = 1;
            }
        }
        if (node->submode == 0) {
            shape = &FlumeCornerShape0;
            if (dir == 1) {
                rev = 1;
            }
        }
        if (node->submode == 1) {
            shape = &FlumeCornerShape1;
            if (dir == 3) {
                rev = 1;
            }
        }
        if (node->submode == 2) {
            shape = &FlumeCornerShape2;
            if (dir == 5) {
                rev = 1;
            }
        }
        break;
    }
    /* as in the original, r is left unset when no shape applies */
    if (shape != NULL) {
        r = InterpolateFlumeShape(shape, f, rev);
    }
    mover->x = r.field1;
    mover->y = r.field2;
    mover->weight = f;
    return flag;
}

// FUNCTION: LEGOLAND 0x004117e0
int GetFlumeDropNodeIndex(struct FlumeSlot *obj) {
    int idx = 0;
    struct FlumeEntry *head = obj->owner;
    struct FlumeEntry *cur = head->link28->link30;

    while (cur != NULL) {
        if ((struct FlumeEntry *)cur == head) {
            return idx;
        }
        cur = cur->parent8;
        idx++;
    }
    return -1;
}

// FUNCTION: LEGOLAND 0x00411810
void UpdateFlumeDropSlot(struct FlumeSlot *m) {
    struct FlumeNode *entry;
    struct FlumeEntry *owner;
    struct LLS *lls;
    float f;
    int idx;

    if (!(m->flags & 2)) {
        return;
    }
    entry = (struct FlumeNode *)m->owner->link28;
    if (AdvanceFlumeMover(m)) {
        m->owner = m->owner->field_8;
        if (m->owner == (struct FlumeEntry *)entry->field_34->field_8) {
            m->flags &= ~2;
            return;
        }
    }
    idx = GetFlumeDropNodeIndex(m);
    f = (float)idx + m->weight;
    if (f < 2.0f) {
        m->field_1c = 0;
    } else if (f < 4.0f) {
        m->field_1c = (int)(120.0f * ((f - 2.0f) * 0.5f));
        m->field_20 = 0.05f;
    }
    if (f >= 4.0f && f <= 5.0f) {
        m->field_1c = 120;
        m->field_20 = 0.1f;
    }
    if (f >= 5.0f && f <= 10.0f) {
        m->field_1c = 120 - (int)(120.0f * ((f - 5.0f) * 0.2f));
        m->field_20 += 0.05f;
        if (f >= 9.0f) {
            m->flags &= ~4;
        } else {
            m->flags |= 4;
        }
    }
    if (f >= 10.0f) {
        m->field_1c = 0;
        m->field_20 = 0.1f;
        if (f < 10.5) {
            owner = m->owner->parent;
            if (owner != NULL && !(owner->flags & 2)) {
                owner->flags |= 2;
                owner->field_24 = 0;
                lls = GetLLSForSprite((struct Sprite *)LogFlumeSplashSprite);
                if (lls != NULL) {
                    owner->field_28 = lls->frame_count;
                }
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x004119a0
void AddFlumeScore(struct FlumeEntry *param_1, unsigned int param_2) {
    if (param_1 != NULL) {
        param_1->field_d0 += param_2;
    }
}

// FUNCTION: LEGOLAND 0x004119c0
int GetBestFlumeScore(Element *obj, int filter) {
    struct FlumeEntry *entry = FlumeEntryList;
    int best = 0;

    while (entry != NULL) {
        if (entry->field_d0 > best) {
            if (filter == 0 || IsFlumeLoopClosed((struct FlumeEntry *)entry) != 0) {
                best = entry->field_d0;
            }
        }
        entry = entry->next;
    }
    return best;
}
