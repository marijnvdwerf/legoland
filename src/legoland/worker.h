#pragma once

#include "legoland.h"

struct Worker {
    /* 0x00 */ struct Worker *next;
    /* 0x04 */ unsigned char pad_4[8];
    /* 0x0c */ unsigned short flags_c;
    /* 0x0e */ unsigned short state;
    /* 0x10 */ unsigned char pad_10[1];
    /* 0x11 */ unsigned char counter_11;
    /* 0x12 */ unsigned char pad_12[10];
    /* 0x1c */ unsigned int flags_1c;
    /* 0x20 */ unsigned char pad_20[22];
    /* 0x36 */ unsigned char progress;
    /* 0x37 */ unsigned char pad_37[15];
    /* 0x46 */ unsigned short var_46;
    /* 0x48 */ unsigned char pad_48[20];
    /* 0x5c */ unsigned long ticks;
    /* 0x60 */ unsigned char pad_60[2];
    /* 0x62 */ unsigned char flags;
};

LEGO_EXPORT void GenerateGardener(void *object, int param_2);
LEGO_EXPORT void GenerateMechanic(void *object, int param_2);
LEGO_EXPORT void RemoveAGardener(struct Worker *worker);
LEGO_EXPORT void RemoveAMechanic(struct Worker *worker);
LEGO_EXPORT int IterateNoneWorkersRepairOrders(void);
LEGO_EXPORT void RemoveGardenersWorkOrderAt(unsigned int x, unsigned int y);
LEGO_EXPORT void RemoveMechanicsWorkOrderAt(unsigned int x, unsigned int y);
LEGO_EXPORT void RemoveNoneWorkersRepairOrderAT(unsigned int x, unsigned int y);
void FUN_00499eb0(void *order);
int FUN_0049a120(void);
int FUN_0049a160(void);
void FUN_0049cfc0(void);
