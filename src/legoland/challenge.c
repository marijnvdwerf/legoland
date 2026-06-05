#include "legoland.h"

struct AnimHandle;
struct RideElem;
struct RideObj;
struct ObjectClass;
struct Bloke;

struct AnimHandle {
    unsigned char pad_0[0x24];
    void *callback;
};

struct RideObj {
    unsigned char pad_0[0xc0];
    unsigned int (*func)(unsigned int, unsigned int);
    unsigned int arg;
};

struct RideElem {
    unsigned char pad_0[8];
    unsigned char flags;
    unsigned char pad_9[0xc - 0x9];
    struct RideObj *obj;
};

struct ObjectClass {
    struct ObjectClass *next;
    unsigned char pad_4[0x8 - 0x4];
    unsigned int count;
    unsigned char pad_c[0x20 - 0xc];
    unsigned short type;
};

struct Bloke {
    struct Bloke *next;
    unsigned char pad_4[0x7a - 0x4];
    short field_7a;
};

extern unsigned int CreateSourceImage(unsigned int param_1, unsigned int param_2);
extern void KillImage(unsigned int image);
extern unsigned int GetGameTimer(void);
extern unsigned int SaveGameWrite(const void *data, unsigned int size);
extern unsigned int SaveGameRead(void *data, unsigned int size);
extern struct RideElem *ElemID(const char *name);
extern void PushRenderingStatusAndLockVideoSurface(void);
extern void PopRenderingStatus(void);
extern void PrintSprite(unsigned int sprite, unsigned int x, unsigned int y, unsigned int param_4, unsigned int param_5);
extern int FUN_004781b0(int param_1, const void *param_2, unsigned int param_3);
extern signed char FUN_0044eb10(struct Bloke *bloke);
extern void PlayInstanceOfSample(void *sample, unsigned int param_2, unsigned int param_3, unsigned int param_4);
extern void KillSprite(unsigned int sprite);
extern void RemoveIconGroup(unsigned int group);
extern void FUN_004597e0(unsigned int param_1, unsigned int param_2);

extern unsigned int DAT_00665e8c;
extern unsigned int DAT_004b7d74;
extern unsigned int DAT_004b7d78;
extern unsigned short DAT_004b7d7e;
extern unsigned int DAT_004b7d84;
extern unsigned int DAT_00665f60;
extern unsigned int DAT_00665fe8;
extern unsigned int DAT_00665fec;
extern unsigned int DAT_0081c088;
extern struct AnimHandle *DAT_0081c08c;
extern struct AnimHandle *DAT_0081c090;
extern struct AnimHandle *DAT_0081c094;
extern struct AnimHandle *DAT_0081c098;
extern unsigned int DAT_0081c09c;
extern struct AnimHandle *DAT_0081c0a0;
extern struct AnimHandle *DAT_0081c0a4;
extern unsigned int DAT_00665ff8;
extern unsigned int DAT_00665ffc;
extern unsigned int DAT_00666000;
extern unsigned int DAT_00666004;
extern unsigned int DAT_00666008;
extern unsigned int DAT_0066600c;
extern unsigned int DAT_00666010;
extern unsigned int DAT_00666014;
extern unsigned int DAT_00666018;
extern unsigned int DAT_0066601c;
extern unsigned int DAT_00666020;
extern unsigned int DAT_00666024;
extern unsigned int DAT_00666030;
extern unsigned int DAT_00666034;
extern unsigned int DAT_00666038;
extern unsigned int DAT_0066603c;
extern unsigned int DAT_00666040;
extern unsigned int DAT_00666044;
extern unsigned int DAT_00666048;
extern unsigned int DAT_0066604c;
extern unsigned int DAT_00666050;
extern unsigned int DAT_00666054;
extern unsigned int DAT_00666058;
extern unsigned int DAT_0066605c;
extern unsigned int DAT_00666060;
extern unsigned int DAT_00666064;
extern unsigned int DAT_00666068;
extern unsigned int DAT_0066606c;
extern unsigned int DAT_00666070;
extern unsigned int DAT_00666074;
extern unsigned int DAT_00666078;
extern unsigned int DAT_0066607c;
extern unsigned int DAT_00666080;
extern unsigned int DAT_00666084;
extern unsigned int DAT_00666088;
extern unsigned int DAT_0066608c;
extern unsigned int DAT_00666090;
extern unsigned int DAT_00666094;
extern unsigned int DAT_00666098;
extern unsigned int DAT_004b7e9c;
extern unsigned int DAT_0081c040[];
extern unsigned int DAT_0081c054[];
extern unsigned int DAT_0081c068[];
extern struct ObjectClass *ObjectClassList;
extern struct Bloke *FirstBloke;
extern int DAT_00832934;
extern int DAT_00832938;
extern unsigned int DAT_0081c038;
extern unsigned int DAT_006660a8;
extern unsigned int DAT_006660ac;
extern void *PTR_004b92c0;
extern unsigned int DAT_0081c028;
extern unsigned int DAT_0081c02c;
extern unsigned int DAT_0081c030;
extern unsigned int DAT_0081c034;
extern unsigned int SPRITE_TitleScreenBk;
extern unsigned int DAT_0081c080;
extern unsigned int DAT_0081c084;
extern unsigned int DAT_00832b9c;
extern unsigned int DAT_00832978;
extern unsigned int DAT_0083297c;

unsigned int FUN_004434d0(unsigned int param_1);
struct AnimHandle *FUN_00443bd0(const char *filename);
void FUN_00443d50(struct AnimHandle *handle);
void FUN_00443d90(void);
void FUN_00443dc0(struct AnimHandle *handle);
unsigned int FUN_00443f90(unsigned int param_1);
unsigned int FUN_00443fe0(unsigned int param_1);
unsigned int FUN_00444020(void);
int FUN_00444c40(struct ObjectClass *node);
void FUN_0044db20(void);

// FUNCTION: LEGOLAND 0x004434d0
unsigned int FUN_004434d0(unsigned int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x004436d0
unsigned int FUN_004436d0(unsigned int param_1, unsigned int param_2) {
    unsigned int image;

    image = CreateSourceImage(param_1, param_2);
    if (image == 0) {
        return 0;
    }
    if (FUN_004434d0(image) == 0) {
        KillImage(image);
        return 0;
    }
    return image;
}

// FUNCTION: LEGOLAND 0x00443710
unsigned int FUN_00443710(void) {
    return DAT_00665e8c;
}

// FUNCTION: LEGOLAND 0x00443720
void FUN_00443720(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004437d0
void FUN_004437d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00443bd0
struct AnimHandle *FUN_00443bd0(const char *filename) { STUB(); }

// FUNCTION: LEGOLAND 0x00443d50
void FUN_00443d50(struct AnimHandle *handle) { STUB(); }

// FUNCTION: LEGOLAND 0x00443d90
void FUN_00443d90(void) {
    DAT_004b7d7e = 0x10;
    DAT_004b7d74 = 0x70;
    DAT_004b7d78 = 0x60;
    DAT_004b7d84 = 0x5400;
}

// FUNCTION: LEGOLAND 0x00443dc0
void FUN_00443dc0(struct AnimHandle *handle) { STUB(); }

// FUNCTION: LEGOLAND 0x00443e30
void FUN_00443e30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00443f90
unsigned int FUN_00443f90(unsigned int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x00443fe0
unsigned int FUN_00443fe0(unsigned int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x00444020
unsigned int FUN_00444020(void) {
    unsigned int handle;

    if (DAT_00665fec == DAT_00665fe8) {
        DAT_00665fec = FUN_00443fe0(DAT_00665fe8);
    }
    handle = DAT_00665fec;
    DAT_00665fe8 = handle;
    DAT_0081c088 = DAT_0081c09c;
    DAT_0081c09c = 0;
    DAT_00665f60 = FUN_00443f90(handle);
    return DAT_00665f60;
}

// FUNCTION: LEGOLAND 0x00444070
void FUN_00444070(unsigned int param_1, unsigned int param_2) {
    DAT_00665fec = param_1;
    DAT_0081c09c = param_2;
    DAT_0081c088 = 0;
}

// FUNCTION: LEGOLAND 0x00444090
void FUN_00444090(void) {
    FUN_00443d90();

    // STRING: LEGOLAND 0x004b7e24
    DAT_0081c08c = FUN_00443bd0("AD_Blink.avi");
    if (DAT_0081c08c != 0) {
        DAT_0081c08c->callback = FUN_00444020;
    }
    // STRING: LEGOLAND 0x004b7e18
    DAT_0081c094 = FUN_00443bd0("AD_LR.avi");
    if (DAT_0081c094 != 0) {
        DAT_0081c094->callback = FUN_00444020;
    }
    // STRING: LEGOLAND 0x004b7e08
    DAT_0081c0a0 = FUN_00443bd0("AD_Phone.avi");
    if (DAT_0081c0a0 != 0) {
        DAT_0081c0a0->callback = FUN_00444020;
    }
    // STRING: LEGOLAND 0x004b7df4
    DAT_0081c0a4 = FUN_00443bd0("AD_PhoneGesture.avi");
    if (DAT_0081c0a4 != 0) {
        DAT_0081c0a4->callback = FUN_00444020;
    }
    // STRING: LEGOLAND 0x004b7de0
    DAT_0081c098 = FUN_00443bd0("AD_PhoneDown.avi");
    if (DAT_0081c098 != 0) {
        DAT_0081c098->callback = FUN_00444020;
    }
    // STRING: LEGOLAND 0x004b7dd0
    DAT_0081c090 = FUN_00443bd0("AD_Wobble.avi");
    if (DAT_0081c090 != 0) {
        DAT_0081c090->callback = FUN_00444020;
    }
    FUN_00443dc0(DAT_0081c08c);
}

// FUNCTION: LEGOLAND 0x00444150
void FUN_00444150(void) {
    if (DAT_0081c08c != 0) {
        FUN_00443d50(DAT_0081c08c);
        DAT_0081c08c = 0;
    }
    if (DAT_0081c094 != 0) {
        FUN_00443d50(DAT_0081c094);
        DAT_0081c094 = 0;
    }
    if (DAT_0081c0a0 != 0) {
        FUN_00443d50(DAT_0081c0a0);
        DAT_0081c0a0 = 0;
    }
    if (DAT_0081c0a4 != 0) {
        FUN_00443d50(DAT_0081c0a4);
        DAT_0081c0a4 = 0;
    }
    if (DAT_0081c098 != 0) {
        FUN_00443d50(DAT_0081c098);
        DAT_0081c098 = 0;
    }
    if (DAT_0081c090 != 0) {
        FUN_00443d50(DAT_0081c090);
        DAT_0081c090 = 0;
    }
}

// FUNCTION: LEGOLAND 0x004441f0
void FUN_004441f0(void) {
    DAT_00665ff8 = 0;
}

// FUNCTION: LEGOLAND 0x00444200
unsigned int FUN_00444200(void) {
    unsigned int elapsed;

    if (SaveGameWrite(&DAT_00665ff8, 0xa0) == 0) {
        return 0;
    }
    if (DAT_00666098 == 0) {
        elapsed = 0xffffffff;
    } else {
        elapsed = DAT_00666098 - GetGameTimer();
    }
    return SaveGameWrite(&elapsed, 0x4) != 0;
}

// FUNCTION: LEGOLAND 0x00444260
unsigned int FUN_00444260(void) {
    unsigned int elapsed;

    if (SaveGameRead(&DAT_00665ff8, 0xa0) == 0) {
        return 0;
    }
    if (SaveGameRead(&elapsed, 0x4) == 0) {
        return 0;
    }
    if (elapsed == 0xffffffff) {
        DAT_00666098 = 0;
        return 1;
    }
    DAT_00666098 = GetGameTimer() + elapsed;
    return 1;
}

// FUNCTION: LEGOLAND 0x004442c0
unsigned int FUN_004442c0(void) {
    struct RideElem *elem;

    // STRING: LEGOLAND 0x004b80b0
    elem = ElemID("DRIVING SCHOOL");
    if ((elem->flags & 1) != 0) {
        return elem->obj->func(elem->obj->arg, 0);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004442f0
unsigned int FUN_004442f0(void) {
    struct RideElem *elem;

    // STRING: LEGOLAND 0x004b536c
    elem = ElemID("BOATING SCHOOL");
    if ((elem->flags & 1) != 0) {
        return elem->obj->func(elem->obj->arg, 0);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00444320
void FUN_00444320(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00444350
unsigned int FUN_00444350(void) {
    struct RideElem *elem;

    // STRING: LEGOLAND 0x004b4bb4
    elem = ElemID("LOG FLUME ENTRANCE");
    if ((elem->flags & 1) != 0) {
        return elem->obj->func(elem->obj->arg, 0);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00444380
unsigned int FUN_00444380(void) {
    struct RideElem *elem;

    // STRING: LEGOLAND 0x004b80c0
    elem = ElemID("JUNGLE CRUISE");
    if ((elem->flags & 1) != 0) {
        return elem->obj->func(elem->obj->arg, 0);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004443b0
unsigned int FUN_004443b0(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ffc = param_1;
        DAT_00665ff8 |= 1;
        return param_1;
    }
    DAT_00665ff8 = (DAT_00665ff8 & 0xffffff00) | ((DAT_00665ff8 & 0xff) & 0xfe);
    return DAT_00665ff8;
}

// FUNCTION: LEGOLAND 0x004443e0
unsigned int FUN_004443e0(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00666000 = param_1;
        DAT_00665ff8 |= 2;
        return param_1;
    }
    DAT_00665ff8 &= ~2;
    return DAT_00665ff8;
}

// FUNCTION: LEGOLAND 0x00444410
unsigned int FUN_00444410(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00666004 = param_1;
        DAT_00665ff8 |= 4;
        return param_1;
    }
    DAT_00665ff8 &= ~4;
    return DAT_00665ff8;
}

// FUNCTION: LEGOLAND 0x00444440
void FUN_00444440(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 8;
        DAT_00666008 = param_1;
    } else {
        DAT_00665ff8 = (DAT_00665ff8 & 0xffffff00) | (DAT_00665ff8 & 0xf7);
    }
}

// FUNCTION: LEGOLAND 0x00444470
void FUN_00444470(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= (param_2 & 3) << 4;
        DAT_0066600c = param_1;
    } else {
        DAT_00665ff8 = (DAT_00665ff8 & 0xffffff00) | (DAT_00665ff8 & 0xcf);
    }
}

// FUNCTION: LEGOLAND 0x004444b0
void FUN_004444b0(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= (param_2 & 3) << 6;
        DAT_00666010 = param_1;
    } else {
        DAT_00665ff8 = (DAT_00665ff8 & 0xffffff00) | (DAT_00665ff8 & 0x3f);
    }
}

// FUNCTION: LEGOLAND 0x004444f0
void FUN_004444f0(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= (param_2 & 3) << 8;
        DAT_00666014 = param_1;
    } else {
        DAT_00665ff8 = (DAT_00665ff8 & 0xffff00ff) | ((DAT_00665ff8 >> 8 & 0xfc) << 8);
    }
}

// FUNCTION: LEGOLAND 0x00444530
void FUN_00444530(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= (param_2 & 3) << 10;
        DAT_00666018 = param_1;
    } else {
        DAT_00665ff8 = (DAT_00665ff8 & 0xffff00ff) | ((DAT_00665ff8 >> 8 & 0xf3) << 8);
    }
}

// FUNCTION: LEGOLAND 0x00444570
void FUN_00444570(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= (param_2 & 3) << 12;
        DAT_0066601c = param_1;
    } else {
        DAT_00665ff8 = (DAT_00665ff8 & 0xffff00ff) | ((DAT_00665ff8 >> 8 & 0xcf) << 8);
    }
}

// FUNCTION: LEGOLAND 0x004445b0
void FUN_004445b0(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x4000;
        DAT_00666020 = param_1;
        DAT_00666024 = param_2;
    } else {
        DAT_00665ff8 &= ~0x4000;
    }
}

// FUNCTION: LEGOLAND 0x004445f0
void FUN_004445f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00444630
void FUN_00444630(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x40000;
        DAT_00666030 = param_1;
        DAT_00666034 = param_2;
    } else {
        DAT_00665ff8 &= 0xfffbffff;
    }
}

// FUNCTION: LEGOLAND 0x00444670
void FUN_00444670(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x8000000;
        DAT_00666070 = param_1;
        DAT_00666074 = param_2;
    } else {
        DAT_00665ff8 &= 0xf7ffffff;
    }
}

// FUNCTION: LEGOLAND 0x004446b0
void FUN_004446b0(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x10000000;
        DAT_00666078 = param_1;
        DAT_0066607c = param_2;
    } else {
        DAT_00665ff8 &= 0xefffffff;
    }
}

// FUNCTION: LEGOLAND 0x004446f0
void FUN_004446f0(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x20000000;
        DAT_00666080 = param_1;
        DAT_00666084 = param_2;
    } else {
        DAT_00665ff8 &= 0xdfffffff;
    }
}

// FUNCTION: LEGOLAND 0x00444730
void FUN_00444730(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x10000;
        DAT_00666040 = param_1;
        DAT_00666044 = param_2;
    } else {
        DAT_00665ff8 &= 0xfffeffff;
    }
}

// FUNCTION: LEGOLAND 0x00444770
void FUN_00444770(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x20000;
        DAT_00666048 = param_1;
        DAT_0066604c = param_2;
    } else {
        DAT_00665ff8 &= 0xfffdffff;
    }
}

// FUNCTION: LEGOLAND 0x004447b0
void FUN_004447b0(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x40000000;
        DAT_00666088 = param_1;
        DAT_0066608c = param_2;
    } else {
        DAT_00665ff8 &= 0xbfffffff;
    }
}

// FUNCTION: LEGOLAND 0x004447f0
void FUN_004447f0(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x80000000;
        DAT_00666090 = param_1;
        DAT_00666094 = param_2;
    } else {
        DAT_00665ff8 &= 0x7fffffff;
    }
}

// FUNCTION: LEGOLAND 0x00444830
void FUN_00444830(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x80000;
        DAT_00666038 = param_1;
        DAT_0066603c = param_2;
    } else {
        DAT_00665ff8 &= 0xfff7ffff;
    }
}

// FUNCTION: LEGOLAND 0x00444870
void FUN_00444870(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004448b0
void FUN_004448b0(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x4000000;
        DAT_00666050 = param_1;
        DAT_00666054 = param_2;
    } else {
        DAT_00665ff8 &= 0xfbffffff;
    }
}

// FUNCTION: LEGOLAND 0x004448f0
void FUN_004448f0(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x200000;
        DAT_00666058 = param_1;
        DAT_0066605c = param_2;
    } else {
        DAT_00665ff8 &= 0xffdfffff;
    }
}

// FUNCTION: LEGOLAND 0x00444930
void FUN_00444930(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x400000;
        DAT_00666060 = param_1;
        DAT_00666064 = param_2;
    } else {
        DAT_00665ff8 &= 0xffbfffff;
    }
}

// FUNCTION: LEGOLAND 0x00444970
void FUN_00444970(unsigned int param_1, unsigned int param_2) {
    if ((param_1 | param_2) != 0) {
        DAT_00665ff8 |= 0x800000;
        DAT_00666068 = param_1;
        DAT_0066606c = param_2;
    } else {
        DAT_00665ff8 &= 0xff7fffff;
    }
}

// FUNCTION: LEGOLAND 0x004449b0
void FUN_004449b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00444a70
void FUN_00444a70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00444b70
void FUN_00444b70(unsigned int param_1, unsigned int param_2, int param_3, unsigned int param_4, unsigned int param_5) {
    PushRenderingStatusAndLockVideoSurface();
    if (param_3 == 1) {
        PrintSprite(DAT_0081c040[param_4], param_1, param_2, 0, 0);
    } else if (param_3 == 0) {
        PrintSprite(DAT_0081c054[param_4], param_1, param_2, 0, 0);
    } else if (param_3 == -1) {
        PrintSprite(DAT_0081c068[param_4], param_1 + 5, param_2 + 5, 0, 0);
    }
    PopRenderingStatus();
}

// FUNCTION: LEGOLAND 0x00444bf0
void FUN_00444bf0(unsigned int *param_1, unsigned int *param_2) {
    struct ObjectClass *node;

    node = ObjectClassList;
    *param_1 = 0;
    *param_2 = 0;
    if (node == 0) {
        return;
    }
    do {
        if (node->count != 0 && (node->type == 1 || node->type == 3)) {
            *param_1 += node->count;
            *param_2 += 1;
        }
        node = node->next;
    } while (node != 0);
}

// FUNCTION: LEGOLAND 0x00444c40
int FUN_00444c40(struct ObjectClass *node) {
    return FUN_004781b0(**(int **)((char *)node + 0xc4), &DAT_004b7e9c, 0x16) >= 0;
}

// FUNCTION: LEGOLAND 0x00444c70
void FUN_00444c70(unsigned int *param_1, unsigned int *param_2) {
    struct ObjectClass *node;

    node = ObjectClassList;
    *param_1 = 0;
    *param_2 = 0;
    if (node == 0) {
        return;
    }
    do {
        if (node->count != 0 && node->type == 2 && FUN_00444c40(node) == 0) {
            *param_1 += node->count;
            *param_2 += 1;
        }
        node = node->next;
    } while (node != 0);
}

// FUNCTION: LEGOLAND 0x00444cd0
void FUN_00444cd0(unsigned int *param_1, unsigned int *param_2) {
    struct ObjectClass *node;

    node = ObjectClassList;
    *param_1 = 0;
    *param_2 = 0;
    if (node == 0) {
        return;
    }
    do {
        if (node->count != 0 && node->type == 4) {
            *param_1 += node->count;
            *param_2 += 1;
        }
        node = node->next;
    } while (node != 0);
}

// FUNCTION: LEGOLAND 0x00444d20
void FUN_00444d20(unsigned int *param_1, unsigned int *param_2) {
    struct ObjectClass *node;

    node = ObjectClassList;
    *param_1 = 0;
    *param_2 = 0;
    if (node == 0) {
        return;
    }
    do {
        if (node->count != 0 && node->type == 5) {
            *param_1 += node->count;
            *param_2 += 1;
        }
        node = node->next;
    } while (node != 0);
}

// FUNCTION: LEGOLAND 0x00444d70
void FUN_00444d70(unsigned int *param_1, unsigned int *param_2, int *param_3) {
    struct Bloke *bloke;
    signed char value;

    bloke = FirstBloke;
    *param_1 = 0;
    *param_2 = 0;
    *param_3 = 0;
    if (bloke == 0) {
        return;
    }
    do {
        *param_1 += 1;
        if (bloke->field_7a > DAT_00832934) {
            *param_2 += 1;
        }
        if (bloke->field_7a > DAT_00832938) {
            *param_2 += 1;
        }
        value = FUN_0044eb10(bloke);
        *param_3 += 4 - value;
        bloke = bloke->next;
    } while (bloke != 0);
}

// FUNCTION: LEGOLAND 0x00444df0
void FUN_00444df0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00444eb0
unsigned char FUN_00444eb0(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_0081c038 = 0;
        DAT_006660a8 = 0;
        DAT_006660ac = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00444ef0
void FUN_00444ef0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00444f90
void FUN_00444f90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00445000
void FUN_00445000(void) {
    unsigned int current;
    unsigned int val1;
    unsigned int val2;
    unsigned int val3;

    current = (unsigned int)&DAT_0081c054;
    while ((int)current < (int)&DAT_0081c068) {
        val1 = *(unsigned int *)(current - 0x14);
        if (val1 != 0) {
            KillSprite(val1);
            *(unsigned int *)(current - 0x14) = 0;
        }
        val2 = *(unsigned int *)current;
        if (val2 != 0) {
            KillSprite(val2);
            *(unsigned int *)current = 0;
        }
        val3 = *(unsigned int *)(current + 0x14);
        if (val3 != 0) {
            KillSprite(val3);
            *(unsigned int *)(current + 0x14) = 0;
        }
        current += 4;
    }
    if (DAT_0081c030 != 0) {
        KillSprite(DAT_0081c030);
        DAT_0081c030 = 0;
    }
    if (DAT_0081c028 != 0) {
        KillSprite(DAT_0081c028);
        DAT_0081c028 = 0;
    }
    if (SPRITE_TitleScreenBk != 0) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = 0;
    }
    if (DAT_0081c02c != 0) {
        KillSprite(DAT_0081c02c);
        DAT_0081c02c = 0;
    }
    if (DAT_0081c034 != 0) {
        KillSprite(DAT_0081c034);
        DAT_0081c034 = 0;
    }
    if (DAT_0081c080 != 0) {
        KillSprite(DAT_0081c080);
        DAT_0081c080 = 0;
    }
    if (DAT_0081c084 != 0) {
        KillSprite(DAT_0081c084);
        DAT_0081c084 = 0;
    }
    RemoveIconGroup(1);
}

// FUNCTION: LEGOLAND 0x00445100
void FUN_00445100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00445190
void FUN_00445190(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00445310
void FUN_00445310(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004453a0
void FUN_004453a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044db20
void FUN_0044db20(void) {
    DAT_00832b9c = 0;
}

// FUNCTION: LEGOLAND 0x0044db40
void FUN_0044db40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044db80
void FUN_0044db80(void) {
    DAT_00832978 = 0;
    DAT_00666098 = 0;
}

// FUNCTION: LEGOLAND 0x0044db90
void FUN_0044db90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044dc70
void FUN_0044dc70(unsigned int param_1, unsigned int param_2) {
    DAT_0083297c = param_1;
    FUN_0044db20();
    FUN_004597e0(0, param_2);
}
