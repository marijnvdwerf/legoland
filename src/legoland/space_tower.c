#include "legoland.h"

#include "gamemap.h"
#include "obj_instance.h"
#include "space_tower.h"

struct RideObject {
    unsigned short var_0;
    unsigned char var_2;
    unsigned char var_3;
    unsigned char var_4;
    unsigned char pad_5[7];
    unsigned int var_c;
    unsigned int var_10;
    void *var_14;
    unsigned char pad_18[8];
    unsigned int var_20;
    unsigned char pad_24[20];
    void *var_38;
    unsigned char pad_3c[8];
    unsigned int var_44;
    unsigned char pad_48[20];
    void *var_5c;
    unsigned char pad_60[8];
    unsigned int var_68;
    unsigned char pad_6c[20];
    void *var_80;
    unsigned char pad_84[8];
    unsigned int var_8c;
    unsigned char pad_90[28];
    unsigned char var_ac;
    unsigned char var_ad;
    unsigned char pad_ae[2];
    unsigned int var_b0;
    unsigned char pad_b4[24];
    unsigned int var_cc;
};

struct SpaceTowerObject {
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

struct ListNode {
    unsigned char pad_0[8];
    struct ListNode *next;
};

extern unsigned int DAT_004b7620;
extern struct RideObject *DAT_0062fd74;
extern struct ListNode *DAT_0062fda8;
extern void *DAT_0062fd80;
extern void *DAT_0062fd7c;
extern void *DAT_0062fd68;
extern void *DAT_0062fd6c;
extern unsigned int EditMode;
extern unsigned int DAT_008119b8;
extern struct Cursor EditCursor;
extern const unsigned char SPACE_TOWER_SFX[15];

extern void PlayInstanceOfSample(unsigned int sample, unsigned int a, unsigned int b, void *data);
extern void KillSprite(void *sprite);
extern void Kill_FXList(const unsigned char *list, int flag);
extern int __strcmpi(const char *s1, const char *s2);
extern void SetEditCursorFootPrint(unsigned int arg);
extern void Put3DBlokesOnRide2(struct RideObject *ride, struct RideObject *obj);
extern void Ride_SetFlagToNotLetAnyoneOn(struct RideObject *ride);

// FUNCTION: LEGOLAND 0x0043a7a0
void FUN_0043a7a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043a820
void FUN_0043a820(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043a8c0
void FUN_0043a8c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043a940
void FUN_0043a940(void *arg) { STUB(); }

// FUNCTION: LEGOLAND 0x0043a9b0
void FUN_0043a9b0(struct RideObject *arg) { STUB(); }

// FUNCTION: LEGOLAND 0x0043aa10
void FUN_0043aa10(unsigned char *arg) {
    unsigned int buffer[4];

    buffer[0] = 2;
    buffer[2] = arg[0];
    buffer[3] = arg[1];
    PlayInstanceOfSample(DAT_004b7620, 1, 1, buffer);
}

// FUNCTION: LEGOLAND 0x0043aa50
void FUN_0043aa50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043aa90
void FUN_0043aa90(struct RideObject *arg) {
    unsigned char tmp = arg->var_2;
    arg->var_c |= 1;
    arg->var_3 = tmp;
    arg->var_10 = 0;
    FUN_0043a9b0(arg);
    FUN_0043aa10((unsigned char *)arg);
}

// FUNCTION: LEGOLAND 0x0043aac0
void FUN_0043aac0(struct RideObject *arg) { STUB(); }

// FUNCTION: LEGOLAND 0x0043ab70
void FUN_0043ab70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043abc0
void FUN_0043abc0(unsigned int arg) { STUB(); }

// FUNCTION: LEGOLAND 0x0043ac20
void FUN_0043ac20(void) {
    while (DAT_0062fda8 != NULL) {
        FUN_0043abc0((unsigned int)DAT_0062fda8);
    }
}

// FUNCTION: LEGOLAND 0x0043ac40
void FUN_0043ac40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043ac70
void FUN_0043ac70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043acb0
void FUN_0043acb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043ad00
void FUN_0043ad00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043ad90
void FUN_0043ad90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043ae20
void FUN_0043ae20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043aee0
void FUN_0043aee0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043af50
void FUN_0043af50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b2b0
void FUN_0043b2b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b420
void FUN_0043b420(void) {
    EditMode = 1;
    DAT_008119b8 = (unsigned int)DAT_0062fd74;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x0043b460
void FUN_0043b460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b4b0
void FUN_0043b4b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b4e0
void FUN_0043b4e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b570
void FUN_0043b570(void) {
    KillSprite(DAT_0062fd80);
    KillSprite(DAT_0062fd7c);
    KillSprite(DAT_0062fd68);
    KillSprite(DAT_0062fd6c);
    FUN_0043ac20();
    Kill_FXList(SPACE_TOWER_SFX, 1);
    DAT_0062fd74->var_cc = 0;
}

// FUNCTION: LEGOLAND 0x0043b5d0
void SpaceTower_Save(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b6a0
void SpaceTower_Load(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b780
void SpaceTowerRide(const char **name, struct SpaceTowerObject *obj) {
    // STRING: LEGOLAND 0x004b789c
    if (__strcmpi("SPACE TOWER RIDE", *name) == 0) {
        obj->field_a4 = FUN_0043b2b0;
        obj->field_8c = FUN_0043b420;
        obj->field_a8 = FUN_0043bac0;
        obj->field_a0 = FUN_0043b4e0;
        obj->field_b0 = FUN_0043af50;
        obj->field_9c = FUN_0043b460;
        obj->field_98 = FUN_0043b4b0;
        obj->field_ac = FUN_0043b570;
        obj->field_bc = SpaceTower_Save;
        obj->field_b8 = SpaceTower_Load;
    }
}

// FUNCTION: LEGOLAND 0x0043b810
void FUN_0043b810(struct RideObject *arg) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b990
void FUN_0043b990(struct RideObject *esi) {
    esi->var_ad++;
    if ((signed char)esi->var_ad >= 16) {
        esi->var_ad = 0;
    }
    esi->var_ac++;
    if ((signed char)esi->var_ac >= 16) {
        esi->var_ac = 0;
    }
    if (esi->var_c & 1) {
        FUN_0043a940(&esi->var_14);
        FUN_0043a940(&esi->var_38);
        FUN_0043a940(&esi->var_5c);
        FUN_0043a940(&esi->var_80);
        if (esi->var_20 == 0 && esi->var_44 == 0 && esi->var_68 == 0 && esi->var_8c == 0) {
            if (GetAllBlokesOffRide((struct Ride *)DAT_0062fd74, esi->var_0) == 0) {
                return;
            }
            FUN_0043aac0(esi);
            return;
        }
    }
    FUN_0043b810(esi);
    Put3DBlokesOnRide2(DAT_0062fd74, esi);
    if (esi->var_c & 1) {
        return;
    }
    if (esi->var_c & 0x4000) {
        if (esi->var_2 == esi->var_4) {
            esi->var_c &= ~0x4000;
            FUN_0043aa90(esi);
        }
        return;
    }
    if (esi->var_2 == 0) {
        return;
    }
    if (esi->var_b0 == 0) {
        esi->var_c |= 0x4000;
        Ride_SetFlagToNotLetAnyoneOn(esi);
    } else {
        esi->var_b0--;
    }
}

// FUNCTION: LEGOLAND 0x0043baa0
void FUN_0043baa0(void) {
    struct ListNode *node;

    node = DAT_0062fda8;
    if (node != NULL) {
        do {
            FUN_0043b990((struct RideObject *)node);
            node = node->next;
        } while (node != NULL);
    }
}

// FUNCTION: LEGOLAND 0x0043bac0
void FUN_0043bac0(void) { STUB(); }
