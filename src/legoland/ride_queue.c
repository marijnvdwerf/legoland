#include "legoland.h"
#include "crt.h"

#include "ride_queue.h"
#include "globals.h"

struct QueueItemInner {
    unsigned char pad_0[0xe];
    unsigned short field_e;
    unsigned char pad_10[0x38 - 0x10];
    short field_38;
};

struct QueueItemMid {
    unsigned char pad_0[0x8];
    struct QueueItemInner *field_8;
};

struct QueueNode {
    struct QueueNode *next;
    struct QueueItemMid *field_4;
};

struct Queue {
    unsigned int *count;
    struct QueueNode *head;
    struct QueueNode *tail;
};

struct LegoConfig {
    unsigned char pad_0[0x14];
    short width;
    short height;
};

struct RideQueueEntry {
    struct RideQueueEntry *next;
    unsigned char pad_4[0x4];
    unsigned short field_8;
    unsigned char pad_a[0x2];
    int x;
    int y;
    unsigned char field_14;
};

struct RideSlotArg {
    unsigned short field_0;
};

struct RideSlot {
    unsigned char pad_0[0x34];
    unsigned char field_34;
    unsigned char pad_35[0x3];
    unsigned short field_38;
    unsigned char pad_3a[0x16];
    struct RideSlotArg *field_50;
    unsigned char pad_54[0xc];
    unsigned char field_60;
};


// FUNCTION: LEGOLAND 0x00411e30
void FUN_00411e30(struct Queue *queue, struct QueueNode *node) {
    if (queue->head == NULL && queue->tail == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
}

// FUNCTION: LEGOLAND 0x00411e60
unsigned int FUN_00411e60(struct Queue *queue) {
    unsigned int count;
    struct QueueNode *node;

    if (queue->head != NULL) {
        count = 0;
        node = queue->head;
        while (node != NULL) {
            count++;
            node = node->next;
        }
        if (count == *queue->count) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00411e90
unsigned int FUN_00411e90(struct Queue *queue) {
    return queue->head != NULL;
}

// FUNCTION: LEGOLAND 0x00411ea0
int FUN_00411ea0(struct Queue *queue) {
    struct QueueNode *head = queue->head;
    if (head != NULL) {
        short value = head->field_4->field_8->field_38;
        if (value == (int)(*queue->count - 1)) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00411ed0
void FUN_00411ed0(struct Queue *queue) {
    struct QueueNode *node = queue->head;
    while (node != NULL) {
        FUN_00411f00(queue);
        FUN_0049e4d0(node);
        node = queue->head;
    }
}

// FUNCTION: LEGOLAND 0x00411f00
void FUN_00411f00(struct Queue *queue) {
    struct QueueNode *head = queue->head;
    if (head != NULL) {
        queue->head = head->next;
        if (queue->tail == head) {
            queue->tail = NULL;
        }
    }
}

// FUNCTION: LEGOLAND 0x00411f20
void FUN_00411f20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00411fa0
void FUN_00411fa0(struct Queue *queue, unsigned int param_2, unsigned int param_3, struct QueueItemInner *param_4) { STUB(); }

// FUNCTION: LEGOLAND 0x00412060
void FUN_00412060(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004120a0
void FUN_004120a0(struct Queue *queue, unsigned int param_2, unsigned int param_3) {
    struct QueueNode *node = queue->head;
    while (node != NULL) {
        struct QueueItemInner *inner = node->field_4->field_8;
        if (inner->field_e == 0) {
            FUN_00411fa0(queue, param_2, param_3, inner);
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00412100
void FUN_00412100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00412290
void FUN_00412290(void *param_1) {
    if (param_1 != NULL) {
        FUN_0049e4d0(param_1);
    }
}

// FUNCTION: LEGOLAND 0x004122a0
void FUN_004122a0(struct RideSlotArg *param_1, struct RideSlot *slot) {
    slot->field_50 = param_1;
    slot->field_38 = param_1->field_0 - 1;
    slot->field_34 = 0xff;
    slot->field_60++;
}

// FUNCTION: LEGOLAND 0x004122d0
void FUN_004122d0(struct RideSlotArg *param_1, struct RideSlot *slot) {
    slot->field_50 = param_1;
    slot->field_38 = 0;
    slot->field_34 = 1;
    slot->field_60++;
}

// FUNCTION: LEGOLAND 0x004122f0
struct RideSlotArg *FUN_004122f0(struct RideSlot *slot) {
    return slot->field_50;
}

// FUNCTION: LEGOLAND 0x00412300
void FUN_00412300(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004123a0
unsigned int FUN_004123a0(struct QueueNode *start, struct QueueNode *stop) {
    unsigned int count = 0;
    struct QueueNode *current = start;
    if (current != NULL) {
        while (1) {
            if (current == stop) {
                break;
            }
            current = current->next;
            count++;
            if (current == NULL) {
                break;
            }
        }
    }
    return count;
}

// FUNCTION: LEGOLAND 0x004123c0
void FUN_004123c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00412470
void FUN_00412470(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00412490
void FUN_00412490(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004125a0
struct RideQueueEntry *FUN_004125a0(int x, int y) {
    struct RideQueueEntry *entry = DAT_004cbeac;
    if (x < 0) {
        return NULL;
    }
    if (x >= (unsigned short)lpConfig->width) {
        return NULL;
    }
    if (y < 0) {
        return NULL;
    }
    if (y >= (unsigned short)lpConfig->height) {
        return NULL;
    }
    if (entry == NULL) {
        return NULL;
    }
    while (entry != NULL) {
        if (entry->x == x && entry->y == y) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x004125f0
void *FUN_004125f0(unsigned int a, unsigned int b) { STUB(); }

// FUNCTION: LEGOLAND 0x00412650
struct RideQueueEntry *FUN_00412650(unsigned short param_1) {
    struct RideQueueEntry *entry = DAT_004cbeac;
    if (entry == NULL) {
        return NULL;
    }
    while (entry != NULL) {
        if (entry->field_8 == param_1 && (entry->field_14 & 0xf) == 6) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00412680
void FUN_00412680(void) { STUB(); }
