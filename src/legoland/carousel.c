#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "binv.h"
#include "bloke.h"
#include "carousel.h"
#include "gamemap.h"
#include "image_sprite.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"
#include "sound_music.h"
#include "tilemap.h"

// FUNCTION: LEGOLAND 0x0042bbc0
void AddCarouselNode(unsigned short *param_1) {
    struct CarouselNode *node = (struct CarouselNode *)malloc(sizeof(struct CarouselNode));
    if (node != NULL) {
        unsigned int *fill = (unsigned int *)node;
        int i;
        for (i = 0xb; i != 0; i--) {
            *fill = 0;
            fill++;
        }
        node->id = *param_1;
        node->next = CarouselNodeList;
        CarouselNodeList = node;
        ResetCarouselRide(node);
    }
}

// FUNCTION: LEGOLAND 0x0042bc00
void RemoveCarouselNode(struct CarouselNode *node) {
    struct CarouselNode *cur;
    struct CarouselNode *prev;

    if (CarouselNodeList == node) {
        CarouselNodeList = node->next;
    } else {
        cur = CarouselNodeList->next;
        prev = CarouselNodeList;
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

// FUNCTION: LEGOLAND 0x0042bc40
void FreeAllCarouselNodes(void) {
    while (CarouselNodeList != NULL) {
        RemoveCarouselNode(CarouselNodeList);
    }
}

// FUNCTION: LEGOLAND 0x0042bc60
struct CarouselNode *FindCarouselNode(unsigned short *param_1) {
    struct CarouselNode *node;

    node = CarouselNodeList;
    if (node == NULL) {
        return NULL;
    }
    while (memcmp(&node->id, param_1, sizeof(node->id)) != 0) {
        node = node->next;
        if (node == NULL) {
            return NULL;
        }
    }
    return node;
}

// FUNCTION: LEGOLAND 0x0042bc90
void StartCarouselRide(struct CarouselNode *node) {
    struct SampleSource params;

    params.type = 2;
    node->leaving_count = node->seated_count;
    node->flags = node->flags & 0xffffbfff | 1;
    node->seated_count = 0;
    node->frame_ticks = 0;
    params.x = ((TileId *)&node->id)->pos.x;
    params.y = ((TileId *)&node->id)->pos.y;
    node->frame = 1;
    PlayInstanceOfSample(CAROUSSEL_SFX[0].sample, 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x0042c210
void ResetCarouselRide(struct CarouselNode *node) {
    int r;
    struct SampleSource params;

    node->frame_ticks = 0;
    node->frame = 0;
    r = rand() % 2;
    node->boarding_count = 0;
    node->cycles_left = (char)r + '\x03';
    node->seated_count = 0;
    node->flags = node->flags & 0xffffbffe;
    params.type = 2;
    params.x = ((TileId *)&node->id)->pos.x;
    params.y = ((TileId *)&node->id)->pos.y;
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x0042c280
void LoadCarouselResources(struct Element *param_1) {
    struct LayerResult layer;

    ActiveCarouselRide = param_1->ride;
    Load_FXList(CAROUSSEL_SFX, 2);
    ActiveCarouselRide->flags |= 0x420;
    CarouselLayer = ActiveCarouselRide->layer;
    CarouselLayer->flags |= 0x2000;
    GetLayer(ActiveCarouselRide->layer, &layer, 0);
    CarouselDrawOffsetX = layer.x + -0x58;
    CarouselDrawOffsetY = layer.y + -0xcd;
    // STRING: LEGOLAND 0x004b65a4
    ZCarouselSprite = LoadSprite("z_Carousel.lls", 1);
    CarouselRiderSprite = ZCarouselSprite;
    // STRING: LEGOLAND 0x004b658c
    CarouselOnBinV = LoadBinV("Zbuffers\\CarouselOn.bnv");
    CarouselBinVs[0] = CarouselOnBinV;
    // STRING: LEGOLAND 0x004b6574
    CarouselBinV = LoadBinV("Zbuffers\\Carousel.bnv");
    CarouselBinVs[1] = CarouselBinV;
    // STRING: LEGOLAND 0x004b6558
    CarouselOffBinV = LoadBinV("Zbuffers\\CarouselOff.bnv");
    CarouselBinVs[2] = CarouselOffBinV;
    // STRING: LEGOLAND 0x004b653c
    CarouselEntranceMatteSprite = LoadSprite("Carousel Entrance Matte.lls", 1);
    // STRING: LEGOLAND 0x004b651c
    CarouselEntranceMatte2Sprite = LoadSprite("Carousel Entrance Matte2.lls", 1);
    HideLayer(CarouselLayer, 2);
    StopLayerPlaying(CarouselLayer, 2);
    LLSSetFrame((struct LLS *)GetLLSForLayer(CarouselLayer, 2), 0);
    HideLayer(CarouselLayer, 0);
    StopLayerPlaying(CarouselLayer, 0);
    LLSSetFrame((struct LLS *)GetLLSForLayer(CarouselLayer, 0), 0);
    HideLayer(CarouselLayer, 1);
}

// FUNCTION: LEGOLAND 0x0042c3f0
void UnloadCarouselResources(struct Element *input) {
    ActiveCarouselRide = input->ride;
    KillSprite(CarouselEntranceMatteSprite);
    KillSprite(CarouselEntranceMatte2Sprite);
    KillSprite(CarouselRiderSprite);
    FreeAllCarouselNodes();
    Kill_FXList(CAROUSSEL_SFX, 2);
    FreeBinV(CarouselBinVs[0]);
    FreeBinV(CarouselBinVs[1]);
    FreeBinV(CarouselBinVs[2]);
}

// FUNCTION: LEGOLAND 0x0042c460
void CarouselSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = ActiveCarouselRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x0042c4a0
void CarouselRemoveObject(struct Element *param_1, TileId tile, struct Cursor *param_3) {
    struct CarouselNode *node;
    struct SampleSource params;

    node = FindCarouselNode(&tile.id);
    if (node != NULL) {
        RemoveCarouselNode(node);
    }
    StandardRemoveObject(param_1, tile, param_3);
    RemoveAllBlokesFromRide(param_1->ride, tile);
    params.x = tile.pos.x;
    params.y = tile.pos.y;
    params.type = 2;
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x0042c520
void CarouselAddObject(struct Element *param_1, unsigned char *param_2) {
    unsigned char pair[2];

    pair[0] = param_2[0];
    pair[1] = param_2[4];
    AddBasicObject(param_1, (int *)param_2);
    AddCarouselNode((unsigned short *)pair);
}

// FUNCTION: LEGOLAND 0x0042c550
struct RideSpriteInfo *GetCarouselSpriteInfo(struct Element *param1, unsigned short param2) {
    struct Ride *ride = param1->ride;

    CarouselSpriteInfo.sprite = ride->layer;
    CarouselSpriteInfo.x = ride->field_14;
    CarouselSpriteInfo.y = ride->field_18;
    CarouselSpriteInfo.id = param2;
    ride->layer->flags |= 0x2000;
    return &CarouselSpriteInfo;
}

// FUNCTION: LEGOLAND 0x0042c590
int Carousel_Save(struct Element *element) {
    struct CarouselNode *current = CarouselNodeList;
    unsigned int flag = 1;
    unsigned int terminator = 0;

    while (current != NULL) {
        if (!SaveGameWrite(&flag, 4)) {
            return 0;
        }
        if (!SaveGameWrite(current, sizeof(struct CarouselNode))) {
            return 0;
        }
        current = current->next;
    }

    if (SaveGameWrite(&terminator, 4)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0042c600
int Carousel_Load(struct Element *param_1) {
    struct Ride *ride = param_1->ride;
    struct CarouselNode *node;
    struct CarouselNode *prev;
    struct RideNode *elem;
    unsigned int marker;

    prev = NULL;
    if (SaveGameRead(&marker, 4) == 0) {
        return 0;
    }
    while (marker != 0) {
        node = (struct CarouselNode *)malloc(sizeof(struct CarouselNode));
        if (SaveGameRead(node, sizeof(struct CarouselNode)) == 0) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            CarouselNodeList = node;
        }
        prev = node;
        if (SaveGameRead(&marker, 4) == 0) {
            return 0;
        }
    }
    for (elem = ride->riders; elem != NULL; elem = elem->next) {
        struct Person *comp = elem->person;
        if (comp->field_30 != 0) {
            comp->sprite = ((struct Sprite **)&ActiveCarouselRide)[comp->field_30];
        } else {
            comp->sprite = NULL;
            elem->person->field_30 = 0;
        }
        {
            struct BNVRef *h = elem->rider->bnv;
            if (h != NULL) {
                h->file = ((struct BinVFile **)CarouselBinVs)[h->index];
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0042c6d0
void UpdateCarouselNode(struct CarouselNode *node) {
    struct RideNode *elem = ActiveCarouselRide->riders;
    unsigned int flags = node->flags;

    if ((flags & 1) != 0) {
        int v;
        unsigned char f10;
        v = node->frame_ticks + 1;
        node->frame_ticks = v;
        f10 = node->cycles_left;
        v = node->frame_ticks;
        if (f10 == 0) {
            if (GetAllBlokesOffRide(ActiveCarouselRide, node->id) == 0) {
                return;
            }
            ResetCarouselRide(node);
            return;
        }
        if (2 <= v) {
            char cVar4;
            node->frame_ticks = 0;
            cVar4 = ++node->frame;
            if (cVar4 >= '@') {
                node->frame = 0;
                node->cycles_left = f10 - 1;
            }
        }
    } else if ((flags & 0x4000) != 0) {
        if ((char)node->seated_count == (char)node->boarding_count) {
            node->flags = flags & 0xffffbfff;
            StartCarouselRide(node);
            return;
        }
    } else if (node->seated_count != 0) {
        if (node->boarding_timer == 0) {
            node->flags = flags | 0x4000;
            Ride_SetFlagToNotLetAnyoneOn((unsigned char *)&node->id);
        } else {
            node->boarding_timer = node->boarding_timer - 1;
        }
    }
    for (; elem != NULL; elem = elem->next) {
        if (node->id == elem->tile.id && elem->rider->field_35 == 1) {
            // STRING: LEGOLAND 0x004b4704
            sprintf(CarouselBlokeBoxName + 8, "%02d", elem->rider->field_36);
            SetBlokePositionFromBNV(CarouselBinV, elem->rider, CarouselBlokeBoxName, (int)(char)node->frame, -1617853.25f, -1618109.0f, 0);
        }
    }
    (*ZCarouselSprite->lls)->frame = (short)(char)node->frame;
}

// FUNCTION: LEGOLAND 0x0042c800
void UpdateCarouselNodes(void) {
    struct CarouselNode *current = CarouselNodeList;

    while (current != NULL) {
        UpdateCarouselNode(current);
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0042c820
void CarouselUpdate(struct Element *param_1) {
    struct Ride *ride = param_1->ride;
    struct RideNode *elem = ride->riders;
    struct RideNode *next;
    struct CarouselNode *node;
    struct Bloke *bloke;
    TileId *tile;
    int x, y;
    int tile_w, tile_h;
    char dir;
    struct Point coords;
    struct Point screen;
    int in_path[3];
    int out_path[3];

    while (elem != NULL) {
        next = elem->next;
        bloke = elem->rider;
        tile = &elem->tile;
        node = FindCarouselNode(&tile->id);
        if (node == NULL) {
            break;
        }
        x = ride->x + tile->pos.x;
        y = tile->pos.y + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                /* Walk up to the boarding point. */
                node->boarding_count++;
                y = (y << 8) + 0x80;
                x = (x - 3) << 8;
                node->boarding_timer = 0xb4;
                bloke->flags |= 8;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->field_58 = 0;
                bloke->param_action++;
                break;
            case 1:
                /* Climb on: path from the bloke's screen position to the seat. */
                coords = GetScreenCoordsForObject(tile, (struct Ride *)ride);
                /* x/y are reused: map position in, then screen x (in y) and screen y (in x). */
                y = bloke->pos.y;
                x = bloke->pos.x;
                GetTileDimensions(&tile_w, &tile_h);
                screen.x = (x - y) * tile_w >> 9;
                screen.y = (x + y) * tile_h >> 9;
                y = lpConfig->view_x - (short)Get_XScroll() + screen.x;
                x = screen.y + (lpConfig->view_y - (short)Get_YScroll());
                in_path[0] = (y - CarouselDrawOffsetX / 2 - coords.x) * 2;
                in_path[1] = (x - CarouselDrawOffsetY / 2 - coords.y) * 2;
                bloke->person->sprite = CarouselRiderSprite;
                bloke->person->field_30 = 1;
                bloke->person->depth = GetUnitDepth(-1617853.25f, -1618109.0f);
                bloke->field_35 = 0;
                sprintf(CarouselBlokeBoxName + 8, "%02d", TakeRandomCarouselSeat(elem, node, (signed char)ActiveCarouselRide->seats));
                bloke->path = NewBNVPath(CarouselBinVs[0], 0, CarouselBlokeBoxName, -1617853.25f, -1618109.0f, in_path);
                UpdateBlokeFromBNVPath(bloke, bloke->path);
                bloke->param_action++;
                bloke->flags |= 0x80;
                break;
            case 2:
                if (UpdateBlokeFromBNVPath(bloke, bloke->path) == 0) {
                    bloke->field_35 = 1;
                    bloke->param_action = 5;
                    free(bloke->path);
                    bloke->path = NULL;
                }
                BlokeSetFrame(bloke, bloke->frame);
                break;
            case 5:
                /* Seated. */
                bloke->flags |= 0x80;
                BlokeSetFrame(bloke, 0);
                bloke->field_35 = 1;
                bloke->person->sprite = CarouselRiderSprite;
                bloke->person->field_30 = 1;
                bloke->person->depth = GetUnitDepth(-1617853.25f, -1618109.0f);
                bloke->param_action++;
                if ((short)(char)++node->seated_count == (short)ActiveCarouselRide->seats) {
                    StartCarouselRide(node);
                }
                break;
            case 7:
                /* Climb off. */
                out_path[0] = bloke->screen_x * 2;
                out_path[1] = bloke->screen_y * 2;
                BlokeWalkAnim(bloke);
                BlokeSetFrame(bloke, 0);
                bloke->flags |= 0x80;
                bloke->person->sprite = CarouselRiderSprite;
                bloke->person->field_30 = 1;
                bloke->person->depth = GetUnitDepth(-1617853.25f, -1618109.0f);
                bloke->field_35 = 2;
                sprintf(CarouselBlokeBoxName + 8, "%02d", bloke->field_36);
                bloke->path = NewBNVPath(CarouselBinVs[2], 2, CarouselBlokeBoxName, -1617853.25f, -1618109.0f, out_path);
                bloke->param_action++;
                break;
            case 8:
                if (UpdateBlokeFromBNVPath(bloke, bloke->path) == 0) {
                    bloke->field_35 = 2;
                    bloke->param_action = 0xd;
                    free(bloke->path);
                    bloke->path = NULL;
                    bloke->dir = 3;
                }
                BlokeSetFrame(bloke, bloke->frame);
                break;
            case 0xd:
                /* Back on the ground: free the seat and walk out. */
                node->slots[bloke->field_36 - 1] = 0;
                bloke->flags &= ~0x80;
                bloke->person->sprite = NULL;
                bloke->person->field_30 = 0;
                UnAdjustBlokePosition(&bloke->person->screen);
                ScreenToMapRef(&bloke->person->screen.x, &bloke->pos.x, 0);
                bloke->person->field_34 = 0;
                y = (y << 8) + 0x80;
                bloke->pos.x <<= 8;
                bloke->pos.y <<= 8;
                x = (x << 8) + 0x80;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 0xe:
                RemoveBlokeFromRide((struct Ride *)ride, (struct RideNode *)elem);
                bloke->flags &= ~8;
                if (--node->leaving_count == 0) {
                    node->seated_count = 0;
                    Ride_ClearFlagToNotLetAnyoneOn(&node->id);
                }
                break;
            }
        }
        elem = next;
    }
    UpdateCarouselNodes();
}

// FUNCTION: LEGOLAND 0x0042bcf0
void RenderCarousel(struct Element *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, void *param_5, unsigned int param_6) {
    struct Ride *ride = param_1->ride;
    struct RideNode *elem = ride->riders;
    struct RideNode *e;
    struct CarouselNode *node;
    struct Bloke *bloke;
    struct Person *person;
    char count = 0;
    struct Bloke *riders[10] = {0};
    char i;
    int sx, sy;
    struct HoverInfo hit;
    struct LayerResult layerres;
    struct Point screen;
    struct Point offset;
    struct Point offset2;

    hit.type = 0x103;
    hit.ptr = (struct Bloke *)param_1;
    hit.data.tile.id = *param_4;
    node = FindCarouselNode(param_4);
    if (node == NULL) {
        return;
    }
    screen = GetScreenCoordsForObject((TileId *)param_4, (struct Ride *)ride);
    sx = screen.x;
    sy = screen.y;
    GetLayer(ride->layer, &layerres, 0);
    layerres.field_10 = 0;

    /* Collect the blokes riding this carousel. */
    for (; elem != NULL; elem = elem->next) {
        if (*param_4 == elem->tile.id) {
            riders[count++] = elem->rider;
        }
    }

    if (count != 0) {
        LLSSetFrame(GetLLSForLayer(CarouselLayer, 0), (char)node->frame);
        offset = GetRenderOffsetForLayer(CarouselLayer, 0);
        AdjustOffsetForViewMode(&offset);
        PrintSprite(GetSpriteForLayer(CarouselLayer, 0), offset.x + sx, offset.y + sy, param_6, (int *)&hit);
        offset2 = GetRenderOffsetForLayer(CarouselLayer, 1);
        AdjustOffsetForViewMode(&offset2);
        PrintSprite(CarouselEntranceMatte2Sprite, offset2.x + sx, offset2.y + sy, param_6, (int *)&hit);
        LLSSetFrame(GetLLSForLayer(CarouselLayer, 2), (char)node->frame);
        offset = GetRenderOffsetForLayer(CarouselLayer, 2);
        AdjustOffsetForViewMode(&offset);
        PrintSprite(GetSpriteForLayer(CarouselLayer, 2), offset.x + sx, offset.y + sy, param_6, (int *)&hit);

        /* Draw the riders back to front by their action. */
        for (i = 0; i < count; i++) {
            if (riders[i]->param_action == 0) {
                IP_RenderBlokeIn3DNow(riders[i]);
            }
        }
        for (i = 0; i < count; i++) {
            if (riders[i]->param_action == 1) {
                IP_RenderBlokeIn3DNow(riders[i]);
            }
        }
        for (i = 0; i < count; i++) {
            if (riders[i]->param_action == 13) {
                IP_RenderBlokeIn3DNow(riders[i]);
            }
        }
        for (i = 0; i < count; i++) {
            if (riders[i]->param_action == 14) {
                IP_RenderBlokeIn3DNow(riders[i]);
            }
        }

        ZCarouselSprite->lls[0]->frame = (char)node->frame;
        for (e = ride->riders; e != NULL; e = e->next) {
            if (*param_4 == e->tile.id) {
                bloke = e->rider;
                if (bloke->flags & 0x80) {
                    struct Point view;

                    view.x = CarouselDrawOffsetX;
                    view.y = CarouselDrawOffsetY;
                    person = bloke->person;
                    person->offset.x = bloke->screen_x;
                    person->offset.y = bloke->screen_y;
                    AdjustBlokePosition(&person->offset);
                    AdjustOffsetForViewMode(&view);
                    person->screen.x = bloke->screen_x + view.x + sx;
                    person->screen.y = bloke->screen_y + view.y + sy;
                    AdjustBlokePosition(&person->screen);
                    IP_RenderBlokeIn3DNow(e->rider);
                }
            }
        }
        offset2 = GetRenderOffsetForLayer(CarouselLayer, 1);
        AdjustOffsetForViewMode(&offset2);
        PrintSprite(CarouselEntranceMatteSprite, offset2.x + sx, offset2.y + sy, param_6, 0);
    } else {
        LLSSetFrame(GetLLSForLayer(CarouselLayer, 0), (char)node->frame);
        offset = GetRenderOffsetForLayer(CarouselLayer, 0);
        AdjustOffsetForViewMode(&offset);
        PrintSprite(GetSpriteForLayer(CarouselLayer, 0), offset.x + sx, offset.y + sy, param_6, (int *)&hit);
        offset = GetRenderOffsetForLayer(CarouselLayer, 1);
        AdjustOffsetForViewMode(&offset);
        PrintSprite(GetSpriteForLayer(CarouselLayer, 1), offset.x + sx, offset.y + sy, param_6, (int *)&hit);
        LLSSetFrame(GetLLSForLayer(CarouselLayer, 2), (char)node->frame);
        offset = GetRenderOffsetForLayer(CarouselLayer, 2);
        AdjustOffsetForViewMode(&offset);
        PrintSprite(GetSpriteForLayer(CarouselLayer, 2), offset.x + sx, offset.y + sy, param_6, (int *)&hit);
    }
}

// FUNCTION: LEGOLAND 0x0042cd20
int TakeRandomCarouselSeat(struct RideNode *elem, struct CarouselNode *node, signed char divisor) {
    int count = divisor;
    int eax = rand();
    int index = eax % count;
    unsigned char *slots = (unsigned char *)node;

    while (slots[0x20 + index] != 0) {
        index++;
        if (index >= count) {
            index = 0;
        }
    }

    slots[0x20 + index] = 1;
    elem->rider->field_36 = (unsigned char)(index + 1);
    return index + 1;
}
