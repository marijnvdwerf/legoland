#include <stdlib.h>
#include "legoland.h"

#include "globals.h"
#include "llidb.h"
#include "obj_instance.h"
#include "objclass.h"
#include "pathfind.h"

struct PathQuery {
    int x;
    int y;
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

// FUNCTION: LEGOLAND 0x00481c50
LEGO_EXPORT void AddPathSquare(struct Point *pos) {
    struct BestNode *node;
    unsigned int x;
    unsigned int y;

    if (FindBestNodeAtPoint(pos)) {
        return;
    }

    node = AddBestNode();
    x = pos->x;
    node->x_max = x;
    node->x_min = x;
    y = pos->y;
    node->y_max = y;
    node->y_min = y;
    MergeBestNode(node);
}

// FUNCTION: LEGOLAND 0x00481c90
LEGO_EXPORT void RemovePathSquare(struct Point *pos) {
    struct BestNode *found;
    struct BestNode *node;
    struct BestBox box;

    found = FindBestNodeAtPoint(pos);
    if (found == NULL) {
        return;
    }
    box = found->box;
    RemoveBestNode(found);

    if (box.y_min < pos->y) {
        node = AddBestNode();
        node->y_min = box.y_min;
        node->y_max = pos->y - 1;
        node->x_min = box.x_min;
        node->x_max = box.x_max;
        MergeBestNode(node);
    }
    if (box.y_max > pos->y) {
        node = AddBestNode();
        node->y_min = pos->y + 1;
        node->y_max = box.y_max;
        node->x_min = box.x_min;
        node->x_max = box.x_max;
        MergeBestNode(node);
    }
    if (box.x_min < pos->x) {
        node = AddBestNode();
        node->y_min = node->y_max = pos->y;
        node->x_min = box.x_min;
        node->x_max = pos->x - 1;
        MergeBestNode(node);
    }
    if (box.x_max > pos->x) {
        node = AddBestNode();
        node->y_min = node->y_max = pos->y;
        node->x_min = pos->x + 1;
        node->x_max = box.x_max;
        MergeBestNode(node);
    }
}

/* Stores the squared distance from the fixed-point query position to the nearest tile center within the path node
 * bounds. */
// FUNCTION: LEGOLAND 0x00481e60
void SetPathNodeDistanceSquared(struct PathQuery *query, struct BestNode *box) {
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
    box->field_1c = (dx * dx) + (dy * dy);
}

// FUNCTION: LEGOLAND 0x00481ee0
void ClearBestNodeVisited(void) {
    struct BestNode *node;

    node = (struct BestNode *)BestNodeList;
    if (node) {
        while (node) {
            node->field_20 = node->field_20 & 0xfffffffe;
            node = node->next;
        }
    }
}

/* Breadth-first search over path best-nodes, starting at the goal node and looking for the current node. Returns 1
 * and stores in *out the node through which the current node is reached (the next node to walk into), or the
 * current node itself when both are the same. Visited nodes get field_20 bit 1; returns 0 when the current node is
 * already marked or unreachable. */
// FUNCTION: LEGOLAND 0x00481f00
int FindNextBestNodeToGoal(struct BestNode *target, struct BestNode *start, struct BestNode **out) {
    struct BestNode *a[1020];
    struct BestNode *b[1020];
    struct BestNode **cur = a;
    struct BestNode **next = b;
    int count;
    int n;
    int i;
    int j;
    struct BestNode *p;

    if (target->field_20 & 1) {
        return 0;
    }
    if (target == start) {
        *out = target;
        return 1;
    }
    a[0] = start;
    count = 1;
    while (count != 0) {
        n = 0;
        for (i = 0; i < count; i++) {
            CollectAdjacentNodes((int *)&cur[i]->x_min);
            for (j = 0; j < (int)AdjacentNodeCount; j++) {
                p = AdjacentNodes[j];
                if (!(p->field_20 & 1)) {
                    next[n++] = p;
                    if (p == target) {
                        *out = cur[i];
                        return 1;
                    }
                    p->field_20 |= 1;
                }
            }
        }
        if (cur == a) {
            cur = b;
            next = a;
        } else {
            cur = a;
            next = b;
        }
        count = n;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00482050
LEGO_EXPORT int SuggestNextMove(struct Point *pos, struct Point *goal, struct Point *out) {
    struct BestNode *start;
    struct BestNode *end;
    struct BestNode *prev;
    int lo_x;
    int hi_x;
    int lo_y;
    int hi_y;

    PathGoalTile.x = goal->x >> 8;
    PathGoalTile.y = goal->y >> 8;
    start = FindBestNodeAtPixel((int *)pos);
    end = FindBestNodeAtPixel((int *)goal);
    if (start == 0) {
        return -2;
    }
    if (end == 0) {
        return -1;
    }
    if (start == end) {
        *out = *goal;
        out->x += 0x80;
        out->y += 0x80;
        return 2;
    }
    ClearBestNodeVisited();
    if (!FindNextBestNodeToGoal(start, end, &prev)) {
        return -1;
    }
    SetPathNodeDistanceSquared((struct PathQuery *)pos, (struct BestNode *)prev);
    lo_x = prev->x_min << 8;
    hi_x = prev->x_max << 8;
    lo_y = prev->y_min << 8;
    hi_y = prev->y_max << 8;
    if (pos->x < lo_x) {
        out->x = lo_x;
    } else if (pos->x > hi_x) {
        out->x = hi_x;
    } else {
        out->x = pos->x;
    }
    if (pos->y < lo_y) {
        out->y = lo_y;
    } else if (pos->y > hi_y) {
        out->y = hi_y;
    } else {
        out->y = pos->y;
    }
    if ((int)prev->field_1c > 0x18000) {
        lo_x = start->x_min << 8;
        hi_x = start->x_max << 8;
        lo_y = start->y_min << 8;
        hi_y = start->y_max << 8;
        if (out->x < lo_x) {
            out->x = lo_x;
        } else if (out->x > hi_x) {
            out->x = hi_x;
        }
        if (out->y < lo_y) {
            out->y = lo_y;
        } else if (out->y > hi_y) {
            out->y = hi_y;
        }
    }
    out->x += 0x80;
    out->y += 0x80;
    return 1;
}

// FUNCTION: LEGOLAND 0x004821c0
void ClearDirSearchGrid(void) {
    int i;

    for (i = 0; i < 1152; i = i + 1) {
        PathVisitedBits[i] = 0;
    }
}

// FUNCTION: LEGOLAND 0x004821e0
void FreeDirNodeList(void) {
    struct DirNode *node;
    struct DirNode *next;

    node = DirSearchNodeList;
    while (node != NULL) {
        next = node->next;
        free(node);
        node = next;
    }
    DirSearchNodeList = NULL;
}

// FUNCTION: LEGOLAND 0x00482210
void FreeDirPathNodes(void) {
    struct DirNode *node;
    struct DirNode *next;

    node = DirPathNodeList;
    while (node != NULL) {
        next = node->next;
        free(node);
        node = next;
    }
    DirPathNodeList = NULL;
}

// FUNCTION: LEGOLAND 0x00482240
void AddPTPSearchNode(int x, int y, struct DirNode *parent) {
    struct MapElement *tile;
    struct DirNode *node;
    int word_index;

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
    if ((1u << (x & 0x1f)) & PathVisitedBits[word_index]) {
        return;
    }

    node = malloc(sizeof(struct DirNode));
    if (node == 0) {
        return;
    }

    node->next = DirSearchNodeList;
    DirSearchNodeList = node;
    node->parent = parent;
    node->x = x;
    node->y = y;
    DirSearchWaveCount++;
    PathVisitedBits[word_index] |= 1u << (x & 0x1f);
}

// FUNCTION: LEGOLAND 0x00482300
struct DirNode *AllocDirNode(unsigned int x, unsigned int y) {
    struct DirNode *node;

    node = malloc(sizeof(struct DirNode));
    if (node != NULL) {
        node->next = DirPathNodeList;
        DirPathNodeList = node;
        node->x = x;
        node->y = y;
    }
    return node;
}

/* Waypoint lookahead on a found tile path: from node a with the next nodes b, c, d, returns 0 to head for b, 1 for
 * c or 2 for d. A turn may only be cut diagonally when the tile beside the corner lacks field_10 bit 2 (or has flag
 * 0x800). A straight run skips to c when d exists. */
// FUNCTION: LEGOLAND 0x00482330
int GetPathWaypointSkip(struct PathLink *a, struct PathLink *b, struct PathLink *c, struct PathLink *d) {
    int cx, cy;
    int dx0, dy0, dx1, dy1;
    struct MapElement *e;

    if (b == 0) {
        return 0;
    }
    if (c == 0) {
        return 0;
    }
    cx = c->x;
    dx0 = b->x - a->x;
    dy0 = b->y - a->y;
    dx1 = cx - b->x;
    cy = c->y;
    dy1 = cy - b->y;
    if ((dx1 != 0 && dy0 != 0) || (dy1 != 0 && dx0 != 0)) {
        e = &GameMap[a->y + dy1][a->x + dx1];
        if ((e->field_10 & 2) && !(e->flags & 0x800)) {
            return 0;
        }
        return 1;
    }
    if (d == 0) {
        return 0;
    }
    dx1 = d->x - cx;
    dy1 = d->y - cy;
    if ((dx1 != 0 && dy0 != 0) || (dy1 != 0 && dx0 != 0)) {
        e = &GameMap[a->y + dy1][a->x + dx1];
        if ((e->field_10 & 2) && !(e->flags & 0x800)) {
            return 0;
        }
        return 2;
    }
    return 1;
}

/* After the direction search has reached DirSearchGoalNode: follows the prev links back to the start, pushes the
 * waypoint chosen by GetPathWaypointSkip (one to three nodes ahead) onto DirPathNodeList, and returns nonzero when
 * the goal is at most three nodes away, so callers head for the goal itself. */
// FUNCTION: LEGOLAND 0x00482430
int SelectNextDirPathWaypoint(void) {
    struct PathLink *cur;
    struct PathLink *p2;
    struct PathLink *p3;
    struct PathLink *p4;
    int dir;

    cur = (struct PathLink *)DirSearchGoalNode;
    p3 = 0;
    p2 = 0;
    p4 = 0;
    if (cur == 0) {
        return 0;
    }
    while (cur->prev != 0) {
        p4 = p3;
        p3 = p2;
        p2 = cur;
        cur = cur->prev;
    }
    dir = GetPathWaypointSkip(cur, p2, p3, p4);
    switch (dir) {
    case 0:
        if (p2 != 0) {
            AllocDirNode(p2->x, p2->y);
        }
        return p4 == 0;
    case 1:
        AllocDirNode(p3->x, p3->y);
        return p4 == 0;
    case 2:
        AllocDirNode(p4->x, p4->y);
        break;
    }
    return p4 == 0;
}

// FUNCTION: LEGOLAND 0x004824d0
LEGO_EXPORT int PTPSuggestNextMove(struct Point *pos, struct Point *goal, struct Point *out) {
    int start_x;
    int start_y;
    int goal_x;
    int goal_y;
    int wave;
    struct DirNode *node;

    start_x = pos->x >> 8;
    start_y = pos->y >> 8;
    goal_x = goal->x >> 8;
    goal_y = goal->y >> 8;

    FreeDirNodeList();
    FreeDirPathNodes();
    ClearDirSearchGrid();

    DirSearchWaveCount = 0;
    DirSearchGoalNode = 0;
    AddPTPSearchNode(start_x, start_y, 0);

    while (DirSearchWaveCount != 0) {
        wave = DirSearchWaveCount;
        node = DirSearchNodeList;
        DirSearchWaveCount = 0;
        while (wave-- != 0) {
            if (node->x == goal_x && node->y == goal_y) {
                DirSearchGoalNode = node;
                if (SelectNextDirPathWaypoint()) {
                    out->x = goal->x;
                    out->y = goal->y;
                    FreeDirNodeList();
                    FreeDirPathNodes();
                    return 2;
                }
                out->x = (DirPathNodeList->x << 8) + 0x80;
                out->y = (DirPathNodeList->y << 8) + 0x80;
                FreeDirNodeList();
                FreeDirPathNodes();
                return 1;
            }
            AddPTPSearchNode(node->x, node->y - 1, node);
            AddPTPSearchNode(node->x + 1, node->y, node);
            AddPTPSearchNode(node->x, node->y + 1, node);
            AddPTPSearchNode(node->x - 1, node->y, node);
            node = node->next;
        }
    }

    FreeDirNodeList();
    FreeDirPathNodes();
    return 0;
}

// FUNCTION: LEGOLAND 0x00482620
void AddWorkerSearchNode(int x, int y, struct DirNode *parent) {
    struct MapElement *tile;
    struct DirNode *node;
    int word_index;

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
    if ((1u << (x & 0x1f)) & PathVisitedBits[word_index]) {
        return;
    }

    node = malloc(sizeof(struct DirNode));
    node->next = DirSearchNodeList;
    DirSearchNodeList = node;
    node->parent = parent;
    node->x = x;
    node->y = y;
    DirSearchWaveCount++;
    PathVisitedBits[word_index] |= 1u << (x & 0x1f);
}

// FUNCTION: LEGOLAND 0x00482710
int WorkerSuggestNextMove(int *a, int *b, int *out) {
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

    FreeDirNodeList();
    FreeDirPathNodes();
    ClearDirSearchGrid();

    DirSearchWaveCount = 0;
    DirSearchGoalNode = 0;
    AddWorkerSearchNode(start_x, start_y, 0);

    while (DirSearchWaveCount != 0) {
        wave = DirSearchWaveCount;
        node = DirSearchNodeList;
        DirSearchWaveCount = 0;
        while (wave-- != 0) {
            if (node->x == goal_x && node->y == goal_y) {
                DirSearchGoalNode = node;
                if (SelectNextDirPathWaypoint()) {
                    out[0] = b[0];
                    out[1] = b[1];
                    FreeDirNodeList();
                    FreeDirPathNodes();
                    return 2;
                }
                out[0] = (DirPathNodeList->x << 8) + 0x80;
                out[1] = (DirPathNodeList->y << 8) + 0x80;
                FreeDirNodeList();
                FreeDirPathNodes();
                return 1;
            }
            AddWorkerSearchNode(node->x, node->y - 1, node);
            AddWorkerSearchNode(node->x + 1, node->y, node);
            AddWorkerSearchNode(node->x, node->y + 1, node);
            AddWorkerSearchNode(node->x - 1, node->y, node);
            node = node->next;
        }
    }

    FreeDirNodeList();
    FreeDirPathNodes();
    return 0;
}

// FUNCTION: LEGOLAND 0x00482860
unsigned int SaveBestNodeList(void) {
    unsigned int count;
    struct BestNode *node;

    count = 0;
    if (BestNodeList != NULL) {
        node = (struct BestNode *)BestNodeList;
        while (node != NULL) {
            count++;
            node = node->next;
        }
    }

    if (SaveGameWrite(&count, 4) == 0) {
        return 0;
    }

    node = (struct BestNode *)BestNodeList;
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
void FreeBestNodeList(void) {
    struct BestNode *next;
    while (BestNodeList != 0) {
        next = ((struct BestNode *)BestNodeList)->next;
        free(BestNodeList);
        BestNodeList = next;
    }
    ClearEntrance1();
}

// FUNCTION: LEGOLAND 0x00482920
int LoadBestNodeList(void) {
    unsigned int count;
    struct BestNode *node;

    FreeBestNodeList();

    if (SaveGameRead(&count, 4) == 0) {
        return 0;
    }

    while (count-- != 0) {
        node = malloc(sizeof(struct BestNode));
        node->next = BestNodeList;
        BestNodeList = node;
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

// Marks `node` as visited (field_20 bit 2), collects its neighbours with CollectAdjacentNodes (into
// AdjacentNodes, count in AdjacentNodeCount), copies that list and recurses into every neighbour not
// visited yet: a flood fill over the path nodes.
// FUNCTION: LEGOLAND 0x004829c0
void MarkConnectedNodes(struct BestNode *node) {
    int count;
    struct BestNode **copy;
    int i;

    node->field_20 |= 2;
    CollectAdjacentNodes((int *)&node->x_min);
    count = AdjacentNodeCount;
    if (count) {
        copy = (struct BestNode **)malloc(count * sizeof(struct BestNode *));
        if (copy != NULL) {
            memcpy(copy, AdjacentNodes, count * 4);
            for (i = 0; i < count; i++) {
                if (!(copy[i]->field_20 & 2)) {
                    MarkConnectedNodes(copy[i]);
                }
            }
            free(copy);
        }
    }
}

// FUNCTION: LEGOLAND 0x00482a40
void UpdateConnectedNodes(struct Point *pos) {
    struct BestNode *node;
    struct BestNode *found;

    if (BestNodeList != NULL) {
        node = (struct BestNode *)BestNodeList;
        while (node != NULL) {
            node->field_20 &= 0xfffffffd;
            node = node->next;
        }
    }

    found = FindBestNodeAtPoint(pos);
    if (found != NULL) {
        MarkConnectedNodes(found);
    }
}

// FUNCTION: LEGOLAND 0x00482a80
void ClearEntrance1(void) {
    Entrance1Point.x = 0;
    Entrance1Elem = 0;
}

// FUNCTION: LEGOLAND 0x00482a90
void InitEntrance1Point(void) {
    struct MapElement *match;
    struct Ride *obj;

    if (Entrance1Point.x != 0) {
        return;
    }

    if (Entrance1Elem == 0) {
        Entrance1Elem = ElemID("ENTRANCE 1");
    }

    match = (struct MapElement *)GetFirstObjectMatching(
        (Element *)Entrance1Elem);
    obj = ((struct Element *)Entrance1Elem)->obj;

    Entrance1Point.x = match->field_4 + obj->footprint.x0 - 1;
    Entrance1Point.y = ((obj->footprint.y1 + obj->footprint.y0) / 2) + match->field_5;
}

// FUNCTION: LEGOLAND 0x00482b00
struct Point *GetEntrance1Point(void) {
    return &Entrance1Point;
}
