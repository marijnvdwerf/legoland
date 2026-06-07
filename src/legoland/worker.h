#pragma once

#include "legoland.h"

struct Worker {
    struct Worker *next;
    unsigned char pad_4[10];
    unsigned short state;
    unsigned char pad_10[38];
    unsigned char progress;
    unsigned char pad_37[15];
    unsigned short var_46;
    unsigned char pad_48[20];
    unsigned long ticks;
    unsigned char pad_60[2];
    unsigned char flags;
};

LEGO_EXPORT void GenerateGardener(void *object, int param_2);
LEGO_EXPORT void GenerateMechanic(void *object, int param_2);
LEGO_EXPORT void RemoveAGardener(struct Worker *worker);
LEGO_EXPORT void RemoveAMechanic(struct Worker *worker);
LEGO_EXPORT int IterateNoneWorkersRepairOrders(void);
LEGO_EXPORT void RemoveGardenersWorkOrderAt(unsigned int x, unsigned int y);
LEGO_EXPORT void RemoveMechanicsWorkOrderAt(unsigned int x, unsigned int y);
LEGO_EXPORT void RemoveNoneWorkersRepairOrderAT(unsigned int x, unsigned int y);
void FUN_0049cfc0(void);
