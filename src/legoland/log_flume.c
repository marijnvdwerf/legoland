#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "gamemap.h"
#include "ride_queue.h"
#include "log_flume.h"
#include "llidb.h"
#include "map_object.h"


struct Sprite;

#include "image_sprite.h"


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

typedef void (*FlumeCallback)(unsigned int *buf, void *res);

struct FlumePos {
    unsigned char pad_0[0x30];
    unsigned int var_30;
    unsigned int var_34;
};

struct FlumeRect {
    unsigned int var_0;
    unsigned int var_4;
    unsigned int var_8;
    unsigned int var_c;
};

struct FlumeSlot0 {
    unsigned int field_0;
    unsigned char pad_4[4];
    unsigned int var_8;
};

struct FlumeSlot1 {
    unsigned int field_0;
    unsigned char pad_4[4];
    unsigned int field_8;
    unsigned int var_c;
};

struct FlumeInput {
    struct FlumeSlot0 *var_0;
    unsigned char pad_4[4];
    struct FlumeSlot1 *var_8;
};

struct FlumeSlots {
    unsigned char pad_0[0x38];
    void *target_38;
    unsigned char pad_3c[4];
    unsigned char slots_40[1];
};

struct FlumeNode {
    struct FlumeNode *next;
    unsigned char pad_4[4];
    unsigned int field_8;
    unsigned int field_c;
    unsigned char pad_10[0x2c - 0x10];
    struct FlumeNode *field_2c;
    unsigned int field_30;
    unsigned int field_34;
};

struct FlumeDims {
    int field1;
    int field2;
};

struct FlumeBytes {
    unsigned char b0;
    unsigned char pad_1[3];
    unsigned char b4;
};

typedef void (*FlumeVtblFn)(void);

struct FlumeObj {
    char *name;
    unsigned char pad_4[0xc - 0x4];
    unsigned int field_c;
};

struct FlumeVtbl {
    unsigned char pad_0[0x8c];
    FlumeVtblFn var_8c;
    FlumeVtblFn var_90;
    FlumeVtblFn var_94;
    FlumeVtblFn var_98;
    FlumeVtblFn var_9c;
    FlumeVtblFn var_a0;
    FlumeVtblFn var_a4;
    FlumeVtblFn var_a8;
    FlumeVtblFn var_ac;
    FlumeVtblFn var_b0;
    unsigned char pad_b4[4];
    FlumeVtblFn var_b8;
    FlumeVtblFn var_bc;
    FlumeVtblFn var_c0;
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
    void *block = malloc(0x38);
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
        printf("bug");
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
        printf("bug");
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
                free(chain);
                chain = next_chain;
            }
            free(current);
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
    SetEditCursorFootPrint((void *)((unsigned int)DAT_008119b8 + 0x3c));
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
    EditCursor.field_1828 |= 8;
    BuildCursorPtr(&EditCursor, 0x8f8, 0);
    SetEditCursorFootPrint((void *)param_2);
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
unsigned int FUN_0040d6f0(struct CursorSource *param_1, unsigned int param_2, unsigned int param_3, unsigned int *param_4, void (*param_5)(void), void (*param_6)(void)) { STUB(); }

// FUNCTION: LEGOLAND 0x0040d900
unsigned int FUN_0040d900(unsigned int param_1, unsigned int *param_2, int param_3, void (*param_4)(void), void (*param_5)(void), void (*param_6)(void)) { STUB(); }

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
unsigned int FUN_0040db00(unsigned int param_1, unsigned int param_2, unsigned int param_3, FlumeCallback param_4) { STUB(); }

// FUNCTION: LEGOLAND 0x0040dbb0
void FUN_0040dbb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040dc00
void FUN_0040dc00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e340
void FUN_0040e340(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e3b0
void FUN_0040e3b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e440
void FUN_0040e440(unsigned int *buf, void *res) { STUB(); }

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
void FUN_0040e6c0(void) {
    struct CursorSource *src = DAT_004c74d4;
    unsigned int local[5];
    memcpy(local, src->var_3c, sizeof(local));
    FUN_0040d3b0(src, local);
}

// FUNCTION: LEGOLAND 0x0040e6f0
void FUN_0040e6f0(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct CursorSource *src = DAT_004c445c;
    unsigned int local[5];
    DAT_004c2af4 = 0;
    memcpy(local, src->var_3c, 20);
    FUN_0040d6f0(src, param_2, param_3, local, FUN_0040e440, FUN_0040e3b0);
}

// FUNCTION: LEGOLAND 0x0040e740
void FUN_0040e740(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct CursorSource *src = DAT_004c2aa0;
    unsigned int local[5];
    DAT_004c2af4 = 1;
    memcpy(local, src->var_3c, 20);
    FUN_0040d6f0(src, param_2, param_3, local, FUN_0040e440, FUN_0040e3b0);
}

// FUNCTION: LEGOLAND 0x0040e790
void FUN_0040e790(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct CursorSource *src = DAT_004c2b0c;
    unsigned int local[5];
    DAT_004c2af4 = 2;
    memcpy(local, src->var_3c, 20);
    FUN_0040d6f0(src, param_2, param_3, local, FUN_0040e440, FUN_0040e3b0);
}

// FUNCTION: LEGOLAND 0x0040e7e0
void FUN_0040e7e0(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct CursorSource *src = DAT_004c74d4;
    unsigned int local[5];
    DAT_004c2af4 = 3;
    memcpy(local, src->var_3c, 20);
    FUN_0040d6f0(src, param_2, param_3, local, FUN_0040e440, FUN_0040e3b0);
}

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
void FUN_0040e920(struct Obj *obj_ptr) {
    struct ResA *resA;
    struct ResB *resB;

    DAT_004c2aa0 = (struct CursorSource *)obj_ptr->field_0c;
    resA = (struct ResA *)DAT_004c2aa0;
    resA->flags_1c |= 0x400;

    resA = (struct ResA *)DAT_004c2aa0;
    if (resA != NULL) {
        resB = resA->ptr_64;
        if (resB != NULL) {
            resB->flags_10 |= 0x2000;
        }
    }

    DAT_004cbe10 = obj_ptr;
    // STRING: LEGOLAND 0x004b4a64
    DAT_004c8d2c = (struct Sprite *)LoadSprite("fc2_m1.lls", 1);
}

// FUNCTION: LEGOLAND 0x0040e970
void FUN_0040e970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040e9e0
void FUN_0040e9e0(struct Obj *obj_ptr) {
    struct ResA *resA;
    struct ResB *resB;

    DAT_004c74d4 = (struct CursorSource *)obj_ptr->field_0c;
    resA = (struct ResA *)DAT_004c74d4;
    resA->flags_1c |= 0x400;
    DAT_004c2ba0 = obj_ptr;

    resA = (struct ResA *)DAT_004c74d4;
    if (resA != NULL) {
        resB = resA->ptr_64;
        if (resB != NULL) {
            resB->flags_10 |= 0x2000;
        }
    }

    // STRING: LEGOLAND 0x004b4a88
    DAT_004c8d70 = (struct Sprite *)LoadSprite("fc4_m.lls", 1);
}

// FUNCTION: LEGOLAND 0x0040ea30
void FUN_0040ea30(void) {
    if (DAT_004c2b6c != 0) {
        KillSprite((struct Sprite *)DAT_004c2b6c);
    }
    if (DAT_004c2b70 != 0) {
        KillSprite((struct Sprite *)DAT_004c2b70);
    }
}

// FUNCTION: LEGOLAND 0x0040ea60
void FUN_0040ea60(void) {
    if (DAT_004c8d2c != NULL) {
        KillSprite(DAT_004c8d2c);
    }
}

// FUNCTION: LEGOLAND 0x0040ea80
void FUN_0040ea80(void) {
    if (DAT_004cbe0c != NULL) {
        KillSprite(DAT_004cbe0c);
    }
    if (DAT_004cbe08 != NULL) {
        KillSprite(DAT_004cbe08);
    }
}

// FUNCTION: LEGOLAND 0x0040eab0
void FUN_0040eab0(void) {
    if (DAT_004c8d70 != NULL) {
        KillSprite(DAT_004c8d70);
    }
}

// FUNCTION: LEGOLAND 0x0040ead0
void FUN_0040ead0(unsigned int param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    unsigned int local[5];
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    memcpy(local, DAT_004c445c->var_3c, 20);
    DAT_004c2af4 = 0;
    FUN_0040d900(packed, local, (int)DAT_004c2b98, FUN_0040dc00, FUN_0040e440, FUN_0040e340);
}

// FUNCTION: LEGOLAND 0x0040eb40
void FUN_0040eb40(unsigned int param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    unsigned int local[5];
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    memcpy(local, DAT_004c2aa0->var_3c, 20);
    DAT_004c2af4 = 1;
    FUN_0040d900(packed, local, (int)DAT_004cbe10, FUN_0040dc00, FUN_0040e440, FUN_0040e340);
}

// FUNCTION: LEGOLAND 0x0040ebb0
void FUN_0040ebb0(unsigned int param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    unsigned int local[5];
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    memcpy(local, DAT_004c2b0c->var_3c, 20);
    DAT_004c2af4 = 2;
    FUN_0040d900(packed, local, (int)DAT_004c8d50, FUN_0040dc00, FUN_0040e440, FUN_0040e340);
}

// FUNCTION: LEGOLAND 0x0040ec20
void FUN_0040ec20(unsigned int param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    unsigned int local[5];
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    memcpy(local, DAT_004c74d4->var_3c, 20);
    DAT_004c2af4 = 3;
    FUN_0040d900(packed, local, (int)DAT_004c2ba0, FUN_0040dc00, FUN_0040e440, FUN_0040e340);
}

// FUNCTION: LEGOLAND 0x0040ec90
void FUN_0040ec90(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    DAT_004c2af4 = 0;
    FUN_0040db00(param_1, param_2, param_3, FUN_0040e440);
}

// FUNCTION: LEGOLAND 0x0040ecc0
unsigned int FUN_0040ecc0(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    DAT_004c2af4 = 1;
    return FUN_0040db00(param_1, param_2, param_3, FUN_0040e440);
}

// FUNCTION: LEGOLAND 0x0040ecf0
unsigned int FUN_0040ecf0(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    DAT_004c2af4 = 2;
    return FUN_0040db00(param_1, param_2, param_3, FUN_0040e440);
}

// FUNCTION: LEGOLAND 0x0040ed20
void FUN_0040ed20(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    DAT_004c2af4 = 3;
    FUN_0040db00(param_1, param_2, param_3, FUN_0040e440);
}

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
void FUN_0040f300(struct FlumePos *src, struct FlumeRect *dst) {
    dst->var_0 = 0;
    dst->var_4 = 0;
    dst->var_8 = 0;
    dst->var_c = 0;
    dst->var_0 = src->var_30;
    dst->var_8 = src->var_34;
}

// FUNCTION: LEGOLAND 0x0040f330
int FUN_0040f330(unsigned int *param_1) {
    int flags = FUN_00409410(param_1);
    if (flags == 0x11 || flags == 0x1 || flags == 0x10) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040f360
void FUN_0040f360(unsigned char a1, unsigned char a2, void *a3) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f3e0
void FUN_0040f3e0(struct Obj *obj_ptr) {
    struct ResA *resA;
    struct ResB *resB;

    DAT_004cbe18 = (struct CursorSource *)obj_ptr->field_0c;
    resA = (struct ResA *)DAT_004cbe18;
    resA->flags_1c |= 0x400;
    DAT_004cbe48 = obj_ptr;

    resA = (struct ResA *)DAT_004cbe18;
    if (resA != NULL) {
        resB = resA->ptr_64;
        if (resB != NULL) {
            resB->flags_10 |= 0x2000;
        }
    }

    // STRING: LEGOLAND 0x004b4a94
    DAT_004c1258 = (struct Sprite *)LoadSprite("tunel_m.lls", 1);
}

// FUNCTION: LEGOLAND 0x0040f430
void FUN_0040f430(void) {
    if (DAT_004c1258 != NULL) {
        KillSprite(DAT_004c1258);
    }
}

// FUNCTION: LEGOLAND 0x0040f450
void FUN_0040f450(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f4f0
void FUN_0040f4f0(void) {
    FUN_0040d3b0(DAT_004cbe18, DAT_004cbe18->var_3c);
}

// FUNCTION: LEGOLAND 0x0040f510
void FUN_0040f510(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    FUN_0040d6f0(DAT_004cbe18, param_2, param_3, DAT_004cbe18->var_3c, FUN_0040f360, FUN_0040f330);
}

// FUNCTION: LEGOLAND 0x0040f540
void FUN_0040f540(unsigned int param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    FUN_0040d900(packed, DAT_004cbe18->var_3c, (int)DAT_004cbe48, FUN_0040f050, FUN_0040f360, FUN_0040f300);
}

// FUNCTION: LEGOLAND 0x0040f580
void FUN_0040f580(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    FUN_0040db00(param_1, param_2, param_3, FUN_0040f360);
}

// FUNCTION: LEGOLAND 0x0040f5a0
void FUN_0040f5a0(unsigned int param_1, unsigned int param_2) {
    FUN_0040d2d0(param_2);
}

// FUNCTION: LEGOLAND 0x0040f5b0
void FUN_0040f5b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f7d0
void FUN_0040f7d0(struct FlumePos *src, struct FlumeRect *dst) {
    dst->var_0 = 0;
    dst->var_4 = 0;
    dst->var_8 = 0;
    dst->var_c = 0;
    dst->var_c = src->var_30;
    dst->var_4 = src->var_34;
}

// FUNCTION: LEGOLAND 0x0040f800
unsigned int FUN_0040f800(unsigned int *param_1) {
    int flags = FUN_00409410(param_1);
    if (flags == 0x44 || flags == 0x4 || flags == 0x40) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040f830
void *FUN_0040f830(void *context, void *result) { STUB(); }

// FUNCTION: LEGOLAND 0x0040f8b0
unsigned int FUN_0040f8b0(struct Obj *obj_ptr) {
    struct ResA *resA;
    struct ResB *resB;

    DAT_004c2bf0 = (struct CursorSource *)obj_ptr->field_0c;
    resA = (struct ResA *)DAT_004c2bf0;
    resA->flags_1c |= 0x400;
    DAT_004c4460 = obj_ptr;

    resA = (struct ResA *)DAT_004c2bf0;
    if (resA != NULL) {
        resB = resA->ptr_64;
        if (resB != NULL) {
            resB->flags_10 |= 0x2000;
        }
    }

    // STRING: LEGOLAND 0x004b4aa0
    DAT_004cbe14 = LoadSprite("csaw2_m.lls", 1);
    return DAT_004cbe14;
}

// FUNCTION: LEGOLAND 0x0040f900
void FUN_0040f900(void) {
    if (DAT_004cbe14 != 0) {
        KillSprite((struct Sprite *)DAT_004cbe14);
    }
}

// FUNCTION: LEGOLAND 0x0040f920
void FUN_0040f920(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040fa00
void FUN_0040fa00(void) {
    FUN_0040d3b0(DAT_004c2bf0, DAT_004c2bf0->var_3c);
}

// FUNCTION: LEGOLAND 0x0040fa20
void FUN_0040fa20(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct CursorSource *src = DAT_004c2bf0;
    FUN_0040d6f0(src, param_2, param_3, src->var_3c, FUN_0040f830, FUN_0040f800);
}

// FUNCTION: LEGOLAND 0x0040fa50
void FUN_0040fa50(unsigned int param_1, unsigned int param_2) {
    FUN_0040d2d0(param_2);
}

// FUNCTION: LEGOLAND 0x0040fa60
void FUN_0040fa60(unsigned int param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    DAT_004c2af4 = 0;
    FUN_0040d900(packed, DAT_004c2bf0->var_3c, (int)DAT_004c4460, FUN_0040f5b0, FUN_0040f830, FUN_0040f7d0);
}

// FUNCTION: LEGOLAND 0x0040fab0
unsigned int FUN_0040fab0(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    return FUN_0040db00(param_1, param_2, param_3, FUN_0040f830);
}

// FUNCTION: LEGOLAND 0x0040fad0
void FUN_0040fad0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0040fe50
void FUN_0040fe50(struct FlumePos *src, struct FlumeRect *dst) {
    dst->var_0 = 0;
    dst->var_4 = 0;
    dst->var_8 = 0;
    dst->var_c = 0;
    dst->var_c = src->var_30;
    dst->var_4 = src->var_34;
}

// FUNCTION: LEGOLAND 0x0040fe80
unsigned int FUN_0040fe80(unsigned int *param_1) {
    int flags = FUN_00409410(param_1);
    if (flags == 0x44 || flags == 0x4 || flags == 0x40) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0040feb0
void FUN_0040feb0(unsigned char a, unsigned char b, void *output) { STUB(); }

// FUNCTION: LEGOLAND 0x0040ff30
void FUN_0040ff30(struct Obj *obj_ptr) {
    struct ResA *resA;
    struct ResB *resB;

    DAT_004c2b60 = (struct CursorSource *)obj_ptr->field_0c;
    resA = (struct ResA *)DAT_004c2b60;
    resA->flags_1c |= 0x400;

    resA = (struct ResA *)DAT_004c2b60;
    DAT_004c2b50 = obj_ptr;
    if (resA != NULL) {
        resB = resA->ptr_64;
        if (resB != NULL) {
            resB->flags_10 |= 0x2000;
        }
    }

    // STRING: LEGOLAND 0x004b4ab8
    DAT_004c2a94 = LoadSprite("hup1_m1.lls", 1);
    // STRING: LEGOLAND 0x004b4aac
    DAT_004c2a98 = LoadSprite("hup1_m2.lls", 1);
    DAT_004b4838 = DAT_004c2a94;
    DAT_004b4850 = DAT_004c2a98;
}

// FUNCTION: LEGOLAND 0x0040ffa0
void FUN_0040ffa0(void) {
    if (DAT_004c2a94 != 0) {
        KillSprite((struct Sprite *)DAT_004c2a94);
    }
    if (DAT_004c2a98 != 0) {
        KillSprite((struct Sprite *)DAT_004c2a98);
    }
}

// FUNCTION: LEGOLAND 0x0040ffd0
void FUN_0040ffd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004100b0
void FUN_004100b0(void) {
    FUN_0040d3b0(DAT_004c2b60, DAT_004c2b60->var_3c);
}

// FUNCTION: LEGOLAND 0x004100d0
unsigned int FUN_004100d0(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct CursorSource *src = DAT_004c2b60;
    return FUN_0040d6f0(src, param_2, param_3, src->var_3c, FUN_0040feb0, FUN_0040fe80);
}

// FUNCTION: LEGOLAND 0x00410100
void FUN_00410100(unsigned int param_1, unsigned int param_2) {
    FUN_0040d2d0(param_2);
}

// FUNCTION: LEGOLAND 0x00410110
void FUN_00410110(unsigned int param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    DAT_004c2af4 = 0;
    FUN_0040d900(packed, DAT_004c2b60->var_3c, (int)DAT_004c2b50, FUN_0040fad0, FUN_0040feb0, FUN_0040fe50);
}

// FUNCTION: LEGOLAND 0x00410160
unsigned int FUN_00410160(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    return FUN_0040db00(param_1, param_2, param_3, FUN_0040feb0);
}

// FUNCTION: LEGOLAND 0x00410180
void FUN_00410180(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004102e0
void FUN_004102e0(struct FlumePos *src, struct FlumeRect *dst) {
    dst->var_0 = 0;
    dst->var_4 = 0;
    dst->var_8 = 0;
    dst->var_c = 0;
    dst->var_0 = src->var_30;
    dst->var_8 = src->var_34;
}

// FUNCTION: LEGOLAND 0x00410310
int FUN_00410310(struct FlumeInput *param) {
    int result = FUN_00409410((unsigned int *)param);
    if (result != 0x11 && result != 0x1 && result != 0x10) {
        return 0;
    }
    if (result & 0x1) {
        if (param->var_0->var_8 != 0) {
            return 0;
        }
    }
    if (result & 0x10) {
        if (param->var_8->var_c != 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00410360
void FUN_00410360(unsigned char a, unsigned char b, unsigned int *result, unsigned char c) { STUB(); }

// FUNCTION: LEGOLAND 0x004103e0
unsigned int FUN_004103e0(struct Obj *obj_ptr) {
    struct ResA *resA;
    struct ResB *resB;

    DAT_004c8d6c = (struct CursorSource *)obj_ptr->field_0c;
    resA = (struct ResA *)DAT_004c8d6c;
    resA->flags_1c |= 0x400;

    resA = (struct ResA *)DAT_004c8d6c;
    DAT_004c8d4c = obj_ptr;
    if (resA != NULL) {
        resB = resA->ptr_64;
        if (resB != NULL) {
            resB->flags_10 |= 0x2000;
        }
    }

    // STRING: LEGOLAND 0x004b4ae0
    DAT_004c2af0 = LoadSprite("drop1_m.lls", 1);
    // STRING: LEGOLAND 0x004b4ad4
    DAT_004c2aec = LoadSprite("drop2_m.lls", 1);
    // STRING: LEGOLAND 0x004b4ac4
    DAT_004c2b64 = LoadSprite("lf_splash.lls", 1);

    return DAT_004c2b64;
}

// FUNCTION: LEGOLAND 0x00410450
void FUN_00410450(void) {
    if (DAT_004c2b64 != 0) {
        KillSprite((struct Sprite *)DAT_004c2b64);
        DAT_004c2b64 = 0;
    }
    if (DAT_004c2af0 != 0) {
        KillSprite((struct Sprite *)DAT_004c2af0);
        DAT_004c2af0 = 0;
    }
    if (DAT_004c2aec != 0) {
        KillSprite((struct Sprite *)DAT_004c2aec);
        DAT_004c2aec = 0;
    }
}

// FUNCTION: LEGOLAND 0x004104b0
void FUN_004104b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004106e0
unsigned int FUN_004106e0(void) {
    return FUN_0040d3b0(DAT_004c8d6c, DAT_004c8d6c->var_3c);
}

// FUNCTION: LEGOLAND 0x00410700
unsigned int FUN_00410700(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct CursorSource *src = DAT_004c8d6c;
    return FUN_0040d6f0(src, param_2, param_3, src->var_3c, FUN_00410360, FUN_00410310);
}

// FUNCTION: LEGOLAND 0x00410730
void FUN_00410730(unsigned int param_1, unsigned int param_2) {
    FUN_0040d2d0(param_2);
}

// FUNCTION: LEGOLAND 0x00410740
void FUN_00410740(unsigned int param_1, struct FlumeBytes *param_2) {
    unsigned int packed;
    *((unsigned char *)&packed) = param_2->b0;
    *((unsigned char *)&packed + 1) = param_2->b4;
    DAT_004c2af4 = 0;
    FUN_0040d900(packed, DAT_004c8d6c->var_3c, (int)DAT_004c8d4c, FUN_00410180, FUN_00410360, FUN_004102e0);
}

// FUNCTION: LEGOLAND 0x00410790
unsigned int FUN_00410790(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    return FUN_0040db00(param_1, param_2, param_3, FUN_00410360);
}

// FUNCTION: LEGOLAND 0x004107b0
void FUN_004107b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410800
void FUN_00410800(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410910
int FUN_00410910(struct FlumeSlots *param_1) {
    int count = 0;
    void *target = param_1->target_38;
    char *current = (char *)param_1->slots_40;

    while (count < 4) {
        if (target == (void *)current) {
            return count;
        }
        count = count + 1;
        current = current + 0x24;
    }
    return -1;
}

// FUNCTION: LEGOLAND 0x00410930
void FUN_00410930(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410a50
void FUN_00410a50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410b60
void *FUN_00410b60(void *base, unsigned int packed) { STUB(); }

// FUNCTION: LEGOLAND 0x00410bb0
void FUN_00410bb0(void *arg0, struct FlumeNode *arg1) {
    struct FlumeNode *current;

    if (arg1 == NULL) {
        return;
    }

    current = arg1;
    do {
        current->field_8 = (unsigned int)FUN_00410b60(arg0, current->field_8);
        current->field_c = (unsigned int)FUN_00410b60(arg0, current->field_c);
        current->field_30 = (unsigned int)FUN_00410b60(arg0, current->field_30);
        current->field_34 = (unsigned int)FUN_00410b60(arg0, current->field_34);
        FUN_00410bb0(arg0, current->field_2c);
        current = current->next;
    } while (current != NULL);
}

// FUNCTION: LEGOLAND 0x00410c10
void FUN_00410c10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00410d60
void FUN_00410d60(struct FlumeObj *flume, struct FlumeVtbl *vtbl) {
    if (_stricmp("LOG FLUME ENTRANCE", flume->name) == 0) {
        vtbl->var_a4 = FUN_0040a2e0;
        vtbl->var_8c = FUN_0040a540;
        vtbl->var_90 = FUN_0040a930;
        vtbl->var_94 = FUN_0040aac0;
        vtbl->var_98 = FUN_0040a600;
        vtbl->var_9c = FUN_0040abf0;
        vtbl->var_a8 = FUN_0040bf70;
        vtbl->var_b0 = FUN_0040b420;
        vtbl->var_ac = FUN_0040a410;
        vtbl->var_bc = FUN_00410930;
        vtbl->var_b8 = FUN_00410c10;
        vtbl->var_c0 = FUN_004119c0;
        return;
    }
    // STRING: LEGOLAND 0x004b4ba4
    if (_stricmp("LOG FLUME TRACK", flume->name) == 0) {
        DAT_0082c688 = flume->field_c;
        vtbl->var_a4 = FUN_0040c350;
        vtbl->var_8c = FUN_0040dbb0;
        vtbl->var_a0 = FUN_0040c970;
        vtbl->var_b0 = FUN_0040cc50;
        vtbl->var_90 = FUN_0040c4a0;
        vtbl->var_94 = FUN_0040c6c0;
        vtbl->var_98 = FUN_0040c780;
        vtbl->var_9c = FUN_0040c8d0;
        vtbl->var_ac = FUN_0040c430;
        return;
    }
    // STRING: LEGOLAND 0x004b4b88
    if (_stricmp("LOG FLUME SPECIAL CORNER 1", flume->name) == 0) {
        vtbl->var_a4 = FUN_0040e8b0;
        vtbl->var_8c = FUN_0040e630;
        vtbl->var_a0 = FUN_0040ed50;
        vtbl->var_b0 = FUN_0040edb0;
        vtbl->var_90 = FUN_0040e6f0;
        vtbl->var_94 = FUN_0040e830;
        vtbl->var_98 = FUN_0040ead0;
        vtbl->var_9c = FUN_0040ec90;
        vtbl->var_ac = FUN_0040ea30;
        return;
    }
    // STRING: LEGOLAND 0x004b4b6c
    if (_stricmp("LOG FLUME SPECIAL CORNER 2", flume->name) == 0) {
        vtbl->var_a4 = FUN_0040e920;
        vtbl->var_8c = FUN_0040e660;
        vtbl->var_a0 = FUN_0040ed50;
        vtbl->var_b0 = FUN_0040ee60;
        vtbl->var_90 = FUN_0040e740;
        vtbl->var_94 = FUN_0040e850;
        vtbl->var_98 = FUN_0040eb40;
        vtbl->var_9c = FUN_0040ecc0;
        vtbl->var_ac = FUN_0040ea60;
        return;
    }
    // STRING: LEGOLAND 0x004b4b50
    if (_stricmp("LOG FLUME SPECIAL CORNER 3", flume->name) == 0) {
        vtbl->var_a4 = FUN_0040e970;
        vtbl->var_8c = FUN_0040e690;
        vtbl->var_a0 = FUN_0040ed50;
        vtbl->var_b0 = FUN_0040ef00;
        vtbl->var_90 = FUN_0040e790;
        vtbl->var_94 = FUN_0040e870;
        vtbl->var_98 = FUN_0040ebb0;
        vtbl->var_9c = FUN_0040ecf0;
        vtbl->var_ac = FUN_0040ea80;
        return;
    }
    // STRING: LEGOLAND 0x004b4b34
    if (_stricmp("LOG FLUME SPECIAL CORNER 4", flume->name) == 0) {
        vtbl->var_a4 = FUN_0040e9e0;
        vtbl->var_8c = FUN_0040e6c0;
        vtbl->var_a0 = FUN_0040ed50;
        vtbl->var_b0 = FUN_0040efb0;
        vtbl->var_90 = FUN_0040e7e0;
        vtbl->var_94 = FUN_0040e890;
        vtbl->var_98 = FUN_0040ec20;
        vtbl->var_9c = FUN_0040ed20;
        vtbl->var_ac = FUN_0040eab0;
        return;
    }
    // STRING: LEGOLAND 0x004b4b24
    if (_stricmp("LOG FLUME CSAW", flume->name) == 0) {
        vtbl->var_a4 = FUN_0040f8b0;
        vtbl->var_8c = FUN_0040fa00;
        vtbl->var_a0 = FUN_0040ed50;
        vtbl->var_b0 = FUN_0040f920;
        vtbl->var_90 = FUN_0040fa20;
        vtbl->var_94 = FUN_0040fa50;
        vtbl->var_98 = FUN_0040fa60;
        vtbl->var_9c = FUN_0040fab0;
        vtbl->var_ac = FUN_0040f900;
        return;
    }
    // STRING: LEGOLAND 0x004b4b10
    if (_stricmp("LOG FLUME TUNNEL", flume->name) == 0) {
        vtbl->var_a4 = FUN_0040f3e0;
        vtbl->var_8c = FUN_0040f4f0;
        vtbl->var_a0 = FUN_0040ed50;
        vtbl->var_b0 = FUN_0040f450;
        vtbl->var_90 = FUN_0040f510;
        vtbl->var_94 = FUN_0040f5a0;
        vtbl->var_98 = FUN_0040f540;
        vtbl->var_9c = FUN_0040f580;
        vtbl->var_ac = FUN_0040f430;
        return;
    }
    // STRING: LEGOLAND 0x004b4b00
    if (_stricmp("LOG FLUME DROP", flume->name) == 0) {
        vtbl->var_a4 = FUN_004103e0;
        vtbl->var_8c = FUN_004106e0;
        vtbl->var_a0 = FUN_0040ed50;
        vtbl->var_b0 = FUN_004104b0;
        vtbl->var_90 = FUN_00410700;
        vtbl->var_94 = FUN_00410730;
        vtbl->var_98 = FUN_00410740;
        vtbl->var_9c = FUN_00410790;
        vtbl->var_ac = FUN_00410450;
        return;
    }
    // STRING: LEGOLAND 0x004b4aec
    if (_stricmp("LOG FLUME HOLD UP", flume->name) == 0) {
        vtbl->var_a4 = FUN_0040ff30;
        vtbl->var_8c = FUN_004100b0;
        vtbl->var_a0 = FUN_0040ed50;
        vtbl->var_b0 = FUN_0040ffd0;
        vtbl->var_90 = FUN_004100d0;
        vtbl->var_94 = FUN_00410100;
        vtbl->var_98 = FUN_00410110;
        vtbl->var_9c = FUN_00410160;
        vtbl->var_ac = FUN_0040ffa0;
    }
}

// FUNCTION: LEGOLAND 0x00411220
void FUN_00411220(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00411250
struct FlumeDims FUN_00411250(void) {
    int dim1;
    int dim2;
    int val1;
    int val2;
    struct FlumeDims result;

    GetTileDimensions(&dim1, &dim2);

    dim1 = dim1 << 1;
    dim2 = dim2 << 1;

    val1 = dim1 >> 1;
    val2 = dim2 >> 1;

    val1 = val1 + dim1;
    val2 = val2 + dim2;

    result.field1 = val1;
    result.field2 = val2;

    return result;
}

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
void FUN_004119a0(struct ParticleEmitter *param_1, unsigned int param_2) {
    if (param_1 != NULL) {
        param_1->var_d0 += param_2;
    }
}

// FUNCTION: LEGOLAND 0x004119c0
void FUN_004119c0(void) { STUB(); }
