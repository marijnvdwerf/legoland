#pragma once

#include "legoland.h"

struct CastleObj;
struct IconNode;

struct InterfaceListNode {
    /* 0x00 */ struct InterfaceListNode *next;
    /* 0x04 */ void *data;
    /* 0x08 */ int flag;
};

struct TrackElemPair {
    /* 0x00 */ char *track_name;
    /* 0x04 */ char *elem_name;
};

struct InterfaceResearchNode {
    /* 0x00 */ struct InterfaceResearchNode *next;
    /* 0x04 */ void *data;
    /* 0x08 */ int field_8;
};

struct InterfacePanel {
    /* 0x00 */ short group;
    /* 0x02 */ unsigned char pad_2[0x04 - 0x02];
    /* 0x04 */ int field_4;
    /* 0x08 */ struct IconNode *icon;
    /* 0x0c */ int field_c;
    /* 0x10 */ int field_10;
    /* 0x14 */ int field_14;
    /* 0x18 */ int field_18;
    /* 0x1c */ int field_1c;
    /* 0x20 */ int field_20;
    /* 0x24 */ int field_24;
    /* 0x28 */ int field_28;
};

void FUN_00474880(void);
void FUN_004748a0(void *a);
int FUN_00474920(void);
int FUN_00474970(void);
LEGO_EXPORT int InitGameInterface(int a);
void FUN_00474ed0(void);
unsigned char FUN_00474f40(void *context, unsigned int flags, const char *a, const char *b);
unsigned char FUN_00474fa0(unsigned int a, unsigned char flags);
unsigned char FUN_00474fc0(void *a, unsigned int flags);
unsigned char FUN_00474f80(unsigned int a, unsigned int flags);
unsigned char FUN_00475000(unsigned int a, unsigned int flags);
unsigned char FUN_00475040(unsigned int a, unsigned int flags);
unsigned char FUN_004751a0(struct IconNode *param_1, unsigned char flags);
unsigned char FUN_004752a0(struct IconNode *param_1, unsigned char flags);
unsigned char FUN_004753a0(struct IconNode *param_1, unsigned char flags);
unsigned char FUN_004754b0(struct IconNode *param_1, unsigned char flags);
unsigned char FUN_00475080(unsigned int a, unsigned char flags);
unsigned char FUN_00475120(unsigned int a, unsigned int flags, unsigned int c, unsigned int d);
LEGO_EXPORT void DelObjectList(void);
LEGO_EXPORT int MakeUpObjectList(int param_1, int param_2, int param_3, int param_4);
LEGO_EXPORT unsigned int ObjectLinkedList(unsigned int *entry);
void FUN_00476000(void);
void FUN_00476050(void);
void FUN_004760a0(void);
void FUN_00476070(int mask, unsigned int value);
void FUN_00476140(int index, int value);
void FUN_00476180(void);
void FUN_004771f0(const char *filename, unsigned int param_2, unsigned int param_3);
/* Both are STUBs with unverified signatures, called only from castle.c — the call
 * sites pass 4 args to FUN_004775b0 (and that 4-push form is what matches the
 * original at 100%), and FUN_004775d0 is passed mixed pointer/int values. Declare
 * with unspecified params (K&R) so callers don't get a forced-prototype mismatch
 * (C4020 / int-conversion) and codegen is preserved. Set real signatures when
 * these get decompiled. */
void *FUN_004775b0();
void FUN_004775d0();
LEGO_EXPORT void DisableSidePanelIcons(void);
LEGO_EXPORT void EnableSidePanelIcons(void);
LEGO_EXPORT void ListChildrenBar(void *node, int group, short x, short y);
LEGO_EXPORT void CloseChildrenBar(void *node, int group, short x, short y);
LEGO_EXPORT void RedrawObjectList(struct InterfacePanel *panel, int param_2, int delta);
void FUN_00474750(void);
LEGO_EXPORT unsigned int TestMenu(unsigned int *entry);
char FUN_00475c50(int param_1, unsigned char param_2);
char FUN_00475c90(int param_1, unsigned char param_2);
