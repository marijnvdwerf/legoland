#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include <stdio.h>
#include "binv.h"
#include "bloke.h"
#include "gamemap.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"
#include "safari_ride.h"
#include "sound_music.h"
#include "tilemap.h"

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004149c0
void AddSafariNode(struct SafariNode *param) {
    struct SafariNode *s = (struct SafariNode *)malloc(sizeof(struct SafariNode));
    if (s) {
        memset(s, 0, sizeof(struct SafariNode));
        s->tile_id = param->tile_id;
        s->next = SafariNodeList;
        SafariNodeList = s;
        ResetSafariRide(s);
    }
}

// FUNCTION: LEGOLAND 0x00414a00
void RemoveSafariNode(struct SafariNode *node) {
    struct SafariNode *prev;
    struct SafariNode *cur;

    if (SafariNodeList == node) {
        SafariNodeList = node->next;
    } else {
        cur = SafariNodeList->next;
        prev = SafariNodeList;
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

// FUNCTION: LEGOLAND 0x00414a60
void FreeAllSafariNodes(void) {
    while (SafariNodeList != NULL) {
        RemoveSafariNode(SafariNodeList);
    }
}

struct SafariKey {
    unsigned short id;
    unsigned short pad;
};

// FUNCTION: LEGOLAND 0x00414a80
void *FindSafariNode(struct SafariKey *key) {
    struct SafariNode *cur = SafariNodeList;

    if (cur != NULL) {
        do {
            if (memcmp(&cur->tile_id, key, 2) == 0) {
                return cur;
            }
            cur = cur->next;
        } while (cur != NULL);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00414ab0
void StartSafariRide(struct SafariNode *a1) {
    struct SampleSource src;

    src.type = 2;
    a1->field_8 = a1->field_4;
    a1->field_14 = (a1->field_14 & 0xFFFFBFFF) | 1;
    a1->field_4 = 0;
    a1->frame = 0;
    a1->frame_ticks = 0;
    src.x = a1->tile_x;
    src.y = a1->tile_y;
    PlayInstanceOfSample(SAFARI_SFX[0].sample, 1, 1, &src);
}

// FUNCTION: LEGOLAND 0x00414b10
void ResetSafariRide(struct SafariNode *node) {
    struct SafariNode *a1 = (struct SafariNode *)node;
    struct SampleSource src;

    a1->frame_ticks = 0;
    a1->frame = 0;
    a1->loops_left = rand() % 2 + 3;
    a1->field_20 = 0;
    a1->field_4 = 0;
    src.type = 2;
    a1->field_14 &= 0xFFFFBFFE;
    src.x = a1->tile_x;
    src.y = a1->tile_y;
    UnSourceAndFadeAllSamplesFromSource(&src, -200);
}

// FUNCTION: LEGOLAND 0x00414b80
void RenderSafari(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct SafariNode *sn;
    struct Point pos;
    char found = 0;

    sn = FindSafariNode((struct SafariKey *)tile);
    if (sn != NULL) {
        RenderItems_New();
        SafariRiderRenderList = NULL;
        pos = GetScreenCoordsForObject((TileId *)tile, ride);
        for (; node != NULL; node = node->next) {
            if (*tile == node->tile.id) {
                found = 1;
                break;
            }
        }
        *(short *)*ZSafariSprite->lls = (short)sn->frame;
        if (found) {
            LLSSetFrame(GetLLSForLayer(SafariLayer, 0), sn->frame);
            PrintSprite(GetSpriteForLayer(SafariLayer, 0), pos.x, pos.y, clip, 0);
            for (node = ride->riders; node != NULL; node = node->next) {
                if (*tile == node->tile.id) {
                    struct Bloke *bloke = node->rider;

                    if ((bloke->flags & 0x80) != 0) {
                        struct Point off2;
                        struct Person *person;
                        struct Point off1;

                        off2 = SafariRideDrawOffset;
                        person = bloke->person;
                        off1.x = SafariRideDrawOffset1X;
                        off1.y = SafariRideDrawOffset1Y;
                        AdjustOffsetForViewMode(&off1);
                        person->offset.x = bloke->screen_x + off1.x;
                        person->offset.y = bloke->screen_y + off1.y;
                        AdjustBlokePosition(&person->offset);
                        AdjustOffsetForViewMode(&off2);
                        person->screen.x = bloke->screen_x + (off2.x + off1.x) + pos.x;
                        person->screen.y = bloke->screen_y + (off2.y + off1.y) + pos.y;
                        AdjustBlokePosition(&person->screen);
                    }
                    AddBlokeToRenderList(&SafariRiderRenderList, node, node->person->field_20);
                }
            }
        } else {
            LLSSetFrame(GetLLSForLayer(SafariLayer, 0), sn->frame);
            PrintSprite(GetSpriteForLayer(SafariLayer, 0), pos.x, pos.y, clip, 0);
        }
        RenderBlokeList((struct RenderItemNode **)&SafariRiderRenderList);
    }
}

// FUNCTION: LEGOLAND 0x00414d90
void SafariRideLoad(struct Element *a1) {
    SafariRide = a1->ride;
    SafariRide->flags |= 0x420;
    SafariLayer = SafariRide->layer;
    SafariLayer->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b4d5c
    SafariRunBNV = LoadBinV("Zbuffers\\Safarirun.bnv");
    // STRING: LEGOLAND 0x004b4d44
    SafariOnBNV[0] = LoadBinV("Zbuffers\\Safarion.bnv");
    // STRING: LEGOLAND 0x004b4d2c
    SafariOffBNV = LoadBinV("Zbuffers\\Safarioff.bnv");
    // STRING: LEGOLAND 0x004b4d1c
    ZSafariSprite = LoadSprite("z_Safari.lls", 1);
    SafariRideDrawOffset.x = 0;
    SafariRideDrawOffset.y = -1;
    SafariRideDrawOffset1X = -41;
    SafariRideDrawOffset1Y = -95;
    HideLayer(SafariLayer, 0);
    StopLayerPlaying(SafariLayer, 0);
    LLSSetFrame(GetLLSForLayer(SafariLayer, 0), 0);
    SafariPathRunBNV = SafariRunBNV;
    SafariPathOnBNV = SafariOnBNV[0];
    SafariPathOffBNV = SafariOffBNV;
    SafariRiderSprite = ZSafariSprite;
    Load_FXList(SAFARI_SFX, 1);
}

// FUNCTION: LEGOLAND 0x00414ea0
void SafariRideUnload(struct Element *a1) {
    SafariRide = a1->ride;
    FreeAllSafariNodes();
    Kill_FXList(SAFARI_SFX, 1);
    FreeBinV(SafariPathRunBNV);
    FreeBinV(SafariPathOnBNV);
    FreeBinV(SafariPathOffBNV);
    KillSprite(SafariRiderSprite);
}

// FUNCTION: LEGOLAND 0x00414f00
void SafariSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = (void *)SafariRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00414f40
void SafariRemoveObject(struct Element *obj, TileId key, struct Cursor *coords, unsigned int cursor) {
    void *node = FindSafariNode((struct SafariKey *)&key);
    struct SampleSource src;

    if (node == 0) {
        return;
    }
    RemoveSafariNode((struct SafariNode *)node);
    StandardRemoveObject(obj, key, (struct Cursor *)coords);
    RemoveAllBlokesFromRide(obj->ride, key);

    src.type = 2;
    src.x = key.pos.x;
    src.y = key.pos.y;
    UnSourceAndFadeAllSamplesFromSource(&src, 0xffffff38);
}

struct SafariBasicObject {
    unsigned char field_0;
    unsigned char pad_1[3];
    unsigned char field_4;
};

// FUNCTION: LEGOLAND 0x00414fc0
void SafariAddObject(struct Element *uid, struct SafariBasicObject *a1) {
    unsigned short local;

    *(unsigned char *)&local = a1->field_0;
    *((unsigned char *)&local + 1) = a1->field_4;
    AddBasicObject(uid, (int *)a1);
    AddSafariNode((struct SafariNode *)&local);
}

// FUNCTION: LEGOLAND 0x00414ff0
struct RideSpriteInfo *GetSafariRideSpriteInfo(struct Element *p1, unsigned short arg2) {
    struct Ride *pB = p1->ride;
    SafariSpriteInfo.sprite = pB->layer;
    SafariSpriteInfo.x = pB->field_14;
    SafariSpriteInfo.y = pB->field_18;
    SafariSpriteInfo.id = arg2;
    pB->layer->flags |= 0x2000;
    return &SafariSpriteInfo;
}

// FUNCTION: LEGOLAND 0x00415030
void SafariRideGetInterfaces(struct Element *name, struct Ride *interfaces) {
    // STRING: LEGOLAND 0x004b4d74
    if (_stricmp("SAFARI RIDE", name->name) == 0) {
        interfaces->cb_a4 = SafariRideLoad;
        interfaces->cb_ac = SafariRideUnload;
        interfaces->cb_8c = SafariSetEditMode;
        interfaces->cb_pre_render = SafariRideUpdate;
        interfaces->cb_b0 = RenderSafari;
        interfaces->cb_9c = SafariRemoveObject;
        interfaces->cb_98 = SafariAddObject;
        interfaces->cb_sprite = GetSafariRideSpriteInfo;
        interfaces->save_hook = SaveSafariRide;
        interfaces->load_hook = LoadSafariRide;
    }
}

// FUNCTION: LEGOLAND 0x004150c0
void UpdateSafariNode(struct SafariNode *node) {
    struct SafariNode *s = (struct SafariNode *)node;
    struct RideNode *r = ((struct Ride *)SafariRide)->riders;
    unsigned int flags = s->field_14;

    if (flags & 1) {
        int v = ++s->frame_ticks;
        int n = s->loops_left;
        if (n == 0) {
            if (GetAllBlokesOffRide((struct Ride *)SafariRide, s->tile_id) == 0) {
                return;
            }
            ResetSafariRide(node);
            return;
        }
        if (v >= 2) {
            s->frame_ticks = 0;
            s->frame++;
            if (s->frame >= 0x30) {
                s->frame = 0;
                s->loops_left = n - 1;
            }
        }
    } else {
        int cur = s->field_4;
        if (flags & 0x4000) {
            if (cur == s->field_20) {
                s->field_14 = flags & 0xffffbfff;
                StartSafariRide((struct SafariNode *)s);
                return;
            }
        } else if (cur != 0) {
            int k = s->field_24;
            if (k == 0) {
                s->field_14 = flags | 0x4000;
                Ride_SetFlagToNotLetAnyoneOn(s);
            } else {
                s->field_24 = k - 1;
            }
        }
    }
    for (; r != NULL; r = r->next) {
        if (s->tile_id == r->tile.id && r->rider->field_35 == 1) {
            // STRING: LEGOLAND 0x004b4704
            sprintf(SafariManboxName + 6, "%02d", r->rider->field_36 + 1);
            SetBlokePositionFromBNV(SafariRunBNV, r->rider, SafariManboxName, s->frame, -1617787.0f, -1618006.0f, 0);
        }
    }
    *(short *)*ZSafariSprite->lls = (short)s->frame;
    Put3DBlokesOnRide2(SafariRide, node);
}

// FUNCTION: LEGOLAND 0x00415200
void UpdateSafariNodes(void) {
    struct SafariNode *current = SafariNodeList;
    if (current != NULL) {
        while (current != NULL) {
            UpdateSafariNode(current);
            current = current->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x00415220
void SafariRideUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *bloke;
    struct SafariNode *s;
    TileId *tile;
    struct Point sc; /* screen position of the ride */
    int w, h;
    int ox, oy;
    int x, y;
    int coords[3]; /* BNV start position (boarding path) */
    int coords2[3]; /* BNV start position (leaving path) */
    char dir;

    UpdateSafariNodes();
    node = ride->riders;
    while (node != NULL) {
        next = node->next;
        bloke = node->rider;
        tile = &node->tile;
        s = (struct SafariNode *)FindSafariNode((struct SafariKey *)tile);
        if (s == NULL) {
            return;
        }
        x = ride->field_24 + tile->pos.x;
        y = tile->pos.y + ride->field_25;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                s->field_20++;
                s->field_24 = 0xb4;
                bloke->flags |= 8;
                bloke->param_action++;
                bloke->field_58 = 0;
                break;
            case 1:
                sc = GetScreenCoordsForObject(tile, ride);
                oy = bloke->pos.y;
                x = bloke->pos.x;
                GetTileDimensions(&w, &h);
                /* x/y are reused: screen x offset in y, screen y in x. */
                y = (x - oy) * w >> 9;
                x = (x + oy) * h >> 9;
                ox = lpConfig->view_x - (short)Get_XScroll() + y;
                x = x + (lpConfig->view_y - (short)Get_YScroll());
                coords[1] = (x - SafariRideDrawOffset.y / 2 - sc.y) * 2;
                coords[0] = (ox - SafariRideDrawOffset.x / 2 - sc.x) * 2;
                bloke->person->sprite = SafariRiderSprite;
                bloke->person->field_30 = 1;
                bloke->person->depth = GetUnitDepth(-1617787.0f, -1618006.0f);
                node->rider->field_35 = 0;
                // STRING: LEGOLAND 0x004b4704
                sprintf(SafariManboxName + 6, "%02d", (GetSafariRiderIndex((struct RideNode *)node, &node->tile.id) >> 1) + 1);
                node->rider->path = NewBNVPath(SafariPathOnBNV, 1, SafariManboxName, -1617787.0f, -1618006.0f, coords);
                UpdateBlokeFromBNVPath(bloke, node->rider->path);
                BNVPath_SetDFrame(bloke, bloke->path, 0);
                bloke->param_action++;
                break;
            case 2:
                bloke->flags |= 0x80;
                if (UpdateBlokeFromBNVPath(bloke, bloke->path) == 0) {
                    bloke->field_35 = 1;
                    bloke->param_action = 5;
                    free(bloke->path);
                    bloke->path = NULL;
                }
                if (bloke->path != NULL && (int)BNVPath_GetDFrame(bloke->path) >= SafariRideFrameThreshold1[bloke->field_36]) {
                    bloke->field_35 = 1;
                    bloke->param_action = 5;
                    free(bloke->path);
                    bloke->path = NULL;
                }
                BlokeSetFrame(bloke, bloke->frame);
                break;
            case 5:
                bloke->flags |= 0x80;
                BlokeSitAnim(bloke);
                BlokeSetFrame(bloke, 0);
                bloke->field_35 = 1;
                bloke->person->depth = GetUnitDepth(-1617787.0f, -1618006.0f);
                bloke->param_action++;
                s->field_4++;
                if (s->field_4 == ((struct Ride *)SafariRide)->seats) {
                    StartSafariRide((struct SafariNode *)s);
                }
                break;
            case 7:
                bloke->flags |= 0x80;
                coords2[0] = bloke->screen_x * 2;
                coords2[1] = bloke->screen_y * 2;
                BlokeWalkAnim(bloke);
                BlokeSetFrame(bloke, 0);
                bloke->person->sprite = SafariRiderSprite;
                bloke->person->field_30 = 1;
                bloke->person->depth = GetUnitDepth(-1617787.0f, -1618006.0f);
                bloke->field_35 = 2;
                sprintf(SafariManboxName + 6, "%02d", (bloke->field_36 >> 1) + 1);
                node->rider->path = NewBNVPath(SafariPathOffBNV, 2, SafariManboxName, -1617787.0f, -1618006.0f, coords2);
                BNVPath_SetDFrame(bloke, bloke->path, 0);
                UpdateBlokeFromBNVPath(bloke, bloke->path);
                bloke->param_action++;
                break;
            case 8:
                if (UpdateBlokeFromBNVPath(bloke, bloke->path) == 0) {
                    bloke->field_35 = 2;
                    bloke->param_action = 13;
                    free(node->rider->path);
                    bloke->path = NULL;
                }
                if (bloke->path != NULL && (int)BNVPath_GetDFrame(bloke->path) >= SafariRideFrameThreshold2[bloke->field_36]) {
                    bloke->field_35 = 2;
                    bloke->param_action = 13;
                    free(bloke->path);
                    bloke->path = NULL;
                }
                BlokeSetFrame(bloke, bloke->frame);
                break;
            case 13:
                bloke->flags &= ~0x80;
                bloke->person->sprite = NULL;
                bloke->person->field_30 = 0;
                UnAdjustBlokePosition(&bloke->person->screen);
                ScreenToMapRef(&bloke->person->screen.x, &bloke->pos.x, 0);
                bloke->person->field_34 = 0;
                bloke->pos.x <<= 8;
                bloke->pos.y <<= 8;
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 14:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= ~8;
                if (--s->field_8 == 0) {
                    s->field_4 = 0;
                    Ride_ClearFlagToNotLetAnyoneOn(s);
                }
                break;
            }
        }
        node = next;
    }
}

struct SafariSlotData {
    unsigned char pad_0[0x36];
    unsigned char index;
};

// FUNCTION: LEGOLAND 0x00415760
int GetSafariRiderIndex(struct RideNode *node, unsigned short *key) {
    struct RideNode *cur;
    int n = 0;

    for (cur = SafariRide->riders; cur != NULL; cur = cur->next) {
        if (memcmp(&cur->tile, key, 2) == 0) {
            if (cur == node) {
                node->rider->field_36 = (unsigned char)n;
                return n;
            }
            n++;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004157b0
LEGO_EXPORT int SaveSafariRide(struct Element *element) {
    struct SafariNode *current = SafariNodeList;
    unsigned int value1 = 1;
    unsigned int value0 = 0;

    while (current != NULL) {
        if (SaveGameWrite(&value1, 0x4) == 0) {
            return 0;
        }
        if (SaveGameWrite(current, sizeof(struct SafariNode)) == 0) {
            return 0;
        }
        current = current->next;
    }

    if (SaveGameWrite(&value0, 0x4) == 0) {
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00415820
LEGO_EXPORT int LoadSafariRide(struct Element *arg) {
    struct Ride *obj = arg->ride;
    struct SafariNode *prev = NULL;
    struct RideNode *list;
    struct Person *car;
    struct Bloke *data;
    struct BNVRef *tc;
    unsigned int marker;

    if (!SaveGameRead(&marker, 4)) {
        return 0;
    }
    while (marker != 0) {
        struct SafariNode *node = (struct SafariNode *)malloc(sizeof(struct SafariNode));
        if (!SaveGameRead(node, sizeof(struct SafariNode))) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            SafariNodeList = node;
        }
        prev = node;
        if (!SaveGameRead(&marker, 4)) {
            return 0;
        }
    }

    list = obj->riders;
    while (list != NULL) {
        car = list->person;
        if (car->field_30 != 0) {
            car->sprite = SafariOnBNV[car->field_30];
        } else {
            car->sprite = NULL;
            list->person->field_30 = 0;
        }
        data = list->rider;
        tc = data->bnv;
        if (tc != NULL) {
            tc->file = (&SafariPathRunBNV)[tc->index];
        }
        list = list->next;
    }
    return 1;
}
