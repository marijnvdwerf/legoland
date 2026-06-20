#pragma once

struct Queue;

struct PathPair {
    /* 0x00 */ int a;
    /* 0x04 */ int b;
};

struct PathTable {
    /* 0x00 */ int count;
    /* 0x04 */ struct PathPair *pairs;
};

int FUN_00412100(struct PathTable *param_1);

struct RideQueueEntry {
    /* 0x00 */ struct RideQueueEntry *next;
    /* 0x04 */ struct RideQueueEntry *field_4;
    /* 0x08 */ unsigned short field_8;
    /* 0x0a */ unsigned char pad_a[0x2];
    /* 0x0c */ int x;
    /* 0x10 */ int y;
    /* 0x14 */ unsigned char field_14;
    /* 0x15 */ unsigned char field_15;
    /* 0x16 */ unsigned char pad_16[0x2];
    /* 0x18 */ struct RideQueueEntry *field_18;
};

void FUN_00411f00(struct Queue *queue);
void FUN_00412290(void *param_1);
void *FUN_004125f0(unsigned int a, unsigned int b);
struct RideQueueEntry *FUN_004125a0(int x, int y);
struct RideQueueEntry *FUN_00412650(unsigned short param_1);
void FUN_00412680(int x, int y, int param_3, int param_4);
