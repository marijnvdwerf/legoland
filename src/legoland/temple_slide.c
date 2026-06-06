#include "legoland.h"

#include "binv.h"
#include "render3d.h"
#include "llidb.h"

struct TempleRide {
    unsigned char pad_0[4];
    unsigned int var_4;
    unsigned char pad_8[4];
    unsigned int var_c;
};

struct SlideCar {
    unsigned char pad_0[0x10];
    unsigned int var_10;
};

struct SlideTrack {
    unsigned char pad_0[0x14];
    unsigned int var_14;
    unsigned int var_18;
    unsigned char pad_1c[0x48];
    struct SlideCar *var_64;
};

struct SlideContext {
    unsigned char pad_0[0xc];
    struct SlideTrack *var_c;
};

struct TempleSlideObject {
    unsigned char pad_0[0x8c];
    void (*field_8c)(void);
    unsigned char pad_90[8];
    void (*field_98)(void);
    void (*field_9c)(void);
    void (*field_a0)(void);
    void (*field_a4)(void);
    void (*field_a8)(void);
    void (*field_ac)(void);
    void (*field_b0)(void);
    unsigned char pad_b4[4];
    void (*field_b8)(void);
    void (*field_bc)(void);
};

struct SaveNode {
    unsigned char pad_0[8];
    struct SaveNode *var_8;
    unsigned char pad_c[0x14];
};

extern unsigned int DAT_004cbf78;
extern unsigned int DAT_004cbf80;
extern unsigned int DAT_004cbfd0;
extern void *DAT_004cbfc4;
extern unsigned int DAT_004cbf98;
extern unsigned int DAT_004cbf9c;
extern unsigned int DAT_004cbfa0;
extern unsigned short DAT_004cbfa4;
extern struct SaveNode *DAT_004cbfd4;

#include "image_sprite.h"
extern int __strcmpi(const char *s1, const char *s2);

// FUNCTION: LEGOLAND 0x00416ec0
void FUN_00416ec0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00416f00
void FUN_00416f00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00416f60
unsigned char *FUN_00416f60(void *arg) { STUB(); }

// FUNCTION: LEGOLAND 0x00416f90
void FUN_00416f90(struct TempleRide *arg) {
    arg->var_c |= 1;
}

// FUNCTION: LEGOLAND 0x00416fa0
void FUN_00416fa0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00417130
void FUN_00417130(struct TempleRide *arg) {
    arg->var_4 = 0;
    arg->var_c &= 0xfffffffe;
}

// FUNCTION: LEGOLAND 0x00417150
void FUN_00417150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00417200
void FUN_00417200(struct TempleRide *arg) {
    DAT_004cbf80 = arg->var_c;
    if (DAT_004cbf78 != 0) {
        KillSprite(DAT_004cbf78);
    }
    KillSprite(DAT_004cbfd0);
    FreeBinV(DAT_004cbfc4);
}

// FUNCTION: LEGOLAND 0x00417240
void FUN_00417240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00417280
void FUN_00417280(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004172d0
void FUN_004172d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00417300
unsigned int *FUN_00417300(struct SlideContext *ctx, unsigned short param) {
    struct SlideTrack *track = ctx->var_c;
    struct SlideCar *car = track->var_64;

    DAT_004cbf98 = (unsigned int)car;
    DAT_004cbf9c = track->var_14;
    DAT_004cbfa0 = track->var_18;
    DAT_004cbfa4 = param;

    car = track->var_64;
    car->var_10 |= 0x2000;

    return &DAT_004cbf98;
}

// FUNCTION: LEGOLAND 0x00417340
void FUN_00417340(void *arg) {
    unsigned int *array = (unsigned int *)(FUN_00416f60(arg) + 0x10);
    int i = 0;

    while (i < 4) {
        if (array[i] == 0) {
            Ride_ClearFlagToNotLetAnyoneOn(arg);
            return;
        }
        i++;
    }
    Ride_SetFlagToNotLetAnyoneOn(arg);
}

// FUNCTION: LEGOLAND 0x00417380
void FUN_00417380(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00417400
void FUN_00417400(unsigned int index, void *arg) {
    unsigned char *result = FUN_00416f60(arg);

    if (result != NULL) {
        ((unsigned int *)(result + 0x10))[index] = 0;
    }
    FUN_00417340(arg);
}

// FUNCTION: LEGOLAND 0x00417430
void FUN_00417430(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004178c0
int SaveTempleSlide(void) {
    int one = 1;
    int zero = 0;
    struct SaveNode *p;

    p = DAT_004cbfd4;
    while (p != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(p, 0x20) == 0) {
            return 0;
        }
        p = p->var_8;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00417930
void LoadTempleSlide(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00417a00
void TempleSlide_GetInterfaces(const char **ctx, struct TempleSlideObject *interfaces) {
    // STRING: LEGOLAND 0x004b4f8c
    if (__strcmpi("TEMPLE SLIDE", *ctx) == 0) {
        interfaces->field_a4 = FUN_00417150;
        interfaces->field_ac = FUN_00417200;
        interfaces->field_8c = FUN_00417240;
        interfaces->field_a8 = FUN_00417430;
        interfaces->field_b0 = FUN_00416fa0;
        interfaces->field_9c = FUN_00417280;
        interfaces->field_98 = FUN_004172d0;
        interfaces->field_a0 = FUN_00417300;
        interfaces->field_bc = SaveTempleSlide;
        interfaces->field_b8 = LoadTempleSlide;
    }
}
