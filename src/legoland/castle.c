#include <windows.h>
#include "legoland.h"
#include <math.h>

extern unsigned int DAT_004d8268;
extern unsigned int DAT_00829ae0;
extern unsigned int DAT_00829b88;
extern unsigned int DAT_00829ba0;
extern float DAT_004ab3d0;
extern float DAT_004ab430;
extern unsigned int DAT_0082adec;
extern unsigned int DAT_004d8270;
extern const unsigned char DAT_004d8250[];

struct Indexed;
struct LookupResult;
struct CastleObj;

unsigned int FUN_0041ee40(unsigned int a, unsigned int b);
void FUN_004273e0(void *obj);
unsigned int FUN_0042a640(void *ptr, unsigned int a, unsigned int b);
unsigned int FUN_0041d1d0(struct Indexed *obj, unsigned int param2, unsigned int param3);
float FUN_0041dca0(float param);
void FUN_0041dd00(float param, float result);
float FUN_0041dd70(float param);
void FUN_00420410(void *param, unsigned int count);
void FUN_0041ddd0(void);
void FUN_0041de10(void);
struct ObjAtC8;
void FUN_0041e950(struct ObjAtC8 *obj);
void FUN_004266b0(unsigned int *param);
void FUN_0041e970(void);
void FUN_0041e990(void);
unsigned int FUN_0041eaf0(unsigned int a, unsigned int b);
void FUN_0041e500(void *obj);
void FUN_0041e6a0(unsigned int *param, unsigned int value);
void FUN_0041e420(struct CastleObj *self, unsigned int flag);
extern struct CastleObj *FUN_004775b0(unsigned int size, unsigned int a, unsigned int b, unsigned int c);
void FUN_00421540(unsigned int *param, unsigned int value);
struct LookupResult *FUN_0041d3b0(const unsigned char *key, unsigned int param2);
unsigned int FUN_0041d630(unsigned int param0, const unsigned char *key, unsigned int param2, const unsigned char *param3);
unsigned int FUN_0041d5b0(unsigned int param0, const unsigned char *key, unsigned int param2, const unsigned char *param3);
unsigned int FUN_004206b0(const char *s);
unsigned int FUN_00420710(const char *s);
unsigned int FUN_00422590(const char *s);
unsigned int FUN_00422600(unsigned int param);
extern void AddBasicObject(unsigned int param1, unsigned int param2);
extern void StandardRemoveObject(unsigned int param1, unsigned int param2, unsigned int param3);
extern void FUN_004775d0(unsigned int param);
extern unsigned int DAT_004d8a40[];
extern unsigned int DAT_004d8abc[];
extern unsigned int DAT_004d8b34[];
extern unsigned int DAT_004d89c8[];
extern unsigned int DAT_004d8bac;
extern unsigned int DAT_004b55f4;
extern unsigned int DAT_004b5608;
extern unsigned int DAT_004b560c;
extern unsigned int DAT_0082add0;
extern unsigned int DAT_0082add4;
extern unsigned int DAT_0082add8;
extern unsigned int DAT_0082ade0;
extern unsigned int DAT_0082ade4;
extern unsigned int DAT_0082ade8;
extern unsigned char DAT_004d88f4[];
extern unsigned char DAT_004d8934[];
extern unsigned char DAT_004d8974[];
extern void *DAT_004d89c4;

struct FlagWord {
    unsigned int field_0;
};

void FUN_0041e630(struct FlagWord *obj);

extern int GetGameTimer(void);

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
void FUN_0041d7f0(void) { STUB(); }

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
void FUN_0041e000(void) { STUB(); }

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
void FUN_0041e2b0(void) { STUB(); }

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
void FUN_0041e4a0(void) { STUB(); }

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
void FUN_0041e5d0(void) { STUB(); }

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
    FUN_004266b0(&obj->field_c8);
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
    // STRING: LEGOLAND 0x004b55d8
    DAT_0082ade0 = FUN_00420710("coastertrain.headcar");
    // STRING: LEGOLAND 0x004b55c4
    DAT_0082ade4 = FUN_00420710("coastertrain.midcar");
    // STRING: LEGOLAND 0x004b55ac
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

extern struct DispatchRow DAT_0082ad20[];
extern struct DispatchRow DAT_0082adb0[];

// FUNCTION: LEGOLAND 0x0041ebd0
unsigned int FUN_0041ebd0(unsigned int arg) {
    unsigned int index = 0;
    struct DispatchRow *row = DAT_0082ad20;
    while ((int)row < (int)DAT_0082adb0) {
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
    struct DispatchTarget *target = (struct DispatchTarget *)DAT_0082ad20[param].field_0;
    return target->field_c;
}

// FUNCTION: LEGOLAND 0x0041ec20
unsigned int FUN_0041ec20(unsigned int param) {
    unsigned int index = 0;
    struct DispatchRow *row = DAT_0082ad20;
    while ((int)row < (int)DAT_0082adb0) {
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
    return DAT_0082ad20[param].field_0;
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

struct Cursor {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
};

// FUNCTION: LEGOLAND 0x0041f850
unsigned int FUN_0041f850(struct Cursor *cursor) {
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
    DAT_004d89c4 = DAT_004d8934;
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
void FUN_00420e90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00420fb0
void FUN_00420fb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00420fd0
void FUN_00420fd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004212a0
void FUN_004212a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004212e0
void FUN_004212e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421320
void FUN_00421320(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421340
void FUN_00421340(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421360
void FUN_00421360(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004213a0
void FUN_004213a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421400
void FUN_00421400(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421430
void FUN_00421430(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421470
void FUN_00421470(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004214f0
void FUN_004214f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421510
void FUN_00421510(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421540
void FUN_00421540(unsigned int *param, unsigned int value) { STUB(); }

// FUNCTION: LEGOLAND 0x00421560
void FUN_00421560(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421590
void FUN_00421590(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004215b0
void FUN_004215b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004215d0
void FUN_004215d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421660
void FUN_00421660(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421890
void FUN_00421890(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421930
void FUN_00421930(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421980
void FUN_00421980(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004219c0
void FUN_004219c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004219f0
void FUN_004219f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421a10
void FUN_00421a10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421a40
void FUN_00421a40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421a70
void FUN_00421a70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421a90
void FUN_00421a90(void) { STUB(); }

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

// FUNCTION: LEGOLAND 0x00421c80
void FUN_00421c80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421cc0
void FUN_00421cc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421ce0
void FUN_00421ce0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421d60
void FUN_00421d60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421da0
void FUN_00421da0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421df0
void FUN_00421df0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421e40
void FUN_00421e40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00421e90
void FUN_00421e90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422000
void FUN_00422000(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004220e0
void FUN_004220e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422180
void FUN_00422180(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422210
void FUN_00422210(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004222f0
void FUN_004222f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422300
void FUN_00422300(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422340
void FUN_00422340(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422390
void FUN_00422390(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004223c0
void FUN_004223c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422400
void FUN_00422400(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422470
void FUN_00422470(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422590
unsigned int FUN_00422590(const char *s) { STUB(); }

// FUNCTION: LEGOLAND 0x004225b0
void FUN_004225b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004225d0
void FUN_004225d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422600
unsigned int FUN_00422600(unsigned int param) { STUB(); }

// FUNCTION: LEGOLAND 0x00422620
void FUN_00422620(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00422640
void FUN_00422640(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004226c0
void FUN_004226c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004227a0
void FUN_004227a0(void) { STUB(); }

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
void FUN_004234e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004236f0
void FUN_004236f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423730
void FUN_00423730(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423740
void FUN_00423740(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423760
void FUN_00423760(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423790
void FUN_00423790(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004237a0
void FUN_004237a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004237f0
void FUN_004237f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004238a0
void FUN_004238a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423930
void FUN_00423930(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423940
void FUN_00423940(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423970
void FUN_00423970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423990
void FUN_00423990(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004239a0
void FUN_004239a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004239b0
void FUN_004239b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004239e0
void FUN_004239e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423a10
void FUN_00423a10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423d40
void FUN_00423d40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423db0
void FUN_00423db0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00423de0
void FUN_00423de0(void) { STUB(); }

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
void FUN_004241e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424240
void FUN_00424240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424280
void FUN_00424280(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424320
void FUN_00424320(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424440
void FUN_00424440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004244b0
void FUN_004244b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004245b0
void FUN_004245b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424620
void FUN_00424620(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004246e0
void FUN_004246e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424700
void FUN_00424700(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424800
void FUN_00424800(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424820
void FUN_00424820(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424830
void FUN_00424830(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424850
void FUN_00424850(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424890
void FUN_00424890(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004248b0
void FUN_004248b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424960
void FUN_00424960(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424990
void FUN_00424990(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004249e0
void FUN_004249e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424a00
void FUN_00424a00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424a20
void FUN_00424a20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424a50
void FUN_00424a50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424ab0
void FUN_00424ab0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424ae0
void FUN_00424ae0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424b10
void FUN_00424b10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424b30
void FUN_00424b30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424b60
void FUN_00424b60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424b90
void FUN_00424b90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424bc0
void FUN_00424bc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424c10
void FUN_00424c10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424c40
void FUN_00424c40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424c70
void FUN_00424c70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424d80
void FUN_00424d80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424dc0
void FUN_00424dc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424df0
void FUN_00424df0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424e20
void FUN_00424e20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424e60
void FUN_00424e60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00424e70
void FUN_00424e70(void) { STUB(); }

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
void FUN_00425c40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425cb0
void FUN_00425cb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425cf0
void FUN_00425cf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425d30
void FUN_00425d30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425d50
void FUN_00425d50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425da0
void FUN_00425da0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425de0
void FUN_00425de0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00425e20
void FUN_00425e20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426000
void FUN_00426000(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004260e0
void FUN_004260e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004260f0
void FUN_004260f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426120
void FUN_00426120(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426190
void FUN_00426190(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004261c0
void FUN_004261c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426230
void FUN_00426230(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426250
void FUN_00426250(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004263a0
void FUN_004263a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426460
void FUN_00426460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426490
void FUN_00426490(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004264e0
void FUN_004264e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426510
void FUN_00426510(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426560
void FUN_00426560(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004265d0
void FUN_004265d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426650
void FUN_00426650(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004266b0
void FUN_004266b0(unsigned int *param) { STUB(); }

// FUNCTION: LEGOLAND 0x004266e0
void FUN_004266e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426700
void FUN_00426700(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426740
void FUN_00426740(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426750
void FUN_00426750(void) { STUB(); }

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
void FUN_00426ba0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426bc0
void FUN_00426bc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426be0
void FUN_00426be0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426c20
void FUN_00426c20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426ce0
void FUN_00426ce0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426d80
void FUN_00426d80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426de0
void FUN_00426de0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426e80
void FUN_00426e80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426ea0
void FUN_00426ea0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426ec0
void FUN_00426ec0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426f10
void FUN_00426f10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426f40
void FUN_00426f40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426f90
void FUN_00426f90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00426ff0
void FUN_00426ff0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427020
void FUN_00427020(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427050
void FUN_00427050(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427070
void FUN_00427070(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004270c0
void FUN_004270c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427100
void FUN_00427100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427130
void FUN_00427130(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427150
void FUN_00427150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427190
void FUN_00427190(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427220
void FUN_00427220(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427240
void FUN_00427240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004272a0
void FUN_004272a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004273c0
void FUN_004273c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004273d0
void FUN_004273d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004273e0
void FUN_004273e0(void *obj) { STUB(); }

// FUNCTION: LEGOLAND 0x004273f0
void FUN_004273f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427410
void FUN_00427410(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004274b0
void FUN_004274b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004274f0
void FUN_004274f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004275b0
void FUN_004275b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004275c0
void FUN_004275c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004275d0
void FUN_004275d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427940
void FUN_00427940(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427970
void FUN_00427970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004279f0
void FUN_004279f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427a00
void FUN_00427a00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427a40
void FUN_00427a40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427a80
void FUN_00427a80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427aa0
void FUN_00427aa0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427af0
void FUN_00427af0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427b20
void FUN_00427b20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427bc0
void FUN_00427bc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427c00
void FUN_00427c00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427c30
void FUN_00427c30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427c70
void FUN_00427c70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427c90
void FUN_00427c90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427ea0
void FUN_00427ea0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427ef0
void FUN_00427ef0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427f30
void FUN_00427f30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427f70
void FUN_00427f70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00427ff0
void FUN_00427ff0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428070
void FUN_00428070(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004280b0
void FUN_004280b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428300
void FUN_00428300(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428350
void FUN_00428350(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004283c0
void FUN_004283c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004284d0
void FUN_004284d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004286e0
void FUN_004286e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428700
void FUN_00428700(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428750
void FUN_00428750(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428840
void FUN_00428840(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428860
void FUN_00428860(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428b70
void FUN_00428b70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428b80
void FUN_00428b80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428cb0
void FUN_00428cb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00428e70
void FUN_00428e70(void) { STUB(); }

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
void FUN_00429490(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004294b0
void FUN_004294b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004294f0
void FUN_004294f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429560
void FUN_00429560(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429690
void FUN_00429690(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004296f0
void FUN_004296f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429750
void FUN_00429750(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429840
void FUN_00429840(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004298a0
void FUN_004298a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429910
void FUN_00429910(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429940
void FUN_00429940(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429990
void FUN_00429990(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004299e0
void FUN_004299e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429a30
void FUN_00429a30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429a80
void FUN_00429a80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429ac0
void FUN_00429ac0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429af0
void FUN_00429af0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429b60
void FUN_00429b60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00429b90
void FUN_00429b90(void) { STUB(); }

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

// FUNCTION: LEGOLAND 0x0042a110
void FUN_0042a110(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a150
void FUN_0042a150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a1b0
void FUN_0042a1b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a2e0
void FUN_0042a2e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a2f0
void FUN_0042a2f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a5e0
void FUN_0042a5e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a620
void FUN_0042a620(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a640
unsigned int FUN_0042a640(void *ptr, unsigned int a, unsigned int b) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a670
void FUN_0042a670(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a680
void FUN_0042a680(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a780
void FUN_0042a780(void) { STUB(); }
