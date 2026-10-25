#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "gamemap.h"
#include "jungle_cruise.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "money.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"
#include "western_town.h"

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004375d0
void LoadGStoreMatteSpritesAndMoneySFX(struct Element *obj) {
    struct Ride *building = obj->ride;
    GeneralStoreBuilding = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b74a4
    GStoreMatteSprite = LoadSprite("G_Store Matte.LLS", 1);
    // STRING: LEGOLAND 0x004b7490
    GStoreMatte2Sprite = LoadSprite("G_Store Matte2.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437610
void KillGStoreMatteSpritesAndMoneySFX(void) {
    KillSprite(GStoreMatteSprite);
    KillSprite(GStoreMatte2Sprite);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437630
void GeneralStoreSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = GeneralStoreBuilding;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00437670
void RenderGeneralStore(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int param_6) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    char count = 0;
    struct Bloke *blokes[10] = {0};
    char i;
    struct Point pos;

    while (node != NULL) {
        if (*tile == node->tile.id) {
            blokes[count++] = node->rider;
        }
        node = node->next;
    }
    if (count == 0) {
        return;
    }
    pos = GetScreenCoordsForObject((TileId *)tile, ride);
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 4) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 5) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 6) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    PrintSprite(GStoreMatte2Sprite, pos.x, pos.y, param_6, NULL);
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 0) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 1) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 2) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 3) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 7) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 8) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 9) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 10) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 11) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    PrintSprite(GStoreMatteSprite, pos.x, pos.y, param_6, NULL);
}

// FUNCTION: LEGOLAND 0x004378e0
void GeneralStoreUpdate(struct Element *param_1) {
    struct Ride *ride = param_1->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    unsigned char *pos;
    int x;
    int y;
    char move;
    char rr;

    while (node != NULL) {
        next = node->next;
        bloke = node->rider;
        pos = (unsigned char *)&node->tile;
        x = pos[0] + ride->x;
        y = pos[1] + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                bloke->dest.x = x * 0x100 - 0x80;
                y = y * 0x100 - 0x100;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 1:
                bloke->dest.x = x * 0x100 + 0x80;
                y = (y - 2) * 0x100;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 2:
                bloke->dest.x = x * 0x100 + 0x80;
                y = (y - 3) * 0x100;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 3:
                rr = rand() % 2;
                if (rr != 0) {
                    y = y * 0x100 - 0x280;
                    x = x * 0x100 - 0x100;
                    bloke->dest.x = x;
                    bloke->dest.y = y;
                    move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = move + 0x10;
                    NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                    y = rand() % 0x32;
                    bloke->param_action++;
                    bloke->field_58 = y;
                } else {
                    bloke->param_action = 6;
                }
                break;
            case 4:
                y = bloke->field_58 - 1;
                bloke->field_58 = y;
                if (y == 0) {
                    bloke->param_action++;
                    break;
                }
                if (y % 10 == 0) {
                    if ((bloke->dir = bloke->dir + 1) > 8) {
                        bloke->dir = 0;
                    }
                }
                break;
            case 5:
                bloke->dest.x = x * 0x100 + 0x80;
                y = (y - 3) * 0x100;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 6:
                bloke->param_action++;
                break;
            case 7:
                BrowseAndBuyItem((struct RideNode *)node, (Element *)param_1, (TileId *)&node->tile, 1);
                break;
            case 8:
                bloke->dest.x = x * 0x100 + 0x80;
                y = (y - 2) * 0x100;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 9:
                bloke->dest.x = x * 0x100 - 0x80;
                y = y * 0x100 - 0x100;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 10:
                bloke->dest.x = x * 0x100 + 0x80;
                y = y * 0x100 + 0x80;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 11:
                RemoveBlokeFromRide((struct Ride *)ride, (struct RideNode *)node);
                bloke->flags &= 0xfff7;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00437ba0
void LoadSherifshutMatteSpriteAndMoneySFX(struct Element *obj) {
    struct Ride *building = obj->ride;
    SheriffBuilding = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b74b8
    SherifshutMatteSprite = LoadSprite("Sherifshut Matte.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437bd0
void KillSherifshutMatteSpriteAndMoneySFX(void) {
    KillSprite(SherifshutMatteSprite);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437bf0
void SheriffSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = SheriffBuilding;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00437c30
void RenderSheriff(struct Element *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, void *param_5, unsigned int param_6) {
    struct Ride *ride = param_1->ride;
    struct RideNode *elem = ride->riders;
    int count = 0;
    struct Point coords;
    if (elem != NULL) {
        do {
            if (*param_4 == elem->tile.id) {
                RenderBlokeIn3D(elem->rider);
                count++;
            }
            elem = elem->next;
        } while (elem != NULL);
        if (count != 0) {
            coords = GetScreenCoordsForObject((TileId *)param_4, ride);
            PrintSprite(SherifshutMatteSprite, coords.x, coords.y, param_6, NULL);
        }
    }
}

// FUNCTION: LEGOLAND 0x00437c90
void SheriffUpdate(struct Element *param_1) {
    struct Ride *ride = param_1->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    unsigned char *pos;
    int x;
    int y;
    char move;

    while (node != NULL) {
        next = node->next;
        bloke = node->rider;
        pos = (unsigned char *)&node->tile;
        x = pos[0] + ride->x;
        y = pos[1] + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                bloke->dest.x = x * 0x100 - 0x100;
                y = y * 0x100 + 0x80;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 1:
                bloke->dest.x = (x - 3) * 0x100;
                y = y * 0x100 + 0x80;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 2:
                bloke->dest.x = (x - 3) * 0x100;
                move = (char)(rand() % 3);
                if (move != 0) {
                    bloke->dest.y = (move + y) * 0x100;
                } else {
                    bloke->dest.y = y * 0x100 + 0x80;
                }
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                bloke->dir = 8;
                break;
            case 3:
                BrowseAndBuyItem((struct RideNode *)node, (Element *)param_1, (TileId *)&node->tile, 1);
                break;
            case 4:
                bloke->dest.x = (x - 3) * 0x100;
                y = y * 0x100 + 0x80;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 5:
                bloke->dest.x = x * 0x100 - 0x100;
                y = y * 0x100 + 0x80;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 6:
                bloke->dest.x = x * 0x100;
                y = y * 0x100;
                bloke->dest.y = y;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(move + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 7:
                RemoveBlokeFromRide((struct Ride *)ride, (struct RideNode *)node);
                bloke->flags &= 0xfff7;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00437f10
void AddJailCell(unsigned short *param) {
    struct JailCell *cell = malloc(sizeof(struct JailCell));
    if (cell != NULL) {
        memset(cell, 0, sizeof(struct JailCell));
        cell->tile_id = *param;
        cell->frame = 9;
        cell->field_8 = 0;
        cell->field_c = 0;
        cell->field_10 = 0;
        cell->field_14 = 0;
        cell->field_18 = 0;
        cell->next = JailCellList;
        JailCellList = cell;
    }
}

// FUNCTION: LEGOLAND 0x00437f60
void JailCellAddObject(struct Element *editObj, int *coords) {
    unsigned char key[2];
    key[0] = (unsigned char)coords[0];
    key[1] = (unsigned char)coords[1];
    AddBasicObject(editObj, coords);
    AddJailCell((unsigned short *)key);
}

// FUNCTION: LEGOLAND 0x00437f90
struct JailCell *FindJailCell(unsigned short *key) {
    struct JailCell *cell;

    cell = JailCellList;
    if (cell == NULL) {
        return NULL;
    }
    while (memcmp(&cell->tile_id, key, 2) != 0) {
        cell = cell->next;
        if (cell == NULL) {
            return NULL;
        }
    }
    return cell;
}

// FUNCTION: LEGOLAND 0x00437fc0
void FreeJailCell(struct JailCell *cell) {
    struct JailCell *cur;
    struct JailCell *prev;

    if (JailCellList == cell) {
        JailCellList = cell->next;
    } else {
        cur = JailCellList->next;
        prev = JailCellList;
        while (cur != cell) {
            prev = prev->next;
            if (prev == NULL) {
                break;
            }
            cur = prev->next;
        }
        if (prev != NULL) {
            prev->next = cell->next;
        }
    }
    free(cell);
}

// FUNCTION: LEGOLAND 0x00438000
void FreeAllJailCells(void) {
    while (JailCellList != NULL) {
        FreeJailCell(JailCellList);
    }
}

// FUNCTION: LEGOLAND 0x00438020
void JailCellRemoveObject(struct Element *editObj, TileId coords, struct Cursor *cursor) {
    struct JailCell *cell = FindJailCell(&coords.id);
    if (cell != NULL) {
        FreeJailCell(cell);
    }
    StandardRemoveObject(editObj, coords, cursor);
    RemoveAllBlokesFromRide((struct Ride *)editObj->ride, coords);
}

// FUNCTION: LEGOLAND 0x00438070
void JailCellLoad(struct Element *obj) {
    struct Ride *building = obj->ride;
    ActiveWesternTownRide = building;
    building->flags |= 0x420;
    WesternTownLayer = ActiveWesternTownRide->layer;
    ActiveWesternTownRide->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b74d0
    JailCellMaskSprite = LoadSprite("JailCellMask.LLS", 1);
    HideLayer(WesternTownLayer, 1);
    StopLayerPlaying((struct Sprite *)WesternTownLayer, 1);
    LLSSetFrame((struct LLS *)GetLLSForLayer((struct Sprite *)WesternTownLayer, 1), 9);
}

// FUNCTION: LEGOLAND 0x004380f0
void JailCellUnload(void) {
    KillSprite(JailCellMaskSprite);
    FreeAllJailCells();
}

// FUNCTION: LEGOLAND 0x00438110
void JailCellSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = ActiveWesternTownRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00438150
void RenderJailCell(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int param_6) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct Bloke *blokes[10] = {0};
    char count = 0;
    char i;
    struct Point pos;
    struct Point offset;
    struct JailCell *cell;

    pos = GetScreenCoordsForObject((TileId *)tile, ride);
    cell = FindJailCell(tile);
    if (cell == NULL) {
        return;
    }
    while (node != NULL) {
        if (*tile == node->tile.id) {
            blokes[count++] = node->rider;
        }
        node = node->next;
    }
    if (count != 0) {
        for (i = 0; i < count; i++) {
            if (blokes[i]->param_action == 2) {
                IP_RenderBlokeIn3DNow(blokes[i]);
            }
        }
        for (i = 0; i < count; i++) {
            if (blokes[i]->param_action == 3) {
                IP_RenderBlokeIn3DNow(blokes[i]);
            }
        }
        LLSSetFrame((struct LLS *)GetLLSForLayer((struct Sprite *)WesternTownLayer, 1), (char)cell->frame);
        offset = GetRenderOffsetForLayer((struct Sprite *)WesternTownLayer, 1);
        AdjustOffsetForViewMode(&offset);
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct Sprite *)WesternTownLayer, 1), pos.x + offset.x, pos.y + offset.y, param_6, NULL);
        for (i = 0; i < count; i++) {
            if (blokes[i]->param_action == 1) {
                IP_RenderBlokeIn3DNow(blokes[i]);
            }
        }
        for (i = 0; i < count; i++) {
            if (blokes[i]->param_action == 4) {
                IP_RenderBlokeIn3DNow(blokes[i]);
            }
        }
        for (i = 0; i < count; i++) {
            if (blokes[i]->param_action == 8) {
                IP_RenderBlokeIn3DNow(blokes[i]);
            }
        }
        offset = GetRenderOffsetForLayer((struct Sprite *)WesternTownLayer, 0);
        AdjustOffsetForViewMode(&offset);
        PrintSprite(JailCellMaskSprite, pos.x + offset.x, pos.y + offset.y, param_6, NULL);
        for (i = 0; i < count; i++) {
            if (blokes[i]->param_action == 0) {
                IP_RenderBlokeIn3DNow(blokes[i]);
            }
        }
        for (i = 0; i < count; i++) {
            if (blokes[i]->param_action == 7) {
                IP_RenderBlokeIn3DNow(blokes[i]);
            }
        }
        for (i = 0; i < count; i++) {
            if (blokes[i]->param_action == 10) {
                IP_RenderBlokeIn3DNow(blokes[i]);
            }
        }
        return;
    }
    LLSSetFrame((struct LLS *)GetLLSForLayer((struct Sprite *)WesternTownLayer, 1), (char)cell->frame);
    offset = GetRenderOffsetForLayer((struct Sprite *)WesternTownLayer, 1);
    AdjustOffsetForViewMode(&offset);
    PrintSprite((struct Sprite *)GetSpriteForLayer((struct Sprite *)WesternTownLayer, 1), pos.x + offset.x, pos.y + offset.y, param_6, NULL);
}

// FUNCTION: LEGOLAND 0x00438430
void JailCellUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct JailCell *cell;
    struct JailCell *jc;
    struct Bloke *bloke;
    TileId *tile;
    int x;
    int y;
    char dir;
    unsigned char field_6;
    int field_8;
    unsigned int field_c;
    int field_10;
    unsigned int field_14;
    int field_18;

    while (node != NULL) {
        next = node->next;
        bloke = node->rider;
        tile = &node->tile;
        cell = FindJailCell(&tile->id);
        if (cell == NULL) {
            return;
        }
        field_6 = cell->frame;
        field_8 = cell->field_8;
        field_c = cell->field_c;
        field_10 = cell->field_10;
        field_14 = cell->field_14;
        field_18 = cell->field_18;
        x = ride->x + tile->pos.x;
        y = tile->pos.y + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                if (field_8 == 0) {
                    bloke->dest.x = (x - 2) << 8;
                    bloke->dest.y = (y << 8) - 0x100;
                    field_8 = 1;
                } else {
                    bloke->dest.x = (x << 8) - 0x80;
                    bloke->dest.y = y << 8;
                    bloke->dir = 7;
                }
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->field_58 = rand() % 100;
                bloke->param_action++;
                break;
            case 1:
                field_14 = 1;
                bloke->param_action++;
                bloke->dir = 3;
                break;
            case 2:
                if (field_18 != 0) {
                    y = bloke->field_58 - 1;
                    bloke->field_58 = y;
                    if (y == 0) {
                        field_c = 1;
                        field_18 = 0;
                        bloke->param_action++;
                    } else if (y % 10 == 0) {
                        bloke->dir = rand() % 2 ? 4 : 2;
                    }
                }
                break;
            case 3:
                if (field_10 != 0) {
                    bloke->param_action++;
                }
                break;
            case 4:
                y = y * 0x100 - 0x80;
                x = x * 0x100 - 0x100;
                bloke->dest.y = y;
                bloke->dest.x = x;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action = 8;
                field_8 = 0;
                break;
            case 7:
                y = bloke->field_58 - 1;
                bloke->field_58 = y;
                if (y == 0) {
                    bloke->param_action++;
                }
                break;
            case 8:
                bloke->dest.x = x * 0x100 + 0x80;
                y = y * 0x100 + 0x80;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action = 10;
                break;
            case 10:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
            }
        }
        cell->frame = field_6;
        cell->field_8 = field_8;
        cell->field_c = field_c;
        cell->field_10 = field_10;
        cell->field_14 = field_14;
        cell->field_18 = field_18;
        node = next;
    }
    for (jc = JailCellList; jc != NULL; jc = jc->next) {
        field_14 = jc->field_14;
        field_6 = jc->frame;
        field_8 = jc->field_8;
        field_c = jc->field_c;
        field_10 = jc->field_10;
        field_18 = jc->field_18;
        if (field_14 != 0) {
            field_6 = field_6 - 1;
            if (field_6 == 0) {
                field_18 = 1;
                field_14 = 0;
            }
        }
        if (field_c != 0) {
            field_6 = field_6 + 1;
            if (field_6 == 9) {
                field_10 = 1;
                field_c = 0;
            }
        }
        jc->frame = field_6;
        jc->field_8 = field_8;
        jc->field_c = field_c;
        jc->field_10 = field_10;
        jc->field_14 = field_14;
        jc->field_18 = field_18;
    }
}

// FUNCTION: LEGOLAND 0x00438780
LEGO_EXPORT int SaveJailCells(struct Element *element) {
    struct JailCell *cell = JailCellList;
    unsigned int marker = 1;
    unsigned int terminator = 0;

    while (cell != NULL) {
        if (SaveGameWrite(&marker, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(cell, sizeof(struct JailCell)) == 0) {
            return 0;
        }
        cell = cell->next;
    }
    return SaveGameWrite(&terminator, 4) != 0;
}

// FUNCTION: LEGOLAND 0x004387f0
LEGO_EXPORT int LoadJailCells(struct Element *element) {
    unsigned int marker;
    struct JailCell *prev = NULL;

    if (SaveGameRead(&marker, 4) == 0) {
        return 0;
    }
    while (marker != 0) {
        struct JailCell *cell = malloc(sizeof(struct JailCell));
        if (SaveGameRead(cell, sizeof(struct JailCell)) == 0) {
            return 0;
        }
        cell->next = NULL;
        if (prev != NULL) {
            prev->next = cell;
        } else {
            JailCellList = cell;
        }
        prev = cell;
        if (SaveGameRead(&marker, 4) == 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00438870
void LoadBankMatteSpriteAndMoneySFX(struct Element *obj) {
    struct Ride *building = obj->ride;
    BankBuilding = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b74e4
    BankMatteSprite = LoadSprite("Bank Matte.lls", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x004388a0
void KillBankMatteSpriteAndMoneySFX(void) {
    KillSprite(BankMatteSprite);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x004388c0
void BankSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = BankBuilding;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00438900
void RenderBank(struct Element *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, void *param_5, unsigned int param_6) {
    struct Ride *ride = param_1->ride;
    struct RideNode *elem = ride->riders;
    int count = 0;
    struct Point coords;
    if (elem != NULL) {
        do {
            if (*param_4 == elem->tile.id) {
                RenderBlokeIn3D(elem->rider);
                count++;
            }
            elem = elem->next;
        } while (elem != NULL);
        if (count != 0) {
            coords = GetScreenCoordsForObject((TileId *)param_4, ride);
            PrintSprite(BankMatteSprite, coords.x, coords.y, param_6, NULL);
        }
    }
}

// FUNCTION: LEGOLAND 0x00438960
void BankUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    TileId *tile;
    int x;
    int y;
    char dir;
    int n;

    while (node != NULL) {
        next = node->next;
        bloke = node->rider;
        tile = &node->tile;
        x = tile->pos.x + ride->x;
        y = tile->pos.y + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) - 0x100;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 1:
                bloke->dest.x = (x + 1) << 8;
                bloke->dest.y = (y - 2) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 2:
                bloke->dest.x = (tile->pos.x + 2) << 8;
                bloke->dest.y = (tile->pos.y << 8) - 0x100;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                bloke->field_58 = rand() % 8;
                break;
            case 3:
                n = --bloke->field_58;
                if (n <= 0) {
                    bloke->param_action = 5;
                } else {
                    bloke->dest.x = tile->pos.x << 8;
                    bloke->dest.y = (tile->pos.y << 8) - 0x100;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = dir + 0x10;
                    NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                    bloke->param_action++;
                }
                break;
            case 4:
                n = --bloke->field_58;
                if (n <= 0) {
                    bloke->param_action = 5;
                } else {
                    bloke->dest.x = (tile->pos.x + 2) << 8;
                    bloke->dest.y = (tile->pos.y << 8) - 0x100;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = dir + 0x10;
                    NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                    bloke->param_action--;
                }
                break;
            case 5:
                bloke->dest.x = (x + 1) << 8;
                bloke->dest.y = (y - 2) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 6:
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) - 0x100;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 7:
                bloke->dest.x = x << 8;
                bloke->dest.y = y << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 8:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00438c60
void LoadSaloonMatteSpritesAndMoneySFX(struct Element *obj) {
    struct Ride *building = obj->ride;
    SaloonBuilding = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b7508
    SaloonMatte1Sprite = LoadSprite("SaloonMatte1.LLS", 1);
    // STRING: LEGOLAND 0x004b74f4
    SaloonMatte2Sprite = LoadSprite("SaloonMatte2.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00438ca0
void KillSaloonMatteSpritesAndMoneySFX(void) {
    KillSprite(SaloonMatte1Sprite);
    KillSprite(SaloonMatte2Sprite);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00438cc0
void SaloonSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = SaloonBuilding;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00438d00
void RenderSaloon(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int param_6) {
    struct Ride *ride = obj->ride;
    char count = 0;
    struct RideNode *node = ride->riders;
    struct Bloke *blokes[10] = {0};
    char i;
    struct Point pos;

    while (node != NULL) {
        if (*tile == node->tile.id) {
            blokes[count++] = node->rider;
        }
        node = node->next;
    }
    if (count == 0) {
        return;
    }
    pos = GetScreenCoordsForObject((TileId *)tile, ride);
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 4) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 5) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 6) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    PrintSprite(SaloonMatte2Sprite, pos.x, pos.y, param_6, NULL);
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 2) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 3) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 7) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 8) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    PrintSprite(SaloonMatte1Sprite, pos.x, pos.y, param_6, NULL);
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 0) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 1) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 9) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
}

// FUNCTION: LEGOLAND 0x00438f10
void SaloonUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    TileId *tile;
    int x;
    int y;
    char dir;

    while (node != NULL) {
        next = node->next;
        bloke = node->rider;
        tile = &node->tile;
        x = tile->pos.x + ride->x;
        y = tile->pos.y + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                x = x * 0x100 - 0x80;
                y = y * 0x100;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 1:
                bloke->dest.x = (x - 2) * 0x100;
                y = y * 0x100 - 0x100;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 2:
                x = (x - 4) * 0x100;
                y = y * 0x100 - 0x80;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 3:
                if ((char)(rand() % 2) != 0) {
                    x = x * 0x100 - 0x380;
                    y = (y + 1) * 0x100;
                    bloke->dest.x = x;
                    bloke->dest.y = y;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = dir + 0x10;
                    NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                } else {
                    x = x * 0x100 - 0x380;
                    y = (y - 2) * 0x100;
                    bloke->dest.x = x;
                    bloke->dest.y = y;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = dir + 0x10;
                    NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                }
                bloke->dir = 8;
                bloke->param_action++;
                break;
            case 4:
                BrowseAndBuyItem(node, obj, tile, 1);
                break;
            case 5:
                bloke->dest.x = (x - 4) * 0x100;
                y = y * 0x100 - 0x80;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 6:
                y = y * 0x100 - 0x100;
                x = (x - 2) * 0x100;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 7:
                x = x * 0x100 - 0x80;
                bloke->dest.x = x;
                bloke->dest.y = y * 0x100;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 8:
                bloke->dest.x = x * 0x100;
                y = y * 0x100;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (unsigned char)(((unsigned char)(dir + 0x10) >> 5) + 3));
                bloke->param_action++;
                break;
            case 9:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
            }
        }
        node = next;
    }
}
