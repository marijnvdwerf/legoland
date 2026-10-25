#include <stdlib.h>
#include <string.h>
#include "legoland.h"
#include "math.h"

#include "bloke.h"
#include "build.h"
#include "clipping.h"
#include "draw.h"
#include "gamemain.h"
#include "gamemap.h"
#include "globals.h"
#include "llidb.h"
#include "map_object.h"
#include "obj_instance.h"
#include "pathfind.h"
#include "print_sprite.h"
#include "tilemap.h"
#include "timer.h"
#include "worker.h"

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0045a9b0
LEGO_EXPORT unsigned int *AllocTileSpace(void *manager, int count, unsigned int *out) {
    struct FXSpriteList *src = (struct FXSpriteList *)manager;
    int base;
    int run;
    struct Sprite **slot;
    struct Sprite **cursor;
    unsigned int i;
    INT_PTR slot_value;

    count = count & 0xffff;
    base = 0;
    while (count + base <= 0x800) {
        slot = &TileSpriteArray[base];
        run = 0;
        slot_value = (INT_PTR)TileSpriteArray[base];
        while (slot_value == -1) {
            run++;
            slot++;
            if (run >= count) {
                cursor = &TileSpriteArray[base];
                memset(cursor, 0, (unsigned int)count * sizeof(struct Sprite *));
                for (i = 0; (int)i < count; i++) {
                    TileSpriteInfo[base + i].src = src;
                    if (src != NULL) {
                        TileSpriteInfo[base + i].sprite = (unsigned short)src->sprite_ids[i];
                    } else {
                        TileSpriteInfo[base + i].sprite = 0;
                    }
                }
                *(unsigned short *)out = (unsigned short)base;
                return (unsigned int *)&TileSpriteArray[base];
            }
            slot_value = (INT_PTR)*slot;
        }
        base = run + base + 1;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0045aa50
int CollectTileSpriteLists(struct FXSpriteList **out) {
    int count = 0;
    struct FXSpriteList *prev = NULL;
    struct FXSpriteList *src;
    struct FXSpriteList **dst = out;
    struct TileSpriteEntry *info = TileSpriteInfo;
    void **slot = (void **)TileSpriteArray;

    do {
        if (*slot != (void *)-1) {
            src = info->src;
            if (src != NULL && src != prev) {
                *dst = src;
                count++;
                prev = src;
                dst++;
            }
        }
        slot++;
        info++;
    } while ((INT_PTR)slot < (INT_PTR)&TileSpriteArray[2048]); /* the original's end: MapRenderOrderList, next to it */
    return count;
}

// FUNCTION: LEGOLAND 0x0045aa90
LEGO_EXPORT void FreeTileSpace(unsigned short index, unsigned short count) {
    memset(&TileSpriteArray[index], 0xFF, count * sizeof(struct Sprite *));
}

// FUNCTION: LEGOLAND 0x0045aad0
LEGO_EXPORT unsigned int LoadMapTiles(void) {
    unsigned short alloc_index;
    struct Element *handle;
    int offset;
    unsigned char *row;
    UINT_PTR p;

    if (GameMapRawBlock == 0) {
        p = (UINT_PTR)calloc(0x140000 + 0x100 * sizeof(void *) + 0x1f, 1);
        GameMap = (struct MapElement **)((p + 0x1f) & ~(UINT_PTR)0x1f);
        row = (unsigned char *)((p + (0x100 * sizeof(void *) + 0x1f)) & ~(UINT_PTR)0x1f);
        GameMapRawBlock = (void *)p;
        offset = 0;
        do {
            *(unsigned char **)((char *)GameMap + offset) = row;
            offset += (int)sizeof(void *);
            row += 0x1400;
        } while (offset < (int)(0x100 * sizeof(void *)));
    }
    FreeTileSpace(0, 0x800);
    AllocTileSpace(0, 1, (unsigned int *)&alloc_index);
    LLIDB_FindElement("MAPPING 1", &handle, 0);
    LLIDB_LoadData((void *)handle);
    LLIDB_FindElement("BASIC TILES 1", &handle, 0);
    BasicTiles1Data = handle->data;
    LevelBaseTileId = *(unsigned int *)BasicTiles1Data;
    BasicTileSlot0 = 0;
    BasicTileSlot1 = 1;
    BasicTileSlot2 = 2;
    BasicTileSlot3 = 3;
    BasicTileSlot4 = 4;
    LLIDB_FindElement("NORMAL PATH TILES", &handle, 0);
    PathSprite = handle->data;
    // STRING: LEGOLAND 0x004b9bc4
    Arrow01Sprite = LoadSprite("arrow01.lls", 1);
    // STRING: LEGOLAND 0x004b9bb8
    Arrow02Sprite = LoadSprite("arrow02.lls", 1);
    // STRING: LEGOLAND 0x004b9bac
    Arrow03Sprite = LoadSprite("arrow03.lls", 1);
    // STRING: LEGOLAND 0x004b9ba0
    Arrow04Sprite = LoadSprite("arrow04.lls", 1);
    return 1;
}

// FUNCTION: LEGOLAND 0x0045ac20
unsigned int UnloadMapTiles(void) {
    struct Element *handle;

    if (GameMapRawBlock != 0) {
        free(GameMapRawBlock);
    }
    // STRING: LEGOLAND 0x004b9be4
    LLIDB_FindElement("MAPPING 1", &handle, 0);
    LLIDB_UnLoadData(handle);
    if (Arrow01Sprite != 0) {
        KillSprite(Arrow01Sprite);
        Arrow01Sprite = 0;
    }
    if (Arrow02Sprite != 0) {
        KillSprite(Arrow02Sprite);
        Arrow02Sprite = 0;
    }
    if (Arrow03Sprite != 0) {
        KillSprite(Arrow03Sprite);
        Arrow03Sprite = 0;
    }
    if (Arrow04Sprite != 0) {
        KillSprite(Arrow04Sprite);
        Arrow04Sprite = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0045acc0
LEGO_EXPORT void GetTileBounds(struct Point *ref, int *out) {
    struct Sprite *sprite = (struct Sprite *)TileSpriteArray[LevelBaseTileId];
    short size = sprite->height;

    out[0] = (short)(((short)(size * 2) + 1) >> 1) * ((ref->x - ref->y) - 1) + lpConfig->view_x - (ScrollX >> 8);
    out[1] = (short)((size + 1) >> 1) * (ref->x + ref->y) + lpConfig->view_y - (ScrollY >> 8);
    out[2] = (short)(size * 2) - 1 + out[0];
    out[3] = out[1] - 1 + size;
}

// FUNCTION: LEGOLAND 0x0045ad60
LEGO_EXPORT void GetTileCentre(struct Point *ref, int *out) {
    struct Sprite *sprite = (struct Sprite *)TileSpriteArray[LevelBaseTileId];
    short size = sprite->height;
    int y;

    out[0] = (short)(((short)(size * 2) + 1) >> 1) * (ref->x - ref->y) + lpConfig->view_x - (ScrollX >> 8);
    y = (short)((size + 1) >> 1) * (ref->y + 1 + ref->x) + lpConfig->view_y;
    out[1] = y - (ScrollY >> 8);
}

// FUNCTION: LEGOLAND 0x0045ade0
void DrawMarkedTileHighlights(void) {
    RECT clip;
    short tile_h;
    int size; /* tile height */
    int dbl; /* tile width (2 * height) */
    int half_y; /* half a tile, vertically */
    int half_x; /* half a tile, horizontally */
    int scroll_x;
    int scroll_y;
    int col_q;
    int x_phase; /* scroll offset inside the first tile column */
    int y_phase; /* scroll offset inside the first tile row */
    int row_q;
    int row; /* map y of the tile being drawn (even half-rows) */
    int saved_col; /* map x of the first tile of the current screen row */
    int saved_row; /* map y of the first tile of the current screen row */
    int x;
    int y; /* screen y of the current row */
    int which;
    struct Point cell; /* map tile being drawn (x always; y for the odd half-rows) */
    struct MapElement tile;

    clip.left = lpConfig->view_x;
    clip.top = lpConfig->view_y;
    clip.right = lpConfig->screen_width;
    clip.bottom = lpConfig->screen_height;
    SetClipping(&clip);

    /* Scroll position -> first visible map tile (same maths as the map renderers). */
    tile_h = ((struct Sprite *)TileSpriteArray[LevelBaseTileId])->height;
    dbl = (short)(tile_h * 2);
    size = tile_h;
    scroll_x = ScrollX >> 8;
    half_y = (tile_h + 1) >> 1;
    if (0) {
        /* never runs: half_y is address-taken in the original, so it stays in memory and
           y + half_y is recomputed in the odd half-row loop */
        SetClipping((RECT *)&half_y);
    }
    scroll_y = (ScrollY >> 8) - half_y;
    col_q = scroll_x / dbl;
    half_x = (dbl + 1) >> 1;
    x_phase = scroll_x % dbl;
    row_q = scroll_y / size;
    y_phase = scroll_y % size;
    cell.x = row_q + col_q - 3;
    row = row_q - col_q;
    which = (x_phase >= half_x) + 1;
    if (y_phase > half_y) {
        which += 2;
    }
    switch (which) {
    case 1:
        if (x_phase < half_x - y_phase * 2) {
            cell.x--;
            x_phase += half_x;
            y_phase += half_y;
        }
        break;
    case 2:
        if (x_phase >= half_x + y_phase * 2) {
            x_phase -= half_x;
            row--;
            y_phase += half_y;
        }
        break;
    case 3:
        if (x_phase < half_x + (y_phase - size) * 2) {
            row++;
            x_phase += half_x;
            y_phase -= half_y;
        }
        break;
    case 4:
        if (x_phase >= half_x + (size - y_phase) * 2) {
            cell.x++;
            x_phase -= half_x;
            y_phase -= half_y;
        }
        break;
    }

    /* Walk the visible tiles in diagonal rows (x+1, y-1 per screen column), two half-rows per
       screen row, and draw the tinted highlight tile on every tile marked with field_10 & 2. */
    for (y = clip.top - size * 2 - y_phase; y < clip.bottom + size * 2; y += size) {
        /* even half-row */
        saved_col = cell.x;
        saved_row = row;
        for (x = clip.left - dbl * 2 - x_phase; x < dbl * 2 + clip.right; x += dbl) {
            if (cell.x >= 0 && cell.x < lpConfig->width && row >= 0 && row < lpConfig->height) {
                tile = GameMap[row][cell.x];
            } else {
                tile.field_10 = 0;
            }
            if (tile.field_10 & 2) {
                PrintSprite(TileSpriteArray[(BasicTileSlot3 & 0xff) + *BasicTiles1Data], x, y, 0xff6868, 0);
            }
            cell.x++;
            row--;
        }
        /* odd half-row, offset by half a tile */
        cell.x = saved_col + 1;
        cell.y = saved_row;
        for (x = clip.left - dbl * 2 - x_phase; x < dbl * 2 + clip.right; x += dbl) {
            if (cell.x >= 0 && cell.x < lpConfig->width && cell.y >= 0 && cell.y < lpConfig->height) {
                tile = GameMap[cell.y][cell.x];
            } else {
                tile.field_10 = 0;
            }
            if (tile.field_10 & 2) {
                PrintSprite(TileSpriteArray[(BasicTileSlot3 & 0xff) + *BasicTiles1Data], x + half_x, y + half_y, 0xff6868, 0);
            }
            cell.x++;
            cell.y--;
        }
        cell.x = saved_col + 1;
        row = saved_row + 1;
    }
}

/* Empty in the original (does nothing). */
// FUNCTION: LEGOLAND 0x0045b170
void Empty_0045b170(struct Point *pt) {
}

/* RideSpriteInfo followed by one more zeroed word (RenderView local copy). */
struct RenderSprite {
    RideSpriteInfo info;
    unsigned int field_14;
};

#define HALF(v) ((v) < 0 ? -(-(v) >> 1) : (v) >> 1)

static __inline struct MapElement *GetTile(int x, int y) {
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        return 0;
    }
    return &GameMap[y][x];
}

// FUNCTION: LEGOLAND 0x0045b180
LEGO_EXPORT void RenderView(void) {
    struct MapElement *list[3000];
    RECT clip;
    struct HitInfo hit_a;
    struct HitInfo hit_b;
    int bounds[4];
    int b[4];
    struct Ride *rides;
    RECT rect;
    int count;
    TileId uid;
    struct Point pt;
    int w; /* tile width in pixels */
    int h; /* tile height in pixels */
    int hw; /* half width */
    int hh; /* half height */
    int q; /* scroll x in tiles */
    int ty; /* scroll y in tiles */
    int px; /* scroll position in pixels */
    int py;
    int rx; /* pixel offset inside the tile */
    int ry;
    int sel; /* quadrant of (rx, ry) inside the tile */
    int xlim;
    int ylim;
    int x;
    int y;
    int savex;
    int savey;
    int i;
    struct MapElement *tile;
    struct MapElement *anchor;

    rect.left = lpConfig->view_x;
    rect.top = lpConfig->view_y;
    rect.right = lpConfig->view_width + lpConfig->view_x;
    rect.bottom = lpConfig->view_height + lpConfig->view_y;
    count = 0;
    rides = ObjectClassList;
    BGFullUpdate = 1;
    UnusedRenderViewCounter = 0;
    GetClipping(&clip);
    DrawViewMapTiles();
    PrintBackground(BackgroundOffsetX, BackgroundOffsetY);
    if (EditMode.unk0 == 2 || (EditMode.unk0 == 1 && EditMode.unk8 == PathControlObject)) {
        ShowPathTileOverlay = 1;
        PathOverlayBlinkMask = (GetTickCount() & 0x100) ? 0xff : 0;
    } else {
        ShowPathTileOverlay = 0;
    }
    SetClipping(&rect);

    /* Scroll position -> first map tile of the view (same maths as DrawMarkedTileHighlights). */
    h = ((struct Sprite *)TileSpriteArray[LevelBaseTileId])->height;
    w = (short)(((struct Sprite *)TileSpriteArray[LevelBaseTileId])->height * 2);
    px = ScrollX >> 8;
    q = px / w;
    hw = (w + 1) >> 1;
    hh = (h + 1) >> 1;
    py = (ScrollY >> 8) - hh;
    rx = px % w;
    ty = py / h;
    ry = py % h;
    pt.x = q + ty - 3;
    pt.y = ty - q;
    sel = (rx >= hw) + 1;
    if (ry > hh) {
        sel += 2;
    }
    switch (sel) {
    case 1:
        if (rx < hw - ry * 2) {
            pt.x--;
            rx += hw;
            ry += hh;
        }
        break;
    case 2:
        if (rx >= hw + ry * 2) {
            pt.y--;
            rx -= hw;
            ry += hh;
        }
        break;
    case 3:
        if (rx < hw + (ry - h) * 2) {
            pt.y++;
            rx += hw;
            ry -= hh;
        }
        break;
    case 4:
        if (rx >= hw + (h - ry) * 2) {
            pt.x++;
            rx -= hw;
            ry -= hh;
        }
        break;
    }

    /* Walk the visible tiles in diagonal rows, two tiles per screen column (x+1, then y-1);
       list each object once (0x400 marks it as listed). */
    ylim = lpConfig->view_height + rect.bottom;
    xlim = w * 2 + rect.right;
    for (y = rect.top - h * 2 - ry; y < ylim; y += h) {
        savex = pt.x;
        savey = pt.y;
        for (x = rect.left - w * 2 - rx; x < xlim; x += w) {
            tile = GetTile(pt.x, pt.y);
            if (tile != NULL) {
                Empty_0045b170(&pt);
                if (tile->flags & 0xa0) {
                    anchor = GetTile(tile->field_4, tile->field_5);
                    if ((anchor->flags & 0x400) == 0) {
                        list[count++] = anchor;
                        anchor->flags |= 0x400;
                    }
                }
            }
            pt.x++;
            if (tile != NULL) {
                if (pt.x == (int)lpConfig->width) {
                    break;
                }
                tile++;
            }
            if (tile == NULL) {
                tile = GetTile(pt.x, pt.y);
            }
            if (tile != NULL) {
                Empty_0045b170(&pt);
                if (tile->flags & 0xa0) {
                    anchor = GetTile(tile->field_4, tile->field_5);
                    if ((anchor->flags & 0x400) == 0) {
                        list[count++] = anchor;
                        anchor->flags |= 0x400;
                    }
                }
            }
            pt.y--;
        }
        pt.x = savex + 1;
        pt.y = savey + 1;
    }

    /* Pre-render callback of each object class. */
    for (; rides != NULL; rides = rides->next) {
        if ((rides->flags & 0x20) && rides->cb_pre_render != NULL) {
            rides->cb_pre_render(rides->element);
        }
    }

    /* Draw every listed object, sliced into vertical strips for depth sorting. */
    list[count] = NULL;
    for (i = 0; i < count; i++) {
        struct RenderSprite loc = {0};
        struct Ride *ride;
        RideSpriteInfo *info;
        unsigned short flags;
        int color; /* strip code: twice the smaller corner offset; drawing: tint colour */
        int step; /* depth key step between strips */
        RECT *prev;
        int left; /* screen x of the object's left corner */
        int half;
        int ya; /* depth key of the object's left corner */
        int right; /* screen x of its right corner */
        int yb; /* depth key of its right corner */
        int dx_a;
        int dx_b;
        int key;
        int quarter3;
        RECT *part;
        unsigned int k;
        struct Point corner;

        tile = list[i];
        tile->flags &= 0xfbff;
        if (tile->field_0 == NULL) {
            continue;
        }
        ride = tile->field_0->ride;
        uid = tile->anchor;
        ObjectPartCount = 0;
        corner.x = tile->field_4 + ride->footprint.x0;
        corner.y = tile->field_5 + ride->footprint.y1;
        GetTileBounds(&corner, b);
        left = b[0];
        ya = ((b[1] + b[3]) >> 1) - lpConfig->view_y;
        corner.x = tile->field_4 + ride->footprint.x1;
        corner.y = tile->field_5 + ride->footprint.y0;
        GetTileBounds(&corner, b);
        right = b[2];
        yb = ((b[1] + b[3]) >> 1) - lpConfig->view_y;
        if (ya != yb) {
            corner.x = tile->field_4 + ride->footprint.x0;
            corner.y = tile->field_5 + ride->footprint.y0;
            GetTileBounds(&corner, b);
            dx_a = (b[0] + b[2]) >> 1;
            corner.x = tile->field_4 + ride->footprint.x1;
            corner.y = tile->field_5 + ride->footprint.y1;
            GetTileBounds(&corner, b);
            dx_a -= left;
            dx_b = ((b[0] + b[2]) >> 1) - left;
            if (dx_b < dx_a) {
                color = dx_b * 2;
                step = -(color / 4);
            } else {
                color = dx_a * 2;
                step = color / 4;
            }
            key = ya;
            ObjectPartKey[ObjectPartCount] = key;
            key += step;
            part = &ObjectPartArray[ObjectPartCount++];
            part->top = rect.top;
            part->bottom = rect.bottom;
            part->left = rect.left;
            quarter3 = color * 3 / 4;
            part->right = left + quarter3;
            if (part->right + quarter3 < right) {
                ObjectPartKey[ObjectPartCount] = key;
                key += step;
                part = &ObjectPartArray[ObjectPartCount++];
                part->top = rect.top;
                part->left = left + quarter3;
                part->bottom = rect.bottom;
                part->right = color * 5 / 4 + left;
            }
            if (part->right + quarter3 < right) {
                half = color / 2;
                do {
                    prev = part;
                    ObjectPartKey[ObjectPartCount] = key;
                    key += step;
                    part = &ObjectPartArray[ObjectPartCount++];
                    part->top = rect.top;
                    part->bottom = rect.bottom;
                    part->left = prev->left + half;
                    part->right = prev->right + half;
                } while (part->right + quarter3 < right);
            }
            ObjectPartKey[ObjectPartCount] = yb;
            prev = part;
            part = &ObjectPartArray[ObjectPartCount++];
            part->top = rect.top;
            part->bottom = rect.bottom;
            part->left = prev->right;
            part->right = rect.right;
        }
        if (ride->flags & 0x400) {
            if (ride->cb_sprite == NULL) {
                continue;
            }
            info = ride->cb_sprite(ride->element, uid.id);
            if (info == NULL) {
                continue;
            }
        } else {
            loc.info.sprite = ride->layer;
            loc.info.x = ride->field_14;
            loc.info.y = ride->field_18;
            loc.info.field_10 = 0;
            info = &loc.info;
        }
        corner.x = tile->field_4;
        corner.y = tile->field_5;
        GetTileBounds(&corner, bounds);
        flags = tile->flags;
        if (flags & 0x20) {
            /* Under construction: building animation frame, green tint when idle. */
            int sx;
            int sy;
            int hx; /* half the sprite offset */
            int hy;

            color = info->field_10;
            hit_a.element = tile->field_0;
            hit_a.coords = tile->anchor.id;
            hit_a.type = 0x104;
            if (ride->anim != NULL) {
                hx = ride->anim_dx + info->x;
                hy = ride->anim_dy + info->y;
                hx = HALF(hx);
                hy = HALF(hy);
                sx = bounds[0] + hx;
                sy = bounds[1] + hy;
                info->sprite = ride->anim;
                SetOverrideFrame(GetBuildAnimFrame(ride, uid));
            } else {
                sx = info->x;
                sy = info->y;
                sx = HALF(sx);
                sy = HALF(sy);
                sx += bounds[0];
                sy += bounds[1];
                color = 0xff00;
            }
            if (ObjectPartCount != 0) {
                for (k = 0; k < ObjectPartCount; k++) {
                    SortClippedSprite(info->sprite, sx, sy, ObjectPartKey[k], &ObjectPartArray[k], color, &hit_a);
                }
            } else {
                SortSprite(info->sprite, sx, sy, ya, color, &hit_a);
            }
        } else {
            int sx;
            int sy;
            int hx;
            int hy;

            color = info->field_10;
            hit_b.element = tile->field_0;
            hit_b.coords = tile->anchor.id;
            hit_b.type = 0x103;
            hx = info->x;
            hy = info->y;
            hx = HALF(hx);
            hy = HALF(hy);
            sx = bounds[0] + hx;
            sy = hy + bounds[1];
            if ((flags & 4) == 0) {
                if ((flags & 0x200) && GetBlink()) {
                    color = 0xff0000;
                } else if (MapStats.power_enabled != 0 && (tile->flags & 0x100) && !GetBlink()) {
                    color = 0xffff;
                }
            }
            if (ObjectPartCount != 0) {
                for (k = 0; k < ObjectPartCount; k++) {
                    SortClippedSprite(info->sprite, sx, sy, ObjectPartKey[k], &ObjectPartArray[k], color, &hit_b);
                }
            } else {
                SortSprite(info->sprite, sx, sy, ya, color, &hit_b);
            }
        }
        ClearOverrideFrame();
    }
    /* People and workers are sorted in with the objects. */
    RenderPeople();
    RenderWorkers();
    DrawAndClearPrintList();
    for (i = 0; i < count; i++) {
        tile = list[i];
        if (tile->field_0 != NULL && (tile->flags & 0x20)) {
            uid = tile->anchor;
            DoBuildEffects(tile->field_0->ride, uid);
        }
    }
    if (ShowPathTileOverlay != 0) {
        PushRenderingStatusAndLockVideoSurface();
        DrawViewPathTiles();
        DrawDeferredSprites();
        PopRenderingStatus();
    }
    RenderWorkerInterfaceGFX();
    SetClipping(&clip);
}

// FUNCTION: LEGOLAND 0x0045bcd0
LEGO_EXPORT void PointToIsoPlane(int *pt, int *out) {
    short tile_h;
    short tile_w;
    int half_w;
    int half_h;
    int x;
    int y;
    int col;
    int row;
    int rem_x;
    int rem_y;
    int quadrant;

    tile_h = (short)TileSpriteArray[LevelBaseTileId]->height;
    tile_w = tile_h * 2;
    half_w = (tile_w + 1) >> 1;
    half_h = (tile_h + 1) >> 1;
    x = pt[0] + half_w;
    y = pt[1];
    col = x / tile_w;
    rem_x = x % tile_w;
    row = y / tile_h;
    rem_y = y % tile_h;
    out[0] = row + col;
    out[1] = row - col;
    if (rem_x < 0) {
        rem_x += tile_w - 2;
        out[1] = row - col + 1;
        out[0] = row + col - 1;
    }
    if (rem_y < 0) {
        out[1]--;
        out[0]--;
        rem_y += tile_h - 1;
    }
    /* which quarter of the tile rectangle the point is in */
    quadrant = (rem_x >= half_w) + 1;
    if (rem_y > half_h) {
        quadrant += 2;
    }
    switch (quadrant) {
    case 1:
        if (rem_x < half_w - rem_y * 2) {
            out[0]--;
        }
        break;
    case 2:
        if (rem_x >= half_w + rem_y * 2) {
            out[1]--;
        }
        break;
    case 3:
        if (rem_x < half_w + (rem_y - tile_h) * 2) {
            out[1]++;
        }
        break;
    case 4:
        if (rem_x >= half_w + (tile_h - rem_y) * 2) {
            out[0]++;
        }
        break;
    }
}

// FUNCTION: LEGOLAND 0x0045be00
LEGO_EXPORT unsigned int ScreenToMapRef2(struct Point *screen, struct Point *out) {
    struct Sprite *sprite;
    short size;
    short twice;
    int ix;
    int iy;

    sprite = (struct Sprite *)TileSpriteArray[LevelBaseTileId];
    if (sprite == NULL) {
        return 0xffffffff;
    }
    size = sprite->height;
    twice = size * 2;
    ix = ((twice + 1 >> 1) - lpConfig->view_x);
    ix += ScrollX >> 8;
    ix = (ix + screen->x) * (0x100 / twice);
    iy = (((ScrollY >> 8) - lpConfig->view_y) + screen->y) * (0x100 / (int)size);
    out->x = iy + ix;
    out->y = iy - ix;
    return 0;
}

// FUNCTION: LEGOLAND 0x0045be90
LEGO_EXPORT unsigned int ScreenToMapRef(int *param_1, int *out, unsigned int param_3) {
    struct Sprite *sprite;
    short size;
    short twice;
    int w;
    int hy;
    int half;
    int sx;
    int sy;
    int qx;
    int rx;
    int qy;
    int ry;
    int sel;

    sprite = (struct Sprite *)TileSpriteArray[LevelBaseTileId];
    if (sprite == NULL) {
        return 0xffffffff;
    }
    do {
        size = sprite->height;
        w = (int)size;
        twice = size * 2;
        hy = w + 1 >> 1;
        half = twice + 1 >> 1;
        sx = ((ScrollX >> 8) - lpConfig->view_x) + *param_1 + half;
        sy = ((ScrollY >> 8) - lpConfig->view_y) + param_1[1];
        qx = sx / twice;
        rx = sx % twice;
        qy = sy / w;
        ry = sy % w;
        *out = qx + qy;
        out[1] = qy - qx;
        if (rx < 0) {
            rx = rx + -2 + twice;
            out[1] = (qy - qx) + 1;
            *out = (qx + qy) + -1;
        }
        if (ry < 0) {
            out[1] = out[1] + -1;
            *out = *out + -1;
            ry = ry + -1 + w;
        }
        sel = (rx >= half) + 1;
        if (ry > hy) {
            sel += 2;
        }
        switch (sel) {
        case 1:
            if (rx < half + ry * -2) {
                *out = *out + -1;
            }
            break;
        case 2:
            if (rx >= half + ry * 2) {
                out[1] = out[1] + -1;
            }
            break;
        case 3:
            if (rx < half + (ry - w) * 2) {
                out[1] = out[1] + 1;
            }
            break;
        case 4:
            if (rx >= half + (w - ry) * 2) {
                *out = *out + 1;
            }
        }
    } while (0);
    return 1;
}

// FUNCTION: LEGOLAND 0x0045c010
LEGO_EXPORT unsigned char Dir_To_Bit(unsigned char param) {
    return DirectionBits[param & 7];
}

// FUNCTION: LEGOLAND 0x0045c020
LEGO_EXPORT unsigned char Bit_To_Dir(unsigned char bit) {
    unsigned char result;

    for (result = 0; result < 8; result++) {
        if (DirectionBits[result] & bit) {
            return result;
        }
    }
    return 8;
}

// FUNCTION: LEGOLAND 0x0045c050
LEGO_EXPORT unsigned char Get_Path_Directions(struct Point *pos, char *param_2, char *param_3) {
    int x;
    INT_PTR y;
    int yp1;
    int xp1;
    int xm1;
    char local_15;
    char local_16;
    unsigned char local_17;
    struct MapElement tile;

    y = pos->y;
    x = pos->x;
    pos = (struct Point *)(y + -1);
    local_15 = '\0';
    local_16 = '\0';
    local_17 = 0;
    if (x < 0 || x >= (int)lpConfig->width || (INT_PTR)pos < 0 || (INT_PTR)pos >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[(INT_PTR)pos][x];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_16 = '\x01';
        local_17 = 1;
    }
    yp1 = y + 1;
    if (x < 0 || x >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[yp1][x];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 0x10;
    }
    xp1 = x + 1;
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[y][xp1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 4;
    }
    xm1 = x + -1;
    if (xm1 < 0 || xm1 >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[y][xm1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 0x40;
    }
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || (INT_PTR)pos < 0 || (INT_PTR)pos >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[(INT_PTR)pos][xp1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_15 = '\x01';
        local_17 = local_17 | 2;
    }
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[yp1][xp1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 8;
    }
    if (xm1 < 0 || xm1 >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[yp1][xm1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 0x20;
    }
    if (xm1 < 0 || xm1 >= (int)lpConfig->width || (INT_PTR)pos < 0 || (INT_PTR)pos >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[(INT_PTR)pos][xm1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 0x80;
    }
    if (param_2 != NULL) {
        *param_2 = local_16;
    }
    if (param_3 != NULL) {
        *param_3 = local_15;
    }
    return local_17;
}

// FUNCTION: LEGOLAND 0x0045c440
unsigned char GetPathNeighbours8(int *param_1, char *param_2, char *param_3) {
    int x;
    INT_PTR y;
    int yp1;
    int xp1;
    int xm1;
    char local_15;
    char local_16;
    unsigned char local_17;
    struct MapElement tile;

    y = param_1[1];
    x = *param_1;
    param_1 = (int *)(y + -1);
    local_15 = '\0';
    local_16 = '\0';
    local_17 = 0;
    if (x < 0 || x >= (int)lpConfig->width || (INT_PTR)param_1 < 0 || (INT_PTR)param_1 >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[(INT_PTR)param_1][x];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_16 = '\x01';
        local_17 = 1;
    }
    yp1 = y + 1;
    if (x < 0 || x >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[yp1][x];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 0x10;
    }
    xp1 = x + 1;
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[y][xp1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 4;
    }
    xm1 = x + -1;
    if (xm1 < 0 || xm1 >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[y][xm1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_16 = local_16 + '\x01';
        local_17 = local_17 | 0x40;
    }
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || (INT_PTR)param_1 < 0 || (INT_PTR)param_1 >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[(INT_PTR)param_1][xp1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_15 = '\x01';
        local_17 = local_17 | 2;
    }
    if (xp1 < 0 || xp1 >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[yp1][xp1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 8;
    }
    if (xm1 < 0 || xm1 >= (int)lpConfig->width || yp1 < 0 || yp1 >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[yp1][xm1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 0x20;
    }
    if (xm1 < 0 || xm1 >= (int)lpConfig->width || (INT_PTR)param_1 < 0 || (INT_PTR)param_1 >= (int)lpConfig->height) {
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[(INT_PTR)param_1][xm1];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        local_15 = local_15 + '\x01';
        local_17 = local_17 | 0x80;
    }
    if (param_2 != NULL) {
        *param_2 = local_16;
    }
    if (param_3 != NULL) {
        *param_3 = local_15;
    }
    return local_17;
}

// FUNCTION: LEGOLAND 0x0045c830
LEGO_EXPORT unsigned char ExcludeIsolatedDiags(unsigned char param) {
    unsigned char result = param;

    if ((result & 0x5) != 0x5) {
        result &= 0xfd;
    }
    if ((result & 0x41) != 0x41) {
        result &= 0x7f;
    }
    if ((result & 0x14) != 0x14) {
        result &= 0xf7;
    }
    if ((result & 0x50) != 0x50) {
        result &= 0xdf;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0045c870
LEGO_EXPORT void AdjustTileRFFlags(int *param_1) {
    struct MapElement *tile;
    unsigned char flags;
    char local_1;
    char dir;

    tile = &GameMap[param_1[1]][*param_1];
    tile->field_10 = tile->field_10 & 0xc3;
    flags = GetPathNeighbours8(param_1, &dir, &local_1);
    if (dir == '\0') {
        tile->field_10 = tile->field_10 | 0x10;
        return;
    }
    if (dir == '\x01') {
        tile->field_10 = tile->field_10 | 0x10;
        return;
    }
    if (dir == '\x02') {
        switch (flags & 0x11) {
        case 1:
        case 0x10:
            tile->field_10 = tile->field_10 | 8;
            return;
        }
    } else {
        if (dir == '\x03') {
            tile->field_10 = tile->field_10 | 4;
            return;
        }
        if (dir == '\x04') {
            tile->field_10 = tile->field_10 | 0x20;
        }
    }
}

// FUNCTION: LEGOLAND 0x0045c900
int IsMapRectAllPath(struct MapRect *param_1) {
    int x;
    unsigned int y;
    struct MapElement tile;

    x = param_1->x0;
    while (x <= param_1->x1) {
        for (y = param_1->y0; y <= param_1->y1; y++) {
            if (x >= 0 && x < (int)lpConfig->width && y >= 0 && y < (int)lpConfig->height) {
                tile = *(struct MapElement *)&GameMap[y][x];
            } else {
                tile.flags = 0x40;
                tile.field_10 = 0;
            }
            if ((tile.flags & 0x10) == 0 || (tile.field_10 & 2) != 0) {
                return 0;
            }
        }
        x++;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0045c9c0
unsigned int GetPathMask5x5(int *param_1) {
    int y;
    int yend;
    unsigned int result;
    unsigned int bit;
    volatile int x; /* volatile: the original re-reads this from its stack slot */
    int xoff;
    int xi;
    struct MapElement tile;

    y = param_1[1];
    result = 0;
    bit = 0x1000000;
    yend = y + 5;
    for (; y < yend; y++) {
        x = *param_1;
        xoff = x * 0x14;
        for (xi = x; xi < x + 5; xi++) {
            if (xoff < 0 || xi >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
                tile.flags = 0x40;
                tile.field_10 = 0;
            } else {
                tile = *(struct MapElement *)((char *)GameMap[y] + xoff);
            }
            if ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0) {
                result |= bit;
            }
            xoff += 0x14;
            bit >>= 1;
        }
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0045ca90
int FindPathSquare3x3(int *param_1, int *param_2) {
    unsigned int mask;
    int i;
    struct Point pt;

    pt.x = *param_1 + -2;
    pt.y = param_1[1] + -2;
    mask = GetPathMask5x5((int *)&pt);
    for (i = 0; i < 9; i++) {
        if ((PathSquare3x3Masks[i] & mask) == PathSquare3x3Masks[i]) {
            param_2[0] = PathSquare3x3OffsetsX[i] + param_1[0];
            param_2[1] = PathSquare3x3OffsetsY[i] + param_1[1];
            param_2[2] = param_2[0] + 2;
            param_2[3] = param_2[1] + 2;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045cb20
void PavePathRect(struct MapRect *param_1) {
    int x;
    int y;

    for (y = param_1->y0; y <= param_1->y1; y++) {
        for (x = param_1->x0; x <= param_1->x1; x++) {
            GameMap[y][x].field_8 = ((((x + y) & 1) ? 1 : 2) & 0xff) + *(unsigned short *)PathSprite;
        }
    }
}

// FUNCTION: LEGOLAND 0x0045cb90
void UnpavePathTile(struct Point *param) {
    struct MapElement *tile;
    unsigned short value;

    tile = (struct MapElement *)GameMap[param->y];
    value = *(unsigned short *)PathSprite;
    tile[param->x].field_8 = value;
}

// FUNCTION: LEGOLAND 0x0045cbc0
int GrowPathRectSide(int *param_1, int param_2) {
    int result;
    struct MapRect r;

    switch (param_2) {
    case 2:
        r.y0 = r.y1 = param_1[1] + -1;
        r.x0 = *param_1;
        r.x1 = param_1[2];
        result = IsMapRectAllPath(&r);
        if (result != 0) {
            param_1[1] = param_1[1] + -1;
            return 1;
        }
        break;
    case 0:
        r.y0 = r.y1 = param_1[3] + 1;
        r.x0 = *param_1;
        r.x1 = param_1[2];
        result = IsMapRectAllPath(&r);
        if (result != 0) {
            param_1[3] = param_1[3] + 1;
            return 1;
        }
        break;
    case 1:
        r.y0 = param_1[1];
        r.y1 = param_1[3];
        r.x1 = param_1[2] + 1;
        r.x0 = r.x1;
        result = IsMapRectAllPath(&r);
        if (result != 0) {
            param_1[2] = param_1[2] + 1;
            return 1;
        }
        break;
    case 3:
        r.y0 = param_1[1];
        r.y1 = param_1[3];
        r.x1 = *param_1 + -1;
        r.x0 = r.x1;
        result = IsMapRectAllPath(&r);
        if (result != 0) {
            *param_1 = *param_1 + -1;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045cd00
void GrowPathRect(int *buf) {
    int zero_count = 0;
    int count = 0;

    do {
        if (GrowPathRectSide(buf, count) != 0) {
            zero_count = 0;
        } else {
            zero_count++;
        }
        count++;
        count = count & 3;
    } while (zero_count < 4);
}

// FUNCTION: LEGOLAND 0x0045cd30
void PavePathSquareAt(int *arg) {
    struct MapRect buf;

    if (FindPathSquare3x3(arg, (int *)&buf) != 0) {
        GrowPathRect((int *)&buf);
        PavePathRect(&buf);
    }
}

// FUNCTION: LEGOLAND 0x0045cd70
void UnpaveNeighbourPathTiles(int *param_1) {
    struct Point pt;
    struct MapRect rect;

    pt.x = *param_1 - 2;
    while (pt.x <= *param_1 + 2) {
        pt.y = param_1[1] - 2;
        while (pt.y <= param_1[1] + 2) {
            if ((pt.x != *param_1 || pt.y != param_1[1]) && IsPavedPathTile((int *)&pt) != 0 &&
                FindPathSquare3x3((int *)&pt, (int *)&rect) == 0) {
                UnpavePathTile(&pt);
            }
            pt.y++;
        }
        pt.x++;
    }
}

// FUNCTION: LEGOLAND 0x0045ce10
int IsPathTile(struct MapElement *tile) {
    if ((tile->field_10 & 0x1) == 0) {
        if ((tile->flags & 0x10) != 0 && (tile->field_10 & 0x2) == 0) {
            return 1;
        }
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0045ce30
int IsPavedPathTile(int *param_1) {
    int x;
    int y;
    struct MapElement *tile;

    x = *param_1;
    if (x >= 0 && x < (int)lpConfig->width && (y = param_1[1], y >= 0) &&
        y < (int)lpConfig->height &&
        (tile = &GameMap[y][x], tile != NULL)) {
        if (IsPathTile(tile) != 0) {
            tile = &GameMap[param_1[1]][*param_1];
            return tile->field_8 != *(unsigned int *)PathSprite;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045ceb0
unsigned char GetPathNeighbourMask(int *coords) {
    int x;
    int y;
    unsigned char local_15;
    struct MapElement tile;

    local_15 = 0;
    x = *coords;
    y = coords[1] + -1;
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.field_8 = 0;
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[y][x];
    }
    if (IsPathTile(&tile) != 0) {
        local_15 = 1;
    }
    y = coords[1];
    x = *coords + 1;
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.field_8 = 0;
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[y][x];
    }
    if (IsPathTile(&tile) != 0) {
        local_15 = local_15 | 2;
    }
    x = *coords;
    y = coords[1] + 1;
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.field_8 = 0;
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[y][x];
    }
    if (IsPathTile(&tile) != 0) {
        local_15 = local_15 | 4;
    }
    x = *coords + -1;
    y = coords[1];
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        tile.field_8 = 0;
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[y][x];
    }
    if (IsPathTile(&tile) != 0) {
        local_15 = local_15 | 8;
    }
    return local_15;
}

// FUNCTION: LEGOLAND 0x0045d080
unsigned char GetPathDiagonalGaps(unsigned char flags, int *coords) {
    unsigned char result = 0;
    struct MapElement tile_data; /* a copy of the tile */
    int x, y;

    if ((flags & 0xc) == 0xc) {
        x = coords[0] - 1;
        y = coords[1] + 1;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            memcpy(&tile_data, &GameMap[y][x], sizeof(tile_data));
        } else {
            tile_data.field_8 = 0;
            tile_data.flags = 0x40;
            tile_data.field_10 = 0;
        }
        if (!IsPathTile(&tile_data)) {
            result = 1;
        }
    }
    if ((flags & 0x3) == 0x3) {
        x = coords[0] + 1;
        y = coords[1] - 1;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            memcpy(&tile_data, &GameMap[y][x], sizeof(tile_data));
        } else {
            tile_data.field_8 = 0;
            tile_data.flags = 0x40;
            tile_data.field_10 = 0;
        }
        if (!IsPathTile(&tile_data)) {
            result |= 2;
        }
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0045d1a0
LEGO_EXPORT unsigned short *AdjustPathTile(struct Point *p, unsigned short a) {
    unsigned short *flags;
    int x;
    int y;
    struct MapElement tile;

    BGFullUpdate = 1;
    x = p->x;
    if (x < 0 || x >= (int)lpConfig->width || (y = p->y, y < 0) || y >= (int)lpConfig->height) {
        tile.field_8 = 0;
        tile.flags = 0x40;
        tile.field_10 = 0;
    } else {
        tile = *&GameMap[y][x];
    }
    if ((tile.field_10 & 1) != 0 || ((tile.flags & 0x10) != 0 && (tile.field_10 & 2) == 0)) {
        AdjustTileRFFlags((int *)p);
    }
    flags = (unsigned short *)(INT_PTR)IsPathTile(&tile);
    if (flags != NULL) {
        flags = (unsigned short *)&GameMap[p->y][p->x].flags;
        *flags = *flags & 0xfffc;
    }
    return flags;
}

// FUNCTION: LEGOLAND 0x0045d260
void AdjustPathTileAndNeighbours(struct Point *param) {
    unsigned int edi = *(unsigned int *)PathSprite;
    struct Point point;

    AdjustPathTile(param, edi);

    point.x = param->x;
    point.y = param->y - 1;
    AdjustPathTile(&point, edi);

    point.x = param->x + 1;
    point.y = param->y;
    AdjustPathTile(&point, edi);

    point.x = param->x;
    point.y = param->y + 1;
    AdjustPathTile(&point, edi);

    point.x = param->x - 1;
    point.y = param->y;
    AdjustPathTile(&point, edi);

    point.x = param->x - 1;
    point.y = param->y + 1;
    AdjustPathTile(&point, edi);

    point.x = param->x + 1;
    point.y = param->y - 1;
    AdjustPathTile(&point, edi);

    point.x = param->x + 1;
    point.y = param->y + 1;
    AdjustPathTile(&point, edi);

    point.x = param->x - 1;
    point.y = param->y - 1;
    AdjustPathTile(&point, edi);
}

// FUNCTION: LEGOLAND 0x0045d350
LEGO_EXPORT void AddPathTileGFX(struct Point *p, unsigned short param1) {
    unsigned short *pb;

    pb = (unsigned short *)&GameMap[p->y][p->x].flags;
    *pb |= 0x10;
    *(unsigned short *)&GameMap[p->y][p->x].field_8 = param1;
    AdjustPathTileAndNeighbours(p);
    if (MapStats.field_184 != 0) {
        PavePathSquareAt((int *)p);
    }
}

// FUNCTION: LEGOLAND 0x0045d3b0
LEGO_EXPORT void AddPathTile(struct Point *p, unsigned short param1) {
    AddPathTileGFX(p, param1);
    /* struct Point and struct Point are identical {x,y} layouts; cast
       bridges tilemap's local Point to pathfind's InstancePos (no shared type). */
    AddPathSquare((struct Point *)p);
}

// FUNCTION: LEGOLAND 0x0045d3d0
void ClearPathUnderFootprint(struct Ride *param_1, int *param_2) {
    int x;
    int y;
    struct Point pt;

    if (param_1->type != 0 && param_1->type != 2) {
        for (y = param_2[1] + param_1->footprint.y0 - 1; y <= param_2[1] + 1 + param_1->footprint.y1; y++) {
            for (x = *param_2 + param_1->footprint.x0 - 1; x <= *param_2 + param_1->footprint.x1 + 1; x++) {
                GameMap[y][x].flags &= 0xffe7;
                GameMap[y][x].field_10 = 0;
                GameMap[y][x].field_8 = GameMap[y][x].field_a;
                pt.x = x;
                pt.y = y;
                AdjustPathTileAndNeighbours(&pt);
                RemovePathSquare(&pt);
            }
        }
        for (y = param_2[1] + param_1->footprint.y0; y <= param_2[1] + param_1->footprint.y1; y++) {
            for (x = *param_2 + param_1->footprint.x0; x <= *param_2 + param_1->footprint.x1; x++) {
                GameMap[y][x].flags &= 0xffe7;
                GameMap[y][x].field_10 = 0;
                GameMap[y][x].field_8 = GameMap[y][x].field_a;
                pt.x = x;
                pt.y = y;
                AdjustPathTileAndNeighbours(&pt);
                RemovePathSquare(&pt);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0045d560
int IntersectMapRect(struct MapRect *out, struct MapRect *a, struct MapRect *b) {
    if (a->x0 > b->x0) {
        out->x0 = a->x0;
    } else {
        out->x0 = b->x0;
    }
    if (a->x1 < b->x1) {
        out->x1 = a->x1;
    } else {
        out->x1 = b->x1;
    }
    if (a->y0 > b->y0) {
        out->y0 = a->y0;
    } else {
        out->y0 = b->y0;
    }
    if (a->y1 < b->y1) {
        out->y1 = a->y1;
    } else {
        out->y1 = b->y1;
    }
    if (out->x0 <= out->x1 && out->y0 <= out->y1) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045d5d0
void SubtractRectFromRectList(struct MapRect *param) {
    struct MapRect r;
    struct MapRect *cur;
    struct MapRect *slot;
    int i;
    struct MapRect old;

    i = 0;
    if (i < RectListCount) {
        cur = RectList;
        do {
            if (IntersectMapRect(&r, param, cur) != 0) {
                old = *cur;
                RectListCount--;
                if (i < RectListCount) {
                    *cur = RectList[RectListCount];
                }
                i--;
                cur--;
                if (old.y0 < r.y0) {
                    slot = &RectList[RectListCount++];
                    slot->x0 = old.x0;
                    slot->y0 = old.y0;
                    slot->x1 = old.x1;
                    slot->y1 = r.y0 - 1;
                }
                if (old.x0 < r.x0) {
                    slot = &RectList[RectListCount++];
                    slot->x0 = old.x0;
                    slot->y0 = r.y0;
                    slot->x1 = r.x0 - 1;
                    slot->y1 = r.y1;
                }
                if (r.x1 < old.x1) {
                    slot = &RectList[RectListCount++];
                    slot->y0 = r.y0;
                    slot->x0 = r.x1 + 1;
                    slot->x1 = old.x1;
                    slot->y1 = r.y1;
                }
                if (r.y1 < old.y1) {
                    slot = &RectList[RectListCount++];
                    slot->y0 = r.y1 + 1;
                    slot->x0 = old.x0;
                    slot->x1 = old.x1;
                    slot->y1 = old.y1;
                }
            }
            cur++;
            i++;
        } while (i < RectListCount);
    }
}

// FUNCTION: LEGOLAND 0x0045d730
int AddRectToRectList(struct MapRect *param) {
    int idx;
    struct MapRect *cur;

    SubtractRectFromRectList(param);
    idx = RectListCount;
    cur = &RectList[idx];
    cur->x0 = param->x0;
    cur->y0 = param->y0;
    cur->x1 = param->x1;
    idx = idx + 1;
    RectListCount = idx;
    cur->y1 = param->y1;
    return idx;
}

struct PathItem {
    /* 0x00 */ int x0;
    /* 0x04 */ int y0;
    /* 0x08 */ int x1;
    /* 0x0c */ int y1;
    /* 0x10 */ struct PathItem *next;
};

/* For a cursor chain with a part flagged 0x1000: lays path tiles over that part's footprint minus the footprints of
 * the parts before it (rect list), then fills the footprint interior with path graphics. */
// FUNCTION: LEGOLAND 0x0045d770
void LayCursorPathFootprint(struct Cursor *param_1) {
    struct Cursor *cur;
    struct Cursor *found;
    struct Footprint *item;
    struct MapRect rect;
    struct Point pt;
    int i;

    /* find the first cursor in the chain that has flag 0x1000 */
    found = NULL;
    for (cur = param_1; cur != NULL; cur = cur->next) {
        if (cur->field_1828 & 0x1000) {
            found = cur;
            break;
        }
    }
    if (found == NULL) {
        return;
    }

    RectListCount = 0;
    rect.x0 = found->footprint.x0 + found->tile_x;
    rect.y0 = found->footprint.y0 + found->tile_y;
    rect.x1 = found->footprint.x1 + found->tile_x;
    rect.y1 = found->footprint.y1 + found->tile_y;
    AddRectToRectList(&rect);

    /* cursors before the flagged one: collect their footprints */
    for (cur = param_1; cur != NULL; cur = cur->next) {
        if (cur->field_1828 & 0x1000) {
            found = cur;
            break;
        }
        rect.x0 = cur->footprint.x0 + cur->tile_x;
        rect.y0 = cur->footprint.y0 + cur->tile_y;
        rect.x1 = cur->footprint.x1 + cur->tile_x;
        rect.y1 = cur->footprint.y1 + cur->tile_y;
        SubtractRectFromRectList(&rect);
        for (item = &cur->footprint; item != NULL; item = item->next) {
            rect.x0 = item->x0 + cur->tile_x;
            rect.y0 = item->y0 + cur->tile_y;
            rect.x1 = item->x1 + cur->tile_x;
            rect.y1 = item->y1 + cur->tile_y;
            SubtractRectFromRectList(&rect);
        }
    }

    /* re-lay path tiles over every collected rectangle */
    for (i = 0; i < RectListCount; i++) {
        for (pt.y = RectList[i].y0; pt.y <= RectList[i].y1; pt.y++) {
            for (pt.x = RectList[i].x0; pt.x <= RectList[i].x1; pt.x++) {
                RemoveObjectAtPoint(&pt);
                GameMap[pt.y][pt.x].field_10 &= 0xfc;
                AddPathTileGFX(&pt, *(unsigned short *)PathSprite);
                ScriptDirtyCategories |= 0x10;
                AddPathSquare(&pt);
            }
        }
    }

    /* fill the interior of the flagged cursor's footprint */
    PathUpdateNeeded = 1;
    for (pt.y = found->footprint.y0 + found->tile_y + 1; pt.y <= found->footprint.y1 + found->tile_y - 1; pt.y++) {
        for (pt.x = found->footprint.x0 + found->tile_x + 1; pt.x <= found->footprint.x1 + found->tile_x - 1; pt.x++) {
            AddPathTileGFX(&pt, *(unsigned short *)PathSprite);
        }
    }
    PathUpdateNeeded = 1;
}

// RestoreBaseMap: puts the tile's base sprite (field_a) back as its current sprite (field_8).
// FUNCTION: LEGOLAND 0x0045da60
LEGO_EXPORT void RestoreBaseMap(int tile_x, int row_y) {
    struct MapElement *tile = &GameMap[row_y][tile_x];
    unsigned short id = tile->field_a;
    unsigned short flags = *(volatile unsigned short *)&TileSpriteInfo[id].sprite;

    tile->field_8 = id;
    (void)(flags & 0x20);
}

// FUNCTION: LEGOLAND 0x0045daa0
LEGO_EXPORT void RemovePathTile(int *param_1, unsigned short param_2) {
    unsigned short *flags;
    struct Point local_8;

    RestoreBaseMap(*param_1, param_1[1]);
    flags = (unsigned short *)&GameMap[param_1[1]][*param_1].flags;
    *flags = *flags & 0xffe4;
    *(unsigned char *)&GameMap[param_1[1]][*param_1].field_10 = 0;
    local_8.x = *param_1;
    local_8.y = param_1[1] + -1;
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1 + 1;
    local_8.y = param_1[1];
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1;
    local_8.y = param_1[1] + 1;
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1 + -1;
    local_8.y = param_1[1];
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1 + -1;
    local_8.y = param_1[1] + 1;
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1 + 1;
    local_8.y = param_1[1] + -1;
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1 + 1;
    local_8.y = param_1[1] + 1;
    AdjustPathTile(&local_8, param_2);
    local_8.x = *param_1 + -1;
    local_8.y = param_1[1] + -1;
    AdjustPathTile(&local_8, param_2);
    RemovePathSquare((struct Point *)param_1);
    if (MapStats.field_184 != 0) {
        UnpaveNeighbourPathTiles(param_1);
    }
}
