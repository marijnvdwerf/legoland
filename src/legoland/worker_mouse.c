#include "worker_mouse.h"
#include "globals.h"
#include "icon.h"
#include "legoland.h"
#include "man3d.h"
#include "math.h"
#include "render3d.h"
#include "string.h"
#include "tilemap.h"

struct WorkerInner {
    unsigned char pad_0[0x1c];
    struct BlokePos pos;
};

struct WorkerOuter {
    unsigned char pad_0[4];
    struct WorkerInner *inner;
    unsigned char pad_8[6];
    unsigned short var_e;
    unsigned char pad_10[0x58];
    unsigned int var_68;
    unsigned int var_6c;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004700c0
unsigned int FUN_004700c0(unsigned int param_1) {
    if (DAT_007fdf9c == 0x306) {
        if (DAT_007fdf8c == param_1) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004700f0
void *FUN_004700f0(void) {
    return DAT_007fdff0;
}

// FUNCTION: LEGOLAND 0x00470100
void FUN_00470100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004701f0
LEGO_EXPORT void SetWorkersPositionAtMouse(void) {
    struct WorkerOuter *worker;
    struct WorkerInner *inner;
    unsigned int pt[2];

    worker = DAT_007fdff0;
    worker->var_e = 13;
    worker = DAT_007fdff0;
    inner = worker->inner;
    inner->pos.field_0 = DAT_00813a44.x;
    worker = DAT_007fdff0;
    inner = worker->inner;
    inner->pos.field_4 = DAT_00813a44.y;
    worker = DAT_007fdff0;
    inner = worker->inner;
    AdjustBlokePosition(&inner->pos);
    ScreenToMapRef((unsigned int)&DAT_00813a44, pt, 0);
    worker = DAT_007fdff0;
    worker->var_68 = pt[0] << 8;
    worker = DAT_007fdff0;
    worker->var_6c = pt[1] << 8;
}

// FUNCTION: LEGOLAND 0x00470270
void FUN_00470270(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00470410
void FUN_00470410(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004704b0
void FUN_004704b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00470620
LEGO_EXPORT int CheckWorkerOnMouseStatus(int a) { STUB(); }

// FUNCTION: LEGOLAND 0x004708c0
LEGO_EXPORT void RenderWorkerOnMouse(void) {
    RenderBlokeIn3D((struct Bloke *)DAT_007fdff0);
}

// FUNCTION: LEGOLAND 0x004708d0
LEGO_EXPORT void ResetWorkersOldCoords(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00470930
LEGO_EXPORT void ResetMoveAWorkerStruct(void) {
    DAT_00668954 = 0;
    DAT_007fdff0 = NULL;
    DAT_007fdffc = 0;
}

// FUNCTION: LEGOLAND 0x00470950
void FUN_00470950(void *a, void *b) {
    unsigned int temp_val, temp_val2;

    if (!DAT_00668938) {
        // STRING: LEGOLAND 0x004baa70
        DAT_00668938 = LoadSprite("PU_OK.lls", 4);
    }
    if (!DAT_00668934) {
        // STRING: LEGOLAND 0x004baa64
        DAT_00668934 = LoadSprite("PU_OKON.lls", 4);
    }
    if (!DAT_0066893c) {
        // STRING: LEGOLAND 0x004baa54
        DAT_0066893c = LoadSprite("CB_Close.lls", 4);
    }
    if (!DAT_00668940) {
        // STRING: LEGOLAND 0x004baa44
        DAT_00668940 = LoadSprite("CB_CloseON.lls", 4);
    }
    if (!DAT_00668904) {
        // STRING: LEGOLAND 0x004baa34
        DAT_00668904 = LoadSprite("CB_BGleft.lls", 4);
    }
    if (!DAT_00668908) {
        // STRING: LEGOLAND 0x004baa24
        DAT_00668908 = LoadSprite("CB_BGCentre.lls", 4);
    }
    if (!DAT_0066890c) {
        // STRING: LEGOLAND 0x004baa14
        DAT_0066890c = LoadSprite("CB_BGRight.lls", 4);
    }

    DAT_007fdea8 = InsertIcon(0, 0, 0x2c3, DAT_00668938);
    DAT_007fdea8->string_id = 0x74;
    DAT_007fdea8->string = GetString(0x74);
    DAT_007fdea8->flags |= 0x2000;
    DAT_007fdea8->flags |= 0x4002;
    temp_val = DAT_007fdea8->flags;
    temp_val2 = temp_val | 0x400;
    DAT_007fdea8->flags = temp_val2;
    DAT_007fdea8->event_handler = a;

    DAT_007fe000 = InsertIcon(0, 0, 0x2c3, DAT_0066893c);
    DAT_007fe000->string_id = 0x75;
    DAT_007fe000->string = GetString(0x75);
    DAT_007fe000->flags |= 0x2000;
    DAT_007fe000->flags |= 0x4002;
    temp_val = DAT_007fe000->flags;
    temp_val2 = temp_val | 0x400;
    DAT_007fe000->flags = temp_val2;
    DAT_007fe000->event_handler = b;
}

// FUNCTION: LEGOLAND 0x00470b00
void FUN_00470b00(void) {
    if (DAT_00668938) {
        KillSprite(DAT_00668938);
        DAT_00668938 = NULL;
    }
    if (DAT_00668934) {
        KillSprite(DAT_00668934);
        DAT_00668934 = NULL;
    }
    if (DAT_00668940) {
        KillSprite(DAT_00668940);
        DAT_00668940 = NULL;
    }
    if (DAT_0066893c) {
        KillSprite(DAT_0066893c);
        DAT_0066893c = NULL;
    }
    if (DAT_00668904) {
        KillSprite(DAT_00668904);
        DAT_00668904 = NULL;
    }
    if (DAT_00668908) {
        KillSprite(DAT_00668908);
        DAT_00668908 = NULL;
    }
    if (DAT_0066890c) {
        KillSprite(DAT_0066890c);
        DAT_0066890c = NULL;
    }
}
