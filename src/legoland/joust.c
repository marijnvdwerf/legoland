#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "binv.h"
#include "bloke.h"
#include "gamemap.h"
#include "joust.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "obj_instance.h"
#include "objclass.h"
#include "print_sprite.h"
#include "render3d.h"
#include "sound_music.h"

struct JoustObject {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00407970
struct JoustNode *AddJoustNode(TileId *key) {
    struct JoustNode *node = (struct JoustNode *)malloc(sizeof(struct JoustNode));

    if (node != NULL) {
        memset(node, 0, sizeof(struct JoustNode));
        node->id.id = key->id;
        node->next = JoustNodeList;
        node->busy = 0;
        node->x = 0;
        node->y = 0;
        memset(&node->sub12, 0, 6);
        memset(&node->pair, 0, 2);
        node->phase = 0;
        node->frame = 0;
        node->field_1c = 0;
        node->field_20 = 0;
        JoustNodeList = node;
    }
    return node;
}

// FUNCTION: LEGOLAND 0x004079e0
void JoustAddObject(Element *editObj, int *coords) {
    TileId key;

    key.pos.x = (unsigned char)coords[0];
    key.pos.y = (unsigned char)coords[1];
    AddBasicObject(editObj, coords);
    AddJoustNode(&key)->sample = 0;
}

// FUNCTION: LEGOLAND 0x00407a20
struct JoustNode *FindJoustNode(TileId *key) {
    struct JoustNode *cur = JoustNodeList;

    if (cur != NULL) {
        do {
            if (memcmp(&cur->id, key, 2) == 0) {
                return cur;
            }
            cur = cur->next;
        } while (cur != NULL);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00407a50
void RemoveJoustNode(struct JoustNode *node) {
    struct JoustNode *prev;
    struct JoustNode *cur;

    if (JoustNodeList == node) {
        JoustNodeList = node->next;
    } else {
        cur = JoustNodeList->next;
        prev = JoustNodeList;
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

// FUNCTION: LEGOLAND 0x00407ab0
void FreeJoustNodeList(void) {
    while (JoustNodeList != NULL) {
        RemoveJoustNode(JoustNodeList);
    }
}

// FUNCTION: LEGOLAND 0x00407ad0
void JoustRemoveObject(Element *editObj, TileId coords, struct Cursor *cursor) {
    struct JoustNode *node;

    node = FindJoustNode(&coords);
    if (node != NULL) {
        struct TileSampleSource source = {2, node->id.pos.x, node->id.pos.y};
        UnSourceAndFadeAllSamplesFromSource(&source, -200);
        node->sample = 0;
        RemoveJoustNode(node);
    }
    StandardRemoveObject(editObj, coords, cursor);
    RemoveAllBlokesFromRide(editObj->ride, coords);
}

// FUNCTION: LEGOLAND 0x00407b50
void JoustLoadResources(struct Element *root) {
    Load_FXList(JOUST_SFX, 1);
    JoustRide = root->ride;
    ((struct Ride *)JoustRide)->flags |= 0x420;
    JoustLayer = ((struct Ride *)JoustRide)->layer;
    ((struct Sprite *)JoustLayer)->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b46f4
    JoustFMaskSprite = LoadSprite("Joust_fmask.lls", 1);
    // STRING: LEGOLAND 0x004b46e0
    JoustSpecRMSprite = LoadSprite("Joust_SpecR_m.lls", 1);
    // STRING: LEGOLAND 0x004b46cc
    JoustSpecLMSprite = LoadSprite("Joust_SpecL_m.lls", 1);
    // STRING: LEGOLAND 0x004b46c0
    JoustRiderSprites[1] = ZJoustSprite = LoadSprite("z_joust.lls", 1);
    // STRING: LEGOLAND 0x004b46a8
    JoustRideBnv = LoadBinV("Zbuffers\\joustride.bnv");
    HideLayer((struct Sprite *)JoustLayer, 1);
    StopLayerPlaying((struct Sprite *)JoustLayer, 1);
    LLSSetFrame((struct LLS *)GetLLSForLayer((struct Sprite *)JoustLayer, 1), 0);
}

/* Returns 1 when the byte is nonzero, else 0. */
// FUNCTION: LEGOLAND 0x00407c20
char IsByteNonZero(unsigned char param_1) {
    return (unsigned int)(param_1 != 0);
}

// FUNCTION: LEGOLAND 0x00407c30
void JoustUpdate(struct Element *elem) {
    struct Ride *ride = elem->ride;
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *b;
    struct JoustNode *jn;
    unsigned char *t;
    char name[9];
    struct JoustPair pair; /* the node's pair copy: one local for both loops, as in the original */

    node = ride->riders;
    // STRING: LEGOLAND 0x004b470c
    strcpy(name, "manBox??");
    for (; node != NULL; node = next) {
        int x;
        int y;
        int busy;
        char count;
        char horses;
        struct JoustSub12 flags;
        char hb;
        unsigned int f1c;
        unsigned int f20;

        next = node->next;
        b = node->rider;
        t = (unsigned char *)&node->tile;
        jn = FindJoustNode((TileId *)t);
        if (jn == NULL) {
            return;
        }
        busy = jn->busy;
        count = jn->x;
        horses = jn->y;
        flags = jn->sub12;
        pair = jn->pair;
        hb = jn->phase;
        f1c = jn->field_1c;
        f20 = jn->field_20;
        (*ZJoustSprite->lls)->frame = jn->frame;
        x = t[0] + ride->x;
        y = t[1] + ride->y;
        if (b->low_level_action == 0) {
            switch (b->param_action) {
            case 0:
                b->flags |= 8;
                b->dest.x = (x + 1) << 8;
                b->dest.y = (y << 8) - 0x100;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                b->field_58 = 0;
                break;
            case 1:
                if (busy == 0) {
                    b->dest.x = (x + 2) << 8;
                    b->dest.y = (y << 8) - 0x100;
                    b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                    b->low_level_action = 7;
                    NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                    b->param_action++;
                    busy = 1;
                } else if (horses < 6) {
                    char slot = rand() % 6;

                    while (flags.v[slot] != 0) {
                        slot++;
                        if (slot > 5) {
                            slot = 0;
                        }
                    }
                    b->field_36 = slot;
                    flags.v[slot] = 2;
                    horses++;
                    b->param_action = (slot < 3) ? 10 : 0x14;
                }
                break;
            case 2:
                if (f1c != 0) {
                    b->dest.x = (x + 1) << 8;
                    b->dest.y = (y - 3) << 8;
                    b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                    b->low_level_action = 7;
                    NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                    b->field_44 = IsByteNonZero(hb);
                    pair.c[b->field_44] = 1;
                    b->param_action++;
                    count++;
                    busy--;
                }
                break;
            case 3:
                b->flags |= 0x80;
                b->person->sprite = JoustRiderSprites[1];
                b->person->field_30 = 1;
                // STRING: LEGOLAND 0x004b4704
                sprintf(name + 6, "%02d", b->field_44 + 1);
                SetBlokePositionFromBNV(JoustRideBnv, b, name, (char)hb, -1617735.0f, -1617993.5f, 0);
                b->param_action++;
                break;
            case 4:
                pair.c[b->field_44] = 2;
                sprintf(name + 6, "%02d", b->field_44 + 1);
                SetBlokePositionFromBNV(JoustRideBnv, b, name, (char)hb, -1617735.0f, -1617993.5f, 0);
                if (b->field_58++ > 0x96) {
                    b->param_action++;
                }
                break;
            case 5:
                sprintf(name + 6, "%02d", b->field_44 + 1);
                SetBlokePositionFromBNV(JoustRideBnv, b, name, (char)hb, -1617735.0f, -1617993.5f, 0);
                pair.c[b->field_44] = 3;
                b->param_action++;
                break;
            case 6:
                sprintf(name + 6, "%02d", b->field_44 + 1);
                SetBlokePositionFromBNV(JoustRideBnv, b, name, (char)hb, -1617735.0f, -1617993.5f, 0);
                if (f20 != 0 && (char)IsByteNonZero(hb) == b->field_44) {
                    b->dest.x = (x + 2) << 8;
                    b->flags &= 0xff7f;
                    b->dest.y = (y << 8) - 0x100;
                    b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                    b->low_level_action = 7;
                    b->person->field_2c = 0;
                    b->person->field_30 = 0;
                    NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                    pair.c[b->field_44] = 1;
                    b->param_action++;
                    count--;
                }
                break;
            case 7:
                b->dest.x = x << 8;
                b->dest.y = y << 8;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                pair.c[b->field_44] = 0;
                b->param_action = 0x1e;
                break;
            case 10:
            case 15:
                b->dest.x = (x << 8) - 0x100;
                b->dest.y = (y << 8) - 0x100;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 12:
                b->dest.x = (x << 8) - 0x364;
                b->dest.y = (y << 8) + b->field_36 * 200 - 0x638;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 11:
                b->dest.x = (x << 8) - 0x364;
                b->dest.y = (y - 3) << 8;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 13:
            case 25:
                b->dir = 3;
                if (b->field_58++ > 0x96) {
                    horses--;
                    flags.v[b->field_36] = 0;
                    b->param_action++;
                }
                break;
            case 14:
                b->dest.x = (x << 8) - 0x364;
                b->dest.y = (y - 3) << 8;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 16:
                b->dest.x = x << 8;
                b->dest.y = y << 8;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action = 0x1e;
                break;
            case 20:
                b->dest.x = (x << 8) - 0x100;
                b->dest.y = (y << 8) - 0x100;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 21:
                b->dest.x = (x << 8) - 0x16a;
                b->dest.y = (y - 2) << 8;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 22:
                b->dest.x = (x << 8) - 0x16a;
                b->dest.y = (y << 8) - 0xf46;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 24:
                b->dest.x = (x << 8) - 0x364;
                b->dest.y = (y << 8) + b->field_36 * 200 - 0x102e;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 23:
            case 26:
                b->dest.x = (x << 8) - 0x364;
                b->dest.y = (y << 8) - 0xf46;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 27:
                b->dest.x = (x << 8) - 0x16a;
                b->dest.y = (y << 8) - 0xf46;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 28:
                b->dest.x = (x << 8) - 0x16a;
                b->dest.y = (y - 2) << 8;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 29:
                b->dest.x = x << 8;
                b->dest.y = y << 8;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action = 0x1e;
                break;
            case 30:
                RemoveBlokeFromRide(ride, node);
                b->flags &= 0xfff7;
                break;
            }
        }
        jn->busy = busy;
        jn->x = count;
        jn->y = horses;
        jn->sub12 = flags;
        jn->pair = pair;
        jn->frame = hb;
        jn->field_1c = f1c;
        jn->field_20 = f20;
    }
    for (jn = JoustNodeList; jn != NULL; jn = jn->next) {
        char step = jn->phase;
        int busy = jn->busy;
        char count = jn->x;
        unsigned int f20 = jn->field_20;
        unsigned int f1c = 0;

        pair = jn->pair;
        do {
            switch (0) {
            default:
                if (step == 0) {
                    if ((busy != 0 && count < 2) || count == 0) {
                        if (pair.c[0] != 0) {
                            break;
                        }
                        f1c = 1;
                    } else if (pair.c[0] != 1) {
                        if (pair.c[0] != 3) {
                            break;
                        }
                        f20 = 1;
                    }
                } else if (step == 0x20) {
                    if ((busy != 0 && count < 2) || count == 0) {
                        if (pair.c[1] != 0) {
                            break;
                        }
                        f1c = 1;
                    } else if (pair.c[1] != 1) {
                        if (pair.c[1] != 3) {
                            break;
                        }
                        f20 = 1;
                    }
                } else {
                    break;
                }
                if (jn->sample != 0) {
                    struct SampleSource stop;

                    stop.type = 2;
                    stop.x = jn->id.pos.x;
                    stop.y = jn->id.pos.y;
                    UnSourceAndFadeAllSamplesFromSource(&stop, -1000);
                    jn->sample = 0;
                }
                continue;
            }
            if (jn->sample == 0) {
                struct SampleSource play;

                play.type = 2;
                play.x = jn->id.pos.x;
                play.y = jn->id.pos.y;
                jn->sample = PlayInstanceOfSample(JOUST_SFX[0].sample, 1, 0, &play);
                SetSampleFlag20(jn->sample);
            }
            step++;
            if (step > 0x3f) {
                step = 0;
            }
            f1c = 0;
            f20 = 0;
        } while (0);
        jn->pair = pair;
        jn->phase = step;
        jn->field_1c = f1c;
        jn->field_20 = f20;
    }
}

// FUNCTION: LEGOLAND 0x00408580
void RenderJoust(struct Element *element, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int param_6) {
    struct Ride *ride;
    struct RideNode *r;
    struct JoustNode *node;
    struct Bloke *bloke;
    struct Person *person;
    struct Point off;
    struct Point coords;
    char frame;
    char i;

    ride = element->ride;
    r = ride->riders;
    {
        struct Bloke *riders[8] = {0};
        char n = 0;
        node = FindJoustNode(tile);
        if (node == NULL) {
            return;
        }
        frame = node->frame;
        coords = GetScreenCoordsForObject(tile, ride);
        if (r != NULL) {
            for (; r != NULL; r = r->next) {
                if (tile->id == r->tile.id) {
                    riders[n++] = r->rider;
                }
            }
            if (n != 0) {
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x18) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x19) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x1a) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x1b) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x1c) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                off = GetRenderOffsetForLayer((struct Sprite *)JoustLayer, 0);
                AdjustOffsetForViewMode(&off);
                PrintSprite(JoustSpecRMSprite, coords.x + off.x, coords.y + off.y, param_6, 0);
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0xc) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0xd) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0xe) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0xf) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                off = GetRenderOffsetForLayer((struct Sprite *)JoustLayer, 0);
                AdjustOffsetForViewMode(&off);
                PrintSprite(JoustSpecLMSprite, coords.x + off.x, coords.y + off.y, param_6, 0);
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x16) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x17) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x1d) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                LLSSetFrame(GetLLSForLayer((struct Sprite *)JoustLayer, 1), frame);
                off = GetRenderOffsetForLayer((struct Sprite *)JoustLayer, 1);
                AdjustOffsetForViewMode(&off);
                PrintSprite(GetSpriteForLayer((struct Sprite *)JoustLayer, 1), coords.x + off.x, coords.y + off.y, param_6, 0);
                for (r = ride->riders; r != NULL; r = r->next) {
                    if (tile->id == r->tile.id) {
                        bloke = r->rider;
                        if (bloke->flags & 0x80) {
                            struct Point poff;
                            person = bloke->person;
                            poff.x = 0;
                            poff.y = -48;
                            person->offset.x = bloke->screen_x;
                            person->offset.y = bloke->screen_y;
                            AdjustBlokePosition(&person->offset);
                            AdjustOffsetForViewMode(&poff);
                            person->screen.x = bloke->screen_x + poff.x + coords.x;
                            person->screen.y = bloke->screen_y + coords.y + poff.y;
                            AdjustBlokePosition(&person->screen);
                            IP_RenderBlokeIn3DNow(r->rider);
                        }
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x10) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0xb) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0xa) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 1) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 2) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 3) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 7) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x14) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x15) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                for (i = 0; i < n; i++) {
                    if (riders[i]->param_action == 0x1e) {
                        IP_RenderBlokeIn3DNow(riders[i]);
                    }
                }
                off = GetRenderOffsetForLayer((struct Sprite *)JoustLayer, 1);
                AdjustOffsetForViewMode(&off);
                PrintSprite(JoustFMaskSprite, coords.x + off.x, coords.y + off.y, param_6, 0);
                return;
            }
        }
        LLSSetFrame(GetLLSForLayer((struct Sprite *)JoustLayer, 1), frame);
        off = GetRenderOffsetForLayer((struct Sprite *)JoustLayer, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite(GetSpriteForLayer((struct Sprite *)JoustLayer, 1), coords.x + off.x, coords.y + off.y, param_6, 0);
    }
}

// FUNCTION: LEGOLAND 0x00408bc0
void JoustSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = (void *)JoustRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00408c00
void JoustFreeResources(void) {
    Kill_FXList(JOUST_SFX, 1);
    KillSprite(JoustFMaskSprite);
    KillSprite(JoustSpecRMSprite);
    KillSprite(JoustSpecLMSprite);
    FreeBinV(JoustRideBnv);
    KillSprite(JoustRiderSprites[1]);
    FreeJoustNodeList();
}

// FUNCTION: LEGOLAND 0x00408c50
struct RideSpriteInfo *GetJoustSpriteInfo(struct Element *param1, unsigned short param2) {
    struct Ride *block = param1->ride;

    JoustSpriteInfo.sprite = block->layer;
    JoustSpriteInfo.x = block->field_14;
    JoustSpriteInfo.y = block->field_18;
    JoustSpriteInfo.id = param2;
    block->layer->flags |= 0x2000;

    return &JoustSpriteInfo;
}

// FUNCTION: LEGOLAND 0x00408c90
LEGO_EXPORT int SaveJoust(struct Element *element) {
    struct JoustNode *current = JoustNodeList;
    unsigned int flag = 1;
    unsigned int terminator = 0;

    while (current != NULL) {
        if (!SaveGameWrite(&flag, 4)) {
            return 0;
        }
        if (!SaveGameWrite(current, sizeof(struct JoustNode))) {
            return 0;
        }
        current = current->next;
    }

    if (SaveGameWrite(&terminator, 4)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00408d00
LEGO_EXPORT int LoadJoust(struct Element *arg) {
    struct Ride *obj = arg->ride;
    struct JoustNode *prev = NULL;
    struct RideNode *list;
    struct Person *car;
    unsigned int marker;

    if (!SaveGameRead(&marker, 4)) {
        return 0;
    }
    while (marker != 0) {
        struct JoustNode *node = (struct JoustNode *)malloc(sizeof(struct JoustNode));
        if (!SaveGameRead(node, sizeof(struct JoustNode))) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            JoustNodeList = node;
        }
        node->sample = 0;
        prev = node;
        if (!SaveGameRead(&marker, 4)) {
            return 0;
        }
    }

    list = obj->riders;
    while (list != NULL) {
        car = list->person;
        if (car->field_30 != 0) {
            car->sprite = JoustRiderSprites[car->field_30];
        } else {
            car->sprite = NULL;
            list->person->field_30 = 0;
        }
        list = list->next;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00408db0
LEGO_EXPORT void Joust_GetInterfaces(struct Element *head, struct Ride *iface) {
    // STRING: LEGOLAND 0x004b4718
    if (_stricmp("JOUST", head->name) == 0) {
        iface->cb_a4 = JoustLoadResources;
        iface->cb_ac = JoustFreeResources;
        iface->cb_8c = JoustSetEditMode;
        iface->cb_pre_render = JoustUpdate;
        iface->cb_b0 = RenderJoust;
        iface->cb_9c = JoustRemoveObject;
        iface->cb_98 = JoustAddObject;
        iface->cb_sprite = GetJoustSpriteInfo;
        iface->save_hook = SaveJoust;
        iface->load_hook = LoadJoust;
    }
}
