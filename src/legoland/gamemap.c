#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "controller.h"
#include "gamemap.h"
#include "llidb.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "objclass.h"
#include "pathfind.h"
#include "sound_music.h"
#include "tilemap.h"
#include "timer.h"
#include "worker.h"

struct PowerEntry {
    const char *name;
    int value;
};

struct MapCellObjKind {
    /* 0x00 */ unsigned char pad_0[0x20];
    /* 0x20 */ short field_20;
    /* 0x22 */ unsigned char pad_22[0x44 - 0x22];
    /* 0x44 */ int footprint_x1;
    /* 0x48 */ char footprint_y1;
};

struct MapCellObj {
    unsigned char pad_0[0xc];
    struct MapCellObjKind *ride;
};

struct MapCell {
    /* 0x00 */ struct MapCellObj *obj;
    /* 0x04 */ union {
        unsigned short coords;
        struct {
            unsigned char byte_4;
            unsigned char byte_5;
        } b;
    } src;
    /* 0x06 */ unsigned short field_6;
    /* 0x08 */ unsigned short word_8;
    /* 0x0a */ unsigned short word_a;
    /* 0x0c */ union {
        unsigned short word;
        unsigned char bytes[2];
    } flags;
    /* 0x0e */ unsigned char pad_e[2];
    /* 0x10 */ unsigned char byte_10;
    /* 0x11 */ unsigned char pad_11[3];
};

struct RectListNode {
    /* 0x00 */ struct RectListNode *next;
    /* 0x04 */ unsigned char pad_4[4];
    /* 0x08 */ int x0;
    /* 0x0c */ int y0;
    /* 0x10 */ int x1;
    /* 0x14 */ int y1;
};

struct RemBlock {
    /* 0x00 */ int f0;
    /* 0x04 */ int f4;
    /* 0x08 */ int f8;
    /* 0x0c */ int fc;
};

// FUNCTION: LEGOLAND 0x00459850
LEGO_EXPORT void InitGameMap(void) {
    // STRING: LEGOLAND 0x004b5c0c
    CastleObjElem = ElemID("CASTLE OBJ");
    Load_FXList(GameFX, FX_COUNT);
}

// FUNCTION: LEGOLAND 0x00459870
LEGO_EXPORT void KillGameMap(void) {
    Kill_FXList(GameFX, FX_COUNT);
}

// FUNCTION: LEGOLAND 0x00459880
void ResetCoverageCounters(void) {
    TotalCoveredSquares = 0;
    RideCoveredSquares = 0;
    ShopCoveredSquares = 0;
    FoodCoveredSquares = 0;
    Type3CoveredSquares = 0;
    PathSquareCount = 0;
    SceneryCoveredSquares = 0;
    UnusedCoverageCounter = 0;
}

// FUNCTION: LEGOLAND 0x004598d0
void CountPathEdgeScenery(struct Point *coord, int *param_2, int *param_3) {
    struct MapCell *cell;
    short kind;

    if (coord->x < 0 || coord->x >= (int)lpConfig->width || coord->y < 0 || coord->y >= (int)lpConfig->height) {
        cell = NULL;
    } else {
        cell = (struct MapCell *)&GameMap[coord->y][coord->x];
    }
    if (cell == NULL) {
        *param_2 = *param_2 + -1;
        return;
    }
    if ((cell->flags.word & 0x10) != 0) {
        *param_2 = *param_2 + -1;
        return;
    }
    if ((cell->flags.word & 0x80) != 0) {
        if (cell->obj->ride == PathControlObject) {
            *param_2 = *param_2 + -1;
            return;
        }
        kind = cell->obj->ride->field_20;
        if (kind == 2 || kind == 3) {
            *param_3 = *param_3 + 1;
        }
    }
}

// FUNCTION: LEGOLAND 0x00459960
void ResetPathSceneryTimer(void) {
    PathSceneryLastUpdate = GetGameTimer();
}

/* Periodically counts scenery along the perimeter of each path rectangle and updates the total, matched count and
 * coverage percentage. */
// FUNCTION: LEGOLAND 0x00459970
void UpdatePathSceneryPercent(void) {
    int now;
    struct RectListNode *node;
    int total;
    int matched;
    struct Point coord;

    now = GetGameTimer();
    if (10000 < now - (int)PathSceneryLastUpdate) {
        PathSceneryDirty = 0;
        matched = 0;
        total = 0;
        PathSceneryLastUpdate = now;
        for (node = (struct RectListNode *)GetBestNodeList(); node != NULL; node = node->next) {
            total = total + 4 + (((node->y1 - node->x0) - node->y0) + node->x1) * 2;
            coord.x = node->x0;
            while (coord.x <= node->x1) {
                coord.y = node->y0 + -1;
                CountPathEdgeScenery(&coord, &total, &matched);
                coord.y = node->y1 + 1;
                CountPathEdgeScenery(&coord, &total, &matched);
                coord.x++;
            }
            coord.y = node->y0;
            while (coord.y <= node->y1) {
                coord.x = node->x0 + -1;
                CountPathEdgeScenery(&coord, &total, &matched);
                coord.x = node->x1 + 1;
                CountPathEdgeScenery(&coord, &total, &matched);
                coord.y++;
            }
        }
        PathEdgeSquareTotal = total;
        PathEdgeScenerySquares = matched;
        if (total != 0) {
            PathSceneryPercent = (matched * 100) / total;
            return;
        }
        PathSceneryPercent = 0;
    }
}

// FUNCTION: LEGOLAND 0x00459ad0
LEGO_EXPORT void PutObjOnMap(struct Ride *obj, struct Element *classid, struct Point *pos) {
    int area;
    struct Ride *entrance;
    struct MapCell *cell;

    if (classid == CastleObjElem) {
        CastlePlacedFlag = 1;
    }
    obj->cb_98(classid, pos);
    if (obj == PathControlObject) {
        PathSquareCount = PathSquareCount + 1;
        TotalCoveredSquares = TotalCoveredSquares + 1;
        PathSceneryDirty = 1;
    } else {
        area = GetRectArea((struct RectNode *)&obj->footprint);
        switch (obj->type) {
        case 1:
            AddObjInstance(pos);
            RideCoveredSquares = RideCoveredSquares + area;
            break;
        case 2:
            PathSceneryDirty = 1;
            SceneryCoveredSquares = SceneryCoveredSquares + area;
            break;
        case 3:
            Type3CoveredSquares = Type3CoveredSquares + area;
            break;
        case 4:
            AddObjInstance(pos);
            ShopCoveredSquares = ShopCoveredSquares + area;
            break;
        case 5:
            AddObjInstance(pos);
            FoodCoveredSquares = FoodCoveredSquares + area;
        }
        TotalCoveredSquares = TotalCoveredSquares + area;
        AddObjectsPowerStats(classid, pos);
    }
    if (classid == ElemID("ENTRANCE 1")) {
        entrance = (struct Ride *)((struct Element *)ElemID("ENTRANCE 1"))->ride;
        if (pos->x < 0 || pos->x >= (int)lpConfig->width || pos->y < 0 || pos->y >= (int)lpConfig->height) {
            cell = NULL;
        } else {
            cell = (struct MapCell *)&GameMap[pos->y][pos->x];
        }
        ParkExitPos.x = (cell->src.b.byte_4 + entrance->footprint.v[0]) * 0x100 + -0x100;
        ParkExitPos.y = ((unsigned int)(entrance->footprint.v[3] - entrance->footprint.v[1]) >> 1) * 0x100 +
            (cell->src.b.byte_5 + entrance->footprint.v[1]) * 0x100;
    }
    ScriptDirtyCategories = ScriptDirtyCategories | 1;
}

// FUNCTION: LEGOLAND 0x00459c90
LEGO_EXPORT void RemObjFromMap(struct Ride *obj, struct Element *classid, TileId tile, void *cursor) {
    int area;
    struct MapCell *cell;
    unsigned short *flags;
    struct Cursor *query;
    int x;
    int y;
    struct RemBlock blk;

    EraseWorkOrdersAtTile(obj, tile);
    if (classid == CastleObjElem) {
        CastlePlacedFlag = 0;
    }
    if (MapBatchUpdate == 0) {
        blk.f8 = tile.pos.x;
        blk.f0 = 2;
        blk.fc = tile.pos.y;
        PlayInstanceOfSample(GameFX[FX_PUNCH].sample, 0, 1, &blk);
    } else {
        MapBatchChanged = 1;
    }
    RemoveObjectsPowerStats(classid, tile);
    obj->cb_9c(classid, tile, cursor);
    if (obj == PathControlObject) {
        PathSquareCount = PathSquareCount + -1;
        TotalCoveredSquares = TotalCoveredSquares + -1;
        PathSceneryDirty = 1;
    } else {
        area = GetRectArea((struct RectNode *)&obj->footprint);
        switch (obj->type) {
        case 1:
            RemoveObjInstance((struct Point *)&blk);
            RideCoveredSquares = RideCoveredSquares - area;
            break;
        case 2:
            PathSceneryDirty = 1;
            SceneryCoveredSquares = SceneryCoveredSquares - area;
            break;
        case 3:
            Type3CoveredSquares = Type3CoveredSquares - area;
            break;
        case 4:
            RemoveObjInstance((struct Point *)&blk);
            ShopCoveredSquares = ShopCoveredSquares - area;
            break;
        case 5:
            RemoveObjInstance((struct Point *)&blk);
            FoodCoveredSquares = FoodCoveredSquares - area;
        }
        TotalCoveredSquares = TotalCoveredSquares - area;
    }
    x = tile.pos.x;
    y = tile.pos.y;
    if (x >= 0 && x < (int)lpConfig->width && y >= 0 && y < (int)lpConfig->height) {
        cell = (struct MapCell *)&GameMap[y][x];
    } else {
        cell = NULL;
    }
    if (cell->flags.bytes[1] & 0x40) {
        RemoveRepairOrderAT(obj, x, y);
        cell->flags.word = cell->flags.word & 0xbfff;
    }
    query = &QueryCursor;
    while (query != NULL) {
        if ((query->field_1828 & 0x1000) != 0) {
            if (query != NULL) {
                for (blk.f4 = query->footprint.y0 + query->tile_y; blk.f4 <= query->tile_y + query->footprint.y1; blk.f4++) {
                    for (blk.f0 = query->footprint.x0 + query->tile_x; blk.f0 <= query->tile_x + query->footprint.x1; blk.f0++) {
                        flags = &((struct MapCell *)&GameMap[blk.f4][blk.f0])->flags.word;
                        *flags &= 0xffe7;
                        cell = (struct MapCell *)&GameMap[blk.f4][blk.f0];
                        cell->byte_10 = 0;
                        cell = (struct MapCell *)&GameMap[blk.f4][blk.f0];
                        cell->word_8 = cell->word_a;
                        AdjustPathTileAndNeighbours((struct Point *)&blk);
                        RemovePathSquare((struct Point *)&blk);
                    }
                }
            }
            break;
        }
        query = query->next;
    }
    ScriptDirtyCategories = ScriptDirtyCategories | 4;
}

// GLOBAL: LEGOLAND 0x004b9340
static const struct PowerEntry PTR_s_Small_Power_Station_004b9340[] = {
    {"Small Power Station", 800},
    {"Crystal Power Station", 2500},
    {"Dino Big", -100},
    {"Dino Small", -30},
    {"Dino Mini", -30},
    {"T-Rex", -130},
    {"Fountain 1", -10},
    {"Fountain 2", -10},
    {"Fountain 3", -10},
    {"Foodcart Drink", -2},
    {"FoodCart Food", -2},
    {"Foodcart Icecream", -2},
    {"LEGO Shop 1", -3},
    {"LEGO Shop 2", -3},
    {"LEGO Media Shop", -3},
    {"Octopus Cafe", -5},
    {"Restaurant 1", -10},
    {"Restaurant 2", -60},
    {"Shark Cafe", -10},
    {"Boating School", -200},
    {"Boating School Mermaid", -30},
    {"Copters", -100},
    {"Driving School", -60},
    {"Space Tower Ride", -40},
    {"Spider Ride", -100},
    {"Water Works Entrance", -30},
    {"Water Works Crocodile Fountain", -6},
    {"Water Works Elephant Fountain", -6},
    {"Water Works Water Block", -4},
    {"Water Pump", -15},
    {"Bank", -5},
    {"Chuck Wagon", -6},
    {"General Store", -4},
    {"Jail Cell", -2},
    {"Saloon", -4},
    {"Sheriff", -3},
    {"Carousel", -60},
    {"Fort", -8},
    {"Gold Rush", -30},
    {"Log Flume Entrance", -100},
    {"Spinning Barrels Ride", -90},
    {"Temple", -3},
    {"Explorers Institute", -20},
    {"Balloonz", -140},
    {"Earth Slide Ride", -40},
    {"Jungle Cruise", -100},
    {"Plane Ride", -160},
    {"Safari Ride", -80},
    {"Temple Slide", -50},
    {"Castle BBQ", -20},
    {"Castle Level 1", -8},
    {"Castle Obj", -400},
    {"Catapult", -2},
    {"Joust", -75},
    {"Miniland San Francisco", -40},
    {"Miniland France", -40},
    {"Miniland Washington", -40},
    {"Miniland New York", -40},
    {"Miniland India", -40},
    {"Miniland Holland", -40},
    {"Miniland London", -40},
    {"Miniland Italy", -40},
    {"Miniland Australia", -40},
    {"Miniland Egypt", -40},
    {"Miniland Denmark", -40},
    {"", 0},
};

// FUNCTION: LEGOLAND 0x00459fa0
LEGO_EXPORT int FindObjectsPower(struct Ride *ride) {
    const struct PowerEntry *entry;

    for (entry = PTR_s_Small_Power_Station_004b9340; strlen(entry->name) != 0; entry++) {
        if (_stricmp(ride->element->name, entry->name) == 0) {
            return entry->value;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045a000
void MarkObjectPowered(int power, MapElement *object) {
    object->flags &= 0xfeff;
    MapStats.unpowered_demand = MapStats.unpowered_demand - power;
    MapStats.unpowered_count = MapStats.unpowered_count - 1;
}

// FUNCTION: LEGOLAND 0x0045a030
void MarkObjectUnpowered(int power, MapElement *object) {
    object->flags |= 0x100;
    MapStats.unpowered_demand = MapStats.unpowered_demand + power;
    MapStats.unpowered_count = MapStats.unpowered_count + 1;
}

// FUNCTION: LEGOLAND 0x0045a060
void RestorePowerToObjects(void) {
    MapElement *object = GetFirstRenderObject();
    int power;
    if (object == NULL) {
        return;
    }
    while (object != NULL) {
        if (object->flags & 0x100) {
            power = -FindObjectsPower(object->field_0->data);
            if (MapStats.power_demand - (int)MapStats.unpowered_demand + power <= MapStats.power_supply) {
                MarkObjectPowered(power, object);
                if (MapStats.power_demand - (int)MapStats.unpowered_demand == MapStats.power_supply) {
                    return;
                }
            }
        }
        object = GetNextRenderObject(object);
    }
}

// FUNCTION: LEGOLAND 0x0045a0d0
void CutPowerToObjects(void) {
    MapElement *object = GetFirstRenderObject();
    int power;
    while (object != NULL) {
        if ((object->flags & 0x100) == 0) {
            power = -FindObjectsPower(object->field_0->data);
            if (power > 0) {
                MarkObjectUnpowered(power, object);
                if (MapStats.power_demand - (int)MapStats.unpowered_demand <= MapStats.power_supply) {
                    break;
                }
            }
        }
        object = GetNextRenderObject(object);
    }
}

// FUNCTION: LEGOLAND 0x0045a130
LEGO_EXPORT void AddObjectsPowerStats(struct Element *classid, struct Point *pos) {
    int power;
    struct MapCell *cell;
    int amount;

    power = FindObjectsPower(((struct Element *)classid)->ride);
    if (power != 0) {
        if (0 < power) {
            MapStats.power_supply = MapStats.power_supply + power;
            if (MapStats.unpowered_demand != 0) {
                RestorePowerToObjects();
            }
            return;
        }
        if (pos->x < 0 || pos->x >= (int)lpConfig->width || pos->y < 0 || pos->y >= (int)lpConfig->height) {
            cell = NULL;
        } else {
            cell = (struct MapCell *)&GameMap[pos->y][pos->x];
        }
        amount = abs(power);
        MapStats.power_demand = MapStats.power_demand + amount;
        if (MapStats.power_supply < MapStats.power_demand - (int)MapStats.unpowered_demand) {
            MapStats.unpowered_demand = MapStats.unpowered_demand + amount;
            MapStats.unpowered_count = MapStats.unpowered_count + 1;
            cell->flags.bytes[1] |= 1;
        } else {
            cell->flags.word = cell->flags.word & 0xfeff;
        }
        if (MapStats.power_supply <= MapStats.power_demand) {
            MapStats.power_spare_percent = 0;
            return;
        }
        MapStats.power_spare_percent = 100 - (MapStats.power_demand * 100) / MapStats.power_supply;
    }
}

// FUNCTION: LEGOLAND 0x0045a230
LEGO_EXPORT void RemoveObjectsPowerStats(struct Element *classid, TileId coords) {
    int power;
    struct MapCell *cell;
    int amount;
    int x;
    int y;

    power = FindObjectsPower(((struct Element *)classid)->ride);
    if (power != 0) {
        if (0 < power) {
            x = coords.pos.x;
            y = coords.pos.y;
            if (x >= 0 && x < (int)lpConfig->width && y >= 0 && y < (int)lpConfig->height) {
                cell = (struct MapCell *)&GameMap[y][x];
            } else {
                cell = NULL;
            }
            if ((cell->flags.bytes[1] & 2) == 0) {
                MapStats.power_supply = MapStats.power_supply - power;
                if (MapStats.power_demand - (int)MapStats.unpowered_demand > MapStats.power_supply) {
                    CutPowerToObjects();
                }
            }
        } else {
            amount = abs(power);
            MapStats.power_demand = MapStats.power_demand - amount;
            if (MapStats.unpowered_demand != 0) {
                x = coords.pos.x;
                y = coords.pos.y;
                if (x >= 0 && x < (int)lpConfig->width && y >= 0 && y < (int)lpConfig->height) {
                    cell = (struct MapCell *)&GameMap[y][x];
                } else {
                    cell = NULL;
                }
                if ((cell->flags.bytes[1] & 1) != 0) {
                    MapStats.unpowered_demand = MapStats.unpowered_demand - amount;
                    MapStats.unpowered_count = MapStats.unpowered_count + -1;
                }
                RestorePowerToObjects();
            }
        }
        if (MapStats.power_supply <= MapStats.power_demand) {
            MapStats.power_spare_percent = 0;
            return;
        }
        MapStats.power_spare_percent = 100 - (MapStats.power_demand * 100) / MapStats.power_supply;
    }
}

// FUNCTION: LEGOLAND 0x0045a390
LEGO_EXPORT void DefaultCursor(struct Cursor *cursor) {
    unsigned int saved_1404 = cursor->tile_x;
    unsigned int saved_1408 = cursor->tile_y;
    memset(cursor, 0, sizeof(struct Cursor));
    cursor->tile_x = saved_1404;
    cursor->field_1828 = 0xc00;
    cursor->tile_y = saved_1408;
    ResetCursorBuildable(cursor);
}

/* Render order helper: takes the pending entry for column param[0], returns the row to resume from in param[1] (0
 * when none) and clears the entry. */
// FUNCTION: LEGOLAND 0x0045a3e0
void PopRenderOrderColumnStart(int *param) {
    struct MapRenderOrderEntry *entry;
    int i;

    for (i = 0, entry = MapRenderOrderList; (INT_PTR)&entry->x < (int)((char *)&CastleObjElem + 2); entry++, i++) {
        if (entry->flag != 0 && (unsigned int)entry->x == *param) {
            param[1] = MapRenderOrderList[i].height;
            MapRenderOrderList[i].flag = 0;
            return;
        }
    }
    param[1] = 0;
}

/* Render order helper: takes the pending entry recorded for the anchor tile id and returns its column and resume
 * row in param_2 (map width/height when none), clearing the entry. */
// FUNCTION: LEGOLAND 0x0045a430
void PopRenderOrderEntryForAnchor(short param_1, int *param_2) {
    struct MapRenderOrderEntry *entry;
    int i;

    for (i = 0, entry = MapRenderOrderList; (INT_PTR)&entry->coords < (INT_PTR)&CastleObjElem; entry++, i++) {
        if (entry->flag != 0 && (short)entry->coords == param_1) {
            *param_2 = MapRenderOrderList[i].x;
            param_2[1] = MapRenderOrderList[i].height;
            MapRenderOrderList[i].flag = 0;
            return;
        }
    }
    *param_2 = lpConfig->width;
    param_2[1] = lpConfig->height;
}

// FUNCTION: LEGOLAND 0x0045a4a0
LEGO_EXPORT void CalculateMapRenderOrder(void) {
    unsigned short *link; /* where the next anchor tile of the chain is written */
    MapElement *cell;
    MapElement *origin;
    struct Ride *ride;
    struct MapRenderOrderEntry *entry;
    int ox;
    int oy;
    Point pt;

    pt.x = 0;
    pt.y = 0;
    MapRenderOrderCount = 0;
    link = &RenderOrderFirstAnchor;
    memset(MapRenderOrderList, 0, sizeof(MapRenderOrderList));
    while (pt.x < lpConfig->width) {
        if (pt.x >= 0 && pt.x < lpConfig->width && pt.y >= 0 && pt.y < lpConfig->height) {
            cell = &GameMap[pt.y][pt.x];
        } else {
            cell = NULL;
        }
        if ((cell->flags & 0xa0) == 0) {
            pt.y++;
        } else {
            ox = cell->field_4;
            oy = cell->field_5;
            if (ox >= 0 && ox < lpConfig->width && oy >= 0 && oy < lpConfig->height) {
                origin = &GameMap[oy][ox];
            } else {
                origin = NULL;
            }
            ride = origin->field_0->ride;
            entry = &MapRenderOrderList[MapRenderOrderCount];
            MapRenderOrderCount++;
            if (MapRenderOrderCount == 0x1000) {
                MapRenderOrderCount = 0;
            }
            entry->coords = cell->anchor.id;
            entry->x = (unsigned char)pt.x;
            entry->height = ride->footprint.y1 + oy + 1;
            entry->flag = 1;
            if (pt.x == ride->footprint.x1 + ox) {
                *link = origin->anchor.id;
                link = &origin->next.id;
                PopRenderOrderEntryForAnchor(cell->anchor.id, &pt.x);
            } else if (pt.x == lpConfig->width - 1) {
                *link = origin->anchor.id;
                link = &origin->next.id;
                PopRenderOrderEntryForAnchor(cell->anchor.id, &pt.x);
            } else {
                pt.x++;
                PopRenderOrderColumnStart(&pt.x);
            }
        }
        while (pt.y >= lpConfig->height) {
            pt.x++;
            PopRenderOrderColumnStart(&pt.x);
        }
    }
    *link = 0;
}

/* Builds the map render order chain for the full map view (RenderFullMap): scans columns top to bottom and links
 * object anchor tiles through their next ids, resuming columns interrupted by objects (like
 * CalculateMapRenderOrder). */
// FUNCTION: LEGOLAND 0x0045a660
void CalculateFullMapRenderOrder(void) {
    unsigned short *link; /* where the next anchor tile of the chain is written */
    MapElement *cell;
    MapElement *origin;
    struct Ride *ride;
    struct MapRenderOrderEntry *entry;
    int ox;
    int oy;
    Element *roads;
    Point pt;

    link = &RenderOrderFirstAnchor;
    pt.x = 0;
    pt.y = 0;
    roads = ElemID("DRIVING SCHOOL ROADS");
    MapRenderOrderCount = 0;
    memset(MapRenderOrderList, 0, sizeof(MapRenderOrderList));
    while (pt.x < lpConfig->width) {
        if (pt.x >= 0 && pt.x < lpConfig->width && pt.y >= 0 && pt.y < lpConfig->height) {
            cell = &GameMap[pt.y][pt.x];
        } else {
            cell = NULL;
        }
        if ((cell->flags & 0xa0) == 0 && ((cell->flags & 8) == 0 || cell->field_0 != roads)) {
            pt.y++;
        } else {
            ox = cell->field_4;
            oy = cell->field_5;
            if (ox >= 0 && ox < lpConfig->width && oy >= 0 && oy < lpConfig->height) {
                origin = &GameMap[oy][ox];
            } else {
                origin = NULL;
            }
            ride = origin->field_0->ride;
            entry = &MapRenderOrderList[MapRenderOrderCount];
            MapRenderOrderCount++;
            if (MapRenderOrderCount == 0x1000) {
                MapRenderOrderCount = 0;
            }
            entry->coords = cell->anchor.id;
            entry->x = (unsigned char)pt.x;
            entry->height = ride->footprint.y1 + oy + 1;
            entry->flag = 1;
            if (pt.x == ride->footprint.x1 + ox) {
                *link = origin->anchor.id;
                link = &origin->next.id;
                PopRenderOrderEntryForAnchor(cell->anchor.id, &pt.x);
            } else if (pt.x == lpConfig->width - 1) {
                *link = origin->anchor.id;
                link = &origin->next.id;
                PopRenderOrderEntryForAnchor(cell->anchor.id, &pt.x);
            } else {
                pt.x++;
                PopRenderOrderColumnStart(&pt.x);
            }
        }
        while (pt.y >= lpConfig->height) {
            pt.x++;
            PopRenderOrderColumnStart(&pt.x);
        }
    }
    *link = 0;
}

// FUNCTION: LEGOLAND 0x0045a850
LEGO_EXPORT MapElement *GetFirstRenderObject(void) {
    int x;
    int y;
    MapElement *element;

    x = ((unsigned char *)&RenderOrderFirstAnchor)[0];
    y = ((unsigned char *)&RenderOrderFirstAnchor)[1];
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        return 0;
    }
    element = &GameMap[y][x];
    if (element == 0) {
        return 0;
    }
    if (RenderOrderFirstAnchor == 0 && (element->flags & 0xa8) == 0) {
        return 0;
    }
    return element;
}

// FUNCTION: LEGOLAND 0x0045a8b0
LEGO_EXPORT MapElement *GetNextRenderObject(MapElement *object) {
    if (object != 0 && object->next.id != 0) {
        Point p;
        p.x = object->next.pos.x;
        p.y = object->next.pos.y;
        return GetTileAtPoint(&p);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045a910
LEGO_EXPORT MapElement *GetFirstObjectMatching(Element *cls) {
    MapElement *object = GetFirstRenderObject();
    while (object != NULL) {
        if (object->field_0 == cls) {
            return object;
        }
        object = GetNextRenderObject(object);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0045a940
LEGO_EXPORT MapElement *GetNextObjectMatching(MapElement *object, Element *cls) {
    object = GetNextRenderObject(object);
    while (object != NULL) {
        if (object->field_0 == cls) {
            return object;
        }
        object = GetNextRenderObject(object);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0045a970
LEGO_EXPORT struct Point PlayfieldToMap(struct Point pos) {
    int width;
    int height;
    struct Point result;

    GetTileDimensions(&width, &height);
    result.x = pos.x / width + pos.y / height;
    result.y = pos.y / height - pos.x / width;
    return result;
}
