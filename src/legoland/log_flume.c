#include "legoland.h"

extern void *_malloc(unsigned int size);
extern void FUN_0049e5c5(const char *msg);

extern unsigned int DAT_004cbe20;
extern unsigned int DAT_004cbe24;
extern unsigned int DAT_004cbe28;
extern unsigned int DAT_004cbe2c;

struct Sprite;

extern struct Sprite *DAT_004cbe4c;
extern struct Sprite *DAT_004cbe74;
extern struct Sprite *DAT_004cbe78;
extern struct Sprite *DAT_004cbe7c;
extern struct Sprite *DAT_004cbe80;
extern struct Sprite *DAT_004cbe88;
extern struct Sprite *DAT_004cbe8c;
extern struct Sprite *DAT_004cbe90;
extern struct Sprite *DAT_004cbe94;
extern struct Sprite *DAT_004cbe98;
extern void *DAT_004cbe84;
extern void *DAT_004c2ae8;
extern void *DAT_004c2af8;
extern unsigned int DAT_004c2b9c;
extern unsigned int EditMode;
extern void *DAT_008119b8;
extern unsigned int EditCursor;
extern unsigned int DAT_008003e8;
extern unsigned int DAT_004cbdd8;
extern unsigned int DAT_004c2a88;
extern unsigned int DAT_004c5c90;
extern unsigned int DAT_004c74c8;
extern struct CursorSource *DAT_004c445c;
extern struct CursorSource *DAT_004c2aa0;
extern struct CursorSource *DAT_004c2b0c;
extern unsigned int DAT_004c2af4;
extern unsigned int DAT_004c2b98;
extern unsigned int DAT_004c2b6c;
extern unsigned int DAT_004c2b70;
extern unsigned int DAT_004b47f8;
extern unsigned int DAT_004b4804;

extern unsigned int LoadSprite(const char *name, int flags);

extern unsigned int DAT_004b4728;
extern unsigned int DAT_004b472c;
extern unsigned int DAT_004b4730;
extern unsigned int DAT_004b4734;
extern unsigned int DAT_004c2afc;
extern struct Sprite *DAT_004c2abc;
extern struct Sprite *DAT_004c2ae4;
extern struct Sprite *DAT_004cbe1c;
extern struct Sprite *DAT_004c8d68;
extern unsigned int DAT_004c2b10;

extern void KillSprite(struct Sprite *sprite);
extern void FUN_00412290(void *param);
extern void FUN_0049e4d0(void *block);
extern void DefaultCursor(unsigned int *cursor);
extern void SetEditCursorFootPrint(unsigned int param);
extern void LLIDB_UnLoadData(unsigned int handle);
extern void BuildCursorPtr(void *cursor, unsigned int param_2, unsigned int param_3);

struct ChainNode {
    struct ChainNode *next;
};

struct CleanupNode {
    struct CleanupNode *next;
    unsigned char pad_4[0x10 - 0x4];
    struct ChainNode *chain;
};

struct ParticleEmitter {
    unsigned char pad_0[0x3c];
    unsigned int var_3c;
    unsigned char pad_40[0xd0 - 0x40];
    unsigned int var_d0;
};

struct CursorSource {
    unsigned char pad_0[0x3c];
    unsigned int var_3c[5];
};

struct Obj {
    unsigned char pad_0[0xc];
    void *field_0c;
};

struct ResA {
    unsigned char pad_0[0x1c];
    unsigned int flags_1c;
    unsigned char pad_20[0x64 - 0x20];
    struct ResB *ptr_64;
};

struct ResB {
    unsigned char pad_0[0x10];
    unsigned int flags_10;
};

struct WalkNode {
    struct WalkNode *var_0;
    unsigned int var_4;
    struct WalkNode *var_8;
};

struct PairHolder {
    unsigned char pad_0[0x2c];
    unsigned int var_2c;
    struct StateNode *var_30;
    struct StateNode *var_34;
};

struct SubBuf {
    unsigned char b0;
    unsigned char b1;
};

struct LinkNode {
    unsigned int field_0;
    unsigned int field_4;
    struct Context *field_8;
    struct Context *field_c;
};

struct Context {
    unsigned char pad_0[0x2c];
    unsigned int var_2c;
    struct Context *var_30;
    struct Context *var_34;
};

struct LinkList {
    struct LinkNode *field_0;
    struct LinkNode *field_4;
    struct LinkNode *field_8;
    struct LinkNode *field_c;
};

struct InputBuffer {
    unsigned char flags;
    unsigned char pad_0[3];
    unsigned char var_4;
    unsigned char pad_4[3];
    unsigned char var_8;
    unsigned char pad_8[3];
    unsigned char var_c;
    unsigned char pad_c[3];
    unsigned char var_10;
    unsigned char pad_10[3];
    unsigned char var_14;
    unsigned char pad_14[3];
    unsigned char var_18;
    unsigned char pad_18[3];
    unsigned char var_1c;
    unsigned char pad_1c[3];
    unsigned char var_20;
};

struct Node {
    unsigned char pad_0[8];
    struct Node *next;
    struct Node *prev;
};

struct ListNode {
    struct ListNode *prev;
    struct ListNode *next;
    struct ListNode *up;
    struct ListNode *down;
};

struct ListContainer {
    unsigned char pad_0[0x10];
    struct ListNode *head_10;
    unsigned char pad_14[0x24 - 0x14];
    struct Node *head_24;
    unsigned char pad_28[0x2c - 0x28];
    struct ListNode *tail_2c;
};

struct Slot {
    unsigned char pad_0[0x24];
    unsigned int key;
};

struct StateNode {
    unsigned char pad_0[0x18];
    unsigned int state;
    unsigned int phase;
};

struct StateSlots {
    struct StateNode *slot0;
    struct StateNode *slot1;
    struct StateNode *slot2;
    struct StateNode *slot3;
};

// FUNCTION: LEGOLAND 0x00408e40
void FUN_00408e40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00408e80
void FUN_00408e80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00408ec0
void FUN_00408ec0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00408ef0
void FUN_00408ef0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00408f30
unsigned int FUN_00408f30(struct SubBuf *buf) { STUB(); }

// FUNCTION: LEGOLAND 0x00408f90
void FUN_00408f90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00409010
void *FUN_00409010(void) {
    void *block = _malloc(0x38);
    if (block != NULL) {
        memset(block, 0, 0x38);
    }
    return block;
}

// FUNCTION: LEGOLAND 0x00409040
void FUN_00409040(struct Node *node, struct Node *insert) {
    if (node->prev != NULL) {
        node->prev->next = insert;
    }
    insert->next = node;
    if (node->prev != NULL) {
        // STRING: LEGOLAND 0x004b48e0
        FUN_0049e5c5("bug");
    }
    node->prev = insert;
}

// FUNCTION: LEGOLAND 0x00409080
void FUN_00409080(struct Node *node, struct Node *insert) {
    if (node->next != NULL) {
        node->next->prev = insert;
    }
    insert->prev = node;
    if (node->next != NULL) {
        // STRING: LEGOLAND 0x004b48e0
        FUN_0049e5c5("bug");
    }
    node->next = insert;
}

// FUNCTION: LEGOLAND 0x004090c0
void FUN_004090c0(struct Node *node1, struct Node *node2, struct Node *node3) {
    node3->prev = node1;
    node3->next = node2;
    node1->next = node3;
    node2->prev = node3;
}

// FUNCTION: LEGOLAND 0x004090e0
struct Node *FUN_004090e0(struct ListContainer *container) { STUB(); }

// FUNCTION: LEGOLAND 0x00409110
void FUN_00409110(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00409140
int FUN_00409140(struct ListContainer *container) {
    struct Node *node = FUN_004090e0(container);
    struct Node *head;
    if (node == NULL) {
        return 0;
    }
    head = container->head_24->next;
    while (node != NULL) {
        if (node == head) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00409170
void FUN_00409170(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004091a0
void FUN_004091a0(struct ListContainer *container, struct ListNode *node) {
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node == container->tail_2c) {
        container->tail_2c = node->prev;
    }
    if (node->down != NULL) {
        node->down->up = NULL;
    }
    if (node->up != NULL) {
        node->up->down = NULL;
    }
}

// FUNCTION: LEGOLAND 0x004091f0
void FUN_004091f0(struct ListContainer *container, struct ListNode *node) {
    struct ListNode *head = container->head_10;
    node->next = NULL;
    node->prev = head;
    head = container->head_10;
    if (head != NULL) {
        head->next = node;
    }
    container->head_10 = node;
}

// FUNCTION: LEGOLAND 0x00409220
void FUN_00409220(struct ListContainer *container, struct ListNode *node) {
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node == container->head_10) {
        container->head_10 = node->prev;
    }
    if (node->down != NULL) {
        node->down->up = NULL;
    }
    if (node->up != NULL) {
        node->up->down = NULL;
    }
}

// FUNCTION: LEGOLAND 0x00409270
void FUN_00409270(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004092b0
void FUN_004092b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00409360
void FUN_00409360(unsigned int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x00409410
int FUN_00409410(unsigned int *ctx) {
    int result = 0;
    if (ctx[0] != 0) {
        result = 1;
    }
    if (ctx[1] != 0) {
        result |= 0x4;
    }
    if (ctx[2] != 0) {
        result |= 0x10;
    }
    if (ctx[3] != 0) {
        result |= 0x40;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00409440
void FUN_00409440(unsigned int param_1, void **param_2) {
    DAT_004cbe20 = 0;
    DAT_004cbe24 = 0;
    DAT_004cbe28 = 0;
    DAT_004cbe2c = 0;
    FUN_00409360(param_1);
    *param_2 = &DAT_004cbe20;
}

// FUNCTION: LEGOLAND 0x00409470
unsigned int FUN_00409470(void) {
    unsigned int result = 0;
    if (DAT_004cbe20 != 0) {
        result = 1;
    }
    if (DAT_004cbe24 != 0) {
        result++;
    }
    if (DAT_004cbe28 != 0) {
        result++;
    }
    if (DAT_004cbe2c != 0) {
        result++;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x004094b0
void FUN_004094b0(unsigned int arg, struct Slot **slots) {
    if (slots[0] != NULL && slots[0]->key != arg) {
        slots[0] = NULL;
    }
    if (slots[1] != NULL && slots[1]->key != arg) {
        slots[1] = NULL;
    }
    if (slots[2] != NULL && slots[2]->key != arg) {
        slots[2] = NULL;
    }
    if (slots[3] != NULL && slots[3]->key != arg) {
        slots[3] = NULL;
    }
}

// FUNCTION: LEGOLAND 0x00409510
void FUN_00409510(struct StateSlots *slots) {
    if (slots->slot0 != NULL && slots->slot0->state != 3 && slots->slot0->state != 4) {
        slots->slot0 = NULL;
    }
    if (slots->slot1 != NULL && slots->slot1->state != 3 && slots->slot1->state != 4) {
        slots->slot1 = NULL;
    }
    if (slots->slot2 != NULL && slots->slot2->state != 3 && slots->slot2->state != 4) {
        slots->slot2 = NULL;
    }
    if (slots->slot3 != NULL && slots->slot3->state != 3 && slots->slot3->state != 4) {
        slots->slot3 = NULL;
    }
}

// FUNCTION: LEGOLAND 0x00409580
void FUN_00409580(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00409620
void FUN_00409620(struct StateNode *node) {
    if (node->state == 3) {
        if (node->phase == 2) {
            node->state = 1;
            node->phase = 0;
            return;
        }
        if (node->phase == 1) {
            node->state = 2;
            node->phase = 2;
            return;
        }
        if (node->phase == 3) {
            node->state = 2;
            node->phase = 1;
            return;
        }
    }
    if (node->state == 4) {
        node->state = 3;
        node->phase = 0;
    }
}

// FUNCTION: LEGOLAND 0x00409680
void FUN_00409680(struct StateNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x004096e0
void FUN_004096e0(struct StateNode *node) {
    if (node->state == 3) {
        if (node->phase == node->state) {
            node->state = 1;
            node->phase = 1;
            return;
        }
        if (node->phase == 0) {
            node->state = 2;
            node->phase = 2;
            return;
        }
        if (node->phase == 2) {
            node->state = node->phase;
            node->phase = 3;
            return;
        }
    }
    if (node->state == 4) {
        node->state = 3;
        node->phase = 1;
    }
}

// FUNCTION: LEGOLAND 0x00409740
void FUN_00409740(struct StateNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x004097a0
void FUN_004097a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00409a50
void FUN_00409a50(void *param) { STUB(); }

// FUNCTION: LEGOLAND 0x00409a90
void FUN_00409a90(void *edi_ptr[4], struct StateNode *esi_ptr[4]) {
    int flags = FUN_00409410((unsigned int *)esi_ptr);
    if (flags & 0x1) {
        FUN_00409a50(edi_ptr[0]);
        FUN_00409620(esi_ptr[0]);
    }
    if (flags & 0x4) {
        FUN_00409a50(edi_ptr[1]);
        FUN_004096e0(esi_ptr[1]);
    }
    if (flags & 0x10) {
        FUN_00409a50(edi_ptr[2]);
        FUN_00409680(esi_ptr[2]);
    }
    if (flags & 0x40) {
        FUN_00409a50(edi_ptr[3]);
        FUN_00409740(esi_ptr[3]);
    }
}

// FUNCTION: LEGOLAND 0x00409b10
void FUN_00409b10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00409b70
void FUN_00409b70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00409c20
void FUN_00409c20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040a010
void FUN_0040a010(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040a080
void FUN_0040a080(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040a0f0
void FUN_0040a0f0(struct StateNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->state == 3 && node->phase == 0) {
        node->state = 4;
        return;
    }
    if (node->state == 1 && node->phase == 0) {
        node->state = 3;
        node->phase = 2;
        return;
    }
    if (node->state == 2) {
        if (node->phase == 2) {
            node->state = 3;
            node->phase = 1;
            return;
        } else if (node->phase == 1) {
            node->state = 3;
            node->phase = 3;
        }
    }
}

// FUNCTION: LEGOLAND 0x0040a160
void FUN_0040a160(struct StateNode *node) {
    if (node != NULL) {
        if (node->state == 3 && node->phase == 1) {
            node->state = 4;
        } else if (node->state == 1 && node->phase == 1) {
            node->state = 3;
            node->phase = 3;
        } else if (node->state == 2) {
            if (node->phase == 3) {
                node->state = 3;
                node->phase = 2;
            } else if (node->phase == 2) {
                node->state = 3;
                node->phase = 0;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0040a1d0
void FUN_0040a1d0(struct StateNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->state == 3 && node->phase == 2) {
        node->state = 4;
        return;
    }
    if (node->state == 1 && node->phase == 0) {
        node->state = 3;
        node->phase = 0;
        return;
    }
    if (node->state == 2) {
        if (node->phase == 3) {
            node->state = 3;
            node->phase = 1;
            return;
        }
        if (node->phase == 0) {
            node->state = 3;
            node->phase = 3;
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x0040a230
void FUN_0040a230(struct StateNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->state == 3 && node->phase == 3) {
        node->state = 4;
    } else if (node->state == 1 && node->phase == 1) {
        node->state = 3;
        node->phase = 1;
    } else if (node->state == 2) {
        if (node->phase == 0) {
            node->state = 3;
            node->phase = 2;
        } else if (node->phase == 1) {
            node->state = 3;
            node->phase = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x0040a2a0
void FUN_0040a2a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040a2e0
void FUN_0040a2e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040a410
void FUN_0040a410(void) {
    struct CleanupNode *current;

    if (DAT_004cbe4c) {
        KillSprite(DAT_004cbe4c);
    }
    if (DAT_004cbe94) {
        KillSprite(DAT_004cbe94);
    }
    if (DAT_004cbe90) {
        KillSprite(DAT_004cbe90);
    }
    if (DAT_004cbe98) {
        KillSprite(DAT_004cbe98);
    }
    if (DAT_004cbe8c) {
        KillSprite(DAT_004cbe8c);
    }
    if (DAT_004cbe88) {
        KillSprite(DAT_004cbe88);
    }
    if (DAT_004cbe74) {
        KillSprite(DAT_004cbe74);
    }
    if (DAT_004cbe78) {
        KillSprite(DAT_004cbe78);
    }
    if (DAT_004c2af8) {
        FUN_00412290(DAT_004c2af8);
    }
    if (DAT_004c2ae8) {
        FUN_00412290(DAT_004c2ae8);
    }
    if (DAT_004cbe7c) {
        KillSprite(DAT_004cbe7c);
    }
    if (DAT_004cbe80) {
        KillSprite(DAT_004cbe80);
    }

    current = (struct CleanupNode *)DAT_004cbe84;
    if (current != NULL) {
        while (current != NULL) {
            struct CleanupNode *next = current->next;
            struct ChainNode *chain = current->chain;
            while (chain != NULL) {
                struct ChainNode *next_chain = chain->next;
                FUN_0049e4d0(chain);
                chain = next_chain;
            }
            FUN_0049e4d0(current);
            current = next;
        }
        DAT_004cbe84 = current;
        return;
    }
    DAT_004cbe84 = NULL;
}

// FUNCTION: LEGOLAND 0x0040a540
void FUN_0040a540(void) {
    EditMode = 1;
    DAT_008119b8 = (void *)DAT_004c2b9c;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((unsigned int)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x0040a580
void FUN_0040a580(struct ParticleEmitter *param) { STUB(); }

// FUNCTION: LEGOLAND 0x0040a5d0
void FUN_0040a5d0(struct ParticleEmitter *param) {
    if (param != NULL) {
        param->var_3c = 4;
        param->var_d0 = 6;
        FUN_0040a580(param);
    }
}

// FUNCTION: LEGOLAND 0x0040a600
void FUN_0040a600(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040a930
void FUN_0040a930(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040aac0
void FUN_0040aac0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040abf0
void FUN_0040abf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ad50
void FUN_0040ad50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040adb0
void FUN_0040adb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ae90
void FUN_0040ae90(unsigned int param_1, int param_2, int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x0040b210
void FUN_0040b210(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040b270
unsigned char FUN_0040b270(unsigned int *param_1, unsigned int param_2) {
    return param_1[5] == param_2;
}

// FUNCTION: LEGOLAND 0x0040b290
void FUN_0040b290(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040b390
void FUN_0040b390(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040b420
void FUN_0040b420(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ba80
int FUN_0040ba80(struct Node *arg) {
    struct Node *current = arg->next;
    if (current != NULL) {
        struct Node *target = arg->prev;
        while (current != NULL) {
            current = current->next;
            if (current == target) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040bab0
void FUN_0040bab0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040bb50
void FUN_0040bb50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040bbb0
void FUN_0040bbb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040bd40
void FUN_0040bd40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040be00
void FUN_0040be00(struct ChainNode *param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x0040bf50
void FUN_0040bf50(void) {
    struct ChainNode *current = (struct ChainNode *)DAT_004cbe84;
    while (current != NULL) {
        FUN_0040be00(current);
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0040bf70
void FUN_0040bf70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040c250
void FUN_0040c250(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040c2e0
void FUN_0040c2e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040c350
void FUN_0040c350(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040c430
void FUN_0040c430(void) {
    struct Sprite **ptr;

    LLIDB_UnLoadData(DAT_004c2afc);

    ptr = &DAT_004c2abc;
    while ((int)ptr < (int)&DAT_004c2ae4) {
        if (*ptr != NULL) {
            KillSprite(*ptr);
        }
        ptr++;
    }

    if (DAT_004cbe1c != NULL) {
        KillSprite(DAT_004cbe1c);
    }
    if (DAT_004c8d68 != NULL) {
        KillSprite(DAT_004c8d68);
    }

    LLIDB_UnLoadData(DAT_004c2b10);
}

// FUNCTION: LEGOLAND 0x0040c4a0
void FUN_0040c4a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040c6c0
void FUN_0040c6c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040c780
void FUN_0040c780(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040c8d0
void FUN_0040c8d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040c970
void FUN_0040c970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ca30
void FUN_0040ca30(void *a1, int a2) {
    struct WalkNode *node = *(struct WalkNode **)a1;
    if (node != NULL) {
        do {
            if (node->var_4 != 0) {
                FUN_0040ae90(node->var_4, a2, 1);
            }
            node = node->var_8;
        } while (node != NULL);
    }
}

// FUNCTION: LEGOLAND 0x0040ca60
void FUN_0040ca60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040cc00
void FUN_0040cc00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040cc50
void FUN_0040cc50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040cca0
unsigned long FUN_0040cca0(struct StateNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x0040cd70
void FUN_0040cd70(struct PairHolder *p, int param1, unsigned long param2, unsigned long param3, unsigned long param4) {
    struct StateNode *first;
    struct StateNode *second;
    unsigned int phase;

    if (p->var_2c == 0) {
        return;
    }

    first = p->var_30;
    second = p->var_34;

    if (first->state == 3) {
        phase = first->phase;
        if (param1 != 0) {
            if (phase != 0 && phase != 3) {
                FUN_0040cca0(first);
            }
        } else {
            if (phase != 1 && phase != 2) {
                FUN_0040cca0(first);
            }
        }
    }

    if (second->state == 3) {
        phase = second->phase;
        if (param1 != 0) {
            if (phase != 0 && phase != 3) {
                FUN_0040cca0(second);
            }
        } else {
            if (phase != 1 && phase != 2) {
                FUN_0040cca0(second);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0040cdf0
void FUN_0040cdf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ce20
void FUN_0040ce20(struct InputBuffer *esi) {
    unsigned int eax = DAT_004b4730;
    unsigned int edx = DAT_004b472c;
    unsigned int ebx = DAT_004b4734;
    unsigned int local_8 = eax - DAT_004b4728;
    struct SubBuf buf;

    ebx = ebx - edx;

    DAT_004cbe20 = 0;
    DAT_004cbe24 = 0;
    DAT_004cbe28 = 0;
    DAT_004cbe2c = 0;

    if (esi->flags & 1) {
        buf.b0 = esi->var_4;
        buf.b1 = esi->var_8 - (unsigned char)ebx;
        DAT_004cbe20 = FUN_00408f30(&buf);
    }
    if (esi->flags & 2) {
        buf.b0 = esi->var_c + (unsigned char)local_8;
        buf.b1 = esi->var_10;
        DAT_004cbe24 = FUN_00408f30(&buf);
    }
    if (esi->flags & 4) {
        buf.b0 = esi->var_14;
        buf.b1 = esi->var_18 + (unsigned char)ebx;
        DAT_004cbe28 = FUN_00408f30(&buf);
    }
    if (esi->flags & 8) {
        buf.b0 = esi->var_1c - (unsigned char)local_8;
        buf.b1 = esi->var_20;
        DAT_004cbe2c = FUN_00408f30(&buf);
    }
}

// FUNCTION: LEGOLAND 0x0040cf10
void FUN_0040cf10(struct InputBuffer *param_1, unsigned int **param_2) {
    FUN_0040ce20(param_1);
    *param_2 = &DAT_004cbe20;
}

// FUNCTION: LEGOLAND 0x0040cf30
unsigned int FUN_0040cf30(unsigned int *param_1) {
    unsigned int count = 0;
    int i;
    for (i = 0; i < 4; i++) {
        if (param_1[i] != 0) {
            count++;
        }
    }
    return count;
}

// FUNCTION: LEGOLAND 0x0040cf50
void FUN_0040cf50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040cf80
unsigned int FUN_0040cf80(struct Slot **arg) {
    int i;
    for (i = 0; i < 4; i++) {
        if (arg[i] != NULL) {
            return arg[i]->key;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040cfa0
void FUN_0040cfa0(struct StateNode *(*arr)[4]) {
    int i;
    for (i = 0; i < 4; i++) {
        struct StateNode *p = (*arr)[i];
        if (p != NULL) {
            if (p->state != 3 && p->state != 4) {
                (*arr)[i] = NULL;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0040cfd0
void FUN_0040cfd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040d090
void FUN_0040d090(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040d210
void FUN_0040d210(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040d2d0
void FUN_0040d2d0(unsigned int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x0040d3b0
unsigned int FUN_0040d3b0(void *param_1, unsigned int *param_2) {
    EditMode = 1;
    DAT_008119b8 = param_1;
    DefaultCursor(&EditCursor);
    DAT_008003e8 |= 8;
    BuildCursorPtr(&EditCursor, 0x8f8, 0);
    SetEditCursorFootPrint((unsigned int)param_2);
    DAT_004cbdd8 = 0x2034;
    DAT_004c2a88 = 0x2034;
    DAT_004c5c90 = 0x2034;
    DAT_004c74c8 = 0x2034;
    return 0x2034;
}

// FUNCTION: LEGOLAND 0x0040d420
void FUN_0040d420(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040d520
void FUN_0040d520(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040d6f0
void FUN_0040d6f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040d900
void FUN_0040d900(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040da10
void FUN_0040da10(struct Context *a, struct LinkList *list) {
    struct LinkNode *node;

    if (list->field_0 != NULL) {
        node = list->field_0;
        if (a->var_2c == 0) {
            if (node->field_8 != a && node->field_c != a) {
                list->field_0 = NULL;
            }
        } else {
            if (node->field_8 != a->var_30 && node->field_c != a->var_30 &&
                node->field_8 != a->var_34 && node->field_c != a->var_34) {
                list->field_0 = NULL;
            }
        }
    }

    if (list->field_4 != NULL) {
        node = list->field_4;
        if (a->var_2c == 0) {
            if (node->field_8 != a && node->field_c != a) {
                list->field_4 = NULL;
            }
        } else {
            if (node->field_8 != a->var_30 && node->field_c != a->var_30 &&
                node->field_8 != a->var_34 && node->field_c != a->var_34) {
                list->field_4 = NULL;
            }
        }
    }

    if (list->field_c != NULL) {
        node = list->field_c;
        if (a->var_2c == 0) {
            if (node->field_8 != a && node->field_c != a) {
                list->field_c = NULL;
            }
        } else {
            if (node->field_8 != a->var_30 && node->field_c != a->var_30 &&
                node->field_8 != a->var_34 && node->field_c != a->var_34) {
                list->field_c = NULL;
            }
        }
    }

    if (list->field_8 != NULL) {
        node = list->field_8;
        if (a->var_2c == 0) {
            if (node->field_8 != a && node->field_c != a) {
                list->field_8 = NULL;
            }
        } else {
            if (node->field_8 != a->var_30 && node->field_c != a->var_30 &&
                node->field_8 != a->var_34 && node->field_c != a->var_34) {
                list->field_8 = NULL;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0040db00
void FUN_0040db00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040dbb0
void FUN_0040dbb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040dc00
void FUN_0040dc00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e340
void FUN_0040e340(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e3b0
void FUN_0040e3b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e440
void FUN_0040e440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e630
void FUN_0040e630(void) {
    unsigned int local[5];
    memcpy(local, DAT_004c445c->var_3c, 20);
    FUN_0040d3b0(DAT_004c445c, local);
}

// FUNCTION: LEGOLAND 0x0040e660
void FUN_0040e660(void) {
    struct CursorSource *src = DAT_004c2aa0;
    unsigned int local[5];
    memcpy(local, src->var_3c, sizeof(local));
    FUN_0040d3b0(src, local);
}

// FUNCTION: LEGOLAND 0x0040e690
void FUN_0040e690(void) {
    struct CursorSource *src = DAT_004c2b0c;
    unsigned int local[5];
    memcpy(local, src->var_3c, sizeof(local));
    FUN_0040d3b0(src, local);
}

// FUNCTION: LEGOLAND 0x0040e6c0
void FUN_0040e6c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e6f0
void FUN_0040e6f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e740
void FUN_0040e740(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e790
void FUN_0040e790(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e7e0
void FUN_0040e7e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e830
void FUN_0040e830(unsigned int param_1, unsigned int param_2) {
    DAT_004c2af4 = 0;
    FUN_0040d2d0(param_2);
}

// FUNCTION: LEGOLAND 0x0040e850
void FUN_0040e850(unsigned int param_1, unsigned int param_2) {
    DAT_004c2af4 = 1;
    FUN_0040d2d0(param_2);
}

// FUNCTION: LEGOLAND 0x0040e870
void FUN_0040e870(unsigned int dummy, unsigned int param_1) {
    DAT_004c2af4 = 2;
    FUN_0040d2d0(param_1);
}

// FUNCTION: LEGOLAND 0x0040e890
void FUN_0040e890(unsigned int param_1, unsigned int param_2) {
    DAT_004c2af4 = 3;
    FUN_0040d2d0(param_2);
}

// FUNCTION: LEGOLAND 0x0040e8b0
void FUN_0040e8b0(struct Obj *obj_ptr) {
    struct ResA *resA;
    struct ResB *resB;

    DAT_004c445c = (struct CursorSource *)obj_ptr->field_0c;
    resA = (struct ResA *)DAT_004c445c;
    resA->flags_1c |= 0x400;

    resA = (struct ResA *)DAT_004c445c;
    if (resA != NULL) {
        resB = resA->ptr_64;
        if (resB != NULL) {
            resB->flags_10 |= 0x2000;
        }
    }

    DAT_004c2b98 = (unsigned int)obj_ptr;

    // STRING: LEGOLAND 0x004b4a58
    DAT_004c2b6c = LoadSprite("fc1_m1.lls", 1);
    // STRING: LEGOLAND 0x004b4a4c
    DAT_004c2b70 = LoadSprite("fc1_m2.lls", 1);

    DAT_004b47f8 = DAT_004c2b6c;
    DAT_004b4804 = DAT_004c2b70;
}

// FUNCTION: LEGOLAND 0x0040e920
void FUN_0040e920(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e970
void FUN_0040e970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e9e0
void FUN_0040e9e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ea30
void FUN_0040ea30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ea60
void FUN_0040ea60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ea80
void FUN_0040ea80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040eab0
void FUN_0040eab0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ead0
void FUN_0040ead0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040eb40
void FUN_0040eb40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ebb0
void FUN_0040ebb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ec20
void FUN_0040ec20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ec90
void FUN_0040ec90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ecc0
void FUN_0040ecc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ecf0
void FUN_0040ecf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ed20
void FUN_0040ed20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ed50
void FUN_0040ed50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040edb0
void FUN_0040edb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ee60
void FUN_0040ee60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ef00
void FUN_0040ef00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040efb0
void FUN_0040efb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f050
void FUN_0040f050(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f300
void FUN_0040f300(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f330
void FUN_0040f330(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f360
void FUN_0040f360(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f3e0
void FUN_0040f3e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f430
void FUN_0040f430(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f450
void FUN_0040f450(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f4f0
void FUN_0040f4f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f510
void FUN_0040f510(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f540
void FUN_0040f540(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f580
void FUN_0040f580(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f5a0
void FUN_0040f5a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f5b0
void FUN_0040f5b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f7d0
void FUN_0040f7d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f800
void FUN_0040f800(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f830
void FUN_0040f830(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f8b0
void FUN_0040f8b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f900
void FUN_0040f900(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f920
void FUN_0040f920(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040fa00
void FUN_0040fa00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040fa20
void FUN_0040fa20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040fa50
void FUN_0040fa50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040fa60
void FUN_0040fa60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040fab0
void FUN_0040fab0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040fad0
void FUN_0040fad0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040fe50
void FUN_0040fe50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040fe80
void FUN_0040fe80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040feb0
void FUN_0040feb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ff30
void FUN_0040ff30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ffa0
void FUN_0040ffa0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ffd0
void FUN_0040ffd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004100b0
void FUN_004100b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004100d0
void FUN_004100d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410100
void FUN_00410100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410110
void FUN_00410110(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410160
void FUN_00410160(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410180
void FUN_00410180(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004102e0
void FUN_004102e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410310
void FUN_00410310(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410360
void FUN_00410360(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004103e0
void FUN_004103e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410450
void FUN_00410450(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004104b0
void FUN_004104b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004106e0
void FUN_004106e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410700
void FUN_00410700(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410730
void FUN_00410730(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410740
void FUN_00410740(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410790
void FUN_00410790(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004107b0
void FUN_004107b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410800
void FUN_00410800(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410910
void FUN_00410910(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410930
void FUN_00410930(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410a50
void FUN_00410a50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410b60
void FUN_00410b60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410bb0
void FUN_00410bb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410c10
void FUN_00410c10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410d60
void FUN_00410d60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00411220
void FUN_00411220(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00411250
void FUN_00411250(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00411290
void FUN_00411290(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004112c0
void FUN_004112c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004112f0
void FUN_004112f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004113d0
void FUN_004113d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00411650
void FUN_00411650(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00411680
void FUN_00411680(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004117e0
void FUN_004117e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00411810
void FUN_00411810(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004119a0
void FUN_004119a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004119c0
void FUN_004119c0(void) { STUB(); }
