#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "gamemap.h"
#include "man3d.h"
#include "money.h"
#include "obj_instance.h"
#include "render3d.h"
#include "sound_music.h"
#include "llidb.h"
#include "map_object.h"

#include "image_sprite.h"


struct EateryObj;
struct EateryFX;
struct EateryInner;
struct BlokeNode;

struct EateryFX {
    /* 0x00 */ unsigned char pad_0[0x14];
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned int field_18;
    /* 0x1c */ unsigned int flags_1c;
    /* 0x20 */ unsigned char pad_20[0x44];
    /* 0x64 */ struct EateryInner *inner_64;
};

struct EateryInner {
    unsigned char pad_0[0x10];
    unsigned int flags_10;
};

struct EateryObj {
    unsigned char pad_0[0xc];
    struct EateryFX *fx_c;
};

struct BlokeNode {
    struct BlokeNode *next;
    unsigned char pad_4[4];
    unsigned int field_8;
    unsigned short field_c;
};

struct BlokeOwner {
    unsigned char pad_0[0xcc];
    struct BlokeNode *head_cc;
};

struct BlokeArg {
    unsigned char pad_0[0xc];
    struct BlokeOwner *owner_c;
};

struct EditArg {
    unsigned char pad_0[0xc];
    unsigned int var_c;
};

struct UserFlagsArg {
    int var_0;
    int var_4;
};

struct SaveBlock {
    struct SaveBlock *next;
    unsigned char data[60];
};


// FUNCTION: LEGOLAND 0x0042e220
void FUN_0042e220(struct EateryObj *obj) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner;
    DAT_0081cd44 = fx;
    fx->flags_1c |= 0x20;
    inner = DAT_0081cd44->inner_64;
    inner->flags_10 |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e260
void FUN_0042e260(struct BlokeArg *arg, unsigned int param2, unsigned int param3, unsigned short *value) {
    struct BlokeOwner *owner = arg->owner_c;
    struct BlokeNode *node = owner->head_cc;
    if (node != NULL) {
        while (node != NULL) {
            if (*value == node->field_c) {
                IP_RenderBlokeIn3DNow((struct Bloke *)node->field_8);
                GetScreenCoordsForObject(value, owner);
                break;
            }
            node = node->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x0042e2a0
void FUN_0042e2a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042e460
void FUN_0042e460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042e4b0
void FUN_0042e4b0(void) {
    LLIDB_UnLoadData(DAT_0061613c);
}

// FUNCTION: LEGOLAND 0x0042e4c0
void FUN_0042e4c0(void) {
    unsigned int temp = DAT_0081cd38;
    EditMode = 1;
    DAT_008119b8 = (void *)temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((void *)((unsigned int)DAT_008119b8 + 0x3c));
}

// FUNCTION: LEGOLAND 0x0042e500
void FUN_0042e500(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042e560
void FUN_0042e560(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042e5d0
void FUN_0042e5d0(struct EateryObj *obj) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner;
    DAT_0081cd18 = fx;
    DAT_0081cd18->flags_1c |= 0x20;
    inner = DAT_0081cd18->inner_64;
    inner->flags_10 |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e610
void FUN_0042e610(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042e770
void FUN_0042e770(struct EateryObj *obj) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner;
    DAT_0081cd14 = (unsigned int)fx;
    fx->flags_1c |= 0x20;
    inner = ((struct EateryFX *)DAT_0081cd14)->inner_64;
    inner->flags_10 |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e7b0
void FUN_0042e7b0(struct EateryObj *obj) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner;
    DAT_0081cde0 = fx;
    fx->flags_1c |= 0x20;
    inner = DAT_0081cde0->inner_64;
    inner->flags_10 |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e7f0
void FUN_0042e7f0(struct EateryObj *obj) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner;
    DAT_0081cd3c = (unsigned int)fx;
    fx->flags_1c |= 0x20;
    inner = ((struct EateryFX *)DAT_0081cd3c)->inner_64;
    inner->flags_10 |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e830
void FUN_0042e830(struct BlokeArg *arg, unsigned int param2, unsigned int param3, unsigned short *value) {
    struct BlokeOwner *owner = arg->owner_c;
    struct BlokeNode *node = owner->head_cc;
    if (node == NULL) {
        return;
    }
    while (node != NULL) {
        if (*value == node->field_c) {
            IP_RenderBlokeIn3DNow((struct Bloke *)node->field_8);
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x0042e870
void FUN_0042e870(struct EateryObj *obj) {
    DAT_0081cd0c = (unsigned int)obj->fx_c;
    ((struct EateryFX *)DAT_0081cd0c)->flags_1c |= 0x20;
    DAT_0081cd10 = (unsigned int)((struct EateryFX *)DAT_0081cd0c)->inner_64;
    ((struct EateryInner *)DAT_0081cd10)->flags_10 |= 0x2000;
    Load_FXList(RESTAURANT_SFX, 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e8b0
void FUN_0042e8b0(void) {
    Kill_FXList(RESTAURANT_SFX, 1);
    DAT_0081cd0c = 0;
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e8d0
void FUN_0042e8d0(struct EditArg *arg) {
    unsigned int temp = arg->var_c;
    EditMode = 1;
    DAT_008119b8 = (void *)temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((void *)((unsigned int)DAT_008119b8 + 0x3c));
}

// FUNCTION: LEGOLAND 0x0042e910
void FUN_0042e910(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042e9c0
void FUN_0042e9c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042ea10
void FUN_0042ea10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042ea60
void FUN_0042ea60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042ec10
void FUN_0042ec10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042ed70
void FUN_0042ed70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042eec0
void FUN_0042eec0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042ef10
void FUN_0042ef10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042ef40
void FUN_0042ef40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042ef70
void FUN_0042ef70(struct BlokeNode *node) {
    struct BlokeNode *head = DAT_00616144;
    if (head == node) {
        DAT_00616144 = node->next;
    } else {
        struct BlokeNode *current = head;
        if (current->next != node) {
            do {
                current = current->next;
            } while (current && current->next != node);
        }
        if (current) {
            current->next = node->next;
        }
    }
    free(node);
}

// FUNCTION: LEGOLAND 0x0042efb0
void FUN_0042efb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042f030
void FUN_0042f030(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042f0f0
void FUN_0042f0f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042f1a0
void FUN_0042f1a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042f4c0
void FUN_0042f4c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042f720
void FUN_0042f720(void) {
    KillSprite(DAT_0081cd28);
    KillSprite(DAT_0081cd8c);
    KillSprite(DAT_0081cd88);
    KillSprite(DAT_0081cd94);
    KillSprite(DAT_0081cd90);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042f770
void FUN_0042f770(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042f920
void FUN_0042f920(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042f9a0
void FUN_0042f9a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042f9d0
unsigned int FUN_0042f9d0(unsigned int *param) { STUB(); }

// FUNCTION: LEGOLAND 0x0042fa00
void FUN_0042fa00(unsigned int param) { STUB(); }

// FUNCTION: LEGOLAND 0x0042fa40
void FUN_0042fa40(unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5) {
    unsigned int result = FUN_0042f9d0(&arg2);
    if (result != 0) {
        FUN_0042fa00(result);
    }
    StandardRemoveObject(arg1, arg2, arg3);
    RemoveAllBlokesFromRide((unsigned int)((struct EateryObj *)arg1)->fx_c, (void *)arg2);
}

// FUNCTION: LEGOLAND 0x0042fa90
void FUN_0042fa90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042fb00
void FUN_0042fb00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042fb60
void FUN_0042fb60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042fbb0
void FUN_0042fbb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004304a0
void *FUN_004304a0(struct EateryObj *obj, unsigned short a2) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner = fx->inner_64;
    DAT_00616120 = (unsigned int)inner;
    DAT_00616124 = fx->field_14;
    DAT_00616128 = fx->field_18;
    DAT_0061612c = a2;
    inner = fx->inner_64;
    inner->flags_10 |= 0x2000;
    return &DAT_00616120;
}

// FUNCTION: LEGOLAND 0x004304e0
void FUN_004304e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00430b10
void FUN_00430b10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00431120
void FUN_00431120(void) {
    Kill_FXList(OCTOPUS_SFX, 3);
    KillMoneySFX();
    KillSprite(DAT_0081cd34);
    KillSprite(DAT_0081cd84);
    KillSprite(DAT_0081cd20);
    KillSprite(DAT_0081cd48);
}

// FUNCTION: LEGOLAND 0x00431170
void FUN_00431170(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004312c0
void FUN_004312c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00431300
void FUN_00431300(struct EateryObj *obj) {
    struct EateryInner *inner;
    DAT_0081cd1c = obj->fx_c;
    DAT_0081cd1c->flags_1c |= 0x20;
    inner = DAT_0081cd1c->inner_64;
    inner->flags_10 |= 0x2000;
    // STRING: LEGOLAND 0x004b7100
    DAT_0081cd60 = LoadSprite("OctTentA.lls", 1);
    // STRING: LEGOLAND 0x004b70f0
    DAT_0081cd64 = LoadSprite("OctTentB.lls", 1);
    // STRING: LEGOLAND 0x004b70e0
    DAT_0081cd68 = LoadSprite("OctTentC.lls", 1);
    // STRING: LEGOLAND 0x004b70d0
    DAT_0081cd6c = LoadSprite("OctTentD.lls", 1);
    // STRING: LEGOLAND 0x004b70c0
    DAT_0081cd70 = LoadSprite("OctTentE.lls", 1);
    // STRING: LEGOLAND 0x004b70b0
    DAT_0081cd74 = LoadSprite("OctTentF.lls", 1);
    // STRING: LEGOLAND 0x004b70a0
    DAT_0081cd78 = LoadSprite("OctTentG.lls", 1);
    // STRING: LEGOLAND 0x004b7090
    DAT_0081cd7c = LoadSprite("OctTentH.lls", 1);
    // STRING: LEGOLAND 0x004b7080
    DAT_0081cd80 = LoadSprite("OctoKiosk.lls", 1);
    // STRING: LEGOLAND 0x004b7070
    DAT_0081cda0 = LoadSprite("OctTabAA.lls", 1);
    // STRING: LEGOLAND 0x004b7060
    DAT_0081cda4 = LoadSprite("OctTabAB.lls", 1);
    // STRING: LEGOLAND 0x004b7050
    DAT_0081cda8 = LoadSprite("OctTabBA.lls", 1);
    // STRING: LEGOLAND 0x004b7040
    DAT_0081cdac = LoadSprite("OctTabBB.lls", 1);
    // STRING: LEGOLAND 0x004b7030
    DAT_0081cdb0 = LoadSprite("OctTabCA.lls", 1);
    // STRING: LEGOLAND 0x004b7020
    DAT_0081cdb4 = LoadSprite("OctTabCB.lls", 1);
    // STRING: LEGOLAND 0x004b7010
    DAT_0081cdb8 = LoadSprite("OctTabDA.lls", 1);
    // STRING: LEGOLAND 0x004b7000
    DAT_0081cdbc = LoadSprite("OctTabDB.lls", 1);
    // STRING: LEGOLAND 0x004b6ff0
    DAT_0081cdc0 = LoadSprite("OctTabEA.lls", 1);
    // STRING: LEGOLAND 0x004b6fe0
    DAT_0081cdc4 = LoadSprite("OctTabEB.lls", 1);
    // STRING: LEGOLAND 0x004b6fd0
    DAT_0081cdc8 = LoadSprite("OctTabFA.lls", 1);
    // STRING: LEGOLAND 0x004b6fc0
    DAT_0081cdcc = LoadSprite("OctTabFB.lls", 1);
    // STRING: LEGOLAND 0x004b6fb0
    DAT_0081cdd0 = LoadSprite("OctTabGA.lls", 1);
    // STRING: LEGOLAND 0x004b6fa0
    DAT_0081cdd4 = LoadSprite("OctTabGB.lls", 1);
    // STRING: LEGOLAND 0x004b6f90
    DAT_0081cdd8 = LoadSprite("OctTabHA.lls", 1);
    // STRING: LEGOLAND 0x004b6f80
    DAT_0081cddc = LoadSprite("OctTabHB.lls", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x004314f0
void FUN_004314f0(unsigned int param_1, struct UserFlagsArg *param_2) {
    AddBasicObject(param_1, (unsigned int)param_2);
    Set_UserFlags(param_2->var_0, param_2->var_4, 0);
}

// FUNCTION: LEGOLAND 0x00431520
void FUN_00431520(void) {
    if (DAT_0081cd60) {
        KillSprite(DAT_0081cd60);
    }
    if (DAT_0081cd64) {
        KillSprite(DAT_0081cd64);
    }
    if (DAT_0081cd68) {
        KillSprite(DAT_0081cd68);
    }
    if (DAT_0081cd6c) {
        KillSprite(DAT_0081cd6c);
    }
    if (DAT_0081cd70) {
        KillSprite(DAT_0081cd70);
    }
    if (DAT_0081cd74) {
        KillSprite(DAT_0081cd74);
    }
    if (DAT_0081cd78) {
        KillSprite(DAT_0081cd78);
    }
    if (DAT_0081cd7c) {
        KillSprite(DAT_0081cd7c);
    }
    if (DAT_0081cd80) {
        KillSprite(DAT_0081cd80);
    }
    if (DAT_0081cda0) {
        KillSprite(DAT_0081cda0);
    }
    if (DAT_0081cda4) {
        KillSprite(DAT_0081cda4);
    }
    if (DAT_0081cda8) {
        KillSprite(DAT_0081cda8);
    }
    if (DAT_0081cdac) {
        KillSprite(DAT_0081cdac);
    }
    if (DAT_0081cdb0) {
        KillSprite(DAT_0081cdb0);
    }
    if (DAT_0081cdb4) {
        KillSprite(DAT_0081cdb4);
    }
    if (DAT_0081cdb8) {
        KillSprite(DAT_0081cdb8);
    }
    if (DAT_0081cdbc) {
        KillSprite(DAT_0081cdbc);
    }
    if (DAT_0081cdc0) {
        KillSprite(DAT_0081cdc0);
    }
    if (DAT_0081cdc4) {
        KillSprite(DAT_0081cdc4);
    }
    if (DAT_0081cdc8) {
        KillSprite(DAT_0081cdc8);
    }
    if (DAT_0081cdcc) {
        KillSprite(DAT_0081cdcc);
    }
    if (DAT_0081cdd0) {
        KillSprite(DAT_0081cdd0);
    }
    if (DAT_0081cdd4) {
        KillSprite(DAT_0081cdd4);
    }
    if (DAT_0081cdd8) {
        KillSprite(DAT_0081cdd8);
    }
    if (DAT_0081cddc) {
        KillSprite(DAT_0081cddc);
    }
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x004316f0
void FUN_004316f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00431c50
void FUN_00431c50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00431d00
void FUN_00431d00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004322a0
void FUN_004322a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00432310
void FUN_00432310(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00432390
void FUN_00432390(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00432400
int FUN_00432400(void) {
    unsigned int count;
    struct SaveBlock *current;
    struct SaveBlock *prev = NULL;

    if (!SaveGameRead(&count, 4)) {
        return 0;
    }
    if (count == 0) {
        return 1;
    }
    while (count != 0) {
        current = malloc(64);
        if (!SaveGameRead(current, 64)) {
            return 0;
        }
        current->next = 0;
        if (prev != NULL) {
            prev->next = current;
        } else {
            DAT_00616148 = current;
        }
        prev = current;
        if (!SaveGameRead(&count, 4)) {
            return 0;
        }
    }
    return 1;
}
