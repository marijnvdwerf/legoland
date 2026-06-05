#include "legoland.h"

struct WorkerInner {
    unsigned char pad_0[0x1c];
    unsigned int var_1c;
    unsigned int var_20;
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

extern void AdjustBlokePosition(unsigned int *coord);
extern void ScreenToMapRef(unsigned int *global, unsigned int *out, unsigned int param3);
extern void RenderBlokeIn3D(unsigned int worker);
extern void KillSprite(struct Sprite *sprite);

extern unsigned int DAT_007fdf9c;
extern unsigned int DAT_007fdf8c;
extern struct WorkerOuter *DAT_007fdff0;
extern unsigned int DAT_007fdffc;
extern unsigned int DAT_00813a44;
extern unsigned int DAT_00813a48;
extern unsigned int DAT_00668954;
extern struct Sprite *DAT_00668934;
extern struct Sprite *DAT_00668938;
extern struct Sprite *DAT_0066893c;
extern struct Sprite *DAT_00668940;
extern struct Sprite *DAT_00668904;
extern struct Sprite *DAT_00668908;
extern struct Sprite *DAT_0066890c;

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
unsigned int FUN_004700f0(void) {
    return (unsigned int)DAT_007fdff0;
}

// FUNCTION: LEGOLAND 0x00470100
void FUN_00470100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004701f0
void SetWorkersPositionAtMouse(void) {
    struct WorkerOuter *worker;
    struct WorkerInner *inner;
    unsigned int pt[2];

    worker = DAT_007fdff0;
    worker->var_e = 13;
    worker = DAT_007fdff0;
    inner = worker->inner;
    inner->var_1c = DAT_00813a44;
    worker = DAT_007fdff0;
    inner = worker->inner;
    inner->var_20 = DAT_00813a48;
    worker = DAT_007fdff0;
    inner = worker->inner;
    AdjustBlokePosition(&inner->var_1c);
    ScreenToMapRef(&DAT_00813a44, pt, 0);
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
void CheckWorkerOnMouseStatus(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004708c0
void RenderWorkerOnMouse(void) {
    RenderBlokeIn3D((unsigned int)DAT_007fdff0);
}

// FUNCTION: LEGOLAND 0x004708d0
void ResetWorkersOldCoords(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00470930
void ResetMoveAWorkerStruct(void) {
    DAT_00668954 = 0;
    DAT_007fdff0 = NULL;
    DAT_007fdffc = 0;
}

// FUNCTION: LEGOLAND 0x00470950
void FUN_00470950(void) { STUB(); }

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
