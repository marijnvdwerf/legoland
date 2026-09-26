#include "event_list.h"
#include "globals.h"

struct EventNode {
    struct EventNode *next;
    unsigned char pad_4[0x1c - 0x4];
    int sort_key;
};

struct QueryNode {
    struct QueryNode *next;
    unsigned char pad_4[0x8 - 0x4];
    unsigned int field_8;
    unsigned int field_c;
};

// FUNCTION: LEGOLAND 0x00477680
int FUN_00477680(int a, int b) {
    unsigned short limit;

    if (a < 0) {
        return 0;
    }
    if (b < 0) {
        return 0;
    }
    if (a >= lpConfig->width) {
        return 0;
    }
    limit = lpConfig->height;
    return b < limit;
}

// FUNCTION: LEGOLAND 0x004776c0
void FUN_004776c0(struct QueryNode *node) {
    node->next = (struct QueryNode *)DAT_00668fc4;
    DAT_00668fc4 = (struct InterfaceQueryNode *)node;
}

// FUNCTION: LEGOLAND 0x004776e0
void FUN_004776e0(struct EventNode *node) {
    struct EventNode *current;
    struct EventNode *previous;

    if (DAT_00668fc0 == NULL) {
        DAT_00668fc0 = node;
        node->next = NULL;
    } else {
        current = DAT_00668fc0;
        previous = NULL;
        while (current != NULL) {
            if (current->sort_key >= node->sort_key) {
                break;
            }
            previous = current;
            current = current->next;
        }
        if (previous != NULL) {
            node->next = previous->next;
            previous->next = node;
        } else {
            node->next = DAT_00668fc0;
            DAT_00668fc0 = node;
        }
    }
}

// FUNCTION: LEGOLAND 0x00477730
struct QueryNode *FUN_00477730(struct QueryNode *ctx) {
    struct QueryNode *node;

    node = (struct QueryNode *)DAT_00668fc4;
    if (node == NULL) {
        return NULL;
    }
    while (node != NULL) {
        if (node->field_8 == *(unsigned int *)ctx && node->field_c == *((unsigned int *)ctx + 1)) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00477760
void FUN_00477760(struct QueryNode *ctx) {
    struct QueryNode *prev;
    struct QueryNode *node;

    prev = NULL;
    node = (struct QueryNode *)DAT_00668fc4;
    while (node != NULL) {
        if (node == ctx) {
            break;
        }
        prev = node;
        node = node->next;
    }
    if (prev != NULL) {
        prev->next = node->next;
    } else {
        DAT_00668fc4 = (struct InterfaceQueryNode *)node->next;
    }
}

// FUNCTION: LEGOLAND 0x00477790
void FUN_00477790(struct EventNode *param_1) {
    struct EventNode *prev;
    struct EventNode *node;

    prev = NULL;
    node = DAT_00668fc0;
    while (node != NULL) {
        if (node == param_1) {
            break;
        }
        prev = node;
        node = node->next;
    }
    if (prev != NULL) {
        prev->next = node->next;
        return;
    }
    DAT_00668fc0 = node->next;
}
