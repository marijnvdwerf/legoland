#include "legoland.h"

#include "gamemap.h"
#include "map_object.h"

typedef void (*FortVtblFn)(void);

struct FortInterface {
    unsigned char pad_0[0x8c];
    FortVtblFn field_8c;
    FortVtblFn field_90;
    FortVtblFn field_94;
    FortVtblFn field_98;
    FortVtblFn field_9c;
    FortVtblFn field_a0;
    FortVtblFn field_a4;
    FortVtblFn field_a8;
    FortVtblFn field_ac;
    FortVtblFn field_b0;
};

struct EditCursorData {
    unsigned char pad_0[0x3c];
    unsigned char field_3c;
};

extern unsigned int DAT_004c11cc;
extern unsigned int DAT_004c11dc;
extern unsigned int EditMode;
extern struct Cursor EditCursor;
extern struct EditCursorData *DAT_008119b8;
extern int __strcmpi(const char *s1, const char *s2);
extern void KillSprite(unsigned int sprite);

// FUNCTION: LEGOLAND 0x00406240
void FUN_00406240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004062a0
void FUN_004062a0(void) {
    unsigned int sprite = DAT_004c11cc;
    if (sprite != 0) {
        KillSprite(sprite);
    }
}

// FUNCTION: LEGOLAND 0x004062c0
void FUN_004062c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004064d0
void FUN_004064d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00406660
void FUN_00406660(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00406820
void FUN_00406820(void) {
    DAT_008119b8 = (struct EditCursorData *)DAT_004c11dc;
    EditMode = 1;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&DAT_008119b8->field_3c);
}

// FUNCTION: LEGOLAND 0x00406860
unsigned int FUN_00406860(unsigned int param1, unsigned int param2) {
    return AddBasicObject(param1, param2);
}

// FUNCTION: LEGOLAND 0x00406880
void FUN_00406880(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004068b0
void FUN_004068b0(const char **name, struct FortInterface *ci) {
    // STRING: LEGOLAND 0x004b45a0
    if (__strcmpi("FORT", *name) != 0) {
        return;
    }
    ci->field_a4 = FUN_00406240;
    ci->field_ac = FUN_004062a0;
    ci->field_8c = FUN_00406820;
    ci->field_a8 = FUN_00406660;
    ci->field_b0 = FUN_004062c0;
    ci->field_9c = FUN_00406880;
    ci->field_98 = (FortVtblFn)FUN_00406860;
}
