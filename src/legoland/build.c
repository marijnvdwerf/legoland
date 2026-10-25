#include "legoland.h"

#include "build.h"
#include "gfx.h"
#include "globals.h"
#include "image_sprite.h"
#include "llidb.h"
#include "map_object.h"
#include "obj_instance.h"
#include "objclass.h"
#include "render.h"
#include "render3d.h"
#include "tilemap.h"

// FUNCTION: LEGOLAND 0x00450b90
LEGO_EXPORT int AddObjectToBuildList(struct Ride *obj, TileId coords) {
    int i;

    if (BuildObjCount >= 256) {
        return 0;
    }
    for (i = 0; i < 256; i++) {
        if (BuildObjArray[i].ride == NULL) {
            break;
        }
    }
    if (i >= 256) {
        return 0;
    }
    BuildObjArray[i].ride = (struct Ride *)obj;
    BuildObjArray[i].coords = coords;
    BuildObjArray[i].elapsed = 0;
    BuildObjCount++;
    return 1;
}

// FUNCTION: LEGOLAND 0x00450c00
void RemoveObjectFromBuildList(TileId coords) {
    int i;
    BuildObj *b;

    i = 0;
    for (b = BuildObjArray; (INT_PTR)&b->coords < (INT_PTR)&ButtonRepeatDelay; b++, i++) {
        if (b->coords.id == coords.id) {
            BuildObjCount--;
            BuildObjArray[i].ride = NULL;
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x00450c40
LEGO_EXPORT int GetBuildTime(Ride *objClass) {
    int cost = GetObjCost(objClass);
    if (cost < 50) {
        return 50;
    }
    if (cost > 150) {
        return 150;
    }
    return cost;
}

/* Does nothing but return 0. */
// FUNCTION: LEGOLAND 0x00450c70
unsigned int ReturnZero_00450c70(Ride *ride) {
    return 0;
}

// FUNCTION: LEGOLAND 0x00450c80
LEGO_EXPORT void ProcessBuildingTimes(void) {
    int i;

    for (i = 0; i < 256; i++) {
        if (BuildObjArray[i].ride != NULL) {
            BuildObjArray[i].elapsed++;
            if (BuildObjArray[i].elapsed >= GetBuildTime(BuildObjArray[i].ride)) {
                BuildObjCount--;
                ObjectIsBuilt((struct Ride *)BuildObjArray[i].ride, BuildObjArray[i].coords);
                BuildObjArray[i].ride = NULL;
            } else {
                ObjectIsBuilding((struct Ride *)BuildObjArray[i].ride, BuildObjArray[i].coords);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00450cf0
LEGO_EXPORT int GetBuildAnimFrame(Ride *ride, TileId coords) {
    int i;
    int frames;
    int n;
    int frame;
    int max;
    BuildObj *b;
    LLS *lls;

    i = 0;
    for (b = BuildObjArray; (INT_PTR)&b->coords < (INT_PTR)&ButtonRepeatDelay; b++, i++) {
        if (b->coords.id == coords.id) {
            break;
        }
    }
    if ((ride->anim->flags & 0x8000) != 0) {
        max = 0;
        for (n = 0; n < ride->anim->group->count; n++) {
            lls = GetLLSForLayer(ride->anim, n);
            if (lls != NULL && lls->frame_count > max) {
                max = lls->frame_count;
            }
        }
        frames = max;
    } else {
        lls = *ride->anim->lls;
        if (lls == NULL) {
            return 0;
        }
        frames = lls->frame_count;
    }
    frame = BuildObjArray[i].elapsed * frames / GetBuildTime(ride);
    if (frame >= frames) {
        frame = frames - 1;
    }
    return frame;
}

// FUNCTION: LEGOLAND 0x00450d90
LEGO_EXPORT void DoBuildEffects(Ride *ride, TileId coords) {
    Point pt;
    int b[4];
    int i;
    int x;
    int y;
    int cx;
    int cy;
    int w;
    int h;
    int ty;
    int left;
    BuildObj *o;

    if (ReturnZero_00450c70(ride) && PopUpShowsBuildProgress == 0) {
        pt.x = ride->footprint.x0 + coords.pos.x;
        pt.y = ride->footprint.y1 + coords.pos.y;
        GetTileBounds(&pt, b);
        left = b[0];
        ty = b[1];
        pt.x = ride->footprint.x1 + coords.pos.x;
        pt.y = ride->footprint.y0 + coords.pos.y;
        GetTileBounds(&pt, b);
        cx = (b[2] + left) / 2;
        cy = (ty + b[3]) / 2;
        x = cx - 32;
        y = cy - 5;
        w = cx - x + 33;
        h = cy - y + 6;
        i = 0;
        for (o = BuildObjArray; (INT_PTR)&o->coords < (INT_PTR)&ButtonRepeatDelay; o++, i++) {
            if (o->coords.id == coords.id) {
                if (i < 256) {
                    RenderBlock(x, y, w, h, 0);
                    RenderBox(x, y, w, h, GetNearestColour(255, 255, 255));
                    RenderBlock(x + 1, y + 1, BuildObjArray[i].elapsed * (w - 2) / GetBuildTime(ride), h - 2, GetNearestColour(0, 255, 0));
                }
                return;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00450f10
LEGO_EXPORT void ClearBuildObjList(void) {
    int i;

    for (i = 0; i < 256; i++) {
        BuildObjArray[i].ride = NULL;
    }
}
