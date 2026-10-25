#include <stdlib.h>
#include "legoland.h"

#include "bloke.h"
#include "globals.h"
#include "llidb.h"
#include "map_object.h"
#include "math.h"
#include "pathfind.h"
#include "ride_queue.h"
#include "tilemap.h"

struct QueueNode {
    struct QueueNode *next;
    struct RideNode *field_4;
};

struct QueueStep {
    int dx;
    int dy;
    int pad;
};

struct QueueTable {
    int count;
    struct QueueStep *steps;
};

struct RQObjClass {
    unsigned char pad_0[0xc4];
    struct Element *element;
};

static __inline struct MapElement *TileAt(int x, int y) {
    if (x < 0 || x >= (unsigned short)lpConfig->width || y < 0 || y >= (unsigned short)lpConfig->height) {
        return NULL;
    }
    return &GameMap[y][x];
}

// FUNCTION: LEGOLAND 0x00411e30
void QueueAppendNode(struct Queue *queue, struct QueueNode *node) {
    if (queue->head == NULL && queue->tail == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
}

// FUNCTION: LEGOLAND 0x00411e60
unsigned int IsQueueFull(struct Queue *queue) {
    unsigned int count;
    struct QueueNode *node;

    if (queue->head != NULL) {
        count = 0;
        node = queue->head;
        while (node != NULL) {
            count++;
            node = node->next;
        }
        if (count == (unsigned int)queue->count->count) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00411e90
unsigned int QueueHasNodes(struct Queue *queue) {
    return queue->head != NULL;
}

// FUNCTION: LEGOLAND 0x00411ea0
int IsQueueHeadAtFront(struct Queue *queue) {
    struct QueueNode *head = queue->head;
    if (head != NULL) {
        short value = head->field_4->rider->field_38;
        if (value == (queue->count->count - 1)) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00411ed0
void FreeAllQueueNodes(struct Queue *queue) {
    struct QueueNode *node = queue->head;
    while (node != NULL) {
        QueueUnlinkHead(queue);
        free(node);
        node = queue->head;
    }
}

// FUNCTION: LEGOLAND 0x00411f00
void QueueUnlinkHead(struct Queue *queue) {
    struct QueueNode *head = queue->head;
    if (head != NULL) {
        queue->head = head->next;
        if (queue->tail == head) {
            queue->tail = NULL;
        }
    }
}

// FUNCTION: LEGOLAND 0x00411f20
void JoinQueue(struct Queue *queue, struct RideNode *mid) {
    struct QueueNode *node = (struct QueueNode *)malloc(sizeof(struct QueueNode));
    if (node != NULL) {
        memset(node, 0, sizeof(*node));
        node->field_4 = mid;
        mid->rider->flags |= 0x40;
        mid->rider->field_38 = 0;
        mid->rider->param_action++;
        QueueAppendNode(queue, node);
    }
}

// FUNCTION: LEGOLAND 0x00411f70
int IsBlokeAtQueueHead(struct Queue *queue, struct Bloke *inner) {
    struct QueueNode *head = queue->head;
    if (head != NULL && head->field_4->rider == inner) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00411fa0
void StepQueueRider(struct Queue *queue, int param_2, int param_3, struct Bloke *inner) {
    struct QueueTable *table = queue->count;
    struct QueueStep *step = &table->steps[inner->field_38];
    short cur;
    struct QueueNode *node;
    char dir;
    int x;
    volatile int y;

    x = (step->dx + param_2) << 8;
    y = (step->dy + param_3) << 8;
    inner->dest.y = y;
    inner->dest.x = x;
    dir = CalcMoveLine(*(struct Point *)&inner->pos.x, *(struct Point *)&inner->dest.x, &inner->nav);
    inner->low_level_action = 7;
    inner->target_dir = dir + 0x10;
    NewDirForAction((Bloke *)inner, (unsigned char)(inner->target_dir >> 5) + 3);
    inner->field_38++;
    cur = inner->field_38;
    if (cur >= table->count) {
        inner->field_38 = (short)(table->count - 1);
        return;
    }
    for (node = queue->head; node != NULL; node = node->next) {
        struct Bloke *other = node->field_4->rider;
        if (other->field_38 == cur && other != inner) {
            inner->field_38 = cur - 1;
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x00412060
void LeaveQueueHead(struct Queue *queue, struct RideNode **out) {
    struct QueueNode *node = queue->head;
    if (node != NULL) {
        struct Bloke *inner = node->field_4->rider;
        inner->flags &= ~0x40;
        inner->param_action++;
        *out = node->field_4;
        QueueUnlinkHead(queue);
        free(node);
    }
}

// FUNCTION: LEGOLAND 0x004120a0
void StepQueueRiders(struct Queue *queue, unsigned int param_2, unsigned int param_3) {
    struct QueueNode *node = queue->head;
    while (node != NULL) {
        struct Bloke *inner = node->field_4->rider;
        if (inner->low_level_action == 0) {
            StepQueueRider(queue, param_2, param_3, inner);
        }
        node = node->next;
    }
}

/* Build the queue's sampled positions from its segment offsets. Each segment contributes
 * its Euclidean length rounded down, and its endpoint becomes the next segment's origin. */
// FUNCTION: LEGOLAND 0x00412100
struct QueueTable *BuildQueueTable(struct PathTable *param_1) {
    int px;
    int py;
    int i;
    struct QueueTable *table;
    int k;
    int j;
    POINT start;
    int dx;
    int dy;
    int len;
    int cx;
    int cy;
    int total;
    Point end;

    total = 0;
    for (i = 0; i < param_1->count; i++) {
        int x = param_1->pairs[i].a;
        int y = param_1->pairs[i].b;
        total += (int)sqrt((double)(y * y + x * x));
    }
    table = (struct QueueTable *)malloc(total * 12 + 8);
    if (table != NULL) {
        memset(table, 0, total * 12 + 8);
        table->count = total;
        table->steps = (struct QueueStep *)(table + 1);
    }
    start.x = 0;
    start.y = 0;
    k = 0;
    for (i = 0; i < param_1->count; i++) {
        struct PathPair *p = &param_1->pairs[i];
        dx = p->a;
        dy = p->b;
        len = (int)sqrt((double)(dy * dy + dx * dx));
        cx = start.x;
        cy = start.y;
        if (len > 0) {
            px = py = 0;
            for (j = len; j > 0; j--) {
                table->steps[k].dx = cx;
                table->steps[k].dy = cy;
                k++;
                cx = start.x + px / len;
                cy = py / len + start.y;
                px += dx;
                py += dy;
            }
        }
        /* Form both endpoint coordinates before updating the origin. MSVC6 then keeps
         * the sums in the original segment registers through the loop's final stores. */
        end.x = dx + start.x;
        end.y = dy + start.y;
        start.x = end.x;
        start.y = end.y;
    }
    return table;
}

// FUNCTION: LEGOLAND 0x00412290
void FreeIfNotNull(void *param_1) {
    if (param_1 != NULL) {
        free(param_1);
    }
}

// FUNCTION: LEGOLAND 0x004122a0
void StartQueueTableBackward(struct QueueTable *param_1, struct Bloke *slot) {
    slot->queue = param_1;
    slot->field_38 = param_1->count - 1;
    slot->field_34 = 0xff;
    slot->param_action++;
}

// FUNCTION: LEGOLAND 0x004122d0
void StartQueueTableForward(struct QueueTable *param_1, struct Bloke *slot) {
    slot->queue = param_1;
    slot->field_38 = 0;
    slot->field_34 = 1;
    slot->param_action++;
}

// FUNCTION: LEGOLAND 0x004122f0
struct QueueTable *GetBlokeQueueTable(struct Bloke *slot) {
    return slot->queue;
}

// FUNCTION: LEGOLAND 0x00412300
void MoveBlokeAlongQueueTable(struct QueueTable *table, int x, int y, struct Bloke *bloke) {
    struct Point p;
    char dir;

    p.x = x + table->steps[bloke->field_38].dx;
    p.y = table->steps[bloke->field_38].dy + y;
    bloke->dest.x = p.x << 8;
    bloke->dest.y = p.y << 8;
    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
    bloke->low_level_action = 7;
    bloke->target_dir = dir + 0x10;
    NewDirForAction(bloke, (unsigned char)(bloke->target_dir >> 5) + 3);
    bloke->field_38 += (signed char)bloke->field_34;
    if ((signed char)bloke->field_34 < 0) {
        if (bloke->field_38 < 0) {
            bloke->param_action++;
        }
    } else if (bloke->field_38 >= table->count) {
        bloke->param_action++;
    }
}

// FUNCTION: LEGOLAND 0x004123a0
unsigned int CountQueueNodesBetween(struct QueueNode *start, struct QueueNode *stop) {
    unsigned int count = 0;
    struct QueueNode *current = start;
    if (current != NULL) {
        while (1) {
            if (current == stop) {
                break;
            }
            current = current->next;
            count++;
            if (current == NULL) {
                break;
            }
        }
    }
    return count;
}

// FUNCTION: LEGOLAND 0x004123c0
void SaveQueue(struct QueueNode *start, struct Queue *queue) {
    struct QueueNode *node;
    int n;

    SaveGameWrite(&queue->count->count, sizeof(queue->count->count));
    for (n = 0; n < queue->count->count; n++) {
        SaveGameWrite(&queue->count->steps[n], sizeof(struct QueueStep));
    }
    n = 0;
    for (node = queue->head; node != NULL; node = node->next) {
        n++;
    }
    SaveGameWrite(&n, 4);
    for (node = queue->head; node != NULL; node = node->next) {
        n = CountQueueNodesBetween(start, (struct QueueNode *)node->field_4);
        SaveGameWrite(&n, 4);
    }
}

// FUNCTION: LEGOLAND 0x00412470
struct QueueNode *GetNthNextQueueNode(struct QueueNode *node, int n) {
    struct QueueNode *p = node;
    while (n-- != 0) {
        p = p->next;
    }
    return p;
}

// FUNCTION: LEGOLAND 0x00412490
void LoadQueueNodes(struct QueueNode *start, struct Queue *queue) {
    int n;
    int idx;

    SaveGameRead(&n, 4);
    queue->count = (struct QueueTable *)malloc(n * sizeof(struct QueueStep) + sizeof(struct QueueTable));
    queue->count->count = n;
    queue->count->steps = (struct QueueStep *)(queue->count + 1);
    for (n = 0; n < queue->count->count; n++) {
        SaveGameRead(&queue->count->steps[n], sizeof(struct QueueStep));
    }
    queue->head = NULL;
    queue->tail = NULL;
    SaveGameRead(&n, 4);
    while (n-- != 0) {
        if (queue->tail == NULL) {
            queue->tail = (struct QueueNode *)malloc(sizeof(struct QueueNode));
            queue->head = queue->tail;
        } else {
            queue->tail->next = (struct QueueNode *)malloc(sizeof(struct QueueNode));
            queue->tail = queue->tail->next;
        }
        queue->tail->next = NULL;
        SaveGameRead(&idx, 4);
        queue->tail->field_4 = (struct RideNode *)GetNthNextQueueNode(start, idx);
    }
}

// FUNCTION: LEGOLAND 0x004125a0
struct RideQueueEntry *FindRoadEntryAt(int x, int y) {
    struct RideQueueEntry *entry = RoadEntryList;
    if (x < 0) {
        return NULL;
    }
    if (x >= (unsigned short)lpConfig->width) {
        return NULL;
    }
    if (y < 0) {
        return NULL;
    }
    if (y >= (unsigned short)lpConfig->height) {
        return NULL;
    }
    if (entry == NULL) {
        return NULL;
    }
    while (entry != NULL) {
        if (entry->x == x && entry->y == y) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x004125f0
void *FindQueueEntryAtTile(unsigned int a, unsigned int b) {
    struct RideQueueEntry *entry = RoadEntryList;
    int x = a;
    int y = b;
    if (x < 0) {
        return NULL;
    }
    if (x >= (unsigned short)lpConfig->width) {
        return NULL;
    }
    if (y < 0) {
        return NULL;
    }
    if (y >= (unsigned short)lpConfig->height) {
        return NULL;
    }
    if (entry == NULL) {
        return NULL;
    }
    while (entry != NULL) {
        if ((unsigned int)(x - entry->x) < 4 && (unsigned int)(y - entry->y) < 4) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00412650
struct RideQueueEntry *FindQueueEntryById(unsigned short param_1) {
    struct RideQueueEntry *entry = RoadEntryList;
    if (entry == NULL) {
        return NULL;
    }
    while (entry != NULL) {
        if (entry->id == param_1 && (entry->field_14 & 0xf) == 6) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00412680
void PlaceRoadTiles(int x, int y, int param_3, int param_4) {
    unsigned short tiles[16] = {0};
    int dm2 = (param_4 - 2) & 3;
    int dm1 = (param_4 - 1) & 3;
    int dp1 = (param_4 + 1) & 3;
    struct RideQueueEntry *entry;
    struct MapElement *tile;
    struct Point pos;
    TileId id;
    int i;
    struct FXSpriteList *set1;
    struct FXSpriteList *set2;
    int col;
    int row;

    entry = FindRoadEntryAt(x, y);
    if (entry == NULL) {
        return;
    }
    BGFullUpdate = 1;
    entry->field_14 = ((param_4 & 3) << 5) | param_3;
    id.pos.x = x;
    id.pos.y = y;
    pos.x = x;
    for (i = 0; i < 4; i++) {
        pos.y = i + y;
        tile = TileAt(pos.x, pos.y);
        tile[0].field_10 = 2;
        tile[0].flags |= 8;
        tile[0].field_0 = ((struct RQObjClass *)DrivingSchoolRoadsClass)->element;
        tile[0].anchor = id;
        tile[1].field_10 = 2;
        tile[1].flags |= 8;
        tile[1].field_0 = ((struct RQObjClass *)DrivingSchoolRoadsClass)->element;
        tile[1].anchor = id;
        tile[2].field_10 = 2;
        tile[2].flags |= 8;
        tile[2].field_0 = ((struct RQObjClass *)DrivingSchoolRoadsClass)->element;
        tile[2].anchor = id;
        tile[3].field_10 = 2;
        tile[3].flags |= 8;
        tile[3].field_0 = ((struct RQObjClass *)DrivingSchoolRoadsClass)->element;
        tile[3].anchor = id;
        RemovePathSquare(&pos);
        pos.x++;
        RemovePathSquare(&pos);
        pos.x++;
        RemovePathSquare(&pos);
        pos.x++;
        RemovePathSquare(&pos);
        pos.x = x;
    }

    switch (param_3 & 0xf) {
    case 1:
        tiles[0] = RoadPieceTileTable[2][param_4];
        tiles[1] = RoadPieceTileTable[4][param_4];
        tiles[2] = RoadPieceTileTable[5][param_4];
        tiles[3] = RoadPieceTileTable[3][param_4];
        tiles[4] = RoadPieceTileTable[0][param_4];
        tiles[5] = RoadPieceTileTable[1][param_4];
        tiles[6] = RoadPieceTileTable[1][dm2];
        tiles[7] = RoadPieceTileTable[0][dm2];
        tiles[8] = RoadPieceTileTable[0][param_4];
        tiles[9] = RoadPieceTileTable[1][param_4];
        tiles[10] = RoadPieceTileTable[1][dm2];
        tiles[11] = RoadPieceTileTable[0][dm2];
        tiles[12] = RoadPieceTileTable[0][param_4];
        tiles[13] = RoadPieceTileTable[1][param_4];
        tiles[14] = RoadPieceTileTable[1][dm2];
        tiles[15] = RoadPieceTileTable[0][dm2];
        break;
    case 7:
        tiles[0] = RoadPieceTileTable[2][param_4];
        tiles[1] = RoadPieceTileTable[4][param_4];
        tiles[2] = RoadPieceTileTable[5][param_4];
        tiles[3] = RoadPieceTileTable[3][param_4];
        tiles[4] = RoadPieceTileTable[0][param_4];
        tiles[5] = RoadPieceTileTable[1][param_4];
        tiles[6] = RoadPieceTileTable[1][dm2];
        tiles[7] = RoadPieceTileTable[0][dm2];
        tiles[8] = RoadPieceTileTable[0][param_4];
        tiles[9] = RoadPieceTileTable[1][param_4];
        tiles[10] = RoadPieceTileTable[1][dm2];
        tiles[11] = RoadPieceTileTable[0][dm2];
        tiles[12] = RoadPieceTileTable[3][dm2];
        tiles[13] = RoadPieceTileTable[5][dm2];
        tiles[14] = RoadPieceTileTable[4][dm2];
        tiles[15] = RoadPieceTileTable[2][dm2];
        break;
    case 3:
        tiles[0] = RoadPieceTileTable[14][param_4];
        tiles[1] = RoadPieceTileTable[0][dp1];
        tiles[2] = RoadPieceTileTable[0][dp1];
        tiles[3] = RoadPieceTileTable[0][dp1];
        tiles[4] = RoadPieceTileTable[0][param_4];
        tiles[5] = RoadPieceTileTable[11][param_4];
        tiles[6] = RoadPieceTileTable[12][param_4];
        tiles[7] = RoadPieceTileTable[8][dp1];
        tiles[8] = RoadPieceTileTable[0][param_4];
        tiles[9] = RoadPieceTileTable[13][param_4];
        tiles[10] = RoadPieceTileTable[10][param_4];
        tiles[11] = RoadPieceTileTable[8][dm1];
        tiles[12] = RoadPieceTileTable[0][param_4];
        tiles[13] = RoadPieceTileTable[8][param_4];
        tiles[14] = RoadPieceTileTable[8][dm2];
        tiles[15] = RoadPieceTileTable[9][param_4];
        break;
    case 4:
        tiles[0] = RoadPieceTileTable[9][dm2];
        tiles[1] = RoadPieceTileTable[11][param_4];
        tiles[2] = RoadPieceTileTable[11][dm2];
        tiles[3] = RoadPieceTileTable[9][dm1];
        tiles[4] = RoadPieceTileTable[1][dp1];
        tiles[5] = RoadPieceTileTable[1][dp1];
        tiles[6] = RoadPieceTileTable[1][dp1];
        tiles[7] = RoadPieceTileTable[1][dp1];
        tiles[8] = RoadPieceTileTable[1][dm1];
        tiles[9] = RoadPieceTileTable[1][dm1];
        tiles[10] = RoadPieceTileTable[1][dm1];
        tiles[11] = RoadPieceTileTable[1][dm1];
        tiles[12] = RoadPieceTileTable[0][dm1];
        tiles[13] = RoadPieceTileTable[0][dm1];
        tiles[14] = RoadPieceTileTable[0][dm1];
        tiles[15] = RoadPieceTileTable[0][dm1];
        break;
    case 5:
        tiles[0] = RoadPieceTileTable[9][dm2];
        tiles[1] = RoadPieceTileTable[11][param_4];
        tiles[2] = RoadPieceTileTable[11][param_4];
        tiles[3] = RoadPieceTileTable[9][dm1];
        tiles[4] = RoadPieceTileTable[11][param_4];
        tiles[5] = RoadPieceTileTable[11][param_4];
        tiles[6] = RoadPieceTileTable[11][param_4];
        tiles[7] = RoadPieceTileTable[11][param_4];
        tiles[8] = RoadPieceTileTable[11][param_4];
        tiles[9] = RoadPieceTileTable[11][param_4];
        tiles[10] = RoadPieceTileTable[11][param_4];
        tiles[11] = RoadPieceTileTable[11][param_4];
        tiles[12] = RoadPieceTileTable[9][dp1];
        tiles[13] = RoadPieceTileTable[11][param_4];
        tiles[14] = RoadPieceTileTable[11][param_4];
        tiles[15] = RoadPieceTileTable[9][param_4];
        break;
    case 6:
        tiles[0] = RoadPieceTileTable[2][param_4];
        tiles[1] = RoadPieceTileTable[4][param_4];
        tiles[2] = RoadPieceTileTable[5][param_4];
        tiles[3] = RoadPieceTileTable[3][param_4];
        tiles[4] = RoadPieceTileTable[0][param_4];
        tiles[5] = RoadPieceTileTable[1][param_4];
        tiles[6] = RoadPieceTileTable[1][dm2];
        tiles[7] = RoadPieceTileTable[0][dm2];
        tiles[8] = RoadPieceTileTable[0][param_4];
        tiles[9] = RoadPieceTileTable[1][param_4];
        tiles[10] = RoadPieceTileTable[1][dm2];
        tiles[11] = RoadPieceTileTable[0][dm2];
        tiles[12] = RoadPieceTileTable[14][dm1];
        tiles[13] = RoadPieceTileTable[0][dm1];
        tiles[14] = RoadPieceTileTable[0][dm1];
        tiles[15] = RoadPieceTileTable[0][dm1];
        break;
    case 2:
    default:
        tiles[0] = RoadPieceTileTable[0][param_4];
        tiles[1] = RoadPieceTileTable[1][param_4];
        tiles[2] = RoadPieceTileTable[1][dm2];
        tiles[3] = RoadPieceTileTable[0][dm2];
        tiles[4] = RoadPieceTileTable[0][param_4];
        tiles[5] = RoadPieceTileTable[1][param_4];
        tiles[6] = RoadPieceTileTable[1][dm2];
        tiles[7] = RoadPieceTileTable[0][dm2];
        tiles[8] = RoadPieceTileTable[0][param_4];
        tiles[9] = RoadPieceTileTable[1][param_4];
        tiles[10] = RoadPieceTileTable[1][dm2];
        tiles[11] = RoadPieceTileTable[0][dm2];
        tiles[12] = RoadPieceTileTable[0][param_4];
        tiles[13] = RoadPieceTileTable[1][param_4];
        tiles[14] = RoadPieceTileTable[1][dm2];
        tiles[15] = RoadPieceTileTable[0][dm2];
        break;
    }

    if (param_3 & 0x10) {
        if (param_4 < 2) {
            tiles[4] = RoadPieceTileTable[6][param_4];
            tiles[5] = RoadPieceTileTable[7][param_4];
            tiles[6] = RoadPieceTileTable[7][param_4];
            tiles[7] = RoadPieceTileTable[6][dm2];
        } else {
            tiles[8] = RoadPieceTileTable[6][param_4];
            tiles[9] = RoadPieceTileTable[7][param_4];
            tiles[10] = RoadPieceTileTable[7][param_4];
            tiles[11] = RoadPieceTileTable[6][dm2];
        }
    }

    switch (param_4) {
    case 0:
        for (col = 0; col < 4; col++) {
            for (row = 0; row < 4; row++) {
                SetMapTile(x + col, y + row, DSchoolMappingData[tiles[row * 4 + col] >> 8].base[0] + (tiles[row * 4 + col] & 0xff));
            }
        }
        break;
    case 1:
        for (col = 0; col < 4; col++) {
            for (row = 0; row < 4; row++) {
                SetMapTile(x + col, y + row, DSchoolMappingData[tiles[(3 - col) * 4 + row] >> 8].base[0] + (tiles[(3 - col) * 4 + row] & 0xff));
            }
        }
        break;
    case 2:
        for (col = 0; col < 4; col++) {
            for (row = 0; row < 4; row++) {
                SetMapTile(x + col, y + row, DSchoolMappingData[tiles[(3 - row) * 4 + (3 - col)] >> 8].base[0] + (tiles[(3 - row) * 4 + (3 - col)] & 0xff));
            }
        }
        break;
    case 3:
        for (col = 0; col < 4; col++) {
            for (row = 0; row < 4; row++) {
                SetMapTile(x + col, y + row, DSchoolMappingData[tiles[col * 4 + (3 - row)] >> 8].base[0] + (tiles[col * 4 + (3 - row)] & 0xff));
            }
        }
        break;
    }

    pos.x = x;
    pos.y = y + 1;
    tile = TileAt(pos.x, pos.y);
    set1 = TileSpriteInfo[tile[1].field_8].src;
    if (tile[1].field_8 - *(int *)set1 == 0x1a) {
        AdjustTileRFFlags((int *)&pos);
        AddPathSquare(&pos);
        tile[0].field_10 = 3;
        pos.x++;
        AdjustTileRFFlags((int *)&pos);
        AddPathSquare(&pos);
        tile[1].field_10 = 3;
        pos.x++;
        AdjustTileRFFlags((int *)&pos);
        AddPathSquare(&pos);
        tile[2].field_10 = 3;
        pos.x++;
        AdjustTileRFFlags((int *)&pos);
        AddPathSquare(&pos);
        tile[3].field_10 = 3;
    } else if (set2 = TileSpriteInfo[tile[2].field_8].src, tile[2].field_8 - *(int *)set2 == 0x1b) {
        pos.x = x + 2;
        pos.y--;
        AdjustTileRFFlags((int *)&pos);
        AddPathSquare(&pos);
        TileAt(pos.x, pos.y)->field_10 = 3;
        pos.y++;
        AdjustTileRFFlags((int *)&pos);
        AddPathSquare(&pos);
        TileAt(pos.x, pos.y)->field_10 = 3;
        pos.y++;
        AdjustTileRFFlags((int *)&pos);
        AddPathSquare(&pos);
        TileAt(pos.x, pos.y)->field_10 = 3;
        pos.y++;
        AdjustTileRFFlags((int *)&pos);
        AddPathSquare(&pos);
        TileAt(pos.x, pos.y)->field_10 = 3;
    }

    pos.x = x - 1;
    pos.y = y + 1;
    tile = TileAt(pos.x, pos.y);
    if (tile != NULL && (tile->field_10 & 1)) {
        AdjustTileRFFlags((int *)&pos);
    }
    pos.x = x + 4;
    pos.y = y + 1;
    tile = TileAt(pos.x, pos.y);
    if (tile != NULL && (tile->field_10 & 1)) {
        AdjustTileRFFlags((int *)&pos);
    }
    pos.x = x + 2;
    pos.y = y - 1;
    tile = TileAt(pos.x, pos.y);
    if (tile != NULL && (tile->field_10 & 1)) {
        AdjustTileRFFlags((int *)&pos);
    }
    pos.x = x + 2;
    pos.y = y + 4;
    tile = TileAt(pos.x, pos.y);
    if (tile != NULL && (tile->field_10 & 1)) {
        AdjustTileRFFlags((int *)&pos);
    }
}
