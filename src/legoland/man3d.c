#include "legoland.h"
#include "resource.h"
#include "print_sprite.h"

struct Person {
    struct Person *prev;
    struct Person *next;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
    unsigned char pad_24[0x2c - 0x24];
    unsigned int field_2c;
    unsigned int field_30;
    unsigned int field_34;
    unsigned int field_38;
    unsigned char pad_3c[0x4c - 0x3c];
    int field_4c;
    unsigned char pad_50[0x54 - 0x50];
    unsigned int sort_id;
    unsigned char pad_58[0x62 - 0x58];
    unsigned char flags;
    unsigned char pad_63[0x7c - 0x63];
    unsigned int field_7c;
    unsigned int field_80;
    unsigned int random;
    unsigned int field_88;
    unsigned int field_8c;
    unsigned int field_90;
};

struct Pos {
    unsigned char pad_0[4];
    int count;
    unsigned char pad_8[0x24 - 0x8];
    void **entries;
};

struct PosHeader {
    unsigned char pad_0[0x50];
    void *field_50;
};

struct Anim3D {
    int divisor;
};

struct Bloke {
    unsigned char pad_0[4];
    struct Person *person;
};

extern void FUN_0049e4d0(void *ptr);
extern void *_malloc(unsigned int size);
extern unsigned int FUN_0049e4b2(void);
extern int FUN_0049e573(char *buffer, const char *format, ...);
extern void FUN_00442c70(void);
extern void FUN_00486250(void);
extern void FUN_004886a0(void);

extern struct Person *DAT_00655a3c;
extern struct Person *FirstBloke;
extern void *DAT_0081c8c0;
extern void *DAT_0081c8c4;
extern void *DAT_0081c8c8;
extern void *DAT_00630100;
extern void *DAT_0062feac;
extern unsigned int DAT_0062feb0[];
extern unsigned int DAT_0062feb8[];
extern unsigned int DAT_0062febc[];
extern unsigned int DAT_0062fed4[];
extern unsigned int DAT_0062feec[];
extern void *DAT_0062fef4;

void FUN_0043f810(struct Person *person);
void FUN_0043fde0(void *ptr);
struct Person *FUN_0043f8c0(unsigned int param_1, unsigned int param_2);
void FUN_0043f970(void *buffer);
void Render3DPerson(struct Person *person);
void FUN_004401b0(unsigned int param_1, struct Person *person);
void SetPersonPosition(struct Person *person, unsigned int x, unsigned int y);
void UpdatePerson(struct Person *person);
void BlokeSetAnim(struct Bloke *bloke, unsigned int anim);
void BlokeWalkAnim(struct Bloke *bloke);
struct Anim3D *GetBlokeAnim3D(struct Bloke *bloke);

// FUNCTION: LEGOLAND 0x0043f660
void LoadPos(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043f7d0
void UnloadPos(struct Pos *pos) {
    int i;

    i = 0;
    if (pos->count > 0) {
        do {
            FUN_0049e4d0(pos->entries[i]);
            i++;
        } while (i < pos->count);
    }
    FUN_0049e4d0(pos->entries);
    FUN_0049e4d0(pos);
}

// FUNCTION: LEGOLAND 0x0043f810
void FUN_0043f810(struct Person *person) {
    person->prev = 0;
    person->next = 0;
    if (DAT_00655a3c != 0) {
        person->next = DAT_00655a3c;
        DAT_00655a3c->prev = person;
    }
    DAT_00655a3c = person;
}

// FUNCTION: LEGOLAND 0x0043f840
void FUN_0043f840(struct Person *person) {
    if (person->prev != 0) {
        person->prev->next = person->next;
    } else {
        DAT_00655a3c = person->next;
    }
    if (person->next != 0) {
        person->next->prev = person->prev;
    }
}

// FUNCTION: LEGOLAND 0x0043f870
void FUN_0043f870(struct PosHeader *param_1) {
    if (param_1->field_50 != 0) {
        FUN_0049e4d0(param_1->field_50);
    }
    FUN_0049e4d0(param_1);
}

// FUNCTION: LEGOLAND 0x0043f890
struct Person *Find3DPersonFromBloke(unsigned int id) {
    struct Person *person;

    person = DAT_00655a3c;
    if (person == 0) {
        return 0;
    }
    while (person->field_c != id) {
        person = person->next;
        if (person == 0) {
            return 0;
        }
    }
    return person;
}

// FUNCTION: LEGOLAND 0x0043f8c0
struct Person *FUN_0043f8c0(unsigned int param_1, unsigned int param_2) {
    struct Person *person;

    person = _malloc(0x94);
    if (person != 0) {
        memset(person, 0, 0x94);
        if (param_2 == 1) {
            person->random = FUN_0049e4b2() & 1;
        } else {
            person->random = 0;
        }
        person->field_8 = param_2;
        person->field_c = param_1;
        person->field_7c = 0xffffffff;
        person->field_80 = 0xffffffff;
        person->field_88 = 0xffffffff;
        person->field_90 = 0xffffffff;
        person->field_8c = 0xffffffff;
        person->field_10 = 0x40000000;
        person->field_14 = 0x40000000;
        person->field_18 = 0x40000000;
        person->prev = 0;
        person->next = 0;
        person->field_4c = 0;
        person->field_1c = 0;
        person->field_20 = 0;
        person->field_2c = 0;
        person->field_30 = 0;
        person->field_34 = 0xff;
        person->field_38 = 0;
    }
    return person;
}

// FUNCTION: LEGOLAND 0x0043f970
void FUN_0043f970(void *buffer) {
    struct Person *p = buffer;

    p->field_2c = p->field_2c + (unsigned int)p;
    p->field_30 = p->field_30 + (unsigned int)p;
}

// FUNCTION: LEGOLAND 0x0043f990
void *FUN_0043f990(const char *param_1, const char *param_2) {
    char path[256];
    struct ResFile *file;
    unsigned int size;
    void *buffer;

    // STRING: LEGOLAND 0x004b7b10
    FUN_0049e573(path, ".\\3ddata\\new\\%s\\%s", param_2, param_1);
    file = RES_OpenFile(path);
    if (file != 0) {
        size = RES_GetFileSize(file);
        buffer = _malloc(size);
        if (buffer != 0) {
            RES_ReadFile(file, buffer, size);
            RES_CloseFile(file);
            FUN_0043f970(buffer);
            return buffer;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0043fa10
void FUN_0043fa10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043fa80
void FUN_0043fa80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043fde0
void FUN_0043fde0(void *ptr) { STUB(); }

// FUNCTION: LEGOLAND 0x0043fe50
void Render3DPerson(struct Person *person) { STUB(); }

// FUNCTION: LEGOLAND 0x0043ffb0
void RenderBlokeIn3D(struct Bloke *bloke) {
    struct Person *person;

    person = bloke->person;
    if (person != 0) {
        Render3DPerson(person);
    }
}

// FUNCTION: LEGOLAND 0x0043ffd0
void SortBlokeIn3D(struct Bloke *bloke) {
    struct {
        unsigned int field_0;
        struct Bloke *bloke;
        unsigned short field_8;
    } info;

    info.field_0 = 0x306;
    info.bloke = bloke;
    info.field_8 = 0;
    if (bloke->person != 0) {
        SortPerson(bloke->person, bloke->person->sort_id, &info);
    }
}

// FUNCTION: LEGOLAND 0x00440010
void IP_RenderBlokeIn3DNow(struct Bloke *bloke) {
    RenderBlokeIn3D(bloke);
}

// FUNCTION: LEGOLAND 0x00440020
void SetPersonRotation(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004400b0
void SetPersonDirection(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00440190
void SetPersonPosition(struct Person *person, unsigned int x, unsigned int y) {
    person->field_1c = x;
    person->field_20 = y;
}

// FUNCTION: LEGOLAND 0x004401b0
void FUN_004401b0(unsigned int param_1, struct Person *person) { STUB(); }

// FUNCTION: LEGOLAND 0x00440290
void UpdatePerson(struct Person *person) {
    if ((person->flags & 0x80) != 0) {
        return;
    }
    if (person->next == 0) {
        return;
    }
    FUN_004401b0((unsigned int)person->next, person);
}

// FUNCTION: LEGOLAND 0x004402b0
void Control3DPeople(void) {
    struct Person *person;

    person = FirstBloke;
    if (person == 0) {
        return;
    }
    do {
        UpdatePerson(person);
        person = person->prev;
    } while (person != 0);
}

// FUNCTION: LEGOLAND 0x004402d0
void FUN_004402d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00440350
void InitMan(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004405a0
void UnInitMan(void) {
    unsigned int *p;

    if (DAT_0081c8c0 != 0) {
        FUN_0049e4d0(DAT_0081c8c0);
    }
    if (DAT_0081c8c8 != 0) {
        FUN_0049e4d0(DAT_0081c8c8);
    }
    if (DAT_0081c8c4 != 0) {
        FUN_0049e4d0(DAT_0081c8c4);
    }
    if (DAT_00630100 != 0) {
        FUN_0049e4d0(DAT_00630100);
    }
    if (DAT_0062feac != 0) {
        FUN_0049e4d0(DAT_0062feac);
    }
    p = DAT_0062febc;
    do {
        if (*p != 0) {
            FUN_0043fde0((void *)*p);
        }
        p++;
    } while ((int)p < (int)DAT_0062fed4);
    p = DAT_0062fed4;
    do {
        if (*p != 0) {
            FUN_0043fde0((void *)*p);
        }
        p++;
    } while ((int)p < (int)DAT_0062feec);
    p = DAT_0062feb0;
    do {
        if (*p != 0) {
            FUN_0043fde0((void *)*p);
        }
        p++;
    } while ((int)p < (int)DAT_0062feb8);
    if (DAT_0062fef4 != 0) {
        FUN_0043fde0(DAT_0062fef4);
    }
    FUN_00442c70();
    FUN_00486250();
    FUN_004886a0();
    FUN_00485fa0();
}

// FUNCTION: LEGOLAND 0x00440680
void Add3DBlokeToList(struct Bloke *bloke, unsigned int param_2) {
    struct Person *person;

    person = FUN_0043f8c0((unsigned int)bloke, param_2);
    bloke->person = person;
    if (person != 0) {
        FUN_0043f810(person);
        FUN_004401b0((unsigned int)person, (struct Person *)bloke);
        BlokeWalkAnim(bloke);
    }
}

// FUNCTION: LEGOLAND 0x004406c0
void BlokeSetAnim(struct Bloke *bloke, unsigned int anim) { STUB(); }

// FUNCTION: LEGOLAND 0x00440780
void BlokeSitAnim(struct Bloke *bloke) {
    BlokeSetAnim(bloke, 0);
}

// FUNCTION: LEGOLAND 0x00440790
struct Anim3D *GetBlokeAnim3D(struct Bloke *bloke) { STUB(); }

// FUNCTION: LEGOLAND 0x00440800
void GetBlokeAnim3DFromPerson(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00440870
void BlokeSetFrame(struct Bloke *bloke, int frame) {
    struct Person *person;
    struct Anim3D *anim;

    person = bloke->person;
    if (person != 0) {
        anim = GetBlokeAnim3D(bloke);
        person->field_4c = frame % anim->divisor;
    }
}

// FUNCTION: LEGOLAND 0x004408a0
void PlayBlokeAnim(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004408e0
void BlokeAnimNextFrame(struct Bloke *bloke) {
    struct Person *person;
    struct Anim3D *anim;

    person = bloke->person;
    if (person != 0) {
        anim = GetBlokeAnim3D(bloke);
        person->field_4c = (person->field_4c + 1) % anim->divisor;
    }
}

// FUNCTION: LEGOLAND 0x00440910
void BlokeWalkAnim(struct Bloke *bloke) { STUB(); }

// FUNCTION: LEGOLAND 0x00440960
void BlokeWalkWithPan(struct Bloke *bloke) {
    BlokeSetAnim(bloke, 5);
}

// FUNCTION: LEGOLAND 0x00440970
void BlokePanWithPan(struct Bloke *bloke) {
    BlokeSetAnim(bloke, 4);
}

// FUNCTION: LEGOLAND 0x00440980
void FUN_00440980(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00440a30
void FUN_00440a30(void) { STUB(); }
