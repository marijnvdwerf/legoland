#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "bloke.h"
#include "earth_slide.h"
#include "gamemap.h"
#include "globals.h"
#include "image_sprite.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"

// FUNCTION: LEGOLAND 0x0042cd70
void AddEarthNode(unsigned short *a1) {
    struct EarthNode *v = (struct EarthNode *)malloc(sizeof(struct EarthNode));
    if (v != NULL) {
        memset(v, 0, sizeof(struct EarthNode));
        v->id = *a1;
        v->next = EarthNodeHead;
        v->field_10 = 0;
        v->field_14 = 0;
        v->field_b = 0;
        v->field_8 = 0;
        v->field_a = 0;
        v->field_4 = 1;
        EarthNodeHead = v;
    }
}

// FUNCTION: LEGOLAND 0x0042cdc0
void RemoveEarthNode(struct EarthNode *node) {
    struct EarthNode *cur;
    struct EarthNode *prev;

    if (EarthNodeHead == node) {
        EarthNodeHead = node->next;
    } else {
        cur = EarthNodeHead->next;
        prev = EarthNodeHead;
        while (cur != node) {
            prev = prev->next;
            if (prev == NULL) {
                break;
            }
            cur = prev->next;
        }
        if (prev != NULL) {
            prev->next = node->next;
        }
    }
    free(node);
}

// FUNCTION: LEGOLAND 0x0042ce20
struct EarthNode *FindEarthNode(volatile unsigned short *param_1) {
    struct EarthNode *node = EarthNodeHead;

    if (node != NULL) {
        if (*param_1 == node->id) {
            return node;
        }
        while (1) {
            node = node->next;
            if (node == NULL) {
                break;
            }
            if (*param_1 == node->id) {
                return node;
            }
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0042ce50
void EnqueueEarthRider(struct EarthNode *node, struct RideNode *elem) {
    struct EarthQueue *q = (struct EarthQueue *)malloc(sizeof(struct EarthQueue));
    if (q != NULL) {
        memset(q, 0, sizeof(struct EarthQueue));
        q->elem = elem;
        *(unsigned char *)&elem->rider->flags |= 0x40;
        EarthNodeAppendQueueEntry(node, q);
        node->queue_count++;
    }
}

// FUNCTION: LEGOLAND 0x0042ce90
void EarthNodeAppendQueueEntry(struct EarthNode *p, struct EarthQueue *value) {
    if (p->queue_head == NULL && p->queue_tail == NULL) {
        p->queue_head = value;
        p->queue_tail = value;
    } else {
        p->queue_tail->next = value;
        p->queue_tail = value;
    }
}

/* Returns the fixed-point waiting position for a new earth-slide rider, using the queue length to select an offset
 * from the queue-position table. */
// FUNCTION: LEGOLAND 0x0042cec0
void GetEarthSlideQueuePosition(struct Ride *param_1, unsigned char *param_2, int *param_3) {
    struct EarthNode *node = FindEarthNode((unsigned short *)param_2);
    struct EarthQueue *q = node->queue_head;
    int x = param_1->x + param_2[0];
    int y = param_2[1] + param_1->y;
    int i;

    if (q == NULL) {
        param_3[0] = (x + EarthSlideQueueOffsets[0]) * 0x100;
        param_3[1] = (EarthSlideQueueOffsets[1] + y) * 0x100;
        return;
    }
    i = 0;
    do {
        q = q->next;
        i++;
    } while (q != NULL);
    param_3[0] = (EarthSlideQueueOffsets[i * 2] + x) * 0x100;
    param_3[1] = (EarthSlideQueueOffsets[i * 2 + 1] + y) * 0x100;
}

// FUNCTION: LEGOLAND 0x0042cf40
int IsEarthQueueHead(struct EarthNode *param_1, void *param_2) {
    if (param_1->queue_head != NULL) {
        if (param_1->queue_head->elem->rider == param_2) {
            return 1;
        }
    }
    return 0;
}

/* Releases and advances the first queued earth-slide rider, removes its queue entry, and moves the remaining riders
 * to their updated waiting positions. */
// FUNCTION: LEGOLAND 0x0042cf70
void AdvanceEarthSlideQueue(struct EarthNode *param_1) {
    struct Bloke *bloke;
    struct EarthQueue *q;
    int x;
    struct Point y;
    int *tbl;
    char cv;

    if (param_1->queue_head != NULL) {
        bloke = param_1->queue_head->elem->rider;
        bloke->flags &= 0xffbf;
        bloke->param_action += 1;
        DequeueEarthRider(param_1);
        q = param_1->queue_head;
        if (q != NULL) {
            tbl = &EarthSlideQueueOffsets[1];
            x = EarthSlideRide->x + *(unsigned char *)param_1;
            y.y = EarthSlideRide->y + *((unsigned char *)param_1 + 1);
            do {
                bloke = q->elem->rider;
                bloke->dest.x = (tbl[-1] + x) * 0x100;
                bloke->dest.y = (tbl[0] + y.y) * 0x100;
                cv = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                q = q->next;
                tbl = tbl + 2;
            } while (q != NULL);
        }
    }
}

// FUNCTION: LEGOLAND 0x0042d040
void DequeueEarthRider(struct EarthNode *list) {
    if (list->queue_head != NULL) {
        struct EarthQueue *node = list->queue_head;
        list->queue_head = node->next;
        if (list->queue_tail == node) {
            list->queue_tail = NULL;
        }
        list->queue_count--;
    }
}

// FUNCTION: LEGOLAND 0x0042d070
void RenderEarthSlide(struct Element *param_1, unsigned int param_2, unsigned int param_3, unsigned char *param_4, void *param_5, unsigned int param_6) {
    struct Ride *ride = param_1->ride;
    struct EarthNode *node = FindEarthNode((unsigned short *)param_4);
    struct Point coords;
    char cv;

    if (node != NULL) {
        cv = node->field_b;
        if (cv <= 0xa || 0xf <= cv) {
            RenderUsingRin((struct RinData *)EarthSlideRin, (int)cv, (struct Ride *)ride, param_4);
        }
        RenderBlokesNotInSeats(ride, (TileId *)param_4);
        if (node->field_4 != 0) {
            coords = GetScreenCoordsForObject((TileId *)param_4, ride);
            PrintSprite(EarthSlideEntranceMatteSprite, coords.x, coords.y, param_6, 0);
        }
        coords = GetScreenCoordsForObject((TileId *)param_4, ride);
        PrintSprite(EarthSlideEntranceMatte2Sprite, coords.x, coords.y, param_6, 0);
    }
}

// FUNCTION: LEGOLAND 0x0042d100
void LoadEarthSlideResources(struct Element *param_1) {
    struct Sprite *layer;

    EarthSlideRide = param_1->ride;
    EarthSlideRide->flags |= 0x20;
    layer = EarthSlideRide->layer;
    if (layer != NULL) {
        layer->flags |= 0x2000;
    }
    EarthNodeHead = NULL;
    // STRING: LEGOLAND 0x004b663c
    EarthPos = LoadPos("3ddata\\earth.pos");
    // STRING: LEGOLAND 0x004b6620
    EarthSlideRin = LoadRin("3ddata\\earthslide.rin", CurrentDirPath);
    if (EarthSlideRin != NULL) {
        EarthSlideRin->x = -0x9e;
        EarthSlideRin->y = -6;
        EarthSlideRin->data_table = NULL;
    }
    if (EarthPos != NULL) {
        EarthPos->field_14 = 0x53;
        EarthPos->field_18 = 0xc4;
        EarthPos->x = 3;
        EarthPos->y = 0x41;
    }
    // STRING: LEGOLAND 0x004b6600
    EarthSlideEntranceMatteSprite = LoadSprite("EarthSlide Entrance Matte.lls", 1);
    // STRING: LEGOLAND 0x004b65e0
    EarthSlideEntranceMatte2Sprite = LoadSprite("EarthSlideEntranceMatte2.lls", 1);
    UnusedEarthSlideA = 0;
    UnusedEarthSlideB = 0;
}

// FUNCTION: LEGOLAND 0x0042d1f0
void UnloadEarthSlideResources(struct Element *arg1) {
    EarthSlideRide = arg1->ride;
    UnLoadRin(EarthSlideRin);
    UnloadPos(EarthPos);
    KillSprite(EarthSlideEntranceMatteSprite);
    KillSprite(EarthSlideEntranceMatte2Sprite);
}

// FUNCTION: LEGOLAND 0x0042d230
void EarthSlideSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = (void *)EarthSlideRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x0042d270
void EarthSlideRemoveObject(struct Element *param_1, TileId tile, struct Cursor *param_3) {
    struct EarthNode *node = FindEarthNode(&tile.id);
    if (node != NULL) {
        RemoveEarthNode(node);
    }
    StandardRemoveObject(param_1, tile, param_3);
    RemoveAllBlokesFromRide(param_1->ride, tile);
}

// FUNCTION: LEGOLAND 0x0042d2c0
void EarthSlideAddObject(struct Element *param_1, unsigned char *param_2) {
    unsigned char buffer[2];

    buffer[0] = param_2[0];
    buffer[1] = param_2[4];
    AddBasicObject(param_1, (int *)param_2);
    AddEarthNode((unsigned short *)buffer);
}

// FUNCTION: LEGOLAND 0x0042d2f0
int EarthSlideRide_Save(struct Element *element) {
    struct EarthNode *node;
    struct EarthQueue *q;
    int count;
    unsigned int value;
    int flag;
    int terminator;

    flag = 1;
    node = EarthNodeHead;
    terminator = 0;
    while (node != NULL) {
        if (SaveGameWrite(&flag, 4) == 0 || SaveGameWrite(node, sizeof(struct EarthNode)) == 0) {
            return 0;
        }
        count = 0;
        for (q = node->queue_head; q != NULL; q = q->next) {
            count++;
        }
        if (SaveGameWrite(&count, 4) == 0) {
            return 0;
        }
        for (q = node->queue_head; q != NULL; q = q->next) {
            value = CountEarthBlokeElemsUntil((struct RideNode *)EarthSlideRide->riders, q->elem);
            if (SaveGameWrite(&value, 4) == 0) {
                return 0;
            }
        }
        node = node->next;
    }
    return SaveGameWrite(&terminator, 4) != 0;
}

// FUNCTION: LEGOLAND 0x0042d3e0
unsigned int CountEarthBlokeElemsUntil(struct RideNode *param_1, struct RideNode *param_2) {
    unsigned int count = 0;
    struct RideNode *node = param_1;

    while (node != NULL && node != param_2) {
        node = node->next;
        count++;
    }
    return count;
}

// FUNCTION: LEGOLAND 0x0042d400
int EarthSlideRide_Load(struct Element *element) {
    struct EarthNode *node = NULL;
    struct EarthNode *next;
    struct EarthQueue *q;
    int flag;
    int count;
    int value;

    if (SaveGameRead(&flag, 4) == 0) {
        return 0;
    }
    while (flag != 0) {
        if (node == NULL) {
            node = (struct EarthNode *)malloc(sizeof(struct EarthNode));
            EarthNodeHead = node;
        } else {
            next = (struct EarthNode *)malloc(sizeof(struct EarthNode));
            node->next = next;
            node = next;
        }
        if (SaveGameRead(node, sizeof(struct EarthNode)) == 0) {
            return 0;
        }
        if (SaveGameRead(&count, 4) == 0) {
            return 0;
        }
        node->queue_head = NULL;
        node->queue_tail = NULL;
        while (count-- != 0) {
            if (node->queue_tail == NULL) {
                q = (struct EarthQueue *)malloc(sizeof(struct EarthQueue));
                node->queue_tail = q;
                node->queue_head = q;
            } else {
                q = (struct EarthQueue *)malloc(sizeof(struct EarthQueue));
                node->queue_tail->next = q;
                node->queue_tail = node->queue_tail->next;
            }
            if (SaveGameRead(&value, 4) == 0) {
                return 0;
            }
            node->queue_tail->elem = GetNthNextEarthBlokeElem((struct RideNode *)EarthSlideRide->riders, value);
        }
        if (node->queue_tail != NULL) {
            node->queue_tail->next = NULL;
        }
        if (SaveGameRead(&flag, 4) == 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0042d540
struct RideNode *GetNthNextEarthBlokeElem(struct RideNode *param_1, unsigned int param_2) {
    struct RideNode *result = param_1;

    while (param_2-- != 0) {
        result = result->next;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0042d560
void UpdateEarthNode(struct EarthNode *param_1) {
    int v14;
    unsigned int flags;
    char nb;

    flags = param_1->field_10;
    if ((flags & 1) != 0) {
        if ((v14 = ++param_1->field_14) > 2) {
            param_1->field_14 = 0;
            nb = ++*(char *)&param_1->field_b;
            if ((int)nb >= EarthSlideRin->data_count) {
                flags = flags & 0xfffffffe;
                param_1->field_b = 0;
                param_1->field_10 = flags;
                GetAllBlokesOffRide((struct Ride *)EarthSlideRide, param_1->id);
                param_1->field_4 = 1;
                return;
            }
        }
        Put3DBlokesOnRide((struct Ride *)EarthSlideRide, (unsigned char *)param_1, (int)*(char *)&param_1->field_b, EarthPos);
    }
    Put3DBlokesOnRide2(EarthSlideRide, param_1);
}

// FUNCTION: LEGOLAND 0x0042d5f0
void UpdateEarthNodes(void) {
    struct EarthNode *node = EarthNodeHead;
    while (node != NULL) {
        UpdateEarthNode(node);
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x0042d610
void EarthSlideRideUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *elem = ride->riders;
    struct RideNode *next;
    struct EarthNode *node;
    struct Bloke *bloke;
    TileId *tile;
    int tx, ty;
    int x, y; /* map tile of the bloke's slide entrance */
    Point exit; /* map tile where the bloke leaves the ride */
    Point target; /* boarding spot, from GetEarthSlideQueuePosition */
    char dir;

    UpdateEarthNodes();
    while (elem != NULL) {
        next = elem->next;
        tile = &elem->tile;
        bloke = elem->rider;
        node = FindEarthNode(&tile->id);
        if (node == NULL) {
            return;
        }
        tx = tile->pos.x;
        x = ride->x + tx;
        ty = tile->pos.y;
        y = ride->y + ty;
        exit.x = ride->field_24 + tx;
        exit.y = ride->field_25 + ty;
        if (bloke->low_level_action == 0) {
            /* param_action is the bloke's step through the ride */
            switch (bloke->param_action) {
            case 0:
                /* walk to the boarding spot */
                bloke->flags |= 8;
                GetEarthSlideQueuePosition(ride, &tile->pos.x, &target.x);
                EnqueueEarthRider(node, (struct RideNode *)elem);
                bloke->dest.x = target.x;
                bloke->dest.y = target.y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                /* wait for a free slide */
                if ((node->field_10 & 0x8000) == 0 && IsEarthQueueHead(node, bloke) != 0) {
                    AdvanceEarthSlideQueue(node);
                    if (++node->field_9 == 1) {
                        node->field_10 |= 0x8000;
                    }
                    bloke->dest.x = (x << 8) - 0x180;
                    bloke->dest.y = (y << 8) + 0x380;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = dir + 0x10;
                    NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                    bloke->param_action++;
                }
                break;
            case 2:
                bloke->dest.x = (x - 3) << 8;
                bloke->dest.y = (y + 3) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 3:
                bloke->dest.x = (x - 4) << 8;
                bloke->dest.y = (y + 3) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                bloke->pos.x = (x - 4) << 8;
                bloke->pos.y = (y + 3) << 8;
                bloke->field_58 = (rand() & 0x1f) + 4;
                bloke->param_action++;
                break;
            case 5:
                if (bloke->field_58 == 0) {
                    elem->rider->param_action++;
                }
                bloke->field_58--;
                break;
            case 6:
                /* sit down in the slide */
                bloke->flags |= 0x80;
                BlokeSitAnim(bloke);
                BlokeSetFrame(bloke, 0);
                node->field_8++;
                node->field_9--;
                if (node->field_8 == 1) {
                    node->field_a = node->field_8;
                    node->field_b = 0;
                    node->field_10 |= 1;
                    node->field_14 = 0;
                    node->field_4 = 0;
                }
                bloke->field_58 = 8;
                bloke->param_action++;
                Put3DBlokesOnRide((struct Ride *)EarthSlideRide, (unsigned char *)node, (char)node->field_b, EarthPos);
                break;
            case 8:
                /* get off at the bottom */
                BlokeWalkAnim(bloke);
                bloke->flags &= ~0x80;
                bloke->pos.x = (exit.x << 8) + 0x80;
                bloke->pos.y = (exit.y << 8) + 0x80;
                RemoveBlokeFromRide(ride, elem);
                bloke->flags &= ~8;
                if (--node->field_a == 0) {
                    node->field_8 = 0;
                    node->field_4 = 1;
                    node->field_10 &= ~0x8000;
                }
                if ((short)(char)node->queue_count != ride->seats) {
                    Ride_ClearFlagToNotLetAnyoneOn(node);
                }
                break;
            }
        }
        elem = next;
    }
}
