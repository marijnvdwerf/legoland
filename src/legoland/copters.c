#include "legoland.h"

#include "ride_queue.h"

typedef void (*CopterVtblFn)(void);

struct CopterInterface {
    unsigned char pad_0[0x8c];
    CopterVtblFn field_8c;
    unsigned char pad_90[8];
    CopterVtblFn field_98;
    CopterVtblFn field_9c;
    CopterVtblFn field_a0;
    CopterVtblFn field_a4;
    CopterVtblFn field_a8;
    CopterVtblFn field_ac;
    CopterVtblFn field_b0;
    unsigned char pad_b4[4];
    CopterVtblFn field_b8;
    CopterVtblFn field_bc;
};

struct CopterNode {
    unsigned short field_0;
    unsigned char pad_2[2];
    struct CopterNode *next;
    unsigned char pad_8[0xd8 - 0x8];
};

struct CopterSource {
    unsigned short field_0;
};

struct CopterItem {
    unsigned char pad_0[8];
    struct CopterSub *field_8;
};

struct CopterSub {
    unsigned char pad_0[0x50];
    int field_50;
};

struct Sprite;
struct Position;

extern struct CopterNode *DAT_004c11b4;
extern struct Sprite *DAT_004c1120;
extern int DAT_004c1124[6];
extern struct Sprite *DAT_004c113c[10];
extern struct Sprite *DAT_004c1164;
extern void *DAT_004c1198;
extern struct Position *DAT_00830f98;
extern unsigned char Helicopter_SFX[];
extern void *DAT_008119b8;
extern unsigned int EditMode;
extern unsigned int EditCursor;

extern void *_malloc(unsigned int size);
extern int __strcmpi(const char *s1, const char *s2);
extern void KillSprite(struct Sprite *sprite);
extern void Kill_FXList(unsigned char *list, unsigned int count);
extern void UnloadPos(struct Position *pos);
extern void ResumeSinglyPausedSample(unsigned int param);
extern void DefaultCursor(unsigned int *cursor);
extern void SetEditCursorFootPrint(void *footprint);

void FUN_00403c80(struct CopterNode *node);
void FUN_00403e90(struct CopterNode *node);
void FUN_00404a90(struct CopterNode *node);

// FUNCTION: LEGOLAND 0x00403c40
void FUN_00403c40(struct CopterSource *src) {
    struct CopterNode *node = (struct CopterNode *)_malloc(0xd8);
    if (node != NULL) {
        memset(node, 0, 0xd8);
        node->field_0 = src->field_0;
        node->next = DAT_004c11b4;
        DAT_004c11b4 = node;
    }
    FUN_00403e90(node);
}

// FUNCTION: LEGOLAND 0x00403c80
void FUN_00403c80(struct CopterNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00403ce0
void FUN_00403ce0(void) {
    while (DAT_004c11b4 != NULL) {
        FUN_00403c80(DAT_004c11b4);
    }
}

// FUNCTION: LEGOLAND 0x00403d00
void FUN_00403d00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00403d30
int FUN_00403d30(struct CopterItem *item) {
    struct CopterSub *sub = item->field_8;
    int *ptr = &DAT_004c1124[0];
    int index = 0;

    while (1) {
        if (sub->field_50 == *ptr) {
            break;
        }
        ptr++;
        index++;
        if ((int)ptr < (int)&DAT_004c113c) {
            continue;
        }
        index = -1;
        break;
    }

    sub->field_50 = index;
    return index;
}

// FUNCTION: LEGOLAND 0x00403d60
void FUN_00403d60(struct CopterItem *item) {
    struct CopterSub *sub = item->field_8;
    int index = sub->field_50;

    if (index < 0 || index >= 6) {
        sub->field_50 = 0;
    } else {
        sub->field_50 = DAT_004c1124[index];
    }
}

// FUNCTION: LEGOLAND 0x00403d90
void FUN_00403d90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00403e90
void FUN_00403e90(struct CopterNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00404040
void FUN_00404040(void) {
    struct Sprite **current;

    if (DAT_004c1120 != NULL) {
        KillSprite(DAT_004c1120);
    }

    current = DAT_004c113c;
    while ((int)current < (int)&DAT_004c1164) {
        if (*current != NULL) {
            KillSprite(*current);
        }
        current++;
    }

    if (DAT_004c1124[0] != 0) {
        FUN_00412290((struct Sprite *)DAT_004c1124[0]);
    }
    if (DAT_004c1124[1] != 0) {
        FUN_00412290((struct Sprite *)DAT_004c1124[1]);
    }
    if (DAT_004c1124[3] != 0) {
        FUN_00412290((struct Sprite *)DAT_004c1124[3]);
    }
    if (DAT_004c1124[4] != 0) {
        FUN_00412290((struct Sprite *)DAT_004c1124[4]);
    }
    if (DAT_004c1124[2] != 0) {
        FUN_00412290((struct Sprite *)DAT_004c1124[2]);
    }

    FUN_00403ce0();
    Kill_FXList(Helicopter_SFX, 4);
    UnloadPos(DAT_00830f98);
}

// FUNCTION: LEGOLAND 0x004040f0
void FUN_004040f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404290
void FUN_00404290(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404450
void FUN_00404450(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_004c1198;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((unsigned char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00404490
void FUN_00404490(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404580
void FUN_00404580(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404600
void FUN_00404600(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404630
void FUN_00404630(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404860
void FUN_00404860(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004048a0
unsigned int FUN_004048a0(unsigned int param) {
    ResumeSinglyPausedSample(param);
    return 0;
}

// FUNCTION: LEGOLAND 0x004048b0
void FUN_004048b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004049a0
void FUN_004049a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404a90
void FUN_00404a90(struct CopterNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00404bc0
void FUN_00404bc0(void) {
    struct CopterNode *node = DAT_004c11b4;
    if (node == NULL) {
        return;
    }
    do {
        FUN_00404a90(node);
        node = node->next;
    } while (node != NULL);
}

// FUNCTION: LEGOLAND 0x00404be0
void FUN_00404be0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404f20
void FUN_00404f20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00404f60
void Copters_Save(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00405050
void Copters_Load(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00405110
void FUN_00405110(const char **name, struct CopterInterface *interfaces) {
    // STRING: LEGOLAND 0x004b43e4
    if (__strcmpi("COPTERS", *name) == 0) {
        interfaces->field_a4 = FUN_00403d90;
        interfaces->field_8c = FUN_00404450;
        interfaces->field_98 = FUN_00404600;
        interfaces->field_9c = FUN_00404580;
        interfaces->field_a8 = FUN_00404be0;
        interfaces->field_a0 = FUN_00404490;
        interfaces->field_b0 = FUN_00404290;
        interfaces->field_ac = FUN_00404040;
        interfaces->field_bc = Copters_Save;
        interfaces->field_b8 = Copters_Load;
    }
}
