#include "worker_mouse.h"
#include "bloke.h"
#include "bloke_ai.h"
#include "debug_alloc.h"
#include "globals.h"
#include "icon.h"
#include "legoland.h"
#include "man3d.h"
#include "math.h"
#include "render3d.h"
#include "sound_music.h"
#include "string.h"
#include "tilemap.h"
#include "worker.h"

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004700c0
unsigned int IsBlokeHovered(void *object) {
    if (PopUpInfoSubjectType == 0x306) {
        if (PopUpInfoBloke == object) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004700f0
void *GetWorkerOnMouse(void) {
    return WorkerOnMouse;
}

// FUNCTION: LEGOLAND 0x00470100
void PickUpWorker(unsigned int type, Bloke *worker) {
    // STRING: LEGOLAND 0x004ba9ec
    DBPrintf("Picking up worker (%x) Workorder = %x\n", worker, worker->order);
    WorkerOnMouse = worker;
    WorkerOnMouseType = type;
    worker->low_level_action = 0xd;
    WorkerOldX = WorkerOnMouse->pos.x;
    WorkerOldY = WorkerOnMouse->pos.y;
    WorkerOnMouse->dir = 5;
    WorkerOnMouseActive = 1;
    if (WorkerOnMouseType == 0x307) {
        ClearAGardenersWorkList(WorkerOnMouse);
        NewLongTermAction(WorkerOnMouse, 0x18);
        ClearAGardenersWorkList(WorkerOnMouse);
    } else {
        ClearAMechanicsWorkList(WorkerOnMouse);
        NewLongTermAction(WorkerOnMouse, 0x19);
        ClearAMechanicsWorkList(WorkerOnMouse);
    }
    worker->order = 0;
    WorkerOnMouse->order = 0;
    if (WorkerOnMouseType == 0x307) {
        NewLongTermAction(WorkerOnMouse, 0x18);
    } else {
        NewLongTermAction(WorkerOnMouse, 0x19);
    }
}

// FUNCTION: LEGOLAND 0x004701f0
LEGO_EXPORT void SetWorkersPositionAtMouse(void) {
    struct Bloke *worker;
    struct Person *inner;
    unsigned int pt[2];

    worker = WorkerOnMouse;
    worker->low_level_action = 13;
    worker = WorkerOnMouse;
    inner = worker->person;
    inner->screen.x = MousePos.x;
    worker = WorkerOnMouse;
    inner = worker->person;
    inner->screen.y = MousePos.y;
    worker = WorkerOnMouse;
    inner = worker->person;
    AdjustBlokePosition(&inner->screen);
    ScreenToMapRef(&MousePos.x, pt, 0);
    worker = WorkerOnMouse;
    worker->pos.x = pt[0] << 8;
    worker = WorkerOnMouse;
    worker->pos.y = pt[1] << 8;
}

/* Handles a worker drop on its matching home building (potting shed or mechanics hut), setting its position/action
 * and playing the drop sound. Also returns true for a path-control object without assigning a work order. */
// FUNCTION: LEGOLAND 0x00470270
int TryDropWorkerOnHomeOrPath(void) {
    unsigned int v = Hover.data.value & 0xffff;
    int x = v & 0xff;
    int y = v >> 8;
    MapElement *elem;
    Ride *ride;

    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
        elem = &GameMap[y][x];
    } else {
        elem = NULL;
    }
    ride = elem->field_0->ride;
    if (ride->element == (Element *)PottingShedHandle && WorkerOnMouseType == 0x307) {
        PutWorkerOnRide(WorkerOnMouse, elem);
        WorkerOnMouse->pos.x = (ride->x + x) << 8;
        WorkerOnMouse->dest.x = WorkerOnMouse->pos.x;
        WorkerOnMouse->pos.y = (ride->y + y) << 8;
        WorkerOnMouse->dest.y = WorkerOnMouse->pos.y;
        WorkerOnMouse->action = 5;
        WorkerOnMouse->param_action = 100;
        PlayInstanceOfSample(GameFX[FX_GARDENER_DROP].sample, 0, 1, 0);
        return 1;
    }
    if (ride->element == (Element *)MechanicsHutHandle && WorkerOnMouseType == 0x308) {
        PutWorkerOnRide(WorkerOnMouse, elem);
        WorkerOnMouse->pos.x = ((ride->x + x) << 8) + 0x80;
        WorkerOnMouse->dest.x = WorkerOnMouse->pos.x;
        WorkerOnMouse->pos.y = (ride->y + y) << 8;
        WorkerOnMouse->dest.y = WorkerOnMouse->pos.y;
        WorkerOnMouse->action = 5;
        WorkerOnMouse->param_action = 100;
        PlayInstanceOfSample(GameFX[FX_MECHANIC_DROP].sample, 0, 1, 0);
        return 1;
    }
    return ride->element == (Element *)PathControlHandle;
}

/* Looks up the hovered gardener/mechanic work order. On success optionally returns its footprint corner in tile
 * coordinates and plays the corresponding worker drop sound. */
// FUNCTION: LEGOLAND 0x00470410
WorkOrder *FindHoveredWorkerOrder(Point *out) {
    WorkOrder *order;
    unsigned int v = Hover.data.value & 0xffff;
    int x = v & 0xff;
    int y = v >> 8;

    if (WorkerOnMouseType == 0x307) {
        order = GetGardenerWorkOrderAt(x, y);
    } else {
        order = GetMechanicWorkOrderAt(x, y);
    }
    if (order != NULL) {
        if (out != NULL) {
            out->x = order->pos.x + order->footprints->x0;
            out->y = order->footprints->y1 + order->pos.y;
        }
        if (WorkerOnMouseType == 0x307) {
            PlayInstanceOfSample(GameFX[FX_GARDENER_DROP].sample, 0, 1, 0);
        } else {
            PlayInstanceOfSample(GameFX[FX_MECHANIC_DROP].sample, 0, 1, 0);
        }
        return order;
    }
    return NULL;
}

/* Creates a repair order for the hovered durable object if its worker type is enabled and no order is already
 * flagged. Marks tile flag 0x4000, optionally returns the order footprint corner, and plays the drop sound. */
// FUNCTION: LEGOLAND 0x004704b0
WorkOrder *CreateHoveredWorkerRepairOrder(Point *out) {
    unsigned int v = Hover.data.value & 0xffff;
    struct Point pos;
    MapElement *elem;
    Ride *ride;
    unsigned short flags;
    WorkOrder *order = NULL;

    pos.x = v & 0xff;
    pos.y = v >> 8;
    if (pos.x >= 0 && pos.x < lpConfig->width && pos.y >= 0 && pos.y < lpConfig->height) {
        elem = &GameMap[pos.y][pos.x];
    } else {
        elem = NULL;
    }
    flags = elem->flags;
    if (flags & 0x88) {
        ride = elem->field_0->ride;
        if (ride->durability != 0) {
            do {
                if ((ride->flags & 0x200000) && WorkerOnMouseType == 0x307 && lpConfig->gardeners_enabled != 0) {
                    if (!(0x4000 & flags)) {
                        order = AddRepairOrderForObject(ride, pos);
                        if (order == NULL) {
                            break;
                        }
                    }
                } else if ((ride->flags & 0x400000) && WorkerOnMouseType == 0x308 && lpConfig->mechanics_enabled != 0) {
                    if (!(0x4000 & flags)) {
                        order = AddRepairOrderForObject(ride, pos);
                    }
                }
                if (order != NULL) {
                    elem->flags |= 0x4000;
                    if (out != NULL) {
                        out->x = order->pos.x + order->footprints->x0;
                        out->y = order->footprints->y1 + order->pos.y;
                    }
                    if (WorkerOnMouseType == 0x307) {
                        PlayInstanceOfSample(GameFX[FX_GARDENER_DROP].sample, 0, 1, 0);
                    } else {
                        PlayInstanceOfSample(GameFX[FX_MECHANIC_DROP].sample, 0, 1, 0);
                    }
                }
            } while (0);
        }
        return order;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00470620
LEGO_EXPORT void CheckWorkerOnMouseStatus(int a) {
    int result = 0;
    int isOrder = 0;
    int pt[2];
    int x;
    WorkOrder *order;
    MapElement *elem;

    if (!(MapRightClickFlags & 2) && a == 0) {
        if (MapLeftClickFlags & 2) {
            MouseClickConsumed = 1;
            WorkerOnMouseActive = 0;
            for (;;) {
                if (Hover.type == 0x103) {
                    if (TryDropWorkerOnHomeOrPath()) {
                        WorkerOnMouseActive = 0;
                        return;
                    }
                    order = FindHoveredWorkerOrder((Point *)pt);
                    if (order != NULL) {
                        x = pt[0];
                        WorkerOnMouseActive = 0;
                        isOrder = 1;
                    } else {
                        order = CreateHoveredWorkerRepairOrder((Point *)pt);
                        if (order == NULL) {
                            break;
                        }
                        x = pt[0];
                        WorkerOnMouseActive = 0;
                        isOrder = 1;
                    }
                } else {
                    if (Hover.type == 0x10a || Hover.type == 2 || MousePos.y < 0x20 || MousePos.y >= 0x174 || MousePos.x < lpConfig->view_x + 9) {
                        break;
                    }
                    ScreenToMapRef(&MousePos.x, pt, 0);
                    x = pt[0];
                    if (x < 0 || x >= lpConfig->width || pt[1] < 0) {
                        break;
                    }
                    if (pt[1] < lpConfig->height) {
                        elem = &GameMap[pt[1]][x];
                        if (elem != NULL && (elem->field_10 & 2)) {
                            WorkerOnMouseActive = 1;
                            if (WorkerOnMouseType == 0x307 && (elem->flags & 0x800)) {
                                WorkerOnMouseActive = 0;
                            } else {
                                SetWorkersPositionAtMouse();
                                return;
                            }
                        } else if (WorkerOnMouseActive != 0) {
                            SetWorkersPositionAtMouse();
                            return;
                        }
                    } else {
                        WorkerOnMouseActive = 1;
                        SetWorkersPositionAtMouse();
                        return;
                    }
                }
                if (WorkerOnMouseType == 0x307) {
                    WorkerOnMouse->pos.x = (x << 8) + 0x80;
                    WorkerOnMouse->pos.y = (pt[1] << 8) + 0x80;
                    result = SetGardenerWorkOrderAtPostion(WorkerOnMouse, pt[0], pt[1]);
                } else {
                    if (isOrder) {
                        WorkerOnMouse->pos.x = ((order->step_x + x) << 8) + 0x80;
                        WorkerOnMouse->pos.y = ((pt[1] - order->step_y) << 8) + 0x80;
                    } else {
                        WorkerOnMouse->pos.x = (x << 8) + 0x80;
                        WorkerOnMouse->pos.y = (pt[1] << 8) + 0x80;
                    }
                    result = SetMechanicsOrderAtPostion(WorkerOnMouse, pt[0], pt[1]);
                }
                if (result == 0) {
                    break;
                }
                if (WorkerOnMouseActive != 0) {
                    SetWorkersPositionAtMouse();
                }
                return;
            }
            WorkerOnMouseActive = 1;
            SetWorkersPositionAtMouse();
            return;
        }
    } else {
        ResetWorkersOldCoords();
    }
    if (WorkerOnMouseActive != 0) {
        SetWorkersPositionAtMouse();
    }
}

// FUNCTION: LEGOLAND 0x004708c0
LEGO_EXPORT void RenderWorkerOnMouse(void) {
    RenderBlokeIn3D((struct Bloke *)WorkerOnMouse);
}

// FUNCTION: LEGOLAND 0x004708d0
LEGO_EXPORT void ResetWorkersOldCoords(void) {
    struct Bloke *bloke;

    bloke = WorkerOnMouse;
    if (bloke != NULL) {
        bloke->pos.x = WorkerOldX;
        bloke = WorkerOnMouse;
        bloke->pos.y = WorkerOldY;
        bloke = WorkerOnMouse;
        bloke->field_50 = 0;
        if (WorkerOnMouseType == 0x307) {
            NewLongTermAction(WorkerOnMouse, 0x10);
        } else {
            NewLongTermAction(WorkerOnMouse, 0x11);
        }
        ResetMoveAWorkerStruct();
    }
}

// FUNCTION: LEGOLAND 0x00470930
LEGO_EXPORT void ResetMoveAWorkerStruct(void) {
    WorkerOnMouseActive = 0;
    WorkerOnMouse = NULL;
    WorkerOnMouseType = 0;
}

// FUNCTION: LEGOLAND 0x00470950
void CreatePopUpInfoButtons(void *a, void *b) {
    unsigned int temp_val, temp_val2;

    if (!PUOKSprite) {
        // STRING: LEGOLAND 0x004baa70
        PUOKSprite = LoadSprite("PU_OK.lls", 4);
    }
    if (!PUOKOnSprite) {
        // STRING: LEGOLAND 0x004baa64
        PUOKOnSprite = LoadSprite("PU_OKON.lls", 4);
    }
    if (!CBCloseSprite) {
        // STRING: LEGOLAND 0x004baa54
        CBCloseSprite = LoadSprite("CB_Close.lls", 4);
    }
    if (!CBCloseOnSprite) {
        // STRING: LEGOLAND 0x004baa44
        CBCloseOnSprite = LoadSprite("CB_CloseON.lls", 4);
    }
    if (!CBBGLeftSprite) {
        // STRING: LEGOLAND 0x004baa34
        CBBGLeftSprite = LoadSprite("CB_BGleft.lls", 4);
    }
    if (!CBBGCentreSprite) {
        // STRING: LEGOLAND 0x004baa24
        CBBGCentreSprite = LoadSprite("CB_BGCentre.lls", 4);
    }
    if (!CBBGRightSprite) {
        // STRING: LEGOLAND 0x004baa14
        CBBGRightSprite = LoadSprite("CB_BGRight.lls", 4);
    }

    PopUpInfoOkIcon = InsertIcon(0, 0, 0x2c3, PUOKSprite);
    PopUpInfoOkIcon->string_id = 0x74;
    PopUpInfoOkIcon->string = GetString(0x74);
    PopUpInfoOkIcon->flags |= 0x2000;
    PopUpInfoOkIcon->flags |= 0x4002;
    temp_val = PopUpInfoOkIcon->flags;
    temp_val2 = temp_val | 0x400;
    PopUpInfoOkIcon->flags = temp_val2;
    PopUpInfoOkIcon->event_handler = a;

    CBCloseIcon = InsertIcon(0, 0, 0x2c3, CBCloseSprite);
    CBCloseIcon->string_id = 0x75;
    CBCloseIcon->string = GetString(0x75);
    CBCloseIcon->flags |= 0x2000;
    CBCloseIcon->flags |= 0x4002;
    temp_val = CBCloseIcon->flags;
    temp_val2 = temp_val | 0x400;
    CBCloseIcon->flags = temp_val2;
    CBCloseIcon->event_handler = b;
}

// FUNCTION: LEGOLAND 0x00470b00
void KillPUOKAndCBSprites(void) {
    if (PUOKSprite) {
        KillSprite(PUOKSprite);
        PUOKSprite = NULL;
    }
    if (PUOKOnSprite) {
        KillSprite(PUOKOnSprite);
        PUOKOnSprite = NULL;
    }
    if (CBCloseOnSprite) {
        KillSprite(CBCloseOnSprite);
        CBCloseOnSprite = NULL;
    }
    if (CBCloseSprite) {
        KillSprite(CBCloseSprite);
        CBCloseSprite = NULL;
    }
    if (CBBGLeftSprite) {
        KillSprite(CBBGLeftSprite);
        CBBGLeftSprite = NULL;
    }
    if (CBBGCentreSprite) {
        KillSprite(CBBGCentreSprite);
        CBBGCentreSprite = NULL;
    }
    if (CBBGRightSprite) {
        KillSprite(CBBGRightSprite);
        CBBGRightSprite = NULL;
    }
}
