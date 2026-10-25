#include "legoland.h"

#include <string.h>

#include "bricks.h"
#include "controller.h"
#include "draw.h"
#include "gamemain.h"
#include "gamemap.h"
#include "globals.h"
#include "interface.h"
#include "llidb.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "popupinfo.h"
#include "ride_queue.h"
#include "sound_music.h"
#include "tilemap.h"
#include "worker.h"

/* An FXSpriteList as seen by the map hover code: its first tile and the per-tile flag words. */
struct TileSetView {
    int first_tile;
    unsigned char pad_4[0x8];
    int *sprite_ids;
};

// FUNCTION: LEGOLAND 0x00457870
void SetBricksLimited(int param_1) {
    UnlimitedBricks = (param_1 == 0);
}

// FUNCTION: LEGOLAND 0x00457890
int AreBricksLimited(void) {
    return UnlimitedBricks == 0;
}

// FUNCTION: LEGOLAND 0x004578a0
LEGO_EXPORT void AddBricks(unsigned int param_1) {
    if (UnlimitedBricks == 0) {
        BrickCount += param_1;
    }
}

// FUNCTION: LEGOLAND 0x004578c0
LEGO_EXPORT void UseBricks(unsigned int param_1) {
    if (UnlimitedBricks == 0) {
        BrickCount -= param_1;
    }
}

// FUNCTION: LEGOLAND 0x004578e0
LEGO_EXPORT int GetBrickCount(void) {
    if (UnlimitedBricks != 0) {
        return 0x7fffffff;
    }
    return BrickCount;
}

// FUNCTION: LEGOLAND 0x00457900
void SetBrickCount(unsigned int param_1) {
    BrickCount = param_1;
}

// FUNCTION: LEGOLAND 0x00457910
int SaveCurrency(void) {
    if (SaveGameWrite(&UnlimitedBricks, 4) == 0) {
        return 0;
    }
    return SaveGameWrite(&BrickCount, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00457940
int LoadCurrency(void) {
    if (SaveGameRead(&UnlimitedBricks, 4) == 0) {
        return 0;
    }
    return SaveGameRead(&BrickCount, 4) != 0;
}

// Can the object being placed (EditMode.unk8) go at offset (dx, dy)? Every tile under its footprint
// must be on the map, carry none of the blocking flags 0x8f8, and if it holds an object
// (field_12), the object being placed must be the PATH CONTROL element. Returns 1 if so, else 0.
// FUNCTION: LEGOLAND 0x00457970
int IsFootprintClear(struct Point off) {
    struct Ride *ride = EditMode.unk8;
    int x, y;
    struct MapElement *elem;

    off.x += ride->footprint.x0;
    off.y += ride->footprint.y0;
    for (y = off.y; y < off.y + FootprintHeight; y++) {
        for (x = off.x; x < off.x + FootprintWidth; x++) {
            if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                elem = &GameMap[y][x];
            } else {
                elem = NULL;
            }
            if (elem == NULL) return 0;
            if (elem->flags & 0x8f8) return 0;
            if (elem->field_12 != 0) {
                struct Element *idx;
                LLIDB_FindElement(PathControlName, &idx, 0);
                if (EditMode.unk8->element != (struct Element *)idx) return 0;
            }
        }
    }
    return 1;
}

/* Processes map query, demolition, and construction modes: resolves the hovered object/work order, updates cursors,
 * handles individual or rectangular edits, and opens object information on an unconsumed query click. */
// FUNCTION: LEGOLAND 0x00457a70
void ProcessMapEditing(void) {
    struct Element *roads;
    struct Element *zebra;
    struct Point pt;
    struct MapElement *tile;
    unsigned int v;
    struct Element *elem;
    unsigned short id;
    struct Ride *cls;
    int x;
    int y;

    // STRING: LEGOLAND 0x004b89cc
    roads = ElemID("DRIVING SCHOOL ROADS");
    // STRING: LEGOLAND 0x004b89bc
    zebra = ElemID("ZEBRA CROSSING");
    if (EditMode.unk0 == 0 || EditMode.unk0 == 2) {
        QueryClass = NULL;
        if (Hover.type == 0x100 || Hover.type == 0x103) {
            if ((int)MouseTileX >= 0 && (int)MouseTileX < lpConfig->width && (int)MouseTileY >= 0 &&
                (int)MouseTileY < lpConfig->height) {
                tile = &GameMap[MouseTileY][MouseTileX];
            } else {
                tile = NULL;
            }
            if (tile) {
                if (tile->flags & 0x888) {
                    if (tile->flags & 0x800) {
                        if ((Hover.ptr = (struct Bloke *)GetGardenerWorkOrderAt(MouseTileX, MouseTileY)) != 0) {
                            Hover.type = 0x10b;
                        } else if ((Hover.ptr = (struct Bloke *)GetMechanicWorkOrderAt(MouseTileX, MouseTileY)) != 0) {
                            Hover.type = 0x10c;
                        }
                        id = tile->anchor.id;
                        Hover.data.tile.id = id;
                        elem = tile->field_0;
                        if (elem) {
                            QueryClass = (struct Ride *)elem->ride;
                            QueryObj.id = id;
                        }
                    } else {
                        Hover.ptr = (struct Bloke *)tile->field_0;
                        Hover.data.tile.id = tile->anchor.id;
                        if (tile->flags & 0x88) {
                            Hover.type = 0x103;
                            if (tile->field_0 == roads) {
                                struct RideQueueEntry *entry;
                                if ((entry = FindQueueEntryAtTile(tile->field_4, tile->field_5)) != 0 && (entry->field_14 & 0x10)) {
                                    Hover.ptr = (struct Bloke *)zebra;
                                }
                            }
                        }
                        elem = (struct Element *)Hover.ptr;
                        QueryClass = (struct Ride *)elem->ride;
                        QueryObj.id = Hover.data.tile.id;
                    }
                } else if (Hover.type == 0x103) {
                    elem = (struct Element *)Hover.ptr;
                    QueryClass = (struct Ride *)elem->ride;
                    QueryObj.id = Hover.data.tile.id;
                } else {
                    struct TileSetView *src;
                    if ((src = (struct TileSetView *)TileSpriteInfo[tile->field_8].src) != 0 && (src->sprite_ids[tile->field_8 - src->first_tile] & 0x10)) {
                        Hover.type = 0x10d;
                    } else {
                        Hover.type = 0x109;
                    }
                }
            } else if (Hover.type == 0x103 && EditMode.unk0 == 2) {
                elem = (struct Element *)Hover.ptr;
                QueryClass = (struct Ride *)elem->ride;
                QueryObj.id = Hover.data.tile.id;
            } else {
                Hover.type = 0x10a;
            }
        }
    }
    cls = QueryClass;
    switch (EditMode.unk0) {
    case 2:
        v = Hover.data.value & 0xffff;
        pt.x = v & 0xff;
        pt.y = v >> 8;
        QueryCursorKeepPath = 0;
        if (DragRemoveClass != NULL && (DragRemoveClass == PathControlObject || DragRemoveClass == HedgeObjectClass)) {
            GamePad |= 0x800;
        } else {
            GamePad &= ~0x800;
        }
        if (!(GamePad & 0x1000)) {
            if (Hover.type == 0x103) {
                QueryClass->cb_94(QueryClass->element, &pt);
            } else if (Hover.type == 0x10c) {
                cls->cb_94(cls->element, &pt);
            } else if (Hover.type == 0x10b) {
                cls->cb_94(cls->element, &pt);
            } else {
                memset(QueryCursor.field_1414, 0, 20);
                QueryCursor.field_1828 = 8;
                SetCursorBlocked(&QueryCursor, 1);
                QueryCursor.tile_x = MouseTileX;
                QueryCursor.tile_y = MouseTileY;
                UnusedMapEditFlag = 0;
                GamePad &= ~0x400;
                if (!(MouseClickFlags & 2)) {
                    DragRemoveClass = NULL;
                }
            }
        } else {
            UpdateEditSelection();
            QueryCursor = EditCursor;
        }
        if (QueryCursorKeepPath == 0) {
            QueryCursor.field_1830 = 0;
        }
        BuildCursorPtr(&QueryCursor, 0, 0);
        RenderCursor(&QueryCursor);
        if (IsCursorBuildable(&QueryCursor)) {
            SetPointer(2);
        } else {
            SetPointer(1);
        }
        if (MouseClickFlags & 0x11) {
            if (DragRemoveClass != NULL && (((struct Ride *)DragRemoveClass)->flags & 0x2000000)) {
                MapBatchUpdate = 1;
                MapBatchChanged = 0;
                for (y = SelectionRect.y0; y <= SelectionRect.y1; y += DragFootprintHeight) {
                    for (x = SelectionRect.x0; x <= SelectionRect.x1; x += DragFootprintWidth) {
                        QueryCursor.field_1414[4] = 0;
                        QueryCursor.tile_x = x;
                        QueryCursor.tile_y = y;
                        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                            tile = &GameMap[y][x];
                        } else {
                            tile = NULL;
                        }
                        if (tile != NULL) {
                            QueryObj.pos.x = QueryCursor.tile_x = tile->field_4;
                            QueryObj.pos.y = QueryCursor.tile_y = tile->field_5;
                            if ((tile->flags & 0x88) && !(tile->flags & 0x40) &&
                                tile->field_0 == (struct Element *)((struct Ride *)DragRemoveClass)->element) {
                                RemObjFromMap((struct Ride *)DragRemoveClass, ((struct Ride *)DragRemoveClass)->element, QueryObj,
                                    &QueryCursor);
                            }
                        }
                    }
                }
                if (MapBatchChanged != 0) {
                    PlayInstanceOfSample(GameFX[FX_PUNCH].sample, 0, 1, 0);
                    CalculateMapRenderOrder();
                }
                MapBatchUpdate = 0;
            } else if (IsCursorBuildable(&QueryCursor)) {
                if (Hover.type == 0x10c) {
                    ClearObjectEntrances((struct Element *)((WorkOrder *)Hover.ptr)->element, &((WorkOrder *)Hover.ptr)->pos.x);
                    ClearPathUnderFootprint((struct Ride *)((WorkOrder *)Hover.ptr)->element->ride, &((WorkOrder *)Hover.ptr)->pos.x);
                    EraseMechanicOrder((WorkOrder *)Hover.ptr);
                } else if (Hover.type == 0x10b) {
                    ClearPathUnderFootprint((struct Ride *)((WorkOrder *)Hover.ptr)->element->ride, &((WorkOrder *)Hover.ptr)->pos.x);
                    EraseGardenerOrder((WorkOrder *)Hover.ptr);
                } else {
                    pt.x = *(unsigned int *)&QueryObj & 0xff;
                    pt.y = QueryObj.pos.y;
                    if (pt.x >= 0 && pt.x < lpConfig->width && pt.y >= 0 && pt.y < lpConfig->height) {
                        tile = &GameMap[pt.y][pt.x];
                    } else {
                        tile = NULL;
                    }
                    QueryClass = (struct Ride *)tile->field_0->ride;
                    if (QueryCursorKeepPath == 0) {
                        ClearPathUnderFootprint((struct Ride *)QueryClass, &pt.x);
                    }
                    RemObjFromMap(QueryClass, QueryClass->element, QueryObj, &QueryCursor);
                }
            }
        }
        if (!(GamePad & 0x1000) && Hover.type == 0x103) {
            if (QueryClass->flags & 0x2000000) {
                GamePad |= 0x400;
                DragRemoveClass = QueryClass;
            } else {
                GamePad &= ~0x400;
                DragRemoveClass = NULL;
            }
        }
        if (RightClickFlags & 2) {
            EditMode.unk0 = 0;
            GamePad &= ~0x1400;
        }
        break;
    case 1:
        if (IsCursorBuildable(&EditCursor)) {
            SetPointer(4);
        } else {
            SetPointer(3);
        }
        if (EditMode.unk8 == PathControlObject || EditMode.unk8 == HedgeObjectClass) {
            GamePad |= 0x800;
        } else {
            GamePad &= ~0x800;
        }
        if (!(GamePad & 0x400) && EditMode.unk8 != NULL) {
            EditMode.unk8->cb_90(EditMode.unk8->element, &MousePos, 0x8f8);
        }
        BuildCursorPtr(&EditCursor, 0x8f8, IsObjectTypeNot0Or2(EditMode.unk8));
        if (MouseClickFlags & 0x11) {
            if (IsCursorBuildable(&EditCursor)) {
                if (EditMode.unk8->flags & 0x2000000) {
                    MapBatchUpdate = 1;
                    MapBatchChanged = 0;
                    if (EditMode.unk8 == PathControlObject) {
                        for (y = SelectionRect.y0; y <= SelectionRect.y1; y += FootprintHeight) {
                            for (x = SelectionRect.x0; x <= SelectionRect.x1; x += FootprintWidth) {
                                pt.x = x - EditMode.unk8->footprint.x0;
                                pt.y = y - EditMode.unk8->footprint.y0;
                                if (pt.x >= 0 && pt.x < lpConfig->width && pt.y >= 0 && pt.y < lpConfig->height) {
                                    tile = &GameMap[pt.y][pt.x];
                                } else {
                                    tile = NULL;
                                }
                                if (tile != NULL && (tile->flags & 0x8a0) && (tile->field_0->ride->flags & 0x200000)) {
                                    RemoveObjectAtPoint(&pt);
                                }
                            }
                        }
                    }
                    for (y = SelectionRect.y0; y <= SelectionRect.y1; y += FootprintHeight) {
                        for (x = SelectionRect.x0; x <= SelectionRect.x1; x += FootprintWidth) {
                            pt.x = x - EditMode.unk8->footprint.x0;
                            pt.y = y - EditMode.unk8->footprint.y0;
                            if (IsFootprintClear(pt)) {
                                if (WorkOrderBuildObject(EditMode.unk8->element, &pt)) {
                                    ScriptDirtyCategories |= 0x10;
                                }
                                PathUpdateNeeded = 1;
                            }
                        }
                    }
                    MapBatchUpdate = 0;
                    if (MapBatchChanged != 0) {
                        PlayAppropriateBuildEffect((struct Ride *)EditMode.unk8, NULL);
                        PlayInstanceOfSample(GameFX[FX_PUNCH].sample, 0, 1, 0);
                        CalculateMapRenderOrder();
                    }
                    MapBatchUpdate = 0;
                } else if (WorkOrderBuildObject(EditMode.unk8->element, (Point *)&EditCursor.tile_x)) {
                    SelectMatchingBuildElement();
                    ScriptDirtyCategories |= 2;
                }
            } else {
                PlayCantBuildAdvice(EditCursor.field_1410);
            }
        } else {
            RenderCursor(&EditCursor);
        }
        if (RightClickFlags & 2) {
            EditMode.unk0 = 0;
            GamePad &= ~0x1400;
        }
        break;
    case 0:
        v = Hover.data.value & 0xffff;
        pt.x = v & 0xff;
        pt.y = v >> 8;
        if (Hover.type == 0x103) {
            QueryClass->cb_94(QueryClass->element, &pt);
        } else if (Hover.type == 0x10c) {
            cls->cb_94(cls->element, &pt);
        } else if (Hover.type == 0x10b) {
            cls->cb_94(cls->element, &pt);
        } else {
            memset(QueryCursor.field_1414, 0, 20);
            QueryCursor.field_1828 = 8;
            SetCursorBlocked(&QueryCursor, 1);
            QueryCursor.tile_x = MouseTileX;
            QueryCursor.tile_y = MouseTileY;
            UnusedMapEditFlag = 0;
            GamePad &= ~0x400;
            if (!(MouseClickFlags & 2)) {
                DragRemoveClass = NULL;
            }
            break;
        }
        QueryCursor.field_1830 = 0;
        BuildCursorPtr(&QueryCursor, 0, 0);
        RenderCursor(&QueryCursor);
        break;
    }
    if ((MapLeftClickFlags & 2) && MouseClickConsumed == 0 && EditMode.unk0 == 0 && WorkerOnMouseActive == 0) {
        PopUpInfoSetUp(Hover, MousePos.x, MousePos.y);
        MouseClickConsumed = 1;
    }
}
