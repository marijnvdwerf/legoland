#include <windows.h>
#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "gamemap.h"
#include "debug_alloc.h"
#include "path_control.h"
#include "timer.h"
#include "obj_instance.h"
#include "draw.h"
#include "castle.h"
#include "interface.h"
#include "llidb.h"
#include "map_object.h"
#include <math.h>

struct LookupResult;
struct CastleObj;
struct Anim;

struct FlagWord {
    unsigned int field_0;
};

#include "image_sprite.h"
// FUNCTION: LEGOLAND 0x0041cc50
unsigned int FUN_0041cc50(unsigned int dir) {
    if (dir == 1) return 4;
    if (dir == 4) return 1;
    if (dir == 2) return 8;
    if (dir == 8) return 2;
    if (dir == 0xffffffff) return 1;
    return 0;
}

// FUNCTION: LEGOLAND 0x0041cc90
unsigned int FUN_0041cc90(unsigned int shift) {
    return 1U << shift;
}

// FUNCTION: LEGOLAND 0x0041cca0
unsigned int FUN_0041cca0(unsigned int dir) {
    if (dir == 1) return 0;
    if (dir == 4) return 2;
    if (dir == 2) return 1;
    if (dir == 8) return 3;
    return 0;
}

// FUNCTION: LEGOLAND 0x0041cce0
void FUN_0041cce0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041cd20
unsigned int FUN_0041cd20(unsigned int a, unsigned int b) {
    return FUN_0041ee40(a, b);
}

// FUNCTION: LEGOLAND 0x0041cd40
void FUN_0041cd40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041cd80
void FUN_0041cd80(void) { STUB(); }

struct Anim {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
};

// FUNCTION: LEGOLAND 0x0041ce10
void FUN_0041ce10(struct Anim *anim) {
    anim->field_8 = 0;
    anim->field_0 = 0xffffffff;
    anim->field_4 = 0;
}

struct AnimPair {
    unsigned int field_0;
    unsigned short field_4;
    unsigned short field_6;
    unsigned char pad_8[0xc - 0x8];
    unsigned int field_c;
    unsigned int field_10;
};

// FUNCTION: LEGOLAND 0x0041ce30
void FUN_0041ce30(struct AnimPair *pair) {
    pair->field_0 = 0;
    pair->field_4 = 0;
    pair->field_6 = 0;
    pair->field_c = 0;
    pair->field_10 = 0;
    FUN_0041ce10((struct Anim *)((unsigned char *)pair + 0x20));
    FUN_0041ce10((struct Anim *)((unsigned char *)pair + 0x14));
}

// FUNCTION: LEGOLAND 0x0041ce60
void FUN_0041ce60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ceb0
void FUN_0041ceb0(void) { STUB(); }

struct NodeA {
    unsigned char flags;
    unsigned char pad_1[0x1c - 0x1];
    struct NodeA *next;
};

// FUNCTION: LEGOLAND 0x0041cee0
unsigned int FUN_0041cee0(struct NodeA *node) {
    unsigned int count = 0;
    while (node != NULL) {
        if (node->flags & 0x6) break;
        node = node->next;
        count++;
    }
    return count;
}

struct NodeB {
    unsigned char flags;
    unsigned char pad_1[0x28 - 0x1];
    struct NodeB *next;
};

// FUNCTION: LEGOLAND 0x0041cf00
unsigned int FUN_0041cf00(struct NodeB *node) {
    unsigned int count = 0;
    while (node != NULL) {
        if (node->flags & 0x6) break;
        node = node->next;
        count++;
    }
    return count;
}

struct CountSource {
    unsigned char pad_0[0x1c];
    struct NodeA *list_a;
    unsigned char pad_20[0x28 - 0x20];
    struct NodeB *list_b;
};

// FUNCTION: LEGOLAND 0x0041cf20
void FUN_0041cf20(struct CountSource *src, unsigned int *out_b, unsigned int *out_a) {
    *out_a = 0xffffffff;
    *out_b = 0xffffffff;
    if (src->list_a != NULL) {
        *out_a = FUN_0041cee0(src->list_a);
    }
    if (src->list_b != NULL) {
        *out_b = FUN_0041cf00(src->list_b);
    }
}

struct CountSource2 {
    unsigned int mode;
    unsigned char pad_4[0xa8 - 0x4];
    struct NodeA *list_a;
    unsigned char pad_ac[0xc0 - 0xac];
    struct NodeB *list_b;
};

// FUNCTION: LEGOLAND 0x0041cf70
void FUN_0041cf70(struct CountSource2 *src, unsigned int *out_a, unsigned int *out_b) {
    *out_a = 0;
    *out_b = 0;
    if (src->mode == 2) return;
    *out_b = FUN_0041cf00(src->list_b);
    *out_a = FUN_0041cee0(src->list_a);
}

struct Handler1 {
    unsigned char pad_0[0x1c];
    void (*method_1c)(void *self);
};

struct HandlerHost1 {
    unsigned char pad_0[0xc];
    struct Handler1 *handler;
};

// FUNCTION: LEGOLAND 0x0041cfc0
void FUN_0041cfc0(struct HandlerHost1 *host) {
    host->handler->method_1c(host);
}

struct Handler2 {
    unsigned char pad_0[0x20];
    unsigned int (*method_20)(void *self, unsigned int arg);
};

struct HandlerHost2 {
    unsigned char pad_0[0xc];
    struct Handler2 *handler;
};

// FUNCTION: LEGOLAND 0x0041cfd0
unsigned int FUN_0041cfd0(struct HandlerHost2 *host, unsigned int arg) {
    return host->handler->method_20(host, arg);
}

// FUNCTION: LEGOLAND 0x0041cff0
unsigned int FUN_0041cff0(unsigned int a, unsigned int *b) { STUB(); }

struct SearchNode {
    unsigned char pad_0[4];
    unsigned int value;
    unsigned char pad_8[0x28 - 0x8];
    struct SearchNode *next;
};

struct SearchHost {
    unsigned char flags;
    unsigned char pad_1[4 - 1];
    struct SearchNode *list1;
    unsigned char pad_8[0xc0 - 0x8];
    struct SearchNode *list2;
};

// FUNCTION: LEGOLAND 0x0041d060
struct SearchNode *FUN_0041d060(struct SearchHost *host, unsigned int *key) {
    if (host->flags & 0x2) {
        struct SearchNode *current = (struct SearchNode *)((unsigned char *)host + 0x4);
        unsigned int search = *key;
        if (current->value == search) {
            return current;
        }
        while (1) {
            current = current->next;
            if (current == (struct SearchNode *)((unsigned char *)host + 0x4)) {
                break;
            }
            if (current->value == search) {
                return current;
            }
        }
    } else {
        struct SearchNode *current = host->list2;
        unsigned int search = *key;
        if (current->value == search) {
            return current;
        }
        while (1) {
            current = current->next;
            if (current == NULL) {
                break;
            }
            if (current->value == search) {
                return current;
            }
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0041d0b0
void FUN_0041d0b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041d100
void FUN_0041d100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041d170
void FUN_0041d170(void) { STUB(); }

struct Indexed {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

// FUNCTION: LEGOLAND 0x0041d190
unsigned int FUN_0041d190(struct Indexed *obj, unsigned int param) {
    return FUN_0041d1d0(obj, obj->field_c + 0x14, param);
}

// FUNCTION: LEGOLAND 0x0041d1b0
void FUN_0041d1b0(unsigned int *param) {
    *param = 0;
}

// FUNCTION: LEGOLAND 0x0041d1c0
void FUN_0041d1c0(void) {
}

// FUNCTION: LEGOLAND 0x0041d1d0
unsigned int FUN_0041d1d0(struct Indexed *obj, unsigned int param2, unsigned int param3) { STUB(); }

// FUNCTION: LEGOLAND 0x0041d210
void FUN_0041d210(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041d2e0
void FUN_0041d2e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041d350
void FUN_0041d350(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041d3b0
struct LookupResult *FUN_0041d3b0(const unsigned char *key, unsigned int param2) { STUB(); }

struct LinkB;

struct LinkA {
    unsigned char pad_0[0x28];
    struct LinkB *field_28;
};

struct LinkB {
    unsigned char pad_0[0x1c];
    struct LinkA *field_1c;
};

// FUNCTION: LEGOLAND 0x0041d430
void FUN_0041d430(struct LinkA *a, struct LinkB *b) {
    a->field_28 = b;
    b->field_1c = a;
}

// FUNCTION: LEGOLAND 0x0041d440
void FUN_0041d440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041d5b0
unsigned int FUN_0041d5b0(unsigned int param0, const unsigned char *key, unsigned int param2, const unsigned char *param3) { STUB(); }

// FUNCTION: LEGOLAND 0x0041d630
unsigned int FUN_0041d630(unsigned int param0, const unsigned char *key, unsigned int param2, const unsigned char *param3) { STUB(); }

// FUNCTION: LEGOLAND 0x0041d6c0
void FUN_0041d6c0(unsigned int value) {
    DAT_004d8268 = value;
}

// FUNCTION: LEGOLAND 0x0041d6d0
unsigned int FUN_0041d6d0(unsigned int delta) {
    DAT_004d8268 += delta;
    return DAT_004d8268;
}

// FUNCTION: LEGOLAND 0x0041d6f0
unsigned int FUN_0041d6f0(void) {
    return DAT_004d8268;
}

struct LookupResult {
    unsigned int field_0;
};

// FUNCTION: LEGOLAND 0x0041d700
unsigned int FUN_0041d700(unsigned int param0, const unsigned char *key, unsigned int param2) {
    struct LookupResult *result = FUN_0041d3b0(key, param2);
    if (result->field_0 != 0) {
        FUN_0041d6d0(1);
        if ((key[0] & 1) != 0) {
            return FUN_0041d630(param0, key, param2, DAT_004d8250);
        } else {
            return FUN_0041d5b0(param0, key, param2, DAT_004d8250);
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0041d760
void FUN_0041d760(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041d7c0
unsigned int FUN_0041d7c0(unsigned int value) {
    if (DAT_00829ae0 != 2) {
        if (value != DAT_00829b88) {
            if (value != DAT_00829ba0) {
                return 0;
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0041d7f0
void FUN_0041d7f0(void *arg) { STUB(); }

// FUNCTION: LEGOLAND 0x0041d950
void FUN_0041d950(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041da10
void FUN_0041da10(void) { STUB(); }

struct FloatHolder {
    unsigned char pad_0[0x28];
    float field_28;
};

// FUNCTION: LEGOLAND 0x0041dad0
void FUN_0041dad0(struct FloatHolder *holder, float value) {
    holder->field_28 = value;
}

// FUNCTION: LEGOLAND 0x0041dae0
void FUN_0041dae0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041db20
void FUN_0041db20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041db90
void FUN_0041db90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041dca0
float FUN_0041dca0(float param) { STUB(); }

// FUNCTION: LEGOLAND 0x0041dd00
void FUN_0041dd00(float param, float result) { STUB(); }

// FUNCTION: LEGOLAND 0x0041dd50
void FUN_0041dd50(float param) {
    float result = FUN_0041dca0(param);
    FUN_0041dd00(param, result);
}

// FUNCTION: LEGOLAND 0x0041dd70
float FUN_0041dd70(float param) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ddb0
float FUN_0041ddb0(float param_1, float param_2, float param_3) {
    float result = FUN_0041dd70(param_1);
    result = result * param_2;
    result = result * param_2;
    result = result * DAT_004ab3d0;
    return result;
}

// FUNCTION: LEGOLAND 0x0041ddd0
void FUN_0041ddd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041de10
void FUN_0041de10(void) { STUB(); }

struct Animator {
    unsigned char pad_0[0x20];
    unsigned int field_20;
    unsigned char pad_24[0x2c - 0x24];
    void (*field_2c)(void);
    unsigned char pad_30[0x34 - 0x30];
    void (*field_34)(void);
    unsigned char pad_38[0x64 - 0x38];
    void *field_64;
    void *field_68;
};

// FUNCTION: LEGOLAND 0x0041dec0
void FUN_0041dec0(struct Animator *self) {
    FUN_00420410(&self->field_2c, 2);
    self->field_2c = FUN_0041ddd0;
    self->field_64 = &self->field_20;
    self->field_68 = self;
    self->field_34 = FUN_0041de10;
    self->field_20 = 2;
}

// FUNCTION: LEGOLAND 0x0041df00
void FUN_0041df00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041e000
unsigned int FUN_0041e000(unsigned int param_1, unsigned int param_2) { STUB(); }

struct Vtable110;

struct Dispatcher {
    unsigned char pad_0[0x6c];
    struct Vtable110 *vtable;
};

struct Vtable110 {
    unsigned char pad_0[0x110];
    unsigned int (*method_110)(struct Dispatcher *self, unsigned int arg);
};

// FUNCTION: LEGOLAND 0x0041e0e0
unsigned int FUN_0041e0e0(struct Dispatcher *self, unsigned int arg) {
    return self->vtable->method_110(self, arg);
}

// FUNCTION: LEGOLAND 0x0041e100
void FUN_0041e100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041e130
void FUN_0041e130(void) { STUB(); }

struct Timed {
    int field_0;
    int field_4;
    int field_8;
};

// FUNCTION: LEGOLAND 0x0041e240
void FUN_0041e240(struct Timed *timed) {
    int now = GetGameTimer();
    if (now > timed->field_8) {
        timed->field_0 &= ~0x40;
    }
    timed->field_4 = now;
}

// FUNCTION: LEGOLAND 0x0041e260
void FUN_0041e260(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041e2b0
unsigned int FUN_0041e2b0(void *a1) { STUB(); }

// FUNCTION: LEGOLAND 0x0041e2f0
void FUN_0041e2f0(void) { STUB(); }

struct RingNode {
    unsigned char pad_0[0xe8];
    struct RingNode *next;
};

struct RingHost {
    unsigned char pad_0[0x70];
    struct RingNode head;
};

// FUNCTION: LEGOLAND 0x0041e330
void FUN_0041e330(struct RingHost *host, void (*visit)()) {
    struct RingNode *start = &host->head;
    struct RingNode *node = start;
    do {
        visit(node);
        node = node->next;
    } while (node != start);
}

// FUNCTION: LEGOLAND 0x0041e360
void FUN_0041e360(struct RingHost *host) {
    FUN_0041e330(host, FUN_0041e950);
}

// FUNCTION: LEGOLAND 0x0041e380
void FUN_0041e380(struct RingHost *host) {
    FUN_0041e330(host, FUN_0041e970);
}

// FUNCTION: LEGOLAND 0x0041e3a0
void FUN_0041e3a0(struct RingHost *host) {
    FUN_0041e330(host, FUN_0041e990);
}

// FUNCTION: LEGOLAND 0x0041e3c0
unsigned int FUN_0041e3c0(unsigned int param) {
    return FUN_0041eaf0(param, DAT_0082adec);
}

// FUNCTION: LEGOLAND 0x0041e3e0
void FUN_0041e3e0(struct RingHost *host, unsigned int value) {
    DAT_0082adec = value;
    FUN_0041e330(host, FUN_0041e3c0);
}

// FUNCTION: LEGOLAND 0x0041e400
void FUN_0041e400(struct RingHost *host) {
    FUN_0041e330(host, FUN_0041e630);
}

// FUNCTION: LEGOLAND 0x0041e420
void FUN_0041e420(struct CastleObj *self, unsigned int flag) { STUB(); }

// FUNCTION: LEGOLAND 0x0041e460
void FUN_0041e460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041e4a0
unsigned int FUN_0041e4a0(struct FlagWord *obj) { STUB(); }

// FUNCTION: LEGOLAND 0x0041e4b0
void FUN_0041e4b0(void) { STUB(); }

struct TimerFlags {
    unsigned int field_0;
    unsigned char pad_4[8 - 4];
    unsigned int field_8;
};

// FUNCTION: LEGOLAND 0x0041e4c0
unsigned int FUN_0041e4c0(struct TimerFlags *obj, unsigned int delay) {
    obj->field_0 |= 2;
    if (delay != 0) {
        obj->field_0 |= 0x40;
        obj->field_8 = GetGameTimer() + delay;
        return obj->field_8;
    }
    obj->field_0 &= ~0x40;
    return obj->field_0;
}

// FUNCTION: LEGOLAND 0x0041e4f0
void FUN_0041e4f0(struct FlagWord *obj) {
    obj->field_0 &= 0xfffffffd;
    FUN_0041e500(obj);
}

// FUNCTION: LEGOLAND 0x0041e500
void FUN_0041e500(void *obj) { STUB(); }

struct CastleObj {
    unsigned int field_0;
    unsigned char pad_4[0x6c - 0x4];
    unsigned int field_6c;
    unsigned int field_70;
};

// FUNCTION: LEGOLAND 0x0041e570
struct CastleObj *FUN_0041e570(unsigned int arg) {
    struct CastleObj *self = FUN_004775b0(0x15c, 0, 0, 0);
    if (self == NULL) {
        return NULL;
    }
    self->field_6c = arg;
    FUN_0041e6a0(&self->field_70, 0);
    FUN_0041e420(self, 2);
    FUN_0041e420(self, 1);
    self->field_0 = 0;
    FUN_0041dec0((struct Animator *)self);
    FUN_0041e500(self);
    FUN_0041e360((struct RingHost *)self);
    return self;
}

// FUNCTION: LEGOLAND 0x0041e5d0
void FUN_0041e5d0(struct CastleObj **param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x0041e620
void FUN_0041e620(void) {
    FUN_00421540(&DAT_004d8270, 10);
}

// FUNCTION: LEGOLAND 0x0041e630
void FUN_0041e630(struct FlagWord *obj) {
    obj->field_0 &= 0xfffffffe;
}

// FUNCTION: LEGOLAND 0x0041e640
void FUN_0041e640(unsigned int *flags, unsigned int set) {
    if (set == 1) {
        *flags |= 1;
    } else {
        *flags &= 0xfffffffe;
    }
}

// FUNCTION: LEGOLAND 0x0041e660
unsigned int FUN_0041e660(unsigned int a) { STUB(); }

struct DualId {
    unsigned char pad_0[0xc];
    unsigned int field_c;
    unsigned char pad_10[0x44 - 0x10];
    unsigned int field_44;
};

// FUNCTION: LEGOLAND 0x0041e670
unsigned int FUN_0041e670(struct DualId *obj, unsigned int id) {
    if (obj->field_c == id) {
        return 1;
    }
    return obj->field_44 == id;
}

// FUNCTION: LEGOLAND 0x0041e6a0
void FUN_0041e6a0(unsigned int *param, unsigned int value) { STUB(); }

struct Slot;

struct Slot {
    unsigned char pad_0[0x10];
    unsigned int (*method_10)(struct Slot *self);
    void (*method_14)(struct Slot *self, unsigned int arg);
    unsigned char pad_18[0x20 - 0x18];
};

struct SlotHost {
    unsigned char pad_0[0x78];
    struct Slot slots[2];
};

// FUNCTION: LEGOLAND 0x0041e720
unsigned int FUN_0041e720(struct SlotHost *host, unsigned int arg) {
    struct Slot *slot = &host->slots[0];
    int i = 0;
    while (i <= 1) {
        if (slot->method_10(slot) == 0) {
            slot->method_14(slot, arg);
            return 1;
        }
        i++;
        slot = (struct Slot *)((unsigned char *)slot + 0x20);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0041e760
struct Slot *FUN_0041e760(struct SlotHost *host) {
    struct Slot *slot = &host->slots[0];
    int i = 0;
    while (i <= 1) {
        if (slot->method_10(slot) == 0) {
            return slot;
        }
        i++;
        slot = (struct Slot *)((unsigned char *)slot + 0x20);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0041e790
void FUN_0041e790(struct SlotHost *host) {
    struct Slot *slot = &host->slots[0];
    int i = 0;
    while (i <= 1) {
        if (slot->method_10(slot) != 0) {
            FUN_004273e0(slot);
            break;
        }
        i++;
        slot = (struct Slot *)((unsigned char *)slot + 0x20);
    }
}

struct Slot2 {
    unsigned char pad_0[0x18];
    void (*method_18)(struct Slot2 *self);
    unsigned char pad_1c[0x20 - 0x1c];
};

struct Slot2Host {
    unsigned char pad_0[0x78];
    struct Slot2 slots[2];
};

// FUNCTION: LEGOLAND 0x0041e7c0
void FUN_0041e7c0(struct Slot2Host *host) {
    struct Slot2 *slot = &host->slots[0];
    int i = 2;
    do {
        slot->method_18(slot);
        slot++;
        i--;
    } while (i != 0);
}

// FUNCTION: LEGOLAND 0x0041e7e0
float FUN_0041e7e0(void) {
    return DAT_004ab430;
}

// FUNCTION: LEGOLAND 0x0041e7f0
void FUN_0041e7f0(unsigned char *obj, unsigned int *out) {
    obj += 0xb8;
    out[0] = *(unsigned int *)obj;
    out[1] = *(unsigned int *)(obj + 4);
    out[2] = *(unsigned int *)(obj + 8);
}

struct FloatAtC4 {
    unsigned char pad_0[0xc4];
    float field_c4;
};

// FUNCTION: LEGOLAND 0x0041e810
float FUN_0041e810(struct FloatAtC4 *obj) {
    return obj->field_c4;
}

// FUNCTION: LEGOLAND 0x0041e820
void FUN_0041e820(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041e8f0
void FUN_0041e8f0(void) { STUB(); }

struct ObjAt40 {
    unsigned char pad_0[0x40];
    unsigned char field_40;
};

// FUNCTION: LEGOLAND 0x0041e930
unsigned int FUN_0041e930(struct ObjAt40 *obj, unsigned int param) {
    return FUN_0042a640(&obj->field_40, 2, param);
}

struct ObjAtC8 {
    unsigned char pad_0[0xc8];
    unsigned int field_c8;
};

// FUNCTION: LEGOLAND 0x0041e950
void FUN_0041e950(struct ObjAtC8 *obj) {
    FUN_004266b0((struct ListLink *)&obj->field_c8);
}

// FUNCTION: LEGOLAND 0x0041e970
void FUN_0041e970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041e990
void FUN_0041e990(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041e9e0
void FUN_0041e9e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ea70
void FUN_0041ea70(unsigned int a) { STUB(); }

// FUNCTION: LEGOLAND 0x0041eaf0
unsigned int FUN_0041eaf0(unsigned int a, unsigned int b) {
    if (FUN_0041e660(a) == 0) {
        if (FUN_0041e670((struct DualId *)a, b) != 0) {
            FUN_0041ea70(a);
            FUN_0041e640((unsigned int *)a, 1);
        }
    }
}

// FUNCTION: LEGOLAND 0x0041eb30
void *FUN_0041eb30(unsigned int arg) {
    void *obj = FUN_004775b0(0xec, 0, 0, 0);
    if (obj == NULL) {
        return NULL;
    }
    FUN_0041e6a0((unsigned int *)obj, arg);
    return obj;
}

// FUNCTION: LEGOLAND 0x0041eb60
void FUN_0041eb60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041eb70
void FUN_0041eb70(void) {
    // STRING: LEGOLAND 0x004b55d8
    DAT_0082add0 = FUN_004206b0("coastertrain.headcar");
    // STRING: LEGOLAND 0x004b55c4
    DAT_0082add4 = FUN_004206b0("coastertrain.midcar");
    // STRING: LEGOLAND 0x004b55ac
    DAT_0082add8 = FUN_004206b0("coastertrain.tailcar");
    DAT_0082ade0 = FUN_00420710("coastertrain.headcar");
    DAT_0082ade4 = FUN_00420710("coastertrain.midcar");
    DAT_0082ade8 = FUN_00420710("coastertrain.tailcar");
}

struct DispatchRow {
    unsigned int field_0;
    unsigned char pad_4[0x18 - 0x4];
};

struct DispatchTarget {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

// FUNCTION: LEGOLAND 0x0041ebd0
unsigned int FUN_0041ebd0(unsigned int arg) {
    unsigned int index = 0;
    struct DispatchRow *row = (struct DispatchRow *)DAT_0082ad20;
    while ((int)row < (int)&DAT_0082adb0) {
        struct DispatchTarget *target = (struct DispatchTarget *)row->field_0;
        if (target != NULL && arg == target->field_c) {
            return index;
        }
        index++;
        row++;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0041ec00
unsigned int FUN_0041ec00(unsigned int param) {
    struct DispatchTarget *target = (struct DispatchTarget *)((struct DispatchRow *)DAT_0082ad20)[param].field_0;
    return target->field_c;
}

// FUNCTION: LEGOLAND 0x0041ec20
unsigned int FUN_0041ec20(unsigned int param) {
    unsigned int index = 0;
    struct DispatchRow *row = (struct DispatchRow *)DAT_0082ad20;
    while ((int)row < (int)&DAT_0082adb0) {
        if (param == row->field_0) {
            return index;
        }
        row++;
        index++;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0041ec40
unsigned int FUN_0041ec40(unsigned int param) {
    return ((struct DispatchRow *)DAT_0082ad20)[param].field_0;
}

// FUNCTION: LEGOLAND 0x0041ec50
void FUN_0041ec50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ec70
void FUN_0041ec70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041eca0
void FUN_0041eca0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ece0
void FUN_0041ece0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ed00
void FUN_0041ed00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ed50
void FUN_0041ed50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ed80
void FUN_0041ed80(unsigned int param) {
    DAT_004b55f4 = param;
}

// FUNCTION: LEGOLAND 0x0041ed90
void FUN_0041ed90(unsigned int param1, unsigned int param2) {
    if (DAT_004b55f4 != 0) {
        AddBasicObject(param1, param2);
    }
}

// FUNCTION: LEGOLAND 0x0041edb0
void FUN_0041edb0(unsigned int param1, unsigned int param2, unsigned int param3) {
    if (DAT_004b55f4 != 0) {
        StandardRemoveObject(param1, param2, param3);
    }
}

// FUNCTION: LEGOLAND 0x0041ede0
void FUN_0041ede0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ee40
unsigned int FUN_0041ee40(unsigned int a, unsigned int b) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ef00
int FUN_0041ef00(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x0041ef20
void FUN_0041ef20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ef60
void FUN_0041ef60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041f030
void FUN_0041f030(unsigned int param) {
    DAT_004b5608 = 0x1c;
    DAT_004b560c = param;
}

// FUNCTION: LEGOLAND 0x0041f050
void FUN_0041f050(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041f2b0
unsigned int FUN_0041f2b0(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int *e) { STUB(); }

struct Pair {
    unsigned int field_0;
    unsigned int field_4;
};

// FUNCTION: LEGOLAND 0x0041f350
unsigned int FUN_0041f350(unsigned int param1, unsigned int param2, unsigned int param3, struct Pair *param4) {
    return FUN_0041f2b0(param1, param2, param3, param4->field_0, &param4->field_4);
}

// FUNCTION: LEGOLAND 0x0041f380
void FUN_0041f380(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041f3e0
void FUN_0041f3e0(void) { STUB(); }

struct CallbackAt24 {
    unsigned char pad_0[0x24];
    unsigned int (*method_24)(unsigned int, unsigned int);
};

// FUNCTION: LEGOLAND 0x0041f4c0
unsigned int FUN_0041f4c0(unsigned int *param1, struct CallbackAt24 *param2, int param3) {
    int n = param3 + 1;
    int tri = ((n + 1) * n) >> 1;
    return param2->method_24(*param1, tri);
}

// FUNCTION: LEGOLAND 0x0041f4e0
void FUN_0041f4e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041f5a0
void FUN_0041f5a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041f650
void FUN_0041f650(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041f710
void FUN_0041f710(unsigned int param) {
    FUN_004775d0(param);
}

// FUNCTION: LEGOLAND 0x0041f720
void FUN_0041f720(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041f790
void FUN_0041f790(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041f7e0
double FUN_0041f7e0(float param) {
    return log(param);
}

// FUNCTION: LEGOLAND 0x0041f7f0
void FUN_0041f7f0(void) { STUB(); }

struct InnerAt50 {
    unsigned char pad_0[0x50];
    unsigned int field_50;
};

struct OuterAt28 {
    unsigned char pad_0[0x28];
    unsigned int field_28;
};

struct AnimWalker {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
};

// FUNCTION: LEGOLAND 0x0041f850
unsigned int FUN_0041f850(struct AnimWalker *cursor) {
    struct InnerAt50 *inner = (struct InnerAt50 *)cursor->field_4;
    unsigned int result = inner->field_50;
    if (result == 0) {
        struct OuterAt28 *outer = (struct OuterAt28 *)cursor->field_0;
        result = outer->field_28;
        cursor->field_0 = result;
        result = FUN_0041cff0(result, &cursor->field_8);
    }
    cursor->field_4 = result;
    return result;
}

struct Node {
    unsigned char pad_0[0x1c];
    struct Node *field_1c;
    unsigned char pad_20[0x50 - 0x20];
    struct Node *field_50;
    struct Node *field_54;
};

struct Walker {
    struct Node *field_0;
    struct Node *field_4;
    unsigned int field_8;
};

// FUNCTION: LEGOLAND 0x0041f880
void FUN_0041f880(struct Walker *walker) {
    struct Node *node = walker->field_4->field_54;
    if (node != NULL) {
        walker->field_4 = node;
        return;
    }
    walker->field_0 = walker->field_0->field_1c;
    walker->field_4 = (struct Node *)FUN_0041cff0((unsigned int)walker->field_0, &walker->field_8);
    if (walker->field_4->field_50 == NULL) {
        return;
    }
    do {
        walker->field_4 = walker->field_4->field_50;
    } while (walker->field_4->field_50 != NULL);
}

// FUNCTION: LEGOLAND 0x0041f8d0
void FUN_0041f8d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041fa10
void FUN_0041fa10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041fba0
void FUN_0041fba0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041fd30
void FUN_0041fd30(void) {
    int i;
    for (i = 0; i < 16; i++) {
        ((unsigned int *)DAT_004d88f4)[i] = 0;
    }
    DAT_004d89c4 = &DAT_004d88f4[0x40];
    for (i = 0x40; i <= 0x7f; i++) {
        DAT_004d88f4[i] = (unsigned char)(i - 0x40);
    }
    for (i = 0; i < 16; i++) {
        ((unsigned int *)DAT_004d8974)[i] = 0x3f3f3f3f;
    }
}

// FUNCTION: LEGOLAND 0x0041fd80
void FUN_0041fd80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041ff80
void FUN_0041ff80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00420200
void FUN_00420200(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00420310
void FUN_00420310(void) { STUB(); }

struct Callback14 {
    unsigned char pad_0[0x14];
    void (*method_14)(unsigned int, unsigned int);
    unsigned char pad_18[0x38 - 0x18];
    unsigned int field_38;
};

// FUNCTION: LEGOLAND 0x004203d0
void FUN_004203d0(struct Callback14 *self, unsigned int param) {
    self->method_14(param, self->field_38);
}

struct Callback14b {
    unsigned char pad_0[0x14];
    unsigned int (*method_14)(unsigned int, unsigned int);
    unsigned char pad_18[0x38 - 0x18];
    unsigned int field_38;
};

// FUNCTION: LEGOLAND 0x004203f0
unsigned int FUN_004203f0(struct Callback14b *self, unsigned int param) {
    return self->method_14(self->field_38, param);
}

struct VTableHost {
    void (*field_0)(struct Callback14 *, unsigned int);
    unsigned int (*field_4)(struct Callback14b *, unsigned int);
    unsigned char pad_8[4];
    unsigned int field_c;
};

// FUNCTION: LEGOLAND 0x00420410
void FUN_00420410(void *param, unsigned int count) {
    struct VTableHost *host = (struct VTableHost *)param;
    FUN_00421540(&host->field_c, 2);
    host->field_0 = FUN_004203d0;
    host->field_4 = FUN_004203f0;
}

// FUNCTION: LEGOLAND 0x00420440
void FUN_00420440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00420530
unsigned int FUN_00420530(const char *param) {
    if (param == NULL) {
        return 0;
    }
    return SetCurrentDirectoryA(param);
}

// FUNCTION: LEGOLAND 0x00420550
int FUN_00420550(const char *param1, unsigned int param2) { STUB(); }

// FUNCTION: LEGOLAND 0x00420640
void FUN_00420640(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004206b0
unsigned int FUN_004206b0(const char *s) {
    unsigned int index = FUN_00422590(s);
    if (index != 0xffffffff) {
        return DAT_004d8a40[index];
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004206d0
void FUN_004206d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00420710
unsigned int FUN_00420710(const char *s) {
    unsigned int index = FUN_00422590(s);
    if (index != 0xffffffff) {
        return DAT_004d8abc[index];
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00420730
unsigned int FUN_00420730(const char *s) {
    unsigned int index = FUN_00422590(s);
    if (index != 0xffffffff) {
        return DAT_004d8b34[index];
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00420750
void FUN_00420750(const char *name) {
    char buffer[256];
    // STRING: LEGOLAND 0x004b56d8
    wsprintfA(buffer, "%s.ltx", name);
    FUN_00420550(buffer, 0);
}

// FUNCTION: LEGOLAND 0x00420780
unsigned int FUN_00420780(unsigned int param) {
    return DAT_004d89c8[param];
}

// FUNCTION: LEGOLAND 0x00420790
unsigned int FUN_00420790(unsigned int param) {
    return FUN_00422600(param);
}

// FUNCTION: LEGOLAND 0x004207a0
int FUN_004207a0(void) {
    // STRING: LEGOLAND 0x004b56e0
    return FUN_00420550("Rollercoaster.lpt", 0);
}

// FUNCTION: LEGOLAND 0x004207c0
unsigned int FUN_004207c0(void) {
    return DAT_004d8bac;
}

// FUNCTION: LEGOLAND 0x004207d0
void FUN_004207d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00420810
void FUN_00420810(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00420a20
void FUN_00420a20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00420c40
void FUN_00420c40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00420e90
unsigned int FUN_00420e90(unsigned int a, unsigned int b, void *c, void *d, unsigned int e) { STUB(); }

// FUNCTION: LEGOLAND 0x00420fb0
unsigned int FUN_00420fb0(unsigned char *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4) {
    return FUN_00426750(param_1 + 0x30, param_2, param_3, param_4);
}

// FUNCTION: LEGOLAND 0x00420fd0
void FUN_00420fd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004212a0
void FUN_004212a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004212e0
void FUN_004212e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421320
void FUN_00421320(void *src, void *dst) {
    memcpy(dst, src, 84);
}

struct FloatArray {
    int count;
    float data[1];
};

// FUNCTION: LEGOLAND 0x00421340
void FUN_00421340(struct FloatArray *arr, float f) {
    int i;
    for (i = 0; i < arr->count; i++) {
        arr->data[i] = f * arr->data[i];
    }
}

// FUNCTION: LEGOLAND 0x00421360
void FUN_00421360(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004213a0
void FUN_004213a0(void) { STUB(); }

struct IntBuffer {
    int size;
    int data[1];
};

// FUNCTION: LEGOLAND 0x00421400
void FUN_00421400(struct IntBuffer *buf, int size) {
    int i;
    if (size > 0) {
        for (i = 0; i < size; i++) {
            buf->data[i] = 0;
        }
    }
    buf->size = size;
}

// FUNCTION: LEGOLAND 0x00421430
void FUN_00421430(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421470
void FUN_00421470(void) {
    void **table;
    unsigned char *cur;

    DAT_004dcbd0[0] = FUN_004212a0;
    DAT_004dcbd0[1] = FUN_004212e0;
    DAT_004dcbd0[2] = FUN_00421320;
    DAT_004dcbd0[3] = FUN_00421340;
    DAT_004dcbd0[4] = FUN_00421360;
    DAT_004dcbd0[5] = FUN_004213a0;
    DAT_004dcbd0[6] = FUN_00421400;
    DAT_004dcbd0[7] = FUN_00421430;
    DAT_004dcbd0[8] = FUN_004214f0;
    DAT_004dcbd0[9] = FUN_00421510;

    table = DAT_0082ac60;
    cur = DAT_004dcc00;
    while ((int)cur < (int)&DAT_004dd5d8) {
        *table = cur;
        cur += 0x54;
        table += 1;
    }
}

// FUNCTION: LEGOLAND 0x004214f0
void **FUN_004214f0(unsigned int param_1) {
    unsigned int index = DAT_004dd5d8;
    DAT_004dd5d8 = DAT_004dd5d8 + param_1;
    return &DAT_0082ac60[index];
}

// FUNCTION: LEGOLAND 0x00421510
void FUN_00421510(unsigned int param_1, unsigned int param_2) {
    DAT_004dd5d8 = DAT_004dd5d8 - param_2;
}

// FUNCTION: LEGOLAND 0x00421540
void FUN_00421540(void *dest, unsigned int value) {
    DAT_004dcbf8 = value;
    memcpy(dest, DAT_004dcbd0, 11 * sizeof(unsigned int));
}

struct Struct1560A {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

struct Struct1560B {
    unsigned char pad_0[4];
    unsigned int field_4;
    unsigned int field_8;
};

// FUNCTION: LEGOLAND 0x00421560
unsigned int FUN_00421560(struct Struct1560B *param_2, struct Struct1560A *param_1) {
    FUN_00425c40();
    return FUN_00420e90(param_2->field_4, param_2->field_8, param_1, &param_1->field_c, 0);
}

struct Struct1590 {
    unsigned char pad_0[0x18];
    unsigned int field_18;
};

// FUNCTION: LEGOLAND 0x00421590
void FUN_00421590(struct Struct1590 *arg1, unsigned int arg2) {
    FUN_004273d0(arg2, arg1);
    arg1->field_18 = arg2;
}

// FUNCTION: LEGOLAND 0x004215b0
void FUN_004215b0(struct Struct1590 *param_1) {
    unsigned int temp = param_1->field_18;
    FUN_004273e0((void *)temp);
    param_1->field_18 = 0;
}

struct TimerNode;

// FUNCTION: LEGOLAND 0x004215d0
struct TimerNode *FUN_004215d0(struct TimerNode *a1) { STUB(); }

// FUNCTION: LEGOLAND 0x00421660
void FUN_00421660(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421890
struct TimerNode *FUN_00421890(unsigned int a1) { STUB(); }

struct TimerNode {
    unsigned int field_0;
    unsigned char pad_4[0x10 - 0x4];
    struct TimerNode *field_10;
    struct TimerNode *field_14;
    unsigned char pad_18[0x1c - 0x18];
    unsigned int field_1c;
};

struct Timer {
    unsigned char pad_0[0xe4];
    struct TimerNode *field_e4;
    unsigned char pad_e8[0xf4 - 0xe8];
    struct TimerNode *field_f4;
};

// FUNCTION: LEGOLAND 0x00421930
struct TimerNode *FUN_00421930(unsigned int param, struct Timer *timer) {
    struct TimerNode *node;
    struct TimerNode *old_head;

    node = FUN_00421890(param);
    node = FUN_004215d0(node);

    old_head = timer->field_f4;
    old_head->field_14 = node;
    old_head = timer->field_f4;
    node->field_10 = old_head;
    node->field_14 = (struct TimerNode *)&timer->field_e4;
    timer->field_f4 = node;

    node->field_1c = GetGameTimer();
    node->field_0 = 1;

    return node;
}

struct LinkPrev {
    unsigned char pad_0[0x14];
    void *field_14;
};

struct LinkNext {
    unsigned char pad_0[0x10];
    void *field_10;
};

struct LinkInput {
    unsigned char field_0;
    unsigned char pad_1[0x7];
    void *field_8;
    unsigned char pad_c[0x4];
    struct LinkPrev *field_10;
    struct LinkNext *field_14;
};

// FUNCTION: LEGOLAND 0x00421980
void FUN_00421980(struct LinkInput *p) {
    p->field_10->field_14 = p->field_14;
    p->field_14->field_10 = p->field_10;

    if ((p->field_0 & 1U) == 0U) {
        FUN_004775d0((unsigned int)p->field_8);
    }

    FUN_004775d0((unsigned int)p);
}

struct Vec5 {
    float field_0;
    float field_4;
    unsigned int field_8;
    float field_c;
    float field_10;
};

struct Vec3 {
    float field_0;
    float field_4;
    unsigned int field_8;
};

// FUNCTION: LEGOLAND 0x004219c0
void FUN_004219c0(struct Vec5 *arg1, float float_arg, struct Vec3 *arg3) {
    arg3->field_0 = arg1->field_0 + (float_arg * arg1->field_c);
    arg3->field_4 = arg1->field_4 + (float_arg * arg1->field_10);
    arg3->field_8 = arg1->field_8;
}

struct Words3 {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
};

// FUNCTION: LEGOLAND 0x004219f0
void FUN_004219f0(unsigned char *param_1, unsigned int param_2, struct Words3 *param_3) {
    param_1 = param_1 + 12;
    param_3->field_0 = *(unsigned int *)param_1;
    param_3->field_4 = *(unsigned int *)(param_1 + 4);
    param_3->field_8 = *(unsigned int *)(param_1 + 8);
}

struct Vec7 {
    float field_0;
    float field_4;
    unsigned int field_8;
    float field_c;
    float field_10;
    unsigned char pad_14[4];
    float field_18;
    float field_1c;
};

// FUNCTION: LEGOLAND 0x00421a10
void FUN_00421a10(struct Vec7 *s1, float arg2, struct Vec3 *s2) {
    s2->field_0 = arg2 * s1->field_c + s1->field_18 + s1->field_0;
    s2->field_4 = arg2 * s1->field_10 + s1->field_1c + s1->field_4;
    s2->field_8 = s1->field_8;
}

// FUNCTION: LEGOLAND 0x00421a40
void FUN_00421a40(struct Vec7 *a, float c, struct Vec3 *b) {
    b->field_0 = c * a->field_c + a->field_0 - a->field_18;
    b->field_4 = c * a->field_10 + a->field_4 - a->field_1c;
    b->field_8 = a->field_8;
}

struct Struct1a70A {
    unsigned char pad_0[0x44];
    unsigned int field_44;
    unsigned int field_48;
};

// FUNCTION: LEGOLAND 0x00421a70
int FUN_00421a70(struct Struct1a70A *param_1, struct Pair *param_2) {
    param_2->field_0 = param_1->field_44;
    param_2->field_4 = param_1->field_48;
    return 2;
}

struct Struct1a90 {
    unsigned int field_0;
    unsigned int field_4;
    float field_8;
};

// FUNCTION: LEGOLAND 0x00421a90
void FUN_00421a90(unsigned int param_1, unsigned int param_2, struct Struct1a90 *param_3) {
    param_3->field_0 = 0;
    param_3->field_4 = 0;
    param_3->field_8 = 1.0f;
}

// FUNCTION: LEGOLAND 0x00421ab0
void FUN_00421ab0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421b40
void FUN_00421b40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421b90
void FUN_00421b90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421be0
void FUN_00421be0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421c30
void FUN_00421c30(void) { STUB(); }

struct Floats6 {
    float field_0;
    float field_4;
    float field_8;
    float field_c;
    float field_10;
    float field_14;
};

// FUNCTION: LEGOLAND 0x00421c80
unsigned int FUN_00421c80(unsigned int param_1, struct Floats6 *ptr_0) {
    ptr_0->field_0 = 0.0f;
    ptr_0->field_4 = 0.31415900588035583f;
    ptr_0->field_8 = 0.6283180117607117f;
    ptr_0->field_c = 0.9424769878387451f;
    ptr_0->field_10 = 1.2566360235214233f;
    ptr_0->field_14 = 1.5707950592041016f;
    return 6;
}

// FUNCTION: LEGOLAND 0x00421cc0
void FUN_00421cc0(int param_1, int param_2, struct Struct1a90 *param_3) {
    param_3->field_0 = 0;
    param_3->field_4 = 0;
    param_3->field_8 = 1.0f;
}

// FUNCTION: LEGOLAND 0x00421ce0
void FUN_00421ce0(void) { STUB(); }

struct Struct1d60 {
    float field_0;
    float field_4;
    unsigned char pad_8[4];
    float field_c;
    float field_10;
    unsigned char pad_14[16];
    float field_24;
    float field_28;
    float field_2c;
    float field_30;
};

struct Floats3 {
    float field_0;
    float field_4;
    float field_8;
};

// FUNCTION: LEGOLAND 0x00421d60
void FUN_00421d60(struct Struct1d60 *a, float b, struct Floats3 *out) {
    out->field_0 = a->field_0 + b * a->field_c;
    out->field_4 = a->field_4 + b * a->field_10;
    out->field_8 = a->field_30 + b * (a->field_2c + b * (a->field_28 + b * a->field_24));
}

// FUNCTION: LEGOLAND 0x00421da0
void FUN_00421da0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421df0
void FUN_00421df0(float *a, float x, float *out) {
    out[0] = x * a[3] + a[6] + a[0];
    out[1] = x * a[4] + a[7] + a[1];
    out[2] = ((x * a[9] + a[10]) * x + a[11]) * x + a[12];
}

struct Struct1e40 {
    float field_0;
    float field_4;
    float pad_8;
    float field_c;
    float field_10;
    float pad_14;
    float field_18;
    float field_1c;
    float pad_20;
    float field_24;
    float field_28;
    float field_2c;
    float field_30;
};

// FUNCTION: LEGOLAND 0x00421e40
void FUN_00421e40(struct Struct1e40 *ptr1, float multiplier, struct Floats3 *ptr2) {
    float temp;

    ptr2->field_0 = multiplier * ptr1->field_c + ptr1->field_0 - ptr1->field_18;
    ptr2->field_4 = multiplier * ptr1->field_10 + ptr1->field_4 - ptr1->field_1c;

    temp = multiplier * ptr1->field_24 + ptr1->field_28;
    temp = temp * multiplier + ptr1->field_2c;
    temp = temp * multiplier + ptr1->field_30;
    ptr2->field_8 = temp;
}

// FUNCTION: LEGOLAND 0x00421e90
void FUN_00421e90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422000
void FUN_00422000(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004220e0
void FUN_004220e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422180
void FUN_00422180(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422210
void FUN_00422210(void) {
    DAT_004dd5e0[0] = FUN_00421df0;
    DAT_004dd5e0[1] = FUN_00421da0;
    DAT_004dd5e0[3] = FUN_00421da0;
    DAT_004dd5e0[5] = FUN_00421da0;
    DAT_004dd5e0[9] = FUN_004219f0;
    DAT_004dd5e0[11] = FUN_004219f0;
    DAT_004dd5e0[13] = FUN_004219f0;
    DAT_004dd5e0[2] = FUN_00421d60;
    DAT_004dd5e0[4] = FUN_00421e40;
    DAT_004dd5e0[6] = FUN_00422000;
    DAT_004dd5e0[7] = FUN_004220e0;
    DAT_004dd5e0[8] = FUN_00421a10;
    DAT_004dd5e0[10] = FUN_004219c0;
    DAT_004dd5e0[12] = FUN_00421a40;
    DAT_004dd5e0[14] = FUN_00421a70;
    DAT_004dd5e0[15] = FUN_00421a90;
    DAT_004dd5e0[16] = FUN_00421be0;
    DAT_004dd5e0[17] = FUN_00421b90;
    DAT_004dd5e0[18] = FUN_00421b40;
    DAT_004dd5e0[19] = FUN_00421b90;
    DAT_004dd5e0[20] = FUN_00421c30;
    DAT_004dd5e0[21] = FUN_00421b90;
    DAT_004dd5e0[22] = FUN_00421c80;
    DAT_004dd5e0[23] = FUN_00421cc0;
}

// FUNCTION: LEGOLAND 0x004222f0
int FUN_004222f0(unsigned short *param_1) {
    return param_1[0] == 0xa0d;
}

// FUNCTION: LEGOLAND 0x00422300
unsigned char *FUN_00422300(unsigned char *src, unsigned char *dst) {
    if (FUN_004222f0((unsigned short *)src) == 0) {
        do {
            *dst = *src;
            dst++;
            src++;
        } while (FUN_004222f0((unsigned short *)src) == 0);
        return dst;
    }
    return dst;
}

// FUNCTION: LEGOLAND 0x00422340
unsigned char *FUN_00422340(unsigned int a, unsigned int b) { STUB(); }

// FUNCTION: LEGOLAND 0x00422390
unsigned int FUN_00422390(unsigned int a1, unsigned int a2, unsigned int a3) {
    unsigned char *result1 = FUN_00422340(a1, a3);
    if (result1 == 0)
        return 0;
    {
        unsigned char *p = FUN_00422300(result1, (unsigned char *)a2);
        p[0] = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004223c0
void FUN_004223c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422400
unsigned int FUN_00422400(unsigned int *param_1, unsigned int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x00422470
void *FUN_00422470(const char *fileName, unsigned int *bytesReadPtr) {
    HANDLE hFile;
    unsigned int fileSize;
    void *buffer;
    unsigned int bytesRead;

    if (fileName == 0) {
        return 0;
    }

    hFile = CreateFileA(fileName, 0x80000000, 0x1, 0, 0x3, 0x8000000, 0);
    if (hFile == (HANDLE)-1) {
        return 0;
    }

    fileSize = GetFileSize(hFile, 0);
    buffer = FUN_004775b0(fileSize, 0, (unsigned int)DAT_004d8bb0, 0);
    if (buffer == 0) {
        CloseHandle(hFile);
        return 0;
    }

    ReadFile(hFile, buffer, fileSize, &bytesRead, 0);
    if (bytesRead != fileSize) {
        FUN_004775d0((unsigned int)buffer);
        CloseHandle(hFile);
        return 0;
    }

    CloseHandle(hFile);
    *bytesReadPtr = fileSize;
    return buffer;
}

// FUNCTION: LEGOLAND 0x00422590
unsigned int FUN_00422590(const char *s) {
    return FUN_00422400(&DAT_004dd860, (unsigned int)s);
}

// FUNCTION: LEGOLAND 0x004225b0
void FUN_004225b0(unsigned int param_1, unsigned int param_2) {
    FUN_00422390((unsigned int)&DAT_004dd860, param_2, param_1);
}

// FUNCTION: LEGOLAND 0x004225d0
unsigned int FUN_004225d0(void) {
    return DAT_004dd868;
}

// FUNCTION: LEGOLAND 0x00422600
unsigned int FUN_00422600(unsigned int param) {
    return FUN_00422400(&DAT_004dd758, param);
}

// FUNCTION: LEGOLAND 0x00422620
void FUN_00422620(int value, char *buffer) {
    // STRING: LEGOLAND 0x004b5afc
    wsprintfA(buffer, "%s%04d", DAT_004dd760, value);
}

// FUNCTION: LEGOLAND 0x00422640
unsigned int FUN_00422640(void) {
    return DAT_004dd86c;
}

// FUNCTION: LEGOLAND 0x004226c0
void FUN_004226c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004227a0
void FUN_004227a0(void) {
    FUN_004775d0(DAT_004dd758);
    FUN_004775d0(DAT_004dd860);
}

// FUNCTION: LEGOLAND 0x004227c0
void FUN_004227c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422e10
void FUN_00422e10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422e40
void FUN_00422e40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422fe0
void FUN_00422fe0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423140
void FUN_00423140(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423200
void FUN_00423200(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004232b0
void FUN_004232b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423350
void FUN_00423350(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423480
void FUN_00423480(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004234e0
void FUN_004234e0(void *param1) { STUB(); }

// FUNCTION: LEGOLAND 0x004236f0
unsigned short FUN_004236f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423730
unsigned short FUN_00423730(unsigned short control_word) { STUB(); }

// FUNCTION: LEGOLAND 0x00423740
void FUN_00423740(void) {
    FUN_00422fe0();
    FUN_0041fd30();
}

struct VideoArg {
    int field_0;
    unsigned char pad_4[8];
    void *field_c;
};

// FUNCTION: LEGOLAND 0x00423760
int FUN_00423760(struct VideoArg *arg) {
    int result = GetVideoSurface(arg);
    if (result == 0) {
        return 0;
    }
    DAT_004b5b20 = arg->field_c;
    DAT_004b5b28 = arg->field_0 >> 1;
    return 1;
}

// FUNCTION: LEGOLAND 0x00423790
void FUN_00423790(void) {
}

// FUNCTION: LEGOLAND 0x004237a0
void FUN_004237a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004237f0
void FUN_004237f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004238a0
void FUN_004238a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423930
unsigned int FUN_00423930(void) {
    return DAT_00610a04;
}

struct Struct3940 {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    float field_18;
    unsigned char pad_1c[4];
    unsigned int field_20;
    float field_24;
};

// FUNCTION: LEGOLAND 0x00423940
void FUN_00423940(struct Struct3940 *arg1) {
    arg1->field_14 = 2;
    arg1->field_18 = (float)DAT_004b5b50;
    arg1->field_20 = 8;
    arg1->field_24 = (float)DAT_004b5b4c;
}

// FUNCTION: LEGOLAND 0x00423970
void FUN_00423970(unsigned char *param_1) {
    *(float *)(param_1 + 0x18) = (float)DAT_004b5b50;
    *(float *)(param_1 + 0x24) = (float)DAT_004b5b4c;
}

// FUNCTION: LEGOLAND 0x00423990
unsigned int *FUN_00423990(void) {
    return &DAT_006102f8;
}

// FUNCTION: LEGOLAND 0x004239a0
void FUN_004239a0(void) {
}

// FUNCTION: LEGOLAND 0x004239b0
void FUN_004239b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004239e0
void FUN_004239e0(unsigned short *a, unsigned short *b) {
    unsigned int global_ptr = DAT_00829bf8 + 0x3c;
    b[0] = *(unsigned short *)global_ptr + a[0] - 2;
    b[1] = *(unsigned short *)(global_ptr + 0xc) + a[1];
}

// FUNCTION: LEGOLAND 0x00423a10
void FUN_00423a10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423d40
void FUN_00423d40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423db0
void FUN_00423db0(void) {
    DAT_00829bec = FUN_00424850;
    DAT_00829bf0 = FUN_00424890;
    DAT_00829bf4 = FUN_00424990;
    FUN_00423d40();
}

// FUNCTION: LEGOLAND 0x00423de0
void FUN_00423de0(void) {
    unsigned int v0 = DAT_00829bf8;
    memcpy(&DAT_00829a80, (void *)(v0 + 0x3c), 20);
    DAT_00829a80.field_10 = &DAT_0060f938;
    DAT_0060f924[DAT_00610a08 * 9] = 0;
}

// FUNCTION: LEGOLAND 0x00423e20
void FUN_00423e20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423ec0
void FUN_00423ec0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423f40
void FUN_00423f40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424050
void FUN_00424050(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424140
void FUN_00424140(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424150
void FUN_00424150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004241e0
void FUN_004241e0(void) {
    KillSprite(DAT_00829c04);
    DAT_00829b8c = 0;
    DAT_00829ba4 = 0;
    FUN_0041d1b0(&DAT_00829b8c);
    FUN_0041d1b0(&DAT_00829ba4);
    DAT_00829b88 = 0;
    DAT_00829ba0 = 0;
    FUN_0041ce10((struct Anim *)&DAT_00829af8);
    FUN_0041ce10((struct Anim *)&DAT_00829b04);
    DAT_00829ae0 = 0;
}

// FUNCTION: LEGOLAND 0x00424240
unsigned int FUN_00424240(void) {
    if (DAT_00610a04 != 0) {
        return DAT_00610a04;
    }
    EditMode = 1;
    DAT_008119b8 = DAT_00829bf8;
    DefaultCursor(&EditCursor);
    FUN_00423de0();
    return ((unsigned int (*)(const char *))SetEditCursorFootPrint)((const char *)&DAT_00829a80); /* TODO: fold — SetEditCursorFootPrint leaves memcpy's eax, used here as uint return */
}

// FUNCTION: LEGOLAND 0x00424280
void FUN_00424280(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424320
void FUN_00424320(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424440
void FUN_00424440(unsigned int param_1, unsigned int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x004244b0
void FUN_004244b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004245b0
void FUN_004245b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424620
void FUN_00424620(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004246e0
unsigned int FUN_004246e0(unsigned int param_1, unsigned int param_2) {
    if (param_2 != 0 && DAT_00829ae0 != 2) {
        return 0;
    }
    return FUN_0041d6f0();
}

// FUNCTION: LEGOLAND 0x00424700
void FUN_00424700(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424800
void FUN_00424800(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424820
void FUN_00424820(void) {
    FUN_00424440(0, 0);
}

// FUNCTION: LEGOLAND 0x00424830
void FUN_00424830(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424850
void FUN_00424850(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424890
unsigned int FUN_00424890(unsigned int param_1, unsigned int param_2) {
    return FUN_0041e000(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x004248b0
void FUN_004248b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424960
int FUN_00424960(int *param_1) {
    if (param_1[4] != (int)&DAT_006103a8) {
        return 0;
    }
    if (*(float *)((unsigned char *)param_1 + 0x24) > FLOAT_004ab454) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00424990
void FUN_00424990(void) { STUB(); }

struct CastleSub {
    unsigned char pad_0[0xd8];
    struct CastleObj *field_d8;
    unsigned char pad_dc[0xe0 - 0xdc];
    unsigned int field_e0;
};

// FUNCTION: LEGOLAND 0x004249e0
void FUN_004249e0(struct CastleSub *param_1) {
    param_1->field_d8 = FUN_0041e570((unsigned int)param_1);
    param_1->field_e0 = 0;
}

// FUNCTION: LEGOLAND 0x00424a00
void FUN_00424a00(struct CastleSub *param_1) {
    FUN_0041e5d0(&param_1->field_d8);
}

struct CastleOuter {
    unsigned char pad_0[0x10];
    struct CastleSub *field_10;
};

// FUNCTION: LEGOLAND 0x00424a20
void FUN_00424a20(struct CastleOuter *param_1) {
    struct CastleObj *obj = param_1->field_10->field_d8;
    if (FUN_00426650() != 0) {
        FUN_0041e3e0((struct RingHost *)obj, (unsigned int)param_1);
    }
}

// FUNCTION: LEGOLAND 0x00424a50
void FUN_00424a50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424ab0
void FUN_00424ab0(struct CastleSub *this) {
    if (FUN_0041e4a0((struct FlagWord *)this->field_d8) != 0) {
        FUN_0041e4f0((struct FlagWord *)this->field_d8);
        FUN_00424d80((struct ListHost *)this);
    }
}

// FUNCTION: LEGOLAND 0x00424ae0
void FUN_00424ae0(struct CastleSub *param_1, unsigned int param_2) {
    FUN_0041e500(param_1->field_d8);
    FUN_0041e4c0((struct TimerFlags *)param_1->field_d8, param_2);
}

struct ListNode {
    unsigned int flags;
    unsigned char pad_4[0x14 - 4];
    struct ListNode *next;
    unsigned char pad_18[0x1c - 0x18];
    unsigned int time;
};

struct ListHost {
    unsigned char pad_0[0xe4];
    struct ListNode *end;
    unsigned char pad_e8[0xf4 - 0xe8];
    struct ListNode *field_f4;
    struct ListNode *field_f8;
};

// FUNCTION: LEGOLAND 0x00424b10
void FUN_00424b10(struct ListHost *param_1) {
    unsigned int addr = (unsigned int)&param_1->end;
    param_1->field_f8 = (struct ListNode *)addr;
    param_1->field_f4 = (struct ListNode *)addr;
}

// FUNCTION: LEGOLAND 0x00424b30
struct ListNode *FUN_00424b30(struct ListHost *list) {
    struct ListNode *node = list->field_f8;
    struct ListNode *list_end = (struct ListNode *)&list->end;
    while (node != list_end && node->flags != 1) {
        node = node->next;
    }
    return (node == list_end) ? 0 : node;
}

// FUNCTION: LEGOLAND 0x00424b60
struct ListNode *FUN_00424b60(struct ListHost *list) {
    struct ListNode *node = list->field_f8;
    struct ListNode *list_end = (struct ListNode *)&list->end;
    while (node != list_end && node->flags != 2) {
        node = node->next;
    }
    return (node == list_end) ? 0 : node;
}

// FUNCTION: LEGOLAND 0x00424b90
void *FUN_00424b90(struct ListHost *list) { STUB(); }

// FUNCTION: LEGOLAND 0x00424bc0
unsigned int FUN_00424bc0(struct ListHost *ctx) {
    unsigned int result = 0;
    unsigned int t = GetGameTimer();
    struct ListNode *cur = ctx->field_f8;
    struct ListNode *end = (struct ListNode *)&ctx->end;

    if (cur != end) {
        do {
            if (cur->flags == 1) {
                unsigned int v = t - cur->time;
                if ((int)v > (int)result) {
                    result = v;
                }
            }
            cur = cur->next;
        } while (cur != end);
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00424c10
unsigned int FUN_00424c10(struct ListHost *ctx) {
    unsigned int result = 0;
    struct ListNode *cur = ctx->field_f8;
    struct ListNode *end = (struct ListNode *)&ctx->end;
    while (cur != end) {
        if (cur->flags == 1) {
            ++result;
        }
        cur = cur->next;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00424c40
int FUN_00424c40(struct ListHost *arg) {
    int temp1 = FUN_00424bc0(arg);
    int temp2 = FUN_00424c10(arg);
    if (temp1 > 0x1388 || temp2 > 3) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00424c70
void FUN_00424c70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424d80
void FUN_00424d80(struct ListHost *list) {
    struct ListNode *current = FUN_00424b60(list);
    while (current != NULL) {
        FUN_004215b0((struct Struct1590 *)current);
        current->flags = 4;
        current = FUN_00424b60(list);
    }
}

struct DcLink {
    unsigned char pad_0[0x60];
    unsigned char field_60;
};

struct DcNode {
    unsigned char pad_0[0xc];
    struct DcLink *field_c;
};

// FUNCTION: LEGOLAND 0x00424dc0
void FUN_00424dc0(struct ListHost *arg) {
    struct DcNode *current = FUN_00424b90(arg);
    if (current != NULL) {
        while (current != NULL) {
            struct DcLink *next = current->field_c;
            next->field_60 = 33;
            FUN_00421980((struct LinkInput *)current);
            current = FUN_00424b90(arg);
        }
    }
}

// FUNCTION: LEGOLAND 0x00424df0
void FUN_00424df0(struct ListHost *esi) {
    struct ListNode *current = esi->field_f8;
    struct ListNode *target = (struct ListNode *)&esi->end;
    while (current != target) {
        FUN_00421980((struct LinkInput *)current);
        current = esi->field_f8;
    }
}

struct RideNode {
    struct RideNode *next;
    unsigned char pad_4[4];
    void *info;
};

struct RideStruct {
    unsigned char pad_0[0xcc];
    struct RideNode *first_node;
};

struct RideNodeInfo {
    unsigned char pad_0[0x62];
    unsigned short flags;
};

// FUNCTION: LEGOLAND 0x00424e20
void FUN_00424e20(void) {
    struct RideStruct *ride = (struct RideStruct *)FUN_0041ec00(0);
    struct RideNode *node = ride->first_node;
    if (node != 0) {
        do {
            struct RideNode *next_node = node->next;
            ((struct RideNodeInfo *)node->info)->flags &= 0xfff7;
            RemoveBlokeFromRide(ride, node);
            node = next_node;
        } while (node != 0);
    }
}

// FUNCTION: LEGOLAND 0x00424e60
void FUN_00424e60(void) {
}

// FUNCTION: LEGOLAND 0x00424e70
void FUN_00424e70(void) {
    FUN_00424ab0((struct CastleSub *)&DAT_00829ae0);
}

// FUNCTION: LEGOLAND 0x00424e80
void FUN_00424e80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425050
void FUN_00425050(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425170
void FUN_00425170(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004251c0
void FUN_004251c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004254d0
void FUN_004254d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425a50
void FUN_00425a50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425bd0
void FUN_00425bd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425c40
void FUN_00425c40(void) {
    float f;
    unsigned int i1;
    unsigned int i2;
    unsigned int i3;

    f = DAT_004b5cbc;
    f = f + DAT_00611650;
    f = f * DAT_004ab3d0;

    i1 = DAT_004b5cc0;
    i2 = DAT_004b5cc4;
    i3 = DAT_004b5cc8;
    DAT_004b5ca0 = i1;

    memcpy(&i1, &DAT_00611650, sizeof(i1));
    DAT_004b5ca4 = i2;
    memcpy(&i2, &DAT_004b5cbc, sizeof(i2));
    DAT_004b5ca8 = i3;

    if (f == f) {
        DAT_00611648 = i1;
        DAT_004b5c9c = i2;
        DAT_00829a60 = f;
    }

    f = DAT_004b5cbc;
    f = f - DAT_00611650;
    f = f * DAT_004ab3d0;
    DAT_0082999c = f;
}

struct Int16Pair {
    short var_0;
    short var_2;
};

struct FVec3 {
    float x;
    float y;
    float z;
};

// FUNCTION: LEGOLAND 0x00425cb0
void FUN_00425cb0(const struct Int16Pair *in, float f, struct FVec3 *out) {
    short temp;
    float temp_f;
    temp = in->var_0;
    out->x = (float)temp * FLOAT_004ab45c;
    temp = in->var_2;
    out->y = (float)temp * FLOAT_004ab45c;
    temp_f = f;
    out->z = temp_f * FLOAT_004ab458;
}

// FUNCTION: LEGOLAND 0x00425cf0
void FUN_00425cf0(const struct FVec3 *a, const struct FVec3 *b, struct FVec3 *result) {
    result->x = b->z * a->y - a->z * b->y;
    result->y = a->z * b->x - a->x * b->z;
    result->z = a->x * b->y - a->y * b->x;
}

// FUNCTION: LEGOLAND 0x00425d30
float FUN_00425d30(struct FVec3 *param_1, struct FVec3 *param_2) {
    return param_1->z * param_2->z + param_1->y * param_2->y + param_1->x * param_2->x;
}

// FUNCTION: LEGOLAND 0x00425d50
void FUN_00425d50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425da0
unsigned int FUN_00425da0(const float *a, const float *b) {
    if (a[0] != b[0]) {
        return 0;
    }
    if (a[1] != b[1]) {
        return 0;
    }
    if (a[2] != b[2]) {
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00425de0
void FUN_00425de0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425e20
void FUN_00425e20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426000
void FUN_00426000(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004260e0
float FUN_004260e0(void) {
    return DAT_008299d4;
}

// FUNCTION: LEGOLAND 0x004260f0
void FUN_004260f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426120
void FUN_00426120(void) { STUB(); }

struct Mat4x4 {
    unsigned int m[4][4];
};

// FUNCTION: LEGOLAND 0x00426190
void FUN_00426190(struct Mat4x4 *param_2, struct Mat4x4 *param_1) {
    int i;
    int j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            param_1->m[i][j] = param_2->m[j][i];
        }
    }
}

// FUNCTION: LEGOLAND 0x004261c0
void FUN_004261c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426230
unsigned int FUN_00426230(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    return FUN_00426250(param_1, param_2, &DAT_008299fc, 0x10, param_3);
}

// FUNCTION: LEGOLAND 0x00426250
unsigned int FUN_00426250(unsigned int param_1, unsigned int param_2, unsigned int *param_3, unsigned int param_4, unsigned int param_5) { STUB(); }

// FUNCTION: LEGOLAND 0x004263a0
void FUN_004263a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426460
void FUN_00426460(unsigned int *dst, void *src) {
    unsigned int i;
    unsigned int j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            dst[j] = *(unsigned int *)((unsigned char *)src + i * 4 + j * 16);
        }
        dst += 3;
    }
}

// FUNCTION: LEGOLAND 0x00426490
void FUN_00426490(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004264e0
void FUN_004264e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426510
void FUN_00426510(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426560
void FUN_00426560(void) { STUB(); }

struct RectI {
    int var_0;
    int var_4;
    int var_8;
    int var_c;
};

// FUNCTION: LEGOLAND 0x004265d0
int FUN_004265d0(struct RectI *a, struct RectI *b) {
    if (a->var_0 > b->var_8) {
        return 0;
    }
    if (a->var_8 < b->var_0) {
        return 0;
    }
    if (a->var_4 > b->var_c) {
        return 0;
    }
    if (a->var_c < b->var_4) {
        return 0;
    }
    if (a->var_0 < b->var_0) {
        a->var_0 = b->var_0;
    }
    if (a->var_8 > b->var_8) {
        a->var_8 = b->var_8;
    }
    if (a->var_4 < b->var_4) {
        a->var_4 = b->var_4;
    }
    if (a->var_c > b->var_c) {
        a->var_c = b->var_c;
    }
    return 0xF;
}

// FUNCTION: LEGOLAND 0x00426650
unsigned int FUN_00426650(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004266b0
void FUN_004266b0(struct ListLink *param_1) {
    DAT_00829a3c.var_18->var_14 = param_1;
    param_1->var_18 = DAT_00829a3c.var_18;
    DAT_00829a3c.var_18 = param_1;
    param_1->var_14 = &DAT_00829a3c;
}

// FUNCTION: LEGOLAND 0x004266e0
void FUN_004266e0(struct ListLink *param_1) {
    struct ListLink *temp_ptr_1;
    struct ListLink *temp_ptr_2;

    temp_ptr_1 = param_1->var_18;
    temp_ptr_2 = param_1->var_14;
    temp_ptr_1->var_14 = temp_ptr_2;

    temp_ptr_1 = param_1->var_14;
    temp_ptr_2 = param_1->var_18;
    temp_ptr_1->var_18 = temp_ptr_2;
}

// FUNCTION: LEGOLAND 0x00426700
void FUN_00426700(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426740
void FUN_00426740(void) {
    DAT_00829a3c.var_18 = &DAT_00829a3c;
    DAT_00829a3c.var_14 = &DAT_00829a3c;
}

// FUNCTION: LEGOLAND 0x00426750
unsigned int FUN_00426750(void *ptr, unsigned int a, unsigned int b, unsigned int c) { STUB(); }

// FUNCTION: LEGOLAND 0x004267b0
void FUN_004267b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426850
void FUN_00426850(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426960
void FUN_00426960(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426980
void FUN_00426980(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004269e0
void FUN_004269e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426a90
void FUN_00426a90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426ab0
void FUN_00426ab0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426b10
void FUN_00426b10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426ba0
void FUN_00426ba0(unsigned int *param_1, unsigned int param_2) {
    if (*param_1 != 0) {
        *param_1 = *param_1 + param_2;
    }
}

// FUNCTION: LEGOLAND 0x00426bc0
void FUN_00426bc0(unsigned int *param_1, unsigned int param_2) {
    unsigned int var_0 = *param_1;
    if (var_0 != 0) {
        *param_1 = var_0 - param_2;
    }
}

struct TrackEntry {
    unsigned int var_0;
    short var_4;
    short var_6;
};

struct TrackList {
    unsigned int var_0;
    unsigned int var_4;
    unsigned int var_8;
    struct TrackEntry *var_c;
};

// FUNCTION: LEGOLAND 0x00426be0
void FUN_00426be0(struct TrackList *edi) {
    int i;
    for (i = 0; i < (int)edi->var_8; i += 1) {
        struct TrackEntry *ptr = edi->var_c + i;
        // STRING: LEGOLAND 0x004b5cd0
        DBPrintf("Track %2x, Type %2x at (%2x, %2x)\n", i, ptr->var_0, ptr->var_4, ptr->var_6);
    }
}

// FUNCTION: LEGOLAND 0x00426c20
void FUN_00426c20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426ce0
void FUN_00426ce0(void) { STUB(); }

struct Struct426d80Y {
    unsigned char pad_0[0xd8];
    unsigned int field_d8;
};

struct Struct426d80X {
    struct Struct426d80Y *field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
};

// FUNCTION: LEGOLAND 0x00426d80
void FUN_00426d80(struct Struct426d80Y *param1, struct Struct426d80X *param2) {
    param2->field_0 = param1;
    param2->field_8 = FUN_00427150(param1);
    if (FUN_0041e4a0((struct FlagWord *)param1->field_d8) != 0) {
        param2->field_c = 1;
    } else {
        param2->field_c = 0;
    }
    param2->field_10 = FUN_00427130((struct Struct427130Main *)param1);
    param2->field_4 = (param2->field_8 + param2->field_c * 4 + 4 + param2->field_10) * 8;
}

// FUNCTION: LEGOLAND 0x00426de0
void FUN_00426de0(void) { STUB(); }

struct Struct426e80Src {
    unsigned int pad_0;
    unsigned int field_4;
    unsigned int field_8;
};

struct Struct426e80Dst {
    unsigned int field_0;
    unsigned int field_4;
};

// FUNCTION: LEGOLAND 0x00426e80
void FUN_00426e80(struct Struct426e80Src *src, struct Struct426e80Dst *dst) {
    dst->field_0 = FUN_0041ebd0(src->field_8);
    dst->field_4 = src->field_4;
}

// FUNCTION: LEGOLAND 0x00426ea0
struct SearchNode *FUN_00426ea0(unsigned char *param_1, struct SearchHost *param_2) {
    return FUN_0041d060(param_2, (unsigned int *)(param_1 + 4));
}

// FUNCTION: LEGOLAND 0x00426ec0
void FUN_00426ec0(unsigned int *arg0, unsigned int *arg1) { STUB(); }

struct Struct426f10Out {
    struct SearchNode *field_0;
    unsigned int field_4;
    unsigned int field_8;
};

// FUNCTION: LEGOLAND 0x00426f10
void FUN_00426f10(unsigned char *a, struct Struct426f10Out *b, struct SearchHost *c) {
    b->field_0 = FUN_00426ea0(a, c);
    b->field_4 = FUN_0041cff0((unsigned int)b->field_0, &b->field_8);
}

struct Struct426f40Src {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
    unsigned char pad_10[0x24 - 0x10];
    unsigned int field_24;
    unsigned int field_28;
};

struct Struct426f40Dst {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
    unsigned int field_14;
};

// FUNCTION: LEGOLAND 0x00426f40
void FUN_00426f40(struct Struct426f40Src *src, struct Struct426f40Dst *dst) {
    FUN_00426ec0(&src->field_c, &dst->field_14);
    dst->field_0 = src->field_0;
    dst->field_4 = src->field_24;
    dst->field_8 = src->field_28;
    dst->field_c = GetGameTimer() - src->field_4;
    dst->field_10 = src->field_8 - GetGameTimer();
}

// FUNCTION: LEGOLAND 0x00426f90
void FUN_00426f90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426ff0
unsigned int FUN_00426ff0(unsigned int param) { STUB(); }

// FUNCTION: LEGOLAND 0x00427020
unsigned int FUN_00427020(unsigned int index) { STUB(); }

struct Struct427050Src {
    unsigned int field_0;
    unsigned char pad_4[0xc - 0x4];
    unsigned int field_c;
};

struct Struct427050Dst {
    unsigned int field_0;
    unsigned int field_4;
};

// FUNCTION: LEGOLAND 0x00427050
void FUN_00427050(struct Struct427050Src *param1, struct Struct427050Dst *param2) {
    param2->field_0 = param1->field_0;
    param2->field_4 = FUN_00426ff0(param1->field_c);
}

struct Struct427070Src {
    unsigned int field_0;
    unsigned int field_4;
};

struct Struct427070Obj {
    unsigned char pad_0[0xd8];
    unsigned int field_d8;
};

// FUNCTION: LEGOLAND 0x00427070
void FUN_00427070(struct Struct427070Src *param_1, struct Struct427070Obj *param_2) { STUB(); }

struct Struct4270c0Node {
    unsigned char pad_0[0x14];
    struct Struct4270c0Node *next;
};

struct Struct4270c0Host {
    unsigned char pad_0[0xf8];
    struct Struct4270c0Node *list;
};

// FUNCTION: LEGOLAND 0x004270c0
void FUN_004270c0(struct Struct4270c0Host *a1, struct Struct427050Dst *a2) {
    struct Struct4270c0Node *node = a1->list;
    struct Struct4270c0Node *end = (struct Struct4270c0Node *)((unsigned char *)a1 + 0xe4);
    if (node != end) {
        struct Struct427050Dst *out = a2;
        do {
            struct Struct427050Dst *cur = out;
            out = out + 1;
            FUN_00427050((struct Struct427050Src *)node, cur);
            node = node->next;
        } while (node != end);
    }
}

// FUNCTION: LEGOLAND 0x00427100
void FUN_00427100(void) { STUB(); }

struct Struct427130Node {
    unsigned char pad_0[0x14];
    struct Struct427130Node *next;
};

struct Struct427130Main {
    unsigned char pad_0[0xf8];
    struct Struct427130Node *list;
};

// FUNCTION: LEGOLAND 0x00427130
unsigned int FUN_00427130(struct Struct427130Main *main) {
    unsigned int counter = 0;
    struct Struct427130Node *node = main->list;
    struct Struct427130Node *end = (struct Struct427130Node *)((unsigned char *)main + 0xe4);
    while (node != end) {
        node = node->next;
        counter++;
    }
    return counter;
}

struct Struct427150Node {
    unsigned int field_0;
    unsigned char pad_4[0x1c - 0x4];
    struct Struct427150Node *field_1c;
    struct Struct427150Node *field_20;
    unsigned char pad_24[0x28 - 0x24];
    struct Struct427150Node *field_28;
    struct Struct427150Node *field_2c;
};

// FUNCTION: LEGOLAND 0x00427150
unsigned int FUN_00427150(struct Struct426d80Y *arg) {
    struct Struct427150Node *node = (struct Struct427150Node *)arg;
    unsigned int count = 1;

    if (node->field_0 == 2) {
        struct Struct427150Node *next = node->field_2c;
        struct Struct427150Node *end = (struct Struct427150Node *)((unsigned char *)node + 4);
        if (next != end) {
            do {
                next = next->field_28;
                count++;
            } while (next != end);
        }
        return count;
    }

    if (node->field_2c != NULL) {
        struct Struct427150Node *curr = node->field_2c;
        do {
            curr = curr->field_28;
            count++;
        } while (curr != NULL);
    }

    if (node->field_20 != NULL) {
        struct Struct427150Node *curr = node->field_20;
        do {
            curr = curr->field_1c;
            count++;
        } while (curr != NULL);
    }

    return count;
}

// FUNCTION: LEGOLAND 0x00427190
void FUN_00427190(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427220
void FUN_00427220(unsigned int *param_1) {
    SaveGameWrite(param_1, *param_1);
}

// FUNCTION: LEGOLAND 0x00427240
void FUN_00427240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004272a0
void FUN_004272a0(void) { STUB(); }

struct Struct4273c0 {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

// FUNCTION: LEGOLAND 0x004273c0
unsigned char FUN_004273c0(struct Struct4273c0 *param_1) {
    return (unsigned char)(param_1->field_c != 0);
}

// FUNCTION: LEGOLAND 0x004273d0
void FUN_004273d0(unsigned int param_1, void *param_2) {
    ((struct Struct4273c0 *)param_1)->field_c = (unsigned int)param_2;
}

// FUNCTION: LEGOLAND 0x004273e0
unsigned int FUN_004273e0(void *obj) {
    struct Struct4273c0 *self = (struct Struct4273c0 *)obj;
    unsigned int value = self->field_c;
    self->field_c = 0;
    return value;
}

struct Struct4273f0Obj {
    unsigned char pad_0[0x24];
    void (*method_24)(struct Struct4273f0Obj *self);
};

struct Struct4273f0Host {
    unsigned char pad_0[0xc];
    struct Struct4273f0Obj *field_c;
};

// FUNCTION: LEGOLAND 0x004273f0
void FUN_004273f0(struct Struct4273f0Host *param_1) {
    struct Struct4273f0Obj *obj = param_1->field_c;
    if (obj != NULL) {
        obj->method_24(obj);
    }
}

// FUNCTION: LEGOLAND 0x00427410
void FUN_00427410(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004274b0
void FUN_004274b0(void) { STUB(); }

struct MapHeader {
    unsigned short field_0;
};

struct Struct4274f0Coord {
    short field_0;
    short field_2;
};


// FUNCTION: LEGOLAND 0x004274f0
void FUN_004274f0(struct Struct4274f0Coord *coord) {
    if (DAT_004b55f4 != 0) {
        SetMapTile(coord->field_0, coord->field_2, ((struct MapHeader *)DAT_00829980)->field_0 + 1);
        SetMapTile(coord->field_0 + 1, coord->field_2, ((struct MapHeader *)DAT_00829980)->field_0 + 1);
        SetMapTile(coord->field_0, coord->field_2 + 1, ((struct MapHeader *)DAT_00829980)->field_0 + 1);
        SetMapTile(coord->field_0 + 1, coord->field_2 + 1, ((struct MapHeader *)DAT_00829980)->field_0 + 1);
    }
}

// FUNCTION: LEGOLAND 0x004275b0
void FUN_004275b0(void) {}

// FUNCTION: LEGOLAND 0x004275c0
void FUN_004275c0(void) {}

// FUNCTION: LEGOLAND 0x004275d0
void FUN_004275d0(void) { STUB(); }

struct Struct427940 {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

// FUNCTION: LEGOLAND 0x00427940
void FUN_00427940(struct Struct427940 *param_1) {
    unsigned int value = param_1->field_c;
    EditMode = 1;
    DAT_008119b8 = value;
    SetEditCursorFootPrint((void *)(value + 60));
    EditCursor.field_1830 = 0;
}

// FUNCTION: LEGOLAND 0x00427970
void FUN_00427970(void) { STUB(); }


// FUNCTION: LEGOLAND 0x004279f0
void FUN_004279f0(void) {
    FUN_0041d7f0(DAT_0081cdec);
}

// FUNCTION: LEGOLAND 0x00427a00
void FUN_00427a00(void) { STUB(); }

struct Struct427a40 {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
    unsigned int field_24;
};

// FUNCTION: LEGOLAND 0x00427a40
void FUN_00427a40(struct Struct427a40 *param_1) {
    if (param_1->field_20 == 0xffffffff) {
        param_1->field_20 = FUN_0041cc50(param_1->field_14);
        param_1->field_24 = param_1->field_18;
    } else if (param_1->field_14 == 0xffffffff) {
        param_1->field_14 = FUN_0041cc50(param_1->field_20);
        param_1->field_18 = param_1->field_24;
    }
}

struct Struct427a80 {
    unsigned char pad_0[0x18];
    unsigned int field_18;
    unsigned char pad_1c[0x24 - 0x1c];
    float field_24;
};

// FUNCTION: LEGOLAND 0x00427a80
void FUN_00427a80(struct Struct427a80 *param_1, float param_2) {
    param_1->field_24 = param_2;
    param_1->field_18 = *(unsigned int *)&param_2;
}

// FUNCTION: LEGOLAND 0x00427aa0
void FUN_00427aa0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427af0
void FUN_00427af0(void) {
    unsigned int handle;
    // STRING: LEGOLAND 0x004b5f34
    if (LLIDB_FindElement("BASIC TILES 1", &handle, 0) == 0) {
        LLIDB_UnLoadData(handle);
    }
}

// FUNCTION: LEGOLAND 0x00427b20
void FUN_00427b20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427bc0
void FUN_00427bc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427c00
void FUN_00427c00(void) { STUB(); }

struct Struct427c30 {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x20 - 0x1c];
    unsigned int field_20;
    unsigned int field_24;
};

// FUNCTION: LEGOLAND 0x00427c30
void FUN_00427c30(struct Struct427c30 *param_1) {
    if (param_1->field_20 == 0xffffffff) {
        param_1->field_20 = FUN_0041cc50(param_1->field_14);
        param_1->field_24 = param_1->field_18;
        return;
    }
    if (param_1->field_14 == 0xffffffff) {
        param_1->field_14 = FUN_0041cc50(param_1->field_20);
        param_1->field_18 = param_1->field_24;
    }
}

struct Struct427c70Inner {
    unsigned char pad_0[4];
    int field_4;
    int field_8;
};

struct Struct427c70 {
    unsigned char pad_0[0xc];
    struct Struct427c70Inner *field_c;
    unsigned char pad_10[0x18 - 0x10];
    float field_18;
    unsigned char pad_1c[0x24 - 0x1c];
    float field_24;
};

// FUNCTION: LEGOLAND 0x00427c70
void FUN_00427c70(struct Struct427c70 *param_1) {
    struct Struct427c70Inner *inner = param_1->field_c;
    param_1->field_24 = (float)inner->field_4;
    param_1->field_18 = (float)inner->field_8;
}

// FUNCTION: LEGOLAND 0x00427c90
void FUN_00427c90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427ea0
void FUN_00427ea0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427ef0
void FUN_00427ef0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427f30
void FUN_00427f30(void) { STUB(); }

struct Struct427f70Arg {
    unsigned char pad_0[4];
    short field_4;
    short field_6;
};

// FUNCTION: LEGOLAND 0x00427f70
void FUN_00427f70(const struct Struct427f70Arg *arg) {
    struct Struct427f70Tmp tmp;

    tmp.field_0 = arg->field_4;
    tmp.field_4 = arg->field_6;
    RemoveRollerCoasterPath(&tmp);

    tmp.field_0 = arg->field_4 + 1;
    tmp.field_4 = arg->field_6;
    RemoveRollerCoasterPath(&tmp);

    tmp.field_0 = arg->field_4 + 1;
    tmp.field_4 = arg->field_6 + 1;
    RemoveRollerCoasterPath(&tmp);

    tmp.field_0 = arg->field_4;
    tmp.field_4 = arg->field_6 + 1;
    RemoveRollerCoasterPath(&tmp);
}

struct Struct427ff0Point {
    unsigned short pad_0;
    unsigned short pad_2;
    short x;
    short y;
};

// FUNCTION: LEGOLAND 0x00427ff0
void FUN_00427ff0(struct Struct427ff0Point *point) {
    int coords[2];

    coords[0] = point->x;
    coords[1] = point->y;
    AddRollerCoasterPath(coords);

    coords[0] = point->x + 1;
    coords[1] = point->y;
    AddRollerCoasterPath(coords);

    coords[0] = point->x + 1;
    coords[1] = point->y + 1;
    AddRollerCoasterPath(coords);

    coords[0] = point->x;
    coords[1] = point->y + 1;
    AddRollerCoasterPath(coords);
}


// FUNCTION: LEGOLAND 0x00428070
void FUN_00428070(void *param_1) {
    unsigned int a = *(unsigned int *)((unsigned char *)param_1 + 0xc);
    unsigned int b = *(unsigned int *)((unsigned char *)a + 0x64);
    *(unsigned int *)((unsigned char *)b + 0x10) |= 0x2000;
    *(unsigned int *)((unsigned char *)&DAT_00611688 + DAT_00611958 * 8) = (unsigned int)param_1;
    *(unsigned int *)((unsigned char *)&DAT_0061168c + DAT_00611958 * 8) = (unsigned int)DAT_004b5dc8;
    DAT_00611958++;
}

// FUNCTION: LEGOLAND 0x004280b0
void FUN_004280b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428300
void FUN_00428300(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428350
void FUN_00428350(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004283c0
unsigned int FUN_004283c0(void *s) { STUB(); }

// FUNCTION: LEGOLAND 0x004284d0
void FUN_004284d0(void) { STUB(); }

struct Struct4286e0Element {
    unsigned int field_0;
    unsigned int field_4;
};

// FUNCTION: LEGOLAND 0x004286e0
struct Struct4286e0Element *FUN_004286e0(void *param1) {
    unsigned int result = FUN_004283c0(param1);
    unsigned int index = result + (result + result * 4) * 2;
    return &((struct Struct4286e0Element *)DAT_00828fe0)[index];
}

// FUNCTION: LEGOLAND 0x00428700
void FUN_00428700(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428750
void FUN_00428750(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428840
unsigned int FUN_00428840(unsigned int param_1) {
    unsigned int mask = 1;
    unsigned int result = 0;
    if ((param_1 & 1) == 0) {
        while ((param_1 & mask) == 0) {
            mask = mask * 2;
            result = result + 1;
        }
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00428860
void FUN_00428860(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428b70
void FUN_00428b70(void) {
    FUN_00428f00();
    FUN_00429270();
    FUN_004294b0();
}

// FUNCTION: LEGOLAND 0x00428b80
void FUN_00428b80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428cb0
void FUN_00428cb0(void *p, unsigned int a, unsigned int b, unsigned int c, const char *name) { STUB(); }

struct Struct428e70 {
    unsigned char pad_0[0x4c];
    unsigned int (**vtable)(void *self, const char *name);
};


// FUNCTION: LEGOLAND 0x00428e70
void FUN_00428e70(struct Struct428e70 *p, unsigned int a, unsigned int b) {
    unsigned short handle = FUN_004236f0();
    unsigned int result = p->vtable[6](p, DAT_00612178);
    DAT_00615f6c = result;
    FUN_00428cb0(p, a, b, result, DAT_00612178);
    FUN_004234e0(DAT_004b5f60);
    FUN_00423730(handle);
}

// FUNCTION: LEGOLAND 0x00428ec0
void FUN_00428ec0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428f00
void FUN_00428f00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429150
void FUN_00429150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429270
void FUN_00429270(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004292f0
void FUN_004292f0(void) { STUB(); }


// FUNCTION: LEGOLAND 0x00429490
void FUN_00429490(unsigned int param_1, unsigned int param_2) {
    FUN_00420e90((unsigned int)&DAT_004b6300, (unsigned int)&DAT_004b62f0, (void *)param_1, (void *)param_2, 1);
}

// FUNCTION: LEGOLAND 0x004294b0
void FUN_004294b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004294f0
void FUN_004294f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429560
void FUN_00429560(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429690
void FUN_00429690(void) { STUB(); }

struct Struct4296f0Node {
    unsigned char pad_0[0xc];
    unsigned int *field_c;
    unsigned char pad_10[0x14 - 0x10];
    unsigned int field_14;
    unsigned char pad_18[0x1c - 0x18];
    unsigned int field_1c;
    unsigned int field_20;
    unsigned char pad_24[0x28 - 0x24];
    struct Struct4296f0Node *field_28;
};

struct Struct4296f0Host {
    unsigned char pad_0[0x28];
    struct Struct4296f0Node *field_28;
};

// FUNCTION: LEGOLAND 0x004296f0
unsigned int FUN_004296f0(struct Struct4296f0Host *param_1, int *param_2) {
    struct Struct4296f0Node *node = param_1->field_28;
    *param_2 = 1;

    if (!FUN_00429910(node->field_c, node->field_14, node->field_20)) {
        return node->field_1c;
    }

    do {
        ++(*param_2);
        node = node->field_28;
    } while (FUN_00429910(node->field_c, node->field_14, node->field_20));

    return node->field_1c;
}

// FUNCTION: LEGOLAND 0x00429750
void FUN_00429750(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429840
void FUN_00429840(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004298a0
void FUN_004298a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429910
unsigned int FUN_00429910(unsigned int *s, unsigned int v, unsigned int c) {
    unsigned int flag = (*s == 0);
    flag &= 1;
    if (flag) {
        if (c == FUN_0041cc50(v)) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00429940
void FUN_00429940(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429990
void FUN_00429990(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004299e0
void FUN_004299e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429a30
void FUN_00429a30(void) { STUB(); }

struct Struct429a80B {
    float x0;
    float x4;
    float x8;
};

struct Struct429a80Elem {
    void (*func)(void *base, int arg, struct Struct429a80B *b);
    unsigned char pad_4[4];
};

struct Struct429a80VTable {
    unsigned char pad_0[0x4c];
    struct Struct429a80Elem *array;
};

struct Struct429a80A {
    unsigned char pad_0[4];
    struct Struct429a80VTable *vtable;
    float x8;
    float xc;
    float x10;
};

// FUNCTION: LEGOLAND 0x00429a80
void FUN_00429a80(struct Struct429a80A *a, int param0, int param2, struct Struct429a80B *b) {
    struct Struct429a80VTable *base = a->vtable;
    base->array[param0].func(base, param2, b);
    b->x0 += a->x8;
    b->x4 += a->xc;
    b->x8 += a->x10;
}

// FUNCTION: LEGOLAND 0x00429ac0
void FUN_00429ac0(int a, int b, int c, void *d) { STUB(); }

// FUNCTION: LEGOLAND 0x00429af0
void FUN_00429af0(int a, void *b) { STUB(); }

// FUNCTION: LEGOLAND 0x00429b60
void FUN_00429b60(int a, int b) {
    int local[3];
    FUN_00429ac0(a, 1, b, local);
    FUN_00429af0((int)&local[0], (void *)local[6]);
}

struct Struct429b90Inner;

struct Struct429b90Method {
    unsigned char pad_0[0x1c];
    unsigned int (*method_1c)(struct Struct429b90Inner *self, unsigned int arg2, unsigned int arg3);
};

struct Struct429b90Inner {
    unsigned char pad_0[0x4c];
    struct Struct429b90Method *field_4c;
};

struct Struct429b90Host {
    unsigned char pad_0[4];
    struct Struct429b90Inner *field_4;
};

// FUNCTION: LEGOLAND 0x00429b90
unsigned int FUN_00429b90(struct Struct429b90Host *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4) {
    struct Struct429b90Inner *inner = param_1->field_4;
    return inner->field_4c->method_1c(inner, param_3, param_4);
}

// FUNCTION: LEGOLAND 0x00429bb0
void FUN_00429bb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429c10
void FUN_00429c10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429c60
void FUN_00429c60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429cf0
void FUN_00429cf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429e20
void FUN_00429e20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429f30
void FUN_00429f30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a020
void FUN_0042a020(void) { STUB(); }

struct Struct42a110 {
    unsigned int field_0;
    unsigned int field_4;
};

// FUNCTION: LEGOLAND 0x0042a110
int FUN_0042a110(struct Struct42a110 *p1, struct Struct42a110 *p2) {
    if (p1->field_0 != p2->field_0) {
        return 0;
    }
    if (p1->field_4 != p2->field_4) {
        return 0;
    }
    return FUN_00425da0((const float *)(p1 + 1), (const float *)(p2 + 1)) != 0;
}

// FUNCTION: LEGOLAND 0x0042a150
void FUN_0042a150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a1b0
void FUN_0042a1b0(void) { STUB(); }


// FUNCTION: LEGOLAND 0x0042a2e0
void FUN_0042a2e0(void) {
    FUN_00421540(&DAT_00615f98, 3);
}

// FUNCTION: LEGOLAND 0x0042a2f0
void FUN_0042a2f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a5e0
void FUN_0042a5e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a620
void FUN_0042a620(unsigned int *param_1, unsigned int *param_2, unsigned int param_5) {
    memcpy(&param_1[1], param_2, 5 * sizeof(unsigned int));
    param_1[0] = param_5;
}

// FUNCTION: LEGOLAND 0x0042a640
unsigned int FUN_0042a640(void *ptr, unsigned int a, unsigned int b) { STUB(); }


// FUNCTION: LEGOLAND 0x0042a670
float FUN_0042a670(void) {
    return FLOAT_004ab390;
}

// FUNCTION: LEGOLAND 0x0042a680
void FUN_0042a680(void) { STUB(); }


// FUNCTION: LEGOLAND 0x0042a780
void FUN_0042a780(void) {
    // STRING: LEGOLAND 0x004b6414
    const char *str = "coastertrain.wheel01";
    DAT_00616000 = FUN_004206b0(str);
    DAT_00616004 = FUN_00420710(str);
}
