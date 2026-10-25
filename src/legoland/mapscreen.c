#include <string.h>

#include "legoland.h"

#include "castle.h"
#include "clipping.h"
#include "draw.h"
#include "gamemap.h"
#include "gfx.h"
#include "globals.h"
#include "interface.h"
#include "llidb.h"
#include "map_object.h"
#include "mapscreen.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render.h"
#include "ride_queue.h"
#include "tilemap.h"

struct MapPoint {
    int x;
    int y;
};

/* A sprite overlaid on the map (OverlayList). */
#define HALF(v) ((v) < 0 ? -(-(v) >> 1) : (v) >> 1)

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004562c0
void SaveMouseHandlers(void) {
    SavedOkKeyHandler = OkKeyHandler;
    SavedCancelKeyHandler = CancelKeyHandler;
}

// FUNCTION: LEGOLAND 0x004562e0
void RestoreMouseHandlers(void) {
    OkKeyHandler = SavedOkKeyHandler;
    CancelKeyHandler = SavedCancelKeyHandler;
}

// FUNCTION: LEGOLAND 0x00456300
LEGO_EXPORT void InitMapScreen(void) {
    if (FullMapRendered != 0) {
        return;
    }
    FullMapSprite = CreateFunctionBasedSprite(
        (int (*)(struct Sprite *))RenderFullMap, 0x280, 0x154);
    if (FullMapSprite != NULL) {
        // STRING: LEGOLAND 0x004b90b4
        MapSpannerSprite = LoadSprite("mapSpanner.lls", 0);
        FullMapSprite->src_x = 0;
        FullMapSprite->src_y = 0;
    }
    SaveMouseHandlers();
    CancelKeyHandler = ToggleMapScreen;
    OkKeyHandler = ToggleMapScreen;
}

// FUNCTION: LEGOLAND 0x00456370
LEGO_EXPORT void KillMapScreen(void) {
    if (FullMapSprite != NULL) {
        KillSprite(FullMapSprite);
        FullMapSprite = NULL;
        if (MapSpannerSprite != 0) {
            KillSprite(MapSpannerSprite);
            MapSpannerSprite = 0;
        }
    }
    FullMapRendered = 0;
}

// FUNCTION: LEGOLAND 0x004563b0
void DrawMapViewBox(void) {
    int x = ((ScrollX >> 8) - MapWorldMinX) * MapViewWidth / MapWorldWidth;
    int y = ((ScrollY >> 8) - MapWorldMinY) * MapViewWidth / MapWorldHeight / 2 +
        MapCenterOffsetY;
    int w = MapViewWidth * MapViewWidth / MapWorldWidth;
    int h = MapViewHeight * MapViewWidth / MapWorldHeight / 2;

    RenderThickBox(MapViewX + x, MapViewY + y, w, h, 2,
        GetNearestColour(255, 255, 255));
}

// FUNCTION: LEGOLAND 0x00456460
LEGO_EXPORT void RenderMouseBounds(void) {
    int in[2];
    int out[2];
    int x;
    int y;
    int w;
    int h;

    in[0] = (MousePos.x - MapViewX) * MapWorldWidth / MapViewWidth +
        MapWorldMinX;
    in[1] = (MousePos.y - MapViewY - MapCenterOffsetY + 1) * MapWorldHeight *
            2 / MapViewWidth +
        MapWorldMinY;
    PointToIsoPlane(in, out);
    if (out[0] >= 0 && out[1] >= 0 && out[0] < MapViewWidth &&
        out[1] < MapViewHeight) {
        x = MousePos.x - MapViewWidth / 2 * MapViewWidth / MapWorldWidth -
            MapViewX;
        y = MousePos.y - MapViewHeight / 2 * MapViewWidth / MapWorldHeight / 2 -
            MapViewY;
        w = MapViewWidth * MapViewWidth / MapWorldWidth;
        h = MapViewHeight * MapViewWidth / (MapWorldHeight * 2);
        RenderBox(MapViewX + x, MapViewY + y, w, h,
            GetNearestColour(0, 255, 0));
    }
}

// FUNCTION: LEGOLAND 0x004565b0
LEGO_EXPORT void MapScreenSetScrollPos(struct Point *point) {
    int in[2];
    int out[2];

    in[0] = (MousePos.x - MapViewX) * MapWorldWidth / MapViewWidth +
        MapWorldMinX;
    in[1] = (MousePos.y - MapViewY - MapCenterOffsetY + 1) * MapWorldHeight *
            2 / MapViewWidth +
        MapWorldMinY;
    PointToIsoPlane(in, out);
    if (out[0] >= 0 && out[1] >= 0 && out[0] < MapViewWidth &&
        out[1] < MapViewHeight) {
        ScrollX = ((point->x - MapViewX + 1) * MapWorldWidth / MapViewWidth -
                      MapViewWidth / 2 + MapWorldMinX)
            << 8;
        ScrollY = ((point->y - MapViewY - MapCenterOffsetY + 1) * MapWorldHeight * 2 /
                          MapViewWidth -
                      MapViewHeight / 2 + MapWorldMinY)
            << 8;
        ClampScrollPosition(lpConfig->view_width << 8, lpConfig->view_height << 8, 0, 0);
    }
}

// FUNCTION: LEGOLAND 0x004566f0
LEGO_EXPORT void DrawMapScreen(void) {
    struct MapMarker(*row)[32];
    struct MapMarker *m;
    int j;

    PrintSprite(FullMapSprite, MapViewX, MapViewY, 0, 0);
    if (FrameCounter & 0x10) {
        for (row = MapSpannerMarks; (INT_PTR)row < (INT_PTR)MapSpannerMarksLastRow; row++) {
            m = *row;
            for (j = 0; j < 31; j++, m++) {
                if (m->x != 0 || m->y != 0) {
                    PrintSprite(MapSpannerSprite, m->x, m->y, 0, 0);
                }
            }
        }
    }
    DrawMapViewBox();
}

// FUNCTION: LEGOLAND 0x00456770
void HalveMapPoint(struct MapPoint *arg) {
    int v0 = arg->x;
    int v1 = arg->y;

    if (v0 < 0) {
        v0 = -((-v0) >> 1);
    } else {
        v0 = v0 >> 1;
    }

    if (v1 < 0) {
        v1 = -((-v1) >> 1);
    } else {
        v1 = v1 >> 1;
    }

    arg->x = v0;
    arg->y = v1;
}

// FUNCTION: LEGOLAND 0x004567a0
LEGO_EXPORT void RenderFullMap(void) {
    struct Element *square_track;
    struct Element *square_track_height;
    struct Element *square_track_height_0;
    struct Element *square_track_height_path;
    struct Element *castle_dummy;
    struct Element *driving_school_roads;
    struct Sprite *blob;
    struct Sprite *stick;
    struct Sprite *lights;
    HPEN pen;

    // STRING: LEGOLAND 0x004b5bfc
    square_track = ElemID("SQUARE_TRACK");
    // STRING: LEGOLAND 0x004b5be8
    square_track_height = ElemID("SQUARE_TRACK_HEIGHT");
    // STRING: LEGOLAND 0x004b5bd0
    square_track_height_0 = ElemID("SQUARE_TRACK_HEIGHT_0");
    // STRING: LEGOLAND 0x004b5bb4
    square_track_height_path = ElemID("SQUARE_TRACK_HEIGHT_PATH");
    // STRING: LEGOLAND 0x004b5b7c
    castle_dummy = ElemID("CASTLE_DUMMY");
    // STRING: LEGOLAND 0x004b89cc
    driving_school_roads = ElemID("DRIVING SCHOOL ROADS");
    // STRING: LEGOLAND 0x004b90e4
    blob = LoadSprite("TRACKBLOB.LLS", 1);
    // STRING: LEGOLAND 0x004b90d4
    stick = LoadSprite("TRACKSTICK.LLS", 1);
    // STRING: LEGOLAND 0x004b90c4
    lights = LoadSprite("MAPLIGHTS.LLS", 1);
    pen = CreatePen(PS_SOLID, 2, 0xff4000);

    if (FullMapRendered == 0) {
        int tile_w;
        int tile_h;
        int half_h;
        int scale_x; /* 16.16 world -> map scale */
        int scale_y;
        int saved_view_x;
        int saved_view_y;
        struct Point pos; /* map tile of the terrain passes */
        int bounds[4];
        struct MapElement tile;
        struct MapElement *obj;
        struct Overlay *node;

        CalculateFullMapRenderOrder();
        memset(MapSpannerMarks, 0, 0x2000); /* all 32 rows, MapSpannerMarksLastRow included */
        MapViewX = 0;
        MapViewY = 0x20;
        MapViewWidth = 0x280;
        MapViewHeight = 0x154;
        StoreClipping();
        {
            RECT clip;

            clip.left = 0;
            clip.right = MapViewWidth;
            clip.top = 0;
            clip.bottom = MapViewHeight;
            SetClipping(&clip);
        }
        GetTileDimensions(&tile_w, &tile_h);
        MapWorldMinY = 0;
        MapWorldMinX = -(lpConfig->height * tile_w / 2);
        MapWorldMaxX = lpConfig->width * tile_w / 2;
        MapWorldMaxY = (lpConfig->width + lpConfig->height) * tile_h / 2;
        MapWorldWidth = MapWorldMaxX - MapWorldMinX + 1;
        MapWorldHeight = MapWorldMaxY + 1;
        scale_x = (MapViewWidth << 16) / MapWorldWidth;
        scale_y = (MapViewHeight << 16) / MapWorldHeight;
        MapDrawWidth = (short)((float)MapViewWidth * tile_w / MapWorldWidth + 1.0f);
        MapDrawHeight = (short)((float)MapViewHeight * tile_h / MapWorldHeight + 1.0f);
        half_h = (tile_h + 1) >> 1;
        RenderBlock(0, 0, MapViewWidth, MapViewHeight, GetNearestColour(0, 0, 0));
        saved_view_x = lpConfig->view_x;
        saved_view_y = lpConfig->view_y;
        lpConfig->view_x = 0;
        lpConfig->view_y = 0;
        FullMapRendered = 1;
        MapCenterOffsetY =
            (MapViewHeight - ((lpConfig->width + lpConfig->height - 2) * half_h - MapWorldMinY) * MapViewWidth / MapWorldHeight / 2) >>
            1;

        /* Pass 1: terrain colour blocks. */
        for (pos.y = 0; pos.y < (int)lpConfig->height; pos.y++) {
            for (pos.x = 0; pos.x < (int)lpConfig->width; pos.x++) {
                if (pos.x >= 0 && pos.x < (int)lpConfig->width && pos.y >= 0 &&
                    pos.y < (int)lpConfig->height) {
                    tile = GameMap[pos.y][pos.x];
                } else {
                    tile.flags = 0x40;
                }
                if ((tile.flags & 8) == 0) {
                    unsigned int id;
                    struct FXSpriteList *src;
                    struct Point pt;
                    int px;
                    int py;

                    id = GameMap[pos.y][pos.x].field_8;
                    src = TileSpriteInfo[id].src;
                    pt.x = pos.x;
                    pt.y = pos.y;
                    GetTileBounds(&pt, bounds);
                    bounds[0] += ScrollX >> 8;
                    bounds[1] += ScrollY >> 8;
                    px = ((bounds[0] - MapWorldMinX) * scale_x) >> 16;
                    py = (((bounds[1] - MapWorldMinY) * scale_y) >> 16) + MapCenterOffsetY;
                    if ((tile.flags & 0x10) && (tile.flags & 0x80)) {
                        RenderBlock(px - 7, py - 2, MapDrawWidth + 5, MapDrawHeight + 4,
                            GetNearestColour(0x80, 0x80, 0x80));
                    } else {
                        switch (src->sprite_ids[id - src->base] & 0x3f) {
                        case 0:
                        case 0x20:
                            RenderBlock(px - 7, py - 2, MapDrawWidth + 5, MapDrawHeight + 4,
                                GetNearestColour(0, 0x8f, 0x4f));
                            break;
                        case 1:
                        case 0x21:
                            RenderBlock(px - 7, py - 2, MapDrawWidth + 5, MapDrawHeight + 4,
                                GetNearestColour(0xff, 0xe0, 0x8f));
                            break;
                        case 0x30:
                            RenderBlock(px - 7, py - 2, MapDrawWidth + 5, MapDrawHeight + 4,
                                GetNearestColour(0x5b, 0xbe, 2));
                            break;
                        }
                    }
                }
            }
        }

        /* Pass 2: the sprites of the ride tiles. */
        PushRenderingStatusAndUnlockVideoSurface();
        for (pos.y = 0; pos.y < (int)lpConfig->height; pos.y++) {
            for (pos.x = 0; pos.x < (int)lpConfig->width; pos.x++) {
                if (pos.x >= 0 && pos.x < (int)lpConfig->width && pos.y >= 0 &&
                    pos.y < (int)lpConfig->height) {
                    tile = GameMap[pos.y][pos.x];
                } else {
                    tile.field_8 = 0;
                    tile.flags = 0x40;
                }
                if (tile.flags & 8) {
                    float sx;
                    float sy;
                    struct Ride *ride;
                    struct Point pt;
                    struct MapPoint off;

                    sx = (float)MapViewWidth / MapWorldWidth;
                    sy = (float)MapViewHeight / MapWorldHeight;
                    ride = tile.field_0->ride;
                    pt.x = pos.x;
                    pt.y = pos.y;
                    GetTileBounds(&pt, bounds);
                    off.x = ride->field_14;
                    off.y = ride->field_18;
                    HalveMapPoint(&off);
                    bounds[0] += off.x;
                    bounds[1] += off.y;
                    bounds[0] += ScrollX >> 8;
                    bounds[1] += ScrollY >> 8;
                    PrintScaledSprite(TileSpriteArray[tile.field_8],
                        (int)((bounds[0] - MapWorldMinX) * sx),
                        (int)((bounds[1] - MapWorldMinY) * sy) + MapCenterOffsetY,
                        MapDrawWidth + 1, MapDrawHeight + 1);
                }
            }
        }

        /* Pass 3: the overlay sprites. */
        {
            RECT clip;

            clip.left = 0;
            clip.right = MapViewWidth;
            clip.top = 0;
            clip.bottom = MapViewHeight;
            SetClipping(&clip);
        }
        {
            int origin[4];
            struct Point pt;
            struct Point base; /* map position of tile (0,0) */

            node = OverlayList;
            pt.x = 0;
            pt.y = 0;
            GetTileBounds(&pt, origin);
            origin[0] += ScrollX >> 8;
            origin[1] += ScrollY >> 8;
            base.x = ((origin[0] - MapWorldMinX) * scale_x) >> 16;
            base.y = (((origin[1] - MapWorldMinY) * scale_y) >> 16) + MapCenterOffsetY;
            while (node != NULL) {
                PrintScaledSprite(node->sprite,
                    (((tile_w * 2 / 3 + node->x) * scale_x) >> 16) + base.x,
                    ((node->y * scale_y) >> 16) + base.y,
                    (node->sprite->width * scale_x) >> 16,
                    ((short)node->sprite->height * scale_y) >> 16);
                node = node->next;
            }
        }

        /* Pass 4: the placed objects. */
        for (obj = GetFirstRenderObject(); obj != NULL; obj = GetNextRenderObject(obj)) {
            TileId uid;
            struct MapElement elem;
            struct Ride *ride;
            RideSpriteInfo *info;
            RideSpriteInfo loc;

            uid.id = obj->anchor.id;
            elem = *obj;
            if ((elem.flags & 0x200) && (elem.flags & 4) == 0) {
                /* Remember the map position of each 8x8 block's centre. */
                struct Point pt;

                pt.x = (elem.field_4 & ~7) + 4;
                pt.y = (elem.field_5 & ~7) + 4;
                GetTileBounds(&pt, bounds);
                bounds[0] += ScrollX >> 8;
                bounds[1] += ScrollY >> 8;
                MapSpannerMarks[elem.field_5 >> 3][elem.field_4 >> 3].x =
                    ((bounds[0] - MapWorldMinX) * scale_x) >> 16;
                MapSpannerMarks[elem.field_5 >> 3][elem.field_4 >> 3].y =
                    (((bounds[1] - MapWorldMinY) * scale_y) >> 16) + MapCenterOffsetY;
            }
            ride = elem.field_0->ride;
            info = NULL;
            if ((ride->flags & 4) == 0 && (ride->flags & 0x400) == 0) {
                /* Plain scenery: only the driving school's traffic lights are drawn. */
                if (ride == driving_school_roads->ride) {
                    struct RideQueueEntry *entry;
                    struct Point pt;

                    loc.sprite = lights;
                    loc.x = -102;
                    loc.y = -88;
                    pt.x = uid.pos.x;
                    pt.y = uid.pos.y;
                    entry = FindQueueEntryAtTile(pt.x, pt.y);
                    if (entry != NULL && (entry->field_14 & 0xf) == 5) {
                        GetTileBounds(&pt, bounds);
                        bounds[0] += HALF(loc.x);
                        bounds[1] += HALF(loc.y);
                        bounds[0] += ScrollX >> 8;
                        bounds[1] += ScrollY >> 8;
                        MapDrawWidth = ((struct Sprite *)loc.sprite)->width;
                        MapDrawHeight = ((struct Sprite *)loc.sprite)->height;
                        PrintScaledSprite(loc.sprite, ((bounds[0] - MapWorldMinX) * scale_x) >> 16,
                            (((bounds[1] - MapWorldMinY) * scale_y) >> 16) + MapCenterOffsetY,
                            (MapDrawWidth * scale_x) >> 16, (MapDrawHeight * scale_y) >> 16);
                    }
                }
            } else if (ride->element == square_track || ride->element == square_track_height ||
                ride->element == square_track_height_0 ||
                ride->element == square_track_height_path || ride->element == castle_dummy) {
                /* Square track pieces: a line to the next piece, with a stick for the height. */
                struct Point piece; /* this track piece */
                struct Point dest; /* the piece it connects to */
                float height;
                float dest_height;
                int flag;

                piece.x = elem.field_4;
                piece.y = elem.field_5;
                if (GetCoasterPieceMapLine(&piece, &height, &dest, &dest_height, &flag)) {
                    struct Point pt;
                    int x1;
                    int y1;
                    int x2;
                    int y2;
                    int rise;
                    HDC hdc;
                    HGDIOBJ old;

                    if (height < FLOAT_004ab390) {
                        height = 0.0f;
                    }
                    if (dest_height < FLOAT_004ab390) {
                        dest_height = 0.0f;
                    }
                    pt.x = piece.x;
                    pt.y = piece.y;
                    GetTileBounds(&pt, bounds);
                    rise = -(int)height;
                    rise = HALF(rise);
                    bounds[1] += rise;
                    bounds[0] += ScrollX >> 8;
                    bounds[1] += ScrollY >> 8;
                    x1 = ((bounds[0] - MapWorldMinX) * scale_x) >> 16;
                    y1 = (((bounds[1] - MapWorldMinY) * scale_y) >> 16) + MapCenterOffsetY;
                    pt.x = dest.x;
                    pt.y = dest.y;
                    GetTileBounds(&pt, bounds);
                    rise = -(int)dest_height;
                    rise = HALF(rise);
                    bounds[1] += rise;
                    bounds[0] += ScrollX >> 8;
                    bounds[1] += ScrollY >> 8;
                    x2 = ((bounds[0] - MapWorldMinX) * scale_x) >> 16;
                    y2 = (((bounds[1] - MapWorldMinY) * scale_y) >> 16) + MapCenterOffsetY;
                    if (ride->element != square_track_height_path) {
                        MapDrawWidth = stick->width;
                        MapDrawHeight = (short)((float)scale_y * height * 7.62939453125e-06f);
                        if (MapDrawHeight > 0) {
                            PrintScaledSprite(stick, x1 - ((MapDrawWidth * scale_x) >> 17), y1,
                                (MapDrawWidth * scale_x) >> 16, MapDrawHeight);
                        }
                    }
                    PushRenderingStatusAndUnlockVideoSurface();
                    renderEngine->lpVtbl->GetDC(renderEngine, &hdc);
                    old = SelectObject(hdc, pen);
                    MoveToEx(hdc, x2, y2, NULL);
                    LineTo(hdc, x1, y1);
                    if (flag) {
                        pt.x = piece.x - 10;
                        pt.y = piece.y;
                        GetTileBounds(&pt, bounds);
                        rise = -(int)height;
                        rise = HALF(rise);
                        bounds[1] += rise;
                        bounds[0] += ScrollX >> 8;
                        bounds[1] += ScrollY >> 8;
                        LineTo(hdc, ((bounds[0] - MapWorldMinX) * scale_x) >> 16,
                            (((bounds[1] - MapWorldMinY) * scale_y) >> 16) + MapCenterOffsetY);
                    }
                    SelectObject(hdc, old);
                    renderEngine->lpVtbl->ReleaseDC(renderEngine, hdc);
                    PopRenderingStatus();
                }
                loc.sprite = blob;
                loc.x = 0;
                loc.y = (int)-height;
            } else if (ride->flags & 0x400) {
                if (ride->cb_sprite == NULL) {
                    continue;
                }
                info = ride->cb_sprite(ride->element, uid.id);
            } else {
                loc.sprite = ride->layer;
                loc.x = ride->field_14;
                loc.y = ride->field_18;
                info = &loc;
            }
            if (info == NULL || info->sprite == NULL) {
                continue;
            }
            if ((((struct Sprite *)info->sprite)->flags & 0x8000) == 0) {
                struct Point pt;
                int hx;
                int hy;
                struct Sprite *sprite;

                pt.x = uid.pos.x;
                pt.y = uid.pos.y;
                GetTileBounds(&pt, bounds);
                hx = info->x;
                hy = info->y;
                hx = HALF(hx);
                hy = HALF(hy);
                bounds[0] += hx;
                bounds[1] += hy;
                bounds[0] += ScrollX >> 8;
                bounds[1] += ScrollY >> 8;
                sprite = info->sprite;
                MapDrawWidth = sprite->width;
                MapDrawHeight = sprite->height;
                PrintScaledSprite(info->sprite, ((bounds[0] - MapWorldMinX) * scale_x) >> 16,
                    (((bounds[1] - MapWorldMinY) * scale_y) >> 16) + MapCenterOffsetY,
                    (MapDrawWidth * scale_x) >> 16, (MapDrawHeight * scale_y) >> 16);
            } else {
                /* A layered sprite: draw each part at its own offset. */
                int i;

                for (i = 0; i < ((struct Sprite *)info->sprite)->group->count; i++) {
                    struct Sprite *sub;
                    int sub_x;
                    int sub_y;
                    volatile int hx; /* volatile: last resort, the natural forms tried are in docs/attempts */
                    int hy;
                    int ox;
                    int oy;
                    struct Point pt;

                    sub = ((struct Sprite *)info->sprite)->group->subs[i];
                    sub_x = ((struct Sprite *)info->sprite)->group->xoffs[i];
                    sub_y = ((struct Sprite *)info->sprite)->group->yoffs[i];
                    pt.x = uid.pos.x;
                    pt.y = uid.pos.y;
                    GetTileBounds(&pt, bounds);
                    hx = HALF(sub_x);
                    hy = HALF(sub_y);
                    ox = info->x;
                    oy = info->y;
                    ox = HALF(ox);
                    oy = HALF(oy);
                    bounds[0] += ox;
                    bounds[1] += oy;
                    bounds[0] += hx;
                    bounds[1] += hy;
                    bounds[0] += ScrollX >> 8;
                    bounds[1] += ScrollY >> 8;
                    MapDrawWidth = sub->width;
                    MapDrawHeight = sub->height;
                    PrintScaledSprite(sub, ((bounds[0] - MapWorldMinX) * scale_x) >> 16,
                        (((bounds[1] - MapWorldMinY) * scale_y) >> 16) + MapCenterOffsetY,
                        (MapDrawWidth * scale_x) >> 16, (MapDrawHeight * scale_y) >> 16);
                }
            }
        }
        PopRenderingStatus();
        lpConfig->view_x = saved_view_x;
        lpConfig->view_y = saved_view_y;
        RestoreClipping();
        CommitCliprectToHardware();
        if (blob != NULL) {
            KillSprite(blob);
        }
        if (stick != NULL) {
            KillSprite(stick);
        }
        if (lights != NULL) {
            KillSprite(lights);
        }
        DeleteObject(pen);
        CalculateMapRenderOrder();
    }
}
