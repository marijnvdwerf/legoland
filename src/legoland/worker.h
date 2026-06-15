#pragma once

#include "legoland.h"

struct WorkOrder;
struct EditObject;
struct Person;

struct Worker {
    /* 0x00 */ struct Worker *next;
    /* 0x04 */ struct Person *field_4;
    /* 0x08 */ unsigned char field_8;
    /* 0x09 */ unsigned char pad_9[1];
    /* 0x0a */ unsigned short field_a;
    /* 0x0c */ unsigned short flags_c;
    /* 0x0e */ unsigned short state;
    /* 0x10 */ unsigned char field_10;
    /* 0x11 */ unsigned char counter_11;
    /* 0x12 */ unsigned char pad_12[10];
    /* 0x1c */ unsigned int flags_1c;
    /* 0x20 */ unsigned int field_20;
    /* 0x24 */ int var_24;
    /* 0x28 */ int var_28;
    /* 0x2c */ int var_2c;
    /* 0x30 */ int var_30;
    /* 0x34 */ unsigned char pad_34[0x36 - 0x34];
    /* 0x36 */ unsigned char progress;
    /* 0x37 */ unsigned char pad_37[0x46 - 0x37];
    /* 0x46 */ unsigned short var_46;
    /* 0x48 */ unsigned char pad_48[0x50 - 0x48];
    /* 0x50 */ struct WorkOrder *var_50;
    /* 0x54 */ unsigned char pad_54[0x5c - 0x54];
    /* 0x5c */ unsigned int ticks;
    /* 0x60 */ unsigned char var_60;
    /* 0x61 */ unsigned char pad_61[1];
    /* 0x62 */ unsigned short flags;
    /* 0x64 */ unsigned char var_64;
    /* 0x65 */ unsigned char pad_65[0x68 - 0x65];
    /* 0x68 */ int var_68;
    /* 0x6c */ int var_6c;
    /* 0x70 */ unsigned short var_70;
    /* 0x72 */ unsigned char var_72;
    /* 0x73 */ unsigned char var_73;
    /* 0x74 */ unsigned char var_74;
    /* 0x75 */ unsigned char var_75;
    /* 0x76 */ unsigned char pad_76[0x7f - 0x76];
    /* 0x7f */ unsigned char var_7f;
    /* 0x80 */ unsigned char pad_80[0x82 - 0x80];
    /* 0x82 */ unsigned char var_82;
    /* 0x83 */ unsigned char pad_83[0x98 - 0x83];
    /* 0x98 */ int var_98;
};

LEGO_EXPORT void GenerateGardener(void *object, int param_2);
LEGO_EXPORT void GenerateMechanic(void *object, int param_2);
LEGO_EXPORT void RemoveAGardener(struct Worker *worker);
LEGO_EXPORT void RemoveAMechanic(struct Worker *worker);
LEGO_EXPORT int IterateNoneWorkersRepairOrders(void);
LEGO_EXPORT void RemoveGardenersWorkOrderAt(unsigned int x, unsigned int y);
LEGO_EXPORT void RemoveMechanicsWorkOrderAt(unsigned int x, unsigned int y);
LEGO_EXPORT void RemoveNoneWorkersRepairOrderAT(unsigned int x, unsigned int y);
LEGO_EXPORT void RemoveRepairOrderAT(struct Worker *worker, unsigned int x, unsigned int y);
/* FUN_0049b270 is still a STUB with an unverified signature; declared K&R. */
void FUN_0049b270();
void FUN_00499eb0(void *order);
int FUN_0049a120(void);
int FUN_0049a160(void);
void FUN_0049cfc0(void);
